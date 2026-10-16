# Alpha.205–209 – Kamerabeweis des Self-Avatars, Welt-Eintrittsdiagnose und Originalicon im HUD

Datum 16.10.2026. Viewer 0.2.0-alpha.209.
Hauptentwicklung: Linux-Mint-PC jam-MS-7D28, Desktop Commander,
Nutzer jam; GitHub marlin198323-glitch/nexverse-viewer,
Branch develop-current, Basis eb2d1855ca07783597b7271e7075bbe52183950c.
Raspberry Pi 5 bleibt NUR GitHub Self-Hosted ARM64 Runner.
Weder echte Grid-Testanmeldung noch Server-/COF-Schreibaktionen,
und KEIN Viewer-Start auf sichtbarem DISPLAY=:0.

## Alpha.205 – Reine Projektion aus authentischer Avatar-/Kamerageometrie
include/nexverse/ui/avatar_view_evidence.hpp modelliert das tatsächliche
Third-Person-Kamerablickfeld (FoV 60°, Aspect aus GL-Framebuffer),
Server-Agentposition + Avatarzentrum Z=1.45m, Camera forward/left/up.
Berechnet NDC/Viewport-Zentrum, valide Projektion und Vorwärts-Tiefe,
mit Guards für NaN/Inf, Framebuffer 0x0, außerhalb near plane,
außerhalb Bildfeld und ungültigen CameraPose-Fallback.
Der Befund "Mitte im Bildfeld" ist ausdrücklich NUR geometrische
Frustum-/Zentrum-Projektion, kein Beweis tatsächlicher Avatar-Pixel
oder Firestorm Bake-Komposition.
Native C++23 Regression deckt Rück-/Vorder-/Seitenansicht,
Widescreen, rückwärts gerichtete Kamera und invaliden Input ab.

## Alpha.206 – Tatsächlich live vom aktuellen WorldModel/GL
Die Anwendung erstellt pro Frame AvatarPipelineProof aus dem
aktuell geladenen render_agent, ThirdPersonCamera mit Zoom/Orbit,
GL-Framebuffer-Maßen und tatsächlich per GL ausgeführten
Self-Avatar-LLM-/Proxy-Draws. NICHT als GPU-Fertig markiert:
TextureEntry/Appearance wird weiter nur für passende
authentifizierte Session-ID, Agent-ID und gültige Region gewertet.
Das neue Stufenlabel trennt: Szene fehlt, AgentPosition fehlt,
Kamera unbrauchbar, Agent außerhalb Bildfeld, kein GL-Draw,
Ersatzgeometrie, Original-LLM-Draw.
Vorher konnte bestehender Avatarstatus 0 GPU-Draws
und echte Frustum-Präsenz nicht gleichzeitig erklären.

## Alpha.207 – Original OGL-Icon auch in der Weltoberfläche
Wie im Login der Alpha.204 werden oben links in der echten
3D-Welt-HUD-Kopfzeile nun die bereits hochgeladenen
Originalicon-Pixel als OpenGL-Image statt eines
Cyan-Platzhalters gezeichnet. Kein neues Bild, kein neues
Rendering, keine Veränderung der vorhandenen Welcome-Webseite.
Der Avatar-Pipeline-Status enthält eine dritte Zeile
"Kamera Bildfeld + echte GL-Anatomie-Draws"; alte UI-
Statuskarten, F5, F9, Welt-Maussteuerung und Gridstatus bleiben.

## Alpha.208 – Echte Ursache des verbleibenden Ladezustands
Neue pure world_entry_stage.hpp modelliert strikte
Login -> UDP-Circuit -> Region-Handle -> region.connected
-> Agent-ID + Session-ID -> AgentPosition -> world ready.
Während Login authentifiziert aber scene_ready=false:
Ladepanel auf dem bisherigen Welcome-Hintergrund zeigt
konkreten fehlenden Schritt statt nur generischem Text.
Progressbar geht bis max 96%, niemals fälschlich 100%,
solange ein erforderlicher Einstiegsschritt fehlt.
Terrain, CAPS und Objektzähler sind weiterhin optional
und blockieren scene_ready() ausdrücklich NICHT.
Zugriffsprüfung der WorldModel-Felder erfolgt mit Mutex;
keine Secrets/Session-UUIDs im Overlay.
Native C++23 Tests prüfen jede Stufe getrennt.

## Alpha.209 – GUI, Builds, Tests, GitHub
F5-freie Weltansicht zeigt nur bei fehlender Agentposition,
Kamera oder Meshdraw eine kleine Hilfszeile und bleibt bei
brauchbarem Avatar geometrisch frei.
Regressions in Pi5 Header-Satz, CMake Native Suite und
Python Branding/UI Contract. Vollständiger Linux-x64 Build,
alle GL/native/Python-Tests, Headless Suite und isolierter
Xvfb-Start gelten als Gates vor GitHub-Push.
OpenGenesisLINK Server-main bestätigt weiterhin:
ec760cacfff2fe505cf51991985553ed434eb121
(OpenJPEG Fallback bei CSJ2K-Fehlern).
Der server-reviewed SHA bleibt
4e99976630f1008db9014ea2b86c3723b09e9fe6,
da die frühere 22-Commit-Review noch nicht abgeschlossen ist.

## Offen
Tatsächlich sichtbarer eigener Avatar/WorldFrame NACH
AUTORISIERTEM Login im NexVerse Grid weiterhin NICHT bestätigt.
Die lokale Kameraprojektion + GL-Draw und Testfixtures dürfen
NICHT als Beweis von Avatar-Bildpixeln gelten.
TexLayer11/ServerBake Pixel/COF Wear/Unwear/Animation-
E2E bleiben separate Arbeitspunkte.
Nächste Blöcke: autorisierter echter Simulator-Pixel-/Scene-
Nachweis, Avatar-Mesh-/Animation-Zustand prüfen; CAPS
EventQueue/JPEG2000 Firestorm-Pfad und OpenGenesisLINK
per-Commit Server-Review; F9 UI und Lesen/Anwenden sicher
serververifizierter Outfit-Änderungen weiterführen.
