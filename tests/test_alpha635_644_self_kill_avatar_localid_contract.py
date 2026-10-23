"""Real simulator self KillObject withdraws LocalID/terse and provisional skin."""
from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text(encoding="utf-8")
class OwnObjectKill(unittest.TestCase):
 def test_real_lludp_kill_dispatcher_current_self_binding(self):
  s=rd("src/app/application.cpp")
  a=s.index("protocol::msg_kill_object,")
  b=s.index("protocol::msg_coarse_location_update,",a)
  h=s[a:b]
  for token in (
      "protocol::decode_kill_object(packet.payload)",
      "self_avatar_local_id==local_id",
      "self_avatar_region_handle==world_model.region().handle",
      "world_model.withdraw_self_object_on_kill(",
      "self_avatar_local_id=0U;",
      "self_avatar_region_handle=0U;",
      "world_model.kill_simulator_local_id(local_id)"):
   self.assertIn(token,h)
  self.assertLess(h.index("self_avatar_local_id=0U;"),
                  h.index("world_model.kill_simulator_local_id(local_id)"))
 def test_both_self_full_and_compressed_require_nonzero_identity(self):
  s=rd("src/app/application.cpp")
  self.assertEqual(s.count("if(entry.local_id==0U||"),2)
  self.assertGreaterEqual(s.count("world::valid_simulator_avatar_pose("),3)
  self.assertIn("if (self_avatar_local_id != 0U &&",s)
  self.assertIn("update.local_id == self_avatar_local_id",s)
  self.assertIn("protocol::msg_improved_terse_object_update,",s)
 def test_model_keeps_trusted_low158_but_revokes_provisional(self):
  m=rd("src/world/world_model.cpp")
  a=m.index("bool WorldModel::withdraw_self_object_on_kill(")
  b=m.index("std::uint32_t WorldModel::self_object_kills()",a)
  body=m[a:b]
  for t in ("!region_.connected","region_.handle!=region_handle",
            "avatar_id!=agent_.agent_id||local_id==0U",
            "self_object_bakes_->local_id==local_id",
            "self_object_bakes_.reset();","++self_object_kills_"):
   self.assertIn(t,body)
  self.assertNotIn("self_appearance_.reset()",body)
  self.assertEqual(m.count("self_object_kills_=0U;"),4)
  self.assertIn("WorldModel::self_object_kills() const noexcept",m)
 def test_native_true_kill_wire_current_binding_and_epochs(self):
  s=rd("tests/self_avatar_kill_binding_tests.cpp")
  for t in ("protocol::decode_kill_object(packet)",
    "current_self_local_id=0U",
    "w.self_render_bakes()!=std::array<std::string,3U>{}",
    "w.self_object_kills()!=1U",
    "w.self_bake_selection().ids[0]!=low158.ids[0]",
    "w.apply_agent({own,",
    "w.apply_region(region_packet(false))",
    "w.withdraw_self_object_on_kill(another,region,51U)"):
   self.assertIn(t,s)
 def test_native_actual_original_firestorm_gl_skin_removed(self):
  s=rd("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for t in ('"SELF_KILLOBJECT_STALE_GPU_SKIN_PIXELS="',
            "self_world.observe_self_object_bakes(own,region,51U,te)",
            "self_world.withdraw_self_object_on_kill(own,region,51U)",
            "self.baked_texture_ids=self_world.self_render_bakes();",
            "remaining!=0U",
            "self_avatar_baked_texture_draw_calls!=0U"):
   self.assertIn(t,s)
 def test_f3_private_report_counts_without_ids(self):
  app=rd("src/app/application.cpp")
  self.assertIn("avatar_diagnostic.self_object_kills=",app)
  self.assertIn('\" | Eigene Objekt-ID verworfen:\"+',app)
  diag=rd("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn('"self_object_localid_kills="+',diag)
  self.assertNotIn('"own_local_id_uuid=',diag)
  native=rd("tests/avatar_diagnostic_report_tests.cpp")
  self.assertIn("r.self_object_kills=2U;",native)
  self.assertIn('text.find("self_object_localid_kills=2")',native)
 def test_version_next_and_status(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.854")
  d=json.loads(rd("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(d["viewer_version"],"0.2.0-alpha.854")
  self.assertEqual(d["next_batch"],"Alpha.855–864")
  self.assertEqual(d["default_blocks_per_run"],10)
  self.assertIn("still open",d["assessment"])
  for p in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("10 ECHTE ENTWICKLUNGSBLÖCKE",rd(p)[:350])
   self.assertIn("Alpha.855–864",rd(p)[:1800])
if __name__=="__main__":unittest.main()
