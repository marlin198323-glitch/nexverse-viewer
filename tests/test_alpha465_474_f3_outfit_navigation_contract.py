"""F3 Outfit navigation must be real and F9 evidence owner-scoped."""
from pathlib import Path
import json
import unittest
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class F3F9OutfitNavigation(unittest.TestCase):
 def test_f3_outfit_click_action_is_existing_f9_workflow(self):
  head=read("include/nexverse/ui/viewer_shell.hpp")
  self.assertIn("return ViewerAction::outfits;",head)
  self.assertIn("panel_w>=715.0f*scale",head)
  self.assertIn("panel_x+571.0f*scale",head)
  app=read("src/app/application.cpp")
  self.assertIn("command==nexverse::ui::ViewerAction::outfits",app)
  self.assertIn("*context->outfits=true;",app)
  self.assertIn("render_outfit_gallery(",app)
 def test_f3_keyboard_o_navigates_but_never_wears(self):
  app=read("src/app/application.cpp")
  self.assertIn("key==GLFW_KEY_O&&context->outfits",app)
  self.assertIn("*context->outfits=true;",app)
  self.assertIn("shell.close();",app)
  self.assertNotIn("encode_agent_is_now_wearing(",app)
 def test_f3_button_has_render_and_real_hit_tests(self):
  view=read("src/ui/viewer_shell_render.cpp")
  self.assertIn("Outfit [O]",view)
  self.assertIn("pw>=715.0f*sc",view)
  cpp=read("tests/viewer_shell_tests.cpp")
  self.assertIn("ViewerAction::outfits)return 95;",cpp)
  self.assertIn("const ViewerShellLayout narrow{480,420};",cpp)
  self.assertIn("return 97;",cpp)
 def test_cof_fetch_is_bound_to_authenticated_owner(self):
  app=read("src/app/application.cpp")
  for token in ("gallery_wearables_fetch_owner=active_session->agent_id",
        "active_session->agent_id==gallery_wearables_fetch_owner",
        "gallery_wearables_fetch_owner==active_session->agent_id",
        "gallery_wearables_fetch_owner.clear()"):
   self.assertIn(token,app)
  for token in ("gallery_wearables_fetch_epoch==current_firestorm_epoch",
                "gallery_wearables_fetch_cof==cof_folder",
                "version->second==gallery_wearables_fetch_version",
                "worn->agent_id==agent.agent_id",
                "worn->session_id==agent.session_id"):
   self.assertIn(token,app)
 def test_no_clipped_disclaimer_and_real_counter_names(self):
  audit=read("include/nexverse/protocol/legacy_wearables_cof_audit.hpp")
  for label in ("A:"," F:"," Z:"," D:"," O:","kein Wear-Beweis"):
   self.assertIn(label,audit)
  test=read("tests/legacy_wearables_cof_audit_tests.cpp")
  self.assertIn("overloaded.label().size()>99U",test)
  self.assertIn("doppelt",read("docs/ALPHA465_474_F3_F9_OUTFIT_NAVIGATION.md"))
 def test_f9_remains_read_only(self):
  app=read("src/app/application.cpp")
  self.assertIn("compare_legacy_wearables_cof(",app)
  self.assertIn("gallery_wearables_status,legacy_cof_status",app)
  self.assertIn("kein Wear-Beweis",read("include/nexverse/protocol/legacy_wearables_cof_audit.hpp"))
 def test_version_progress_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.664")
  obj=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(obj["viewer_version"],"0.2.0-alpha.664")
  self.assertEqual(obj["next_batch"],"Alpha.665–674")
  self.assertEqual(obj["default_blocks_per_run"],10)
  self.assertIn("still open",obj["assessment"])
  for f in ("CHATUEBERGABE.md","AGENTS.md","projectplan.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.665–674",read(f)[:1300])
if __name__=="__main__":unittest.main()
