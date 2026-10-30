/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <algorithm>
#include <string>
#include <string_view>

namespace nexverse::ui
{
// The original compact footer discarded everything after 86 BYTES,
// including the middle of a multibyte German UTF-8 letter. Clip by
// visible pixel width and only on codepoint boundaries. Always reserve
// space for "..." so a clipped warning is visibly incomplete.
//
// The byte budget also bounds how much untrusted XML-RPC text the
// renderer may measure every frame; it is NOT used to parse secrets.
template <typename Measure>
[[nodiscard]] std::string fit_notice_to_width(
    std::string_view source,float max_width,Measure measure)
{
    constexpr std::string_view suffix{"..."};
    constexpr std::size_t max_bytes=512U;
    if(source.empty()||max_width<=0.0f)return {};
    if(measure(suffix)>max_width)return {};

    std::string out;
    out.reserve(std::min(source.size(),max_bytes));
    std::size_t offset=0U;
    while(offset<source.size()&&offset<max_bytes)
    {
        const unsigned char head=
            static_cast<unsigned char>(source[offset]);
        const std::size_t width=(head<0x80U)?1U:
            (head>=0xC2U&&head<=0xDFU)?2U:
            (head>=0xE0U&&head<=0xEFU)?3U:
            (head>=0xF0U&&head<=0xF4U)?4U:0U;
        if(width==0U||offset+width>source.size()||
           offset+width>max_bytes)break;
        bool valid=true;
        for(std::size_t i=1U;i<width;++i)
        {
            const auto next=
                static_cast<unsigned char>(source[offset+i]);
            if((next&0xC0U)!=0x80U){valid=false;break;}
        }
        if(!valid)break;
        const auto previous=out.size();
        out.append(source.substr(offset,width));
        offset+=width;
        if(measure(out)>max_width||
           (offset<source.size()&&measure(out+std::string{suffix})>max_width))
        {
            out.resize(previous);
            break;
        }
    }
    if(offset<source.size())
    {
        if(measure(out+std::string{suffix})<=max_width)
            out+=suffix;
        else
        {
            while(!out.empty()&&measure(out+std::string{suffix})>max_width)
            {
                out.pop_back();
                while(!out.empty()&&
                      (static_cast<unsigned char>(out.back())&0xC0U)==0x80U)
                    out.pop_back();
            }
            out+=suffix;
        }
    }
    return out;
}
}
