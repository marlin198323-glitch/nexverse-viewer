"""A confirmed newer COF outfit invalidates the previous outfit's provisional TE bakes."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text()
class CofOutfitBakeLease(unittest.TestCase):
 def test_worldmodel_outfit_switch_after_authoritative_admission(self):
  s=rd("src/world/world_model.cpp")
  self.assertIn("const auto previous_outfit=self_appearance_",s)
  self.assertIn("protocol::latest_nonzero_avatar_cof(appearance)",s)
  self.assertIn("self_cof_version_highwater_!=0U",s)
  self.assertIn("const bool prior_outfit_known=",s)
  self.assertIn("if(prior_outfit_known&&incoming_outfit&&",s)
  self.assertIn("self_object_bakes_.reset();",s)
  self.assertIn("++self_object_bake_outfit_invalidations_;",s)
  self.assertLess(s.index("if((self_cof_version_highwater_!=0U&&"),
                  s.index("if(prior_outfit_known&&incoming_outfit&&"))
  self.assertLess(s.index("if(prior_outfit_known&&incoming_outfit&&"),
                  s.index("self_appearance_ = std::move(appearance);"))
 def test_no_implicit_outfit_rewrite_or_break_initial_fallback(self):
  s=rd("src/world/world_model.cpp")
  self.assertIn("SelfRenderBakeSelection WorldModel::self_bake_selection() const",s)
  self.assertIn("out.from_object_update[i]=true;",s)
  self.assertIn("out.from_low158[i]=!out.ids[i].empty();",s)
  self.assertIn("if(self_object_bakes_)",s)
  self.assertIn("self_object_bake_outfit_invalidations_=0U;",s)
 def test_native_multiple_cof_and_legacy_interleave(self):
  s=rd("tests/self_object_bake_fallback_tests.cpp")
  for token in ('first_cof.appearance_data={{1U,100U,0U}};',
    'newer_cof.appearance_data={{1U,101U,0U}};',
    'outfit.apply_self_appearance(authoritative)',
    'newer_cof.appearance_data={{2U,1U,0U}};',
    'outfit.self_object_bake_outfit_invalidations()!=2U',
    'outfit.observe_self_object_bakes(avatar_id,region,61U,head_te)',
    'outfit.apply_agent({avatar_id,"cccccccc-bbbb-cccc-dddd-eeeeeeeeeeee"'):
   self.assertIn(token,s)
  self.assertIn('nexverse-self-object-bake-fallback-tests',rd("CMakeLists.txt"))
  self.assertIn("self_object_bake_fallback_tests",rd("scripts/pi5-headless-tests.sh"))
 def test_f3_evidence_without_faking_cof_skin(self):
  s=rd("src/app/application.cpp")
  h=rd("include/nexverse/world/world_model.hpp")
  self.assertIn('COF-Wechsel: alte Objekt-Bakes verworfen:',s)
  self.assertIn('world_model.self_object_bake_outfit_invalidations()',s)
  self.assertIn('self_object_bake_outfit_invalidations()',h)
  self.assertIn('const noexcept;',h)
  self.assertIn('self_object_bake_outfit_invalidations_=0U;',h)
  self.assertIn('world_model.self_render_bakes()',s)
 def test_release(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.1074")
  self.assertIn("Alpha.1075–1084",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
