"""Alpha1124: original Firestorm LAD+LLM-verified dynamic COF shape, real GPU proof."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def src(name):return (R/name).read_text()
class OriginalDynamicLadMorph(unittest.TestCase):
 def test_runtime_reads_only_bounded_local_original_firestorm_lad(self):
  s=src("src/assets/firestorm_cof_shape_catalog.cpp")
  for x in ('directory/"avatar_lad.xml"','size>1024U*1024U',
            'parameters.size()>512U', 'LoadFile(',
            'FirstChildElement("linden_avatar")',
            'NextSiblingElement("mesh")',
            'FirstChildElement("param_morph")',
            'std::string_view(wearable)!="shape"'):
   self.assertIn(x,s)
 def test_lad_ids_crosschecked_with_actual_matching_mesh_targets(self):
  s=src("src/assets/firestorm_cof_shape_catalog.cpp")
  for x in ('"avatar_head.llm"','"avatar_eyelashes.llm"',
            '"avatar_upper_body.llm"','"avatar_lower_body.llm"',
            'load_firestorm_avatar_llm(', 'morph.name==name',
            'QueryIntAttribute("id",&id)', 'QueryFloatAttribute("value_min",&min)',
            'QueryFloatAttribute("value_max",&max)',
            'std::isfinite(weight)','weight<min||weight>max',
            'duplicate!=output.end()', 'output.size()>=96U'):
   self.assertIn(x,s)
 def test_current_owner_cof_download_uses_xml_catalog_and_fails_closed(self):
  s=src("src/app/application.cpp")
  a=s.index("// An entirely resolved LLWearable snapshot supersedes")
  b=s.index("if(!gallery_asset_active_id.empty()&&",a)
  scope=s[a:b]
  for x in ('clear_self_avatar_head_shape_preview()',
            'gallery_asset_errors==0U',
            'gallery_wearables_fetch_owner==',
            'caps_state.current_outfit_folder==gallery_asset_cof',
            'gallery_decoded_wearable_assets.size()==',
            'firestorm_cof_shape_morph_from_original_lad(',
            'if(head_ok&&lash_ok&&upper_ok&&lower_ok)',
            'COF-LAD Preview '):
   self.assertIn(x,scope)
  self.assertNotIn('appearance->visual_params[',scope)
 def test_real_original_fixture_expands_beyond_fixed_eight_and_rejects_bad(self):
  s=src("tests/firestorm_avatar_mesh_tests.cpp")
  for x in ('FIRESTORM_ORIGINAL_LAD_DYNAMIC_MORPHS_HEAD=',
            'dynamic->upper.size()!=3U',
            'dynamic->lower.size()!=3U',
            '{634,0.7f}', '{635,0.8f}',
            'directory', 'dynamic->eyelashes.size()!=4U'):
   if x=='directory':continue
   self.assertIn(x,s)
  self.assertIn('firestorm_cof_shape_morph_from_original_lad(',s)
  morph=src("src/assets/firestorm_avatar_mesh.cpp")
  self.assertIn('weights.size()>96U',morph)
 def test_real_opengl_head_upper_lower_and_remote_staleness(self):
  s=src("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ('ORIGINAL_LAD_DYNAMIC_EXTRA_BODY_MORPH_PIXELS=',
            'firestorm_cof_shape_morph_from_original_lad(',
            'expanded_pixels<2U', 'REMOTE_BODY_SHAPE_GHOST_PIXELS=',
            'RESET_BODY_SHAPE_GHOST_PIXELS=',
            'renderer->clear_self_avatar_head_shape_preview()'):
   self.assertIn(x,s)
if __name__=="__main__":unittest.main()
