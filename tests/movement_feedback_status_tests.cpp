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
    std::cout<<"OGL movement server readback and truthful F3 statuses PASS\n";
}
