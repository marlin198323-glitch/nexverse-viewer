#include "nexverse/assets/asset_upload_telemetry.hpp"
#include <chrono>
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    AssetUploadTelemetry stats;
    if(stats.recent_count()!=0U||stats.recent_p95_us()!=0U)return 1;
    stats.observe(microseconds{100});
    stats.observe(microseconds{200});
    stats.observe(microseconds{300});
    if(stats.recent_avg_us()!=200U ||stats.recent_p95_us()!=300U||
       stats.recent_max_us()!=300U||stats.total_count()!=3U)return 2;
    for(int i=0;i<80;++i)stats.observe(microseconds{1000});
    if(stats.recent_count()!=64U||stats.total_count()!=83U||
       stats.recent_p95_us()!=1000U||stats.recent_avg_us()!=1000U)return 3;
    stats.observe(microseconds{-25});
    if(stats.recent_count()!=64U||stats.recent_max_us()!=1000U)return 4;
    stats.reset();
    if(stats.recent_count()!=0U||stats.total_count()!=0U||
       stats.recent_max_us()!=0U)return 5;
    std::cout<<"Upload CPU time rolling avg/p95/max, bounded samples OK\n";
    return 0;
}
