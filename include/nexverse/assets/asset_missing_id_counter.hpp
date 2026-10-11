/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/asset_failure.hpp"
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_set>
namespace nexverse::assets
{
// Track unique missing UUIDs within ONE region. Only aggregate counters
// leave the process; never write these identifiers in a JSON health record.
// Limit prevents unbounded memory growth if a simulator sends bad IDs.
class AssetMissingIdCounter
{
public:
    explicit AssetMissingIdCounter(std::size_t limit=4096U) : limit_{limit} {}
    void observe(std::string_view uuid,AssetFailure failure)
    {
        if(failure!=AssetFailure::missing ||uuid.empty())return;
        ++events_;
        if(ids_.contains(std::string{uuid}))return;
        if(ids_.size()>=limit_){++overflow_;return;}
        ids_.emplace(uuid);
    }
    void reset()
    {
        ids_.clear();
        events_=0U;
        overflow_=0U;
    }
    [[nodiscard]] std::size_t unique() const noexcept{return ids_.size();}
    [[nodiscard]] std::size_t events() const noexcept{return events_;}
    [[nodiscard]] std::size_t overflow() const noexcept{return overflow_;}
private:
    std::size_t limit_;
    std::size_t events_=0U;
    std::size_t overflow_=0U;
    std::unordered_set<std::string> ids_;
};
}
