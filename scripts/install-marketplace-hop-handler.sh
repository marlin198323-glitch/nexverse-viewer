#!/usr/bin/env bash
set -euo pipefail
# User-invoked ONLY. It changes the current user's default hop:// handler,
# so never run automatically during build/test or when deploying Marketplace.
if ! command -v nexverse-viewer >/dev/null 2>&1; then
  echo 'ERROR: nexverse-viewer not installed in PATH' >&2
  exit 2
fi
if ! command -v xdg-mime >/dev/null 2>&1; then
  echo 'ERROR: xdg-mime missing' >&2
  exit 2
fi
here=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
target="${XDG_DATA_HOME:-$HOME/.local/share}/applications"
install -d -m 700 "$target"
install -m 644 "$here/packaging/linux/opengenesislink-viewer.desktop" "$target/opengenesislink-viewer.desktop"
xdg-mime default opengenesislink-viewer.desktop x-scheme-handler/hop
echo 'HOP_HANDLER_INSTALLED=opengenesislink-viewer.desktop'
xdg-mime query default x-scheme-handler/hop
