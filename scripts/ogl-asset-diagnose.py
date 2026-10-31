#!/usr/bin/env python3
"""Read-only OGL texture/mesh and real-avatar diagnostics; never print UUIDs."""
from __future__ import annotations
import json
from pathlib import Path
import re

LOG = Path.home() / ".local/state/opengenesislink/viewer.log"
KEYS = ("textures_wanted", "textures_resident", "textures_pending",
        "mesh_scene_samples", "mesh_in_flight", "texture_in_flight",
        "texture_mip0_gpu_ids", "texture_reduced_mip_gpu_ids",
        "mesh_missing_unique", "texture_missing_unique",
        "texture_coverage_pct")


def summarize(raw: str) -> list[str]:
    lines = raw.splitlines()
    sessions = [i for i, line in enumerate(lines) if
                "OpenSim login succeeded; simulator session established" in line]
    if not sessions:
        return ["Keine erfolgreiche authentifizierte Viewer-Sitzung im Log."]
    session = lines[sessions[-1]:]
    output = ["Aktuelle Log-Sitzung (nicht zwingend noch aktiv): "
              + session[0][:19]]
    snapshots = []
    for line in session:
        marker = "InWorldAssetSnapshot "
        if marker not in line:
            continue
        try:
            entry = json.loads(line.split(marker, 1)[1])
            if entry.get("schema") == "ogl.asset.streaming.v1":
                snapshots.append((line[:19], entry))
        except (ValueError, TypeError):
            continue
    if snapshots:
        when, latest = snapshots[-1]
        if latest.get("textures_wanted", 0) > 0:
            output.append("Letzte Weltprobe: " + when + " – " +
                          ", ".join(f"{key}={latest[key]}" for key in KEYS
                                    if key in latest))
        else:
            output.append("Letzte Weltprobe enthält keine angeforderten Texturen; "
                          "Neustart/Region noch nicht fertig geladen.")
    missing = [line for line in session if
               "Avatar SELF bake pipeline:" in line]
    if missing:
        record = missing[-1]
        body = re.search(r"gpu_body=(\d+)/3", record)
        refs = re.search(r"low158_refs=(\d+)/3", record)
        if body and refs:
            output.append(f"Eigener Avatar: {refs.group(1)}/3 authentifizierte "
                          f"Server-Bakes, {body.group(1)}/3 GPU-Bakes.")
            if int(refs.group(1)) < 3:
                output.append("Avatar ggf. grau/unbekleidet: Der Server liefert "
                              "Head/Upper/Lower nicht vollständig. F9 "
                              "Outfit/COF prüfen und Server-Appearance/Bake "
                              "im Grid kontrollieren. Cache kann fehlende "
                              "Bakes nicht ersetzen.")
    errors = {
        "texture404": sum("GetTexture failed" in x and "404" in x
                          for x in session),
        "mesh404": sum("GetMesh failed" in x and "404" in x
                       for x in session),
    }
    output.append("HTTP-404-Fehlerereignisse in dieser Sitzung: Textur=" +
                  str(errors["texture404"]) + ", Mesh=" +
                  str(errors["mesh404"]) +
                  " (Ereignisse, keine eindeutigen Assets).")
    texture_stats = [x for x in session if
                     "GetTexture streaming metrics:" in x]
    mesh_stats = [x for x in session if "GetMesh streaming metrics:" in x]
    for label, records in (("Texturstream", texture_stats),
                           ("Meshstream", mesh_stats)):
        if records:
            s = records[-1]
            fields = re.findall(
                r"(parallel_peak|parallel_in_flight|disk_hits|network_loads"
                r"|lod_swaps|mip_swaps|scene_mip_rescue_uploads"
                r"|scene_compaction_requests|scene_compaction_uploads"
                r"|gpu_staging_waiting|budget_deferrals|gpu_texture_bytes"
                r"|gpu_texture_byte_budget)"
                r"=(\d+)", s)
            if fields:
                output.append(label + ": " +
                              ", ".join(f"{key}={value}" for key, value in fields))
            if "parallel_peak=" not in s:
                output.append(label + ": Altes Binary ohne Paralleltelemetrie.")
            if label == "Texturstream":
                params = dict(fields)
                used = int(params.get("gpu_texture_bytes", "0"))
                limit = int(params.get("gpu_texture_byte_budget", "0"))
                if limit > 0:
                    output.append("Textur-GPU: " +
                                  str(round(100 * used / limit)) +
                                  "% des begrenzten Budgets belegt." +
                                  (" Nicht geladene Materialien koennen "
                                   "am GPU-Budget scheitern."
                                   if used >= limit - limit // 20 else ""))
    # No asset identifiers leave the log parser; only repetition counts.
    mesh_uploads = re.findall(
        r"GetMesh loaded ([0-9a-f-]{36}); requested_lod=([a-z]+)",
        "\n".join(session))
    if mesh_uploads:
        from collections import Counter
        repeats = Counter(mesh_uploads)
        duplicated = sum(count - 1 for count in repeats.values()
                         if count > 1)
        output.append("Mesh-Uploads: " + str(len(mesh_uploads)) +
                      " gesamt, " + str(len(repeats)) +
                      " verschiedene UUID/LOD-Paare, " + str(duplicated) +
                      " Wiederholungen (keine IDs ausgegeben).")
    cof = [line for line in session if
           "Avatar COF bootstrap:" in line]
    if cof:
        last = cof[-1]
        state = re.findall(
            r"(skeleton_cof_available|inventory_cap_available|"
            r"fetch_cap_available|current_region_lease)=(\d+)", last)
        if state:
            output.append("COF-Bootstrap: " +
                          ", ".join(f"{key}={value}" for key, value in state))
        else:
            output.append("COF-Bootstrap: Schreibgeschuetzte Nachfrage ausgefuehrt.")
    readbacks = [line for line in session
                 if "Avatar COF readback:" in line]
    if readbacks:
        last = readbacks[-1]
        if "read-only request failed" in last:
            output.append("COF-Readback: gescheitert (Server/CAPS prüfen).")
        else:
            parts = re.findall(
                r"(valid|owner_region_current|returned_folders|"
                r"cof_version_present|cof_item_records|cof_folder_seen"
                r"|cof_items_array_present|cof_raw_items|cof_rejected_items)"
                r"=(\d+)", last)
            if parts:
                output.append("COF-Readback: " +
                              ", ".join(f"{key}={val}" for key, val in parts))
                info=dict(parts)
                if "cof_raw_items" not in info:
                    output.append("COF-Rohdaten nicht belegt: altes Viewer-Binary. "
                                  "Null gepruefte Items beweist NICHT, dass "
                                  "der Serverordner leer ist.")
                elif info.get("cof_raw_items") != "0":
                    if info.get("cof_rejected_items") != "0":
                        output.append("COF Parser-Problem moeglich: Server "
                                      "meldet rohe Eintraege, die beim "
                                      "Pruefen verworfen wurden.")
                elif info.get("cof_items_array_present")=="0":
                    output.append("COF-Antwort ohne Items-Array: kein Beleg "
                                  "fuer einen tatsaechlich leeren Ordner.")
                elif info.get("cof_item_records") == "0":
                    output.append("COF mit authentisch leerem Items-Array: "
                                  "Viewer kann daraus keine eigene "
                                  "Kleidung/Bakes ableiten.")
    legacy = [line for line in session if
              "Avatar legacy originals readback:" in line]
    if legacy:
        details=re.findall(
            r"(valid|requested|matched|bodyparts|complete_primary)"
            r"=(\d+)",legacy[-1])
        if details:
            output.append("Simulator-Wearables / Originalinventar: "+
                          ", ".join(f"{key}={value}" for key,value in details)+
                          " (keine Server-Bake-Bestaetigung).")
    originals_http = {
        "http_requests":sum("Avatar original wearable request: "
            "authenticated ViewerAsset GET" in line for line in session),
        "http_validated":sum("Avatar original wearable ViewerAsset: "
            "HTTP source validated" in line for line in session),
        "http_fallback":sum("Avatar original wearable ViewerAsset: "
            "bounded HTTP attempt failed" in line for line in session),
        "udp_dispatches":sum("Avatar original wearable request: "
            "bounded LLUDP transfer dispatched" in line for line in session),
        "udp_timeouts":sum("Avatar original wearable LLUDP/HTTP timeout:"
            in line for line in session),
        "originals_accepted":sum("Avatar original wearable accepted:"
            in line for line in session),
        "originals_rejected":sum("Avatar original wearable rejected:"
            in line for line in session),
        "session_ram_hits":sum("Avatar original wearable RAM cache hit:"
            in line for line in session),
        "unsupported_cap_methods":sum(
            "Avatar ViewerAsset method unsupported:" in line
            for line in session),
    }
    # Categorical provenance only: no raw server response, private UUID,
    # CAP URL or account name is ever copied into diagnostic summaries.
    http_causes = ("http_not_found", "http_forbidden",
                   "http_method_unsupported", "http_server_error",
                   "http_client_error", "http_transport_error",
                   "parser_rejected", "category_mismatch",
                   "request_unavailable", "stale_region")
    udp_causes = ("simulator_absent", "simulator_refused",
                  "metadata_mismatch", "info_invalid", "packet_budget",
                  "conflicting_packet", "packet_after_final",
                  "conflicting_final", "payload_size_mismatch",
                  "wearable_parse_rejected",
                  "stale_owner_session_cof_region",
                  "original_not_current", "original_flags_missing",
                  "original_wearable_subtype_mismatch",
                  "original_inventory_asset_type_mismatch")
    http_errors = []
    udp_errors = []
    for line in session:
        if "Avatar original wearable ViewerAsset failure:" in line:
            cause = re.search(r"\bcause=([a-z_]+)", line)
            status = re.search(r"\bhttp_status=(\d+)", line)
            if cause and cause.group(1) in http_causes:
                http_errors.append(cause.group(1))
                # HTTP number only, not server text or URL.
                if status and int(status.group(1)) <= 599:
                    http_errors[-1] += "(" + status.group(1) + ")"
        elif "Avatar original wearable rejected:" in line:
            cause = re.search(r"\bcause=([a-z_]+)", line)
            if cause and cause.group(1) in udp_causes:
                udp_errors.append(cause.group(1))
    if http_errors:
        from collections import Counter
        errors=Counter(http_errors)
        output.append("Wearable ViewerAsset Fehlerursachen: " +
                      ", ".join(f"{k}={v}" for k,v in sorted(errors.items())) +
                      "; HTTP-Antwort ist kein verifizierter Server-Bake.")
    if udp_errors:
        from collections import Counter
        errors=Counter(udp_errors)
        output.append("Wearable LLUDP/Original-Pruefung: " +
                      ", ".join(f"{k}={v}" for k,v in sorted(errors.items())) +
                      "; kein lokaler GPU-Erfolg abgeleitet.")
    if not http_errors and originals_http["http_fallback"]:
        output.append("Wearable-HTTP-Fehlerursache im alten Viewer-Log "
                      "NICHT vorhanden: neuen Viewer starten.")
    if not udp_errors and originals_http["originals_rejected"]:
        output.append("Wearable-LLUDP-Ablehnungsursache im alten Viewer-Log "
                      "NICHT vorhanden: neuen Viewer starten.")
    cap = [line for line in session if
           "Avatar original asset transports:" in line]
    if cap:
        c = re.search(r"ViewerAsset CAP available=(\d+)", cap[-1])
        if c:
            output.append("Wearable-HTTP-CAP angeboten: " +
                          ("ja" if c.group(1)=="1" else "nein") +
                          "; nur schreibgeschuetzte Originalassets.")
    if any(originals_http.values()):
        output.append("Wearable-Originaltransfer: " +
                      ", ".join(f"{key}={value}"
                                for key,value in originals_http.items()) +
                      "; kein Server-Bake-Beleg.")
    local = [line for line in session
             if "Avatar local unbaked self-preview:" in line]
    if local:
        values = re.findall(
            r"(source|gpu_head_upper_lower|real_local_source_textures"
            r"|incomplete_layers)=([A-Za-z0-9_/]+)", local[-1])
        if values:
            state = dict(values)
            output.append("EIGENER Avatar – lokale Teilvorschau: " +
                          ", ".join(f"{key}={value}" for key,value in values))
            if state.get("source")=="authenticated_legacy_originals":
                output.append("Lokale Original-Wearables aus dem Simulator "
                              "sichtbar gemacht; KEINE vom Grid "
                              "bestätigten Body-Bakes, keine fremden Avatare.")
    elif any("Avatar legacy local preview:" in line for line in session):
        output.append("Lokale Low382-Vorschau wurde gestartet; "
                      "erfolgreiche GPU-Head-/Upper-/Lower-Uploads "
                      "noch NICHT nachgewiesen.")
    return output


def main() -> int:
    try:
        with LOG.open("rb") as source:
            source.seek(0, 2)
            length = source.tell()
            source.seek(max(0, length - 8_000_000))
            raw = source.read().decode("utf-8", errors="replace")
    except OSError:
        print("Viewer-Log noch nicht vorhanden.")
        return 0
    for text in summarize(raw):
        print(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
