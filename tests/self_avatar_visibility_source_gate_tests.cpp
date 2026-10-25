/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/render/self_avatar_visibility_source_gate.hpp"
#include <iostream>
#include <optional>
using nexverse::render::SelfAvatarVisibilitySourceGate;
int main()
{
    using Ids=SelfAvatarVisibilitySourceGate::BakeIds;
    SelfAvatarVisibilitySourceGate gate;
    const Ids original{"head-a","upper-a","lower-a"};
    if(!gate.observe(original)||gate.observe(original))return 1;
    if(!gate.observe(Ids{"head-a","upper-b","lower-a"}))return 2;
    if(gate.observe(Ids{"head-a","upper-b","lower-a"}))return 3;
    if(!gate.observe(Ids{"head-a","upper-b","lower-b"}))return 4;
    // Losing self ownership, e.g. logout or missing avatar, must retire
    // old depth proof. Reappearance is a new source incarnation.
    if(!gate.observe(std::nullopt)||gate.observe(std::nullopt))return 5;
    if(!gate.observe(original)||gate.observe(original))return 6;
    gate.reset();
    if(!gate.observe(original))return 7;
    // Empty texture slots (placeholder/local layers) are distinct
    // sources: replacing an actual Upper bake with no bake also resets.
    if(!gate.observe(Ids{"head-a","","lower-a"}))return 8;
    std::cout<<"OGL_SELF_AVATAR_BAKE_VISIBILITY_SOURCE_GATE_PASS\n";
}
