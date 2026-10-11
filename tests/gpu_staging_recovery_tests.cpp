#include "nexverse/assets/gpu_staging_recovery.hpp"
#include <chrono>
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    GpuStagingRecovery recovery;
    const auto t=GpuStagingRecovery::Clock::time_point{seconds{100}};
    recovery.defer("tex-A",t,300,40);
    if(recovery.pending()!=1U ||
       !recovery.ready(t+seconds{1},300,40).empty())return 1;
    auto released=recovery.ready(t+seconds{1},299,40);
    if(released.size()!=1U||released[0]!="tex-A" ||
       recovery.pending()!=0U)return 2;
    recovery.defer("tex-A",t+seconds{3},299,40);
    if(!recovery.ready(t+seconds{4},299,40).empty())return 3;
    if(!recovery.ready(t+seconds{5},299,40).empty())return 4;
    released=recovery.ready(t+seconds{7},299,40);
    if(released.size()!=1U)return 9;
    recovery.defer("tex-A",t+seconds{8},299,40);
    recovery.defer("tex-B",t+seconds{8},299,40);
    released=recovery.ready(t+seconds{9},299,39);
    if(released.size()!=2U||recovery.pending()!=0U)return 5;
    if(recovery.released()!=4U)return 6;
    recovery.defer("tex-C",t+seconds{8},100,10);
    recovery.resolved("tex-C");
    if(recovery.pending()!=0U)return 7;
    recovery.defer("tex-D",t+seconds{9},10,10);
    std::unordered_set<std::string> blocked{"tex-D","obsolete"};
    blocked.insert("orphan");
    recovery.synchronize(blocked,{"tex-D","orphan"});
    if(recovery.pending()!=1U||blocked.size()!=1U ||
       !blocked.contains("tex-D")||blocked.contains("orphan"))return 11;
    recovery.synchronize(blocked,{"other"});
    if(recovery.pending()!=0U||!blocked.empty())return 10;
    recovery.reset();
    if(recovery.pending()!=0U||recovery.released()!=0U)return 8;
    std::cout<<"Deferred GPU staging retry: time, memory drop, resolution, epoch reset OK\n";
    return 0;
}
