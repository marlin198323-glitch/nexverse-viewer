/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

namespace nexverse::ui
{
// Independent, asynchronous depth-test evidence for original .llm body,
// never a claim that baked textures or outfits have been verified.
struct AvatarPartVisibility
{
    std::array<bool,3U> known{};
    std::array<bool,3U> passed{};
    std::uint64_t original_draws=0U;

    [[nodiscard]] bool any_known() const noexcept
    {
        return original_draws>0U && std::any_of(known.begin(),known.end(),
            [](bool v){return v;});
    }
    [[nodiscard]] bool any_passed() const noexcept
    {
        if(original_draws==0U)return false;
        for(std::size_t i=0U;i<3U;++i)
            if(known[i]&&passed[i])return true;
        return false;
    }
    [[nodiscard]] std::string stage(std::size_t part) const
    {
        if(part>=3U)return "nicht verfuegbar";
        if(original_draws==0U)return "kein LLM-Draw";
        if(!known[part])return "Depth ausstehend";
        return passed[part]?"Depth sichtbar":"Depth 0 (verdeckt)";
    }
    [[nodiscard]] std::string compact() const
    {
        const auto code=[&](std::size_t part)->char
        {
            if(original_draws==0U)return '-';
            if(!known[part])return '?';
            return passed[part]?'V':'0';
        };
        return "Depth H:"+std::string(1,code(0))+
            " U:"+std::string(1,code(1))+
            " L:"+std::string(1,code(2))+
            " (V=sichtbar 0=verdeckt ?=wartet)";
    }
};
}
