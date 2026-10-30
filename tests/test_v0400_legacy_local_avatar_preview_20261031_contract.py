"""Safety and actual GL/provenance contracts for self-only Low382 local fallback."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(path):return (ROOT/path).read_text()
spec=importlib.util.spec_from_file_location("ogl_diag",ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)

class LocalAvatarPreview(unittest.TestCase):
 def test_authenticated_five_originals_can_only_start_local(self):
  h=source("include/nexverse/protocol/legacy_wearable_local_preview.hpp")
  for s in ("evidence.complete_primary","evidence.missing==0U",
            "evidence.matched==evidence.requested",
            "server_cof_raw_empty","source_epoch==current_epoch",
            "source_serial==current_serial",
            "owner==current_owner","session==current_session"):
   self.assertIn(s,h)
 def test_native_rejects_stale_or_other_account(self):
  cpp=source("tests/agent_wearables_readback_tests.cpp")
  for s in ("legacy_local_preview_admitted(", "proof,false,9U,9U",
            "proof,true,9U,10U", "proof,true,9U,9U,42U,43U",
            "proof,true,9U,9U,42U,42U,self_id,other_id"):
   self.assertIn(s,cpp)
 def test_wire_empty_necessary_not_parser_empty(self):
  cpp=source("src/app/application.cpp")
  self.assertIn("update.cof_items_array_present&&",cpp)
  self.assertIn("update.cof_raw_items==0U",cpp)
  self.assertIn("update.cof_rejected_items==0U",cpp)
  self.assertIn("legacy_cof_raw_version=found->second",cpp)
 def test_no_malformed_cof_as_source(self):
  cpp=source("src/app/application.cpp")
  self.assertIn("legacy_cof_raw_empty&&",cpp)
  self.assertIn("legacy_cof_raw_epoch==current_firestorm_epoch",cpp)
  self.assertIn("legacy_cof_raw_version==version->second",cpp)
 def test_local_preview_plan_uses_authenticated_inventory_items(self):
  app=source("src/app/application.cpp")
  self.assertIn("gallery_wearable_originals=originals.items",app)
  self.assertIn("gallery_asset_pending.push_back(item)",app)
  self.assertIn("gallery_asset_pending.size()==proof.matched",app)
  self.assertIn("legacy_local_preview_proof=proof",app)
 def test_bounded_download_counts_and_no_invented_assets(self):
  a=source("src/app/application.cpp")
  self.assertIn("originals.items.size()<=16U",a)
  self.assertIn("asset_keys.insert(item.asset_id).second",a)
  self.assertIn("protocol::uuid_to_bytes(item.asset_id)",a)
 def test_local_only_does_not_emit_mutations(self):
  a=source("src/app/application.cpp")
  p=a.index("const auto legacy_preview_current=[&]()")
  q=a.index("// FetchInventory2 resolves selected outfit",p)
  excerpt=a[p:q]
  for forbidden in ("encode_agent_set_appearance(", "encode_agent_is_now_wearing(",
                    "gallery_cof_batch.stage(", "outbound_queue.push("):
   self.assertNotIn(forbidden,excerpt)
 def test_low382_gated_by_current_region_agent_session_serial(self):
  a=source("src/app/application.cpp")
  p=a.index("const auto legacy_preview_current=[&]()")
  q=a.index("// If current-region InventoryDescendents2 accepted no COF",p)
  x=a[p:q]
  for field in ("legacy_originals_serial,worn->serial",
                "legacy_originals_agent,worn->agent_id",
                "legacy_originals_session,worn->session_id",
                "world_model.region().connected"):
   self.assertIn(field,x)
 def test_on_stale_source_all_gpu_previews_cleared(self):
  a=source("src/app/application.cpp")
  p=a.index("if(legacy_local_preview_active&&!legacy_preview_current())")
  q=a.index("// If current-region InventoryDescendents2 accepted no COF",p)
  self.assertIn("clear_gallery_local_texture_previews();",a[p:q])
  self.assertIn("legacy_local_preview_proof={};",a[p:q])
 def test_existing_cof_pipeline_still_operates(self):
  a=source("src/app/application.cpp")
  self.assertIn("auto_cof_read.matches(active_session->agent_id,",a)
  self.assertIn("||\n                        legacy_preview_current())",a)
  self.assertIn("||\n                    legacy_preview_current()))",a)
 def test_only_self_preview_gl_and_real_unbaked_source(self):
  a=source("src/app/application.cpp")
  self.assertIn("set_self_avatar_local_layer_preview(",a)
  self.assertIn("render::select_avatar_bake_previews(",a)
  self.assertIn("Server-Bake NICHT freigegeben",a)
  self.assertIn("authenticated_legacy_originals",a)
 def test_renderer_reports_real_gl_success_not_declared_success(self):
  a=source("src/app/application.cpp")
  self.assertIn("if(world_renderer->\n                                set_self_avatar_local_layer_preview(",a)
  self.assertIn("++local_avatar_gpu_slots;",a)
  self.assertIn('" gpu_head_upper_lower="',a)
 def test_diagnostic_old_session_does_not_claim_gpu_done(self):
  raw=('2026-10-31 00:31:12 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:31:17 [INFO] Avatar legacy originals readback: '
       'valid=1 requested=5 matched=5 bodyparts=4 complete_primary=1\n')
  r='\n'.join(diag.summarize(raw))
  self.assertIn('matched=5',r)
  self.assertNotIn('EIGENER Avatar – lokale Teilvorschau:',r)
 def test_diagnostic_queued_is_not_gpu_success(self):
  raw=('2026-10-31 00:31:12 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:31:17 [INFO] Avatar legacy local preview: '
       'verified originals admitted to current LLWearable/local layer pipeline\n')
  r='\n'.join(diag.summarize(raw))
  self.assertIn('GPU-Head-/Upper-/Lower-Uploads noch NICHT nachgewiesen',r)
 def test_diagnostic_gl_slots_distinguishes_true_source(self):
  raw=('2026-10-31 00:31:12 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:31:17 [INFO] Avatar local unbaked self-preview: '
       'source=authenticated_legacy_originals gpu_head_upper_lower=2/3 '
       'real_local_source_textures=4 incomplete_layers=5; '
       'server_bakes_not_claimed\n')
  r='\n'.join(diag.summarize(raw))
  self.assertIn('gpu_head_upper_lower=2/3',r)
  self.assertIn('KEINE vom Grid',r)
  self.assertNotIn('vollständiger Avatar',r)
 def test_original_start_commands_and_project_status_preserved(self):
  s=source("CHATUEBERGABE.md")
  for x in ('cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"',
            "python3 scripts/ogl-cache-status.py","./build/debug/nexverse-viewer"):
   self.assertIn(x,s)
if __name__=="__main__":unittest.main()
