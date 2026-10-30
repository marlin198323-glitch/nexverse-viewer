#!/usr/bin/env python3
"""Read-only NexVerse asset-cache summary. No cache mutation or network access.

Usage: python3 scripts/ogl-cache-status.py
Reports number of lossless compressed/raw textures and mesh cache sizes;
does not expose UUIDs, personal paths, URLs or user/session information.
"""
from __future__ import annotations

import os
from pathlib import Path
import struct
import sys

MAX_IMAGE_BYTES = 64 * 1024 * 1024
DEFAULT_BUDGET_MIB = 512
MIN_BUDGET_MIB = 128
MAX_BUDGET_MIB = 2048


def budget_mib(config: str | None) -> int:
    if config is None or not config.isascii() or not config.isdecimal():
        return DEFAULT_BUDGET_MIB
    if len(config) > 4:
        return DEFAULT_BUDGET_MIB
    size = int(config)
    return size if MIN_BUDGET_MIB <= size <= MAX_BUDGET_MIB else DEFAULT_BUDGET_MIB


def cache_base() -> Path:
    xdg = os.environ.get("XDG_CACHE_HOME", "")
    if xdg and Path(xdg).is_absolute():
        return Path(xdg) / "opengenesislink"
    return Path.home() / ".cache/opengenesislink"


def measure_textures(folder: Path) -> dict[str, int]:
    counts = {"raw": 0, "compressed": 0, "invalid": 0,
              "bytes": 0, "logical_bytes": 0}
    if not folder.exists():
        return counts
    for entry in folder.glob("*.rgba"):
        try:
            if not entry.is_file():
                continue
            size = entry.stat().st_size
            with entry.open("rb") as source:
                header = source.read(24)
            if len(header) != 24:
                counts["invalid"] += 1
                continue
            magic = header[:8]
            width, height, _hash = struct.unpack("<IIQ", header[8:24])
            original = width * height * 4
            if (magic not in (b"OGLTEX01", b"OGLTEX02") or
                    width < 1 or height < 1 or
                    width > 4096 or height > 4096 or
                    original > MAX_IMAGE_BYTES or
                    size < 24 or size > MAX_IMAGE_BYTES + 24 or
                    (magic == b"OGLTEX01" and size != original + 24)):
                counts["invalid"] += 1
                continue
            counts["raw" if magic == b"OGLTEX01" else "compressed"] += 1
            counts["bytes"] += size
            counts["logical_bytes"] += original + 24
        except (OSError, ValueError, struct.error):
            counts["invalid"] += 1
    return counts


def measure_meshes(folder: Path) -> tuple[int, int]:
    if not folder.exists():
        return 0, 0
    count = 0
    used = 0
    for entry in folder.iterdir():
        try:
            if entry.is_file() and not entry.name.endswith(".part"):
                count += 1
                used += entry.stat().st_size
        except OSError:
            pass
    return count, used


def main() -> int:
    root = cache_base()
    t = measure_textures(root / "textures-v1")
    m_count, m_used = measure_meshes(root / "meshes-v1")
    show = lambda b: f"{b / 1048576:.1f} MiB"
    print(f"Cache-Budget: {budget_mib(os.getenv('OGL_TEXTURE_CACHE_MIB'))} MiB (Texturen)")
    print(f"Texturen: {t['raw'] + t['compressed']} Assets "
          f"({t['compressed']} verlustfrei komprimiert, {t['raw']} RGBA)")
    print(f"Textur-Speicher: {show(t['bytes'])} auf Platte, "
          f"{show(t['logical_bytes'])} unkomprimiert")
    print(f"Mesh-Dateien: {m_count}, {show(m_used)} auf Platte")
    if t["invalid"]:
        print(f"Ungueltige Texturnachweise: {t['invalid']} "
              "(nur geprueft, nicht geloescht)")
    print("Cache wird beim nächsten Start wiederverwendet; "
          "Web/LLUDP-Assets und Texturdarstellung werden dadurch nicht veraendert.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
