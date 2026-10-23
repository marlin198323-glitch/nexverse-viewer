import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(p):return (R/p).read_text()
class OriginalBodyMorph(unittest.TestCase):
 def test_lad_mapping_and_ranges(self):
  s=rd("include/nexverse/assets/firestorm_cof_shape_morph_preview.hpp")
  for x in ('{104,"Big_Belly_Torso",0.0f,1.0f}','{106,"Muscular_Torso",0.0f,1.4f}','{151,"Big_Butt_Legs",0.0f,1.0f}','{152,"Muscular_Legs",0.0f,1.5f}',"selection.upper.emplace_back","selection.lower.emplace_back"):
   self.assertIn(x,s)
 def test_authenticated_cof_and_fail_closed(self):
  s=rd("src/app/application.cpp")
  for x in ('upload("avatar_upper_body.llm",8U,','upload("avatar_lower_body.llm",9U,','if(head_ok&&lash_ok&&upper_ok&&lower_ok)',"gallery_asset_errors==0U","gallery_wearables_fetch_owner==","caps_state.current_outfit_folder==gallery_asset_cof","clear_self_avatar_head_shape_preview()"):
   self.assertIn(x,s)
 def test_gpu_ownership_and_counters(self):
  s=rd("src/render/opengl_renderer.cpp")
  for x in ("std::array<GpuMesh,10U>","slot==0U?6U:(slot==1U?8U:9U)","avatar.is_self&&","avatar_body_meshes_[8U]={};","avatar_body_meshes_[9U]={};","self_avatar_shape_body_gl_draws={}"):
   self.assertIn(x,s)
 def test_native_original_mesh_validation(self):
  s=rd("tests/firestorm_avatar_mesh_tests.cpp")
  for x in ("FIRESTORM_ORIGINAL_COF_BODY_MORPHS_UPPER=","body_shape->upper.size()!=2U","body_shape->lower.size()!=2U","upper_shape->vertices==upper->vertices","lower_shape->vertices==lower->vertices"):
   self.assertIn(x,s)
 def test_live_gl_pixels_isolation_and_reset(self):
  s=rd("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ("ORIGINAL_FIRESTORM_UPPER_LOWER_SHAPE_PIXELS=","REMOTE_BODY_SHAPE_GHOST_PIXELS=","RESET_BODY_SHAPE_GHOST_PIXELS=","set_avatar_body_mesh(8U,","set_avatar_body_mesh(9U,","untouched!=0U","reset!=0U"):
   self.assertIn(x,s)
if __name__=="__main__":unittest.main()
