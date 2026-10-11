#include "nexverse/assets/asset_gpu_budget.hpp"
#include <iostream>
#include <limits>
int main()
{
    using namespace nexverse::assets;
    constexpr std::size_t mib=1024U*1024U;
    if(effective_gpu_family_budget(256*mib,384*mib,0U)!=256*mib)
        return 1;
    if(effective_gpu_family_budget(256*mib,384*mib,180*mib)!=204*mib)
        return 2;
    if(effective_gpu_family_budget(256*mib,384*mib,400*mib)!=0U)
        return 3;
    if(!can_replace_gpu_bytes(250,100,100,256) ||
       !can_replace_gpu_bytes(250,100,106,256) ||
       can_replace_gpu_bytes(250,100,107,256))
        return 4;
    if(can_replace_gpu_bytes(99,100,10,256) ||
       can_replace_gpu_bytes(100,0,257,256) ||
       can_replace_gpu_bytes(
            std::numeric_limits<std::size_t>::max(),0,1,256))
        return 5;
    std::cout<<"Shared estimated GPU family-budget arithmetic OK\n";
    return 0;
}
