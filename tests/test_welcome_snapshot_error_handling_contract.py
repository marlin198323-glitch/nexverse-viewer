"""Regression contracts for genuine WebKit snapshot error handling and refresh."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(path):return (R/path).read_text()
class WelcomeFailurePipeline(unittest.TestCase):
 def test_revision_rejects_old_resize_callback(self):
  policy=src("include/nexverse/ui/welcome_snapshot_policy.hpp")
  self.assertIn("token!=revision_",policy)
  self.assertIn("return !pending_ && failures_<3U",policy)
  self.assertIn("std::chrono::seconds{30}",policy)
  self.assertIn("std::chrono::milliseconds{250*failures_}",policy)
  self.assertIn("snapshot_policy.invalidate();",src("src/ui/welcome_page.cpp"))
 def test_async_snapshot_does_not_dereference_dead_viewer(self):
  code=src("src/ui/welcome_page.cpp")
  self.assertIn("impl_->lifetime->owner=nullptr;",code)
  self.assertIn("std::unique_ptr<SnapshotRequest> request",code)
  self.assertIn("g_signal_handlers_disconnect_by_data",code)
  self.assertIn("self==nullptr",code)
  self.assertIn("self->snapshot_policy.is_current(request->revision)",code)
  self.assertIn("request->width",code)
  self.assertIn("request->height",code)
 def test_same_resolution_updates_live_gl_texture(self):
  app=src("src/app/application.cpp")
  frame=src("include/nexverse/ui/welcome_page.hpp")
  self.assertIn("std::uint64_t generation = 0U;",frame)
  self.assertIn("++self->frame.generation;",src("src/ui/welcome_page.cpp"))
  self.assertIn("welcome_texture_generation!=welcome_frame.generation",app)
  self.assertIn("welcome_texture_generation=welcome_frame.generation",app)
  self.assertIn("renderer.update_rgba_texture(",app)
 def test_native_async_tests_registered(self):
  cmake=src("CMakeLists.txt")
  case=src("tests/welcome_snapshot_policy_tests.cpp")
  self.assertIn("nexverse-welcome-snapshot-policy-tests",cmake)
  self.assertIn("s.invalidate();",case)
  self.assertIn("t+31s",case)
if __name__=="__main__":unittest.main()
