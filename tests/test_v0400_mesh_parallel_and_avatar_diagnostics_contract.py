"""Mesh streaming concurrency, caches and truthful grey-avatar diagnosis."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(n):return (ROOT/n).read_text()
spec=importlib.util.spec_from_file_location(
    "ogl_asset_diagnose",ROOT/"scripts/ogl-asset-diagnose.py")
module=importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
class MeshParallelContracts(unittest.TestCase):
 def test_three_concurrent_mesh_slots_with_per_job_provenance(self):
  a=source("src/app/application.cpp")
  for token in ("max_parallel_mesh_downloads=3U",
                "mesh_in_flight.size()<max_parallel_mesh_downloads",
                "std::vector<MeshInFlight> mesh_in_flight;",
                "mesh_asset_loading_id=std::move(ready->id);",
                "mesh_asset_loading_lod=ready->lod;",
                "mesh_asset_request_epoch=ready->epoch;",
                "mesh_request_started=ready->started;"):
   self.assertIn(token,a)
 def test_every_network_task_is_deduped_and_scoped(self):
  a=source("src/app/application.cpp")
  for t in ("pending_mesh_ids.insert(task.id);",
            "mesh_residency.prioritized_ids,pending_mesh_ids",
            "live_epoch->load(","shutdown_wait(task.future,",
            "load_mesh_cached(id,"):self.assertIn(t,a)
 def test_cached_first_but_network_fair(self):
  a=source("src/app/application.cpp")
  self.assertIn("mesh_request_fairness.dispatched_count()%3U!=2U",a)
  self.assertIn("mesh_memory_cache->contains(id,desired->second)",a)
  self.assertIn("disk.contains(id,desired->second)",a)
  self.assertIn("if(!request_choice)",a)
 def test_serial_gpu_budget_remains(self):
  a=source("src/app/application.cpp")
  for t in ("asset_gpu_upload_gate.ready(", "can_replace_gpu_bytes(",
            "effective_gpu_family_budget(", "world_renderer->upload_mesh_asset("):
   self.assertIn(t,a)
 def test_concurrent_atomic_lod_cache(self):
  s=source("src/assets/mesh_cache.cpp")
  for t in ("temp_sequence.fetch_add(", '".part."',
            "std::filesystem::rename(temp,dest)", "checksum(id, lod, bytes)"):
   self.assertIn(t,s)
 def test_mesh_disk_config_budget_and_limits(self):
  s=source("src/assets/mesh_cache.cpp")
  self.assertIn('std::getenv("OGL_MESH_CACHE_MIB")',s)
  self.assertIn("fallback=1024ULL*mib",s)
  self.assertIn("size<128U||size>4096U",s)
  self.assertIn("std::filesystem::symlink_status(path,error)",s)
 def test_ram_cache_bounded_and_threadsafe(self):
  h=source("include/nexverse/assets/mesh_cache.hpp")
  s=source("src/assets/mesh_cache.cpp")
  self.assertIn("96U * 1024U * 1024U",h)
  self.assertIn("bool MeshMemoryCache::contains(",s)
  self.assertIn("std::lock_guard guard{mutex_};",s)
 def test_cache_status_matches_binary_and_excludes_parts(self):
  s=source("scripts/ogl-cache-status.py")
  self.assertIn("DEFAULT_BUDGET_MIB = 1024",s)
  self.assertIn("DEFAULT_MESH_BUDGET_MIB = 1024",s)
  self.assertIn('entry.suffix == ".mesh"',s)
 def test_native_parallel_cache_regression_exists(self):
  s=source("tests/mesh_cache_tests.cpp")
  self.assertIn("for(int i=0;i<12;++i)",s)
  self.assertIn("std::async(std::launch::async",s)
  self.assertIn("reopened.load(key)",s)
 def test_missing_body_bakes_not_reported_as_complete(self):
  a=source("src/app/application.cpp")
  self.assertIn("self_body_ref_count==0",a)
  self.assertIn("self_body_ref_count<3",a)
  self.assertIn("Avatar grau: 0/3 Server-Koerpertexturen",a)
 def test_read_only_report_never_prints_asset_ids(self):
  raw=('2026-10-30 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-30 00:00:02 [INFO] Avatar SELF bake pipeline: '
       'blocker=server_body_bakes_missing; low158_refs=0/3; gpu_body=0/3\n'
       '2026-10-30 00:00:03 [WARN] GetMesh failed for '
       'ffffffff-ffff-ffff-ffff-ffffffffffff: HTTP server returned status 404\n')
  output="\n".join(module.summarize(raw))
  self.assertIn("0/3",output)
  self.assertIn("Mesh=1",output)
  self.assertNotIn("ffffffff-ffff",output)
 def test_older_log_and_missing_session_are_distinct(self):
  self.assertIn("Keine erfolgreiche",module.summarize("garbage")[0])
  raw=('2026-10-30 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-30 00:00:02 [INFO] GetMesh streaming metrics: parallel_peak=3 disk_hits=12\n')
  self.assertIn("parallel_peak=3","\n".join(module.summarize(raw)))
if __name__=="__main__":unittest.main()
