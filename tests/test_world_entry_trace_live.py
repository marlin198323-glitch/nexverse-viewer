"""Actual GUI, network and private-report wiring for alpha.225-229."""
import pathlib
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[1]
APP=(ROOT/"src/app/application.cpp").read_text()
REPORT=(ROOT/"src/ui/avatar_diagnostic_report.cpp").read_text()
CONTRACT=(ROOT/"include/nexverse/ui/avatar_diagnostic_report.hpp").read_text()
TRACE=(ROOT/"include/nexverse/ui/world_entry_trace.hpp").read_text()
POLICY=(ROOT/"include/nexverse/ui/loading_circuit_retry_policy.hpp").read_text()
HANDOFF=(ROOT/"CHATUEBERGABE.md").read_text()
AGENTS=(ROOT/"AGENTS.md").read_text()
PLAN=(ROOT/"projectplan.md").read_text()

class WorldEntryTraceLiveContracts(unittest.TestCase):
    def test_input_is_real_authenticated_world_not_a_mock(self):
        self.assertIn("world_entry_trace.observe(world_entry_stage,trace_now)",APP)
        self.assertIn("world_entry_trace.snapshot()",APP)
        self.assertIn("world_entry_trace.reset()",APP)
        self.assertIn("avatar_diagnostic.entry_trace_count=world_entry_trace.size()",APP)
        self.assertIn("world_entry_wait_monitor.seconds(world_entry_stage,trace_now)",APP)

    def test_modal_uses_whitelisted_stages_and_real_retry_reason(self):
        self.assertIn('std::string trail="Verlauf: "',APP)
        self.assertIn("world_entry_stage_key(",APP)
        self.assertIn("loading_retry_reason_label(report.retry_reason)",APP)
        self.assertIn("std::to_string(report.manual_reconnect_count)",APP)
        self.assertIn("loading_circuit_retry_reason=ui::loading_retry_reason(",APP)
        self.assertIn("manual_circuit_reconnect_count<9999U",APP)
        self.assertIn("launch_circuit(std::move(retry_session));",APP)

    def test_report_contains_stage_only_and_no_raw_session(self):
        self.assertIn("std::array<WorldEntryTraceItem,WorldEntryTrace::capacity>",CONTRACT)
        self.assertIn("std::min(r.entry_trace_count,",REPORT)
        self.assertIn("world_entry_stage_key(r.entry_trace[i].stage)",REPORT)
        self.assertIn("avatar_diagnostic_trace_text(report)",REPORT)
        self.assertNotIn("simulator_ip=",REPORT)
        self.assertNotIn("agent_id=",REPORT)
        self.assertNotIn("password=",REPORT)
        self.assertIn("static constexpr std::size_t capacity=5U",TRACE)
        self.assertIn("std::array<WorldEntryTraceItem,capacity>",TRACE)

    def test_retry_reason_is_derived_from_guards(self):
        for needle in ("worker_finished", "stop_requested", "valid_target",
                       "final_failed", "cooldown_expired"):
            self.assertIn(needle,POLICY)
        self.assertIn("LoadingCircuitRetryReason::ready",POLICY)
        self.assertIn("LoadingCircuitRetryReason::worker_still_running",POLICY)
        self.assertIn("LoadingCircuitRetryReason::target_not_verified",POLICY)

    def test_five_blocks_rule_is_at_top_for_future_chat(self):
        # The newly mandatory % progress heading now comes first,
        # and the equally mandatory five-block rule must still be near the top.
        self.assertTrue("BEI JEDEM WEITER FÜNF ECHTE ENTWICKLUNGSBLÖCKE"
                        in HANDOFF[:2700])
        self.assertTrue("FÜNF-BLÖCKE-REGEL" in AGENTS[:1900])
        self.assertTrue("FÜNF-BLÖCKE-REGEL" in PLAN[:2500])

if __name__=="__main__":
    unittest.main()
