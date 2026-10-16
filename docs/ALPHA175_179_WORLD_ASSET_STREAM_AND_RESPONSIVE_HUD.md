# Alpha.175–179 – Welt-Texturen/Mesh-GPU-Coverage und Responsive Inworld-HUD

Datum 16.10.2026. OpenGenesisLINK Viewer 0.2.0-alpha.179.
Entwicklung primär auf Linux-Mint-PC jam-MS-7D28 über Desktop Commander
als jam, Branch develop-current. Baseline-Commit
c96213cb9ae8edf017d9a534d22005de3c9fbcf1. Raspberry Pi 5 nur
automatischer ARM64-Runner. Keine ungefragten Viewer-Fenster auf :0,
keine produktive Simulator-/Outfit-Mutation oder Account-Anmeldung.

## 175 – Tatsächliche Textur-/Mesh-GPU-Abdeckung
Neue read-only UI-Provenienz include/nexverse/ui/asset_stream_proof.hpp
erfasst die eindeutigen von Weltobjekten referenzierten Asset-UUIDs
separat für Texturen und Meshes. Doppelte Verweise und Null-UUIDs
werden nicht mehrfach als fehlende Ressourcen gezählt. Nur
tatsächlich in der aktuellen GL-Region resident vorhandene
entsprechende IDs zählen als GPU geladen. Der Wert bedeutet
NICHT, dass das Asset bereits auf dem Bildschirm zu sehen ist.
Header-only C++23-Test asset_stream_proof_tests testet fehlende,
doppelte, veraltete und geladene Assets. Dieser Test ist für den
Self-hosted Pi5-ARM64-CI und die CMake-x64-Tests registriert.

## 176 – Sichtbare Inworld-Weltstreaming-Diagnostik
In der bestehenden rechten NexVerse-Verbindungskarte erscheint nun
"Tex N/M GPU | Mesh P/Q GPU" direkt unter Terrain/WorldDraw/CAPS.
Der Zähler kommt pro Frame aus scene_texture_samples,
scene_mesh_samples, object_texture_loaded_ids und
mesh_asset_loaded_ids, nicht aus hypothetischen Server-Fetches.
Damit kann ein fehlender Weltaufbau nach authentifiziertem
Regionbeitritt künftig gezielt in fehlende Assets oder fehlende
WorldModel/GL-Draws getrennt werden. Nur read-only, keine extra
GetTexture/GetMesh-Anforderungen, keine neuen CAPS-/LLUDP-Aktionen.

## 177 – Sichtbare, responsive Statuskarten ohne Überlappung
Neue reine header-only Layoutfunktion
ui::world_hud_panel_visibility(rail_height,scale) statt
starr bedingter Avatar- und immer sichtbarer Systemkarte.
Die Verbindungskarte (untere Kante y+215), Systemkarte (y+359)
und Avatar-Pipeline-Karte (y+471) werden unabhängig erst dann
angezeigt, wenn bis zur unten verankerten FPS/Frame-Leiste
58 skalierte Punkte plus Sicherheitsabstand von 12 skalierte
Punkte frei sind. Verhindert Überdeckung bei niedrigen
Fensterhöhen einschließlich hochskalierter Widescreen-Layouts.
Neue C++23 Layouttests prüfen alle drei Breakpoints und
ungültige Fenstergrößen. Source-Regressionen prüfen die echte
Render-Verkabelung auf dem Mint-PC.

## 178 – Native NVIDIA-GL-Texturlebensdauer als UI-Beleg
Der versteckte echte OpenGL-3.3-CTest
renderer_region_transition_tests wurde um die
Weltstreaming-UI-Provenienz erweitert. Die Test-GL-Textur
avatar-bake ist tatsächlich GPU-resident und ihre zweimal
referenzierte UUID ergibt genau einen geladenen Textureintrag.
Nach GL-Eviction des originalen Texturhandles bleibt die
angeforderte Texture-UUID erhalten, aber die geladenen GPU
Assets stehen korrekt bei null. Native RegionTransition-
Regression auf NVIDIA GeForce RTX 4070 SUPER PASS.
Der GL-Test nutzt absichtlich synthetische Images und
keinen Live-Server-Avatar.

## 179 – Native Linux-x64-Gesamtvalidierung und GitHub
CMake/Ninja Linux-x64-Vollbuild des tatsächlichen
0.2.0-alpha.179 Viewers, alle Native/GL3.3/Python-Tests,
Pi5-kompatibler Headless-C++23-Satz und isolierter
GUI-Smoke unter scripts/mint-isolated-gui-smoke.sh.
GUI-Test ausschließlich unter Xvfb, nicht auf
DISPLAY=:0 des Nutzers; original Firestorm .llm/
133-Joint Rig wird lokal aus dem Cache geladen.
Nach erfolgreicher Prüfung GitHub-Commit direkt
vom Mint-PC nach develop-current und Pi5-CI-Bestätigung.
Die lokalen/finalen Prüfzähler sind aus tatsächlichen
Testprotokollen zu entnehmen, nicht im Voraus zu behaupten.

## OpenGenesisLINK und Firestorm Compatibility
Zum Abgleich: opengenesislink/opengenesislinkserver main
ec760cacfff2fe505cf51991985553ed434eb121.
Keine neue Serveränderung seit Alpha.174; der
OpenJPEG-Fallback J2KDecoderModule wurde bereits
klassifiziert, aber die alte 22-Commit-Kompatibilitäts-
Überprüfung ist weiterhin unvollständig. Deshalb
docs/OPENGENESISLINK_SERVER_BASELINE.json reviewed_sha
4e99976630f1008db9014ea2b86c3723b09e9fe6
nicht automatisch erhöhen. Firestorm originale Body
LLMs und vorhandene Viewer UI/Protokollintegration bleiben
führend; zusätzliche Diagnose ist kein eigenes Protokoll.

## Offen
Echter NexVerse-Login mit verifizierter Welt-/Avatar-
Framebuffer-Sichtbarkeit und Kamera/Frustum, vollständige
Firestorm TexLayer Pixelkomposition aller elf Bakes,
sicherer COF-Wear/Unwear mit Nutzerautorisierung,
Server-Readback/rollback und vollständig bedienbare
Firestorm-Menüs sind weiterhin nicht Ende-zu-Ende bewiesen.

## Nächste fünf Arbeitsblöcke
1. Autorisierte echte Grid-Sitzung: gezielt Terrain/LLUDP/CAPS,
   GPU Tex-/Mesh-Zähler und Avatar-Pipeline beobachten.
2. Falls WorldModel-Objekte vorhanden aber GL0: Kamera-/Frustum-,
   Primitive-/Mesh-/Material-Renderpipeline gezielt korrigieren.
3. Falls GL>0 aber Tex/Mesh-GPU0: echte Asset-/JPEG2000/CAPS-
   Fallbacks und Region-Epoch-/Cachefehler untersuchen.
4. F9 Outfit/Appearance mit sicheren zustandsbestätigten
   COF-Kommandos weiterentwickeln und sichtbar bedienen.
5. OpenGenesisLINK ältere Servercommits vollständig auf
   CAPS/LLUDP/Asset-Rechte patchweise prüfen.
