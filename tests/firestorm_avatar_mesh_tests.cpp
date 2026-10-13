/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    const auto dir=find_firestorm_avatar_character_directory();
    if(!dir)
    {
        std::cerr << "Firestorm avatar assets missing: run with NEXVERSE_AVATAR_ASSETS\n";
        return 77; // CTest skips on machines without installed Firestorm
    }
    const auto rig=load_firestorm_avatar_rig(*dir);
    if(!rig || rig->joints.size()!=133U ||
       rig->attachment_count!=55U ||
       !rig->attachment_available[1U] ||
       !rig->attachment_available[2U] ||
       !rig->attachment_available[55U] ||
       rig->attachment_available[0U] ||
       !rig->attachment_is_hud[31U] ||
       !rig->attachment_is_hud[38U] ||
       rig->attachment_is_hud[40U] ||
       rig->attachment_bind_offsets[1U][2U]<1.0f)
    {
        std::cerr<<"Official Firestorm skeleton/attachment rig parse failed\n";
        return 13;
    }
    std::size_t vertices=0U,faces=0U;
    for(const auto name:{"avatar_head.llm","avatar_upper_body.llm",
                         "avatar_lower_body.llm"})
    {
        const auto path=*dir/name;
        const auto mesh=load_firestorm_avatar_llm(path);
        if(!mesh || !mesh->valid() || !mesh->has_weights ||
           mesh->joint_names.empty())
        {
            std::cerr<<"Cannot parse original Firestorm "<<name<<"\n";
            return 4;
        }
        const auto palette=firestorm_joint_render_palette(*rig,
                                                           mesh->joint_names);
        const std::size_t expected=std::string_view(name)=="avatar_head.llm"
            ?3U:(std::string_view(name)=="avatar_upper_body.llm"?15U:9U);
        if(!palette||palette->size()!=expected)return 14;
        for(float weight:mesh->skin_weights)
            if(weight<0.0f ||
               weight>=static_cast<float>(palette->size()))
                return 15;
        // Firestorm inserts ancestor joints between mesh named joints:
        // Head indices must be mChest, mNeck, mHead.
        if(std::string_view(name)=="avatar_head.llm")
        {
            const auto& j=rig->joints;
            if(j[(*palette)[0]].name!="mChest" ||
               j[(*palette)[1]].name!="mNeck" ||
               j[(*palette)[2]].name!="mHead")return 16;
        }
        vertices+=mesh->vertices.size()/8U;
        faces+=mesh->indices.size()/3U;
        if(mesh->joint_names.size()>256U ||
           mesh->skin_weights.size()!=mesh->vertices.size()/8U)
            return 5;
        std::error_code ec;
        const auto length=std::filesystem::file_size(path,ec);
        std::vector<std::uint8_t> buffer(static_cast<std::size_t>(length));
        FILE* file=std::fopen(path.string().c_str(),"rb");
        if(!file)return 6;
        const auto read=std::fread(buffer.data(),1U,buffer.size(),file);
        std::fclose(file);
        if(read!=buffer.size())return 7;
        for(std::size_t i=0;i<buffer.size();i+=421U)
        {
            auto corrupt=buffer;
            corrupt[0U]=0U;
            if(decode_firestorm_avatar_llm(corrupt))return 8;
            break;
        }
        auto truncated=buffer;
        truncated.resize(83U);
        if(decode_firestorm_avatar_llm(truncated))return 9;
        auto bad_index=buffer;
        // Vertex count at file offset 63; forcing huge out-of-bounds count
        // must reject without allocating an enormous vector.
        bad_index[63U]=0xffU;
        bad_index[64U]=0xffU;
        if(decode_firestorm_avatar_llm(bad_index))return 10;
        // The input is immutable and file-decoder allocations are capped.
        if(decode_firestorm_avatar_llm({}).has_value())return 11;
    }
    if(vertices<2000U || faces<4500U)return 12;
    std::cout<<"Firestorm real avatar mesh parsed: vertices="<<vertices
             <<" triangles="<<faces<<"\n";
    return 0;
}
