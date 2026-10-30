"""31 Oct: no redundant GPU mesh uploads, bounded scene fallback, COF diagnostics."""
import importlib.util
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
def src(p):return (ROOT/p).read_text()
spec=importlib.util.spec_from_file_location("asset_diagnosis",ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)

class MeshRescueCofContracts(unittest.TestCase):
 def test_live_mesh_duplicate_root_cause_fixed(self):
  a=src("src/app/application.cpp")
  self.assertEqual(a.count("assets::mesh_gpu_request_needed("),2)
  self.assertNotIn("const bool fits=(existing!=mesh_gpu_lods.end()&&",a)
 def test_single_shared_mesh_eligibility(self):
  h=src("include/nexverse/assets/mesh_lod_policy.hpp")
  for s in ('mesh_gpu_request_needed(', 'current->second==target',
            'return false;', 'gpu_ids.contains(id)||gpu_ids.size()<gpu_slots'):
   self.assertIn(s,h)
 def test_native_mesh_regression_for_same_lod(self):
  cpp=src("tests/mesh_lod_policy_tests.cpp")
  self.assertIn('if(mesh_gpu_request_needed("a",MeshLod::high',cpp)
  self.assertIn('if(!mesh_gpu_request_needed("a",MeshLod::medium',cpp)
  self.assertIn('if(mesh_gpu_request_needed("new",MeshLod::high',cpp)
 def test_cached_meshes_not_starf_network(self):
  a=src("src/app/application.cpp")
  self.assertIn("mesh_request_fairness.dispatched_count()%3U!=2U",a)
  self.assertIn("if(!request_choice)",a)
  self.assertIn("pending_mesh_ids.insert(task.id)",a)
 def test_cof_immediate_once_per_region(self):
  a=src("src/app/application.cpp")
  for text in ("gallery_bootstrap_cof_polled_epoch",
               "gallery_bootstrap_cof_polled_epoch!=current_firestorm_epoch",
               "gallery_bootstrap_cof_polled_epoch=current_firestorm_epoch",
               "auto_cof_metadata_missing &&"):
   self.assertIn(text,a)
 def test_no_trusted_cof_in_login_is_explicit(self):
  a=src("src/app/application.cpp")
  self.assertIn("bootstrap_cof.empty()||auto_cof_metadata_missing",a)
  self.assertIn("skeleton_cof_available=",a)
  self.assertIn("current_region_lease=",a)
  self.assertIn("inventory_cap_available=",a)
  self.assertIn("fetch_cap_available=",a)
 def test_cof_does_not_mutate_server_appearance(self):
  a=src("src/app/application.cpp")
  start=a.index("// Login should not require F9")
  end=a.index("// FetchInventory2 resolves selected outfit",start)
  scoped=a[start:end]
  self.assertIn("protocol::fetch_outfit_descendents",scoped)
  self.assertNotIn("encode_agent_set_appearance",scoped)
  self.assertNotIn("gallery_cof_batch.begin(",scoped)
 def test_scene_rescue_preserves_texture_gpu_limits(self):
  s=src("include/nexverse/assets/scene_texture_budget_rescue.hpp")
  for token in ("effective_gpu_family_budget(", "gpu_staging_budget(",
                "plan_texture_byte_budget(", "current_mip+extra<=6U",
                "if(fits(width,height))return std::nullopt;"):
   self.assertIn(token,s)
 def test_scene_rescue_never_uses_avatar_bakes(self):
  a=src("src/app/application.cpp")
  for phrase in ("const bool other_avatar_bake=std::any_of(",
                 "avatar.eyes_bake_id==object_texture_loading_id",
                 "avatar.hair_bake_id==object_texture_loading_id",
                 "if(!current_self_bake&&!other_avatar_bake)"):
   self.assertIn(phrase,a)
 def test_scene_cooldown_after_success_only(self):
  a=src("src/app/application.cpp")
  s=src("include/nexverse/assets/scene_texture_budget_rescue.hpp")
  self.assertIn("scene_texture_mip_rescue.admitted(",a)
  self.assertIn("scene_texture_mip_rescue.minimum_mip(",a)
  self.assertIn("scene_texture_mip_rescue.enter(material_scope.epoch())",a)
  self.assertIn("std::chrono::seconds{90}",s)
  self.assertIn("accepted_.size()>=256U",s)
  self.assertLess(a.index("world_renderer->replace_object_texture("),a.index("if(rescued_scene_mip)"))
 def test_scene_rescue_uses_original_pixels_with_no_new_http(self):
  a=src("src/app/application.cpp")
  self.assertIn("texture_mip_image(",a)
  self.assertIn("result.image,*extra)",a)
  self.assertIn("render::RenderImageRgba8 image;",a)
  self.assertIn("world_renderer->replace_object_texture(",a)
 def test_real_diagnostic_reports_duplicate_counts_without_ids(self):
  raw=('2026-10-31 00:00:00 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:01 [INFO] GetMesh loaded '
       '12345678-1234-1234-1234-123456789abc; requested_lod=high\n'
       '2026-10-31 00:00:02 [INFO] GetMesh loaded '
       '12345678-1234-1234-1234-123456789abc; requested_lod=high\n'
       '2026-10-31 00:00:03 [INFO] Avatar COF bootstrap: skeleton_cof_available=0 '
       'inventory_cap_available=1 fetch_cap_available=0 current_region_lease=1\n')
  report='\n'.join(diag.summarize(raw))
  self.assertIn("1 Wiederholungen",report)
  self.assertIn("skeleton_cof_available=0",report)
  self.assertNotIn("12345678-1234",report)
 def test_unchanged_user_start_commands_still_documented(self):
  doc=src("docs/V0_4_0_0_TEN_BLOCKS_MESH_CACHE_GREY_AVATAR_20261030.md")
  self.assertIn("python3 scripts/ogl-cache-status.py",doc)
  self.assertIn("./build/debug/nexverse-viewer",doc)

if __name__=="__main__":unittest.main()
