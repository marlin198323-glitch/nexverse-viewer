"""Universal OGL graphics contract: planner and OpenGL backend must share
the same mesh residency ceiling; UUID counts never imply texture fidelity.
No tuning to a specific card or manufacturer."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def code(name):return (R/name).read_text()
class UniversalGraphicsAndHonestTelemetry(unittest.TestCase):
 def test_renderer_uses_planner_single_mesh_ceiling(self):
  p=code("include/nexverse/assets/mesh_residency_target.hpp")
  gl=code("src/render/opengl_renderer.cpp")
  self.assertIn("max_mesh_resident_assets=384U",p)
  self.assertIn("mesh_assets_.size()>=assets::max_mesh_resident_assets",gl)
  self.assertNotIn("mesh_assets_.size() >= 256U",gl)
 def test_economy_and_detailed_profiles_not_4070_specific(self):
  p=code("include/nexverse/assets/mesh_residency_target.hpp")
  self.assertIn("economy_mesh_resident_assets=256U",p)
  self.assertIn("return economy_mesh_resident_assets",p)
  self.assertIn("max_mesh_resident_assets",p)
  self.assertNotIn("4070",p)
  self.assertNotIn("NVX",p)
 def test_independent_vram_byte_limits_remain(self):
  a=code("src/app/application.cpp")
  self.assertIn("mesh_gpu_budget_bytes = 256U*1024U*1024U",a)
  self.assertIn("shared_gpu_budget_bytes = 384U*1024U*1024U",a)
  self.assertIn("effective_gpu_family_budget(",a)
  self.assertIn("can_replace_gpu_bytes(",a)
 def test_native_real_gl_test_loads_384_distinct_meshes(self):
  t=code("tests/renderer_gpu_pressure_tests.cpp")
  self.assertIn("n<nexverse::assets::max_mesh_resident_assets",t)
  self.assertIn('renderer->upload_mesh_asset(\n                    "mesh-over-limit",mesh)',t)
  self.assertIn("GpuUploadFailure::capacity",t)
  self.assertIn('"mesh-reused",mesh',t)
 def test_gpu_driver_allocation_checks_not_disabled(self):
  gl=code("src/render/opengl_renderer.cpp")
  self.assertIn("GpuUploadFailure::allocation",gl)
  self.assertIn("GpuUploadFailure::driver_error",gl)
  self.assertIn("upload_mesh(gpu,mesh,&gpu_error)",gl)
 def test_old_uuid_coverage_retained_but_explicitly_unverified(self):
  s=code("include/nexverse/assets/asset_streaming_snapshot.hpp")
  self.assertIn('field("texture_coverage_pct",textures.percentage())',s)
  self.assertIn('field("texture_visual_quality_verified",0U)',s)
  self.assertIn('field("texture_ids_gpu_residency_pct",textures.percentage())',s)
 def test_mip0_vs_reduced_mip_residency_is_not_same(self):
  s=code("include/nexverse/assets/texture_mip_evidence.hpp")
  self.assertIn("if(it->second==0U)++result.base_mip",s)
  self.assertIn("else ++result.reduced_mip",s)
  self.assertIn("else ++result.unknown_mip",s)
 def test_only_current_priority_textures_counted_once(self):
  s=code("include/nexverse/assets/texture_mip_evidence.hpp")
  self.assertIn("!seen.insert(id).second",s)
  self.assertIn("!resident_ids.contains(id)",s)
 def test_real_runtime_snapshot_connects_gpu_mip_map(self):
  s=code("src/app/application.cpp")
  self.assertIn("snapshot.texture_mips=assets::texture_mip_evidence(",s)
  self.assertIn("object_texture_loaded_ids,texture_gpu_mips);",s)
  self.assertIn("snapshot.renderer_mesh_slot_capacity=",s)
 def test_diagnostic_log_no_longer_claims_complete_textures(self):
  s=code("src/app/application.cpp")
  self.assertIn('" texture_gpu_ids="',s)
  self.assertIn('" texture_quality_unverified=1"',s)
  self.assertNotIn('" textures_loaded=" +',s)
 def test_on_screen_counts_explicitly_gpu_ids(self):
  s=code("include/nexverse/ui/asset_stream_proof.hpp")
  self.assertIn('"Tex-IDs "',s)
  self.assertIn('" GPU | Mesh-IDs "',s)
 def test_real_opengl_shader_draw_counts_distinct_from_uuid_residency(self):
  render=code("src/render/opengl_renderer.cpp")
  self.assertIn("if(use_object_texture)",render)
  self.assertIn("++diagnostics_.scene_textured_draw_calls",render)
  self.assertIn("++diagnostics_.scene_requested_texture_fallback_draw_calls",render)
  test=code("tests/renderer_gpu_pressure_tests.cpp")
  self.assertIn("missing_tex.scene_requested_texture_fallback_draw_calls==0U",test)
  self.assertIn("recovered.scene_requested_texture_fallback_draw_calls!=0U",test)
 def test_actual_visible_draw_counts_present_in_json(self):
  header=code("include/nexverse/assets/asset_streaming_snapshot.hpp")
  app=code("src/app/application.cpp")
  for f in ("previous_frame_scene_textured_draw_calls",
            "previous_frame_scene_requested_texture_fallback_draw_calls"):
   self.assertIn(f,header)
   self.assertIn(f,app)
 def test_native_case_and_snapshot_maintain_compat_schema(self):
  s=code("tests/asset_streaming_snapshot_tests.cpp")
  self.assertIn('texture_reduced_mip_gpu_ids',s)
  self.assertIn('renderer_mesh_slot_capacity',s)
  self.assertIn('texture_coverage_pct',s)
if __name__=="__main__":unittest.main()
