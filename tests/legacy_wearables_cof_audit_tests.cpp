/* MPL-2.0: read-only vintage Low382 vs authenticated COF originals. */
#include "nexverse/protocol/legacy_wearables_cof_audit.hpp"
#include <iostream>
using namespace nexverse::protocol;
static const char* u1="11111111-1111-1111-1111-111111111111";
static const char* u2="22222222-2222-2222-2222-222222222222";
static const char* a1="aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
static const char* a2="bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
static const char* zero="00000000-0000-0000-0000-000000000000";
int main()
{
    AgentWearablesUpdateMessage legacy{};
    legacy.wearables={{u1,a1,0U},{u2,a2,4U}};
    std::vector<OutfitInventoryItem> originals{
        {u1,a1,"shape",13,18,0U},{u2,a2,"shirt",5,18,4U}};
    std::vector<OutfitInventoryItem> links{
        {"link1",u1,"shape link",24,18},
        {"link2",u2,"shirt link",24,18}};
    if(compare_legacy_wearables_cof(nullptr,&links,&originals)
       .cof_originals_available)return 1;
    if(compare_legacy_wearables_cof(&legacy,nullptr,&originals)
       .cof_originals_available)return 2;
    auto result=compare_legacy_wearables_cof(&legacy,&links,&originals);
    if(!result.legacy_received||!result.cof_originals_available||
       result.exact_item_asset_type!=2U||result.cof_wearables!=2U||
       result.can_prove_worn()||
       result.label().find("kein Wear-Beweis")==std::string::npos)
        return 3;
    auto stale=legacy;
    stale.wearables[0].asset_id=a2;
    result=compare_legacy_wearables_cof(&stale,&links,&originals);
    if(result.exact_item_asset_type!=1U||
       result.mismatched_asset_or_type!=1U||
       result.cof_not_in_legacy!=1U)return 4;
    stale=legacy;
    stale.wearables[1].wearable_type=5U;
    result=compare_legacy_wearables_cof(&stale,&links,&originals);
    if(result.mismatched_asset_or_type!=1U)return 5;
    stale=legacy;
    stale.wearables.push_back({u1,a1,0U});
    result=compare_legacy_wearables_cof(&stale,&links,&originals);
    if(result.duplicate_legacy_items!=1U||
       result.exact_item_asset_type!=2U||
       result.label().find("D:1")==std::string::npos||
       result.label().find("kein Wear-Beweis")==std::string::npos||
       result.label().size()>99U)return 6;
    stale=legacy;
    stale.wearables[1]={zero,zero,4U};
    result=compare_legacy_wearables_cof(&stale,&links,&originals);
    if(result.cof_not_in_legacy!=1U||
       result.exact_item_asset_type!=1U)return 7;
    auto unknown=links;unknown.push_back({"link3","other","",24,18});
    result=compare_legacy_wearables_cof(&legacy,&unknown,&originals);
    if(result.unresolved_cof_links!=1U||
       result.exact_item_asset_type!=2U)return 8;
    auto malformed=originals;malformed[0].flags.reset();
    result=compare_legacy_wearables_cof(&legacy,&links,&malformed);
    if(result.unresolved_cof_links!=1U||
       result.cof_wearables!=1U||result.not_in_cof!=1U)return 9;
    stale=legacy;stale.wearables.clear();
    result=compare_legacy_wearables_cof(&stale,&links,&originals);
    if(!result.legacy_received||
       result.cof_not_in_legacy!=2U||
       result.exact_item_asset_type!=0U)return 10;
    if(result.label().find(u1)!=std::string::npos||
       result.label().find(a1)!=std::string::npos)return 11;
    // Worst-case synthetic audit counters may grow to 255. Preserve
    // the explicit disclaimer in the actual F9 label clipping range.
    LegacyWearableCofAudit overloaded{};
    overloaded.legacy_received=true;
    overloaded.cof_originals_available=true;
    overloaded.legacy_items=255U;
    overloaded.cof_wearables=255U;
    overloaded.exact_item_asset_type=255U;
    overloaded.mismatched_asset_or_type=255U;
    overloaded.cof_not_in_legacy=255U;
    overloaded.not_in_cof=255U;
    overloaded.duplicate_legacy_items=255U;
    overloaded.unresolved_cof_links=255U;
    if(overloaded.label().size()>99U||
       overloaded.label().find("kein Wear-Beweis")==std::string::npos)
        return 12;
    std::cout<<"OGL_LEGACY_COF_READONLY_AUDIT_ZERO_FAKE_WORN_PASS\n";
}
