"""v0.4.0.0: backwards x-j2c PNG-compatible response and private diagnostics."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path):return (R/path).read_text()
class ReverseCapsImageFormatContract(unittest.TestCase):
 def test_png_signature_must_be_complete(self):
  header=read("include/nexverse/assets/jpeg2000_payload_signature.hpp")
  self.assertIn("is_png_payload(std::string_view bytes)",header)
  self.assertIn("bytes.size()>=signature.size()",header)
  self.assertIn("bytes.substr(0U,signature.size())==signature",header)
 def test_j2c_png_response_requires_full_libpng_decode(self):
  source=read("src/assets/texture_loader.cpp")
  self.assertIn("if(is_png_payload(j2c_response.body))",source)
  self.assertIn("decode_png_texture(texture_id,j2c_response.body)",source)
  self.assertIn("GetTexture x-j2c returned valid PNG",source)
  self.assertIn("x-j2c returned invalid PNG",source)
  self.assertLess(source.index("if(is_png_payload(j2c_response.body))"),source.index("result = decode_jpeg2000_texture(texture_id, j2c_response.body);"))
 def test_reverse_png_fallback_success_no_new_request(self):
  mock=read("tests/texture_loader_tests.cpp")
  self.assertIn("Mode::j2c_returns_valid_png",mock)
  self.assertIn("png_on_j2c.image.width!=1U",mock)
  self.assertIn("transport->requests.size()!=2U",mock)
 def test_truncated_png_not_a_valid_image(self):
  mock=read("tests/texture_loader_tests.cpp")
  self.assertIn("Mode::j2c_returns_truncated_png",mock)
  self.assertIn("truncated_png.failure!=AssetFailure::invalid",mock)
  self.assertIn("truncated_png.message.find",mock)
 def test_zero_relaxation_of_png_security_limits(self):
  source=read("src/assets/texture_loader.cpp")
  self.assertIn("max_texture_dimension = 4096U",source)
  self.assertIn("32U * 1024U * 1024U",source)
  self.assertIn("png_image_finish_read(",source)
  self.assertIn("result.image.valid()",source)
 def test_gpu_upload_warnings_do_not_expose_asset_ids(self):
  app=read("src/app/application.cpp")
  self.assertIn("Deferred GetTexture GPU upload [asset id omitted]",app)
  self.assertIn("GetTexture GPU upload rejected [asset id omitted]",app)
  self.assertNotIn('"Deferred GetTexture GPU upload " +',app)
  self.assertNotIn('"GetTexture GPU upload rejected for " +',app)
if __name__=="__main__": unittest.main()
