# OpenGenesisLINK Viewer – Chat-Übergabe

Stand: 10.10.2026

## 1. Projekt

- Produkt: **OpenGenesisLINK Viewer**
- Zweck: eigener OpenSim/OpenGenesisLINK-Viewer für NexVerse
- Sprache: C++23
- kein Firestorm-Fork; Firestorm/Second Life nur Referenz
- Repo: `marlin198323-glitch/nexverse-viewer`
- lokaler Arbeitsbranch: `develop-current`
- GitHub-Handoff-Branch: `develop-current`
- Hinweis: GitHub-`develop` besitzt eine ältere divergierende Historie; **nicht** als aktuellen Stand verwenden, bis sie bewusst reconciled wurde.
- lokal: `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`
- vcpkg: `/home/jam/vcpkg`
- `projectplan.md` ist bindend

## 2. Neuer Chat – zuerst

```bash
cd '/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer'
git status --short --branch
git log -8 --oneline
cat VERSION
```

Wenn direkt über GitHub gearbeitet wird, zuerst Branch `develop-current` verwenden. Danach lesen: `CHATUEBERGABE.md`, `projectplan.md`, `info.md`, `docs/ARCHITECTURE.md`, `docs/DESIGN.md`.

Keine destruktiven Resets/Cleans gegen den lokalen Stand.

## 3. Aktueller Code-Stand

Version: `0.2.0-alpha.49`

Letzter wesentlicher Codecommit vor den Planungscommits:
`6f4d9c4 feat: support OpenSim planar texture mapping`

Vorhanden:
- Phase 1 Grid Discovery
- Phase 2 echter OpenSim Login
- Phase 3 echter LLUDP-Circuit
- Phase 4 CAPS/EventQueue/SimulatorFeatures
- Phase 5 WorldModel
- LayerData Terrain
- Terrain-Detailtexturen
- EEP/WindLight/ExtEnvironment
- sichtbare Prim-Grundformen und dynamische Prim-Geometrie
- OpenSim Face-Ranges
- TextureEntry Default/per-Face
- explizite Prim-UVs
- Planar TexGen
- Fullbright/Shiny/Bumpiness/Media-Daten im Renderer-Vertrag

Letzter vollständiger Teststand: 23/23 Tests grün plus OpenGL Runtime-Smoke; In-World-Sichttest noch offen.

## 4. NexVerse Runtime

- Grid: `hg.stadt-nexverse.de`
- Discovery: `http://hg.stadt-nexverse.de/get_grid_info`
- Welcome: `https://stadt-nexverse.de/service/welcome`
- Discovery bei jedem Start
- In-World-UI erst bei Simulation Ready
- Start maximiert, normales dekoriertes Fenster
- F11 exklusiver Fullscreen
- Widescreen responsiv

## 5. Module Runtime: erster Basisblock implementiert

Version `0.2.0-alpha.38`:

- `NexVerse::Modules` als Core-Library
- öffentliche C-ABI v1 und Lifecycle `ogl_module_get_api`, init/start/stop/shutdown
- Manifest Loader, lokale Modulerkennung, Plattform-/Architektur- und Versionscheck
- Dependencies inklusive Zyklusprüfung, deklarierte Capabilities
- dynamische Bibliotheken, Start-Rollback, umgekehrtes Stoppen
- Aktivieren/Deaktivieren und persistenter Aktivierungszustand
- OGL Example Module als Testmodul
- Debug-Build und 20/20 Tests bestanden, OpenGL Runtime-Smoke erfolgreich

Sicherheitsgrenze: **Nicht signierte native Plugins werden standardmäßig blockiert**.
Developer Mode muss von der Hostanwendung explizit erlaubt werden und alle
Capabilities freigeben. Signatur-/Hashprüfung, sichere Isolation und
Nutzer-Modulverwaltung fehlen noch. Details: `docs/MODULES.md`.

Version `0.2.0-alpha.39` (aktueller Stand):
- ABI v1 unverändert; zusätzlich ABI v2 mit `ogl_host_api_v2`,
  `ogl_diagnostics_snapshot_v1` und Main-Thread `on_frame`-Callback
- OGL Developer Tools und OGL Performance Monitor als echte dynamische
  Testmodule mit Dependency, Lifecycle und read-only Host-Metriken
- `--dev-modules` als ausdrückliche CLI-Freigabe für lokal installierte,
  bereits aktivierte unsignierte Developer-Module; Standard weiter blockiert
- Metriken: FPS, geglättete Framezeit, Draw Calls, Triangles, Objects,
  RAM/VRAM, `simulation_ready`, Frameindex
- Logs im geladenen Viewer erfolgreich; 21/21 Tests grün inklusive ABI
  v1/v2 und Dependency-Test; OpenGL-Runtime-Smoke mit Developer-Modulen OK

**Nächster verbindlicher Arbeitsblock: Phase 8** gemäß `projectplan.md`:
Prims/Materialien/Scene-Darstellung vervollständigen; anschließend
Mesh/Sculpt/Partikel/Lichter. Kein Vulkan-/RTX-/Weather-/Hydro-Vorgriff.
Die Entwickler-Module sind aktuell reine Log-/Diagnosemodule ohne UI,
ohne Produktions-Signaturfreigabe und ohne Sandbox. Anleitung: `docs/MODULES.md`.

## 6. Plugins / Add-ons

Offizielle spätere Registry: `https://addons.opengenesislink.de/`.

Der Zielkatalog umfasst aktuell 38 Plugin-/Modulideen. Online-Store, Download, `.oglpkg`, Signaturen und automatische Updates folgen später.

## 7. Outfit / Text-only / Weather

Outfit-Basis bleibt Core.
Plugins: `OGL Outfit Manager`, `OGL Outfit Preview`.

Text-only ist Core-Modus.

Reale Tag-/Nachtzeit ist Core-Environment-Override.

`OGL Weather` ist Plugin für Regen, Schnee, Nebel, Sturm und Wind; RTX nur optional.

## 8. NextGen Renderer

`OGLRender` ist die Render-Abstraktion.
OpenGL 3.3 bleibt zunächst Legacy/Fallback.
Vulkan wird langfristig primär.
RTX/Hybrid-Raytracing bleibt optional.

Geplant: PBR, HDR, moderne Schatten/Reflexionen, Terrain, Wasser, Atmosphäre, Post Processing, Streaming, GPU Culling, Upscaling.

## 9. HydroEngine

Einstufung: **SPÄTER VORBEREITEN**.

HydroEngine ist serverautoritativ. Der Viewer soll später dynamische Wasserzustände empfangen und darstellen. `region.water_height` bleibt kompatibel. Keine Hydro-Solver-Implementierung jetzt.

## 10. Zeitplanung

Grobe Planungswerte, keine festen Zusagen:

- nutzbare Alpha: 3–6 Monate
- brauchbare Beta: 6–9 Monate
- weitgehender Firestorm-Ersatz: 9–15 Monate
- vollständige langfristige OGL-Plattform: 12–24 Monate

Core-Alltagstauglichkeit kommt vor Vulkan/RTX/Weather/Hydro-Ausbau.

## 11. Build / Test

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
cmake --preset debug
cmake --build --preset debug -j2
ctest --preset debug --output-on-failure
```

Runtime-Smoke:

```bash
timeout 4s env DISPLAY=:0 ./build/debug/nexverse-viewer >/tmp/ogl-run.log 2>&1
cat /tmp/ogl-run.log
```

Timeout 124 ist bei erfolgreichem Start durch `timeout` normal.

## 12. Verhalten im neuen Chat

Wenn der Benutzer nur **„viewer“** oder **„weiter“** schreibt:

1. lokalen Repo-Stand prüfen
2. diese Übergabe + Projektplan lesen
3. keine unnötige Rückfrage
4. Phase 8: Renderer/Prims/Material/Scene weiterbauen
5. build + tests, bei GUI-Änderungen Runtime-Smoke
6. nicht signierte Module ohne ausdrücklichen Developer Mode niemals laden
7. `info.md` und `CHATUEBERGABE.md` aktualisieren
8. Commit und GitHub-Handoff-Branch synchron halten
9. Remote-Stand niemals destruktiv über lokale Arbeit legen

## Phase 8: laufender Codeblock (09.10.2026)

0.2.0-alpha.40: erste Legacy-Shiny-Darstellung pro Objekt/Face im
OpenGL-Fragmentshader, kein vollständiger PBR-/Normal-Map-Pfad.
21/21 CTest grün. Weiter mit Material-/RenderMaterial-Auswertung
und OpenSim-Facegruppen. `projectplan.md` bleibt maßgeblich.


## Neuester Rendering-Block · 0.2.0-alpha.41

- Getrennte deckende/transparente Material-Durchläufe.
- RGBA8-Alpha-Analyse bei Texture-Upload, kein pauschales Blending
  deckender Prim-Texturen.
- Transparent-Objekte stabil back-to-front sortiert;
  transparentes Depth-Write deaktiviert.
- Default `material_id` / `render_material_id` werden jetzt an
  `RenderSceneObject` übergeben, Face-Inheritance durch Tests abgesichert.
- `src/render/material_pass.cpp` + Header + `material_pass_tests.cpp`.
- 22/22 Tests grün, OpenGL-Viewer-Start OK, In-World-Vergleich offen.

**Nächster Schritt:** tatsächliche Legacy-/PBR-Material-Auflösung
aus OpenSim/CAPS anhand validierter Protokollantworten; keine
erfundenen Shader-Parameter aus der Material-UUID ableiten. Als
Fallback bleiben die bestehenden OpenSim-TextureEntry-Eigenschaften.
`projectplan.md` bleibt verbindlich.


## Neuester Legacy-Materials-CAPS-Block · 0.2.0-alpha.42

- Verifizierter OpenSimulator `RenderMaterials` GET: LLSD-XML `Zipped`
  -> zlib -> LLSD Binary Array mit Material-UUIDs und Definitionen.
- Begrenzter Parser, async GET nach CAPS Discovery; Fallback bei
  fehlender/fehlerhafter Capability. 23/23 Tests grün, OpenGL-Start OK.
- DiffuseAlphaMode (0-3) und AlphaMaskCutoff werden in Scene- und
  RenderFace-Daten übertragen; Renderer unterscheidet Mask/Blend/
  Opaque/Emissive und berücksichtigt Depth-Write.
- Noch KEIN vollständiges Live-Update per POST, kein GLTF/PBR-Asset-Fetch,
  keine Normal-/Specular-Maps, kein in-world End-to-end-Test.
- Nächster Schritt: MaterialIDs gezielt per POST nachladen und
  Cache/Scene-Invaliderung bei nachträglichen Änderungen erweitern.
- **Maßgeblich:** `projectplan.md`; Details: `docs/MATERIALS.md`.


## Neuester Entwicklungsblock · 0.2.0-alpha.43

- Phase 8: gezielte `RenderMaterials`-POST-Abfragen anhand
  serverseitig verifizierter OpenSimulator-Zipped-LLSD-Spezifikation.
- `src/protocol/render_materials.cpp`: UUID -> 16-Byte-Binary, zlib-Kompression, LLSD-XML-`Zipped`; bis
  32 deduplizierte UUIDs pro Batch, Antwort auf angefragte IDs begrenzt.
- `src/app/application.cpp`: nach initialem GET unbekannte MaterialIDs
  aus Default-/Face-Materialien asynchron nachladen; Cache bei
  geänderten Einträgen aktualisieren, Scene-Revision invalidieren,
  maximal drei Versuche bei fehlenden IDs, 2-Minuten-Cooldown.
- Tests: 23/23 CTest; GET/POST-Codec inklusive Negativ- und
  Response-Whitelist-Tests. OpenGL-Starttest bestanden.
- Weiter: Legacy-Normal-/Specular-Map-Pipeline, Material-UVs,
  Region-/Teleport-Wechsel und In-World-Validierung; danach PBR.
- Dokumentation `docs/MATERIALS.md`; Projektplan bleibt bindend.


## Neuester Phase-8-Block · 0.2.0-alpha.44

- OpenSimulator `SOPMaterial.cs` bestätigt Legacy-UV- und Glanzfelder
  inklusive Festkommaskalierung 1/10000.
- RenderMaterials-Parser ergänzt `Norm/SpecOffsetX/Y`,
  `Norm/SpecRepeatX/Y`, `Norm/SpecRotation`, `SpecColor`,
  `SpecExp`, `EnvIntensity`, mit Negativtests.
- Application übernimmt getrennte Legacy-Maps in Scene/Face-Material;
  GetTexture-Downloader lädt deren PNGs über denselben begrenzten Cache.
- OpenGL nutzt GL-Texturnummern 5/6, getrennte UV-Uniforms und
  eine derivative-basierte TBN-Normalapproximation sowie
  vereinfachten Specular-Lichtreflex. Ohne Assets Legacy-Fallback.
- 23/23 CTest grün; OpenGL-3.3-Shader-Start erfolgreich, Welcome geladen.
  Keine angemeldete OpenSim-Region visuell geprüft.
- Nächster Arbeitsblock: Region-/Teleport-Lifecycle, Material-Cache-
  Invalidierung und In-World-Vergleich; danach Phase 8 weiterführen.
- Bindend: `projectplan.md`; Detaildokument: `docs/MATERIALS.md`.


## Neuester Phase-8-Block · 0.2.0-alpha.45

- Regionsgebundene Material-Caches: WorldModel-Handle, Generation,
  Seed-Capability bilden eindeutige Anfrage-Epoche.
- `RegionMaterialScope` testet die Epoche unabhängig von Netz/Render.
- Separater regionsgebundener Seed-CAPS-Request für RenderMaterials
  und GetTexture; neue Region erhält eigene Capability-Adressen.
- Stale GET-/POST-Material- und Texture-Download-Resultate werden
  nach Teleport oder Grenzübertritt ignoriert, ohne auf Futures im
  Render-Thread zu warten.
- Pending IDs, Cooldowns, fehlgeschlagene Texture-IDs werden beim
  Regionswechsel invalidiert; Scene-Revision wird neu aufgebaut.
- Noch offen: vollständiger EventQueue-/Circuit-Wechsel beim Teleport,
  GPU-Asset-Eviction und Live-In-World-Test.
- Weiter mit Phase 8 nach `projectplan.md`; neues Scope-Header:
  `include/nexverse/protocol/region_material_scope.hpp`.

Build/Smoketest 0.2.0-alpha.45: 23/23 CTest, OpenGL 3.3,
NexVerse UI und Welcome erfolgreich; Teleport-End-to-End offen.


## Phase 8 – JPEG2000-GetTexture-Reparatur · 10.10.2026 · 0.2.0-alpha.46

Live-Beobachtung 0.2.0-alpha.45 (2026-10-10 00:13 bis 00:18):
- Echter Login erfolgreich, LLUDP UDP LIVE, 17 CAPS-Endpunkte,
  RegionHandshake, WorldModel READY, OpenSim RenderMaterials GET 46,
  POST-Nachladen und Terrain-Mesh bis 126040 Triangles.
- 300 Objekt-Texturfehler, 0 erfolgreich geladene Objekt-Texturen,
  Terrain-Texturen im Fallback. Frühere Fehlerdiagnostik war zu unscharf.
- CAPS EventQueueGet zeitweise erfolgreich, später wiederholte Retries.
  Das ist ein **eigenständiges** noch offenes Problem.
- 0.2.0-alpha.46: vcpkg `openjpeg` + explizites `libpng`.
  `TextureAssetLoader::fetch_rgba` probiert zuerst `format=png`,
  bei fehlender Serverkonvertierung/HTTP-/Decode-Fehler `format=x-j2c`.
- OpenJPEG dekodiert J2C sowie JP2 (inkl. RGB/RGBA, Alpha, Grau,
  grenzenbehaftete Dimensionen/Bit-Tiefe/Datenmengen) in RGBA8.
  Fehlermeldungen enthalten beide Abrufursachen und die Texture-UUID;
  keine geheimen CAPS-URLs im Log.
- Testerweiterungen: echte verlustlose 16x16 J2C-, JP2- und
  J2C-mit-Alpha-Fixtures; PNG-Erfolg, HTTP404-Fallback, ungültiges
  PNG, beide Formate HTTP404, defekte JPEG2000-Daten.
- **23/23 CTest**, Debug-Build, OpenGL-3.3-Smoke und Welcome erfolgreich.
  **Erfolgreiche NexVerse-Welttexturen noch nicht live bestätigt**,
  dafür Viewer neu starten und einloggen.
- Verbindlich `projectplan.md`; technische Details
  `docs/TEXTURES.md`, `docs/MATERIALS.md`.
- Nächste Arbeit: echten Download-Erfolg/Nicht-Erfolg inkl.
  konkreter HTTP-/Decoderfehler im Live-Log verifizieren,
  CAPS-EventQueue-Reconnect und sichtbare Welttexturen.


## CAPS EventQueue Recovery · 10.10.2026 · 0.2.0-alpha.47

- In-World-Live-Logs von alpha.46: CAPS-Live-Batch erfolgreich,
  danach wiederholte Poll-Fehler exakt alle 36–40 Sekunden.
  `CapabilityService::poll_event_queue` hatte nur 35 Sekunden
  HTTP-Total-Timeout; OpenSimulator hält einen leeren Long-Poll
  **bis zu 58 Sekunden**, antwortet dann regulär mit HTTP 502.
- Alpha.47 stellt den EventQueue-Timeout auf 80 Sekunden ein.
  HTTP 502 wird als normales leeres Keepalive behandelt, HTTP 404
  als abgelaufener Endpoint, andere HTTP-/Transport-/LLSD-
  Fehler mit HTTP-Status und Ursache protokolliert.
- EventQueueRecovery: gesunde Batches und 502 resetten den
  Fehlzähler; echte Fehler nutzen begrenzte 1/2/4/8/12-Sekunden-
  Backoffs. Bei 404 sofort, sonst jede dritte echte Störung
  neue `EventQueueGet`-Capability aus dem Seed abrufen.
  Unveränderte URL bewahrt das Ack, neue URL setzt Ack zurück.
  Reconnect-/Idle-Logs dokumentieren den Zustand ohne Token/URL-Leak.
- Der Stopp ist im Backoff nach spätestens 100 ms überprüfbar.
  23/23 CTest inklusive HTTP 502/404/503/Timeout/LLSD-
  Regressionen bestanden; Debug-Build und OpenGL-3.3-GUI-Smoke
  erfolgreich. **Authentifizierter EventQueue-Langzeittest mit alpha.47
  steht noch aus.**
- WICHTIG: Diese Recovery ist für den aktuellen Login-Region-Seed;
  vollständiger Teleport-/Regionswechsel mit neuem UDP-Circuit,
  alter EventQueue-Abmeldung und neuem Seed braucht separaten Block.
- Dokumentation: `docs/CAPS.md`, `projectplan.md` (maßgeblich).
- GitHub Branch weiterhin `develop-current` (nicht `develop`).
- Zum In-World-Test:
  `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl47-live.log`.


## Phase 8/9 – LLSD-Integer + OpenSim-Mesh-Pipeline · 10.10.2026 · 0.2.0-alpha.48

- Grundlage Live-Log alpha.47: Login/WorldModel OK, 293 Objekttexturen,
  4 Terraintexturen, 9 fehlende/defekte Texturassets, normaler
  EventQueue HTTP502-Idle bestätigt. Ein initiales `HTTP=200;
  LLSD integer is invalid` konnte sich selbst erholen.
- `parse_llsd_xml` akzeptiert nun leere/whitespace Integer-Elemente,
  optionale führende Pluszeichen und gültige 64-Bit-XML-Integer;
  vollständige Zahl-/Bereichsprüfung. Binary-LLSD-Wireformat
  bleibt 32 Bit; Langwerte bei Binary-Ausgabe nicht als interoperable
  Transportwerte verwenden.
- `ObjectUpdate.ExtraParams` mit 0x30 Sculpt-Block und Typ 0x05
  wird erkannt und mit UUID im WorldModel gespeichert.
  Partial-Compressed-/Terse-Updates erhalten **bestehende**
  Mesh-Identifier, dekodieren aber noch keine neuen Compressed-
  Mesh-ExtraParams. Das bleibt ein offener Block.
- Regionsgebundene GetMesh/GetMesh2-CAPS, einzelner asynchroner
  Download, bounded 32-MiB-Asset/16-MiB-LOD-Dekompression,
  binärer LLSD-Header-Prefix, High-/Medium-LOD und
  16-Bit-Positions-/UV-/Dreiecksdaten. CPU-Triangulation,
  Face-Material-Ranges, GPU-Upload durch OGLRender.
- UUID-Caches und Region-Epochen; keine alten HTTP-Replies in
  neue Region übernehmen. Ungültiges Mesh bleibt im sichtbaren
  Fallback. 256 einzigartige GPU-Meshes als vorläufiges Limit.
- Tests: 24/24 CTest inkl. synthetischem OpenSim-Mesh-Asset.
  Realer NexVerse-Mesh-Sichttest ohne erneuten Login
  **noch nicht durchgeführt**.
- Mesh-Rigging, Skinning/Animesh, OBJ/DAE-Import, Textur-Sculpties,
  vollständiges LOD-Streaming, Disk-LRU, Compressed-ObjectUpdate
  und kompletter Teleport-Reconnect sind **noch offen**.
- Projektplan verbindlich, Details `docs/MESH.md`.
- GitHub Handoff: `develop-current`, lokaler Branch entsprechend.


## alpha.49 – LLSD-Date und Mesh-Diagnostik · 10.10.2026

- Zuletzt bewiesene Live-Zahlen aus alpha.48:
  448 Meshanforderungen; 232 Erfolge, 216 Fehler
  (208 unbekannter LLSD-Binary-Marker, 7 Submeshes, 1 HTTP 404).
  287 erfolgreiche Objekttexturen, 9 fehlende/defekte Texturen.
  EventQueue HTTP502-Idle korrekt; einmaliger HTTP200-Fehler
  `Unsupported LLSD XML type: date` wurde anschließend recovered.
- Neu: `LlsdType::date`, XML `<date>` und Binary-Marker
  `d` (8-Byte-Big-Endian-IEEE754 epoch seconds) dekodieren.
  LLSD unknown-type Fehler melden jetzt `0xNN at offset N`,
  statt unlesbarem Steuerzeichen.
- Mesh-Asset-Fehlerprotokoll nennt sichere 8-Byte-Hex-Signatur,
  Gesamtlänge und LLSD-Fehlerposition (keine URL/Token).
- Optionales `TexCoord0Domain` erhält jetzt Standard-UV-Domain,
  invalide Submesh-Faces werden mit Rollback übersprungen,
  gültige restliche Faces bleiben renderbar.
- `GetMesh2` wird bevorzugt, `GetMesh` bei Fehler als
  einmaliger alternativer CAPS-Pfad versucht, solange getrennt
  angeboten. Regions-Epochenabsicherung und GPU-Limits bleiben.
- 24/24 Offline-Tests grün, echte In-World-Abnahme alpha.49 offen.
  Die bisherigen 208 unbekannten Mesh-Header sind **noch nicht
  als behoben bestätigt**; neue `prefix_hex`-/Offset-Logs
  sind die Basis für die nächste formatgenaue Korrektur.
- Ganz offene Funktionen: initiale SculptExtraParams in
  ObjectUpdateCompressed, vollständige Mesh-LOD, Rigging/Animesh,
  Teleport-Reconnect sowie 9 fehlerhafte/fehlende Servertexturen.
- Verbindlich `projectplan.md`. Detail `docs/MESH.md`,
  `docs/CAPS.md`. GitHub `develop-current`.
- Live-Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl49-live.log`.
