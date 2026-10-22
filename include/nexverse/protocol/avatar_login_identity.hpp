/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <optional>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
// Matches the two login identities accepted by the current server:
// legacy first/last and a single account username (OpenGenesisLINK #268).
// Never invent a Resident surname: server canonicalizes account usernames.
struct AvatarLoginIdentity
{
    std::string first;
    std::string last;
    bool account_username=false;
};
[[nodiscard]] inline std::optional<AvatarLoginIdentity>
parse_avatar_login_identity(std::string_view raw)
{
    constexpr std::string_view whitespace=" \t\r\n";
    const auto start=raw.find_first_not_of(whitespace);
    if(start==std::string_view::npos)return std::nullopt;
    const auto end=raw.find_last_not_of(whitespace);
    const auto name=raw.substr(start,end-start+1U);
    // Embedded control characters have no meaning in an avatar account.
    if(name.find_first_of("\r\n\t")!=std::string_view::npos)
        return std::nullopt;
    const auto separator=name.find(' ');
    if(separator==std::string_view::npos)
        return AvatarLoginIdentity{std::string{name},{},true};
    const auto start_last=name.find_first_not_of(' ',separator);
    if(start_last==std::string_view::npos)return std::nullopt;
    return AvatarLoginIdentity{
        std::string{name.substr(0U,separator)},
        std::string{name.substr(start_last)},false};
}
}
