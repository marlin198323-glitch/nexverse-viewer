/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/login.hpp"

#include <algorithm>
#include <string_view>

namespace nexverse::protocol
{
// The OpenGenesisLINK server uses reason="presence" for several
// DISTINCT failures. Its XML-RPC message disambiguates them, but is
// untrusted (possibly including URLs, identifiers and session data).
// Only static, localized text and static diagnostic codes leave here.
// This does not change credentials, server state or login retry policy.
struct LoginFailureDiagnostic
{
    std::string_view category;
    std::string_view user_notice; // empty: preserve legacy non-presence UI
};

[[nodiscard]] inline bool login_text_contains(
    std::string_view text,std::string_view needle) noexcept
{
    if(text.size()>2048U)return false;
    const auto ascii_lower=[](unsigned char c) noexcept
    {
        return static_cast<unsigned char>(
            (c>='A'&&c<='Z')?c+('a'-'A'):c);
    };
    return std::search(text.begin(),text.end(),
        needle.begin(),needle.end(),
        [&](char l,char r)
        {
            return ascii_lower(static_cast<unsigned char>(l))==
                   ascii_lower(static_cast<unsigned char>(r));
        })!=text.end();
}

[[nodiscard]] inline LoginFailureDiagnostic diagnose_login_failure(
    const LoginResult& result) noexcept
{
    const auto reason=std::string_view{result.reason};
    const auto message=std::string_view{result.message};
    if(reason=="presence")
    {
        if(login_text_contains(message,"already logged in"))
            return {"presence_already_online",
                    "Dieser Avatar wird vom Gridserver noch als angemeldet "
                    "geführt. Andere Viewer schließen, kurz warten und "
                    "erneut anmelden. Falls es bleibt: Gridbetreiber "
                    "muss den Presence-Status prüfen."};
        if(login_text_contains(message,"not yet been verified")||
           login_text_contains(message,"has not been verified"))
            return {"presence_unverified",
                    "Dieses Avatar-Konto wurde noch nicht bestätigt. "
                    "Bitte die Kontobestätigung prüfen oder den "
                    "Gridbetreiber kontaktieren."};
        if(login_text_contains(message,"logins are currently restricted"))
            return {"presence_restricted",
                    "Der Gridserver beschränkt derzeit Anmeldungen. "
                    "Bitte später erneut versuchen oder den "
                    "Gridbetreiber kontaktieren."};
        return {"presence_unknown",
                "Der Gridserver hat die Anmeldung abgelehnt (Presence). "
                "Der genaue Grund ist nicht erkennbar. ROBUST-Loginlogs "
                "des Gridservers prüfen lassen."};
    }
    // Reject arbitrary untrusted 'reason' strings in diagnostic logs.
    if(reason=="tos")return {"tos",{}};
    if(reason=="key")return {"key",{}};
    if(reason=="user")return {"user",{}};
    if(reason=="grid")return {"grid",{}};
    if(reason=="inventory")return {"inventory",{}};
    if(reason=="login_failed")return {"login_failed",{}};
    if(reason=="http_error")return {"http_error",{}};
    if(reason=="invalid_xml")return {"invalid_xml",{}};
    if(reason=="invalid_response")return {"invalid_response",{}};
    if(reason=="incomplete_session")return {"incomplete_session",{}};
    if(reason=="invalid_login_uri")return {"invalid_login_uri",{}};
    if(reason=="transport_unavailable")return {"transport_unavailable",{}};
    if(reason=="missing_credentials")return {"missing_credentials",{}};
    return {"other",{}};
}
}
