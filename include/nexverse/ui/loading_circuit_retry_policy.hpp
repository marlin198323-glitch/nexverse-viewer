/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
namespace nexverse::ui
{
// Pure UI state, never contains a host, IP, UUID or raw error string.
enum class LoadingCircuitRetryReason
{
    ready, login_missing, not_started, worker_still_running,
    circuit_not_failed, stopping_or_handoff, target_not_verified,
    cooldown_wait
};
[[nodiscard]] constexpr LoadingCircuitRetryReason loading_retry_reason(
    bool authenticated,bool circuit_started,bool final_failed,
    bool worker_finished,bool stop_requested,bool valid_target,
    bool cooldown_expired) noexcept
{
    if(!authenticated)return LoadingCircuitRetryReason::login_missing;
    if(!circuit_started)return LoadingCircuitRetryReason::not_started;
    if(!worker_finished)
        return LoadingCircuitRetryReason::worker_still_running;
    if(!final_failed)return LoadingCircuitRetryReason::circuit_not_failed;
    if(stop_requested)
        return LoadingCircuitRetryReason::stopping_or_handoff;
    if(!valid_target)
        return LoadingCircuitRetryReason::target_not_verified;
    if(!cooldown_expired)
        return LoadingCircuitRetryReason::cooldown_wait;
    return LoadingCircuitRetryReason::ready;
}
[[nodiscard]] constexpr const char* loading_retry_reason_label(
    LoadingCircuitRetryReason reason) noexcept
{
    switch(reason)
    {
        case LoadingCircuitRetryReason::ready:
            return "Neuer Versuch moeglich";
        case LoadingCircuitRetryReason::login_missing:
            return "Nicht angemeldet";
        case LoadingCircuitRetryReason::not_started:
            return "LLUDP-Verbindung wird vorbereitet";
        case LoadingCircuitRetryReason::worker_still_running:
            return "Aktueller Verbindungsversuch laeuft";
        case LoadingCircuitRetryReason::circuit_not_failed:
            return "Kein abgeschlossener Verbindungsfehler";
        case LoadingCircuitRetryReason::stopping_or_handoff:
            return "Regionswechsel oder Beenden laeuft";
        case LoadingCircuitRetryReason::target_not_verified:
            return "Simulatorziel nicht bestaetigt";
        case LoadingCircuitRetryReason::cooldown_wait:
            return "Wiederholung erst nach Wartezeit";
    }
    return "Status unbekannt";
}

// Manual retry only after the original LLUDP worker has exited.
// An active/handing-off circuit must never be restarted by the loading UI.
[[nodiscard]] constexpr bool can_retry_loading_circuit(
    bool authenticated,bool circuit_started,bool final_failed,
    bool worker_finished,bool stop_requested,bool valid_target,
    bool cooldown_expired) noexcept
{
    return loading_retry_reason(authenticated,circuit_started,
        final_failed,worker_finished,stop_requested,valid_target,
        cooldown_expired)==LoadingCircuitRetryReason::ready;
}
}
