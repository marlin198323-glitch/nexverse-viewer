/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace nexverse::assets
{
// GPU staging deferrals are NOT permanent exclusions. Reconsider if
// *either* resident family shrinks, or after a bounded progressive
// cooldown. Expired entries are removed so the existing queue can retry.
class GpuStagingRecovery
{
public:
    using Clock=std::chrono::steady_clock;
    void defer(const std::string& id,Clock::time_point now,
               std::size_t texture_bytes,std::size_t mesh_bytes)
    {
        auto& item=deferred_[id];
        item.texture_bytes=texture_bytes;
        item.mesh_bytes=mesh_bytes;
        const auto attempts=attempts_[id];
        item.next=now+std::chrono::seconds{2U<<std::min(attempts,5U)};
        attempts_[id]=std::min(attempts+1U,6U);
    }
    [[nodiscard]] std::vector<std::string> ready(
        Clock::time_point now,std::size_t texture_bytes,
        std::size_t mesh_bytes)
    {
        std::vector<std::string> result;
        for(auto it=deferred_.begin();it!=deferred_.end();)
        {
            if(texture_bytes<it->second.texture_bytes ||
               mesh_bytes<it->second.mesh_bytes ||now>=it->second.next)
            {
                result.push_back(it->first);
                it=deferred_.erase(it);
                ++released_;
            }
            else ++it;
        }
        return result;
    }
    void resolved(const std::string& id)
    {
        deferred_.erase(id);
        attempts_.erase(id);
    }
    void prune(const std::unordered_set<std::string>& desired)
    {
        for(auto it=deferred_.begin();it!=deferred_.end();)
            if(!desired.contains(it->first))it=deferred_.erase(it);
            else ++it;
        for(auto it=attempts_.begin();it!=attempts_.end();)
            if(!desired.contains(it->first))it=attempts_.erase(it);
            else ++it;
    }
    void reset()
    {
        deferred_.clear();
        attempts_.clear();
        released_=0U;
    }
    [[nodiscard]] std::size_t pending() const noexcept
    {return deferred_.size();}
    [[nodiscard]] std::size_t released() const noexcept
    {return released_;}
private:
    struct State
    {
        Clock::time_point next{};
        std::size_t texture_bytes=0U;
        std::size_t mesh_bytes=0U;
    };
    std::unordered_map<std::string,State> deferred_;
    std::unordered_map<std::string,unsigned> attempts_;
    std::size_t released_=0U;
};
}
