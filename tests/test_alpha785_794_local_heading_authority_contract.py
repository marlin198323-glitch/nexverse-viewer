"""Actual WASD must not forge simulator-verified yaw in WorldModel."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def r(name):return (R/name).read_text()
class LocalHeadingAuthority(unittest.TestCase):
 def test_requested_rotation_is_not_model_authority(self):
  s=r('src/app/application.cpp')
  begin=s.index('const auto queued_controls=update.control_flags;')
  end=s.index('last_agent_update = now;',begin)
  actual=s[begin:end]
  self.assertNotIn('world_model.apply_agent(',actual)
  self.assertIn('if(outbound_queue.push(std::move(update)))',actual)
  self.assertIn('local_heading_preview=protocol::LocalHeadingPreview{',actual)
  self.assertLess(actual.index('if(outbound_queue.push('),actual.index('local_heading_preview=protocol::LocalHeadingPreview{'))
  self.assertIn('turn_feedback.observe_server_rotation(',s)
 def test_actual_render_camera_preview(self):
  app=r('src/app/application.cpp')
  self.assertIn('protocol::active_local_heading_preview(',app)
  self.assertIn('render_agent.rotation=*yaw;',app)
  self.assertEqual(app.count('local_heading_preview.reset();'),3)
  header=r('include/nexverse/protocol/local_heading_preview.hpp')
  for key in ('preview->agent_id!=agent_id','preview->session_id!=session_id','age>std::chrono::milliseconds{350}', '!simulator_connected','std::isfinite(n)'):
   self.assertIn(key,header)
 def test_native_regression_and_pi_parity(self):
  s=r('tests/local_heading_preview_tests.cpp')
  for key in ('controls.step(keys,0.2f)','const protocol::Quaternion server{};','now+std::chrono::milliseconds{351}', 'OGL_LOCAL_VISUAL_YAW_NOT_SERVER_AUTHORITY_PASS'):
   self.assertIn(key,s)
  self.assertIn('nexverse-local-heading-preview-tests',r('CMakeLists.txt'))
  self.assertIn('local_heading_preview_tests',r('scripts/pi5-headless-tests.sh'))
 def test_next_version(self):
  self.assertEqual(r('VERSION').strip(),'0.2.0-alpha.864')
  self.assertIn('Alpha.865–874',r('CHATUEBERGABE.md'))
if __name__=='__main__':unittest.main()
