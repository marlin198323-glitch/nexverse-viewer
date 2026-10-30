"""October 31 authenticated legacy fallback, COF parser proof, safe texture GPU policy."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(path):return (ROOT/path).read_text()
spec=importlib.util.spec_from_file_location("ogl_asset_diag",ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)

class AuthenticatedLegacyOriginalsContracts(unittest.TestCase):
 def test_low382_originals_read_only_helper_present(self):
  a=source("include/nexverse/protocol/legacy_wearable_inventory_probe.hpp")
  self.assertIn("plan_legacy_wearable_inventory_probe(",a)
  self.assertIn("audit_legacy_wearable_inventory_probe(",a)
  self.assertIn("uuid_to_bytes",a)
  self.assertIn("report->agent_id!=authenticated_agent",a)
  self.assertIn("report->session_id!=authenticated_session",a)
 def test_no_fabricated_originals_or_duplicate_slots(self):
  h=source("include/nexverse/protocol/legacy_wearable_inventory_probe.hpp")
  for t in ("w.item_id==zero", "w.asset_id==zero",
            "w.wearable_type>=17U", "types[w.wearable_type]",
            "!ids.insert(w.item_id).second"):
   self.assertIn(t,h)
 def test_original_item_subtype_flags_and_asset_match(self):
  h=source("include/nexverse/protocol/legacy_wearable_inventory_probe.hpp")
  for t in ("item->asset_id!=w.asset_id", "item->inventory_type!=18",
            "(*item->flags&0xffU)!=w.wearable_type",
            "item->asset_type!=(w.wearable_type<4U?13:5)",
            "out.complete_primary=out.bodyparts==4U&&out.missing==0U"):
   self.assertIn(t,h)
 def test_fetch_inventory_requires_current_owner_epoch_and_session(self):
  s=source("src/app/application.cpp")
  for t in ("legacy_originals_epoch=current_firestorm_epoch",
            "legacy_originals_agent=active_session->agent_id",
            "legacy_originals_session=active_session->session_id",
            "legacy_originals_serial=serial",
            "worn->serial==legacy_originals_serial",
            "firestorm_region_lease.matches(legacy_originals_epoch)",
            "protocol::fetch_inventory2("):
   self.assertIn(t,s)
 def test_only_confirmed_empty_processed_cof_starts_legacy_lookup(self):
  s=source("src/app/application.cpp")
  p=s.index("if(!legacy_originals_future.valid()&&")
  q=s.index("// FetchInventory2 resolves selected outfit",p)
  block=s[p:q]
  for t in ("gallery_inventory_sync.read()", "cof->second.empty()",
            "version!=gallery_inventory_sync.versions().end()",
            "legacy_originals_requests<3U",
            "plan_legacy_wearable_inventory_probe("):
   self.assertIn(t,block)
 def test_async_is_joined_and_epoch_state_cleared(self):
  s=source("src/app/application.cpp")
  for t in ("legacy_originals_plan={};",
            "legacy_originals_requests=0U;",
            "shutdown_wait(legacy_originals_future,",
            "Avatar legacy original readback discarded:"):
   self.assertIn(t,s)
 def test_no_bake_or_outfit_mutation_in_fallback(self):
  s=source("src/app/application.cpp")
  p=s.index("// If current-region InventoryDescendents2 accepted no COF")
  q=s.index("// FetchInventory2 resolves selected outfit",p)
  block=s[p:q]
  for bad in ("AgentSetAppearance(", "AgentIsNowWearing(",
              "CreateInventoryItem(", "AddInventoryItem(",
              "gallery_cof_batch.begin(", "gallery_link_consent.begin("):
   self.assertNotIn(bad,block)
 def test_cof_wire_raw_count_and_parse_rejections(self):
  a=source("src/protocol/firestorm_inventory_fetch.cpp")
  h=source("include/nexverse/protocol/firestorm_inventory_fetch.hpp")
  for k in ("cof_raw_items", "cof_rejected_items",
            "cof_folder_seen", "cof_items_array_present"):
   self.assertIn(k,a)
   self.assertIn(k,h)
  self.assertIn("result.cof_raw_items-dest.size()",a)
 def test_native_covers_real_empty_and_rejected_cof(self):
  c=source("tests/firestorm_inventory_fetch_tests.cpp")
  self.assertIn("const auto truly_empty=read_cof_test({});",c)
  self.assertIn("const auto discarded=read_cof_test({",c)
  self.assertIn("discarded.cof_rejected_items!=1U",c)
 def test_native_covers_wearable_original_mismatch(self):
  c=source("tests/agent_wearables_readback_tests.cpp")
  for t in ("plan_legacy_wearable_inventory_probe(", "proof.complete_primary",
            "mismatch.matched!=3U", "legacy.wearables.push_back("):
   self.assertIn(t,c)
 def test_compaction_never_degrades_avatar_eyes_or_hair(self):
  a=source("src/app/application.cpp")
  p=a.index("std::unordered_set<std::string> protected_ids")
  q=a.index("choose_scene_texture_compaction(",p)
  for t in ("admitted_bakes.accepted","avatar.body",
            "avatar.eyes_bake_id","avatar.hair_bake_id"):
   self.assertIn(t,a[p:q])
 def test_avatar_status_clearly_mentions_server_cof_not_fake_bakes(self):
  a=source("src/app/application.cpp")
  self.assertIn("COF ohne verifizierte Items; Raw-LLSD/Low382 pruefen",a)
  self.assertIn("Outfit/Server-Bake trotzdem UNBEWIESEN",a)
 def test_cli_old_binary_does_not_claim_server_empty(self):
  raw=('2026-10-31 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:02 [INFO] Avatar COF readback: '
       'valid=1 owner_region_current=1 returned_folders=1 '
       'cof_version_present=1 cof_item_records=0\n')
  out='\n'.join(diag.summarize(raw))
  self.assertIn("Rohdaten nicht belegt",out)
  self.assertNotIn("authentisch leerem",out)
 def test_cli_rejected_raw_items_are_distinguished_from_empty(self):
  raw=('2026-10-31 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:02 [INFO] Avatar COF readback: valid=1 '
       'owner_region_current=1 cof_item_records=0 cof_raw_items=3 '
       'cof_rejected_items=3 cof_items_array_present=1\n'
       '2026-10-31 00:00:03 [INFO] Avatar legacy originals readback: '
       'valid=1 requested=5 matched=4 bodyparts=4 complete_primary=0\n')
  out='\n'.join(diag.summarize(raw))
  self.assertIn("Parser-Problem moeglich",out)
  self.assertIn("matched=4",out)
  self.assertNotIn("authentisch leerem",out)
 def test_cli_actual_server_empty_is_proven_by_wire_level(self):
  raw=('2026-10-31 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:02 [INFO] Avatar COF readback: '
       'valid=1 owner_region_current=1 cof_item_records=0 '
       'cof_raw_items=0 cof_rejected_items=0 cof_items_array_present=1\n')
  out='\n'.join(diag.summarize(raw))
  self.assertIn("authentisch leerem Items-Array",out)
 def test_immutable_login_commands_retained(self):
  s=source("CHATUEBERGABE.md")
  for t in ('cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"',
            "python3 scripts/ogl-cache-status.py",
            "./build/debug/nexverse-viewer"):
   self.assertIn(t,s)
if __name__=="__main__": unittest.main()
