"""Alpha.295-304: SSA handshake ACK correlation and truthful avatar readback."""
import json
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def source(path):return (R/path).read_text(encoding="utf-8")
class Alpha295AckProof(unittest.TestCase):
    def test_real_circuit_ack_uses_reliable_tracker(self):
        cpp=source("src/protocol/circuit.cpp")
        h=source("include/nexverse/protocol/circuit.hpp")
        for needle in ("const bool tracker_matched=tracker.acknowledge(ack)",
            "self_appearance_receipt_.note_ack(ack,tracker_matched)",
            "self_appearance_receipt_.note_refresh(reply_sequence)",
            "status.self_appearance_support_acknowledged=true;",
            "self_appearance_receipt_.reset();",
            "status.self_appearance_ack_at=std::chrono::steady_clock::now()"):
            self.assertIn(needle,cpp)
        self.assertIn("SelfAppearanceHandshakeReceipt self_appearance_receipt_",h)
        self.assertIn("self_appearance_support_acknowledged=false;",h)

    def test_old_wrong_duplicate_ack_not_valid(self):
        receipt=source("include/nexverse/protocol/region_handshake_appearance.hpp")
        test=source("tests/self_appearance_ack_receipt_tests.cpp")
        self.assertIn("ack_sequence!=sequence",receipt)
        self.assertIn("!pending_tracker_matched",receipt)
        self.assertIn("if(outgoing_sequence!=0U)",receipt)
        self.assertIn("receipt.note_sent(113U)",test)
        self.assertIn("receipt.note_ack(111U,true)",test)
        self.assertIn("receipt.note_ack(111U,false)",test)
        self.assertIn("receipt.note_sent(0U)",test)
        self.assertIn("self_appearance_ack_receipt_tests",source("scripts/pi5-headless-tests.sh"))

    def test_passive_timeout_never_claims_server_failure(self):
        ui=source("include/nexverse/ui/avatar_appearance_runtime.hpp")
        app=source("src/app/application.cpp")
        self.assertIn("since_ack>=std::chrono::seconds{15}",ui)
        self.assertIn("Self nach 15s weiter ausstehend",ui)
        self.assertIn("Simulator-ACK ausstehend",ui)
        self.assertIn("Self-Paket empfangen",ui)
        self.assertIn("std::chrono::steady_clock::now()-",app)
        self.assertIn("circuit_snapshot.self_appearance_ack_at",app)
        self.assertIn("avatar_proof.appearance_received,ack_wait",app)

    def test_f3_only_current_authenticated_self(self):
        app=source("src/app/application.cpp")
        self.assertIn("shell_view.appearance_ready=avatar_proof.appearance_received",app)
        self.assertNotIn("shell_view.appearance_ready=\n                        world_model.self_appearance().has_value()",app)
        self.assertIn("firestorm_region_lease.matches(current_firestorm_epoch)",app)
        self.assertIn("line(9,compact(v.avatar_handshake_status",source("src/ui/viewer_shell_render.cpp"))

    def test_local_whitelist_ack_and_bounded_wait(self):
        report=source("src/ui/avatar_diagnostic_report.cpp")
        app=source("src/app/application.cpp")
        for needle in ("region_handshake_self_appearance_bit2_acked=",
                       "self_appearance_reply_wait_seconds=",
                       "std::to_string(r.self_appearance_ack_wait_seconds)"):
            self.assertIn(needle,report)
        self.assertIn("std::int64_t{120}",app)
        self.assertIn("self_appearance_support_acknowledged=",app)
        self.assertNotIn('"simulator_ip="',report)
        self.assertNotIn('"session_id="',report)

    def test_release_and_handoff(self):
        data=json.loads(source("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(source("VERSION").strip(),"0.2.0-alpha.1054")
        self.assertEqual(data["viewer_version"],"0.2.0-alpha.1054")
        self.assertEqual(data["next_batch"],"Alpha.1055–1064")
        self.assertEqual(data["default_blocks_per_run"],10)
        for name in ("CHATUEBERGABE.md","AGENTS.md","info.md",
                     "projectplan.md","docs/PROJEKTFORTSCHRITT.md"):
            text=source(name)[:2100]
            self.assertIn("39 %",text)
            self.assertIn("Alpha.1055–1064",text)
if __name__=="__main__":unittest.main()
