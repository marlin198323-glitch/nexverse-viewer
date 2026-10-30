"""OGL v0.4 secure optional persisted login + faster scene cache + camera sky."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(x):return (R/x).read_text()
class SecureLoginCacheAndSkyContracts(unittest.TestCase):
 def test_system_secret_service_optional(self):
  cm=src("CMakeLists.txt")
  self.assertIn("pkg_check_modules(OGL_LIBSECRET QUIET",cm)
  self.assertIn("NEXVERSE_HAVE_LIBSECRET=1",cm)
 def test_never_plaintext_password_config_fallback(self):
  vault=src("src/ui/login_keyring.cpp")
  self.assertIn("secret_password_store_sync(",vault)
  self.assertIn("secret_password_lookup_sync(",vault)
  self.assertNotIn("std::ofstream",vault)
  self.assertNotIn("popen(",vault)
  self.assertNotIn("system(",vault)
  self.assertIn("return false;",vault)
 def test_avatar_stored_only_after_authenticated_login(self):
  a=src("src/app/application.cpp")
  self.assertLess(a.index("login_ui.authenticated = true;"),a.index("const bool saved=ui::save_ogl_login("))
  self.assertIn("pending_login_password_for_keyring=credentials.password",a)
  self.assertIn("std::fill(pending_login_password_for_keyring.begin()",a)
 def test_dedicated_remember_control_does_not_toggle_password_visibility(self):
  a=src("src/app/application.cpp")
  self.assertIn("layout.password_eye.contains",a)
  self.assertIn("state.show_password=!state.show_password",a)
  self.assertIn("state.remember_login=!state.remember_login",a)
  self.assertIn("Gespeicherten Login aus Schlüsselbund entfernt",a)
 def test_restore_does_not_auto_login(self):
  a=src("src/app/application.cpp")
  self.assertIn("load_saved_ogl_login()",a)
  self.assertIn("login_ui.username=saved->avatar",a)
  self.assertIn("login_ui.password=saved->password",a)
  self.assertIn("!login_ui.busy&&!login_ui.authenticated",a)
 def test_cache_prio_2_of_3_never_starves_network(self):
  a=src("src/app/application.cpp")
  self.assertIn("texture_request_fairness.dispatched_count()%3U!=2U",a)
  self.assertIn("return disk.contains(id);",a)
  self.assertIn("if(!request_choice)",a)
  self.assertIn("texture_request_fairness.dispatched();",a)
 def test_cache_contains_checks_uuid_and_file_size(self):
  s=src("src/assets/texture_disk_cache.cpp")
  self.assertIn("if(!safe_uuid(id)||directory_.empty())return false",s)
  self.assertIn("std::filesystem::file_size(file,error)",s)
  self.assertIn("size<=max_image_bytes+24U",s)
 def test_cache_still_uses_epoch_and_byte_budget(self):
  a=src("src/app/application.cpp")
  self.assertIn("load_texture_cached_scoped(",a)
  self.assertIn("texture_gpu_budget_bytes =",a)
  self.assertIn("const auto request_epoch=material_scope.epoch()",a)
  self.assertIn("object_texture_request_epoch=ready->epoch;",a)
 def test_sun_position_camera_not_fixed_viewport(self):
  r=src("src/render/opengl_renderer.cpp")
  self.assertIn("project_environment_sun(env,camera_,sky_aspect)",r)
  self.assertNotIn("0.5f + 0.42f * std::cos(env.east_angle)",r)
  self.assertIn('"uCameraPitch"',r)
  self.assertIn('"uSunVisible"',r)
 def test_sky_forward_behind_sun_angle_guard(self):
  s=src("include/nexverse/render/sky_projection.hpp")
  self.assertIn("if(z<=0.001f)return result;",s)
  self.assertIn("environment.sun_angle",s)
  self.assertIn("camera.left",s)
 def test_native_sky_and_tex_mip_tests(self):
  s=src("tests/texture_mip_tests.cpp")
  self.assertIn("project_environment_sun(",s)
  self.assertIn("if(rear.sun_in_view)",s)
  self.assertIn('near_high.at("street")!=0U',s)
 def test_mip_48m_near_priority(self):
  s=src("src/assets/texture_mip.cpp")
  self.assertIn("dist < 48.0 ? 0U",s)
  self.assertIn("dist < 96.0 ? 1U",s)
  self.assertIn("dist < 176.0 ? 2U",s)
if __name__=="__main__":unittest.main()
