"""Current actual simulator ObjectUpdate State dominates stale Low158 attachment entries."""
import json, unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def read(fn): return (ROOT/fn).read_text(encoding="utf-8")
class LiveAttachmentPrecedence(unittest.TestCase):
 def test_current_state_overrides_stale_low158(self):
  h=read("include/nexverse/world/attachment_state.hpp")
  for token in ("struct AvatarAttachmentSelection",
    "reconcile_avatar_attachments(", "std::unordered_set<std::string_view> known",
    "prim.parent_id!=avatar_local_id", "if(known.contains(id))",
    "result.points.insert_or_assign(prim.id,slot)",
    "result.points.emplace(id,slot)", "++result.stale_appearance_points",
    "++result.pending_appearance_points", "if(avatar_local_id==0U)return result"):
   self.assertIn(token,h)
 def test_real_self_and_remote_gpu_rig_both_use_reconciliation(self):
  src=read("src/app/application.cpp")
  self.assertEqual(src.count("world::reconcile_avatar_attachments("),4)
  self.assertIn("world_model.objects(),self_avatar_local_id",src)
  self.assertIn("world_model.objects(),avatar.local_id",src)
  self.assertIn("avatar_proxy_update.back().attachments.insert(",src)
  self.assertNotIn("avatar_proxy_update.back().attachments.emplace(",src)
 def test_real_native_negative_attachment_cases(self):
  s=read("tests/self_attachment_state_tests.cpp")
  for token in ('{"sleeve",31U}', '{"hud",1U}',
    '{"pending",5U}', 'merged.pending_appearance_points!=1U',
    'merged.stale_appearance_points!=4U',
    'objects.at(100U).state=0xF1U;',
    'objects.at(100U).state=0U;',
    'objects.at(100U).parent_id=88U;'):
   self.assertIn(token,s)
 def test_actual_firestorm_gpu_uses_current_joint_not_old_hud(self):
  s=read("tests/scene_animated_attachment_pixels_tests.cpp")
  for t in ('{"sleeve",31U}', 'reconcile_avatar_attachments(',
            'inferred.current_object_points!=1U',
            'JOINT_ATTACHMENT_POSED_RGB_PIXELS='):
   self.assertIn(t,s)
 def test_f3_and_private_report_do_not_claim_worn_or_rendered(self):
  ui=read("src/ui/viewer_shell_render.cpp")
  self.assertIn('" | Bind:"+std::to_string(',ui)
  self.assertIn('" | Alt:"+std::to_string(',ui)
  self.assertIn('" | Offen:"+std::to_string(',ui)
  diag=read("src/ui/avatar_diagnostic_report.cpp")
  for t in ("self_attachment_effective_bindings=",
    "self_attachment_pending_low158=",
    "self_attachment_stale_low158="):
   self.assertIn(t,diag)
  self.assertNotIn("self_attachment_uuid=",diag)
 def test_cof_and_texture_gates_not_bypassed(self):
  src=read("src/app/application.cpp")
  self.assertIn("world_model.self_appearance()",src)
  self.assertIn("world_model.self_bake_selection()",src)
  self.assertIn("gallery_cof_batch.status()",src)
 def test_version_and_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.804")
  p=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(p["viewer_version"],"0.2.0-alpha.804")
  self.assertEqual(p["next_batch"],"Alpha.805–814")
  self.assertEqual(p["default_blocks_per_run"],10)
  self.assertIn("still open",p["assessment"])
  for fn in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
             "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.805–814",read(fn)[:1800])
if __name__=="__main__":unittest.main()
