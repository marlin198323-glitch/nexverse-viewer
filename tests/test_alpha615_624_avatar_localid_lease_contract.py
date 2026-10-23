"""Full/Compressed Avatar ObjectUpdate UUID is owner of current LocalID."""
import json,unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text(encoding="utf-8")
class LocalAvatarLease(unittest.TestCase):
 def test_real_full_and_compressed_object_dispatcher(self):
  src=read("src/app/application.cpp")
  self.assertEqual(src.count(
    "entry.id,entry.position,entry.rotation,entry.local_id,\n                        true);"),2)
  self.assertIn("world_model.kill_simulator_local_id(local_id)",src)
  self.assertIn("entry.is_avatar()",src)
  self.assertIn("entry.pcode == protocol::pcode_avatar",src)
 def test_uuid_and_localid_guard_retires_stale_scene_only(self):
  src=read("src/world/world_model.cpp")
  a=src.index("void WorldModel::update_avatar_transform(")
  b=src.index("bool WorldModel::apply_coarse_avatar_locations(",a)
  body=src[a:b]
  for t in (
    "if(from_avatar_object_update&&(!region_.connected||local_id==0U))",
    "old.local_id==local_id",
    "old.simulator_object_update_seen",
    "previous->second.local_id!=local_id",
    "saved=previous->second;",
    "kill_simulator_local_id(retired_id,false)",
    "++remote_avatar_local_id_rebinds_;",
    "saved->local_id=0U;",
    "avatars_.insert_or_assign(id,std::move(*saved))",
    "avatar.simulator_object_update_seen=true;",
    "avatar.coarse_position_only=false;"
  ):self.assertIn(t,body)
 def test_separate_real_killobject_from_implicit_rebind_counters(self):
  s=read("src/world/world_model.cpp")
  self.assertIn("bool count_kill_object)",s)
  self.assertIn("if(count_kill_object&&remote_avatar_kills_<UINT32_MAX)",s)
  self.assertIn("if(count_kill_object)\n        {",s)
  self.assertEqual(s.count("remote_avatar_local_id_rebinds_=0U;"),4)
  self.assertIn("WorldModel::remote_avatar_local_id_rebinds() const",s)
 def test_native_outfit_and_cof_metadata_retained_on_same_uuid(self):
  src=read("tests/avatar_localid_reuse_tests.cpp")
  for x in (
    'constexpr auto alice="aaaaaaaa-1111',
    'constexpr auto bob="bbbbbbbb-1111',
    'prim(w,111U,110U,"alice-worn-child")',
    "w.update_avatar_transform(bob,{4,5,6},{0,0,0,1},51U,true);",
    "w.avatars().contains(alice)",
    "w.remote_avatar_local_id_rebinds()!=1U",
    "w.remote_avatar_kills()!=0U",
    "w.avatars().at(bob).texture_entry!=bob_meta.texture_entry",
    "w.avatars().at(bob).visual_params!=bob_meta.visual_params",
    'w.avatars().at(bob).attachments[0].object_id!="bob-live-worn"',
    "w.remote_avatar_local_id_rebinds()!=2U",
    "w.avatars().at(alice).coarse_position_only",
    "w.update_avatar_transform(alice,{1,2,3},{0,0,0,1},0U,true)",
    "w.apply_region(region(false));"
  ):self.assertIn(x,src)
 def test_real_firestorm_gl_pixels_and_f7_status(self):
  s=read("tests/scene_animated_attachment_pixels_tests.cpp")
  self.assertIn('"REUSED_AVATAR_LOCALID_STALE_GPU_PIXELS="',s)
  self.assertIn('region_objects.update_avatar_transform(\n            "new-remote"',s)
  self.assertIn("if(orphan_rgb!=0U)failure=21;",s)
  ui=read("src/ui/viewer_shell_render.cpp")
  self.assertIn('" | ID neu:"+std::to_string(',ui)
  app=read("src/app/application.cpp")
  self.assertIn("world_model.remote_avatar_local_id_rebinds();",app)
  self.assertIn("view.remote_avatar_local_id_rebinds=3U;",
                read("tests/viewer_shell_gl_tests.cpp"))
 def test_version_and_next_batch(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.854")
  progress=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(progress["viewer_version"],"0.2.0-alpha.854")
  self.assertEqual(progress["next_batch"],"Alpha.855–864")
  self.assertEqual(progress["default_blocks_per_run"],10)
  self.assertIn("still open",progress["assessment"])
  for fn in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
             "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("10 ECHTE ENTWICKLUNGSBLÖCKE",read(fn)[:350])
   self.assertIn("Alpha.855–864",read(fn)[:1900])
if __name__=="__main__":unittest.main()
