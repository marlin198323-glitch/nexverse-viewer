"""v0.4.0.0: previous-frame GPU depth proof must match the active self bake."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(path):return (ROOT/path).read_text()
class SelfBakeGpuEpochContract(unittest.TestCase):
 def test_bake_source_identity_is_region_local_and_not_logged(self):
  hdr=source("include/nexverse/render/self_avatar_visibility_source_gate.hpp")
  self.assertIn("SelfAvatarVisibilitySourceGate",hdr)
  self.assertIn("using BakeIds=std::array<std::string,3U>",hdr)
  self.assertIn("if(previous_==source)return false;",hdr)
  self.assertIn("previous_.reset();",hdr)
  self.assertNotIn("core::info(",hdr)
  self.assertNotIn("logger",hdr)
 def test_actual_renderer_resets_on_self_bake_source_change(self):
  render=source("src/render/opengl_renderer.cpp")
  self.assertIn("self_avatar_visibility_sources_.observe(current_self_bakes)",render)
  self.assertIn("current_self_bakes=avatar.baked_texture_ids;",render)
  self.assertIn("reset_self_avatar_visibility();",render)
  self.assertIn("self_avatar_visibility_query_epoch_[i]!=",render)
 def test_only_successful_same_id_gpu_texture_update_resets(self):
  render=source("src/render/opengl_renderer.cpp")
  start=render.index("object_texture_alpha_[texture_id]=texture_has_alpha(image);")
  finish=render.index("return {true,GpuUploadFailure::none,0U};",start)
  block=render[start:finish]
  self.assertIn("avatar.is_self&&",block)
  self.assertIn("avatar.baked_texture_ids.end(),texture_id",block)
  self.assertIn("reset_self_avatar_visibility();",block)
  self.assertNotIn("glDeleteTextures(1,&candidate);",block)
 def test_removing_active_texture_resets_old_alpha_proof(self):
  render=source("src/render/opengl_renderer.cpp")
  remove=render[render.index("void remove_object_texture("):render.index("avatar_bake_preview_handle(")]
  self.assertIn("avatar.baked_texture_ids.end(),id",remove)
  self.assertIn("reset_self_avatar_visibility();",remove)
  clear=render[render.index("void clear_object_textures()"):render.index("bool set_object_texture(")]
  self.assertIn("reset_self_avatar_visibility();",clear)
 def test_real_gl_mocks_source_swap_same_id_and_unrelated_asset(self):
  native=source("tests/renderer_region_transition_tests.cpp")
  for needle in ("changed_self.baked_texture_ids={",
                 "new-upper",
                 "after_rebake.self_avatar_visible_samples_known",
                 'renderer->set_object_texture(',
                 '"unrelated-asset",opaque',
                 "error=48", "error=52", "error=54"):
   self.assertIn(needle,native)
 def test_no_false_claim_of_server_identity_or_skin_pixels(self):
  hdr=source("include/nexverse/render/self_avatar_visibility_source_gate.hpp")
  app=source("src/app/application.cpp")
  self.assertNotIn("server_confirmed",hdr)
  self.assertIn("avatar_proof.current_body_bake_sources_drawn[slot]",app)
  self.assertIn("AvatarBakeProgressTrace",app)
if __name__=="__main__":unittest.main()
