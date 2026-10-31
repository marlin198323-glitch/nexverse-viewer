"""RAM-only wearable original reuse and real HTTP-CAP rejection contracts."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(path):return (ROOT/path).read_text()
spec=importlib.util.spec_from_file_location(
    "ogl_asset_diag", ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)
class OriginalSessionCacheContracts(unittest.TestCase):
 def test_ram_only_bounded_container(self):
  h=read("include/nexverse/protocol/wearable_original_session_cache.hpp")
  self.assertIn("max_entries=24U",h)
  self.assertIn("std::deque<Entry> entries_",h)
  self.assertIn("entries_.pop_front()",h)
  self.assertNotIn("std::filesystem",h)
  self.assertNotIn("fstream",h)
 def test_cache_requires_authenticated_owner_and_session(self):
  h=read("include/nexverse/protocol/wearable_original_session_cache.hpp")
  self.assertIn("entry.owner==owner&&entry.session==session",h)
  self.assertIn("uuid_valid(owner)&&uuid_valid(session)",h)
 def test_asset_specific_key_includes_inventory_item(self):
  h=read("include/nexverse/protocol/wearable_original_session_cache.hpp")
  for x in ("entry.item_id==item.item_id","entry.asset_id==item.asset_id",
            "entry.asset_type==item.asset_type",
            "entry.inventory_type==item.inventory_type",
            "entry.flags==*item.flags"):
   self.assertIn(x,h)
 def test_only_bodypart_clothing_with_wearable_subtypes(self):
  h=read("include/nexverse/protocol/wearable_original_session_cache.hpp")
  for x in ("item.inventory_type==18&&item.flags",
            "(item.asset_type==5||item.asset_type==13)",
            "(*item.flags&0xffU)<17U",
            "(item.asset_type==13)==((*item.flags&0xffU)<4U)"):
   self.assertIn(x,h)
 def test_never_cache_an_unverified_original(self):
  h=read("include/nexverse/protocol/wearable_original_session_cache.hpp")
  self.assertIn("original.type!=static_cast<int>(*item.flags&0xffU)",h)
  self.assertIn("(original.type<4)!=(item.asset_type==13)",h)
  self.assertIn("max_firestorm_visual_params",h)
  self.assertIn("max_firestorm_local_textures",h)
 def test_cache_replaces_changed_inventory_asset(self):
  h=read("include/nexverse/protocol/wearable_original_session_cache.hpp")
  self.assertIn("std::erase_if(entries_",h)
  self.assertIn("e.item_id==item.item_id",h)
 def test_cache_clears_on_any_session_change(self):
  a=read("src/app/application.cpp")
  for x in ("cached_wearable_owner!=active_session->agent_id",
            "cached_wearable_session!=active_session->session_id",
            "wearable_original_session_cache.clear();"):
   self.assertIn(x,a)
 def test_cache_is_allowed_across_regions_but_stale_queue_is_cleared(self):
  a=read("src/app/application.cpp")
  self.assertIn('gallery_cached_wearable_result.reset();',a)
  self.assertIn('gallery_asset_status="Region gewechselt:',a)
  self.assertIn('gallery_asset_status=\n                                    "COF veraendert:',a)
 def test_cache_hit_still_passes_common_original_validator(self):
  a=read("src/app/application.cpp")
  for x in ("wearable_original_session_cache.find(",
            "gallery_cached_wearable_result=", 
            "const bool from_ram=gallery_cached_wearable_result.has_value()",
            "const bool scope=gallery_asset_epoch==current_firestorm_epoch",
            "original->flags", "original->asset_type==13"):
   self.assertIn(x,a)
 def test_receive_side_rechecks_current_owner_session(self):
  a=read("src/app/application.cpp")
  self.assertIn(
      "active_session->session_id==gallery_asset_source_session",a)
  self.assertIn(
      "active_session->agent_id==gallery_wearables_fetch_owner",a)
  self.assertIn(
      "gallery_asset_source_session=active_session->session_id",a)
  self.assertIn("gallery_asset_source_session.clear()",a)
 def test_no_lludp_request_on_cache_hit(self):
  a=read("src/app/application.cpp")
  p=a.index("const auto cached=wearable_original_session_cache.find(")
  q=a.index("protocol::WearableAssetRequest request{",p)
  x=a[p:q]
  self.assertIn("if(cached)",x)
  self.assertIn("else if(!gallery_viewer_asset_cap.empty()",x)
 def test_cache_does_not_falsify_server_bakes(self):
  a=read("src/app/application.cpp")
  p=a.index("const auto cached=wearable_original_session_cache.find(")
  q=a.index("protocol::WearableAssetRequest request{",p)
  x=a[p:q]
  self.assertIn("kein Bake",x)
  self.assertNotIn("AgentSetAppearance(",x)
 def test_http_unsupported_methods_are_specific(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  self.assertIn("http_status==405L||http_status==501L",h)
  self.assertIn("result.http_status=response.status_code",h)
  self.assertNotIn("http_status==404L",h)
 def test_only_method_rejections_disable_cap(self):
  a=read("src/app/application.cpp")
  p=a.index("if(response.method_unsupported())")
  q=a.index("gallery_viewer_asset_fallback_ids.insert(",p)
  self.assertIn("gallery_viewer_asset_cap.clear()",a[p:q])
 def test_native_tests_validate_cache_isolation_and_bounds(self):
  x=read("tests/firestorm_wearable_transfer_tests.cpp")
  for t in ("WearableOriginalSessionCache original_cache",
            "original_cache.find(TX,TID,verified_item)",
            "original_cache.find(ASSET,TX,verified_item)",
            "original_cache.size()!=WearableOriginalSessionCache::max_entries",
            "original_cache.clear()"):
   self.assertIn(t,x)
 def test_native_http_405_501_cases_present(self):
  x=read("tests/firestorm_wearable_transfer_tests.cpp")
  self.assertIn("http->code=405",x)
  self.assertIn("http->code=501",x)
  self.assertIn("unsupported.method_unsupported()",x)
 def test_cli_reports_real_events_without_credentials(self):
  raw=('2026-10-31 00:51:00 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:51:01 [INFO] Avatar original wearable RAM cache hit: matched\n'
       '2026-10-31 00:51:02 [WARN] Avatar ViewerAsset method unsupported: LLUDP source now preferred\n')
  report='\n'.join(diag.summarize(raw))
  self.assertIn('session_ram_hits=1',report)
  self.assertIn('unsupported_cap_methods=1',report)
  self.assertNotIn('Body-Bakes bestätigt',report)
if __name__=="__main__":unittest.main()
