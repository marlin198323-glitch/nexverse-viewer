# OGL v0.4.0.0 – Region EventQueueGet: abgelaufenen Endpunkt ohne Rediscovery nicht weiter abfragen

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Ausgangscommit `83e106be20014400e747c3a030a42084e25cecac`.**

## Konkreter Fehler und Auswirkung

Nach einem `EventQueueGet` HTTP 404 (Queue abgelaufen) wurde das authentifizierte Region-Seed-CAPS erneut abgefragt. Wenn diese erneute Seed-Abfrage HTTP 503 lieferte oder `EventQueueGet` nicht mehr enthielt, verwendete `poll_region_event_queue()` anschließend die **bereits als abgelaufen bestätigte Queue-URL** weiter. Die Region-EventQueue-Schleife konnte so ohne aktuelle CAP festsitzen, und die bereits vorhandene `RegionEventRestartGate` mit 5–60 Sekunden Cooldown bekam nie ein entsprechendes Fehlerergebnis. CAPS-Nachrichten wie Teleport/Inventarupdates konnten deshalb fehlen, obwohl die simulatorseitige Region als aktiv galt.

Der Client beendet in dieser Situation jetzt den Worker ausdrücklich mit `advertised=false`, einem epochgebundenen Ergebnis und einer anonymen Statistik `expired_rediscovery_failures`. Die bestehende Restart-Gate startet bei weiter aktivem Simulator-Circuit nach seiner Wartezeit einen **neuen** Seed-/EventQueueGet-Worker. Ein gültig wiederentdeckter CAP (auch unter derselben URL) wird weiterhin verwendet, und alte ACK/Batch-Duplikate werden dabei zurückgesetzt. HTTP 502 bleibt ein normales EventQueue-Long-Poll-Idle und löst keinen unnötigen Neustart aus.

## Zehn Entwicklungs- und Verifikationsblöcke

1. Verfügbarkeit des **Linux-Mint-Commanders** `jam-MS-7D28` geprüft; keine Entwicklung auf Raspberry Pi; `CHATUEBERGABE.md` als erste Übergabequelle gelesen.
2. `develop-current` mit `origin/develop-current` abgeglichen und vorherige GitHub-CI überprüft: Source Check und Linux-x64-Build des Ausgangscommits erfolgreich.
3. Versionierte `ROADMAP.md`, `projectplan.md` und den 39-%-Projektfortschritt geprüft; weiterhin offene echte Live-A01/A02-Abnahmen festgestellt.
4. Viewer-Log bis 29.10.2026 geprüft: jüngster Lauf nur lokaler Start ohne Authentifizierung; kein neuer Nachweis zu Skin, COF, tatsächlicher Bewegung oder Live-EventQueue.
5. `RegionEventQueueStats`, `EventQueueRecovery`, Seed-CAPS-Discovery, ACK-Cursor und `RegionEventRestartGate` analysiert; den festhängenden HTTP-404-Pfad identifiziert.
6. Fehlerpfad so korrigiert, dass fehlgeschlagene Rediscovery nach abgelaufenem Endpunkt den Worker gezielt beendet und einen **retryfähigen** Zustand an den vorhandenen Gate meldet. Verhalten bei gültigem neuem bzw. gleichlautendem CAP beibehalten.
7. Native HTTP-Simulationen für HTTP404 → Seed HTTP503 sowie HTTP404 → erfolgreicher Seed **ohne EventQueueGet** ergänzt und tatsächlichen Worker-Ausstieg überprüft.
8. Native Regressionen für erneuten erfolgreichen Worker in derselben Region, 5-Sekunden-Restartgate, bestehende ACK-Deduplizierung, 404-ACK-Reset und 502-Idle beibehalten. Sieben weitere Python-Vertragsprüfungen ergänzt.
9. Die nur numerische Warnungs-/Status-Telemetrie um `expired_rediscovery_failures` erweitert; keine CAP-URL, Agent-UUID, Zugangsdaten oder Event-Bodys im zusätzlichen Log. Sporadischen Mesa-llvmpipe-GPU-Test-Code 46 unter parallelem CTest untersucht: dreimaliger isolierter Nachtest grün, nur dessen CTest-Markierung auf `RUN_SERIAL TRUE` gesetzt, keine Abschwächung seiner Assertions.
10. Linux-Mint-CMake-/CTest-/Python-/Firestorm-Xvfb-OpenGL-GUI-/X11-Icon-Gates vollständig ausgeführt, Roadmap/Projektstatus/Chatübergabe aktualisiert und anschließend auf `develop-current` veröffentlicht. Das genaue Testergebnis steht im aktualisierten Fortschritts-JSON und der Commit-/CI-Anzeige.

## Aktueller Upstream

Zum 30.10.2026 wurde `opengenesislink/opengenesislinkserver` auf `08bf15f306bb4f882f24f32b84eb8df3a3175cea` geprüft (Konfiguration Login-Zeitzone). Der vorliegende Fix ist unabhängig davon ein Client-Fehler beim Ablauf einer vorhandenen CAPS-URL und erfordert keinen Server-Protokollumbau.

## Abnahmegrenzen

Der isolierte Test beweist den automatischen Wiederanlauf unter den genannten HTTP-Fehlerszenarien; **kein** aktueller authentifizierter Live-Login, Regionswechsel oder Teleport wurde damit nachgewiesen. Weiter offen bleiben echtes `AgentMovementComplete`-/WASD-/Flug-Readback, Head-/Upper-/Lower-Skin-RGB-Bakes 3/3, COF-/Outfit-Synchronisierung und Shutdown-Latenz nach Login.

**Zielversion `v0.4.0.0` IN ARBEIT; Projektgesamtfortschritt 38,65 % (39 % gerundet). Kein Merge nach `main`, kein Produktivdeploy.**
