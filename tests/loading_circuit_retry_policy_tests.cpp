/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/loading_circuit_retry_policy.hpp"
#include <iostream>
int main()
{
    using nexverse::ui::can_retry_loading_circuit;
    if(!can_retry_loading_circuit(true,true,true,true,false,true,true))
        return 1;
    for(int bit=0;bit<7;++bit)
    {
        bool checks[7]={true,true,true,true,false,true,true};
        checks[bit]=bit==4?true:false;
        if(can_retry_loading_circuit(
            checks[0],checks[1],checks[2],checks[3],checks[4],
            checks[5],checks[6]))return bit+2;
    }
    using namespace nexverse::ui;
    if(loading_retry_reason(true,true,true,true,false,true,true)!=
       LoadingCircuitRetryReason::ready)return 11;
    if(loading_retry_reason(true,true,false,true,false,true,true)!=
       LoadingCircuitRetryReason::circuit_not_failed)return 12;
    if(loading_retry_reason(true,true,true,false,false,true,true)!=
       LoadingCircuitRetryReason::worker_still_running)return 13;
    if(loading_retry_reason(true,true,true,true,false,true,false)!=
       LoadingCircuitRetryReason::cooldown_wait)return 14;
    if(loading_retry_reason(true,true,true,true,false,false,true)!=
       LoadingCircuitRetryReason::target_not_verified)return 15;
    if(loading_retry_reason(true,true,true,true,true,true,true)!=
       LoadingCircuitRetryReason::stopping_or_handoff)return 16;
    if(loading_retry_reason_label(LoadingCircuitRetryReason::ready)[0]=='\0')
        return 17;
    std::cout<<"Failed completed worker only, valid session/endpoint, cooldown & stop gates PASS\n";
    return 0;
}
