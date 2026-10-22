/* MPL-2.0: Local requested yaw is not confirmed simulator rotation. */
#include "nexverse/protocol/local_heading_preview.hpp"
#include "nexverse/protocol/movement_controls.hpp"
#include "nexverse/render/third_person_camera.hpp"
#include <iostream>
#include <limits>
using namespace nexverse;
int main(){
    using Clock=std::chrono::steady_clock;
    const auto now=Clock::time_point{}+std::chrono::seconds{10};
    protocol::MovementController controls;
    controls.reset({});
    protocol::MovementKeys keys{};
    keys.turn_left=true;
    const auto update=controls.step(keys,0.2f);
    const protocol::Quaternion server{};
    const std::optional<protocol::LocalHeadingPreview> local{
      protocol::LocalHeadingPreview{"agent-a","session-b",update.body_rotation,now}};
    const auto preview=protocol::active_local_heading_preview(local,"agent-a","session-b",true,now);
    if(!preview||preview->z==server.z||server.z!=0.0f||server.w!=1.0f)return 1;
    const auto real_camera=render::third_person_camera({128.0f,128.0f,25.0f},server);
    const auto local_camera=render::third_person_camera({128.0f,128.0f,25.0f},*preview);
    if(!local_camera.position_valid||local_camera.position.y==real_camera.position.y)return 2;
    if(protocol::active_local_heading_preview(std::nullopt,"agent-a","session-b",true,now))return 3;
    if(protocol::active_local_heading_preview(local,"other","session-b",true,now)||
       protocol::active_local_heading_preview(local,"agent-a","other",true,now)||
       protocol::active_local_heading_preview(local,"agent-a","session-b",false,now))return 4;
    if(protocol::active_local_heading_preview(local,"agent-a","session-b",true,now+std::chrono::milliseconds{351})||
       protocol::active_local_heading_preview(local,"agent-a","session-b",true,now-std::chrono::milliseconds{1}))return 5;
    if(!protocol::active_local_heading_preview(local,"agent-a","session-b",true,now+std::chrono::milliseconds{350}))return 6;
    auto corrupt=*local;
    corrupt.rotation.x=std::numeric_limits<float>::quiet_NaN();
    if(protocol::active_local_heading_preview(corrupt,"agent-a","session-b",true,now))return 7;
    corrupt=*local;corrupt.rotation={0.0f,0.0f,0.0f,0.0f};
    if(protocol::active_local_heading_preview(corrupt,"agent-a","session-b",true,now))return 8;
    std::cout<<"OGL_LOCAL_VISUAL_YAW_NOT_SERVER_AUTHORITY_PASS\n";
}
