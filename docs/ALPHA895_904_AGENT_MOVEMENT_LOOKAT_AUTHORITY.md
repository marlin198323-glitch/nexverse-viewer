# Alpha.895–904 – AgentMovementComplete LookAt statt vorläufiger BodyRotation

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current
OGL Viewer 0.2.0-alpha.904 | Projekt 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke
895. GitHub develop-current HEAD 471e88e, CHATUEBERGABE.md und projektplan.md vor Änderungen gelesen. Pi5 Alpha.894 Run 39984523949 beim letzten Abruf IN_PROGRESS, nicht als erfolgreich behaupten.
896. OpenGenesisLINK Servermain ccfc644d nach #286 OGLVoice Go Bootstrap geprüft. Kein geändertes AvatarAppearance/High4-/MovementComplete-Wireformat.
897. ECHTEN BodyRotation-Autoritätsfehler gefunden: AgentMovementComplete hat echte Position und LookAt-Vec3, KEINE Quaternion. Bisher wurde vorläufige Login-Agent.rotation trotz ungültigem LookAt als echte Simulatorrotation bestätigt, womit WASD/Yaw fälschlich freigegeben werden konnte.
898. Neues MPL-2.0-Modul include/nexverse/protocol/agent_movement_look_at.hpp konvertiert echte endliche horizontale Server-LookAt-Richtung mit atan2(y,x) in Quaternion-Z-Yaw. Null, vertikal, NaN, Infinity und fast null werden zurückgewiesen.
899. WorldModel::accept_simulator_self_position prüft Agent, Session, Region und endliche Simulatorposition ohne dadurch eine nicht gelieferte BodyRotation oder High4-Freigabe zu erfinden. Bereits früher unabhängig bestätigte aktuelle BodyQuat bleibt erhalten.
900. Echten AgentMovementComplete LLUDP-Handler auf LookAt/Yaw-Quaternion bei gültiger Blickrichtung und ansonsten strikt position-only umgestellt. Keine Änderungen am Firestorm-Wireformat oder Server-API, keine Client-COF-/Skin-Uploads.
901. Die bestehende WorldModel/High4/AgentUpdate-Freigabe aus Alpha.894 verlangt weiterhin aktuelle vollständige bestätigte eigene Position UND Rotation; ohne LookAt beziehungsweise spätere echte Full/Compressed/Terse-Rotation keine vorgetäuschte WASD- oder Yaw-Success-Meldung.
902. Native C++23-Regression prüft Ost/Nord/West, Null-/vertikale/NaN/Infinity-LookAt, fremde Agent/Session/Region, NaN-Position, position-only vor erster Rotation, legitimen Server-Yaw plus WASD-ControlFlag, späteres Positionsupdate ohne Verlust gültiger BodyQuat, Reconnect und Logout.
903. Native CMake Mint x64 und Pi5 ARM64-Tests eingebunden; Python Realpath-Regression sichert echten Low-Level Decoder, LLUDP-Handler, Model-Provenienz und High4/WASD-Gate. Vollständiger Mint x64 CMake/vcpkg-Build, CTests, Python und isoliertes Xvfb-GUI/Icon vor GitHub-Push.
904. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS/info, Fortschritt-JSON und Firestorm-/Server-/UI-Paritätsmatrizen auf Alpha.904 aktualisiert; nach grünem Gate Code-SHA und getrennte Pi5-/Hosted-CI dokumentieren.

## Noch offene reale End-to-End-Abnahme
Authentifizierter NexVerse-Inworld-Avatar inklusive tatsächlicher Skin-/Outfit-/COF-GPU-Pixel und serverbestätigter WASD/Yaw-Live-Sitzung weiterhin still open. Es wurde kein echter Login oder Produktionsdeploy ausgeführt. Projekt 39 %, nächste Alpha.905–914.

## Vollständige Linux-Mint-x64-Freigabe
scripts/mint-viewer-validation.sh EXIT 0:
CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.904,
180/180 native CTests einschließlich neuer
nexverse-self-agent-movement-look-at-tests,
432/432 Python-Verträge, isolierter Xvfb-GUI-Start
mit Original Firestorm Skeleton 133 Joints,
drei GPU-residenten Original-Bodymeshes und 7186 Dreiecken,
OGL X11 WM_CLASS/Systemicon 128x128 PASS.
Kein echter NexVerse-Login, kein Produktivdeploy.

## Exakte GitHub SHA / getrennte CI-Ergebnisse
Feature-Code-SHA 90bde599854eb805eec66ebe00f10c57d1b12594.
Pi5 Source Check Run 39986215063 IN_PROGRESS beim letzten Abruf;
nicht grün ohne abgeschlossenen SHA-genauen Test.
Hosted Linux-x64 Run 39986215086 COMPLETED/FAILURE,
Job 120525941955, runner_id=0, steps=0:
kein Compilerprozess gestartet. Voriger Pi5 Alpha.894
Feature-SHA 5247d3f Run 39984523949 SUCCESS.
Lokales Mint-x64-Gate grün mit 180 native CTests,
432 Python, Xvfb Original-Firestorm-Körper und X11-Icon.
Keine Live-Grid-Sitzung/kein Deployment.
