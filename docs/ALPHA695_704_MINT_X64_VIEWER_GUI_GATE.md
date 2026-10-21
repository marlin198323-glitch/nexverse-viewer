# Alpha.695–704 – Linux Mint Commander: realer Viewer-x64-Build und GUI-Provenienz

21.10.2026 | GitHub develop-current | Version 0.2.0-alpha.704 | 39 % (38,65 % gewichtet)

## Verbindlicher Arbeitsort ab dieser Übergabe
**Primäres Entwicklungssystem: Desktop Commander auf Linux Mint, Gerät jam-MS-7D28, GitHub-Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current.** PC besitzt CMake/Ninja/vcpkg und NVIDIA GeForce RTX 4070 SUPER, Treiber 615.71.09, OpenGL 4.6.
**Raspberry Pi 5 s359142.srv.nexvortex.de ausschließlich als ergänzender GitHub ARM64 CI-Testserver**, nicht mehr als Codearbeitsort, solange kein anderer Auftrag erteilt wird.
Neue Chats: zunächst CHATUEBERGABE.md und projektplan.md im Mint-Checkout lesen, git status/fetch/fast-forward, anschließend 10 echte Arbeitsblöcke. Alle Änderungen direkt in GitHub develop-current pushen. Kein Produktivdeploy, kein Start auf sichtbarem Nutzerdesktop ohne gesonderte Anforderung.

## Tatsächliche zehn Arbeitsblöcke
695. Mit Remote Desktop Commander das aktive Linux-Mint-Gerät jam-MS-7D28 ausgewählt, Betriebssystem, GPU, Toolchains, vcpkg, Display und Projektordner überprüft; NICHT auf Raspberry Pi gearbeitet.
696. Bisheriges sauberes lokales develop-current bei d669213 auf den aktuellen SHA 805fb3f per git fetch und FF-only synchronisiert; zuerst lokale CHATUEBERGABE.md gelesen, danach aktuelle Übergabe und Projektplan geprüft. Keine lokalen Nutzerdateien gelöscht.
697. CMake --preset debug mit der tatsächlichen Mint-vcpkg-Toolchain erfolgreich konfiguriert, installierte cached x64-linux Abhängigkeiten verwendet.
698. Gesamten tatsächlichen OpenGenesisLINK Viewer als Linux-x64 Desktop-Programm inklusive GUI, OpenGL, WebKit-Umgebung und LoginService gebaut; vollständiger Debug-CMake Build erfolgreich.
699. Alle vorhandenen **174/174 nativen CTest-Tests** auf dem Mint-PC erfolgreich, einschließlich der Avatar-/GL-Framebuffer-, Render-, LLUDP- und TOS-Parser-Tests; echte XMLRPC-TOS-Tests bestanden.
700. Isolierten Xvfb-Viewer-Start ausgeführt, ohne Benutzerdesktop zu berühren: Version/Logo, Firestorm-Original-Skelett mit 133 Gelenken, drei originale Körpermesh-Teile GPU-resident (7186 Dreiecke), UI Renderer initialisiert; keine authentifizierte Live-Grid-Behauptung.
701. Isolierten X11-Fenster-/Taskleisten-Integrationstest mit Window Manager ausgeführt: WM_CLASS opengenesislink-viewer und original OGL-Systemicon als X11-Icon 128x128 BESTANDEN; kein graues Zahnrad im getesteten X11-Window-Metadatenpfad.
702. ECHTE Linux-GCC -Wconversion-Warnungen in src/app/application.cpp bei keyframe-/serverselected-Skin-Draw-Diagnose entdeckt. uint64_t GL-Diagnose wurde bisher implizit auf uint32_t reduziert; bei großen Framezählern drohten Überlauf/false Zero Skin-Status. Zentrale saturierende GL-Draw-Count-Konvertierung und sichere 3er-Körpersummenfunktion implementiert, echte C++23-Regression für UINT64_MAX, 0, kleine Werte; vollständiger Viewer und Native-Test ohne Warnungen erneut gebaut.
703. Neuen kanonischen Linux-Mint-x64-Gate-Script scripts/mint-viewer-validation.sh implementiert: nur x86_64, CMake Debug Vollbuild, alle CTests, Python-Unittests, Xvfb GUI Smoke, X11/Taskbar Icon, Quellcodevertrag; kein Live-Login, kein Deployment, kein Start auf echtem Desktop. Full-Gate bereits im tests-only Modus PASS (vor finaler Versionierung 174 native Tests, 353 Python, GUI, Icon). Nach Versionierung erneut als Full-Gate zu prüfen.
704. VERSION, Projektplan, AGENTS, Handoff, technische Referenzmatrizen und JSON-Fortschritt aktualisiert, damit jeder nächste Chat sofort wieder Linux Mint Commander nutzt; GitHub-SHA und neue CI-Läufe nach Push separat dokumentieren. Voriger Pi5 GitHub CI Run 39650618129 für SHA 4150521 verifiziert SUCCESS; zuvor scheiterte gehostete Linux-x64-CI vor Runner-Vergabe – durch echte lokale Mint-Tests ersetzt, nicht als GitHub-hosted SUCCESS behauptet.

## Präzise Grenzen
Ein erfolgreicher Debug-Vollbuild, Xvfb GUI-Start, GPU-/Window-Readbacks und Unit-Framebuffer-Tests beweisen die eigentliche lokale Softwarekette. Die Xvfb-Routine emuliert einen Bildschirm und nutzt Software-GL; NVIDIA GL 4.6 wurde unabhängig mit glxinfo -B auf dem PC bestätigt, **aber nicht als echter eingeloggter NexVerse-NVIDIA-Framebuffer-End-to-End-Test behauptet**.
Authentifizierter eigener Avatar im NexVerse Live-Grid inklusive Low158, COF/Outfits, Skin/Bakes und WASD-/Yaw-Simulatorreadback bleibt OFFEN. Keine Geheimnisse, Passwörter oder Login-Tokens ins Repo geschrieben. Projektbewertung unverändert 38,65 % = 39 %. Nächster Zehnerblock **Alpha.705–714** auf Linux-Mint-Commander mit vollem GUI-Gate.

## Exakter Commit und Folge-CI
Vollständig lokal auf Linux Mint gegen Alpha.704 durchgelaufen:
scripts/mint-viewer-validation.sh Exit 0; 174 CTest, 353 Python,
isolierter GUI-Start, originale Firestorm 3x GPU-Bind-Meshes, 133
Skeleton-Joints, X11 OGL-Icon 128x128. Code-SHA
eceb5065ef886ce32255b4ac50c649d482ce84ba.
GitHub Pi5 CI Run 39652812099 IN_PROGRESS.
GitHub gehosteter Linux-x64 GUI Run 39652812111 FAILURE,
Job 119405263374 ohne Runner (runner_id 0, 0 steps): kein
CMake Build gestartet. Vollständiger GUI-Build auf dem
Linux Mint Desktop Commander PC dagegen tatsächlich PASS.
