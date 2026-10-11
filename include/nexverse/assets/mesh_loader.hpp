/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/http_transport.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::assets
{
// OpenSim mesh LOD tiers, from cheapest to highest geometry quality.
enum class MeshLod : std::uint8_t { lowest = 0U, low = 1U, medium = 2U, high = 3U };
[[nodiscard]] constexpr std::string_view mesh_lod_name(MeshLod lod) noexcept
{
    switch (lod) {
    case MeshLod::lowest: return "lowest";
    case MeshLod::low: return "low";
    case MeshLod::medium: return "medium";
    case MeshLod::high: return "high";
    }
    return "high";
}
struct MeshFace
{
    std::uint32_t face = 0U;
    std::uint32_t first_index = 0U;
    std::uint32_t index_count = 0U;
};

struct MeshGeometry
{
    // Interleaved x,y,z / nx,ny,nz / u,v, exactly as required by OGLRender.
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<MeshFace> faces;

    [[nodiscard]] bool valid() const noexcept
    {
        return !vertices.empty() && vertices.size() % 8U == 0U &&
               !indices.empty() && indices.size() % 3U == 0U;
    }
};

struct MeshAssetResult
{
    bool success = false;
    std::string asset_id;
    MeshGeometry geometry;
    std::string message;
    MeshLod decoded_lod = MeshLod::high;
};

[[nodiscard]] std::string build_get_mesh_uri(
    std::string_view capability, std::string_view asset_id);
[[nodiscard]] MeshAssetResult decode_mesh_asset(
    std::string_view asset_id, std::string_view data,
    MeshLod preferred_lod = MeshLod::high);

class MeshAssetLoader
{
public:
    explicit MeshAssetLoader(std::shared_ptr<net::IHttpTransport> transport);
    [[nodiscard]] MeshAssetResult fetch(
        std::string_view get_mesh_capability,
        std::string_view mesh_id,
        std::string_view fallback_mesh_capability = {},
        MeshLod preferred_lod = MeshLod::high) const;

private:
    std::shared_ptr<net::IHttpTransport> transport_;
};
}
