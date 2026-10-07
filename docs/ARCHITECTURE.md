# NexVerse Viewer Architecture

## Objective

NexVerse Viewer is a native cross-platform OpenSim client.

The project intentionally avoids coupling the OpenSim protocol implementation to a specific renderer or UI toolkit.

## Layer model

```text
Application
├── OpenSim Protocol
│   ├── GridInfo
│   ├── Login
│   ├── LLSD
│   ├── LLUDP
│   ├── CAPS
│   ├── Assets
│   ├── Inventory
│   ├── Appearance
│   └── Teleport / Hypergrid
├── WorldModel
│   ├── Grid
│   ├── Region
│   ├── Agent
│   ├── Avatar
│   ├── Object
│   ├── Terrain
│   ├── Parcel
│   └── Environment
├── AssetCache
├── Input
├── RenderWorld
├── Renderer
└── UI
```

## Dependency rules

1. Protocol code may update typed protocol/domain data but must not call OpenGL.
2. Renderer code must not decode LLUDP or LLSD.
3. UI code must not parse raw network packets.
4. WorldModel is the boundary between network state and visual state.
5. Platform-specific code stays behind interfaces.
6. External payloads must have explicit bounds and validation.
7. Server-authoritative state must never be silently invented by the client.

## Phase 0

Phase 0 deliberately contains no OpenSim network implementation.

Its job is to prove:

- C++23 toolchain
- dependency management
- native application lifecycle
- graphical UI foundation
- testability
- cross-platform CI structure

The first renderer is OpenGL 3.3 Core because it gives the project a small common bootstrap backend across the initial desktop targets. It is not a permanent architectural dependency: protocol and WorldModel layers remain renderer-independent.

## Future renderer boundary

A future interface may provide backends such as:

```text
Renderer
├── Vulkan
├── Metal
└── OpenGL fallback
```

The OpenSim protocol layer must require no changes when a renderer backend changes.
