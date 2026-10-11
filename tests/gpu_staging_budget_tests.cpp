#include "nexverse/assets/gpu_staging_budget.hpp"
#include <iostream>
#include <limits>
int main()
{
    using namespace nexverse::assets;
    constexpr std::size_t mib=1024U*1024U;
    auto ordinary=gpu_staging_budget(100*mib,110*mib,16*mib,384*mib);
    if(!ordinary.fits || ordinary.reserve_bytes!=12*mib ||
       ordinary.transient_peak_bytes!=226*mib ||
       ordinary.maximum_safe_bytes!=372*mib)return 1;
    auto peak=gpu_staging_budget(240*mib,128*mib,8*mib,384*mib);
    if(peak.fits || peak.transient_peak_bytes!=0U)return 2;
    auto edge=gpu_staging_budget(240*mib,128*mib,4*mib,384*mib);
    if(!edge.fits || edge.transient_peak_bytes!=372*mib)return 3;
    if(gpu_staging_budget(0U,0U,1U,0U).fits)return 4;
    if(!gpu_staging_budget(0U,0U,0U,0U).fits)return 5;
    if(gpu_staging_budget(std::numeric_limits<std::size_t>::max(),
                         100U,20U,384*mib).fits)return 6;
    auto small=gpu_staging_budget(11U,11U,10U,32U);
    if(small.fits || small.reserve_bytes!=1U)return 7;
    std::cout<<"Transactional GPU peak, reserve and overflow guard OK\n";
    return 0;
}
