#!/usr/bin/env python3
"""Add OGL launcher to the current Cinnamon grouped-window-list safely.

Does not remove or reorder pinned applications, alter window manager state,
or restart Cinnamon. Run with --apply to write. Default is dry-run.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import tempfile

APP = "opengenesislink-viewer.desktop"


def pin_config(path: Path, apply: bool) -> str:
    original = path.read_bytes()
    data = json.loads(original.decode("utf-8"))
    settings = data.get("pinned-apps")
    if not isinstance(settings, dict) or not isinstance(settings.get("value"), list):
        return "SKIP_NO_PINNED_APPS"
    pinned = settings["value"]
    if not all(isinstance(value, str) for value in pinned):
        return "SKIP_INVALID_PINNED_APPS"
    if APP in pinned:
        return "ALREADY_PINNED"
    if not apply:
        return "WOULD_PIN"
    pinned.append(APP)
    # Verify that the file has not been changed by Cinnamon meanwhile.
    if hashlib.sha256(path.read_bytes()).digest() != hashlib.sha256(original).digest():
        raise RuntimeError("Cinnamon settings changed during update; retry")
    updated = (json.dumps(data, ensure_ascii=False, indent=4) + "\n").encode("utf-8")
    fd, temporary = tempfile.mkstemp(prefix=".ogl-pin-", dir=str(path.parent))
    try:
        os.fchmod(fd, stat.S_IMODE(path.stat().st_mode))
        with os.fdopen(fd, "wb") as stream:
            stream.write(updated)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temporary, path)
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)
    return "PINNED"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="Add OGL launcher without removing existing entries")
    parser.add_argument("--config-root", type=Path, default=
                        Path.home()/".config/cinnamon/spices/grouped-window-list@cinnamon.org")
    options = parser.parse_args()
    configs = sorted(options.config_root.glob("[0-9]*.json"))
    if not configs:
        print("OGL_CINNAMON_PIN=NOT_FOUND")
        return 0
    for path in configs:
        result = pin_config(path, options.apply)
        print(f"OGL_CINNAMON_PIN={result} config={path.name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
