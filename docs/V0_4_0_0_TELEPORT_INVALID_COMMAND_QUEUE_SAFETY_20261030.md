# OGL Viewer v0.4.0.0 – Ungültigen Teleport vor zuverlässigem LLUDP-Sendepfad ablehnen

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Basis `0acd94ab3930a3421c944116833bf4d5e68ab7d0`.**

## Reproduzierbarer Fehler

Im bisherigen `CircuitConnection::run` wird ein `TeleportLocation`-Befehl aus der zuverlässigen `OutboundCommandQueue` entnommen und von `encode_teleport_location_request` serialisiert. Dessen Encoder überprüfte bisher weder das RegionHandle auf Null noch die `Vec3`-Werte auf NaN oder Unendlich. Der nachfolgende zuverlässige UDP-Sendepfad behandelt ein **leeres oder nicht erzeugbares** Befehlspaket als `CircuitState::failed` und beendet den gesamten Circuit statt nur die ungültige Anwendungskomponente abzuweisen.

Die normalen UI-Teleportfelder werden bereits vor dem Queue-Aufruf begrenzt. Die zuverlässige Sendewarteschlange ist aber ein eigenständiger Protokoll-Eingang für interne Aufrufer, Tests und zukünftige Funktionen. Ein ungültiger Teleport darf nicht die bis dahin funktionierende Simulatorverbindung oder die zuvor eingereihten Chatnachrichten verlieren.

## Zehn implementierte Entwicklungs- und Testblöcke

1. `CHATUEBERGABE.md`, `ROADMAP.md` zuerst auf Linux Mint gelesen; lokales und entferntes `develop-current` auf `0acd94a` bestätigt, Arbeitsbaum sauber.
2. GitHub-CI des vorherigen Commits überprüft: **Source Check erfolgreich**, vollständiger Linux-x64-Viewer-Build zunächst `in_progress`. Log enthält weiterhin nur unauthentifizierte Viewer-/Xvfb-Abnahmen; keine erfundene Live-Teleport-Bestätigung.
3. Tatsächlichen Teleport-UI-Submit, `ui::valid_teleport_target`, `OutboundCommandQueue::push`, `CircuitConnection::run`, `encode_teleport_location_request` und den allgemeinen `send_reliable`-Fehlerweg untersucht.
4. Gemeinsam verwendeten `valid_teleport_location()`-Check direkt beim `TeleportLocation`-Protokollobjekt eingeführt: nichtnulliges RegionHandle, nur endliche x/y/z-Koordinaten und nur endliche x/y/z-`LookAt`-Werte. Legacy-`LookAt={0,0,0}` bleibt zulässig.
5. Die `OutboundCommandQueue::push()`-Sperre **vor** Mutex-/Kapazitäts- und Änderungslogik platziert. Ein ungültiger Teleport wird als nicht angenommen gemeldet; vorhandene zuverlässige Chat- und Steuerbefehle sowie die Reihenfolge bleiben erhalten.
6. Den tatsächlichen Wire-Encoder `encode_teleport_location_request()` vor dem Schreiben von Agent-/Session-/Zielbytes abgesichert. Auch direkte Encoder-Aufrufer können jetzt keine NaN/Inf-/Nullhandle-Ziele an den Simulator serialisieren.
7. Native `outbound_queue_tests.cpp` um NaN, +/-Inf, Null-RegionHandle, unveränderte Queue-Größe und Erhalt des zuvor eingereihten Chatbefehls ergänzt. Den existierenden Test-Teleport auf ein tatsächlich gültiges RegionHandle umgestellt.
8. Native `viewer_messages_tests.cpp` um die gleichen ungültigen Direktencoder-Ziele erweitert und den unveränderten 64-Byte-`TeleportLocationRequest`-Wirepayload für gültige Ziele beibehalten. Null-`LookAt` bleibt eine legale ältere Server-Konvention.
9. Zehn weitere Python-Vertragsprüfungen sichern gemeinsame Validator-Verwendung in Queue und Encoder, Anwendungs-UI-Teleport, nativen Negativtests, Legacy-Kompatibilität sowie das unveränderte Queue- und reliable-Packet-Verhalten ab.
10. Vollständige Linux-Mint-x64-Abnahme per `scripts/mint-viewer-validation.sh` einschließlich C++-Build, allen CTests, Python, Original-Firestorm-Xvfb-/OpenGL-/GUI und X11-Systemicon; anschließend `CHATUEBERGABE.md`, `ROADMAP.md`, `projectplan.md` und Fortschritts-JSON aktualisieren, auf `develop-current` committen und GitHub-CI prüfen.

## Grenzen

Ein **ungültiges** Teleportziel wird am Queue-Eingang frühzeitig abgewiesen. Bei einem gültigen Befehl kann weiterhin ein echter Netzwerk- oder Socketfehler auftreten; der Fix darf keine bestätigten UDP-ACKs erfinden. Die normalen GUI-Grenzen für X/Y innerhalb der Region und Z von -100 bis 4096 gelten weiterhin. Der zusätzliche Protokollvalidator ist bewusst einfacher, um von internen Aufrufern beliebige **endliche** Positionen zuzulassen; nur die UI kennt die aktuelle Regionsgröße.

Für die echte Abnahme auf `hg.stadt-nexverse.de` weiterhin offen: erfolgreicher authentifizierter Teleport, Regionswechsel samt `UseCircuitCode/CompleteAgentMovement`, serverseitiger WASD-/Flug-/Z-/Yaw-Readback, aktuelle Self-Head-/Upper-/Lower-RGB-Bakes und COF/Outfit sowie Shutdown nach Login.

Zuletzt geprüfter OpenGenesisLINK-Server: `08bf15f306bb4f882f24f32b84eb8df3a3175cea` (30.10.2026, Login-Zeitzone; kein neues Teleport-Paketlayout).

**v0.4.0.0 IN ARBEIT – 38,65 % gewichtet (39 % gerundet), kein Merge nach `main`, kein Release/Deployment.**
