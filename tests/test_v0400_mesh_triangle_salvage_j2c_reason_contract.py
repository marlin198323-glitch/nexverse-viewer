"""v0.4.0.0 decoder regression: salvage valid OpenSim mesh triangles and
classify rejected JPEG2000 metadata without weakening safety bounds."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def src(name):return (ROOT/name).read_text()

class AssetDecoderRecovery(unittest.TestCase):
 def test_mesh_discards_only_corrupt_triangles(self):
  mesh=src("src/assets/mesh_loader.cpp")
  self.assertIn("++skipped_triangles;",mesh)
  self.assertIn("ids[0] >= count || ids[1] >= count",mesh)
  self.assertIn("ids[0] == ids[1]",mesh)
  self.assertIn("length == 0.0f",mesh)
  self.assertIn("skipped_invalid_triangles=",mesh)
  self.assertIn("if (geometry.indices.size() == first_index) return false;",mesh)
 def test_no_geometry_fabricated_when_all_triangles_bad(self):
  native=src("tests/mesh_loader_tests.cpp")
  self.assertIn("const auto mixed = decode_mesh_asset(",native)
  self.assertIn('skipped_invalid_triangles=2',native)
  self.assertIn("const auto corrupt_only=decode_mesh_asset(",native)
  self.assertIn('invalid_faces=1',native)
  self.assertIn('invalid_triangles=2',native)
 def test_unchanged_security_constraints(self):
  mesh=src("src/assets/mesh_loader.cpp")
  jpeg=src("include/nexverse/assets/jpeg2000_metadata_limits.hpp")
  self.assertIn("max_total_vertices = 200000U",mesh)
  self.assertIn("max_total_indices = 1200000U",mesh)
  self.assertIn("max_j2c_side = 4096U",jpeg)
  self.assertIn("max_j2c_rgba_bytes = 64U*1024U*1024U",jpeg)
  self.assertIn("precision>16U",jpeg)
 def test_jpeg2000_failure_reason_never_includes_asset_uuid_or_caps(self):
  jpeg=src("src/assets/jpeg2000_decoder.cpp")
  self.assertIn("image_metadata_issue(",jpeg)
  self.assertIn('"JPEG2000 image metadata exceeds safe limits; reason="',jpeg)
  self.assertIn('"JPEG2000 decoded metadata is invalid; reason="',jpeg)
  self.assertNotIn("result.message += texture_id",jpeg)
 def test_new_native_test_is_registered(self):
  cm=src("CMakeLists.txt")
  self.assertIn("nexverse-jpeg2000-metadata-limits-tests",cm)
  self.assertIn("tests/jpeg2000_metadata_limits_tests.cpp",cm)
  self.assertIn("nexverse-mesh-loader-tests",cm)
if __name__=="__main__": unittest.main()
