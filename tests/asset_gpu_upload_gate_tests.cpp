#include "nexverse/assets/asset_gpu_upload_gate.hpp"
#include <chrono>
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    AssetGpuUploadGate gate{milliseconds{40}};
    const auto t=AssetGpuUploadGate::Clock::time_point{seconds{100}};
    if(!gate.ready(t))return 1;
    gate.uploaded(t,1024);
    if(gate.ready(t) || gate.ready(t+milliseconds{39}) ||
       !gate.ready(t+milliseconds{40}) ||
       gate.count()!=1 ||gate.bytes()!=1024)return 2;
    gate.reset();
    if(!gate.ready(t)||gate.count()!=1)return 3;
    gate.uploaded(t,1024,microseconds{22000});
    if(gate.effective_interval()!=milliseconds{88} ||
       gate.ready(t+milliseconds{87}) ||
       !gate.ready(t+milliseconds{88}))return 9;
    gate.uploaded(t+milliseconds{88},1024,microseconds{1000});
    if(gate.effective_interval()!=milliseconds{40})return 10;

    const auto fit=plan_texture_slot_admission({"new","a","b"},
        {"a","b"},{},"new",2);
    if(!fit.fits||fit.additional_evictions!=
        std::vector<std::string>({"b"}))return 4;
    const auto insufficient=plan_texture_slot_admission(
        {"a","b","new"},{"a","b"},{},"new",2);
    if(insufficient.fits)return 5;
    const auto byte_eviction=plan_texture_slot_admission(
        {"new","a","b"},{"a","b"},{"b"},"new",2);
    if(!byte_eviction.fits||
       !byte_eviction.additional_evictions.empty())return 6;
    const auto replacement=plan_texture_slot_admission(
        {"a","b"},{"a","b"},{},"a",2);
    if(!replacement.fits)return 7;
    const auto zero=plan_texture_slot_admission(
        {"new"},{},{},"new",0);
    if(zero.fits)return 8;
    std::cout<<"GPU upload pacing and slot admission priority OK\n";
    return 0;
}
