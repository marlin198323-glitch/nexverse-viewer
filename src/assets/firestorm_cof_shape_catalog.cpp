/*
 * MPL-2.0. Original Firestorm locally installed character data reader.
 * Only explicit name/id morph links in avatar_lad.xml are admitted,
 * intersected with actual morph names parsed from the matching .llm.
 * Identity-compatible original param_driver -> param_morph links
 * are supported; piecewise drivers, skeleton/Bone morphs and wearable
 * material composition remain incomplete. This cannot prove final
 * simulator-authenticated avatar appearance.
 */
#include "nexverse/assets/firestorm_cof_shape_morph_preview.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <tinyxml2.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <map>
#include <string_view>
#include <system_error>

namespace nexverse::assets
{
std::optional<FirestormCofMorphPreview>
firestorm_cof_shape_morph_from_original_lad(
    const std::filesystem::path& directory,
    const std::map<std::int32_t,float>& parameters)
{
    std::error_code ec;
    const auto lad=directory/"avatar_lad.xml";
    const auto size=std::filesystem::file_size(lad,ec);
    if(ec||size<80U||size>1024U*1024U||parameters.size()>512U)
        return std::nullopt;
    tinyxml2::XMLDocument xml;
    if(xml.LoadFile(lad.string().c_str())!=tinyxml2::XML_SUCCESS)
        return std::nullopt;
    auto* root=xml.FirstChildElement("linden_avatar");
    if(!root)return std::nullopt;

    FirestormCofMorphPreview selected;
    struct Source
    {
        const char* filename;
        std::vector<std::pair<std::string,float>>
            FirestormCofMorphPreview::* destination;
    };
    constexpr std::array<Source,4U> sources{{
        {"avatar_head.llm",&FirestormCofMorphPreview::head},
        {"avatar_eyelashes.llm",&FirestormCofMorphPreview::eyelashes},
        {"avatar_upper_body.llm",&FirestormCofMorphPreview::upper},
        {"avatar_lower_body.llm",&FirestormCofMorphPreview::lower}
    }};
    for(const auto& source:sources)
    {
        // Reads the ORIGINAL locally installed Firestorm mesh's parsed
        // bounded MorphTargets. XML may never introduce a new geometry,
        // path, alias or executable morph name.
        const auto mesh=load_firestorm_avatar_llm(
            directory/source.filename);
        if(!mesh)return std::nullopt;
        auto& output=selected.*(source.destination);
        for(auto* entry=root->FirstChildElement("mesh");entry;
            entry=entry->NextSiblingElement("mesh"))
        {
            const char* file=entry->Attribute("file_name");
            if(!file||std::string_view(file)!=source.filename)
                continue;
            for(auto* param=entry->FirstChildElement("param");param;
                param=param->NextSiblingElement("param"))
            {
                const char* wearable=param->Attribute("wearable");
                const char* name=param->Attribute("name");
                if(!wearable||std::string_view(wearable)!="shape"||
                   !name||!param->FirstChildElement("param_morph"))
                    continue;
                // Exact name must exist in the matching original LLM.
                if(std::none_of(mesh->morph_targets.begin(),
                    mesh->morph_targets.end(),
                    [&](const auto& morph){return morph.name==name;}))
                    continue;
                int id=-1;
                if(param->QueryIntAttribute("id",&id)!=tinyxml2::XML_SUCCESS||
                   id<0||id>65535)return std::nullopt;
                const auto found=parameters.find(id);
                if(found==parameters.end())continue;
                float min=0.0f,max=0.0f;
                if(param->QueryFloatAttribute("value_min",&min)!=
                       tinyxml2::XML_SUCCESS||
                   param->QueryFloatAttribute("value_max",&max)!=
                       tinyxml2::XML_SUCCESS||
                   !std::isfinite(min)||!std::isfinite(max)||
                   min>max||min< -3.0f||max>3.0f)
                    return std::nullopt;
                const float weight=found->second;
                if(!std::isfinite(weight)||weight<min||weight>max)
                    return std::nullopt;
                if(std::abs(weight)<0.0001f)continue;
                const auto duplicate=std::find_if(
                    output.begin(),output.end(),[&](const auto& old)
                    {return old.first==name;});
                if(duplicate!=output.end())
                {
                    // Firestorm LAD sometimes has shared param IDs.
                    // Equal repeated sources are safe; conflicting
                    // shape weights must fail closed, never double apply.
                    if(std::abs(duplicate->second-weight)>0.0001f)
                        return std::nullopt;
                    continue;
                }
                if(output.size()>=96U)return std::nullopt;
                output.emplace_back(name,weight);
            }
        }
        // Firestorm driver_parameters contains the user-facing Shape
        // slider. A driver <driven id=...> may name a mesh param_morph
        // AND a param_skeleton: apply ONLY directly identity-mapped
        // morph records here. Bone/skeleton contributions deliberately
        // remain unimplemented, not fabricated or counted as skin proof.
        const auto* driver_root=root->FirstChildElement(
            "driver_parameters");
        if(!driver_root)continue;
        for(auto* parent=driver_root->FirstChildElement("param");parent;
            parent=parent->NextSiblingElement("param"))
        {
            const char* wearable=parent->Attribute("wearable");
            const auto* driver=parent->FirstChildElement("param_driver");
            if(!wearable||std::string_view(wearable)!="shape"||!driver)
                continue;
            int driver_id=-1;
            if(parent->QueryIntAttribute("id",&driver_id)!=
               tinyxml2::XML_SUCCESS||driver_id<0||driver_id>65535)
                return std::nullopt;
            const auto found=parameters.find(driver_id);
            if(found==parameters.end())continue;
            float dmin=0.0f,dmax=0.0f;
            if(parent->QueryFloatAttribute("value_min",&dmin)!=
                   tinyxml2::XML_SUCCESS||
               parent->QueryFloatAttribute("value_max",&dmax)!=
                   tinyxml2::XML_SUCCESS||
               !std::isfinite(dmin)||!std::isfinite(dmax)||
               dmin>dmax||dmin< -3.0f||dmax>3.0f)
                return std::nullopt;
            const float weight=found->second;
            if(!std::isfinite(weight)||weight<dmin||weight>dmax)
                return std::nullopt;
            if(std::abs(weight)<0.0001f)continue;
            std::size_t links=0U;
            for(auto* link=driver->FirstChildElement("driven");link;
                link=link->NextSiblingElement("driven"))
            {
                if(++links>32U)return std::nullopt;
                // Firestorm supports shaped interpolation through
                // min1/max1/min2/max2. That transfer function is NOT
                // identity; postpone until implemented exactly.
                if(link->Attribute("min1")||link->Attribute("max1")||
                   link->Attribute("min2")||link->Attribute("max2"))
                    continue;
                int driven_id=-1;
                if(link->QueryIntAttribute("id",&driven_id)!=
                   tinyxml2::XML_SUCCESS||
                   driven_id<0||driven_id>65535)
                    return std::nullopt;
                // Note actual Firestorm param_skeleton driver peers
                // separately: this GPU-Morph-only implementation must
                // not pretend to deform bones/joints as Firestorm does.
                for(auto* skeleton=root->FirstChildElement("skeleton");
                    skeleton;skeleton=skeleton->NextSiblingElement("skeleton"))
                    for(auto* sk=skeleton->FirstChildElement("param");sk;
                        sk=sk->NextSiblingElement("param"))
                    {
                        int sid=-1;
                        if(sk->QueryIntAttribute("id",&sid)==
                               tinyxml2::XML_SUCCESS&&
                           sid==driven_id&&
                           sk->FirstChildElement("param_skeleton"))
                            selected.skeletal_driver_pending=true;
                    }
                for(auto* entry=root->FirstChildElement("mesh");entry;
                    entry=entry->NextSiblingElement("mesh"))
                {
                    const char* file=entry->Attribute("file_name");
                    if(!file||std::string_view(file)!=source.filename)
                        continue;
                    for(auto* param=entry->FirstChildElement("param");param;
                        param=param->NextSiblingElement("param"))
                    {
                        if(!param->FirstChildElement("param_morph"))
                            continue;
                        int target_id=-1;
                        if(param->QueryIntAttribute("id",&target_id)!=
                               tinyxml2::XML_SUCCESS||
                           target_id!=driven_id)
                            continue;
                        const char* name=param->Attribute("name");
                        if(!name||std::none_of(
                            mesh->morph_targets.begin(),
                            mesh->morph_targets.end(),
                            [&](const auto& morph)
                            {return morph.name==name;}))
                            continue;
                        float tmin=0.0f,tmax=0.0f;
                        if(param->QueryFloatAttribute("value_min",&tmin)!=
                               tinyxml2::XML_SUCCESS||
                           param->QueryFloatAttribute("value_max",&tmax)!=
                               tinyxml2::XML_SUCCESS||
                           !std::isfinite(tmin)||!std::isfinite(tmax)||
                           std::abs(tmin-dmin)>0.0001f||
                           std::abs(tmax-dmax)>0.0001f)
                            continue; // Nonidentity driver, not supported.
                        const auto duplicate=std::find_if(
                            output.begin(),output.end(),
                            [&](const auto& previous)
                            {return previous.first==name;});
                        if(duplicate!=output.end())
                        {
                            if(std::abs(duplicate->second-weight)>0.0001f)
                                return std::nullopt;
                            continue;
                        }
                        if(output.size()>=96U)return std::nullopt;
                        output.emplace_back(name,weight);
                        ++selected.driver_morph_links;
                    }
                }
            }
        }
    }
    return selected;
}
}
