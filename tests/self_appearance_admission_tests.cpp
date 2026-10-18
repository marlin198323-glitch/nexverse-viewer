/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/self_appearance_admission.hpp"
#include "nexverse/ui/avatar_appearance_runtime.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::protocol;
    AvatarAppearanceMessage a{};
    a.avatar_id="11111111-2222-3333-4444-555555555555";
    a.texture_entry={2U,3U,4U};
    a.visual_params={10U,20U,30U};
    const auto id=a.avatar_id;
    if(assess_self_appearance(a,id,false)!=
        SelfAppearanceAdmission::region_not_ready)return 1;
    if(assess_self_appearance(a,"",true)!=
        SelfAppearanceAdmission::not_self)return 2;
    if(assess_self_appearance(a,"foreign",true)!=
        SelfAppearanceAdmission::not_self)return 3;
    if(!appearance_is_authoritative(
        assess_self_appearance(a,id,true)))return 4;
    if(assess_self_appearance(a,id,true)!=SelfAppearanceAdmission::accepted)return 12;
    a.trial=true;
    if(appearance_is_authoritative(assess_self_appearance(a,id,true))||
       assess_self_appearance(a,id,true)!=
        SelfAppearanceAdmission::trial_preview)return 5;
    a.trial=false;
    a.texture_entry.clear();
    if(assess_self_appearance(a,id,true)!=
        SelfAppearanceAdmission::texture_entry_missing)return 6;
    a.texture_entry={2U,3U,4U};
    a.visual_params.clear();
    if(assess_self_appearance(a,id,true)!=
        SelfAppearanceAdmission::visual_params_missing)return 7;
    a.visual_params={3U};
    if(!appearance_is_authoritative(assess_self_appearance(a,id,true)))
        return 8;
    nexverse::ui::AvatarAppearanceRuntime summary{};
    if(!nexverse::ui::describe_self_appearance_admission(
        summary,true).empty())return 9;
    summary.self_trial_ignored=2U;
    summary.self_incomplete_ignored=3U;
    summary.self_invalid_texture_ignored=4U;
    if(nexverse::ui::describe_self_appearance_admission(summary,true)
          .find("Skin erhalten")==std::string::npos)return 10;
    if(nexverse::ui::describe_self_appearance_admission(summary,true)
          .find("4 Texturblock ungueltig")==std::string::npos)return 13;
    if(nexverse::ui::describe_self_appearance_admission(summary,false)
          .find("Self weiter ausstehend")==std::string::npos)return 11;
    std::cout<<"Authenticated original body preserved against partial and trial SelfAvatarAppearance PASS\n";
    return 0;
}
