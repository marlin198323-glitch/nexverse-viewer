/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
namespace nexverse::assets
{
// Read-only self-appearance bootstrap. Each authenticated owner+region
// lease+COF version is fetched at most once automatically; a manual F9
// retry is still possible. Never sends outfit mutations, new bakes,
// consent packets, or assumes a completed server AvatarAppearance.
class FirestormAutoCofRead
{
public:
    [[nodiscard]] bool begin(const std::string& owner,
        std::uint64_t epoch,const std::string& cof,
        std::uint64_t version)
    {
        if(owner.empty()||cof.empty()||epoch==0U||version==0U)
            return false;
        const auto next=std::make_tuple(owner,epoch,cof,version);
        if(std::find(attempts_.begin(),attempts_.end(),next)!=
           attempts_.end())return false;
        // At most 64 keys from the current region/login can be
        // remembered. Real region changes reset this state; if a
        // pathological session exceeds the bound, fail CLOSED rather
        // than re-request an old outfit on every frame.
        if(attempts_.size()>=64U)return false;
        attempts_.push_back(next);
        return true;
    }
    [[nodiscard]] bool matches(const std::string& owner,
        std::uint64_t epoch,const std::string& cof,
        std::uint64_t version) const
    {
        const auto key=std::make_tuple(owner,epoch,cof,version);
        return std::find(attempts_.begin(),attempts_.end(),key)!=
            attempts_.end();
    }
    void reset() {attempts_.clear();}
private:
    using Key=std::tuple<std::string,std::uint64_t,
                         std::string,std::uint64_t>;
    std::vector<Key> attempts_;
};
}
