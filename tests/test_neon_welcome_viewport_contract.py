"""Ensure the original live welcome HTML is never vertically distorted by chrome."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path): return (R/path).read_text()
class NexVerseViewportContract(unittest.TestCase):
 def test_native_webkit_snapshot_matches_visible_client_area(self):
  app=read("src/app/application.cpp")
  self.assertIn("initial_login_layout.welcome_capture_height()",app)
  self.assertIn("welcome_layout.welcome_capture_height()",app)
  self.assertIn("welcome_page.initialize(window_width, welcome_initial_height)",app)
  self.assertIn("welcome_page.resize(window_width,welcome_viewport_height)",app)
  self.assertIn("renderer.image(welcome_texture,0.0f,l.header_h,w,",app)
  self.assertIn("static_cast<float>(l.welcome_capture_height()),1.0f);",app)
 def test_original_landing_page_has_no_dark_scrim(self):
  app=read("src/app/application.cpp")
  section=app[app.index("// Real neon login composition:"):app.index("    return;\n\n    renderer.rect(",app.index("// Real neon login composition:"))]
  self.assertNotIn("Color{0.004f,0.008f,0.022f,0.07f}",section)
  self.assertIn("no additional black/purple scrim",section)
 def test_resize_waits_until_gtk_allocation(self):
  code=read("src/ui/welcome_page.cpp")
  self.assertIn("gtk_window_resize(GTK_WINDOW(impl_->window), new_width, new_height)",code)
  self.assertIn("webkit_web_view_set_zoom_level(",code)
  self.assertIn("welcome_zoom(new_width,new_height)",code)
  section=code[code.index("void WelcomePage::resize(int width, int height)"):code.index("void WelcomePage::pump()")]
  self.assertNotIn("impl_->request_snapshot();",section)
 def test_ultrawide_uses_available_width_without_tiny_height_scaling(self):
  layout=read("include/nexverse/ui/neon_login_layout.hpp")
  self.assertIn("height/820.0f",layout)
  self.assertIn("width*0.26f",layout)
  self.assertIn("trailing_toggle",layout)
if __name__=="__main__": unittest.main()
