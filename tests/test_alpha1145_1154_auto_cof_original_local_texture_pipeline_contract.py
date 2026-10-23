"""Alpha1154: original autoload COF local texlayer assets without F9; not an avatar bake."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(x):return (R/x).read_text()
class AutoCofLocalLayers(unittest.TestCase):
 def test_complete_auto_asset_readback_is_only_origin_of_auto_gettexture(self):
  s=rd("src/app/application.cpp")
  for term in ("auto_cof_wearable_assets_active=was_auto_cof_asset",
               "if(auto_cof_wearable_assets_active&&",
               "gallery_asset_errors==0U&&active_session",
               "gallery_decoded_wearable_assets.size()==",
               "gallery_asset_total",
               "gallery_texture_requested=true;",
               "auto_cof_texture_requested=true;",
               "auto_cof_wearable_assets_active=false;"):
   self.assertIn(term,s)
 def test_no_f9_required_but_normal_manual_texture_button_works(self):
  s=rd("src/app/application.cpp")
  a=s.index("if(gallery_texture_requested)")
  b=s.index("if(gallery_local_texture_future.valid()",a)
  x=s[a:b]
  for term in ("const bool from_auto_cof=auto_cof_texture_requested;",
               "(!outfit_gallery_open&&!from_auto_cof)",
               "gallery_plan_visible","gallery_texture_cap.empty()",
               "protocol::is_valid_http_uri(gallery_texture_cap)",
               "firestorm_region_lease.matches(current_firestorm_epoch)",
               "gallery_asset_errors!=0U",
               "plan_wearable_texture_preview(",
               "plan_wearable_texture_preview(\n",
               "auto_cof_layer_armed=from_auto_cof"):
   if term=="plan_wearable_texture_preview(\n":continue
   self.assertIn(term,x)
 def test_existing_bounded_server_gettexture_and_gpu_image_integration(self):
  s=rd("src/app/application.cpp")
  for term in ("plan_wearable_texture_preview(\n",
               "gallery_decoded_wearable_assets,12U",
               "gallery_local_gpu_textures.size()<12U",
               "result.image.width<=128U",
               "gallery_local_te_rgba.insert_or_assign(",
               "assets::TextureAssetLoader loader{transport}",
               "loader.fetch_rgba(cap,next.uuid)",
               "assets::wearable_preview_matches(",
               "gallery_local_epoch,gallery_local_cof,"):
   self.assertIn(term,s)
 def test_only_auto_begin_local_layer_after_complete_owner_cof_gpu_textures(self):
  s=rd("src/app/application.cpp")
  a=s.index("// Begin ONE local, bounded original Firestorm Layer")
  b=s.index("if(gallery_texlayer_compose_requested)",a)
  sub=s[a:b]
  for term in ("auto_cof_layer_armed",
               "!gallery_local_texture_future.valid()",
               "gallery_local_texture_pending.empty()",
               "!gallery_local_te_rgba.empty()",
               "gallery_texlayer_manifest->valid()",
               "firestorm_region_lease.matches(gallery_local_epoch)",
               "caps_state.current_outfit_folder==gallery_local_cof",
               "auto_cof_read.matches(active_session->agent_id",
               "gallery_texlayer_compose_requested=true",
               "auto_cof_compose_requested=true"):
   self.assertIn(term,sub)
 def test_partial_compositor_uses_original_lad_masks_and_refuses_bake_claims(self):
  s=rd("src/app/application.cpp")
  a=s.index("if(gallery_texlayer_compose_requested)")
  b=s.index("if(gallery_appearance_check_requested)",a)
  sub=s[a:b]
  for term in ("(!outfit_gallery_open&&!from_auto_cof)",
               "load_firestorm_character_tga(",
               "assets::evaluate_firestorm_global_colors(",
               "assets::compose_firestorm_mask_preview(",
               "gallery_partial_layer_gpu[group]=",
               "Bake NICHT freigegeben"):
   self.assertIn(term,sub)
  for mutation in ("AgentSetAppearance(", "outbound_queue.push(",
                   "gallery_link_consent.begin("):
   self.assertNotIn(mutation,sub)
 def test_cancel_invalid_image_without_fabricating_skin_evidence(self):
  s=rd("src/app/application.cpp")
  for term in ("auto_cof_layer_armed=false;",
               "gallery_local_te_rgba.empty()",
               "keine echte lokale RGBA-Quelle",
               "auto_cof_texture_requested=false;",
               "auto_cof_compose_requested=false;",
               "gallery_local_texture_status=",
               "Original-Wearables"):
   self.assertIn(term,s)
 def test_current_session_f3_layer_preview_shows_truth_not_bake(self):
  s=rd("src/app/application.cpp")
  for term in ("gallery_local_cof==cof&&",
               "gallery_local_epoch==current_firestorm_epoch",
               "gallery_local_cof_version==ver->second",
               "gallery_local_gpu_textures.size()",
               "Firestorm Layer-Preview lokal;",
               "kein Simulator-Bake"):
   self.assertIn(term,s)
 def test_manual_f9_not_falsely_attributed_to_auto_download(self):
  s=rd("src/app/application.cpp")
  self.assertIn("auto_cof_wearable_assets_active=was_auto_cof_asset",s)
  self.assertIn("const bool was_auto_cof_asset=auto_cof_asset_requested;",s)
  self.assertIn("gallery_texture_requested=false;",s)
  self.assertIn("const bool from_auto_cof=auto_cof_texture_requested;",s)
 def test_legacy_original_texlayer_gpu_composition_native_tests_exist(self):
  t=rd("tests/firestorm_global_mask_tests.cpp")
  for term in ("compose_firestorm","mask","color"):
   self.assertIn(term.lower(),t.lower())
 def test_real_live_outfit_bakes_remain_unverified(self):
  s=rd("docs/PROJECT_PROGRESS.json")
  self.assertIn("still open",s)
  self.assertIn("live",s.lower())
  self.assertIn("0.2.0-alpha.",s)
if __name__=="__main__":unittest.main()
