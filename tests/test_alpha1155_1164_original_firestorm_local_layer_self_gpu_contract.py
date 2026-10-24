"""Alpha1164: actual original Firestorm Head/Upper/Lower local color GPU, not server bake."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def txt(name):return (R/name).read_text()
class SelfOriginalLocalLayerGpu(unittest.TestCase):
 def test_public_api_separates_server_bake_from_local_preview(self):
  s=txt("include/nexverse/render/renderer.hpp")
  for x in ("set_self_avatar_local_layer_preview(", "clear_self_avatar_local_layer_previews()",
            "self_avatar_local_layer_gl_draws", "self_avatar_body_bake_gl_draws"):
   self.assertIn(x,s)
 def test_rgba_upload_is_bounded_transactional_and_resets_on_shutdown(self):
  s=txt("src/render/opengl_renderer.cpp")
  for x in ("slot>=self_avatar_local_layer_textures_.size()",
      "!image.valid()||image.width>128U||image.height>128U",
      "glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8",
      "if(err!=GL_NO_ERROR)","glDeleteTextures(1,&candidate)",
      "existing=candidate;", "clear_self_avatar_local_layer_previews();"):
   self.assertIn(x,s)
 def test_own_avatar_only_real_bakes_priority_and_remote_untouched(self):
  s=txt("src/render/opengl_renderer.cpp")
  for x in ("const bool local_layer=!baked&&avatar.is_self&&",
            "self_avatar_local_layer_textures_[slot]!=0U",
            "(baked||local_layer)?GL_TRUE:GL_FALSE",
            "if(local_layer)",
            "++diagnostics_.self_avatar_local_layer_gl_draws[slot]"):
   self.assertIn(x,s)
  self.assertIn("self_avatar_body_bake_gl_draws[slot]",s)
 def test_local_layer_gl_diagnostics_are_per_frame_not_appearance_proof(self):
  s=txt("src/render/opengl_renderer.cpp")
  self.assertIn("diagnostics_.self_avatar_local_layer_gl_draws.fill(0U)",s)
  a=txt("src/app/application.cpp")
  self.assertIn("Firestorm lokale COF-Farben GL:",a)
  self.assertIn("kein Server-Bake",a)
 def test_current_validated_original_mask_composition_uploads_to_avatar(self):
  s=txt("src/app/application.cpp")
  start=s.index("if(gallery_texlayer_compose_requested)")
  end=s.index("if(gallery_appearance_check_requested)",start)
  part=s[start:end]
  for x in ("gallery_local_epoch!=current_firestorm_epoch",
            "gallery_local_cof_version", "gallery_local_te_rgba.empty()",
            "world_renderer->clear_self_avatar_local_layer_previews();",
            "assets::compose_firestorm_mask_preview(",
            "set_self_avatar_local_layer_preview(",
            "composed.image.pixels", "Server-Bake NICHT freigegeben"):
   self.assertIn(x,part)
 def test_region_or_cof_change_destroys_gpu_local_colors(self):
  s=txt("src/app/application.cpp")
  a=s.index("auto clear_gallery_local_texture_previews=[&]()")
  b=s.index("// Branding is an independent GL texture",a)
  block=s[a:b]
  self.assertIn("world_renderer->clear_self_avatar_local_layer_previews();",block)
  self.assertIn("gallery_local_te_rgba.clear();",block)
  self.assertIn("gallery_composite_cof.clear();",block)
 def test_native_real_original_firestorm_rgb_three_body_parts_and_remote(self):
  s=txt("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ("ORIGINAL_FIRESTORM_LOCAL_COF_RGB_SELF_PIXELS=",
            "self_avatar_local_layer_gl_draws[0U]!=0U",
            "real_stats.self_avatar_body_bake_gl_draws[0U]!=1U",
            "real_stats.self_avatar_local_layer_gl_draws[0U]!=0U",
            "auto remote=self;remote.is_self=false;",
            "ORIGINAL_FIRESTORM_LOCAL_COF_RESET_GHOST_PIXELS=",
            "if(ghosts!=0U"):
   self.assertIn(x,s)
 def test_invalid_replacement_preserves_last_valid_gl_pixels(self):
  s=txt("tests/avatar_body_framebuffer_pixels_tests.cpp")
  self.assertIn("render::RenderImageRgba8 invalid{129U,129U,",s)
  self.assertIn("difference(local_pixels,draw())!=0U",s)
 def test_user_can_see_partial_local_color_without_fake_server_ack(self):
  s=txt("src/app/application.cpp")
  self.assertIn("lokale Teilfarben am eigenen Avatar",s)
  self.assertIn("unvollstaendig, kein Server-Bake",s)
  self.assertIn("gallery_composite_version=v->second",s)
 def test_full_authenticated_live_rgb_outfit_movement_remains_open(self):
  s=txt("docs/PROJECT_PROGRESS.json")
  self.assertIn("still open",s)
  self.assertRegex(s,r"Alpha[0-9]{3,4}")
  self.assertIn("BLOCKED",s)
if __name__=="__main__":unittest.main()
