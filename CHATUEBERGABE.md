# OpenGenesisLINK Viewer – Chat-Übergabe

Stand: 11.10.2026

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

Version: `0.2.0-alpha.60`

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


## alpha.50 – Compressed-ObjectUpdate und Mesh-Prims · 10.10.2026

**Status:** Beim Start dieses Blocks gab es kein aktuelles
/tmp/ogl49-live.log auf der Linux-Maschine. Die 208 vorherigen
Mesh-Headerfehler sind deshalb weiterhin unbestätigt.

**Implementiert:**
- Vollständige ObjectUpdateCompressed-Pakete mit CompressedFlags,
  Owner UUID, optionalen Feldern AngularVelocity, Parent,
  ScratchPad, Text, MediaURL, Particles, Sound und NameValues.
- SculptExtraParams Typ 0x30 nach den optionalen Feldern;
  Mesh-UUID und Sculpt-Typ werden ins WorldModel übernommen.
- Vollständige 23-Byte-Prim-Form und TextureEntry-Daten mit
  32-Bit-Längenfeld und Face-Textur sowie Skalierung/Parent-ID.
- Bounds-Checks und Ablehnung abgeschnittener Compressed-Daten.
- Test: synthetisches vollständiges komprimiertes Mesh-Prim
  samt Scale/Parent/TextureEntry, plus Truncation-Test.
- Formatbeleg: OpenMetaverse ObjectManager.cs,
  ObjectUpdateCompressedHandler:
  https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/ObjectManager.cs

**Noch offen:** echter NexVerse-Live-Test; 208 vorherige
Headerfehler über prefix_hex / Byteoffset analysieren;
Mesh-LOD, Rigging/Animesh, Teleport-Regionswechsel und
fehlende Servertexturen.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl50-live.log


## 0.2.0-alpha.51 – Dynamischer GPU-Mesh-Cache · 10.10.2026

**Ausgangsbefund alpha.50:** Der Live-Test meldete 256 erfolgreiche
Mesh-Uploads, 2 vollständig fehlerhafte Mesh-LODs,
3 teilweise gerettete Submeshes. Durch die Bedingung
loaded_ids.size() < 256 wurden danach keine weiteren
Meshes angefordert. Das ist ein Cache-Limit und nicht
die tatsächliche Anzahl von Meshes der NexVerse-Region.

**Implementiert:**
- Neue pure Mesh-Residency-Planung mit nach Entfernung zum
  Avatar priorisierten Mesh-UUIDs und Duplikatentfernung.
- GPU-Asset-Budget 256. Werden durch Ortswechsel neue Meshes
  relevanter, verdrängen sie zuvor geladene entfernte Meshes.
- 24 Meter Prioritätsbonus für residente Meshes vermeidet
  Cache-Churn bei geringfügiger Bewegung.
- Plan-Neuberechnung bei SceneObject-Änderung oder nach
  maximal 750 ms; GPU-Eviction pro Zyklus auf acht begrenzt.
- Renderer: set_mesh_asset liefert Upload-Erfolg, dazu
  remove_mesh_asset und clear_mesh_assets für explizite
  GL-VAO/VBO/EBO-Freigabe auf dem Renderer-Thread.
- Bei Region-Epochenwechsel werden ältere GPU-Meshes gelöscht.
  Stale Mesh-HTTP-Ergebnisse und nicht mehr priorisierte
  Ergebnisse werden nicht hochgeladen.
- Unbekannte/doppelte UUIDs und nicht-finite Positionen
  werden bei der Auswahl ignoriert; Gleichstände deterministisch.
- Zusätzlicher Testsatz für Bewegung, Eviction, Hysterese,
  Deduplikation, NaN, Nullbudget und 320 Meshes; 25/25 CTests.

**Offen und begrenzt:**
- Echte In-World-Abnahme von alpha.51 bei Ortswechsel.
- Noch keine Frustum-/Occlusion-Priorisierung.
- Mesh-Kindposition nur angenähert mit Parent-Position
  plus lokalem Offset (ohne Rotation).
- Keine persistenten Mesh-Disk-/CPU-LOD-Caches;
  beim Zurückkommen können Downloads erneut erfolgen.
- Zwei vollständig ungültige Mesh-LODs, beschädigte oder
  HTTP404-Textur-Assets; Rigging/Animesh und Teleport
  bleiben eigenständige Entwicklungsblöcke.

Live-Test: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl51-live.log


## alpha.52 – Avatar sichtbar machen und Bewegung · 10.10.2026

**Auslöser:** Der Screenshot der verbundenen Welt zeigt
Terrain/Meshes, aber keinen eigenen Avatar und bislang
keine nachvollziehbare Bewegung. WORLD UDP LIVE beweist
weder den Avatar-Renderpfad noch die Bewegungsfähigkeit.

**Implementiert:**
- OpenGL-Avatar-Proxy-Pfad mit sieben Basis-Meshes für
  Kopf, Torso, Arme, Beine, für eigenen Avatar und bis
  zu 63 weitere Avatar-Datenpunkte. Die Darstellung wird
  pro Frame anhand der Agentposition aktualisiert.
- Die Proxies sind absichtlich keine echten OpenSim-Avatare:
  keine Skin-Bakes, Avatar-Meshes, Skeletons,
  Wearables, Haare, Attachments oder Animationen.
- Neues Keyboard-Control-Modul mit W/S/A/D, linker/rechter
  Pfeiltaste zur Rotation, Shift für schnelleres Bewegen,
  Space für Sprung, C für Abwärtsbewegung, F Flug
  per Toggle. Die AgentUpdate-Pakete enthalten dabei
  korrekte Körper-/Kopf-Quaternionen und Kameraachsen.
- Self-Avatar-LocalID wird aus ObjectUpdate bzw.
  ObjectUpdateCompressed gewonnen, mit dem RegionHandle
  gekoppelt und bei ImprovedTerseObjectUpdate
  zur Aktualisierung der serverbestätigten eigenen
  Avatarposition im WorldModel verwendet.
- Bei Steuerungsänderungen und bestätigten Simulator-
  Positionsupdates werden diagnostische Logzeilen geschrieben.
  Fokusverlust sendet keine gedrückten Bewegungs-Keys.
- Nach erfolgreichem Login wird das Login-Eingabefeld
  nicht mehr durch die Bewegungssteuerung verändert.
- Regressionstests für Steuerbits, Quaternion, Fly-Toggle
  und Steuerungsreset; 26/26 CTests grün.

**Noch offen und nicht als fertig behaupten:**
- Echte NexVerse-In-World-Bewegungsabnahme, insbesondere
  muss die Self-Avatar-LocalID und die Terse-Positions-
  Bestätigung mit neuer Version beobachtet werden.
- Richtige Firestorm-Avatar-Appearance (Skeleton,
  Body Shape, Skin-Bakes, Wearables, Attachments,
  Animationen) sowie Mouselook, Camera-Orbit/Zoom,
  Teleport/Regionswechsel, Collision/Reconciliation.
- Die einfachen Avatar-Proxies sind nur eine
  Übergangsdarstellung bis zu Phase-10-Appearance.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl52-live.log


## 0.2.0-alpha.53 – Phase 8: OpenSim-Point-Lights · 10.10.2026

**Verbindliche Nutzerentscheidung vom 10.10.2026:**
Fortsetzung der Entwicklung entlang der Reihenfolge des
projectplan.md. Die zuletzt untersuchten Avatar-/Bewegungs-
und übrigen Rendering-Asset-Fehler werden **zunächst
nicht behoben**, sondern dokumentiert und für eine
spätere gemeinsame Fehlerbehebungsphase vorgemerkt.
Keine implizite Behauptung, dass die Probleme gelöst seien.

**Jetzt neu in Phase 8 implementiert:**
- Point-Light ExtraParams Blocktyp 0x20, 16 Byte:
  4x RGBA-UNORM8 (Alpha = Intensität), dazu
  little-endian float32 Radius, Cutoff und Falloff.
  Implementierung orientiert sich an libopenmetaverse,
  Primitive.LightData.
- Sichere Bounds/Finite-/Radius-Prüfungen;
  Daten werden in vollständigem ObjectUpdate und
  ObjectUpdateCompressed nach SculptExtraParams separat
  dekodiert. Bei Partial-Updates behalten vorhandene
  Objekte ihre bereits bekannte Lichtdefinition.
- Eigenes Feld im WorldModel für lokale Lichtquellen.
- Die acht nächsten Punktlichter werden aus
  den Objektpositionen pro Frame ausgewählt. Child-Prim-
  Position wird vorläufig mit Parent+Offset angenähert.
- Legacy OGLRender: Weltposition aus uModel-Transform,
  acht Point-Light-Uniforms und diffuse Beleuchtung
  von nicht-fullbright Objekt-/Meshflächen nach
  Abstand, Radius, Intensität und Falloff.
- Terrain/Avatar werden in dieser ersten Version
  noch nicht von den lokalen Punktlichtern beleuchtet.
  Keine dynamischen Schatten, keine Projektoren,
  keine PBR-Energiekonservierung.
- Kompletter Debug-Build, 26/26 CTests bestanden,
  GL-3.3-Smoke auf RTX 4070 SUPER erfolgreich.
  **In-World-Sichtabnahme mit echter Lichtquelle
  steht noch aus**.

**Bekannte Fehler für spätere Fehlerphase (unverändert):**
- Eigene Avatar-LocalID wurde in alpha.52 Live-Log nicht
  gebunden, daher keine serverbestätigten
  Self-Avatar-Positionsupdates / keine verifizierte
  Benutzbarkeit der WASD-Bewegung.
- Eigenes Erscheinungsbild ist bisher nur Proxy,
  echtes Skeleton/Bakes/Wearables fehlen.
- Alpha.52: 252 geladene, vier fehlgeschlagene Meshes;
  neun fehlende/beschädigte Objekttexturen.
- Keine aktuelle Bestätigung einer GPU-Cache-Eviction
  bei tatsächlichem Ortswechsel.

**Fortsetzung Phase 8:** Partikel-Basis, weitere
Lichttypen (Projektor/Material-Licht), Render-Performance
und anschließend Phase 9 bzw. weitere Funktionssysteme.
Maßgeblich bleibt der Plan, nicht die Bugliste.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl53-live.log


## 0.2.0-alpha.54 – Phase 8: OpenSim ParticleSystem/PSBlock · 10.10.2026

**Nutzerentscheidung:** Nach projectplan.md die Funktionalität
weiterentwickeln und bekannte Fehler erst später beheben.
Die Priorität bleibt Phase 8 vor neuen großen Folgephasen.

**Geliefert:**
- OpenSim ObjectUpdate PSBlock (Variable1 nach MediaURL) und
  ObjectUpdateCompressed Partikelblock (CompressedFlag 0x08,
  86 Bytes) werden abgegriffen und mit strikter Größenprüfung
  dekodiert; ungültige/fehlende Blöcke blockieren den Prim nicht.
- Protokollkompatibles Legacy-ParticleSystem mit 68-Byte-
  Systemteil und 18-Byte-Partikeldaten (86 Byte) sowie
  erweitertem Format 94–98 Byte mit Größenfeldern,
  Glow-/Blend-Metadaten; Bytes nicht aus C# kopiert, sondern
  eigenständiger C++23-Parser.
- Emissionsparameter: Source/Particle-Lifetime, Burst-Rate,
  BurstCount, BurstRadius, Min/Max-Speed, Beschleunigung,
  Start/End-Farbe, Scale, Textur-UUID (nur Referenz)
  sowie Pattern und Flags.
- Partikelsystem liegt im WorldModel der jeweiligen Prim;
  Terse-/Legacy-Partial-Updates bewahren den bekannten Wert,
  KillObject entfernt den Prim mitsamt Emitter.
- Render-Integration: maximal 32 nahe Emitter in ca. 192m
  Radius, maximal 768 Partikel in einem Drawcall; eigener
  OpenGL-3.3-Shader und Streaming-VBO. Weiche runde Sprites
  mit alphablendfähigem Fade, Z-Test ohne Z-Writes.
  Die Positionen sind deterministisch generierte Grund-
  Partikelpositionen mit Ballistik/Beschleunigung.
- Unterstützte Basispfade: Drop, Explode und grob nach
  oben gerichtete Angle-/Cone-Patterns; keine native
  Target-/Ribbon-/Beam-Logik, Wind, Bounce, Rotationsflags,
  korrekte Emitter-Restart-Events, Textur-Sampling oder
  diskrete persistente Partikel-Physik.
- Smoke-Test: Linux 3440x1440, NVIDIA RTX 4070 SUPER,
  GL 3.3, Welcome-UI lädt, kein Shaderfehler gemeldet.
- CMake-Debug Build und 26/26 CTests; Parser-Regressions-
  tests für Legacy und Extended ParticleSystem,
  invaliden Datenblock, ObjectUpdate und
  ObjectUpdateCompressed mit aktivem PSBlock.
- **Die erste echte In-World-Sichtprüfung ist offen.**
  Simulierte Sprites sind kein fertiger Firestorm-
  Partikelrenderer. Keine Änderungen an der
  bekannten Avatar-/Bewegungssynchronisationslücke.

**Spätere Ausbaustufen im Plan:** Partikeltexturen über
AssetCache, Ziel-Partikel, Ribbon/Beam, Wind/Bounce,
lokal zustandsbehaftete LSL-Emission, Partikel-LOD
und GPU-Instancing. Danach Phase 8 weitere
Material-/Licht-Effekte und Phase 9 Asset-Streaming.

Live-Start: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl54-live.log


## 0.2.0-alpha.55 – Phase 8: Partikeltexturen / GetTexture · 10.10.2026

**Verbindlicher Kurs:** Umsetzung der Funktionen aus projectplan.md
fortsetzen. Die bekannten Avatar-/WASD-/Mesh-/Texturfehler bleiben
auf Nutzerwunsch für die spätere Fehlerphase zurückgestellt.

**Neu:**
- Die in OpenSim ParticleSystem/PSBlock enthaltene Particle
  Texture-UUID wird nicht mehr nur gespeichert: Für sichtbare
  Emitter (maximal 32 im Umkreis) wird die Textur im vorhandenen
  GetTexture-Asset-Loader vor regulären Hintergrund-Objekttexturen
  angefordert und als RGBA8/ggf. JPEG2000 dekodiert.
- Emittierte Bilder teilen denselben 512-Assets-GPU-Cache mit
  Object-TextureEntry; gleiche UUID wird nur einmal angefragt
  und kann für Mesh/Prim und Partikel gleichzeitig genutzt werden.
  Die bereits bekannte Epochen-Prüfung verhindert das Hochladen
  veralteter GetTexture-Replies aus früheren Regionen.
- Neuer Pure-Logic Asset-Priorisierer prüft null/Zero-UUIDs,
  doppelte IDs, bereits geladene und zuvor fehlgeschlagene
  Requests. Bei Bedarf wird auf Objekttexturen zurückgegriffen.
  Eigener automatischer Regressionstest.
- Der Particle-Fragmentshader übernimmt echte Textur-RGBA
  über GL_TEXTURE4 (getrennt nach Textur-UUID gebatcht),
  verrechnet sie mit Start-/End-Farbe und Soft-Mask-Alpha.
  Bei nicht geladenem Asset nutzt er weiterhin die zuvor
  existierende weiche Kreis-Sprite-Darstellung.
- Maximal 768 Partikel/Frame, höchstens 32 Emitter; künftig
  statt eines einzelnen bis zu einem Drawcall je
  verwendeter Partikeltextur plus optional Fallback-Batch.
  Keine eigenen GL-Texturduplikate, kein weiteres HTTP-Subsystem.
- Neue Tests fuer priorisierte Assetwahl, gemeinsame Cache-
  Nutzung, ungültige IDs, geladene und fehlgeschlagene Texturen.
  **27/27 CTests**, CMake-Debug-Build sowie OpenGL-GUI-Smoke
  auf NVIDIA RTX 4070 SUPER ohne Shaderfehler.
- Echte In-World-Sichtabnahme steht noch aus; das vorhandene
  Cachelimit von 512 Assets, transparenzabhängige Sortierung,
  Partikeltextur-Orientierung, diskrete Simulation und
  exakte OpenSim-Partikelmodi bleiben spätere Schritte.
- Gesammelte Avatar-/Bewegungs-/Mesh-Fixphase ausdrücklich
  nicht vorgezogen.

Weitere Planblöcke: Wind-/Bounce-/Target-/Ribbon-Partikel,
Effekt-Lebenszyklen/Blending und komplexere Materialbeleuchtung,
danach Phase 9 Asset-Streaming und übrige Viewerfunktionen.
Protokolldetails: docs/PARTICLES.md.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl55-live.log


## 0.2.0-alpha.56 – Großrelease Phase 8 / vier Entwicklungsblöcke · 10.10.2026

Auf Nutzerwunsch werden zukünftig **3–4 zusammenhängende Funktionen
pro Release** umgesetzt, mit einem gemeinsamen Abschluss aus Build,
Tests, Versionspflege, Dokumentation und GitHub-Commit. Einzelne
Zwischentests bleiben zur Qualitätssicherung möglich.

**Block A – Partikelphysik:**
- Neuer testbarer reiner Partikelkinematik-Helfer, mit Begrenzung
  von Lebensdauer und festen Integrationsschritten (48).
- Gravity/PSBlock-Beschleunigung, Wind-Flag 0x008 und Bounce-Flag
  0x004. Wind ist aktuell ein **lokal generiertes Feld** und
  keine behauptete Simulator-Windmessung; Bounce erfolgt auf der
  Höhe der Quellprim, nicht durch Terrain-/Collision-Abfragen.
- Flags für InterpColor (0x001) und InterpScale (0x002) werden
  berücksichtigt statt unbedingter Interpolation.

**Block B – Ziel- und Verbindungseffekte:**
- Target UUID aus dem 68-Byte-PSBlock-Systemteil auslesen;
  bekannte Zielprims, Avatare und eigener Agent werden anhand
  UUID dem Emitter zugeordnet; unbekannte Ziele bleiben
  unaufgelöst, ohne auf erfundene Positionen zu springen.
- TargetPos (0x040) und TargetLinear (0x080) im CPU-Sampler,
  Beam (0x200) als verteilte Partikel entlang einer Strecke
  und Ribbon (0x400) als zeitlich versetzte Punktspur.
- Echte zusammenhängende Ribbon-Flächen, SourceRotation,
  Target-Bewegungs-Prediction und spezifische Formmodi sind
  **noch offen**.

**Block C – Transparenz, Blending, Budget:**
- OpenMetaverse ParticleSystem BlendFunc-Enums 0..9 für
  Extended PSBlock werden validiert und mit OpenGL abgebildet.
  Default SourceAlpha=7, OneMinusSourceAlpha=9.
- Neue distanzbasierte Sortierung in acht Tiefenbändern,
  innerhalb eines Bands Batching nach Blend/Texture.
- Feste Obergrenzen von 768 Partikeln und maximal 192
  Zeichen-Batches schützen vor übergroßer Drawcall-Zahl.
- Das ist nur näherungsweise Alpha-Tiefensortierung; echte
  per-Partikel-Sortierung über alle Texturen ist offen.

**Block D – Beleuchtung/Materialgrundlagen:**
- Bei den bis zu acht Legacy-Point-Lights wird zusätzlich
  zum bestehenden Radius/Falloff der Cutoff-Wert im Shader
  als näherungsweiche Kernausblendung berücksichtigt.
- Lokales Blinn-Phong-Highlight für glänzende,
  nicht-fullbright Objektoberflächen ergänzt.
- Kein vollständiges PBR, keine Projektorlichter, keine
  dynamischen Schatten oder Energieerhaltung.

**Abnahme:** Debug-CMake + C++23 erfolgreich, 28/28 CTests
einschließlich neuer reiner Wind-/Bounce-/Target-Tests,
Parser-Tests für TargetUUID und Blend-Enums;
OpenGL-GUI-Smoke auf NVIDIA RTX 4070 SUPER, Version alpha.56,
NexVerse-Welcome geladen, keine Shader-Fehler (der Exit 124
des Smoke-Tests ist die beabsichtigte 7-Sekunden-Begrenzung).
**Authentifizierter In-World-Sichttest dieser Effekte ausstehend.**

**Zurückgestellte Fehler unverändert:** Eigene Avatar-LocalID
und Bewegungsbestätigung, echte Avatar-Skeleton-/Appearance,
defekte/fehlende Mesh-/Texturassets und Teleport/Region-Handoff.
Keine dieser Fehlergruppen wird in diesem Release als gelöst
ausgegeben.

**Nächste gemeinsame Blöcke:** echte PSBlock-Emitter-Lebenszyklen,
Parent-/ObjectRelative- und Target-Pfade, verbundene Ribbons
und Beam-Geometrie, Asset-Caching und moderne Materialeffekte.
Die Reihenfolge bleibt an projectplan.md gebunden.

Live-Abnahme:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl56-live.log


## 0.2.0-alpha.57 – Großrelease: vier zusammenhängende Funktionsblöcke · 10.10.2026

**Block 1 – Emitter-Lebenszyklus / ObjectRelative**
- Emitterstart auf Renderer-Zeitbasis (nicht mehr globaler Partikelphase).
  CRC-Wechsel startet ein emittierendes Prim neu; entfernte Emitter
  werden aus dem Laufzeitstatus entfernt.
- SourceMaxAge stoppt neue Bursts, lässt bereits gestartete Partikel
  innerhalb ihrer eigenen PartMaxAge-Zeit auslaufen.
- Rotationskompatibilität: bei OpenSim SourceFlags ObjectRelative
  (Bit 0) werden die Quellbeschleunigung und Initialgeschwindigkeit
  durch die Objekt-Quaternion transformiert.
- Offene Details: vollständige LSL-Start/Stop- und SimTime-Synchronisation,
  Quelle mit ParentRotation, Simulator-Wind statt synthetischem Wind.

**Block 2 – durchgehende Beam-/Ribbon-Geometrie**
- Neuer Kameraseiten-ausgerichteter Ribbon-Generator mit
  verbundenen Dreiecken statt einer bloßen Punktfolge.
  Separater OpenGL-3.3-Triangle-Shader/-VAO/-VBO, UVs,
  transparente Soft-Edges sowie geteilte GetTexture-Assets.
- Beam folgt dem bekannten Zielpunkt; Ribbons folgen
  einer auf Basis der Partikelkinematik erzeugten Spur.
  Begrenzung 32 Emitter/je 16 Samples und 15 Segmente.
- Offene Details: echte bewegungsabhängige kontinuierliche
  Emitter-Historie, native Ribbon-Rotation und robuste
  Transparenzsortierung aller überlappenden Flächen.

**Block 3 – Partikelmaterial / Glow**
- In Extended PSBlock vorhandener Start/EndGlow wird an den
  GPU-Renderer durchgegeben und für Sprites und Ribbon-Material
  als begrenzte RGB-Aufhellung berücksichtigt; nicht als
  echtes physikalisches HDR- oder PBR-Emissive ausgegeben.
- OpenSim-Blend-Enums 0..9 aus alpha.56 bleiben aktiv.

**Block 4 – Phase 9: wiederverwendbarer Texture-Asset-Cache**
- Frühere harte 512-GPU-Texturen-Grenze wird durch ein
  dynamisches Residency-Budget ersetzt; unbenutzte Texturen
  werden als GL-Texturen freigegeben, höchstens 8 pro
  Planungsrunde (750ms).
- Reihenfolge: nahe Partikelemitter, anschließend Prim-/
  Meshmaterialien; letztere erhalten eine Objekt-Distanz-
  priorisierte Ordnung beim Aufbau der Scene-Revision.
- Neue Region leert den GPU-Objekttexturcache; veraltete
  HTTP-Replies außerhalb von Region/Epoche/Residency werden
  verworfen. Gemeinsames GetTexture-Ladesystem bleibt.
- Grenzen: noch kein persistentes CPU-/Disk-Texture-Cache,
  keine Texture-LOD-/Mip-Streaming-Stufen, keine Occlusion,
  UI/Textur-Distanz-Neusortierung nur bei Scene-Update;
  beim Zurückkehren eviktierter Texturen erneut HTTP möglich.

**Tests:** 30/30 CTests inklusive Lifecycle, Quaternion,
Ribbon-Triangles, Cache-Umschichtung. C++23-Debug-Build
und OpenGL-3.3-Starttest auf NVIDIA RTX 4070 SUPER
ohne gemeldeten Shaderfehler. Der 7-Sekunden-Exit 124
ist der absichtliche Timeout, kein Crash.
**NexVerse In-World-Darstellung bleibt ungeprüft.**

**Auf Nutzerwunsch zurückgestellt:** Avatar-LocalID/
serverbestätigte Bewegung, echte Avatar-Appearance,
defekte/missing Mesh-/Texture-Assets und Teleport.
Kein dieser Fehlerbereiche wird als behoben ausgegeben.

Nächster Großrelease-Block: Asset-Caching-Disk/CPU,
LOD/Prioritätsstufen und weitere OpenSim-Renderer-
Kompatibilität. projectplan.md bleibt verbindlich.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl57-live.log


## 0.2.0-alpha.58 – Phase 9 Großrelease (vier Blöcke) · 10.10.2026

Nutzerentscheid: Jeweils drei bis vier vollständige Entwicklungsblöcke
pro Release; bekannte Avatar-/Bewegungs-/Mesh-Asset-Fehler bis
zur späteren Bugfixphase zurückstellen. Verbindlich: projectplan.md.

**Block A – Persistenter Texture-Disk-Cache:**
- TextureDiskCache lädt/speichert validierte RGBA8-Assetdaten
  unter $XDG_CACHE_HOME/opengenesislink/textures-v1 bzw.
  $HOME/.cache/opengenesislink/textures-v1.
- 8-Byte-Version-Magic, Width/Height, FNV-64-Inhaltshash über
  Dimensionen und Pixeldaten, Größen-/Bounds-Prüfungen, sichere
  UUID-Dateinamen, temp-Datei + Umbenennung.
- 256 MiB Diskbudget, 64 MiB maximales Einzelbild,
  least-recently-used-ähnliche Eviction anhand File-MTime;
  auf Cache-Miss/Fälschung/IO-Problem normaler GetTexture-Download.
- Cache wird im Hintergrund-Texture-Future geprüft; bei Erfolg
  wird HTTP übersprungen. Kein persistenter Mesh-/Animationscache
  in diesem Release, keine echte Hash-/Version-Aushandlung mit
  dem OpenSim-Asset-Service.

**Block B – Dynamische Texturprioritäten:**
- Texturpositionen werden bei Object-/Scene-Update für Default,
  Face sowie Normal-/Specular-Materialien gesammelt.
- Alle 750ms/Scene-Revisionsänderung werden sie ausgehend von
  der aktuellen Avatarposition nach Entfernung neu sortiert.
- Identische UUIDs werden zusammengefasst; bereits GPU-geladene
  Assets erhalten 8m Hysterese gegen Thrashing.
- Partikel-Assetpriorität sowie 512 gleichzeitige GPU-Texturen,
  begrenzte GL-Eviction und Regions-Epochenprüfungen bleiben.

**Block C – Mesh-Sichtgrößenpriorität:**
- MeshDistanceSample führt zusätzlich den maximalen
  Prim-Scale-Durchmesser; Priorisierung gewichtet
  distance / sqrt(clamp(diameter,0.5,16)).
- Größe beeinflusst ausschließlich Assetabruf/Priorität,
  **noch nicht** echte Mesh-LOD-Stufen, View-Frustum-/Occlusion-
  Culling oder Mesh-Vertex-Dekodierung. Defekte Meshes werden
  nicht als repariert bezeichnet.

**Block D – GPU-Sampling und Asset-Diagnose:**
- Bei unterstützter GL_EXT_texture_filter_anisotropic
  aktiviert das Legacy-GL-Backend für Objekttexturen
  bis zu 8x anisotrope Filterung zusätzlich zum Mipmap-Sampler.
  Auf inkompatiblen GPUs bleibt das alte Verhalten.
- Log alle ~60s: kumulative Disk-Cache-Hits, HTTP-Downloads,
  GPU-Evictions, residente Texturen und GPU-Slotbudget.
  Die Zähler sind Sitzungsstatistik, keine serverbestätigte
  Bandbreitenmessung.

**Verifikation:**
- CMake Debug C++23 erfolgreich; 32/32 CTests.
- Neue Tests für persistente Cacheintegrität, manipulierte
  Cache-Dateien, Neustart-/LRU-Verhalten, bewegungsabhängige
  Texture-Distance-Ordering/Hysterese und größenbasierte
  Mesh-Priorität.
- Linux NVIDIA GeForce RTX 4070 SUPER OpenGL 3.3 GUI-Smoke
  bei 3440x1440 mit Welcome-Seite, keine gemeldeten GL-
  Shaderfehler; Exitcode 124 war beabsichtigtes 7s-Timeout.
- **Authentifizierte In-World-Streaming-Abnahme ausstehend.**

**Bekannte, absichtlich unangetastete Probleme:**
Self-Avatar-LocalID/Bewegung, echtes Avatar-Skeleton/Bakes,
noch fehlerhafte/fehlende Mesh-/Texturassets, Teleport.
Keiner dieser Punkte wird als geschlossen markiert.

**Nächste Großrelease-Ideen gemäß Phase 9:** Mesh-Disk-Cache,
RAM-Assetcache, Chunk-/Mip-Streaming-Priorität, GPU/CPU-
Speicherbudget und anschließend Avatar-Funktionssysteme.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl58-live.log


## 0.2.0-alpha.59 – GitHub-Handoff · 11.10.2026

**Block A – persistente Mesh-Dateien:** 512 MiB Diskbudget,
Versionmagic OGLMESH1, Little-Endian, UUID-gebundene FNV64-
Integritätsprüfung, Größen-/Index-/Face-Checks und MTime-LRU.
Keine CAPS-Token/URL-Cachedaten.

**Block B – Mesh-RAM-LRU:** threadgeschützte CPU-Geometry-
Residency mit 64 MiB Bytebudget und LRU-Eviction. Durch
GPU-Eviction entfernte Meshes können ohne HTTP erneut
aus dem RAM geladen werden.

**Block C – Integration:** asynchroner Ladepfad RAM → Disk →
GetMesh2/GetMesh; Region-/Epochenschutz und GPU-Budget 256
Meshes (max. acht Evictions je Planung) unverändert.

**Block D – Telemetrie und Tests:** getypte Cacheherkunft,
60s-Metriken über erfolgreiche GPU-Uploads nach Quelle,
CPU-RAM-Bytes und GPU-Slots. Manipulations-, Neustart-,
UUID-, LRU-/Budget- und Fehler-Fallback-Tests.
CMake Debug C++23, **33/33 CTests bestanden**,
OpenGL 3.3 GUI-Starttest auf NVIDIA RTX 4070 SUPER mit
NexVerse-Welcome erfolgreich; Timeout 124 erwartungsgemäß.

**Noch offen:** In-World-Abnahme, echtes Mesh-LOD-Streaming,
GPU/CPU-Bytebudget und Texture-Mip-Streaming.
Avatar-LocalID, WASD, Skeleton/Bakes, defekte Server-Mesh-/
Texturassets und Regions-Teleport bleiben auf Nutzerwunsch
bis zur Bugfixphase zurückgestellt.
`projectplan.md` bleibt verbindlich.
Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl59-live.log`.

Arbeitsstand zu Beginn: `b5271bc` auf `develop-current`.
Nächste 3–4 Blöcke: echte Mesh-LOD-/Mip-Stufen und
GPU-/CPU-Bytebudgets. Vor jedem weiteren Commit
Git-Status prüfen, keine destruktiven Resets/Cleans;
CMake Debug, CTest und GUI-Smoke nach Änderungen.


## 0.2.0-alpha.60 – GitHub-Handoff · 11.10.2026

**Block A – OpenSim-LOD-Decoder:** High, Medium, Low und Lowest. Gewünschte Stufe zuerst; bei fehlenden/defekten Chunks Fallback auf andere gültige Level. Bestehende LLSD-/zlib-/Mesh-Bounds gelten weiter.

**Block B – Adaptive LOD-Planung:** `plan_mesh_lods` gewichtet Entfernung und Primgröße, dedupliziert mehrfach verwendete Mesh-UUIDs und nutzt Hysterese. Aktualisierung alle etwa 750 ms oder bei Szene-Änderung.

**Block C – LOD-getrennte RAM-/Disk-Caches:** High behält den bisherigen `<uuid>.mesh`-Schlüssel; andere Stufen werden unter `<uuid>.<lod>.mesh` inklusive LOD-gebundener Checksumme gespeichert. Budgets 64 MiB RAM und 512 MiB Disk. Nach Server-Fallback bezeichnet der Cache-Key den angeforderten, nicht zwingend den tatsächlich dekodierten LOD.

**Block D – GPU-Swap und Diagnose:** Mesh-LOD-Upgrades und -Downgrades ohne vorherige Entfernung der Mesh-UUID. GPU lädt zuerst neue Buffers, gibt alte erst nach erfolgreichem Upload frei. Aktuelle Regions-Epoche und Ziel-LOD werden vor Upload geprüft. Neues Log: LOD-Swap-Zähler und geschätzte VBO/EBO-Bytes.

**Verifikation:** C++23-Debug-Build, 34/34 CTests (LOD-Codec/Fallback, Cache-Varianten, Hysterese). OpenGL-Start-Smoke zum Release. Weiter offen: authentifizierter In-World-LOD-Test, GPU-Bytebudget, Texture-Mip-Streaming, Rigging/Animesh und zurückgestellte Avatar-/Bewegungs-/Teleport-Fehler.

Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl60-live.log`. Maßgeblich: `projectplan.md`, Branch `develop-current`.

**Nächste 3–4 Blöcke:** Texture-Mip/Chunk-Streaming, durchgesetztes GPU-Bytebudget, priorisierte Asset-Anfragen. Verbindlicher Prozess: GitHub-Branch `develop-current` lesen (nicht `develop`), `projectplan.md`, `info.md`, `CHATUEBERGABE.md` und relevante Docs pflegen, Debug-Build/CTest/GUI-Smoke, danach Code und Docs gemeinsam pushen.
