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
const char* avatar_skin_blocker_key(const AvatarDiagnosticReport& r) noexcept
{
    if(!r.authenticated||!r.simulator_circuit_active||!r.region_connected)
        return "session_or_circuit_missing";
    if(!r.appearance_received)return "self_low158_missing";
    if(!r.self_low158_texture_entry_received)
        return "self_low158_texture_not_verified";
    if(r.self_low158_body_bake_refs==0U)
        return r.self_object_bake_candidate_faces>0U
            ?"server_body_missing_object_fallback"
            :"server_body_bakes_missing";
    if(r.self_low158_body_bake_refs<3U)
        return "server_body_bakes_partial";
    if(r.avatar_body_bakes_gpu<3U)
        return "body_bake_assets_not_gpu_resident";
    // Three draws of one body part do not prove all three source
    // textures reached the correct Firestorm .llm slot.
    if(!r.self_low158_current_body_gl_drawn[0]||
       !r.self_low158_current_body_gl_drawn[1]||
       !r.self_low158_current_body_gl_drawn[2])
        return "original_body_bake_gl_draw_missing";
    // The renderer's aggregate GL flag means ANY part query returned.
    // That is not proof for all three current Low158 body LLM draws.
    // Do not call 3/3 visibility complete if Upper or Lower is pending.
    if(!r.avatar_depth_known[0]||!r.avatar_depth_known[1]||
       !r.avatar_depth_known[2])
        return "depth_visibility_unverified";
    // Distinguish fully resolved, fully occluded geometry from merely
    // pending asynchronous GPU queries. One visible body region can
    // prove some fragments passed but cannot prove final Skin RGB.
    if(!(r.avatar_depth_passed[0]||
         r.avatar_depth_passed[1]||
         r.avatar_depth_passed[2]))
        return "avatar_depth_fragments_zero";
    // Depth query is not an image readback of the final RGB skin.
    return "depth_visible_final_skin_pixels_unverified";
}
std::string avatar_skin_blocker_hint(const AvatarDiagnosticReport& r)
{
    const auto* key=avatar_skin_blocker_key(r);
    if(std::strcmp(key,"session_or_circuit_missing")==0)
        return "Avatar: Login/Simulatorverbindung pruefen";
    if(std::strcmp(key,"self_low158_missing")==0)
        return "Avatar: Self-Low158 vom Simulator fehlt";
    if(std::strcmp(key,"self_low158_texture_not_verified")==0)
        return "Avatar: Self-Low158 TextureEntry nicht bestaetigt";
    if(std::strcmp(key,"server_body_missing_object_fallback")==0)
        return "Avatar: Self-Low158 0/3 Body-Bakes; Objekt-TE nur vorlaeufig";
    if(std::strcmp(key,"server_body_bakes_missing")==0)
        return "Avatar: Self-Low158 0/3 Body-Bakes; Server-Outfit pruefen";
    if(std::strcmp(key,"server_body_bakes_partial")==0)
        return "Avatar: Self-Low158 nur "+
            std::to_string(r.self_low158_body_bake_refs)+
            "/3 Body-Bakes; Server-Outfit pruefen";
    if(std::strcmp(key,"body_bake_assets_not_gpu_resident")==0)
        return "Avatar: Server-Bakes 3/3; GetTexture/GPU-Residency pruefen";
    if(std::strcmp(key,"original_body_bake_gl_draw_missing")==0)
        return "Avatar: Bakes auf GPU, Original-Body-Draw fehlt";
    if(std::strcmp(key,"depth_visibility_unverified")==0)
        return "Avatar: GL-Draw vorhanden, Depth-Test noch ausstehend";
    if(std::strcmp(key,"avatar_depth_fragments_zero")==0)
        return "Avatar: GL-Draw vorhanden, 0 sichtbare Depth-Fragmente";
    return "Avatar: Depth-Fragmente ja; finale Skin-Pixel nicht geprueft";
}
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
       "self_skin_blocker_stage="+std::string(avatar_skin_blocker_key(r))+"\n"
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
       "appearance_self_invalid_texture_ignored="+
           std::to_string(r.appearance_self_invalid_texture_ignored)+"\n"
       "appearance_self_stale_cof_rejected="+
           std::to_string(r.appearance_self_stale_cof_rejected)+"\n"
       "wearables_read_request_sent="+yn(r.wearables_read_request_sent)+"\n"
       "wearables_legacy_update_received="+
           yn(r.wearables_legacy_update_received)+"\n"
       "wearables_legacy_items="+
           std::to_string(r.wearables_legacy_items)+"\n"
       "wearables_legacy_assets="+
           std::to_string(r.wearables_legacy_assets)+"\n"
       "wearables_legacy_serial="+
           std::to_string(r.wearables_legacy_serial)+"\n"
       "self_low158_texture_entry_received="+
           yn(r.self_low158_texture_entry_received)+"\n"
       "self_low158_valid_bake_refs="+
           std::to_string(r.self_low158_valid_bake_refs)+"\n"
       "self_low158_body_bake_refs="+
           std::to_string(r.self_low158_body_bake_refs)+"\n"
       "self_low158_head_ref="+yn(r.self_low158_head_upper_lower_refs[0])+"\n"
       "self_low158_upper_ref="+yn(r.self_low158_head_upper_lower_refs[1])+"\n"
       "self_low158_lower_ref="+yn(r.self_low158_head_upper_lower_refs[2])+"\n"
       "self_low158_head_gettexture_stage="+
           std::to_string(r.self_low158_body_gettexture_stages[0])+"\n"
       "self_low158_upper_gettexture_stage="+
           std::to_string(r.self_low158_body_gettexture_stages[1])+"\n"
       "self_low158_lower_gettexture_stage="+
           std::to_string(r.self_low158_body_gettexture_stages[2])+"\n"
       "self_render_head_bake_source="+
           std::to_string(r.self_render_body_bake_sources[0])+"\n"
       "self_render_upper_bake_source="+
           std::to_string(r.self_render_body_bake_sources[1])+"\n"
       "self_render_lower_bake_source="+
           std::to_string(r.self_render_body_bake_sources[2])+"\n"
       "self_render_head_gettexture_stage="+
           std::to_string(r.self_render_body_gettexture_stages[0])+"\n"
       "self_render_upper_gettexture_stage="+
           std::to_string(r.self_render_body_gettexture_stages[1])+"\n"
       "self_render_lower_gettexture_stage="+
           std::to_string(r.self_render_body_gettexture_stages[2])+"\n"
       "self_attachment_block_received="+yn(r.self_attachment_block_received)+"\n"
       "self_attachment_block_count="+
           std::to_string(r.self_attachment_block_count)+"\n"
       "self_object_attachment_roots="+
           std::to_string(r.self_object_attachment_roots)+"\n"
       "self_attachment_effective_bindings="+
           std::to_string(r.self_attachment_effective_bindings)+"\n"
       "self_attachment_pending_low158="+
           std::to_string(r.self_attachment_pending_low158)+"\n"
       "self_attachment_stale_low158="+
           std::to_string(r.self_attachment_stale_low158)+"\n"
       "self_object_bake_candidate_faces="+
           std::to_string(r.self_object_bake_candidate_faces)+"\n"
       "self_object_bake_withdrawals="+
           std::to_string(r.self_object_bake_withdrawals)+"\n"
       "self_object_localid_kills="+
           std::to_string(r.self_object_kills)+"\n"
       "self_object_bake_candidate_active="+
           yn(r.self_object_bake_candidate_active)+"\n"
       "self_object_bake_candidate_gpu_resident="+
           std::to_string(r.self_object_bake_candidate_gpu_resident)+"\n"
       "self_object_bake_candidate_bake_gl_draws="+
           std::to_string(r.self_object_bake_candidate_bake_gl_draws)+"\n"
       "self_object_bake_candidate_depth_known="+
           yn(r.self_object_bake_candidate_depth_known)+"\n"
       "self_object_bake_candidate_depth_visible="+
           yn(r.self_object_bake_candidate_depth_visible)+"\n"
       "movement_queued="+yn(r.movement_queued)+"\n"
       "movement_udp_socket_sent="+yn(r.movement_socket_sent)+"\n"
       "movement_server_confirmed="+yn(r.movement_server_confirmed)+"\n"
       "movement_server_stalled="+yn(r.movement_server_stalled)+"\n"
       "movement_server_samples="+
           std::to_string(r.movement_server_samples)+"\n"
       "movement_server_source_code="+
           std::to_string(r.movement_source_code)+"\n"
       "movement_discontinuities="+
           std::to_string(r.movement_discontinuities)+"\n"
       "movement_stale_samples_ignored="+
           std::to_string(r.movement_stale_samples_ignored)+"\n"
       "movement_waiting_after_jump="+yn(r.movement_waiting_after_jump)+"\n"
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
       "self_low158_head_current_gl_drawn="+
           std::string(yn(r.self_low158_current_body_gl_drawn[0]))+"\n"
       "self_low158_upper_current_gl_drawn="+
           std::string(yn(r.self_low158_current_body_gl_drawn[1]))+"\n"
       "self_low158_lower_current_gl_drawn="+
           std::string(yn(r.self_low158_current_body_gl_drawn[2]))+"\n"
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
