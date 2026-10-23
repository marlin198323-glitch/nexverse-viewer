/* MPL-2.0: authenticated self-ObjectUpdate TE carries provisional body
 * bakes until Low158 replaces it; no fabricated appearance or ghost. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <iostream>
#include <vector>
using namespace nexverse;
static constexpr auto avatar_id="11111111-2222-3333-4444-555555555555";
static constexpr auto session_id="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
static constexpr std::uint64_t region=0x0000010000000200ULL;
static std::vector<std::uint8_t> tex(bool baked,bool placeholder=false)
{
    std::vector<std::uint8_t> te(16U,0x11U);
    if(baked)
    {
        te.insert(te.end(),{0x82U,0U}); // Firestorm head face 8
        const auto head=protocol::uuid_to_bytes(
            placeholder?"5a9f4a74-30f2-821c-b88d-70499d3e7183":
            "11111111-1111-1111-1111-111111111111");
        te.insert(te.end(),head->begin(),head->end());
        if(!placeholder)
        {
            te.insert(te.end(),{0x84U,0U}); // upper face 9
            const auto upper=protocol::uuid_to_bytes(
                "22222222-2222-2222-2222-222222222222");
            te.insert(te.end(),upper->begin(),upper->end());
        }
    }
    te.push_back(0U);
    const std::vector<std::uint8_t> fields{
        0U, // no UUID face overrides
        255U,255U,255U,255U,0U, // default white, no color overrides
        0U,0U,128U,63U,0U, // repeat U = 1.0
        0U,0U,128U,63U,0U, // repeat V = 1.0
        0U,0U,0U, // offset U and mask
        0U,0U,0U, // offset V and mask
        0U,0U,0U, // rotation and mask
        0U,0U, // material
        0U,0U, // media
        0U,0U // glow
    };
    te.insert(te.end(),fields.begin()+1,fields.end());
    return te;
}
static world::WorldModel model()
{
    world::WorldModel w;
    w.apply_region({region,"Test",{},"127.0.0.1",9000U,"","",
        0U,0U,0U,13U,0.0f,"",256U,256U,true});
    w.apply_agent({avatar_id,session_id,{},{},true});
    return w;
}
int main()
{
    auto w=model();
    if(w.self_object_bakes()||w.self_appearance())return 1;
    const auto head_te=tex(true);
    if(protocol::extract_avatar_bakes(head_te).valid_count()!=2U)
        return 2;
    if(w.observe_self_object_bakes("other",region,51U,head_te)||
       w.observe_self_object_bakes(avatar_id,region+1U,51U,head_te)||
       w.observe_self_object_bakes(avatar_id,region,0U,head_te))
        return 3;
    if(w.observe_self_object_bakes(avatar_id,region,51U,
                                  std::vector<std::uint8_t>{2U,3U}))
        return 4;
    if(w.observe_self_object_bakes(avatar_id,region,51U,tex(false))||
       w.observe_self_object_bakes(avatar_id,region,51U,tex(true,true)))
        return 5;
    if(!w.observe_self_object_bakes(avatar_id,region,51U,head_te))
        return 6;
    if(w.self_appearance()||!w.self_object_bakes()||
       w.self_object_bakes()->local_id!=51U||
       w.snapshot().self_object_bakes->visible_body[0]!=
           "11111111-1111-1111-1111-111111111111")
        return 7;
    // Repeated texture updates should not spam diagnostic logs, and do
    // not discard the already accepted visual candidate.
    if(w.observe_self_object_bakes(avatar_id,region,51U,head_te))
        return 18;
    const auto fallback=w.self_render_bakes();
    if(fallback[1]!="22222222-2222-2222-2222-222222222222"||
       !fallback[2].empty())return 8;
    // Empty/malformed update on same LocalID must not erase candidate.
    if(w.observe_self_object_bakes(avatar_id,region,51U,{})||
       w.self_render_bakes()!=fallback)return 9;
    // A valid current ObjectUpdate with all placeholder bakes is a
    // genuine withdrawal; unlike an absent or malformed packet it MUST
    // invalidate the previous provisional skin, not keep a ghost outfit.
    if(!w.observe_self_object_bakes(avatar_id,region,51U,tex(true,true))||
       w.self_object_bakes()||
       w.self_render_bakes()!=std::array<std::string,3U>{}||
       w.self_object_bake_withdrawals()!=1U)return 33;
    if(w.observe_self_object_bakes(avatar_id,region,51U,tex(true,true))||
       w.observe_self_object_bakes(avatar_id,region,51U,tex(false))||
       w.self_object_bake_withdrawals()!=1U)
        return 34;
    if(!w.observe_self_object_bakes(avatar_id,region,51U,head_te))
        return 35;
    // A valid current all-default TextureEntry also revokes the
    // provisional bake, but not a genuine Low158 appearance reference.
    if(!w.observe_self_object_bakes(avatar_id,region,51U,tex(false))||
       w.self_object_bakes()||
       w.self_render_bakes()!=std::array<std::string,3U>{}||
       w.self_object_bake_withdrawals()!=2U)return 36;
    if(!w.observe_self_object_bakes(avatar_id,region,51U,head_te))
        return 37;
    // Different server LocalID invalidates old provisional visual data.
    if(w.observe_self_object_bakes(avatar_id,region,52U,{})||
       w.self_object_bakes()||w.self_render_bakes()==fallback)return 10;
    if(!w.observe_self_object_bakes(avatar_id,region,52U,head_te))
        return 11;
    protocol::AvatarAppearanceMessage authoritative;
    authoritative.avatar_id=avatar_id;
    authoritative.texture_entry=tex(false); // valid, no actual bakes
    authoritative.visual_params={125U,126U};
    if(!w.apply_self_appearance(authoritative)||
       w.self_render_bakes()!=fallback || !w.self_object_bakes())return 12;
    const auto provisional_after_low158=w.self_bake_selection();
    if(!provisional_after_low158.from_object_update[0]||
       !provisional_after_low158.from_object_update[1]||
       provisional_after_low158.from_low158[0])return 27;
    if(!w.observe_self_object_bakes(avatar_id,region,53U,head_te)||
       w.self_render_bakes()!=fallback)return 13;
    // A later Low158 Head reference beats an old ObjectUpdate Head,
    // while a missing Low158 Upper may still use the real provisional TE.
    auto mixed=authoritative;
    mixed.texture_entry=tex(true,true); // placeholder only, no valid head
    if(!w.apply_self_appearance(mixed)||
       w.self_render_bakes()!=fallback)return 28;
    mixed.texture_entry=head_te; // actual Head+Upper from Low158
    if(!w.apply_self_appearance(mixed)||
       w.self_bake_selection().from_object_update[0]||
       !w.self_bake_selection().from_low158[0]||
       !w.self_bake_selection().from_low158[1])return 29;
    // A later valid ObjectUpdate withdraws just the provisional
    // candidate. Already authenticated Low158 Head/Upper remain
    // authoritative and MUST NOT disappear with it.
    if(!w.observe_self_object_bakes(avatar_id,region,53U,tex(false))||
       w.self_object_bakes()||
       w.self_render_bakes()!=fallback ||
       !w.self_bake_selection().from_low158[0]||
       !w.self_bake_selection().from_low158[1])return 38;
    if(!w.observe_self_object_bakes(avatar_id,region,53U,head_te))
        return 39;
    // Important conflicting-source regression: the authoritative
    // Low158 Head differs from the provisional ObjectUpdate Head.
    // Incomplete Low158 Upper is filled but its Head cannot be replaced.
    auto one_head=head_te;
    one_head.erase(one_head.begin()+34,one_head.begin()+52);
    for(std::size_t k=18U;k<34U;++k)one_head[k]=0x44U;
    mixed.texture_entry=one_head;
    if(!w.apply_self_appearance(mixed))return 30;
    const auto selected=w.self_bake_selection();
    if(selected.ids[0]!="44444444-4444-4444-4444-444444444444"||
       selected.ids[1]!="22222222-2222-2222-2222-222222222222"||
       !selected.from_low158[0]||
       !selected.from_object_update[1]||
       selected.from_object_update[0]||selected.from_low158[1])
        return 31;
    // New LocalID without a valid TE evicts only provisional leftovers,
    // while keeping the unmodified Low158 Head.
    if(w.observe_self_object_bakes(avatar_id,region,54U,{})||
       w.self_bake_selection().from_object_update[1]||
       !w.self_bake_selection().ids[1].empty()||
       w.self_bake_selection().ids[0]!=selected.ids[0])return 32;
    // Authenticated self with different session is not the same visual
    // lifetime, even on same simulator region.
    w.apply_agent({avatar_id,"ffffffff-bbbb-cccc-dddd-eeeeeeeeeeee",{},{},true});
    if(w.self_object_bakes()||w.self_appearance()||w.self_animations()||
       w.self_object_bake_withdrawals()!=0U)
        return 14;
    auto transferred=model();
    if(!transferred.observe_self_object_bakes(avatar_id,region,51U,head_te))
        return 15;
    transferred.begin_region_transition({region+3U,"Other",{},
        "127.0.0.1",9001U,"","",0U,0U,0U,13U,0.0f,"",
        256U,256U,true});
    if(transferred.self_object_bakes()||
       transferred.self_render_bakes()!=std::array<std::string,3U>{})
        return 16;
    // A matching region, but a new authenticated agent, also invalidates
    // the previous user's authoritative Low158 appearance.
    auto reauth=model();
    auto old=authoritative;
    if(!reauth.apply_self_appearance(old))return 19;
    reauth.apply_agent({"99999999-2222-3333-4444-555555555555",
        session_id,{},{},true});
    if(reauth.self_appearance()||reauth.self_object_bakes()||
       reauth.self_render_bakes()!=std::array<std::string,3U>{})
        return 20;
    // Unchanged agent/session pose feedback preserves valid self skin.
    reauth.apply_agent({avatar_id,session_id,{},{},true});
    if(!reauth.apply_self_appearance(old))return 21;
    reauth.apply_agent({avatar_id,session_id,{0.5f,0.5f,0.0f},{},true});
    if(!reauth.self_appearance())return 22;
    // A changed region via apply_region (without begin_transition)
    // must also invalidate both appearance provenance paths.
    reauth.apply_region({region+3U,"Different",{},"127.0.0.1",
        9001U,"","",0U,0U,0U,13U,0.0f,"",256U,256U,true});
    if(reauth.self_appearance()||reauth.self_object_bakes())return 23;
    // The same region can disconnect without a formal teleport event.
    // A successful reconnect must not inherit the previous skin.
    auto disconnected=model();
    if(!disconnected.observe_self_object_bakes(
            avatar_id,region,51U,head_te))return 24;
    if(!disconnected.apply_self_appearance(old))return 25;
    disconnected.apply_region({region,"Disconnected",{},
        "127.0.0.1",9000U,"","",0U,0U,0U,13U,0.0f,"",
        256U,256U,false});
    if(disconnected.self_appearance()||disconnected.self_object_bakes())
        return 26;
    // An apparently valid skin from another user is never accepted.
    auto wrong=model();
    wrong.apply_agent({"another",session_id,{},{},true});
    if(wrong.observe_self_object_bakes(avatar_id,region,51U,head_te))
        return 17;
    // Alpha.955–964: a REAL newer modern COF outfit must retire the
    // *previous* outfit's provisional ObjectUpdate TE, even when new
    // Low158 only contains a partial/placeholder baked body.
    auto outfit=model();
    if(!outfit.observe_self_object_bakes(avatar_id,region,60U,head_te))
        return 40;
    protocol::AvatarAppearanceMessage first_cof=authoritative;
    first_cof.appearance_data={{1U,100U,0U}};
    if(!outfit.apply_self_appearance(first_cof)||
       outfit.self_render_bakes()!=fallback||
       outfit.self_object_bake_outfit_invalidations()!=0U)return 41;
    auto same_revision=first_cof;
    same_revision.texture_entry=tex(true,true);
    if(!outfit.apply_self_appearance(same_revision)||
       outfit.self_render_bakes()!=fallback)return 42;
    auto newer_cof=first_cof;
    newer_cof.appearance_data={{1U,101U,0U}};
    if(!outfit.apply_self_appearance(newer_cof)||
       outfit.self_object_bakes()||
       outfit.self_render_bakes()!=std::array<std::string,3U>{}||
       outfit.self_object_bake_outfit_invalidations()!=1U)return 43;
    // A delayed identical old self ObjectUpdate from the RETIRED
    // LocalID can arrive after the newer COF over unordered UDP.
    // It must never restore the old outfit's skin.
    if(outfit.observe_self_object_bakes(avatar_id,region,60U,head_te)||
       outfit.self_object_bakes()||
       outfit.self_object_bake_stale_replays()!=1U)return 51;
    // A duplicate late retransmission is rejected, not an upload.
    if(outfit.observe_self_object_bakes(avatar_id,region,60U,head_te)||
       outfit.self_object_bake_stale_replays()!=2U)return 52;
    // A delayed prior revision cannot erase the newly accepted
    // outfit or count a second invalidation.
    if(outfit.apply_self_appearance(first_cof)||
       outfit.self_object_bake_outfit_invalidations()!=1U)return 44;
    // Fresh verified ObjectUpdate TE can provision new bakes again.
    if(!outfit.observe_self_object_bakes(avatar_id,region,61U,head_te)||
       outfit.self_render_bakes()!=fallback)return 45;
    // An OLD local60 retransmission cannot evict the newer local61,
    // even though both happened to carry identical old asset UUIDs.
    if(outfit.observe_self_object_bakes(avatar_id,region,60U,head_te)||
       outfit.self_render_bakes()!=fallback||
       outfit.self_object_bakes()->local_id!=61U||
       outfit.self_object_bake_stale_replays()!=3U)return 53;
    // Legacy unversioned Low158 isn't a distinct COF outfit and must
    // keep genuine provisional fallback for Firestorm/OpenSim.
    if(!outfit.apply_self_appearance(authoritative)||
       outfit.self_render_bakes()!=fallback||
       outfit.self_object_bake_outfit_invalidations()!=1U)return 46;
    // A different modern appearance format is a different proven
    // outfit source; never retain the older format's fallback.
    newer_cof.appearance_data={{2U,1U,0U}};
    if(!outfit.apply_self_appearance(newer_cof)||
       outfit.self_render_bakes()!=std::array<std::string,3U>{}||
       outfit.self_object_bake_outfit_invalidations()!=2U)return 47;
    if(!outfit.observe_self_object_bakes(avatar_id,region,62U,head_te))
        return 48;
    // Once a DIFFERENT genuine TE on the SAME LocalID is received
    // after an outfit switch, allow it, but still reject a subsequent
    // retransmission of the earlier outfit's exact TE.
    auto rebaked=model();
    if(!rebaked.observe_self_object_bakes(avatar_id,region,60U,head_te)||
       !rebaked.apply_self_appearance(first_cof)||
       !rebaked.apply_self_appearance(protocol::AvatarAppearanceMessage(newer_cof)))
        return 54;
    // The first modern COF after the old one has different format here.
    if(rebaked.observe_self_object_bakes(avatar_id,region,60U,head_te))
        return 55;
    auto fresh_te=head_te;
    for(std::size_t k=18U;k<34U;++k)fresh_te[k]=0x77U;
    if(!rebaked.observe_self_object_bakes(avatar_id,region,60U,fresh_te)||
       rebaked.self_render_bakes()[0]!="77777777-7777-7777-7777-777777777777")
        return 56;
    if(rebaked.observe_self_object_bakes(avatar_id,region,60U,head_te)||
       rebaked.self_render_bakes()[0]!="77777777-7777-7777-7777-777777777777"||
       rebaked.self_object_bake_stale_replays()!=2U)
        return 57;
    // Alpha.975-984: multiple successive confirmed COF changes must
    // retain *each* retired LocalID+body TE, not just the most recent.
    auto chained=model();
    auto cof102=first_cof;
    cof102.appearance_data={{1U,102U,0U}};
    if(!chained.observe_self_object_bakes(avatar_id,region,70U,head_te)||
       !chained.apply_self_appearance(first_cof)||
       !chained.apply_self_appearance(protocol::AvatarAppearanceMessage(
           protocol::AvatarAppearanceMessage(newer_cof))))
        return 58;
    // new101 was changed to format2 above; this still legitimately
    // retires the first source. A third accepted format change retires B.
    if(!chained.observe_self_object_bakes(avatar_id,region,71U,fresh_te)||
       !chained.apply_self_appearance(cof102)||
       !chained.observe_self_object_bakes(avatar_id,region,72U,head_te)||
       chained.self_object_bake_replay_guards()!=2U)return 59;
    if(chained.observe_self_object_bakes(avatar_id,region,70U,head_te)||
       chained.observe_self_object_bakes(avatar_id,region,71U,fresh_te)||
       !chained.self_object_bakes()||
       chained.self_object_bakes()->local_id!=72U||
       chained.self_object_bake_stale_replays()!=2U)return 60;
    if(!chained.withdraw_self_object_on_kill(avatar_id,region,70U)||
       chained.self_object_bake_replay_guards()!=1U)return 61;
    // Bounded history: after ten unique retirement events retain eight,
    // never unbounded UUID state across an extended appearance session.
    auto bounded=model();
    if(!bounded.apply_self_appearance(first_cof))return 62;
    for(std::uint32_t k=0U;k<10U;++k)
    {
        auto te=head_te;
        for(std::size_t j=18U;j<34U;++j)
            te[j]=static_cast<std::uint8_t>(0x30U+k);
        if(!bounded.observe_self_object_bakes(avatar_id,region,200U+k,te))
            return 63;
        auto next=first_cof;
        next.appearance_data={{1U,101U+k,0U}};
        if(!bounded.apply_self_appearance(next)||
           bounded.self_object_bake_replay_guards()!=
              (k<8U?k+1U:8U))return 64;
    }
    auto within=head_te;
    for(std::size_t j=18U;j<34U;++j)within[j]=0x32U; // index 2
    if(bounded.observe_self_object_bakes(avatar_id,region,202U,within)||
       bounded.self_object_bake_stale_replays()!=1U)return 65;
    // KillObject removes only matching retired history entries.
    if(!bounded.withdraw_self_object_on_kill(avatar_id,region,209U)||
       bounded.self_object_bake_replay_guards()!=7U)return 66;
    bounded.apply_agent({avatar_id,"dddddddd-bbbb-cccc-dddd-eeeeeeeeeeee",
                         {},{},true});
    if(bounded.self_object_bake_replay_guards()!=0U||
       bounded.self_object_bake_stale_replays()!=0U)return 67;
    // Unchanged COF does not trigger another retirement.
    if(!outfit.apply_self_appearance(newer_cof)||
       outfit.self_render_bakes()!=fallback||
       outfit.self_object_bake_outfit_invalidations()!=2U)return 49;
    // A different authenticated session cannot inherit the counter
    // or provisional TE from a previous resident.
    outfit.apply_agent({avatar_id,"cccccccc-bbbb-cccc-dddd-eeeeeeeeeeee",
                        {},{},true});
    if(outfit.self_object_bake_outfit_invalidations()!=0U||
       outfit.self_object_bake_stale_replays()!=0U||
       outfit.self_object_bakes()||outfit.self_appearance())return 50;
    std::cout<<"OGL_SELF_OBJECT_BAKE_CANDIDATE_AND_LO158_PRECEDENCE_PASS\n";
}
