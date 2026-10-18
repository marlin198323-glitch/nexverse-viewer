/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_diagnostic_report.hpp"
#include <chrono>
#include <fstream>
#include <system_error>
#include <cstring>
#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <unistd.h>
#endif
namespace nexverse::ui
{
std::string avatar_diagnostic_text(const AvatarDiagnosticReport& r)
{
    const auto yn=[](bool v){return v?"yes":"no";};
    const std::string evidence=r.original_mesh_draws==0U
        ?"no_original_mesh":!r.gpu_fragment_result_known
        ?"pending_gpu_query":r.gpu_fragments_passed
        ?"depth_fragments_passed":"zero_depth_fragments";
    return "OpenGenesisLINK Viewer - local avatar rendering diagnosis\n"
       "format=1\n"
       "source=local_read_only_no_grid_credentials\n"
       "authenticated="+std::string(yn(r.authenticated))+"\n"
       "world_ready="+yn(r.world_ready)+"\n"
       "region_connected="+yn(r.region_connected)+"\n"
       "simulator_circuit_active="+yn(r.simulator_circuit_active)+"\n"
       "region_handle_present="+yn(r.region_handle_present)+"\n"
       "identity_pair_present="+yn(r.agent_identity_present)+"\n"
       "agent_present="+yn(r.agent_present)+"\n"
       "appearance_received="+yn(r.appearance_received)+"\n"
       "region_handshake_self_appearance_bit2_sent="+
           std::string(yn(r.self_appearance_support_sent))+"\n"
       "region_handshake_self_appearance_bit2_acked="+
           std::string(yn(r.self_appearance_support_acknowledged))+"\n"
       "self_appearance_reply_wait_seconds="+
           std::to_string(r.self_appearance_ack_wait_seconds)+"\n"
       "camera_valid="+yn(r.camera_valid)+"\n"
       "avatar_center_in_frustum="+yn(r.center_in_frustum)+"\n"
       "rendered_original_llm_draws="+std::to_string(r.original_mesh_draws)+"\n"
       "rendered_proxy_draws="+std::to_string(r.proxy_draws)+"\n"
       "rendered_baked_texture_draws="+std::to_string(r.baked_texture_draws)+"\n"
       "gpu_query_known="+yn(r.gpu_fragment_result_known)+"\n"
       "gpu_depth_fragments_passed="+
           std::string(yn(r.gpu_fragments_passed))+"\n"
       "visibility_stage="+evidence+"\n"
       "appearance_packets="+
           std::to_string(r.appearance_packets)+"\n"
       "appearance_malformed="+
           std::to_string(r.appearance_malformed)+"\n"
       "appearance_malformed_stage_code="+
           std::to_string(r.appearance_malformed_stage_code)+"\n"
       "appearance_other_avatar="+
           std::to_string(r.appearance_other_avatar)+"\n"
       "appearance_self_accepted="+
           std::to_string(r.appearance_self_accepted)+"\n"
       "appearance_self_trial_ignored="+
           std::to_string(r.appearance_self_trial_ignored)+"\n"
       "appearance_self_incomplete_ignored="+
           std::to_string(r.appearance_self_incomplete_ignored)+"\n"
       "world_scene_draws="+std::to_string(r.objects_drawn)+"\n"
       "scene_linkset_max_depth="+std::to_string(r.scene_linkset_max_depth)+"\n"
       "scene_linkset_deep_visible="+std::to_string(r.scene_linkset_deep_visible)+"\n"
       "scene_bone_pose_avatars="+std::to_string(r.scene_bone_pose_avatars)+"\n"
       "world_missing_mesh_objects="+
           std::to_string(r.missing_mesh_objects)+"\n"
       "terrain_real_layers="+
           std::to_string(r.terrain_real_layers)+"\n"
       "terrain_textured_gl="+yn(r.terrain_textured_gl)+"\n"
       "terrain_fallback_gl="+yn(r.terrain_fallback_gl)+"\n"
       "mesh_lod_total="+std::to_string(r.mesh_lod_total)+"\n"
       "mesh_lod_matched="+std::to_string(r.mesh_lod_matched)+"\n"
       "mesh_lod_high_requested="+
           std::to_string(r.mesh_lod_high_requested)+"\n"
       "avatar_body_bakes_gpu="+
           std::to_string(r.avatar_body_bakes_gpu)+"\n"
       "avatar_body_bakes_gl="+
           std::to_string(r.avatar_body_bakes_gl)+"\n"
       "avatar_depth_head_known="+
           std::to_string(r.avatar_depth_known[0])+"\n"
       "avatar_depth_head_passed="+
           std::to_string(r.avatar_depth_passed[0])+"\n"
       "avatar_depth_upper_known="+
           std::to_string(r.avatar_depth_known[1])+"\n"
       "avatar_depth_upper_passed="+
           std::to_string(r.avatar_depth_passed[1])+"\n"
       "avatar_depth_lower_known="+
           std::to_string(r.avatar_depth_known[2])+"\n"
       "avatar_depth_lower_passed="+
           std::to_string(r.avatar_depth_passed[2])+"\n"
       "avatar_self_keyframe_draws="+
           std::to_string(r.avatar_self_keyframe_draws)+"\n"
       "avatar_self_server_motion_draws="+
           std::to_string(r.avatar_self_server_motion_draws)+"\n"
       "avatar_server_animation_received="+
           yn(r.avatar_server_animation_received)+"\n"
       "world_entry_stage="+
           std::string(world_entry_stage_key(r.world_entry_stage))+"\n"
       "world_entry_wait_seconds="+
            std::to_string(r.world_entry_wait_seconds)+"\n"
       "manual_reconnect_attempts="+
            std::to_string(r.manual_reconnect_count)+"\n"
       "retry_reason_code="+
            std::to_string(static_cast<unsigned>(r.retry_reason))+"\n"
       "terrain_packets="+std::to_string(r.terrain_packets)+"\n"
       "object_packets="+std::to_string(r.object_packets)+"\n"
       "avatar_packets="+std::to_string(r.avatar_packets)+"\n"
       "other_packets="+std::to_string(r.other_packets)+"\n"
       "caps_event_queue_available="+yn(r.caps_event_queue_available)+"\n"
       "framebuffer_width="+std::to_string(r.frame_width)+"\n"
       "framebuffer_height="+std::to_string(r.frame_height)+"\n"
       "limitations=GL_depth_fragments_are_not_final_avatar_pixels_or_COFPose\n";
}
// Include only predefined stage enums and finite elapsed seconds.
std::string avatar_diagnostic_trace_text(const AvatarDiagnosticReport& r)
{
    std::string result="world_entry_trace_size="+
        std::to_string(std::min(r.entry_trace_count,
                           WorldEntryTrace::capacity))+"\n";
    for(std::size_t i=0U;i<std::min(r.entry_trace_count,
                                  WorldEntryTrace::capacity);++i)
    {
        result+="world_entry_trace_"+std::to_string(i)+"="+
            std::string(world_entry_stage_key(r.entry_trace[i].stage))+
            ":"+std::to_string(r.entry_trace[i].dwell_seconds)+"s\n";
    }
    return result;
}

std::optional<std::filesystem::path> save_avatar_diagnostic(
    const std::filesystem::path& folder,const AvatarDiagnosticReport& report)
{
    if(folder.empty())return std::nullopt;
    std::error_code ec;
    std::filesystem::create_directories(folder,ec);
    if(ec)return std::nullopt;
    const auto now=std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    const auto target=folder/("avatar-diagnose-"+std::to_string(now)+".txt");
    const std::string body=avatar_diagnostic_text(report)+
        avatar_diagnostic_trace_text(report);
#if defined(__unix__) || defined(__APPLE__)
    const int fd=open(target.c_str(),O_CREAT|O_EXCL|O_WRONLY
#ifdef O_NOFOLLOW
        |O_NOFOLLOW
#endif
        ,0600);
    if(fd<0)return std::nullopt;
    std::size_t written=0;
    bool ok=true;
    while(written<body.size())
    {
        const ssize_t count=write(fd,body.data()+written,body.size()-written);
        if(count<=0){ok=false;break;}
        written+=static_cast<std::size_t>(count);
    }
    if(close(fd)!=0)ok=false;
    if(!ok){std::filesystem::remove(target,ec);return std::nullopt;}
#else
    std::ofstream out(target,std::ios::out|std::ios::trunc|std::ios::binary);
    if(!out)return std::nullopt;
    out<<body;out.close();
    if(!out){std::filesystem::remove(target,ec);return std::nullopt;}
#endif
    return target;
}
}
