/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/avatar_bakes.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

namespace nexverse::assets
{
// Tracks only current-simulator bake source changes. It never directly
// fetches assets, bypasses HTTP backoff, or uploads a client-generated skin.
// A changed source simply schedules the EXISTING texture residency planner.
class AvatarBakeSourceRefresh
{
public:
    using Body=std::array<std::string,3U>;

    // No source is retained without a valid current authenticated lease.
    // Epoch transitions always reset the last source; the caller already
    // resets the texture request scope separately on teleport/relogin.
    [[nodiscard]] bool observe(std::uint64_t epoch, bool authenticated,
                               bool lease_current,const Body& source)
    {
        if(epoch==0U || !authenticated || !lease_current)
        {
            if(epoch_!=0U || has_source_)clear();
            return false;
        }
        if(epoch_!=epoch)
        {
            clear();
            epoch_=epoch;
        }
        Body next=source;
        for(auto& id:next)
            if(id=="00000000-0000-0000-0000-000000000000" ||
               std::find(protocol::baked_placeholders.begin(),
                         protocol::baked_placeholders.end(),id)!=
                   protocol::baked_placeholders.end())
                id.clear();
        if(has_source_&&next==source_)return false;
        const bool old_present=has_source_&&
            (!source_[0].empty()||!source_[1].empty()||
             !source_[2].empty());
        source_=std::move(next);
        has_source_=true;
        // On removal, also replan immediately to release the old
        // appearance's GPU residency. The planner—not this tracker—
        // decides whether any new GetTexture request is necessary.
        return old_present||!source_[0].empty()||!source_[1].empty()||
               !source_[2].empty();
    }
    [[nodiscard]] bool source_known() const noexcept {return has_source_;}
    void clear() noexcept
    {
        epoch_=0U;
        source_={};
        has_source_=false;
    }
private:
    std::uint64_t epoch_=0U;
    Body source_{};
    bool has_source_=false;
};
}
