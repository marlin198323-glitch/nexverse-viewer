"""Compact real OpenGL login UI and authenticated shutdown cancellation."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def source(name):return (R/name).read_text()
class CompactNeonAndFastClose(unittest.TestCase):
 def test_header_and_footer_smaller_at_reference_resolution(self):
  h=source("include/nexverse/ui/neon_login_layout.hpp")
  self.assertIn("header_h=72.0f*scale;",h)
  self.assertIn("footer_h=77.0f*scale;",h)
  self.assertIn("field_w=245.0f*scale;",h)
  self.assertIn("210.0f*scale,39.0f*scale",h)
 def test_focus_does_not_draw_cyan_border(self):
  app=source("src/app/application.cpp")
  self.assertNotIn("focused?theme::cyan:",app)
  self.assertIn("Color{0.075f,0.240f,0.355f,0.75f}",app)
  self.assertIn("focused?Color{0.030f,0.078f,0.125f,0.98f}",app)
 def test_welcome_snapshot_fills_more_area_between_bars(self):
  app=source("src/app/application.cpp")
  self.assertIn("renderer.image(welcome_texture,0.0f,l.header_h,w,",app)
  self.assertIn("l.footer_y-l.header_h,1.0f);",app)
 def test_real_field_clicks_share_layout_rectangles(self):
  app=source("src/app/application.cpp")
  for name in ("layout.username.contains","layout.password.contains",
               "layout.enter_world.contains","layout.password_eye.contains"):
   self.assertIn(name,app)
 def test_shutdown_signal_before_gpu_teardown(self):
  app=source("src/app/application.cpp")
  i=app.index("transport->request_stop();")
  self.assertLess(i,app.index("renderer.destroy_texture(brand_texture);",i))
  self.assertLess(i,app.index("circuit_future.wait();",i))
  self.assertIn("Viewer shutdown cleanup elapsed_ms=",app)
 def test_threadsafe_http_cancelled_early_and_during_transfer(self):
  hdr=source("include/nexverse/net/http_transport.hpp")
  src=source("src/net/curl_http_transport.cpp")
  for needle in ("std::atomic<bool> stopping_{false}","void request_stop() noexcept"):
   self.assertIn(needle,hdr)
  for needle in ("CURLOPT_XFERINFOFUNCTION, stop_on_shutdown",
                 "CURLOPT_NOPROGRESS, 0L","CURLOPT_XFERINFODATA",
                 'HTTP cancelled: Viewer is closing',"stopping_"):
   self.assertIn(needle,src)
 def test_real_loopback_cancel_test_registered(self):
  cmake=source("CMakeLists.txt")
  cpp=source("tests/http_viewer_shutdown_tests.cpp")
  self.assertIn("nexverse-http-viewer-shutdown-tests",cmake)
  self.assertIn("transport.request_stop()",cpp)
  self.assertIn("future.wait_for(2500ms)",cpp)
if __name__=="__main__":unittest.main()
