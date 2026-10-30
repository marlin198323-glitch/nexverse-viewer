"""Compact neon login must show the distinct presence hints without
splitting German UTF-8 text and without changing Welcome or dock size."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
def src(name):return (ROOT/name).read_text()

class LoginPresenceNoticeLayoutTests(unittest.TestCase):
    def test_old_byte_clip_removed(self):
        a=src("src/app/application.cpp")
        self.assertNotIn("login.notice.substr(0U,86U)",a)
        self.assertIn("nexverse::ui::fit_notice_to_width(",a)

    def test_pixel_width_reserved_for_version_label(self):
        a=src("src/app/application.cpp")
        self.assertIn("w-l.username.x-125.0f*sc",a)
        self.assertIn("renderer.text_width(8.0f*sc,sample)",a)

    def test_neon_60px_dock_and_welcome_intact(self):
        h=src("include/nexverse/ui/neon_login_layout.hpp")
        self.assertIn("footer_h=60.0f*scale;",h)
        self.assertIn("header_h=58.0f*scale;",h)
        self.assertIn("welcome_capture_height()",h)

    def test_utf8_codepoint_boundary_is_checked(self):
        h=src("include/nexverse/ui/notice_clipping.hpp")
        self.assertIn("(next&0xC0U)!=0x80U",h)
        self.assertIn("offset+width>source.size()",h)

    def test_notice_renderer_cpu_budget(self):
        h=src("include/nexverse/ui/notice_clipping.hpp")
        self.assertIn("max_bytes=512U",h)
        self.assertIn("offset<max_bytes",h)

    def test_truncated_notice_honestly_shows_ellipsis(self):
        h=src("include/nexverse/ui/notice_clipping.hpp")
        self.assertIn('constexpr std::string_view suffix{"..."}',h)
        self.assertIn("out+=suffix;",h)

    def test_native_layout_tests_cover_real_utf8(self):
        t=src("tests/neon_login_layout_tests.cpp")
        self.assertIn("Kontobestätigung",t)
        self.assertIn("fit_notice_to_width",t)
        self.assertIn("glyph_width(clipped)>200.0f",t)
        self.assertIn("hostile.size()>515U",t)

    def test_login_version_label_remains(self):
        a=src("src/app/application.cpp")
        self.assertIn("l.footer_y-14.0f*sc,8.0f*sc,neon_version",a)

if __name__=="__main__":unittest.main()
