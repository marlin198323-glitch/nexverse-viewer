# OpenGenesisLINK Viewer – P0 CAPS-EventQueue Wiederholungen, 25.10.2026

Arbeitsrechner: Linux Mint `jam-MS-7D28`. GitHub-Branch `develop-current`, Ausgangscommit `e8a21a11ddd258a14405199c86d20940ebcc4680`. Zielversion `v0.4.0.0` (IN_ARBEIT).

## Auslöser

Die regionale EventQueue prüfte bisher nur die letzte Batch-ID. Liefert der Simulator beispielsweise die Folge 17, 18, 17, 19, wurde die dritte Antwort erneut an die Event-Dispatcher geschickt; der Cursor sprang zurück auf 17. Die ursprüngliche Login-CAPS-EventQueue hatte sogar gar keine entsprechende Replay-Prüfung und behandelte gültige leere ID-lose HTTP-200-Antworten als `CAPS LIVE`, ohne eine angemessene Polling-Pause.

## Zehn ausgeführte Bearbeitungsblöcke

1. Branch, Commit, Übergabe, Roadmap und CI auf Linux Mint und GitHub abgeglichen; keine ungesicherten lokalen Änderungen vor Bearbeitung.
2. Bestehende Region-CAPS-Verarbeitung, Long-Poll-ACK, Seed-Wiederermittlung und Dispatcher auf Replay- und Empty-Batch-Verhalten geprüft.
3. Neuer `EventQueueRecentBatches`-Zustand: maximal 64 zuletzt zugestellte IDs je Queue-Inkarnation; keine unbeschränkte Speichernutzung.
4. Bei nicht direkt aufeinanderfolgenden Wiederholungen (17,18,17) keine erneute Zustellung und keine Rücksetzung auf den alten ACK.
5. Wiederholungstests mit realistisch geskripteten HTTP-/LLSD-Antworten und exakter ACK-Folge [-1,17,18,18] ergänzt.
6. Nach 404/erneuter Seed-CAPS-Ermittlung alte Recent-ID-Historie zusammen mit ACK verwerfen – auch wenn die URL identisch bleibt. Native Wiederverwendung derselben ID nach Queue-Neustart geprüft.
7. Dieselbe Replay-Historie in die **ursprüngliche Login-CAPS-EventQueue** integriert; kein doppelter Inventar-/Teleport-Dispatcheraufruf aus einer wiederholten Batch.
8. Erfolgreiche leere EventQueue-HTTP-200-Antworten ohne ACK-ID im Loginpfad mit kontrollierter 150-ms-Pause behandeln. Kein künstliches `CAPS LIVE`, keine Löschung eines gültigen vorherigen ACK und kein Busy-Loop.
9. Datensparsame Region-EventQueue-Workerdiagnostik ergänzt: Anzahlen von `duplicate_batches`, `idless_batches`, `idle_polls`, `failed_polls` und `rediscoveries`. Keine URLs, Tokens, UUIDs, Event-Bodies oder Login-Daten in neuen Logs.
10. Native Unit-/Mock-Regressionsprüfungen und fünf zusätzliche Python-Vertragstests, vollständiger Mint-x64-CMake-Build/CTest/Python/Xvfb-OpenGL-GUI/128px-X11-Icon-Gate.

## Abnahmegrenzen

Die Login- und Regionsqueue verhalten sich bei Replay, ID-losen leeren HTTP-200-Antworten und einer nach 404 erneuerten Endpoint-Inkarnation konsistent. Die letzten 64 IDs sind eine bewusst begrenzte deduplizierende Historie; weit ältere Replays können nicht garantiert erkannt werden. Ein echter Live-Grid-Login mit einem korrekt auslösenden Teleport und End-to-End-AvatarAppearance-/COF-/Bewegungsreadback liegt **nicht** als prüfbares Ergebnis dieser Unit-Tests vor.

Das reproduzierbare Ergebnis ist keine vollständige Freigabe von CAPS/EventQueue oder `v0.4.0.0`. Gesamtfortschritt bleibt **38,65 % (39 %)**, ohne Deployment, main-Merge oder Tag. **Exaktes lokales Prüfgate:** 188/188 native CTests, 685/685 Python, isolierte Firestorm-Xvfb-OpenGL-GUI und X11-Taskleistenicon PASS (scripts/mint-viewer-validation.sh). GitHub CI für den nachfolgenden Commit ist separat zu verifizieren.
