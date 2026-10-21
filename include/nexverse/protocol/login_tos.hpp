/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
// Upstream OpenGenesisLINK #263 canonical versioned citizens' ToS page.
inline constexpr std::string_view nexverse_tos_page=
    "https://stadt-nexverse.de/service/tos";
// Explicit compatibility only for deployed grids still using #260.
inline constexpr std::string_view nexverse_legacy_tos_page=
    "https://stadt-nexverse.de/buergerverordnung";

// Never follow arbitrary LoginService-supplied URLs. Explicitly authorize
// exactly the canonical new and old NexVerse paths, on one official host,
// over HTTP (legacy Firestorm entrypoint) or HTTPS. Normalize to HTTPS
// for external browser use; reject query, fragment, userinfo or subdomains.
[[nodiscard]] inline std::string_view canonical_nexverse_tos_url(
    std::string_view url) noexcept
{
    if(url=="http://stadt-nexverse.de/service/tos"||
       url=="https://stadt-nexverse.de/service/tos"||
       url=="http://stadt-nexverse.de/service/tos/"||
       url=="https://stadt-nexverse.de/service/tos/")
        return nexverse_tos_page;
    if(url=="http://stadt-nexverse.de/buergerverordnung"||
       url=="https://stadt-nexverse.de/buergerverordnung"||
       url=="http://stadt-nexverse.de/buergerverordnung/"||
       url=="https://stadt-nexverse.de/buergerverordnung/")
        return nexverse_legacy_tos_page;
    return {};
}

inline bool trusted_nexverse_tos_challenge(std::string_view url) noexcept
{
    return !canonical_nexverse_tos_url(url).empty();
}

enum class TosPhase { none, awaiting_document, awaiting_consent, agreed };
struct LoginTosConsent
{
    TosPhase phase=TosPhase::none;
    // A fixed official HTTPS literal, never a pointer into a login packet.
    std::string_view document_url=nexverse_tos_page;
    std::string avatar_name;
    std::string login_uri;
    void reset()
    {
        phase=TosPhase::none;document_url=nexverse_tos_page;
        avatar_name.clear();login_uri.clear();
    }
    bool challenge(std::string_view reason,std::string_view message,
                   std::string_view avatar,std::string_view grid)
    {
        reset();
        const auto official=canonical_nexverse_tos_url(message);
        if(reason!="tos"||official.empty()||
           avatar.empty()||grid.empty())return false;
        document_url=official;
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
