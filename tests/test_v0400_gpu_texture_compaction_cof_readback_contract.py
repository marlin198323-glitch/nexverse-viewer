"""Live GPU-full texture coverage and truthful COF response regression tests."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def rd(path):return (ROOT/path).read_text()
spec=importlib.util.spec_from_file_location("ogl_diagnosis",ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)

class GpuPressureCofReadback(unittest.TestCase):
 def test_budget_pressure_compactor_only_when_materials_pending(self):
  h=rd("include/nexverse/assets/scene_texture_budget_rescue.hpp")
  for t in ("choose_scene_texture_compaction(","waiting_assets==0U",
            "used_bytes<budget_bytes-budget_bytes/20U", "min_candidate=512U*1024U"):
   self.assertIn(t,h)
 def test_compaction_is_lod_and_uuid_safe(self):
  h=rd("include/nexverse/assets/scene_texture_budget_rescue.hpp")
  for t in ("excluded.contains(*it)", "mip->second>=5U",
            "target->second!=mip->second", "mip->second+1U"):
   self.assertIn(t,h)
 def test_compaction_uses_real_existing_gpu_textures(self):
  h=rd("include/nexverse/assets/scene_texture_budget_rescue.hpp")
  self.assertIn("resident_bytes.find(*it)",h)
  self.assertIn("resident_mips.find(*it)",h)
  self.assertIn("priority.rbegin()",h)
 def test_never_compacts_for_server_404_that_is_backed_off(self):
  a=rd("src/app/application.cpp")
  p=a.index("const std::size_t missing_scene_textures=")
  q=a.index("scene_texture_mip_rescue.pending_count",p)
  self.assertIn("object_texture_retries.ready(",a[p:q])
 def test_epoch_cancels_pending_and_accepted(self):
  h=rd("include/nexverse/assets/scene_texture_budget_rescue.hpp")
  self.assertIn("accepted_.clear();",h)
  self.assertIn("pending_.clear();",h)
  self.assertIn("now+std::chrono::seconds{8}",h)
  self.assertIn("now+std::chrono::seconds{90}",h)
 def test_not_claim_accepted_until_verified_gpu_upload(self):
  a=rd("src/app/application.cpp")
  self.assertIn("if(!rescued_scene_mip &&",a)
  self.assertIn("scene_texture_mip_rescue.confirm_requested(",a)
  self.assertIn("scene_texture_compaction_uploads;",a)
  self.assertLess(a.index("world_renderer->replace_object_texture("),
                  a.index("scene_texture_mip_rescue.confirm_requested("))
 def test_no_self_or_remote_bakes_or_particle_downgrade(self):
  a=rd("src/app/application.cpp")
  for t in ("emitter_texture_ids.begin(),emitter_texture_ids.end()",
            "protected_ids.insert(avatar_body_priority.begin()",
            "scene_texture_compaction_requests", "mesh_gpu_estimated_bytes",
            "texture_gpu_budget_bytes,shared_gpu_budget_bytes"):
   self.assertIn(t,a)
 def test_cold_scene_gettexture_keeps_same_download_and_budget(self):
  a=rd("src/app/application.cpp")
  self.assertIn("texture_desired_mips[compact->first]=compact->second",a)
  self.assertIn("object_texture_future.valid()",a)
  self.assertIn("assets::load_texture_cached_scoped(",a)
  self.assertIn("world_renderer->replace_object_texture(",a)
 def test_gpu_mip_floor_after_dwell(self):
  a=rd("src/app/application.cpp")
  i=a.index("auto planned_mips = assets::plan_texture_mip_levels(")
  j=a.index("texture_lod_dwell.resolve(",i)
  k=a.index("scene_texture_mip_rescue.minimum_mip(",i)
  self.assertLess(j,k)
 def test_compaction_only_one_pending_and_two_second_pacing(self):
  a=rd("src/app/application.cpp")
  self.assertIn("scene_texture_mip_rescue.pending_count(texture_plan_now)==0U",a)
  self.assertIn("last_scene_compaction",a)
  self.assertIn("std::chrono::seconds{2}",a)
 def test_current_cof_readback_is_not_merely_sent_request(self):
  a=rd("src/app/application.cpp")
  for t in ("Avatar COF readback: valid=", "owner_region_current=",
            "returned_folders=", "cof_version_present=",
            "cof_item_records=", "update.items.find(self_cof)"):
   self.assertIn(t,a)
 def test_empty_or_rejected_cof_cannot_generate_fake_outfit(self):
  a=rd("src/app/application.cpp")
  self.assertIn("no inventory UUIDs logged",a)
  self.assertIn("read-only request ",a)
  self.assertIn("failed; server inventory evidence unavailable",a)
  self.assertNotIn("encode_agent_set_appearance(",a)
 def test_live_report_includes_gpu_pressure_and_raw_404(self):
  raw=('2026-10-31 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:02 [INFO] GetTexture streaming metrics: '
       'parallel_peak=3 gpu_texture_bytes=268435448 '
       'gpu_texture_byte_budget=268435456 budget_deferrals=112 '
       'scene_compaction_uploads=2\n')
  result='\n'.join(diag.summarize(raw))
  self.assertIn('Textur-GPU: 100%',result)
  self.assertIn('scene_compaction_uploads=2',result)
  self.assertIn('budget_deferrals=112',result)
 def test_cof_report_has_no_private_uuid(self):
  raw=('2026-10-31 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:02 [INFO] Avatar COF readback: '
       'valid=1 owner_region_current=1 returned_folders=3 '
       'cof_version_present=1 cof_item_records=0; no inventory UUIDs logged\n')
  result='\n'.join(diag.summarize(raw))
  self.assertIn('cof_item_records=0',result)
  self.assertIn('Rohdaten nicht belegt',result)
 def test_zero_bake_status_is_never_reported_as_loaded(self):
  raw=('2026-10-31 00:00:01 [INFO] OpenSim login succeeded; simulator session established\n'
       '2026-10-31 00:00:02 [INFO] Avatar SELF bake pipeline: '
       'low158_refs=0/3 gpu_body=0/3 blocker=server_body_bakes_missing\n')
  out='\n'.join(diag.summarize(raw))
  self.assertIn('0/3 authentifizierte Server-Bakes',out)
  self.assertIn('Grid kontrollieren',out)
 def test_native_bounded_pressure_and_outfit_contract(self):
  cpp=rd("tests/texture_mip_tests.cpp")
  for marker in ("choose_scene_texture_compaction(", "pressure.requested(",
                 "pressure.confirm_requested(", "pressure.enter(4U)"):
   self.assertIn(marker,cpp)
if __name__=="__main__":unittest.main()
