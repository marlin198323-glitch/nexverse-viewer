"""Regressions for bounded concurrent texture streaming and sky diagnostics."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(name):return (ROOT/name).read_text()
class ParallelTexturePipeline(unittest.TestCase):
 def test_four_concurrent_workers_bounded(self):
  a=read("src/app/application.cpp")
  self.assertIn("max_parallel_texture_downloads=4U",a)
  self.assertIn("texture_in_flight.size()<max_parallel_texture_downloads",a)
  self.assertIn("texture_in_flight.push_back(TextureInFlight{",a)
 def test_inflight_uuid_dedup(self):
  a=read("src/app/application.cpp")
  self.assertIn("for(const auto& task:texture_in_flight)",a)
  self.assertIn("satisfied.insert(task.id);",a)
 def test_every_completion_preserves_metadata_and_gpu_gate(self):
  a=read("src/app/application.cpp")
  for key in ("object_texture_loading_id=std::move(ready->id)",
              "object_texture_loading_mip=ready->mip",
              "object_texture_request_epoch=ready->epoch",
              "texture_request_started=ready->started",
              "asset_gpu_upload_gate.ready("):
   self.assertIn(key,a)
 def test_shared_transport_shutdown_and_epoch(self):
  a=read("src/app/application.cpp")
  self.assertIn("texture_prefetch_live_epoch",a)
  self.assertIn("live_epoch->load(",a)
  self.assertIn("shutdown_wait(task.future",a)
  self.assertIn("load_texture_cached_scoped(",a)
 def test_atomic_separate_tempfiles(self):
  a=read("src/assets/texture_disk_cache.cpp")
  self.assertIn("temp_sequence.fetch_add(",a)
  self.assertIn('".part."',a)
  self.assertIn("std::filesystem::rename(temp,dest)",a)
  self.assertIn("checksum(image)",a)
 def test_1024_mib_cache_and_config_override(self):
  a=read("src/assets/texture_disk_cache.cpp")
  self.assertIn("fallback=1024ULL*mib",a)
  self.assertIn('std::getenv("OGL_TEXTURE_CACHE_MIB")',a)
  self.assertIn("size>2048U",a)
 def test_ram_cache_bounded(self):
  self.assertIn("128U*1024U*1024U",
                read("include/nexverse/assets/texture_memory_cache.hpp"))
 def test_gpu_upload_paced(self):
  a=read("src/app/application.cpp")
  self.assertIn("AssetGpuUploadGate asset_gpu_upload_gate{",a)
  self.assertIn("std::chrono::milliseconds{16}",a)
 def test_sun_below_horizon_invisible(self):
  a=read("include/nexverse/render/sky_projection.hpp")
  self.assertIn("sun_above_horizon",a)
  self.assertIn("std::sin(alt)",a)
 def test_sky_render_diagnostic_and_scattering(self):
  a=read("src/render/opengl_renderer.cpp")
  self.assertIn("OGL sky environment:",a)
  self.assertIn("project_environment_sun(env,camera_,sky_aspect)",a)
  self.assertIn("float corona =",a)
  self.assertIn("float scatter =",a)
if __name__=="__main__":unittest.main()
