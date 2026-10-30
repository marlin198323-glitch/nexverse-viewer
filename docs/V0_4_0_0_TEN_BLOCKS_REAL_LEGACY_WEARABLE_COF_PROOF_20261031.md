# OGL Viewer – 10 Blöcke: Legacy-Originalwearables und COF-Rohdaten, 31.10.2026

Branch: `develop-current` | Version: `v0.4.0.0` IN_ARBEIT | Kein Release-Merge auf main.

## Live-Befund VOR den neuen Änderungen (authentifizierter Lauf)

- **GetTexture GPU:** Snapshot 233/238 = 97,9 % tatsächliche GPU-Residenz in Sitzung vom 31.10.2026 00:21:40. 13 echte, erfolgreich commitete GPU-Texturkompaktierungen, keine Doppeluploads derselben Mesh-/LOD-Kombination, 356 GPU-Meshes ohne falsche LOD-Swaps.
- **Avatar:** ein authentifiziertes Low158 Appearance-Paket, aber 0/3 Head/Upper/Lower Server-Bake-UUIDs und 0/3 GPU-Körpertexturen. Er bleibt grau bzw. unbekleidet.
- **COF:** Owner-/Region-bestätigte FetchInventoryDescendents2-Antwort, ein Ordner, `cof_item_records=0` NACH dem bisherigen Parser. **Wichtig:** Bisher wurde nicht protokolliert, ob die unverarbeiteten Server-Rohdaten tatsächlich ein leeres Items-Array enthalten oder ob Inhalte an strengen Parser-Gates scheiterten. Es wäre unzulässig, die Serverinventardaten allein wegen 0 verarbeiteter Einträge als sicher leer zu bezeichnen.
- **Low382:** Simulator meldet 5 getragene Einträge; ohne Prüfung von Inventar-Asset-UUIDs und Wearable-Typen beweist das keine echten Skin-/Kleidungsdaten. Kein Zugriff auf Serverdatenbank oder heimliche AgentSetAppearance/AgentIsNowWearing-Mutationen erfolgt.
- `opengenesislink/opengenesislinkserver` GitHub main SHA `1b8fb1cbbcddbf3f229879bd5046d0a1b1c72b3f`, unverändert.

## 10 implementierte Entwicklungsblöcke

1. **Live-Abnahme 97,9 % Material-Residenz:** letztes Commit `6ebf6fd` anhand neuer authentifizierter Welt-Snapshots und GetTexture-Kompaktierungsstatistik erfolgreich bewertet; nur 5 von 238 Texturen noch ohne GPU-Beleg; visueller Screenshot bleibt offen.
2. **COF-Defizit präzisiert:** echtes COF-Readback zeigt null parserakzeptierte Originalitems. Fehlerursachen in Serverantwort, Parser und getrenntem Simulator-AvatarAppearance müssen unterscheidbar bleiben.
3. **Read-only Low382-Probe:** neuer `legacy_wearable_inventory_probe.hpp` aus authentifizierten Low382-Wearable-IDs, mit echter UUID-Validierung, agent-/session-bound, 17 Slots, ohne Null-IDs/Duplikate/Typkonflikte.
4. **Exakter Wearable-Originalnachweis:** `audit_legacy_wearable_inventory_probe` verlangt Originalinventar item-id, asset-id, inventory_type 18, asset_type 13/5 und wearable-subtype flags & 255; zählt verifizierte Bodyparts (Shape/Skin/Hair/Eyes) 0–4. Vier verifizierte Items sind KEIN Beleg für die 3 finalen Bakes.
5. **Asynchrone Originalitems-Abfrage:** nur wenn das aktuelle authentifizierte COF nach dem Parser leer ist, startet der Viewer max. drei read-only FetchInventory2-Lookups pro Region, nur bei einem neu bestätigten LLUDP-Wearable-Serial. Kein UI-/Render-Thread-HTTP-Warten.
6. **Stale-Reply- und Shutdown-Schutz:** Antwortaufnahme nur für identische Agent-ID, Session-ID, Regions-Epoche und Wearable-Serial; ausstehende Async-Jobs werden beim Shutdown verbunden, nach Regionwechsel keine alten Originals übernommen.
7. **COF-Rohdaten-Validierung:** `OutfitInventoryRead` unterscheidet `cof_folder_seen`, `cof_items_array_present`, `cof_raw_items`, `cof_rejected_items`. Parser akzeptiert weiterhin ausschließlich fremd-ID-geschützte, gültige Items. Ein rohes Item, das an Feldern scheitert, wird nicht als serverseitig leer ausgegeben.
8. **Wahrheitsgemäßer UI-Status:** graues Avatarbild = 0/3 Körper-Bakes; nur die *geprüften* COF-Daten sind leer. F9 kann nun den getrennt geprüften Legacy-Originalstatus anzeigen; weder originäre Skin noch Outfit werden fingiert.
9. **GPU-Qualitätsschutz:** der Szene-Mip-Kompaktor schützt ausdrücklich auch Eyes-/Hair-Bakes aller aktuell zugelassenen Self-/Remote-Avatare sowie ihre Body-Bakes und Particle-Texturen vor Welttextur-Reduktion.
10. **Diagnose, Native-/Python-Regressionen, Dokumentation:** `ogl-asset-diagnose.py` berichtet rohe vs. verworfene COF-Einträge, Legacy-Originalcounts, fehlende echte Bakes und vorherige GPU-Metriken ohne Agent-IDs, Item-IDs, Passwörter oder CAPS-URLs. Native Tests prüfen leere/verworfene COF-Rohdatensätze sowie Item-/Asset-/Subtype-Mismatches; 16 zusätzliche Python-Verträge und die Projektübergabe aktualisiert.

## Verbindlicher Linux-Mint-Viewerstart

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach einem frischen authentifizierten Login zusätzlich:

```bash
python3 scripts/ogl-asset-diagnose.py
```

Prüfen: `Avatar COF readback:` mit `cof_raw_items`, `cof_rejected_items`; `Avatar legacy originals readback:` mit `requested/matched/bodyparts`; `Avatar SELF bake pipeline:` mit echten `low158_refs/gpu_body`.

## Nachvollziehbar bestandene lokale Tests

- Linux-Mint-x64 Full Viewer C++ Build: **PASS**, `cmake --build build/debug --parallel 6`.
- Native CTest: **195/195 PASS**, inklusive Owner-/Serial-/Bodypart-Abweisungen und tatsächlichem LLSD-COF-Roharray-Test für leer vs. verworfen.
- Python-Unittest: **1001/1001 PASS**, 16 zusätzliche COF-/Legacy-/Privatsphäre-/Avatar-/GPU-Schutz-Verträge und alle bestehenden Release-Gates.
- GitHub-Source-Check-kompatibles `bash scripts/pi5-headless-tests.sh`: **PASS**, ebenfalls 1001 Python-Tests und native Headless.
- X11-Systemfenster-Branding und 128px Viewer-Icon: **PASS**.
- `git diff --check`, JSON-Validität der Release-Roadmap: **PASS**.
- **Noch nicht getestet:** tatsächlicher neuer Low382-FetchInventory2-Fallback mit neu gestartetem authentifizierten Viewer. Laufende Benutzersitzung wurde nicht beendet.

## Nicht erledigt / weitere Priorität

- **0/3 echte Head/Upper/Lower-Bakes sind weiterhin offen**. Ein leerer authentischer COF-Server-Ordner benötigt Robust-/Grid-Daten oder eine autorisierte echte Outfit-Aktualisierung; der Viewer wird nicht ohne Einverständnis Kleidung verändern.
- Selbst wenn Low382-Originale vorhanden sind, muss die native Shape/Skin/Clothing-Texturkomposition und das serverseitige Appearance-Commit unabhängig End-to-End geprüft werden. Die Probe ist ausdrücklich kein sichtbarer Avatar-Bake.
- Das neue Original-Lookup/COF-Raw-Logging ist noch **nicht nach dem erneuten Viewer-Neustart unter authentifizierter Sitzung gemessen**. Laufende Sitzung bleibt unverändert.
- HTTP-404-Mesh-/Texturassets werden nicht durch Cache oder Client gerendert. Welttextur-Vollauflösung/realer Frame-Rate-Vergleich noch ausstehend.
- **Projektstand 38,65 % gewichtet (~39 %)** bleibt, `v0.4.0.0` IN_ARBEIT, kein main-Release.
