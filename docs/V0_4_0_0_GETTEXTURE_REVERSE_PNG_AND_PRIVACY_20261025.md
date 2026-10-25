# OGL Viewer v0.4.0.0 – GetTexture-Rückwärtsformat und sichere GPU-Logs

**25.10.2026 – Linux Mint Desktop Commander `jam-MS-7D28`, GitHub `develop-current`**

Ausgangscommit: `e379769a88c39d56feee80b14f95c33749e99c12`. Ziel bleibt v0.4.0.0 ohne Freigabe für `main`.

## Gesicherter Fehlerbefund

Der GetTexture-Loader erkannte nach einem `format=png`-Request bereits echte J2C-/JP2-Bodys. Auf den ergänzenden `format=x-j2c`-Request konnte eine OpenSim-/Proxy-Implementierung dagegen mit einem intakten PNG antworten. Der Loader versuchte dort ausschließlich OpenJPEG; ein valides PNG erschien somit als defekter JPEG2000-Asset und bekam ohne Korrektur eine 30-minütige ungültig-Sperre. Dieser Pfad ist mit transportgetreuen HTTP-Testantworten reproduziert; **eine betroffene aktuelle NexVerse-Live-Antwort ist nicht nachgewiesen.**

Außerdem protokollierten zwei GPU-Fehlerpfade die vollen Textur-UUIDs im Klartext, obwohl die restliche aktuelle Avatar-/GetTexture-Diagnostik sie ausdrücklich ausspart.

## Zehn erledigte Arbeitsblöcke

1. Linux-Mint-Branch, `CHATUEBERGABE.md`, Roadmap, Projektplan und GitHub-CI geprüft; Arbeitsverzeichnis zu Beginn sauber.
2. Vorhandene realistische Login-/Textur-Logs erneut geprüft: ältere JPG2000-Metadatenablehnungen und erfolgreiche x-j2c-Fallbacks, keine neue authentifizierte Session im letzten Testabschnitt.
3. Beide Anfragepfade und die 30-minütige Invalid-Asset-Retryklassifikation gelesen; einen unbehandelten gültigen PNG-Body bei `format=x-j2c` identifiziert.
4. Die vollständige acht Byte lange PNG-Dateisignatur zu `jpeg2000_payload_signature.hpp` ergänzt. Ein teilweise vorhandener Header gilt nicht als gültiges Format.
5. Bei `x-j2c`-HTTP-200 mit nachgewiesener PNG-Signatur denselben sicheren `decode_png_texture()`-Pfad verwenden, inklusive 4.096px-Dimension, 32 MiB-Download und vollständig validiertem RGBA-Bild.
6. Bei beschädigtem PNG mit gültiger Signatur weiter `AssetFailure::invalid`, keine Erfindung von Pixeln, keine zusätzliche HTTP-Schleife und keine Freigabe eines unvollständigen Bilds.
7. Bestehenden nativen Transport-Mock um `j2c_returns_valid_png` und `j2c_returns_truncated_png` ergänzt; Requests, HTTP-Zähler, Bilddaten und Failure-Pfad überprüft.
8. Nativen Signaturtest gegen vollständigen PNG-Header, unvollständigen Header und J2C-Marker erweitert.
9. Zwei GPU-Uploadwarnungen nach dem Datenschutzprinzip auf `[asset id omitted]` umgestellt; GPU-Fehlercode, GL-Fehler und Budget-Hinweise werden weiter protokolliert. Sechs zusätzliche Python-Quellverträge für die Änderung ergänzt.
10. Vollständigen Linux-Mint-x64-Build, Native-CTest-, Python-, Firestorm-Xvfb-OpenGL-GUI- und X11-Systemicon-Gate durchgeführt; Dokumentation und Übergabe danach aktualisiert.

## Abnahmegrenzen

Dieser Fix rettet gültige PNG-Antworten auf einem `x-j2c`-Endpoint. Er erklärt **nicht**, warum historisch 33 echte JPEG2000-Metadatenablehnungen aufgetreten sind, und ist **kein Nachweis** für vollständige Body-Bakes, sichtbare Skin, COF oder installierte Kleidung. Ein neuer authentifizierter Login ist weiterhin nötig, damit `Avatar SELF bake pipeline: blocker=` mit dem tatsächlichen Client-/Simulatorzustand verglichen werden kann. 26 ms Shutdown wurden nur für einen lokalen Test ohne Login gemessen; angemeldeter Shutdown bleibt offen.

**v0.4.0.0 IN_ARBEIT, Gesamtprojekt 38,65 % (39 % gerundet). main unverändert, kein Produktivdeploy.**
