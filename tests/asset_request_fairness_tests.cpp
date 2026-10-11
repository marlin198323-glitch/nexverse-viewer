#include "nexverse/assets/asset_request_fairness.hpp"
#include <chrono>
#include <iostream>
#include <string>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    const auto now=AssetRetrySchedule::Clock::time_point{seconds{1234}};
    AssetRetrySchedule retry;
    retry.enter(4);
    (void)retry.failure("old",AssetFailure::transient,now-seconds{12});
    AssetRequestFairness selector;
    const std::vector<std::string> priority{"old","new","new","other"};
    for(int i=0;i<3;++i)
    {
        const auto selected=selector.choose(priority,{}, {},retry,now);
        if(!selected || selected->id!="new" || selected->is_retry)return 1;
        selector.dispatched();
    }
    const auto fourth=selector.choose(priority,{}, {},retry,now);
    if(!fourth||fourth->id!="old"||!fourth->is_retry)return 2;
    selector.dispatched();
    if(selector.choose(priority,{}, {},retry,now)->id!="new")return 3;
    const auto only_retry=selector.choose(
        priority,{"new","other"},{},retry,now);
    if(!only_retry || only_retry->id!="old")return 4;
    const auto blocked=selector.choose(priority,{},{"old"},retry,now);
    if(!blocked || blocked->id!="new")return 5;
    const auto empty=selector.choose(priority,{"new","other"},
        {"old"},retry,now);
    if(empty)return 6;
    const auto filtered=selector.choose(priority,{}, {},retry,now,
        {},[](const std::string& id){return id=="other";});
    if(!filtered || filtered->id!="other")return 7;
    selector.reset();
    if(selector.dispatched_count()!=0U)return 8;

    retry.enter(5);
    (void)retry.failure("mesh.medium",AssetFailure::transient,
        now-seconds{10});
    const auto mesh=selector.choose({"mesh"}, {}, {},retry,now,
        [](const std::string& id){return id+".medium";});
    if(!mesh || mesh->retry_key!="mesh.medium" || !mesh->is_retry)
        return 9;
    std::cout<<"Asset fairness, retry no-starvation, deferred slots and LOD keys OK\n";
    return 0;
}
