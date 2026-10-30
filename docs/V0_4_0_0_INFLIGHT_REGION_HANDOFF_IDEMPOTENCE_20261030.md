# OGL Viewer v0.4.0.0 – Doppelte CAPS-Teleport-/Regionsereignisse nicht erneut auf Weltmodell anwenden

**30.10.2026 | Linux Mint `jam-MS-7D28` | GitHub-Branch `develop-current` | Ausgangscommit `014c576622201f9a439c972d98d6b95faccd3e6c`.**

## Reproduzierter Fehler

In `src/app/application.cpp` werden authentifizierte EventQueue-Nachrichten `TeleportFinish` und `CrossedRegion` im ursprünglichen Firestorm-Weltmodell mit `WorldModel::begin_region_transition()` verarbeitet. Das EventQueue-ACK-/Replay-System filtert doppelte Batch-IDs innerhalb eines Queue-Lebenszyklus, kann aber bei neu eröffneten CAPS-Queues mit eigenem ACK-Cursor nicht allgemein beweisen, dass ein erneut ausgeliefertes Teleport-Ereignis denselben bereits begonnenen Übergang beschreibt.

`WorldModel::begin_region_transition()` setzte **bei jedem Aufruf** die Region-Generation um 1 hoch, verwarf Avatar-, Skin-/Wearable-, ObjectUpdate-, Terrain-/Parcel- und Weltzustände und setzte die bereits laufende Bewegung/Session-Readbacks zurück. Ein zweites inhaltlich identisches Ereignis konnte damit einen gerade begonnenen Regionswechsel erneut initialisieren und vorher eingetroffene Welt-/Avatar-Daten verlieren.

## Zehn bearbeitete Entwicklungs-/Abnahmeblöcke

1. Auf Linux Mint `CHATUEBERGABE.md` zuerst gelesen, `develop-current` mit GitHub abgeglichen; Arbeitsbaum sauber. Beide GitHub-CIs des Ausgangscommits grün.
2. Aktuelle Viewer-Logs und offenen P0-Bereich `v0.4.0.0-T02` geprüft: kein neuer authentifizierter Grid-/Teleport-Test im neuesten lokalen Log, daher keine erfundene E2E-Abnahme.
3. Authentifizierten `TeleportFinish`-/`CrossedRegion`-CAPS-Dispatch samt Agent-ID-/Session-ID-Schutz, Login-Queue und separater Region-Queue untersucht.
4. `WorldModel::begin_region_transition()` gegen tatsächliche Resetwirkungen auf Region-Generation, Scene/Prims, Self-Avatar, Skin-COF und Animation geprüft; fehlende Idempotenz als Fehlerursache reproduziert.
5. Eng begrenzte `pending_region_transition_`-Fingerprint-Sperre hinzugefügt. Sie gilt **nur während eines laufenden Übergangs** und nur bei exakter Übereinstimmung von RegionHandle, Ziel-IP, Port, Seed-CAPS und Zielposition (x/y/z).
6. Bei exaktem Replay vor `avatars_.clear()`, Szenen-Reset und Generation-Inkrement ohne Wirkung zurückkehren. Keine Entscheidung allein aufgrund gleicher RegionHandle- oder gleicher CAPS-URL treffen.
7. Bei neuer Zielposition, geänderter Seed-CAPS, anderem Simulator-Port oder anderer Region weiterhin vollständigen echten Übergang ausführen. Das ist für absichtliche neue Same-Region-Teleports und Server-Neustarts nötig.
8. Auf `apply_region(connected=true)`, Identitätswechsel und `WorldModel::reset()` den Pending-Fingerprint verwerfen, damit **nach** erfolgreichem Handoff/Relog derselbe Zielserver erneut erreichbar bleibt. Alle Änderungen nur im Weltmodell; keine Logausgabe der Seed-CAPS-URL oder Sessiondaten.
9. Nativen `nexverse-world-model-tests` um exaktes Duplikat, Zielposition, Seed, Port, erneuten Same-Region-Übergang nach Verbindung und Logout erweitert. Bestehende Region-EventQueue-, CAPS-World-Event- und Login-ACK-/Replay-Tests unverändert. Acht neue Python-Verträge für die ganze CAPS-zu-Weltmodell-Verkabelung ergänzt.
10. Vollständiges Linux-Mint-x64-CMake-/CTest-/Python-/Xvfb-Original-Firestorm-OpenGL-/GUI-/X11-Icon-Gate durchgeführt, danach `projectplan.md`, `ROADMAP.md`, `CHATUEBERGABE.md`, Fortschritts-JSON und `develop-current` aktualisiert; neue GitHub-CI separat kontrolliert.

## Grenzen

Die automatisierten Tests bestätigen, dass ein exakt wiederholter, noch laufender Regionstransfer nicht erneut den Weltmodellzustand löscht. Sie bestätigen **nicht** den erfolgreichen Live-Teleport auf `hg.stadt-nexverse.de`, die Darstellung aller drei aktuellen Body-Bakes oder die tatsächliche Skin-/COF-/Wearables-Verarbeitung. Auch WASD/Flug, Region-Crossing und Shutdown nach einem echten Login bleiben zur manuellen Abnahme offen. Keine Änderung an der vorhandenen Welcome-Website, dem Login-Layout oder an den Firestorm-Grundfunktionen.

**v0.4.0.0 IN ARBEIT; Projekt gesamt 38,65 % (39 % gerundet); `main` unverändert, kein Release/Produktivdeploy.**

## Zusätzliche OpenGL-Abnahme

Der vollständige Mint-Test hat einen zuvor intermittierenden Fehler im Offscreen-Avatar-Test reproduziert (Code 46): Auf Mesa llvmpipe muss ein vollständig außerhalb der Frustum liegender Avatar nicht binnen sechs Frames einen aufgelösten GPU-Depth-Query liefern. Der ursprüngliche Test forderte diese nicht garantierte Aussage. Korrigiert wurde nur die fehlerhafte Testerwartung: Bei unbekanntem Query-Status sind self_avatar_visible_samples_known=false und self_avatar_visible_samples_passed=false erforderlich, zusammen mit den unveränderten originalen Mesh-Drawwerten und fehlerfreiem GL. Ein bekanntes Query-Ergebnis mit unerwartet sichtbaren Fragmenten bleibt weiterhin ein harter Fehler. Das ist keine Freigabe eines nicht verifizierten Skin-/GPU-Pixel-Readbacks.

Verifizierter kompletter lokaler Gate: **195/195 native Tests, 767/767 Python, Firestorm-Xvfb-GUI/OpenGL und X11-Systemicon PASS**.
