/* MPL-2.0: real async welcome snapshot lifecycle regression. */
#include "nexverse/ui/welcome_snapshot_policy.hpp"
#include <chrono>
#include <iostream>
using nexverse::ui::WelcomeSnapshotPolicy;
int main()
{
    using namespace std::chrono_literals;
    const auto t=WelcomeSnapshotPolicy::Clock::now();
    WelcomeSnapshotPolicy s;
    if(!s.can_request(t))return 1;
    auto old=s.begin(t);
    if(old==0||s.can_request(t))return 2;
    // The old snapshot must not re-enable a ready frame after a resize.
    s.invalidate();
    if(s.finish(old,true,t+10ms)||!s.can_request(t+10ms))return 3;
    auto current=s.begin(t+10ms);
    if(!current||current==old||!s.finish(current,true,t+20ms))return 4;
    if(s.can_request(t+29s)||!s.can_request(t+31s))return 5;
    auto fresh=s.begin(t+31s);
    if(!s.finish(fresh,true,t+31s))return 6;
    // Three transient failures are retried with backoff, not every frame.
    s.invalidate();
    for(int i=1;i<=3;++i)
    {
        const auto step=t+32s+std::chrono::seconds{i};
        const auto token=s.begin(step);
        if(!token||s.finish(token,false,step)||s.can_request(step))return 7;
    }
    if(!s.exhausted()||s.can_request(t+70s))return 8;
    s.invalidate();
    if(s.exhausted()||!s.can_request(t+70s))return 9;
    std::cout<<"OGL_WELCOME_SNAPSHOT_LIFECYCLE_PASS\n";
}
