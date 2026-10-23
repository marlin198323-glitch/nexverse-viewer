"""The real High4 AgentUpdate must wait for authenticated inworld pose; no synthetic camera origin."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()
class VerifiedHigh4WASD(unittest.TestCase):
 def test_real_worldmodel_control_authority(self):
  header=read("include/nexverse/world/world_model.hpp")
  model=read("src/world/world_model.cpp")
  self.assertIn("bool self_avatar_controls_ready() const noexcept;",header)
  self.assertIn("return self_avatar_renderable()&&",model)
  self.assertIn("valid_simulator_avatar_pose(agent_.position,agent_.rotation)",model)
  self.assertIn("bool WorldModel::accept_simulator_self_pose(",model)
 def test_high4_live_production_gate_before_command_step(self):
  s=read("src/app/application.cpp")
  start=s.index("world::AgentState agent_for_camera;",s.index("const auto now = std::chrono::steady_clock::now();",s.index("simulation_ready = world_model.scene_ready();")))
  end=s.index("last_agent_update = now;",start)
  body=s[start:end]
  for token in ("world_model.self_avatar_controls_ready()",
                "agent_for_camera.agent_id==active_session->agent_id",
                "agent_for_camera.session_id==active_session->session_id",
                "else if(!verified_self_controls)",
                "AgentUpdate paused: no verified simulator self ",
                "movement_feedback.reset();",
                "movement_heading_initialized=false;"):
   self.assertIn(token,body)
  self.assertLess(body.index("else if(!verified_self_controls)"),body.index("movement_controller.step("))
  self.assertLess(body.index("else if(!verified_self_controls)"),body.index("outbound_queue.push("))
 def test_epoch_clears_outbound_and_heading_without_faking_success(self):
  s=read("src/app/application.cpp")
  a=s.index("if(animation_asset_epoch!=current_firestorm_epoch)")
  b=s.index("avatar_appearance_received.store(",a)
  scope=s[a:b]
  for token in ("outbound_queue.clear();","movement_controller.reset();",
                "movement_heading_initialized=false;","local_heading_preview.reset();",
                "last_reported_control_flags=0U;","movement_feedback.reset();",
                "turn_feedback.reset();"):
   self.assertIn(token,scope)
  self.assertIn("WASD: warte auf bestaetigte Simulatorposition",s)
  self.assertIn("!world_model.self_avatar_controls_ready()",s)
 def test_native_proof_pi5_and_release(self):
  native=read("tests/self_agent_high4_pose_gate_tests.cpp")
  for token in ("valid_agent_update_position(world.agent().position)",
                "world.self_avatar_controls_ready()", "wasd.forward=true;",
                "world.accept_simulator_self_pose(", "world.apply_region(region(9001U,true));",
                'world.apply_agent({agent,"different-session"', "control_at_pos"):
   self.assertIn(token,native)
  self.assertIn("nexverse-self-agent-high4-pose-gate-tests",read("CMakeLists.txt"))
  self.assertIn("self_agent_high4_pose_gate_tests",read("scripts/pi5-headless-tests.sh"))
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.964")
  self.assertIn("Alpha.965–974",read("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
