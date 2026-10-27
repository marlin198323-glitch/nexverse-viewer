# R536 – Marketplace-Hop-Link startet OpenGenesisLINK am richtigen Standort

Stand 27.10.2026 · Linux Mint · Viewer develop-current 83e106b · kein automatischer Desktop-Install und keine Simulatordatenänderungen.

## Verhalten

Ein Marketplace-Angebot hat ein ausschließlich durch den Marketplace-Server geprüftes `hop://hg.stadt-nexverse.de:80/Region/X/Y/Z`. Der Linux-Viewer kann über den von der Desktopumgebung registrierten `x-scheme-handler/hop` mit diesem URI gestartet werden. Das `.desktop`-Paket unterstützt `Exec=nexverse-viewer %u` und `MimeType=x-scheme-handler/hop;`.

Die native CLI prüft **vor** jeder Initialisierung von GLFW bzw. jedem Netzwerkzugriff die Host-Domain und den optionalen Port, den Regionsnamen sowie X/Y im Intervall 0..255 und Z im Intervall 0..4096. Nur der feste Host `hg.stadt-nexverse.de` ist erlaubt; fremde Domains, Querystrings, Fragmente, Credentials, doppelte Dekodierung, Kontrollzeichen, Pfad-Traversal, extra Segmente und Shell-Zeichen werden zurückgewiesen. Erlaubte Regionsnamen mit URL-kodierten Leerzeichen werden kontrolliert dekodiert. Ungültige URLs beenden den Prozess mit Exit 2.

Erst nach einem **bewussten regulären Login** wird der so begrenzte Zielpunkt als standardkonformer OpenSimulator-XMLRPC-Startort `uri:Regionsname&X&Y&Z` verwendet. Die bisherige normale Anmeldung ohne URI verwendet weiter `last`. Es gibt keine automatische Eingabe von Passwörtern, keine unautorisierte Region-Anmeldung und keine Freigabe fremder Grids.

## Einrichtung

Nach dem von den Viewer-Tests freigegebenen Bau und der Installation von `nexverse-viewer` kann ein Benutzer unter Linux Mint **selbst entscheiden**, ob OpenGenesisLINK zum Standard-Handler für `hop://` werden soll:

```bash
bash scripts/install-marketplace-hop-handler.sh
xdg-mime query default x-scheme-handler/hop
```

Der Script ändert die User-Defaultzuordnung **nur nach bewusster Ausführung**. Er ist kein CI-/Build-Schritt und sollte nicht stillschweigend Firestorm als bislang registrierten Viewer überschreiben. Browser wie Firefox können trotz Registrierung weiterhin eine systemeigene App-Öffnen-Bestätigung anzeigen.

## Technische Grenze

Der aktuelle OGL-Protokollpfad für einen **bereits eingeloggten Viewer** hat noch keinen abgesicherten Single-Instance-IPC-Empfänger. Ein externer Link kann daher eine **neue** Viewer-Instanz öffnen, welche beim Login direkt im Zielgebiet startet. Ein sofortiger Regionswechsel einer bereits laufenden OGL-Instanz ist **noch nicht implementiert oder getestet**; dafür sind ein authentisierter IPC-Handoff und die Cross-Region-Teleport-Simulatorbestätigung nötig. Diese wichtige Grenze darf nicht als fertig ausgegeben werden.

## Tests

`tests/marketplace_hop_r536_tests.cpp`: valide und fehlerhafte Host-/Port-/Region-/Koordinaten-/Query-/Pfad-Links; Zielsyntax wird exakt gebaut. `g++ -std=c++23 -Wall -Wextra -Werror -Iinclude` für Parser, Test und Viewer-Main: grün auf Linux Mint. CMake-CTest-Integration hinzugefügt. Vollständiger lokaler Viewer-CMake-Build **noch blockiert** durch fehlendes CURL_LIBRARY / CURL_INCLUDE_DIR auf diesem Checkout; GitHub-CI-Build muss die native Codeintegration unabhängig testen. Keine Live-Installation.
