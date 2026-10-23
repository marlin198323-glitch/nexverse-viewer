"""Stale simulator ObjectUpdate TE replay must not undo an accepted modern COF switch."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(file):return (R/file).read_text()
class OldOutfitTeReplay(unittest.TestCase):
 def test_exact_previous_te_and_localid_fingerprint(self):
  c=rd("src/world/world_model.cpp")
  self.assertIn("retired_self_object_bakes_.push_back(*self_object_bakes_);",c)
  self.assertIn("old.local_id==local_id",c)
  self.assertIn("old.visible_body==bakes",c)
  self.assertIn("++self_object_bake_stale_replays_;",c)
  self.assertIn("if(self_object_bake_stale_replays_<UINT32_MAX)",c)
  self.assertLess(c.index("old.visible_body==bakes"),
                  c.index("self_object_bakes_=SelfAvatarObjectBakes{bakes,local_id};"))
 def test_new_provisional_source_allowed_and_old_replay_does_not_evict(self):
  c=rd("src/world/world_model.cpp")
  self.assertIn("old.local_id==local_id;",c)
  self.assertIn("self_object_bakes_->local_id!=local_id&&",c)
  self.assertIn("region_.handle!=region_handle",c)
  self.assertIn("avatar_id!=agent_.agent_id||local_id==0U",c)
  self.assertIn("self_object_bake_stale_replays_=0U;",c)
 def test_native_cof_retransmission_after_current_body(self):
  t=rd("tests/self_object_bake_fallback_tests.cpp")
  for k in ("outfit.observe_self_object_bakes(avatar_id,region,60U,head_te)",
    "outfit.self_object_bakes()->local_id!=61U",
    "outfit.self_object_bake_stale_replays()!=3U",
    "rebaked.observe_self_object_bakes(avatar_id,region,60U,fresh_te)",
    'fresh_te[k]=0x77U',
    "rebaked.self_object_bake_stale_replays()!=2U",
    "outfit.self_object_bake_stale_replays()!=0U"):
   self.assertIn(k,t)
  self.assertIn("self_object_bake_fallback_tests",rd("scripts/pi5-headless-tests.sh"))
  self.assertIn("nexverse-self-object-bake-fallback-tests",rd("CMakeLists.txt"))
 def test_avatar_f3_counter_without_asset_leaks(self):
  c=rd("src/app/application.cpp")
  h=rd("include/nexverse/world/world_model.hpp")
  self.assertIn("world_model.self_object_bake_stale_replays()",c)
  self.assertIn("Alte Objekt-Bakes erneut verworfen:",c)
  self.assertIn("self_object_bake_stale_replays()",h)
  self.assertIn("retired_self_object_bakes_",h)
  self.assertIn("self_render_bakes() const",h)
 def test_next_release(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.1164")
  self.assertIn("Alpha.1165–1174",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
