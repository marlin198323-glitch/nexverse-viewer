/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/grid_info.hpp"

#include <tinyxml2.h>

#include <cctype>
#include <string>

namespace
{
std::string trim(std::string_view input)
{
    std::size_t first = 0;
    while (first < input.size() && std::isspace(static_cast<unsigned char>(input[first])) != 0) ++first;

    std::size_t last = input.size();
    while (last > first && std::isspace(static_cast<unsigned char>(input[last - 1])) != 0) --last;

    return std::string{input.substr(first, last - first)};
}
}

namespace nexverse::protocol
{
const std::string* GridInfo::get(std::string_view key) const noexcept
{
    const auto found = fields.find(key);
    return found == fields.end() ? nullptr : &found->second;
}

std::string_view GridInfo::login_uri() const noexcept
{
    const auto* value = get("login");
    return value == nullptr ? std::string_view{} : std::string_view{*value};
}

std::string_view GridInfo::grid_name() const noexcept
{
    const auto* value = get("gridname");
    return value == nullptr ? std::string_view{} : std::string_view{*value};
}

std::string_view GridInfo::grid_nick() const noexcept
{
    const auto* value = get("gridnick");
    return value == nullptr ? std::string_view{} : std::string_view{*value};
}

bool is_valid_http_uri(std::string_view uri) noexcept
{
    const bool http = uri.starts_with("http://");
    const bool https = uri.starts_with("https://");
    if (!http && !https) return false;

    const std::size_t host_start = http ? 7U : 8U;
    if (host_start >= uri.size()) return false;

    const std::size_t host_end = uri.find_first_of("/?#", host_start);
    const std::string_view authority = uri.substr(
        host_start, host_end == std::string_view::npos ? std::string_view::npos : host_end - host_start);

    if (authority.empty()) return false;
    for (const char ch : authority)
    {
        if (std::isspace(static_cast<unsigned char>(ch)) != 0) return false;
    }
    return true;
}

GridInfoParseResult parse_grid_info_xml(std::string_view xml)
{
    if (xml.empty()) return {std::nullopt, "GridInfo XML is empty"};
    if (xml.size() > max_grid_info_bytes) return {std::nullopt, "GridInfo XML exceeds size limit"};

    tinyxml2::XMLDocument document;
    if (document.Parse(xml.data(), xml.size()) != tinyxml2::XML_SUCCESS)
    {
        return {std::nullopt, std::string{"GridInfo XML parse failed: "} + document.ErrorStr()};
    }

    const tinyxml2::XMLElement* root = document.RootElement();
    if (root == nullptr || std::string_view{root->Name()} != "gridinfo")
    {
        return {std::nullopt, "GridInfo XML root must be <gridinfo>"};
    }

    GridInfo info;
    for (const tinyxml2::XMLElement* child = root->FirstChildElement();
         child != nullptr; child = child->NextSiblingElement())
    {
        const char* name = child->Name();
        if (name == nullptr || *name == '\0') continue;
        const char* text = child->GetText();
        info.fields[std::string{name}] = text == nullptr ? std::string{} : trim(text);
    }

    if (info.login_uri().empty())
    {
        return {std::nullopt, "GridInfo XML does not contain a non-empty <login> value"};
    }
    if (!is_valid_http_uri(info.login_uri()))
    {
        return {std::nullopt, "GridInfo <login> is not a valid HTTP(S) URI"};
    }

    GridInfoDocument parsed{std::move(info), std::string{xml}};
    return {std::move(parsed), {}};
}
}
