#include "nexverse/assets/texture_loader.hpp"
#include "jpeg2000_fixture.hpp"

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
    enum class Mode
    {
        png_ok,
        png_missing,
        png_invalid,
        png_returns_native_j2c,
        png_returns_native_jp2,
        png_returns_truncated_j2c,
        both_missing,
        both_unavailable,
        first_j2c_truncated_then_valid,
        always_j2c_truncated,
        j2c_non_tile_corruption
    };
    Mode mode = Mode::png_ok;
    std::string body;
    std::vector<std::string> requests;
    std::size_t j2c_requests=0U;

    nexverse::net::HttpResponse get(
        std::string_view url,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        requests.emplace_back(url);
        if (mode == Mode::both_unavailable)
            return {503,{}, "HTTP server returned status 503"};
        if (url.find("format=png") != std::string_view::npos)
        {
            if(mode==Mode::png_returns_native_j2c)
                return {200,std::string{
                    reinterpret_cast<const char*>(j2c_fixture),
                    sizeof(j2c_fixture)},{}};
            if(mode==Mode::png_returns_native_jp2)
                return {200,std::string{
                    reinterpret_cast<const char*>(jp2_fixture),
                    sizeof(jp2_fixture)},{}};
            if(mode==Mode::png_returns_truncated_j2c)
                return {200,std::string{
                    reinterpret_cast<const char*>(j2c_fixture),12U},{}};
            if (mode == Mode::first_j2c_truncated_then_valid ||
                mode == Mode::always_j2c_truncated ||
                mode == Mode::j2c_non_tile_corruption)
                return {200,"This is not PNG",{}};
            if (mode == Mode::png_missing || mode == Mode::both_missing)
                return {404, {}, "HTTP server returned status 404"};
            if (mode == Mode::png_invalid)
                return {200, "This is not PNG", {}};
            return {200, body, {}};
        }
        if (url.find("format=x-j2c") != std::string_view::npos)
        {
            ++j2c_requests;
            if((mode==Mode::first_j2c_truncated_then_valid&&
                j2c_requests==1U)||
                mode==Mode::always_j2c_truncated)
            {
                std::string bad{
                    reinterpret_cast<const char*>(j2c_fixture),
                    sizeof(j2c_fixture)};
                // Genuine codestream SOC/SOT present but Psot claims
                // a tile-part larger than the received server transfer.
                const auto sot=bad.find(
                    std::string{"\xff\x90\x00\x0a",4U});
                if(sot!=std::string::npos && sot+10U<=bad.size())
                    bad[sot+9U]=static_cast<char>(0x7f);
                return {200,std::move(bad),{}};
            }
            if (mode == Mode::both_missing)
                return {404, {}, "HTTP server returned status 404"};
            if(mode == Mode::j2c_non_tile_corruption)
                return {200, "not a jpeg2000 frame", {}};
            return {200, {
                reinterpret_cast<const char*>(j2c_fixture),
                sizeof(j2c_fixture)}, {}};
        }
        return {400, {}, "Unexpected GetTexture query"};
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
    return {
        reinterpret_cast<const char*>(bytes),
        sizeof(bytes)};
}

bool valid_fixture(const nexverse::assets::TextureAssetResult& result)
{
    if (!result.success || !result.image.valid() ||
        result.image.width != 16U || result.image.height != 16U ||
        result.image.pixels.size() != 16U * 16U * 4U)
        return false;
    const auto& px = result.image.pixels;
    return px[0] >= 208U && px[0] <= 212U &&
           px[1] >= 43U && px[1] <= 47U &&
           px[2] >= 18U && px[2] <= 22U &&
           px[3] == 255U;
}
}

int main()
{
    using namespace nexverse::assets;
    constexpr std::string_view id =
        "20000000-0000-0000-0000-000000000001";

    const std::string uri = build_get_texture_uri(
        "https://caps.example/GetTexture/", id);
    if (uri !=
        "https://caps.example/GetTexture/?texture_id="
        "20000000-0000-0000-0000-000000000001&format=png")
    {
        std::cerr << "GetTexture URI construction failed\n";
        return 1;
    }

    const auto decoded = decode_png_texture(id, one_pixel_png());
    if (!decoded.success || !decoded.image.valid() ||
        decoded.image.width != 1U || decoded.image.height != 1U ||
        decoded.image.pixels.size() != 4U)
    {
        std::cerr << "PNG decode failed: " << decoded.message << '\n';
        return 2;
    }

    auto transport = std::make_shared<MockTransport>();
    transport->body = one_pixel_png();

    TextureAssetLoader loader{transport};
    const auto png = loader.fetch_rgba(
        "https://caps.example/GetTexture/", id);
    if (!png.success || !png.image.valid() ||
        transport->requests.size() != 1U ||
        transport->requests.back() != uri)
    {
        std::cerr << "PNG-only fetch unexpectedly failed: " << png.message << '\n';
        return 3;
    }

    if (!build_get_texture_uri(
            "https://caps.example/GetTexture/", "not-a-uuid").empty())
    {
        std::cerr << "Invalid texture UUID unexpectedly accepted\n";
        return 4;
    }

    const std::string j2c{
        reinterpret_cast<const char*>(j2c_fixture), sizeof(j2c_fixture)};
    const std::string jp2{
        reinterpret_cast<const char*>(jp2_fixture), sizeof(jp2_fixture)};

    const auto j2c_image=decode_jpeg2000_texture(id,j2c);
    const auto jp2_image=decode_jpeg2000_texture(id,jp2);
    if (!valid_fixture(j2c_image))
    {
        std::cerr << "JPEG2000 raw codestream decode failed\n";
        return 5;
    }
    if (!valid_fixture(jp2_image))
    {
        std::cerr << "JPEG2000 JP2 image decode failed\n";
        return 6;
    }
    // OpenGenesisLINK server 2026-10-16 J2KDecoderModule now falls
    // back to OpenJPEG layer boundaries after CSJ2K failure. Viewer
    // already uses OpenJPEG for both raw J2C and boxed JP2 pixels;
    // ensure the transport-equivalent fixture pixels remain identical.
    if(j2c_image.image.width!=jp2_image.image.width ||
       j2c_image.image.height!=jp2_image.image.height ||
       j2c_image.image.pixels!=jp2_image.image.pixels)
    {
        std::cerr << "JPEG2000 codestream vs JP2 pixels differ\n";
        return 24;
    }

    const std::string rgba_j2c{
        reinterpret_cast<const char*>(j2c_rgba_fixture),
        sizeof(j2c_rgba_fixture)};
    const auto decoded_alpha = decode_jpeg2000_texture(id, rgba_j2c);
    if (!decoded_alpha.success || !decoded_alpha.image.valid() ||
        decoded_alpha.image.width != 16U ||
        decoded_alpha.image.pixels[0] < 208U ||
        decoded_alpha.image.pixels[3] < 118U ||
        decoded_alpha.image.pixels[3] > 122U)
    {
        std::cerr << "JPEG2000 alpha channel was not preserved: "
                  << decoded_alpha.message << '\n';
        return 11;
    }

    transport->requests.clear();
    transport->mode = MockTransport::Mode::png_missing;
    const auto native = loader.fetch_rgba("https://caps.example/GetTexture/", id);
    if (!valid_fixture(native) || transport->requests.size() != 2U ||
        transport->requests[0] != uri ||
        transport->requests[1].find("format=x-j2c") == std::string::npos)
    {
        std::cerr << "GetTexture HTTP 404 fallback failed: " << native.message << '\n';
        return 7;
    }

    // Real OpenSim proxies sometimes ignore format=png and return a
    // native J2C or JP2 body. Do not needlessly download it twice.
    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::png_returns_native_j2c;
    const auto mislabeled_j2c=loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if(!valid_fixture(mislabeled_j2c)||
       transport->requests.size()!=1U||transport->j2c_requests!=0U||
       mislabeled_j2c.message.find(
           "PNG response contained valid JPEG2000")==std::string::npos)
        return 28;
    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::png_returns_native_jp2;
    const auto mislabeled_jp2=loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if(!valid_fixture(mislabeled_jp2)||
       transport->requests.size()!=1U||transport->j2c_requests!=0U)
        return 29;
    // An authentic SOC marker followed by a truncated body must NOT
    // count as a valid PNG response; fetch the independently requested
    // native J2C instead and accept only its full decoded pixels.
    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::png_returns_truncated_j2c;
    const auto bad_native=loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if(!valid_fixture(bad_native)||
       transport->requests.size()!=2U||
       transport->j2c_requests!=1U||
       bad_native.message.find("fallback succeeded")==std::string::npos)
        return 30;

    transport->requests.clear();
    transport->mode = MockTransport::Mode::png_invalid;
    const auto bad_png = loader.fetch_rgba("https://caps.example/GetTexture/", id);
    if (!valid_fixture(bad_png) || transport->requests.size() != 2U)
    {
        std::cerr << "Malformed PNG fallback failed: " << bad_png.message << '\n';
        return 8;
    }

    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::first_j2c_truncated_then_valid;
    const auto recovered=loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if(!valid_fixture(recovered)||transport->requests.size()!=3U||
       transport->j2c_requests!=2U||!recovered.recovered_j2c_transfer||
       recovered.message.find("fresh retry succeeded")==std::string::npos)
        return 25;
    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::always_j2c_truncated;
    const auto unrecoverable=loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if(unrecoverable.success||unrecoverable.recovered_j2c_transfer||
       transport->requests.size()!=3U||transport->j2c_requests!=2U||
       unrecoverable.message.find("retry:")==std::string::npos||
       unrecoverable.failure!=AssetFailure::invalid)
        return 26;
    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::j2c_non_tile_corruption;
    const auto invalid_format=loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if(invalid_format.success||invalid_format.recovered_j2c_transfer||
       transport->requests.size()!=2U||
       transport->j2c_requests!=1U ||
       invalid_format.failure!=AssetFailure::invalid)
        return 27;
    transport->requests.clear();
    transport->j2c_requests=0U;
    transport->mode=MockTransport::Mode::both_missing;
    const auto missing = loader.fetch_rgba("https://caps.example/GetTexture/", id);
    if (missing.success ||
        missing.failure != AssetFailure::missing ||
        transport->requests.size() != 2U ||
        missing.message.find("png:") == std::string::npos ||
        missing.message.find("x-j2c:") == std::string::npos ||
        missing.message.find("404") == std::string::npos)
    {
        std::cerr << "Dual GetTexture failure diagnostics missing\n";
        return 9;
    }

    transport->mode = MockTransport::Mode::both_unavailable;
    const auto unavailable = loader.fetch_rgba(
        "https://caps.example/GetTexture/",id);
    if (unavailable.success ||
        unavailable.failure != AssetFailure::transient ||
        unavailable.message.find("503") == std::string::npos)
        return 12;
    transport->mode = MockTransport::Mode::png_ok;

    if (decode_jpeg2000_texture(id, "invalid").success ||
        decode_jpeg2000_texture(id, j2c.substr(0U, 22U)).success ||
        decode_jpeg2000_texture(id, "").success)
    {
        std::cerr << "Malformed JPEG2000 unexpectedly decoded\n";
        return 10;
    }

    std::cout << "PNG, native J2C/JP2 decoding, HTTP fallback, invalid image"
                 " and diagnostics tests passed\n";
    return 0;
}
