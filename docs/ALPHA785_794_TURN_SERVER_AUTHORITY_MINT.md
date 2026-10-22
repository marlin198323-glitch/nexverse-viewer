# Alpha.785–794 – Keine lokale WASD-Drehung als Serverbestätigung ausgeben

22.10.2026 | Linux Mint Commander jam-MS-7D28 | develop-current
OGL 0.2.0-alpha.794 | Projektfortschritt unverändert 39 % (38,65 % gewichtet)

## Tatsächliche zehn Arbeitsschritte
785. Linux-Mint-Checkout, GitHub SHA a789792bd77757dafaa712d26b7ebf66da970dbd,
Branch develop-current, sauberen Arbeitsbaum und CHATUEBERGABE.md
sowie projectplan.md vor Codeänderungen gelesen. Pi5-Source CI
Alpha.784 Run 39825721706 noch IN_PROGRESS, nicht als erfolgreich ausgegeben.

786. Aktuellen OpenGenesisLINK Servermain 7396897d3d24ab8fac238ded1e2760d7b4544f13
gegen vorherigen bd6bf4f verglichen: ein Commit #275
zu NexGroups-Land-API-Key-Setup, kein neues AgentUpdate/
Low158-/COF-Client-Protokoll implementiert.

787. ECHTEN Fehler im Linux-x64 OGL-Tastatureingabepfad
gefunden: src/app/application.cpp übertrug nach
MovementController::step noch VOR dem erfolgreichen
AgentUpdate-Queue-Push die LOKAL ANFORDERTE
body_rotation mit WorldModel::apply_agent in
die autoritative simulatorseitige AgentState-Rotation.
Damit waren lokale Drehung und serverbestätigte
Rotation nicht mehr zuverlässig getrennt.

788. Falsche lokale Autorität im echten WASD-Hauptloop
entfernt. camera_from_agent kann weiterhin den
angeforderten lokalen Kamerablick für das ausgehende
High4 AgentUpdate bauen. Aber die Serverrotation
im WorldModel wird ausschließlich durch echte
simulatorseitige Full/Compressed/ImprovedTerse
ObjectUpdate-Nachrichten geändert.

789. Neues MPL-2.0/C++23-Modul
include/nexverse/protocol/local_heading_preview.hpp:
kurzlebige lokale Renderrotation mit gebundener
Agent-ID, Session-ID, Zeitstempel, plausibler
Quaternionnorm und maximal 350 ms Altersfenster.
Kein Zugriff auf Serverdaten oder ungeschützte
Änderung an WorldModel.

790. Die lokale Vorschau wird NUR bei tatsächlich
erfolgreich in die OutboundCommandQueue eingestellter
Drehsteuerung aktiviert. Eine volle Queue,
stale Session, unplausible lokale Rotation oder
abgelaufenes Zeitfenster kann keine visuelle
Drehung als aktuelle Serverantwort vortäuschen.

791. Echter GPU-Self-Avatar und ThirdPersonCamera
verwenden in der Renderkopie optional nur die
zeitlich/sessiongeprüfte Vorschau; die
serververifizierte Modelrotation bleibt unverändert.
Die bestehenden drei Resetpfade bei Logout/Login/
Circuitstatus löschen das Preview-Objekt ebenfalls.

792. Native C++23 Regression
tests/local_heading_preview_tests.cpp erstellt
und in CMake sowie Pi5-Headless-Tests integriert:
echte MovementController::step-Quaternion,
visuelle ThirdPersonCamera-Differenz, gleichzeitig
unveränderte Serverrotation, Queue ohne Preview,
Agent-/Session-Gate, alter/Future-Zeitstempel,
ungültige Rotation, Sim-Verbindungsverlust.
Natives x64 Target erfolgreich gebaut und getestet.

793. Python-Integrationsvertrag
tests/test_alpha785_794_local_heading_authority_contract.py
prüft unmittelbar den realen WASD-Block:
kein WorldModel::apply_agent zwischen Control-Snapshot
und AgentUpdate-Takt; Preview erst nach erfolgreichem
Queue-Push, drei Resetpfade und echte Rendererbindung.
Vollständigen Linux Mint CMake/vcpkg-Debug-Build,
alle nativen Tests, Python, isolierten Xvfb-Start
und OGL-X11-Icon validieren.

794. VERSION, CHATUEBERGABE.md, projectplan.md,
AGENTS.md, info.md, docs/PROJECT_PROGRESS.json und
UI-/Firestorm-/Server-Paritätsmatrizen aktualisieren.
Erst nach grünem vollständigem Mint-Gate
auf GitHub develop-current pushen und SHA-genaue
CI getrennt dokumentieren. Kein Produktivdeployment.

## Aussagegrenze und nächster Schwerpunkt
Die Änderung verhindert nachweislich falsche
simulatorbestätigte Drehwinkel und erhält unmittelbare
lokale Kamerabewegung. Ohne autorisierte NexVerse-Login-
Sitzung kann kein tatsächliches WASD-/Yaw-Server-E2E,
eigene AvatarAppearance, COF/Outfit/Skin oder NVIDIA-
Live-Inworld-Framebuffer nachgewiesen werden.
Diese Prüfungen sind weiterhin still open.
Nächste zehn Blöcke: Alpha.795–804 auf Linux Mint.

## Vollständige lokale Linux-Mint-Releaseprüfung
scripts/mint-viewer-validation.sh EXIT 0:
vollständiger CMake/vcpkg x64 Debug Viewer-Build,
176/176 native CTests (einschließlich separatem
nexverse-local-heading-preview-tests),
388/388 Python-Tests, isolierter Xvfb-Viewer
0.2.0-alpha.794, 133 originale Firestorm-Joints,
3 original Body LLM GPU-Bindpose-Meshes/7186 Dreiecke,
OGL X11 WM_CLASS und System-/Taskleistenicon
128x128 erfolgreich. Keine echte Grid-Anmeldung,
kein serverbestätigter WASD-/Yaw-Live-Test.
