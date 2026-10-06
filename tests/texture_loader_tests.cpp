#include "nexverse/assets/texture_loader.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace
{
class MockTransport final : public nexverse::net::IHttpTransport
{
public:
    nexverse::net::HttpResponse get(
        std::string_view url,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        last_url = std::string{url};
        return {200, body, {}};
    }

    nexverse::net::HttpResponse post(
        std::string_view,
        std::string_view,
        std::string_view,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        return {405, {}, "not supported"};
    }

    std::string body;
    std::string last_url;
};

std::string one_pixel_png()
{
    static constexpr std::uint8_t bytes[] = {
        0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,
        0x00,0x00,0x00,0x0D,0x49,0x48,0x44,0x52,
        0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,
        0x08,0x06,0x00,0x00,0x00,0x1F,0x15,0xC4,
        0x89,0x00,0x00,0x00,0x0D,0x49,0x44,0x41,
        0x54,0x78,0x9C,0x63,0xF8,0xCF,0xC0,0xF0,
        0x1F,0x00,0x05,0x00,0x01,0xFF,0x89,0x99,
        0x3D,0x1D,0x00,0x00,0x00,0x00,0x49,0x45,
        0x4E,0x44,0xAE,0x42,0x60,0x82
    };

    return std::string{
        reinterpret_cast<const char*>(bytes),
        sizeof(bytes)};
}
}

int main()
{
    using namespace nexverse::assets;

    constexpr std::string_view id =
        "20000000-0000-0000-0000-000000000001";

    const std::string uri = build_get_texture_uri(
        "https://caps.example/GetTexture/",
        id);

    if (uri !=
        "https://caps.example/GetTexture/?texture_id="
        "20000000-0000-0000-0000-000000000001&format=png")
    {
        std::cerr << "GetTexture URI construction failed\n";
        return 1;
    }

    const auto decoded = decode_png_texture(id, one_pixel_png());
    if (!decoded.success ||
        !decoded.image.valid() ||
        decoded.image.width != 1U ||
        decoded.image.height != 1U ||
        decoded.image.pixels.size() != 4U)
    {
        std::cerr << "PNG decode failed: " << decoded.message << '\n';
        return 2;
    }

    auto transport = std::make_shared<MockTransport>();
    transport->body = one_pixel_png();

    TextureAssetLoader loader{transport};
    const auto fetched = loader.fetch_png(
        "https://caps.example/GetTexture/",
        id);

    if (!fetched.success ||
        !fetched.image.valid() ||
        transport->last_url != uri)
    {
        std::cerr << "Texture fetch failed: " << fetched.message << '\n';
        return 3;
    }

    if (!build_get_texture_uri(
            "https://caps.example/GetTexture/",
            "not-a-uuid").empty())
    {
        std::cerr << "Invalid texture UUID unexpectedly accepted\n";
        return 4;
    }

    std::cout << "Texture loader tests passed\n";
    return 0;
}
