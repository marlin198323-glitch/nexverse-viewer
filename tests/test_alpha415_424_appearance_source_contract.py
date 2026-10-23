"""Real authenticated avatar texture provenance and privacy diagnostics."""
import unittest,json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def text(name):return (R/name).read_text(encoding="utf-8")
class AvatarSourceProvenance(unittest.TestCase):
 def test_no_cross_identity_authentication_leak(self):
  s=text("src/world/world_model.cpp")
  fn=s.split("void WorldModel::apply_agent(",1)[1].split("void WorldModel::set_agent_profile",1)[0]
  for needle in ("agent_.agent_id!=update.agent_id","agent_.session_id!=update.session_id",
                 "self_appearance_.reset()","self_object_bakes_.reset()",
                 "self_animations_.reset()","agent_profile_ = {}"):
   self.assertIn(needle,fn)
 def test_disconnected_or_other_region_cannot_keep_skin(self):
  fn=text("src/world/world_model.cpp").split("void WorldModel::apply_region(",1)[1].split("void WorldModel::begin_region_transition(",1)[0]
  self.assertIn("region_.handle!=update.handle",fn)
  self.assertIn("region_.connected && !update.connected",fn)
  self.assertIn("self_appearance_.reset()",fn)
 def test_object_te_cannot_impersonate_low158(self):
  s=text("include/nexverse/ui/avatar_pipeline_proof.hpp")
  self.assertIn("provisional_object_bakes",s)
  self.assertIn("return appearance_received && current_bake_sources_match() &&",s)
  self.assertIn("provisional_body_gpu_submitted()",s)
  self.assertIn("Low158 fehlt",s)
  app=text("src/app/application.cpp")
  self.assertIn("candidate_bakes.ids[i]=candidate->visible_body[i]",app)
  self.assertIn("avatar_proof.provisional_object_bakes=true",app)
  self.assertIn("protocol::audit_avatar_bake_evidence(",app)
 def test_bake_inspector_has_provisional_states_and_real_depth(self):
  s=text("include/nexverse/ui/avatar_bake_inspector.hpp")
  for needle in ("Obj-TE: ID / GPU fehlt","Obj-TE: ID / GPU geladen",
                 "Obj-TE BakeGL eingereicht","keine sichtbaren Depth-Pixel",
                 "Low158 nicht belegt","source_available"):
   self.assertIn(needle,s)
 def test_numerical_privacy_only(self):
  s=text("src/ui/avatar_diagnostic_report.cpp")
  for k in ("self_object_bake_candidate_gpu_resident",
            "self_object_bake_candidate_bake_gl_draws",
            "self_object_bake_candidate_depth_known",
            "self_object_bake_candidate_depth_visible"):
   self.assertIn(k+"=",s)
  self.assertNotIn("self_object_bake_candidate_asset_id=",s)
  self.assertNotIn("self_object_bake_candidate_agent_id=",s)
 def test_native_evidence_not_just_source_grep(self):
  native=text("tests/avatar_provisional_bake_ui_tests.cpp")
  for needle in ("provisional_body_gpu_submitted()","body_gpu_drawn()",
                 "fragment_samples_passed","Low158 nicht belegt"):
   self.assertIn(needle,native)
  native2=text("tests/self_object_bake_fallback_tests.cpp")
  for needle in ("reauth.apply_agent","reauth.self_appearance()",
                 "disconnected.apply_region","disconnected.self_appearance()"):
   self.assertIn(needle,native2)
 def test_handoff(self):
  self.assertEqual(text("VERSION").strip(),"0.2.0-alpha.1054")
  d=json.loads(text("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(d["viewer_version"],"0.2.0-alpha.1054")
  self.assertEqual(d["next_batch"],"Alpha.1055–1064")
  self.assertEqual(d["default_blocks_per_run"],10)
  self.assertIn("still open",d["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md","info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.1055–1064",text(f)[:1200])
if __name__=="__main__":unittest.main()
