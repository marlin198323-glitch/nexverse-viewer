# OGL Alpha.235–244 – Avatar-, Terrain- und Mesh-Evidenz

Stand 16.10.2026. Entwicklung auf Linux Mint jam-MS-7D28, Benutzer
jam, Repository marlin198323-glitch/nexverse-viewer, Branch
develop-current. Raspberry Pi5 ausschließlich self-hosted ARM64 CI.
Version 0.2.0-alpha.244. Upstream OpenGenesisLINK Server main
ec760cacfff2fe505cf51991985553ed434eb121 bleibt die
aktuell verglichene Basis (CSJ2K/OpenJPEG Fallback). Der reviewed_sha
wird ohne vollständigen Patch-Level-Review nicht erhöht.

## Die zehn Blöcke

235. Der vom Nutzer verlangte verbindliche Arbeitsumfang wird
von fünf auf zehn echte Code-/UI-Blöcke je WEITER angehoben,
direkt am Anfang von CHATUEBERGABE.md, AGENTS.md,
projectplan.md, info.md und als default_blocks_per_run=10
in docs/PROJECT_PROGRESS.json. Neuer Chat beginnt mit
Alpha.245–254 und Gesamtstatus ca. 38 %.

236. AvatarVisualTruth unterscheidet akzeptiertes serverseitiges
Self-AvatarAppearance von Head/Upper/Lower Bake-Referenzen,
aktuellen GPU-residenten Body-Bakes und tatsächlich auf dem
originalen Linden-Mesh gezeichneten Bake-Texturen.
Ein farbloses Original-LLM-Mesh ist noch kein fertiger Avatar.

237. Der tatsächliche Terrain-GL-Upload zählt jetzt exakt,
wie viele der vier Texturlayer gültige RGBA-Bilddaten tragen.
1x1 Fallback-Swatches werden getrennt von Originaltexturen
behandelt. Bei Materialwechsel werden die Zahlen zurückgesetzt.

238. Der echte Terrain-Shader-Draw zeigt die unterschiedlichen
Zustände: kein GPU-Draw, reiner Fallback, nur 1–3 echte Layer
nebst Ersatzfarben oder volle 4/4 echte Terrainlayer.
Der rechte Welt-HUD-Status behauptet nicht mehr TEXTURE OK
bei einer einzigen echten Materialtextur. Region- und
Materialversionsschutz aus vorherigen Releases bleibt aktiv.

239. MeshQualityEvidence zählt angeforderte High/Medium/
Low/Lowest-LODs und exakt passende GPU-residente
Meshes der identischen UUID. Ein LOW-LOD-Asset wird bei
einer HIGH-Anforderung explizit NICHT als fertig gezählt.
Diese Statistik ist unabhängig von einer Netzwerkanfrage.

240. Der originale OpenGL-Skin zählt Server-Motion und
Keyframe-Skinning getrennt für den EIGENEN Avatar.
GL-Draws fremder Avatare können damit keinen echten
Self-Animationserfolg mehr vortäuschen.

241. Der rechte Welt-HUD-Avatarstatus zeigt die Keyframe-
und Server-Motion-Reife. F3 Mein Avatar zeigt aktuelle
Self-Skin-/Bake-Evidenz und Self-Animationsstufe;
das vorhandene Fenster Objekte zeigt die neue echte
LOD-Match-Quote. HUD-Karte und Höhen-Gate sind gemeinsam
angepasst, um Überlappung mit der FPS-Leiste zu verhindern.

242. Benutzerinitiierter privater F3-Diagnoseexport (Linux
POSIX 0600) enthält nur Whitelist-Zahlen für Materiallayer,
Shader-Fallback, Mesh-LOD-Matches, Skin-Bakes und
Self-Keyframe-Darstellung. Kein Username, Passwort,
Agent-/Session-UUID, IP, Chat oder Grid-Capability-URL.

243. Native C++23 Tests für AvatarVisualTruth,
TerrainLayerCoverage, MeshQualityEvidence, Body-Proof
und World-HUD-Höhenregeln sowie Python-Kontrakte für
tatsächliche Codeverbindungen, Exportdatensparsamkeit,
Zehnerregel und identischen Fortschrittswert.

244. Vollständiger Mint Linux-x64 CMake/Ninja Debug-Viewer
und native NVIDIA OpenGL Tests, Python Suite, ARM64-
kompatibler Headless-Satz, ausschließlich isolierter
Xvfb-Vollviewerstart über scripts/mint-isolated-gui-smoke.sh,
X11 WM_CLASS Originalicon, Commit/Push develop-current
und separater Pi5-ARM64-CI-Nachweis mit genau diesem SHA.
Nach Ausführung Status-/CI-Ergebnisse dokumentieren.

## Verbindliche fachliche Grenze und Projektfortschritt

**OGL-Gesamtfortschritt weiterhin ca. 38 % (gewichtete
Schätzung, 37,75 % ungerundet)**. Neue Beweise und
Statusfunktionen sind echte Entwicklungsfortschritte,
aber kein bestätigtes Ende-zu-Ende des NexVerse-Grids.

Offen: Server-Self-AvatarAppearance und vollständige
Head/Upper/Lower-Bake-Pixel in der Live-Sitzung;
Originalterrain versus Ersatzfarb-Layer;
durchgängige High-LOD-Meshes und Firestorm-kompatible
Server-Animationen/Attachments/Outfit-COF; weitere
GUI-/Inventar-/Social-Funktionen und Windows/macOS-E2E.
Kein sichtbarer Viewer-Teststart auf Host DISPLAY=:0.
Original-Systemicon und NexVerse-Welcome-Seite erhalten.
