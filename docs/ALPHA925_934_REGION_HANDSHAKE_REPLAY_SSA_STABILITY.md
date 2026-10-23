# Alpha.925–934 – RegionHandshake-Replay darf aktive Szene nicht zuruecksetzen

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current
OpenGenesisLINK Viewer 0.2.0-alpha.934 | Gesamtprojekt 39 %

## Zehn Entwicklungsbloecke
925. GitHub develop-current HEAD ab38ddc und CHATUEBERGABE.md und projektplan.md zuerst geprueft; Alpha924 Pi5 Run 39991058245 fuer SHA 6be966b SUCCESS.
926. OpenGenesisLINK Servermain ccfc644d unveraendert; keine neuen RegionHandshake-, Low158- oder Firestorm-Wireformate.
927. ECHTEN Replay-Race gefunden: Erneute RegionHandshake-LLUDP-Nachricht setzte auch im aktiven Viewer SSA-ACK/Timestamp zurueck und CircuitState von active auf awaiting_region_ready.
928. MPL-2.0 RegionHandshakeReplay mit 16 circuit-spezifischen Eingangspaket-Sequenzen, inklusive out-of-order Retransmits, bounded RAM und U32 Wrap.
929. Der echte CircuitConnection::handle_packet bestaetigt reliable Retransmits weiterhin via LLUDP Transport-ACK, verhandelt aber dieselbe Handshake-Sequenz nicht erneut und loescht keine SSA-Autoritaet.
930. Neue Handshake-Sequenz wird weiterhin per echtem SSA-bit2 RegionHandshakeReply beantwortet, ohne den bereits aktiven Circuit zurueckzustufen; bestaetigte fruehere SSA-ACK bleibt erhalten.
931. CircuitStatus enthaelt unabhaengige, begrenzte Zaehler fuer duplizierte und frische Handshake-Refreshes, keine persoenlichen IDs oder Secrets.
932. F3-Avatar-Handshakestatus zeigt RegionHandshake Duplikate und Aktualisierung als Transport-Diagnostik, nicht als angenommenen COF-/Skin-/WASD-GPU-Beweis.
933. Native C++23-Regression gegen gleiche, verspaetete und out-of-order Sequenzen, FIFO-Kapazitaet 16, Reset, Wrap und verifizierten SSA-ACK. In Mint CTest und Pi5 ARM64 Headless eingebunden; Python-Quellvertrag prueft echten Handler, ACK, F3 und Release.
934. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Projektfortschritt-JSON sowie OpenGenesisLINK/Firestorm/UI-Paritaet gepflegt. Voller Mint x64 CMake/vcpkg/CTest/Python/Xvfb GUI/Icon-Gate vor Feature-Push und SHA-genaue CI-Dokumentation danach.

## Weiter offene Abnahme
Authentifizierter NexVerse-Live-Avatar mit realem COF/Outfit/Skin-GPU-Pixelnachweis und serverbestaetigtem WASD/Yaw bleibt still open.
Naechste Alpha.935–944; kein Produktivdeploy.

## Voller Linux-Mint-x64-Release-Gate bestanden
scripts/mint-viewer-validation.sh EXIT 0.
Full Linux-x64 CMake/vcpkg Debug Viewer 0.2.0-alpha.934.
183/183 native CTests, 447/447 Python Contracts, isolierter
Xvfb Viewer mit originalem Firestorm-Skeleton 133 Gelenke,
drei originalen GPU-LLM Bodymeshes / 7186 Dreiecke,
X11 WM_CLASS und OGL Taskleisten-Systemicon 128x128 PASS.
Kein eingeloggter authentifizierter Avatar, kein Produktivdeploy.
