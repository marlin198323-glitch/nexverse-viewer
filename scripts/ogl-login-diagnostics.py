#!/usr/bin/env python3
"""Read-only OGL login failure summary without exposing XML-RPC secrets.

Usage: python3 scripts/ogl-login-diagnostics.py [--all-sessions] [--log PATH]
Default: only failures since the last Viewer startup. No network access,
server changes, credentials, login URLs, usernames or log bodies are printed.
"""
from __future__ import annotations

import argparse
from collections import Counter
from pathlib import Path
import re

EVENT = re.compile(
    r"^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\b.*"
    r"OpenSim login failed, (?:reason|category):\s*([^\s]+)"
)
START = "Starting OpenGenesisLINK Viewer"
SAFE = {
    "presence", "presence_already_online", "presence_unknown",
    "presence_unverified", "presence_restricted", "key", "user",
    "grid", "inventory", "tos", "login_failed", "http_error",
    "invalid_xml", "invalid_response", "incomplete_session",
    "invalid_login_uri", "transport_unavailable", "missing_credentials",
    "other",
}
HINTS = {
    "presence_already_online": "Avatar gilt serverseitig noch als online.",
    "presence_restricted": "Gridserver schraenkt Anmeldungen ein.",
    "presence_unverified": "Gridserver verlangt Kontobestaetigung.",
    "presence": "Presence-Code ist mehrdeutig; ROBUST-Loginlogs pruefen.",
    "presence_unknown": "Presence-Code ist mehrdeutig; ROBUST-Loginlogs pruefen.",
}


def summarize(content: str, *, latest_session: bool = True) -> tuple[list[tuple[str, str]], Counter]:
    lines = content.splitlines()
    if latest_session:
        starts = [i for i, line in enumerate(lines) if START in line]
        if starts:
            lines = lines[starts[-1]:]
    entries: list[tuple[str, str]] = []
    for line in lines:
        match = EVENT.match(line)
        if not match:
            continue
        category = match.group(2)
        # The network-supplied reason is untrusted, even in historical
        # viewer logs. Keep only literal known category labels.
        if category not in SAFE:
            category = "other"
        entries.append((match.group(1), category))
    return entries, Counter(category for _, category in entries)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument(
        "--log", type=Path,
        default=Path.home() / ".local/state/opengenesislink/viewer.log")
    ap.add_argument(
        "--all-sessions", action="store_true",
        help="Include previous viewer startups rather than only latest")
    args = ap.parse_args()
    try:
        content = args.log.read_text(encoding="utf-8", errors="replace")
    except OSError as error:
        # Do not echo arbitrary paths or original log contents.
        print(f"Viewer-Log konnte nicht gelesen werden: {type(error).__name__}")
        return 2
    entries, counts = summarize(content, latest_session=not args.all_sessions)
    print(f"Viewer-Anmeldefehler: {len(entries)} "
          f"({'alle Starts' if args.all_sessions else 'letzter Start'})")
    for category, number in sorted(counts.items()):
        print(f"  {category}: {number}")
    for date, category in entries[-12:]:
        print(f"  {date}: {category}")
    if any(key in counts for key in ("presence", "presence_unknown")):
        print(HINTS["presence"])
    for category in ("presence_already_online", "presence_restricted",
                     "presence_unverified"):
        if category in counts:
            print(HINTS[category])
    if not entries:
        print("Keine abgelehnten XML-RPC-Anmeldungen im ausgewaehlten Logabschnitt.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
