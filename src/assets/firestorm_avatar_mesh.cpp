/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 * Linden Binary Mesh 1.0 reader based on the on-disk format implemented
 * by Firestorm LLPolyMeshSharedData::loadMesh (LGPL-2.1 source reference).
 * This implementation does not copy Firestorm parser code.
 */
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <limits>
#include <string_view>
namespace nexverse::assets
{
namespace
{
constexpr std::string_view signature="Linden Binary Mesh 1.0";
struct Reader
{
    std::span<const std::uint8_t> bytes;
    std::size_t cursor=0U;
    [[nodiscard]] bool can(std::size_t amount) const
    {return amount<=bytes.size()-cursor;}
    [[nodiscard]] bool u8(std::uint8_t& value)
    {
        if(!can(1U))return false;
        value=bytes[cursor++];
        return true;
    }
    [[nodiscard]] bool u16(std::uint16_t& value)
    {
        if(!can(2U))return false;
        value=static_cast<std::uint16_t>(
            bytes[cursor]|(static_cast<std::uint16_t>(bytes[cursor+1U])<<8U));
        cursor+=2U;
        return true;
    }
    [[nodiscard]] bool u32(std::uint32_t& value)
    {
        if(!can(4U))return false;
        value=static_cast<std::uint32_t>(bytes[cursor]) |
            (static_cast<std::uint32_t>(bytes[cursor+1U])<<8U) |
            (static_cast<std::uint32_t>(bytes[cursor+2U])<<16U) |
            (static_cast<std::uint32_t>(bytes[cursor+3U])<<24U);
        cursor+=4U;
        return true;
    }
    [[nodiscard]] bool f32(float& value)
    {
        if(!can(4U))return false;
        const auto raw=static_cast<std::uint32_t>(bytes[cursor])|
            (static_cast<std::uint32_t>(bytes[cursor+1U])<<8U)|
            (static_cast<std::uint32_t>(bytes[cursor+2U])<<16U)|
            (static_cast<std::uint32_t>(bytes[cursor+3U])<<24U);
        cursor+=4U;
        value=std::bit_cast<float>(raw);
        return std::isfinite(value);
    }
};
}
std::optional<FirestormAvatarMesh> decode_firestorm_avatar_llm(
    std::span<const std::uint8_t> data)
{
    constexpr std::size_t max_bytes=8U*1024U*1024U;
    if(data.size()<66U || data.size()>max_bytes ||
       !std::equal(signature.begin(),signature.end(),data.begin()))
        return std::nullopt;
    Reader r{data,24U};
    std::uint8_t has_weights=0U,details=0U,rotation_order=0U;
    if(!r.u8(has_weights)||!r.u8(details)||has_weights>1U||details>1U)
        return std::nullopt;
    FirestormAvatarMesh out;
    out.has_weights=has_weights!=0U;
    for(auto& value:out.origin)if(!r.f32(value))return std::nullopt;
    std::array<float,3U> angles{};
    for(auto& value:angles)if(!r.f32(value))return std::nullopt;
    if(!r.u8(rotation_order))return std::nullopt;
    for(auto& value:out.scale)
        if(!r.f32(value)||std::abs(value)>100.0f)
            return std::nullopt;
    std::uint16_t n=0U;
    if(!r.u16(n)||n<3U||n>20000U)
        return std::nullopt;
    // Each vertex field is columnar. Never trust file-reported allocations.
    std::vector<std::array<float,3U>> positions(n),normals(n);
    for(auto& vec:positions)
        for(auto& v:vec)if(!r.f32(v)||std::abs(v)>1e4f)
            return std::nullopt;
    for(auto& vec:normals)
        for(auto& v:vec)if(!r.f32(v)||std::abs(v)>1e4f)
            return std::nullopt;
    float ignored=0.0f;
    for(std::size_t i=0U;i<static_cast<std::size_t>(n)*3U;++i)
        if(!r.f32(ignored))return std::nullopt; // binormals
    std::vector<std::array<float,2U>> uvs(n);
    for(auto& uv:uvs)for(auto& v:uv)
        if(!r.f32(v)||std::abs(v)>100.0f)
            return std::nullopt;
    if(details!=0U)
        for(std::size_t i=0U;i<static_cast<std::size_t>(n)*2U;++i)
            if(!r.f32(ignored))return std::nullopt;
    if(out.has_weights)
    {
        out.skin_weights.resize(n);
        for(auto& v:out.skin_weights)
            if(!r.f32(v)||v<0.0f||v>256.0f)
                return std::nullopt;
    }
    out.vertices.reserve(static_cast<std::size_t>(n)*8U);
    for(std::size_t i=0U;i<n;++i)
    {
        out.vertices.insert(out.vertices.end(),
            positions[i].begin(),positions[i].end());
        out.vertices.insert(out.vertices.end(),
            normals[i].begin(),normals[i].end());
        out.vertices.insert(out.vertices.end(),
            uvs[i].begin(),uvs[i].end());
    }
    std::uint16_t faces=0U;
    if(!r.u16(faces)||faces==0U||!r.can(static_cast<std::size_t>(faces)*6U))
        return std::nullopt;
    out.indices.reserve(static_cast<std::size_t>(faces)*3U);
    for(std::size_t i=0U;i<static_cast<std::size_t>(faces)*3U;++i)
    {
        std::uint16_t index=0U;
        if(!r.u16(index)||index>=n)return std::nullopt;
        out.indices.push_back(index);
    }
    if(out.has_weights)
    {
        std::uint16_t joints=0U;
        if(!r.u16(joints)||joints==0U||joints>256U||
           !r.can(static_cast<std::size_t>(joints)*64U))
            return std::nullopt;
        for(std::size_t i=0U;i<joints;++i)
        {
            const auto name_start=r.cursor;
            const auto* raw=reinterpret_cast<const char*>(
                r.bytes.data()+name_start);
            std::size_t length=0U;
            while(length<64U && raw[length]!='\0')++length;
            if(length==0U||length==64U)return std::nullopt;
            out.joint_names.emplace_back(raw,length);
            r.cursor+=64U;
        }
        // Firestorm LLPolyMesh stores 1-based, sometimes sparse joint
        // indices (e.g. upper body indices up to 14 with 12 entries).
        // Validating against numJointNames would reject original meshes.
    }
    // Original Linden Binary Mesh 1.0 morph records:
    // char name[64], S32 vertex count, sparse records of
    // U32 index + vec3 coords/normals/binormals + vec2 UV (48 bytes).
    // Existing Firestorm's "End Morphs" tag is mandatory. Never trust
    // counts, names, offsets or sparse vertex indexes from disk.
    bool morph_end=false;
    constexpr std::size_t max_morph_targets=160U;
    std::size_t total_morphed_vertices=0U;
    constexpr std::size_t max_morph_vertices=60000U;
    for(std::size_t target=0U;target<=max_morph_targets;++target)
    {
        if(!r.can(64U))return std::nullopt;
        const auto first=r.cursor;
        std::size_t length=0U;
        while(length<64U&&data[first+length]!=0U)++length;
        if(length==0U||length==64U)return std::nullopt;
        const std::string name(reinterpret_cast<const char*>(
            data.data()+first),length);
        r.cursor+=64U;
        if(name=="End Morphs")
        {
            morph_end=true;
            break;
        }
        for(const unsigned char ch:name)
            if(!((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||
                 (ch>='0'&&ch<='9')||ch=='_'||ch=='.'||ch=='-'))
                return std::nullopt;
        if(target==max_morph_targets)return std::nullopt;
        for(const auto& prior:out.morph_targets)
            if(prior.name==name)return std::nullopt;
        std::uint32_t count=0U;
        if(!r.u32(count)||count==0U||count>n||
           total_morphed_vertices+count>max_morph_vertices||
           !r.can(static_cast<std::size_t>(count)*48U))
            return std::nullopt;
        total_morphed_vertices+=count;
        FirestormMorphTarget record;
        record.name=name;
        record.vertices.reserve(count);
        std::vector<bool> seen(n,false);
        for(std::uint32_t j=0U;j<count;++j)
        {
            FirestormMorphVertex entry;
            if(!r.u32(entry.index)||entry.index>=n||
               seen[entry.index])return std::nullopt;
            seen[entry.index]=true;
            for(float& v:entry.position_delta)
                if(!r.f32(v)||std::abs(v)>10.0f)
                    return std::nullopt;
            for(float& v:entry.normal_delta)
                if(!r.f32(v)||std::abs(v)>10000.0f)
                    return std::nullopt;
            // Binormal deltas currently unused by the legacy renderer;
            // still validate all bytes and bounds, never skip blindly.
            for(std::size_t k=0U;k<3U;++k)
                if(!r.f32(ignored)||std::abs(ignored)>10000.0f)
                    return std::nullopt;
            for(float& v:entry.uv_delta)
                if(!r.f32(v)||std::abs(v)>10.0f)
                    return std::nullopt;
            record.vertices.push_back(entry);
        }
        out.morph_targets.push_back(std::move(record));
    }
    if(!morph_end)return std::nullopt;
    // Optional U32 remap count + pairs from original Firestorm LLM.
    // Out-of-bounds or truncated sections are NOT accepted. Preserve
    // correct original base mesh + morph parsing without using remaps.
    if(r.can(4U))
    {
        std::uint32_t remaps=0U;
        if(!r.u32(remaps)||remaps>n||!r.can(
               static_cast<std::size_t>(remaps)*8U))
            return std::nullopt;
        for(std::uint32_t i=0U;i<remaps;++i)
        {
            std::uint32_t from=0U,to=0U;
            if(!r.u32(from)||!r.u32(to)||from>=n||to>=n)
                return std::nullopt;
        }
    }
    if(r.cursor!=data.size())return std::nullopt;
    return out.valid()?std::optional<FirestormAvatarMesh>{std::move(out)}
        :std::nullopt;
}
std::optional<FirestormAvatarMesh> morph_firestorm_avatar_mesh(
    const FirestormAvatarMesh& base,
    const std::vector<std::pair<std::string,float>>& weights)
{
    if(!base.valid()||weights.size()>96U)return std::nullopt;
    FirestormAvatarMesh result=base;
    // No accumulation into the input or other avatars sharing this .llm.
    for(std::size_t i=0U;i<weights.size();++i)
    {
        const auto& [name,value]=weights[i];
        if(name.empty()||!std::isfinite(value)||
           std::abs(value)>3.0f)return std::nullopt;
        for(std::size_t j=0U;j<i;++j)
            if(weights[j].first==name)return std::nullopt;
        const auto it=std::find_if(base.morph_targets.begin(),
            base.morph_targets.end(),
            [&](const auto& target){return target.name==name;});
        if(it==base.morph_targets.end())return std::nullopt;
        for(const auto& entry:it->vertices)
        {
            const std::size_t at=static_cast<std::size_t>(entry.index)*8U;
            if(at+8U>result.vertices.size())return std::nullopt;
            for(std::size_t k=0U;k<3U;++k)
            {
                result.vertices[at+k]+=
                    entry.position_delta[k]*value;
                result.vertices[at+k+3U]+=
                    entry.normal_delta[k]*value;
            }
            for(std::size_t k=0U;k<2U;++k)
                result.vertices[at+k+6U]+=
                    entry.uv_delta[k]*value;
        }
    }
    for(std::size_t i=0U;i<result.vertices.size();++i)
        if(!std::isfinite(result.vertices[i])||
           std::abs(result.vertices[i])>1e4f)return std::nullopt;
    return result;
}
std::optional<FirestormAvatarMesh> load_firestorm_avatar_llm(
    const std::filesystem::path& file)
{
    std::error_code ec;
    const auto length=std::filesystem::file_size(file,ec);
    if(ec||length<66U||length>8U*1024U*1024U)return std::nullopt;
    std::ifstream stream(file,std::ios::binary);
    if(!stream)return std::nullopt;
    std::vector<std::uint8_t> data{
        std::istreambuf_iterator<char>{stream},std::istreambuf_iterator<char>{}};
    return decode_firestorm_avatar_llm(data);
}
std::optional<std::filesystem::path>
find_firestorm_avatar_character_directory()
{
    std::vector<std::filesystem::path> roots;
    if(const char* explicit_root=std::getenv("NEXVERSE_AVATAR_ASSETS");
       explicit_root&&*explicit_root)roots.emplace_back(explicit_root);
    roots.emplace_back("assets/character");
    roots.emplace_back("share/opengenesislink/character");
    if(const char* home=std::getenv("HOME");home&&*home)
    {
        // Locally installed Firestorm assets; avoid bundling unknown-license
        // blobs until third-party asset redistribution is reviewed.
        roots.emplace_back(std::filesystem::path(home)/"firestorm"/"character");

        // Only inspect explicitly designated local cached character
        // extractions. Prefer newer cached Nightly roots if present.
        // Do not unpack archives automatically or ship third-party assets.
        const auto cache=std::filesystem::path(home)/
            ".cache"/"nexverse-viewer";
        {
            std::vector<std::filesystem::path> nightly_cache;
            std::error_code ec;
            std::filesystem::directory_iterator it{cache,ec};
            const std::filesystem::directory_iterator end;
            for(;!ec&&it!=end;it.increment(ec))
            {
                if(!it->is_directory(ec))continue;
                const auto name=it->path().filename().string();
                if(name.starts_with("firestorm-nightly-"))
                    nightly_cache.push_back(it->path());
            }
            std::sort(nightly_cache.begin(),nightly_cache.end(),
                std::greater<>{});
            roots.insert(roots.end(),nightly_cache.begin(),
                nightly_cache.end());
        }

        // Firestorm's unpacked Linux packages are often on Desktop or
        // Schreibtisch. Look only one directory deep under these known
        // user-owned locations; never crawl arbitrary home trees or
        // redistribute third-party assets with our binary.
        std::vector<std::filesystem::path> user_installs;
        for(const auto folder:{"Schreibtisch","Desktop","Downloads"})
        {
            const auto base=std::filesystem::path(home)/folder;
            std::error_code ec;
            std::filesystem::directory_iterator it{base,ec};
            const std::filesystem::directory_iterator end;
            for(;!ec&&it!=end;it.increment(ec))
            {
                if(!it->is_directory(ec))continue;
                const auto name=it->path().filename().string();
                if(name.starts_with("Phoenix-FirestormOS-")||
                   name.starts_with("Firestorm-"))
                    user_installs.push_back(it->path());
            }
        }
        // Nightly first when both extracted builds exist; stable names
        // break ties so selection remains deterministic across filesystems.
        std::sort(user_installs.begin(),user_installs.end(),
            [](const auto& lhs,const auto& rhs)
            {
                const bool left_nightly=lhs.filename().string().find(
                    "Nightly")!=std::string::npos;
                const bool right_nightly=rhs.filename().string().find(
                    "Nightly")!=std::string::npos;
                if(left_nightly!=right_nightly)return left_nightly;
                return lhs.generic_string()>rhs.generic_string();
            });
        roots.insert(roots.end(),user_installs.begin(),user_installs.end());
    }
    for(auto root:roots)
    {
        if(root.filename()!="character")root/="character";
        std::error_code ec;
        if(std::filesystem::is_regular_file(root/"avatar_head.llm",ec)&&
           std::filesystem::is_regular_file(root/"avatar_upper_body.llm",ec)&&
           std::filesystem::is_regular_file(root/"avatar_lower_body.llm",ec)&&
           std::filesystem::is_regular_file(root/"avatar_skeleton.xml",ec))
            return root;
    }
    return std::nullopt;
}
}
