"""Legacy Low382 vs COF is read-only evidence, never proof of worn outfit."""
import unittest,json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class LegacyCofComparison(unittest.TestCase):
 def test_expected_cof_join(self):
  s=read("include/nexverse/protocol/legacy_wearables_cof_audit.hpp")
  for t in ("link.asset_type!=24","originals.find(link.asset_id)",
    "item.asset_type!=5&&item.asset_type!=13","item.inventory_type!=18",
    "(*item.flags&0xffU)>16U","expected.find(entry.item_id)",
    "original.asset_id!=entry.asset_id",
    "(*original.flags&0xffU)!=entry.wearable_type"):
   self.assertIn(t,s)
 def test_missing_sources_are_unknown(self):
  s=read("include/nexverse/protocol/legacy_wearables_cof_audit.hpp")
  for t in ("if(!legacy)return result;","if(!cof_links||!fetched_originals)return result;",
     "can_prove_worn() const noexcept {return false;}","kein Wear-Beweis"):
   self.assertIn(t,s)
 def test_null_slots_and_duplicates(self):
  s=read("include/nexverse/protocol/legacy_wearables_cof_audit.hpp")
  for t in ("if(entry.item_id==zero||entry.item_id.empty())continue;",
       "duplicate_legacy_items","unresolved_cof_links"):
   self.assertIn(t,s)
 def test_f9_auth_and_lease_scoping(self):
  s=read("src/app/application.cpp")
  for t in ("gallery_wearables_fetch_epoch==current_firestorm_epoch",
    "gallery_wearables_fetch_cof==cof_folder",
    "version->second==gallery_wearables_fetch_version",
    "worn->agent_id==agent.agent_id",
    "worn->session_id==agent.session_id",
    "linked!=caps_snapshot.outfit_folder_items.end()",
    "compare_legacy_wearables_cof("):
   self.assertIn(t,s)
 def test_f9_real_render_line(self):
  s=read("src/app/application.cpp")
  for t in ("legacy_cof_status).substr(0U,99U)",
     "gallery_wearables_status,legacy_cof_status",
     "Legacy/COF: Simulator-Wearables ausstehend"):
   self.assertIn(t,s)
 def test_native_asset_type_dummy_mismatch(self):
  s=read("tests/legacy_wearables_cof_audit_tests.cpp")
  for t in ("mismatched_asset_or_type!=1U",
       "duplicate_legacy_items!=1U","cof_not_in_legacy!=2U",
       "unresolved_cof_links!=1U","not_in_cof!=1U",
       "result.can_prove_worn()","result.label().find(u1)"):
   self.assertIn(t,s)
 def test_release(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.724")
  data=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(data["viewer_version"],"0.2.0-alpha.724")
  self.assertEqual(data["next_batch"],"Alpha.725–734")
  self.assertEqual(data["default_blocks_per_run"],10)
  self.assertIn("still open",data["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.725–734",read(f)[:1200])
if __name__=="__main__":unittest.main()
