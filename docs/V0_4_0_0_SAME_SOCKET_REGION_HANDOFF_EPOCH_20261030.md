# OGL Viewer v0.4.0.0 – LLUDP-Handoff für zwei Regionen am gleichen Simulator-Socket

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Ausgangscommit `1f60f1081f59f8538464363ae9da4056bf47b9de`**

## Konkreter Fehler

Der ursprüngliche `region_handoff_action()`-Algorithmus entschied ausschließlich anhand von Simulator-IP und Port, ob ein neuer LLUDP-Circuit gestartet werden muss. Das reicht für OpenSim/OpenGenesisLINK nicht aus: Ein anderer `RegionHandle` oder ein erneuter tatsächlicher `TeleportFinish` kann denselben Simulator-UDP-Socket verwenden. Der `WorldModel::begin_region_transition()`-Pfad setzt dabei bewusst den alten Avatar-/Terrain-/Objektzustand zurück und erhöht die Region-Generation, die bisherige Handoff-Entscheidung meldete jedoch weiterhin `unchanged`.

Damit konnte der Zustand **Region bereits neu, Circuit noch alt** entstehen. Die bisherige `region_circuit_ready()`-Abfrage verwendete ebenfalls nur IP/Port; bei `CircuitState::active` des alten Workeres konnte die neue Region daher voreilig als verbunden gelten. Dadurch riskierte der Viewer stale Region-Pakete, falsche Bewegungsbestätigung und verfrühtes Freischalten der Region-EventQueue. Es gab bisher keinen authentifizierten Live-Beleg für diesen konkreten Same-Socket-Fall; das Problem ist aber im nativen Handoff-Entscheidungsweg reproduzierbar.

## Zehn bearbeitete Entwicklungs-/Prüfblöcke

1. `CHATUEBERGABE.md`, Version und Roadmap zuerst auf dem **Linux-Mint-Commander** gelesen, lokalen `develop-current`-HEAD und Remote gleichgesetzt.
2. GitHub-Source-Check des Ausgangscommits als **PASS** verifiziert; GitHub-Full-x64-Build zu Arbeitsbeginn noch `in_progress`. Neueste lokale Logs sind weiterhin unauthentifizierte Xvfb-/GUI-Läufe.
3. `region_handoff_action`, `region_circuit_ready`, `region_packet_matches`, `WorldModel::begin_region_transition`, Simulator-Circuit-Worker, EventQueue-Start und manuelle Circuit-Retry-Logik über den tatsächlichen Quellcode verfolgt.
4. Einen expliziten `RegionCircuitKey` aus IP, Port, RegionHandle und Generation eingeführt. Die älteren reinen Socket-Hilfsfunktionen bleiben für bisherige Aufrufer und native Regressionen kompatibel.
5. Ein tatsächlich **ausstehender** Teleport/Regionswechsel wird bei veränderter Regionidentität als neues Handoff behandelt, selbst bei gleichem Socket. Erst alte Worker-Stop-Anforderung, dann `std::future::get()`, Leeren der ausstehenden Befehlsqueue und Start eines neuen `UseCircuitCode/CompleteAgentMovement`-Workeres.
6. `WorldModel::region_transition_pending()` gibt nur das vorhandene private Pending-Flag ohne Änderung oder zusätzliche Speicherung frei. Ein reiner Circuit-Verbindungsabbruch ohne neuen Teleport löst nicht automatisch immer neue Workerstarts aus.
7. Die Regionidentität wird beim Start des LLUDP-Workeres **wertkopiert**. Der Packet-Callback prüft Socket, RegionHandle und Generation, damit verspätete Pakete einer alten Same-Socket-Inkarnation nicht in das neue Weltmodell gelangen.
8. Die Circuit-Ready- und Zielregion-EventQueue-Gates prüfen zusätzlich Handle und Generation. Der alte `CircuitState::active` wird vor dem neuen Worker explizit auf `opening_socket` zurückgesetzt, sodass die neue Region vor dem wirklichen Handshake nicht irrtümlich als aktiv markiert wird. Ein manueller Retry übernimmt die neueste Regionidentität.
9. `region_circuit_handoff_tests.cpp` um Same-Socket-Regionswechsel, Same-Region-Teleport mit neuer Generation, alten Packet-Source-Blocker, Übergangszustände, Portwechsel und getrennte automatische versus manuelle Recovery ergänzt. `world_model_tests.cpp` prüft Pending-Flag während Teilhandshake, nach bestätigtem Connect und nach Logout. Zehn Python-Verträge prüfen die echte App-/Worker-/EventQueue-Verkabelung.
10. Gesamtprüfung Linux Mint: C++-Build, alle nativen CTests, Python-Unittests, Original-Firestorm-Xvfb-OpenGL-/GUI- und X11-Systemicon-Abnahme; `CHATUEBERGABE.md`, `ROADMAP.md`, `projectplan.md`, Fortschritts-JSON aktualisieren, nach `develop-current` pushen und GitHub-CI überprüfen.

## Abnahmegrenzen

Die Tests belegen, dass ein **expliziter** Same-Socket-Regionswechsel nicht mehr mit der alten LLUDP-Generation verwechselt wird. Sie belegen keinen erfolgreichen authentifizierten Teleport auf `hg.stadt-nexverse.de`; ohne echte Anmeldung wurden keine Benutzerpasswörter an den Gridserver übermittelt.

Weiter offen: Live-`UseCircuitCode/CompleteAgentMovement` bei Port- und Same-Socket-Regionswechsel, Avatar-Head-/Upper-/Lower-RGB-Skin (3/3), COF-Outfit, WASD-/Flug-/Z-/Yaw-Readback und die Logout-/Shutdown-Latenz nach Login.

Der zuletzt geprüfte OpenGenesisLINK-Server-Stand `08bf15f306bb4f882f24f32b84eb8df3a3175cea` betrifft nur die Login-Zeitzone. Willkommen-Hintergrund, kompaktes Login und originale Firestorm-Renderfunktionen wurden nicht verändert.

**Zielversion `v0.4.0.0` IN ARBEIT; Projektfortschritt 38,65 % gewichtet (39 % gerundet); kein Merge auf `main`, kein Release.**
