# OGL Viewer v0.4.0.0 – zehn Entwicklungsblöcke: Original-Wearable-HTTP und echter LLUDP-Rückfall

Datum: 31.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | `v0.4.0.0` IN_ARBEIT, kein main-Release.

## Verifizierter authentifizierter Live-Ausgangsbefund vor diesem Batch

- Neue angemeldete 3D-Sitzung ab **00:42:04**, vorheriger Quellcommit `d852aca`; 5/5 echte Original-Wearables via `FetchInventory2` verifiziert, 4/4 primäre Körperteile (Shape/Skin/Hair/Eyes).
- Current Outfit Folder **tatsächlich serverseitig leer**: owner-/epoch-bestätigt, `cof_items_array_present=1,cof_raw_items=0,cof_rejected_items=0`. Der Viewer startet um 00:42:08 den read-only lokalen Legacy-Farb-Fallback, aber bis zum Sitzungsende **keine erfolgreiche `Avatar local unbaked self-preview`-GPU-Telemetrie**. Kein realer Haut-/Kleidungs-Farb-Erfolgsbeleg.
- `Avatar SELF bake pipeline`: weiterhin 0/3 Head/Upper/Lower Server-Bake-UUIDs und 0/3 tatsächliche GPU-Bakes, keine autorisierte Simulator-Outfit-Änderung.
- Welttexturen in neuer Sitzung 218/237 (=92 % gerundet) nach 30s; vier einzigartige 404-GetTexture-Assets (einige Log-Events mehrfach). Meshstream 354 unterschiedliche UUID/LOD-Uploads, **0 Wiederholungen**, ohne LOD-Swaps.
- Grundsätzlicher Protokollunterschied: OpenSimulator bietet für Originalwearables ggf. `ViewerAsset` HTTP GET `?bodypart_id=<UUID>`/`?clothing_id=<UUID>` an; die bisherige Viewerimplementierung nutzte ausschließlich alten LLUDP `TransferRequest` und ließ Transfer-Fehler ohne klare Fortschrittsanzeige stehen. Das Capability-Angebot des konkret angemeldeten Grid-Servers ist **noch ungeprüft**; der neue Code fragt es erstmals optional über die reguläre SEED-CAPS-Liste an. Ein nicht angebotenes CAP ist **kein** Fehler.

## Zehn technisch erledigte Blöcke

1. **Live-Fehlerbeleg:** aktuelle authentifizierte Sitzung, COF Roharray 0, Low382 5/5, fehlende lokale GL-Head/Upper/Lower-Erfolge und 0/3 echte Bakes in den Logs nachgewiesen.
2. **OpenSim-/Firestorm-Protokollabgleich:** offizielles `ViewerAsset`-Capability als authentifizierte HTTP-Assetquelle ergänzend zum unverändert vorhandenen LLUDP-Transfer eingeplant; Original-Wearables, keine Textur-Bake-Upload-API.
3. **CAP-SEED-Erweiterung:** `FirestormRegionRead` fragt `ViewerAsset` zusätzlich an und akzeptiert ausschließlich vom aktuell authentifizierten SEED gelieferte gültige HTTP(S)-CAPs ohne fremde Fragmente/Query; URLs bleiben Secret-only RAM, kein Log.
4. **Original-Asset-URL-Grenzen:** `wearable_viewer_asset_url` akzeptiert nur echte 16-Byte-UUIDs und Bodypart/Clothing, fragt `?bodypart_id=` beziehungsweise `?clothing_id=` ab, niemals Folders/AvatarAppearance/Mutation.
5. **Abgesicherter asynchroner GET:** `fetch_wearable_viewer_asset` begrenzt auf 2 MiB Original-Wearable, Connect-/Transfer-Timeout 3s/10s, Regionslease-Guard vor und nach Netz-I/O, strikter original Firestorm `LLWearable version 1` Parser und Bodypart-vs-Clothing-Check.
6. **Bestehende Avatar-Datenpipeline wiederverwendet:** erfolgreich dekodierte HTTP-Wearables gehen durch dieselbe Owner-/COF-Version-/Asset-ID-/Inventar-Flags-/Subtype-Zulassung wie LLUDP, danach lokale Self-GetTexture/TexLayer/LLM-Vorschau; keine synthetischen Bakes, keine Remote-Avatar-Eingriffe.
7. **Gezielter LLUDP-Fallback:** wenn CAP fehlt, nimmt der Viewer sofort die bestehende LLUDP-Anfrage; wenn HTTP 404/ungültig wird, wird **maximal einmal** dasselbe bereits verifizierte Original-Item über LLUDP nachgefragt. Kein Asset-Neuanlegen und keine Endlosschleifen.
8. **Korrektes HTTP-Einmalresultat + Shutdown:** nach erfolgreicher HTTP-Resultatübernahme explizit `viewerasset_completed.reset()` (C++ `std::move(optional)` allein würde nicht leeren), HTTP-Future async nonblocking und beim Beenden sauber verbunden; Regionswechsel/Stale-Resultate werden verworfen.
9. **Transparente datensparsame Diagnose:** Source/Original-Anfrage, bestätigte HTTP-Originale, einmaliger UDP-Fallback, serverseitige 404, Transfer-Timeouts sowie akzeptierte/verwarfene Originalwearables werden ohne CAP-URL, UUID oder private Sitzungsdaten protokolliert. `scripts/ogl-asset-diagnose.py` zählt diese pro authentifizierter Sitzung getrennt; zählt keinen Bake-Erfolg ohne GPU-Upload.
10. **Native HTTP-Transport-Fakes und Python-Verträge:** echte ViewerAsset-URL für clothing/bodypart, 2-MiB-Begrenzung, Parse-Check, falscher Kategorie-Typ, HTTP-404, Region-Stale, CAP-Discovery und keine Mutationsnachrichten durch Regressionen abgesichert. Projektübergabe, Roadmap und Release-JSON aktualisiert.

## Start für E2E-Abnahme

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach authentifiziertem 3D-Login:
```bash
python3 scripts/ogl-asset-diagnose.py
```

Erwartete *Messdaten*, keine Erfolgsgarantie: `Avatar original asset transports: ViewerAsset CAP available=0/1`, `Avatar original wearable request:`, `Avatar original wearable accepted:`, `Avatar local unbaked self-preview: source=authenticated_legacy_originals gpu_head_upper_lower=N/3`. **Nur N>0 und eine tatsächlich sichtbare Färbung bestätigen die lokale Darstellung.** `low158_refs=0/3` bedeutet weiter ungelöstes Server-Bake-Problem.

## Lokaler Testnachweis

- `cmake --build build/debug --parallel 6` **PASS**, vollständige Linux-Mint-Viewer-Binary einschließlich neuer HTTP-CAP-Quelle.
- `ctest --test-dir build/debug --parallel 6 --output-on-failure` **195/195 PASS**; darunter Native Tests für ViewerAsset-URL/bodypart/clothing, ungültige Formate, HTTP-404, Timeouts und Stale-Region sowie normale LLUDP-Transfers.
- `python3 -m unittest discover -s tests -p 'test_*.py'` **1032/1032 PASS**, 15 neue Verträge für Security/Source/Fallback/Privatsphäre/Debug.
- `bash scripts/pi5-headless-tests.sh` **PASS**, erneut **1032 Python-Tests** und native Headless-Komponenten.
- `bash scripts/test-mint-window-branding.sh` **PASS**, X11-Systemicon 128px und WM_CLASS `opengenesislink-viewer`.
- `git diff --check` und JSON-Validierung der Release-Roadmap **PASS**. Der bestehende authentifizierte Viewer wurde nicht ohne Benutzereingabe geschlossen oder neu gestartet.

## Noch nicht abgeschlossen

- Neues HTTP-Asset-CAP des angemeldeten `hg.stadt-nexverse.de` ist erst nach neuem Login messbar; ein zuvor gestarteter Viewer-Prozess hat den CAP-Erweiterungscode noch nicht geladen.
- Liegt das CAP beim Grid nicht vor und antwortet LLUDP ebenfalls nicht, bleiben echte Original-Asset-Bytes und somit lokales GL-Skin unbewiesen; Server-/CAP-Konfiguration muss dann geprüft werden.
- Vollständiger Firestorm-Style/Alpha/Attachments, Simulator-Appearance-Commit, fremde Avatare, Assetserver-404 und finale Release-E2E-Gates offen.
- **38,65 % gewichtet / ~39 %**, Version `v0.4.0.0` IN_ARBEIT. Kein Tag, Merge auf main oder behaupteter Release.
