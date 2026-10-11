#include "nexverse/assets/asset_scene_priority_feedback.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    AssetScenePriorityFeedback feedback;
    feedback.observe({"a","b","c","d"});
    if(feedback.churn_events()!=0U||feedback.loaded_bonus(8.0f)!=8.0f)
        return 1;
    feedback.observe({"a","b","c","d"});
    if(feedback.active())return 2;
    feedback.observe({"a","b","c","new"});
    if(!feedback.active()||feedback.churn_events()!=1U ||
       feedback.loaded_bonus(8.0f)!=14.0f)return 3;
    feedback.observe({"a","b","c","new"});
    if(!feedback.active())return 4;
    feedback.observe({"a","b","c","new"});
    feedback.observe({"a","b","c","new"});
    if(feedback.active()||feedback.loaded_bonus(8.0f)!=8.0f)return 5;
    feedback.reset();
    feedback.observe({"x"});
    if(feedback.active()||feedback.churn_events()!=0U)return 6;
    std::cout<<"Scene churn soft residency bonus, recovery and epoch reset OK\n";
    return 0;
}
