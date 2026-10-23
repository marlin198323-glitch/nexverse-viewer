"""Low158 per-face authority with clearly provisional ObjectUpdate gaps."""
import unittest,json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class MixBakeContract(unittest.TestCase):
 def test_authoritative_faces_not_overwritten(self):
  h=read("include/nexverse/world/world_model.hpp")
  c=read("src/world/world_model.cpp")
  for s in ("SelfRenderBakeSelection","from_low158","from_object_update","self_bake_selection() const"):
   self.assertIn(s,h)
  self.assertIn("if(out.ids[i].empty()&&!self_object_bakes_->visible_body[i].empty())",c)
  self.assertIn("out.from_low158[i]=!out.ids[i].empty();",c)
  self.assertIn("return self_bake_selection().ids;",c)
 def test_reauth_and_localid_invalidation(self):
  s=read("src/world/world_model.cpp")
  for t in ("region_.handle!=region_handle","avatar_id!=agent_.agent_id","agent_.session_id.empty()",
        "local_id==0U","self_object_bakes_->local_id!=local_id","self_object_bakes_.reset();"):
   self.assertIn(t,s)
  t=read("tests/self_object_bake_fallback_tests.cpp")
  for q in ("selected.ids[0]!=\"44444444-4444-4444-4444-444444444444\"",
       "selected.from_object_update[1]","selected.from_low158[0]",
       "w.observe_self_object_bakes(avatar_id,region,54U,{})"):
   self.assertIn(q,t)
 def test_real_renderer_and_whole_model(self):
  a=read("src/app/application.cpp")
  for token in ("baked_ids=world_model.self_render_bakes();",
    "const auto selection=world_model.self_bake_selection();",
    "avatar_proof.object_fallback_faces[i]=true;",
    "avatar_proof.object_fallback_bakes="):
   self.assertIn(token,a)
 def test_appearance_truth_is_never_promoted(self):
  a=read("src/app/application.cpp")
  self.assertIn("protocol::extract_avatar_bakes(packet->texture_entry)",a)
  h=read("include/nexverse/ui/avatar_pipeline_proof.hpp")
  self.assertIn("AvatarBakeEvidence object_fallback_bakes{}",h)
  self.assertIn("Skin: Low158 Teil-Bakes / Obj-TE vorlaeufig",h)
 def test_f3_renders_two_source_provenance(self):
  h=read("include/nexverse/ui/avatar_bake_inspector.hpp")
  for x in ("proof.object_fallback_faces[i]","proof.object_fallback_bakes.regions[i]",
   "Obj-TE vorlaeufig / GPU geladen","Obj-TE vorlaeufig / GPU fehlt",
   "Obj-TE Body provisorisch (kein COF-Beweis)"):
   self.assertIn(x,h)
 def test_no_invented_wear_event(self):
  a=read("src/app/application.cpp")
  self.assertIn("self_bake_owner_is_current(",a)
  self.assertNotIn("automatic_cof_wear_on_fallback",a)
 def test_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1074")
  p=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(p["viewer_version"],"0.2.0-alpha.1074")
  self.assertEqual(p["next_batch"],"Alpha.1075–1084")
  self.assertEqual(p["default_blocks_per_run"],10)
  self.assertIn("still open",p["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md","info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.1075–1084",read(f)[:1600])
if __name__=="__main__":unittest.main()
