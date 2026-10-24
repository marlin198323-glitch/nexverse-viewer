"""Alpha1194: real fly button in the viewer Movement window; no fake simulator success."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(x):return (R/x).read_text()
class FlightButton(unittest.TestCase):
 def test_viewer_action_is_real_not_disabled_and_window_existing(self):
  s=rd("include/nexverse/ui/viewer_shell.hpp")
  self.assertIn("toggle_fly, disabled",s)
  self.assertIn("ViewerWindow::move",s)
 def test_mouse_hit_targets_exactly_the_rendered_flight_button(self):
  s=rd("include/nexverse/ui/viewer_shell.hpp")
  r=rd("src/ui/viewer_shell_render.cpp")
  for x in ("panel_x+27.0f*scale",
            "panel_y+panel_h-97.0f*scale",
            "204.0f*scale,42.0f*scale",
            "state.fly_available?ViewerAction::toggle_fly"):
   self.assertIn(x,s)
  for x in ("x+27.0f*sc,y+ph-97.0f*sc",
     "204.0f*sc,42.0f*sc",
     "Flugmodus anfordern", "Flugmodus beenden"):
   self.assertIn(x,r)
 def test_disabled_unless_server_self_pose_verified(self):
  s=rd("src/app/application.cpp")
  self.assertIn("viewer_shell.fly_available=verified_self_controls&&",s)
  self.assertIn("protocol::valid_agent_update_position(",s)
  self.assertIn("agent_for_camera.agent_id==active_session->agent_id",s)
  self.assertIn("agent_for_camera.session_id==active_session->session_id",s)
  self.assertIn("if(!viewer_shell.fly_available)",s)
 def test_actual_mouse_handler_requests_real_fly_command_once(self):
  s=rd("src/app/application.cpp")
  for x in ("if(command==nexverse::ui::ViewerAction::toggle_fly)",
      "shell.window==nexverse::ui::ViewerWindow::move &&",
      "shell.fly_toggle_requested=true;",
      "pressed(GLFW_KEY_F)||viewer_shell.fly_toggle_requested;"):
   self.assertIn(x,s)
 def test_only_validated_outbound_queue_acceptance_consumes_ui_request(self):
  s=rd("src/app/application.cpp")
  a=s.index("if(outbound_queue.push(std::move(update)))")
  b=s.index("last_agent_update = now;",a)
  sub=s[a:b]
  self.assertIn("viewer_shell.fly_toggle_requested=false;",sub)
  self.assertIn("movement_controller.flying();",sub)
  self.assertIn("movement_controller=controller_before_queue;",sub)
 def test_flight_toggle_uses_real_existing_agentupdate_wire_flags(self):
  s=rd("include/nexverse/protocol/movement_controls.hpp")
  self.assertIn("keys.toggle_fly && !previous_fly_key_",s)
  self.assertIn("(fly_ ? control_fly : 0U)",s)
  self.assertIn("previous_fly_key_ = keys.toggle_fly;",s)
 def test_visual_label_says_only_local_not_fake_server_ack(self):
  r=rd("src/ui/viewer_shell_render.cpp")
  self.assertIn("Server-Flight nur nach Readback bestaetigt",r)
  self.assertNotIn("Serverflug erfolgreich",r)
 def test_reset_ui_on_disconnect_and_invalid_pose(self):
  s=rd("src/app/application.cpp")
  for x in ("viewer_shell.fly_toggle_requested=false;",
     "viewer_shell.fly_local_requested=false;",
     "viewer_shell.fly_available=false;"):
   self.assertIn(x,s)
 def test_native_layout_and_real_opengl_regression(self):
  s=rd("tests/viewer_shell_tests.cpp")
  r=rd("tests/viewer_shell_gl_tests.cpp")
  for x in ("ViewerAction::toggle_fly", "s.fly_available=true",
            "s.movement_input_captured()"):
   self.assertIn(x,s)
  for x in ("state.fly_available=true;", "state.fly_local_requested=true",
            "glGetError()!=GL_NO_ERROR"):
   self.assertIn(x,r)
 def test_real_live_flight_readback_and_complete_viewer_still_open(self):
  p=rd("docs/PROJECT_PROGRESS.json")
  self.assertIn("Alpha1194",p)
  self.assertIn("BLOCKED",p)
  self.assertIn("still open",p)
if __name__=="__main__":unittest.main()
