#!/usr/bin/env python3
"""Validated, evidence-led OGL completion snapshot (not CI pass rate)."""
from __future__ import annotations
from decimal import Decimal, ROUND_HALF_UP
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
def load_progress():
    data = json.loads((ROOT/"docs/PROJECT_PROGRESS.json").read_text(encoding="utf-8"))
    parts = data["areas"]
    assert len({part["id"] for part in parts}) == len(parts)
    assert sum(part["weight_pct"] for part in parts) == 100
    assert all(isinstance(part["weight_pct"], int)
               and part["weight_pct"] > 0
               and isinstance(part["completion_pct"], int)
               and 0 <= part["completion_pct"] <= 100
               and part["basis"].strip() for part in parts)
    exact = sum(Decimal(part["weight_pct"])*Decimal(part["completion_pct"])
                for part in parts)/Decimal(100)
    displayed = int(exact.quantize(Decimal(1),rounding=ROUND_HALF_UP))
    return data, exact, displayed

if __name__=="__main__":
    data, exact, pct = load_progress()
    print(f"OGL-Gesamtfortschritt: ca. {pct} % "
          f"(gewichtete Schätzung, Stand {data['as_of']}, "
          f"Viewer {data['viewer_version']}).")
    print(f"Bewertungswert ungerundet: {exact} %; Summe der Gewichte: 100 %.")
    for part in data["areas"]:
        print(f"- {part['name']}: {part['completion_pct']} % "
              f"(Gewichtung {part['weight_pct']} %)")
    print(f"Nächster Zehnerdurchlauf: {data['next_five_blocks']}.")
    print("100 % setzt vollständige Integration und echte "
          "Firestorm/OpenSim-/Plattform-E2E-Tests voraus.")
