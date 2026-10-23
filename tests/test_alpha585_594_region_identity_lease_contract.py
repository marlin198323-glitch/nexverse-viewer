"""LLUDP circuit drop, direct region switch and re-auth cannot keep old scene."""
from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text(encoding="utf-8")
class RegionLease(unittest.TestCase):
 def test_direct_region_switch_clear_whole_scene(self):
  s=read("src/world/world_model.cpp")
  a=s.index("void WorldModel::apply_region(")
  b=s.index("void WorldModel::begin_region_transition(",a)
  fragment=s[a:b]
  for token in ("region_.handle!=0U && region_.handle!=update.handle",
                "region_.connected && !update.connected",
                "avatars_.clear();","objects_.clear();",
                "++object_revision_;","terrain_={};","parcel_={};",
                "environment_={};","agent_.present=false;",
                "++region_.generation;",
                "self_appearance_.reset();","self_wearables_.reset();",
                "self_object_bakes_.reset();"):
   self.assertIn(token,fragment)
  self.assertIn("if(leave_region)",fragment)
 def test_same_region_handshake_keeps_scene(self):
  s=read("tests/world_direct_region_invalidation_tests.cpp")
  for t in ('same.name="New region title"', 'same.water_height=24.0f',
            'w.object_revision()!=first_revision',
            'w.region().generation!=first_gen',
            'w.objects().size()!=1U||w.avatars().size()!=1U'):
   self.assertIn(t,s)
 def test_actual_circuit_drop_reconnect_and_direct_switch(self):
  s=read("tests/world_direct_region_invalidation_tests.cpp")
  for t in ("w.apply_region(region(r1,false));",
            "w.apply_region(region(r1,true));",
            "w.apply_region(region(r2,true));",
            "w.region().generation!=next_gen+1U",
            "w.object_revision()!=next_revision+1U",
            "world::infer_avatar_attachment_points(",
            "!w.self_appearance()&&!w.self_object_bakes()",
            "!w.terrain().available",
            "!w.parcel().available&&!w.environment().available"):
   self.assertIn(t,s)
 def test_reauthentication_does_not_reuse_old_user_objects(self):
  s=read("src/world/world_model.cpp")
  a=s.index("void WorldModel::apply_agent(")
  b=s.index("void WorldModel::set_agent_profile",a)
  code=s[a:b]
  self.assertIn("const bool replacing_identity=",code)
  self.assertIn("if(replacing_identity)",code)
  for t in ("avatars_.clear();","objects_.clear();",
            "++object_revision_;","++region_.generation;",
            "terrain_={};","parcel_={};","environment_={};"):
   self.assertIn(t,code)
  t=read("tests/world_direct_region_invalidation_tests.cpp")
  self.assertIn('w.apply_agent({"own-agent","second-session"',t)
  self.assertIn('w.apply_agent({"different-agent","second-session"',t)
 def test_real_f7_scene_epoch_ui_and_app(self):
  ui=read("src/ui/viewer_shell_render.cpp")
  self.assertIn('" | Szene-Epoch: "+std::to_string(v.region_generation)',ui)
  h=read("include/nexverse/ui/viewer_shell_render.hpp")
  self.assertIn("std::uint32_t region_generation=0U;",h)
  app=read("src/app/application.cpp")
  self.assertIn("shell_view.region_generation=region.generation;",app)
  self.assertIn("world_model.apply_region(",app)
  self.assertIn("protocol::region_circuit_ready(",app)
 def test_next_release_and_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1044")
  p=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(p["viewer_version"],"0.2.0-alpha.1044")
  self.assertEqual(p["next_batch"],"Alpha.1045–1054")
  self.assertEqual(p["default_blocks_per_run"],10)
  self.assertIn("still open",p["assessment"])
  for doc in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
              "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("10 ECHTE ENTWICKLUNGSBLÖCKE",read(doc)[:350])
   self.assertIn("Alpha.1045–1054",read(doc)[:1700])
if __name__=="__main__":unittest.main()
