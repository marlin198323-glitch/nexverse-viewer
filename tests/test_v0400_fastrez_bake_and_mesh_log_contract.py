"""v0.4.0.0: FastRez Low158 bake coverage and bounded successful-mesh logging."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(name):return (ROOT/name).read_text()
class AvatarMeshDiagnostics(unittest.TestCase):
 def test_authenticated_early_low158_has_bake_provenance(self):
  s=read("src/app/application.cpp")
  start=s.index("const auto recovered=\n                    world_model.flush_early_self_readbacks();")
  section=s[start:start+2750]
  for text in ("if(recovered.appearance)","world_model.self_appearance()",
               "protocol::extract_avatar_bakes(",
               "world_model.self_bake_selection()",
               "body_bakes_low158=","body_bakes_candidates=",
               "provisional_object_faces=", "GPU textures/skin/COF not verified",
               "asset IDs omitted"):
   self.assertIn(text,section)
 def test_no_fake_bakes_from_valid_early_appearance(self):
  native=read("tests/early_avatar_readback_tests.cpp")
  self.assertIn("const auto empty_body=model.self_bake_selection();",native)
  self.assertIn("std::any_of(empty_body.ids.begin()",native)
  self.assertIn("return 39;",native)
 def test_success_upload_log_is_aggregate_only(self):
  cpp=read("src/render/opengl_renderer.cpp")
  self.assertIn("mesh_upload_log_checkpoint(",cpp)
  self.assertIn("OGLRender mesh GPU uploads: count=",cpp)
  self.assertIn("triangles_total=",cpp)
  self.assertIn("mesh_upload_triangle_sum_",cpp)
  self.assertNotIn("OGLRender mesh geometry uploaded: triangles=",cpp)
 def test_checkpoint_is_bounded_in_native_test(self):
  test=read("tests/mesh_upload_log_checkpoint_tests.cpp")
  header=read("include/nexverse/render/mesh_upload_log_checkpoint.hpp")
  self.assertIn("successful_uploads%128U==0U",header)
  self.assertIn("upload<=102762U",test)
  self.assertIn("emits!=803U",test)
 def test_build_and_error_logs_remain_available(self):
  cm=read("CMakeLists.txt")
  src=read("src/render/opengl_renderer.cpp")
  self.assertIn("nexverse-mesh-upload-log-checkpoint-tests",cm)
  self.assertIn("OGLRender mesh GPU upload failed",src)
  self.assertIn("GpuUploadFailure::driver_error",src)
if __name__=="__main__":unittest.main()
