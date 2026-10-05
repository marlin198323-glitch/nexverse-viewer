# OpenGenesisLINK Viewer Branding

This directory contains the application branding for the OpenGenesisLINK Viewer used with NexVerse.

## Canonical icon

The project owner supplied the canonical OpenGenesisLINK Viewer desktop icon on 05.10.2026.

Required generated assets:

```text
assets/branding/
├── opengenesislink-viewer.png   # canonical high-resolution PNG
├── opengenesislink-viewer.ico   # Windows multi-resolution icon
└── opengenesislink-viewer.icns  # macOS application icon
```

The supplied artwork is the project icon, not a style reference.

## Usage

Linux:

- application/window icon where supported
- desktop entry
- package/AppImage icon

Windows:

- executable resource
- installer
- taskbar/window icon

macOS:

- app bundle icon
- DMG/package branding

Do not replace the artwork with generated substitute branding.

The binary icon assets are intentionally kept separate from the C++ UI layer. The renderer and OpenSim protocol must not depend on packaging assets.
