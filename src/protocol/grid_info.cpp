/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/protocol/grid_info.hpp"

#include <tinyxml2.h>

#include <algorithm>
#include <cctype>
#include <string>

namespace
{
std::string trim(std::string_view input)
{
    const auto is_space = [](unsigned char value)
    {
        return std::isspace(value) != 0;
    };

    auto begin = input.begin();
    auto end = input.end();

    while (begin != end && is_space(static_cast<unsigned char>(*begin)))
    {
        ++begin;
    }

    while (end != begin && is_space(static_cast<unsigned char>(*(end - 1))))
    {
        --end;
    }

    return std::string{begin, end};
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
    const std::string* value = get("login");
    return value == nullptr ? std::string_view{} : std::string_view{*value};
}

std::string_view GridInfo::grid_name() const noexcept
{
    const std::string* value = get("gridname");
    return value == nullptr ? std::string_view{} : std::string_view{*value};
}

std::string_view GridInfo::grid_nick() const noexcept
{
    const std::string* value = get("gridnick");
    return value == nullptr ? std::string_view{} : std::string_view{*value};
}

GridInfoParseResult parse_grid_info_xml(std::string_view xml)
{
    if (xml.empty())
    {
        return GridInfoParseResult{std::nullopt, "GridInfo XML is empty"};
    }

    if (xml.size() > max_grid_info_bytes)
    {
        return GridInfoParseResult{std::nullopt, "GridInfo XML exceeds size limit"};
    }

    tinyxml2::XMLDocument document;
    const auto parse_result = document.Parse(xml.data(), xml.size());
    if (parse_result != tinyxml2::XML_SUCCESS)
    {
        return GridInfoParseResult{
            std::nullopt,
            std::string{"GridInfo XML parse failed: "} + document.ErrorStr()};
    }

    const tinyxml2::XMLElement* root = document.RootElement();
    if (root == nullptr || std::string_view{root->Name()} != "gridinfo")
    {
        return GridInfoParseResult{std::nullopt, "GridInfo XML root must be <gridinfo>"};
    }

    GridInfo info;

    for (const tinyxml2::XMLElement* child = root->FirstChildElement();
         child != nullptr;
         child = child->NextSiblingElement())
    {
        const char* name = child->Name();
        if (name == nullptr || *name == '\0')
        {
            continue;
        }

        const char* text = child->GetText();
        info.fields[std::string{name}] = text == nullptr ? std::string{} : trim(text);
    }

    if (info.login_uri().empty())
    {
        return GridInfoParseResult{
            std::nullopt,
            "GridInfo XML does not contain a non-empty <login> value"};
    }

    if (!info.login_uri().starts_with("http://") &&
        !info.login_uri().starts_with("https://"))
    {
        return GridInfoParseResult{
            std::nullopt,
            "GridInfo <login> must use http:// or https://"};
    }

    GridInfoDocument parsed;
    parsed.info = std::move(info);
    parsed.raw_xml.assign(xml.data(), xml.size());

    return GridInfoParseResult{std::move(parsed), {}};
}
