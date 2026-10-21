"""Live OpenGenesisLINK yaw flag semantics and simulator-origin readback."""
from pathlib import Path
import unittest,json
ROOT=Path(__file__).resolve().parents[1]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class MovementWireAndReadback(unittest.TestCase):
 def test_current_server_yaw_left_and_right(self):
  c=read('include/nexverse/protocol/movement_controls.hpp')
  self.assertIn('rotate_left ? control_yaw_pos | control_turn_left',c)
  self.assertIn('rotate_right ? control_yaw_neg | control_turn_right',c)
  self.assertNotIn('keys.turn_left ? control_yaw_neg | control_turn_left',c)
 def test_opposing_inputs_cancel_not_diagonal(self):
  c=read('include/nexverse/protocol/movement_controls.hpp')
  for token in ('keys.forward&&!keys.backward','keys.backward&&!keys.forward',
     'keys.left&&!keys.right','keys.right&&!keys.left',
     'keys.turn_left&&!keys.turn_right','keys.turn_right&&!keys.turn_left'):
   self.assertIn(token,c)
 def test_full_lludp_agent_update_actual_control_bytes(self):
  t=read('tests/movement_controls_tests.cpp')
  for text in ('decode_lludp_packet(', 'encode_agent_update(52U,session,action)',
               'wire.packet->payload.size()!=114U', 'bytes[109]',
               'control_turn_left|control_yaw_pos',
               'control_turn_right|control_yaw_neg',
               'server_control_bits(opposite)!=0U',
               'server_control_bits(diagonal)'):
   self.assertIn(text,t)
 def test_turn_not_falsely_confirmed_on_send(self):
  t=read('include/nexverse/protocol/turn_readback.hpp')
  for x in ('void outbound_controls(', 'bool observe_server_rotation(',
      'if(!active_||now<sent_at_)return false;',
      'if(!baseline_){baseline_=yaw;return false;}',
      'delta*static_cast<float>(direction_)>',
      '0.08726646f','now<*last_sample_at_',
      'if(!std::isfinite(q.x)||!std::isfinite(q.y)',
      'std::scoped_lock lock{mutex_}'):
   self.assertIn(x,t)
 def test_upstream_self_quaternions_only(self):
  s=read('src/app/application.cpp')
  self.assertEqual(s.count('turn_feedback.observe_server_rotation('),3)
  for x in ('entry.rotation,std::chrono::steady_clock::now()',
            'update.rotation,now',
            'real self','turn_feedback.outbound_controls(',
            'turn_feedback.turn_active()','turn_feedback.status()'):
   self.assertIn(x,s)
  self.assertEqual(s.count('turn_feedback.reset();'),4)
  self.assertIn('Avatar turn VERIFIED by real self',s)
 def test_native_negative_direction_and_invalid_quaternion(self):
  t=read('tests/movement_controls_tests.cpp')
  for x in ('turn.server_confirmed()', 'server_heading(-0.4f)',
       'server_heading(0.3f)', 'server_heading(-0.5f)',
       '{NAN,0.0f,0.0f,1.0f}', 'turn.reset();',
       'return 39;'):
   self.assertIn(x,t)
 def test_version_plan_handoff(self):
  self.assertEqual(read('VERSION').strip(),'0.2.0-alpha.644')
  s=json.loads(read('docs/PROJECT_PROGRESS.json'))
  self.assertEqual(s['viewer_version'],'0.2.0-alpha.644')
  self.assertEqual(s['next_batch'],'Alpha.645–654')
  self.assertEqual(s['default_blocks_per_run'],10)
  self.assertIn('still open',s['assessment'])
  for file in ('CHATUEBERGABE.md','projectplan.md','AGENTS.md',
               'info.md','docs/PROJEKTFORTSCHRITT.md'):
   self.assertIn('Alpha.645–654',read(file)[:1500])
if __name__=='__main__': unittest.main()
