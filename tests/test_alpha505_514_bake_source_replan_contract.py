"""Authenticated self body texture changes replan existing scheduler immediately."""
from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class BodySourceReplan(unittest.TestCase):
 def test_epoch_lease_and_authentication(self):
  h=read("include/nexverse/assets/avatar_bake_source_refresh.hpp")
  for t in ("class AvatarBakeSourceRefresh","std::uint64_t epoch",
            "bool authenticated","bool lease_current",
            "if(epoch==0U || !authenticated || !lease_current)",
            "if(epoch_!=epoch)","void clear() noexcept"):
   self.assertIn(t,h)
 def test_dedup_placeholder_and_removal(self):
  h=read("include/nexverse/assets/avatar_bake_source_refresh.hpp")
  for t in ("if(has_source_&&next==source_)return false;",
            "return old_present||","protocol::baked_placeholders",
            "id.clear();"):
   self.assertIn(t,h)
 def test_real_per_frame_scheduler(self):
  app=read("src/app/application.cpp")
  for t in ("avatar_bake_source_refresh.observe(",
            "material_scope.epoch(),self_bake_authenticated,",
            "self_bake_lease,self_bake_ids",
            "if (self_bake_source_changed || scene_objects_changed ||",
            "assets::plan_texture_residency("):
   self.assertIn(t,app)
 def test_existing_gettexture_guardrails(self):
  app=read("src/app/application.cpp")
  for s in ("object_texture_retries","texture_request_pacer.ready(",
            "plan_texture_slot_admission(","texture_gpu_staging_recovery",
            "texture_residency.eviction_ids","max_object_texture_assets"):
   self.assertIn(s,app)
  h=read("include/nexverse/assets/avatar_bake_source_refresh.hpp")
  self.assertNotIn("fetch_rgba(",h)
  self.assertNotIn("send_reliable(",h)
 def test_native_real_residency_priority_evict(self):
  t=read("tests/avatar_bake_source_refresh_tests.cpp")
  for s in ("assets::prioritize_avatar_body_bakes(",
            "assets::plan_texture_residency(",
            'second.desired_ids.contains("old-head")',
            "second.eviction_ids.size()!=3U",
            'third.preferred_ids[2]!="particle"',
            'third.eviction_ids!=std::vector<std::string>{"lower"}'):
   self.assertIn(s,t)
 def test_f3_mixed_provenance_and_private_log(self):
  app=read("src/app/application.cpp")
  for s in ("Obj-TE vorlaeufig (Low158 Teil-Bakes)",
            "Obj-TE vorlaeufig (Self-Low158 fehlt)",
            "self_bake_selection()",
            'core::info("Self Body TE changed; scheduling immediate "'):
   self.assertIn(s,app)
  self.assertNotIn("Self Body TE changed; UUID=",app)
 def test_version_and_ten_blocks(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1064")
  state=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(state["viewer_version"],"0.2.0-alpha.1064")
  self.assertEqual(state["next_batch"],"Alpha.1065–1074")
  self.assertEqual(state["default_blocks_per_run"],10)
  self.assertIn("still open",state["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.1065–1074",read(f)[:1400])
if __name__=="__main__":unittest.main()
