# NexVerse Viewer

NexVerse Viewer is a new native OpenSim viewer written in C++23.

It is not a Firestorm fork. The project is designed around a strict separation between the OpenSim protocol layer, the authoritative local WorldModel, rendering, input and UI.

## Current status

Version: `0.2.0-alpha.3`

Phase 5 (WorldModel) is under active development.

The current executable provides the first native desktop shell:

- C++23 / CMake project foundation
- Windows, Linux and macOS target structure
- GLFW desktop window and input loop
- OpenGL 3.3 Core UI backend
- FreeType system-font rendering
- NexVerse dark/cyan/violet design system
- graphical login shell
- OpenSim GridInfo discovery and login
- live LLUDP circuit and CAPS/EventQueue
- locale-independent LLSD XML/Binary handling
- renderer-independent WorldModel foundation
- validated LayerData ingestion for WorldModel terrain readiness
- persistent viewer logging
- version reporting and unit-test foundation

## Architecture

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

Networking and OpenSim contracts must remain independent from the graphics backend.

## License

Mozilla Public License 2.0.

See `LICENSE`, `projectplan.md` and `docs/ARCHITECTURE.md`.
