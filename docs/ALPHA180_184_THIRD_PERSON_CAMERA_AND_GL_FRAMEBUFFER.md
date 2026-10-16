# Alpha.180–184 – Stabile Third-Person-Kamera und sichtbare GPU-Welttests

Stand: 16.10.2026. OpenGenesisLINK Viewer 0.2.0-alpha.184.
Primärer Entwickler: Desktop Commander auf Linux Mint jam-MS-7D28
als Benutzer jam, Checkout
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer.
GitHub Branch develop-current, Start-HEAD
d5c10618d1943f359765cdb4f41510b85ecb3ecc.
Raspberry Pi 5 nur Self-hosted ARM64 CI.
Keine automatischen sichtbaren Viewerstarts auf DISPLAY=:0.
Keine echten produktiven Login- oder Outfit-Mutationen.

## 180 – Korrekte dritte-Person-Ansicht bei Simulator-Rotation
Die bisherige Third-Person-Kamera verwendete eine nicht gesondert
normalisierte Quaternion aus world::AgentState. Nonunit-/Null- oder
unvollständige AgentUpdate-Rotation kann Blickrichtung oder
Kameraversatz verfälschen und die Welt/den Avatar scheinbar
verschwinden lassen. Neuer reiner C++23-Header
render/third_person_camera.hpp normiert die eingehende
Serverquaternion als double und ermittelt einen stabilen
horizontalen Heading (Yaw). Kamera: acht Meter hinter dem
Avatar, 4,2 Meter oberhalb des Zielpunkts, Blick zum Avatar.
Nullrotations- oder nicht-endliche Quaternionswerte verwenden
sichere +X-Orientierung. NaN/Inf und unrealistische Koordinaten
außerhalb des plausiblen Bereichs von +/-1 Mio. Meter werden
durch finite Referenzposition ersetzt. Dies ist eine lokale
GL-Kamerasicherung, KEINE Manipulation am Simulator oder am
autoritativen AgentWorldModel. Neue native C++ Tests prüfen
Identity, 90 Grad, nicht normierten Quat, ungültige Rotation,
extreme Positionswerte und Pitch.

## 181 – Echte Viewer-Kameraverkabelung
src/app/application.cpp render_camera_from_agent verwendet
die neue mathematisch abgesicherte Pose für
RenderCamera.position, forward, left und up. Die sonstigen
lokalen Movement-, Grid- und Loginpfade bleiben unberührt.
Das Render-Backend normalisiert und berechnet die GL-Basis
aus denselben Werten. Eine korrupte Quaternion landet
nicht mehr ungefiltert in der Third-Person-Sichtprojektion.

## 182 – Sichtbarer Kamera-Fallback in der Inworld-Oberfläche
Die bisherige UI/WorldSceneProof unterscheidet WorldModel-
Objekte, GL-Draws, fehlende Meshes und Terrain.
Ein neuer Flag camera_fallback wird aus derselben
berechneten pro-Frame Third-Person-CameraPose gespeist.
Ist die Simulatorpose ungültig, meldet die bestehende
Verbindungs-/Szenekarte sichtbar
"Kamera: ungültige Serverpose (Fallback)" statt
lediglich scheinbar erfolgreicher GL-Zähler.
Native Headless Regression prüft diese Meldung.
Keine neuen Netzwerkrequests, keine UI-Überlappung.

## 183 – Echtes OpenGL Framebuffer-Bild nach Third-Person-Kamera
Der NVIDIA-GL3.3 Renderer AlphaFramebuffer-CTest nutzt
jetzt die exakt in der App verwendete dritte-Person-Kamera.
Er zeichnet ein tatsächliches Mesh ohne vorausgesetzte
Terrain-LayerData und vergleicht den mittleren GL_RGBA-Pixel
mit einer Szene ohne Mesh. Es muss ein substanzieller
sichtbarer Pixelunterschied UND ein Scene-Mesh-Draw
vorliegen. Das ist stärker als nur erfolgreiche
glDrawElements-Zähler; es ist aber noch kein Live-Grid-
Welt-/Avatar-Framebuffernachweis.

## 184 – Ungültige Quaternions und GPU-Fallback geprüft
Derselbe native GL-Framebuffer-Test verwendet zusätzlich
eine ungültige Nullquaternion und bestätigt, dass das
lokale Fallback bei identischem Avatarziel denselben
Frame-Pixel erzeugt und keinen OpenGL-Fehler auslöst.
Extreme finite Positionswerte werden als ungültig markiert,
um instabile Matrizen zu vermeiden. CMake registriert
nexverse-third-person-camera-tests, Pi5 Native-Header-
Skript integriert sie. Der gesamte Linux-x64 CMake-/Ninja-
Build mit GLFW/OpenGL GPU CTests und Python-Regressions-
suite wird vor Veröffentlichung erneut geprüft.
Vollständiger Viewer Smoke ausschließlich via
scripts/mint-isolated-gui-smoke.sh (Xvfb), keine sichtbare
Desktop-Anwendung. Nach Push Self-hosted Pi5-CI prüfen.

## OpenGenesisLINK-Serverabgleich
Zum Arbeitszeitpunkt unverändertes Server main:
ec760cacfff2fe505cf51991985553ed434eb121
(JPEG2000 CSJ2K -> OpenJPEG Layer-Retry).
Der reviewed_sha in
docs/OPENGENESISLINK_SERVER_BASELINE.json
bleibt 4e99976630f1008db9014ea2b86c3723b09e9fe6,
weil 22 ältere Upstream-Commits noch nicht vollständig
fachlich geprüft sind; kein automatisches Promote.

## Weiter offene Punkte
Ein echter erfolgreicher OpenGenesisLINK/NexVerse Grid-Login
und der sichtbare echte Avatar/Welt-Framebuffer sind weiterhin
NICHT bestätigt. Die Kamera-Tests nutzen synthetische GL
Meshes, keine echten Userdaten. Firestorm Original .llm,
vollständige 11-Bake TexLayer Pixel-/Wearable-Komposition,
sichere COF Wear/Unwear/Readback/Rollback, Viewer-
Menü-/Button-/Fenster-Parität und finaler Release-Build
bleiben im Projektplan.

## Nächste Entwicklungsschritte
1. Nutzerautorisierte Grid-Sitzung und die vorhandene
   Kamera-/WorldModel/CAPS-/GPU-Diagnose zusammen auswerten.
2. Wenn trotz validem Camera und GL>0 nichts sichtbar:
   echten Szenepixel-/Depth-/Frustum-/Regionformat-Beleg prüfen.
3. Texture- und Mesh-Ladefehler samt JPEG2000 für Live-
   AssetUUIDs und GPU-Epoch-Verlauf analysieren.
4. F9 Outfit-/Appearance-Workflows mit sicherer und
   überprüfbarer COF-Readback-Synchronisierung ergänzen.
5. Firestorm GUI-Navigation und OpenGenesisLINK
   Upstream-Patchprüfung weiterhin parallel fortsetzen.
