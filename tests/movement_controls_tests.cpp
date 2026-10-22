#include "nexverse/protocol/movement_controls.hpp"
#include "nexverse/protocol/movement_feedback.hpp"
#include "nexverse/protocol/turn_readback.hpp"

#include <cmath>
#include <iostream>
#include <numbers>
#include <limits>
#include <array>
#include <bit>

int main()
{
    using namespace nexverse::protocol;
    MovementController c;
    c.reset();
    MovementKeys k;
    k.forward = true;
    auto forward = c.step(k, 0.1f);
    if ((forward.control_flags & control_at_pos) == 0U ||
        (forward.control_flags & control_fly) != 0U ||
        forward.body_rotation.w != 1.0f)
    {
        std::cerr << "W movement control not initialized\n";
        return 1;
    }
    k = {};
    k.left = true;
    k.sprint = true;
    auto strafe = c.step(k, 0.1f);
    if ((strafe.control_flags & control_left_pos) == 0U ||
        (strafe.control_flags & control_fast_left) == 0U)
    {
        std::cerr << "Strafe/sprint protocol bits incorrect\n";
        return 2;
    }
    k = {};
    k.toggle_fly = true;
    auto flying = c.step(k, 0.1f);
    if (!c.flying() || (flying.control_flags & control_fly) == 0U)
    {
        std::cerr << "Fly toggle did not arm\n";
        return 3;
    }
    (void)c.step(k, 0.1f); // holding F should not toggle a second time
    if (!c.flying())
    {
        std::cerr << "Fly key bounced while held\n";
        return 4;
    }
    k.toggle_fly = false;
    (void)c.step(k, 0.1f);
    k.toggle_fly = true;
    flying = c.step(k, 0.1f);
    if (c.flying() || (flying.control_flags & control_fly) != 0U)
    {
        std::cerr << "Flight cannot be disabled on new press\n";
        return 5;
    }
    k = {};
    k.turn_left = true;
    auto left = c.step(k, 0.2f);
    if ((left.control_flags & control_turn_left) == 0U ||
        (left.control_flags & control_yaw_pos) == 0U ||
        (left.control_flags & (control_yaw_neg|control_turn_right))!=0U ||
        left.body_rotation.z <= 0.0f ||
        std::abs(left.body_rotation.z * left.body_rotation.z +
                 left.body_rotation.w * left.body_rotation.w - 1.0f) > 0.0001f ||
        left.body_rotation.z != left.head_rotation.z)
    {
        std::cerr << "Left rotation quaternion / control flags incorrect\n";
        return 6;
    }
    k={};
    k.turn_right=true;
    const auto right=c.step(k,0.4f);
    if((right.control_flags&(control_turn_right|control_yaw_neg))!=
       (control_turn_right|control_yaw_neg)||
       (right.control_flags&(control_turn_left|control_yaw_pos))!=0U||
       right.body_rotation.z>=left.body_rotation.z)return 20;
    // Opposing turn keys may not emit both yaw signs. There is no
    // unambiguous server rotation request in that case.
    k.turn_left=true;
    const auto opposite=c.step(k,0.15f);
    if((opposite.control_flags&
        (control_yaw_pos|control_yaw_neg|
         control_turn_left|control_turn_right))!=0U)return 21;
    // OpenGenesisLINK Server ScenePresence: the ACTUAL LLUDP High 4
    // AgentUpdate must pair TURN_LEFT/YAW_POS and TURN_RIGHT/YAW_NEG.
    // Decode wire bytes at the fixed message_template ControlFlags
    // offset, rather than only inspecting the input Controller result.
    LoginSession session;
    session.agent_id="11111111-2222-3333-4444-555555555555";
    session.session_id="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
    auto server_control_bits=[&](const AgentUpdateData& action)
    {
        const auto wire=decode_lludp_packet(
            encode_agent_update(52U,session,action));
        if(!wire.ok()||
           wire.packet->header.message!=msg_agent_update||
           wire.packet->payload.size()!=114U)return std::uint32_t{0U};
        const auto& bytes=wire.packet->payload;
        return static_cast<std::uint32_t>(bytes[109])|
            static_cast<std::uint32_t>(bytes[110])<<8U|
            static_cast<std::uint32_t>(bytes[111])<<16U|
            static_cast<std::uint32_t>(bytes[112])<<24U;
    };
    if(server_control_bits(left)!=(control_turn_left|control_yaw_pos)||
       server_control_bits(right)!=(control_turn_right|control_yaw_neg)||
       server_control_bits(opposite)!=0U)return 22;
    c.reset();
    k={};
    k.toggle_fly=true;
    const auto enabled=c.step(k,0.1f);
    if((server_control_bits(enabled)&control_fly)==0U)return 23;
    k={};
    k.turn_left=true;
    const auto flying_left=c.step(k,0.1f);
    if(server_control_bits(flying_left)!=
       (control_fly|control_turn_left|control_yaw_pos))return 24;
    k={};
    k.turn_right=true;
    const auto flying_right=c.step(k,0.1f);
    if(server_control_bits(flying_right)!=
       (control_fly|control_turn_right|control_yaw_neg))return 25;
    c.reset();
    k={};k.forward=true;k.backward=true;
    const auto cancel_forward=c.step(k,0.1f);
    if((server_control_bits(cancel_forward)&
       (control_at_pos|control_at_neg))!=0U)return 26;
    k={};k.left=true;k.right=true;
    const auto cancel_strafe=c.step(k,0.1f);
    if((server_control_bits(cancel_strafe)&
       (control_left_pos|control_left_neg))!=0U)return 27;
    k={};k.forward=true;k.left=true;
    const auto diagonal=c.step(k,0.1f);
    if(server_control_bits(diagonal)!=(control_at_pos|control_left_pos))
        return 28;
    k = {};
    k.jump = true;
    k.descend = true;
    auto vertical = c.step(k, 0.1f);
    if ((vertical.control_flags & control_up_pos) == 0U ||
        (vertical.control_flags & control_up_neg) == 0U)
    {
        std::cerr << "Jump/down input missing\n";
        return 7;
    }
    c.reset();
    if (c.flying())
    {
        std::cerr << "Region reset did not clear fly mode\n";
        return 8;
    }
    c.reset({0.0f, 0.0f, std::sin(std::numbers::pi_v<float>/4.0f),
            std::cos(std::numbers::pi_v<float>/4.0f)});
    const auto heading = c.step({}, 0.1f);
    if (std::abs(heading.body_rotation.z -
                 std::sin(std::numbers::pi_v<float>/4.0f)) > 0.0001f)
    {
        std::cerr << "Initial simulator heading not preserved\n";
        return 9;
    }
    // Server-side motion verification: input bits are not sufficient.
    // Only an actual server position delta >=0.25m can confirm movement.
    using Clock=std::chrono::steady_clock;
    const auto t=Clock::time_point{};
    MovementFeedbackMonitor feedback;
    if(feedback.server_position({10.0f,20.0f,30.0f},t)!=
       MovementEvidence::none)return 10;
    feedback.outbound_controls(control_turn_left,t); // rotate only
    if(feedback.server_position({10.0f,20.0f,30.0f},
                                t+std::chrono::seconds{5})!=
       MovementEvidence::none)return 11;
    // Use monotonic arrivals: a turn-only simulator sample was observed
    // at +5s, so the subsequent WASD send cannot predate it. This guards
    // against a test fixture falsely simulating time travel on the wire.
    feedback.outbound_controls(control_at_pos,t+std::chrono::seconds{6});
    for(int i=7;i<=8;++i)
        if(feedback.server_position({10.0f,20.0f,30.0f},
                      t+std::chrono::seconds{i})!=MovementEvidence::none)
            return 12;
    if(feedback.server_position({10.0f,20.0f,30.0f},
               t+std::chrono::seconds{11})!=
       MovementEvidence::server_position_stalled)return 13;
    if(feedback.server_position({10.0f,20.0f,30.0f},
               t+std::chrono::seconds{12})!=MovementEvidence::none)
        return 14; // stalled report deduplicated
    if(feedback.server_position({10.0f,20.3f,30.0f},
               t+std::chrono::seconds{13})!=
       MovementEvidence::server_position_changed)return 15;
    if(feedback.server_position({10.0f,20.6f,30.0f},
               t+std::chrono::seconds{14})!=MovementEvidence::none)
        return 16; // confirmed report deduplicated
    feedback.outbound_controls(0U,t+std::chrono::seconds{15});
    feedback.outbound_controls(control_at_pos,t+std::chrono::seconds{16});
    if(feedback.server_position({10.3f,20.6f,30.0f},
               t+std::chrono::seconds{17})!=
       MovementEvidence::server_position_changed)return 17;
    feedback.reset();
    if(feedback.server_position({20.0f,20.0f,30.0f},
               t+std::chrono::seconds{30})!=MovementEvidence::none)
        return 18; // teleports are not WASD evidence
    feedback.outbound_controls(control_at_pos|control_at_neg,
               t+std::chrono::seconds{32});
    if(feedback.server_position({20.0f,20.0f,30.0f},
               t+std::chrono::seconds{40})!=MovementEvidence::none)
        return 19; // opposing controls cancel
    // Real turn evidence is not a queued key or local quaternion.
    TurnReadbackMonitor turn;
    auto server_heading=[](float radians)->Quaternion
    {
        return {0.0f,0.0f,std::sin(radians*0.5f),
                std::cos(radians*0.5f)};
    };
    if(turn.observe_server_rotation(server_heading(0.0f),t))return 29;
    turn.outbound_controls(control_turn_left|control_yaw_pos,t+std::chrono::seconds{1});
    if(turn.server_confirmed()||
       turn.observe_server_rotation(server_heading(-0.4f),t+std::chrono::seconds{2})||
       turn.server_confirmed())return 30;
    if(!turn.observe_server_rotation(server_heading(0.3f),t+std::chrono::seconds{3})||
       !turn.server_confirmed())return 31;
    if(turn.observe_server_rotation(server_heading(0.5f),t+std::chrono::seconds{4}))return 32;
    turn.outbound_controls(0U,t+std::chrono::seconds{5});
    if(turn.server_confirmed()||turn.turn_active())return 33;
    turn.outbound_controls(control_turn_right|control_yaw_neg,t+std::chrono::seconds{6});
    if(turn.observe_server_rotation(server_heading(0.6f),t+std::chrono::seconds{5})||
       turn.server_confirmed())return 34;
    if(!turn.observe_server_rotation(server_heading(-0.5f),t+std::chrono::seconds{7})||
       !turn.server_confirmed())return 35;
    turn.reset();
    if(turn.server_confirmed()||turn.turn_active()||
       turn.observe_server_rotation(server_heading(2.0f),t+std::chrono::seconds{8}))return 36;
    turn.reset(); // new region: no pre-send simulator rotation
    turn.outbound_controls(control_turn_left|control_yaw_pos,t+std::chrono::seconds{9});
    if(turn.observe_server_rotation({NAN,0.0f,0.0f,1.0f},t+std::chrono::seconds{10})||
       turn.observe_server_rotation({0.0f,0.0f,0.0f,0.0f},t+std::chrono::seconds{10})||
       turn.server_confirmed())return 37;
    if(turn.observe_server_rotation(server_heading(1.4f),t+std::chrono::seconds{11})||
       turn.server_confirmed())return 38; // initial baseline only
    if(!turn.observe_server_rotation(server_heading(1.7f),t+std::chrono::seconds{12}))
        return 39;
    // The real AgentUpdate serializer must NEVER send a nonfinite
    // rotation after a hung/minimized desktop frame or bad simulator pose.
    const float nan=std::numeric_limits<float>::quiet_NaN();
    const float inf=std::numeric_limits<float>::infinity();
    for(const float duration:{nan,inf,-inf,-0.1f})
    {
        c.reset();
        MovementKeys turn_keys{};
        turn_keys.turn_left=true;
        const auto safe=c.step(turn_keys,duration);
        if(safe.body_rotation.z!=0.0f||safe.body_rotation.w!=1.0f||
           server_control_bits(safe)!=(control_turn_left|control_yaw_pos))
            return 40;
    }
    // Large finite pause MUST cap at 0.25s, no teleport spin on resume.
    c.reset();
    MovementKeys rotate{};
    rotate.turn_left=true;
    const auto capped=c.step(rotate,3600.0f);
    if(std::abs(capped.body_rotation.z-
            std::sin(0.25f*1.65f*0.5f))>0.0001f)return 41;
    for(const auto heading_bad:{
        Quaternion{nan,0.0f,0.0f,1.0f},
        Quaternion{inf,0.0f,0.0f,1.0f},
        Quaternion{0.0f,0.0f,0.0f,0.0f}})
    {
        c.reset(heading_bad);
        const auto safe=c.step({},0.1f);
        if(safe.body_rotation.z!=0.0f||safe.body_rotation.w!=1.0f||
           server_control_bits(safe)!=0U)return 42;
    }
    // A finite but very large quaternion used to overflow float
    // intermediates: double precision normalization preserves heading.
    c.reset({0.0f,0.0f,7e25f,7e25f});
    const auto large=c.step({},0.1f);
    if(std::abs(large.body_rotation.z-0.70710678f)>0.0001f||
       std::abs(large.body_rotation.w-0.70710678f)>0.0001f)
        return 43;
    for(const Vec3 invalid:std::array<Vec3,4>{{
        {nan,128.0f,25.0f},{128.0f,inf,25.0f},
        {128.0f,128.0f,-inf},{1e8f,128.0f,25.0f}}})
        if(valid_agent_update_position(invalid))return 45;
    if(!valid_agent_update_position({128.0f,128.0f,25.0f}))
        return 46;
    const auto wire=decode_lludp_packet(
        encode_agent_update(53U,session,large));
    if(!wire.ok()||wire.packet->header.message!=msg_agent_update||
       wire.packet->payload.size()!=114U)
        return 44;
    // Real High4 wire body quaternion is x,y,z at offsets 32..43.
    // The server rebuilds w; a 4th transmitted float breaks flags.
    const auto& pl=wire.packet->payload;
    const auto bits=std::uint32_t{pl[40]}|
        (std::uint32_t{pl[41]}<<8U)|
        (std::uint32_t{pl[42]}<<16U)|
        (std::uint32_t{pl[43]}<<24U);
    const auto actual_z=std::bit_cast<float>(bits);
    if(!std::isfinite(actual_z)||
       std::abs(actual_z-0.70710678f)>0.0001f)return 47;
    c.reset();
    const auto safe_nan=c.step(rotate,nan);
    const auto nan_wire=decode_lludp_packet(
        encode_agent_update(54U,session,safe_nan));
    if(!nan_wire.ok()||nan_wire.packet->payload.size()!=114U||
       server_control_bits(safe_nan)!=(control_turn_left|control_yaw_pos))
        return 48;
    const auto& npl=nan_wire.packet->payload;
    const auto nbits=std::uint32_t{npl[40]}|
        (std::uint32_t{npl[41]}<<8U)|
        (std::uint32_t{npl[42]}<<16U)|
        (std::uint32_t{npl[43]}<<24U);
    if(std::bit_cast<float>(nbits)!=0.0f)return 49;
    // Server sends an independent 90-degree body rotation after login.
    // Idle input MUST adopt that authoritative heading, not keep
    // retransmitting the old local yaw. Flight toggle remains active.
    c.reset();
    k={};k.toggle_fly=true;
    if((c.step(k,0.1f).control_flags&control_fly)==0U)return 50;
    c.adopt_server_heading({0.0f,0.0f,0.70710678f,0.70710678f});
    k={};k.forward=true;
    const auto server_aligned=c.step(k,0.1f);
    if((server_aligned.control_flags&(control_at_pos|control_fly))!=
       (control_at_pos|control_fly)||
       std::abs(server_aligned.body_rotation.z-0.70710678f)>0.0001f)
        return 51;
    const auto authority_wire=decode_lludp_packet(
        encode_agent_update(59U,session,server_aligned));
    if(!authority_wire.ok()||
       authority_wire.packet->payload.size()!=114U)return 52;
    const auto& wire_bytes=authority_wire.packet->payload;
    const auto wire_z=std::bit_cast<float>(
        std::uint32_t{wire_bytes[40]}|
        (std::uint32_t{wire_bytes[41]}<<8U)|
        (std::uint32_t{wire_bytes[42]}<<16U)|
        (std::uint32_t{wire_bytes[43]}<<24U));
    if(std::abs(wire_z-0.70710678f)>0.0001f)return 53;
    // New server heading can also be adopted while grounded.
    // The adapter does not reset the fly-key debouncing state.
    c.adopt_server_heading({0.0f,0.0f,0.0f,1.0f});
    if(c.step(k,0.1f).body_rotation.z!=0.0f)return 54;

    std::cout << "Avatar controls and rotation tests passed\n";
    return 0;
}
