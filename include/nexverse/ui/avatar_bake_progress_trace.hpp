/* This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0.
 */
#pragma once
#include "nexverse/ui/avatar_diagnostic_report.hpp"
#include <array>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

namespace nexverse::ui
{
// Contains ONLY whitelisted aggregate counts/stages from the current
// authenticated region. No server URL, asset UUID, avatar or session ID.
// Report first observation, meaningful stage transitions and at most
// one repeated unresolved status every 30 seconds.
class AvatarBakeProgressTrace
{
public:
    using Clock=std::chrono::steady_clock;
    using Time=Clock::time_point;
    void reset() noexcept
    {
        region_epoch_=0U;
        last_.reset();
        last_emit_={};
    }
    [[nodiscard]] std::optional<std::string> observe(
        std::uint64_t region_epoch,const AvatarDiagnosticReport& r,Time now)
    {
        if(region_epoch==0U||!r.authenticated||
           !r.simulator_circuit_active||!r.region_connected||
           !r.agent_identity_present)
        {
            reset();
            return std::nullopt;
        }
        if(region_epoch_!=region_epoch)
        {
            region_epoch_=region_epoch;
            last_.reset();
        }
        State next{};
        next.blocker=avatar_skin_blocker_key(r);
        next.body_refs=r.self_low158_body_bake_refs;
        next.texture_stages=r.self_low158_body_gettexture_stages;
        next.sources=r.self_render_body_bake_sources;
        next.selected_stages=r.self_render_body_gettexture_stages;
        next.source_gl=r.self_low158_current_body_gl_drawn;
        next.gpu_count=r.avatar_body_bakes_gpu;
        next.depth_known=r.gpu_fragment_result_known;
        next.depth_visible=r.gpu_fragments_passed;
        next.body_depth_known=r.avatar_depth_known;
        next.body_depth_passed=r.avatar_depth_passed;
        next.provisional=r.self_object_bake_candidate_active;
        const bool changed=!last_||*last_!=next;
        // Prevent rapidly oscillating asynchronous GetTexture/GL
        // readiness from flooding the log on every rendered frame.
        // A real transition that survives this 250ms window will
        // still be emitted on a subsequent observation.
        if(last_&&changed&&
           now-last_emit_<std::chrono::milliseconds{250})
            return std::nullopt;
        // The final stage explicitly does NOT verify final RGB pixels;
        // it is terminal only for this log-repetition policy.
        const bool final_stage=next.blocker==
            "depth_visible_final_skin_pixels_unverified";
        if(!changed&&(final_stage||
           now-last_emit_<std::chrono::seconds{30}))
            return std::nullopt;
        last_=next;
        last_emit_=now;
        // Byte-sized source and failure codes are logged as unsigned
        // decimal values, never as binary characters.
        const auto tri=[](const auto& a)
        {
            return std::to_string(static_cast<unsigned>(a[0]))+","+
                   std::to_string(static_cast<unsigned>(a[1]))+","+
                   std::to_string(static_cast<unsigned>(a[2]));
        };
        const auto bool_tri=[](const auto& a)
        {
            return std::string{a[0]?"1":"0"}+","+
                   (a[1]?"1":"0")+","+(a[2]?"1":"0");
        };
        return std::string{"Avatar SELF bake pipeline: blocker="}+
            next.blocker+
            "; low158_refs="+std::to_string(next.body_refs)+"/3"+
            "; low158_gettexture="+tri(next.texture_stages)+
            "; selected_source="+tri(next.sources)+
            "; selected_gettexture="+tri(next.selected_stages)+
            "; current_low158_gl="+bool_tri(next.source_gl)+
            "; gpu_body="+std::to_string(next.gpu_count)+"/3"+
            "; depth_known="+(next.depth_known?"1":"0")+
            "; depth_passed="+(next.depth_visible?"1":"0")+
            "; depth_part_known="+bool_tri(next.body_depth_known)+
            "; depth_part_passed="+bool_tri(next.body_depth_passed)+
            "; provisional_objte="+(next.provisional?"1":"0")+
            "; final_skin_rgb_unverified";
    }
private:
    struct State
    {
        std::string blocker;
        std::uint32_t body_refs=0U;
        std::array<std::uint8_t,3U> texture_stages{};
        std::array<std::uint8_t,3U> sources{};
        std::array<std::uint8_t,3U> selected_stages{};
        std::array<bool,3U> source_gl{};
        std::array<bool,3U> body_depth_known{};
        std::array<bool,3U> body_depth_passed{};
        std::uint32_t gpu_count=0U;
        bool depth_known=false,depth_visible=false,provisional=false;
        bool operator==(const State&) const = default;
    };
    std::uint64_t region_epoch_=0U;
    std::optional<State> last_;
    Time last_emit_{};
};
}
