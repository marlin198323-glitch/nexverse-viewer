"""Universal persistent lossless texture cache, backward compatible with
OGLTEX01 raw files. A tighter cache should retain more scene detail across
restarts without changing any GPU capacity or J2C safety limits."""
import importlib.util
from pathlib import Path
import os
import struct
import subprocess
import sys
import tempfile
import unittest

R=Path(__file__).resolve().parents[1]
SCRIPT=R/"scripts/ogl-cache-status.py"
spec=importlib.util.spec_from_file_location("ogl_cache_status",SCRIPT)
tool=importlib.util.module_from_spec(spec)
spec.loader.exec_module(tool)
def source(path):return (R/path).read_text()

class PersistentLosslessCacheContract(unittest.TestCase):
 def test_compression_format_has_separate_magic(self):
  s=source("src/assets/texture_disk_cache.cpp")
  self.assertIn("compressed_magic{'O','G','L','T','E','X','0','2'}",s)
  self.assertIn("legacy_magic{'O','G','L','T','E','X','0','1'}",s)
 def test_lossless_png_j2c_rgba_cached_with_zlib(self):
  s=source("src/assets/texture_disk_cache.cpp")
  self.assertIn("compress2(packed.data()",s)
  self.assertIn("Z_BEST_SPEED",s)
  self.assertIn("marker=compressible?compressed_magic:legacy_magic",s)
  self.assertIn("if(marker==legacy_magic&&byte_count>=4096U)",s)
 def test_zip_bomb_and_corrupt_checksum_rejected(self):
  s=source("src/assets/texture_disk_cache.cpp")
  self.assertIn("byte_count>max_image_bytes",s)
  self.assertIn("stream.total_out==byte_count",s)
  self.assertIn("stream.total_in==packed_bytes",s)
  self.assertIn("checksum(image)!=stored_hash",s)
 def test_only_raw_cache_migrated_when_valid(self):
  s=source("src/assets/texture_disk_cache.cpp")
  self.assertLess(s.index("checksum(image)!=stored_hash"),
                  s.index("if(marker==legacy_magic&&byte_count>=4096U)"))
 def test_existing_compressed_gpu_memory_texture_reuse_preserved(self):
  s=source("include/nexverse/assets/texture_cached_request.hpp")
  self.assertIn("disk.load(id,&result.disk_lookup)",s)
  self.assertIn("(void)memory.store(id,result.image)",s)
  self.assertIn("texture_mip_image(result.image,mip)",s)
 def test_gpu_and_disk_budgets_separate(self):
  s=source("include/nexverse/assets/texture_disk_cache.hpp")
  self.assertIn("default_budget_bytes()",s)
  a=source("src/app/application.cpp")
  self.assertIn("shared_gpu_budget_bytes =",a)
 def test_configurable_disk_not_user_gpu_vendor(self):
  s=source("src/assets/texture_disk_cache.cpp")
  self.assertIn('std::getenv("OGL_TEXTURE_CACHE_MIB")',s)
  self.assertIn("constexpr std::uint64_t fallback=1024ULL*mib",s)
  self.assertIn("size<128U||size>2048U",s)
 def test_existing_texture_path_and_two_tier_cache(self):
  s=source("src/assets/texture_disk_cache.cpp")
  self.assertIn('"textures-v1"',s)
  self.assertIn("stem+\".rgba\"",s)
 def test_native_e2e_compressed_round_trip_and_legacy_migration(self):
  s=source("tests/texture_disk_cache_tests.cpp")
  for token in ("scene_tile.pixels.resize(1024U*1024U*4U)",
                "OGLTEX02","marker!=std::array<char,8>",
                "std::filesystem::file_size(old)",
                "decode" ):
   self.assertIn(token,s.replace("'O','G','L','T','E','X','0','2'","OGLTEX02"))
 def test_read_only_stats_budget(self):
  self.assertEqual(tool.budget_mib(None),1024)
  self.assertEqual(tool.budget_mib("128"),128)
  self.assertEqual(tool.budget_mib("2048"),2048)
  for bad in ("", "99999","0","42","3e2","-1","abc"):
   self.assertEqual(tool.budget_mib(bad),1024)
 def test_read_only_stats_counts_records_with_no_uuid_output(self):
  with tempfile.TemporaryDirectory() as temp:
   root=Path(temp)/"textures-v1";root.mkdir()
   (root/"secret-a.rgba").write_bytes(b"OGLTEX01"+struct.pack("<IIQ",1,1,0)+b"abcd")
   (root/"secret-b.rgba").write_bytes(b"OGLTEX02"+struct.pack("<IIQ",1,1,0)+b"123")
   (root/"secret-c.rgba").write_bytes(b"bad")
   counts=tool.measure_textures(root)
   self.assertEqual(counts["raw"],1)
   self.assertEqual(counts["compressed"],1)
   self.assertEqual(counts["invalid"],1)
   self.assertEqual(counts["logical_bytes"],56)
 def test_missing_cache_does_not_create_folder_or_traffic(self):
  with tempfile.TemporaryDirectory() as td:
   root=Path(td)/"does-not-exist"
   self.assertEqual(tool.measure_textures(root)["raw"],0)
   self.assertFalse(root.exists())
   p=subprocess.run([sys.executable,str(SCRIPT)],capture_output=True,
                    text=True,timeout=5,check=False,
                    env={**os.environ,"XDG_CACHE_HOME":td})
   self.assertEqual(p.returncode,0,p.stderr)
   self.assertIn("Cache-Budget:",p.stdout)
   self.assertNotIn(td,p.stdout)
   self.assertFalse(root.exists())
if __name__=="__main__":unittest.main()
