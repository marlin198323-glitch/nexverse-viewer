# Alpha.160–164 – Sicherer Desktop-Testmodus, Avatar GL-Anzeige und Integration

Stand: 16.10.2026, OGL Viewer 0.2.0-alpha.164.
Primärer Entwicklungsrechner Linux Mint jam-MS-7D28 (jam),
Remote Desktop Commander; Repository marlin198323-glitch/nexverse-viewer,
Branch develop-current. Basis 6cc14982def7a3c8507e4a836c717aeb6de5eb82.
Raspberry Pi 5 bleibt ausschließlich ARM64 Self-hosted GitHub CI.

## 160 – Unerwartete sichtbare Viewer-Teststarts untersucht
User berichtet, bei Arbeiten auf Linux-PC sei der Viewer immer wieder
testweise gestartet worden. GUI-Quelltests setzen GLFW_VISIBLE auf
GLFW_FALSE; native OpenGL-GPU-CTests sind kein dauerhafter Viewerstart.
Vollständige Viewer-Prozess-Starts waren bisher temporäre GUI-
Smoke-Tests. Diese dürfen das interaktive Desktop-DISPLAY=:0 nicht
benutzen. Ein mit dem Desktop sichtbarer Start findet künftig
ausschließlich auf ausdrückliche Nutzeranweisung statt.

## 161 – Eigenes hart isoliertes GUI-Smoke-Skript
scripts/mint-isolated-gui-smoke.sh startet den tatsächlichen Viewer
AUSSCHLIESSLICH unter xvfb-run auf frischem virtuellem
1600x900-X11-Display. Ohne xvfb-run, timeout oder ausführbaren
Viewer ist Abbruch, kein Fallback auf DISPLAY des Nutzers.
Wayland-Display und Desktop SessionBus werden ausgeblendet;
XDG-Konfigurations-, Cache- und Datenordner sind temporär.
HOME wird ausschließlich zur lokalen Firestorm-Asseterkennung
beibehalten. Timeout stoppt den gestarteten Viewer nach acht
Sekunden; exit=124 ist nur zusammen mit bekannten Startup-
Logmarkern und ohne Fehler/Fallback zulässig. Temporäre Testdaten
und Logs werden anschließend gelöscht.
tests/test_mint_isolated_gui_smoke.py prüft harte
Display-Isolation per synthetischem Fake-Viewer und externem
DISPLAY-Marker auch bei vererbtem DISPLAY=:776.
Auf ARM64 ohne Xvfb wird nur dieser grafische Test übersprungen,
statische Isolation-Vertragsprüfung bleibt aktiv.

## 162 – Wahrheitsgemäße Avatar-Statusanzeige in Inworld-UI
Neue reine C++23-Hilfe avatar_gpu_submission_status.hpp verwendet
echte RenderDiagnostics des aktiven GPU-Frames, nicht den Status
bereits hochgeladener Firestorm-Assets:
- kein Body-Draw
- provisorische Ersatzgeometrie (ohne originales LLM)
- Original LLM-Draws 0/3 bis 3/3
- Head/Upper/Lower Bake-G/- (G = GL-Draw Submit).
Systemstatus-Karte im Inworld-HUD zeigt diese Zeile parallel zu
Renderer, RAM und VRAM. Die Karte wurde um 20 Layoutpunkte erhöht,
die Statuswerte entsprechend verschoben. Kein Anspruch auf echte
Framebuffer-Pixelparität oder visuell sichtbaren authentifizierten
Avatar ohne Inworld-Sitzung.
Neuer nativer C++23-Test prüft alle Anzeigestatus und Cap >3.

## 163 – Unabhängige SelfAvatar-Auswahl für MIP und F9
Bisher wurde in der MIP0-Qualitätspräferenz und F9-GPU-Vorschau
avatar_proxy_update.front().is_self als unausgesprochene
Datenreihenfolge vorausgesetzt. Beide Pfade wählen nun den
SelfAvatar mit find_if(avatar.is_self), auch wenn andere Avatare
vor ihm in der Liste stehen. Unverändert: gleiche Login-/Renderer-
AgentUUID, gültige Simulation und RegionLease/Epoch.
Statische Regression prüft, dass weder F9 noch MIP0 den ersten
Eintrag voraussetzen. Keine zusätzlichen Simulator-Requests.

## 164 – Native Mint-Builds, CI und Upstream
Version alpha.164 auf Linux Mint vollständig mit CMake/Ninja/vcpkg
gebaut. Echte RTX4070-SUPER-OpenGL-Tests plus gesamte Suite:
114/114 CTest PASS und 38/38 Python PASS.
Neues Shell-Skript mit echtem Viewer unter Xvfb ausgeführt:
original Firestorm 133 Joints, 55 Attachmentplätze und 7.186
Dreiecke, UI init PASS. Test-Desktop bleibt DISPLAY=:0,
Viewer benutzte ausschließlich virtuelles Xvfb-Display.
Pi5 prüft weiterhin nur headless Tests nach GitHub-Push.
Upstream opengenesislink/opengenesislinkserver main
ec760cacfff2fe505cf51991985553ed434eb121, unverändert.
Die separate Server-JPEG2000/OpenJPEG-Klassifizierung bleibt,
22 ältere Commits weiter unvollständig fachlich geprüft;
reviewed_sha 4e99976630f1008db9014ea2b86c3723b09e9fe6
nicht freigeben/ändern. Compiler-Build meldete noch
23 Warnungen in vorhandenen Testfixture-Dateien;
kein claim eines warning-freien Release.

## Offen, nächste fünf Blöcke
1. Echten OpenGenesisLINK Viewer nur auf ausdrücklichen Nutzerbefehl
   sichtbar öffnen; ansonsten Xvfb für GUI/Startup, versteckte
   GL-Kontexte für GPU-Regression verwenden.
2. Nach autorisiertem Login echte EventQueue/WorldModel/Avatar-
   Mesh-/Texture-Sichtbarkeit auf dem Inworld HUD diagnostizieren.
3. F9 Outfit/COF/Head-Upper-Lower Live Grid-Provenienz prüfen,
   inklusive geordneter Epoch-/Texture-Invalidierung.
4. Firestorm-kompatible Wear/Unwear-Protokolle nur mit
   ausdrücklichem Consent, Server-COF-Readback, Idempotenz/Rollback.
5. Ältere OpenGenesisLINK Server-Patches weiter bewerten;
   nicht allein wegen grünem GUI/GPU Smoke für komplett erklärt.
