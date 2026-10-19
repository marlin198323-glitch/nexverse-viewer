/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_diagnostic_report.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/stat.h>
#endif
int main()
{
    namespace fs=std::filesystem;
    nexverse::ui::AvatarDiagnosticReport r{};
    r.authenticated=true;
    r.world_ready=true;
    r.agent_present=true;
    r.region_connected=true;
    r.self_appearance_support_sent=true;
    r.self_appearance_support_acknowledged=true;
    r.self_appearance_ack_wait_seconds=27U;
    r.appearance_malformed_stage_code=7U;
    r.appearance_self_trial_ignored=2U;
    r.appearance_self_incomplete_ignored=3U;
    r.appearance_self_invalid_texture_ignored=2U;
    r.wearables_read_request_sent=true;
    r.wearables_legacy_update_received=true;
    r.wearables_legacy_items=17U;
    r.wearables_legacy_assets=14U;
    r.wearables_legacy_serial=8U;
    r.self_attachment_block_received=true;
    r.self_attachment_block_count=85U;
    r.movement_discontinuities=3U;
    r.movement_stale_samples_ignored=2U;
    r.movement_waiting_after_jump=true;
    r.self_object_bake_candidate_faces=2U;
    r.self_object_bake_candidate_active=true;
    r.self_object_bake_candidate_gpu_resident=2U;
    r.self_object_bake_candidate_bake_gl_draws=2U;
    r.self_object_bake_candidate_depth_known=true;
    r.self_object_bake_candidate_depth_visible=false;
    r.movement_queued=true;
    r.movement_socket_sent=true;
    r.movement_server_confirmed=true;
    r.movement_server_samples=3U;
    r.movement_source_code=2U;
    r.simulator_circuit_active=true;
    r.region_handle_present=true;
    r.agent_identity_present=false; // identity may be missing after region
    r.terrain_packets=11U;
    r.object_packets=27U;
    r.avatar_packets=2U;
    r.world_entry_stage=nexverse::ui::WorldEntryStage::agent_position;
    r.manual_reconnect_count=1U;
    r.retry_reason=nexverse::ui::LoadingCircuitRetryReason::cooldown_wait;
    r.entry_trace_count=2U;
    r.entry_trace[0]={nexverse::ui::WorldEntryStage::simulator_circuit,16U};
    r.entry_trace[1]={nexverse::ui::WorldEntryStage::agent_position,18U};
    r.world_entry_wait_seconds=18;
    r.caps_event_queue_available=true;
    r.original_mesh_draws=3U;
    r.proxy_draws=0U;
    r.frame_width=3440U;
    r.frame_height=1440U;
    r.gpu_fragment_result_known=true;
    r.gpu_fragments_passed=false;
    const std::string text=nexverse::ui::avatar_diagnostic_text(r);
    if(text.find("visibility_stage=zero_depth_fragments")==
       std::string::npos||text.find("rendered_original_llm_draws=3")==
       std::string::npos)return 1;
    if(text.find("appearance_self_trial_ignored=2")==std::string::npos||
       text.find("appearance_self_incomplete_ignored=3")==std::string::npos||
       text.find("appearance_self_invalid_texture_ignored=2")==std::string::npos||
       text.find("wearables_read_request_sent=yes")==std::string::npos||
       text.find("wearables_legacy_update_received=yes")==std::string::npos||
       text.find("wearables_legacy_items=17")==std::string::npos||
       text.find("wearables_legacy_assets=14")==std::string::npos||
       text.find("wearables_legacy_serial=8")==std::string::npos||
       text.find("self_attachment_block_received=yes")==std::string::npos||
       text.find("self_attachment_block_count=85")==std::string::npos||
       text.find("movement_discontinuities=3")==std::string::npos||
       text.find("movement_stale_samples_ignored=2")==std::string::npos||
       text.find("movement_waiting_after_jump=yes")==std::string::npos||
       text.find("self_object_bake_candidate_faces=2")==std::string::npos||
       text.find("self_object_bake_candidate_active=yes")==std::string::npos||
       text.find("self_object_bake_candidate_gpu_resident=2")==std::string::npos||
       text.find("self_object_bake_candidate_bake_gl_draws=2")==std::string::npos||
       text.find("self_object_bake_candidate_depth_known=yes")==std::string::npos||
       text.find("self_object_bake_candidate_depth_visible=no")==std::string::npos||
       text.find("self_appearance_reply_wait_seconds=27")==std::string::npos||
       text.find("region_handshake_self_appearance_bit2_acked=yes")==std::string::npos||
       text.find("region_handshake_self_appearance_bit2_sent=yes")==std::string::npos||
       text.find("region_connected=yes")==std::string::npos||
       text.find("identity_pair_present=no")==std::string::npos||
       text.find("movement_udp_socket_sent=yes")==std::string::npos||
       text.find("movement_server_confirmed=yes")==std::string::npos||
       text.find("movement_server_samples=3")==std::string::npos||
       text.find("movement_server_source_code=2")==std::string::npos||
       text.find("terrain_packets=11")==std::string::npos||
       text.find("object_packets=27")==std::string::npos||
       text.find("world_entry_stage=agent_position")==std::string::npos||
       text.find("world_entry_wait_seconds=18")==std::string::npos||
       text.find("caps_event_queue_available=yes")==std::string::npos)
        return 7;
    for(const auto word:{"agent_id","session_id","password","capability_url",
                         "avatar_name","chat_message","simulator_ip"})
        if(text.find(word)!=std::string::npos)return 2;
    const auto root=fs::temp_directory_path()/(
        "ogl-avatar-diagnose-test-"+std::to_string(
        std::chrono::high_resolution_clock::now().
        time_since_epoch().count()));
    const auto file=nexverse::ui::save_avatar_diagnostic(root,r);
    if(!file||!fs::exists(*file))return 3;
    std::ifstream in(*file);
    std::string contents{std::istreambuf_iterator<char>{in},{}};
    if(contents!=text+nexverse::ui::avatar_diagnostic_trace_text(r))
        return 4;
    for(const auto secret:{"agent_id=","session_id=","password=",
                           "avatar_name=","simulator_ip=","chat_message="})
        if(contents.find(secret)!=std::string::npos)return 9;
    if(contents.find("world_entry_trace_0=simulator_circuit:16s")==
       std::string::npos||
       contents.find("manual_reconnect_attempts=1")==std::string::npos)
        return 8;
#if defined(__unix__) || defined(__APPLE__)
    struct stat state{};
    if(stat(file->c_str(),&state)!=0||(state.st_mode&0777)!=0600)
        return 5;
#endif
    if(nexverse::ui::save_avatar_diagnostic({},r))return 6;
    fs::remove_all(root);
    std::cout<<"Private local avatar report has no IDs/credentials and mode 0600 PASS\n";
    return 0;
}
