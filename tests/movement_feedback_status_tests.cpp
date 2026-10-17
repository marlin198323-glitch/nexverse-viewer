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
    std::cout<<"OGL movement server readback and truthful F3 statuses PASS\n";
}
