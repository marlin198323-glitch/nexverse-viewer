"""Alpha.1034 actual renderer selected body bakes keep Low158 vs provisional ObjTE distinct."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(rel):return (R/rel).read_text()
class CurrentRendererBakeProvenance(unittest.TestCase):
 def test_worldmodel_low158_precedence_and_provisional_fallback(self):
  s=rd("src/world/world_model.cpp")
  a=s.index("SelfRenderBakeSelection WorldModel::self_bake_selection() const")
  b=s.index("std::array<std::string,3U> WorldModel::self_render_bakes()",a)
  body=s[a:b]
  self.assertLess(body.index("out.from_low158[i]"),body.index("out.from_object_update[i]"))
  self.assertIn("out.ids[i].empty()",body)
 def test_actual_application_current_agent_session_lease_provenance(self):
  app=rd("src/app/application.cpp")
  anchor=app.index("const auto selected=world_model.self_bake_selection();",app.index("avatar_diagnostic.self_low158_texture_entry_received=true;"))
  body=app[anchor-630:anchor+1200]
  for token in ("active_session->agent_id==render_agent.agent_id",
                "world_model.agent().session_id==active_session->session_id",
                "selected.from_low158","selected.from_object_update",
                "assets::inspect_self_render_bake_gettexture(",
                "avatar_diagnostic.self_render_body_bake_sources[i]",
                "avatar_diagnostic.self_render_body_gettexture_stages[i]"):
   self.assertIn(token,body)
  self.assertIn("assets::self_render_bake_gettexture_summary(effective)",app)
  self.assertIn("self_bake_it!=admitted_bakes.accepted.end()",app)
  self.assertIn("if(selected.ids==self_bake_ids)",app)
 def test_helper_requires_true_winning_source_and_no_cof_promotion(self):
  s=rd("include/nexverse/assets/avatar_bake_recheck.hpp")
  for token in ("SelfBakeSource::provisional_object_te",
                "if(from_low158[i])","else if(from_object_update[i])",
                "out.stages=inspect_self_bake_gettexture(admitted",
                "if(out.stages[i]==SelfBakeGetTextureStage::no_server_reference)",
                "(L=Low158 O=ObjTE)"):
   self.assertIn(token,s)
 def test_native_mixed_real_sources_and_epoch(self):
  s=rd("tests/avatar_bake_recheck_tests.cpp")
  for token in ("Body H:L/GPU U:O/HTTP404 L:O/ungueltig",
                "U:L/bereit", "empty_source.sources",
                "provisional_probe.sources[1]!=B::provisional_object_te",
                "retry.enter(25U)"):
   self.assertIn(token,s)
 def test_private_report_is_numeric_and_low158_source_is_independent(self):
  s=rd("src/ui/avatar_diagnostic_report.cpp")
  for token in ("self_low158_upper_gettexture_stage=",
                "self_render_upper_bake_source=",
                "self_render_upper_gettexture_stage="):
   self.assertIn(token,s)
  self.assertNotIn("self_render_upper_bake_uuid",s)
if __name__=="__main__":unittest.main()
