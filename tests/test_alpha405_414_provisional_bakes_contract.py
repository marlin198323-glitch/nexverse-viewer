"""Only real current-region self ObjectUpdate bakes can provisionally render."""
import unittest, json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def src(name):return (R/name).read_text(encoding="utf-8")
class ProvisionalBakes(unittest.TestCase):
 def test_exact_self_and_region(self):
  cpp=src("src/world/world_model.cpp")
  for s in ("agent_.session_id.empty()","region_.handle!=region_handle",
            "avatar_id!=agent_.agent_id","local_id==0U",
            "protocol::extract_avatar_bakes(texture_entry).visible_body()"):
   self.assertIn(s,cpp)
  self.assertIn("self_object_bakes_->local_id!=local_id",cpp)
  self.assertIn("self_object_bakes_->visible_body==bakes",cpp)
 def test_both_real_object_packet_families(self):
  app=src("src/app/application.cpp")
  self.assertEqual(app.count("world_model.observe_self_object_bakes("),2)
  self.assertIn("entry.id,update->region_handle",app)
  self.assertIn("entry.local_id,entry.texture_entry",app)
  self.assertIn("msg_object_update_compressed",app)
  self.assertIn("msg_object_update,",app)
 def test_verified_low158_wins_and_no_ghost(self):
  model=src("src/world/world_model.cpp")
  self.assertIn("if(self_appearance_)",model)
  # Since Alpha.504, Low158 retains precedence PER FACE; valid self
  # ObjectUpdate may fill only missing slots without claiming COF truth.
  self.assertIn("if(out.ids[i].empty()&&!self_object_bakes_->visible_body[i].empty())",model)
  self.assertIn("out.from_low158[i]=!out.ids[i].empty();",model)
  self.assertIn("return self_bake_selection().ids;",model)
  app=src("src/app/application.cpp")
  self.assertIn("baked_ids=world_model.self_render_bakes();",app)
  self.assertIn("protocol::self_bake_owner_is_current(",app)
  self.assertNotIn("apply_self_appearance(",src("src/world/world_model.cpp").split("bool WorldModel::observe_self_object_bakes(",1)[1].split("bool WorldModel::apply_avatar_animation(",1)[0])
 def test_candidate_visual_provenance(self):
  app=src("src/app/application.cpp")
  self.assertIn("Obj-TE vorlaeufig (Self-Low158 fehlt)",app)
  self.assertIn("avatar_diagnostic.self_object_bake_candidate_faces",app)
  self.assertIn("avatar_diagnostic.self_object_bake_candidate_active",app)
  report=src("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("self_object_bake_candidate_faces=",report)
  self.assertIn("self_object_bake_candidate_active=",report)
  self.assertNotIn("self_object_bake_candidate_uuid=",report)
 def test_native_fixture(self):
  case=src("tests/self_object_bake_fallback_tests.cpp")
  for s in ("world::WorldModel","protocol::extract_avatar_bakes",
            "tex(true,true)","observe_self_object_bakes","self_render_bakes",
            "begin_region_transition","w.apply_self_appearance(authoritative)",
            "w.snapshot().self_object_bakes"):
   self.assertIn(s,case)
  self.assertIn("nexverse-self-object-bake-fallback-tests",src("CMakeLists.txt"))
 def test_documented_release(self):
  self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.874")
  data=json.loads(src("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(data["next_batch"],"Alpha.875–884")
  self.assertEqual(data["viewer_version"],"0.2.0-alpha.874")
  self.assertEqual(data["default_blocks_per_run"],10)
  self.assertIn("still open",data["assessment"])
  for f in ("CHATUEBERGABE.md","AGENTS.md","projectplan.md","info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.875–884",src(f)[:1400])
if __name__=="__main__":unittest.main()
