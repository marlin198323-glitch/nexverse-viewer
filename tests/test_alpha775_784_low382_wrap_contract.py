"""Actual authenticated Low382 readback preserves worn state after serial wrap."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text()
class Low382Wrap(unittest.TestCase):
 def test_real_handler_modular_serial(self):
  s=read("src/world/world_model.cpp")
  self.assertIn("protocol::wearables_serial_newer(",s)
  self.assertNotIn("update.serial<=self_wearables_->serial",s)
  h=read("include/nexverse/protocol/wearables_serial_order.hpp")
  self.assertIn("return delta!=0U&&delta<0x80000000U;",h)
 def test_native_real_low382_datagrams_wrap_and_retransmit(self):
  s=read("tests/agent_wearables_readback_tests.cpp")
  for token in ("const auto near_wrap=decode(packet(",
   "const auto wrapped0=decode(packet(","const auto wrapped1=decode(packet(",
   "const auto old_retransmit=decode(packet(",
   "w.apply_self_wearables(*old_retransmit)",
   "w.apply_agent({other_id,session_id,{},{},true});"):
   self.assertIn(token,s)
 def test_project_version(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1064")
  self.assertIn("Alpha.1065–1074",read("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
