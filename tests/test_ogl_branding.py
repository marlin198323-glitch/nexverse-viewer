"""Original user asset and real GLFW/window/desktop/Login integration."""
import pathlib
import struct
import unittest

ROOT=pathlib.Path(__file__).resolve().parents[1]
ASSETS=ROOT/"assets/branding"

class OriginalBrandingTests(unittest.TestCase):
    def test_png_assets_are_actual_rgba_icons_with_correct_sizes(self):
        for size in (32,48,64,128,256,512):
            raw=(ASSETS/f"opengenesislink-viewer-{size}.png").read_bytes()
            self.assertEqual(raw[:8],bytes.fromhex("89504e470d0a1a0a"))
            self.assertEqual(struct.unpack(">II",raw[16:24]),(size,size))
            self.assertEqual(raw[25],6) # RGBA
        self.assertEqual((ASSETS/"opengenesislink-viewer.png").read_bytes(),
                         (ASSETS/"opengenesislink-viewer-256.png").read_bytes())

    def test_window_and_login_use_real_icon_and_keep_web_welcome(self):
        src=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("glfwSetWindowIcon(window,1,&icon)",src)
        self.assertIn('glfwWindowHintString(GLFW_X11_CLASS_NAME,"opengenesislink-viewer")',src)
        self.assertIn("const ui::BrandIcon brand_icon=ui::load_brand_icon_png(icon_file)",src)
        self.assertIn("renderer.image(brand_texture,19.0f*scale",src)
        self.assertIn("renderer.destroy_texture(brand_texture)",src)
        self.assertIn("welcome_page.initialize(window_width, window_height)",src)
        self.assertIn("const ui::WelcomeFrame& welcome_frame = welcome_page.frame()",src)
        self.assertIn("renderer.image(welcome_texture,",src)

    def test_linux_launcher_matches_x11_window_class(self):
        data=(ROOT/"packaging/linux/opengenesislink-viewer.desktop").read_text()
        self.assertIn("Icon=opengenesislink-viewer",data)
        self.assertIn("StartupWMClass=opengenesislink-viewer",data)
        self.assertIn("Exec=nexverse-viewer",data)
        self.assertIn("Terminal=false",data)
        self.assertIn("Name=OpenGenesisLINK Viewer",data)

if __name__=="__main__":
    unittest.main()
