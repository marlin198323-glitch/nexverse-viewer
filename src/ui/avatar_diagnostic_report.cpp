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
       "agent_present="+yn(r.agent_present)+"\n"
       "appearance_received="+yn(r.appearance_received)+"\n"
       "camera_valid="+yn(r.camera_valid)+"\n"
       "avatar_center_in_frustum="+yn(r.center_in_frustum)+"\n"
       "rendered_original_llm_draws="+std::to_string(r.original_mesh_draws)+"\n"
       "rendered_proxy_draws="+std::to_string(r.proxy_draws)+"\n"
       "rendered_baked_texture_draws="+std::to_string(r.baked_texture_draws)+"\n"
       "gpu_query_known="+yn(r.gpu_fragment_result_known)+"\n"
       "gpu_depth_fragments_passed="+
           std::string(yn(r.gpu_fragments_passed))+"\n"
       "visibility_stage="+evidence+"\n"
       "world_scene_draws="+std::to_string(r.objects_drawn)+"\n"
       "world_missing_mesh_objects="+
           std::to_string(r.missing_mesh_objects)+"\n"
       "framebuffer_width="+std::to_string(r.frame_width)+"\n"
       "framebuffer_height="+std::to_string(r.frame_height)+"\n"
       "limitations=GL_depth_fragments_are_not_final_avatar_pixels_or_COFPose\n";
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
    const std::string body=avatar_diagnostic_text(report);
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
