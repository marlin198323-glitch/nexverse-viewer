# OpenGenesisLINK Viewer

OpenGenesisLINK Viewer ist ein eigenständiger nativer OpenSim/OpenGenesisLINK-Viewer für NexVerse in C++23.

**Verbindliche Zielarchitektur (11.10.2026):** Firestorm bildet die **vollständige Funktions- und technische Implementierungsbaseline**, OpenGenesisLINK Server den autoritativen OpenSim-/Serverprotokollvertrag. Bestehende Firestorm-Funktionen werden funktionsgleich geplant und implementiert, an den Server angepasst; neue OGL-Funktionen ergänzen sie. Die **derzeitige C++23-Codebasis ist noch kein echter Firestorm-Quellcode-Fork**; eine Portierung/Migration von Firestorm-Code braucht eine gesonderte Lizenz- und Architekturprüfung. Maßgeblich: `projectplan.md`, `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md` und die offene `docs/FIRESTORM_PARITY_MATRIX.md`.

## Aktueller Stand (24.10.2026)

- **Öffentliche Entwicklungsnummer: `v0.3.0.0` (lokal geprüft, noch kein Release auf main).**
- Geprüfter vorheriger Implementierungsstand: 184/184 native CTests,
  606/606 Python-Tests, isolierter originaler Firestorm-Xvfb-GPU-
  und Linux-X11-Taskleistenicon-Test. Aktueller Versionsumbau ist separat zu testen.
- Technisch ist das C++23-Projekt erst **ca. 39 %** fertig
  (38,65 % gewichtet); vollständige Firestorm-/Grid-Funktionsparität ist offen.
- Erste vollständige stabile Zielversion: **`v1.0.0.0`**.
  Erste Wartung danach: **`v1.0.0.1`**.
- **Versionsroadmap:** `ROADMAP.md`,
  `docs/RELEASE_ROADMAP.json` und
  `docs/RELEASE_VERSION_POLICY.md`.
  Eine Nachricht wie `/goal v1.0.0.0 komplett fertigstellen und releasen`
  bedeutet alle Abnahmen erfüllen und erst dann in `main` mergen/taggen.
- `develop-current` ist der Entwicklungsbranch,
  `CHATUEBERGABE.md` dokumentiert die Fortsetzung und
  `projectplan.md` bewahrt die historische Alpha-Implementierung.
- **Angefragtes Hotfixrelease v1.0.0.1: BLOCKIERT**, weil v1.0.0.0 noch nicht als offizielles Release auf main existiert; siehe `docs/V1_0_0_1_RELEASE_BLOCKERS_2026-10-24.md`.
- **Kein produktives v1.0.0.0 veröffentlicht**:
  Live-Grid-Head/Upper/Lower-Skin 3/3, Outfit, WASD/Yaw, volle
  Firestorm-UI, echte Plattform-/Security-/CI-Prüfungen fehlen.

## Historische technische Alpha-Entwicklung

Seit alpha.51 wählt der Viewer aus allen Meshes die bis zu 256
nächsten unterschiedlichen Mesh-Assets anhand der Avatarposition aus.
Bereits geladene Assets erhalten einen 24-Meter-Prioritätsbonus gegen
ständiges Austauschen. Entfernte Meshes werden auf dem GPU-Renderthread
freigegeben, anschließend können neue Meshes nachgeladen werden.
Diese Priorisierung arbeitet noch nicht mit Frustum/Occlusion-Culling.
Details: docs/MESH.md.

Der aktuelle Mesh-Decoder erhält zusätzliche Kompatibilität für
optionale TexCoord0Domain-Felder, bewahrt valide Faces bei defekten
Submeshes und versucht bei fehlerhaften Assets auch die zweite,
regionsgebundene Mesh-Capability (falls angeboten).
Unbekannte binäre Header werden als Hex-Präfix, Länge und
Marker-Offset protokolliert – eine automatische Reparatur der
208 zuvor unbekannten Header **ist damit noch nicht bestätigt**.
Der LLSD-Parser verarbeitet jetzt auch `<date>` und binäre 'd'-Daten.

Die erste OpenSim-Mesh-Pipeline dekodiert Mesh-Asset-Dateien über
GetMesh/GetMesh2 (falls angeboten), und übernimmt Mesh-Faces als echte
OpenGL-Dreiecke. Komplette ObjectUpdateCompressed-Pakete werden nun ebenfalls mit
Mesh-UUID, Scale, Parent-ID, Prim-Form und TextureEntry gelesen.
Skinning/Animesh, OBJ/DAE-Import und LOD-Streaming bleiben offen.
Technik und Grenzen: `docs/MESH.md`.

Die EventQueue toleriert OpenSims erwartete HTTP-502-Leerantworten; 80-Sekunden-
Long-Polls decken das etwa 58-Sekunden-Server-Keepalive ab.
Echte Fehler werden mit Ursache, HTTP-Status, begrenztem Retry-Backoff
und erneuter Seed-CAPS-Ermittlung behandelt. Siehe `docs/CAPS.md`.

Der neue GetTexture-Pfad verlangt erst PNG und wechselt bei HTTP-/PNG-Fehlern auf OpenSims natives `x-j2c`; J2C und JP2 werden per OpenJPEG sicher als RGBA8 decodiert. Details: `docs/TEXTURES.md`.

Bereits vorhanden sind Grid Discovery, echter OpenSim-Login, LLUDP, CAPS/EventQueue, WorldModel, Terrain/EEP, sichtbare Prim-Geometrie, OpenSim Face-Materialien, explizite Prim-UVs und Planar TexGen.

## Architektur

```text
OpenSimulator / OpenGenesisLINK
            ↓
         Protocol
            ↓
        WorldModel
            ↓
    Scene Representation
        ↙         ↘
       UI        OGLRender
                   ↓
      OpenGL Fallback / Vulkan / RTX
```

Protocol und WorldModel dürfen nicht von einem konkreten Renderer abhängen.

## Module

Die lokale Module Runtime ist als erster Basisblock implementiert:

- ModuleManager
- versionierte native C-ABI
- Manifest Loader
- Lifecycle
- Aktivieren/Deaktivieren
- Dependencies
- Capabilities
- Plattform-/Architekturprüfung
- persistenter Aktivierungszustand

Technischer ABI-Vertrag, Sicherheitsgrenzen, Diagnosemodule und Entwicklerstart:
`docs/MODULES.md`. ABI v1 bleibt unverändert; ABI v2 liefert read-only
Diagnosewerte für Developer Tools/Performance Monitor. Die Module liefern
derzeit Logs, keine eigenen UI-Panels.

Die spätere offizielle Add-on-Plattform ist `https://addons.opengenesislink.de/`.

## NextGen Renderer

OpenGL 3.3 Core bleibt während der Migration der Legacy-/Fallback-Pfad. Vulkan ist das langfristige primäre moderne Backend. PBR, HDR, moderne Schatten/Reflexionen, Terrain, Wasser, Wetter, Streaming und optionales Raytracing werden schrittweise aufgebaut.

## Build / Test

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
cmake --preset debug
cmake --build --preset debug -j2
ctest --preset debug --output-on-failure
./build/debug/nexverse-viewer
```

## Dokument-Reihenfolge für neue Chats

1. `CHATUEBERGABE.md`
2. `projectplan.md`
3. `info.md`
4. `docs/ARCHITECTURE.md`
5. `docs/DESIGN.md`
6. `git status`, `git log`, `VERSION`

## Lizenz

Mozilla Public License 2.0.
