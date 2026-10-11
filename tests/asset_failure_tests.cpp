#include "nexverse/assets/asset_failure.hpp"
#include <chrono>
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    using nexverse::net::HttpResponse;
    if(classify_http_asset_failure({200,"",{}})!=AssetFailure::none ||
       classify_http_asset_failure({404,{},"not found"})!=AssetFailure::missing ||
       classify_http_asset_failure({410,{},"gone"})!=AssetFailure::missing ||
       classify_http_asset_failure({429,{},"busy"})!=AssetFailure::transient ||
       classify_http_asset_failure({503,{},"server"})!=AssetFailure::transient ||
       classify_http_asset_failure({0,{},"timeout"})!=AssetFailure::transient ||
       classify_http_asset_failure({403,{},"forbidden"})!=AssetFailure::configuration ||
       classify_http_asset_failure({422,{},"bad"})!=AssetFailure::invalid)
        return 1;
    if(merge_asset_failure(AssetFailure::missing,AssetFailure::transient)
         !=AssetFailure::transient ||
       merge_asset_failure(AssetFailure::none,AssetFailure::missing)
         !=AssetFailure::missing)
        return 2;
    AssetRetrySchedule schedule;
    schedule.enter(15);
    const auto start=AssetRetrySchedule::Clock::time_point{seconds{1000}};
    if(!schedule.ready("a",start))return 3;
    for(int i=0;i<3;++i)
    {
        const auto delay=schedule.failure("a",AssetFailure::transient,start);
        if(delay!=seconds{2<<i} || schedule.ready("a",start+delay-milliseconds{1}) ||
           !schedule.ready("a",start+delay))return 4;
    }
    if(schedule.failure("a",AssetFailure::transient,start)!=minutes{5})
        return 5;
    if(schedule.failure("missing",AssetFailure::missing,start)!=minutes{10} ||
       schedule.failure("invalid",AssetFailure::invalid,start)!=minutes{30} ||
       schedule.failure("auth",AssetFailure::configuration,start)!=minutes{5})
        return 6;
    if(schedule.count()!=4U ||
       schedule.count_kind(AssetFailure::transient)!=1U)return 7;
    schedule.success("a");
    if(!schedule.ready("a",start) || schedule.failed("a"))return 8;
    schedule.enter(15);
    if(schedule.count()!=3U)return 9;
    schedule.enter(16);
    if(schedule.count()!=0U || !schedule.ready("missing",start))return 10;
    if(schedule.failure("a",AssetFailure::transient,start)!=seconds{2})
        return 11;
    if(schedule.failure("a",AssetFailure::missing,start)!=minutes{10})
        return 12;
    std::cout<<"Asset HTTP failure classification and epoch-local backoff OK\n";
    return 0;
}
