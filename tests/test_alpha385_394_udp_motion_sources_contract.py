"""True UDP movement receipts and simulator position readback, no fake movement."""
import unittest,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class TrueMotionReadback(unittest.TestCase):
 def test_socket_success_circuit_callback(self):
  cpp=read("src/protocol/circuit.cpp")
  self.assertIn("if(on_agent_update_sent)",cpp)
  self.assertIn("on_agent_update_sent(value.control_flags)",cpp)
  self.assertIn("sent = !packet.empty() && send_datagram(socket, packet, status)",cpp)
  self.assertIn("AgentUpdateSentCallback",read("include/nexverse/protocol/circuit.hpp"))
 def test_queue_status_not_false_send(self):
  app=read("src/app/application.cpp")
  self.assertIn("movement_feedback.queued_controls(queued_controls)",app)
  self.assertNotIn("movement_feedback.outbound_controls(queued_controls,now)",app)
  self.assertIn("movement_feedback.outbound_controls(",app)
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("wartet auf UDP-Versand",mon)
  self.assertIn("if(queued_signals_seen_&&!queued_direction_)return;",mon)
 def test_multiple_actual_simulator_sources(self):
  app=read("src/app/application.cpp")
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("MovementPositionSource::full_object",app)
  self.assertIn("MovementPositionSource::compressed_object",app)
  self.assertIn("ObjectUpdate",mon)
  self.assertIn("Compressed",mon)
  self.assertIn("MovementPositionSource::improved_terse",mon)
 def test_guard_and_privacy(self):
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("baseline_=latest_",mon)
  self.assertIn("distance_squared>=0.0625f",mon)
  self.assertIn("samples_>=3U",mon)
  self.assertIn("std::scoped_lock lock{mutex_}",mon)
  app=read("src/app/application.cpp")
  self.assertIn("avatar_diagnostic.movement_source_code=",app)
  report=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("movement_udp_socket_sent=",report)
  self.assertIn("movement_server_source_code=",report)
  self.assertNotIn("movement_agent_id=",report)
 def test_real_cpp_cases(self):
  test=read("tests/movement_feedback_status_tests.cpp")
  self.assertIn("MovementPositionSource::full_object",test)
  self.assertIn("MovementPositionSource::compressed_object",test)
  self.assertIn("wartet auf UDP-Versand",test)
 def test_updated_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.514")
  progress=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(progress["next_batch"],"Alpha.515–524")
  self.assertEqual(progress["default_blocks_per_run"],10)
  for f in ["CHATUEBERGABE.md","projectplan.md","AGENTS.md","info.md"]:
   self.assertIn("Alpha.515–524",read(f)[:1100])
if __name__=="__main__": unittest.main()
