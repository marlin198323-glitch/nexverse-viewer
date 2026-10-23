"""Alpha1104: audited original LAD multi-param COF Shape + self-only GPU Head/Lashes."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(rel):return (R/rel).read_text()
class CompoundCofShape(unittest.TestCase):
 def test_four_exact_firestorm_lad_ids_and_individual_bounds(self):
  h=rd("include/nexverse/assets/firestorm_cof_shape_morph_preview.hpp")
  for v in ('{770,"Elongate_Head",-1.0f,1.0f}',
            '{186,"Egg_Head",-1.3f,1.0f}',
            '{686,"Head_Eyes_Big",-2.0f,2.0f}',
            '{518,"Eyelashes_Long",-0.3f,1.5f}',
            "std::isfinite(value)", "value<param.min",
            "selection.head.emplace_back", "selection.eyelashes.emplace_back"):
   self.assertIn(v,h)
  self.assertNotIn("visual_params[",h)
 def test_compound_real_sparses_on_copy_not_faked_scale(self):
  cpp=rd("tests/firestorm_avatar_mesh_tests.cpp")
  for v in ("ORIGINAL_FIRESTORM_MULTI_COF_SHAPE_HEAD_MORPHS=",
            "selected->head.size()!=3U",
            "selected->eyelashes.size()!=4U",
            "morph_firestorm_avatar_mesh(", "compound_lash->vertices==lash->vertices",
            "firestorm_cof_shape_morph_preview({{686,2.01f}})",
            "firestorm_cof_shape_morph_preview({{186,-1.31f}})",
            "firestorm_cof_shape_morph_preview({{518,1.51f}})"):
   self.assertIn(v,cpp)
 def test_authenticated_complete_owner_shape_cof_read_and_fail_closed(self):
  app=rd("src/app/application.cpp")
  start=app.index("// An entirely resolved LLWearable snapshot supersedes")
  end=app.index("if(!gallery_asset_active_id.empty()&&",start)
  chunk=app[start:end]
  self.assertLess(chunk.index("clear_self_avatar_head_shape_preview()"),
                  chunk.index("if(gallery_asset_errors==0U"))
  for v in ("gallery_asset_errors==0U",
            "gallery_wearables_fetch_owner==",
            "active_session->agent_id",
            "firestorm_region_lease.matches(",
            "caps_state.current_outfit_folder==gallery_asset_cof",
            "gallery_decoded_wearable_assets.size()==",
            "wearable.type!=0", "multiple_shape",
            "firestorm_cof_shape_morph_preview(",
            'upload("avatar_head.llm",6U,',
            'upload("avatar_eyelashes.llm",7U,',
            "if(head_ok&&lash_ok&&upper_ok&&lower_ok)",
            "clear_self_avatar_head_shape_preview();",
            "shape_head_preview_active=true"):
   self.assertIn(v,chunk)
  self.assertNotIn("appearance->visual_params[",chunk)
 def test_self_only_second_eye_lash_slot_not_remote(self):
  gl=rd("src/render/opengl_renderer.cpp")
  for token in ("std::array<GpuMesh,10U>",
      "avatar.is_self&&", "avatar_body_meshes_[6U]",
      "avatar.is_self&&avatar_body_meshes_[7U].vao",
      "avatar_body_meshes_[7U]={};",
      "self_avatar_shape_eyelash_gl_draws=0U",
      "++diagnostics_.self_avatar_shape_eyelash_gl_draws"):
   self.assertIn(token,gl)
  app=rd("src/app/application.cpp")
  self.assertIn("Firestorm COF-Shape-Wimpern-GL:",app)
  self.assertIn("ver->second!=shape_head_preview_cof_version",app)
  self.assertIn("current_firestorm_epoch!=shape_head_preview_epoch",app)
 def test_actual_gl_framebuffer_compound_head_and_lashes(self):
  src=rd("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for token in ("ORIGINAL_FIRESTORM_COMPOUND_HEAD_PIXELS",
                "ORIGINAL_FIRESTORM_COMPOUND_LASH_PIXELS",
                "remote_stale!=0U",
                "difference(remote_baseline,remote_after)!=0U",
                "renderer->clear_self_avatar_head_shape_preview()",
                "self_avatar_shape_eyelash_gl_draws!=1U",
                "self_avatar_shape_head_gl_draws!=1U"):
   self.assertIn(token,src)
if __name__=="__main__":unittest.main()
