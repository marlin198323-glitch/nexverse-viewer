/* MPL-2.0: actual LLUDP login/movement-complete decoder and Circuit gate. */
#include "nexverse/protocol/movement_complete_admission.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <string_view>
using namespace nexverse::protocol;
static constexpr std::string_view agent=
    "11111111-2222-3333-4444-555555555555";
static constexpr std::string_view session=
    "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
static void u32(std::vector<std::uint8_t>& p,std::uint32_t v)
{
    for(int i=0;i<4;++i)p.push_back(std::uint8_t((v>>(i*8))&255U));
}
static void f32(std::vector<std::uint8_t>& p,float f)
{u32(p,std::bit_cast<std::uint32_t>(f));}
static void u64(std::vector<std::uint8_t>& p,std::uint64_t v)
{
    for(int i=0;i<8;++i)p.push_back(std::uint8_t((v>>(i*8))&255ULL));
}
static std::vector<std::uint8_t> movement()
{
    std::vector<std::uint8_t> p;
    const auto a=uuid_to_bytes(agent),s=uuid_to_bytes(session);
    p.insert(p.end(),a->begin(),a->end());
    p.insert(p.end(),s->begin(),s->end());
    f32(p,128.0f);f32(p,129.0f);f32(p,30.0f);
    f32(p,1.0f);f32(p,0.0f);f32(p,0.0f);
    u64(p,0x0000010000000200ULL);
    u32(p,123456U);
    p.push_back(0U);p.push_back(0U); // variable2 empty channel
    return p;
}
int main()
{
    LoginSession login{};
    login.agent_id=std::string(agent);
    login.session_id=std::string(session);
    // Region handle (256 << 32) | 512 matches login's XML-RPC
    // region coordinates. A wrong region cannot activate the circuit.
    login.region_x=256U;
    login.region_y=512U;
    const auto p=movement();
    const auto packet=encode_packet(packet_reliable,4U,
        msg_agent_movement_complete,p);
    const auto parsed=decode_lludp_packet(packet);
    if(!parsed.ok()||parsed.packet->header.message!=
         msg_agent_movement_complete)return 1;
    if(valid_circuit_movement_complete(parsed.packet->payload,login,false))
        return 2;
    if(!valid_circuit_movement_complete(parsed.packet->payload,login,true))
        return 3;
    auto foreign=login;
    foreign.agent_id="99999999-2222-3333-4444-555555555555";
    if(valid_circuit_movement_complete(p,foreign,true))return 4;
    foreign=login;
    foreign.session_id="bbbbbbbb-bbbb-cccc-dddd-eeeeeeeeeeee";
    if(valid_circuit_movement_complete(p,foreign,true))return 5;
    auto mutated=p;
    mutated[0]^=1U; // foreign agent UUID
    if(valid_circuit_movement_complete(mutated,login,true))return 6;
    mutated=p;mutated[16]^=1U; // different session
    if(valid_circuit_movement_complete(mutated,login,true))return 7;
    mutated=p;mutated.resize(50U); // truncated wire payload
    if(valid_circuit_movement_complete(mutated,login,true))return 8;
    mutated=p;
    for(std::size_t i=56U;i<64U;++i)mutated[i]=0U; // RegionHandle zero
    if(valid_circuit_movement_complete(mutated,login,true))return 9;
    mutated=p;
    // Position x=2 million: finite but implausible for avatar/local scene.
    const auto huge=std::bit_cast<std::uint32_t>(2000000.0f);
    for(int i=0;i<4;++i)
        mutated[32U+std::size_t(i)]=std::uint8_t((huge>>(8*i))&255U);
    if(valid_circuit_movement_complete(mutated,login,true))return 10;
    mutated=p;
    // NaN LookAt must be rejected by the original decoded contract.
    const auto nan=std::bit_cast<std::uint32_t>(
        std::numeric_limits<float>::quiet_NaN());
    for(int i=0;i<4;++i)
        mutated[44U+std::size_t(i)]=std::uint8_t((nan>>(8*i))&255U);
    if(valid_circuit_movement_complete(mutated,login,true))return 11;
    mutated=p;
    // A valid vertical LookAt allows POSITION-only world readback.
    const auto one=std::bit_cast<std::uint32_t>(1.0f);
    for(int i=0;i<4;++i)mutated[44U+std::size_t(i)]=0U;
    for(int i=0;i<4;++i)mutated[48U+std::size_t(i)]=0U;
    for(int i=0;i<4;++i)
        mutated[52U+std::size_t(i)]=std::uint8_t((one>>(8*i))&255U);
    if(!valid_circuit_movement_complete(mutated,login,true))return 12;
    mutated=p;mutated[69U]=1U; // malformed variable2 length
    if(valid_circuit_movement_complete(mutated,login,true))return 13;
    mutated=p;
    // Same login AgentID and SessionID, finite position, nonzero
    // RegionHandle, but DIFFERENT simulator region. Must reject.
    mutated[56U]^=1U;
    if(valid_circuit_movement_complete(mutated,login,true))return 14;
    if(!movement_complete_wrong_login_region(mutated,login))return 19;
    auto wrong_login_region=login;
    wrong_login_region.region_y=768U;
    if(valid_circuit_movement_complete(p,wrong_login_region,true))return 15;
    wrong_login_region=login;
    wrong_login_region.region_x=0U;
    if(valid_circuit_movement_complete(p,wrong_login_region,true))return 16;
    // Genuine legacy grids that omit BOTH login coordinates can still
    // start LLUDP; WorldModel separately admits only the real region.
    wrong_login_region.region_y=0U;
    if(!valid_circuit_movement_complete(p,wrong_login_region,true))return 17;
    if(movement_complete_wrong_login_region(p,wrong_login_region))return 20;
    if(movement_complete_wrong_login_region(p,login))return 21;
    // A legitimate x=0 edge-of-world region must NOT be treated as
    // unknown when y is supplied.
    wrong_login_region.region_y=512U;
    if(valid_circuit_movement_complete(p,wrong_login_region,true))return 18;
    std::cout<<"OGL_LLUDP_MOVEMENT_COMPLETE_CURRENT_LOGIN_ADMISSION_PASS\n";
}
