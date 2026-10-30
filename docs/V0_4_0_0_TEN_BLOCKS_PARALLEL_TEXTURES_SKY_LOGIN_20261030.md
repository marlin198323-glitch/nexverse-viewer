# v0.4.0.0 – Zehn Blöcke: Login merken, paralleles Streaming, Cache, Himmel

Stand 30.10.2026 – Linux Mint jam-MS-7D28, `develop-current`. Kein Release oder main-Merge.

## Befund aus echter eingeloggter Viewer-Sitzung

Der bisherige Live-Log dokumentiert 268/280 GPU-residente Texturen (95 %), zwölf Textur-UUIDs, bei denen sowohl PNG als auch J2C serverseitig HTTP 404 liefern, und nach wie vor 0/3 bestätigte SELF-Avatar-Head-/Upper-/Lower-Body-Bakes. Ein Cache kann keine serverseitig fehlenden Assets ersetzen. Die letzte vorhandene Live-Binary benutzte noch einen einzigen GetTexture-Worker. Ein echtes neues Live-Benchmark für diese Änderung wird ausdrücklich **nicht** behauptet.

Die Prüfung des OpenGenesisLINK-Server-Upstream am 30.10.2026 ergab Commit `1b8fb1c` (Freigabe des Versionspräfixes v0.4.0.0); der eigene numerische Wire-Versions-Workaround bleibt kompatibel.

## Zehn erledigte Entwicklungsblöcke

1. **Bestandsanalyse**: authentifizierte Login-Logs, 404-Assetfehler, GPU-Residency, aktuelle Quellen und Upstream geprüft.
2. **Login merken**: vorhandene, opt-in geschützte `libsecret`-Integration geprüft; GNOME-Keyring/Secret-Service auf Linux Mint verfügbar. Erfolgsmeldung nach tatsächlicher Schlüsselbundspeicherung ergänzt; gespeicherte Daten werden nur nach erfolgreichem Login geschrieben, nie in Klartext-Konfigurationsdateien. Kein Auto-Login.
3. **Vier parallele Texture-Downloads**: `TextureInFlight`-Liste mit höchstens vier unabhängigen HTTP-/Dekodierungs-Futures, je eigener Startzeit; stündliche Kapazität wird nicht vorgetäuscht.
4. **Entdoppelung und Fairness**: In-flight-UUIDs werden bei jeder Anfrage wie bereits erfüllte Anforderungen übersprungen. Die bisherige 2:1 Cache-/Netzwerk-Fairness bleibt; Pacing auf 35 ms für schnelleres Auffüllen freier Slots.
5. **Regions-/Upload-Sicherheit**: Jede Antwort besitzt ihre unveränderlichen ID-/Mip-/Epoch-Metadaten, stale Regionen werden verworfen. Uploads erfolgen weiterhin *nur* seriell im GL-Thread mit Bytebudget, LOD-/Slot-/Staging-Gates. Shutdown wartet sämtliche Worker.
6. **Atomarer paralleler Diskcache**: eindeutige temporäre Dateinamen je Schreibaufruf und atomare Umbenennung nach vollständigem Checksum-/RGBA-Record; gleichzeitige Schreibvorgänge können einander keine `.part`-Datei abschneiden.
7. **Größerer persistenter Cache**: 1 GiB Default statt 512 MiB, begrenzte Überschreibung per `OGL_TEXTURE_CACHE_MIB=128..2048`, bestehende Dateien ohne Cache-Formatbruch.
8. **RAM-/GPU-Warmstart**: LRU-RGBA-RAM-Cache 128 statt 96 MiB; adaptive GL-Upload-Mindestdistanz 16 statt 30 ms. CPU-Submission-Zeit verlängert die Distanz bei Last weiterhin automatisch; GPU-VRAM-Budget unverändert.
9. **Himmel/Sonne**: Sonnen-Disc/Corona/Atmosphärenstreuung sichtbar verstärkt, abhängig von echter Kameraprojektion und Simulator-WindLight-Horizont; unter Horizont keine irreführende Nachtsonne. Revision-basierte, kennwortfreie Sky-Diagnose in Logs. Keine Bildtapete oder Änderung der echten WebKit-Welcome-Seite.
10. **Regression und Projektübergabe**: Acht gleichzeitige Diskcache-Schreibaufträge in nativen Tests; Nacht-Sonnen-Projektion; zehn zusätzliche Python-Contracts für die neue Pipeline, sämtliche vorhandenen Gates, Roadmap und Übergabe aktualisieren. Commit erst nach grünem Build/Tests.

## Geprüfte lokale Gates

- Linux-Mint-x64 CMake/vcpkg Build: PASS.
- Native CTest: **195/195 PASS**, einschließlich neuer paralleler Disk-Schreibvorgänge und WindLight-Nachtsonne.
- Python-Unittest: **944/944 PASS**, inklusive zehn neuer Pipeline-Verträge.
- Originales Firestorm Xvfb/OpenGL GUI in nativen Tests und 128px X11-Taskleistenicon: PASS.
- Authentifizierter Nachher-Grid-Test mit aktuellen Performance-/Sky-Messwerten: **OFFEN**, keine behauptete Beschleunigung in Prozent.

## Noch offen

- Realer kalter und warmer **Neustart** der neuen Binary mit Messung von Download-/Upload-Zeiten, Anzahl paralleler HTTP-Anfragen, Mip0 und tatsächlichen sichtbaren Pixeln/Render-Screenshots.
- **Zwölf HTTP-404-Assets:** Server-/Asset-Service-Daten prüfen und ggf. wiederherstellen; Viewer darf keine Textur-Erfolge erfinden.
- **3/3 Server-Avatar-Bakes:** Region/Appearance/COF auf dem Grid prüfen; lokal mögliche Vorschau ist kein echtes Server-Bake.
- Tatsächliche Himmels-/Sonnen-Darstellung bei Tageslicht und Kameraschwenk in NexVerse visuell abnehmen; WindLight-/Clouds-/PBR-Parität weiterhin offen.
- Keyring-Opt-in/Opt-out durch Nutzer ohne Auslesen oder Offenlegung realer Passwörter abnehmen.
- Bis zu diesen Belegen bleibt v0.4.0.0 IN_ARBEIT und das Produkt 38,65 % gewichtet (39 %). Kein Release/Tag/main-Merge.
