# Alpha.725–734 – Sichere echte AgentUpdate-/WASD-/Yaw-Pakete auf Linux Mint

21.10.2026 | develop-current | OGL 0.2.0-alpha.734
Gesamtfortschritt weiterhin 39 % (38,65 % gewichtet), weil der
authentifizierte NexVerse Avatar-/COF-/WASD-/GPU-Ende-zu-Ende-Beleg fehlt.

## Zehn konkrete Arbeitsblöcke

725. Linux Mint Desktop Commander jam-MS-7D28, Arbeitsbaum
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer; Branch
develop-current clean bei HEAD 86a8aa0 und identisch zu Origin.
CHATUEBERGABE.md und projectplan.md zuerst gelesen. GitHub Pi5
Alpha.724 Run 39660444288 für Feature-SHA 09d1c0c SUCCESS.
Raspberry Pi bleibt nur ergänzender ARM64-CI-Testserver.

726. Aktuellen OpenGenesisLINK Servermain 86f293d948390b2546a92f848eef5ee17ccdd626
mit 31f3d129 verglichen: genau ein Commit #264, Änderungen in
bin/Robust.HG.ini und tools/ci/verify_nexdiscovery.py,
jetzt SearchURL https://stadt-nexverse.de/service/search,
DestinationGuide https://stadt-nexverse.de/service/destination,
GridInfo welcome https://stadt-nexverse.de/service/welcome.
OGL welcome_page.hpp nutzt letztere Adresse schon.
Der ältere ungeprüfte Server-Commitrückstand bleibt MANUAL_REVIEW.

727. Die öffentliche, nicht authentifizierte GridInfo auf Linux Mint
per HTTP GET unter http://hg.stadt-nexverse.de/get_grid_info
geprüft: HTTP 200, aktuelle XML-Felder login http://hg.stadt-nexverse.de:80/,
gridname Stadt NexVerse, welcome https://stadt-nexverse.de/service/welcome.
Keine Avatar-Anmeldedaten versandt. Eine authentifizierte
Region/Avatar-Sitzung lässt sich dadurch nicht belegen.

728. ECHTEN OpenSim High4 AgentUpdate Bewegungsfehler analysiert:
MovementController::step(float seconds) verwendete std::clamp
auf NaN. Das lieferte NaN; beim Halten der Drehpfeiltaste
gelangte NaN in Quaternion sin/cos und in das REAL verschickte
AgentUpdate (12-byte LLQuaternion).

729. Ein zentrales MPL-2.0-Header-Modul movement_frame_safety.hpp
implementiert: nonfinite/negative Zeitdeltas werden als 0s
behandelt, positive Lag-Deltas auf 0,25s begrenzt. Echte
MovementController::step nutzt dies vor jeder yaw-Änderung.
Steuerbits/WASD-Strafing/Fly-Control bleiben kompatibel.

730. Auch reset(Quaternion) aus echter Simulatorrotation war
anfällig für Float-Overflow oder NaN. Neue
valid_initial_agent_yaw normalisiert gültige Quaternionen
in double; Zero/Nan/Infinity-Fälle verwenden nur lokal
sicheren yaw=0 statt verfälschte LLUDP-Quaternionen.
Real-world Kopf-/Avatar-Rotation ohne erfundene Serverreaktion.

731. Einen zweiten echten Sendewegfehler im Anwendungscode
abgefangen: camera_from_agent(agent.position) kopierte
ungültige WorldModel AgentPosition ohne Prüfung in den
AgentUpdate CameraCenter. Der Haupt-Inputpfad verweigert
deshalb bei nichtfiniten/unplausiblen Simulatorpositionen
das Verschicken, meldet einmalig per Log und setzt den
Bewegungsfeedbackmonitor zurück. Keine erdachte
Ersatzposition oder künstliche bestätigte Bewegung.
Sobald die Serverposition valide wird, kann die Steuerung
erneut AgentUpdate senden.

732. Native C++23 movement_controls_tests.cpp erweitert:
NaN/±Inf/negative frame delta, 3600s Lag-Cap, Zero/NaN/Inf-
Serverquaternion, riesige finite Quaternionen, unplausible
Positionen, WASD-/Fly-Yaw-ControlFlags und 114 Byte reales
AgentUpdate LLUDP-Payload. Die tatsächliche, serialisierte
12-Byte LLQuaternion-Z-Komponente wird per bit_cast an
Wire-Offset 40 dekodiert. Nie NaN/Inf an den Simulator.

733. Echten Linux Mint x64 Viewer, native Bewegungstests und
Python-Quellverträge gebaut/ausgeführt. Vor Release
scripts/mint-viewer-validation.sh mit full CMake debug,
gesamtem CTest, Python, isoliertem Xvfb Login-UI
und echtem OGL X11 Icon durchführen. Dieser Test ist
keine signierte/authentifizierte NexVerse-Live-Sitzung.

734. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS,
PROJEKTFORTSCHRITT, Viewer-UI-/Server-/Firestorm-Matrizen
und JSON-Projektstatus aktualisiert. GitHub Code-SHA exakt
nach erfolgreich durchgelaufener Mint-Validierung übertragen,
GitHub Pi5 CI sowie gehosteten x64 Runner-Sonderfall
getrennt dokumentieren. Kein Produktionsdeploy.

## Offene P0-End-to-End-Prüfungen
Nachweisbar sind echte LLUDP High4 Encoder-Wirebytes und der
kompilierte Linux-x64 Viewer, nicht die Reaktion eines real
authentifizierten NexVerse-Avatars. Serverbestätigte WASD/
Yaw-Bewegung, Low158 AvatarAppearance des eigenen Avatars,
COF-/Outfit-/Skin-Bake-Texturen in OpenGL und echter
NVIDIA-Desktop-GPU-Pixel-Readback müssen separat mit
autorisierter Benutzersitzung überprüft werden. Bis dahin
39 Prozent Projektfortschritt unverändert.

Nächster Zehnerblock: Alpha.735–744 auf Linux Mint Commander.

## Exakter Feature-Code-Commit und CI-Provenienz
Feature-SHA a31201f2490e40041fa08fff6bc1c7fb8c725129.
Direkt auf Linux Mint Commander ausgeführt:
scripts/mint-viewer-validation.sh Exit 0, 175 native CTests,
365 Python-Verträge, isolierter Xvfb-Viewer Version
0.2.0-alpha.734, originales Firestorm Bind-Skeleton/Bodymesh
und OGL-Taskleistenicon 128x128. GitHub Pi5 Run 39662405115
für genau diesen SHA IN_PROGRESS bei Übergabe.
Gehosteter x64 CI Run 39662405234 FAILURE vor Runnerzuteilung,
Job 119438565129, runner_id 0, Steps 0; kein Build ausgeführt.
Letzter bestätigter erfolgreicher SHA-GitHub-Pi5-Run:
39660444288 zur Alpha.724, SHA 09d1c0c.
Keine authentifizierte NexVerse-Inworld-Avatar-/Skin-/WASD-Abnahme.
