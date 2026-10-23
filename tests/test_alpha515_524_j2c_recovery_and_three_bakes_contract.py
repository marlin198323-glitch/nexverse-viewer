"""Exact malformed J2C tile length: one bounded reread; three original baked skins."""
from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class SkinPixelRescue(unittest.TestCase):
 def test_exact_j2c_retry_not_permanent_loop(self):
  s=read("src/assets/texture_loader.cpp")
  for x in ("Tile part length size inconsistent with stream length",
            "const bool truncated_tile =","if(truncated_tile)",
            "const auto fresh=transport_->get(","if(retry.success)",
            "retry.recovered_j2c_transfer=true;",
            "j2c_failure=merge_asset_failure(j2c_failure,AssetFailure::invalid);"):
   self.assertIn(x,s)
  self.assertEqual(s.count("const auto fresh=transport_->get("),1)
 def test_mock_one_rescue_one_permanent_one_non_tile(self):
  s=read("tests/texture_loader_tests.cpp")
  for x in ("first_j2c_truncated_then_valid",
     "always_j2c_truncated","j2c_non_tile_corruption",
     "recovered.recovered_j2c_transfer",
     "unrecoverable.recovered_j2c_transfer",
     "invalid_format.recovered_j2c_transfer"):
   self.assertIn(x,s)
  self.assertIn("transport->j2c_requests!=2U",s)
  self.assertIn("transport->j2c_requests!=1U",s)
 def test_caps_limits_preserved(self):
  s=read("src/assets/texture_loader.cpp")
  self.assertIn("max_texture_download_bytes",s)
  self.assertIn("connect_timeout,total_timeout",s)
  self.assertNotIn("while(truncated_tile)",s)
  self.assertIn("PNG request failed",s)
 def test_three_distinct_original_llm_gpu_skin_draws(self):
  s=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ("head-bake-live","upper-bake-live","lower-bake-live",
   "THREE_INDEPENDENT_FIRESTORM_BAKE_PIXELS",
   "self.baked_texture_ids=bake_ids",
   "upper-replacement",
   "renderer->remove_object_texture(\"head-bake-live\")",
   "std::array<std::uint64_t,3>{0U,1U,1U}"):
   self.assertIn(x,s)
 def test_f3_recovered_count_only_after_uploaded(self):
  s=read("src/app/application.cpp")
  for x in ("texture_recovered_j2c_transfers=0U;",
    "if(result.recovered_j2c_transfer)",
    "++texture_recovered_j2c_transfers;",
    "object_texture_loaded_ids.insert(","J2C-Retry gerettet:"):
   self.assertIn(x,s)
  self.assertIn("bool recovered_j2c_transfer=false;",
    read("include/nexverse/assets/texture_loader.hpp"))
 def test_no_new_bake_mutation(self):
  s=read("src/app/application.cpp")
  self.assertIn("texture_request_pacer.ready(",s)
  self.assertIn("plan_texture_slot_admission(",s)
  self.assertIn("object_texture_retries",s)
 def test_version_ten_blocks(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.914")
  j=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(j["viewer_version"],"0.2.0-alpha.914")
  self.assertEqual(j["next_batch"],"Alpha.915–924")
  self.assertEqual(j["default_blocks_per_run"],10)
  self.assertIn("still open",j["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.915–924",read(f)[:1600])
if __name__=="__main__":unittest.main()
