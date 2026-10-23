"""Valid current self TextureEntry withdrawals must not leave ghost skins."""
from pathlib import Path
import json
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(file):return (ROOT/file).read_text(encoding="utf-8")
class BakeWithdrawalContract(unittest.TestCase):
 def test_valid_empty_clears_only_previous_provisional(self):
  s=read("src/world/world_model.cpp")
  a=s.index("bool WorldModel::observe_self_object_bakes(")
  b=s.index("const std::optional<SelfAvatarObjectBakes>&",a)
  body=s[a:b]
  for token in ("if(!protocol::parse_texture_entry(texture_entry))",
    "const auto bakes=protocol::extract_avatar_bakes(texture_entry).visible_body();",
    "if(bakes[0].empty()&&bakes[1].empty()&&bakes[2].empty())",
    "self_object_bakes_->local_id==local_id",
    "self_object_bakes_.reset();", "++self_object_bake_withdrawals_;"):
   self.assertIn(token,body)
  self.assertLess(body.index("if(!protocol::parse_texture_entry(texture_entry))"),
                  body.index("++self_object_bake_withdrawals_;"))
 def test_withdrawal_scoped_and_diagnostic_only_counts(self):
  model=read("src/world/world_model.cpp")
  self.assertIn("std::uint32_t WorldModel::self_object_bake_withdrawals()",model)
  self.assertEqual(model.count("self_object_bake_withdrawals_=0U;"),4)
  diag=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn('"self_object_bake_withdrawals="+',diag)
  app=read("src/app/application.cpp")
  self.assertIn("avatar_diagnostic.self_object_bake_withdrawals=",app)
  self.assertIn('" | Self-Bake reset:"+std::to_string(removed)',app)
  self.assertIn("world_model.agent().session_id==active_session->session_id",app)
  self.assertNotIn('"self_object_bake_withdrawn_uuid=',diag)
 def test_native_per_face_low158_and_corrupt(self):
  s=read("tests/self_object_bake_fallback_tests.cpp")
  for token in ("tex(true,true)", "tex(false)",
      "std::vector<std::uint8_t>{2U,3U}",
      "self_object_bake_withdrawals()!=1U",
      "self_object_bake_withdrawals()!=2U",
      "self_object_bake_withdrawals()!=0U",
      "w.self_render_bakes()!=fallback",
      "w.self_bake_selection().from_low158[1]"):
   self.assertIn(token,s)
 def test_real_gl_no_ghost_from_still_gpu_resident_bake(self):
  s=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for token in ("WITHDRAWN_SELF_BAKES_STALE_GPU_PIXELS=",
    "self.baked_texture_ids={};", "renderer->set_avatar_proxies({self});",
    "ghost_pixels!=0U", "draw_status.self_avatar_baked_texture_draw_calls!=0U",
    "std::array<std::uint64_t,3>{0U,0U,0U}",
    'self.baked_texture_ids={"fixture-bake","fixture-bake","fixture-bake"};'):
   self.assertIn(token,s)
 def test_cof_and_region_security_untouched(self):
  model=read("src/world/world_model.cpp")
  self.assertIn("self_appearance_",model)
  self.assertIn("agent_.session_id!=update.session_id",model)
  self.assertIn("region_.handle!=update.handle",model)
  self.assertIn("self_render_bakes()",model)
 def test_version_and_next(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.994")
  p=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(p["viewer_version"],"0.2.0-alpha.994")
  self.assertEqual(p["next_batch"],"Alpha.995–1004")
  self.assertEqual(p["default_blocks_per_run"],10)
  self.assertIn("still open",p["assessment"])
  for fn in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
              "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.995–1004",read(fn)[:1800])
if __name__=="__main__":unittest.main()
