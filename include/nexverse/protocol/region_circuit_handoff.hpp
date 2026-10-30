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
// Socket equality is insufficient when two regions share the same
// simulator IP:port. Capture the authenticated region incarnation at
// worker launch and compare it against the current WorldModel.
struct RegionCircuitKey
{
    std::string_view ip;
    std::uint16_t port=0U;
    std::uint64_t handle=0U;
    std::uint32_t generation=0U;
};
[[nodiscard]] constexpr bool region_circuit_key_matches(
    RegionCircuitKey active,RegionCircuitKey target) noexcept
{
    return active.port!=0U &&active.ip==target.ip &&
        active.port==target.port &&active.handle==target.handle &&
        active.generation==target.generation;
}
[[nodiscard]] constexpr RegionHandoffAction region_handoff_action(
    RegionCircuitKey active,RegionCircuitKey destination,
    bool old_worker_finished,bool transition_pending) noexcept
{
    const auto socket=region_handoff_action(
        active.ip,active.port,destination.ip,destination.port,
        old_worker_finished);
    if(socket!=RegionHandoffAction::unchanged)return socket;
    if(!transition_pending||region_circuit_key_matches(active,destination))
        return RegionHandoffAction::unchanged;
    return old_worker_finished
        ?RegionHandoffAction::launch_new_worker
        :RegionHandoffAction::wait_for_old_worker;
}
// Readiness depends on the active circuit target, not the original
// login region. A successful cross-region handshake must unlock the
// destination's WorldModel and controls.
[[nodiscard]] constexpr bool region_circuit_ready(
    std::string_view circuit_ip,std::uint16_t circuit_port,
    std::string_view region_ip,std::uint16_t region_port,
    bool circuit_active,bool stop_requested) noexcept
{
    return circuit_active&&!stop_requested&&circuit_port!=0U &&
        circuit_ip==region_ip && circuit_port==region_port;
}
[[nodiscard]] constexpr bool region_circuit_ready(
    RegionCircuitKey active,RegionCircuitKey target,
    bool circuit_active,bool stop_requested) noexcept
{
    return circuit_active&&!stop_requested&&
        region_circuit_key_matches(active,target);
}
[[nodiscard]] constexpr bool region_packet_matches(
    std::string_view packet_source_ip,std::uint16_t packet_source_port,
    std::string_view region_ip,std::uint16_t region_port,
    bool stop_requested) noexcept
{
    return !stop_requested&&packet_source_port!=0U &&
        packet_source_ip==region_ip && packet_source_port==region_port;
}
[[nodiscard]] constexpr bool region_packet_matches(
    RegionCircuitKey source,RegionCircuitKey destination,
    bool stop_requested) noexcept
{
    return !stop_requested&&
        region_circuit_key_matches(source,destination);
}
}
