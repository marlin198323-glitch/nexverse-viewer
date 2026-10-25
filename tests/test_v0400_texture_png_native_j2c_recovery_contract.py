"""OpenGenesisLINK Viewer v0.4.0.0: PNG-endpoint native J2C/JP2 recovery."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
def read(path):
    return (ROOT/path).read_text()

class NativeTexturePayloadFallbackContract(unittest.TestCase):
    def test_native_payload_must_have_magic_signature(self):
        header=read("include/nexverse/assets/jpeg2000_payload_signature.hpp")
        self.assertIn("is_native_jpeg2000_payload(",header)
        self.assertIn("0xffU",header)
        self.assertIn("0x4fU",header)
        self.assertIn("boxed_jp2",header)
        self.assertIn("return codestream||boxed_jp2;",header)

    def test_positive_png_response_is_always_decoded(self):
        loader=read("src/assets/texture_loader.cpp")
        self.assertIn("if(is_native_jpeg2000_payload(png_response.body))",loader)
        self.assertIn("decode_jpeg2000_texture(",loader)
        self.assertIn("if(native.success)",loader)
        self.assertIn("PNG response contained valid JPEG2000",loader)
        self.assertIn("png_error=\"GetTexture PNG returned invalid JPEG2000: \"",loader)
        self.assertLess(loader.index("if(native.success)"),
                        loader.index("const auto j2c_response = transport_->get("))

    def test_corrupt_j2c_after_png_does_not_bypass_new_http_fetch(self):
        native=read("tests/texture_loader_tests.cpp")
        self.assertIn("png_returns_truncated_j2c",native)
        self.assertIn("transport->requests.size()!=2U",native)
        self.assertIn("transport->j2c_requests!=1U",native)

    def test_png_mislabeled_j2c_and_jp2_not_double_downloaded(self):
        native=read("tests/texture_loader_tests.cpp")
        for kind in ("png_returns_native_j2c","png_returns_native_jp2"):
            self.assertIn(kind,native)
        self.assertIn("transport->requests.size()!=1U",native)
        self.assertIn("transport->j2c_requests!=0U",native)

    def test_decoder_safety_and_native_signature_ctest(self):
        decoder=read("src/assets/jpeg2000_decoder.cpp")
        cmake=read("CMakeLists.txt")
        self.assertIn("max_compressed_bytes = 32U * 1024U * 1024U",decoder)
        self.assertIn("jpeg2000_image_limits_reason(",decoder)
        self.assertIn("jpeg2000_component_limits_reason(",decoder)
        self.assertIn("nexverse-jpeg2000-payload-signature-tests",cmake)

if __name__=="__main__":
    unittest.main()
