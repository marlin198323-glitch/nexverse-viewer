#include "nexverse/protocol/firestorm_region_lease.hpp"
#include <future>
#include <iostream>
int main()
{
    using namespace nexverse::protocol;
    FirestormRegionLease lease;
    if(lease.epoch()!=0U||lease.matches(0U))return 1;
    const auto epoch=lease.enter(123U,0U,"https://seed.example.test/first");
    if(epoch==0U||!lease.matches(epoch)||
       lease.enter(123U,0U,"https://seed.example.test/first")!=epoch)
       return 2;
    std::promise<void> captured,release;
    auto allow=release.get_future();
    auto old_request=std::async(std::launch::async,[&]
    {
        captured.set_value();
        allow.wait();
        return lease.matches(epoch);
    });
    captured.get_future().wait();
    const auto next=lease.enter(456U,1U,"https://seed.example.test/second");
    release.set_value();
    if(next==epoch||old_request.get()||!lease.matches(next))return 3;
    if(lease.enter(456U,1U,"https://seed.example.test/third")==next)
       return 4;
    if(lease.enter(456U,2U,"https://seed.example.test/third")==next)
       return 5;
    ExperienceUiState ui;
    ui.toggle(false,false);
    if(ui.open)return 6;
    ui.toggle(true,false);
    if(ui.open)return 7;
    ui.toggle(true,true);
    if(!ui.open)return 8;
    ui.select(ExperienceUiState::Tab::blocked);
    if(ui.tab!=ExperienceUiState::Tab::blocked)return 9;
    ui.close();
    if(ui.open)return 10;
    ui.select(ExperienceUiState::Tab::owned);
    if(ui.tab!=ExperienceUiState::Tab::blocked)return 11;
    std::cout<<"Firestorm region epoch and UI gating OK\n";
    return 0;
}
