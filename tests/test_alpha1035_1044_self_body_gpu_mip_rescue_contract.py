"""Alpha.1044: bounded no-HTTP current self skin fallback under GPU pressure."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text()
class SelfBodyMipRescue(unittest.TestCase):
 def test_existing_transactional_gpu_budget_checks_are_not_bypassed(self):
  s=rd("include/nexverse/assets/avatar_bake_mip_priority.hpp")
  for part in ("gpu_staging_budget(resident_textures,resident_meshes",
               "plan_texture_byte_budget(priority,resident_bytes",
               "if(fits(width,height))return std::nullopt;",
               "extra<=3U-requested_mip","return std::nullopt;",
               "std::chrono::seconds{60}","floor_.clear();"):
   self.assertIn(part,s)
 def test_real_current_session_body_only_and_no_second_download(self):
  app=rd("src/app/application.cpp")
  a=app.index("const bool current_self_bake=active_session&&")
  b=app.index("const std::size_t next_bytes =",a)
  body=app[a:b]
  for t in ("login_ui.authenticated", "render_agent.present",
            "active_session->agent_id==render_agent.agent_id",
            "firestorm_region_lease.matches(current_firestorm_epoch)",
            "admitted_bakes.accepted.end()",
            "own_proxy->body",
            "texture_residency.desired_ids.contains",
            "plan_texture_slot_admission",
            "choose_self_avatar_budget_mip_rescue",
            "texture_mip_image(\n                                       result.image,*extra)"):
   self.assertIn(t,body)
  self.assertIn("admit_avatar_bake_gpu_candidates(",app)
  self.assertIn("if(rescued_self_mip)",app)
  self.assertIn("if (result.message == \"RAM texture cache hit\")",app)
 def test_commit_after_native_gpu_success_and_session_expiry(self):
  app=rd("src/app/application.cpp")
  self.assertLess(app.index("if (const auto old = texture_gpu_bytes.find("),
                   app.index("if(rescued_self_mip)"))
  self.assertIn("self_avatar_mip_rescue.admitted(",app)
  self.assertIn("texture_desired_mips[object_texture_loading_id]=",app)
  self.assertIn("self_avatar_mip_rescue.minimum_mip(",app)
  self.assertIn("self_avatar_mip_rescue.enter(material_scope.epoch())",app)
  self.assertIn("Body-Mip-GPU-Rettung:",app)
 def test_native_real_size_pressure_and_stale_lifetime(self):
  s=rd("tests/avatar_bake_mip_priority_tests.cpp")
  for t in ("4096U,4096U,0U,100U*mib,260U*mib",
            "if(!extra||*extra!=2U)", "now+std::chrono::seconds{60}",
            "floor.enter(42U)"):
   self.assertIn(t,s)
if __name__=="__main__":unittest.main()
