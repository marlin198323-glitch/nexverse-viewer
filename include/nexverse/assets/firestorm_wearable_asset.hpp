/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/lludp.hpp"
#include <algorithm>
#include <array>
#include <locale>
#include <utility>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::assets
{
// Independent bounded interpretation of Firestorm_7.2.4
// indra/llappearance/llwearable.cpp importStream/exportStream.
// Parsing is diagnostic only; no unverified textures are uploaded to GPU.
inline constexpr std::size_t max_firestorm_wearable_bytes=2U*1024U*1024U;
inline constexpr std::size_t max_firestorm_wearable_line=2047U;
inline constexpr std::size_t max_firestorm_visual_params=1000U;
inline constexpr std::size_t max_firestorm_local_textures=100U;
// Firestorm_7.2.4 llavatarappearancedefines.h:
 // TEX_NUM_INDICES is 45; indices 8,9,10,11,19,20,40..44 are
// already-composited outputs, NOT input layers to claim as GPU bakes.
inline constexpr std::size_t firestorm_texture_indices=45U;
inline constexpr std::array<unsigned,11> firestorm_baked_texture_entry_indices{
    8U,9U,10U,11U,19U,20U,40U,41U,42U,43U,44U
};
// Original LLAvatarAppearanceDictionary::BakedTextures mapping.
// -1 is a baked output or otherwise not a recognized local layer.
// These are only INPUT references, not decoded pixels.
inline constexpr auto firestorm_local_bake_groups=[] {
    std::array<int,45U> groups{};
    groups.fill(-1);
    for(unsigned index:{0U,26U,23U,29U})groups[index]=0; // head
    for(unsigned index:{1U,5U,13U,15U,16U,27U,22U,30U})
        groups[index]=1; // upper
    for(unsigned index:{2U,6U,7U,12U,14U,17U,28U,21U,31U})
        groups[index]=2; // lower
    for(unsigned index:{3U,34U,24U})groups[index]=3; // eyes
    for(unsigned index:{18U,32U})groups[index]=4; // skirt
    for(unsigned index:{4U,33U,25U})groups[index]=5; // hair
    groups[35U]=6;groups[36U]=7; // left arm/leg
    groups[37U]=8;groups[38U]=9;groups[39U]=10; // aux 1-3
    return groups;
}();
struct FirestormWearableAsset
{
    int version=0,type=-1;
    std::string name,description;
    std::map<std::int32_t,float> visual_params;
    std::map<unsigned,std::string> textures;
};
struct FirestormWearableParse
{
    std::optional<FirestormWearableAsset> asset;
    std::string error;
    [[nodiscard]] bool ok() const noexcept{return asset.has_value();}
};
[[nodiscard]] inline FirestormWearableParse parse_firestorm_wearable(
    std::string_view input,std::optional<unsigned> expected_type={})
{
    const auto fail=[](std::string_view msg)->FirestormWearableParse{
        return {std::nullopt,std::string{msg}};
    };
    if(input.empty()||input.size()>max_firestorm_wearable_bytes||
       input.find('\0')!=std::string_view::npos)
        return fail("Wearable asset empty/oversized/binary");
    std::vector<std::string_view> lines;
    std::size_t first=0U;
    while(first<input.size())
    {
        auto end=input.find('\n',first);
        if(end==std::string_view::npos)end=input.size();
        auto line=input.substr(first,end-first);
        if(!line.empty()&&line.back()=='\r')line.remove_suffix(1U);
        if(line.size()>max_firestorm_wearable_line||
           lines.size()>=8192U)return fail("Wearable line/count bound");
        lines.push_back(line);
        first=end+1U;
    }
    if(lines.size()<7U)return fail("Truncated Firestorm wearable asset");
    const auto integer=[](std::string_view input)->std::optional<int> {
        if(input.empty())return std::nullopt;
        int v{};
        const auto [end,ec]=std::from_chars(
            input.data(),input.data()+input.size(),v);
        if(ec!=std::errc{}||end!=input.data()+input.size())
            return std::nullopt;
        return v;
    };
    const auto keyint=[&](std::string_view line,std::string_view key)
        ->std::optional<int> {
        if(!line.starts_with(key)||line.size()<=key.size()||
           line[key.size()]!=' ')return std::nullopt;
        return integer(line.substr(key.size()+1U));
    };
    FirestormWearableAsset result;
    const auto ver=keyint(lines[0],"LLWearable version");
    // Current definition version = 1. Legacy version 24 is imported
    // only by a special Firestorm migration path, not by this safe parser.
    if(!ver||*ver!=1)return fail("Unsupported Firestorm wearable version");
    result.version=*ver;result.name=std::string{lines[1]};
    result.description=std::string{lines[2]};
    if(result.name.size()>255U||result.description.size()>1024U)
        return fail("Wearable name/description length");
    // Legacy permissions and sale_info contain variable-length, additional
    // owner/UUID records; never interpret their numbers as visual parameters.
    std::size_t at=3U;
    const auto seek=[&](std::string_view prefix,std::size_t max_steps)
        ->bool{
        std::size_t walked=0U;
        while(at<lines.size()&&walked++<max_steps)
        {
            if(lines[at].starts_with(prefix))return true;
            ++at;
        }
        return false;
    };
    if(!seek("permissions ",8U)||!keyint(lines[at],"permissions")||
       *keyint(lines[at],"permissions")!=0)return fail("Invalid permissions section");
    ++at;
    if(!seek("sale_info ",36U)||!keyint(lines[at],"sale_info")||
       *keyint(lines[at],"sale_info")!=0)return fail("Invalid sale_info section");
    ++at;
    if(!seek("type ",16U))return fail("Missing wearable type");
    const auto type=keyint(lines[at++],"type");
    if(!type||*type<0||*type>=17||
       (expected_type&&static_cast<unsigned>(*type)!=*expected_type))
        return fail("Wearable type mismatches original inventory flags");
    result.type=*type;
    if(at>=lines.size())return fail("Missing parameters header");
    const auto params=keyint(lines[at++],"parameters");
    if(!params||*params<0||*params>1000)return fail("Invalid parameter count");
    for(int i=0;i<*params;++i)
    {
        if(at>=lines.size())return fail("Truncated wearable visual params");
        const auto line=lines[at++];
        const auto split=line.find(' ');
        if(split==std::string_view::npos)return fail("Invalid visual parameter");
        const auto id=integer(line.substr(0,split));
        const auto raw=line.substr(split+1U);
        if(!id||*id<0||*id>200000||raw.empty()||raw.size()>48U)
            return fail("Invalid visual parameter id/value");
        std::string value_str{raw};
        std::istringstream stream{value_str};
        stream.imbue(std::locale::classic());
        float value{};
        std::string extra;
        if(!(stream>>value)||(stream>>extra)||!std::isfinite(value)||
           std::abs(value)>1000.0f||
           !result.visual_params.emplace(*id,value).second)
            return fail("Nonfinite/duplicate/malformed wearable param");
    }
    if(at>=lines.size())return fail("Missing textures header");
    const auto tex=keyint(lines[at++],"textures");
    if(!tex||*tex<0||*tex>100)return fail("Invalid local texture count");
    const auto nonzero=[](const auto& bytes){
        return bytes&&std::any_of(bytes->begin(),bytes->end(),
            [](std::uint8_t b){return b!=0U;});
    };
    for(int i=0;i<*tex;++i)
    {
        if(at>=lines.size())return fail("Truncated wearable textures");
        const auto line=lines[at++];
        const auto split=line.find(' ');
        if(split==std::string_view::npos)return fail("Malformed wearable texture");
        const auto id=integer(line.substr(0,split));
        const auto uuid=line.substr(split+1U);
        if(!id||*id<0||static_cast<unsigned>(*id)>=
               firestorm_texture_indices||
           !nonzero(nexverse::protocol::uuid_to_bytes(uuid))||
           !result.textures.emplace(static_cast<unsigned>(*id),
                                    std::string{uuid}).second)
            return fail("Invalid/duplicate wearable texture");
    }
    // Reject extra nonempty trailing payload rather than accepting
    // unrecognized format extensions or injected control lines.
    for(;at<lines.size();++at)
        if(!lines[at].empty())return fail("Trailing wearable data");
    return {std::move(result),{}};
}
struct FirestormWearableBakeAudit
{
    std::size_t params=0U,texture_refs=0U;
    std::array<bool,11> baked_layers{}; // local layers REFERENCED per bake
    std::size_t baked_output_refs=0U; // never assume these are actual bakes
    bool has_asset=false;
    bool bake_ready=false;
};
[[nodiscard]] inline FirestormWearableBakeAudit audit_firestorm_bake(
    const FirestormWearableAsset* item)
{
    FirestormWearableBakeAudit audit;
    if(!item)return audit;
    audit.has_asset=true;audit.params=item->visual_params.size();
    audit.texture_refs=item->textures.size();
    for(const auto& [index,uuid]:item->textures)
    {
        (void)uuid;
        if(index>=firestorm_local_bake_groups.size())continue;
        const auto group=firestorm_local_bake_groups[index];
        if(group>=0)
            audit.baked_layers[static_cast<std::size_t>(group)]=true;
        else
            ++audit.baked_output_refs;
    }
    // Local wearable texture references are not baked outputs and do NOT
    // prove the presence of decoded JPEG2000, masks, or layered composites.
    return audit;
}
}
