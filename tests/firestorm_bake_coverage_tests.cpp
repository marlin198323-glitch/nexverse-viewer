/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_bake_coverage.hpp"
#include <iostream>
using namespace nexverse::assets;
int main()
{
    auto missing=audit_firestorm_bake_sources({},{});
    if(missing.upload_authorized()||missing.decoded||missing.covered_regions)
        return 1;
    auto parsed=parse_firestorm_texlayers(R"(<linden_avatar wearable_definition_version="22">
      <layer_set body_region="head" width="512" height="512">
        <layer name="skin" global_color="skin_color">
          <texture local_texture="head_bodypaint"/>
        </layer>
      </layer_set>
      <layer_set body_region="eyes" width="128" height="128">
        <layer name="iris"><texture local_texture="eyes_iris"/></layer>
      </layer_set>
    </linden_avatar>)");
    if(!parsed.manifest)return 2;
    const std::map<unsigned,ImageRgba8> sources{
        {0U,{1U,1U,{0U,0U,0U,255U}}},
        {3U,{1U,1U,{255U,255U,255U,255U}}}};
    const auto audit=audit_firestorm_bake_sources(*parsed.manifest,sources);
    if(audit.upload_authorized()||audit.covered_regions!=2U||
       audit.decoded!=2U||audit.complex!=1U||
       audit.regions[0].complex_layers!=1U||
       audit.regions[3].decoded_sources!=1U||
       audit.regions[10].declared)
        return 3;
    auto incomplete=sources;incomplete.erase(3U);
    const auto no_eye=audit_firestorm_bake_sources(
        *parsed.manifest,incomplete);
    if(no_eye.upload_authorized()||no_eye.covered_regions!=1U||
       no_eye.missing!=1U||no_eye.regions[3].missing_sources!=1U)
        return 4;
    std::cout<<"All eleven Firestorm region input counts are read-only; "
                "source availability never authorizes uploading a bake\n";
    return 0;
}
