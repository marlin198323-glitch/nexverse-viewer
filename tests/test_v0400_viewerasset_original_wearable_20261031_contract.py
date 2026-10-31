"""Authenticated OpenSim ViewerAsset wearable retrieval, failover, no fake bakes."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(path):return (ROOT/path).read_text()
spec=importlib.util.spec_from_file_location("asset_diagnose",ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)

class ViewerAssetOriginals(unittest.TestCase):
 def test_cap_is_discovered_from_authenticated_seed_only(self):
  cpp=read("src/protocol/firestorm_region_read.cpp")
  h=read("include/nexverse/protocol/firestorm_region_read.hpp")
  self.assertIn('"GetTexture","ViewerAsset"',cpp)
  self.assertIn('caps.capabilities.find("ViewerAsset")',cpp)
  self.assertIn('viewer_asset_uri',h)
  self.assertIn('is_valid_http_uri(it->second)',cpp)
 def test_get_only_original_wearable_not_bakes(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  self.assertIn('?bodypart_id=',h)
  self.assertIn('?clothing_id=',h)
  self.assertIn('transport->get(*url,',h)
  self.assertNotIn('transport->post(',h)
  self.assertNotIn('AgentSetAppearance(',h)
 def test_reject_unsafe_uri_and_invalid_asset_uuid(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  for text in ('is_valid_http_uri(authenticated_cap)',"authenticated_cap.size()>4096U",
               "authenticated_cap.find('?')","authenticated_cap.find('#')",
               'uuid_to_bytes(asset_id)', 'asset_type!=asset_type_bodypart'):
   self.assertIn(text,h)
 def test_bounded_http_lease_and_type_validated(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  for text in ("assets::max_firestorm_wearable_bytes",
               "std::chrono::seconds{3}","std::chrono::seconds{10}",
               "lease.matches(epoch)", "parse_firestorm_wearable(",
               "(asset_type==asset_type_bodypart)!=(decoded.asset->type<4)"):
   self.assertIn(text,h)
 def test_data_tied_to_original_inventory_on_render_thread(self):
  a=read("src/app/application.cpp")
  self.assertIn("gallery_wearable_originals.begin()",a)
  self.assertIn("original->flags",a)
  self.assertIn("original->asset_type==13",a)
  self.assertIn("response.asset_id==gallery_asset_active_id",a)
  self.assertIn("response.epoch==gallery_asset_epoch",a)
 def test_failed_http_one_udp_retry_from_same_real_item(self):
  a=read("src/app/application.cpp")
  for text in ("gallery_viewer_asset_fallback_ids.insert(",
               "gallery_asset_pending.insert(",
               "gallery_asset_pending.begin(),*item",
               "gallery_viewer_asset_fallback_ids.contains(next.asset_id)",
               "wearable_transfers.begin(gallery_asset_epoch,request)"):
   self.assertIn(text,a)
 def test_bad_response_not_accepted_as_avatar_appearance(self):
  a=read("src/app/application.cpp")
  p=a.index("// HTTP ViewerAsset can deliver wearable-original bytes")
  q=a.index("// Explicit F9 click.",p)
  for bad in ("encode_agent_set_appearance(", "encode_agent_is_now_wearing(",
              "gallery_cof_batch.stage(","AgentIsNowWearing"):
   self.assertNotIn(bad,a[p:q])
 def test_http_future_is_one_shot_and_nonblocking(self):
  a=read("src/app/application.cpp")
  self.assertIn("viewerasset_completed.reset();",a)
  self.assertIn("gallery_viewer_asset_future.wait_for(",a)
  self.assertIn("std::chrono::seconds{0}",a)
  self.assertIn("shutdown_wait(gallery_viewer_asset_future,",a)
 def test_udp_not_broken_if_cap_unoffered(self):
  a=read("src/app/application.cpp")
  self.assertIn("if(!gallery_viewer_asset_cap.empty()&&",a)
  self.assertIn("protocol::WearableAssetRequest request{",a)
  self.assertIn("outbound_queue.push(request)",a)
 def test_native_fixtures_include_404_mismatch_stale(self):
  a=read("tests/firestorm_wearable_transfer_tests.cpp")
  for x in ("struct TestViewerAsset", "mismatched_category",
            "http->code=404", "lease.enter(2U,2U,",
            "fetch_wearable_viewer_asset("):
   self.assertIn(x,a)
 def test_no_secrets_logged(self):
  a=read("src/app/application.cpp")
  self.assertIn("CAP URL secret omitted",a)
  self.assertIn("asset/CAP URL omitted",a)
  self.assertIn("no bake, asset ID or auth tokens logged",a)
  self.assertIn("no URLs, IDs or response bodies logged",a)
 def test_diagnostic_counts_failed_and_accepted(self):
  raw=('2026-10-31 00:42:04 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:42:05 [INFO] Avatar original asset transports: ViewerAsset CAP available=1; LLUDP fallback\n'
       '2026-10-31 00:42:07 [INFO] Avatar original wearable request: authenticated ViewerAsset GET (read-only)\n'
       '2026-10-31 00:42:08 [INFO] Avatar original wearable ViewerAsset: bounded HTTP attempt failed; fallback LLUDP queued\n'
       '2026-10-31 00:42:08 [INFO] Avatar original wearable request: bounded LLUDP transfer dispatched\n'
       '2026-10-31 00:42:09 [INFO] Avatar original wearable accepted: original validated\n')
  msg='\n'.join(diag.summarize(raw))
  self.assertIn("Wearable-HTTP-CAP angeboten: ja",msg)
  for item in ("http_requests=1","http_fallback=1","udp_dispatches=1",
               "originals_accepted=1"):
   self.assertIn(item,msg)
 def test_diagnostic_no_legacy_success_claim_without_real_gpu(self):
  raw=('2026-10-31 00:42:04 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:42:09 [INFO] Avatar original wearable accepted: original validated\n')
  msg='\n'.join(diag.summarize(raw))
  self.assertNotIn("EIGENER Avatar – lokale Teilvorschau:",msg)
 def test_existing_low382_cof_guards_preserved(self):
  a=read("src/app/application.cpp")
  for x in ("legacy_preview_current()", "legacy_cof_raw_empty",
            "legacy_originals_serial","legacy_originals_session",
            "avatar_bake_stream_status"):
   self.assertIn(x,a)
 def test_build_contract_not_permitted_to_skip_gates(self):
  a=read("CHATUEBERGABE.md")
  for x in ("ZEHN-BLÖCKE-REGEL","./build/debug/nexverse-viewer"):
   self.assertIn(x,a)
if __name__=="__main__":unittest.main()
