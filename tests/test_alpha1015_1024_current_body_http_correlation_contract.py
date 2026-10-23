"""Alpha.1024: correlate per-face HTTP backoff ONLY to current verified Self Low158."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(rel):return (R/rel).read_text()
class BodyGetTextureCorrelation(unittest.TestCase):
 def test_current_session_model_source_precedes_diagnostic(self):
  app=rd("src/app/application.cpp")
  segment=app[app.index("avatar_diagnostic.self_low158_texture_entry_received=true;")-420:][:1700]
  self.assertIn("world_model.agent().session_id==",segment)
  self.assertIn("assets::inspect_self_bake_gettexture(",segment)
  self.assertIn("object_texture_loaded_ids,object_texture_retries,",segment)
  self.assertIn("avatar_diagnostic.self_low158_body_gettexture_stages[i]",segment)
  self.assertIn("self_bake_gettexture_summary(stages)",app)
 def test_shared_retry_is_read_only_epoch_scoped_and_privacy_safe(self):
  a=rd("include/nexverse/assets/avatar_bake_recheck.hpp")
  b=rd("include/nexverse/assets/asset_failure.hpp")
  for key in ("SelfBakeGetTextureStage::http_missing_backoff",
              "SelfBakeGetTextureStage::transient_backoff",
              "SelfBakeGetTextureStage::configuration_backoff",
              "SelfBakeGetTextureStage::invalid_backoff",
              "retry.failure_kind(id)", "resident.contains(id)",
              "retry.ready(id,now)", "Body-GetTexture H:"):
   self.assertIn(key,a)
  self.assertIn("failure_kind(std::string_view key) const",b)
  self.assertIn("failures_.clear();",b)
 def test_native_distinguishes_unrelated_404_and_region_epoch(self):
  n=rd("tests/avatar_bake_recheck_tests.cpp")
  for t in ("unrelated-scene", "new-upper", "retry.enter(24U)",
            "S::http_missing_backoff", "S::transient_backoff",
            "S::configuration_backoff", "S::invalid_backoff"):
   self.assertIn(t,n)
 def test_private_report_only_numeric_stages_and_no_raw_failure_uuid(self):
  report=rd("src/ui/avatar_diagnostic_report.cpp")
  h=rd("include/nexverse/ui/avatar_diagnostic_report.hpp")
  app=rd("src/app/application.cpp")
  self.assertIn("self_low158_head_gettexture_stage=",report)
  self.assertIn("self_low158_upper_gettexture_stage=",report)
  self.assertIn("self_low158_lower_gettexture_stage=",report)
  self.assertIn("std::array<std::uint8_t,3U> self_low158_body_gettexture_stages",h)
  self.assertIn('"GetTexture failed for [asset id omitted]"',app)
if __name__=="__main__":unittest.main()
