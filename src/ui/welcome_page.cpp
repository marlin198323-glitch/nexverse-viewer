/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/ui/welcome_page.hpp"

#if defined(__linux__)

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include <algorithm>
#include <cstring>
#include <utility>

namespace nexverse::ui
{
struct WelcomePage::Impl
{
    GtkWidget* window = nullptr;
    WebKitWebView* web_view = nullptr;
    WelcomeFrame frame;
    int width = 1;
    int height = 1;
    bool loaded = false;
    bool snapshot_pending = false;

    void request_snapshot()
    {
        if (!loaded || snapshot_pending || web_view == nullptr) return;
        snapshot_pending = true;

        webkit_web_view_get_snapshot(
            web_view,
            WEBKIT_SNAPSHOT_REGION_VISIBLE,
            WEBKIT_SNAPSHOT_OPTIONS_NONE,
            nullptr,
            [](GObject* source, GAsyncResult* result, gpointer user_data)
            {
                auto* self = static_cast<Impl*>(user_data);
                self->snapshot_pending = false;

                GError* error = nullptr;
                cairo_surface_t* surface = webkit_web_view_get_snapshot_finish(
                    WEBKIT_WEB_VIEW(source),
                    result,
                    &error);

                if (surface == nullptr)
                {
                    self->frame.failed = true;
                    self->frame.status =
                        error != nullptr && error->message != nullptr
                            ? error->message
                            : "Welcome snapshot failed";
                    if (error != nullptr) g_error_free(error);
                    return;
                }

                cairo_rectangle_int_t extents{
                    0, 0,
                    std::max(self->width, 1),
                    std::max(self->height, 1)};
                cairo_surface_t* image = cairo_surface_map_to_image(surface, &extents);
                if (image == nullptr)
                {
                    cairo_surface_destroy(surface);
                    self->frame.failed = true;
                    self->frame.status = "Welcome snapshot could not be mapped";
                    return;
                }

                cairo_surface_flush(image);
                const int image_width = cairo_image_surface_get_width(image);
                const int image_height = cairo_image_surface_get_height(image);
                const int stride = cairo_image_surface_get_stride(image);
                unsigned char* data = cairo_image_surface_get_data(image);

                if (image_width <= 0 || image_height <= 0 || data == nullptr)
                {
                    cairo_surface_unmap_image(surface, image);
                    cairo_surface_destroy(surface);
                    self->frame.failed = true;
                    self->frame.status = "Welcome snapshot is empty";
                    return;
                }

                self->frame.width = image_width;
                self->frame.height = image_height;
                self->frame.rgba.resize(
                    static_cast<std::size_t>(image_width) *
                    static_cast<std::size_t>(image_height) * 4U);

                for (int y = 0; y < image_height; ++y)
                {
                    const auto* row = data + static_cast<std::ptrdiff_t>(y) * stride;
                    for (int x = 0; x < image_width; ++x)
                    {
                        const auto* pixel = row + static_cast<std::ptrdiff_t>(x) * 4;
                        const std::size_t out =
                            (static_cast<std::size_t>(y) *
                             static_cast<std::size_t>(image_width) +
                             static_cast<std::size_t>(x)) * 4U;

#if G_BYTE_ORDER == G_LITTLE_ENDIAN
                        self->frame.rgba[out + 0U] = pixel[2];
                        self->frame.rgba[out + 1U] = pixel[1];
                        self->frame.rgba[out + 2U] = pixel[0];
                        self->frame.rgba[out + 3U] = pixel[3];
#else
                        self->frame.rgba[out + 0U] = pixel[1];
                        self->frame.rgba[out + 1U] = pixel[2];
                        self->frame.rgba[out + 2U] = pixel[3];
                        self->frame.rgba[out + 3U] = pixel[0];
#endif
                    }
                }

                cairo_surface_unmap_image(surface, image);
                cairo_surface_destroy(surface);

                self->frame.ready = true;
                self->frame.failed = false;
                self->frame.status = "Welcome page ready";
            },
            this);
    }
};

WelcomePage::WelcomePage() = default;

WelcomePage::~WelcomePage()
{
    if (impl_ != nullptr)
    {
        if (impl_->window != nullptr)
        {
            gtk_widget_destroy(impl_->window);
        }
        delete impl_;
        impl_ = nullptr;
    }
}

bool WelcomePage::initialize(int width, int height)
{
    if (impl_ != nullptr) return true;

    int argc = 0;
    char** argv = nullptr;
    if (!gtk_init_check(&argc, &argv))
    {
        return false;
    }

    impl_ = new Impl{};
    impl_->width = std::max(width, 1);
    impl_->height = std::max(height, 1);
    impl_->frame.status = "Loading NexVerse welcome page";

    impl_->window = gtk_offscreen_window_new();
    impl_->web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());

    gtk_widget_set_size_request(
        GTK_WIDGET(impl_->web_view),
        impl_->width,
        impl_->height);
    gtk_container_add(GTK_CONTAINER(impl_->window), GTK_WIDGET(impl_->web_view));
    gtk_widget_show_all(impl_->window);

    g_signal_connect(
        impl_->web_view,
        "load-failed",
        G_CALLBACK(+[](
            WebKitWebView*,
            WebKitLoadEvent,
            const gchar*,
            GError* error,
            gpointer user_data) -> gboolean
        {
            auto* self = static_cast<Impl*>(user_data);
            self->frame.failed = true;
            self->frame.status =
                error != nullptr && error->message != nullptr
                    ? error->message
                    : "Welcome page load failed";
            return FALSE;
        }),
        impl_);

    g_signal_connect(
        impl_->web_view,
        "load-changed",
        G_CALLBACK(+[](WebKitWebView* view, WebKitLoadEvent event, gpointer user_data)
        {
            if (event != WEBKIT_LOAD_FINISHED) return;

            auto* self = static_cast<Impl*>(user_data);
            WebKitWebResource* resource = webkit_web_view_get_main_resource(view);
            WebKitURIResponse* response =
                resource != nullptr ? webkit_web_resource_get_response(resource) : nullptr;

            const guint status =
                response != nullptr ? webkit_uri_response_get_status_code(response) : 0U;

            if (status < 200U || status >= 400U)
            {
                self->frame.failed = true;
                self->frame.ready = false;
                self->frame.status =
                    status == 0U
                        ? "Welcome page returned no HTTP status"
                        : "Welcome page HTTP " + std::to_string(status);
                return;
            }

            self->loaded = true;
            self->request_snapshot();
        }),
        impl_);

    webkit_web_view_load_uri(impl_->web_view, std::string{nexverse_welcome_url}.c_str());
    return true;
}

void WelcomePage::resize(int width, int height)
{
    if (impl_ == nullptr) return;

    const int new_width = std::max(width, 1);
    const int new_height = std::max(height, 1);
    if (new_width == impl_->width && new_height == impl_->height) return;

    impl_->width = new_width;
    impl_->height = new_height;
    gtk_widget_set_size_request(GTK_WIDGET(impl_->web_view), new_width, new_height);
    impl_->frame.ready = false;
    impl_->request_snapshot();
}

void WelcomePage::pump()
{
    if (impl_ == nullptr) return;

    while (gtk_events_pending())
    {
        gtk_main_iteration_do(FALSE);
    }

    if (impl_->loaded && !impl_->frame.ready && !impl_->frame.failed)
    {
        impl_->request_snapshot();
    }
}

const WelcomeFrame& WelcomePage::frame() const noexcept
{
    static const WelcomeFrame empty{};
    return impl_ != nullptr ? impl_->frame : empty;
}
}

#else

namespace nexverse::ui
{
struct WelcomePage::Impl
{
    WelcomeFrame frame;
};

WelcomePage::WelcomePage() = default;
WelcomePage::~WelcomePage()
{
    delete impl_;
}

bool WelcomePage::initialize(int, int)
{
    if (impl_ == nullptr) impl_ = new Impl{};
    impl_->frame.failed = true;
    impl_->frame.status = "Embedded welcome webview is not implemented on this platform yet";
    return false;
}

void WelcomePage::resize(int, int) {}
void WelcomePage::pump() {}

const WelcomeFrame& WelcomePage::frame() const noexcept
{
    static const WelcomeFrame empty{};
    return impl_ != nullptr ? impl_->frame : empty;
}
}

#endif
