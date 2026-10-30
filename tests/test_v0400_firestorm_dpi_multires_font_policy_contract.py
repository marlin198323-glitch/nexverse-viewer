"""Cross-resolution FreeType raster text contract: real UI font metrics
must not use a single 48px atlas scaled down into blurred tiny text.
Firestorm principle: UI scale distinct from font size. Browser background
and in-world geometry are unchanged by UI font changes."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def code(x):return (R/x).read_text()
class DpiFontAndUnicodeContract(unittest.TestCase):
 def test_old_48px_only_font_atlas_gone(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertNotIn("FT_Set_Pixel_Sizes(face, 0, 48)",r)
  self.assertIn("for(const auto raster_px:nexverse::ui::font_raster_tiers)",r)
 def test_freetype_raster_tiers_cover_small_and_retina(self):
  f=code("include/nexverse/ui/font_raster_policy.hpp")
  self.assertIn("12U,16U,24U,36U,56U,96U",f)
  self.assertIn("font_raster_tier(float logical_px",f)
 def test_ui_font_floor_not_dependent_on_user_gpu(self):
  f=code("include/nexverse/ui/font_raster_policy.hpp")
  self.assertIn("std::clamp(requested,10.5f,96.0f)",f)
  self.assertNotIn("RTX",f)
  self.assertNotIn("NVIDIA",f)
 def test_real_framebuffer_density_queried(self):
  a=code("src/app/application.cpp")
  self.assertIn("glfwGetFramebufferSize(window,",a)
  self.assertIn("renderer.set_framebuffer_scale(std::max(",a)
  self.assertIn("static_cast<float>(framebuffer_width)/window_width",a)
 def test_font_geometry_font_width_uses_same_raster_size(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertIn("const float logical_size=nexverse::ui::readable_ui_font_size",r)
  self.assertIn("nexverse::ui::font_raster_tier(\n        pixel_size,framebuffer_scale_)",r)
  self.assertIn("const float scale=nexverse::ui::readable_ui_font_size(pixel_size)",r)
 def test_utf8_not_bytewise_rendered(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertIn("for_each_ui_codepoint(value",r)
  self.assertNotIn("for (const char character : value)",r)
 def test_real_unicode_ranges_include_german_greek_cyrillic(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertIn("codepoint<=0x024FU",r)
  self.assertIn("codepoint=0x0370U",r)
  self.assertIn("codepoint<=0x052FU",r)
 def test_missing_glyph_visible_as_question_mark(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertIn("glyph_cache_key(raster,'?')",r)
 def test_font_gl_resources_destroyed_on_exit(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertIn("glDeleteTextures(1, &glyph.texture)",r)
  self.assertIn("glyphs_.clear()",r)
 def test_font_dpi_has_no_effect_on_hit_geometry(self):
  r=code("src/ui/ui_renderer.cpp")
  self.assertIn("void UiRenderer::set_framebuffer_scale",r)
  self.assertIn("framebuffer_scale_",r)
  self.assertNotIn("glfwSetWindowSize",r)
 def test_welcome_original_page_webkit_pump_unchanged(self):
  a=code("src/app/application.cpp")
  self.assertIn("welcome_page.resize(window_width,welcome_viewport_height)",a)
  self.assertIn("welcome_page.pump();",a)
 def test_native_tests_cover_resolutions_and_multibyte_utf8(self):
  t=code("tests/neon_login_layout_tests.cpp")
  self.assertIn("font_raster_tier(8.0f,2.0f)!=24U",t)
  self.assertIn("0x00FCU",t)
  self.assertIn("0x041AU",t)
if __name__=="__main__":unittest.main()
