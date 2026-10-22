# Alpha.755–764 – Authentisches Low158 COF-Multirecord-Skin-Replay verhindern

22.10.2026 | Linux Mint Commander jam-MS-7D28
Branch develop-current | OGL 0.2.0-alpha.764 | 39 % (38,65 % gewichtet)

Primäres Arbeitsgerät bleibt jam-MS-7D28, Checkout
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer.
Pi5 nur ergänzende GitHub ARM64 CI. Kein Produktivdeploy und
kein Start des interaktiven Desktops ohne ausdrücklichen Auftrag.

## Zehn konkrete Entwicklungsblöcke
755. Zuerst CHATUEBERGABE.md und projectplan.md auf Linux Mint
gelesen. Sauberer Git-HEAD db5252a1505a8e6c89f4d55d92d2f49561f9ef18
mit origin/develop-current abgeglichen. Letzter Pi5 GitHub CI
Run 39812188923 für Alpha.754 SHA 706b6117421c9b157c67f88058bda1c1903f4cc0
war beim Start IN_PROGRESS (nicht als SUCCESS behauptet).

756. OpenGenesisLINK-Server main
bcb528ac5c8ff4972da0f0435f322a1f386acd78
per GitHub Compare gegen 881b373 überprüft: ein Commit,
#272 Gruppen-SQL-Berechtigungstests. Kein neuer AvatarAppearance/
Low158/COF/Skin-Protokollvertrag aus dem Commit abgeleitet.

757. Reale OpenSim Low158-Verarbeitung geprüft:
decode_avatar_appearance akzeptiert bis zu 16
AppearanceData-Datensätze je authentischer Nachricht. Der
WorldModel-COF-Replay-Schutz sah aber ausschließlich
appearance_data.back() an. Trägt der letzte Datensatz die
sentinel COF-Version 0, ging eine davor liegende reale
veraltete COF-Version ungeprüft durch und konnte den
bisherigen authentifizierten Body-TextureEntry überschreiben.

758. C++23 WorldModel::apply_self_appearance an der echten
serverseitig eingespeisten Selbstavatar-Appearance-Stelle
geändert: rückwärts das letzte COF-versionsführende
AppearanceData-Element wählen; ein nachgestelltes null-
versioniertes Element ist keine Erlaubnis zur Rücknahme
des bereits akzeptierten Skin. Bestehende Highwater-Checks,
Agent-/Region-Gate und Versionsformat-Grenzen bleiben.

759. Offizielle OpenGenesisLINK-Legacy-Fälle weiterhin
explizit unterstützt: AppearanceData count=0 oder alle
COF-Versionen 0 liefern keine valide Vergleichsrevision.
Der bisher gespeicherte Highwater-Zähler wird dabei
nicht zurückgesetzt, sodass verspätete ältere echte
COF-Revisionspakete danach noch immer abgewiesen werden.

760. Native C++23 Model-Regressions in
tests/self_appearance_texture_integrity_tests.cpp:
ältere COF 899 + trailing 0 abweisen, neues COF 901 +
trailing 0 übernehmen, verspätetes COF 900 +
trailing 0 danach erneut abweisen, nur Version=0
zulassen aber Highwater unverändert behalten.
Bestehende Session- und Regions-Reset-Tests bleiben.

761. Den Provenienz-/Sicherheitsvertrag zusätzlich über
Python test_alpha755_764_cof_multirecord_gpu_contract.py
abgesichert, der exakt die echte Modelentscheidung
vor self_appearance_ = std::move(appearance) und
die native OpenGL-Regression kontrolliert. Keine
fiktiven Assets/Avatarsitzungen.

762. Tatsächlichen Firestorm-Body-FBO-Test
tests/avatar_body_framebuffer_pixels_tests.cpp
um veraltetes zweizeiliges AppearanceData ergänzt:
GL-Neuskin-Farbpixel dürfen nach altem COF 201 plus
trailing 0 NICHT wieder alte Texturen anzeigen;
reales neueres COF 203 plus trailing 0 MUSS
sichtbar andere Skin-Pixel zeichnen. Isolierter
GL-Test auf Linux Mint (Xvfb, original 3x .llm)
PASS, nicht bloß Text-/Sourcecode-Test.

763. Echten Linux Mint x64 Viewer und gezielte
WorldModel-/Skin-/OpenGL-Tests per CMake/vcpkg
erfolgreich gebaut. Vor Push komplette
scripts/mint-viewer-validation.sh-Prüfung
inklusive sämtlicher nativen Tests, Python
Source-Contracts, Xvfb GUI und X11-Taskleistenicon.

764. VERSION auf 0.2.0-alpha.764, CHATUEBERGABE.md,
projectplan.md, AGENTS.md, Projektfortschritt,
Firestorm/Server/UI-Integration und docs/PROJECT_PROGRESS.json
aktualisiert. Nach vollständigem Mint-Gate geprüften
Code nach GitHub develop-current committen/pushen;
exakte Pi5-/Hosted-x64-CI-Läufe getrennt dokumentieren.

## Statusgrenzen und folgende Phase
Dieses Update verhindert das GPU-Rendering bereits
zurückgenommener COF-/Skin-Versionen bei verspäteten
Low158-Datensätzen. Die Tests verwenden echte
WorldModel- und OpenGL-Renderpfade, aber kontrollierte
Testnachrichten. Ein authentifizierter NexVerse-Live-
Avatar mit bestätigtem COF-Outfit und Skin-Uploads,
tatsächlichen NVIDIA-Inworld-Pixeln und WASD-/Yaw-
Simulatorreadback ist noch still open.
Der gewichtete Projektfortschritt bleibt 39 %.
Nächster Abschnitt Alpha.765–774, auf Linux Mint.

## Abschluss des vollständigen Linux-Mint-x64-Gates
scripts/mint-viewer-validation.sh Exit 0: vollständiger
CMake/vcpkg-x64-Debug-Viewer, 175 native CTests (einschließlich
selbstavatar-Low158/COF und realem Original-Firestorm-
Head/Upper/Lower-GL-Pixeltest), 377 Python-Prüfungen,
isolierter Xvfb-Viewer 0.2.0-alpha.764, 133 original
Firestorm-Skelettgelenke, drei GPU-residente Original-
Körpermeshes und erfolgreiches OGL X11-Icon 128x128.
Keine echte Grid-Session oder Nutzeranmeldedaten verwendet.

## GitHub SHA und anschließend beobachtete CI
Code-SHA f2c53f302fa6c8dfcc00361590f14b6c19f510c4.
Neue GitHub ARM64 Pi5 Source Check Run 39814357748 queued
und GitHub Hosted Linux x64 Run 39814357774 queued,
Hosted-Job 119948820858: runner_id=null, steps=0
beim letzten Abruf. KEINEN CI-Erfolg vor Abschluss erfinden.
Vorheriger Pi5 Run 39812188923 zu Alpha.754 war
noch in_progress. Letzter SHA-bestätigter Pi5-SUCCESS
39809196307 Alpha.744 (cbdfd85).
Getrennte lokale Linux Mint x64 Full-GUI-Prüfung Exit 0:
175 native Tests, 377 Python, Xvfb Viewer und OGL X11-Icon.
