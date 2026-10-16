# Alpha.190–194 – Echter Third-Person-Zoom und F9-Server-Bake-Bedienung

Stand: 16.10.2026. OGL Viewer 0.2.0-alpha.194.
Primärer Host Linux Mint jam-MS-7D28 via Desktop Commander (User jam),
Repo marlin198323-glitch/nexverse-viewer, Branch develop-current;
Basiscommit a7ed20d39aa4b5f40eb807533a3e9854963cc1aa.
Pi5 nur GitHub ARM64 CI. Keine produktiven Konto-/Outfitänderungen.

## Block 190 – Third-Person-Kamerazoom
Render-Hilfe third_person_camera nimmt optional einen lokal verstellbaren
Abstand zwischen 3 und 24 Metern (Standard acht) an. Höhe und Blickrichtung
bleiben konsistent zum Avatar. Nicht-endliche Werte und extreme Eingaben
werden auf sichere Defaults geklemmt. Kein Movement-/LLUDP-Paket.

## Block 191 – Eingaben in die echte Kamera integriert
Echter GLFW ScrollCallback: Rad nach oben zoomt herein, nach unten heraus.
Auch Plus/Minus-Tasten inkl. Keypad steuern dieselbe Kamera. Guards
erfordern authentifizierte, aktive Simulation und ignorieren Eingaben in
F9/F10/Experiences oder sonstigen Eingabe-Floaters. Alle drei bisherigen
render_camera_from_agent-Verwendungen (GL-Welt, Texture- und Mesh-
Streaming) plus Objektauswahl/Raycast verwenden nun einheitlich
camera_zoom_distance statt fester 8m.

## Block 192 – Sichtbare Bedienelemente
Im Inworld-HUD und bei F5-freier Weltansicht erscheinen echte anklickbare
Plus/Minus-Kamerabuttons mit skalierter Live-Meterangabe. Hit-Test-
Geometrie ist identisch zu den gerenderten Buttons, korrekt auf
Widescreen und in kleineren Fenstern. Mousewheel/Buttons/Keyboard
verändern unmittelbar die echte Renderer-Kamera, nicht nur eine Zahl.

## Block 193 – F9 Bake-Vorschau und GPU-Nachweis
Die vorhandene Firestorm F9-Outfit-Galerie hat jetzt eine B-Taste als
Read-only-Umschaltung zwischen Inhalt und Head/Upper/Lower-Server-Bakes.
Beschriftung Server-Bakes [B] / Outfit [B] ist im echten Fenster sichtbar;
keine COF-/Avatar-/Server-Schreibaktion. C++23-Test prüft Zoom-Grenzen,
Nonfinite-Werte, Kameraposition und Widescreen-Click-Rects. NVIDIA
OpenGL3.3 Test rendert dasselbe GPU-Mesh aus 3m und 24m Distanz und
fordert real unterschiedliche Framebuffer-Pixel. Dies ist ein lokaler
GPU-Test mit synthetischem Mesh, kein echter Avatar-Login-Beweis.

## Block 194 – Vollständige Freigabeprüfung
Verbindlich auf Linux Mint CMake/Ninja Debug Full Build, gesamte nativen/
GL-/Python-Suite, Pi5 Headless-C++ Regressionen, syntaktische Quellchecks
und Vollviewer-Smoke ausschließlich unter Xvfb, NIE sichtbar auf dem
Desktop DISPLAY=:0. Danach GitHub-Commit/Push auf develop-current,
Remote SHA/clean worktree und erfolgreichen Selfhosted Pi5-CI verifizieren.
OpenGenesisLINK server main unverändert:
ec760cacfff2fe505cf51991985553ed434eb121;
reviewed_sha bleibt 4e99976630f1008db9014ea2b86c3723b09e9fe6, da
22 ältere Upstream-Commits nicht vollständig manuell geprüft wurden.

## Weiter offen
Echter NexVerse-Login mit Avatar/Welt-Framebuffernachweis sowie volle
Firestorm-11-Bake-TexLayer-Komposition und sichere COF Wear/Unwear-
Transaktion samt Readback/Rollback. Folgende Arbeit: Avatar-Orbit/
Kameramodi, direkte Pixelprovenienz nach autorisiertem Login, F9
serverbestätigter Outfit-Lifecycle und ältere OpenGenesisLINK-CAPS-
Patches einzeln prüfen. Kein Release-Claim ohne diese Prüfungen.
