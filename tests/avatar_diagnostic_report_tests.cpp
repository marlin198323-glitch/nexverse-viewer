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
    r.simulator_circuit_active=true;
    r.region_handle_present=true;
    r.agent_identity_present=false; // identity may be missing after region
    r.terrain_packets=11U;
    r.object_packets=27U;
    r.avatar_packets=2U;
    r.world_entry_stage=nexverse::ui::WorldEntryStage::agent_position;
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
    if(text.find("region_connected=yes")==std::string::npos||
       text.find("identity_pair_present=no")==std::string::npos||
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
    if(contents!=text)return 4;
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
