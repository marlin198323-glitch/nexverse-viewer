# OpenGenesisLINK Viewer – Architektur

## Ziel

Eigenständiger plattformübergreifender OpenSim/OpenGenesisLINK-Client in C++23. Kein Firestorm-Fork.

## Schichtenmodell

```text
Application
├── Protocol
│   ├── GridInfo / Login / LLSD / LLUDP
│   ├── CAPS / EventQueue
│   ├── Assets / Inventory / Appearance
│   └── Teleport / Hypergrid
├── WorldModel
│   ├── Grid / Region / Agent / Avatar / Object
│   ├── Terrain / Parcel
│   └── Environment
├── Scene / RenderWorld
├── AssetCache
├── Input
├── UI
├── Module Runtime
└── OGLRender
    ├── OpenGL Legacy/Fallback
    ├── Vulkan NextGen
    └── optional RTX/weitere Backends
```

## Verbindliche Regeln

1. Protocol ruft niemals OpenGL/Vulkan direkt auf.
2. Renderer dekodieren keine LLUDP-/LLSD-Pakete.
3. UI parst keine rohen Netzwerkdaten.
4. WorldModel ist die Grenze zwischen Netzwerkzustand und Darstellung.
5. OGLRender konsumiert Scene-/Renderdaten, keine Netzwerkdetails.
6. Externe Daten werden begrenzt und validiert.
7. Serverautoritärer Zustand wird nicht clientseitig erfunden.
8. Plugins greifen nicht auf private C++-Layouts oder interne Rendererstrukturen zu.
9. Öffentliche Plugin- und Renderer-APIs werden versioniert.

## Module Runtime

Sie wird vor weiteren großen Viewer-Systemen implementiert. Die native Grenze soll C-kompatibel sein: keine STL-Typen oder Exceptions über die ABI, versionierte Structs/Function Tables, UTF-8 Strings und kontrollierte Host-Callbacks.

Zustände:

```text
installed → compatible → enabled → loaded → initialized → running
                                                   ↓
                                         stopped → unloaded
```

Online-Registry, Download, Signaturen und automatische Updates folgen später.

## Core versus Plugin

Core bleiben mindestens Login, LLUDP/CAPS, WorldModel, Basis-Assets, Basis-Avatar, Basis-Inventar, Basis-Outfits/Appearance, Basis-UI, Environment, Text-only-Modus, ModuleManager und mindestens ein funktionierender Fallback-Renderer.

Optionale Module umfassen später Vulkan/RTX, Voice, Media, Weather, Advanced AO, RLVa, Outfit Manager/Preview, Build-/Material-/Mesh-Tools und Diagnosewerkzeuge.

## Environment / Wetter / Wasser

Region/EEP bleibt Standard. Reale Tag-/Nachtzeit ist ein lokaler Core-Override.

`OGL Weather` ist ein optionales Plugin.

HydroEngine ist später serverautoritativ. Der Viewer visualisiert dynamische Wasserdaten, berechnet aber nicht die verbindliche Wassermenge. Legacy `region.water_height` bleibt kompatibel; spätere Water-State-Daten dürfen Chunks, Surface Height, Depth, Velocity und Flow enthalten.

## Maßgeblichkeit

`projectplan.md` ist bindend. `info.md` dokumentiert den realen Stand. `CHATUEBERGABE.md` ist der Wiedereinstiegspunkt.
