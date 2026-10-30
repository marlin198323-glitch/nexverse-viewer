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
        "mesh_missing_unique", "texture_missing_unique")


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
                r"(parallel_peak|parallel_in_flight|disk_hits|network_loads)"
                r"=(\d+)", s)
            if fields:
                output.append(label + ": " +
                              ", ".join(f"{key}={value}" for key, value in fields))
            if "parallel_peak=" not in s:
                output.append(label + ": Altes Binary ohne Paralleltelemetrie.")
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
