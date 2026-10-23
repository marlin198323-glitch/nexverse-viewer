/*
 * MPL-2.0. Original Firestorm locally installed character data reader.
 * Only explicit name/id morph links in avatar_lad.xml are admitted,
 * intersected with actual morph names parsed from the matching .llm.
 * It intentionally does NOT implement driver, skeleton or wearables
 * composition and cannot prove a final server-baked appearance.
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
    }
    return selected;
}
}
