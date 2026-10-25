/* MPL-2.0 */
#include "nexverse/protocol/vertical_movement_feedback.hpp"
#include <chrono>
#include <iostream>
#include <string_view>
using namespace nexverse::protocol;
using namespace std::chrono_literals;
int main()
{
    VerticalMovementFeedbackMonitor monitor;
    const auto t=VerticalMovementFeedbackMonitor::TimePoint{};
    monitor.reset();
    (void)monitor.server_position({10,20,30},t);
    monitor.queued_controls(control_up_pos);
    if(monitor.server_position({10,20,31},t+1s)||
       monitor.snapshot().sent)return 1; // not actually UDP-sent
    monitor.outbound_controls(control_up_pos,t+2s);
    if(monitor.server_position({10,20,32},t+1s))return 2; // pre-send
    if(monitor.server_position({10,20,31.1f},t+3s)||
       monitor.snapshot().confirmed)return 3; // after send, less than +0.25m
    monitor.reset();
    (void)monitor.server_position({10,20,30},t+10s);
    monitor.queued_controls(control_up_pos);
    monitor.outbound_controls(control_up_pos,t+11s);
    if(monitor.server_position({12,21,30},t+12s)||
       monitor.snapshot().confirmed)return 4; // horizontal is not jump
    if(monitor.server_position({12,21,29.6f},t+13s)||
       monitor.snapshot().contradictory_samples!=1U)return 5; // wrong way
    if(!monitor.server_position({12,21,30.4f},t+14s)||
       !monitor.snapshot().confirmed)return 6;
    monitor.queued_controls(control_up_neg);
    monitor.outbound_controls(control_up_neg|control_fly,t+15s);
    if(monitor.snapshot().confirmed ||
       monitor.server_position({12,21,31.0f},t+16s))return 7;
    if(!monitor.server_position({12,21,29.9f},t+17s)||
       !monitor.snapshot().confirmed)return 8;
    monitor.queued_controls(0U);
    if(monitor.snapshot().sent)return 9;
    monitor.outbound_controls(control_up_pos,t+18s); // old socket queue
    if(monitor.snapshot().sent)return 10;
    monitor.reset();
    (void)monitor.server_position({10,20,30},t+20s);
    monitor.queued_controls(control_up_pos);
    monitor.outbound_controls(control_up_pos,t+21s);
    if(monitor.server_position({100,100,55},t+22s)||
       monitor.snapshot().confirmed)return 11; // teleport
    if(monitor.server_position({100,100,55},t+23s))return 12;
    if(monitor.server_position({100,100,55.4f},t+24s)||
       monitor.snapshot().sent||monitor.snapshot().confirmed||
       !monitor.snapshot().awaiting_fresh_udp_after_jump)
       return 13;
    if(std::string_view(monitor.status()).find("neuer UDP-Versand")==
       std::string_view::npos)return 18;
    monitor.queued_controls(control_up_pos);
    if(monitor.server_position({100,100,55.55f},t+24s+100ms)||
       monitor.snapshot().sent||monitor.snapshot().confirmed)return 19;
    monitor.outbound_controls(control_up_pos,t+25s);
    if(!monitor.snapshot().sent||monitor.snapshot().confirmed||
       monitor.snapshot().awaiting_fresh_udp_after_jump)
       return 16;
    if(!monitor.server_position({100,100,55.90f},t+26s)||
       !monitor.snapshot().confirmed)return 17;
    monitor.reset();
    (void)monitor.server_position({10,20,30},t+30s);
    monitor.queued_controls(control_up_neg);
    monitor.outbound_controls(control_up_neg,t+31s);
    if(monitor.server_position({10,20,30},t+35s) ||
       monitor.server_position({10,20,30},t+36s) ||
       monitor.server_position({10,20,30},t+37s) ||
       !monitor.snapshot().stalled)return 14;
    if(std::string_view(monitor.status()).find("bleibt aus")==
       std::string_view::npos)return 15;
    std::cout<<"OGL_REAL_SIMULATOR_VERTICAL_POSITION_READBACK_SIGN_AND_RESET_PASS\n";
}
