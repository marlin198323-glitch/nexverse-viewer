/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

namespace nexverse::render
{
// GUI diagnostic evidence only. GPU draw submissions do not establish
// simulator avatar identity, final framebuffer visibility or pixel parity.
[[nodiscard]] inline std::string describe_avatar_gpu_submission(
    std::uint64_t original_body_draws,
    std::uint64_t procedural_proxy_draws,
    const std::array<std::uint64_t,3U>& self_bake_draws)
{
    if(original_body_draws==0U)
        return procedural_proxy_draws!=0U
            ? "Avatar GL: Ersatz-Mesh (kein LLM)"
            : "Avatar GL: kein Body-Draw";
    const auto yes=[](std::uint64_t count)->char
    {
        return count!=0U?'G':'-';
    };
    return "LLM GL "+std::to_string(
        std::min(original_body_draws,std::uint64_t{3U}))+
        "/3 | Bake H:"+yes(self_bake_draws[0])+
        " U:"+yes(self_bake_draws[1])+
        " L:"+yes(self_bake_draws[2]);
}
}
