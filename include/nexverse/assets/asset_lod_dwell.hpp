/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <utility>
namespace nexverse::assets
{
// A new asset uses its target LOD immediately; only changes to an already
// resident asset must remain stable for the configured dwell interval.
template <typename Quality>
class AssetLodDwell
{
public:
    using Clock = std::chrono::steady_clock;
    explicit AssetLodDwell(
        std::chrono::milliseconds dwell = std::chrono::milliseconds{1400})
        : dwell_{dwell} {}
    [[nodiscard]] Quality resolve(const std::string& id, Quality target,
        const std::unordered_map<std::string, Quality>& resident,
        Clock::time_point now)
    {
        const auto loaded = resident.find(id);
        if (loaded==resident.end() || loaded->second==target)
        {
            pending_.erase(id);
            return target;
        }
        const auto it=pending_.find(id);
        if(it==pending_.end() || it->second.target!=target)
        {
            pending_.insert_or_assign(id, Entry{target,now});
            return loaded->second;
        }
        if(now-it->second.since<dwell_)return loaded->second;
        pending_.erase(it);
        return target;
    }
    void prune(const std::unordered_map<std::string,Quality>& desired)
    {
        for(auto it=pending_.begin();it!=pending_.end();)
        {
            if(!desired.contains(it->first))it=pending_.erase(it);
            else ++it;
        }
    }
    void reset() { pending_.clear(); }
    [[nodiscard]] std::size_t waiting() const noexcept
    {return pending_.size();}
private:
    struct Entry {Quality target;Clock::time_point since;};
    std::chrono::milliseconds dwell_;
    std::unordered_map<std::string,Entry> pending_;
};
}
