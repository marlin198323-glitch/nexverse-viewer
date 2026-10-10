#include "nexverse/assets/texture_residency.hpp"
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using nexverse::assets::plan_texture_residency;

int main()
{
    std::vector<std::string> objects;
    objects.reserve(540U);
    for(int i=0;i<540;++i)
        objects.push_back("tex-"+std::to_string(i));
    auto initial=plan_texture_residency(
        {"fire","smoke","fire",""},objects,{},512U);
    if(initial.preferred_ids.size()!=512U ||
        initial.desired_ids.size()!=512U ||
        initial.preferred_ids[0]!="fire" ||
        initial.preferred_ids[1]!="smoke" ||
        initial.desired_ids.contains("tex-539") ||
        !initial.eviction_ids.empty())
    {
        std::cerr<<"Texture priority or budget mismatch\n";
        return 1;
    }
    std::unordered_set<std::string> loaded{
        initial.desired_ids.begin(),initial.desired_ids.end()};
    const auto moved=plan_texture_residency(
        {"fire","smoke","new-particle"},objects,loaded,512U);
    if(!moved.desired_ids.contains("new-particle") ||
       moved.eviction_ids.size()!=1U ||
       moved.eviction_ids[0]!="tex-509")
    {
        std::cerr<<"Unable to make room for newly relevant texture\n";
        return 2;
    }
    const auto empty=plan_texture_residency({}, {},loaded,0U);
    if(!empty.preferred_ids.empty() ||
       empty.eviction_ids.size()!=loaded.size())
    {
        std::cerr<<"Empty scene fails to release GPU textures\n";
        return 3;
    }
    const auto noop=plan_texture_residency(
        {"00000000-0000-0000-0000-000000000000"},
        {},{},512U);
    if(!noop.desired_ids.empty())
    {
        std::cerr<<"Zero UUID retained\n";
        return 4;
    }
    std::cout<<"Texture streaming residency, bounded eviction verified\n";
    return 0;
}
