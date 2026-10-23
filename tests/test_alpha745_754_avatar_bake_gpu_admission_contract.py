"""Avatar GetTexture streaming must use real GL accepted poses."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def content(name):return (R/name).read_text()
class AvatarGpuBakeAdmission(unittest.TestCase):
 def test_real_planner_and_recheck_share_one_admission(self):
  src=content("src/app/application.cpp")
  self.assertEqual(src.count("assets::admit_avatar_bake_gpu_candidates("),1)
  self.assertIn("self_bake_it==admitted_bakes.accepted.end()",src)
  self.assertIn("self_bake_it->body",src)
  self.assertIn("admitted_bakes.accepted,render_agent.position.x",src)
  self.assertIn("for(const auto& avatar:admitted_bakes.accepted)",src)
  self.assertIn("mip_self->body",src)
 def test_real_backend_renderer_pose_contract_reused(self):
  hdr=content("include/nexverse/assets/avatar_bake_gpu_admission.hpp")
  self.assertIn("sanitize_avatar_proxy_pose(checked)",hdr)
  self.assertIn("if(!result.position_valid)",hdr)
  self.assertIn("proxy.baked_texture_ids",hdr)
  self.assertIn("if(proxy.is_self)admitted.self_position_skipped=true;",hdr)
 def test_native_stream_priority_recovery_and_missing_self(self):
  s=content("tests/avatar_bake_stream_priority_tests.cpp")
  self.assertIn("admit_avatar_bake_gpu_candidates(proxies)",s)
  self.assertIn("invalid_self.self_position_skipped",s)
  self.assertIn("proxies[2].rotation={nan",s)
  self.assertIn("invalid-head",s)
 def test_next_release_docs(self):
  self.assertEqual(content("VERSION").strip(),"0.2.0-alpha.964")
  self.assertIn("Alpha.965–974",content("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
