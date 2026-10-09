# OpenGenesisLINK Viewer

OpenGenesisLINK Viewer ist ein eigenständiger nativer OpenSim/OpenGenesisLINK-Viewer für NexVerse in C++23.

Er ist **kein Firestorm-Fork**. Firestorm und der Second-Life-Viewer dienen ausschließlich als Funktions- und Qualitätsreferenz. Maßgeblich für Architektur und Reihenfolge ist `projectplan.md`.

## Aktueller Stand

- Version: `0.2.0-alpha.37`
- Branch: `develop`
- direkter Chat-Wiedereinstieg: `CHATUEBERGABE.md`
- letzter vollständig geprüfter Stand: 19/19 Tests grün plus OpenGL-Runtime-Smoke
- nächster Entwicklungsblock: **Module Runtime / ModuleManager**

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

Die lokale Module Runtime wird jetzt als Grundlage gebaut:

- ModuleManager
- versionierte native C-ABI
- Manifest Loader
- Lifecycle
- Aktivieren/Deaktivieren
- Dependencies
- Capabilities
- Plattform-/Architekturprüfung
- persistenter Aktivierungszustand

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
