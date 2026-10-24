"""A connected LLUDP Circuit alone is not a renderable self avatar pose."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text()
class AuthenticatedSelfPose(unittest.TestCase):
 def test_worldmodel_source_verification_and_region_revocation(self):
  s=rd("src/world/world_model.cpp")
  h=rd("include/nexverse/world/world_model.hpp")
  self.assertIn("bool WorldModel::accept_simulator_self_pose(",s)
  for token in ("avatar_id!=agent_.agent_id",
                "session_id!=agent_.session_id",
                "region_handle!=region_.handle",
                "valid_simulator_avatar_pose(position,rotation)",
                "self_pose_confirmed_=true;"):
   self.assertIn(token,s)
  self.assertEqual(s.count("self_pose_confirmed_=false;"),5)
  self.assertIn("return scene_ready()&&self_pose_confirmed_;",h)
 def test_live_production_handlers_and_render_proxy(self):
  s=rd("src/app/application.cpp")
  self.assertEqual(s.count("world_model.accept_simulator_self_pose("),4)
  self.assertIn("movement->region_handle,movement->position,",s)
  self.assertIn("update->region_handle,",s)
  self.assertIn("terse->region_handle,",s)
  self.assertIn("if (world_model.self_avatar_renderable())",s)
  self.assertIn("avatar_proof.self_present=world_model.self_avatar_renderable();",s)
  self.assertIn("avatar_diagnostic.agent_present=avatar_proof.self_present;",s)
 def test_native_render_epoch_and_pi5(self):
  s=rd("tests/self_avatar_pose_authority_tests.cpp")
  for token in ("!model.scene_ready()","model.self_avatar_renderable()",
                "model.begin_region_transition(", "model.apply_region(current(region2,false))",
                "withdraw_self_object_on_kill(", "model.reset();"):
   self.assertIn(token,s)
  script=rd("scripts/pi5-headless-tests.sh")
  self.assertIn("world_model_tests",script)
  self.assertIn("self_avatar_pose_authority_tests",script)
  self.assertIn("self_object_bake_fallback_tests",script)
  self.assertIn("early_avatar_readback_tests; do",script)
  self.assertIn("nexverse-self-avatar-pose-authority-tests",rd("CMakeLists.txt"))
 def test_version_and_next(self):
  self.assertEqual(rd("VERSION").strip(),"v0.3.0.0")
  self.assertIn("v0.4.0.0",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
