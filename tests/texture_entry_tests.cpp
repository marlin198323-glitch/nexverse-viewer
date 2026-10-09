#include "nexverse/protocol/texture_entry.hpp"

#include <bit>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

namespace
{
void append_uuid(
    std::vector<std::uint8_t>& out,
    std::string_view id)
{
    const auto hex = [](char value) -> std::uint8_t
    {
        if (value >= '0' && value <= '9')
        {
            return static_cast<std::uint8_t>(value - '0');
        }
        if (value >= 'a' && value <= 'f')
        {
            return static_cast<std::uint8_t>(
                10 + value - 'a');
        }
        if (value >= 'A' && value <= 'F')
        {
            return static_cast<std::uint8_t>(
                10 + value - 'A');
        }
        return 0U;
    };

    for (std::size_t index = 0U;
         index < id.size();)
    {
        if (id[index] == '-')
        {
            ++index;
            continue;
        }

        const std::uint8_t high = hex(id[index++]);
        const std::uint8_t low = hex(id[index++]);
        out.push_back(
            static_cast<std::uint8_t>(
                (high << 4U) | low));
    }
}

void append_f32(
    std::vector<std::uint8_t>& out,
    float value)
{
    const std::uint32_t bits =
        std::bit_cast<std::uint32_t>(value);

    out.push_back(
        static_cast<std::uint8_t>(
            bits & 0xFFU));
    out.push_back(
        static_cast<std::uint8_t>(
            (bits >> 8U) & 0xFFU));
    out.push_back(
        static_cast<std::uint8_t>(
            (bits >> 16U) & 0xFFU));
    out.push_back(
        static_cast<std::uint8_t>(
            (bits >> 24U) & 0xFFU));
}

void append_u16(
    std::vector<std::uint8_t>& out,
    std::uint16_t value)
{
    out.push_back(
        static_cast<std::uint8_t>(
            value & 0xFFU));
    out.push_back(
        static_cast<std::uint8_t>(
            value >> 8U));
}
}

int main()
{
    std::vector<std::uint8_t> bytes;

    append_uuid(
        bytes,
        "89556747-24cb-43ed-920b-47caed15465f");
    bytes.push_back(0U);

    bytes.insert(
        bytes.end(),
        {0U, 0U, 0U, 0U});
    bytes.push_back(0U);

    append_f32(bytes, 1.5f);
    bytes.push_back(0U);
    append_f32(bytes, 0.75f);
    bytes.push_back(0U);

    append_u16(bytes, 8192U);
    bytes.push_back(0U);
    append_u16(bytes, 0U);
    bytes.push_back(0U);

    append_u16(bytes, 4096U);
    bytes.push_back(0U);

    bytes.push_back(0x63U);
    bytes.push_back(0U);

    bytes.push_back(0x03U);
    bytes.push_back(0U);

    bytes.push_back(64U);
    bytes.push_back(0U);

    const auto parsed =
        nexverse::protocol::parse_texture_entry_default(
            bytes);

    if (!parsed.has_value())
    {
        std::cerr << "TextureEntry parse failed\n";
        return 1;
    }

    if (parsed->texture_id !=
            "89556747-24cb-43ed-920b-47caed15465f" ||
        parsed->color[0] != 1.0f ||
        parsed->color[1] != 1.0f ||
        parsed->color[2] != 1.0f ||
        parsed->color[3] != 1.0f ||
        parsed->repeat_u != 1.5f ||
        parsed->repeat_v != 0.75f ||
        parsed->material != 0x63U ||
        parsed->media != 0x03U ||
        !parsed->fullbright() ||
        parsed->shiny() != 1U ||
        parsed->bumpiness() != 3U ||
        !parsed->media_present() ||
        !parsed->texgen_planar() ||
        parsed->texgen() != 1U ||
        parsed->glow < 0.24f ||
        parsed->glow > 0.26f)
    {
        std::cerr << "TextureEntry default values mismatch\n";
        return 2;
    }

    if (nexverse::protocol::parse_texture_entry_default(
            std::span<const std::uint8_t>{})
            .has_value())
    {
        std::cerr << "Empty TextureEntry unexpectedly accepted\n";
        return 3;
    }

    std::vector<std::uint8_t> overrides;

    append_uuid(
        overrides,
        "89556747-24cb-43ed-920b-47caed15465f");
    overrides.push_back(0x01U);
    append_uuid(
        overrides,
        "11111111-2222-3333-4444-555555555555");
    overrides.push_back(0U);

    overrides.insert(
        overrides.end(),
        {0U, 0U, 0U, 0U});
    overrides.push_back(0x04U);
    overrides.insert(
        overrides.end(),
        {255U, 127U, 0U, 0U});
    overrides.push_back(0U);

    append_f32(overrides, 1.0f);
    overrides.push_back(0U);
    append_f32(overrides, 1.0f);
    overrides.push_back(0U);

    append_u16(overrides, 0U);
    overrides.push_back(0U);
    append_u16(overrides, 0U);
    overrides.push_back(0U);

    append_u16(overrides, 0U);
    overrides.push_back(0U);

    overrides.push_back(0U);
    overrides.push_back(0U);

    overrides.push_back(0U);
    overrides.push_back(0U);

    overrides.push_back(0U);
    overrides.push_back(0x02U);
    overrides.push_back(128U);
    overrides.push_back(0U);

    // Legacy normal/specular MaterialID and RenderMaterial UUID metadata.
    append_uuid(overrides, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");
    overrides.push_back(0x04U); // MaterialID override on face 2
    append_uuid(overrides, "10101010-2020-3030-4040-505050505050");
    overrides.push_back(0U);
    append_uuid(overrides, "aaaaaaaa-1111-2222-3333-444444444444");
    overrides.push_back(0x02U); // RenderMaterial override on face 1
    append_uuid(overrides, "bbbbbbbb-1111-2222-3333-444444444444");
    overrides.push_back(0U);

    const auto full =
        nexverse::protocol::parse_texture_entry(
            overrides);
    if (!full.has_value())
    {
        std::cerr << "Per-face TextureEntry parse failed\n";
        return 4;
    }

    const auto& face0 = full->face(0U);
    const auto& face1 = full->face(1U);
    const auto& face2 = full->face(2U);

    if (face0.texture_id !=
            "11111111-2222-3333-4444-555555555555" ||
        face0.repeat_u != 1.0f ||
        face1.glow < 0.49f ||
        face1.glow > 0.51f ||
        face1.texture_id !=
            "89556747-24cb-43ed-920b-47caed15465f" ||
        face2.color[0] != 0.0f ||
        face2.color[1] < 0.49f ||
        face2.color[1] > 0.51f ||
        face2.texture_id !=
            "89556747-24cb-43ed-920b-47caed15465f" ||
        face0.material_id != "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee" ||
        face1.material_id != "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee" ||
        face2.material_id != "10101010-2020-3030-4040-505050505050" ||
        face0.render_material_id != "aaaaaaaa-1111-2222-3333-444444444444" ||
        face1.render_material_id != "bbbbbbbb-1111-2222-3333-444444444444" ||
        face2.render_material_id != "aaaaaaaa-1111-2222-3333-444444444444")
    {
        std::cerr << "Per-face TextureEntry inheritance mismatch\n";
        return 5;
    }

    std::cout << "TextureEntry tests passed\n";
    return 0;
}
