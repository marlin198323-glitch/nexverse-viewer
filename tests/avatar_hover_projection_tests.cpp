/* MPL-2.0: invalid Low158 hover may not suppress a real
 * current-agent's third-person projection or corrupt remote GL proxy. */
#include "nexverse/render/avatar_hover_safety.hpp"
#include "nexverse/ui/avatar_view_evidence.hpp"
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include <cmath>
#include <iostream>
#include <limits>
int main()
{
    using namespace nexverse;
    const protocol::Vec3 agent{128.0f,128.0f,25.0f};
    const protocol::Quaternion orientation{0,0,0,1};
    const auto pose=render::third_person_camera(agent,orientation,8.0f,0.0f);
    for(const float bad:{std::numeric_limits<float>::quiet_NaN(),
                         std::numeric_limits<float>::infinity(),
                         -std::numeric_limits<float>::infinity()})
    {
        const auto hover=render::safe_avatar_hover(bad);
        const protocol::Vec3 drawn{agent.x,agent.y,agent.z+hover.meters};
        const auto projected=ui::inspect_avatar_view(pose,drawn,3440,1440);
        if(!hover.invalid||!std::isfinite(drawn.z)||
           !projected.valid_pose||!projected.center_in_frustum)
            return 1;
    }
    ui::AvatarPipelineProof proof;
    proof.world_ready=true;
    proof.self_present=true;
    proof.self_hover_invalid=true;
    proof.remote_hover_invalid=2U;
    if(proof.hover_stage()!="Hover korrigiert: Self 1 / Fremde 2")
        return 2;
    proof.self_hover_invalid=false;
    proof.remote_hover_invalid=0U;
    if(!proof.hover_stage().empty())return 3;
    std::cout<<"OGL_AVATAR_HOVER_REAL_CAMERA_PROJECTION_RECOVERED_PASS\n";
}