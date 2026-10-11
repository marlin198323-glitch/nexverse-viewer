/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstdint>
#include <string_view>

namespace nexverse::protocol
{
// A change of simulator endpoint requires a complete new LLUDP circuit.
// The old future MUST become ready before its stop flag can be reused.
enum class RegionHandoffAction
{
    unchanged,
    wait_for_old_worker,
    launch_new_worker,
    invalid_destination
};
[[nodiscard]] constexpr RegionHandoffAction region_handoff_action(
    std::string_view active_ip,std::uint16_t active_port,
    std::string_view destination_ip,std::uint16_t destination_port,
    bool old_worker_finished) noexcept
{
    if(destination_ip.empty()||destination_port==0U||
       destination_ip.size()>255U)
        return RegionHandoffAction::invalid_destination;
    if(active_ip==destination_ip&&active_port==destination_port)
        return RegionHandoffAction::unchanged;
    return old_worker_finished
        ?RegionHandoffAction::launch_new_worker
        :RegionHandoffAction::wait_for_old_worker;
}
[[nodiscard]] constexpr bool region_packet_matches(
    std::string_view packet_source_ip,std::uint16_t packet_source_port,
    std::string_view region_ip,std::uint16_t region_port,
    bool stop_requested) noexcept
{
    return !stop_requested&&packet_source_port!=0U &&
        packet_source_ip==region_ip && packet_source_port==region_port;
}
}
