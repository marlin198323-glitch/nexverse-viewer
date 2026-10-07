/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/protocol/texture_entry.hpp"

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <utility>

namespace
{
using nexverse::protocol::TextureEntry;
using nexverse::protocol::TextureEntryDefaultFace;

std::string uuid_from_bytes(
    std::span<const std::uint8_t> bytes,
    std::size_t offset)
{
    if (offset + 16U > bytes.size())
    {
        return {};
    }

    static constexpr char hex[] =
        "0123456789abcdef";

    std::string out;
    out.reserve(36U);

    for (std::size_t index = 0U; index < 16U; ++index)
    {
        if (index == 4U || index == 6U ||
            index == 8U || index == 10U)
        {
            out.push_back('-');
        }

        const std::uint8_t value = bytes[offset + index];
        out.push_back(hex[value >> 4U]);
        out.push_back(hex[value & 0x0FU]);
    }

    return out;
}

bool read_face_bitfield(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    std::uint64_t& face_bits)
{
    if (offset >= bytes.size())
    {
        return false;
    }

    face_bits = 0U;
    std::uint8_t value = 0U;
    std::size_t count = 0U;

    do
    {
        if (offset >= bytes.size() || count >= 10U)
        {
            return false;
        }

        value = bytes[offset++];
        ++count;

        if (face_bits >
            (std::numeric_limits<std::uint64_t>::max() >> 7U))
        {
            return false;
        }

        face_bits =
            (face_bits << 7U) |
            static_cast<std::uint64_t>(
                value & 0x7FU);
    }
    while ((value & 0x80U) != 0U);

    return true;
}

std::uint16_t read_le_u16(
    std::span<const std::uint8_t> bytes,
    std::size_t offset)
{
    return
        static_cast<std::uint16_t>(bytes[offset]) |
        static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(
                bytes[offset + 1U]) << 8U);
}

float read_le_f32(
    std::span<const std::uint8_t> bytes,
    std::size_t offset)
{
    const std::uint32_t bits =
        static_cast<std::uint32_t>(bytes[offset]) |
        (static_cast<std::uint32_t>(
             bytes[offset + 1U]) << 8U) |
        (static_cast<std::uint32_t>(
             bytes[offset + 2U]) << 16U) |
        (static_cast<std::uint32_t>(
             bytes[offset + 3U]) << 24U);

    return std::bit_cast<float>(bits);
}

float texture_offset(std::uint16_t raw)
{
    const auto signed_value =
        static_cast<std::int16_t>(raw);
    return static_cast<float>(signed_value) /
        32767.0f;
}

float texture_rotation(std::uint16_t raw)
{
    constexpr float two_pi =
        6.28318530717958647692f;
    const auto signed_value =
        static_cast<std::int16_t>(raw);
    return
        static_cast<float>(signed_value) /
        32767.0f * two_pi;
}

template <typename T>
using FaceValues =
    std::array<std::optional<T>, 64>;

template <typename T, typename Reader>
bool read_override_section(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    std::size_t value_size,
    FaceValues<T>& values,
    std::array<bool, 64>& touched,
    Reader&& reader)
{
    while (true)
    {
        std::uint64_t face_bits = 0U;
        if (!read_face_bitfield(
                bytes,
                offset,
                face_bits))
        {
            return false;
        }

        if (face_bits == 0U)
        {
            return true;
        }

        if (offset + value_size > bytes.size())
        {
            return false;
        }

        const T value = reader(bytes, offset);
        offset += value_size;

        for (std::size_t face = 0U;
             face < values.size();
             ++face)
        {
            if ((face_bits &
                 (std::uint64_t{1} << face)) == 0U)
            {
                continue;
            }

            values[face] = value;
            touched[face] = true;
        }
    }
}

std::array<float, 4> read_color(
    std::span<const std::uint8_t> bytes,
    std::size_t offset)
{
    constexpr float inv_255 =
        1.0f / 255.0f;

    return {
        static_cast<float>(255U - bytes[offset]) * inv_255,
        static_cast<float>(255U - bytes[offset + 1U]) * inv_255,
        static_cast<float>(255U - bytes[offset + 2U]) * inv_255,
        static_cast<float>(255U - bytes[offset + 3U]) * inv_255};
}

bool finite_face(const TextureEntryDefaultFace& face)
{
    return
        std::isfinite(face.repeat_u) &&
        std::isfinite(face.repeat_v) &&
        std::isfinite(face.offset_u) &&
        std::isfinite(face.offset_v) &&
        std::isfinite(face.rotation) &&
        std::isfinite(face.glow);
}
}

namespace nexverse::protocol
{
std::optional<TextureEntry>
parse_texture_entry(
    std::span<const std::uint8_t> bytes)
{
    if (bytes.size() < 16U)
    {
        return std::nullopt;
    }

    TextureEntry result;
    std::array<bool, 64> touched{};

    FaceValues<std::string> texture_ids;
    FaceValues<std::array<float, 4>> colors;
    FaceValues<float> repeat_u_values;
    FaceValues<float> repeat_v_values;
    FaceValues<float> offset_u_values;
    FaceValues<float> offset_v_values;
    FaceValues<float> rotation_values;
    FaceValues<std::uint8_t> material_values;
    FaceValues<std::uint8_t> media_values;
    FaceValues<float> glow_values;
    FaceValues<std::string> material_ids;
    FaceValues<std::string> render_material_ids;

    std::size_t offset = 0U;

    result.default_face.texture_id =
        uuid_from_bytes(bytes, offset);
    if (result.default_face.texture_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    if (!read_override_section<std::string>(
            bytes,
            offset,
            16U,
            texture_ids,
            touched,
            [](auto source, std::size_t at)
            {
                return uuid_from_bytes(source, at);
            }))
    {
        return std::nullopt;
    }

    if (offset + 4U > bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.color =
        read_color(bytes, offset);
    offset += 4U;

    if (!read_override_section<std::array<float, 4>>(
            bytes,
            offset,
            4U,
            colors,
            touched,
            [](auto source, std::size_t at)
            {
                return read_color(source, at);
            }))
    {
        return std::nullopt;
    }

    if (offset + 4U > bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.repeat_u =
        read_le_f32(bytes, offset);
    offset += 4U;

    if (!read_override_section<float>(
            bytes,
            offset,
            4U,
            repeat_u_values,
            touched,
            [](auto source, std::size_t at)
            {
                return read_le_f32(source, at);
            }))
    {
        return std::nullopt;
    }

    if (offset + 4U > bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.repeat_v =
        read_le_f32(bytes, offset);
    offset += 4U;

    if (!read_override_section<float>(
            bytes,
            offset,
            4U,
            repeat_v_values,
            touched,
            [](auto source, std::size_t at)
            {
                return read_le_f32(source, at);
            }))
    {
        return std::nullopt;
    }

    if (offset + 2U > bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.offset_u =
        texture_offset(
            read_le_u16(bytes, offset));
    offset += 2U;

    if (!read_override_section<float>(
            bytes,
            offset,
            2U,
            offset_u_values,
            touched,
            [](auto source, std::size_t at)
            {
                return texture_offset(
                    read_le_u16(source, at));
            }))
    {
        return std::nullopt;
    }

    if (offset + 2U > bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.offset_v =
        texture_offset(
            read_le_u16(bytes, offset));
    offset += 2U;

    if (!read_override_section<float>(
            bytes,
            offset,
            2U,
            offset_v_values,
            touched,
            [](auto source, std::size_t at)
            {
                return texture_offset(
                    read_le_u16(source, at));
            }))
    {
        return std::nullopt;
    }

    if (offset + 2U > bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.rotation =
        texture_rotation(
            read_le_u16(bytes, offset));
    offset += 2U;

    if (!read_override_section<float>(
            bytes,
            offset,
            2U,
            rotation_values,
            touched,
            [](auto source, std::size_t at)
            {
                return texture_rotation(
                    read_le_u16(source, at));
            }))
    {
        return std::nullopt;
    }

    if (offset >= bytes.size())
    {
        return std::nullopt;
    }
    result.default_face.material =
        bytes[offset++];

    if (!read_override_section<std::uint8_t>(
            bytes,
            offset,
            1U,
            material_values,
            touched,
            [](auto source, std::size_t at)
            {
                return source[at];
            }))
    {
        return std::nullopt;
    }

    if (offset >= bytes.size())
    {
        if (!finite_face(result.default_face))
        {
            return std::nullopt;
        }
        return result;
    }

    result.default_face.media =
        bytes[offset++];

    if (!read_override_section<std::uint8_t>(
            bytes,
            offset,
            1U,
            media_values,
            touched,
            [](auto source, std::size_t at)
            {
                return source[at];
            }))
    {
        return std::nullopt;
    }

    if (offset >= bytes.size())
    {
        if (!finite_face(result.default_face))
        {
            return std::nullopt;
        }
    }
    else
    {
        constexpr float inv_255 =
            1.0f / 255.0f;
        result.default_face.glow =
            static_cast<float>(bytes[offset++]) *
            inv_255;

        if (!read_override_section<float>(
                bytes,
                offset,
                1U,
                glow_values,
                touched,
                [](auto source, std::size_t at)
                {
                    return
                        static_cast<float>(source[at]) /
                        255.0f;
                }))
        {
            return std::nullopt;
        }

        if (offset + 16U <= bytes.size())
        {
            result.default_face.material_id =
                uuid_from_bytes(bytes, offset);
            offset += 16U;

            if (!read_override_section<std::string>(
                    bytes,
                    offset,
                    16U,
                    material_ids,
                    touched,
                    [](auto source, std::size_t at)
                    {
                        return uuid_from_bytes(
                            source,
                            at);
                    }))
            {
                return std::nullopt;
            }
        }

        if (offset + 16U <= bytes.size())
        {
            result.default_face.render_material_id =
                uuid_from_bytes(bytes, offset);
            offset += 16U;

            if (!read_override_section<std::string>(
                    bytes,
                    offset,
                    16U,
                    render_material_ids,
                    touched,
                    [](auto source, std::size_t at)
                    {
                        return uuid_from_bytes(
                            source,
                            at);
                    }))
            {
                return std::nullopt;
            }
        }
    }

    if (!finite_face(result.default_face))
    {
        return std::nullopt;
    }

    for (std::size_t face = 0U;
         face < result.face_overrides.size();
         ++face)
    {
        if (!touched[face])
        {
            continue;
        }

        TextureEntryDefaultFace resolved =
            result.default_face;

        if (texture_ids[face].has_value())
            resolved.texture_id = *texture_ids[face];
        if (colors[face].has_value())
            resolved.color = *colors[face];
        if (repeat_u_values[face].has_value())
            resolved.repeat_u = *repeat_u_values[face];
        if (repeat_v_values[face].has_value())
            resolved.repeat_v = *repeat_v_values[face];
        if (offset_u_values[face].has_value())
            resolved.offset_u = *offset_u_values[face];
        if (offset_v_values[face].has_value())
            resolved.offset_v = *offset_v_values[face];
        if (rotation_values[face].has_value())
            resolved.rotation = *rotation_values[face];
        if (material_values[face].has_value())
            resolved.material = *material_values[face];
        if (media_values[face].has_value())
            resolved.media = *media_values[face];
        if (glow_values[face].has_value())
            resolved.glow = *glow_values[face];
        if (material_ids[face].has_value())
            resolved.material_id = *material_ids[face];
        if (render_material_ids[face].has_value())
            resolved.render_material_id =
                *render_material_ids[face];

        if (!finite_face(resolved))
        {
            return std::nullopt;
        }

        result.face_overrides[face] =
            std::move(resolved);
    }

    return result;
}

std::optional<TextureEntryDefaultFace>
parse_texture_entry_default(
    std::span<const std::uint8_t> bytes)
{
    const auto parsed =
        parse_texture_entry(bytes);
    if (!parsed.has_value())
    {
        return std::nullopt;
    }

    return parsed->default_face;
}
}
