"""Alpha.345-354: F3 can move, actual UDP send receipts, no fake grid ACKs."""
import json,unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def source(p):return (R/p).read_text()
class LiveInputParity(unittest.TestCase):
 def test_non_modal_f3_still_moves(self):
  s=source('include/nexverse/ui/viewer_shell.hpp')
  self.assertIn('movement_input_captured()',s)
  self.assertIn('window==ViewerWindow::chat||window==ViewerWindow::teleport',s)
  a=source('src/app/application.cpp')
  self.assertIn('!viewer_shell.movement_input_captured()',a)
  t=source('tests/viewer_shell_tests.cpp')
  self.assertIn('if(s.movement_input_captured())return 90',t)
 def test_s_does_not_save_diagnostic_while_walking(self):
  a=source('src/app/application.cpp')
  self.assertIn('key==GLFW_KEY_S&&(mods&GLFW_MOD_CONTROL)!=0',a)
  self.assertIn('!ctrl_held&&pressed(GLFW_KEY_S)',a)
  self.assertIn('keys.backward =',a)
  self.assertIn('"Bericht [Strg+S]"',source('src/ui/viewer_shell_render.cpp'))
 def test_real_circuit_udp_send_receipts(self):
  a=source('src/protocol/circuit.cpp')
  h=source('include/nexverse/protocol/circuit.hpp')
  for marker in ('++status.agent_updates_sent',
                 '++status.directional_agent_updates_sent',
                 'if(sent)','send_datagram(socket, packet, status)'):
   self.assertIn(marker,a)
  self.assertIn('directional_agent_updates_sent=0U',h)
  self.assertIn('NOT a simulator ACK',h)
 def test_f3_readback_not_server_ack(self):
  a=source('src/app/application.cpp')
  s=source('src/ui/viewer_shell_render.cpp')
  self.assertIn('avatar_udp_directional_sent=',a)
  self.assertIn('circuit_snapshot.directional_agent_updates_sent',a)
  self.assertIn('" | UDP-WASD:"',s)
  self.assertIn('movement_feedback.status()',a)
 def test_invalid_avatar_appearance_is_visible_without_f3(self):
  a=source('src/app/application.cpp')
  self.assertIn('appearance_runtime.invalid_stage',a)
  self.assertIn('9.1f*scale,avatar_warning.empty()',a)
  self.assertIn('!avatar_proof.appearance_received&&appearance_runtime.malformed>0U',a)
  self.assertNotIn('avatar_id+" | "',a)
 def test_release_docs(self):
  self.assertEqual(source('VERSION').strip(),'0.2.0-alpha.784')
  d=json.loads(source('docs/PROJECT_PROGRESS.json'))
  self.assertEqual(d['viewer_version'],'0.2.0-alpha.784')
  self.assertEqual(d['next_batch'],'Alpha.785–794')
  self.assertEqual(d['default_blocks_per_run'],10)
  for p in ('CHATUEBERGABE.md','AGENTS.md','info.md','projectplan.md','docs/PROJEKTFORTSCHRITT.md'):
   self.assertIn('Alpha.785–794',source(p)[:1900])
   self.assertIn('39 %',source(p)[:1900])
if __name__=='__main__':unittest.main()
