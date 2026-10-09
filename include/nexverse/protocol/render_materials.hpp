/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/net/http_transport.hpp"
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
// OpenSim serializes per-channel UV transforms as integers * 10000.
struct LegacyMapTransform
{
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float repeat_x = 1.0f;
    float repeat_y = 1.0f;
    float rotation = 0.0f;
    bool operator==(const LegacyMapTransform&) const = default;
};

// Legacy OpenSim RenderMaterials (not the newer glTF/PBR material asset).
struct LegacyMaterial
{
    std::uint8_t diffuse_alpha_mode = 1U; // 0=none, 1=blend, 2=mask, 3=emissive
    std::uint8_t alpha_mask_cutoff = 0U;
    std::string normal_id;
    std::string specular_id;
    LegacyMapTransform normal_transform;
    LegacyMapTransform specular_transform;
    std::array<float, 3> specular_color{1.0f, 1.0f, 1.0f};
    std::uint8_t specular_exponent = 51U;
    std::uint8_t environment_intensity = 0U;

    bool operator==(const LegacyMaterial&) const = default;
};

struct RenderMaterialsResult
{
    bool success = false;
    std::map<std::string, LegacyMaterial, std::less<>> materials;
    std::string message;
};

inline constexpr std::size_t max_render_materials_batch_ids = 32U;

[[nodiscard]] bool is_valid_legacy_material_id(std::string_view id) noexcept;

// POST /RenderMaterials: LLSD XML {"Zipped": binary(zlib(LLSD binary
// array of sixteen-byte material UUIDs))}. Invalid batches are rejected.
[[nodiscard]] std::string build_render_materials_request(
    const std::vector<std::string>& material_ids);

[[nodiscard]] RenderMaterialsResult parse_render_materials_response(
    std::string_view llsd_xml);

class RenderMaterialsService
{
public:
    explicit RenderMaterialsService(std::shared_ptr<net::IHttpTransport> transport);
    // GET returns a region snapshot; do not run on the render thread.
    [[nodiscard]] RenderMaterialsResult fetch_all(std::string_view capability_uri) const;
    [[nodiscard]] RenderMaterialsResult fetch_ids(
        std::string_view capability_uri,
        const std::vector<std::string>& material_ids) const;
private:
    std::shared_ptr<net::IHttpTransport> transport_;
};
}
