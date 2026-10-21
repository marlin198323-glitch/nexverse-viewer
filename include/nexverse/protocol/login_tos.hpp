/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
inline constexpr std::string_view nexverse_tos_page=
    "https://stadt-nexverse.de/buergerverordnung";

// NexVerse currently returns a plain HTTP start URL because native
// Firestorm's ToS viewer recognizes that scheme. The external browser
// always opens the official HTTPS page instead. No arbitrary URL, redirect
// target, query/fragment, or embedded login credentials can be launched.
inline bool trusted_nexverse_tos_challenge(std::string_view url) noexcept
{
    return url=="http://stadt-nexverse.de/buergerverordnung"||
           url=="https://stadt-nexverse.de/buergerverordnung"||
           url=="http://stadt-nexverse.de/buergerverordnung/"||
           url=="https://stadt-nexverse.de/buergerverordnung/";
}

enum class TosPhase { none, awaiting_document, awaiting_consent, agreed };
struct LoginTosConsent
{
    TosPhase phase=TosPhase::none;
    std::string avatar_name;
    std::string login_uri;
    void reset()
    {
        phase=TosPhase::none;avatar_name.clear();login_uri.clear();
    }
    bool challenge(std::string_view reason,std::string_view message,
                   std::string_view avatar,std::string_view grid)
    {
        reset();
        if(reason!="tos"||!trusted_nexverse_tos_challenge(message)||
           avatar.empty()||grid.empty())return false;
        avatar_name=avatar;
        login_uri=grid;
        phase=TosPhase::awaiting_document;
        return true;
    }
    [[nodiscard]] bool visible() const noexcept
    {
        return phase==TosPhase::awaiting_document||
               phase==TosPhase::awaiting_consent;
    }
    bool document_opened() noexcept
    {
        if(phase!=TosPhase::awaiting_document)return false;
        phase=TosPhase::awaiting_consent;
        return true;
    }
    bool agree() noexcept
    {
        if(phase!=TosPhase::awaiting_consent)return false;
        phase=TosPhase::agreed;
        return true;
    }
    [[nodiscard]] bool approved_for(std::string_view avatar,
                                    std::string_view grid) const noexcept
    {
        return phase==TosPhase::agreed&&avatar==avatar_name&&grid==login_uri;
    }
};
}
