"""Actual Low158 multi-format COF replay must never repaint old outfits."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(n): return (R/n).read_text()
class CofFormatReplay(unittest.TestCase):
 def test_world_model_highwater_scoped_per_appearance_format(self):
  h=read("include/nexverse/world/world_model.hpp")
  s=read("src/world/world_model.cpp")
  self.assertIn("std::array<std::uint32_t,256U> self_cof_by_format_highwater_{}",h)
  self.assertIn("self_cof_by_format_highwater_[incoming.appearance_version]",s)
  self.assertIn("per_format=std::max(per_format,incoming.cof_version)",s)
  self.assertEqual(s.count("self_cof_by_format_highwater_.fill(0U);"),4)
 def test_native_real_low158_wire_format_switch(self):
  s=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for x in ("trailing_unknown_cof?2U:1U",
            "const auto replay_v2=decode(datagram(self,1U,29U,110U,2U,true));",
            "const auto fresh_v3=decode(datagram(self,1U,30U,109U,3U,true));",
            "const auto delayed_v3=decode(datagram(self,1U,33U,108U,3U,true));"):
   self.assertIn(x,s)
 def test_gpu_pixel_no_outfit_rollback(self):
  s=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ("format3.appearance_data={{3U,52U,0U}};",
            "v2_stale.appearance_data.front().cof_version=202U;",
            "difference(skin_v3,after_format_replay)!=0U"):
   self.assertIn(x,s)
 def test_version_docs(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.914")
  self.assertIn("Alpha.915–924",read("CHATUEBERGABE.md"))
if __name__=="__main__": unittest.main()
