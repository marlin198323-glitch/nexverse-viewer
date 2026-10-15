"""Offline tests for the OpenGenesisLINK upstream gate."""
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parent.parent
SCRIPT=ROOT/"scripts/check_opengenesislink_server.py"
SPEC=importlib.util.spec_from_file_location("server_sync",SCRIPT)
MOD=importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MOD)
class ServerSyncTests(unittest.TestCase):
    def test_classification(self):
        cases={
            "doc/NexVerse/CROSS_REGION_LSL_CHAT_09312.md":"SERVER_ONLY_LSL_CHAT",
            "doc/NexVerse/OGL_REGION_DIMENSIONS_LSL.md":"VIEWER_CORE_LSL_EDITOR_FUTURE",
            "doc/OpenGenesisLINK/LSL_TOUCH_HOLD_COMPAT.md":"VIEWER_CORE_GRAB_COMPAT",
            "OpenSim/Region/Framework/Scenes/Scene.PacketHandlers.cs":"VIEWER_CORE_PROTOCOL_REVIEW",
            "NexVerse/RegionModules/NodeAgent/NexVerseNodeAgentModule.cs":"SERVER_ONLY_NODE_BUS",
            "OpenSim/Region/ScriptEngine/Shared/Api/Implementation/LSL_Api.cs":"SERVER_ONLY_LSL_RUNTIME",
            "OpenSim/Services/Connectors/Inventory/XInventoryServicesConnector.cs":
                "VIEWER_CORE_INVENTORY_THUMBNAIL",
            "doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md":
                "VIEWER_CORE_INVENTORY_THUMBNAIL",
            "tools/diagnostics/ogl_cross_region_chat_smoke.lsl":
                "SERVER_ONLY_LSL_CHAT_TEST",
            "NexVerse/Server/Api/OglApiDocsAccess.cs":
                "SERVER_ONLY_OPERATOR_ADMIN_UI",
            "doc/OpenGenesisLINK/WORLD_API_CONTROL_CENTER.md":
                "SERVER_ONLY_OPERATOR_ADMIN_UI",
            "OpenSim/Region/CoreModules/Framework/EntityTransfer/EntityTransferModule.cs":
                "VIEWER_CORE_CROSS_REGION_FLIGHT_COMPAT",
            "OpenSim/Framework/AssetUploadLevelPolicy.cs":
                "SERVER_ONLY_ASSET_UPLOAD_PERMISSION",
            "OpenSim/Server/Handlers/Inventory/XInventoryInConnector.cs":
                "SERVER_ONLY_HG_INVENTORY_AUTHORIZATION",
            "NexVerse/Server/Api/NexStatisticsApi.cs":"SERVER_ONLY_WORLDAPI",
            "bin/opt/cache/fsassets/123.gz":"SERVER_ONLY_ASSET_CACHE_FILES",
            "bin/opt/cache/assetcache/001/uuid":"SERVER_ONLY_ASSET_CACHE_FILES",
            "bin/nexverse-deutschland/terrain/Test.r32":
                "SERVER_ONLY_TERRAIN_HEIGHTMAP_DATA",
            "bin/nexverse-deutschland/regionen.csv":
                "SERVER_ONLY_TERRAIN_IMPORT_DATA",
            ".gitignore":"REPOSITORY_METADATA",
            "unknown/NewFeature.cs":"MANUAL_REVIEW"}
        for name,expected in cases.items():
            with self.subTest(name=name): self.assertEqual(MOD.classify(name),expected)
    def test_changed_shas_require_manual_review(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            (root/"baseline.json").write_text(json.dumps({"reviewed_sha":"a"*40}))
            (root/"diff.json").write_text(json.dumps({
                "head":"b"*40,"ahead_by":2,"status":"ahead",
                "files":[{"filename":"doc/OpenGenesisLINK/LSL_TOUCH_HOLD_COMPAT.md"},
                         {"filename":"OpenSim/Region/Framework/Scenes/Scene.PacketHandlers.cs"}]}))
            result=subprocess.run([sys.executable,str(SCRIPT),
                "--baseline",str(root/"baseline.json"),"--fixture",str(root/"diff.json"),
                "--strict"],capture_output=True,text=True,check=False,timeout=5)
            self.assertEqual(result.returncode,2,result.stderr)
            self.assertIn("OFFLINE FIXTURE",result.stdout)
            self.assertIn("VIEWER_CORE_GRAB_COMPAT",result.stdout)
            self.assertIn("VIEWER_CORE_PROTOCOL_REVIEW",result.stdout)
    def test_github_compare_truncation_fails_closed(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            (root/"baseline.json").write_text(json.dumps({"reviewed_sha":"a"*40}))
            (root/"diff.json").write_text(json.dumps({
                "head":"b"*40,"ahead_by":22,"files":[
                    {"filename":f"bin/cache/{i}.jpg"} for i in range(300)]}))
            r=subprocess.run([sys.executable,str(SCRIPT),
                "--baseline",str(root/"baseline.json"),
                "--fixture",str(root/"diff.json"),"--strict"],
                capture_output=True,text=True,check=False,timeout=5)
            self.assertEqual(r.returncode,2,r.stderr)
            self.assertIn("TRUNCATED COMPARE",r.stdout)
if __name__=="__main__":unittest.main()
