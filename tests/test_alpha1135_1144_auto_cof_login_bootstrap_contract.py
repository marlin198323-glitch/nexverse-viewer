"""Automatic read-only authenticated COF Shape bootstrap after login: no F9 required."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(rel):return (R/rel).read_text()
class AutoCofBootstrap(unittest.TestCase):
 def test_once_per_authenticated_owner_region_cof_version(self):
  h=read("include/nexverse/assets/firestorm_auto_cof_read.hpp")
  for x in ("class FirestormAutoCofRead","owner.empty()",
            "cof.empty()","epoch==0U","version==0U",
            "std::find(attempts_.begin(),attempts_.end(),next)",
            "attempts_.size()>=64U", "attempts_.push_back(next)",
            "void reset()"):
   self.assertIn(x,h)
  t=read("tests/firestorm_avatar_mesh_tests.cpp")
  self.assertIn("AUTO_COF_AUTHENTICATED_ONCE_PER_OWNER_LEASE_VERSION_PASS",t)
 def test_normal_login_bootstrap_is_not_manual_f9(self):
  s=read("src/app/application.cpp")
  a=s.index("// One bounded, READ-ONLY original COF fetch")
  b=s.index("if(gallery_wearables_requested)",a)
  x=s[a:b]
  for phrase in ("!gallery_wearables_fetch_future.valid()",
    "!gallery_wearables_visible",
    "!gallery_plan_visible","active_session&&simulation_ready&&login_ui.authenticated",
    "gallery_inventory_items_cap.empty()",
    "firestorm_region_lease.matches(current_firestorm_epoch)",
    "auto_cof_read.begin(active_session->agent_id",
    "gallery_wearables_requested=true"):
   self.assertIn(phrase,x)
  self.assertNotIn("outfit_gallery_open",x)
 def test_original_owner_epoch_inventory_scope_retained(self):
  s=read("src/app/application.cpp")
  a=s.index("if(gallery_wearables_requested)")
  b=s.index("if(gallery_wearable_asset_requested)",a)
  x=s[a:b]
  for phrase in ("(!outfit_gallery_open&&!auto_cof_items_fetching)",
     "gallery_wearables_fetch_owner=active_session->agent_id",
     "gallery_wearables_fetch_epoch=current_firestorm_epoch",
     "gallery_wearables_fetch_version=v->second",
     "protocol::fetch_inventory2(", "auto_cof_items_fetching=false"):
   self.assertIn(phrase,x)
 def test_followup_original_assets_only_after_complete_ready_readback(self):
  s=read("src/app/application.cpp")
  a=s.index("if(gallery_wearables_fetch_future.valid()&&")
  b=s.index("if(gallery_wearables_requested)",a)
  x=s[a:b]
  for phrase in ("fetched.valid&&active_session",
     "gallery_wearables_fetch_epoch",
     "caps_state.current_outfit_folder==",
     "auto_cof_items_fetching&&",
     "ready_for_appearance_pipeline",
     "gallery_wearable_asset_requested=true",
     "auto_cof_asset_requested=true"):
   self.assertIn(phrase,x)
  self.assertIn("(!outfit_gallery_open&&!was_auto_cof_asset)",s)
  self.assertIn("gallery_asset_total=gallery_asset_pending.size()",s)
  self.assertIn("gallery_asset_pending.size()>16U",s)
 def test_read_only_caps_recovery_when_initial_cof_metadata_missing(self):
  s=read("src/app/application.cpp")
  a=s.index("// Login should not require F9")
  b=s.index("// FetchInventory2 resolves selected outfit",a)
  x=s[a:b]
  for phrase in ("auto_cof_metadata_missing=",
    "login_ui.authenticated&&simulation_ready&&active_session",
    "bootstrap_items==caps_state.outfit_folder_items.end()",
    "outfit_gallery_open||auto_cof_metadata_missing",
    "std::chrono::seconds{30}",
    "firestorm_region_lease.matches(current_firestorm_epoch)",
    "protocol::fetch_outfit_descendents("):
   self.assertIn(phrase,x)
  self.assertNotIn("gallery_link_consent.begin(",x)
 def test_f3_status_truthful_and_user_can_keep_f9_closed(self):
  s=read("src/app/application.cpp")
  a=s.index("// The automatic COF read is a real")
  b=s.index("if((render_diagnostics.self_avatar_shape_body_gl_draws",a)
  x=s[a:b]
  for phrase in ("auto_cof_read.matches(active_session->agent_id",
    "COF Auto: Originalinventar wird geladen",
    "COF Auto: Original-Wearables werden geladen",
    "Bake/Outfit-E2E offen",
    "COF Auto: Inventar-/Asset-Beleg fehlt"):
   self.assertIn(phrase,x)
  self.assertNotIn("active_session->session_id",x)
 def test_final_release_requires_real_live_evidence_not_synthetic_gpu(self):
  s=read("scripts/ogl-release-readiness.py")
  for phrase in ("authenticated_live_grid_3_body_bakes_gpu_rgb",
                 "authenticated_live_grid_actual_outfit_and_cof",
                 "authenticated_live_grid_server_confirmed_wasd_yaw",
                 "windows_macos_linux_release_builds",
                 'proof[name].get("verified") is not True',
                 "FINAL RELEASE: BLOCKED"):
   self.assertIn(phrase,s)
 def test_fail_closed_on_stale_epoch_and_only_read_actions(self):
  s=read("src/app/application.cpp")
  for phrase in ("auto_cof_read.reset();",
    "auto_cof_items_fetching=false;",
    "auto_cof_asset_requested=false;",
    "gallery_wearables_fetch_owner==",
    "gallery_asset_epoch==current_firestorm_epoch",
    "gallery_asset_errors==0U",
    "gallery_decoded_wearable_assets.size()=="):
   self.assertIn(phrase,s)
  a=s.index("// One bounded, READ-ONLY original COF fetch")
  b=s.index("// A completed transfer is not a successful bake.",a)
  sub=s[a:b]
  for unwanted in ("AgentSetAppearance(","AddInventoryItem(",
                   "gallery_link_consent.begin(","gallery_cof_batch.begin("):
   self.assertNotIn(unwanted,sub)
if __name__=="__main__":unittest.main()
