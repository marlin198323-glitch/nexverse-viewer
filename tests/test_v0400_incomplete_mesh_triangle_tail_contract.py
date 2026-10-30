"""v0.4.0.0 P0: salvage complete valid mesh triangles when final
TriangleList has 1-5 truncated bytes; reject all-invalid geometry."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(name):return (ROOT/name).read_text()
class MeshIncompleteTriangleTailContract(unittest.TestCase):
 def test_only_complete_six_byte_triangles_are_read(self):
  code=read("src/assets/mesh_loader.cpp")
  self.assertIn("i + 6U <= tris.size()",code)
  self.assertIn("skipped_trailing_bytes+=tris.size()%6U",code)
  self.assertIn("triangles->binary_value.size() < 6U",code)
 def test_invalid_indices_still_rejected(self):
  code=read("src/assets/mesh_loader.cpp")
  for needle in ("ids[0] >= count", "ids[1] >= count",
                 "ids[2] >= count", "ids[0] == ids[1]",
                 "ids[1] == ids[2]", "ids[0] == ids[2]"):
   self.assertIn(needle,code)
 def test_invalid_area_still_rejected(self):
  code=read("src/assets/mesh_loader.cpp")
  self.assertIn("!std::isfinite(length) || length == 0.0f",code)
  self.assertIn("if (geometry.indices.size() == first_index) return false",code)
 def test_byte_tail_accounted_without_exposing_mesh_id(self):
  code=read("src/assets/mesh_loader.cpp")
  self.assertIn("skipped_incomplete_triangle_bytes=",code)
  self.assertIn("invalid_tail_bytes=",code)
  self.assertNotIn('result.message += std::string{asset_id}',code)
 def test_native_tests_cover_all_short_tails(self):
  test=read("tests/mesh_loader_tests.cpp")
  self.assertIn("for(std::size_t tail=1U;tail<6U;++tail)",test)
  self.assertIn('small_mesh(true,false,false,false,tail)',test)
  self.assertIn('skipped_incomplete_triangle_bytes=',test)
 def test_entirely_incomplete_faces_must_not_succeed(self):
  test=read("tests/mesh_loader_tests.cpp")
  self.assertIn("only_incomplete_triangle",test)
  self.assertIn("small_mesh(true,false,false,false,tail,true)",test)
  self.assertIn("all_incomplete.success",test)
 def test_all_full_triangles_corrupt_with_tail_stays_invalid(self):
  test=read("tests/mesh_loader_tests.cpp")
  self.assertIn("small_mesh(true,false,true,true,5U)",test)
  self.assertIn("only_bad_with_tail.success",test)
  self.assertIn("invalid_triangles=2",test)
 def test_original_mesh_size_and_geometry_budgets_unchanged(self):
  code=read("src/assets/mesh_loader.cpp")
  self.assertIn("max_total_vertices = 200000U",code)
  self.assertIn("max_total_indices = 1200000U",code)
  self.assertIn("max_lod_bytes = 16U * 1024U * 1024U",code)
 def test_real_getmesh_path_uses_same_decoder(self):
  code=read("src/assets/mesh_loader.cpp")
  self.assertIn("auto decoded = decode_mesh_asset(asset_id,response.body,preferred_lod)",code)
  cmake=read("CMakeLists.txt")
  self.assertIn("nexverse-mesh-loader-tests",cmake)
if __name__=="__main__":unittest.main()
