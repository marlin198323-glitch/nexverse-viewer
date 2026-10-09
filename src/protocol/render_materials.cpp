/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/render_materials.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <zlib.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <utility>
#include <chrono>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

namespace
{
constexpr std::size_t max_compressed_bytes = 2U * 1024U * 1024U;
constexpr std::size_t max_uncompressed_bytes = 8U * 1024U * 1024U;
constexpr std::size_t max_materials = 8192U;

std::string id_from_bytes(std::span<const std::uint8_t> id)
{
    if (id.size() != 16U) return {};
    constexpr char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(36U);
    for (std::size_t i = 0U; i < id.size(); ++i)
    {
        if (i == 4U || i == 6U || i == 8U || i == 10U)
            result.push_back('-');
        result.push_back(hex[id[i] >> 4U]);
        result.push_back(hex[id[i] & 15U]);
    }
    return result;
}

bool valid_uuid(std::string_view id)
{
    if (id.size() != 36U) return false;
    for (std::size_t i = 0U; i < id.size(); ++i)
    {
        if (i == 8U || i == 13U || i == 18U || i == 23U)
        {
            if (id[i] != '-') return false;
        }
        else if (!((id[i] >= '0' && id[i] <= '9') ||
                   (id[i] >= 'a' && id[i] <= 'f') ||
                   (id[i] >= 'A' && id[i] <= 'F')))
            return false;
    }
    return true;
}

bool inflate_bounded(std::span<const std::uint8_t> compressed,
                     std::vector<std::uint8_t>& output)
{
    if (compressed.empty() || compressed.size() > max_compressed_bytes)
        return false;
    z_stream zs{};
    zs.next_in = const_cast<Bytef*>(compressed.data());
    zs.avail_in = static_cast<uInt>(compressed.size());
    if (inflateInit(&zs) != Z_OK) return false;
    std::array<std::uint8_t, 16384> buffer{};
    bool done = false;
    while (output.size() < max_uncompressed_bytes)
    {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(
            std::min(buffer.size(), max_uncompressed_bytes - output.size()));
        const auto limit = static_cast<std::size_t>(zs.avail_out);
        const int code = inflate(&zs, Z_NO_FLUSH);
        const std::size_t written = limit - zs.avail_out;
        output.insert(output.end(), buffer.begin(),
                      buffer.begin() + static_cast<std::ptrdiff_t>(written));
        if (code == Z_STREAM_END)
        {
            done = zs.avail_in == 0U;
            break;
        }
        if (code != Z_OK || written == 0U) break;
    }
    inflateEnd(&zs);
    return done;
}

std::string uuid_field(const nexverse::protocol::LlsdValue* field)
{
    using nexverse::protocol::LlsdType;
    if (!field) return {};
    if (field->type == LlsdType::binary)
        return id_from_bytes(field->binary_value);
    if ((field->type == LlsdType::uuid || field->type == LlsdType::string)
        && valid_uuid(field->string_value))
        return field->string_value;
    return {};
}

bool parse_byte(const nexverse::protocol::LlsdValue* field,
                std::uint8_t& value, std::int32_t maximum)
{
    using nexverse::protocol::LlsdType;
    if (!field || field->type != LlsdType::integer ||
        field->integer_value < 0 || field->integer_value > maximum)
        return false;
    value = static_cast<std::uint8_t>(field->integer_value);
    return true;
}
}

namespace nexverse::protocol
{
RenderMaterialsResult parse_render_materials_response(std::string_view xml)
{
    if (xml.empty() || xml.size() > max_compressed_bytes * 2U)
        return {false, {}, "RenderMaterials response exceeds limits"};
    auto parsed = parse_llsd_xml(xml);
    if (!parsed.ok() || !parsed.value->is_map())
        return {false, {}, "RenderMaterials outer LLSD is invalid"};
    const auto* zipped = parsed.value->find("Zipped");
    if (!zipped || zipped->type != LlsdType::binary)
        return {false, {}, "RenderMaterials Zipped binary is missing"};

    std::vector<std::uint8_t> inflated;
    if (!inflate_bounded(zipped->binary_value, inflated))
        return {false, {}, "RenderMaterials zlib payload is invalid or too large"};
    const auto binary = parse_llsd_binary(inflated);
    if (!binary.ok() || !binary.value->is_array() ||
        binary.value->array_value.size() > max_materials)
        return {false, {}, "RenderMaterials LLSD payload is invalid"};

    RenderMaterialsResult result;
    for (const auto& element : binary.value->array_value)
    {
        if (!element.is_map()) continue;
        const auto id = uuid_field(element.find("ID"));
        const auto* definition = element.find("Material");
        if (id.empty() || !definition || !definition->is_map()) continue;

        LegacyMaterial material;
        const auto* mode = definition->find("DiffuseAlphaMode");
        if (mode && !parse_byte(mode, material.diffuse_alpha_mode, 3))
            continue;
        const auto* cutoff = definition->find("AlphaMaskCutoff");
        if (cutoff && !parse_byte(cutoff, material.alpha_mask_cutoff, 255))
            continue;
        material.normal_id = uuid_field(definition->find("NormMap"));
        material.specular_id = uuid_field(definition->find("SpecMap"));
        result.materials[id] = std::move(material);
    }
    result.success = true;
    result.message = "RenderMaterials snapshot decoded";
    return result;
}

RenderMaterialsService::RenderMaterialsService(
    std::shared_ptr<net::IHttpTransport> transport)
    : transport_{std::move(transport)} {}

RenderMaterialsResult RenderMaterialsService::fetch_all(
    std::string_view capability_uri) const
{
    if (!transport_ || !is_valid_http_uri(capability_uri))
        return {false, {}, "RenderMaterials capability URL is missing or invalid"};
    const auto response = transport_->get(
        capability_uri, max_compressed_bytes * 2U,
        std::chrono::seconds{8}, std::chrono::seconds{20});
    if (!response.ok())
        return {false, {}, response.error.empty()
            ? "RenderMaterials capability GET failed" : response.error};
    return parse_render_materials_response(response.body);
}
}
