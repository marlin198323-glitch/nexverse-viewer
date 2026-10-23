# Alpha.865-874 - Simulator-Socket-Wechsel bei gleichem Regionshandle

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current
OGL Viewer 0.2.0-alpha.874 | Projekt 39 % (38.65 % gewichtet)

## Zehn Entwicklungsblöcke
865. Linux-Mint-Checkout develop-current bei HEAD 4459503 sauber; CHATUEBERGABE.md und projectplan.md zuerst gelesen. Voriger Pi5 Alpha.864 Source Check Run 39977723209 fuer Code-SHA 66e0211 inzwischen COMPLETED/SUCCESS.
866. OpenGenesisLINK Servermain 08f95e7 geprüft. Seit 08f95e7 keine neue Avatar-/COF-/WASD-Wireänderung; aktuell letzter Commit #285 ist Voice/WebRTC Deployment.
867. Ursache gefunden: WorldModel::apply_region betrachtete einen Wechsel der Simulator-IP oder UDP-Port bei gleichbleibendem RegionHandle als einfache Auffrischung. Alte Avatarobjekte, LocalIDs, Terrain, Pose/COF-Outfit und Skin konnten aus der alten Simulatorinstanz erhalten bleiben.
868. Neuer simulator_socket_changed-Vertrag: Nur bei derselben bekannten Region UND beidseitig bekannten IP/Port-Daten werden IP-/Portabweichungen als echte neue Region-Epoch behandelt.
869. Neuen Socketwechsel in den bestehenden leave_region-Reset integriert. Damit werden Avatar-/Objektlisten, Terrain-/Parzellen-/Umgebungspuffer, Region-LocalIDs und die Revisionen sauber invalidiert; die Region-Generation steigt.
870. WorldModel widerruft bei Socketwechsel die serverbestaetigte Self-Avatar-Pose, Low158/COF-/Low382-/Skin-/Objekt-Bake-Quellen, Animationen und fruehe FastRez-Antworten. Der alte Original-Firestorm-Koerper darf ohne neue echte Simulatorposition nicht mehr erscheinen.
871. Unvollstaendige same-region Handshake-/AgentMovementComplete-RegionUpdates mit leerer IP oder Port=0 lassen die letzte wirklich bekannte Simulator-IP/Port bestehen. Dies verhindert auch eine Luecke, bei der zuerst eine leere Zwischenmeldung die spaetere Endpunktaenderung verschleiern koennte.
872. Native C++ WorldModel-Regression im realen early_avatar_readback_tests: same socket/handle refresh bewahrt bestaetigten Avatar und Skin; same handle mit Port 9001 oder geaenderter IP loescht Szene, Skin, Wearables und Pose. Erst eine neue gueltige Simulatorposition und neue Low158 duerfen den Koerper wieder zurueckbringen.
873. Extra native Regression: leeres Zwischen-Handshake-IP-/Portfeld behaelt bekannten Socket, bestaetigte Pose und Stage-Quelle. Python Realsource-Integration sichert leave_region, Endpoint-Normalisierung und Pi5-Nativtest-Einbindung; kompletter Mint-x64-CMake/CTest/Python/Xvfb-GUI-Gate.
874. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS.md, Fortschritt-JSON, Firestorm-/Server-/UI-Matrizen aktualisieren. GitHub-Feature-Push NUR nach gruenem lokalem Gate; danach exakte Pi5/Hosted-CI-SHA getrennt dokumentieren.

## Weiter offene E2E-Abnahme
Der Reset verhindert veraltete Darstellung und macht einen Reconnect korrekt. Er ist kein authentifiziertes NexVerse-Live-Grid-Login-Ergebnis; sichtbar gerenderter Avatar mit aktuellen COF/Outfit-/Skin-Daten und simulatorbestaetigte WASD-Bewegung bleiben OFFEN. Projekt 39 %, naechste Alpha.875-884. Kein Deployment.

## Linux Mint x64 Release Gate
scripts/mint-viewer-validation.sh EXIT 0.
Full CMake/vcpkg Linux-x64 Debug OGL Viewer 0.2.0-alpha.874,
178/178 native CTests, 420/420 Python contracts,
isolated Xvfb GUI with original Firestorm 133 bind joints,
three original GPU LLM Bodymeshes/7186 triangles,
X11 WM_CLASS+128x128 OGL system taskbar icon PASS.
No authenticated NexVerse Live-Grid session or deployment.

## Exakte Github SHA und CI
Code SHA 6eabf2f9d93dbb6188dcfdc60d739ef309bfd965.
Pi5 Source Check 39979417843 IN_PROGRESS zuletzt;
nicht als Erfolg markieren ohne completed/success.
Hosted Linux x64 39979417903 COMPLETED/FAILURE
Job 120502450767 runner_id 0 / steps 0,
kein Compiler gestartet. Letzter Pi5-SUCCESS
Alpha.864 SHA 66e0211 Run 39977723209.
Mint x64 Full-Gate gruen: 178 native, 420 Python,
Xvfb GUI, Original-Firestorm Bodymeshes + OGL Icon.
Kein Produktivdeploy.
