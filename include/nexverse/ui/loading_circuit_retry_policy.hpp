/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
namespace nexverse::ui
{
// Manual retry only after the original LLUDP worker has exited.
// An active/handing-off circuit must never be restarted by the loading UI.
[[nodiscard]] constexpr bool can_retry_loading_circuit(
    bool authenticated,bool circuit_started,bool final_failed,
    bool worker_finished,bool stop_requested,bool valid_target,
    bool cooldown_expired) noexcept
{
    return authenticated&&circuit_started&&final_failed&&
        worker_finished&&!stop_requested&&valid_target&&
        cooldown_expired;
}
}
