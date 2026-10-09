/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/abi_v2.h"
#include <cstdio>

#if defined(_WIN32)
#define OGL_EXPORT extern "C" __declspec(dllexport)
#else
#define OGL_EXPORT extern "C" __attribute__((visibility("default")))
#endif

namespace {
const ogl_host_api_v2* host = nullptr;
bool initialized = false;
bool running = false;
uint64_t seen = 0;
uint32_t last_ready = 2;

void report(const char* text)
{
    if (host && host->base.log) host->base.log(host->base.context, 1, text);
}
int init(const ogl_host_api* base)
{
    if (!base || base->abi_version != OGL_MODULE_ABI_VERSION_2 ||
        base->struct_size < sizeof(ogl_host_api_v2)) return 1;
    const auto* expanded = reinterpret_cast<const ogl_host_api_v2*>(base);
    if (!expanded->read_diagnostics || !expanded->base.log) return 2;
    host = expanded;
    initialized = true;
    report("OGL Developer Tools v1 initialized (read-only diagnostics)");
    return 0;
}
int start()
{
    if (!initialized || running) return 1;
    running = true;
    seen = 0;
    last_ready = 2;
    report("OGL Developer Tools running");
    return 0;
}
int on_frame()
{
    if (!running || !host) return 1;
    ogl_diagnostics_snapshot_v1 snapshot{};
    snapshot.struct_size = sizeof(snapshot);
    snapshot.version = OGL_DIAGNOSTICS_VERSION_1;
    if (host->read_diagnostics(host->base.context, &snapshot) != 0) return 0;
    ++seen;
    if (last_ready != snapshot.simulation_ready || seen % 600 == 0)
    {
        last_ready = snapshot.simulation_ready;
        char line[256];
        std::snprintf(line, sizeof(line),
                      "Developer Tools: frame=%llu world=%s draws=%llu triangles=%llu objects=%llu",
                      static_cast<unsigned long long>(snapshot.frame_index),
                      snapshot.simulation_ready ? "ready" : "loading",
                      static_cast<unsigned long long>(snapshot.draw_calls),
                      static_cast<unsigned long long>(snapshot.triangles),
                      static_cast<unsigned long long>(snapshot.visible_objects));
        report(line);
    }
    return 0;
}
void stop() { running = false; }
void shutdown() { running = false; initialized = false; host = nullptr; }
const ogl_module_api_v2 api = {
    {sizeof(ogl_module_api_v2), OGL_MODULE_ABI_VERSION_2,
     "org.opengenesislink.developer-tools", &init, &start, &stop, &shutdown},
    &on_frame
};
}
OGL_EXPORT const ogl_module_api* ogl_module_get_api(uint32_t version)
{
    return version == OGL_MODULE_ABI_VERSION_2 ? &api.base : nullptr;
}
