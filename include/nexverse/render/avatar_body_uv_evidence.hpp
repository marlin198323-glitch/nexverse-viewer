/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <string>

namespace nexverse::render
{
// OGL vertex layout: (position xyz, normal xyz, original Firestorm uv).
// Use only as input validation/audit: do not regenerate mesh UVs or claim
// parity with real on-grid Firestorm bake pixels.
struct AvatarBodyUvEvidence
{
    std::size_t vertex_count=0U;
    std::size_t triangle_count=0U;
    std::array<float,2U> min_uv{};
    std::array<float,2U> max_uv{};
    bool safe_for_gpu=false;
};

[[nodiscard]] inline AvatarBodyUvEvidence audit_avatar_body_uv(
    std::span<const float> vertices,std::span<const std::uint32_t> indices)
{
    AvatarBodyUvEvidence result;
    if(vertices.size()<24U||vertices.size()%8U!=0U||
       indices.size()<3U||indices.size()%3U!=0U)
        return result;
    result.vertex_count=vertices.size()/8U;
    result.triangle_count=indices.size()/3U;
    result.min_uv={std::numeric_limits<float>::max(),
                   std::numeric_limits<float>::max()};
    result.max_uv={-std::numeric_limits<float>::max(),
                   -std::numeric_limits<float>::max()};
    for(std::size_t i=0U;i<vertices.size();i+=8U)
    {
        // Reject infinity/NaN in original geometry, normals or UVs.
        for(std::size_t j=0U;j<8U;++j)
            if(!std::isfinite(vertices[i+j]))
                return result;
        // Firestorm LLM decoder already accepts +/-100 original UV units;
        // keep this bound when a render mesh comes from another source.
        if(std::abs(vertices[i+6U])>100.0f||
           std::abs(vertices[i+7U])>100.0f)
            return result;
        for(std::size_t uv=0U;uv<2U;++uv)
        {
            result.min_uv[uv]=std::min(result.min_uv[uv],vertices[i+6U+uv]);
            result.max_uv[uv]=std::max(result.max_uv[uv],vertices[i+6U+uv]);
        }
    }
    for(const auto index:indices)
        if(index>=result.vertex_count)return result;
    // Entire original body meshes need a nonzero mapping in both axes.
    // No wrap/clamp/flip transform is applied in this audit.
    if(result.max_uv[0]-result.min_uv[0]<=1e-6f||
       result.max_uv[1]-result.min_uv[1]<=1e-6f)
        return result;
    result.safe_for_gpu=true;
    return result;
}

// Actual draw call counts are proof of a GL submission per original mesh,
// not of correct avatar pixel parity, garment wear, or asset upload.
[[nodiscard]] inline std::array<char,3U> avatar_gl_body_symbols(
    const std::array<std::uint64_t,3U>& actual_original_mesh_draws)
{
    return {
        actual_original_mesh_draws[0]>0U?'G':'-',
        actual_original_mesh_draws[1]>0U?'G':'-',
        actual_original_mesh_draws[2]>0U?'G':'-'};
}

// Used by the existing F9 outfit floater; the only "G" path is the
// anatomical original mesh draw, not the procedural proxy draw or cache hit.
[[nodiscard]] inline std::string describe_avatar_gl_body_draws(
    const std::array<std::uint64_t,3U>& draws,bool user_activated)
{
    if(!user_activated)return "3D GL: erst Appearance pruefen";
    const auto gl=avatar_gl_body_symbols(draws);
    return std::string{"3D Mesh GL H:"}+gl[0]+
        " U:"+gl[1]+" L:"+gl[2]+" (G=Draw, keine Pixelparitaet)";
}

}
