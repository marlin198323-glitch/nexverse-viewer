"""Alpha.285-294: actual Firestorm self-appearance handshake negotiation."""
import json
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def s(path):return (R/path).read_text(encoding="utf-8")
class SelfAppearanceHandshakeContract(unittest.TestCase):
    def test_golden_firestorm_bit_two_default_and_explicit_live_circuit(self):
        header=s("include/nexverse/protocol/region_handshake_appearance.hpp")
        lludp=s("include/nexverse/protocol/lludp.hpp")
        circuit=s("src/protocol/circuit.cpp")
        for needle in ("1U<<2U","ogl_region_handshake_flags =",
                       "region_handshake_supports_self_appearance",
                       "region_handshake_object_cache_probes",
                       "region_handshake_empty_object_cache"):
            self.assertIn(needle,header)
        self.assertIn("std::uint32_t flags = ogl_region_handshake_flags",lludp)
        self.assertIn("reply_sequence,session_,ogl_region_handshake_flags",circuit)
        self.assertIn("send_reliable(",circuit)
    def test_never_conflate_advertising_with_server_packet(self):
        circuit=s("src/protocol/circuit.cpp")
        status=s("include/nexverse/protocol/circuit.hpp")
        ui=s("include/nexverse/ui/avatar_appearance_runtime.hpp")
        self.assertIn("status.self_appearance_support_sent=false;",circuit)
        self.assertIn("status.self_appearance_support_sent=self_appearance_receipt_.sent;",circuit)
        self.assertLess(circuit.index("if (!send_reliable(",
                        circuit.index("auto reply = encode_region_handshake_reply(")),
                        circuit.index("status.self_appearance_support_sent=self_appearance_receipt_.sent;"))
        self.assertIn("bool self_appearance_support_sent=false;",status)
        self.assertIn("Self-Serverantwort fehlt",ui)
        self.assertIn("Self vom Simulator empfangen",ui)
        self.assertIn("nicht gesendet",ui)
    def test_real_f3_and_source_handshake_status(self):
        app=s("src/app/application.cpp")
        view=s("include/nexverse/ui/viewer_shell_render.hpp")
        gui=s("src/ui/viewer_shell_render.cpp")
        self.assertIn("shell_view.avatar_handshake_status=",app)
        self.assertIn("circuit_snapshot.self_appearance_support_sent",app)
        self.assertIn("avatar_proof.appearance_received",app)
        self.assertIn("firestorm_region_lease.matches(current_firestorm_epoch)",app)
        self.assertIn("avatar_handshake_status",view)
        self.assertIn("line(9,compact(v.avatar_handshake_status",gui)
    def test_private_report_only_bool_not_identifiers(self):
        report=s("src/ui/avatar_diagnostic_report.cpp")
        self.assertIn("region_handshake_self_appearance_bit2_sent=",report)
        self.assertIn("yn(r.self_appearance_support_sent)",report)
        app=s("src/app/application.cpp")
        self.assertIn("avatar_diagnostic.self_appearance_support_sent=",app)
        for secret in ("session_id=","agent_id=","password=","capability_url="):
            self.assertNotIn('"'+secret+'"',report)
    def test_golden_wire_positive_and_legacy_explicit_negative(self):
        test=s("tests/lludp_tests.cpp")
        self.assertIn("ssa.packet->payload[32U]!=4U",test)
        self.assertIn("decoded_region_reply.packet->payload[32U]!=0U",test)
        self.assertIn("(ssa.packet->header.flags&packet_reliable)==0U",test)
    def test_handoff_and_percent(self):
        self.assertEqual(s("VERSION").strip(),"0.2.0-alpha.304")
        data=json.loads(s("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(data["viewer_version"],"0.2.0-alpha.304")
        self.assertEqual(data["default_blocks_per_run"],10)
        self.assertEqual(data["next_batch"],"Alpha.305–314")
        for name in ("CHATUEBERGABE.md","AGENTS.md","info.md",
                     "projectplan.md","docs/PROJEKTFORTSCHRITT.md"):
            self.assertIn("39 %",s(name)[:2400])
            self.assertIn("Alpha.305–314",s(name)[:2400])
if __name__=="__main__": unittest.main()
