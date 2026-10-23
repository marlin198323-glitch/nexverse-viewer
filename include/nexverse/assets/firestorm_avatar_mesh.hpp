/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <utility>
#include <string>
#include <vector>
namespace nexverse::assets
{
struct FirestormMorphVertex
{
    std::uint32_t index=0U;
    std::array<float,3U> position_delta{};
    std::array<float,3U> normal_delta{};
    std::array<float,2U> uv_delta{};
};
struct FirestormMorphTarget
{
    std::string name;
    std::vector<FirestormMorphVertex> vertices;
};
struct FirestormAvatarMesh
{
    // Firestorm Linden Binary Mesh 1.0: x/y/z,nx/ny/nz,original u/v
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
    // Firestorm joint blending is floor(weight) plus fractional joint mix.
    // Store it for future skeletal skinning rather than discard it.
    std::vector<float> skin_weights;
    std::vector<std::string> joint_names;
    // Original .llm sparse MorphTargets, stored separately from immutable
    // bind-pose geometry for future per-avatar rendering.
    std::vector<FirestormMorphTarget> morph_targets;
    std::array<float,3U> origin{};
    std::array<float,3U> scale{1.0f,1.0f,1.0f};
    bool has_weights = false;
    [[nodiscard]] bool valid() const noexcept
    {
        return vertices.size()>=24U && vertices.size()%8U==0U &&
            indices.size()>=3U && indices.size()%3U==0U &&
            (!has_weights || skin_weights.size()==vertices.size()/8U);
    }
};
[[nodiscard]] std::optional<FirestormAvatarMesh> decode_firestorm_avatar_llm(
    std::span<const std::uint8_t> bytes);
// Apply named original sparse morph deltas to a COPY of the bind pose.
 // No viewer shape/COF inference, no shared renderer mutation, no new
 // asset download. Values must come from independently verified sources.
[[nodiscard]] std::optional<FirestormAvatarMesh> morph_firestorm_avatar_mesh(
    const FirestormAvatarMesh& base,
    const std::vector<std::pair<std::string,float>>& weights);
[[nodiscard]] std::optional<FirestormAvatarMesh> load_firestorm_avatar_llm(
    const std::filesystem::path& file);
[[nodiscard]] std::optional<std::filesystem::path>
find_firestorm_avatar_character_directory();
}
