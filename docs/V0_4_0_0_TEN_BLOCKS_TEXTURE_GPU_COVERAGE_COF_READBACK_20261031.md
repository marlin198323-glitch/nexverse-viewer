# OGL Viewer v0.4.0.0 – 10 Entwicklungsblöcke, GPU-Textur-Residenz und Avatar-COF

Datum: 31.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | KEIN main-Merge.

## Tatsächlich verifizierte Live-Ausgangslage

- **Neue angemeldete Sitzung am 31.10.2026, 00:14:43**, mit dem vorherigen GitHub-Commit `44703d3`; der Nutzer hat den Viewer während dieser Arbeiten selbst gestartet. Die aktuell bearbeitete Binary wurde in dieser Sitzung **noch nicht** neu gestartet. Kein Eingriff in die laufende Benutzersitzung.
- Mesh-Fix vorheriger Batch ist **live bewiesen**: 356 unterschiedliche UUID/LOD-Meshes, **0 duplizierte Mesh-GPU-Uploads**, **0 nutzlose Mesh-LOD-Swaps**; der Vergleichslauf zuvor enthielt 368 doppelte Uploads / 366 LOD-Swaps.
- Welt-Texture-Coverage im neuen Lauf: 184/237 GPU-resident (77 %) nach 90s; weitere Nachbeobachtung 192/269 (71 %) bei neu sichtbaren Szenen. Der GPU-Texturpuffer erreichte **268435328 von 268435456 Bytes (~100 %)**. Über 397 nicht zugelassene Materialuploads, mehr als 30 schon erfolgreiche originale Mip-Rettungen; trotz 1 GiB Textur-Diskcache blieben die Assets im VRAM unvollständig.
- Authentifizierte eigene AvatarAppearance-Readback-TE: 64 Bytes, 253 visual parameters; **Head/Upper/Lower 0/3 servergültige Bakes und GPU 0/3**; fünf legacy Wearable-Einträge beweisen **nicht** das echte Current-Outfit-Folder-Outfit. Der laufende Viewer startete bereits wiederholte schreibgeschützte COF-Anfragen, aber deren Resultat war bisher nicht nachvollziehbar.
- Einige Texturen liefern tatsächlich HTTP 404: Cache, Kompaktierung und OpenGL können sie nicht ersetzen. Der Server-Upstream `opengenesislink/opengenesislinkserver` main: `1b8fb1cbbcddbf3f229879bd5046d0a1b1c72b3f`.

## Zehn tatsächlich umgesetzte Entwicklungsblöcke

1. **GitHub + Live-Log geprüft:** aktuellen `develop-current`, CI-Status, Grid-Server-Referenz, Welt-Snapshots, Textur-Bytebudget und eigenes Avatar-Bake-Defizit abgeglichen; *vorheriger* Mesh-Fix anhand realer, neuer Sitzung abgenommen.
2. **GPU-Druck-Entscheider:** neuer `choose_scene_texture_compaction` in `scene_texture_budget_rescue.hpp`: nur bei Textur-Familienbudget von mindestens 95 % plus tatsächlich weiteren anforderbaren Szene-Assets.
3. **Schonende Priorisierung:** nur **bereits GPU-residente** echte Materialbilder mit mindestens 512 KiB; erst niedrigste Szene-Priorität, maximal **eine Mip-Stufe** und bis Mip 5; keine direkten GPU-Evictions, keine erfundenen Bilddaten.
4. **Avatar und Partikel schützen:** alle authentisch ausgewählten Self-/Remote-Avatar-Body-, Eyes- und Hair-Bake-IDs sowie aktive Partikelbilder vom Kompaktierungsplan ausgeschlossen; Cache und GPU-Slot-Admission bleiben verbindlich.
5. **Saubere Transaktion:** Qualitätswechsel wird über bestehende `load_texture_cached_scoped`-RAM/Disk-/HTTP-Reihenfolge und `world_renderer->replace_object_texture` ausgeführt; alter GPU-Handle bleibt bis zu tatsächlich erfolgreichem GL-Upload.
6. **Ausstehende Mips gegen Plan-Churn:** Regionsepochengebundene `requested`-/`confirm_requested`-Verwaltung für 8s, garantiert nach dem AssetLodDwell angewendete Mip-Floors; 90s Cooldown nach GPU-Erfolg und höchstens ein Kompaktierungsversuch alle 2s.
7. **Robuster Langzeitbetrieb:** zeitlich abgelaufene Kompaktierungs-Anfragen werden aus der begrenzten Pending-Map entfernt, damit nach zahlreichen Langzeitsitzungs-Requests keine endgültige Sperre entsteht. Server-404-Backoff-Assets zählen **nicht** als durch GPU-Verkleinerung lösbare Anfragen.
8. **COF-Readback überprüfbar:** zusätzlich zur Owner-/Epoch-gebundenen, schreibgeschützten InventoryDescendents2-Nachfrage werden wirklich eingetroffene Serverantworten mit `valid`, `owner_region_current`, `returned_folders`, `cof_version_present`, `cof_item_records` sowie Fehlerstatus ohne private IDs protokolliert.
9. **CLI-Livediagnose erweitert:** `scripts/ogl-asset-diagnose.py` zeigt GPU-Auslastung, Budgetablehnungen, GPU-Kompaktierungsanfragen/Uploads, Body-Bakes, HTTP404 und Server-COF-Rückmeldung ohne Login-/UUID-/CAPS-Details.
10. **Native und Python-Regressionsfälle + Dokumentation:** C++-Regressionen für VRAM-Druck, Avatar-Ausschluss, 90s-Cooldown, 8s-Pending, 14 sequenzielle 9s-Abstände mit sauberer Freigabe; Python-Verträge zu Datenschutz, COF-Server-Readback, Epoch-Guard, 404-Sperre, Pending-Mips und GL-Transaktion; Roadmap und Chatübergabe aktualisiert.

## Unveränderte Startbefehle

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach einem authentifizierten Login: `python3 scripts/ogl-asset-diagnose.py`.

## Vollständig geprüfter lokaler Build

- `cmake --build build/debug --parallel 6`: **PASS**, inklusive Viewer-Binary.
- `ctest --test-dir build/debug --output-on-failure --parallel 6`: **195/195 PASS**.
- `python3 -m unittest discover -s tests -p 'test_*.py'`: **985/985 PASS**. 16 neue Verträge zum COF-Readback, Textur-GPU-Druck und datensparsamen Bericht.
- `bash scripts/pi5-headless-tests.sh`: **PASS**, enthält dieselben **985/985 Python-Tests** wie das GitHub-Source-Check-Gate.
- `bash scripts/test-mint-window-branding.sh`: **X11-Icon und App-Klasse PASS**.
- Release-Roadmap ist gültiges JSON, `git diff --check` ohne Fehler. Der zuvor gestartete authentifizierte Viewer-Prozess wurde nicht gestoppt oder neugestartet.

## Absichtlich offen

- Die neuen GPU-Kompaktierungsaktionen **sind noch nicht in einer nach diesem Batch neu gestarteten und angemeldeten Viewer-Binary nachgemessen**. Keine erfundene FPS-Beschleunigung, GPU-Coverage oder bestätigte Texturen.
- **Grauer, nackter Avatar ist nicht gelöst:** weder lokale CPU-/GPU-Rettung noch erfundene Bake-UUIDs können authentische, serverseitig fehlende Head-/Upper-/Lower-Bakes ersetzen. Echte Server-/Outfit-Daten und vollständige Firestorm-TeXlayer-Komposition fehlen im End-to-End-Abnahmestand.
- Die 404 fehlenden GetTexture/GetMesh-Assets müssen im Grid-/Assetserver behoben werden.
- `v0.4.0.0` bleibt `IN_ARBEIT`, **38,65 % gewichtet (~39 %)**; kein Merge/Tag/Release ohne 7 reale E2E-Belege.
