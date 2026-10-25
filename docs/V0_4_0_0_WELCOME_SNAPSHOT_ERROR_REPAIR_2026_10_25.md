# OGL v0.4.0.0 – Fehlerbehebung WebKit/Welcome, 25.10.2026

**Quelle:** GitHub develop-current ab `4a726ff7c4d394651a349248177a95ac2b6b3c81`, Desktop Commander auf Linux Mint `jam-MS-7D28`. `CHATUEBERGABE.md` und `projectplan.md` wurden vor der Arbeit gelesen.

## Befund und reproduzierbare Ursache

Der Viewer zeigt die Live-Welcome-Seite über einen Offscreen-WebKit-Snapshot als OpenGL-Textur an. Während einer laufenden Bildaufnahme kann ein Fenster-Resize neue Maße setzen; die ältere Antwort wurde anschließend ohne Generationsprüfung als aktueller Frame übernommen. Zusätzlich lud die GPU neue Bilddaten nur bei geändertem Breite/Höhe-Paar hoch. Damit konnten aktuelle Grid-/News-Daten bei unveränderten Maßen unsichtbar bleiben. Ein Fehler beim Snapshot verhinderte wegen `frame.failed` bislang erneute Versuche.

## Zehn Entwicklungs-/Fehlerblöcke

1. **Stale Frame / Race:** Revisionszähler für Browser-Viewport eingeführt; alte Bildantworten nach Resize werden verworfen.
2. **Nur ein Inflight-Request:** Ein eigener Snapshot-Zustandsautomat erlaubt nicht mehrere parallel unkontrollierte WebKit-Bildaufnahmen.
3. **Feste Request-Geometrie:** Snapshot-Callbacks verwenden die beim Start gespeicherte Breite/Höhe, nicht zwischenzeitlich mutierte Widget-Maße.
4. **Fehlerversuche:** Bei vorübergehenden WebKit-Snapshotfehlern bis zu drei gedrosselte Versuche (250/500/750 ms), statt Endlosschleife oder permanentem Erstfehler.
5. **Wiederaufnahme nach Resize:** Eine echte Änderung der Fenstergröße setzt begrenzte Fehlerversuche zurück und veranlasst einen neuen Snapshot.
6. **Laufende Welcome-Anzeige:** Alle 30 Sekunden erneute Aufnahme, auch bei gleichbleibender Fensterauflösung, ohne Eingriff in HTML oder Netzwerksicherheit.
7. **GPU-Aktualisierung:** `WelcomeFrame.generation` und `welcome_texture_generation` werden verglichen, damit neue News-/Grid-Pixel auch bei gleichen Maßen auf die GPU übertragen werden. Fehlgeschlagene Uploads gelten nicht als abgeschlossen.
8. **Asynchrone Lebensdauer:** Vor GTK-/WebKit-Widget-Abbau wird der Callback-Eigentümer entkoppelt und nachfolgende Snapshot-Callbacks entsorgen Ressourcen ohne Zugriff auf zerstörten Viewer-Zustand.
9. **Regressionstests:** Native `nexverse-welcome-snapshot-policy-tests` plus vier Python-Verträge für Resize, Stale-Callback, bounded retry, 30-s-Refresh, GPU-Generationen und Callback-Lebensdauer.
10. **Echte GUI-Abnahme:** Vollständiger Mint-x64-Build, **188/188 CTests**, **675/675 Python-Tests**, Xvfb-Firestorm-OpenGL-GUI/X11-Icon PASS; Xlib-basierter realer Fenster-Resize bei 1100×640, 1910×790, 1280×720, 1648×928 ohne neuen Welcome-Fehler im Viewer-Log.

## Bewusst weiterhin offen

- **Kein authentifizierter Nachweis** für AvatarAppearance/COF/Skin/Outfits, LLUDP-WASD/Flug/Teleport und CAPS/EventQueue-Laufzeit. Dafür fehlen im lokalen Befund echte Session-Logs aus einem aktuellen angemeldeten Grid-Test.
- Shutdown-Gesamtdauer nach Login ist nicht gemessen; die vorhandenen Worker-/Total-Zeitmarker bleiben dafür aktiv.
- Die Live-Welcome-WebKit-Darstellung im Viewer ist eine Snapshot-Textur. Native HTML-Link-Interaktionen sind nicht automatisch Browser-Klicks.
- Kein Release auf `main`, keine Produktionsinstallation. `v0.4.0.0` bleibt IN_ARBEIT; Projektgewichtung **38,65 % (39 % gerundet)**. Dieser Fehlerblock ist ein getesteter Teilfortschritt, kein Beleg für fertige Welt-/Avatar-Funktionalität.
