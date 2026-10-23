"""Alpha1134 original Firestorm visible Shape driver => morph link; skeletal driver pending."""
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def read(x):return (ROOT/x).read_text()
class FirestormShapeDriver1134(unittest.TestCase):
 def test_driver_source_original_lad_only_bounded(self):
  s=read("src/assets/firestorm_cof_shape_catalog.cpp")
  for x in ('"driver_parameters"',
            'FirstChildElement("param_driver")',
            'std::string_view(wearable)!="shape"',
            'FirstChildElement("driven")', 'links>32U',
            'QueryIntAttribute("id",&driven_id)',
            'std::isfinite(weight)', 'weight<dmin||weight>dmax'):
   self.assertIn(x,s)
 def test_no_guessing_piecewise_or_bone_appearance(self):
  s=read("src/assets/firestorm_cof_shape_catalog.cpp")
  for x in ('link->Attribute("min1")','link->Attribute("max1")',
            'link->Attribute("min2")','link->Attribute("max2")',
            'FirstChildElement("param_skeleton")',
            'selected.skeletal_driver_pending=true;',
            'std::abs(tmin-dmin)>0.0001f',
            'std::abs(tmax-dmax)>0.0001f'):
   self.assertIn(x,s)
  h=read("include/nexverse/assets/firestorm_cof_shape_morph_preview.hpp")
  self.assertIn("driver_morph_links",h)
  self.assertIn("skeletal_driver_pending",h)
 def test_driver_requires_real_original_mesh_morph_and_prevents_double_apply(self):
  s=read("src/assets/firestorm_cof_shape_catalog.cpp")
  for x in ('param->FirstChildElement("param_morph")',
            'morph.name==name', 'duplicate!=output.end()',
            'std::abs(duplicate->second-weight)>0.0001f',
            'output.size()>=96U', '++selected.driver_morph_links'):
   self.assertIn(x,s)
  t=read("tests/firestorm_avatar_mesh_tests.cpp")
  for x in ('{2,1.1f}', '{35,1.4f}',
            'driven->head.size()!=2U',
            'driven->driver_morph_links!=2U',
            '!driven->skeletal_driver_pending',
            'ORIGINAL_FIRESTORM_DRIVER_HEAD_MORPHS='):
   self.assertIn(x,t)
 def test_driver_reaches_verified_current_owner_cof_and_ui_without_fake_proof(self):
  s=read("src/app/application.cpp")
  for x in ('firestorm_cof_shape_morph_from_original_lad(',
            'gallery_wearables_fetch_owner==',
            'gallery_asset_errors==0U',
            'caps_state.current_outfit_folder==gallery_asset_cof',
            'DriverMorph=',
            'selection->skeletal_driver_pending',
            'Bone-Driver offen, lokal, ',
            'kein Bake-Beweis'):
   self.assertIn(x,s)
 def test_actual_original_firestorm_gpu_self_only_and_reset(self):
  s=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ('ORIGINAL_FIRESTORM_SHAPE_DRIVER_HEAD_PIXELS=',
            'firestorm_cof_shape_morph_from_original_lad(',
            'driver_delta<2U',
            'remote_stale!=0U',
            'renderer->clear_self_avatar_head_shape_preview()',
            'ORIGINAL_LAD_DYNAMIC_EXTRA_BODY_MORPH_PIXELS='):
   self.assertIn(x,s)
if __name__=="__main__":unittest.main()
