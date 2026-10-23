# Alpha.915–924 – Low AgentMovementComplete aktiviert Circuit nur nach Authentifizierung

23.10.2026 | Linux Mint jam-MS-7D28 | Branch develop-current
OpenGenesisLINK Viewer 0.2.0-alpha.924 | Gewichteter Fortschritt 38,65 %, Bericht 39 %

## Zehn Entwicklungsblöcke
915. Sauberen HEAD 06ffa63 auf develop-current mit origin abgeglichen, CHATUEBERGABE.md und projectplan.md zuerst gelesen. Letzter Pi5 Alpha.914 Feature-SHA 55b719e Run 39989404270 ist inzwischen COMPLETED/SUCCESS.
916. OpenGenesisLINK Servermain ccfc644d aktuell geprüft: keine neuen AgentMovementComplete/AvatarAppearance/High4-Wireänderungen. Firestorm bleibt technische Grundlage.
917. ECHTEN LLUDP-Circuit-Auth-Fehler gefunden: CircuitConnection::handle_packet markierte bei bloßer msg_agent_movement_complete-Paket-ID den Circuit als active, ohne Payload, AgentID, SessionID oder Position zu prüfen. Daraufhin konnte der Low381-Outfit-Readback gestartet werden, obwohl WorldModel den korrupten oder fremden AgentMovementComplete korrekt zurückgewiesen hatte.
918. Neues reines MPL-2.0-Modul include/nexverse/protocol/movement_complete_admission.hpp: originale LLUDP-Payload decodieren, AgentID/SessionID der LoginSession verlangen, nichtleeren RegionHandle und endliche/beschränkte Simulatorposition voraussetzen. Außerdem muss zuvor der echte CompleteAgentMovement-Request gesendet worden sein. Vertikaler, aber gültiger LookAt bleibt position-only erlaubt.
919. ECHTEN CircuitConnection::handle_packet auf diese Prüfung umgestellt, BEVOR CircuitState::active gesetzt wird. Fehlerhafte/Fremd-Pakete werden bei reliable-Flagge weiterhin LLUDP-konform bestätigt, ändern aber nicht CircuitState und lösen keinen Low381-Wearables-Request aus.
920. CircuitStatus ergänzt um movement_complete_validated und bounded movement_complete_rejected. Keine AgentIDs, Sessions, Paketpayloads oder sensible Login-Felder in Telemetrie. Fehlerstatus beim ersten verworfenen Paket ohne Log-Spam.
921. F3-Avatar-Handshakestatus zeigt bei verworfenen Low15-Paketen die Anzahl Low15 abgewiesen: N. Aktiver Circuit, akzeptierte Simulator-Self-Pose und reine Paket-ID sind getrennt; keine gefälschte Skin-/COF-/WASD-Bestätigung.
922. Native C++23-Regression mit echter encode_packet/LLUDP-Decode-Pipeline prüft gültiges Self-AgentMovementComplete, fremde Agent-/Session-IDs, fehlende Anfrage, abgeschnittene Payload, RegionHandle=0, extrem große Position und NaN-LookAt. Vertikaler LookAt bleibt gültiger position-only-Empfang. In CMake/CTest Mint x64 integriert.
923. Den nativen Echtdaten-Test im Pi5 ARM64 Headless World+Protocol-Testloop registriert; Python-Quellvertrag sichert CircuitState-Reihenfolge vor Low381-Readback, F3-Diagnose, vollständigen Decoder, Mint/Pi5-Tests und nächste Version.
924. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS.md, info.md, Fortschritts-JSON und Firestorm-/Server-/Viewer-UI-Paritätsmatrizen aktualisiert; voller Mint-x64 CMake/vcpkg/C++/GL/Python/Xvfb-GUI/Icon-Release-Gate vor Feature-GitHub-Push. Exakte Pi5-CI und Hosted-x64-Provenienz dokumentieren; KEIN Produktivdeploy.

## Offene echte Integration
Vollständig authentifizierter NexVerse-Live-Eintritt mit sichtbaren Avatar-/COF-/Skin-/Outfit-GPU-Pixeln und simulatorbestätigter WASD-/Yaw-Bewegung bleibt still open. Projekt bleibt 39 %, nächste zehn Blöcke Alpha.925–934.

## Verifizierter vollständiger Mint-x64-Release-Gate
scripts/mint-viewer-validation.sh EXIT 0.
Vollständiger CMake/vcpkg Linux-x64 Debug OGL Viewer
0.2.0-alpha.924, 182/182 native CTests,
442/442 Python-Quell-/Integrationsverträge,
isolierter Xvfb-GUI-Start mit Original Firestorm
Skeleton 133 bind joints, drei GPU-residente
LLM-Körpermeshes/7186 Dreiecke sowie
OGL X11 WM_CLASS/Taskleistenicon 128x128 PASS.
Kein authentifizierter NexVerse Live-Grid-Login
und kein Produktivdeploy.

## Exakte GitHub Feature-SHA und CI-Prüfung
Code SHA 6be966b3cd090d6cb3315f612f4dc158bc7d4315.
Pi5 ARM64 Source Check Run 39991058245 IN_PROGRESS zuletzt;
vor completed/success keine Pi5-Behauptung.
Hosted GitHub x64 Run 39991058331 COMPLETED/FAILURE,
Job 120542614653 runner_id 0 und Schritte 0: kein Compiler.
Letzter erfolgreicher Pi5 Alpha.914 SHA 55b719e,
Run 39989404270. Lokales Mint-x64-Gate EXIT 0:
182 native CTests, 442 Python, Xvfb Original-Firestorm-
GPU-Bodymeshes und 128px X11-Systemicon.
Keine authentifizierte NexVerse-Live-Sitzung/kein Deploy.
