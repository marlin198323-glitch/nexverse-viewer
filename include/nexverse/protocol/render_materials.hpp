/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/net/http_transport.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
// Legacy OpenSim RenderMaterials (not the newer glTF/PBR material asset).
struct LegacyMaterial
{
    std::uint8_t diffuse_alpha_mode = 1U; // 0=none, 1=blend, 2=mask, 3=emissive
    std::uint8_t alpha_mask_cutoff = 0U;
    std::string normal_id;
    std::string specular_id;
};

struct RenderMaterialsResult
{
    bool success = false;
    std::map<std::string, LegacyMaterial, std::less<>> materials;
    std::string message;
};

[[nodiscard]] RenderMaterialsResult parse_render_materials_response(
    std::string_view llsd_xml);

class RenderMaterialsService
{
public:
    explicit RenderMaterialsService(std::shared_ptr<net::IHttpTransport> transport);
    // GET returns a region snapshot; do not run on the render thread.
    [[nodiscard]] RenderMaterialsResult fetch_all(std::string_view capability_uri) const;
private:
    std::shared_ptr<net::IHttpTransport> transport_;
};
}
