/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>
namespace nexverse::assets
{
// Bounded feedback to loaded-asset retention when desired UUIDs churn.
// Retaining an already loaded UUID is a *soft* priority bias, never a
// hard pin: a genuinely nearby new asset can still replace it.
class AssetScenePriorityFeedback
{
public:
    void observe(const std::unordered_set<std::string>& wanted)
    {
        if(!initialized_)
        {
            previous_=wanted;initialized_=true;return;
        }
        std::size_t changed=0;
        for(const auto& id:previous_)
            if(!wanted.contains(id))++changed;
        for(const auto& id:wanted)
            if(!previous_.contains(id))++changed;
        const auto baseline=std::max(previous_.size(),wanted.size());
        if(baseline>0U && changed*4U>=baseline)
        {
            pressure_=3U;
            ++churn_events_;
        }
        else if(pressure_>0U)--pressure_;
        previous_=wanted;
    }
    [[nodiscard]] float loaded_bonus(float baseline) const noexcept
    {
        return baseline*(pressure_>0U ? 1.75f : 1.0f);
    }
    [[nodiscard]] std::size_t churn_events() const noexcept
    {return churn_events_;}
    [[nodiscard]] bool active() const noexcept{return pressure_>0U;}
    void reset()
    {
        initialized_=false;previous_.clear();pressure_=0U;
        churn_events_=0U;
    }
private:
    bool initialized_=false;
    std::unordered_set<std::string> previous_;
    unsigned pressure_=0U;
    std::size_t churn_events_=0U;
};
}
