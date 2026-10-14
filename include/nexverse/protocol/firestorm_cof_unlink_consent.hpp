/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_cof_change_plan.hpp"
#include "nexverse/protocol/firestorm_outfit_mutation.hpp"
#include <chrono>
#include <optional>
#include <set>
#include <string>
namespace nexverse::protocol
{
enum class CofUnlinkPhase
{
    idle,confirmation_required,waiting_server,verified,
    unconfirmed,stale_region
};
class CofSingleUnlinkConsent
{
public:
    using Clock=std::chrono::steady_clock;
    [[nodiscard]] CofUnlinkPhase phase() const noexcept {return phase_;}
    [[nodiscard]] bool active() const noexcept
    {return phase_==CofUnlinkPhase::confirmation_required||
            phase_==CofUnlinkPhase::waiting_server;}
    [[nodiscard]] bool awaiting_server() const noexcept
    {return phase_==CofUnlinkPhase::waiting_server;}
    [[nodiscard]] const std::string& status() const noexcept {return status_;}
    void reset(std::string status="COF-Entfernung nach Regionwechsel abgebrochen")
    {
        phase_=CofUnlinkPhase::stale_region;region_=0U;version_=0U;
        folder_.clear();link_id_.clear();removed_target_.clear();
        remaining_.clear();status_=std::move(status);
    }
    [[nodiscard]] bool stage(const CofChangePlan& plan,
        std::string_view cof_folder,std::uint64_t region,
        Clock::time_point now)
    {
        if(active()||phase_==CofUnlinkPhase::unconfirmed)return false;
        // Never strip the last link, and never mix deletion with additions.
        const auto valid_uuid=[](std::string_view input) {
            const auto id=uuid_to_bytes(input);
            return id&&std::any_of(id->begin(),id->end(),
                [](std::uint8_t b){return b!=0U;});
        };
        if(!plan.preview_ready()||plan.remove.size()!=1U||
           !plan.add.empty()||plan.desired_targets.empty()||
           !valid_uuid(cof_folder)||region==0U||
           plan.cof_version==0U||
           !valid_uuid(plan.remove.front().link_id)||
           !valid_uuid(plan.remove.front().target_id))
        {status_="Nur ein entfernbarer Link bei erhaltenem COF erlaubt";return false;}
        folder_=std::string{cof_folder};
        link_id_=plan.remove.front().link_id;
        removed_target_=plan.remove.front().target_id;
        remaining_=plan.desired_targets;version_=plan.cof_version;
        region_=region;started_=now;
        phase_=CofUnlinkPhase::confirmation_required;
        status_="Link entfernen erneut bestaetigen (20s, ohne Auto-Undo)";
        return true;
    }
    [[nodiscard]] std::optional<CofUnlinkCommand> confirm(
        const CofChangePlan& live,std::string_view folder,
        std::uint64_t region,Clock::time_point now)
    {
        if(phase_!=CofUnlinkPhase::confirmation_required)
            return std::nullopt;
        if(region_!=region||folder_!=folder||
           live.cof_version!=version_||!live.preview_ready()||
           !live.add.empty()||live.remove.size()!=1U||
           live.remove.front().link_id!=link_id_||
           live.remove.front().target_id!=removed_target_||
           live.desired_targets!=remaining_||
           now<started_||now-started_>std::chrono::seconds{20})
        {
            reset("Entfernung abgebrochen: COF/Region/Frist geaendert");
            return std::nullopt;
        }
        phase_=CofUnlinkPhase::waiting_server;started_=now;
        status_="Link-Entfernung im Sendepfad: COF-Readback abwarten";
        return CofUnlinkCommand{link_id_};
    }
    void queue_failed()
    {reset("Link-Entfernung nicht eingereiht");}
    void observe(std::uint64_t region,std::uint64_t server_version,
        const std::vector<OutfitInventoryItem>* server_items,
        Clock::time_point now)
    {
        if(phase_==CofUnlinkPhase::confirmation_required)
        {
            if(region!=region_||now<started_||
               now-started_>std::chrono::seconds{20})
                reset("Link-Entfernung Bestaetigungsfrist abgelaufen");
            return;
        }
        if(phase_!=CofUnlinkPhase::waiting_server)return;
        if(region!=region_)
        {reset("Region gewechselt: Loeschungsergebnis unbekannt");return;}
        if(server_version>version_&&server_items)
        {
            std::set<std::string> observed,ids;
            bool valid=true;
            for(const auto& item:*server_items)
            {
                const auto valid_id=[](std::string_view input) {
                    const auto uuid=uuid_to_bytes(input);
                    return uuid&&std::any_of(uuid->begin(),uuid->end(),
                        [](std::uint8_t b){return b!=0U;});
                };
                if(item.asset_type!=24||!valid_id(item.item_id)||
                   !valid_id(item.asset_id)||
                   !ids.insert(item.item_id).second||
                   !observed.insert(item.asset_id).second||
                   item.item_id==link_id_)
                {valid=false;break;}
            }
            if(valid&&observed==remaining_)
            {
                phase_=CofUnlinkPhase::verified;
                status_="COF-Link vom Server entfernt; Avatar-Appearance offen";
                return;
            }
            phase_=CofUnlinkPhase::unconfirmed;
            status_="COF nach Entfernung geaendert: manuell pruefen";
            return;
        }
        if(now<started_||now-started_>std::chrono::seconds{18})
        {
            phase_=CofUnlinkPhase::unconfirmed;
            status_="Entfernung nicht bestaetigt; kein automatischer Retry";
        }
    }
private:
    CofUnlinkPhase phase_=CofUnlinkPhase::idle;
    std::string folder_,link_id_,removed_target_,status_="Kein Link entfernt";
    std::set<std::string> remaining_;
    std::uint64_t region_=0U,version_=0U;
    Clock::time_point started_{};
};
}
