# OpenGenesisLINK Viewer – Chat-Übergabe

Stand: 11.10.2026

## 1. Projekt

- Produkt: **OpenGenesisLINK Viewer**
- Zweck: eigener OpenSim/OpenGenesisLINK-Viewer für NexVerse
- Sprache: C++23
- **Neuer verbindlicher Auftrag 11.10.2026:** Alle vorhandenen Firestorm-Funktionen als vollständige Funktions-/Implementierungsbaseline erfassen und für OpenGenesisLINK implementieren. Neue OGL-Funktionen nur als Ergänzung/gezielten Umbau; nichts stillschweigend weglassen
- **Ist:** aktueller OGL-C++23-Code ist noch kein tatsächlicher Firestorm-Source-Fork. Quellcodeübernahme/Forkstrategie mit Lizenz-, Release-Branch- und Migrationsprüfung, keine falsche bereits-erledigt-Meldung
- Autoritativer Server: https://github.com/opengenesislink/opengenesislinkserver , vor JEDEM künftigen Entwicklungsblock aktuelle `main`-HEAD-SHA neu prüfen und kompatible Server-CAPS/LLUDP-/API-Codepfade abgleichen
- Firestorm Quell-/Feature-Referenz: https://github.com/FirestormViewer/phoenix-firestorm (offizielle Releasebranches, nicht Nightly); Details: `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`
- Repo: `marlin198323-glitch/nexverse-viewer`
- lokaler Arbeitsbranch: `develop-current`
- GitHub-Handoff-Branch: `develop-current`
- Hinweis: GitHub-`develop` besitzt eine ältere divergierende Historie; **nicht** als aktuellen Stand verwenden, bis sie bewusst reconciled wurde.
- lokal: `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`
- vcpkg: `/home/jam/vcpkg`
- `projectplan.md` ist bindend; seit 11.10.2026 inklusive vollständiger Firestorm-Parität und aktueller OpenGenesisLINK-Server-HEAD-Referenz
- `docs/FIRESTORM_PARITY_MATRIX.md` ist die offene Feature-Inventur; ältere Ausschlüsse von RLV/Phototools gelten nicht mehr

## 2. Neuer Chat – zuerst

```bash
cd '/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer'
git status --short --branch
git log -8 --oneline
cat VERSION
```

Wenn direkt über GitHub gearbeitet wird, zuerst Branch `develop-current` verwenden. Danach lesen: `CHATUEBERGABE.md`, `projectplan.md`, `info.md`, **`docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`**, `docs/ARCHITECTURE.md`, `docs/DESIGN.md`. Zusätzlich vor jedem neuen Feature und Release aktuellen OpenGenesisLINK-Server-`main`-HEAD und dessen relevante Implementierungsdateien prüfen; der einmalige SHA vom 11.10.2026 ersetzt **keine** erneute Aktualitätsprüfung.

Keine destruktiven Resets/Cleans gegen den lokalen Stand.

## 3. Aktueller Code-Stand

Version: `0.2.0-alpha.79`.

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


## 0.2.0-alpha.61 – Phase 9 Handoff · 11.10.2026

**Block A – CPU-Mip-Vorstufen:** `texture_mip_image` erzeugt bis zu drei kleinere RGBA8-Basisstufen mit alpha-gewichteter Mittelung (premultiplizierte Farbsummen); NPOT-Bilder werden begrenzt unterstützt. Der Renderer erzeugt darauf weiter gewöhnliche OpenGL-3.3-Mipmapketten. Es wird kein JPEG2000-Progressive- oder HTTP-Chunk-Streaming vorgetäuscht.

**Block B – Distanzabhängige Qualitätswahl:** `plan_texture_mip_levels` wählt 0/1/2/3 je nach Agentabstand (24/64/128 m), mit Hysterese an den Grenzwerten. Mehrfach verwendete Texture-UUIDs verwenden die nächste Objektposition; aktive Partikeltexturen erhalten Vorrang mit Level 0. Die Planung läuft bei Scene-Revision oder spätestens etwa alle 750 ms.

**Block C – Asynchroner Full-Resolution-Cache/Hot-Reload:** HTTP und der bestehende 256-MiB-RGBA8-Disk-Cache speichern ausschließlich Originalbilder. Der asynchrone Worker erstellt daraus die gewünschte CPU-Mipstufe. Ändert sich die Entfernung, kann die vorhandene GPU-Textur erneut hochgeladen und verkleinert oder vergrößert werden, ohne die ursprüngliche GetTexture-Assetdatei erneut laden zu müssen. Region-/Epochenschutz, Residency und veraltete Qualitäts-Requests werden vor dem Upload geprüft.

**Block D – Geplantes 256-MiB-GPU-Texturbytebudget:** Mit `gpu_rgba8_mip_bytes` werden RGBA8-Mipketten berechnet; `plan_texture_byte_budget` kann vor Upload Texturen niedrigerer Priorität zur Eviction bestimmen. Höher priorisierte Texturen werden nicht verdrängt, um weiter entfernte Assets nachzuladen. Kann ein Upload nicht ins Byte-/512-Textur-Slotbudget passen, wird er zurückgestellt statt unkontrolliert zu wachsen. Telemetrie: aktuelle geschätzte Texturbytes, LOD-Swaps, zurückgestellte Uploads und GPU-Evictions.

**Verifikation:** Debug-Build C++23, 35/35 CTests einschließlich RGBA-Alpha, ungerader Dimensionen, Mip-Byteberechnung, Distanzhysterese und Budgetpriorität. OpenGL-3.3-GUI-Smoke nach Versionspflege. Live-In-World-Abnahme mit echtem NexVerse-Login steht aus.

**Abgrenzung:** Die 256 MiB sind ein softwareseitig aus RGBA8-Mipdimensionen berechnetes Texturbudget, keine gemessene oder vollständig durchgesetzte physische VRAM-Gesamtgrenze. Terrain-, UI- und Mesh-GPU-Ressourcen gehören nicht zu diesem Texture-Budget. Bei sehr großen/zahlreichen Bildassets können zusätzliche Inhalte unter Budgetdruck vorerst im Fallback bleiben.

**Offen:** Netzwerk-Chunks und native JPEG2000-Progressive-LOD, effizientes CPU-RAM-Caching für sämtliche Texturlevel, gemeinsames GPU-Bytebudget für Mesh/Terrain/Texture, authentifizierter Streaming-Sichttest; bekannte Avatar-/WASD-/Mesh-Asset-/Teleportfehler bleiben auf Nutzerwunsch für die spätere gesammelte Fehlerbehebung dokumentiert.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl61-live.log`.

**Nächste Releasegruppe (3–4 Blöcke):** Asset-Prefetch nach Sichtkegel, Texture-Mip-Debounce/RAM-Slab-Cache, gekoppelte GPU-Budgets für Mesh+Texture+Terrain (zunächst vorsichtig), messbare Streaming-Performance. Weiter nur auf `develop-current` arbeiten, divergiertes `develop` nicht verwenden. Nach jedem Großblock Build + alle Tests + GUI-Smoke, `projectplan.md`/`info.md`/`CHATUEBERGABE.md`/einschlägige Docs aktualisieren, Commit und GitHub-Synchronisierung.


## 0.2.0-alpha.62 – Handoff und nächste Arbeit · 11.10.2026

**Block A – Kameraabhängige Asset-Priorität:** Ein weicher Blickrichtungsfaktor gewichtet Textur- und Mesh-Asset-Anfragen relativ zur Third-Person-Kamera. Objekte hinter dem Betrachter werden nicht ausgeblendet; sehr nahe Objekte bleiben grundsätzlich hoch priorisiert. Die bestehenden Entfernungssortierungen und GPU-Residency-Grenzen bleiben erhalten. Das ist keine echte Frustum- oder Occlusion-Culling-Implementierung.

**Block B – CPU-RAM-Texturcache:** Ein threadgeschützter 96-MiB-LRU-Cache hält vollständige dekodierte RGBA8-Bilder pro gültiger UUID vor. Der asynchrone GetTexture-Pfad arbeitet RAM → persistenter Diskcache → HTTP/Decoder. Für kleinere GPU-Mip-Basisbilder wird das dekodierte Original aus dem RAM wiederverwendet. Keine Cache-Speicherung von CAPS-URLs, Tokens oder ungültigen Bildern.

**Block C – gemeinsam begrenzte GPU-Mesh-/Objekttexturbytes:** Geschätztes, softwareseitiges gemeinsames Budget von 384 MiB mit zusätzlich jeweils 256 MiB als Maximalgrenze für Mesh-Buffer und Objekttexturen. Vor GPU-Upload werden bestehende Ressourcen und ersetzte LOD-Varianten eingerechnet. Bei Überschreitung wird ein Mesh-Upload zurückgestellt; Texturen können zuerst niedrig priorisierte Texturen verdrängen. Bei freiwerdendem Speicher werden zurückgestellte Anfragen wieder berücksichtigt. Dies ist ausdrücklich keine vollständige VRAM-Bilanz: Terrain, UI, Treiber-Overhead und weitere Ressourcen sind nicht enthalten.

**Block D – Streaming-Diagnose und Regressionen:** 60-s-Metriken für RAM-Treffer, RAM-Größe/Einträge, GPU-Mesh-/Texturbytes, gemeinsames geschätztes Budget und Deferred Counts. Durchschnittliche Zeit vom Asset-Worker-Start bis zur Verarbeitung der fertigen Future (keine reine Netzwerklatenz). Neue Tests für Kamera-Priorität, sicheren gemeinsamen Budget-Rechenweg sowie den parallelen Textur-RAM-LRU-Cache.

**Verifikation:** C++23-Debug-Build und 38/38 automatisierte CTests erfolgreich. OpenGL-3.3-Welcome-Starttest nach Versionspflege ausführen. Kein bestätigter In-World-Test des NexVerse-Asset-Streamings.

**Bewusst offen:** echte Frustum-/Occlusion-Abfrage, globale inklusive Terrain/UI gemessene VRAM-Budgetierung, inkrementeller J2C-Netzstream, Avatar-Skeleton/Bakes, Bewegungs-Synchronisierung und vollständige Teleport-/Circuit-Behandlung. Bekannte Mesh-/Texture-Serverfehler bleiben in der späteren Fehlerbehebungsphase.

**Nächste 3–4 Blöcke:** Asset-Streaming-Queue/Prioritätsregeln, planbares Prefetching mit Limits, Cache-/Regions-Invalidierung und zusätzliche Mess- und In-World-Integrationsdiagnostik.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl62-live.log`. Verbindlicher GitHub-Handoff-Branch: `develop-current`.

Neuer Chat: zuerst GitHub-Branch `develop-current` und `CHATUEBERGABE.md`, danach `projectplan.md` und `info.md` lesen. Je Großrelease 3–4 zusammenhängende Blöcke, vor Commit Build, 38 CTests, Linux-GUI-Smoke, dann Versionsdatei, Projektplan, Übergabe und technische Dokumentation mitsamt Code zu GitHub hochladen. `develop` nicht automatisch zusammenführen. Der letzte Release-Commit ist in `git log` zu überprüfen.


## 0.2.0-alpha.63 – GitHub-Übergabe · 11.10.2026

**Block A – GetTexture-/GetMesh-Anfrage-Taktung:** `AssetRequestPacer` taktet neu gestartete Textur-Demand-Worker auf mindestens 125 ms und Mesh-Demand-Worker auf mindestens 180 ms Abstand. Die bereits vorhandene Kamera-/Distanz- und Partikelpriorität bleibt maßgeblich; höchstens je ein aktiver Demand-Worker pro Assettyp und keine künstliche Blockade bereits laufender Requests. Pacer werden bei Regionswechsel zurückgesetzt.

**Block B – begrenztes Texture-Prefetching:** Der Viewer wärmt vor dem eigentlichen GPU-Upload vollständige RGBA8-Bilder aus dem persistenten Texture-Diskcache in den 96-MiB-RAM-LRU vor. Die Auswahl berücksichtigt die bereits priorisierte GPU-Residency-Reihenfolge, ignoriert GPU-residente und bekannte fehlgeschlagene UUIDs und untersucht maximal vier Kandidaten pro Durchgang. Es läuft höchstens ein Prefetch-Worker gleichzeitig, mit mindestens 650 ms Abstand und maximal 48 Prefetch-Kandidaten pro Region. Es finden dabei ausdrücklich keine zusätzlichen HTTP-Anfragen oder GPU-Uploads statt.

**Block C – Regions-Epochen und Cache-Admission:** `TexturePrefetchScope` setzt geprüfte Kandidaten bei neuem `material_scope.epoch()` zurück. Die aktuell gültige Regionsepoche wird atomar an den Prefetch-Worker weitergegeben; ein beim Abschluss veralteter Disk-Read wird vor der Cache-Aufnahme zurückgewiesen. Bestehende GetTexture-/GetMesh-Epochenschutzprüfungen im GPU-Pfad bleiben unangetastet. Diskcache und RAMcache sind UUID-basiert, CAPS-Tokens werden nicht gespeichert. Prefetch-Worker lassen sich nicht während laufender Dateizugriffe zwangsweise abbrechen.

**Block D – Telemetrie und Regressionstests:** Das 60-Sekunden-Streaminglog meldet Demand-Starts je Assettyp sowie Prefetch-Versuche, erfolgreiche RAM-Erwärmungen, Diskcache-Misses und stale Prefetch-Resultate. Neue deterministische Tests für Zeitintervalle, Kandidaten-Limits und Epochensprünge; Integrationstest des echten Disk→RAM-Prefetch-Pfads einschließlich manipuliertem Cachefile. `TextureMemoryCache::contains()` ermöglicht den Kandidatenabgleich ohne teure Bildkopie.

**Verifikation:** C++23-Debug-Build, 40/40 CTests einschließlich neuer Scheduler- und Disk-Prefetch-Tests. Linux OpenGL-3.3-GUI-Starttest nach Versionspflege. Authentifizierter NexVerse-In-World-Streaming-Test ist noch ausstehend.

**Grenzen:** Kein HTTP-Prefetch, kein eigenständiger parallel ladender Mesh-Prefetcher, kein inkrementeller J2C-Assetstream. 384-MiB-Budget bleibt eine Schätzung für Mesh-/Objekttextur-GPU-Ressourcen ohne Terrain/UI und Treiber-Overhead. Unvollständiger Avatar, Bewegung, Teleport und einzelne Mesh-/Texturserverfehler bleiben entsprechend Projektplan für die spätere Bugfixphase dokumentiert.

**Nächste 3–4 Blöcke:** Recovery und Fehlerklassifikation des Asset-Loaders, temporäre Retry-/Backoff-Logik statt permanenter Fail-Sets, robustere Regionsepochen-Aufräumlogik und In-World-Assetmetriken. `projectplan.md` bleibt bindend.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl63-live.log`; Branch `develop-current`.

Verbindlicher Wiedereinstieg im neuen Chat: `develop-current`-Stand und diese Datei lesen, danach `projectplan.md` und `info.md`. Vier gebündelte Entwicklungsblöcke und nach jedem Release Build, alle Tests, GUI-Smoke, Versions- und technische Dokumentationsupdates, gemeinsamer Push nach GitHub. `develop` hat weiterhin eine ältere divergierende Historie und darf nicht automatisch überschrieben werden.


## 0.2.0-alpha.64 – Projektübergabe · 11.10.2026

**Block A – strukturierte Asset-Fehlerklassifikation:** Der GetTexture- und GetMesh-HTTP-Pfad liefert zusätzlich zum Diagnosetext `AssetFailure`: `transient` (Timeout/Transport, 408/425/429/5xx), `missing` (404/410), `invalid` (fehlerhafter Decoder/unerwartete HTTP-4xx) oder `configuration` (400/401/403, fehlender Transport/ungültige CAPS-URI). Bei PNG→J2C beziehungsweise GetMesh2→GetMesh-Fallback bestimmt die kombinierte Fehlerklasse die Wiederholungsstrategie; ein erfolgreicher Fallback gilt als Erfolg.

**Block B – zeitlich begrenzte Retry-/Backoff-Logik:** `AssetRetrySchedule` ersetzt die bisher bis zum Regionswechsel dauerhaft gesperrten Fehler-UUIDs. Temporäre Fehler: nach 2, 4 und 8 Sekunden erneute Versuche, danach fünf Minuten Sperrzeit; HTTP 404/410: zehn Minuten; ungültige Assets: 30 Minuten; Konfigurationsfehler: fünf Minuten. Erfolgreiche GPU-Uploads löschen den zugehörigen Fehlerstatus. Mesh-Fehlerstatus unterscheidet UUID und angeforderten LOD; GPU-Bytebudget-Deferrals bleiben weiterhin getrennt. Die Map ist auf 4096 Einträge begrenzt; bei darüber hinausgehender Kartenfluktuation wird sie geleert.

**Block C – regiongebundene Fehlerverwaltung:** Bei `material_scope.epoch()`-Wechsel werden Mesh- und Textur-Retrytabellen geleert. Bereits laufende Downloads werden nicht auf dem GUI-Thread blockiert oder abgebrochen. Fertige Antworten aus alten Epochen bleiben durch die bestehenden Epochenschutzprüfungen vom GPU-Upload ausgeschlossen; zusätzliche Stale-Counter dokumentieren diese Verwerfungen. Cache-Dateiformate und Mesh-/Texture-Cachebudgets bleiben unverändert.

**Block D – Diagnose und Regressionstests:** Streaminglogs führen `failure_events`, `retry_dispatches`, `backoff_assets`, `transient_failures` und `stale_results` je Mesh-/Textur-Familie. Tests prüfen Klassifikation von HTTP-Antworten, Rechenfolge des Backoffs, Regionsepochen, Zurücksetzen bei Erfolg und beide echten Loader-Fallback-Pfade (404 und 503) mittels Mock-HTTP-Transport.

**Verifikation:** Vollständiger C++23-Debug-Build und 41/41 CTests sind erfolgreich. Nach Versionspflege erfolgen erneuter vollständiger Rebuild und Linux OpenGL-3.3-Welcome-Starttest. Ein authentifizierter NexVerse-In-World-Streaming-Test ist weiterhin nicht ausgeführt.

**Grenzen und offene Arbeiten:** Diese Verbesserung beseitigt nicht die eigentlichen Fehler einzelner Server-Assets. Sie verhindert stattdessen endlose schnelle Retries beziehungsweise permanente lokale Sperren. Tatsächliche Simulator-Synchronisierung, Avatar-LocalID/Appearance, Bewegungsanzeige sowie Regions-Teleport bleiben wie vereinbart für spätere Fehlerblöcke offen. Ein genereller Netzwerkanfrage-Abbruch mittels Cancellation-Token ist nicht implementiert.

**Nächster Release (3–4 Blöcke):** In-World-Integrationsdiagnostik und Echtzeit-Aussagekraft, bessere Unterscheidung von Cache-/Serverfehlern, mögliche asynchrone Netzwerk-Cancellation und Asset-Streaming-Queue-Optimierung; die verbindliche Roadmap bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl64-live.log`; maßgeblicher Branch ist `develop-current`.

Neuer Chat: zuerst GitHub `develop-current` und diese Datei, danach `projectplan.md` und `info.md` lesen. 3–4 koordinierte Blöcke je Release, `cmake --preset debug`, `cmake --build --preset debug -j2`, `ctest --preset debug --output-on-failure`, GUI-Smoke unter DISPLAY=:0; dann Version, Projektplan, Übergabe, technische Docs aktualisieren und Code+Docs auf `develop-current` pushen. `develop` nicht automatisch mergen oder zurücksetzen.


## 0.2.0-alpha.65 – Handoff und nächste Blöcke · 11.10.2026

**Block A – Cache-Miss- und Dateifehlerdiagnose:** `AssetCacheLookup` und die optionale Statusausgabe von `TextureDiskCache::load()` unterscheiden `hit`, `miss`, `corrupt`, `io_error`. Der bereits vorhandene Disk-Cache-Header, die CRC-artige FNV64-Inhaltsprüfung, die 256-MiB-Ressourcengrenze und UUID-Validierung bleiben kompatibel. Texturworker berichten Disk-Misses, beschädigte Einträge und Dateisystemfehler getrennt von HTTP-Versuchen. Diese Quellenklassifikation verändert keine Serverantworten.

**Block B – koordinierte Regions-Epochen im Asset-Worker:** Der bestehende atomare Live-Epochenmarker aus dem Textur-Prefetch wird auch von Demand-GetTexture und GetMesh verwendet. Worker prüfen die Epoche vor Beginn und unmittelbar vor einem bislang nicht gestarteten HTTP-Download. Nach einem bereits laufenden HTTP-Download prüfen sie erneut vor dem Schreiben in den RAM-/Disk-Cache. Die `load_mesh_cached`-Funktion bietet dafür einen optionalen Admission-Callback. Die GPU-Ladepfade verwerfen nach wie vor veraltete Epochen. Bereits laufendes libcurl-HTTP kann noch nicht per Cancellation-Token unterbrochen werden.

**Block C – In-World-Health-Telemetrie:** Nach Aufbau der Simulation schreibt das Rendering alle etwa 30 Sekunden eine Kennzahlenzeile `InWorld asset health` (Regionsepoche, Anzahl der Szenen-Samples, gewünschte vs. bereits GPU-residente UUIDs und Deckungsgrad in Prozent, fehlende Assets, laufende Texture-/Mesh-Worker, Retry-Einträge und geschätzte GPU-Mesh-/Objekttexturbytes). Ohne aktive Simulation erfolgt keine falsche In-World-Bestandsmeldung. Die Coverage-Metrik bedeutet nur UUID-Residency, nicht vollständige grafische Darstellung, Avatar- oder Netzwerk-Korrektheit.

**Block D – Regressionen und Detailzähler:** Der Diskcache-Test prüft saubere Treffer, Miss und beschädigte Datei. Der Meshcache-Test prüft zwei Abbruchsituationen, einmal vor HTTP und einmal nach Abschluss einer künstlich veralteten Downloadfunktion; in beiden Fällen gelangt nichts Falsches in RAM/Disk. Der neue Test `asset_scene_coverage_tests` prüft leere Szenen, deduplizierte UUIDs, teilweise und vollständige Abdeckung. Die GetTexture-/GetMesh-Streaminglogs nennen zusätzlich `skipped_before_http`, `stale_after_http`, `disk_corrupt` und `http_attempts`.

**Teststand:** C++23-Debug-Build und 42/42 CTests nach Implementierung erfolgreich. Für die finale Version erfolgt nochmals vollständiger Rebuild und Linux-OpenGL-3.3-Welcome-Smoke. Es gibt weiterhin keinen authentifizierten NexVerse-In-World-Livetest; die In-World-Kennzahlen sind instrumentiert, aber noch nicht an realen Simulatordaten verifiziert.

**Unverändert offen:** Eigentliche serverseitig fehlende/beschädigte Mesh- und Texturassets, Avatar-/WASD-/Teleport-Funktionsdefizite und die vollständige visuelle Weltabnahme. Die Fehler werden protokolliert, aber nicht fälschlich als bereits behoben markiert. Keine echte libcurl-Abbruch-API in diesem Release.

**Nächste 3–4 Blöcke:** Realistisches Asset-Recovery-Verhalten unter Last, Retry-Schleifen und Prioritäts-Stabilisierung, strukturierte Diagnostics-/Health-Snapshots statt nur Textlogs und Renderer-/Szenenabnahme. `projectplan.md` bleibt verbindlich.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl65-live.log`. Branch: `develop-current`.

Neuer Chat: `develop-current`-Branch auf GitHub und diese Übergabe zuerst prüfen; danach `projectplan.md`, `info.md` und technische Docs. 3–4 gebündelte Blöcke, alle Debug-Tests, OpenGL-GUI-Smoke, Code/Tests/Docs zusammen pushen. Keine ungesicherten lokalen Änderungen oder automatisches Umschalten auf den divergierenden Branch `develop`.


## 0.2.0-alpha.66 – Projektübergabe · 11.10.2026

**Block A – stabile Asset-Auswahl:** `AssetRequestFairness` integriert die bereits vorhandene Kamera-/Entfernungs- und Partikel-Priorisierung mit der pro Asset/LOD geführten Retry-Tabelle. Neue, berechtigte Assets werden innerhalb ihrer bisherigen Reihenfolge bevorzugt; bei einem bereiten Retry wird jeder vierte tatsächliche Dispatch für eine Wiederholungsanfrage reserviert. Bei ausschließlich bereiten Retries werden diese unmittelbar berücksichtigt. Ungültige UUID-Leereinträge, vollständig geladene Assets, Budget-Deferrals und noch aktive Cooldowns bleiben ausgeschlossen. Per-Region-Zähler werden beim Epochensprung zurückgesetzt. Das ist eine kleine Fairness-Schicht vor den bestehenden unabhängigen Textur-/Mesh-Demand-Workern, keine parallele Netzwerk-Multiplexer-Queue.

**Block B – stabile Retry-Zustände unter Last:** `AssetRetrySchedule` setzt bei mehr als 4096 Einträgen nicht mehr die gesamte Retry-Tabelle zurück, sondern verdrängt nur den Eintrag mit dem frühesten Retry-Zeitpunkt. Neue `blocked_count(now)`- und `ready_retry_count(now)`-Metriken trennen noch gesperrte Assets von wieder zulässigen Wiederholungsversuchen. Die bestehenden kurzen 2-/4-/8-Sekunden-Transient-Retries und längeren Cooldowns aus alpha.64 bleiben erhalten. Alte Fehlerzustände werden weiterhin bei einem Regionsepochen-Wechsel invalidiert.

**Block C – strukturierte In-World-Streaming-Snapshots:** Zu den vorhandenen 30-Sekunden-Health-Logs kommt `InWorldAssetSnapshot` mit maschinenlesbarem, numerischem JSON-Schema `ogl.asset.streaming.v1`. Felder: Regionsepoche, Szenensamples, gewünschte/geladene/offene Textur-/Mesh-UUIDs samt Abdeckungsprozent, aktuell laufende Asset-Worker, noch gesperrte und retryfähige Fehler, RAM-Cachebytes und die geschätzte GPU-Mesh-/Objekttexturbelegung inklusive gemeinsamem Softwarebudget. Ohne `simulation_ready` wird kein In-World-Snapshot behauptet; ein leeres Desired-Set ergibt rechnerisch 100 % Coverage und ist kein Nachweis einer vollständigen Welt.

**Block D – Streaming-Integrationstests:** Drei neue CTests: Fairness/Reihenfolge/Duplikate/Deferred-Assets/LOD-Keys, JSON-Schema inklusive leerer Region sowie die gemeinsame Planung aus Texture-Residency, Retry, Byte-Budget, Region-Reset und Health-Snapshot. Zudem ein Stress-Test mit 4098 Failure-Einträgen und den getrennten blocked-/ready-Kennzahlen. C++23-Debug-Build und 45/45 CTests bestanden; vollständiger Release-Rebuild und GUI-Starttest nach Versionserhöhung folgen.

**Wichtige Abgrenzung:** Die neue Integrationstest-Suite prüft die Asset-Planungs- und Budgetkomponenten, nicht einen authentifizierten In-World-Renderer- oder OpenSim-Server-End-to-End-Test. Die bereits bekannten Defizite (Avatar-Körper und Bewegung, fehlende Meshobjekte, fehlerhafte Assetantworten, Teleport und EventQueue) bleiben im Projektplan dokumentiert und werden wie vereinbart in späteren Fehlerbehebungsphasen behandelt. Die GPU-Bytes bleiben geschätzte Mesh-/Objekttexturwerte ohne Terrain-, UI- und Treiber-Overhead.

**Nächster 3–4-Blöcke-Release:** Datengetriebene Auswertung realer Simulatorlogs und Regionen, Entzerrung von Textur-/Mesh-LOD-Swaps, Renderer-GPU-Upload- und Slot-Backpressure sowie reproduzierbare In-World-Testpfade. `projectplan.md` bleibt die maßgebliche Roadmap.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl66-live.log`. Alle Änderungen auf `develop-current`; `develop` nicht automatisch verwenden.

Wiedereinstieg: Repository `marlin198323-glitch/nexverse-viewer`, einzig maßgebliche Branch `develop-current`, lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`; zuerst diese Übergabe, danach `projectplan.md`, `info.md`, einschlägige Docs und `git status` lesen. Pro Release drei bis vier größere Blöcke, anschließend Build, alle CTests, DISPLAY=:0 GUI-Smoke, Release-Version und sämtliche Markdown-Dokumente, Git-Commit und HTTPS-GitHub-Push. Nicht automatisch auf `develop` wechseln oder bestehende Geschichte zurücksetzen.


## 0.2.0-alpha.67 – GitHub-Übergabe · 11.10.2026

**Block A – tatsächliches Viewerlog analysiert:** `tools/ogl_asset_log_report.py` (Python 3, Standardbibliothek) fasst das reale `$HOME/.local/state/opengenesislink/viewer.log` sowie künftige `InWorldAssetSnapshot`-Zeilen in einem numerischen, UUID-/CAP-URL-freien JSON-Bericht zusammen. Der historische Stand vom 11.10.2026 enthielt 230 Mesh- und 56 Texture-Fehlermeldungen, davon 42 mit 404 und eine mit 5xx-Muster; 243 weitere sind historische unklassifizierte Meldungen. Validierte In-World-Snapshots waren in dieser Datei noch nicht vorhanden. Dokumentierte Bedienung: `docs/LOG_ANALYSIS.md`.

**Block B – LOD-Dwell gegen Flackern:** Die neue generische `AssetLodDwell<Quality>` verlangt für wechselnde GPU-residente Mesh-LODs und Textur-Basismips eine über 1400 ms stabile Zielstufe. Neue noch nicht GPU-residente Assets werden sofort bedient; aktive Partikeltexturen müssen weiterhin mit höchster Bildauflösung angefordert werden. Die Anti-Oszillationszustände werden bei Regionswechsel geleert und von verschwundenen Scene-IDs bereinigt. Bereits bestehende Distanz-/Größen-LOD-Hysteresen bleiben wirksam.

**Block C – Renderthread-Upload-Backpressure und Slotfreigabe:** Der gemeinsame `AssetGpuUploadGate` hält mindestens 30 ms Abstand zwischen erfolgreichen Objekttextur- und Mesh-GPU-Uploads. Fertige Worker-Futures bleiben bis zum nächsten Upload-Fenster ausstehend; es gehen keine bereits dekodierten Daten verloren. `plan_texture_slot_admission` prüft zusätzlich zur bytebasierten Planung das 512-Slot-Limit: Ist das Limit erreicht, darf ein eingehender, höher priorisierter Textur-UUID einen niedrig priorisierten GPU-Slot ersetzen. Die Budgetrechnung berücksichtigt bereits geplante Byte-Evictions und verhindert die Verdrängung höher priorisierter Assets. Terrain-/UI-GPU-Uploads gehören nicht zur neuen Sperre.

**Block D – Regressionen und Telemetrie:** Zwei neue C++ CTests für LOD-Dwell/Regionsreset und GPU-Upload-Gate/Slot-Zuteilung; ein Python-CTest prüft den Parser mit Legacylogs, gültigem JSON, defektem Snapshot und Sanitizing unbekannter Felder. Die regulären Textur-Streaminglogs führen nun `mip_dwell_pending`, `slot_priority_evictions`, `gpu_uploads`, `gpu_uploaded_bytes`; die Mesh-Streaminglogs führen `lod_dwell_pending`. Ein vollständiger C++23-Debug-Build und 48/48 CTests waren erfolgreich; nach der Versionspflege werden sie erneut und zusätzlich der Linux-GUI-Smoke geprüft.

**Grenzen:** Die aus echten Logs gewonnenen Zahlen beschreiben historische Verbindungs-/Asset-Fehler und keine bestätigte Beseitigung der 404/Decoderprobleme. Neuer Renderthread-Takt reduziert zeitliche Upload-Spitzen, garantiert aber keine fixe Framezeit. Weiterhin ausstehend sind authentifizierte In-World-Abnahme, vollständige Mesh-Objekte, Avatar-Body/Bewegung, Teleport und EventQueue-Fehlerbehebung.

**Nächster Release (3–4 Blöcke):** Datenbasierte Prüfung anhand neuer In-World-Logs und GPU-Diagnose, weitere Upload-Messungen, scene-priority-Feedback und gezielte Render-Integration. `projectplan.md` ist weiter maßgebend; bei ausdrücklicher Nutzerpräferenz Fehlerbehebung separat.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl67-live.log`. GitHub-Branch `develop-current`.

Neuer Chat: zuerst den GitHub-Branch `develop-current` und `CHATUEBERGABE.md` prüfen, anschließend `projectplan.md`, `info.md` und `docs/LOG_ANALYSIS.md`. Immer 3–4 größere Entwicklungsblöcke, danach CMake-Debug, alle CTests, GUI-Smoke, Versionspflege, komplette Docs und Code auf `develop-current` pushen. Keine automatischen Merges aus `develop`. Authentifizierter Simulator-Test ist weiterhin offen.


## 0.2.0-alpha.68 – Übergabe und Fortschritt · 11.10.2026

**Block A – reale CPU-Uploadzeit statt Worker-Wartezeit:** `AssetUploadTelemetry` misst die Wandzeit, die das Render-Thread-Aufrufen von `set_object_texture(...)` und `set_mesh_asset(...)` benötigt. Das feste Ringfenster hält die letzten 64 Uploadversuche und bietet `recent_avg_us`, `recent_p95_us`, `recent_max_us`, `recent_count`; Nullzustand und Überlauf werden konservativ behandelt. Messgröße ist **CPU-seitige OpenGL-Übermittlung**, keine GPU-Fertigstellungszeit, kein `glFinish` oder OpenGL-Timer-Query.

**Block B – adaptives GPU-Upload-Backpressure:** `AssetGpuUploadGate` bleibt bei schnellen Uploads bei mindestens 30 ms Abstand; langsame CPU-Übermittlungen verlängern das nächste Uploadfenster auf etwa 4× beobachtete Millisekunden, bis maximal 150 ms. Der nächstfolgende kurze Upload senkt das Intervall wieder auf den Grundwert. Mesh und Textur teilen sich den Renderthread-Gate. Bereits fertiggeladene Futures bleiben für spätere Frames verfügbar. Terrain/UI und vorbestehende Renderer-Arbeiten unterliegen dem Gate nicht.

**Block C – Szenenpriorisierung und erweiterte Health-Snapshots:** `AssetScenePriorityFeedback` beobachtet alle etwa 750 ms die GPU-gewünschten UUID-Sets, erkennt erhebliche Wechsel (symmetrische Differenz mindestens ein Viertel der vorherigen/neuen Set-Größe) und erhöht für drei stabile Planzyklen den weichen Loaded-Bonus temporär: Mesh von 24 auf 42 gewichtete Meter, Textur von 8 auf 14. Neue näher gelegene Assets bleiben weiterhin auswählbar; keine UUID wird fest gepinnt. Regionsepochen löschen den Zustand. `InWorldAssetSnapshot` behält `ogl.asset.streaming.v1`, ergänzt rein numerische Felder zu CPU-Uploadavg/p95/max, Sampleanzahl, effektiver Uploadsperre und Texture-/Mesh-Prioritäts-Churn. Der vorhandene Log-Analyzer wertet zulässige Felder aus und berechnet Peak-p95 bzw. Peak-Churn; er gibt keine CAPS-URLs/UUIDs aus.

**Block D – echter Renderer-Integrationstest:** `renderer_gpu_asset_integration_tests.cpp` erzeugt einen unsichtbaren echten GLFW/OpenGL-3.3-Kontext, lädt/repliziert/entfernt synthetische Textur und Mesh auf dem GPU-Pfad, prüft ungültige Meshdaten, führt einen Renderer-Frame aus, kontrolliert `visible_objects`, Renderer-Backend und GL-Fehlerfreiheit. Ohne OpenGL-Display überspringt CTest den Test via Exitcode 77; auf dem autorisierten Linux-PC wurde der Test unter `DISPLAY=:0` vollständig ausgeführt. Zwei weitere C++-Tests prüfen Rolling-Telemetrie und Scene-Prioritätsrückkopplung; erweiterte JSON-/Log-/Upload-Gate-Regressionstests. Gesamt: **51/51 CTests bestanden**, inkl. realem OpenGL-Test, anschließend vollständiger Versions-Rebuild/GUI-Smoke.

**Grenzen:** Der Renderer-Integrationstest arbeitet mit synthetischer Geometrie und einem lokalen GPU-Kontext, ohne authentifizierte NexVerse-Region oder echtes Asset. Er belegt noch nicht, dass Avatar-Body/Bewegung, Mesh-Objekte aus dem Simulator, CAPS-EventQueue, Teleport oder alle Weltassets korrekt sind. Diese bekannten Punkte bleiben bis zur vereinbarten späteren Bugfixphase dokumentiert. CPU-Uploadzeiten dürfen nicht als verifizierte GPU-Latenz bezeichnet werden.

**Nächster 3–4-Blöcke-Release:** Wiederholbare Smoke-/GPU-Profiling-Sessions anhand synthetischer Szenen unterschiedlicher Größe, Renderer-Upload-Fehlertransparenz, Scene-Priority-Schutz unter voller Asset-Residency und Ausbau der testbaren Welt-/Mesh-Renderpfade. Maßgeblich bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl68-live.log`. CLI-Logauswertung: `python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"`; Source Branch `develop-current`.

Wiedereinstieg: GitHub `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`, lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, diese Datei, verbindliches `projectplan.md`, `info.md`, technische Docs lesen. Vier zusammenhängende Blöcke, `cmake --preset debug`, `cmake --build --preset debug -j2`, `env DISPLAY=:0 ctest --preset debug --output-on-failure`, Linux GUI-Smoke und Push nach GitHub. Der neue reale Render-GPU-CTest braucht einen gültigen GL3.3-Kontext und zeigt andernfalls einen expliziten Skip; auf dem Linux-PC ist `DISPLAY=:0` der bekannte aktive X11-Displayserver.


## 0.2.0-alpha.69 – Projektübergabe und nächste Schritte · 11.10.2026

**Block A – echter synthetischer GPU-Kapazitäts-/Belastungstest:** `tests/renderer_gpu_pressure_tests.cpp` erzeugt im versteckten OpenGL-3.3-Kontext auf dem Linux-Testrechner tatsächlich 512 kleine GPU-Objekttexturen und 256 kleine GPU-Meshes (Renderer-Grenzen). Bei 16, 128, 256 und 512 Texturen wird ein Frame mit den aktuell angelegten Szenenobjekten gezeichnet; Tests prüfen `visible_objects`, `draw_calls`, OpenGL-Fehlerfreiheit, abgewiesene 513. Textur/257. Mesh, korrekte Ersetzung bestehender Ressourcen sowie erneute Aufnahme nach Slot-Freigabe. Beispiel: unter dieser synthetischen Auslastung ca. 35 ms CPU-Gesamttestzeit inklusive Context-Init/Uploads/Draws. Diese Zeit ist ausdrücklich KEIN Benchmark realistischer größerer OpenSim-Assets.

**Block B – verlässliche GetTexture-GPU-Upload-Rückmeldung:** Die Renderer-Schnittstelle `Renderer::set_object_texture` meldet nun `bool` statt `void`. `LegacyOpenGlRenderer` baut Texturen immer erst in einem neuen GL-Namen auf, prüft Dimensionen und GL-Fehler, übernimmt sie nur bei erfolgreichem Upload und löscht den vorherigen Textur-Namen erst danach (transaktionale Ersetzung). Bei fehlerhaften Daten, vollem internen GPU-Slotlimit oder GL-Fehlern kehrt sie `false` zurück, ohne eine vorhandene Textur derselben UUID zu zerstören. Die Anwendung zählt `texture_gpu_upload_failures`, gibt eine Warnung aus, plant einen transienten Retry und trägt fehlgeschlagene Uploads nicht mehr als GPU-resident ein. Überlegungen zur atomaren Freigabe *anderer* prioritätsbedingt zuvor verdrängter Texturen bleiben ausdrücklich offen.

**Block C – robusterer GetMesh-GPU-Pfad:** Schon vor GPU-Submission prüft der Renderer Meshvertex-Stride, Dreiecks-Indexvielfache, jeden Index gegen die tatsächlich vorhandenen Vertices und angegebene Face-Bereiche gegen den Indexbuffer. Der eigentliche GL-Mesh-Upload kontrolliert Fehler und gültige VAO/VBO/EBO-Handles und gibt teilweise angelegte GL-Ressourcen bei Fehlern frei. Vorheriger LOD-Buffer bleibt bei gescheiterter Ersetzung erhalten. Viewer meldet `mesh_gpu_upload_failures`, klassifiziert GPU-Uploadfehler als transient und plant einen Backoff statt eines permanenten `invalid`-Fehlerstatus.

**Block D – Vollauslastung und Logdiagnose:** Der neue CTest `asset_full_residency_tests.cpp` übt 80 neue vorrangige Partikeltexturen bei 512 belegten Slots und eine Kamera-Fahrt über 400 Mesh-UUIDs bei 256 GPU-Slots aus. Er kontrolliert Priorität, Freigabe, Slotlimits und das Verbleiben naher/nützlicher Assets. `InWorldAssetSnapshot` führt zusätzliche Zähler `texture_gpu_upload_failures` und `mesh_gpu_upload_failures` im vorhandenen numerischen Schema `ogl.asset.streaming.v1`; der Python-Loganalysator nimmt sie in die sichere Feldauswahl auf und zeigt die jeweiligen Maximalwerte. JSON-, GL-Renderer- und Log-Regressionstests angepasst. C++23-Debug-Build und **53/53 CTests** inklusive zweier realer OpenGL-Kontexttests erfolgreich; vollständiger Release-Rebuild, Versionstest und GUI-Start folgen.

**Unverändert offene Grenzen:** Ein OpenGL-Test mit kleinen künstlichen Assets validiert Ressourcenhandling und GPU-Slots, aber weder echtes GetMesh/OpenSim-Decoding noch Live-Weltgeometrie, Avatar, Netzwerk-EventQueue, Teleport und In-World-Interaktion. Tests laufen auf echtem GL3.3/NVIDIA-Linux mit `DISPLAY=:0`; ohne GL-Display überspringen sich die beiden GPU-Kontext-CTests per Returncode 77. Shared GPU-Budget von 384 MiB ist eine Software-Schätzung ohne Terrain/UI/Treiber-Speicher. Die atomare Textur-Ersetzung gilt nur für denselben UUID-Slot, nicht für bereits vorab verdrängte andere Slots.

**Nächster Release / verbindlicher Plan:** Schrittweise GPU-Upload-Rollback/Recovery bei Slot-Evictions, realistischere Szenengeometrien/Texture-Mip-Lastprofile, strukturierte Upload-Failure-Gründe und weitere Renderer-Timing-Integration. Avatar/Bewegung/Mesh-/Teleport-/CAPS-Bugfixes wie vom Nutzer vereinbart separat.

Run: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Pressure einzeln: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-pressure-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl69-live.log`; nur `develop-current` nutzen.

Wiedereinstieg nach Chatwechsel: `marlin198323-glitch/nexverse-viewer`/`develop-current`, lokal `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, `CHATUEBERGABE.md`, `projectplan.md` und `info.md` prüfen. Pro Release 3–4 große koordinierte Blöcke; Source/Tests/technische Docs aktualisieren, Debug-Preset bauen, unter DISPLAY=:0 ALLE CTests inkl. echter GL-Kontexte, Viewer-GUI-Smoke, GitHub-Branch verify & push. Divergierenden Branch `develop` nicht verwenden. Authentifizierter Live-OpenSim-Welt-Test bleibt offen.


## 0.2.0-alpha.70 – Handoff und nächste Entwicklungsblöcke · 11.10.2026

**Block A – texturschonender, transaktionaler GPU-Austausch:** `Renderer::replace_object_texture(uuid, image, evictions)` übernimmt ein komplett neues OpenGL-Textur-Handle erst nach erfolgreichem `glTexImage2D` und `glGenerateMipmap`. Erst danach werden die vorab ausgewählten Eviction-UUIDs gelöscht und die neue Textur eingebunden. Bei Fehlschlag bleiben alle bisherigen UUID-Slots, inklusive der Eviction-Kandidaten, erhalten. Der Viewer spiegelt die Freigaben erst nach erfolgreichem GL-Commit in `object_texture_loaded_ids`, `texture_gpu_bytes`, Mip-Tabellen und GPU-Bytezähler; abgelehnte Uploads zählen nicht als geladene Ressource. Der bisherige bool-Aufruf `set_object_texture` bleibt als Wrapper erhalten.

**Block B – strukturierte GPU-Uploadergebnisse für Texture UND Mesh:** `GpuUploadResult`/`GpuUploadFailure` enthält `success`, `failure`, `gl_error` mit Klassen `none`, `invalid_input`, `capacity`, `unsupported_dimensions`, `allocation`, `driver_error`. Beide Rendererpfade bieten strukturierte Methoden; der bisherige bool-Mesh-Aufruf ist ein Wrapper. Bei Texturen werden GL-Größenlimits sowie GL-Uploadfehler gemeldet, beim Mesh getrennt fehlerhafte Index-/Vertex-/Face-Daten, belegte 256 Mesh-Slots und sonstige Driver-Fehler. Application-Logs nennen nun `gpu_failure` (und bei Texturen `gl_error`), invalid/unsupported nutzt längeren invalid-Cooldown, transient/kapazitätsbedingt kurzzeitige Retries. Eine verlässliche OpenGL-Mesh-Fehlernummer wird im Renderer bei GL-Fehlern noch nicht als separates numerisches Feld exportiert und ist als Folgeschritt dokumentiert.

**Block C – reproduzierbare GPU-Upload-Lastprofile mit großen Assets:** Neuer CTest `renderer_gpu_profiles_tests.cpp` verwendet unsichtbaren ECHTEN OpenGL-3.3-Context und prüft 40 × 64², 20 × 256² und 6 × 1024² RGBA-Texturen sowie Mesh-Vertexgrids 16/32/64 (8/8/4 Instanzen, bis zu 8192 Dreiecke je Grid). Für jede Stufe werden GPU-Uploads, Renderframe, sichtbare Objekte und GL-Fehler geprüft; CPU-Upload-Zeiten (nicht GPU-Fertigstellungszeit) pro Gruppe protokolliert. Beispiel Linux RTX 4070 SUPER am 11.10.2026: Texture 283/369/4604 µs und Mesh 17/61/157 µs je Upload. Diese Zahlen gelten nur für synthetische Assets/diesen Testlauf, nicht für typische Serverdownloads oder FPS.

**Block D – Rollback-/Überbelegungs-Regressionen:** `renderer_gpu_pressure_tests.cpp` prüft weiterhin tatsächliche Belegung 512 Texturen/256 Meshes, jetzt zusätzlich Transaktions-Rollback bei ungültigem Input, fehlender Slotkapazität, unerlaubter Selbsteviction sowie übergroßen Texturen; erfolgreiches Commit entfernt exakt den vorgesehenen GPU-Victim und macht die neue ID resident. Vorhandene Mesh-Überbelegungs- und ungültige-Index-Tests prüfen die passende strukturierte Fehlerklasse. Insgesamt nach Implementierung **54/54 CTests bestanden**, inklusive drei echter OpenGL-Context-Tests; nach finaler Versionspflege erneut Full-Rebuild und GUI-Smoke.

**Wichtige technische Grenze:** Der Transaktionspfad hält den alten GPU-Textur-Slot und die neue Kandidatenressource kurzfristig gleichzeitig im VRAM. Bei sehr knappem physischem VRAM kann die sichere Transaktion scheitern und die alten Texturen behalten – ergebnisabhängig beabsichtigtes Schutzverhalten, aber später mit zusätzlicher Budgetreserve/Backpressure zu optimieren. Netzwerk-CAPS, echte Meshasset-/Avatar-/Bewegungs-/Teleportfehler wurden wie vereinbart nicht als bereits behoben deklariert. Der OpenGL-Integrationstest ist synthetisch, nicht authentifiziert in einer NexVerse-Region.

**Nächste vier Blöcke:** Strukturiertes Mesh-GL-Fehlerdetail, VRAM-Transientbudget/Reserve bei Rollback-Uploads, Instrumentierung echter Weltobjekt-Draws und Aufbau eines reproduzierbaren In-World-Akzeptanztests. `projectplan.md` bleibt verbindlich, und die ausdrücklich aufgeschobenen Avatar/Bewegungs/EventQueue-Bugfixes bleiben späteren Blöcken vorbehalten.

Start: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Profile: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-profiles-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl70-live.log`; nur `develop-current`.

Neuer Chat: zuerst GitHub-Branch `develop-current` prüfen (`marlin198323-glitch/nexverse-viewer`), lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Dann `CHATUEBERGABE.md`, verbindliches `projectplan.md`, `info.md` und technische Docs lesen. 3–4 größere, zusammenhängende Entwicklungsblöcke, vollständiger C++23-Debug-Build, ALLE CTests inklusive NVIDIA OpenGL 3.3 via `DISPLAY=:0`, GUI-Smoke, Versionspflege, vollständiger Git-Commit/Push und SHA-Verification. Divergierendes `develop` niemals ohne Benutzerweisung mergen; authentifizierte Live-NexVerse-In-World-Verifikation steht weiterhin aus.


## 0.2.0-alpha.71 – Chatübergabe und Folgeblöcke · 11.10.2026

**Block A – konkrete GetMesh-OpenGL-Fehlernummern:** Der vorhandene GL-Uploadhelfer `upload_mesh` kann nun über einen optionalen `GLenum*` die tatsächlich gemeldete OpenGL-Fehlernummer an `Renderer::upload_mesh_asset` weiterreichen. `GpuUploadResult.gl_error` ist bei GL-Fehlern gesetzt; `GL_OUT_OF_MEMORY` wird als `allocation`, andere GL-Fehler als `driver_error` klassifiziert. Der Viewer protokolliert bei gescheiterten Mesh-Uploads `gpu_failure` und `gl_error` zusammen mit Retry-Delay. Kein GL-Fehler/fehlgeschlagene Handle-Anlage kann weiterhin `driver_error` mit `gl_error=0` ergeben; diese semantische Grenze ist beabsichtigt.

**Block B – transaktionale GPU-Textur-Staging-Reserve:** Neue reine C++-Hilfsfunktion `assets::gpu_staging_budget(texture_resident_bytes,mesh_resident_bytes,incoming_bytes,shared_budget_bytes)`. Sie prüft vor einer Kandidaten-Upload-Transaktion den kurzzeitigen geschätzten Höchstbedarf, während Altressourcen für Rollback noch auf der GPU liegen. Sicherheitsabstand: `min(16 MiB, shared_budget / 32)`; bei 384 MiB geteiltem Assetbudget also 12 MiB. Überlauf und Nullbudget werden sicher behandelt. Ein unzureichendes Staging-Budget führt zur nichtdestruktiven Deferral statt zum sofortigen Entfernen alter Texturen; zusätzliches Diagnosefeld `texture_gpu_staging_deferrals` und Warnlog mit numerischen Bytes. Die Angaben sind Software-Schätzungen, **keine** Garantie real freien VRAMs einschließlich Terrain/UI/Driver-Overhead.

**Block C – echte Weltobjekt-Renderzähler:** `RenderDiagnostics` liefert jetzt zusätzlich `scene_draw_calls`, `scene_mesh_draw_calls`, `scene_prim_draw_calls`. Zähler werden je Frame zurückgesetzt und ausschließlich beim tatsächlich ausgeführten glDrawElements aus der Weltobjekt-/Face-Schleife fortgeschrieben; Sky, Terrain, Wasser, Avatar-Proxies, UI zählen nicht dazu. Mesh-Zähler erfordern ein tatsächlich vorhandenes GPU-Mesh-Handle für die Mesh-ID, sonst zählt der tatsächlich gerenderte Fallback als Prim. `InWorldAssetSnapshot` enthält das letzte beendete Frame als `previous_frame_*`, wodurch In-World-Protokolle zwischen Asset-Residency und wirklichen Mesh-Draws unterscheiden. Synthetische Linux-OpenGL-Tests bestätigen Mesh UND Prim sowie die Summe aller Szene-Draws.

**Block D – reproduzierbarer In-World-Akzeptanzprüfer:** `tools/ogl_inworld_acceptance.py` wertet sanitizte Berichte des bestehenden `ogl_asset_log_report.py` aus; erforderlich sind mindestens 3 **aufeinanderfolgende Snapshots derselben Regionsepoche**, aktive Region, nichtleere gewünschte Textur-/Meshsets, mindestens 75 % Coverage beider Familien und ein tatsächlich ausgeführter Scene- sowie GPU-Mesh-Draw im neuesten Snapshot. Nur dann liefert die CLI `evidence_pass`/Exitcode 0; ansonsten `not_verified`/Exitcode 2. JSON liefert pro Kriterium booleschen Prüfstatus; keine UUID/CAPS-URL wird ausgegeben. Zwei zusätzliche CTests für GPU-Reserve und Akzeptanzlogik, erweitert um Regionsepochenwechsel, ergänzen die Renderer-/Snapshot-Regressionen. C++23-Debug und 56/56 CTests wurden nach Implementierung erfolgreich geprüft.

**Lokale Ist-Daten / korrekte Abgrenzung:** Die autorisierte Datei `$HOME/.local/state/opengenesislink/viewer.log` enthielt am 11.10.2026 weiterhin 0 valide `InWorldAssetSnapshot`-Zeilen; der neue Akzeptanzprüfer lieferte folgerichtig `not_verified` mit Exitcode 2. Die Datei verzeichnet historische 230 Mesh- und 56 Texture-Fehler (42 mit 404-Muster, eine mit 5xx-Muster); daraus kann keine aktuelle in-world Rendering-Freigabe abgeleitet werden. Auch `evidence_pass` allein würde Avatarsteuerung, Teleport und EventQueue **nicht** bestätigen. Diese Problemfelder bleiben wie vereinbart in einer späteren Bugfixphase.

**Nächste zusammenhängende Releaseblöcke (alpha.72):** Verbesserte In-World-Snapshot-Serienvalidierung mit Zeitstempeln, konkreter Renderer-Frame-Mesh-Face-Abnahme, GPU-Staging-Deferral-Lebenszyklus/Recovery sowie weiteres Mesh- und Texture-Streaming-Feedback. `projectplan.md` bleibt verbindlich.

CLI: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`; CMake: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Branch ausschließlich `develop-current`.

Wiedereinstieg: Repo `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`, lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, `CHATUEBERGABE.md`, `projectplan.md`, `info.md` und relevante Renderer/Asset-Docs lesen. Größere 3–4 Blöcke pro Release, C++23-Release-Preset Debug bauen, alle CTests unter DISPLAY=:0, GUI Smoke, VERSION/Docs, Commit+Push, lokale/Tracking/GitHub-SHA vergleichen. Keinen automatischen Merge mit dem divergenten Branch `develop` durchführen. Der In-World-Akzeptanzprüfer meldete mit den bisher vorhandenen echten Logs **not_verified**; das ist ausdrücklich kein Softwarefehler und kein bestandener Live-Test.


## 0.2.0-alpha.72 – Fortsetzung und Übergabe · 11.10.2026

**Block A – belastbare Snapshotserien statt altem Log-Zufall:** `AssetStreamingSnapshot` erzeugt bei `simulation_ready` ergänzend `session_started_unix_ms` (Prozesskennung als Startzeit), `emitted_unix_ms` und monoton steigende `sequence`; Schema `ogl.asset.streaming.v1` bleibt additive-only. Der Parser `tools/ogl_asset_log_report.py` zählt als neueste zusammenhängende Serie nur Snapshots derselben Session UND Regionsepoche mit lückenloser Sequenz und realistischem zeitlichen Abstand 15–90 s. Der CLI-Akzeptanzprüfer `tools/ogl_inworld_acceptance.py` nutzt `ogl.inworld.acceptance.v2` und prüft zusätzlich den Echtzeitstatus: letzter Snapshot maximal 180 s alt (10 s Uhr-Toleranz), 3 zusammenhängende Snapshots, nichtleere Desired-Textur- und Mesh-Sets, jeweils mindestens 75 % Residency und tatsächlich ausgeführte Scene- bzw. Mesh-Draws. `--max-age-seconds`, `--min-snapshots`, `--min-coverage` sind konfigurierbar. Ohne die neuen Zeitfelder oder bei historischen Logs ist das Ergebnis absichtlich `not_verified` (Exit 2), kein erfundener Liveerfolg.

**Block B – realer GPU-Mesh-Face-Nachweis:** OpenGL-RenderDiagnostics unterscheidet nun die erfassten `scene_mesh_draw_calls` (wirklich hochgeladenes Mesh) und `scene_missing_mesh_draw_calls`: Letztere zählen echte Prim-Fallback-Draws für Objekte, die eine Mesh-UUID anfordern, aber keine passende GPU-Meshressource besitzen. Ein erweiteter echter GLFW/OpenGL-3.3-Test lädt 2 getrennte Face-Indexbereiche, prüft exakt 2 GPU-Mesh-Drawcalls plus Prim-/Missing-Mesh-Fallback sowie den Frame-Reset nach dem Entfernen der Meshressource. Das In-World-Snapshotfeld `previous_frame_scene_missing_mesh_draw_calls` und der sanitized Log-Report `peak_missing_mesh_fallback_draws` liefern eine konkrete Anzeige der bisher fehlenden Meshobjekte, aber noch keine Fehlerbehebung des OpenSim-Meshdownloads.

**Block C – GPU-Staging-Deferrals werden wieder freigegeben:** `GpuStagingRecovery` speichert zurückgestellte Textur-UUIDs mit dem zugehörigen geschätzten Texture-/Mesh-Byteverbrauch und kontrollierter Cooldownzeit. Ein erneuter Versuch wird ermöglicht, sobald eine der beiden Assetfamilien weniger GPU-Speicher benötigt oder der Retry-Zeitpunkt erreicht ist; wiederholte Deferrals verwenden je UUID steigende 2/4/8/16/32/64-s-Cooldowns, ohne beim Freigeben der Retry-Anfrage die Historie zu verlieren. Erfolgreiche Uploads löschen die Historiereihe; bei Regionswechsel Reset; bei geänderter Desired-Residency verschwundene Assets werden bereinigt. Laufende Worker werden nicht zwangsweise unterbrochen. `texture_gpu_staging_waiting` / `texture_gpu_staging_released` und die bereits vorhandenen Deferrals gehen in In-World-JSON/Streaminglogs und den sicheren Report ein.

**Block D – Regressionen/Streaming-Feedback:** Ein neuer C++-CTest prüft Memory-Drop-Retry, Cooldown-Aufstieg, Recovery, Resolution, Prune und Region-Reset. Der Python-Akzeptanz-CTest prüft fehlende, veraltete und synthetische valide Serien sowie Sessionwechsel, Epochensprünge, Sequenzlücken und unrealistische Abstände; die Renderer-Integration prüft die Multi-Face-Geometrie, tatsächliche GL-Zeichnungen und fehlende Mesh-GPU-Residency. Erwartet sind 57/57 CTests inkl. dreier echter GL-Context-Tests. Die numerische Log-Whitelist wird um die neuen Snapshotfelder erweitert, ohne Asset-UUIDs/CAPS-URLs in Ausgaben aufzunehmen.

**Lokaler Status am 11.10.2026:** Die autorisierte `$HOME/.local/state/opengenesislink/viewer.log` enthält noch 0 neue `InWorldAssetSnapshot`-Zeilen. Der überarbeitete CLI-Prüfer liefert `not_verified`/Exitcode 2. Das ist technisch korrekt und darf nicht als abgeschlossene Live-NexVerse-Regions-, Avatar-, Mesh- oder Bewegungsabnahme dargestellt werden. Bekannte EventQueue-/Teleport-/Avatarprobleme bleiben wie gewünscht auf dem späteren Bugfix-Pfad. Außerdem sind GPU-Bytegrenzen nur Software-Schätzungen und keine Treiber-VRAM-Garantie.

**Folgerelease Alpha.73 / 3–4 Blöcke:** GPU-Streaming-Retry-Integration und Budget-Pressure-Regressionen mit realem GL-Kontext, weitere Missing-Mesh-/Face-Fallback-Statistik, wiederholbare Langzeit-OpenGL-Szenentests sowie echte In-World-Log-Akzeptanz sobald Regionenzugang/Assets bereit sind. Maßgeblich ist weiter `projectplan.md`.

CLI auf dem autorisierten Linux-PC: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"` (Exitcode 0 nur für neueste gültige Live-Daten). Branch ausschließlich `develop-current`.

GitHub: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; lokal `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Für neuen Chat zuerst `git status`, diese Übergabe, `projectplan.md` und `info.md` sowie Asset-/Mesh-/Texture-/Log-Docs. Dann 3–4 größere Entwicklungsblöcke, `cmake --preset debug`, `cmake --build --preset debug -j2`, `env DISPLAY=:0 ctest --preset debug --output-on-failure`, `timeout 7s env DISPLAY=:0 ./build/debug/nexverse-viewer` GUI-Smoke, alle Docs/VERSION, Commit & Push nach GitHub verifizieren. Den divergenten Branch `develop` nicht automatisch mergen. `not_verified` des In-World-Prüfers ist bei fehlenden Live-Daten erwartungsgemäß.


## 0.2.0-alpha.73 – Chatübergabe und nächste 4 Blöcke · 11.10.2026

**Block A – vollständig belegte GetTexture-GPU-Slots korrekt behandeln:** Die Demand-Anfrageprüfung vor HTTP blockierte bisher jede neue Asset-UUID, sobald bereits 512 Texturen GPU-resident waren, obwohl die bestehende `plan_texture_slot_admission` danach bereits eine niedriger priorisierte Textur verdrängen kann. In `src/app/application.cpp` wird bei voller Tabelle jetzt eine rangbewusste Preflight-Slotzulassung (gewünschte und zur Räumung markierte Resident-UUIDs) durchgeführt; nur unmögliche Transfers werden deferred. Nach dem Download prüfen Bytebudget, Staging-Reserve, Slotpriorität und transaktionaler Renderer-Commit weiterhin die tatsächliche Machbarkeit. Unit-Test erweitert für 512 belegte Slots und die Nichtverdrängung höher priorisierter Objekte.

**Block B – tatsächlicher OpenGL-Soak-Test mit GPU-Pressure und Recovery:** `tests/renderer_gpu_soak_tests.cpp` erstellt einen versteckten echten OpenGL-3.3-Kontext mit 24 Weltobjekten, davon 20 gültige GPU-Meshes und vier bewusst nicht hochgeladene Mesh-UUIDs, plus acht echte GPU-Texturen. In 160 gerenderten Frames ersetzt es wiederholt Texturen, ersetzt GPU-Meshes und kontrolliert pro Frame GL-Fehlerfreiheit, korrekte Drawcall-Summe, vorhandene GPU-Mesh-Draws und stabile Zahl fehlender Meshobjekte. 40 gezielte *simulierte* Software-Staging-Druckereignisse werden mit `gpu_staging_budget`/`GpuStagingRecovery` freigegeben; 160 Transaktionen tatsächlich im Renderer ausgeführt. Beispiel auf der Linux RTX 4070 SUPER: 43 ms CPU-Laufzeit für diese kleine synthetische Szene, kein FPS- oder realistischer Simulatorperformance-Benchmark. Ohne GL-Kontext überspringt sich der CTest (Exit 77).

**Block C – präzise Mesh-Fallback-Metriken:** Der Renderer zählt zusätzlich zu bisherigen pro Frame ausgeführten Missing-Mesh-Fallback-Drawcalls jetzt **einzigartige betroffene Szenenobjekte** und die wirklich als Ersatz gezeichneten **Dreiecke**. Da eine Region pro Objekt mehrere Materialfaces und transparente/opaque Renderpasses besitzt, vermeidet ein Frame-lokales Set eine Überzählung der Objekte. Die numerischen `InWorldAssetSnapshot`-Keys `previous_frame_scene_missing_mesh_objects` und `previous_frame_scene_missing_mesh_triangles` sowie der sanitizte Logbericht zeigen das Ergebnis. Reale OpenGL-Multi-Face- und Mesh-Entfernungs-Tests prüfen diese Zähler; fehlende OpenSim-Meshdownloads werden damit diagnostiziert, nicht behauptet repariert.

**Block D – verschärfte In-World-Abnahme und Streaming-Regressionen:** `tools/ogl_inworld_acceptance.py` verlangt nun standardmäßig null zuletzt tatsächlich gerenderte Missing-Mesh-Fallback-Draws, zusätzlich zu frischer Session-/Epoch-/Zeitserie, nichtleerer Region, >=75 % Texture- und Mesh-Residency und tatsächlichen GPU-Mesh-Draws. Nur für bewusst teilweise bereite Szenen kann der Aufrufer `--max-missing-mesh-draws N` setzen. Fehlende Diagnosefelder gelten nicht als null und können keinen Pass erzeugen. Python-Cases prüfen strikten Status und Override; C++-Regressionen GPU-Full-Residency/Byte-Slot-Vorabzulassung/Snapshot werden mit GL-Tests ausgeführt. Insgesamt **58/58 CTests bestanden** (erster vollständiger Entwicklungsbuild).

**Abnahmegrenze:** Auf der autorisierten Linux-Workstation ist die frühere `viewer.log` weiterhin ohne aktuelle In-World-Snapshotserie (`not_verified`), insbesondere ohne Prüfung echter NexVerse-Mesh-, Avatar- und Simulatorinteraktionen. Synthetische Tests ersetzen keinen erfolgreichen Simulatorbeitritt. Seit Alpha.70 bewusst später zu beheben: fehlender Avatar, WASD/Bewegung, Teleport, CAPS EventQueue, teilweise fehlende Meshobjekte. Die aktuelle Arbeit führt den maßgeblichen `projectplan.md` ohne ungefragte Fehlerbehebungsphase fort.

**Nächste 3–4 Releaseblöcke Alpha.74:** Zeitliche Priorität und aktive GPU-Budget-Deferral-Diagnose bei geteiltem Tex-/Mesh-Speicher, Wiederverwendung von Assets über Regions-/LOD-Wechsel, erweitertes realistisches GL-Szenen-/Materialtestprofil und automatische Prüfung wirklich neuer Regionlogs sobald echte Server- und Loginbedingungen erfüllt sind.

Lokaler Debug/Soak: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-soak-tests`. In-World-Prüfer: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repo `marlin198323-glitch/nexverse-viewer`, einzig maßgeblicher Branch `develop-current`.

Wiedereinstieg: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; Workstation-Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, `projectplan.md`, `info.md`, diese Übergabe, technische Docs lesen. Wie gewohnt drei bis vier große Entwicklungsblöcke, vollständiger Debug-Build, alle CTests mit `DISPLAY=:0` inklusive echten GLFW/GL3.3-Soak, GUI Smoke, `VERSION` und Docs, GitHub Commit+Push und SHA-Verifikation. Divergierenden Branch `develop` nicht eigenständig mergen. Live-OpenSim bleibt `not_verified` bis echte In-World-Daten vorliegen.


## 0.2.0-alpha.74 – Handoff und nächster Entwicklungsplan · 11.10.2026

**Block A – geteiltes Tex-/Mesh-GPU-Budget und Mesh-Retry-Recovery:** Mesh-Anfragen, deren dekodierte GPU-Geometrie das kombinierte Budget überschreitet, werden nicht mehr unbegrenzt allein durch `mesh_gpu_budget_deferred_ids` blockiert. Neuer `GpuStagingRecovery` auch für Meshes, mit Key `UUID.LOD`, progressivem 2/4/8/16/32/64-s-Cooldown und vorgezogener Re-Evaluierung nach sinkendem Texture- oder Mesh-Residency-Byteverbrauch. Bei Regionswechsel Reset, bei geänderter Scene-Desired-Liste Prune, nach erfolgreichem Mesh-Upload Resolution. Dieselbe Recovery wird nun auch bei nicht passendem Texture-Byte-/Slotplan verwendet, nicht nur bei Staging-Reserveverletzung. Numeric Snapshotfelder `mesh_gpu_budget_waiting`, `mesh_gpu_budget_released`, plus bestehende Textur-Deferrals. Risiken der nur geschätzten (Terrain/UI/Driver nicht eingeschlossenen) GPU-Bytegrenzen bleiben.

**Block B – tatsächliche wiederverwendete CPU-Assets, keine falsche GPU-Hot-Reuse-Behauptung:** Die bisherigen Mesh-/Texture-RAM- und Disk-Caches bleiben regionsübergreifend im Speicher bzw. auf Festplatte bestehen; GPU-Handles werden aus Sicherheitsgründen weiterhin bei Regionswechsel geräumt. Der neue begrenzte `AssetReuseHistory` (max. 4096 UUIDs/Familie) beobachtet ausschließlich erfolgreiche erneute GPU-Uploads und zählt sie als `cached_region_reuse` oder `cached_lod_swap` NUR, wenn der Worker ausdrücklich einen RAM-/Disk-Cache-Hit meldete. Texture-Mipqualität und Mesh-LOD werden pro UUID erfasst, unabhängig vom Cache-Tier. Snapshotfelder für `texture_cpu_cached_region_reuse`, `texture_cpu_cached_lod_reuse`, `mesh_cpu_cached_region_reuse`, `mesh_cpu_cached_lod_reuse`; gleichartige Streaming-Logs. Keine Gewähr, dass dieselbe UUID bei Regionwechsel auf einem fremden Server existiert oder identische GPU-Handles weiterverwendet werden.

**Block C – langlebiger realer OpenGL-Material-/Szenentest:** Der bereits echte GLFW/OpenGL-3.3-Soak mit 160 Frames, 160 transaktionalen Texture-Replacements, 40 simulierten Staging-Deferrals, 20 GPU-Meshes plus vier absichtlich fehlenden Meshobjekten testet jetzt abwechselnd opake und teiltransparente Texturen, per-Face-Alpha-Tints, Fullbright, Shininess sowie Repeat-/Offset-Materialparameter. Szenenmaterial wird in jedem Frame aktualisiert. Echte OpenGL-Frames, `scene_mesh_draw_calls`, `scene_prim_draw_calls`, `scene_missing_mesh_objects`, GL-Fehlerfreiheit und GPU-Assetreplacements werden kontrolliert. Das ist eine synthetische GPU-/Legacy-Material-Regressionsprüfung, keine vollständige PBR- oder NexVerse-InWorld-Visualabnahme.

**Block D – End-to-End GPU-Pressure/Queue-Diagnostik und reproduzierbare Tests:** Neuer CTest `asset_gpu_pressure_pipeline_tests.cpp` simuliert gemeinsam beanspruchtes 384-MiB-Softwarebudget: eine blockierte Mesh-Anfrage wird unmittelbar nach 12-MiB-Texturfreigabe wieder möglich, Staging schützt alte GPU-Assets, Textur-Cooldown läuft ab, Regionswechsel leert Deferrals, RAM-/Disk-Hit bleibt auf Cacheebene für folgende Region nachvollziehbar. Neuer Unit-Test `asset_reuse_history_tests.cpp` kontrolliert bounded History, Regionswechsel, Qualitätswechsel und Nicht-Cache-Fälle. Der Log-Analyser nimmt neue numerische Snapshotfelder in die Whitelist auf und gibt Spitzenwerte der mesh-budget-Warteschlange frei, ohne UUIDs/CAPS-Adressen/Secrets auszugeben; Python-Test prüft dies. Damit **60/60 CTests** im ersten vollständigen Debug-Testlauf.

**Lokale Live-Abgrenzung 11.10.2026:** Der unverändert mit der autorisierten Workstation vorliegende `$HOME/.local/state/opengenesislink/viewer.log` enthält noch keine gültigen `InWorldAssetSnapshot`-Serien; die CLI `tools/ogl_inworld_acceptance.py` ergibt `not_verified`/Exitcode 2 (0 geeignete Snapshots). Keine verifizierte Aussage zur vollständigen NexVerse-Welt, Avatar-Body, WASD/Bewegung, Teleport, fehlenden realen Meshobjekten oder EventQueue. Diese bereits bekannten Themen verbleiben auf der ausdrücklich später geplanten Fehlerbehebungsphase.

**Nächster geplanter Stand alpha.75:** Noch gezielteres CPU/GPU-Cache-Residency-Management mit eindeutiger Kennzeichnung von HTTP-/RAM-/Disk-Treffern und anhaltender Budgetlast, stärkere GL-Legacy-Material-/Multi-Face-Szenen, prüfbare Wiederverbindung nach Regionenwechsel und aussagekräftige Auswertung echter In-World-Testlogs, sobald Authentifizierung und Regionsdaten verfügbar sind. `projectplan.md` bleibt die verbindliche Roadmap.

Repo `marlin198323-glitch/nexverse-viewer`, aktiver Branch `develop-current`, lokale Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Standard: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; anschließend GUI-Smoke und GitHub-SHA-Vergleich.

Für den nächsten Chat: ZUERST GitHub-Branch `develop-current`, `CHATUEBERGABE.md`, verbindliches `projectplan.md`, `info.md`, die Renderer-/Asset-Dokumentation prüfen. Entwicklung in vier großen Blöcken pro Release; alle Quellen, Tests, Version und Docs gemeinsam committen und pushen. CMake-Debug-Build, alle CTests bei `DISPLAY=:0`, Linux-GUI-Welcome-Smoke; SHA von GitHub/Tracking/Local gleich. Branch `develop` ist divergent und nicht ungefragt mergen. Live-OpenSim `not_verified` darf NICHT als bestandener Welt-Test gelten.


## 0.2.0-alpha.75 – Übergabe und nächste Entwicklungsgruppe · 11.10.2026

**Block A – deterministic bounded CPU-Asset-Reuse (RAM vs Disk):** `include/nexverse/assets/asset_reuse_history.hpp` was changed from arbitrary `unordered_map.begin()` eviction to real LRU with a `std::list` and O(1)-average lookup/touch/evict. The 4096 UUID/family limit is maintained. New `AssetReuseHistory::Tier {network,memory,disk}` distinguishes actual reported successful memory/disk-backed GPU reuploads at a later RegionEpoch or different texture mip / mesh LOD; counters `region_ram_hits`, `region_disk_hits`, `lod_ram_hits`, `lod_disk_hits` and `evictions`, plus prior combined hit counts. Application maps the texture loader's explicit `RAM texture cache hit` / `Disk texture cache hit` and mesh `MeshCacheTier` into this tier. New numeric `InWorldAssetSnapshot` and safe log parser fields distinguish the cache provenance (including history LRU evictions). This is CPU-/disk-cache reuse; GPU textures and VBOs remain invalidated on region changes.

**Block B – targeted GPU Deferred/Retry lifecycle:** `GpuStagingRecovery::synchronize(blocked, desired)` prunes obsolete blocked UUIDs in lockstep with active desired sets, **preserving cooldown for still-required UUIDs** and unblocking orphans that have no recovery deadline; a corresponding regression test verifies this. Texture plan changes no longer indiscriminately clear all deferred IDs; specific changed mip-levels release their own state, and full-slot preflight rejection explicitly adds a timed recovery record. Mesh desired-Lod set changes use synchronized `UUID.LOD` deferred keys rather than a blanket clear. Existing actual GPU-memory-eviction code can still trigger early retry, and its software VRAM estimates still exclude other driver allocations. No unauthenticated network or EventQueue behavior is asserted fixed.

**Block C – real native GL3.3 three-face region transition test:** New `tests/renderer_region_transition_tests.cpp` creates a hidden actual GLFW/OpenGL 3.3 context. It uploads three independently materialized faces, including opaque texture, blended tint and alpha-textured face. It verifies three GPU mesh Face-Draws and no missing-Mesh fallback. It then simulates a region change by clearing all GPU meshes and textures, validates correct Prim fallback without any stale GPU mesh, and reuploads the SAME immutable UUID with refreshed texture bytes; a new rendered frame must again show 3 GPU Mesh Face-Draws and 0 missing mesh objects. All stages check `glGetError()`; skip code 77 if no GL context. The test is synthetic, not a real authenticated OpenSim region reconnect.

**Block D – series-wide latest-session In-World acceptance:** The log analyzer now computes only from the most recent contiguous valid `(session_started_unix_ms, region_epoch, sequence, 15–90 s interval)` series `latest_region_min_texture_coverage_pct`, `latest_region_min_mesh_coverage_pct`, `latest_region_min_mesh_draw_calls`, `latest_region_max_missing_mesh_fallback_draws` and `latest_region_max_mesh_budget_waiting`. `tools/ogl_inworld_acceptance.py` now rejects an apparently healthy latest frame if the current series contains an earlier low-coverage frame, absent required mesh-draw evidence, or missing-mesh fallback. Missing fields result in `not_verified`, not assumed zeros. New dedicated `tests/test_ogl_inworld_transition_report.py` tests healthy series, degraded middle frame, missing mesh in earlier frame, missing telemetry field and session/epoch boundaries. End-to-end C++ / OpenGL and Python regression count **62 CTests**, all passed before final version rebuild.

**Proven live status:** The authorized workstation's `$HOME/.local/state/opengenesislink/viewer.log` still contains **zero** valid current `InWorldAssetSnapshot` samples; acceptance returns `not_verified` with exit code 2. It is not possible to claim a real NexVerse Region/Avatar/Movement/Teleport/EventQueue acceptance from the GL tests or these logs. Those user-deferred issues remain in the project roadmap for later troubleshooting.

**Next large release alpha.76 (3–4 blocks):** cache-pressure eviction and persistent asset-tier observations, region-switch cancellation and stale HTTP event contract tests (without assuming live simulator connectivity), wider material/texture transparency scenario coverage, explicit failure diagnosis of missing Real Mesh UUIDs once real region snapshots exist. Keep the planned staged sequence in `projectplan.md`; don't silently move debugging of WASD, avatar, Teleport or CAPS queue ahead.

Build: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Live check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Private GitHub repo `marlin198323-glitch/nexverse-viewer`, branch ONLY `develop-current`.

For next chat: read authoritative `projectplan.md`, `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md`, `docs/LOG_ANALYSIS.md` from branch `develop-current`. Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Maintain 3–4 substantial release blocks, all source/tests/markdown/VERSION together, full debug build, `DISPLAY=:0` CTests/GL Smoke, GitHub push and exact SHA verification. Never automatically merge the divergent `develop` branch. All authentic in-world acceptance remains `not_verified` without new genuine simulator data.


## 0.2.0-alpha.76 – Handoff und nächster Entwicklungsstand · 11.10.2026

**Block A – echte CPU-RAM-Cache-Belastungstransparenz:** Die beiden bestehenden LRU-Caches `TextureMemoryCache` und `MeshMemoryCache` erfassen nun direkt `hits`, `misses`, `evictions`, `bytes` und `entries`. Der Zugriff auf die Statistiken ist mutexgeschützt, die Evictionzählung erfolgt ausschließlich beim wirklichen Verdrängen eines älteren Eintrags. Ein neuer gemeinsamer Header `asset_memory_cache_stats.hpp` hält das numerische Messformat. Der Viewer überträgt je Familie Treffer, Fehlzugriffe und Verdrängungen in `InWorldAssetSnapshot` (`texture_ram_hits`, `texture_ram_misses`, `texture_ram_evictions`, `mesh_ram_hits`, `mesh_ram_misses`, `mesh_ram_evictions`). Der sanitizte Python-Logbericht berücksichtigt die Felder und berechnet `peak_texture_ram_evictions` / `peak_mesh_ram_evictions`. Vorhandene CPU-RAM-Cachetests prüfen die neuen Zähler und Speichergrenzen.

**Block B – kontrolliert verspätete HTTP-Antwort beim Regionswechsel:** Neuer nativer Test `tests/region_http_epoch_contract_tests.cpp`: Eine asynchrone, simulierte GetMesh-HTTP-Antwort wird nach Beginn des Requests in Region A gezielt zurückgehalten. Nach dem Übergang zu B wird die alte Antwort freigegeben und muss an der vorhandenen Epochensperre verworfen werden, ohne die Mesh-Disk-/RAM-Caches zu befüllen. Anschließend wird ein neuer Request im aktiven B akzeptiert. Auch der Rückwechsel B → A erhält eine neue Epoche und erlaubt einen authentifizierten Cachetreffer, ohne dass die alte Antwort wieder gültig wird. Dies prüft reale asynchrone Steuerlogik mit kontrolliertem Transportcallback und existierender `RegionMaterialScope`/`load_mesh_cached`-Integration, jedoch keinen echten Login/HTTP-Endpunkt.

**Block C – umfangreicherer nativer OpenGL-3.3-Material-/Multiface-Test:** `renderer_region_transition_tests.cpp` prüft weiterhin GPU-Freigabe und Wiederaufbau derselben Mesh-UUID nach einem Regionswechsel. Ergänzend rendert der echte versteckte GLFW/OpenGL-3.3-Kontext **48 Frames mit zwei Objekten und jeweils drei Mesh-Faces**. Face-Tint/Alpha, transparente Textur, Fullbright, Glow, UV-Rotation, UV-Repeat/Offset und Planar-Texturmodus wechseln. Pro Frame müssen sechs GPU-Mesh-Face-Draws, null Missing-Mesh-Objekte, konsistente Drawcall-Summen und GL-Fehlerfreiheit vorliegen. Diese synthetischen Renderer-Regressionen prüfen technische Pfade, nicht die visuelle Pixelgenauigkeit einer realen OpenSim-Region.

**Block D – kategorisierte Missing-Mesh-/GetTexture-Ausfälle:** Neues `AssetFailureTelemetry` mit numerischen Zählern `missing`, `transient`, `invalid`, `configuration`, ohne UUID/CAPS-Adresse. Der reale Applikationspfad zählt kategorisiert fehlgeschlagene Texture- und Mesh-Downloads bzw. GPU-Rejections (mit bestehenden AssetFailure-Regeln); bei erfolgreichen GPU-Uploads wird keine Fehlerzählung erhöht. `InWorldAssetSnapshot` ergänzt `mesh_fail_{missing,transient,invalid,configuration}` und `texture_fail_{missing,transient,invalid,configuration}`. Der vorhandene sichere Python-Analyzer nimmt die Felder auf und meldet `max_mesh_missing_events`, `max_mesh_transient_events`; Tests kontrollieren Kategorieabbildung, Snapshotformat und bestehende Cacheauswertung. Zusammen **64/64 CTests** im Entwicklungsbuild, darunter native GLFW/OpenGL-Tests und der neu hinzugefügte zeitversetzte Regions-HTTP-Test.

**Bewertungsgrenzen / Ist-Status:** Ein Simulator-HTTP-404 / 410 wird getrennt als `missing` dokumentiert, Retry-/Serverfehler als `transient` und Fehlkonfiguration separat. Diese Kategorien erlauben später eine gezielte Ursachenanalyse der realen fehlenden Meshobjekte, beheben aber noch nicht den OpenSim-Netzwerkdownload oder die CAPS-EventQueue. Der autorisierte echte lokale `viewer.log` enthält am 11.10.2026 weiterhin 0 geeignete `InWorldAssetSnapshot`-Zeilen; In-World-Prüfer meldet `not_verified`/Exitcode 2. Ohne authentifizierte Region kann weder Avatar, Bewegung, Teleport noch die Bildqualität vollständig abgenommen werden. Diese bekannten Probleme bleiben wie vereinbart für die spätere Bugfixphase vorgesehen.

**Nächste 3–4 koordinierte Blöcke für Alpha.77:** Auswertung von Cachemiss-Raten und Prioritätskontrolle unter längerer Streaminglast, zuverlässige stale-Response-Trennung auch für Texturdownloads/Prefetch in isolierten Regressionstests, Mesh-Face-/Material-Transparenz-Routing samt GPU-Speicherlast und Diagnose tatsächlicher OpenSim-Regiondaten sobald verfügbar. Der verbindliche `projectplan.md` bestimmt weiterhin die Reihenfolge.

Build/Test auf der autorisierten Workstation: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.

Wiedereinstieg: Branch `develop-current` des privaten Repos `marlin198323-glitch/nexverse-viewer`; Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Verbindlich sind `projectplan.md`, `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md` und `docs/LOG_ANALYSIS.md`. Bei `weiter`: 3–4 substanzielle zusammenhängende Blöcke implementieren; vollständiger C++23-CMake-Debug-Build, alle CTests mit `DISPLAY=:0`, realer Linux-Viewer-Smoke; VERSION/Docs aktualisieren; Commit+Push auf `develop-current`, lokalen/Tracking/GitHub-SHA vergleichen und clean worktree sicherstellen. Divergentes altes `develop` nicht automatisch mergen. Der erfolgreiche synthetische Region-HTTP-/OpenGL-Test ist keine In-World-Live-Abnahme; echte Region aktuell `not_verified`.


## 0.2.0-alpha.77 – Übergabe und nächste Entwicklungsgruppe · 11.10.2026

**Block A – 30-s-Intervallanalyse echter RAM-LRU-Belastung:** Neues `AssetCachePressureWindow` berechnet nicht nur kumulative, sondern delta-basierte Hitquote und Anzahl echter Evictions pro Asset-RAM-Cache-Sampling-Intervall. `under_pressure` wird nur bei mindestens 8 neuen Cache-Misses, mindestens 2 Verdrängungen und einer Trefferrate unter 50 % aktiviert. Der Renderer beobachtet Textur- und Mesh-RAM-Cache getrennt und veröffentlicht `texture_ram_hit_rate_pct`, `texture_ram_interval_evictions`, `texture_ram_pressure` und entsprechende `mesh_*`-Werte in `ogl.asset.streaming.v1`. Bei aktivem Texturcache-Druck setzt das freiwillige **Disk-only-Texture-Prefetching** aus, während GetTexture-Anforderungen benötigter sichtbarer Weltassets weiterhin starten dürfen. Regionswechsel resetten die Fenster. Cache-Pressure-CTest deckt Druck, Recovery, Nulländerung und Counter-Reset ab; die Schwellwerte sind konservativ und noch nicht live kalibriert.

**Block B – GetTexture-Worker mit überprüfbarer Regionsantwortsperre:** Die bisher nur in `src/app/application.cpp` kopierte RAM/Disk/HTTP-Mip-Laderoutine ist in `include/nexverse/assets/texture_cached_request.hpp` als `load_texture_cached_scoped` extrahiert; die App verwendet diese unmittelbar mit dem existierenden authentifizierten `TextureAssetLoader`, den CPU-/Disk-Caches, gewünschtem Mip und dem atomischen Regionsepochen-Accept-Callback. Vor HTTP und nach einer Antwort wird die Epoche kontrolliert; eine verspätete HTTP-Antwort wird **vor** der RAM-/Disk-Cache-Übernahme verworfen. Zusätzlich wird eine vom angefragten UUID abweichende erfolgreiche Antwort als `invalid` abgelehnt. Der neue native CTest `texture_http_epoch_contract_tests.cpp` hält eine simulierte HTTP-Antwort gezielt bis nach dem A→B→C-Wechsel zurück, prüft keine stale Cache-Befüllung, erfolgreiche Antwort der aktuellen Region, GPU-Mip-reduziertes Arbeitsergebnis bei weiterhin voller RAM-Assetauflösung, RAM- und Disk-Treffer. Das ist ein kontrollierter Transportvertragstest, keine externe Netzabnahme.

**Block C – echte OpenGL Opaque-vs-Alpha-Face-Telemetrie:** `RenderDiagnostics` zählt bei tatsächlich ausgeführten Szene-`glDrawElements` nun getrennt `scene_opaque_draw_calls` und `scene_alpha_draw_calls`, die sich genau auf `scene_draw_calls` summieren müssen. Beide Zahlen werden beim Frame-Beginn zurückgesetzt und als `previous_frame_scene_opaque_draw_calls` / `previous_frame_scene_alpha_draw_calls` in In-World-Health-Snapshots aufgenommen. Die echten GLFW/OpenGL-3.3-Tests `renderer_region_transition_tests.cpp` (2 Meshobjekte, je 3 Materialfaces, 48 wechselnde Materialframes) und `renderer_gpu_soak_tests.cpp` (160 Frames mit Texture-Updates und fehlenden Meshes) fordern nun tatsächliche Draws in BEIDEN Durchläufen und exakte Summengleichheit. GPU-/Materialfunktionalität im synthetischen GL-Kontext bewiesen, nicht Pixelidentität mit dem Live-Simulator.

**Block D – datensparsame Ursachen-/Missing-Asset-Diagnostik:** Neues `AssetMissingIdCounter` speichert maximal 4096 verschiedene intern beobachtete UUID-Strings pro Region/Familie und zählt nur `AssetFailure::missing`, nicht Transient-/GPU-Failures; bei vollem Set werden Overflow-Ereignisse separat gezählt. Regionswechsel setzt die ID-Zählung zurück. Die App zählt so `mesh_missing_unique`, `mesh_missing_unique_overflow`, `texture_missing_unique`, `texture_missing_unique_overflow` und schreibt nur deren numerische Werte in `ogl.asset.streaming.v1`, niemals die eigentlichen UUIDs/CAPS-Adressen. Der sanitizte Python-Analyzer berechnet auf der neuesten gültigen Region-Snapshot-Serie `latest_region_mesh_missing_events_delta`, `latest_region_texture_missing_events_delta`, `latest_region_peak_missing_mesh_unique`, `latest_region_peak_texture_ram_pressure`, `latest_region_peak_mesh_ram_pressure` und die minimalen Cache-Hitraten. `ogl_inworld_acceptance.py` enthält diese Befunde unter `observations`, verändert jedoch keine bisher gültigen Nachweiskriterien. Zusätzliche C++- und Python-Tests prüfen Bounded/Reset/Overflow, zeitliche Serienaggregation und keinen Secret-Leak. Insgesamt **67/67 CTests** im Entwicklungsbuild erfolgreich.

**Live-Status / Abgrenzung 11.10.2026:** Die echte lokale `$HOME/.local/state/opengenesislink/viewer.log` auf der autorisierten Workstation enthält weiterhin 0 gültige aktuelle `InWorldAssetSnapshot`-Zeilen. `tools/ogl_inworld_acceptance.py` liefert `not_verified`, Exitcode 2. Der Code kann ohne authentifizierten Regionsbeitritt nicht als visuelle Abnahme von Meshes, Avatar, Welt, WASD/Bewegung, Teleport oder CAPS-EventQueue gelten. Diese bereits bekannten Mängel bleiben wie vereinbart für die spätere Fehlerbehebungs- und In-World-Abnahmephase dokumentiert. Hinweis: Der begrenzte Missing-ID-Counter meldet bei Sättigung eine Untergrenze unterschiedlicher UUIDs plus Overflows, keine exakte unbeschränkte Kardinalität.

**Nächster geplanter Release alpha.78, 3–4 Blöcke:** Cache-Pressure-Werte gegen fortlaufende Simulator-Samples validieren; transparentes Texture-/Mesh-Fehlverhalten mit vollständigem Regionswechsel und Asset-Staging prüfen; zusätzliche GPU-Szenenfälle für unterschiedliche Alpha-Mask-/Blending-Kontrakte und eine aussagekräftige Diagnose der wirklich vom Server fehlenden Assets sobald Live-Daten vorliegen. Maßgeblich bleibt `projectplan.md`.

Debug-Kommandos: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, nur Branch `develop-current`.

Wiedereinstieg: zuerst den tatsächlichen Status aus `marlin198323-glitch/nexverse-viewer` Branch `develop-current` holen; Workstation-Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Verbindlich `projectplan.md`, `info.md`, diese `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/TEXTURES.md`, `docs/MESH.md`, `docs/LOG_ANALYSIS.md`. Je `weiter` drei bis vier größere zusammenhängende Blöcke implementieren, alle Tests mit OpenGL auf `DISPLAY=:0`, GUI-Smoke, Docs/VERSION, Commit/Push und vollständige lokale/Tracking/GitHub-SHA bestätigen. Divergenten alten Branch `develop` niemals ungefragt zusammenführen. `not_verified` im echten Simulatorlog ist weiterhin kein bestandener Live-Test.


## 0.2.0-alpha.78 – Chatübergabe und Folgeentwicklung

**Block A – RAM-Cache-Druck ohne Prefetch-Pendeln:** `AssetCachePressureWindow` benötigt nach aktivem Texture-/Mesh-Thrashing jetzt zwei aufeinanderfolgende unkritische 30-s-Auswertungsfenster, bevor `under_pressure` zurückgesetzt wird. Dadurch pausiert das rein optionale Texture-Disk-Prefetch auch nach einem zufällig ruhigen Einzelintervall weiter; erforderliche GetTexture-Netzwerkrequests bleiben unabhängig. Ergänzter Unit-Test für die Hysterese und neuer End-to-End-C++-Test `asset_cache_pressure_lru_tests.cpp`, der tatsächlich `TextureMemoryCache` und `MeshMemoryCache` mit jeweils nur zwei residenten Assets über 24 wechselnde UUIDs belastet, echte Hits/Misses/Evictions beobachtet und anschließend die zwei stabilen Recovery-Intervalle sowie den regionsübergreifend erhaltenen RAM-Inhalt kontrolliert.

**Block B – veraltetes Disk-Prefetch während Regionswechsel:** Produktionsfunktion `prefetch_texture_disk_to_memory` delegiert jetzt an einen separat kontrollierbaren `prefetch_texture_disk_to_memory_scoped`-Vertrag. Der integrierte `texture_prefetch_tests.cpp` hält eine asynchrone, gültige Disk-Leseantwort zwischen Start und Cache-Aufnahme an, wechselt dann die Regionsepoche und gibt den Worker frei. Das alte Texture-Bild darf nicht in die RAM-LRU übernommen werden. GetTexture-Hauptpfad und zuvor bestehende HTTP-Regionsepochen-Sperren bleiben unverändert. Das ist ein deterministischer Worker-/Cache-Vertragstest, kein echter Grid-Download.

**Block C – OpenGL3.3-Alpha-Mask-vs-Blend-Routing:** Neue zentrale CPU-Routingfunktion `face_material_requires_blending(tint_alpha,loaded_texture_has_alpha,mode)`. AlphaMode=1 berücksichtigt Texture-Alpha für den transparenten Pass; Mode=2 behandelt Texture-Alpha als GPU-Discard-/Mask-Bedingung im grundsätzlich opaken Pass, sofern Tint-Alpha 1 ist; Mode 0/3 lösen nicht allein wegen Texture-Alpha einen Blend-Pass aus. Der Shader führt `discard` bei AlphaMode 2 wie zuvor aus. Neuer tatsächlich ausgeführter Frame-Zähler `scene_alpha_mask_draw_calls`, zusätzlich zu bestehenden `scene_alpha_draw_calls` und `scene_opaque_draw_calls`, wird in `RenderDiagnostics` und im anonymisierten `InWorldAssetSnapshot.previous_frame_scene_alpha_mask_draw_calls` ausgegeben. Die echten OpenGL-Regionstransitions-/Mesh-Face-Tests prüfen Mask-Faces und sowohl opaque als auch blended Drawcalls; `material_pass_tests.cpp` enthält alle vier Moduskontrakte.

**Block D – realistische Cache-Druck- und Renderer-Sitzungsanalyse:** `tools/ogl_asset_log_report.py` wertet neueste zusammenhängende gültige Region-Snapshotserien aus und berichtet `latest_region_texture_ram_pressure_samples`, `latest_region_mesh_ram_pressure_samples`, `latest_region_peak_alpha_draw_calls`, `latest_region_peak_alpha_mask_draw_calls`, `latest_region_peak_gpu_upload_p95_us`. `tools/ogl_inworld_acceptance.py` ergänzt die sicheren Werte unter `observations`; sie sind Diagnose, nicht automatisch ein weiterer erfolgreicher Abnahmebeweis. Python-Regressionen belegen Anonymisierung und korrekte serielle Metrik. **68/68 CTests** vor finalem Release-Rebuild bestanden, einschließlich echter GLFW/OpenGL-Szenen und CPU-LRU-Stress.

**Ist-/Abnahmegrenzen:** Autorisierte Linux-Workstation `$HOME/.local/state/opengenesislink/viewer.log`: weiterhin **0** gültige aktuelle `InWorldAssetSnapshot`-Einträge, `ogl_inworld_acceptance.py` meldet `not_verified`, Exitcode 2. Daher keine echte NexVerse-Region-, Avatar-, WASD/Bewegungs-, Mesh-Vollständigkeits-, Teleport- oder CAPS-EventQueue-Freigabe. Diese bekannten Themen bleiben wie vereinbart für die spätere Live-Integrations- und Fehlerbehebungsphase dokumentiert. Alle aktuell getesteten Alpha-/Staging- und Cache-Szenen sind synthetisch, auch wenn der tatsächliche NVIDIA-OpenGL3.3-Treiber verwendet wurde.

**Nächster 3–4-Blöcke-Release Alpha.79:** Transparenz-/Maske-Sichtbarkeit zusätzlich mit reproduzierbarer Pixel-/Framebuffer-Abnahme prüfen, kombiniertes Texture-/Mesh-Staging über mehrere Regionswechsel noch strenger validieren, Ressourcen-Counter/Hysterese bei typischen Cachegrößen profilieren und In-World-Akzeptanzmeldungen für echte Simulatorlogs verfeinern sobald aktuelle Regionsdaten vorliegen. Maßgeblich bleibt `projectplan.md`.

GitHub: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (alten divergenten `develop` nicht mergen). Tests `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Diagnose `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`.

Fortsetzung beim nächsten `weiter`: GitHub `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`, lokale Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Verbindlich zuerst `projectplan.md`, `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md`, `docs/LOG_ANALYSIS.md` lesen und `git status` kontrollieren. 3–4 größere Blöcke, Debug-Build, alle CTests auf DISPLAY=:0, Linux GUI-Welcome Smoke, VERSION/Docs, GitHub Commit+Push und SHA von Local/Tracking/GitHub exakt vergleichen. Keine ungefragte Verschmelzung des divergenten Branches `develop`. Echte In-World-Abnahme bleibt bis aktueller erfolgreicher Regionsdaten `not_verified`.


## 0.2.0-alpha.79 – Chatübergabe und nächster Entwicklungsstand

**Block A – visuelle Pixel-/Framebuffer-Abnahme für Alpha, AlphaMask und opake Meshfaces:** Neuer echter GL3.3-GPU-CTest `tests/renderer_alpha_framebuffer_tests.cpp` erstellt ein verstecktes GLFW-Fenster, lädt ein GPU-Mesh und eine 2×2-Texture und liest nach `render_world()` den zentralen tatsächlich gerenderten RGBA-Pixel mit `glReadPixels`. Geprüft werden ein opakes rotes Face, vollständiger AlphaMask-Discard bei AlphaMode 2/Cutoff 128, anschließend restaurierte Opakheit und Blend-Rendering bei 50 % Alpha; Framepass-Zähler müssen mit der Sichtbarkeit übereinstimmen. Die maskierte Pixelreferenz ist ein vollständig transparenter AlphaMode-1-Face im **gleichen aktiven Szene-Rendermodus**, nicht ein No-Scene-Frame, da diese unterschiedliche Hintergrundpfade nehmen. Der neue GL-Test läuft auf dem echten NVIDIA OpenGL 3.3-Treiber und bestand. Eine synthetische Einzelpixelabnahme ist keine vollständige PBR-/OpenSim-Bildqualitätsfreigabe.

**Block B – drei vollständige GPU-Regionswechsel unter Mesh-/Texture-Staginglast:** Der bestehende tatsächliche GLFW/OpenGL-3.3-Test `tests/renderer_gpu_soak_tests.cpp` führt weiter 160 Frame-Draws, 160 transaktionale Texture-Replacements und 40 künstlich erzeugte Staging-Deferrals aus. Zusätzlich entfernt er bei Frame 40, 80 und 120 alle GPU-Texture- und Mesh-Ressourcen, setzt die Region-bezogene `GpuStagingRecovery` zurück und verifiziert vor dem Wiederaufbau **null GPU-Mesh-Draws, 24 fehlende GPU-Mesh-Objekte** und keine wartenden alten Staging-Einträge. Anschließend werden alle 20 gültigen synthetischen GPU-Meshes und acht Texturen frisch geladen; die vier absichtlich unbereit gehaltenen Meshobjekte bleiben als echte Fallbacks sichtbar. Alle Frame-/Passsummen und GL-Fehlerprüfungen müssen bestehen. Das testet GPU-Regions-Lebenszyklus und Upload-Recovery, nicht die Netz-/Simulatorintegration.

**Block C – Cache-Kapazitätsmatrix für echte Texture-/Mesh-RAM-LRUs:** Neuer C++-CTest `tests/asset_cache_capacity_profiles_tests.cpp` verwendet die realen `TextureMemoryCache`-/`MeshMemoryCache`-Implementierungen mit 8, 64 und 128 zulässigen Asset-Slots. Über sechs deterministische Zugriffswellen mit 96 verschiedenen UUIDs werden 1152 Zugriffe je Familie und Profil durchgeführt, inklusive bevorzugt wiederholten Hotspots, Hit/Miss und LRU-Evictions sowie Kapazitätsgrenzen. Beispiel-Testlauf: 8 Slots 383 Hits/769 Misses/761 Evictions; 64 Slots 875/277/213; 128 Slots 1056/96/0 pro Assetfamilie. Diese Testwerte sind reproduzierbare synthetische Zugriffsmuster und kein kalibriertes Profil realer OpenSim-Assetgrößen/Hardwarelatzenzen. Für alle drei Größen sind triviale Fehlzustände, unkontrolliertes Wachstum und fehlende Wiederverwendung abgesichert.

**Block D – konkrete, datensparsame In-World-Fehlerdiagnose:** `tools/ogl_inworld_acceptance.py` erweitert das bisherige `ogl.inworld.acceptance.v2` um `failure_codes` und `next_check`. Aus den existierenden konkreten Health-Checks werden nur feste sichere Diagnosecodes erzeugt, beispielsweise `no_current_region_snapshots`, `insufficient_region_samples`, `texture_residency_incomplete`, `gpu_mesh_draws_missing`, `mesh_fallback_still_drawn`; niemals UUIDs, CAP-URLs oder Rohlogs. Der empfohlene nächste Prüfpunkt priorisiert fehlende Live-Regionensnapshots, veraltete/unterbrochene Serien, nichtleere Asset-Demandsets, niedrige Residency und echte Mesh-/Fallback-Draws. `evidence_pass` erhält die leere Failure-Liste. Python-Regressionen prüfen fehlende Daten, valide Serien, veraltete/unterbrochene Daten und kritische Missing-Mesh-Fälle. Im aktuellen echten Viewerlog: **0** gültige InWorld-Snapshots, `not_verified`, `failure_codes=['no_current_region_snapshots']`, CLI-Exitcode 2. Der fehlende Live-Test wird ausdrücklich nicht als Fehlerbehebung oder als bestanden ausgegeben.

**Gesamtprüfung:** Debug-C++23-CMake-Build, reale OpenGL-Regressionen und **70/70 CTests** im Entwicklungsstand bestanden. Nach der Versionspflege vollständiger Neuaufbau und GUI-Welcome-Smoke auf NVIDIA RTX 4070 SUPER vorgesehen; GitHub-Push ausschließlich nach bestandener Endprüfung auf `develop-current`.

**Nächste vier geplante Releaseblöcke Alpha.80:** Erweiterte Pixelreferenzen für Mehrfach-Overlays und AlphaSort, kontrollierter GPU-Rollback-/Uploadfehler-Regionswechsel, Cache-/Residency-Priorisierung nach messbarer Druckphase und tatsächliche In-World-Test-Protokollierung sobald erfolgreiche Authentifizierung/Regionsdaten vorliegen. Bekannte Avatar-/WASD-/Teleport-/CAPS-EventQueue-Probleme bleiben wie vereinbart bis zur späteren Integrations-/Bugfixphase zurückgestellt; `projectplan.md` bleibt maßgeblich.

CLI auf autorisiertem Linux-Desktop: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Live-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.

Nächster Chat: Repository `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (NICHT den divergenten `develop` übernehmen), lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst Git-Status sowie `projectplan.md` (maßgeblich), `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md`, `docs/LOG_ANALYSIS.md` lesen. User will 3–4 größere Entwicklungsblöcke je `weiter`. Nach Implementierung `VERSION`, sämtliche Docs, vollständigen C++23-Debug-Build, alle CTests mit `DISPLAY=:0` und den Linux-GUI-Welcome-Smoke ausführen. Commit+Push nur auf `develop-current`, lokale SHA, Tracking-SHA und GitHub-SHA vergleichen, sauberes Worktree bestätigen. Die echten InWorld-Snapshots fehlen: keinesfalls Synthetic-Framebuffer-Test als erfolgreichen Simulatorbeitritt deklarieren.
