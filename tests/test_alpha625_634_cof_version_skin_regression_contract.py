"""Modern Low158 COF version replay cannot roll back self Firestorm skin."""
from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def rd(p):return (R/p).read_text(encoding="utf-8")
class CofVersionRollback(unittest.TestCase):
 def test_versioned_model_only_rejects_lower_same_protocol(self):
  s=rd("src/world/world_model.cpp")
  a=s.index("bool WorldModel::apply_self_appearance(")
  b=s.index("bool WorldModel::apply_self_wearables(",a)
  code=s[a:b]
  for t in ("valid_self_appearance_texture_entry(appearance.texture_entry)",
    "if(!appearance.appearance_data.empty())",
    "incoming.cof_version!=0U",
    "self_cof_version_highwater_!=0U",
    "self_cof_format_highwater_==incoming.appearance_version",
    "incoming.cof_version<self_cof_version_highwater_",
    "++stale_self_cof_appearances_;",
    "self_cof_version_highwater_=incoming.cof_version;",
    "self_appearance_ = std::move(appearance);"):
   self.assertIn(t,code)
  self.assertLess(code.index("incoming.cof_version<self_cof_version_highwater_"),
                  code.index("self_appearance_ = std::move(appearance);"))
 def test_login_and_region_reset_old_revision(self):
  s=rd("src/world/world_model.cpp")
  self.assertEqual(s.count("stale_self_cof_appearances_=0U;"),4)
  self.assertEqual(s.count("self_cof_version_highwater_=0U;"),4)
  self.assertEqual(s.count("self_cof_format_highwater_=0U;"),4)
  self.assertIn("WorldModel::stale_self_cof_appearances() const noexcept",s)
 def test_actual_modern_and_legacy_low158_wire(self):
  t=rd("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for x in ("cof_version=111U",
    "le32(payload,cof_version)",
    "protocol::decode_avatar_appearance(",
    "const auto older=decode(datagram(self,2U,24U,110U))",
    "const auto newer=decode(datagram(self,2U,25U,112U))",
    "const auto same=decode(datagram(self,3U,26U,112U))",
    "const auto legacy_again=decode(upstream_legacy_hover_packet(self,true))",
    "const auto old_after_legacy=decode(datagram(self,1U,28U,111U))",
    "const auto different_format=decode(datagram(self,1U,27U,108U))",
    "w.stale_self_cof_appearances()!=0U"):
   self.assertIn(x,t)
 def test_real_same_session_and_reset_skin(self):
  s=rd("tests/self_appearance_texture_integrity_tests.cpp")
  for x in ("current.appearance_data={{2U,900U,0U}}",
    "stale.appearance_data.back().cof_version=899U",
    "legacy.appearance_data.clear()",
    "model.stale_self_cof_appearances()!=2U",
    "model.apply_agent(", "model.apply_region("):
   self.assertIn(x,s)
 def test_original_three_body_open_gl_pixel_rejection(self):
  s=rd("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ("#include \"nexverse/world/world_model.hpp\"",
    "skin.apply_self_appearance(appearance)",
    "renderer->set_object_texture(new_id,make_paint(235U,20U))",
    "renderer->set_object_texture(old_id,make_paint(20U,235U))",
    "delayed.appearance_data.back().cof_version=201U",
    "self.baked_texture_ids=skin.self_render_bakes();",
    "STALE_COF_LOW158_OLD_SKIN_GPU_PIXELS=",
    "if(stale_gpu!=0U)error=26;",
    "delayed.appearance_data.back().cof_version=203U",
    "if(difference(new_skin,actually_newer)<100U)error=28;"):
   self.assertIn(x,s)
 def test_f3_and_private_report_limited_to_count(self):
  s=rd("src/app/application.cpp")
  self.assertIn("world_model.stale_self_cof_appearances();",s)
  self.assertIn('" | COF alt verworfen:"+std::to_string(stale)',s)
  r=rd("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn('"appearance_self_stale_cof_rejected="+',r)
  self.assertNotIn('"current_cof_uuid=',r)
  self.assertNotIn('"old_skin_uuid=',r)
 def test_release_handoff(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.644")
  j=json.loads(rd("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(j["viewer_version"],"0.2.0-alpha.644")
  self.assertEqual(j["next_batch"],"Alpha.645–654")
  self.assertEqual(j["default_blocks_per_run"],10)
  self.assertIn("still open",j["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("10 ECHTE ENTWICKLUNGSBLÖCKE",rd(f)[:350])
   self.assertIn("Alpha.645–654",rd(f)[:1800])
if __name__=="__main__":unittest.main()
