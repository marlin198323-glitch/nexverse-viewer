/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_cof_change_plan.hpp"
#include "nexverse/protocol/firestorm_outfit_mutation.hpp"
#include <chrono>
#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <vector>
namespace nexverse::protocol
{
enum class CofBatchPhase
{
    idle,awaiting_consent,awaiting_queue,awaiting_readback,
    confirmed_links,partial_or_unknown,stale_region
};
class CofBatchAdd
{
public:
    using Clock=std::chrono::steady_clock;
    [[nodiscard]] CofBatchPhase phase() const noexcept {return phase_;}
    [[nodiscard]] bool active() const noexcept
    {return phase_==CofBatchPhase::awaiting_consent||
            phase_==CofBatchPhase::awaiting_queue||
            phase_==CofBatchPhase::awaiting_readback;}
    [[nodiscard]] bool awaiting_readback() const noexcept
    {return phase_==CofBatchPhase::awaiting_readback;}
    [[nodiscard]] std::size_t done() const noexcept {return confirmed_;}
    [[nodiscard]] std::size_t total() const noexcept {return operations_.size();}
    [[nodiscard]] const std::string& status() const noexcept {return status_;}
    void reset(std::string reason="Batch nach Regionswechsel ungueltig")
    {
        phase_=CofBatchPhase::stale_region;operations_.clear();
        expected_.clear();cof_folder_.clear();
        region_epoch_=0U;version_=0U;confirmed_=0U;
        status_=std::move(reason);
    }
    [[nodiscard]] bool stage(const CofChangePlan& plan,
        std::string_view cof_folder,std::uint64_t region_epoch,
        Clock::time_point now)
    {
        // Unknown partial success MUST be reconciled by a fresh
        // operator-requested COF read before any further mutation.
        if(active()||phase_==CofBatchPhase::partial_or_unknown)return false;
        if(!plan.preview_ready()||plan.add.size()<2U||
           plan.add.size()>4U||!plan.remove.empty()||
           plan.cof_version==0U||region_epoch==0U||
           !uuid_to_bytes(cof_folder)||
           plan.desired_targets.size()<=plan.add.size())
        {status_="Nur 2-4 neue Links ohne Loeschungen zulaessig";return false;}
        std::vector<CofLinkAddition> validated;
        LoginSession dummy;
        dummy.agent_id="11111111-1111-1111-1111-111111111111";
        dummy.session_id="22222222-2222-2222-2222-222222222222";
        for(const auto& item:plan.add)
        {
            if(item.inventory_type<0||item.inventory_type>127||
               encode_cof_link(1U,dummy,CofLinkCommand{
                std::string{cof_folder},item.target_id,item.name,
                "OpenGenesisLINK Viewer COF Link",1U,
                static_cast<std::int8_t>(item.inventory_type)}).empty())
            {status_="Batch enthaelt ungueltiges Inventaritem";return false;}
            validated.push_back(item);
        }
        std::set<std::string> base=plan.desired_targets;
        for(const auto& item:validated)
            if(base.erase(item.target_id)!=1U)
            {status_="COF Plan-Konflikt: Ziel nicht eindeutig";return false;}
        operations_=std::move(validated);
        expected_=std::move(base);
        cof_folder_=std::string{cof_folder};
        region_epoch_=region_epoch;version_=plan.cof_version;
        confirmed_=0U;phase_=CofBatchPhase::awaiting_consent;
        started_=now;
        status_="Batch "+std::to_string(operations_.size())+
            " Links: erneut klicken zum Bestaetigen (20s)";
        return true;
    }
    [[nodiscard]] std::optional<CofLinkCommand> confirm(
        const CofChangePlan& live,std::string_view cof_folder,
        std::uint64_t epoch,Clock::time_point now,
        std::uint32_t callback)
    {
        if(phase_!=CofBatchPhase::awaiting_consent)return std::nullopt;
        if(!matches(live,cof_folder,epoch)||
           now<started_||now-started_>std::chrono::seconds{20}||
           callback==0U)
        {reset("Batch-Bestaetigung verfallen oder COF geaendert");
         return std::nullopt;}
        phase_=CofBatchPhase::awaiting_queue;
        return command(callback);
    }
    // Never mark a command as sent before the queue accepts it.
    void queue_result(bool accepted,Clock::time_point now)
    {
        if(phase_!=CofBatchPhase::awaiting_queue)return;
        if(!accepted)
        {
            phase_=CofBatchPhase::partial_or_unknown;
            status_="Batch gestoppt nach "+std::to_string(confirmed_)+
                " bestaetigten Links; Queue voll – manuell COF pruefen";
            return;
        }
        phase_=CofBatchPhase::awaiting_readback;started_=now;
        status_="Batch "+std::to_string(confirmed_)+"/"+
            std::to_string(operations_.size())+
            ": warte auf Current-Outfit-Folder Serverantwort";
    }
    // Called only with new, independently fetched owner-bound COF.
    // Returns the NEXT wire command if and only if the PREVIOUS
    // link was observed, version advanced, and no unrelated link changed.
    [[nodiscard]] std::optional<CofLinkCommand> observe(
        std::uint64_t epoch,std::uint64_t server_version,
        const std::vector<OutfitInventoryItem>* server_items,
        Clock::time_point now,std::uint32_t next_callback)
    {
        if(!active())return std::nullopt;
        if(epoch!=region_epoch_)
        {reset("Region gewechselt: Batch-Status unbestimmt");
         return std::nullopt;}
        if(phase_==CofBatchPhase::awaiting_consent)
        {
            if(now<started_||now-started_>std::chrono::seconds{20})
                reset("Batch-Klickbestaetigung abgelaufen");
            return std::nullopt;
        }
        if(phase_!=CofBatchPhase::awaiting_readback)return std::nullopt;
        if(now<started_||now-started_>std::chrono::seconds{18})
        {
            phase_=CofBatchPhase::partial_or_unknown;
            status_="Batch unbestaetigt nach "+std::to_string(confirmed_)+
                " von "+std::to_string(operations_.size())+
                " Links. Kein Auto-Resend; COF manuell pruefen";
            return std::nullopt;
        }
        if(!server_items||server_version<=version_)
            return std::nullopt;
        std::set<std::string> observed;
        std::set<std::string> item_ids;
        for(const auto& item:*server_items)
        {
            const auto valid=[](const std::string& str) {
                const auto uuid=uuid_to_bytes(str);
                return uuid&&std::any_of(uuid->begin(),uuid->end(),
                    [](std::uint8_t b){return b!=0U;});
            };
            if(item.asset_type!=24||!valid(item.item_id)||
               !valid(item.asset_id)||
               !item_ids.insert(item.item_id).second||
               !observed.insert(item.asset_id).second)
            {
                phase_=CofBatchPhase::partial_or_unknown;
                status_="Batch angehalten: Server-COF-Linkbestand ungueltig";
                return std::nullopt;
            }
        }
        auto expected=expected_;
        expected.insert(operations_[confirmed_].target_id);
        if(observed!=expected)
        {
            phase_=CofBatchPhase::partial_or_unknown;
            status_="Batch angehalten: COF unterscheidet sich vom Plan";
            return std::nullopt;
        }
        expected_=std::move(expected);
        version_=server_version;
        ++confirmed_;
        if(confirmed_==operations_.size())
        {
            phase_=CofBatchPhase::confirmed_links;
            status_=std::to_string(confirmed_)+
                " COF-Links serverseitig bestaetigt; Avatar-Appearance offen";
            return std::nullopt;
        }
        if(next_callback==0U)
        {
            phase_=CofBatchPhase::partial_or_unknown;
            status_="Batch gestoppt: Callback-ID ungueltig";
            return std::nullopt;
        }
        phase_=CofBatchPhase::awaiting_queue;
        return command(next_callback);
    }
private:
    [[nodiscard]] CofLinkCommand command(std::uint32_t callback) const
    {
        const auto& operation=operations_[confirmed_];
        return {cof_folder_,operation.target_id,operation.name,
            "OpenGenesisLINK Viewer COF Link",callback,
            static_cast<std::int8_t>(operation.inventory_type)};
    }
    [[nodiscard]] bool matches(const CofChangePlan& live,
        std::string_view folder,std::uint64_t epoch) const
    {
        if(!live.preview_ready()||live.cof_version!=version_||
           epoch!=region_epoch_||folder!=cof_folder_||
           live.add.size()!=operations_.size()||
           !live.remove.empty())return false;
        for(std::size_t i=0U;i<operations_.size();++i)
            if(live.add[i].target_id!=operations_[i].target_id||
               live.add[i].name!=operations_[i].name||
               live.add[i].inventory_type!=operations_[i].inventory_type)
                return false;
        return true;
    }
    CofBatchPhase phase_=CofBatchPhase::idle;
    std::string cof_folder_,status_="Kein Batch gestartet";
    std::vector<CofLinkAddition> operations_;
    std::set<std::string> expected_;
    std::uint64_t region_epoch_=0U,version_=0U;
    std::size_t confirmed_=0U;
    Clock::time_point started_{};
};
}
