"""Strict offline unit tests for complete commit pagination and missing pages."""
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT=Path(__file__).resolve().parent.parent
SCRIPT=ROOT/"scripts/audit_opengenesislink_commits.py"
sys.path.insert(0,str(SCRIPT.parent))
import audit_opengenesislink_commits as audit

class PerCommitInventoryTests(unittest.TestCase):
    def test_pages_exactly_100_and_preserve_categories(self):
        sha="b"*40
        base=f"{audit.API}/commits/{sha}?per_page=100&page="
        first=[{"filename":f"doc/Audit/{n}.md"} for n in range(100)]
        last=[{"filename":"OpenSim/Framework/AssetUploadLevelPolicy.cs"}]
        fetch=lambda url:{'sha':sha,'files':{base+"1":first,base+"2":last}[url]}
        result=audit.inventory("a"*40,sha,
            {"ahead_by":1,"commits":[{"sha":sha}]},fetch)
        self.assertEqual(result["changed_file_entries"],101)
        self.assertEqual(result["commits_inventoried"],1)
        self.assertEqual(result["classifications"].get(
            "SERVER_ONLY_ASSET_UPLOAD_PERMISSION"),1)
        self.assertFalse(result["compatibility_approved"])
    def test_missing_or_duplicate_pages_block(self):
        sha="c"*40
        compare={"ahead_by":1,"commits":[{"sha":sha}]}
        base=f"{audit.API}/commits/{sha}?per_page=100&page="
        first=[{"filename":f"f{n}"} for n in range(100)]
        with self.assertRaises((KeyError,ValueError)):
            audit.inventory("a"*40,sha,compare,
                lambda url:{'sha':sha,'files':{base+"1":first}[url]})
        with self.assertRaises(ValueError):
            audit.inventory("a"*40,sha,compare,
                lambda url:{'sha':sha,
                    'files':[{'filename':'same'},{'filename':'same'}]})
    def test_compare_missing_commits_fails_closed(self):
        with self.assertRaises(ValueError):
            audit.inventory("a"*40,"b"*40,
                {"ahead_by":2,"commits":[{"sha":"b"*40}]},lambda url:{})
    def test_cli_fixture_is_not_live_approval(self):
        sha="b"*40
        page=f"{audit.API}/commits/{sha}?per_page=100&page=1"
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            (root/"base.json").write_text(json.dumps({"reviewed_sha":"a"*40}))
            (root/"fixture.json").write_text(json.dumps({
                "head":sha,"compare":{"ahead_by":1,
                                      "commits":[{"sha":sha}]},
                "pages":{page:{"sha":sha,"files":[
                    {"filename":"OpenSim/Framework/AssetUploadLevelPolicy.cs"}
                ]}}}))
            output=root/"inventory.json"
            proc=subprocess.run([sys.executable,str(SCRIPT),
                "--baseline",str(root/"base.json"),
                "--fixture",str(root/"fixture.json"),
                "--output",str(output)],
                text=True,capture_output=True,timeout=6,check=False)
            self.assertEqual(proc.returncode,0,proc.stderr)
            data=json.loads(output.read_text())
            self.assertFalse(data["compatibility_approved"])
            self.assertIn("OFFLINE FIXTURE",proc.stdout)
if __name__=="__main__":
    unittest.main()
