# nexverse-viewer – Projektanweisungen

## Verbindliche OGL-Arbeitsumgebung ab 16.10.2026 (neueste Nutzeranweisung)

- **Primäre interaktive Entwicklungsmaschine ist wieder der Linux-Mint-PC `jam-MS-7D28`** über Remote Desktop Commander (Device-ID `3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d`). Beim Start `list_devices` prüfen und ausdrücklich dieses Gerät auswählen.
- **Verifizierter Mint-Checkout am 16.10.2026:** /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, CMake debug mit /home/jam/vcpkg, RTX 4070 SUPER OpenGL 4.6. Originale Firestorm-Assets bleiben lokal, keine Binaries davon committen.
- **GitHub bleibt führend:** `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; vor jeder Änderung Git-Status/Remote-SHA abgleichen, lokale uncommittete Änderungen auf Mint nicht überschreiben. Änderungen dort programmieren, lokal nativ/GUI testen und danach zu GitHub pushen.
- **Raspberry Pi 5 `s359142.srv.nexvortex.de` nur als Self-hosted-CI-/ARM64-Test-Runner** für OGL einsetzen, nicht als primären interaktiven Editor, Build-/Grafikrechner oder Ersatz für den PC. Die Pi-Anweisungen unten dokumentieren den historischen Stand bis Alpha.149 und sind in dieser Hinsicht überholt.
- Falls `jam-MS-7D28` offline ist, keine PC-Checkout-/Build-/GUI-Ergebnisse behaupten. Für rein dokumentarische Aktualisierungen darf der GitHub-Connector genutzt werden; keine eigenmächtige Verlagerung der OGL-Programmierarbeit auf den Pi.
- Lokalen Mint-Pfad und tatsächlichen Git-Status bei Wiederverbindung **neu ermitteln**, nicht vom früheren Pfad ausgehen. Als letzter bestätigter GitHub-Stand liegt Alpha.149 auf `develop-current` (`b7dabf4b805d7d9beb93a4452e19a23e66a63e46`); zwischenzeitliche neue Commits immer zuerst prüfen.

## OpenGenesisLINK Viewer (OGL)-spezifisch
- GitHub: `marlin198323-glitch/nexverse-viewer`; Pi-Pfad: `/srv/nexdev/development/projects/ogl-viewer`.
- **Verbindlicher aktueller Arbeitsbranch `develop-current`, nicht `main` und nicht der ältere divergierende `develop`.** Bestehende lokale Linux-Mint-Kopie nicht löschen oder blind zurücksetzen.
- Zuerst `CHATUEBERGABE.md`, `projectplan.md`, `info.md`, Architektur- und Firestorm-Kompatibilitätsdokumentation lesen.
- Firestorm-Feature-Baseline erhalten/implementieren; vor jedem relevanten Block den aktuellen OpenGenesisLINK-Server-Stand in `opengenesislink/opengenesislinkserver` prüfen. GUI/UX parallel zu Protokoll, Bewegung, Welt-/Avatar-/Meshdarstellung entwickeln.
- Pi ARM64 eignet sich für ausgewählte Quelltext-/CMake-/Headless-Tests; vollständige Windows/macOS- und Linux-x64-GUI/GPU-Builds auf geeigneten Hostsystemen ausführen. Nicht behaupten, damit Firestorm-Parität oder GUI-E2E geprüft zu haben.

# Arbeitsanweisung für KI-Entwicklung / ChatGPT (Stand 15.10.2026)

## Zentrale Arbeitsumgebung
- Raspberry Pi 5 (ARM64), Gerät in Remote Desktop Commander: `s359142.srv.nexvortex.de`.
- Device-ID: `76f55079-2e1f-4660-a6b5-4c9ac490b3be`. Bei mehreren Geräten in ChatGPT `list_devices` ausführen und exakt diesen Pi auswählen, nicht `jam-MS-7D28`.
- Angemeldeter Linux-Arbeitsbenutzer des Commander-Dienstes: `nexdev`; Home: `/home/nexdev`. Für Programmierarbeiten keine Root-Sitzung verwenden.
- Projektbasis auf dem Pi: `/srv/nexdev/development/projects/`.
- Zentrale Pi-Anweisung: `/srv/nexdev/development/PI5_ENTWICKLUNGSREGELN.md` (nur bei Remote-Zugriff lesbar).
- Desktop Commander ist als `nexdev-commander.service` eingerichtet und war zuletzt online. Verbindung vor jedem Zugriff prüfen. Auto-Neustart eingerichtet, Neustart-Test noch nicht abgeschlossen.

## Verbindlicher Ablauf bei „weiter“
1. Zuerst dieses `AGENTS.md`, die vorhandene Chat-Übergabe, den Projektplan und den aktuellen Branch/Commit prüfen. Keine alten Statusmeldungen als gegenwärtig annehmen.
2. GitHub bleibt die führende Quelle für alle Änderungen. Entweder direkt über den GitHub-Connector programmieren oder über Desktop Commander in einem überprüften Pi-Checkout arbeiten; niemals zwei parallele Schreibvorgänge auf dieselben Dateien/Branches unkoordiniert starten.
3. Auf dem Pi vor Bearbeitung Git-Status, Remotes und Branch prüfen, neue Remote-Commits abholen und lokale Änderungen bewahren. Kein blindes `reset --hard`, `clean -fd`, Überschreiben oder erzwungenes Pushen.
4. Mehrere zusammenhängende Entwicklungsblöcke (auf Nutzerwunsch typisch 4–5 größere Blöcke) über Core, Renderer, GUI, Tests und Protokoll gemeinsam bearbeiten; Tests gezielt und reproduzierbar ausführen. Ergebnisse und offene Prüfungen unterscheiden.
5. Automatische Tests nach GitHub-Pushes werden künftig von separaten **GitHub Self-hosted Runnern** auf dem Pi ausgeführt. **Stand 15.10.2026: alle fünf Runner sind registriert und wurden mit erfolgreichem GitHub-Runnersmoke getestet. Reguläre Projekt-Tests sind davon separat zu prüfen.** GitHub sendet CI-Jobs an Runner, niemals an Desktop Commander. Commander ist für interaktive Entwicklungsarbeit/Diagnose.
6. Pi hat 8 GB RAM: umfangreiche Builds und parallele Jobs begrenzen, CPU/RAM/SSD kontrollieren. Nicht unterstützte Plattformen (z. B. Windows/macOS/x64 GUI) auf geeigneten Build-Hosts testen.
7. Entwicklungstests nur mit isolierten Benutzerrechten und Testdaten ausführen. Keine Produktion, produktive DB, echte Kundendaten oder Produktionssecrets für CI nutzen. SSH-Keys und Zugangsdaten nicht in Code/Logs/Chats ablegen.
8. GitHub-Actions-Minuten sparen: zuerst lokale gezielte Prüfungen, später Self-hosted CI und Caches, GitHub-gehostete Komplettbuilds nur für passende Releases. Bestehende CI/CD-Trigger vor Änderungen prüfen; keinen Prod-Deploy durch reine Entwicklung auslösen.
9. Pro Durchlauf geänderte Dateien, Commit-SHA, tatsächlich gelaufene Tests/Ergebnis, Fehler, offene Punkte und nächste Aufgaben in bestehenden Projekt-/Übergabedateien nachführen, ohne diese zu ersetzen oder neue widersprüchliche Pläne anzulegen.
10. Projekt-Code und CI-Konfiguration nicht als „fertig“ ausweisen, bevor echte Tests bzw. freigegebene Produktionsprüfung stattgefunden haben.

**Wichtig für ChatGPT Projects:** Dieses Repository-Dokument verändert die separaten ChatGPT-Projektanweisungen nicht automatisch. Dort einmalig Anweisung hinterlegen, zu Beginn dieses `AGENTS.md` des jeweiligen Repositories zu lesen.

## Stand Pi5-Runner-Migration (15.10.2026)
- Alle fünf Repo-Runner wurden auf Raspberry Pi 5 registriert und per GitHub Actions Smoke auf separatem Branch erfolgreich getestet.
- Pi5 develop-current CI: Quelltext-/Bash-Prüfung plus native ARM64 C++23 und Python-Regressionstests. CMake/vcpkg und Windows/macOS/Linux-x64 GUI/GPU-Builds bleiben offen.
- Ein erfolgreicher Smoke-Test beweist Runner-/Checkout-Funktion, nicht die vollständige Projekt-Testabdeckung.
