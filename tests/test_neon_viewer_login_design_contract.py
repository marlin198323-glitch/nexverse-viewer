"""The reference neon NexVerse design must be an actual usable Viewer login."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(p):return (R/p).read_text()
class NeonLoginDesignContract(unittest.TestCase):
 def test_real_opengl_login_has_shared_responsive_layout(self):
  a=src("src/app/application.cpp")
  self.assertIn('#include "nexverse/ui/neon_login_layout.hpp"',a)
  self.assertGreaterEqual(a.count("nexverse::ui::NeonLoginLayout"),2)
  self.assertIn("const nexverse::ui::NeonLoginLayout l{width,height};",a)
 def test_original_live_nexverse_welcome_retained_not_fake_image(self):
  a=src("src/app/application.cpp")
  self.assertIn("renderer.image(welcome_texture,0.0f,72.0f*sc,w,h*0.82f,1.0f);",a)
  self.assertIn("Color{0.004f,0.008f,0.022f,0.07f}",a)
  self.assertIn("https://stadt-nexverse.de/service/welcome",src("include/nexverse/ui/welcome_page.hpp"))
 def test_centered_dark_modal_removed_from_actual_login_draw_path(self):
  a=src("src/app/application.cpp")
  s=a[a.index("// Real neon login composition:"):a.index("    return;\n\n    renderer.rect(",a.index("// Real neon login composition:"))]
  self.assertNotIn("panel_x",s)
  self.assertNotIn("panel_w",s)
  self.assertIn("l.footer",s)
  self.assertIn("l.enter_world",s)
 def test_header_grid_and_live_connection_pills(self):
  a=src("src/app/application.cpp")
  self.assertIn('status_chip_neon(l.grid_status,"GRID",grid_status,grid_status_color)',a)
  self.assertIn('status_chip_neon(l.login_status,"LOGIN",login.status,login_color)',a)
  self.assertIn('status_chip_neon(l.world_status,"WORLD","OFFLINE",theme::text_muted)',a)
  self.assertIn("VIEWER FUER NEXVERSE",a)
 def test_footer_fields_submit_and_visual_proportions_are_real(self):
  a=src("src/app/application.cpp")
  h=src("include/nexverse/ui/neon_login_layout.hpp")
  for key in ("l.avatar_icon","l.username","l.password",
      "l.enter_world","l.remember_avatar", "WELT BETRETEN"):
   self.assertIn(key,a)
  for key in ("width/1648.0f","height/928.0f","259.0f*scale",
      "298.0f*scale","footer_y=height-footer_h"):
   self.assertIn(key,h)
 def test_actual_mouse_hit_matches_rendered_geometries(self):
  a=src("src/app/application.cpp")
  section=a[a.index("const nexverse::ui::NeonLoginLayout layout{width,height};"):a.index("// Shared with native login regression tests:")]
  for k in ("layout.username.contains","layout.password.contains",
      "layout.password_eye.contains","layout.enter_world.contains",
      "state.submit_requested=true"):
   self.assertIn(k,section)
 def test_eye_button_is_actual_local_password_visibility_action(self):
  a=src("src/app/application.cpp")
  self.assertIn("state.show_password=!state.show_password;",a)
  self.assertIn("login.show_password?login.password:",a)
  self.assertIn("bool show_password = false;",a)
  self.assertNotIn("login.password = state.show_password",a)
 def test_existing_authenticated_login_is_not_replaced_by_mock(self):
  a=src("src/app/application.cpp")
  for key in ("credentials.password = std::move(login_ui.password)",
      "login_ui.submit_requested = false;",
      "if (login_ui.username.empty() || login_ui.password.empty())",
      "login_ui.authenticated = true;"):
   self.assertIn(key,a)
 def test_resizable_decorated_full_monitor_start_not_exclusive(self):
  a=src("src/app/application.cpp")
  for x in ("glfwWindowHint(GLFW_DECORATED, GLFW_TRUE)",
    "glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE)",
    "glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE)",
    "const int initial_width=primary_mode?primary_mode->width:1440",
    "nullptr,\n        nullptr"):
   self.assertIn(x,a)
 def test_native_multiple_resolution_mouse_layout_ctest(self):
  c=src("CMakeLists.txt")
  t=src("tests/neon_login_layout_tests.cpp")
  self.assertIn("nexverse-neon-login-layout-tests",c)
  for resolution in ("800,600","1100,700","1280,720","1648,928",
                     "1920,1080","3440,1440"):
   self.assertIn(resolution,t)
 def test_no_fake_remember_me_or_placeholder_html_image(self):
  a=src("src/app/application.cpp")
  self.assertIn("not the mockup's",a)
  self.assertNotIn("state.remember_password=true",a)
if __name__=="__main__":unittest.main()
