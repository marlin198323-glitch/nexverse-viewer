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
    // Used for credentials-bearing XML-RPC login and CAPS URLs. A naive
    // "http(s):// followed by nonempty text" check allows userinfo
    // injection, e.g. hg.stadt-nexverse.de@evil.invalid. Never send
    // login secrets or capability tokens to a malformed authority.
    if(uri.empty()||uri.size()>8192U)return false;
    const bool http=uri.starts_with("http://");
    const bool https=uri.starts_with("https://");
    if(!http&&!https)return false;
    for(unsigned char ch:uri)
        if(ch<=0x20U||ch==0x7fU||ch=='\\'||ch=='#')return false;
    const std::size_t start=http?7U:8U;
    const std::size_t end=uri.find_first_of("/?",start);
    const auto authority=uri.substr(
        start,end==std::string_view::npos?std::string_view::npos:end-start);
    if(authority.empty()||authority.size()>255U||
       authority.find('@')!=std::string_view::npos||
       authority.find('%')!=std::string_view::npos)
        return false;

    std::string_view host,port;
    if(authority.front()=='[')
    {
        // Explicit IPv6 address. Reject zone IDs and unbalanced brackets,
        // but permit simulator IPv6 literals such as [::1]:9000.
        const auto close=authority.find(']');
        if(close==std::string_view::npos)return false;
        host=authority.substr(1U,close-1U);
        const auto suffix=authority.substr(close+1U);
        if(host.size()<2U||host.find(':')==std::string_view::npos)
            return false;
        for(unsigned char ch:host)
            if(!std::isxdigit(ch)&&ch!=':'&&ch!='.')return false;
        if(!suffix.empty())
        {
            if(suffix.front()!=':')return false;
            port=suffix.substr(1U);
        }
    }
    else
    {
        const auto colon=authority.find(':');
        host=authority.substr(0U,colon);
        if(colon!=std::string_view::npos)
        {
            if(authority.find(':',colon+1U)!=std::string_view::npos)
                return false; // unbracketed IPv6 or multiple port fields
            port=authority.substr(colon+1U);
        }
        if(host.empty()||host.size()>253U||host.front()=='.'||
           host.back()=='.')
            return false;
        std::size_t label_start=0U;
        for(std::size_t i=0;i<=host.size();++i)
        {
            if(i==host.size()||host[i]=='.')
            {
                const auto label=host.substr(label_start,i-label_start);
                if(label.empty()||label.size()>63U||
                   label.front()=='-'||label.back()=='-')
                    return false;
                label_start=i+1U;
            }
            else
            {
                const auto ch=static_cast<unsigned char>(host[i]);
                if(!std::isalnum(ch)&&ch!='-')return false;
            }
        }
    }
    if(authority.back()==':')return false; // empty explicit port
    if(!port.empty())
    {
        if(port.size()>5U)return false;
        unsigned value=0U;
        for(unsigned char ch:port)
        {
            if(ch<'0'||ch>'9')return false;
            value=value*10U+static_cast<unsigned>(ch-'0');
        }
        if(value==0U||value>65535U)return false;
    }
    // Prevent opaque query-only or slash-only lookalike authorities
    // from crossing to a different host in the HTTP transport.
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
