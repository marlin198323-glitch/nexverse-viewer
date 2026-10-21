"""Regression: keyboard-accessible legal consent and no stale approval."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def txt(p):return (R/p).read_text()
class TosRetry(unittest.TestCase):
 def test_keyboard_tab_shift_and_enter(self):
  s=txt("src/app/application.cpp")
  for x in ("GLFW_KEY_TAB","GLFW_MOD_SHIFT","GLFW_KEY_SPACE",
            "GLFW_KEY_KP_ENTER","GLFW_KEY_ESCAPE",
            "next_tos_focus(", "handle_login_tos_action(state,state.tos_focus,window)",
            "render_login_tos_dialog(renderer,window_width,window_height,"):
   self.assertIn(x,s)
 def test_failed_login_revokes_previous_true(self):
  s=txt("src/app/application.cpp")
  i=s.index('if(result.reason=="tos")')
  segment=s[i:s.index('// No raw server URL/password',i)]
  self.assertIn('login_ui.tos.reset();',segment)
  self.assertIn('pending_login_avatar,pending_login_uri',segment)
  self.assertIn('pending_login_avatar.clear();',s)
  self.assertIn('pending_login_uri.clear();',s)
 def test_escape_modal_precedes_global_close(self):
  s=txt("src/app/application.cpp")
  key=s[s.index("void key_callback("):s.index("void mouse_button_callback(")]
  self.assertLess(
   key.index("context->login->tos.visible()&&"),
   key.index("if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)"))
  self.assertIn("nexverse::ui::TosAction::decline,window);",key)
 def test_browser_child_reaped_and_windows_linked(self):
  s=txt("src/app/application.cpp")
  self.assertIn('waitpid(child,&status,0)',s)
  self.assertIn('std::thread([child]',s)
  self.assertIn('Browser-Aufruf gestartet',s)
  self.assertIn('target_link_libraries(nexverse-viewer PRIVATE shell32)',
                txt("CMakeLists.txt"))
 def test_focus_is_visible_and_old_contract_survives(self):
  s=txt("src/app/application.cpp")
  self.assertIn('login.tos_focus=nexverse::ui::TosAction::accept;',s)
  self.assertIn('tos_focus==nexverse::ui::TosAction::decline',s)
  self.assertIn('tos_focus==nexverse::ui::TosAction::open_document',s)
  self.assertIn('tos_focus==nexverse::ui::TosAction::accept',s)
if __name__=="__main__":unittest.main()
