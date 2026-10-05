# NexVerse Viewer Design Direction

## Reference

The first NexVerse Viewer concept created for this project is the visual reference for Phase 0.

The design is intentionally NexVerse-specific and is not intended to reproduce Firestorm or OpenGenesisLINK pixel-for-pixel.

## Visual language

- dark navy / near-black background
- silver-white typography
- cyan and electric blue primary accents
- violet and magenta secondary accents
- restrained glass-style panels
- large world viewport
- compact controls
- minimal permanent chrome
- expandable panels instead of dense always-visible toolbars

## Login shell

The Phase 0 login shell contains:

- NexVerse branding
- viewer version
- Grid / Server URL
- username
- password
- start Region
- primary Enter World action
- large visual world-preview surface
- runtime/system-status panel

The controls are visual only during Phase 0. Network login becomes functional in Phase 2.

## In-world target

The final viewer should keep the world view dominant.

```text
+----------------------------------------------------------+
| NexVerse       Region / status                 Profile   |
+----------------------------------------------------------+
|                                                          |
|                       3D WORLD                           |
|                                                          |
|                                                [panel]   |
|                                                          |
+----------------------------------------------------------+
| chat        movement / voice / inventory / camera        |
+----------------------------------------------------------+
```

Side panels must be collapsible.

## Accessibility and scaling

The UI architecture must later support:

- HiDPI
- UI scale
- keyboard navigation
- readable contrast
- configurable font size
- Windows, Linux and macOS input conventions

Phase 0 establishes the color and spacing language; accessibility behavior is expanded with the interactive UI phases.
