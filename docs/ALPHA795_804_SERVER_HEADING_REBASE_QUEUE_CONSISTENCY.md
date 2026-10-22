# Alpha.795–804 – Serverbestätigte Avatar-Ausrichtung und echte WASD-Queue

22.10.2026 | Linux Mint Commander jam-MS-7D28 | develop-current
Version 0.2.0-alpha.804 | gewichteter Stand 39 % (38,65 %)

## Zehn konkrete Entwicklungsblöcke

795. Vor Codeänderung auf Linux Mint Branch develop-current, GitHub-HEAD
8bc693f79ab1ff60b2a95788493728ff02b76e64 und sauberen Arbeitsbaum
geprüft; CHATUEBERGABE.md und projectplan.md zuerst gelesen. Raspberry Pi 5
bleibt ausschließlich ARM64-CI; bisherige GitHub Pi5 Source Check
Run 39827583351 für Feature-SHA 567d7a3 war completed/success.

796. Aktuellen OpenGenesisLINK Servermain 7396897d3d24ab8fac238ded1e2760d7b4544f13
geprüft: unverändert seit #275 (NexGroups Land API Key), keine neue
High4 AgentUpdate-/Low158-Avatar-Protokollversion behauptet.

797. Konkreten Avatar-Yaw-Folgefehler nach Alpha.794 festgestellt:
MovementController::reset(agent.rotation) wurde nur einmal beim Start
aufgerufen. Spätere serverseitige Avatar-Drehungen durch Teleport,
Simulatoraktionen oder Regionsänderungen verblieben im WorldModel,
während die nächsten High4 AgentUpdate-Body-Quaternionen weiterhin
aus einem veralteten lokalen yaw-Baseline gesendet wurden.

798. C++23 MovementController::adopt_server_heading eingeführt. Der
Controller übernimmt nach tatsächlichem Server-Readback im Leerlauf die
normierte Body-Yaw, ohne Fly-Toggle, Fly-Key-Debounce oder Bewegungstasten
zurückzusetzen. Die neue Ausrichtung wird ausschließlich als Client-
Steuerbaseline verwendet und ändert niemals das WorldModel.

799. Im echten GLFW-WASD-Hauptloop gilt: Bei aktiv gedrückter
Links-/Rechts-Drehtaste kein Rebase. Bei nur eben erfolgreich eingereihtem
TURN-Kommando wird der 350-ms-LocalHeadingPreview respektiert, um
unmittelbare lokale Kameradrehung nicht schon vor der Serverantwort
zurückzusetzen. Erst danach wird die echte Simulatorrotation erneut
als Ausgangspunkt nachfolgender AgentUpdate-Pakete verwendet.

800. Zweiten echten Fehler in demselben Hauptloop korrigiert: Die Variable
last_reported_control_flags (für Self-Avatar-Walk-Fallback) wurde
bereits vor outbound_queue.push auf die ungeprüft angeforderte Bewegung
gesetzt. Bei voller Queue konnten scheinbare Bewegung und Logs entstehen,
obwohl kein AgentUpdate angenommen wurde. Flags/Logs werden jetzt nur
nach erfolgreichem Queue-Push veröffentlicht.

801. Alle lokalen MovementController-Zustandsänderungen aus step() werden
bis zum erfolgreichen Queue-Push als spekulativ behandelt. Wird die Queue
mit zuverlässigen Nachrichten blockiert, setzt der Viewer den
Controller auf den Stand vor dem Versuch zurück: unsent yaw und
Fly-Toggle-Key-Edge werden nicht als gesendete Steuerung übernommen.
Tatsächliche Serverreadbacks bleiben unangetastet.

802. Native C++23-Regressionsprüfungen in tests/movement_controls_tests.cpp
und tests/outbound_queue_tests.cpp erweitert. Ein authentisch aus
Quaternionen aufgebauter High4 LLUDP AgentUpdate belegt die nachträgliche
90-Grad-Serverausrichtung im tatsächlichen Wire-Payload (114 Bytes), ohne
Fly-Zustand zu verlieren. Ein voller OutboundCommandQueue-Replaytest
bestätigt, dass verworfene TURN- und Flugumschaltungen keine lokalen
Steuerzustände hinterlassen. Beide CTest-Targets erfolgreich.

803. Quellintegration per Python-Regression auf exakte Reihenfolge
WASD-Key-Erfassung -> nicht mehr aktive Turn-Lease ->
serverautoritative Baseline -> spekulativer Controller-State ->
OutboundQueue-Push -> erst dann Flags/Preview/MovementFeedback, andernfalls
Rollback. Vollständiger Linux-Mint-x64-Release-Gate erforderlich:
CMake/vcpkg Debug Viewer, 176 native CTests, Python, isolierte Xvfb GUI,
Firestorm Original-Körpermeshes und echtes OGL-X11-Taskleistenicon.

804. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS.md,
Fortschritts-JSON und Firestorm-/Server-/UI-Paritätsmatrizen aktualisieren;
erst nach grünem Linux Mint Release-Gate Feature-Commit auf GitHub
`develop-current` pushen und SHA-exakte Pi5-/Hosted-x64-CI getrennt
festhalten. Kein Produktivdeployment.

## Abnahmegrenzen

Diese Regressionen beweisen echte native Controller- und LLUDP-Wire-
Konsistenz und erfolgreichen Linux-x64-Build, nicht einen authentifizierten
NexVerse-Avatar mit Low158/COF/Skin-Outfit-Texturen, serverbestätigter
WASD-Bewegung und sichtbarer NVIDIA-GPU-Inworld-Darstellung. Der echte
Live-End-to-End-Nachweis bleibt **still open**. Fortschritt 39 %.
Nächste zehn Blöcke: **Alpha.805–814** auf Linux Mint Commander.

## Vollständiger Mint-x64-Gate: überprüft

`scripts/mint-viewer-validation.sh` EXIT 0. Vollständiger
CMake/vcpkg Linux-x64-Debug-Viewer-Build, 176/176 native CTests
(inklusive echtem High4 LLUDP-Quaternion-Wiretest, voller Queue
und nicht gesendeter Fly-/Turnsteuerung), 392/392 Python-Verträge,
isolierter Xvfb Viewer Version 0.2.0-alpha.804, originale
Firestorm 133 Skeleton Joints und drei GPU-residente LLM
Bodymeshes mit 7186 Dreiecken; korrektes OGL-X11-WM_CLASS
und Taskleistenicon 128x128 PASS. Kein Live-Login/Deploy.

## GitHub-Code-SHA und CI-Belege
Code-SHA 3f6483b264d362e60c92f052faea5f9e28f84488. Pi5 CI 39829298694 CANCELLED, Run 39829308631 IN_PROGRESS bei letzter Pruefung; kein gruener CI-Lauf behauptet. Hosted Linux-x64 39829298689 und 39829308722 COMPLETED/FAILURE VOR RUNNERSTART (runner_id 0, 0 Build-Steps). Lokales Mint-x64 Full-GUI-Gate Exit 0. Letzter SHA-exakter Pi5 SUCCESS Run 39827583351 Alpha.794, SHA 567d7a3. Kein Live-Grid-Login.
