/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_cof_change_plan.hpp"
#include "nexverse/protocol/firestorm_outfit_mutation.hpp"
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
namespace nexverse::protocol
{
enum class CofLinkPhase
{
    idle,confirmation_required,waiting_server,verified,
    not_confirmed,stale_or_cancelled
};
class CofSingleLinkConsent
{
public:
    using Clock=std::chrono::steady_clock;
    static constexpr auto confirmation_timeout=std::chrono::seconds{20};
    static constexpr auto server_timeout=std::chrono::seconds{18};
    [[nodiscard]] CofLinkPhase phase() const noexcept {return phase_;}
    [[nodiscard]] bool awaiting_server() const noexcept
    {return phase_==CofLinkPhase::waiting_server;}
    [[nodiscard]] std::uint64_t epoch() const noexcept {return epoch_;}
    [[nodiscard]] std::string_view target() const noexcept {return target_;}
    [[nodiscard]] const std::string& status() const noexcept {return status_;}
    void reset(std::string message="Bereitschaft nach Region-/Sessionwechsel")
    {
        phase_=CofLinkPhase::stale_or_cancelled;
        epoch_=0U;cof_version_=0U;target_.clear();command_={};
        status_=std::move(message);
    }
    [[nodiscard]] bool stage(
        const CofChangePlan& plan,std::string_view cof_folder,
        std::uint64_t epoch,Clock::time_point now)
    {
        // An addition alone is the only permitted mutation, and it
        // cannot remove a previous outfit link or bypass version scope.
        if(phase_==CofLinkPhase::not_confirmed||
           awaiting_server()||!plan.preview_ready()||
           plan.add.size()!=1U||!plan.remove.empty()||
           epoch==0U||plan.cof_version==0U)
        {
            status_="Nur genau EIN neuer Link ohne Loeschung erlaubt";
            return false;
        }
        CofLinkCommand proposed{std::string{cof_folder},
            plan.add.front().target_id,plan.add.front().name,
            "OpenGenesisLINK Viewer Outfit-Link",1U,
            static_cast<std::int8_t>(plan.add.front().inventory_type)};
        // Validate exactly what the wire packet encoder will accept,
        // but do not transmit on the first click.
        LoginSession synthetic;
        synthetic.agent_id="11111111-1111-1111-1111-111111111111";
        synthetic.session_id="22222222-2222-2222-2222-222222222222";
        if(!uuid_to_bytes(cof_folder)||
           plan.add.front().inventory_type<0||
           plan.add.front().inventory_type>127||
           encode_cof_link(1U,synthetic,proposed).empty())
        {
            status_="Link-Paket ungueltig (keine Aenderung)";
            return false;
        }
        command_=std::move(proposed);
        target_=command_.linked_item_id;
        cof_version_=plan.cof_version;epoch_=epoch;
        started_=now;phase_=CofLinkPhase::confirmation_required;
        status_="Bestaetigen: nochmals auf Link anlegen klicken (20s)";
        return true;
    }
    [[nodiscard]] std::optional<CofLinkCommand> confirm(
        const CofChangePlan& current,std::uint64_t epoch,
        std::string_view cof_folder,Clock::time_point now,
        std::uint32_t callback_id)
    {
        if(phase_!=CofLinkPhase::confirmation_required)return std::nullopt;
        if(epoch!=epoch_||now<started_||
           now-started_>confirmation_timeout||
           !current.preview_ready()||current.cof_version!=cof_version_||
           current.add.size()!=1U||!current.remove.empty()||
           current.add.front().target_id!=target_||
           command_.folder_id!=cof_folder||callback_id==0U)
        {
            reset("Bestaetigung abgebrochen: COF/Region/Frist geaendert");
            return std::nullopt;
        }
        command_.callback_id=callback_id;
        const auto ready=command_;
        phase_=CofLinkPhase::waiting_server;
        started_=now;
        status_="Link in Sendewarteschlange; COF-Readback ausstehend";
        return ready;
    }
    void failed_to_queue()
    {
        reset("Senden fehlgeschlagen: Inventar unveraendert");
    }
    void observe(std::uint64_t epoch,std::uint64_t server_version,
        const std::vector<OutfitInventoryItem>* server_items,
        Clock::time_point now)
    {
        if(phase_==CofLinkPhase::confirmation_required)
        {
            if(epoch!=epoch_||now<started_||
               now-started_>confirmation_timeout)
                reset("Bestaetigung verfallen");
            return;
        }
        if(phase_!=CofLinkPhase::waiting_server)return;
        if(epoch!=epoch_)
        {
            reset("Region gewechselt; Server-Ergebnis unbestimmt");
            return;
        }
        if(server_version>cof_version_&&server_items)
        {
            for(const auto& item:*server_items)
                if(item.asset_type==24&&item.asset_id==target_&&
                   uuid_to_bytes(item.item_id))
                {
                    phase_=CofLinkPhase::verified;
                    status_="Link vom Server bestaetigt (Avatar-Appearance offen)";
                    return;
                }
        }
        if(now<started_||now-started_>server_timeout)
        {
            phase_=CofLinkPhase::not_confirmed;
            status_="Keine COF-Bestaetigung; nicht erneut automatisch senden";
        }
    }
private:
    CofLinkPhase phase_=CofLinkPhase::idle;
    CofLinkCommand command_;
    std::string target_;
    std::string status_="Keine COF-Aktion";
    std::uint64_t epoch_=0U,cof_version_=0U;
    Clock::time_point started_{};
};
}
