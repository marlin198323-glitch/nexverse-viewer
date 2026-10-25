"""Compact real OpenGL login UI and authenticated shutdown cancellation."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def source(name):return (R/name).read_text()
class CompactNeonAndFastClose(unittest.TestCase):
 def test_header_and_footer_smaller_at_reference_resolution(self):
  h=source("include/nexverse/ui/neon_login_layout.hpp")
  self.assertIn("header_h=58.0f*scale;",h)
  self.assertIn("footer_h=60.0f*scale;",h)
  self.assertIn("field_w=205.0f*scale;",h)
  self.assertIn("180.0f*scale,32.0f*scale",h)
 def test_focus_does_not_draw_cyan_border(self):
  app=source("src/app/application.cpp")
  self.assertNotIn("focused?theme::cyan:",app)
  self.assertIn("// Identical neutral outline for both focused/unfocused states.",app)
  self.assertIn("focused?Color{0.036f,0.074f,0.111f,0.98f}",app)
 def test_welcome_snapshot_fills_more_area_between_bars(self):
  app=source("src/app/application.cpp")
  self.assertIn("renderer.image(welcome_texture,0.0f,l.header_h,w,",app)
  self.assertIn("static_cast<float>(l.welcome_capture_height()),1.0f);",app)
 def test_real_field_clicks_share_layout_rectangles(self):
  app=source("src/app/application.cpp")
  for name in ("layout.username.contains","layout.password.contains",
               "layout.enter_world.contains","layout.password_eye.contains"):
   self.assertIn(name,app)
 def test_shutdown_signal_before_gpu_teardown(self):
  app=source("src/app/application.cpp")
  i=app.index("transport->request_stop();")
  self.assertLess(i,app.index("renderer.destroy_texture(brand_texture);",i))
  self.assertLess(i,app.index('shutdown_wait(circuit_future,"LLUDP circuit");',i))
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
 def test_all_background_work_is_joined_and_shutdown_is_timed(self):
  app=source("src/app/application.cpp")
  for worker in ("LLUDP circuit","CAPS EventQueue","region EventQueue",
                 "login","outfit images","mesh asset","texture prefetch"):
   self.assertIn('"'+worker+'"',app)
  self.assertIn("Viewer shutdown waiting on ",app)
  self.assertIn("Viewer shutdown total elapsed_ms=",app)
  self.assertIn("!circuit_stop.load(); ++tick",app)
 def test_real_loopback_cancel_test_registered(self):
  cmake=source("CMakeLists.txt")
  cpp=source("tests/http_viewer_shutdown_tests.cpp")
  self.assertIn("nexverse-http-viewer-shutdown-tests",cmake)
  self.assertIn("transport.request_stop()",cpp)
  self.assertIn("future.wait_for(2500ms)",cpp)
if __name__=="__main__":unittest.main()
