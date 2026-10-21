"""Alpha.335-344: real server-readback diagnostics and latest queue."""
from pathlib import Path
import json
import unittest
R=Path(__file__).resolve().parents[1]
def read(name): return (R/name).read_text()
class LiveWireTests(unittest.TestCase):
 def test_latest_movement_queue(self):
  self.assertIn("queue_.erase(pending)",read("src/protocol/outbound_queue.cpp"))
  self.assertIn("queue_.push_back(std::move(command))",read("src/protocol/outbound_queue.cpp"))
  self.assertIn("outbound_queue.push(std::move(update))",read("src/app/application.cpp"))
 def test_decoder_stage_and_private_report(self):
  app=read("src/app/application.cpp")
  self.assertIn("appearance_failure_name(reason)",app)
  self.assertIn("decode_avatar_appearance(packet.payload,&reason)",app)
  self.assertIn("appearance_malformed_stage_code",read("src/ui/avatar_diagnostic_report.cpp"))
 def test_f3_true_server_movement_status(self):
  self.assertIn("movement_feedback.status()",read("src/app/application.cpp"))
  self.assertIn("v.avatar_movement_status",read("src/ui/viewer_shell_render.cpp"))
  self.assertIn("Serverposition bleibt gleich",read("include/nexverse/protocol/movement_feedback.hpp"))
 def test_current_release(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.734")
  data=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(data["next_batch"],"Alpha.735–744")
  self.assertEqual(data["default_blocks_per_run"],10)
if __name__=="__main__":unittest.main()
