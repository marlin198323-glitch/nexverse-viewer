/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
namespace nexverse::assets
{
struct FirestormAvatarJoint
{
    std::string name;
    int parent=-1;
    std::array<float,3> local{};
    std::array<float,3> bind_world{};
    std::array<float,3> euler_degrees{};
    bool support_base=false;
};
struct FirestormAvatarRig
{
    // The original Firestorm avatar_skeleton.xml bind-joint hierarchy.
    std::vector<FirestormAvatarJoint> joints;
    // The official attachment slot mapping from avatar_lad.xml.
    std::array<std::array<float,3>,64U> attachment_bind_offsets{};
    // Firestorm avatar_lad.xml local joint offsets; index+1 is zero
    // for virtual root/HUD frames, never an invented skeleton bone.
    std::array<std::array<float,3>,64U> attachment_local_offsets{};
    std::array<std::uint16_t,64U> attachment_joint_plus_one{};
    std::array<bool,64U> attachment_available{};
    std::array<bool,64U> attachment_is_hud{};
    std::size_t attachment_count=0U;
    [[nodiscard]] bool valid() const noexcept
    {
        return joints.size()>=100U && attachment_count>=40U;
    }
};
[[nodiscard]] std::optional<FirestormAvatarRig> load_firestorm_avatar_rig(
    const std::filesystem::path& character_dir);
// Reproduce Firestorm LLAvatarJointMesh::setupJoint's depth-first list of
// ancestor/current render joints, whose indices are stored in .llm weights.
// Returns skeleton indices including potentially repeated ancestor entries.
[[nodiscard]] std::optional<std::vector<std::uint16_t>>
firestorm_joint_render_palette(const FirestormAvatarRig& rig,
    const std::vector<std::string>& mesh_joint_names);
}
