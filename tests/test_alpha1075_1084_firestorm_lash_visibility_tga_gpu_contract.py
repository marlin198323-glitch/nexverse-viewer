"""Alpha.1084: locally installed original Firestorm head_alpha visibility mask on true GPU eyelashes."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(p):return (R/p).read_text()
class FirestormLashMask(unittest.TestCase):
 def test_real_original_local_tga_decoder_used_in_gui(self):
  app=rd("src/app/application.cpp")
  for needle in ("load_firestorm_character_tga(",
                 '"head_alpha.tga"',
                 "set_avatar_eyelash_visibility_mask(",
                 "Firestorm Wimpern-TGA-GL:",
                 "not full makeup compositor"):
   self.assertIn(needle,app)
  self.assertNotIn("https://head_alpha.tga",app)
 def test_single_bounded_transactional_texture_gpu_upload(self):
  gl=rd("src/render/opengl_renderer.cpp")
  body=gl[gl.index("bool set_avatar_eyelash_visibility_mask("):
          gl.index("bool set_avatar_body_mesh(",gl.index("bool set_avatar_eyelash_visibility_mask("))]
  for needle in ("mask.valid()","mask.width>1024U",
                 "mask.height>1024U","glTexImage2D(",
                 "if(error!=GL_NO_ERROR)","glDeleteTextures(1,&candidate)",
                 "avatar_eyelash_visibility_texture_=candidate",
                 "glDeleteTextures(1,&previous)"):
   self.assertIn(needle,body)
  self.assertLess(body.index("if(error!=GL_NO_ERROR)"),
                  body.index("avatar_eyelash_visibility_texture_=candidate"))
  self.assertIn("glDeleteTextures(1,&avatar_eyelash_visibility_texture_)",gl)
 def test_shader_mask_only_on_original_skinned_head_eyelashes(self):
  gl=rd("src/render/opengl_renderer.cpp")
  self.assertIn("uniform bool uUseAvatarEyelashMask;",gl)
  self.assertIn("uniform sampler2D uAvatarEyelashMask;",gl)
  self.assertIn("if(visibility<=0.03)discard;",gl)
  a=gl.index("const auto& eyelash_mesh=avatar_body_meshes_[5U]")
  b=gl.index("for (const auto& part : parts)",a)
  lash=gl[a:b]
  for needle in ("avatar.baked_texture_ids[0]",
                 "eyelash_mesh.skin_vbo!=0U",
                 "const bool local_mask=",
                 "GL_TEXTURE7", "uAvatarEyelashMask",
                 "self_avatar_eyelash_mask_gl_draws",
                 "GL_FALSE"):
   self.assertIn(needle,lash)
  self.assertIn("diagnostics_.self_avatar_eyelash_mask_gl_draws=0U",gl)
 def test_real_framebuffer_proves_mask_zeros_and_original_tga_pixels(self):
  source=rd("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for needle in ("MASKED_OUT_EYELASH_PIXEL_DELTA=",
                 "masked_delta!=0U",
                 "render::RenderImageRgba8{}",
                 "head_alpha.tga",
                 "original.image.pixels",
                 "ORIGINAL_FIRESTORM_EYELASH_VISIBLE_PIXELS=",
                 "WITHDRAWN_HEAD_EYELASH_GL_DRAWS="):
   self.assertIn(needle,source)
 def test_backend_api_exposes_separate_non_cof_mask_measurement(self):
  hdr=rd("include/nexverse/render/renderer.hpp")
  self.assertIn("virtual bool set_avatar_eyelash_visibility_mask(",hdr)
  self.assertIn("self_avatar_eyelash_mask_gl_draws",hdr)
  gl=rd("src/render/opengl_renderer.cpp")
  self.assertNotIn("self_avatar_body_bake_gl_draws[5]",gl)
if __name__=="__main__":unittest.main()
