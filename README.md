# NexVerse Viewer

NexVerse Viewer is a new native OpenSim viewer written in C++23.

It is not a Firestorm fork. The project is designed around a strict separation between the OpenSim protocol layer, the authoritative local WorldModel, rendering, input and UI.

## Current status

Version: `0.1.0-alpha.1`

Phase 0 is under active development.

The current executable provides the first native desktop shell:

- C++23 / CMake project foundation
- Windows, Linux and macOS target structure
- GLFW desktop window and input loop
- OpenGL 3.3 Core UI backend
- FreeType system-font rendering
- NexVerse dark/cyan/violet design system
- graphical login shell
- graphical world-preview placeholder
- version reporting
- logging
- unit-test foundation

No OpenSim login or network traffic is performed in Phase 0.

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
