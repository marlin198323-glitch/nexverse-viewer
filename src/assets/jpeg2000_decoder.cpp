/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/assets/texture_loader.hpp"
#include "nexverse/assets/jpeg2000_metadata_limits.hpp"

#include <openjpeg-2.5/openjpeg.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace
{
constexpr std::size_t max_compressed_bytes = 32U * 1024U * 1024U;

struct MemorySource
{
    const OPJ_BYTE* data = nullptr;
    std::size_t size = 0U;
    std::size_t offset = 0U;
};

OPJ_SIZE_T read_memory(void* buffer, OPJ_SIZE_T count, void* state)
{
    auto& input = *static_cast<MemorySource*>(state);
    if (input.offset >= input.size)
        return static_cast<OPJ_SIZE_T>(-1);
    const std::size_t actual = std::min<std::size_t>(
        static_cast<std::size_t>(count), input.size - input.offset);
    std::memcpy(buffer, input.data + input.offset, actual);
    input.offset += actual;
    return static_cast<OPJ_SIZE_T>(actual);
}

OPJ_OFF_T skip_memory(OPJ_OFF_T count, void* state)
{
    auto& input = *static_cast<MemorySource*>(state);
    if (count < 0)
    {
        const auto distance = static_cast<std::uint64_t>(-(count + 1)) + 1U;
        const auto actual = std::min(distance,
                                     static_cast<std::uint64_t>(input.offset));
        input.offset -= static_cast<std::size_t>(actual);
        return -static_cast<OPJ_OFF_T>(actual);
    }
    const auto actual = std::min(
        static_cast<std::uint64_t>(count),
        static_cast<std::uint64_t>(input.size - input.offset));
    input.offset += static_cast<std::size_t>(actual);
    return static_cast<OPJ_OFF_T>(actual);
}

OPJ_BOOL seek_memory(OPJ_OFF_T count, void* state)
{
    auto& input = *static_cast<MemorySource*>(state);
    if (count < 0 || static_cast<std::uint64_t>(count) > input.size)
        return OPJ_FALSE;
    input.offset = static_cast<std::size_t>(count);
    return OPJ_TRUE;
}

void on_decode_error(const char* message, void* state)
{
    auto& text = *static_cast<std::string*>(state);
    if (!text.empty() || message == nullptr) return;
    text.assign(message);
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r'))
        text.pop_back();
    if (text.size() > 200U) text.resize(200U);
}

using Stream = std::unique_ptr<opj_stream_t, decltype(&opj_stream_destroy)>;
using Codec = std::unique_ptr<opj_codec_t, decltype(&opj_destroy_codec)>;
using Image = std::unique_ptr<opj_image_t, decltype(&opj_image_destroy)>;

std::uint8_t sample_component(const opj_image_comp_t& comp,
                              std::uint32_t image_x,
                              std::uint32_t image_y)
{
    const auto x = static_cast<std::int64_t>(image_x) -
                   static_cast<std::int64_t>(comp.x0);
    const auto y = static_cast<std::int64_t>(image_y) -
                   static_cast<std::int64_t>(comp.y0);
    const std::uint32_t cx = static_cast<std::uint32_t>(std::clamp(
        x / static_cast<std::int64_t>(comp.dx),
        std::int64_t{0}, static_cast<std::int64_t>(comp.w - 1U)));
    const std::uint32_t cy = static_cast<std::uint32_t>(std::clamp(
        y / static_cast<std::int64_t>(comp.dy),
        std::int64_t{0}, static_cast<std::int64_t>(comp.h - 1U)));
    const auto value = static_cast<std::int64_t>(
        comp.data[static_cast<std::size_t>(cy) * comp.w + cx]);
    const std::int64_t normalized = value +
        (comp.sgnd != 0U ? (std::int64_t{1} << (comp.prec - 1U)) : 0);
    const std::int64_t max_value = (std::int64_t{1} << comp.prec) - 1;
    return static_cast<std::uint8_t>(std::clamp(
        (normalized * 255 + max_value / 2) / max_value,
        std::int64_t{0}, std::int64_t{255}));
}

std::string_view image_metadata_issue(const opj_image_t& image)
{
    if(image.x1<=image.x0 || image.y1<=image.y0)
        return "image_extent";
    const auto issue=nexverse::assets::jpeg2000_image_limits_reason(
        static_cast<std::uint64_t>(image.x1)-image.x0,
        static_cast<std::uint64_t>(image.y1)-image.y0,
        image.numcomps);
    if(!issue.empty())return issue;
    if(image.comps==nullptr)return "component_header";
    for(OPJ_UINT32 i=0U;i<image.numcomps;++i)
    {
        const auto& c=image.comps[i];
        const auto comp_issue=nexverse::assets::jpeg2000_component_limits_reason(
            c.w,c.h,c.dx,c.dy,c.prec);
        if(!comp_issue.empty())return comp_issue;
    }
    return {};
}
}

namespace nexverse::assets
{
TextureAssetResult decode_jpeg2000_texture(
    std::string_view texture_id, std::string_view bytes)
{
    TextureAssetResult result;
    result.asset_id.assign(texture_id);
    if (bytes.size() < 2U || bytes.size() > max_compressed_bytes)
    {
        result.message = "JPEG2000 input size exceeds limits";
        return result;
    }

    // JPEG2000 codestream (J2C) and boxed JP2 are both supported.
    const bool codestream =
        static_cast<unsigned char>(bytes[0]) == 0xffU &&
        static_cast<unsigned char>(bytes[1]) == 0x4fU;
    const bool jp2 = bytes.size() >= 12U &&
        bytes.substr(0U, 12U) ==
            std::string_view{"\x00\x00\x00\x0c\x6a\x50\x20\x20\x0d\x0a\x87\x0a", 12U};
    if (!codestream && !jp2)
    {
        result.message = "Not a JPEG2000 codestream or JP2 image";
        return result;
    }

    MemorySource source{
        reinterpret_cast<const OPJ_BYTE*>(bytes.data()),
        bytes.size(), 0U};
    Stream stream{opj_stream_create(16384U, OPJ_TRUE),
                  &opj_stream_destroy};
    Codec codec{opj_create_decompress(
        codestream ? OPJ_CODEC_J2K : OPJ_CODEC_JP2),
        &opj_destroy_codec};
    if (!stream || !codec)
    {
        result.message = "OpenJPEG initialization failed";
        return result;
    }

    opj_stream_set_user_data(stream.get(), &source, nullptr);
    opj_stream_set_user_data_length(
        stream.get(), static_cast<OPJ_UINT64>(source.size));
    opj_stream_set_read_function(stream.get(), &read_memory);
    opj_stream_set_skip_function(stream.get(), &skip_memory);
    opj_stream_set_seek_function(stream.get(), &seek_memory);

    std::string error;
    opj_set_error_handler(codec.get(), &on_decode_error, &error);
    opj_dparameters_t options{};
    opj_set_default_decoder_parameters(&options);
    if (!opj_setup_decoder(codec.get(), &options))
    {
        result.message = "OpenJPEG decoder setup failed";
        return result;
    }
    opj_image_t* decoded = nullptr;
    if (!opj_read_header(stream.get(), codec.get(), &decoded))
    {
        result.message = "JPEG2000 header failed: " + error;
        return result;
    }
    Image image{decoded, &opj_image_destroy};
    const auto header_issue=image
        ? image_metadata_issue(*image) : std::string_view{"image_header"};
    if(!header_issue.empty())
    {
        result.message = "JPEG2000 image metadata exceeds safe limits; reason=" +
            std::string{header_issue};
        return result;
    }
    if (!opj_decode(codec.get(), stream.get(), image.get()) ||
        !opj_end_decompress(codec.get(), stream.get()))
    {
        result.message = "JPEG2000 decode failed: " + error;
        return result;
    }
    const auto decoded_issue=image_metadata_issue(*image);
    if(!decoded_issue.empty())
    {
        result.message = "JPEG2000 decoded metadata is invalid; reason=" +
            std::string{decoded_issue};
        return result;
    }
    for (OPJ_UINT32 i = 0U; i < image->numcomps; ++i)
    {
        if (image->comps[i].data == nullptr)
        {
            result.message = "JPEG2000 image channel is missing";
            return result;
        }
    }

    const std::uint32_t width = image->x1 - image->x0;
    const std::uint32_t height = image->y1 - image->y0;
    result.image.width = width;
    result.image.height = height;
    result.image.pixels.resize(
        static_cast<std::size_t>(width) * height * 4U);

    for (std::uint32_t y = 0U; y < height; ++y)
    {
        for (std::uint32_t x = 0U; x < width; ++x)
        {
            std::array<std::uint8_t, 4> channels{0U, 0U, 0U, 255U};
            for (OPJ_UINT32 i = 0U; i < image->numcomps; ++i)
                channels[i] = sample_component(
                    image->comps[i], image->x0 + x, image->y0 + y);

            std::array<std::uint8_t, 4> rgba{
                channels[0], channels[0], channels[0], 255U};
            if (image->numcomps == 2U)
                rgba[3] = channels[1];
            if (image->numcomps >= 3U)
            {
                rgba[0] = channels[0];
                rgba[1] = channels[1];
                rgba[2] = channels[2];
                if (image->color_space == OPJ_CLRSPC_SYCC)
                {
                    const float cb = static_cast<float>(channels[1]) - 128.0f;
                    const float cr = static_cast<float>(channels[2]) - 128.0f;
                    const float yy = static_cast<float>(channels[0]);
                    const auto clamp_byte = [](float value)
                    {
                        return static_cast<std::uint8_t>(
                            std::clamp(value, 0.0f, 255.0f));
                    };
                    rgba[0] = clamp_byte(yy + 1.402f * cr);
                    rgba[1] = clamp_byte(yy - 0.344136f * cb - 0.714136f * cr);
                    rgba[2] = clamp_byte(yy + 1.772f * cb);
                }
                if (image->numcomps == 4U) rgba[3] = channels[3];
            }
            const std::size_t offset =
                (static_cast<std::size_t>(y) * width + x) * 4U;
            std::copy(rgba.begin(), rgba.end(),
                      result.image.pixels.begin() +
                          static_cast<std::ptrdiff_t>(offset));
        }
    }

    result.success = result.image.valid();
    result.message = result.success ? "JPEG2000 RGBA loaded"
                                    : "JPEG2000 RGBA size mismatch";
    return result;
}
}
