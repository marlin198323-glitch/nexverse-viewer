/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/assets/mesh_loader.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <zlib.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace
{
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;
constexpr std::size_t max_asset_bytes = 32U * 1024U * 1024U;
constexpr std::size_t max_lod_bytes = 16U * 1024U * 1024U;
constexpr std::size_t max_total_vertices = 200000U;
constexpr std::size_t max_total_indices = 1200000U;

bool is_uuid(std::string_view s)
{
    if (s.size() != 36U) return false;
    for (std::size_t i = 0; i < s.size(); ++i)
    {
        if (i == 8U || i == 13U || i == 18U || i == 23U)
        {
            if (s[i] != '-') return false;
        }
        else if (!((s[i] >= '0' && s[i] <= '9') ||
                   (s[i] >= 'a' && s[i] <= 'f') ||
                   (s[i] >= 'A' && s[i] <= 'F')))
            return false;
    }
    return true;
}

std::uint16_t le16(std::span<const std::uint8_t> bytes, std::size_t index)
{
    return static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(bytes[index]) |
        (static_cast<std::uint16_t>(bytes[index + 1U]) << 8U));
}

std::optional<std::vector<std::uint8_t>> decompress(
    std::span<const std::uint8_t> bytes)
{
    if (bytes.empty() || bytes.size() > max_asset_bytes) return std::nullopt;
    std::vector<std::uint8_t> output(max_lod_bytes);
    z_stream stream{};
    stream.next_in = const_cast<Bytef*>(bytes.data());
    stream.avail_in = static_cast<uInt>(bytes.size());
    stream.next_out = output.data();
    stream.avail_out = static_cast<uInt>(output.size());
    if (inflateInit2(&stream, 15 + 32) != Z_OK) return std::nullopt;
    const int code = inflate(&stream, Z_FINISH);
    const auto produced = static_cast<std::size_t>(stream.total_out);
    inflateEnd(&stream);
    if (code != Z_STREAM_END || produced == 0U) return std::nullopt;
    output.resize(produced);
    return output;
}

std::optional<std::vector<float>> vector_domain(
    const LlsdValue* domain, std::string_view key,
    std::size_t dimensions)
{
    if (!domain || !domain->is_map()) return std::nullopt;
    const auto* value = domain->find(key);
    if (!value || !value->is_array() ||
        value->array_value.size() != dimensions) return std::nullopt;
    std::vector<float> result;
    for (const auto& item : value->array_value)
    {
        if (item.type != LlsdType::real &&
            item.type != LlsdType::integer) return std::nullopt;
        const auto number = item.type == LlsdType::real
            ? item.real_value : static_cast<double>(item.integer_value);
        if (!std::isfinite(number) || std::abs(number) > 65536.0)
            return std::nullopt;
        result.push_back(static_cast<float>(number));
    }
    return result;
}

float unit_u16(std::span<const std::uint8_t> data, std::size_t offset,
               float minimum, float maximum)
{
    return minimum + (maximum - minimum) *
        (static_cast<float>(le16(data, offset)) / 65535.0f);
}

bool append_submesh(const LlsdValue& part,
                    std::uint32_t face, nexverse::assets::MeshGeometry& geometry)
{
    if (!part.is_map()) return false;
    if (const auto* skipped = part.find("NoGeometry");
        skipped && skipped->type == LlsdType::boolean &&
        skipped->boolean_value) return true;
    const auto* positions = part.find("Position");
    const auto* triangles = part.find("TriangleList");
    if (!positions || positions->type != LlsdType::binary ||
        !triangles || triangles->type != LlsdType::binary ||
        positions->binary_value.empty() ||
        positions->binary_value.size() % 6U != 0U ||
        triangles->binary_value.empty() ||
        triangles->binary_value.size() % 6U != 0U) return false;
    const std::size_t count = positions->binary_value.size() / 6U;
    const std::size_t faces_indices = triangles->binary_value.size() / 2U;
    if (count > 65536U ||
        geometry.vertices.size() / 8U + faces_indices > max_total_vertices ||
        geometry.indices.size() + faces_indices > max_total_indices)
        return false;

    std::vector<float> pmin{-0.5f, -0.5f, -0.5f};
    std::vector<float> pmax{0.5f, 0.5f, 0.5f};
    if (const auto* domain = part.find("PositionDomain"))
    {
        auto mn = vector_domain(domain, "Min", 3U);
        auto mx = vector_domain(domain, "Max", 3U);
        if (!mn || !mx) return false;
        pmin = std::move(*mn); pmax = std::move(*mx);
    }

    const auto* uv = part.find("TexCoord0");
    std::vector<float> umin{0.0f, 0.0f}, umax{1.0f, 1.0f};
    if (uv)
    {
        if (uv->type != LlsdType::binary ||
            uv->binary_value.size() != count * 4U) return false;
        auto mn = vector_domain(part.find("TexCoord0Domain"), "Min", 2U);
        auto mx = vector_domain(part.find("TexCoord0Domain"), "Max", 2U);
        if (!mn || !mx) return false;
        umin = std::move(*mn); umax = std::move(*mx);
    }

    const auto& pos = positions->binary_value;
    const auto& tris = triangles->binary_value;
    const std::uint32_t first_index =
        static_cast<std::uint32_t>(geometry.indices.size());
    // Generate flat normals per triangle to avoid malformed vertex-normal data.
    for (std::size_t i = 0U; i < tris.size(); i += 6U)
    {
        const std::array<std::uint16_t, 3> ids{
            le16(tris, i), le16(tris, i + 2U), le16(tris, i + 4U)};
        if (ids[0] >= count || ids[1] >= count || ids[2] >= count)
            return false;
        std::array<std::array<float, 3>, 3> xyz{};
        for (std::size_t j = 0U; j < 3U; ++j)
        {
            for (std::size_t axis = 0U; axis < 3U; ++axis)
                xyz[j][axis] = unit_u16(
                    pos, static_cast<std::size_t>(ids[j]) * 6U + axis * 2U,
                    pmin[axis], pmax[axis]);
        }
        const std::array<float, 3> a{
            xyz[1][0] - xyz[0][0], xyz[1][1] - xyz[0][1],
            xyz[1][2] - xyz[0][2]};
        const std::array<float, 3> b{
            xyz[2][0] - xyz[0][0], xyz[2][1] - xyz[0][1],
            xyz[2][2] - xyz[0][2]};
        std::array<float, 3> n{
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]};
        const float length =
            std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        if (!std::isfinite(length)) return false;
        if (length > 0.000001f)
            for (float& value : n) value /= length;
        else n = {0.0f, 0.0f, 1.0f};

        for (std::size_t j = 0U; j < 3U; ++j)
        {
            const std::size_t vertex = geometry.vertices.size() / 8U;
            geometry.indices.push_back(static_cast<std::uint32_t>(vertex));
            geometry.vertices.insert(geometry.vertices.end(), {
                xyz[j][0], xyz[j][1], xyz[j][2],
                n[0], n[1], n[2],
                uv ? unit_u16(uv->binary_value,
                             static_cast<std::size_t>(ids[j]) * 4U,
                             umin[0], umax[0]) : 0.0f,
                uv ? unit_u16(uv->binary_value,
                             static_cast<std::size_t>(ids[j]) * 4U + 2U,
                             umin[1], umax[1]) : 0.0f});
        }
    }
    geometry.faces.push_back({
        face, first_index,
        static_cast<std::uint32_t>(geometry.indices.size()) - first_index});
    return true;
}
}

namespace nexverse::assets
{
std::string build_get_mesh_uri(std::string_view capability,
                               std::string_view asset_id)
{
    if (!is_uuid(asset_id) ||
        (capability.substr(0U, 7U) != "http://" &&
         capability.substr(0U, 8U) != "https://")) return {};
    std::string uri{capability};
    if (!uri.empty() && uri.back() != '?' && uri.back() != '&')
        uri.push_back(uri.find('?') == std::string::npos ? '?' : '&');
    uri += "mesh_id=";
    uri.append(asset_id);
    return uri;
}

MeshAssetResult decode_mesh_asset(std::string_view asset_id,
                                  std::string_view data)
{
    MeshAssetResult result;
    result.asset_id.assign(asset_id);
    if (!is_uuid(asset_id) || data.empty() || data.size() > max_asset_bytes)
    {
        result.message = "Mesh UUID or asset payload invalid";
        return result;
    }
    const auto raw = std::span{
        reinterpret_cast<const std::uint8_t*>(data.data()), data.size()};
    std::size_t header_size = 0U;
    const auto header = nexverse::protocol::parse_llsd_binary_prefix(
        raw, header_size);
    if (!header.ok() || !header.value->is_map() ||
        header_size >= raw.size())
    {
        result.message = "Mesh binary LLSD header invalid: " + header.error;
        return result;
    }
    const auto* lod = header.value->find("high_lod");
    if (!lod || !lod->is_map())
        lod = header.value->find("medium_lod");
    if (!lod || !lod->is_map())
    {
        result.message = "Mesh has no supported LOD descriptor";
        return result;
    }
    const auto* offset = lod->find("offset");
    const auto* size = lod->find("size");
    if (!offset || !size || offset->type != LlsdType::integer ||
        size->type != LlsdType::integer ||
        offset->integer_value < 0 || size->integer_value <= 0 ||
        static_cast<std::uint64_t>(offset->integer_value) > raw.size() - header_size ||
        static_cast<std::uint64_t>(size->integer_value) >
            raw.size() - header_size -
                static_cast<std::size_t>(offset->integer_value))
    {
        result.message = "Mesh LOD block outside asset boundaries";
        return result;
    }
    const std::size_t begin =
        header_size + static_cast<std::size_t>(offset->integer_value);
    const auto expanded = decompress(
        raw.subspan(begin, static_cast<std::size_t>(size->integer_value)));
    if (!expanded)
    {
        result.message = "Mesh LOD zlib/gzip decompression failed";
        return result;
    }
    const auto parsed = nexverse::protocol::parse_llsd_binary(*expanded);
    if (!parsed.ok() || !parsed.value->is_array() ||
        parsed.value->array_value.size() > 64U)
    {
        result.message = "Mesh LOD LLSD array invalid: " + parsed.error;
        return result;
    }
    MeshGeometry geometry;
    for (std::size_t i = 0U; i < parsed.value->array_value.size(); ++i)
    {
        if (!append_submesh(parsed.value->array_value[i],
                            static_cast<std::uint32_t>(i), geometry))
        {
            result.message = "Mesh submesh geometry invalid; face=" +
                             std::to_string(i);
            return result;
        }
    }
    if (!geometry.valid())
    {
        result.message = "Mesh LOD contains no renderable triangles";
        return result;
    }
    result.geometry = std::move(geometry);
    result.success = true;
    result.message = "Mesh high/medium LOD loaded; triangles=" +
                     std::to_string(result.geometry.indices.size() / 3U);
    return result;
}

MeshAssetLoader::MeshAssetLoader(
    std::shared_ptr<net::IHttpTransport> transport)
    : transport_{std::move(transport)}
{
}

MeshAssetResult MeshAssetLoader::fetch(
    std::string_view capability, std::string_view asset_id) const
{
    MeshAssetResult result;
    result.asset_id.assign(asset_id);
    const std::string uri = build_get_mesh_uri(capability, asset_id);
    if (!transport_ || uri.empty())
    {
        result.message = "GetMesh transport or URI invalid";
        return result;
    }
    const auto response = transport_->get(
        uri, max_asset_bytes, std::chrono::seconds{8},
        std::chrono::seconds{45});
    if (!response.ok())
    {
        result.message = response.error.empty()
            ? "GetMesh HTTP request failed" : response.error;
        return result;
    }
    return decode_mesh_asset(asset_id, response.body);
}
}
