"""A recycled numeric High9 LocalID cannot prove the SELF avatar after a simulator epoch change."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(name):return (R/name).read_text()
class SelfLocalIdGeneration(unittest.TestCase):
 def test_generation_admission_even_at_initial_epoch_zero(self):
  h=rd("include/nexverse/world/self_avatar_localid_lease.hpp")
  self.assertIn("current_self_avatar_local_id(",h)
  self.assertIn("bound_generation==current_generation",h)
  self.assertNotIn("bound_generation!=0U",h)
 def test_real_full_compressed_bind_and_terse_kill_render(self):
  s=rd("src/app/application.cpp")
  self.assertIn("std::uint32_t self_avatar_region_generation = 0U;",s)
  self.assertEqual(s.count("self_avatar_region_generation=\n                                world_model.region().generation;"),2)
  self.assertGreaterEqual(s.count("world::current_self_avatar_local_id("),7)
  self.assertIn("world_model.objects(),world::current_self_avatar_local_id(",s)
  self.assertIn("true,baked_ids,world::current_self_avatar_local_id(",s)
  self.assertIn("update.local_id!=0U &&\n                        world::current_self_avatar_local_id(",s)
 def test_current_epoch_rebind_can_arrive_before_ui_loop(self):
  s=rd("src/app/application.cpp")
  self.assertIn("if(animation_asset_epoch!=current_firestorm_epoch)",s)
  self.assertIn("if(world::current_self_avatar_local_id(",s)
  self.assertIn("self_avatar_region_generation=0U;",s)
  t=rd("tests/early_avatar_readback_tests.cpp")
  self.assertIn("prior_self_local_id,region,stable_epoch",t)
  self.assertIn("socket_region.region().generation)!=0U",t)
  self.assertIn("prior_self_local_id,region,socket_region.region().generation",t)
  native=rd("tests/self_avatar_pose_authority_tests.cpp")
  self.assertIn("local(avatar_local_id,region,0U,region,0U)!=avatar_local_id",native)
 def test_version_and_next_ten(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.1174")
  self.assertIn("Alpha.1175–1184",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
