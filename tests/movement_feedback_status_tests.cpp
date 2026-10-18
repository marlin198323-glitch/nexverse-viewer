/* MPL-2.0 */
#include "nexverse/protocol/movement_feedback.hpp"
#include <chrono>
#include <string_view>
#include <iostream>

int main()
{
    using namespace nexverse::protocol;
    using namespace std::chrono_literals;
    MovementFeedbackMonitor monitor;
    const auto now=MovementFeedbackMonitor::TimePoint{};
    if(std::string_view(monitor.status()).find("kein Richtungsbefehl")==std::string_view::npos)
        return 1;
    (void)monitor.server_position({127,80,22},now);
    monitor.outbound_controls(control_at_pos,now);
    if(std::string_view(monitor.status()).find("ausstehend")==std::string_view::npos)
        return 2;
    (void)monitor.server_position({127,80,22},now+4s);
    (void)monitor.server_position({127,80,22},now+5s);
    if(monitor.server_position({127,80,22},now+6s)!=MovementEvidence::server_position_stalled)
        return 3;
    if(std::string_view(monitor.status()).find("bleibt gleich")==std::string_view::npos)
        return 4;
    if(monitor.server_position({128,80,22},now+7s)!=MovementEvidence::server_position_changed)
        return 5;
    if(std::string_view(monitor.status()).find("geaendert")==std::string_view::npos)
        return 6;
    monitor.reset();
    if(std::string_view(monitor.status()).find("kein Richtungsbefehl")==std::string_view::npos)
        return 7;
    // A queue entry is never evidence of a socket transmission.
    (void)monitor.server_position({127,80,22},now+10s);
    monitor.queued_controls(control_at_pos);
    if(std::string_view(monitor.status()).find("wartet auf UDP-Versand")==
       std::string_view::npos)return 8;
    for(int i=11;i<=17;++i)
        if(monitor.server_position({127,80,22},now+std::chrono::seconds{i})!=
           MovementEvidence::none)return 9;
    monitor.queued_controls(0U);
    monitor.outbound_controls(control_at_pos,now+18s); // superseded
    if(std::string_view(monitor.status()).find("kein Richtungsbefehl")==
       std::string_view::npos)return 10;
    monitor.queued_controls(control_at_pos);
    monitor.outbound_controls(control_at_pos,now+19s);
    if(std::string_view(monitor.status()).find("ausstehend")==
       std::string_view::npos)return 11;
    (void)monitor.server_position({127,80,22},now+20s);
    (void)monitor.server_position({127,80,22},now+21s);
    if(monitor.server_position({127,80,22},now+24s)!=
       MovementEvidence::server_position_stalled)return 12;
    monitor.reset();
    (void)monitor.server_position({8,4,3},now+30s);
    monitor.queued_controls(control_left_pos);
    monitor.outbound_controls(control_left_pos,now+30s);
    if(monitor.server_position({8,4.6f,3},now+31s,
        MovementPositionSource::full_object)!=
        MovementEvidence::server_position_changed)return 13;
    if(std::string_view(monitor.status()).find("ObjectUpdate")==
       std::string_view::npos)return 14;
    monitor.reset();
    (void)monitor.server_position({8,4,3},now+40s);
    monitor.queued_controls(control_at_pos);
    monitor.outbound_controls(control_at_pos,now+40s);
    if(monitor.server_position({8.5f,4,3},now+41s,
        MovementPositionSource::compressed_object)!=
        MovementEvidence::server_position_changed)return 15;
    if(std::string_view(monitor.status()).find("Compressed")==
       std::string_view::npos)return 16;
    // A gravity snap, jump, or terrain height correction is not a
    // horizontal WASD response; previously >=0.25m Z counted falsely.
    monitor.reset();
    (void)monitor.server_position({30,40,4},now+50s);
    monitor.queued_controls(control_at_pos);
    monitor.outbound_controls(control_at_pos,now+50s);
    if(monitor.server_position({30,40,6.5f},now+51s)!=MovementEvidence::none||
       monitor.server_position({30,40,6.5f},now+52s)!=MovementEvidence::none||
       monitor.server_position({30,40,6.5f},now+55s)!=
           MovementEvidence::server_position_stalled)return 17;
    if(monitor.snapshot().server_confirmed)return 18;
    if(monitor.server_position({30.30f,40,6.5f},now+56s)!=
       MovementEvidence::server_position_changed)return 19;

    // Teleport-like discontinuities invalidate the old baseline,
    // rather than fabricating motion confirmation or false stall.
    monitor.reset();
    (void)monitor.server_position({0,0,0},now+100s);
    monitor.queued_controls(control_at_pos);
    monitor.outbound_controls(control_at_pos,now+100s);
    if(monitor.server_position({100,100,5},now+101s)!=
       MovementEvidence::server_position_discontinuity)return 20;
    if(monitor.snapshot().server_confirmed ||
       monitor.snapshot().discontinuities!=1U ||
       monitor.snapshot().server_samples!=0U ||
       !monitor.snapshot().awaiting_after_discontinuity ||
       std::string_view(monitor.status()).find("Positionssprung/Teleport")==
           std::string_view::npos)return 21;
    if(monitor.server_position({100,100,5},now+102s)!=
       MovementEvidence::none)return 22;
    if(monitor.server_position({100.4f,100,5},now+103s,
        MovementPositionSource::compressed_object)!=
       MovementEvidence::server_position_changed)return 23;
    if(!monitor.snapshot().server_confirmed ||
       monitor.snapshot().awaiting_after_discontinuity ||
       monitor.snapshot().confirmed_source!=
           MovementPositionSource::compressed_object)return 24;

    // Ignore server observations explicitly timestamped before a socket
    // send and out-of-order server samples without advancing baseline.
    monitor.reset();
    (void)monitor.server_position({3,4,5},now+200s);
    monitor.queued_controls(control_at_pos);
    monitor.outbound_controls(control_at_pos,now+201s);
    if(monitor.server_position({4,4,5},now+200s)!=
       MovementEvidence::none||
       monitor.snapshot().stale_samples_ignored!=1U ||
       monitor.snapshot().server_samples!=0U)return 25;
    if(monitor.server_position({3.4f,4,5},now+202s)!=
       MovementEvidence::server_position_changed)return 26;
    if(monitor.server_position({50,50,5},now+201s)!=
       MovementEvidence::none||
       monitor.snapshot().stale_samples_ignored!=2U)return 27;

    // Unsent enqueued controls cannot start a timeout or confirm a
    // simulator response even when large positions arrive.
    monitor.reset();
    (void)monitor.server_position({0,0,0},now+250s);
    monitor.queued_controls(control_at_pos);
    (void)monitor.server_position({10,10,0},now+260s);
    if(monitor.snapshot().socket_direction_sent||
       monitor.snapshot().server_confirmed||
       monitor.snapshot().discontinuities!=0U)return 28;
    monitor.queued_controls(0U);
    if(monitor.snapshot().awaiting_after_discontinuity)return 29;

    std::cout<<"OGL movement server readback and truthful F3 statuses PASS\n";
}
