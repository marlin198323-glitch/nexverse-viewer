"""F9 Bake-Recheck: new genuine outfit may replan; epoch cap persists."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path):return (R/path).read_text()
class OutfitRecheckRefresh(unittest.TestCase):
 def test_authentication_and_limits(self):
  s=read("include/nexverse/assets/avatar_bake_recheck.hpp")
  for key in ("max_requests=3U","max_total_requests=12U",
              "void observe_source(","!authenticated||!lease_valid",
              "std::all_of(next.begin(),next.end()",
              "total_requests<max_total_requests",
              "++total_requests;","retry_after=now+interval;"):
   self.assertIn(key,s)
 def test_real_viewer_binds_current_source_and_ui(self):
  s=read("src/app/application.cpp")
  self.assertIn("avatar_bake_source_refresh.observe(",s)
  self.assertIn("avatar_bake_recheck.observe_source(",s)
  self.assertIn("Bakes: Outfit-Limit fuer Nachpruefung erreicht",s)
  self.assertIn("Bakes: Regions-Limit fuer Nachpruefung erreicht",s)
  self.assertIn("object_texture_retries,texture_plan_now",s)
 def test_native_cyclic_no_free_infinite_retry(self):
  s=read("tests/avatar_bake_recheck_tests.cpp")
  for key in ("policy.observe_source(23U,true,true,ids)",
              "policy.observe_source(23U,true,true,{})",
              "policy.observe_source(23U,false,true,",
              "policy.observe_source(23U,true,false,",
              "policy.total_requests!=12U",
              "policy.enter(24U)"):
   self.assertIn(key,s)
 def test_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1104")
  self.assertIn("Alpha.1105–1114",read("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
