# v0.4.0.0 – OpenSim GetTexture: JPEG2000 trotz PNG-Antwort

**25.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current` | Ausgangscommit `029f9ac3859527a8a8ee6562ad5dd145fe793416`.**

## Anlass und gesicherter Befund

Das lokale Viewer-Laufzeitlog umfasst 33 historische Meldungen `JPEG2000 image metadata exceeds safe limits`. Insgesamt wurden mehrere Tausend `GetTexture x-j2c fallback succeeded`-Meldungen gezählt, häufig nach `Texture PNG header is invalid`. Dies belegt die hohe Bedeutung des nativen JPEG2000-Pfades, **nicht** aber, dass die PNG-Antwort tatsächlich JPEG2000 enthielt – der konkrete Antwort-Body wurde nicht gespeichert. Der Viewer holte bisher für jede PNG-Anfrage, deren PNG-Decoder fehlschlug, unabhängig vom tatsächlichen Bytestream eine separate `format=x-j2c`-Antwort.

## Zehn Bearbeitungsblöcke

1. `CHATUEBERGABE.md`, `ROADMAP.md`, `projectplan.md`, GitHub-`develop-current`, sauberen Linux-Mint-Checkout und CI geprüft.
2. Vorhandene Viewer-Logs zu PNG-Headerfehlern, J2C-Fallback-Erfolgen und 33 Metadatenablehnungen ohne Asset-UUIDs, vollständige URLs oder Sitzungstoken statistisch untersucht.
3. Den tatsächlichen `TextureAssetLoader::fetch_rgba`-Pfad von `format=png` über HTTP 200 bis `format=x-j2c` und OpenJPEG geprüft.
4. Neue reine Payload-Signaturprüfung für natives OpenSim-J2C (SOC-Marker `FF 4F`) und standardisierte JP2-Boxsignatur eingeführt.
5. Bei gültigem PNG-HTTP-200 mit J2C-Signatur den bereits vorhandenen sicheren JPEG2000-Decoder statt fälschlich den PNG-Decoder verwenden; Erfolg erfordert vollständig valide RGBA-Pixel.
6. Dieselbe Behandlung für vollständige JP2-Dateien; ein erfolgreicher validierter Download benötigt keine zweite HTTP-Anfrage.
7. **Keine Format-Erfindung:** Bei fehlender Signatur, abgeschnittenem SOC/JP2 oder fehlgeschlagenem OpenJPEG-Decode weiterhin die zweite, unabhängig geholte `x-j2c`-Antwort prüfen. Ein Decoderfehler ist nie ein echter Texturerfolg.
8. C++-Mocktests für J2C auf PNG-Endpoint (1 GET), JP2 auf PNG-Endpoint (1 GET), positives aber abgeschnittenes J2C (2 GETs), normalen PNG-Erfolg und bisherigen J2C-Fallback erweitert.
9. Nativen isolierten Signatur-CTest plus fünf Python-Verträge ergänzt, einschließlich der unveränderten 32-MiB-komprimiert/4.096px/64-MiB-RGBA-/16-Bit-Sicherheitsgrenzen.
10. Vollständige Mint-x64-Build-/CTest-/Python-/Xvfb-Firestorm-OpenGL-/GUI-/128px-X11-Icon-Gates, Projektstand, Roadmap und Übergabe aktualisiert. Ergebnisse siehe Projektstatus und CI für den entsprechenden Commit.

## Weiter offen

Die ursprünglichen 33 Metadatenablehnungen waren aus einem **älteren authentifizierten Lauf**. Dessen Bytes liegen nicht für eine erneute lokale Dekodierung vor. Dieser Fix behebt die Klasse *gültiges J2C/JP2 statt angefordertem PNG*; er ist **kein Beleg**, dass das ursprünglich abgewiesene JPEG2000-Asset repariert wurde. Die weitergehende Diagnose kann erst bei einem authentifizierten Viewer-Lauf anhand des neu eingeführten `reason=`-Schlüssels erfolgen.

Weiterhin offen: Echte Head-/Upper-/Lower-Bakes 3/3, COF/Outfit, AvatarAppearance-Textur-Readback, fehlende reale Mesh-Assets, WASD/Flug nach Teleport und Gesamtdauer des Schließens nach angemeldetem Login.

**Version `v0.4.0.0` IN_ARBEIT; Gesamtfortschritt 38,65 % (~39 %). Kein `main`-Merge, Tag oder Deployment.**
