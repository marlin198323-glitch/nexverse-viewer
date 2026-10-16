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
    std::cout<<"Failed completed worker only, valid session/endpoint, cooldown & stop gates PASS\n";
    return 0;
}
