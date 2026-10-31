"""Original asset protocol evidence: HTTP vs UDP, redaction and no false bake."""
from pathlib import Path
import importlib.util
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(path):return (ROOT/path).read_text()
spec=importlib.util.spec_from_file_location("ogl_diagnose",ROOT/"scripts/ogl-asset-diagnose.py")
diag=importlib.util.module_from_spec(spec)
spec.loader.exec_module(diag)
def report(events):
 raw=("2026-10-31 11:05:00 [INFO] OpenSim login succeeded; "
      "simulator session established\n")
 return "\n".join(diag.summarize(raw+events))
class OriginalWearableFailureDiagnosis(unittest.TestCase):
 def test_http_failure_enum_has_precise_labels(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  for k in ("http_not_found","http_forbidden",
            "http_method_unsupported","http_server_error",
            "http_client_error","http_transport_error",
            "parser_rejected","category_mismatch","stale_region"):
   self.assertIn(k,h)
 def test_http_404_not_identified_as_server_asset_proof(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  self.assertIn("if(status==404L)",h)
  self.assertIn("http_not_found",h)
  self.assertNotIn('return "server_asset_missing"',h)
 def test_http_permission_rejection_categorized(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  self.assertIn("status==401L||status==403L",h)
 def test_http_unsupported_get_method_categorized(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  self.assertIn("status==405L||status==501L",h)
 def test_http_parser_and_category_differ(self):
  h=read("include/nexverse/protocol/firestorm_wearable_viewer_asset.hpp")
  self.assertIn("failure=WearableViewerAssetFailure::parser_rejected",h)
  self.assertIn("failure=WearableViewerAssetFailure::category_mismatch",h)
 def test_http_no_url_server_response_in_logs(self):
  s=read("src/app/application.cpp")
  self.assertIn("wearable_viewer_asset_failure_label(",s)
  self.assertIn("http_status=",s)
  self.assertIn("no URLs, IDs or response bodies logged",s)
  self.assertNotIn("core::warning(response.error)",s)
 def test_udp_server_missing_is_specific_not_ambiguity(self):
  h=read("include/nexverse/protocol/wearable_asset_transfer.hpp")
  self.assertIn("WearableTransferFailure::simulator_absent",h)
  self.assertIn("Asset absent on simulator",h)
 def test_udp_negative_other_status_not_false_absent(self):
  h=read("include/nexverse/protocol/wearable_asset_transfer.hpp")
  self.assertIn("status<0&&status>=-16",h)
  self.assertIn("WearableTransferFailure::simulator_refused",h)
 def test_udp_bad_packets_not_accepted(self):
  h=read("include/nexverse/protocol/wearable_asset_transfer.hpp")
  for x in ("transfer_metadata_mismatch","transfer_info_invalid",
            "packet_budget","conflicting_packet","packet_after_final",
            "conflicting_final","wearable_parse_rejected"):
   self.assertIn(x,h)
 def test_udp_complete_payload_size_mismatch_fails_promptly(self):
  h=read("include/nexverse/protocol/wearable_asset_transfer.hpp")
  native=read("tests/firestorm_wearable_transfer_tests.cpp")
  self.assertIn("WearableTransferFailure::payload_size_mismatch",h)
  self.assertIn("p.size!=*p.expected",h)
  self.assertIn("TransferPacket complete byte count mismatch",h)
  self.assertIn("finished->failure!=WearableTransferFailure::payload_size_mismatch",native)
 def test_udp_logs_reason_only_no_identifiers(self):
  s=read("src/app/application.cpp")
  self.assertIn("wearable_transfer_failure_label(completed->failure)",s)
  self.assertIn("Avatar original wearable rejected: ",s)
  self.assertIn("no bake, asset ID or auth tokens logged",s)
 def test_udp_stale_and_subtype_differ(self):
  s=read("src/app/application.cpp")
  for x in ("stale_owner_session_cof_region",
            "original_not_current","original_flags_missing",
            "original_wearable_subtype_mismatch",
            "original_inventory_asset_type_mismatch"):
   self.assertIn(x,s)
 def test_diagnostic_summary_http_404_and_403(self):
  r=report(
   "2026-10-31 11:05:05 [WARN] Avatar original wearable ViewerAsset failure: cause=http_not_found http_status=404; no URLs\n"
   "2026-10-31 11:05:06 [WARN] Avatar original wearable ViewerAsset failure: cause=http_forbidden http_status=403; no URLs\n")
  self.assertIn("http_not_found(404)=1",r)
  self.assertIn("http_forbidden(403)=1",r)
  self.assertNotIn("Server-Bake bestätigt",r)
 def test_diagnostic_summary_sim_absent_and_parse(self):
  r=report(
   "2026-10-31 11:05:05 [WARN] Avatar original wearable rejected: cause=simulator_absent; UUIDs omitted\n"
   "2026-10-31 11:05:06 [WARN] Avatar original wearable rejected: cause=wearable_parse_rejected; UUIDs omitted\n")
  self.assertIn("simulator_absent=1",r)
  self.assertIn("wearable_parse_rejected=1",r)
 def test_no_untrusted_log_injection_into_summary(self):
  r=report(
   "2026-10-31 11:05:05 [WARN] Avatar original wearable rejected: cause=evil_url_with_token https://secret.invalid/\n"
   "2026-10-31 11:05:06 [WARN] Avatar original wearable ViewerAsset failure: cause=untrusted http_status=404 username\n")
  self.assertNotIn("evil_url_with_token",r)
  self.assertNotIn("secret.invalid",r)
  self.assertNotIn("username",r)
 def test_legacy_session_does_not_fabricate_root_cause(self):
  r=report(
   "2026-10-31 11:05:05 [INFO] Avatar original wearable ViewerAsset: bounded HTTP attempt failed; fallback LLUDP queued\n"
   "2026-10-31 11:05:06 [WARN] Avatar original wearable rejected: owner/COF/type/transfer evidence invalid\n")
  self.assertIn("NICHT vorhanden",r)
  self.assertNotIn("simulator_absent=",r)
 def test_rate_limited_logging_has_unmodified_counters(self):
  a=read("src/app/application.cpp")
  h=read("include/nexverse/assets/stream_pressure_log_policy.hpp")
  self.assertIn("stream_pressure_warning_due(",a)
  self.assertIn("texture_budget_rejections",a)
  self.assertIn("texture_gpu_staging_deferrals",a)
  self.assertIn("total<=3U||total%64U==0U",h)
 def test_texture_pressure_warning_keeps_redaction(self):
  a=read("src/app/application.cpp")
  self.assertIn("Deferred GetTexture GPU upload [asset id omitted]",a)
  self.assertIn("total_deferrals=",a)
 def test_native_protocol_fixture_validates_real_negative_codes(self):
  t=read("tests/firestorm_wearable_transfer_tests.cpp")
  self.assertIn("info(TID,ASSET,13,-1,0)",t)
  self.assertIn("WearableTransferFailure::simulator_refused",t)
  self.assertIn("WearableViewerAssetFailure::http_not_found",t)
 def test_no_auto_simulator_wearable_write(self):
  a=read("src/app/application.cpp")
  p=a.index("// HTTP ViewerAsset can deliver wearable-original bytes")
  q=a.index("// Explicit F9 click.",p)
  self.assertNotIn("encode_agent_set_appearance(",a[p:q])
  self.assertNotIn("encode_agent_is_now_wearing(",a[p:q])
 def test_output_contains_real_actionable_status_and_no_user_data(self):
  r=report("2026-10-31 11:05:05 [WARN] Avatar original wearable ViewerAsset failure: cause=http_not_found http_status=404; omitted\n")
  self.assertIn("Wearable ViewerAsset Fehlerursachen:",r)
  self.assertIn("kein verifizierter Server-Bake",r)
if __name__=="__main__":unittest.main()
