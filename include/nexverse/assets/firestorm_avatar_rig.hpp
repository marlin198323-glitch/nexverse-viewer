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
};
struct FirestormAvatarRig
{
    // The original Firestorm avatar_skeleton.xml bind-joint hierarchy.
    std::vector<FirestormAvatarJoint> joints;
    // The official attachment slot mapping from avatar_lad.xml.
    std::array<std::array<float,3>,64U> attachment_bind_offsets{};
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
}
