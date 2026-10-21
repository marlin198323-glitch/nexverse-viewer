"""Coarse map dots are not proof of current simulator 3D avatar objects."""
from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text(encoding="utf-8")
class CoarseAvatarAdmission(unittest.TestCase):
 def test_true_sim_object_pose_marks_avatar_not_coarse(self):
  model=read("include/nexverse/world/world_model.hpp")
  for x in ("bool simulator_object_update_seen=false;",
            "bool coarse_position_only=false;",
            "bool from_avatar_object_update=false",
            "apply_coarse_avatar_locations("):
   self.assertIn(x,model)
  src=read("src/app/application.cpp")
  self.assertEqual(src.count("entry.id,entry.position,entry.rotation,entry.local_id,\n                        true);"),2)
  self.assertIn("!avatar.simulator_object_update_seen ||",src)
 def test_coarse_snapshots_not_clear_real_appearance(self):
  src=read("src/world/world_model.cpp")
  a=src.index("bool WorldModel::apply_coarse_avatar_locations(")
  b=src.index("void WorldModel::remove_avatar",a)
  c=src[a:b]
  for x in ("if(!region_.connected||agent_.agent_id.empty())",
            "id==agent_.agent_id",
            'id=="00000000-0000-0000-0000-000000000000"',
            "if(seen.size()>=128U)break;",
            "if(!seen.insert(id).second)continue;",
            "existing->second.simulator_object_update_seen",
            "update_avatar_transform(id,entry.position,rotation,0U);",
            "it->second.coarse_position_only=true;",
            "!av.simulator_object_update_seen&&av.local_id==0U&&",
            "av.texture_entry.empty()&&av.visual_params.empty()&&",
            "av.appearance_data.empty()&&av.attachments.empty()"):
   self.assertIn(x,c)
  self.assertNotIn("avatar.texture_entry.clear()",c)
 def test_dispatcher_uses_one_secure_worldmodel_path(self):
  src=read("src/app/application.cpp")
  a=src.index("protocol::msg_coarse_location_update,")
  b=src.index("// Simulator AvatarAnimation High20",a)
  c=src[a:b]
  self.assertIn("world_model.apply_coarse_avatar_locations(*coarse)",c)
  self.assertNotIn("world_model.remove_avatar(id)",c)
  self.assertNotIn("world_model.update_avatar_transform(",c)
 def test_native_wire_map_to_world_and_failure_handling(self):
  t=read("tests/coarse_avatar_render_admission_tests.cpp")
  for x in ("protocol::decode_coarse_location_update(payload)",
            "protocol::uuid_to_bytes(b)",
            "w.apply_coarse_avatar_locations(*decoded)",
            "decoded->avatars[0].position.z!=20.0f",
            "w.update_avatar_transform(a,{36.75f,40.25f,15.5f}",
            "w.avatars().at(a).simulator_object_update_seen",
            "w.avatars().at(a).appearance_data.size()!=1U",
            "w.avatars().at(a).position.x!=36.75f",
            "w.apply_region(region(0x0000010000000200ULL,false))",
            "payload.pop_back()"):
   self.assertIn(x,t)
 def test_f7_ui_evidence_and_gl_regression(self):
  app=read("src/app/application.cpp")
  self.assertIn("shell_view.remote_avatar_object_updates;",app)
  self.assertIn("shell_view.remote_avatar_coarse_only;",app)
  h=read("include/nexverse/ui/viewer_shell_render.hpp")
  self.assertIn("remote_avatar_object_updates=0U;",h)
  self.assertIn("remote_avatar_coarse_only=0U;",h)
  ui=read("src/ui/viewer_shell_render.cpp")
  self.assertIn('" (ObjUpdate:"+std::to_string(',ui)
  self.assertIn('" / Karte:"+std::to_string(',ui)
  gl=read("tests/viewer_shell_gl_tests.cpp")
  self.assertIn("view.remote_avatar_object_updates=1U;",gl)
  self.assertIn("view.remote_avatar_coarse_only=1U;",gl)
 def test_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.654")
  j=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(j["viewer_version"],"0.2.0-alpha.654")
  self.assertEqual(j["next_batch"],"Alpha.655–664")
  self.assertEqual(j["default_blocks_per_run"],10)
  self.assertIn("still open",j["assessment"])
  for p in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.655–664",read(p)[:1800])
if __name__=="__main__":unittest.main()
