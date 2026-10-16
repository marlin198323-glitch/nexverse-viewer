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
    // The remaining optional MorphTarget records are deliberately not read:
    // the original triangle indices, UVs and skin-joint weights suffice
    // for the non-morphed bind-pose mesh. Never execute morph names as paths.
    return out.valid()?std::optional<FirestormAvatarMesh>{std::move(out)}
        :std::nullopt;
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
