"""Real WASD: only queued commands affect local moving state; idle yaw uses server."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(name): return (R/name).read_text()
class HeadQueueContract(unittest.TestCase):
 def test_idle_rebases_only_from_real_server_heading(self):
  app=read('src/app/application.cpp')
  begin=app.index('const bool turn_requested=',app.index('AgentUpdate paused: simulator position invalid'))
  end=app.index('last_agent_update = now;',begin)
  part=app[begin:end]
  self.assertIn('!protocol::active_local_heading_preview(',part)
  self.assertIn('movement_controller.adopt_server_heading(',part)
  self.assertIn('agent_for_camera.rotation);',part)
  self.assertLess(part.index('movement_controller.adopt_server_heading('),part.index('movement_controller.step('))
  self.assertNotIn('world_model.apply_agent(',part)
 def test_full_queue_does_not_advance_yaw_fly_or_movement_ui(self):
  app=read('src/app/application.cpp')
  start=app.index('const auto controller_before_queue=movement_controller;')
  end=app.index('last_agent_update = now;',start)
  s=app[start:end]
  queued=s.index('if(outbound_queue.push(std::move(update)))')
  flags=s.index('last_reported_control_flags=queued_controls;')
  confirm=s.index('movement_feedback.queued_controls(queued_controls);')
  rollback=s.index('movement_controller=controller_before_queue;')
  self.assertLess(queued,flags)
  self.assertLess(queued,confirm)
  self.assertLess(confirm,rollback)
  self.assertIn('local_heading_preview=protocol::LocalHeadingPreview{',s)
  self.assertIn('AgentUpdate not queued: queue full',s)
 def test_cpp_real_lowlevel_wire_and_fly_recovery(self):
  cpp=read('tests/movement_controls_tests.cpp')
  for t in ('c.adopt_server_heading({0.0f,0.0f,0.70710678f,0.70710678f});',
            'encode_agent_update(59U,session,server_aligned)',
            'wire_bytes[40]','control_at_pos|control_fly'):
   self.assertIn(t,cpp)
  q=read('tests/outbound_queue_tests.cpp')
  for t in ('controller=old_controller;', 'fly_controller=before_flight;',
            'blocked.push(rejected)','flight_blocked.push(blocked_flight)',
            'accepted_flight.control_flags&control_fly'):
   self.assertIn(t,q)
  header=read('include/nexverse/protocol/movement_controls.hpp')
  self.assertIn('void adopt_server_heading(',header)
  self.assertIn('yaw_ = valid_initial_agent_yaw(heading);',header)
 def test_version_and_next_step(self):
  self.assertEqual(read('VERSION').strip(),'0.2.0-alpha.914')
  self.assertIn('Alpha.915–924',read('CHATUEBERGABE.md'))
if __name__=='__main__': unittest.main()
