/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <array>
#include <charconv>
#include <optional>
#include <string>
#include <string_view>

namespace nexverse::app
{
// An untrusted browser link may request only a start location in the
// designated NexVerse grid. It cannot select a login endpoint, credentials,
// execute code, trigger a teleport without login or control a foreign grid.
struct MarketplaceHop
{
    std::string region;
    std::array<unsigned,3> coords{};
    [[nodiscard]] std::string login_start() const
    {
        return "uri:"+region+"&"+std::to_string(coords[0])+"&"+
               std::to_string(coords[1])+"&"+std::to_string(coords[2]);
    }
};

[[nodiscard]] inline std::optional<MarketplaceHop>
parse_marketplace_hop(std::string_view url)
{
    constexpr std::string_view host=
        "hop://hg.stadt-nexverse.de:80/";
    constexpr std::string_view host_noport=
        "hop://hg.stadt-nexverse.de/";
    if(url.size()>512||url.empty())return std::nullopt;
    if(url.starts_with(host))url.remove_prefix(host.size());
    else if(url.starts_with(host_noport))
        url.remove_prefix(host_noport.size());
    else return std::nullopt;

    MarketplaceHop target{};
    const auto slash=url.find('/');
    if(slash==std::string_view::npos||slash==0||
       slash>160)return std::nullopt;
    const auto encoded=url.substr(0,slash);
    for(std::size_t i=0;i<encoded.size();++i)
    {
        char ch=encoded[i];
        if(ch=='%'&&i+2<encoded.size()&&
           encoded[i+1]=='2'&&encoded[i+2]=='0')
        {
            target.region+=' ';
            i+=2;
        }
        else if((ch>='A'&&ch<='Z')||
                (ch>='a'&&ch<='z')||
                (ch>='0'&&ch<='9')||ch=='_'||ch=='-'||ch=='.')
            target.region+=ch;
        else return std::nullopt;
    }
    if(target.region.empty()||target.region.size()>128||
       target.region=="."||target.region==".."||
       target.region.front()==' '||target.region.back()==' ')
        return std::nullopt;

    url.remove_prefix(slash+1);
    for(std::size_t i=0;i<3;++i)
    {
        const auto pos=url.find('/');
        if((i<2&&pos==std::string_view::npos)||
           (i==2&&pos!=std::string_view::npos))
            return std::nullopt;
        const auto digits=i<2?url.substr(0,pos):url;
        if(digits.empty()||digits.size()>4)return std::nullopt;
        for(char ch:digits)
            if(ch<'0'||ch>'9')return std::nullopt;
        unsigned number=0;
        const auto outcome=std::from_chars(
            digits.data(),digits.data()+digits.size(),number);
        if(outcome.ec!=std::errc{}||
           (i<2&&number>255)||(i==2&&number>4096))
            return std::nullopt;
        target.coords[i]=number;
        if(i<2)url.remove_prefix(pos+1);
    }
    return target;
}
}
