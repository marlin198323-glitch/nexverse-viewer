"""A v1.0.0.1 hotfix cannot be released without the actual stable v1.0.0.0."""
from __future__ import annotations
import importlib.util
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location("ogl_version_policy", ROOT/"scripts/ogl-version.py")
policy=importlib.util.module_from_spec(spec)
spec.loader.exec_module(policy)

class HotfixPrerequisiteGateTests(unittest.TestCase):
    def git(self,*args,repo=None):
        return subprocess.run(["git","-C",str(repo or self.repo),*args],
          capture_output=True,text=True,check=True).stdout.strip()

    def setUp(self):
        self.tmp=tempfile.TemporaryDirectory(prefix="ogl-hotfix-prereq-")
        self.addCleanup(self.tmp.cleanup)
        self.repo=Path(self.tmp.name)/"work"
        self.remote=Path(self.tmp.name)/"origin.git"
        self.repo.mkdir()
        self.git("init","-q")
        self.git("init","--bare","-q",str(self.remote),repo=self.repo)
        self.git("remote","add","origin",str(self.remote))
        self.git("checkout","-qb","main")
        (self.repo/"VERSION").write_text("v0.9.9.0\n")
        self.git("add","VERSION")
        self.git("-c","user.name=Viewer CI","-c","user.email=ci@example.invalid",
                 "commit","-qm","pre stable")
        self.original=self.git("rev-parse","HEAD")
        (self.repo/"VERSION").write_text("v1.0.0.0\n")
        self.git("add","VERSION")
        self.git("-c","user.name=Viewer CI","-c","user.email=ci@example.invalid",
                 "commit","-qm","stable")
        self.stable_sha=self.git("rev-parse","HEAD")
        self.git("-c","user.name=Viewer CI","-c","user.email=ci@example.invalid",
                 "tag","-a","v1.0.0.0","-m","original stable release")
        self.git("push","-q","origin","main","refs/tags/v1.0.0.0")
        self.git("fetch","-q","origin")
        self.record={"published_releases":{"v1.0.0.0":{
            "published":True,
            "release_commit_sha":self.stable_sha,
            "release_url":"https://github.com/marlin198323-glitch/nexverse-viewer/releases/tag/v1.0.0.0"
        }}}

    def checked(self,receipt=None):
        return policy.stable_predecessor_released(
            "v1.0.0.1",self.record if receipt is None else receipt,self.repo)

    def test_verified_previous_release_accepted_locally(self):
        self.assertTrue(self.checked()[0])
    def test_no_stable_tag_fails_closed(self):
        self.git("tag","-d","v1.0.0.0")
        self.assertFalse(self.checked()[0])
    def test_lightweight_tag_is_not_release_proof(self):
        self.git("tag","-d","v1.0.0.0")
        self.git("tag","v1.0.0.0")
        self.assertFalse(self.checked()[0])
    def test_stable_commit_must_be_ancestor_of_origin_main(self):
        self.git("update-ref","refs/remotes/origin/main",self.original)
        self.assertFalse(self.checked()[0])
    def test_stable_tag_must_describe_actual_version(self):
        self.git("tag","-d","v1.0.0.0")
        self.git("-c","user.name=Viewer CI","-c","user.email=ci@example.invalid",
                 "tag","-a","v1.0.0.0",self.original,"-m","misnamed version")
        self.assertFalse(self.checked()[0])
    def test_published_receipt_required(self):
        self.assertFalse(self.checked({})[0])
    def test_wrong_receipt_sha_fails(self):
        data={"published_releases":{"v1.0.0.0":{
          **self.record["published_releases"]["v1.0.0.0"],
          "release_commit_sha":self.original}}}
        self.assertFalse(self.checked(data)[0])
    def test_wrong_release_url_fails(self):
        data={"published_releases":{"v1.0.0.0":{
          **self.record["published_releases"]["v1.0.0.0"],
          "release_url":"https://example.invalid/releases/v1.0.0.0"}}}
        self.assertFalse(self.checked(data)[0])
    def test_missing_remote_tag_fails_even_if_local_tag_exists(self):
        self.git("push","-q","origin",":refs/tags/v1.0.0.0")
        self.assertFalse(self.checked()[0])
    def test_only_first_hotfix_needs_v1_release_specific_gate(self):
        self.assertTrue(policy.stable_predecessor_released(
          "v1.0.0.0",{},self.repo)[0])
    def test_live_repo_guard_is_blocked_without_stable_tag(self):
        result=subprocess.run(["python3",str(ROOT/"scripts/ogl-version.py"),
             "--release-check","v1.0.0.1"],capture_output=True,text=True)
        self.assertEqual(result.returncode,2)
        self.assertIn("predecessor tag v1.0.0.0",result.stdout)

if __name__=="__main__":
    unittest.main()
