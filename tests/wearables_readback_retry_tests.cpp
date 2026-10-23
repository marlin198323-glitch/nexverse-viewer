/* MPL-2.0: deterministic read-only LLUDP wearable reply timeout. */
#include "nexverse/protocol/wearables_readback_retry.hpp"
#include <chrono>
#include <iostream>
using namespace nexverse::protocol;
using namespace std::chrono_literals;
int main()
{
    using Clock=WearablesReadbackRetry::Clock;
    const auto base=Clock::time_point{}+100s;
    WearablesReadbackRetry retry;
    if(retry.due(base+100s)||retry.retries()!=0U||
       retry.received())return 1;
    retry.note_initial_sent(base);
    if(retry.due(base+4s)||!retry.due(base+5s))return 2;
    retry.note_retry_sent();
    if(retry.retries()!=1U||retry.due(base+11s)||
       !retry.due(base+12s))return 3;
    retry.note_retry_sent();
    if(retry.retries()!=2U||retry.due(base+60s))return 4;
    // Retransmitted initial request must not reset the timeout epoch.
    WearablesReadbackRetry valid;
    valid.note_initial_sent(base);
    valid.note_initial_sent(base+4s);
    if(!valid.due(base+5s))return 5;
    valid.note_response();
    if(!valid.received()||valid.due(base+200s)||
       valid.retries()!=0U)return 6;
    WearablesReadbackRetry late;
    late.note_initial_sent(base);
    late.note_retry_sent();
    late.note_response();
    if(late.due(base+20s)||late.retries()!=1U)return 7;
    WearablesReadbackRetry new_session;
    if(new_session.due(base+200s))return 8;
    new_session.note_initial_sent(base+120s);
    if(new_session.due(base+124s)||
       !new_session.due(base+125s))return 9;
    std::cout<<"OGL_WEARABLES_READONLY_LOW381_BOUNDED_RETRY_PASS\n";
}
