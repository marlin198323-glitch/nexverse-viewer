[Reading 947 lines from start (total: 947 lines, 0 remaining)]

# NexVerse Viewer – Projektplan

Stand: 05.10.2026  
Repository: `marlin198323-glitch/nexverse-viewer`  
Ziel: Eigenständiger, plattformübergreifender OpenSim-Viewer in C++23 unter MPL 2.0.

---

## 1. Zielbild

Der NexVerse Viewer wird kein Firestorm-Fork. Er wird als eigenständiger Viewer neu entwickelt und kommuniziert direkt mit OpenSimulator über das OpenSim/Linden-Viewer-Protokoll.

Die Architektur wird strikt getrennt:

```text
OpenSimulator
      |
      v
OpenSim Protocol Layer
      |
      v
WorldModel
      |
   +--+---+
   |      |
Renderer  UI
```

Dadurch bleiben Netzwerk, Weltzustand, Darstellung und Benutzeroberfläche voneinander entkoppelt.

Technische Basis:

- Sprache: C++23
- Buildsystem: CMake
- Paketverwaltung: vcpkg
- Lizenz: MPL 2.0
- Zielplattformen:
  - Windows x64
  - Linux x64
  - macOS x64
  - macOS ARM64 / Apple Silicon
- Netzwerk:
  - HTTPS / HTTP
  - OpenSim Login
  - LLSD
  - LLUDP
  - CAPS
- Renderer:
  - zunächst abstrahierte Render-Schnittstelle
  - erste lauffähige Implementierung voraussichtlich OpenGL/Vulkan-kompatibel
  - spätere native Backends ohne Änderung des Protocol-Core möglich

---

## 2. Grundregeln der Entwicklung

1. Der OpenSimulator-Server ist autoritativ.
2. Netzwerkcode darf niemals direkt Renderer-Objekte verändern.
3. Der Renderer darf keine OpenSim-Protokolllogik kennen.
4. Der UI-Code darf nicht direkt LLUDP-Pakete verarbeiten.
5. Jede Netzwerkantwort wird zuerst in typisierte Datenstrukturen übersetzt.
6. Das WorldModel bildet den aktuellen Zustand der Welt ab.
7. Speicherverbrauch wird von Anfang an gemessen und begrenzt.
8. Externe Daten werden validiert und mit Größenlimits verarbeitet.
9. Passwörter werden niemals als Kommandozeilenargumente oder in Logs gespeichert.
10. Neue Module erhalten Tests, bevor die nächste große Schicht darauf aufgebaut wird.

---

## 2.1 Firestorm-Funktionsbaseline – verbindlicher Mindestumfang

OpenGenesisLINK Viewer bleibt eine eigenständige Neuimplementierung und wird kein Firestorm-Fork. Funktional soll der Viewer für den normalen NexVerse-Betrieb jedoch mindestens die üblichen Kernfunktionen eines modernen Firestorm/OpenSim-Viewers abdecken.

Diese Liste ist verbindliche Produktanforderung und wird phasenweise umgesetzt.

### Anmeldung, Grid und Session
- fester NexVerse-Gridzugang
- GridInfo-Discovery bei jedem Start
- Login mit Avatarname und Passwort
- Startposition: letzte Position, Zuhause und definierte Region/Position
- Session-Aufbau und Reconnect
- Logout
- mehrere Simulator-/Regionswechsel innerhalb einer Session

### Avatar und Bewegung
- Gehen
- Rennen
- Springen
- Fliegen
- Landen
- Sitzen / Aufstehen
- Teleport
- Autopilot / Zielbewegung soweit vom OpenSim-Protokoll unterstützt
- Mouselook
- Kamera frei bewegen
- Zoom / Orbit / Fokus
- Gesten
- Animationen
- Animation Override / AO-Grundfunktion
- Attachments
- Wearables / Outfit-Basis
- Avatar Appearance aktualisieren

### Welt und Regionen
- Region laden
- Nachbarregionen
- Regionswechsel
- Terrain
- Wasser
- Sky / Environment
- Tageszeit
- Wind-/Cloud-Basisdaten soweit vom Simulator geliefert
- Objekte / Prims
- Sculpt-/Mesh-Objekte
- Texturen und Materialien
- Lichtquellen
- Partikel
- Sitzplätze
- Physics-/Collision-relevante Zustände soweit für den Viewer erforderlich
- Parcel-/Land-Daten
- Estate-/Region-Basisinformationen

### Kommunikation
- Local Chat
- Whisper / Say / Shout
- Instant Messages
- Gruppenchat
- Conference Chat soweit OpenSim unterstützt
- Chat-Verlauf lokal
- Tippstatus
- Busy / Away
- Freundschaftsanfragen
- Freundesliste
- Online-/Offline-Status
- Blockieren / Muten
- Gruppen
- Gruppenmitgliedschaften
- Gruppenrollen und Notices soweit vom Grid unterstützt
- Voice als separates Modul
- Streaming Audio / Parcel Music
- Parcel Media / Web Media

### Inventar
- kompletter Inventarbaum
- Ordner
- Items
- Assets laden
- Copy / Move / Rename / Delete
- Trash
- Lost & Found
- Rezzen aus Inventar
- Take / Take Copy
- Give Inventory
- Attach / Detach
- Wear / Remove
- Outfits
- Gestures
- Landmarks
- Notecards
- Scripts
- Textures
- Sounds
- Animations
- Objects
- Mesh / Asset-Unterstützung entsprechend OpenSim
- Inventory Offers und Accept/Decline

### Karte, Orte und Navigation
- World Map
- Mini Map / Radar
- eigene Position
- Avatarpositionen soweit freigegeben
- Regionen
- Landmarks
- Favoriten
- Home
- Teleport History
- Location Bar
- Koordinaten
- SLurl-/hop-/OpenSim-URI-Verarbeitung soweit für NexVerse relevant

### Freunde, Profile und soziale Funktionen
- Profil anzeigen
- Profilbild
- About / Picks soweit verfügbar
- Freunde hinzufügen / entfernen
- Rechte für Freunde
- Nearby Avatars / Radar
- Gruppenprofil
- Gruppenbeitritt / Austritt
- Notices
- Mute-/Block-Liste

### Bauen und Bearbeiten
- Prims erstellen
- Position / Rotation / Scale
- Link / Unlink
- Duplicate
- Delete
- Take / Return soweit berechtigt
- Texture / Color
- Transparency
- Glow
- Full Bright
- Materialparameter soweit OpenSim unterstützt
- Flexible / Light / Physics-Eigenschaften soweit unterstützt
- Object Contents
- Permissions
- Verkaufspreis / Verkaufseinstellungen
- Script- und Notecard-Inhalte öffnen/bearbeiten
- Object Inspect / Creator / Owner
- Land Build-/Rez-Rechte beachten

### Grundstücke und Land
- Parcelinformationen
- Eigentümer
- Area
- Traffic soweit geliefert
- Access / Ban
- Media
- Music
- Landing Point
- Set Home soweit erlaubt
- About Land
- Buy Land / Abandon / Divide / Join nur sofern vom NexVerse-Grid erlaubt und serverseitig unterstützt

### Suche und Entdecken
- Personen
- Gruppen
- Orte
- Regionen
- Events
- Land
- Marketplace-Anbindung nur über eine dafür vorgesehene NexVerse-/Web-Schnittstelle

### UI-Standardfunktionen
- Toolbar
- konfigurierbare Panels
- Notifications
- Dialoge
- Preferences
- Audio-Lautstärken
- Voice-Steuerung
- Kamera-Steuerung
- Grafikoptionen
- Netzwerk-/Cacheoptionen
- Tastaturbelegung
- Mausbelegung
- Vollbild / Fenster
- mehrere Auflösungen
- HiDPI
- Dark Theme / NexVerse Branding
- lokalisierbare UI
- Screenshot-Funktion

### Diagnose und Sicherheit
- Netzwerkstatus
- Ping / Packet Loss
- FPS
- Simulatorstatus
- Circuit-/CAPS-Status
- sichere Credential-Behandlung
- keine Passwörter in Logs
- Asset-/Netzwerkgrößenlimits
- Timeouts
- Reconnect
- Crash-/Fehlerdiagnose ohne sensible Daten

### Nicht automatisch Teil der Mindestbaseline

Spezielle Firestorm-Power-User-Erweiterungen wie RLV/RLVa, sehr spezielle Phototools, Firestorm-spezifische Debug-Optionen oder proprietäre Service-Integrationen werden nicht automatisch übernommen. Solche Funktionen kommen nur hinzu, wenn sie für NexVerse ausdrücklich benötigt werden.

Die UI muss nicht wie Firestorm aussehen. Maßgeblich bleibt das definierte OpenGenesisLINK-/NexVerse-Zieldesign. Firestorm dient hier ausschließlich als funktionaler Referenzumfang.

---

# 3. Projektphasen

## Phase 0 – Projektgrundlage

Ziel: Das Repository kann auf Windows, Linux und macOS kompiliert und getestet werden.

Aufgaben:

- MPL-2.0-Lizenz
- CMake-Projekt
- CMakePresets
- vcpkg-Konfiguration
- Basis-Verzeichnisstruktur
- Viewer-Versionierung
- Logging
- Fehlerbehandlung
- Konfigurationssystem
- Unit-Test-Grundlage
- GitHub Actions für Linux, Windows und macOS
- Debug- und Release-Builds

Ergebnis:

```text
nexverse-viewer --version
```

liefert beispielsweise:

```text
NexVerse Viewer 0.1.0-alpha.1
```

Status: ALS NÄCHSTES

---

## Phase 1 – OpenSim Grid Discovery

Ziel: Der Viewer erkennt einen OpenSimulator-Gridserver und verwendet beim Start immer die aktuell vom NexVerse-Grid veröffentlichten Grid-Daten.

### Verbindlicher NexVerse Discovery-Endpunkt

Für NexVerse ist die feste Grid-Adresse:

```text
hg.stadt-nexverse.de
```

Der Discovery-Endpunkt lautet:

```text
http://hg.stadt-nexverse.de/get_grid_info
```

Dieser Endpunkt ist die maßgebliche Quelle für die Grid-Konfiguration des Viewers.

Verbindliche Produktregel für den NexVerse-Build:
- Der Discovery-Endpunkt ist fest im Viewer verdrahtet und kann in der normalen Anwendung nicht geändert werden.
- Es gibt keine Grid-Auswahl und kein Eingabefeld für eine alternative Login-/Grid-Adresse.
- Bei jedem Start wird ausschließlich `http://hg.stadt-nexverse.de/get_grid_info` neu abgefragt.
- Die vom Server aktuell gelieferte `login`-URI und die übrigen GridInfo-Werte werden anschließend verwendet.
- Ändert sich die XML auf dem NexVerse-Server, wird die Änderung beim nächsten Start automatisch übernommen.

### Verhalten bei jedem Viewer-Start

Bei jedem Start des NexVerse Viewers muss vor dem eigentlichen Login automatisch `get_grid_info` abgerufen werden.

Ablauf:

```text
Viewer startet
      |
      v
GET http://hg.stadt-nexverse.de/get_grid_info
      |
      v
XML sicher parsen und validieren
      |
      v
Änderungen gegenüber lokalem Cache erkennen
      |
      v
aktuelle GridInfo übernehmen
      |
      v
Login-Oberfläche / Login-Endpunkt aktualisieren
      |
      v
Benutzer kann sich anmelden
```

Wichtig:

- Die GridInfo darf nicht dauerhaft hart im Viewer dupliziert werden.
- Bei jedem Start wird die XML neu vom Server geladen.
- Änderungen an der XML müssen ohne neues Viewer-Release wirksam werden.
- Der Viewer verwendet immer die aktuell erfolgreich validierten Serverwerte.
- Relevante Änderungen werden im Log protokolliert, jedoch ohne Zugangsdaten oder andere sensible Werte.
- Die letzte erfolgreich validierte GridInfo wird lokal als Cache gespeichert.
- Ist der Discovery-Endpunkt vorübergehend nicht erreichbar, darf der Viewer die letzte gültige gecachte Version verwenden und muss den Offline-/Fallback-Zustand sichtbar kennzeichnen.
- Eine fehlerhafte, unvollständige oder nicht parsebare XML darf niemals eine zuvor gültige Konfiguration überschreiben.
- Netzwerkantworten erhalten Timeout- und Größenlimits.
- Später soll HTTPS verwendet werden, sobald der Discovery-Endpunkt dafür zuverlässig bereitsteht; bis dahin wird der angegebene HTTP-Endpunkt unterstützt.
- Nach erfolgreichem Abruf werden ausschließlich die vom Server gelieferten aktuellen Werte für Login, Welcome, Economy, About, Register, Help, Password und Web-Profile verwendet, sofern vorhanden.

Implementierung:

- HTTP-Client
- URI-Parser
- `GET /get_grid_info`
- XML-Auswertung
- GridInfo-Datenmodell
- Timeouts
- TLS-Prüfung
- Antwortgrößenlimit
- Fehlerbehandlung

Daten unter anderem:

- gridname
- gridnick
- login
- welcome
- economy
- about
- register
- help
- password
- web_profile_url

CLI-Test:

```bash
./nexverse-viewer --grid-info https://grid.example.org
```

Erwartetes Ergebnis:

```text
Grid: NexVerse
Login URI: https://...
Status: compatible
```

---

## Phase 2 – OpenSim Login ✅ abgeschlossen 05.10.2026

Ziel: Ein Benutzer kann sich authentifizieren.

Implementierung:

- Login Request
- LLSD/XML-RPC/HTTP-Verarbeitung entsprechend OpenSim-Endpunkt
- sichere Credential-Verarbeitung
- Session-Datenmodell

Relevante Login-Daten:

- Agent UUID
- Session UUID
- Secure Session UUID
- Circuit Code
- Seed Capability
- Region
- Simulator IP
- Simulator UDP Port
- Inventory Skeleton
- Buddy-/Appearance-Basisdaten

Ergebnis:

```text
LOGIN OK
Agent: ...
Region: ...
Simulator: ...
```

---

## Phase 3 – LLUDP Transport 🚧 in Arbeit seit 05.10.2026

Ziel: Stabile Echtzeitverbindung zum Simulator.

Implementierung:

- UDP Socket
- Circuit-Aufbau
- OpenSim/Linden Message Template
- Packet Encoder
- Packet Decoder
- Sequenznummern
- ACK
- Resend
- Reliable Messages
- Packet Throttling
- MTU-Grenzen
- Timeout/Reconnect

Wichtig:

Dieser Layer darf noch keinerlei Renderer enthalten.

---

## Phase 4 – CAPS / HTTP Capabilities

Ziel: Der Viewer verwendet die vom Simulator bereitgestellten Capability-Endpunkte.

Implementierung:

- Seed Capability
- Capability Discovery
- HTTP GET/POST
- LLSD XML/Binary
- EventQueue
- SimulatorFeatures

Später verwendete Bereiche:

- Inventory
- Assets
- Mesh
- Texturen
- Uploads
- Environment
- Appearance

---

## Phase 5 – WorldModel

Ziel: Ein rendering-unabhängiges Abbild der Welt.

Struktur:

```text
WorldModel
├── Grid
├── Region
├── Agent
├── Avatar
├── Object
├── Terrain
├── Parcel
└── Environment
```

Das WorldModel wird ausschließlich durch validierte Protocol-Ereignisse aktualisiert.

---

## Phase 6 – Fenster, Input und Renderer-Grundlage

Ziel: Erstes echtes Viewer-Fenster.

Funktionen:

- Desktop-Fenster
- Tastatur
- Maus
- Kamera
- Resize
- VSync
- Frame Timing
- Renderer Interface
- Debug Overlay

Erste Darstellung:

- Hintergrund
- Kamera
- Testgeometrie
- FPS
- RAM
- VRAM soweit verfügbar

---

## Phase 7 – Region und Terrain

Ziel: Eine echte OpenSim-Region darstellen.

Funktionen:

- Terrain-Daten empfangen
- Terrain Mesh
- Wasser
- Region-Grenzen
- Umgebung
- progressive Nachladung

---

## Phase 8 – Prims und Scene Objects

Ziel: OpenSim-Objekte sichtbar machen.

Funktionen:

- ObjectUpdate
- Transform
- Rotation
- Scale
- Parent/Child
- Linksets
- Primitive Types
- Object Lifecycle
- Culling

Zunächst einfache Materialien, danach vollständige Texturen/PBR.

---

## Phase 9 – Asset-System

Ziel: Assets sicher und speichereffizient laden.

Asset-Typen:

- Textures
- Meshes
- Animations
- Sounds
- Wearables
- Notecards
- Scripts nur als Inventarinhalt
- weitere OpenSim Asset-Typen

Cache:

```text
RAM Asset Cache
      |
      v
Disk Cache
      |
      v
OpenSim Asset Service / CAPS
```

Vorgaben:

- LRU/ähnliche Eviction
- Größenlimit
- Content-Hash/Versionierung
- keine vollständige Inventar-Vorladung
- sichtbare Assets priorisieren

---

## Phase 10 – Avatar

Ziel: Eigener und fremde Avatare darstellen.

Funktionen:

- Avatar Skeleton
- Shape
- Skin
- Wearables
- Attachments
- Bakes
- Animationen
- Name Tags
- Sit/Stand

---

## Phase 11 – Bewegung

Ziel: Avatar-Steuerung.

Funktionen:

- WASD
- Laufen
- Rennen
- Springen
- Fliegen
- Sitzen
- Kamera
- serverautoritatives Movement
- später Client Prediction / Reconciliation

---

## Phase 12 – Inventar

Funktionen:

- Ordner
- Items
- Suche
- Drag & Drop
- Rez
- Wear
- Attach
- Delete
- Create
- Rename
- Move
- Asset Referenzen

---

## Phase 13 – Kommunikation

Funktionen:

- Local Chat
- IM
- Offline Messages
- Notifications
- Block/Mute
- Freunde
- Presence

---

## Phase 14 – Teleport und Regionen

Funktionen:

- Teleport
- Home
- Landmarks
- Region Crossing
- Map
- Destination Handling
- Fehler-Recovery

---

## Phase 15 – Gruppen und Profile

Funktionen:

- Profile
- Friends
- Groups
- Rollen
- Einladungen
- Notices
- Group Chat

---

## Phase 16 – Build Tools

Funktionen:

- Rez
- Move
- Rotate
- Scale
- Link/Unlink
- Texture
- Material
- Permissions
- Object Properties

---

## Phase 17 – Land / Parcels

Funktionen:

- Parcel Grenzen
- Parcel Informationen
- Access Lists
- Ban Lists
- Media
- Music URL
- Land Settings
- Terrain Editing

---

## Phase 18 – Hypergrid

Ziel: Kompatible Reisen in andere OpenSim-Grids.

Funktionen:

- Hypergrid URI
- Gatekeeper
- Foreign Agent
- Foreign Inventory
- Asset Transfer
- Teleport
- Identity Handling

---

## Phase 19 – Voice

Voice wird als Adapter implementiert und nicht fest in den World-/Protocol-Core eingebaut.

```text
Voice API
├── Provider A
├── Provider B
└── später NexVerse Voice
```

---

## Phase 20 – NexVerse Integration

Optionale NexVerse-spezifische Funktionen:

- Bürgerportal
- Events
- Marketplace
- Support
- Update Service
- News
- Regionsuche
- NexVerse Account Services

Diese Funktionen dürfen die normale OpenSim-Kompatibilität nicht beschädigen.

---

## Phase 21 – Optimierung

Speicher:

- Texture Budget
- Mesh Budget
- Asset Cache Budget
- Region Cache
- Avatar Complexity
- HTTP Buffer Limits
- UI Memory
- Hintergrund-Regionen entladen

Performance:

- Frustum Culling
- Occlusion
- LOD
- Instancing
- Texture Streaming
- Mesh Streaming
- Multithreading
- Worker Queues

Messwerte:

- RAM
- VRAM
- CPU
- GPU
- Netzwerk
- Frame Time
- Asset Download Rate

---

## Phase 22 – Release

Release-Kanäle:

- alpha
- beta
- stable

Plattformpakete:

- Windows Installer
- Linux AppImage oder vergleichbares Paket
- macOS App Bundle / DMG

---

# 4. Branch-Modell

Nach dem ersten Commit:

```text
main
  |
  +-- develop
       |
       +-- feature/grid-info
       +-- feature/login
       +-- feature/lludp
       +-- feature/caps
       +-- ...
```

Regeln:

- `main` = stabiler Stand
- `develop` = aktuelle Entwicklung
- Features in eigenen Branches
- Pull Request nach `develop`
- Releases von `develop` nach `main`

---

# 5. Versionierung

Schema:

```text
MAJOR.MINOR.PATCH-STAGE.N
```

Beispiele:

```text
0.1.0-alpha.1
0.1.0-alpha.2
0.2.0-alpha.1
0.9.0-beta.1
1.0.0
```

---

# 6. Lokales Kompilieren

## Linux – Debian/Ubuntu

### Grundpakete

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  ninja-build \
  git \
  curl \
  zip \
  unzip \
  tar \
  pkg-config
```

vcpkg einmalig installieren:

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

Umgebungsvariable setzen:

```bash
export VCPKG_ROOT="$HOME/vcpkg"
```

Optional dauerhaft:

```bash
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc
source ~/.bashrc
```

Repository holen:

```bash
cd ~
git clone https://github.com/marlin198323-glitch/nexverse-viewer.git
cd nexverse-viewer
```

Später den Entwicklungsbranch verwenden:

```bash
git checkout develop
git pull
```

Debug konfigurieren:

```bash
cmake --preset debug
```

Kompilieren:

```bash
cmake --build --preset debug
```

Tests:

```bash
ctest --preset debug --output-on-failure
```

Viewer starten:

```bash
./build/debug/nexverse-viewer
```

Release Build:

```bash
cmake --preset release
cmake --build --preset release
ctest --preset release --output-on-failure
```

---

## Windows 11

Benötigt:

- Visual Studio 2022 oder neuer
- Desktop development with C++
- CMake
- Git
- vcpkg
- Windows SDK

PowerShell:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

$env:VCPKG_ROOT="C:\vcpkg"

git clone https://github.com/marlin198323-glitch/nexverse-viewer.git
cd nexverse-viewer

git checkout develop

cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

Start:

```powershell
.\build\debug\nexverse-viewer.exe
```

Release:

```powershell
cmake --preset release
cmake --build --preset release
ctest --preset release --output-on-failure
```

---

## macOS

Benötigt:

```bash
xcode-select --install
brew install cmake ninja git
```

vcpkg:

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT="$HOME/vcpkg"
```

Viewer:

```bash
git clone https://github.com/marlin198323-glitch/nexverse-viewer.git
cd nexverse-viewer
git checkout develop

cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

---

# 7. Nach jedem Update testen

Linux:

```bash
cd ~/nexverse-viewer

git checkout develop
git pull

cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure

./build/debug/nexverse-viewer
```

Wenn nur kleine Änderungen vorhanden sind, muss CMake nicht jedes Mal alles neu kompilieren. Der Build ist inkrementell.

---

# 8. Clean Build

Wenn Abhängigkeiten oder CMake-Konfigurationen geändert wurden:

Linux/macOS:

```bash
rm -rf build
cmake --preset debug
cmake --build --preset debug
```

Windows PowerShell:

```powershell
Remove-Item -Recurse -Force build
cmake --preset debug
cmake --build --preset debug
```

---

# 9. Fehlerberichte beim Testen

Für jeden Test benötigen wir mindestens:

```text
Betriebssystem:
CPU:
RAM:
GPU:
Viewer-Version:
Commit:
Grid:
Region:

Fehler:
...

Letzte Logzeilen:
...
```

Später bekommt der Viewer dafür eine automatische Diagnosefunktion.

---

# 10. Unmittelbar nächste Arbeit

Die nächsten Arbeitsschritte sind:

1. Repository-Grundgerüst erstellen.
2. MPL 2.0 hinzufügen.
3. CMake + CMakePresets einrichten.
4. vcpkg einrichten.
5. Viewer-Version implementieren.
6. Logging und Tests einrichten.
7. GitHub Actions für Linux, Windows und macOS hinzufügen.
8. `develop` erstellen.
9. Ersten erfolgreichen lokalen Build sicherstellen.
10. Danach Phase 1: OpenSim `get_grid_info`.

Erst wenn Phase 0 auf allen Zielplattformen reproduzierbar baut, beginnt die eigentliche OpenSim-Protokollentwicklung.

[executed on device: jam-MS-7D28 (3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d)]