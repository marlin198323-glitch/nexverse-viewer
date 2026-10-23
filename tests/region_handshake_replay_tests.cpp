/* MPL-2.0. LLUDP RegionHandshake reliable duplicate/proof isolation. */
#include "nexverse/protocol/region_handshake_replay.hpp"
#include "nexverse/protocol/region_handshake_appearance.hpp"
#include <array>
#include <cstdint>
#include <iostream>
using namespace nexverse::protocol;
int main()
{
    RegionHandshakeReplay replay{};
    if(replay.already_replied(9U)||replay.already_replied(0U))return 1;
    replay.note_replied(9U);
    if(!replay.already_replied(9U)||replay.already_replied(10U))return 2;
    replay.note_replied(9U);
    replay.note_replied(10U);
    replay.note_replied(11U);
    // UDP retransmissions can be OUT OF ORDER, not only newest.
    if(!replay.already_replied(9U)||!replay.already_replied(10U)||
       !replay.already_replied(11U)||replay.already_replied(12U))return 3;
    // The associated reliable SSA bit2 ACK must remain valid as a
    // circuit proof even if old RegionHandshake packets arrive again.
    SelfAppearanceHandshakeReceipt receipt{};
    receipt.note_sent(77U);
    if(!receipt.note_ack(77U,true)||!receipt.acknowledged)return 4;
    if(!replay.already_replied(9U)||!receipt.acknowledged)return 5;
    replay.note_replied(12U); // distinct updated region metadata
    receipt.note_refresh(79U); // even before full region activation
    if(!replay.already_replied(12U)||!receipt.acknowledged||
       receipt.sequence!=77U)return 6;
    if(!advertises_self_appearance(ogl_region_handshake_flags))return 7;
    replay.reset();
    if(replay.already_replied(9U)||replay.already_replied(12U))return 8;
    // Legal wrap-around from 0xFFFF'FFFF to 0 is also supported.
    replay.note_replied(0xFFFFFFFFU);
    replay.note_replied(0U);
    if(!replay.already_replied(0U)||!replay.already_replied(0xFFFFFFFFU))
        return 9;
    // Bounded history allows a VERY old sequence to expire without
    // memory growth: 16 full slots including zero, FIFO replacement.
    replay.reset();
    for(std::uint32_t i=100U;i<116U;++i)replay.note_replied(i);
    for(std::uint32_t i=100U;i<116U;++i)
        if(!replay.already_replied(i))return 10;
    replay.note_replied(116U);
    if(replay.already_replied(100U)||!replay.already_replied(116U)||
       !replay.already_replied(115U))return 11;
    std::cout<<"OGL_REGION_HANDSHAKE_REPLAY_SSA_ACK_ACTIVE_LEASE_PASS\n";
    return 0;
}
