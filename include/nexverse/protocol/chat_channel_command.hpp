/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>
namespace nexverse::protocol
{
struct ParsedLocalChat
{
    std::string message;
    std::int32_t channel=0;
    bool valid=false;
    bool explicit_channel=false;
};
[[nodiscard]] inline ParsedLocalChat parse_local_chat_command(
    std::string_view original)
{
    ParsedLocalChat result;
    const auto start=original.find_first_not_of(" \t\r\n");
    if(start==std::string_view::npos)return result;
    original.remove_prefix(start);
    const auto trailing=original.find_last_not_of(" \t\r\n");
    original=original.substr(0,trailing+1U);
    if(original.size()>1U&&original[0]=='/'&&
       ((original[1]>='0'&&original[1]<='9')||
         (original[1]=='-'&&original.size()>2U&&
          original[2]>='0'&&original[2]<='9')))
    {
        const auto space=original.find_first_of(" \t");
        if(space==std::string_view::npos)return result;
        const auto digits=original.substr(1U,space-1U);
        std::int32_t parsed=0;
        const auto decoded=std::from_chars(
            digits.data(),digits.data()+digits.size(),parsed);
        if(decoded.ec!=std::errc{}||
           decoded.ptr!=digits.data()+digits.size())
            return result;
        const auto body=original.find_first_not_of(" \t",space);
        if(body==std::string_view::npos)return result;
        result.message=std::string{original.substr(body)};
        result.channel=parsed;result.explicit_channel=true;
    }
    else result.message=std::string{original};
    result.valid=!result.message.empty()&&
                 result.message.size()<=500U;
    return result;
}
}
