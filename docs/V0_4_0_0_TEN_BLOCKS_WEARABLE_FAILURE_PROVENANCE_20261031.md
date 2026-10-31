# OGL Viewer v0.4.0.0 – zehn Blöcke: Original-Wearable-Fehlerursachen und GPU-Diagnose

Datum: 31.10.2026 | Linux Mint jam-MS-7D28 | Branch develop-current, kein Release

## Echte Live-Ausgangsdaten vor diesem Update

Der Benutzer startete das vorherige Binary mit Git-SHA `e37a091` um **10:59:52**. Der Viewer wurde um **11:00:59 ordnungsgemäß beendet**, es gab keinen Crash.

- **ViewerAsset CAP available=1**, Originalinventar über den tatsächlich eingegangenen `AgentWearablesUpdate` und owner-scoped `FetchInventory2` geprüft: **5/5 authentifizierte Wearables**, darunter Shape/Skin/Hair/Eyes **4/4**. Die Raw-COF-Antwort hatte `cof_items_array_present=1`, `cof_raw_items=0`, `cof_rejected_items=0`: Serverordner authentisch leer.
- **5/5 HTTP-ViewerAsset-Original-Abrufe scheiterten**, **5/5 LLUDP-Original-Abrufe wurden unmittelbar abgelehnt**, **0 erfolgreiche dekodierte Original-Wearables**, **0 lokale Self-Avatar-Layer GL-Uploads** und weiterhin **0/3 echte Server-Body-Bakes**. Der letzte Viewer protokollierte nur den generischen `owner/COF/type/transfer evidence invalid`-Fehler; er unterscheidet NICHT HTTP-Status, LLUDP -2, Asset-Typ-/Parserfehler oder echte Regionstaleness. Keine Ursache über die vorhandenen Fakten hinaus erfinden.
- Aktueller Weltstream: 262/278 Texturen GPU-resident (**94 %**), dabei 233 reduzierte Mips und nur 29 Mip-0-Texturen; Meshes **384/384 (100 %)**, null duplizierte UUID/LOD-Uploads.
- **GetTexture**: 4 HTTP-404-Fehlereignisse, 6 ungültige J2C-Ereignisse (5 `channel_count`, 1 inkonsistente Tile-Part-Length), 592 wiederholte GPU-Budget-Deferred-Warnungen, 583 GPU-Budget-Deferrals, GPU 268435404 von 268435456 Bytes belegt (**nahezu 100 %**). Keine Freigabe zum Erfinden von Assets, Serverbakes oder höheren GPU-Budgets.
- GitHub `e37a091` Source-Check success, vollständige CI zum Beginn dieses Batches noch in_progress. Repo `opengenesislinkserver` Referenzversion `1b8fb1cbbcddbf3f229879bd5046d0a1b1c72b3f`.
- OpenSimulator dokumentiert `ViewerAsset` als Main-Fetch-CAP, siehe https://opensimulator.dev/wiki/Capabilities ; query `?bodypart_id=`/`?clothing_id=` ist als OpenSim-ViewerAsset-Form belegt, siehe https://docs.rs/metaverse_objects/latest/metaverse_objects/http_handler/index.html. Das allein beweist NICHT, dass ein konkretes Asset vom Grid ausgeliefert wird.

## Die zehn erledigten Entwicklungsblöcke

1. **Live-Fakten eingefroren:** 5/5 HTTP-Fallback, 5/5 sofortige LLUDP-Rejections, 0/3 Server-Bakes, Textur-/Mesh-RAM-/GPU-Probe und letztes reguläres Viewer-Shutdown. Keine falsche Ursachenbehauptung.
2. **Typed LLUDP-Transferfehler:** `WearableTransferFailure` in der tatsächlichen `WearableTransferInbox`: `simulator_absent`, `metadata_mismatch`, `info_invalid`, Paketlimit, widersprüchlicher/zu später Packet, mehrfacher Final, Parser-Fehler. Die genaue Kategorie bleibt ohne IDs für die Hauptschleife verfügbar.
3. **Alternative negative Statuswerte:** `TransferInfo` mit negativen Statuswerten -1..-16 und 0 Bytes wird unmittelbar und bounded abgelehnt. `-2` ist eindeutig `simulator_absent`; andere negative Werte `simulator_refused`, niemals fälschlich als fehlendes Asset behaupten.
4. **Vollständiger Transfer mit inkonsistenter Länge:** Bei lückenloser Empfangsfolge und vom `TransferInfo` abweichender Gesamtzahl Bytes sofort `payload_size_mismatch` statt unzutreffendem 12s-Timeout. Spät ankommende Out-of-order-Pakete sind weiterhin erlaubt.
5. **Typed ViewerAsset HTTP-/Parsergründe:** `WearableViewerAssetFailure` trennt HTTP 404, 401/403, 405/501, 5xx, andere 4xx, Transport, ungültiges Wearable-Format, Bodypart-/Clothing-Typkonflikt und veraltete Region. Nur verifizierte Statusnummern und statische Ursache, kein URL-/Body-/Token-Logging.
6. **Hauptschleife mit echtem Fehlerbeleg:** Die fünf ursprünglichen generischen `owner/COF/type/transfer`-Ablehnungen unterscheiden künftig aktuelle Login-/Region-/COF-Lease, fehlendes Inventaritem, fehlende Flags, falschen Wearable-Subtype, falschen Inventarasset-Typ sowie die echten LLUDP-Transferversager. Keine versehentliche Annahme nicht verifizierter Wearables.
7. **Diagnose auf aktuelle Sitzung begrenzt:** `scripts/ogl-asset-diagnose.py` extrahiert nur statisch freigegebene Kategorie-Labels und max. HTTP-Status 599; liefert Gruppenzahlen ohne UUID, private URLs oder vom Server gelieferte Rohtexte.
8. **Rückwärtskompatible Ungewissheit:** Bei alten Viewer-Logs ohne typed Gründe wird explizit `NICHT vorhanden: neuen Viewer starten` angezeigt. Keine erfundene 404-Erklärung und kein Avatar-GL-Erfolg.
9. **Deutlich weniger GPU-Budget-Logflut:** Neue pure `stream_pressure_warning_due` meldet die ersten 3 GPU-Druckereignisse und anschließend nur jedes 64. Mal. **Alle** Budget-Deferrals werden weiter gezählt; Referenzen, Renderer-Upload-Limits und Qualität werden nicht geändert.
10. **Native- und Python-Regressionen plus Projektübergabe:** C++-Tests decken authentischen Transfer und Paketfolge, -2/weitere negative Statuswerte, Byte-Mismatch, HTTP 403/404/405/500/501 und die unveränderte GPU-Druck-Event-Statistik. Neue Python-Verträge prüfen Diagnose, Whitelist, Privacy, Scope, Fehlertypen; alte Contracts an genauere Semantik angepasst.

## Nächste E2E-Abnahme

Das laufende Benutzerkonto/der Viewer wurde NICHT eigenmächtig gestartet. Regulär erneut den neuesten Build öffnen:

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach dem 3D-Login:

```bash
python3 scripts/ogl-asset-diagnose.py
```

Jetzt erwartet: 
- `Wearable ViewerAsset Fehlerursachen:` mit z. B. `http_not_found(404)`, `http_forbidden(403)` oder `parser_rejected`, **nur, wenn die echte neue Serverantwort genau diese Ursache liefert**.
- `Wearable LLUDP/Original-Pruefung:` mit z. B. `simulator_absent` oder `metadata_mismatch`, **nur nach tatsächlich empfangener bestätigter Transferantwort**.
- Erst nach `Avatar original wearable accepted:` und `Avatar local unbaked self-preview: ... gpu_head_upper_lower=N/3` mit N>0 ist ein lokaler Selbstavatar-GL-Layer nachgewiesen. Serverbakes weiterhin separat.
- 404 oder -2 können auch an falschem Cap-/Serverrouting liegen; selbst gleichzeitige Rückgaben sind kein Freibrief für ungeprüfte Ersatzassets. Wenn Serverantworten tatsächlich fehlen, Assetstore-/Caps- und OpenGenesisLINK-Server-Konfiguration prüfen.

## Verifizierte Linux-Mint-Abnahme

- `cmake --build build/debug --parallel 6`: **PASS**, komplettes GUI-Binary mit typed HTTP/LLUDP-Diagnose.
- `ctest --test-dir build/debug --parallel 6 --output-on-failure`: **195/195 PASS**, inklusive echter LLUDP-Paketsequenzen, -1/-2-Antworten, HTTP 403/404/405/500/501, verifiziertem Packet-Byte-Length-Mismatch sowie GPU-Rate-Limit-Policy. Ein OpenGL-Regionwechseltest scheiterte bei einem ersten Lauf mit gleichzeitiger Python-Belastung, bestand isoliert und im nachfolgenden kompletten 195/195-Lauf; **keinen 100-%-Testnachweis aus dem zunächst fehlerhaften Lauf behaupten**.
- `python3 -m unittest discover -s tests -p 'test_*.py'`: **1070/1070 PASS**, darunter 21 neue Source-/Privacy-/Reason-Diagnose-Fälle und aktualisierte ältere Verträge.
- `bash scripts/pi5-headless-tests.sh`: **PASS**, inklusive erneut **1070/1070 Python-Tests** und allen Native-Headless-Komponenten.
- `bash scripts/test-mint-window-branding.sh`: **PASS**, Systemicon 128x128 und WM_CLASS `opengenesislink-viewer`.
- `git diff --check` und JSON-Release-Roadmap: **PASS**. Der bisherige Live-Userprozess war regulär beendet; kein automatisches Login, keine Simulator-Mutation.

**Fortschritt weiterhin 38,65 % gewichtet / rund 39 %, Version v0.4.0.0 IN_ARBEIT. Kein main-Merge oder Release.**
