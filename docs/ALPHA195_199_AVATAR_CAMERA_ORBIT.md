# Alpha.195–199 – Avatar-Kamera-Orbit und reale Vorderansicht

Stand: 16.10.2026. Version 0.2.0-alpha.199.
Primärer Arbeitsplatz Linux Mint jam-MS-7D28 via Desktop Commander,
User jam, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer.
GitHub develop-current, Basis d00089261326bb33aba715311d9387636f8e3b9f.
Pi5 ausschliesslich GitHub ARM64 Headless-Runner.
Nie unaufgefordert auf sichtbarem DISPLAY=:0 starten.

## Alpha.195 – Richtige lokale Avatar-Orbit-Kamerageometrie
third_person_camera.hpp unterstützt nach dem 3–24m-Zoom jetzt einen
optionalen horizontalen Kamerawinkel im lokalen Agent-Heading-System.
0 Grad = hinter dem eigenen Avatar, +90 Grad = Avatar-Seitenansicht,
180 Grad = vor dem eigenen Avatar mit Blick zurück. Blickziel bleibt
die Position des eigenen Avatars +1,45m, 3D-Forward/Left bleiben
normiert. Die Simulation erhält keine Änderung der Avatarrotation,
-position, AgentUpdate oder Asset-/COF-/CAPS-Anfragen.
Zusätzlich echter Fehlerfix gegenüber Alpha.194:
der Zoom-Fallback wird jetzt auch im Nenner des Blickvektors verwendet;
NaN/Inf-Abstände gelangen nicht mehr in die GPU-Kamera-Matrix.

## Alpha.196 – Tastaturbedienung im realen Viewer
V schaltet direkt von Hinten nach Vorne, bei Frontansicht zurück
nach Hinten. Linke/rechte eckige Klammer drehen die Kamera lokal
um jeweils 45 Grad. Die Tasten werden nur bei authentifizierter
aktiver Szene und ohne F9/F10/Experiences/Chat/andere modale
Eingaben angenommen. Bestehende F5-, Zoom-, Bewegungs- und
Inventartasten bleiben erhalten. Die autoritative Rotation
des Avatars wird dabei nicht verändert.

## Alpha.197 – Sichtbare echte UI-Kontrollen
Neben Zoom +/- gibt es auf der bestehenden rechten HUD-Leiste
drei anfassbare Tasten für orbit links, Front/Rückseite und
orbit rechts. Beschriftung Hinten/Vorne/Rechts/Links/Seite N
zeigt die tatsächliche lokale Orientierung. Exakte identische
Buttonkoordinaten gelten für Maus-Hit-Test und UI, auch bei
16:9, 21:9, 32:9 sowie F5-freier Weltansicht. Keine
dekorativen oder unverdrahteten Viewer-Buttons.
Auch WorldRenderer, Texture-/Mesh-Streaming-Priorisierung
und WorldRay-Objektauswahl verwenden nun dieselbe verstellte
Kamera statt der bisherigen festen Avatar-Nachlaufposition.

## Alpha.198 – Native Tests und echte OpenGL Pixel
Neuer C++23 Test nexverse-camera-orbit-controls-tests,
auch in Pi5 Headless-CI registriert. Er prüft gültige
Kamerapositionen in allen Richtungen, Normalisierung,
180-Grad-Front und Rückschaltung, 45-Grad-Schritte,
Widescreen-Maushitboxen, Abstände und NaN-/Inf-Fallback.
Versteckter Linux-NVIDIA OpenGL-Framebuffer-CTest
nexverse-renderer-alpha-framebuffer-tests rendert
ein echtes double-sided Testmesh ohne TerrainLayerData
sowohl von der Rück- als auch der Vorderseite des
Avatars. Für beide Orbitpositionen werden wirkliche
GPU-Bildpixelunterschiede zu einem Frame ohne Mesh
und Mesh-GL-Draws gefordert. Dies ist eine
lokale GPU-Fixture, kein echter Grid-Avatarbeweis.

## Alpha.199 – Dokumentation, vollständiger Build und CI
Mit CMake/Ninja Linux-x64 Vollbuild, allen nativen/GL-
CTests, Python-Regressionen, Pi5 ARM64 Headless-C++-Satz
und Voll-Viewer-Start unter isoliertem Xvfb getestet.
GitHub develop-current direkt vom Mint-Checkout pushen
und SHA/saubere Worktree + Pi5-CI verifizieren.
OpenGenesisLINK Server-main am 16.10.2026 weiterhin
ec760cacfff2fe505cf51991985553ed434eb121.
Der ältere 22-Commit-Server-Kompatibilitätsreview
bleibt offen. reviewed_sha nicht ohne Review vorziehen.

## Noch nicht abgeschlossen
Echter autorisierter NexVerse-Login mit sichtbarer
3D-Welt und eigenem Avatar, originale Firestorm-
TexLayer 11-Bake-Pixelparität und sichere
COF Wear/Unwear mit Session-/Version-/Readback-/
Rollback-Logik sind weiterhin offen. Die neue
Kamera-Orbit-Funktion ersetzt keine Avatar-Mesh-/
Textur-/Skinnning- oder Server-E2E-Verifizierung.

## Nächste Entwicklung
Alpha.200+: echte Serverdaten/Welt-Rendering-Korrelation,
Frustum-/Avatar-Pixelprobe und TextureEntry; F9 Wearables
und UI-Parität parallel; ältere OpenGenesisLINK-CAPS/
JPEG2000-Änderungen einzeln reviewen.
