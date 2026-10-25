/* This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_bake_progress_trace.hpp"
#include <chrono>
#include <iostream>
#include <string>
using nexverse::ui::AvatarBakeProgressTrace;
using nexverse::ui::AvatarDiagnosticReport;
using namespace std::chrono_literals;
int main()
{
    AvatarBakeProgressTrace logger{};
    const auto now=AvatarBakeProgressTrace::Clock::now();
    AvatarDiagnosticReport r{};
    if(logger.observe(7U,r,now))return 1; // unauthenticated
    r.authenticated=true;
    r.region_connected=true;
    r.simulator_circuit_active=true;
    r.agent_identity_present=true;
    auto stage=logger.observe(7U,r,now);
    if(!stage||stage->find("self_low158_missing")==std::string::npos||
       stage->find("final_skin_rgb_unverified")==std::string::npos)
        return 2;
    if(logger.observe(7U,r,now+1s)||
       logger.observe(7U,r,now+29s))
        return 3;
    stage=logger.observe(7U,r,now+30s);
    if(!stage||stage->find("self_low158_missing")==std::string::npos)
        return 4;
    // Server packet appears. Tracking makes no fake GPU assertion.
    r.appearance_received=true;
    r.self_low158_texture_entry_received=true;
    r.self_low158_body_bake_refs=2U;
    r.self_low158_body_gettexture_stages={1U,3U,0U};
    r.self_render_body_bake_sources={1U,1U,0U};
    stage=logger.observe(7U,r,now+31s);
    if(!stage||stage->find("server_body_bakes_partial")==std::string::npos||
       stage->find("low158_gettexture=1,3,0")==std::string::npos||
       stage->find("selected_source=1,1,0")==std::string::npos)
        return 5;
    r.self_low158_body_bake_refs=3U;
    r.self_low158_body_gettexture_stages={1U,1U,1U};
    r.avatar_body_bakes_gpu=3U;
    r.avatar_body_bakes_gl=9U; // all draws could be from Head
    r.self_low158_current_body_gl_drawn={true,false,false};
    stage=logger.observe(7U,r,now+32s);
    if(!stage||stage->find("original_body_bake_gl_draw_missing")==
                   std::string::npos||
       stage->find("current_low158_gl=1,0,0")==std::string::npos)
        return 6;
    r.self_low158_current_body_gl_drawn={true,true,true};
    r.gpu_fragment_result_known=true;
    r.gpu_fragments_passed=true;
    // A single Head GPU query must NOT complete 3/3 Depth proof.
    r.avatar_depth_known={true,false,false};
    r.avatar_depth_passed={true,false,false};
    stage=logger.observe(7U,r,now+33s);
    if(!stage||stage->find("depth_visibility_unverified")==
                  std::string::npos||
       stage->find("depth_part_known=1,0,0")==std::string::npos)
        return 15;
    r.avatar_depth_known={true,true,true};
    stage=logger.observe(7U,r,now+33s+300ms);
    if(!stage||stage->find("depth_visible_final_skin_pixels_unverified")==
                   std::string::npos)
        return 7;
    // Terminal means all observable pre-RGB stages present;
    // never assert actual GPU RGB readback/complete outfit.
    if(logger.observe(7U,r,now+200s))return 8;
    // Rapidly oscillating asynchronous readiness should not spam logs;
    // stable changes are reported after the minimum transition window.
    r.gpu_fragments_passed=false;
    r.avatar_depth_passed={false,false,false};
    if(logger.observe(7U,r,now+33s+320ms))return 13;
    stage=logger.observe(7U,r,now+33s+600ms);
    if(!stage||stage->find("avatar_depth_fragments_zero")==
                   std::string::npos)return 14;
    // Another region cannot inherit previous progress / suppression.
    stage=logger.observe(8U,r,now+201s);
    if(!stage)return 9;
    r.authenticated=false;
    if(logger.observe(8U,r,now+202s))return 10;
    r.authenticated=true;
    stage=logger.observe(8U,r,now+203s);
    if(!stage)return 11;
    // No raw asset identifiers or network URLs are present.
    for(const auto* forbidden:{"http:","https:","agent_id=",
                              "session_id=","texture_id="})
        if(stage->find(forbidden)!=std::string::npos)return 12;
    std::cout<<"OGL_AVATAR_BAKE_PROGRESS_TRACE_PASS\n";
}
