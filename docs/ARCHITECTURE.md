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

Die erste Core-Library wurde in `0.2.0-alpha.38` implementiert. Die native Grenze soll C-kompatibel sein: keine STL-Typen oder Exceptions über die ABI, versionierte Structs/Function Tables, UTF-8 Strings und kontrollierte Host-Callbacks.

Zustände:

```text
installed → compatible → enabled → loaded → initialized → running
                                                   ↓
                                         stopped → unloaded
```

Lokale ABI v1/v2, Manifest- und Lifecycle-Prüfung sind implementiert; die ABI v2 liefert eine schreibgeschützte Snapshot-Tabelle für Developer Tools und Performance Monitor. Ungeprüfte native Module bleiben standardmäßig blockiert. Online-Registry, geprüfte Signaturen, Download, automatische Updates und eine sichere Drittmodul-Isolation folgen später. Siehe `docs/MODULES.md`.

## Legacy RenderMaterials (0.2.0-alpha.42)

Der CAPS-Client lädt einen serverseitigen RenderMaterials-Snapshot
asynchron. Der Parser verarbeitet strikt begrenzte zlib/LLSD-Binärdaten
und stellt Legacy-Alpha-Informationen als typisierte Werte bereit.
Application bindet per MaterialID an die Scene-Objekte. OGLRender
enthält ausschließlich Renderdaten und keine Netzwerkroutinen.
RenderMaterialIDs für PBR bleiben getrennt; Details in
`docs/MATERIALS.md`.

## Core versus Plugin

Core bleiben mindestens Login, LLUDP/CAPS, WorldModel, Basis-Assets, Basis-Avatar, Basis-Inventar, Basis-Outfits/Appearance, Basis-UI, Environment, Text-only-Modus, ModuleManager und mindestens ein funktionierender Fallback-Renderer.

Optionale Module umfassen später Vulkan/RTX, Voice, Media, Weather, Advanced AO, RLVa, Outfit Manager/Preview, Build-/Material-/Mesh-Tools und Diagnosewerkzeuge.

## Environment / Wetter / Wasser

Region/EEP bleibt Standard. Reale Tag-/Nachtzeit ist ein lokaler Core-Override.

`OGL Weather` ist ein optionales Plugin.

HydroEngine ist später serverautoritativ. Der Viewer visualisiert dynamische Wasserdaten, berechnet aber nicht die verbindliche Wassermenge. Legacy `region.water_height` bleibt kompatibel; spätere Water-State-Daten dürfen Chunks, Surface Height, Depth, Velocity und Flow enthalten.

## Maßgeblichkeit

`projectplan.md` ist bindend. `info.md` dokumentiert den realen Stand. `CHATUEBERGABE.md` ist der Wiedereinstiegspunkt.


## OpenSim EventQueue-Lifecycle (0.2.0-alpha.47)

Die CAP-HTTP-Schicht unterscheidet reguläres EventQueue-Idle
(HTTP 502) von abgelaufenen Endpunkten (HTTP 404),
Transport-/HTTP-Fehlern und ungültigem LLSD. Der Poll-Timeout
beträgt 80 Sekunden, während OpenSimulator die Anfragen rund
58 Sekunden offenhalten kann. Wiederholte echte Fehler
verwenden eine begrenzte Retry-Pause und Seed-Re-Discovery.
Die ursprüngliche Region bleibt bis zur separaten
Teleport-/Circuit-Lifecycle-Implementierung gebunden.

Siehe `docs/CAPS.md`.


## GPU-Mesh-Residency (0.2.0-alpha.51)

Der CPU-Distanzplaner bestimmt Top-256 Mesh-Asset-UUIDs,
mit Hysterese für bereits hochgeladene Daten. Der Renderer
verwaltet die OpenGL-Ressourcen und entfernt GPU-VAO/VBO/EBO
bei Eviction und Regionenwechsel. HTTP-Antworten werden vor
jedem Upload auf Residency und Region-Epoche geprüft.
View-Frustum-/Occlusion-Prioritäten und Diskcache fehlen noch.

Siehe docs/MESH.md.
