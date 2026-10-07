/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/protocol/texture_entry.hpp"

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

namespace
{
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
    std::size_t& offset)
{
    if (offset >= bytes.size())
    {
        return false;
    }

    std::uint64_t face_bits = 0U;
    std::uint8_t value = 0U;

    do
    {
        if (offset >= bytes.size())
        {
            return false;
        }

        value = bytes[offset++];
        face_bits =
            (face_bits << 7U) |
            static_cast<std::uint64_t>(
                value & 0x7FU);
    }
    while ((value & 0x80U) != 0U);

    return face_bits != 0U;
}

bool skip_overrides(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    std::size_t value_size)
{
    while (true)
    {
        const std::size_t before = offset;
        const bool has_faces =
            read_face_bitfield(bytes, offset);

        if (offset == before)
        {
            return false;
        }

        if (!has_faces)
        {
            return true;
        }

        if (offset + value_size > bytes.size())
        {
            return false;
        }

        offset += value_size;
    }
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
}

namespace nexverse::protocol
{
std::optional<TextureEntryDefaultFace>
parse_texture_entry_default(
    std::span<const std::uint8_t> bytes)
{
    if (bytes.size() < 16U)
    {
        return std::nullopt;
    }

    TextureEntryDefaultFace face;
    std::size_t offset = 0U;

    face.texture_id =
        uuid_from_bytes(bytes, offset);
    if (face.texture_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    if (!skip_overrides(bytes, offset, 16U) ||
        offset + 4U > bytes.size())
    {
        return std::nullopt;
    }

    constexpr float inv_255 =
        1.0f / 255.0f;
    for (std::size_t index = 0U;
         index < 4U;
         ++index)
    {
        face.color[index] =
            static_cast<float>(
                255U - bytes[offset + index]) *
            inv_255;
    }
    offset += 4U;

    if (!skip_overrides(bytes, offset, 4U) ||
        offset + 4U > bytes.size())
    {
        return std::nullopt;
    }
    face.repeat_u = read_le_f32(bytes, offset);
    offset += 4U;

    if (!skip_overrides(bytes, offset, 4U) ||
        offset + 4U > bytes.size())
    {
        return std::nullopt;
    }
    face.repeat_v = read_le_f32(bytes, offset);
    offset += 4U;

    if (!skip_overrides(bytes, offset, 2U) ||
        offset + 2U > bytes.size())
    {
        return std::nullopt;
    }
    face.offset_u =
        texture_offset(read_le_u16(bytes, offset));
    offset += 2U;

    if (!skip_overrides(bytes, offset, 2U) ||
        offset + 2U > bytes.size())
    {
        return std::nullopt;
    }
    face.offset_v =
        texture_offset(read_le_u16(bytes, offset));
    offset += 2U;

    if (!skip_overrides(bytes, offset, 2U) ||
        offset + 2U > bytes.size())
    {
        return std::nullopt;
    }
    face.rotation =
        texture_rotation(read_le_u16(bytes, offset));
    offset += 2U;

    if (!skip_overrides(bytes, offset, 1U) ||
        offset + 1U > bytes.size())
    {
        return std::nullopt;
    }
    face.material = bytes[offset++];

    if (!skip_overrides(bytes, offset, 1U))
    {
        return std::nullopt;
    }

    if (offset >= bytes.size())
    {
        return face;
    }

    face.media = bytes[offset++];

    if (!skip_overrides(bytes, offset, 1U))
    {
        return std::nullopt;
    }

    if (offset >= bytes.size())
    {
        return face;
    }

    face.glow =
        static_cast<float>(bytes[offset++]) *
        inv_255;

    if (!skip_overrides(bytes, offset, 1U))
    {
        return std::nullopt;
    }

    if (offset + 16U <= bytes.size())
    {
        face.material_id =
            uuid_from_bytes(bytes, offset);
        offset += 16U;

        if (!skip_overrides(bytes, offset, 16U))
        {
            return std::nullopt;
        }
    }

    if (offset + 16U <= bytes.size())
    {
        face.render_material_id =
            uuid_from_bytes(bytes, offset);
        offset += 16U;

        if (!skip_overrides(bytes, offset, 16U))
        {
            return std::nullopt;
        }
    }

    if (!std::isfinite(face.repeat_u) ||
        !std::isfinite(face.repeat_v) ||
        !std::isfinite(face.offset_u) ||
        !std::isfinite(face.offset_v) ||
        !std::isfinite(face.rotation))
    {
        return std::nullopt;
    }

    return face;
}
}
