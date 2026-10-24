"""Four-part product version, milestone gates and conditional release flow."""
import json, re, subprocess, sys, unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def load(f):return (R/f).read_text()
class ReleaseRoadmapContract(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.road=json.loads(load("docs/RELEASE_ROADMAP.json"))
  cls.progress=json.loads(load("docs/PROJECT_PROGRESS.json"))
 def test_version_agrees_everywhere(self):
  self.assertEqual(load("VERSION").strip(),"v0.4.0.0")
  self.assertEqual(self.road["current_version"],"v0.4.0.0")
  self.assertEqual(self.progress["viewer_version"],"v0.4.0.0")
 def test_cmake_has_no_stale_version(self):
  c=load("CMakeLists.txt")
  self.assertIn("NEXVERSE_VERSION_NUMERIC",c)
  self.assertIn("CMAKE_CONFIGURE_DEPENDS",c)
  self.assertNotIn("project(NexVerseViewer VERSION 0.1.0",c)
 def test_ui_does_not_make_double_v(self):
  c=load("src/app/application.cpp")
  self.assertNotIn('std::string{"v"} + std::string{nexverse::core::version()}',c)
  self.assertEqual(c.count('std::string{nexverse::core::version()};'),3)
 def test_stages_unique_ordered(self):
  ms=self.road["milestones"]
  keys=[tuple(map(int,x["version"][1:].split("."))) for x in ms]
  self.assertEqual(keys,sorted(set(keys)))
  self.assertEqual(len(ms),12)
  self.assertEqual(sum(len(m["tasks"]) for m in ms),36)
  self.assertEqual(sum(len(m["acceptance"]) for m in ms),34)
 def test_completed_v03_development_baseline_is_preserved(self):
  ms={m["version"]:m for m in self.road["milestones"]}
  self.assertEqual(ms["v0.3.0.0"]["status"],"FERTIG")
  self.assertEqual(ms["v0.4.0.0"]["status"],"IN_ARBEIT")
  self.assertEqual(ms["v0.4.0.0"]["tasks"][0]["status"],"IN_ARBEIT")
 def test_complete_stable_and_followup_exist(self):
  ms={m["version"]:m for m in self.road["milestones"]}
  self.assertIn("v1.0.0.0",ms["v1.0.0.1"]["depends_on"])
  self.assertTrue(ms["v1.0.0.0"]["release_to_main"])
  self.assertFalse(ms["v0.9.9.0"]["release_to_main"])
 def test_known_targets_and_unknown_rejected(self):
  def call(a,v=None):
   cmd=[sys.executable,str(R/"scripts/ogl-version.py"),a]
   if v:cmd.append(v)
   return subprocess.run(cmd,capture_output=True,text=True,check=False)
  self.assertEqual(call("--check").returncode,0)
  self.assertEqual(call("--target","v1.0.0.0").returncode,0)
  self.assertEqual(call("--target","v1.0.0.1").returncode,0)
  self.assertEqual(call("--target","v999.1.1.1").returncode,2)
 def test_unfinished_full_release_blocks_main(self):
  p=subprocess.run([sys.executable,str(R/"scripts/ogl-version.py"),
     "--release-check","v1.0.0.0"],capture_output=True,text=True)
  self.assertEqual(p.returncode,2)
  self.assertIn("BLOCKED",p.stdout)
 def test_live_release_requirements_are_strict(self):
  s=load("docs/RELEASE_VERSION_POLICY.md")
  for item in ("100 %","sieben","Exit 0","main","v1.0.0.1","SHA"):
   self.assertIn(item,s)
 def test_historical_alpha_kept_but_not_current(self):
  self.assertIn("HISTORIE",load("projectplan.md")[:1900])
  self.assertIn("v0.4.0.0",load("README.md")[:950])
  self.assertIn("ROADMAP.md",load("CHATUEBERGABE.md")[:2800])
  self.assertIn("ALPHA.1194",load("CHATUEBERGABE.md"))
 def test_no_main_release_occurred(self):
  self.assertIn("NOT_RELEASED",self.road["current_status"])
  self.assertIn("BLOCKED",self.progress["goal_100_release_status"])
 def test_numeric_four_parts(self):
  nums=load("VERSION").strip().removeprefix("v").split(".")
  self.assertEqual(len(nums),4)
  self.assertTrue(all(i.isdecimal() for i in nums))
  self.assertEqual(load("VERSION").strip().count("v"),1)
 def test_next_version_clearly_defined(self):
  self.assertEqual(self.progress["next_batch"],"v0.4.0.0")
  self.assertEqual(self.progress["next_five_blocks"],"v0.4.0.0")
  self.assertIn("v0.4.0.0",load("projectplan.md")[:2500])
if __name__=="__main__":unittest.main()
