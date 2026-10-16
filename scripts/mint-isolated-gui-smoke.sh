#!/usr/bin/env bash
# MPL-2.0. Reproducible DISPLAY-ISOLATED viewer startup.
# Never start the viewer on the interactive Linux Mint desktop.
set -euo pipefail
script_dir="$(cd -- "$(dirname -- "$0")" && pwd)"
root="$(cd "$script_dir/.." && pwd)"
viewer="$root/build/debug/nexverse-viewer"
if [[ "$#" -gt 0 ]]; then viewer="$1"; fi
if ! command -v xvfb-run >/dev/null 2>&1; then
  echo "OGL GUI smoke aborted: xvfb-run missing; refuse to use user desktop" >&2
  exit 2
fi
if ! command -v timeout >/dev/null 2>&1; then
  echo "OGL GUI smoke aborted: timeout missing" >&2
  exit 2
fi
if [[ ! -x "$viewer" ]]; then
  echo "OGL GUI smoke aborted: no executable viewer at $viewer" >&2
  exit 2
fi
runtime="$(mktemp -d /tmp/ogl-gui-isolated-XXXXXXXX)"
trap 'rm -rf -- "$runtime"' EXIT
mkdir -p "$runtime/config" "$runtime/cache" "$runtime/data"
log="$runtime/viewer.log"
# Start a fresh virtual X server. Remove Wayland and desktop bus
# context; keep HOME only to find the user's LOCAL Firestorm assets.
# A failure MUST NOT fall back to inherited DISPLAY or WAYLAND_DISPLAY.
set +e
(
  cd "$root" || exit 2
  xvfb-run -a -s "-screen 0 1600x900x24" \
    env -u WAYLAND_DISPLAY -u DBUS_SESSION_BUS_ADDRESS \
      XDG_CONFIG_HOME="$runtime/config" \
      XDG_CACHE_HOME="$runtime/cache" \
      XDG_DATA_HOME="$runtime/data" \
      LIBGL_ALWAYS_SOFTWARE=1 \
      timeout --signal=TERM --kill-after=2s 8s "$viewer" \
      >"$log" 2>&1
)
status=$?
set -e
if [[ "$status" -ne 124 ]]; then
  echo "OGL GUI smoke unexpected exit: $status" >&2
  tail -n 25 "$log" >&2
  exit 1
fi
version="$(tr -d '\r\n' <"$root/VERSION")"
for marker in \
  "Starting OpenGenesisLINK Viewer $version" \
  "Firestorm original skeleton bind joints=" \
  "Firestorm ORIGINAL three-part avatar bind-pose meshes GPU-resident" \
  "NexVerse UI renderer initialized"
do
  if ! grep -Fq -- "$marker" "$log"; then
    echo "OGL GUI smoke missing marker: $marker" >&2
    tail -n 25 "$log" >&2
    exit 1
  fi
done
if grep -Eq '\[ERROR\]|Firestorm character assets unavailable' "$log"; then
  echo "OGL GUI smoke logged an error or fallback avatar" >&2
  tail -n 25 "$log" >&2
  exit 1
fi
printf 'OGL_GUI_ISOLATED_SUCCESS (virtual Xvfb; no login, no user desktop)\n'
grep -E 'Starting OpenGenesisLINK Viewer|Firestorm original skeleton bind joints|Firestorm ORIGINAL three-part|NexVerse UI renderer initialized' "$log" | head -n 5
