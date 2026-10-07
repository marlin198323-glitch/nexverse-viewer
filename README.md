# OpenGenesisLINK Viewer for NexVerse

OpenGenesisLINK Viewer is a new native OpenSim viewer written in C++23 and used for the NexVerse grid.

It is not a Firestorm fork. The project is designed around a strict separation between the OpenSim protocol layer, the authoritative local WorldModel, rendering, input and UI.

## Current status

Version: `0.1.0-alpha.1`

Phase 0 is under active development.

The current executable provides the native desktop shell:

- C++23 / CMake project foundation
- Windows, Linux and macOS target structure
- GLFW desktop window and input loop
- OpenGL 3.3 Core UI backend
- GLAD OpenGL loader
- FreeType system-font rendering
- NexVerse dark/cyan/violet design system
- responsive graphical login shell
- keyboard and mouse focus handling
- real text input state
- password masking
- start-region field
- graphical world-preview placeholder
- version reporting
- logging
- unit-test foundation

The NexVerse grid address is not intended to be manually maintained by users. Phase 1 will load the current grid configuration on every viewer start from:

```text
http://hg.stadt-nexverse.de/get_grid_info
```

No OpenSim authentication or world connection is performed in Phase 0.

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

## Planning

`projectplan.md` is the authoritative project plan.

`info.md` records the actual current implementation status and the next development block.

## License

Mozilla Public License 2.0.
