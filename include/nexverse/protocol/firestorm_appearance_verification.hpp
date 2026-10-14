/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <chrono>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
namespace nexverse::protocol
{
enum class CofAppearancePhase
{
    idle,cof_pending,appearance_pending,appearance_stale,
    appearance_newer,appearance_matched,expired,region_invalid
};
class CofAppearanceVerifier
{
public:
    using Clock=std::chrono::steady_clock;
    [[nodiscard]] CofAppearancePhase phase() const noexcept{return phase_;}
    [[nodiscard]] const std::string& status() const noexcept{return status_;}
    [[nodiscard]] std::uint32_t target_version() const noexcept
    {return target_version_;}
    [[nodiscard]] bool active() const noexcept
    {return phase_==CofAppearancePhase::cof_pending||
        phase_==CofAppearancePhase::appearance_pending||
        phase_==CofAppearancePhase::appearance_stale||
        phase_==CofAppearancePhase::appearance_newer;}
    void reset(std::string reason="Appearance: Region/Sitzung gewechselt")
    {
        phase_=CofAppearancePhase::region_invalid;
        epoch_=0U;target_version_=0U;
        status_=std::move(reason);
    }
    void begin(std::uint64_t epoch,std::uint64_t last_seen_serial,
               Clock::time_point now)
    {
        if(epoch==0U)return;
        epoch_=epoch;baseline_serial_=last_seen_serial;
        target_version_=0U;started_=now;
        phase_=CofAppearancePhase::cof_pending;
        status_="COF-Mutation gesendet; Server-COF noch nicht bestaetigt";
    }
    [[nodiscard]] bool confirm_cof(std::uint64_t epoch,
        std::uint64_t cof_version,Clock::time_point now)
    {
        if(!active()||phase_!=CofAppearancePhase::cof_pending||
           epoch!=epoch_||cof_version==0U||
           cof_version>static_cast<std::uint64_t>(
               std::numeric_limits<std::int32_t>::max()))
            return false;
        target_version_=static_cast<std::uint32_t>(cof_version);
        phase_=CofAppearancePhase::appearance_pending;
        started_=now;
        status_="COF bestaetigt, warte auf AvatarAppearance CofVersion "+
            std::to_string(target_version_);
        return true;
    }
    void observe(std::uint64_t epoch,std::uint64_t serial,
        std::string_view agent_id,
        const std::optional<AvatarAppearanceMessage>& appearance,
        Clock::time_point now)
    {
        if(!active())return;
        if(epoch!=epoch_)
        {reset();return;}
        if(phase_==CofAppearancePhase::cof_pending)
        {
            if(now<started_||now-started_>std::chrono::seconds{120})
            {
                phase_=CofAppearancePhase::expired;
                status_="COF-Mutation nicht binnen 120s bestaetigt";
            }
            return;
        }
        if(now<started_||now-started_>std::chrono::seconds{30})
        {
            phase_=CofAppearancePhase::expired;
            status_="Keine passende frische AvatarAppearance binnen 30s";
            return;
        }
        if(serial<=baseline_serial_||!appearance||
           appearance->avatar_id!=agent_id||appearance->trial||
           appearance->appearance_data.empty())
            return;
        bool newer=false;
        for(const auto& data:appearance->appearance_data)
        {
            if(data.cof_version==target_version_)
            {
                if(appearance->texture_entry.empty()||
                   appearance->visual_params.empty())
                {
                    phase_=CofAppearancePhase::appearance_stale;
                    status_="CofVersion passt, aber TextureEntry/VisualParams fehlen";
                }
                else
                {
                    phase_=CofAppearancePhase::appearance_matched;
                    status_="Simulator AvatarAppearance CofVersion "+
                        std::to_string(target_version_)+
                        " empfangen (sichtbarer Bake unbewiesen)";
                }
                return;
            }
            if(data.cof_version>target_version_&&
               data.cof_version!=UINT32_MAX)newer=true;
        }
        if(newer)
        {
            phase_=CofAppearancePhase::appearance_newer;
            status_="Simulator meldet neuere Appearance als erwarteter COF";
        }
        else
        {
            phase_=CofAppearancePhase::appearance_stale;
            status_="Simulator Appearance CofVersion noch nicht aktuell";
        }
    }
private:
    CofAppearancePhase phase_=CofAppearancePhase::idle;
    std::uint64_t epoch_=0U,baseline_serial_=0U;
    std::uint32_t target_version_=0U;
    Clock::time_point started_{};
    std::string status_="AvatarAppearance nach COF-Aenderung nicht geprueft";
};
}
