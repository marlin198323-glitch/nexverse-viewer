# OpenGenesisLINK Viewer für NexVerse – Arbeitsstand

> **WICHTIG:** `projectplan.md` ist für Architektur, Reihenfolge und Projektumfang immer maßgebend.
> Diese Datei ist ausschließlich das fortlaufende Arbeitsprotokoll und der Wiedereinstiegspunkt für neue Chats.

Stand: 05.10.2026  
Arbeitsbranch: `develop`  
Repository: `marlin198323-glitch/nexverse-viewer`

---

## 1. Wiedereinstieg in einem neuen Chat

Wenn der Benutzer in einem neuen Chat **„viewer“** schreibt, ist der Arbeitsablauf:

1. GitHub-Repository `marlin198323-glitch/nexverse-viewer` öffnen.
2. Zuerst `projectplan.md` lesen.
3. Danach diese `info.md` lesen.
4. Aktuellen Stand von `develop`, letzte Commits und CI prüfen.
5. Keine bereits erledigten Schritte neu erfinden oder erneut aufbauen.
6. Beim nächsten offenen Arbeitspunkt aus dieser Datei weiterarbeiten.
7. Nach einem relevanten Entwicklungsblock diese `info.md` aktualisieren.
8. Wenn `info.md` und `projectplan.md` voneinander abweichen, gilt immer `projectplan.md`.

Der Benutzer möchte nicht nach jedem kleinen Commit erneut lokal kompilieren. Erst wenn ein sinnvoller sichtbarer oder technisch abgeschlossener Teststand erreicht und CI grün ist, sollen die konkreten Update-/Compile-Befehle ausgegeben werden.

---

## 2. Festgelegte Produktarchitektur

OpenGenesisLINK Viewer ist in diesem Projekt der eigenständige OpenSim-Viewer für NexVerse und kein Firestorm-Fork.

Technische Basis:

- C++23
- CMake 3.25+
- vcpkg
- MPL 2.0
- GLFW
- GLAD
- OpenGL 3.3 Core als erster plattformübergreifender Bootstrap-Renderer
- FreeType
- spätere Renderer-Abstraktion für modernere Backends
- Windows x64
- Linux x64
- macOS x64 / ARM64

Architekturregel:

```text
OpenSimulator
      |
      v
OpenSim Protocol
      |
      v
WorldModel
      |
   +--+---+
   |      |
Renderer  UI
```

Protocol, WorldModel, Rendering und UI bleiben voneinander getrennt.

---

## 3. Verbindliches NexVerse Grid Discovery

NexVerse Grid:

```text
hg.stadt-nexverse.de
```

GridInfo:

```text
http://hg.stadt-nexverse.de/get_grid_info
```

Verbindliches Verhalten:

- Bei jedem Viewer-Start wird `get_grid_info` erneut abgerufen.
- Die XML ist die maßgebende dynamische Grid-Konfiguration.
- Änderungen auf dem Server müssen beim nächsten Start ohne Viewer-Update übernommen werden.
- XML wird validiert und mit Größen-/Timeout-Grenzen verarbeitet.
- Eine fehlerhafte Antwort überschreibt niemals die letzte gültige GridInfo.
- Die letzte gültige Antwort wird als Fallback gecacht.
- Login-, Welcome-, Economy-, About-, Register-, Help-, Password- und Web-Profile-Werte werden aus der aktuellen GridInfo verwendet, sofern vorhanden.
- Keine dauerhaft duplizierten GridInfo-Werte im Viewer.
- HTTPS soll verwendet werden, sobald der Discovery-Endpunkt zuverlässig HTTPS anbietet.

Die vollständige Anforderung steht in `projectplan.md` Phase 1.

---

## 4. Phase-0-Stand

Version:

```text
0.1.0-alpha.1
```

Bereits vorhanden:

- MPL-2.0-Lizenz
- CMake-Projekt
- CMakePresets
- vcpkg Manifest
- `main` und `develop`
- Core-Versionierung
- Logging
- Unit-Test-Grundlage
- GitHub Actions
- Linux-Build
- Windows-x64-Build
- macOS-Build
- GLFW Desktop-Fenster
- OpenGL 3.3 Core Context
- GLAD
- FreeType Text Rendering
- eigene einfache native UI-Rendering-Schicht
- NexVerse Dark/Cyan/Violet Theme
- Phase-0 Login-Shell
- Phase-0 World-Preview
- Statusanzeige
- `--version`
- `--help`

Wichtige Dateien:

```text
CMakeLists.txt
CMakePresets.json
vcpkg.json
VERSION
apps/viewer/main.cpp
src/app/application.cpp
src/ui/ui_renderer.cpp
src/core/logger.cpp
src/core/version.cpp
include/nexverse/
docs/ARCHITECTURE.md
docs/DESIGN.md
projectplan.md
```

---

## 5. Erster realer Linux-Test

Der Benutzer hat Phase 0 am 05.10.2026 erfolgreich lokal gebaut und den Viewer gestartet.

Lokaler Arbeitsordner:

```text
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer
```

vcpkg:

```text
/home/jam/vcpkg
```

GitHub-Zugriff für dieses Projekt erfolgt getrennt über einen eigenen SSH-Key und Host-Alias, damit andere GitHub-Projekte nicht beeinflusst werden:

```text
Host: github-nexverse
Key: ~/.ssh/nexverse_viewer_github
```

Der erste Versuch lag unter `OGL Viewer` mit Leerzeichen. Ein autotools/vcpkg-Port (`pthread-stubs`) konnte den Pfad nicht korrekt verarbeiten. Der Benutzer hat den Ordner anschließend in `OGL-Viewer` umbenannt. Der reale Viewer-Build startete danach erfolgreich.

---

## 6. Aktueller sichtbarer UI-Stand

Der reale Phase-0-Build zeigt derzeit:

- sehr dunkle native Desktop-Oberfläche
- Kopfzeile `NEXVERSE VIEWER`
- Versionsanzeige rechts
- linke Login-Karte
- Grid-/Server-URL
- Benutzername
- Passwort
- Startregion
- `WELT BETRETEN`
- große World-Preview-Fläche
- abstrakte Stadt-Silhouette
- Status-Karte:
  - Desktop shell READY
  - Renderer READY
  - OpenSim protocol NEXT
  - World stream PENDING

Dieser Bildschirm ist aktuell eine Phase-0-Design-/Renderer-Demonstration. Die Login-Felder sind noch nicht mit OpenSim verbunden.

---

## 7. Produktname und Desktop-Icon

Produktname:

```text
OpenGenesisLINK Viewer
```

Einsatz/Zielwelt:

```text
NexVerse
```

Das vom Benutzer bereitgestellte OpenGenesisLINK-Viewer-Icon ist das vorgesehene Desktop-/Anwendungsicon dieses Projekts und keine bloße Stilreferenz.

Es soll später für alle Zielplattformen integriert werden:

- Linux Desktop/Icon-Paket
- Windows EXE/Installer `.ico`
- macOS App Bundle/`.icns`
- Viewer-Fenstericon, soweit die Plattform dies unterstützt

Das Repository heißt weiterhin `nexverse-viewer`; Produktname und Repositoryname müssen nicht identisch sein.

---

## 8. Designreferenz

Der Benutzer hat zwei visuelle Referenzen bereitgestellt:

### A. Tatsächlicher Phase-0-Build

Der erste Screenshot zeigt den real laufenden Linux-Build. Er beweist, dass Desktop-Fenster, GL-Kontext, UI-Renderer und Schrift-Rendering funktionieren.

### B. Zielbild NexVerse In-World UI

Die zweite Referenz zeigt die gewünschte spätere In-World-Oberfläche:

- großes NexVerse-Logo oben links
- dominante 3D-Welt in der Mitte
- Topbar mit Region, FPS/Ping, Voice, Notifications, Friends, Settings
- linke Navigation:
  - Start
  - Welt
  - Orte
  - Freunde
  - Gruppen
  - Events
  - Inventar
  - Marketplace
- Benutzerkarte unten links
- rechte kontextabhängige Seitenleiste:
  - In der Nähe
  - Region
  - Orte
- Nearby-Avatare
- Regionsinformationen
- Events
- Chat unten
- Bottom-Dock:
  - Chat
  - Sprechen
  - Gesten
  - Bewegen
  - Teleport
  - Kamera
  - Umsehen
  - Inventar
  - Bauen

Die In-World-Seitenleisten sollen später einklappbar sein, damit die 3D-Welt den Hauptteil des Fensters einnimmt.

Wichtig: Die Phase-0-Login-Shell ist nicht das fertige In-World-HUD. Das Zielbild wird schrittweise nach dem World-/Renderer-Unterbau umgesetzt.

---

## 9. Aktuelle Design-/UI-Bewertung

Der aktuelle Phase-0-Build funktioniert technisch, ist optisch aber noch deutlich einfacher als das Zielbild.

Noch zu verbessern:

- NexVerse-Logo statt reiner Textmarke
- saubere responsive Skalierung
- HiDPI / 4K-Skalierung
- bessere visuelle Hierarchie
- konsistentere Abstände
- hochwertigere Karten/Panels
- Hover/Focus/Pressed-Zustände
- echte Texteingabe
- Passwort-Masking als echter UI-Zustand
- Tastaturnavigation
- Button-Interaktion
- Fenster-/App-Icon
- spätere Trennung Login-Ansicht / In-World-Shell
- Internationalisierung vorbereiten

Kein Pixel-für-Pixel-Nachbau von Firestorm oder OpenGenesisLINK.

---

## 10. CI-Status

Phase-0 CI wurde bereits erfolgreich auf folgenden Zielsystemen ausgeführt:

- Linux x64
- Windows x64 / MSVC
- macOS

Bekannter erfolgreicher Stand vor den reinen Dokumentations-/Pfadänderungen:

```text
b6d4fe7dc005eb66b5c27085c25cfe5bd35322c2
ci: pin Windows build to MSVC x64
```

---

## 11. Letzte wichtige Commits

```text
de3202b build: support Unix source paths containing spaces
d21e492 docs: define live NexVerse grid discovery on startup
b6d4fe7 ci: pin Windows build to MSVC x64
bfef1da ci: install Linux autotools for vcpkg
ff4f213 fix: finish GLAD migration in CMake
9a2c4cf app: implement NexVerse phase zero graphical shell
4447a5e ui: implement OpenGL and FreeType renderer
```

Hinweis:

Der Benutzer hat den lokalen Ordner inzwischen auf `OGL-Viewer` ohne Leerzeichen umbenannt. Der Commit `de3202b` mit dem Unix-Pfad-Workaround ist damit nicht mehr zwingend erforderlich und soll bei der nächsten Build-Bereinigung geprüft bzw. entfernt werden, sofern dadurch keine gewünschte Portabilität verloren geht.

---

## 12. Nächster Arbeitsblock

Weiterarbeit innerhalb **Phase 0**, bevor Phase 1 begonnen wird:

1. Build-Konfiguration nach der lokalen Ordnerumbenennung bereinigen.
2. Phase-0 UI in klar getrennte Komponenten zerlegen statt alles dauerhaft in `application.cpp` zu halten.
3. UI-Skalierungsmodell für 1080p / 1440p / 4K vorbereiten.
4. Login-Formular als echten UI-Zustand implementieren:
   - Fokus
   - Maus
   - Tastatur
   - Textfelder
   - Passwortmaskierung
   - Startregion
   - Button
5. Noch **keinen echten Login** senden.
6. Grid-Adresse im UI künftig nicht als manuell notwendige Konfiguration behandeln; NexVerse Discovery wird automatisch über `get_grid_info` erfolgen.
7. NexVerse Branding/App-Icon vorbereiten.
8. Tests für UI-State und Startkonfiguration ergänzen.
9. CI wieder vollständig grün prüfen.
10. Dann einen neuen lokalen Teststand an den Benutzer geben.

Erst danach beginnt gemäß `projectplan.md`:

```text
Phase 1 – OpenSim Grid Discovery
GET http://hg.stadt-nexverse.de/get_grid_info
```

---

## 13. Arbeitsregel für zukünftige Änderungen

Nach jedem größeren abgeschlossenen Block:

- Code committen
- Tests/CI prüfen
- `info.md` aktualisieren
- offenen nächsten Schritt eintragen

`projectplan.md` wird nur geändert, wenn sich Anforderungen, Phasen oder verbindliche Architekturentscheidungen ändern.

`info.md` wird geändert, wenn sich der tatsächliche Entwicklungsstand ändert.
