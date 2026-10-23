# Alpha.935–944 – LLUDP Low15 muss zur tatsächlichen Login-Region gehören

23.10.2026 | Linux Mint jam-MS-7D28 | develop-current
Viewer 0.2.0-alpha.944 | 39 % Projekt (gewichtet 38,65 %)

## Zehn Entwicklungsblöcke
935. GitHub HEAD b5f8da7 sauber und gleich origin; zuerst CHATUEBERGABE.md und projectplan.md gelesen; voriger Pi5 Alpha.934 Code f9d8a75 Run 39992793374 COMPLETED/SUCCESS.
936. OpenGenesisLINK Servermain ccfc644d erneut geprüft, keine neuen LLUDP Avatar-/COF-/WASD-Protokolländerungen.
937. ECHTEN Low15 Identity Gap in der kürzlich gesicherten CircuitConnection gefunden: valid_circuit_movement_complete prüfte nur RegionHandle != 0, nicht Übereinstimmung mit authentifizierter LoginSession-Region. WorldModel verweigerte die fremde Region aber bereits. Dies konnte CircuitState::active und Low381 trotz fehlender eigener gültiger Weltposition auslösen.
938. valid_circuit_movement_complete prüft jetzt den aus LoginSession.region_x/region_y gebildeten vollen 64-bit RegionHandle mittels (uint64(region_x)<<32)|uint64(region_y), so wie die WorldModel-Loginanlage im produktiven Application-Pfad.
939. Unvollständige Legacy-Logins mit beiden Regionkoordinaten 0 erhalten Kompatibilität und delegieren die Regionsprüfung weiterhin an WorldModel. Ist nur EINE Koordinate 0, bleibt die andere eine echte Erwartung; kein versehentliches Freischalten fremder Regionsdaten am Weltrand.
940. Nach wie vor müssen decodierte Paketform, richtige AgentID/SessionID, gültige endliche Position, nichtleerer RegionHandle und zuvor gesendetes CompleteAgentMovement für CircuitState::active vorliegen. Ohne Gleichheit wird weder aktiv geschaltet noch initialer Low381-WearablesRequest gesendet. Transportreliable ACK bleibt unabhängig.
941. Separater datensparsamer movement_complete_wrong_login_region Klassifizierer für decodierte Low15, bei der Agent und Session passen, die volle Region aber nicht. Im CircuitStatus eigener bis 9999 begrenzter Zähler, ohne raw UUID/Token/Endpunkt.
942. F3-Avatar-Handshakestatus ergänzt Low15 falsche Login-Region: N, getrennt vom bisherigen generischen Low15 abgewiesen: N. Keine gefälschte Avatar-GPU/COF/WASD-Freigabe.
943. Native echte LLUDP-encode/decode-Regression erweitert: gültiger exakter Loginhandle 256/512, fremder nichtleerer Handle, abweichende Loginregion, x=0 Randregion, Legacy beide Koordinaten 0; CMake Mint x64 und Pi5 ARM64 Headless. Python echter Quellpfad Vertrag für Circuit-WorldModel-Übereinstimmung und F3.
944. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Fortschritts-JSON sowie Server-/Firestorm-/UI-Matrizen aktualisieren, kompletten Mint-x64 CMake/CTest/Python/Xvfb-GUI/Icon-Gate testen, geprüften Code nach GitHub pushen und CI getrennt SHA-genau dokumentieren.

## Live-Abnahme
Echt authentifizierter NexVerse Avatar mit COF, Outfit, Skin-GPU und serverbestätigtem WASD/Yaw weiterhin still open. Kein Produktivdeploy, Projekt 39 %, nächste zehn Blöcke Alpha.945–954.

## Voller Linux-Mint-x64-Release-Gate
scripts/mint-viewer-validation.sh EXIT 0:
0.2.0-alpha.944 vollständiger CMake/vcpkg-Linux-x64-Build,
183/183 C++-/OpenGL-CTests, 452/452 Python-Kontrakte,
isolierter Xvfb-Start Original-Firestorm-Skelett 133 Joints,
drei Original-Bodymeshes GPU-resident, 7186 Dreiecke,
OGL X11 WM_CLASS und Systemicon 128x128 PASS.
Kein authentifizierter Grid-Live-Login, kein Deployment.

## Exakte GitHub Code-SHA und CI
Feature-Code-SHA 7a97831bd9ef52f5a001cd174d70ffba006c0e30.
Pi5 ARM64 Source Check Run 39994940156 IN_PROGRESS beim letzten
Abruf, kein Erfolgsstatus vor Abschluss. Hosted GitHub x64
Run 39994940090 COMPLETED/FAILURE,
Job 120555900346 runner_id=0, steps=0:
kein zugeteilter Runner oder Compiler.
Vorheriger Pi5 Erfolg Alpha.934 SHA f9d8a75
Run 39992793374.
Separat lokaler Linux Mint voller x64-Gate EXIT 0:
183 CTests, 452 Python, Xvfb Firestorm Original-LLM GPU
und 128px OGL X11-Systemicon. Kein Live-Grid-Login.
