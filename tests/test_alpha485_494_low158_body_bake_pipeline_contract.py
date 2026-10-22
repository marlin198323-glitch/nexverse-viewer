"""Server-owned Low158 body bake provenance, priorities and truthful F3."""
import unittest,json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text(encoding="utf-8")
class BodyBakePipeline(unittest.TestCase):
 def test_legacy_low158_with_actual_three_face_overrides(self):
  test=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for s in ("baked_entry()", "0x82U,0x84U,0x88U",
    "upstream_legacy_hover_packet(self,true)","const auto baked_low158=decode(baked_wire);",
    "!with_body.apply_self_appearance(*baked_low158)",
    "protocol::extract_avatar_bakes(", "with_body.self_render_bakes()"):
   self.assertIn(s,test)
 def test_three_ordered_bakes_prioritized_and_region_assets_evicted(self):
  t=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for s in ("assets::prioritize_avatar_body_bakes(", "assets::plan_texture_residency(",
    "residency.preferred_ids[0]!=body[0]",
    "residency.preferred_ids[1]!=body[1]",
    "residency.preferred_ids[2]!=body[2]",
    "residency.desired_ids.contains(", "residency.eviction_ids.size()!=1U"):
   self.assertIn(s,t)
 def test_cross_identity_revokes_body_bake_references(self):
  t=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  self.assertIn("with_body.apply_agent({other,session",t)
  self.assertIn("with_body.self_render_bakes()!=std::array<std::string,3U>{}",t)
 def test_diagnostic_is_authenticated_and_same_region(self):
  app=read("src/app/application.cpp")
  for s in ("ap&&login_ui.authenticated&&active_session&&",
    "firestorm_region_lease.matches(current_firestorm_epoch)",
    "ap->avatar_id==active_session->agent_id",
    "avatar_diagnostic.self_low158_texture_entry_received=true",
    "avatar_diagnostic.self_low158_valid_bake_refs=",
    "avatar_diagnostic.self_low158_body_bake_refs",
    "self_low158_head_upper_lower_refs[i]"):
   self.assertIn(s,app)
 def test_private_report_counts_are_not_uuids(self):
  rep=read("src/ui/avatar_diagnostic_report.cpp")
  for s in ("self_low158_texture_entry_received=",
    "self_low158_valid_bake_refs=","self_low158_body_bake_refs=",
    "self_low158_head_ref=","self_low158_upper_ref=","self_low158_lower_ref="):
   self.assertIn(s,rep)
  self.assertNotIn("self_low158_texture_uuid=",rep)
  self.assertNotIn("self_low158_asset_id=",rep)
 def test_f3_distinguishes_absent_refs_from_missing_gpu(self):
  hdr=read("include/nexverse/ui/avatar_bake_inspector.hpp")
  self.assertIn("Self Low158 da, Server liefert keine Body-Bake-IDs",hdr)
  self.assertIn("Self Low158: Body-Bake-IDs noch unvollstaendig",hdr)
  self.assertIn("Original Head/Upper/Lower BakeGL noch unvollstaendig",hdr)
  tests=read("tests/avatar_bake_recheck_tests.cpp")
  self.assertIn('keine Body-Bake-IDs',tests)
  self.assertIn('Body-Bake-IDs noch unvollstaendig',tests)
 def test_upstream_real_packet_and_strict_gate_retained(self):
  t=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  self.assertIn("protocol::packet_zerocoded",t)
  self.assertIn("protocol::valid_self_appearance_texture_entry(",t)
  self.assertIn("legacy->attachment_block_present",t)
  self.assertIn("with_body.apply_agent(",t)
 def test_release(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.834")
  meta=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(meta["viewer_version"],"0.2.0-alpha.834")
  self.assertEqual(meta["next_batch"],"Alpha.835–844")
  self.assertEqual(meta["default_blocks_per_run"],10)
  self.assertIn("still open",meta["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.835–844",read(f)[:1500])
if __name__=="__main__":unittest.main()
