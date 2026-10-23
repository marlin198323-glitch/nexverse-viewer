# Alpha.945–954 – SSA-ACK auch vor Inworld bei frischem RegionHandshake erhalten

2026-10-23 | Linux Mint jam-MS-7D28 | Branch develop-current | OGL 0.2.0-alpha.954

## Zehn Entwicklungsblöcke

945. Sauberen develop-current HEAD e498ed5 gegen origin geprüft; CHATUEBERGABE.md und projectplan.md gelesen. Pi5 Alpha.944 exakte Code-SHA 7a97831 Run 39994940156 COMPLETED/SUCCESS.
946. OpenGenesisLINK Servermain ccfc644d geprüft: keine neuen SSA-/RegionHandshake-/AvatarAppearance-Wireänderungen.
947. ECHTEN Pre-Inworld-SSA-Race gefunden: bei einer weiteren NEUEN, aber gültigen RegionHandshake-Sequenz während awaiting_region_ready löschte CircuitConnection trotz bereits zuverlässig bestätigter SSA-bit2-Antwort den SelfAppearanceHandshakeReceipt-Beweis samt Timestamp. Alpha.934 schützte nur CircuitState::active.
948. Neue MPL-2.0-Methode SelfAppearanceHandshakeReceipt::note_refresh: Bei nachweislich acknowledged SSA bleibt die bestehende zuverlässige Reply-Sequenz erhalten, während ausstehende unACKed Verhandlungen auf die neueste zuverlässige Sequenz gebunden werden.
949. Produktiven CircuitConnection::handle_packet für beide Phasen – vor und nach active – auf denselben kontrollierten Receipt-Refresh umgestellt. Bereits bestätigte SSA-Antwort und Bestätigungszeitpunkt bleiben bei allen neuen Handshakes desselben Login-Circuits erhalten.
950. Während Pre-Inworld bleibt CircuitState weiterhin awaiting_region_ready, bis die authentische AgentMovementComplete vorliegt. Keine vorzeitige Welt-/WASD-Freigabe, kein vorgetäuschter Low158-, COF- oder Skin-Erfolg.
951. Neu-Login setzt SelfAppearanceHandshakeReceipt und Handshake-Replay-Historie weiterhin explizit zurück; eine fremde neue Sitzung kann keine bestätigte alte SSA-Bit2-Freigabe erben.
952. CircuitStatus um einen auf 9999 begrenzten Zähler bestätigter SSA-ACK beim Pre-Inworld-Refresh ergänzt; F3 zeigt SSA ACK vor Inworld erhalten, ohne Agent-, Session-, Token- oder IP-Daten.
953. Bestehende Native-Mint-/Pi5 C++23 SSA-ACK-Tests um Pre-Inworld vorher bestätigt, Refresh und noch ausstehende ACK, veraltete ACK, Neusitzung-Reset und Out-of-Order RegionHandshake ergänzt. Python-Realpath prüft echten Circuit-Handler, ACK-Timestamp und F3.
954. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Fortschritts-JSON sowie Server-/Firestorm-/UI-Matrizen auf Alpha.954 aktualisiert. Voller Linux Mint x64 CMake/vcpkg/CTest/Python/Xvfb-GUI/Icon-Gate und SHA-genauer GitHub-Feature-/Dokumentations-Push, kein Produktivdeploy.

## Abnahmegrenze

Authenticated NexVerse Live Avatar, Low158/COF, Outfit-/Skin-GPU-Pixel und serverbestätigte WASD/Yaw-Bewegung bleiben still open. Projektstand 39 % (38,65 % gewichtet), nächste Alpha.955–964. Kein Produktivdeploy.

## Vollständig verifizierter Mint-x64-Release-Gate
scripts/mint-viewer-validation.sh EXIT 0:
vollständiger Linux Mint CMake/vcpkg Debug Viewer 0.2.0-alpha.954;
183/183 native CTests und 457/457 Python-Quellverträge,
isolierter Xvfb-Viewer mit Firestorm Originalskelett 133 Joints,
drei Original-LLM-Bodymeshes GPU-resident 7186 Dreiecke,
OGL X11 WM_CLASS/Systemicon 128x128 PASS.
Keine echte authentifizierte Live-Grid-Sitzung, kein Produktivdeploy.

## Exakte GitHub Feature-SHA und CI
Code-SHA c86977f978cc15d3495df6cc88af65c75ce0d7d6.
Pi5 ARM64 Source Check Run 40008385875 IN_PROGRESS beim
letzten Abruf; nicht als Erfolg behaupten.
Hosted x64 Run 40008385892 COMPLETED/FAILURE,
Job 120602080388 runner_id 0, steps 0, kein Compiler.
Vorheriger Pi5 SUCCESS Alpha.944 SHA 7a97831,
Run 39994940156. Separat Mint x64 voller Release-Gate
EXIT 0: 183 CTests, 457 Python, Firestorm-LLM-Xvfb-GUI,
OGL X11 Taskleisten-Icon 128px PASS.
Kein Live-Grid-Login und kein Deployment.
