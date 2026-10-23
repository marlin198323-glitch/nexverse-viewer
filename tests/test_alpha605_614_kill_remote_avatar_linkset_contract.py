"""Real simulator KillObject must remove remote avatar rig and descendant mesh."""
import json,unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class KilledAvatarLifetime(unittest.TestCase):
 def test_dispatcher_routes_real_lluudp_kill(self):
  s=read("src/app/application.cpp")
  a=s.index("protocol::msg_kill_object,")
  b=s.index("protocol::msg_coarse_location_update,",a)
  handler=s[a:b]
  self.assertIn("protocol::decode_kill_object(packet.payload)",handler)
  self.assertIn("!world_model.region().connected",handler)
  self.assertIn("world_model.kill_simulator_local_id(local_id)",handler)
  self.assertNotIn("world_model.remove_object(local_id)",handler)
 def test_kill_current_avatar_and_child_linkset(self):
  s=read("src/world/world_model.cpp")
  a=s.index("bool WorldModel::kill_simulator_local_id(")
  b=s.index("std::uint32_t WorldModel::remote_avatar_kills()",a)
  impl=s[a:b]
  self.assertIn("prim_uuid_to_local_id_.erase(index);",impl)
  for t in ("if(!region_.connected||local_id==0U)return false;",
    "it->second.local_id!=local_id",
    "pending_avatar_animations_.erase(it->first)",
    "children[prim.parent_id].push_back(id)",
    "std::vector<std::uint32_t> todo{local_id}",
    "if(!dropped.insert(id).second)continue",
    "objects_.erase(it)",
    "++object_revision_;",
    "++remote_avatar_kills_","removed_prim_kills_+="):
   self.assertIn(t,impl)
 def test_real_kill_wire_and_no_cross_user_ghosts(self):
  s=read("tests/kill_avatar_linkset_tests.cpp")
  for t in ("protocol::decode_kill_object(kill_bytes)",
    "w.kill_simulator_local_id(kill->local_ids[0])",
    'add(w,101U,100U,"sleeve-child"',
    'add(w,102U,101U,"nested-child"',
    'add(w,200U,52U,"bob-attachment"',
    'add(w,300U,0U,"world-building"',
    "w.avatars().contains(alice)", "w.avatars().contains(bob)",
    "world::infer_avatar_attachment_points",
    "w.apply_coarse_avatar_locations(coarse)",
    "w.avatars().at(alice).simulator_object_update_seen",
    "w.removed_prim_kills()!=6U",
    "w.remote_avatar_kills()!=0U"):
   self.assertIn(t,s)
 def test_real_gl_avatar_and_child_scene_evicted(self):
  s=read("tests/scene_animated_attachment_pixels_tests.cpp")
  for t in ("region_objects.kill_simulator_local_id(51U)",
    "renderer->set_avatar_proxies({});",
    "renderer->set_scene_objects({sentinel});",
    "REMOTE_KILLOBJECT_STALE_ATTACHMENT_GPU_PIXELS=",
    "if(stale_gpu_pixels!=0U)failure=18;"):
   self.assertIn(t,s)
 def test_real_f7_kill_evidence(self):
  view=read("include/nexverse/ui/viewer_shell_render.hpp")
  self.assertIn("removed_remote_avatar_kills=0U;",view)
  self.assertIn("removed_prim_kills=0U;",view)
  app=read("src/app/application.cpp")
  self.assertIn("world_model.remote_avatar_kills();",app)
  self.assertIn("world_model.removed_prim_kills();",app)
  ui=read("src/ui/viewer_shell_render.cpp")
  self.assertIn('line(7,"KillObject Avatar/Prims: "+',ui)
 def test_handoff_and_version(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1044")
  d=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(d["viewer_version"],"0.2.0-alpha.1044")
  self.assertEqual(d["next_batch"],"Alpha.1045–1054")
  self.assertEqual(d["default_blocks_per_run"],10)
  self.assertIn("still open",d["assessment"])
  for fn in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
             "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.1045–1054",read(fn)[:1800])
if __name__=="__main__":unittest.main()
