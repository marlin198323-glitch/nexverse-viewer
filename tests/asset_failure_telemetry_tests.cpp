#include "nexverse/assets/asset_failure_telemetry.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    AssetFailureTelemetry events;
    events.observe(AssetFailure::missing);
    events.observe(AssetFailure::missing);
    events.observe(AssetFailure::transient);
    events.observe(AssetFailure::invalid);
    events.observe(AssetFailure::configuration);
    events.observe(AssetFailure::none);
    if(events.total()!=5U || events.missing!=2U ||
       events.transient!=1U||events.invalid!=1U||
       events.configuration!=1U)return 1;
    events={};
    if(events.total()!=0U)return 2;
    std::cout<<"Sanitized failure-category counters: missing/transient/invalid/config OK\n";
    return 0;
}
