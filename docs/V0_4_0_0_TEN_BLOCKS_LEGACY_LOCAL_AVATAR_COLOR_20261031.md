# OGL Viewer v0.4.0.0 – zehn Blöcke: authentische lokale Avatar-Fallback-Farben

Datum: 31.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current`. Kein Release auf main.

## Neuer authentifizierter 3D-Lauf (VOR dieser Änderung)

- **GetTexture Streaming:** real eingeloggter Benutzer, weiterhin mehrere hundert lokale Cache-Hits und erfolgreiche lokale GPU-Kompaktierungen; nach 30 Sekunden 209/237 (88 %) GPU-resident, frühere Sitzung nach 60 Sekunden **233/238 (98 % gerundet)**. Keine pauschale 100-%-Behauptung für beliebige Regionen; 404 weiterhin serverseitig offen.
- **Mesh Streaming:** 354 eindeutige GPU-Uploads, **0 duplizierte Mesh-LOD-Uploads**; frühere massive Wiederholungen dauerhaft beseitigt (weitere Liveabnahmen nötig).
- **Current Outfit Folder:** Raw InventoryDescendents2 **authentisch leer**: `valid=1`, `cof_folder_seen=1`, `cof_items_array_present=1`, `cof_raw_items=0`, `cof_rejected_items=0`. Dies ist KEIN Parser-Discard-Bug.
- **Low382:** 5 gemeldete Original-Wearables durch owner-scoped FetchInventory2 unabhängig bestätigt: `requested=5 matched=5 bodyparts=4 complete_primary=1`, Shape/Skin/Hair/Eyes sind als vertrauenswürdige Originalitems vorhanden. **Nicht** im COF verlinkt und 0/3 authentische Head/Upper/Lower Server-Bakes.
- Diese Befunde stammen vom existierenden Viewer-Prozess; für die heute erstellte neue lokale Fallback-Renderingpipeline ist ein separater erneuter Login erforderlich. Laufenden Benutzerprozess nicht beendet.

## Zehn umgesetzte technische Blöcke

1. **Authentifizierte Ursachen-Abnahme:** Live-Diagnose zeigt 5/5 vollständig vorhandene Server-Originalwearables trotz wirklich leerem COF und 0/3 Server-Bakes. Keinen Defekt der GetTexture-Pipeline vorgetäuscht.
2. **Eigener Legacy-Fallback-Provenance-Guard:** neues `legacy_wearable_local_preview.hpp` erlaubt nur lokales Self-Rendering bei 4/4 Original-Bodyparts, 0 Mismatches, identischem Agent/Session/Serial/Region, wirklich leerem COF.
3. **Server-Rohdatenbindung:** der Read-only COF-LLSD-Parser muss ein tatsächlich leeres Items-Array liefern, nicht nur 0 akzeptierte oder verworfene Items; zusätzlich aktuelle COF-Version und Regionsepoche geprüft.
4. **Original-Assets in bestehenden LLUDP-Firestorm-Pfad:** die echten bereits FetchInventory2-validierten Low382-Wearable-Originalitems bilden die Quelle des bounded LLWearable Transfers (max.16), ohne neue / erfundene Asset-ID.
5. **Lokale Original-Texturen automatisch laden:** nach vollständigen Original-Wearables darf die bestehende GetTexture-RGBA-Preview-Funktion auch diese explizit verifizierte Legacy-Quelle benutzen; nur für den eigenen Avatar, keine remote Avatare.
6. **Firestorm LAD / TexLayer-Komposition wiederverwenden:** die vorhandene Firestorm-`avatar_lad.xml`-Ebene und drei Self-Avatar-Head/Upper/Lower-GL-Preview-Slots werden auch ohne COF-Links aktiviert, wenn der originäre COF nachweislich leer ist. Reale Bakes haben weiter Vorrang.
7. **Stale-Proof / Widerruf:** Änderung des Simulator-Wearable-Serials, Konto-, Sitzungs-, Regions-, COF-Version- oder COF-Inhaltsstatus verwirft die lokalen Farben, Originaldaten und GPU-Previews; kein fremdes Skin-Artefakt nach Teleport/Re-Login.
8. **Keine Änderung des Serverinventars:** niemals automatisch `AgentSetAppearance`, `AgentIsNowWearing`, Create-Link oder COF-Batch senden; der lokale Notfall-Farbpfad ist bewusst UNBAKED und serverseitig nicht sichtbar.
9. **Exakte GPU-Telemetrie und read-only Diagnose:** die echte Anzahl erfolgreich hochgeladener Self-Avatar-Layer (0–3) wird nach OpenGL-ACK protokolliert, mit unbaked Source (= legacy vs COF), verifizierten lokalen Bildquellen und offenen Layern. `scripts/ogl-asset-diagnose.py` unterscheidet Start von echten GPU-Uploads und gibt niemals Wearable-UUIDs aus.
10. **Tests / Projektübergabe:** native tests auf Ablehnung bei nicht leerem COF, fremdem Konto, falscher Session, Region, Serial und nicht vollständigen Primärwearables; 16 zusätzliche Python-Regressionen; Roadmap/Chatübergabe mit *LIVE ABNAHME OFFEN* aktualisiert.

## Live-Abnahme nach einem sauberen Neustart

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach dem 3D-Login:

```bash
python3 scripts/ogl-asset-diagnose.py
```

In den neuen Logs nachprüfen: `Avatar legacy local preview:` -> nur **gestartet**; `Avatar local unbaked self-preview: source=authenticated_legacy_originals gpu_head_upper_lower=N/3` -> **erfolgreich lokal GPU-gerendert**, nur wenn N größer Null. Wenn keiner der echten lokalen Original-Layer dekodiert wird, bleibt der Avatar grau. Bei 0/3 `low158_refs` und 0/3 `gpu_body` fehlen die echten serverseitigen Texturen weiterhin.

## Verifizierter lokaler Build- und Teststand

- Linux-Mint-x64: `cmake --build build/debug --parallel 6` **PASS**.
- Native C++: `ctest --test-dir build/debug --parallel 6 --output-on-failure` **195/195 PASS**; neue echte Owner-/Serial-/Session-/Region-/COF-Admissonsprüfungen sind enthalten.
- Python-Regressionen: `python3 -m unittest discover -s tests -p 'test_*.py'` **1017/1017 PASS** (16 neue Local-Legacy-Self-Pipeline-Verträge).
- GitHub Source-Check-kompatibler Lauf: `bash scripts/pi5-headless-tests.sh` **PASS**, inkl. 1017 Python-Tests.
- Original X11 Branding/App-Class und 128px System-Icon: `bash scripts/test-mint-window-branding.sh` **PASS**.
- JSON-Roadmap gültig, `git diff --check` ohne Fehler; keine GitHub-main-Freigabe.
- Nachher-Vergleich: **nicht** behauptet; in aktiver Viewer-Sitzung läuft noch das vorherige Binary, also noch keine beobachteten lokalen Legacy-Layer-GL-Uploads.

## Verbleibende wesentliche Arbeiten

- Shader-/Original-Texturauflösung für großflächige Firestorm-Layer, ankommende Attachment-Objekte, echte COF-Reparatur durch autorisierte Aktion und simulatorbestätigtes AvatarAppearance/Server-Bake-Protokoll. Keine unwirksamen künstlichen UUIDs.
- Diese Änderung färbt maximal den EIGENEN Avatar lokal. Andere Benutzer sehen dadurch NICHT den fertigen Skin oder die Kleidung. Full Firestorm-Avatar-Parität ist nicht erreicht.
- Der aktuell angemeldete Benutzerprozess ist nicht neugestartet; neuer GPU-Fallback **noch nicht live verifiziert**.
- Projektfortschritt weiterhin **38,65 % gewichtet / gerundet 39 %**, Version `v0.4.0.0` IN_ARBEIT; kein Merge auf main/Release.

