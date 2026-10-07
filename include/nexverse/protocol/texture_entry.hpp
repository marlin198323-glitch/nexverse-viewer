/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string>

namespace nexverse::protocol
{
struct TextureEntryDefaultFace
{
    std::string texture_id;
    std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
    float repeat_u = 1.0f;
    float repeat_v = 1.0f;
    float offset_u = 0.0f;
    float offset_v = 0.0f;
    float rotation = 0.0f;
    std::uint8_t material = 0U;
    std::uint8_t media = 0U;
    float glow = 0.0f;
    std::string material_id;
    std::string render_material_id;

    [[nodiscard]] bool fullbright() const noexcept
    {
        return (material & 0x20U) != 0U;
    }

    [[nodiscard]] std::uint8_t shiny() const noexcept
    {
        return static_cast<std::uint8_t>((material >> 6U) & 0x03U);
    }

    [[nodiscard]] std::uint8_t bumpiness() const noexcept
    {
        return static_cast<std::uint8_t>(material & 0x1FU);
    }

    [[nodiscard]] bool media_present() const noexcept
    {
        return (media & 0x01U) != 0U;
    }

    [[nodiscard]] std::uint8_t texgen() const noexcept
    {
        return static_cast<std::uint8_t>((media >> 1U) & 0x03U);
    }

    [[nodiscard]] bool texgen_planar() const noexcept
    {
        return texgen() == 1U;
    }
};

struct TextureEntry
{
    TextureEntryDefaultFace default_face;
    std::array<std::optional<TextureEntryDefaultFace>, 64> face_overrides;

    [[nodiscard]] const TextureEntryDefaultFace& face(
        std::size_t index) const noexcept
    {
        if (index < face_overrides.size() &&
            face_overrides[index].has_value())
        {
            return *face_overrides[index];
        }
        return default_face;
    }
};

[[nodiscard]] std::optional<TextureEntry>
parse_texture_entry(
    std::span<const std::uint8_t> bytes);

[[nodiscard]] std::optional<TextureEntryDefaultFace>
parse_texture_entry_default(
    std::span<const std::uint8_t> bytes);
}
