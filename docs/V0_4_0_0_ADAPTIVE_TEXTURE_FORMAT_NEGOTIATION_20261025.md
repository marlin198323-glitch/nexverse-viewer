# OGL v0.4.0.0 – GetTexture PNG/JPEG2000-Format lernen und unnötige HTTP-Anfragen vermeiden

**25.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current` | Ausgangscommit `8857dcc732bf74ef2750f55428fa046e26fe9f93`.**

## Gesicherte Ausgangslage

Im lokalen authentifizierten Viewer-Log vom 25.10.2026 wurden **4.757** erfolgreiche GetTexture-Meldungen `x-j2c fallback succeeded` gefunden. Die bisherige Ladepipeline versuchte pro Asset zunächst `format=png`; nur bei erfolglosem PNG-Decoder/HTTP-Response erfolgte `format=x-j2c`. Auf Simulatoren mit dauerhaft defekter oder nicht vorhandener PNG-Konvertierung verursacht dies über die gesamte Szene tausende vermeidbare erste HTTP-Anfragen. Eine tatsächliche Übertragungszeitersparnis ist aus diesen historischen Logs allein nicht berechenbar.

## Zehn bearbeitete Entwicklungsblöcke

1. GitHub-Branch `develop-current`, CI, `CHATUEBERGABE.md`, `projectplan.md`, Roadmap und sauberen Linux-Mint-Checkout geprüft.
2. Echte Viewer-Protokolle datensparsam nach erfolgreichen PNG-/J2C-Fallbacks und Fehlern gezählt; die 4.757 nativen J2C-Fallbacks bilden den konkreten Ansatzpunkt.
3. Bestehenden C++-TextureAssetLoader, PNG-/J2C-Decodersicherheit, Shared Cache, asynchrone GetTexture-Worker und Region-Epochenstruktur untersucht.
4. `TextureFormatPreference` als mutexgeschützte **reine regionslokale Statistik** implementiert. Sie speichert **keine** Asset-UUIDs, Avataridentität, CAPS-URLs oder Sitzungstokens.
5. Nur nach **drei** erfolgreich dekodierten J2C-Antworten bei zuvor ausdrücklich fehlgeschlagener PNG-Konvertierung bevorzugt der Viewer den nativen Endpunkt. Normale PNG-Erfolge, einzelne Fehler oder leere Regionen können den bevorzugten Modus nicht aktivieren.
6. Bis zu 15 von 16 darauffolgenden GetTexture-Transfers umgehen die unnötige PNG-Abfrage. **Jede 16.** mögliche Anfrage kontrolliert wieder die PNG-Konvertierung. Sobald PNG wieder erfolgreich ist, wird die Präferenz aufgehoben.
7. Bei einem Fehler des inzwischen bevorzugten J2C-Endpunkts wird **dieselbe Textur unmittelbar erneut über PNG** geprüft, bevor die Asset-Retry-Sperre greifen darf. Das gilt auch für abgeschnittene PNG-Rückgaben auf einem J2C-Endpunkt; fehlerhafte Bilddaten werden weiterhin nicht als Erfolg akzeptiert.
8. Neue Regions-Epoche setzt die Verhandlungsstatistik zurück; verspätete Resultate eines älteren Workers dürfen sie nicht erneut verändern. Die vorhandenen Bildlimits, VRAM-Budgets, Worker-Pacing und 404-/HTTP-Backoffs bleiben erhalten.
9. Nativen C++-Test `nexverse-texture-format-preference-tests` und Transport-Mocktests für dreimaligen Lernvorgang, 15 eingesparte PNG-GETs, 16. Probe, Wiederherstellung, nativen Ausfall, direkte PNG-Rettung und Regionswechsel ergänzt. Acht Python-Vertragstests prüfen die tatsächliche Einbindung in die Produktionspipeline.
10. Bestehende 60-Sekunden-GetTexture-Telemetrie um ausschließlich aggregierte Werte `native_png_gets_avoided`, `native_png_reprobes` und `native_preferred` erweitert; vollständiges Linux-Mint-x64-CMake-/CTest-/Python-/Xvfb-Firestorm-OpenGL-/GUI-/X11-Icon-Gate ausgeführt und Projektübergabe vorbereitet.

## Testergebnis

- **195 / 195 native CTests PASS**, davon ein neuer direkter Format-Präferenz-Test.
- **743 / 743 Python-Tests PASS**, einschließlich acht neuer Verhandlungs-/Sicherheitsverträge.
- Isolierter originaler Firestorm-GL-Viewer-GUI-Start mit Xvfb **PASS**; OGL-X11-App-Icon **PASS**.
- Ein echter neuer Grid-Login ist für diesen Test nicht erfolgt. Die HTTP-Einsparungen wurden deterministisch mit validen PNG- und JPEG2000-Fixtures in Transport-Mocks bewiesen, nicht durch Live-Grid-Latenzmessung.

## Offene P0-Abnahme

Nach authentifiziertem Login: `GetTexture streaming metrics` und die Zähler für vermiedene/probierte PNG-GETs prüfen; Bildqualitätsunterschiede und Serverantworten beobachten, danach `Avatar SELF bake pipeline` mit Head/Upper/Lower 3/3, COF-/Outfit-Daten und finalem Skin-RGB-Readback verifizieren. Die historischen JPEG2000-Metadatenfehler und fehlenden Meshes sind durch diesen **Streaming-Optimierungsschritt nicht automatisch behoben**. Live-Grid-Region/Teleport/WASD/Flug und tatsächliche Shutdown-Dauer nach Login sind ebenfalls noch offen.

**Zielversion v0.4.0.0 IN_ARBEIT, Projektgesamtfortschritt 38,65 % (39 % gerundet). Kein main-Merge, kein Tag, kein Deployment.**
