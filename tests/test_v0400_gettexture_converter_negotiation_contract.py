"""v0.4.0.0: region-scoped native JPEG2000 converter negotiation and
same-asset fallbacks reduce real network traffic, without inventing skin."""
from pathlib import Path
import unittest

R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()

class TextureFormatNegotiationContract(unittest.TestCase):
 def test_three_confirmed_native_fallbacks_before_skipping_png(self):
  hdr=read("include/nexverse/assets/texture_format_preference.hpp")
  self.assertIn("native_streak_<3U",hdr)
  self.assertIn("native_succeeded(",hdr)
  self.assertIn("converter_failed",hdr)
  self.assertIn("skips_%16U==0U",hdr)
  self.assertIn("png_converter_reprobes",hdr)
 def test_region_epoch_disallows_old_workers_changing_new_policy(self):
  hdr=read("include/nexverse/assets/texture_format_preference.hpp")
  self.assertIn("std::scoped_lock lock{mutex_}",hdr)
  self.assertIn("epoch_!=epoch",hdr)
  self.assertIn("epoch_=epoch;",hdr)
  self.assertIn("png_avoided_=0U",hdr)
 def test_single_gettexture_future_uses_shared_region_policy(self):
  app=read("src/app/application.cpp")
  self.assertIn("std::make_shared<assets::TextureFormatPreference>()",app)
  self.assertIn("texture_format_preference->enter(material_scope.epoch())",app)
  self.assertIn("loader.set_format_preference(",app)
  self.assertIn("texture_format_preference,expected_epoch",app)
 def test_same_asset_falls_back_when_j2c_fails(self):
  loader=read("src/assets/texture_loader.cpp")
  self.assertIn("recover_current_as_png",loader)
  self.assertIn("if(!skip_png)return std::nullopt;",loader)
  self.assertIn("format_preference_->native_failed(format_epoch_)",loader)
  self.assertIn("GetTexture converter fallback PNG succeeded",loader)
  mock=read("tests/texture_loader_tests.cpp")
  self.assertIn("Mode::j2c_down_png_ok",mock)
  self.assertIn("Mode::j2c_invalid_png_converter_recovers",mock)
  self.assertIn("same_asset_recovered",mock)
 def test_no_skipping_supported_converter_without_proof(self):
  test=read("tests/texture_loader_tests.cpp")
  self.assertIn("for(int i=0;i<3;++i)",test)
  self.assertIn("for(int i=0;i<15;++i)",test)
  self.assertIn('adaptive_http->requests[0].find("format=x-j2c")',test)
  self.assertIn('adaptive_http->requests[0].find("format=png")',test)
 def test_bounded_reprobe_and_no_permanent_invalid_backoff(self):
  header=read("include/nexverse/assets/texture_format_preference.hpp")
  native=read("tests/texture_format_preference_tests.cpp")
  self.assertIn("skips_%16U==0U",header)
  self.assertIn("png_gets_avoided!=15U",native)
  self.assertIn("png_converter_reprobes!=1U",native)
  self.assertIn("n.enter(5U)",native)
 def test_existing_jpeg_png_security_limits_unchanged(self):
  loader=read("src/assets/texture_loader.cpp")
  jpeg=read("include/nexverse/assets/jpeg2000_metadata_limits.hpp")
  self.assertIn("max_texture_download_bytes",loader)
  self.assertIn("max_j2c_side = 4096U",jpeg)
  self.assertIn("max_j2c_rgba_bytes = 64U*1024U*1024U",jpeg)
  self.assertIn("decode_png_texture(texture_id,response.body)",loader)
 def test_telemetry_private_and_reliable(self):
  app=read("src/app/application.cpp")
  self.assertIn('" native_png_gets_avoided="',app)
  self.assertIn('" native_png_reprobes="',app)
  self.assertIn('" native_preferred="',app)
  preference=read("include/nexverse/assets/texture_format_preference.hpp")
  self.assertNotIn("capability_uri",preference)
  self.assertNotIn("session_id",preference)
if __name__=="__main__":unittest.main()
