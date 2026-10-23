/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include "nexverse/assets/firestorm_auto_cof_read.hpp"
#include "nexverse/assets/firestorm_cof_shape_morph_preview.hpp"
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
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
       rig->attachment_bind_offsets[1U][2U]<1.0f ||
       rig->attachment_joint_plus_one[1U]==0U ||
       rig->attachment_joint_plus_one[31U]!=0U ||
       rig->attachment_joint_plus_one[40U]!=0U)
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
    // Real alpha.1094 original Firestorm MorphTarget delta records.
    // Read original immutable Head/Upper/Lower and sparse remaps, not
    // fabricated body scaling. A COF visual param is NOT automatically
    // a server-verified shape until the specific shape asset is validated.
    const auto head=load_firestorm_avatar_llm(*dir/"avatar_head.llm");
    const auto upper=load_firestorm_avatar_llm(*dir/"avatar_upper_body.llm");
    const auto lower=load_firestorm_avatar_llm(*dir/"avatar_lower_body.llm");
    const auto lash=load_firestorm_avatar_llm(*dir/"avatar_eyelashes.llm");
    if(!head||!upper||!lower||!lash||
       head->morph_targets.size()!=95U||
       upper->morph_targets.size()!=30U||
       lower->morph_targets.size()!=24U||
       lash->morph_targets.size()!=34U)return 30;
    const auto shape=morph_firestorm_avatar_mesh(
        *head,{{"Elongate_Head",1.0f}});
    const auto unchanged=morph_firestorm_avatar_mesh(
        *head,{{"Elongate_Head",0.0f}});
    const auto reversed=morph_firestorm_avatar_mesh(
        *head,{{"Elongate_Head",-1.0f}});
    if(!shape||!unchanged||!reversed||
       unchanged->vertices!=head->vertices||
       shape->indices!=head->indices||
       shape->skin_weights!=head->skin_weights||
       shape->joint_names!=head->joint_names||
       shape->vertices==head->vertices||
       reversed->vertices==head->vertices||
       head->morph_targets.size()!=95U)return 31;
    std::size_t altered=0U;
    for(std::size_t i=0U;i<head->vertices.size();i+=8U)
        if(shape->vertices[i]!=head->vertices[i]||
           shape->vertices[i+1U]!=head->vertices[i+1U]||
           shape->vertices[i+2U]!=head->vertices[i+2U])
            ++altered;
    if(altered<100U)return 32;
    if(morph_firestorm_avatar_mesh(*head,{{"UNTRUSTED_MORPH",1.0f}})||
       morph_firestorm_avatar_mesh(*head,{{"Elongate_Head",5.0f}})||
       morph_firestorm_avatar_mesh(*head,{{"Elongate_Head",
                                          std::numeric_limits<float>::infinity()}})||
       morph_firestorm_avatar_mesh(*head,{{"Elongate_Head",0.3f},
                                        {"Elongate_Head",0.4f}}))
        return 33;
    // Corrupt a single original sparse vertex index in a COPY:
    // never silently read outside immutable Firestorm GPU mesh.
    {
        const auto file=*dir/"avatar_head.llm";
        std::ifstream in(file,std::ios::binary);
        std::vector<std::uint8_t> raw{
           std::istreambuf_iterator<char>{in},
           std::istreambuf_iterator<char>{}};
        const std::string token="Elongate_Head";
        const auto it=std::search(raw.begin(),raw.end(),
                                  token.begin(),token.end());
        if(it==raw.end())return 34;
        const auto at=static_cast<std::size_t>(
            std::distance(raw.begin(),it))+64U;
        if(at+8U>raw.size())return 35;
        raw[at]=255U;raw[at+1U]=255U;
        raw[at+2U]=255U;raw[at+3U]=127U;
        if(decode_firestorm_avatar_llm(raw))return 36;
        raw.resize(at+10U);
        if(decode_firestorm_avatar_llm(raw))return 37;
    }
    // Alpha1104 verified Firestorm avatar_lad.xml multi-Shape mapping.
    // Same OWN COF Shape controls three Head morphs and FOUR
    // eyelash-original morphs, not a fabricated cross-avatar profile.
    const auto selected=firestorm_cof_shape_morph_preview({
        {770,0.8f},{186,-0.65f},{686,1.1f},{518,0.9f}});
    if(!selected||selected->head.size()!=3U||
       selected->eyelashes.size()!=4U||!selected->active())
        return 38;
    const auto compound_head=morph_firestorm_avatar_mesh(
        *head,selected->head);
    const auto compound_lash=morph_firestorm_avatar_mesh(
        *lash,selected->eyelashes);
    if(!compound_head||!compound_lash||
       compound_head->vertices==shape->vertices||
       compound_head->vertices==head->vertices||
       compound_lash->vertices==lash->vertices||
       compound_head->indices!=head->indices||
       compound_lash->skin_weights!=lash->skin_weights)
        return 39;
    if(firestorm_cof_shape_morph_preview({{686,2.01f}})||
       firestorm_cof_shape_morph_preview({{186,-1.31f}})||
       firestorm_cof_shape_morph_preview({{518,1.51f}})||
       firestorm_cof_shape_morph_preview({
           {770,std::numeric_limits<float>::quiet_NaN()}}))
        return 40;
    const auto reset=firestorm_cof_shape_morph_preview({
        {770,0.0f},{186,0.0f},{686,0.0f},{518,0.0f}});
    if(!reset||reset->active())return 41;
    // Alpha1114: exact original LAD Upper/Lower shape IDs and boundaries.
    const auto body_shape=firestorm_cof_shape_morph_preview({
        {104,0.9f},{106,1.1f},{151,0.8f},{152,1.2f}});
    if(!body_shape||body_shape->upper.size()!=2U||
       body_shape->lower.size()!=2U||
       !body_shape->head.empty()||!body_shape->eyelashes.empty())
        return 42;
    const auto upper_shape=morph_firestorm_avatar_mesh(
        *upper,body_shape->upper);
    const auto lower_shape=morph_firestorm_avatar_mesh(
        *lower,body_shape->lower);
    if(!upper_shape||!lower_shape||
       upper_shape->vertices==upper->vertices||
       lower_shape->vertices==lower->vertices||
       upper_shape->skin_weights!=upper->skin_weights||
       lower_shape->indices!=lower->indices||
       upper_shape->joint_names!=upper->joint_names)
        return 43;
    if(firestorm_cof_shape_morph_preview({{104,1.01f}})||
       firestorm_cof_shape_morph_preview({{106,1.41f}})||
       firestorm_cof_shape_morph_preview({{151,-0.01f}})||
       firestorm_cof_shape_morph_preview({{152,1.51f}}))
        return 44;
    // Alpha.1124 dynamic ORIGINAL LAD+LLM catalog (no permanent
    // 8-param whitelist). These extra two torso/legs IDs are only
    // accepted when both original XML param_morph AND the actual
    // matching LLM MorphTarget name agree. Old helper knows only 8.
    const auto dynamic=firestorm_cof_shape_morph_from_original_lad(
        *dir,{{770,0.65f},{186,-0.4f},{686,0.9f},{518,0.7f},
              {104,0.8f},{106,0.9f},{634,0.7f},
              {151,0.65f},{152,1.1f},{635,0.8f}});
    if(!dynamic||dynamic->head.size()!=3U||
       dynamic->eyelashes.size()!=4U||
       dynamic->upper.size()!=3U||
       dynamic->lower.size()!=3U)return 45;
    const auto dyn_upper=morph_firestorm_avatar_mesh(
        *upper,dynamic->upper);
    const auto dyn_lower=morph_firestorm_avatar_mesh(
        *lower,dynamic->lower);
    if(!dyn_upper||!dyn_lower||
       dyn_upper->vertices==upper->vertices||
       dyn_lower->vertices==lower->vertices)return 46;
    if(firestorm_cof_shape_morph_from_original_lad(
           *dir,{{634,1.01f}})||
       firestorm_cof_shape_morph_from_original_lad(
           *dir,{{635,1.01f}})||
       firestorm_cof_shape_morph_from_original_lad(
           *dir,{{634,std::numeric_limits<float>::infinity()}}))
        return 47;
    const auto absent=firestorm_cof_shape_morph_from_original_lad(
        *dir,{{9999,0.5f}});
    if(!absent||absent->active())return 48;
    // Reject corrupt or missing character directory, not an
    // unverified dictionary or fabricated default morph.
    if(firestorm_cof_shape_morph_from_original_lad(
           *dir/"missing-character-assets",{{104,0.8f}}))
        return 49;
    // Alpha1134 original Firestorm DRIVER-param chains. Visible
    // shape IDs 2 and 35 aren't mesh morphs; original LAD maps them
    // via driver_parameters -> IDs 20002/20035 in headMesh, while
    // skeleton IDs 30002/30035 still require separate skeleton code.
    const auto driven=firestorm_cof_shape_morph_from_original_lad(
        *dir,{{2,1.1f},{35,1.4f}});
    if(!driven||driven->head.size()!=2U||
       !driven->upper.empty()||!driven->lower.empty()||
       !driven->eyelashes.empty()||!driven->active()||
       driven->driver_morph_links!=2U||
       !driven->skeletal_driver_pending)
        return 50;
    const auto driver_head=morph_firestorm_avatar_mesh(
        *head,driven->head);
    if(!driver_head||driver_head->vertices==head->vertices)
        return 51;
    if(firestorm_cof_shape_morph_from_original_lad(
            *dir,{{35,2.01f}})||
       firestorm_cof_shape_morph_from_original_lad(
            *dir,{{2,-0.81f}})||
       firestorm_cof_shape_morph_from_original_lad(
            *dir,{{35,std::numeric_limits<float>::quiet_NaN()}}))
        return 52;
    // A driver and explicit direct target cannot silently apply
    // conflicting weights to the same original named LLM morph.
    const auto compatible=firestorm_cof_shape_morph_from_original_lad(
        *dir,{{2,0.9f},{20002,0.9f}});
    if(!compatible||compatible->head.size()!=1U)
        return 53;
    // Alpha.1144 automatic login COF bootstrap: exactly once for
    // the same authenticated owner+lease+COF-version; a manual F9
    // reread remains independent. Invalid scope never arms the gate.
    FirestormAutoCofRead auto_read;
    if(auto_read.begin("",1U,"cof-a",10U)||
       auto_read.begin("owner",0U,"cof-a",10U)||
       auto_read.begin("owner",1U,"",10U)||
       auto_read.begin("owner",1U,"cof-a",0U))
        return 60;
    if(!auto_read.begin("owner",1U,"cof-a",10U)||
       auto_read.begin("owner",1U,"cof-a",10U)||
       !auto_read.matches("owner",1U,"cof-a",10U)||
       auto_read.matches("owner-b",1U,"cof-a",10U)||
       !auto_read.begin("owner",1U,"cof-a",11U)||
       !auto_read.begin("owner",2U,"cof-a",11U)||
       !auto_read.begin("owner-b",2U,"cof-a",11U)||
       auto_read.begin("owner-b",2U,"cof-a",11U)||
       // Revisiting an old COF must not silently bypass the
       // 'one auto read per exact owner/lease/version' guarantee.
       auto_read.begin("owner",1U,"cof-a",10U))
        return 61;
    auto_read.reset();
    if(!auto_read.begin("owner",1U,"cof-a",10U))return 62;
    std::cout<<"AUTO_COF_AUTHENTICATED_ONCE_PER_OWNER_LEASE_VERSION_PASS\n";
    std::cout<<"ORIGINAL_FIRESTORM_DRIVER_HEAD_MORPHS="
             <<driven->head.size()
             <<"; SKELETON_DRIVER_REMAINS_OPEN=1\n";
    std::cout<<"FIRESTORM_ORIGINAL_LAD_DYNAMIC_MORPHS_HEAD="
             <<dynamic->head.size()<<"; LASH="
             <<dynamic->eyelashes.size()<<"; UPPER="
             <<dynamic->upper.size()<<"; LOWER="
             <<dynamic->lower.size()<<"\n";
    std::cout<<"FIRESTORM_ORIGINAL_COF_BODY_MORPHS_UPPER="
             <<body_shape->upper.size()
             <<"; LOWER="<<body_shape->lower.size()<<"\n";
    std::cout<<"ORIGINAL_FIRESTORM_MULTI_COF_SHAPE_HEAD_MORPHS="
             <<selected->head.size()<<"; LASH_MORPHS="
             <<selected->eyelashes.size()<<"\n";
    std::cout<<"FIRESTORM_REAL_MORPH_TARGETS_HEAD="
             <<head->morph_targets.size()
             <<"; UPPER="<<upper->morph_targets.size()
             <<"; LOWER="<<lower->morph_targets.size()
             <<"; LASH="<<lash->morph_targets.size()
             <<"; DEFORMED_HEAD_VERTICES="<<altered<<"\n";
    if(vertices<2000U || faces<4500U)return 12;
    std::cout<<"Firestorm real avatar mesh parsed: vertices="<<vertices
             <<" triangles="<<faces<<"\n";
    return 0;
}
