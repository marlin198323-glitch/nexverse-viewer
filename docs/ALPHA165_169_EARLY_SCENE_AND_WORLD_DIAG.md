# Alpha.165–169 – Frühe Szene, echte Welt-Diagnose und sichere GUI-Tests

Datum: 16.10.2026. OpenGenesisLINK Viewer Version 0.2.0-alpha.169.
Entwickelt ausschließlich auf Linux Mint jam-MS-7D28 mit Desktop Commander,
Branch develop-current. Ausgangs-SHA:
1d74ed177370a84c198084082c03dfa2986e07dd.
Pi5 nur GitHub Self-hosted ARM64-CI. Keine Produktiv- oder Grid-Account-
Schreibaktionen, automatischer Voll-Viewer-Start nur isoliert mit Xvfb.

## Alpha.165 – Terrain als spätes Streaming, nicht als blockierender Login-Gate
Bisher koppelte WorldModel::ready() den ganzen Viewer an das erste
TerrainLayerData-Paket: Region verbunden, Regionhandle, Agent präsent,
gültige Agent-ID, Session-ID UND terrain.available. Bei verzögerten
Terrain-Paketen konnte trotz gültigem Circuit die Inworld-UI nicht
aufgehen, einschließlich Avatar- und Mesh-Diagnostik.
Neue WorldModel::scene_ready() erfordert Region.connected, handle != 0,
Agent.present, Agent-ID und Session-ID; ist bewusst nicht terrainabhängig.
Die App nutzt scene_ready() an den zwei bisherigen
simulation_ready-Übergängen. Die vorherige Methode ready() bleibt als
strenges Scene-plus-Terrain-Readiness-Signal erhalten. Damit wird
keine Simulator-Authentizitätsprüfung abgeschaltet. SceneRendering
und Bewegung bleiben an verbundenen Simulator und Agent gebunden.

## Alpha.166 – WorldModel vs GL-Renderstatus getrennt sichtbar
Neue read-only UI-Hilfe world_scene_proof.hpp meldet nur echte
WorldModel.terrain().valid_patch_count (0..256) bzw. WARTE bei
fehlendem Terrain, die empfangenen Weltobjekte und tatsächliche
RenderDiagnostics.scene_draw_calls / scene_missing_mesh_draw_calls.
Der Inworld-Status meldet bei empfangenen Objekten, aber ohne GL-
Submission explizit empfangen | GL 0 statt erfolgreiche Darstellung.
Fehlende Mesh-Assets werden als MeshFehlt aus echter GL-Fallback-
Metrik gekennzeichnet. Kein künstlicher Welt- oder Avatar-Status.

## Alpha.167 – CAPS EventQueue lifecycle statt erfundenem LIVE
Die rechte GUI-Verbindungs-/Statuskarte zeigt nun CAPS mit dem
tatsächlichen vom bestehenden RuntimeState gelieferten Lifecycle
(WAITING, DISCOVERY, CAPS READY, EVENTQUEUE, CAPS LIVE, ERROR).
Nur echtes CAPS LIVE gilt in der Anzeige als positiv. Eine bloß
advertisierte CAPS/EventQueue-URL ist kein beweis für Eventempfang.
Statuskarte und Systemstatus wurden angepasst, um Kollisionen
mit Terrain-, Objekt- und GL-Zeilen zu vermeiden.

## Alpha.168 – Readiness/Rendering durch native Tests
world_model_tests: Szene wird bei gültiger Region- und Agent-Identität
schon vor Terrain bereit, volle terrainabhängige readiness später;
Session-ID fehlt -> nicht bereit, Agent nicht present -> nicht
bereit, Regionhandle null -> nicht bereit. Transition und Reset
entziehen Szene und Terrain fertig.
Neuer Header-only C++-Test world_scene_proof_tests verifiziert
Terrain fehlt, Layer-Patches, Objekte ohne GL und echte GL-Draws.
Vorhandener versteckter GLFW/NVIDIA OpenGL-3.3 Region-Transition-
CTest prüft explizit 3 Mesh-GL-Draws schon OHNE set_terrain.
Neue Targets in CMake, World-Szene-Prüfung in Pi5-Headless CI.
Keine echten Nutzeranmeldedaten oder produktive Simulation geändert.

## Alpha.169 – Dokumentation, Mint Build/GPU und CI
Version 0.2.0-alpha.169. Vollständigen x64-Viewer-Build und
sämtliche nativen/GL/Python-Tests auf Linux-Mint ausführen;
vollen Viewer ausschließlich mit scripts/mint-isolated-gui-smoke.sh
im eigenen Xvfb-Display starten (kein Fenster auf Nutzerdesktop).
Nach Mint Commit und Push muss Self-hosted Pi5-ARM64-CI separat
bestätigt werden. OpenGenesisLINK-Server main
ec760cacfff2fe505cf51991985553ed434eb121; unverändert gegenüber
Alpha.164. Server-Review für ältere 22 Commits bleibt offen und die
reviewed_sha wird nicht automatisch angehoben.

## Offene E2E-Prüfungen
Die unabhängige frühe SceneReady kann einen realen UI-/World-Start-
Blocker beseitigen, beweist aber noch keine korrekt sichtbare
NexVerse-Welt oder Avatar-Darstellung nach authentifiziertem Login.
Hierfür fehlen echte CAPS/EventQueue-/ObjectUpdate-/Terrain-/
AvatarAppearance-Traces und GPU-Framebuffer-Sichtprüfung.
Firestorm TexLayer 11-Bake-Pixelparität, COF Outfit Wear/Unwear mit
Server-Readback und sicheren Rollbacks sowie finale Release-Builds
bleiben gesonderte Aufgaben.

## Nächste Arbeitsblöcke
1. In autorisierter echter Simulatorsitzung neuen WorldSceneProof
   und EventQueue-Lifecycle bei verzögertem Terrain messen.
2. Avatar und Weltfrustum, Mesh- und Texture-Pipeline mit
   Original OpenGenesisLINK Live-Events durchtesten.
3. Outfit-Kommandos nur mit User-Consent, COF-Version, Readback
   und Rollback integrieren und in F9 sichtbar absichern.
4. Firestorm Menü-/Fenster-/Widescreen UX parallel zum Kern verbessern.
5. Ältere Server-CAPS-/LLUDP-/Texture-Änderungen einzeln überprüfen.
