#!/usr/bin/env bash
# Read-only X11 property test under a virtual display. Never touches :0.
set -euo pipefail
cd "$(dirname "$0")/.."
if ! command -v xvfb-run >/dev/null || ! command -v xwininfo >/dev/null ||
   ! command -v xprop >/dev/null; then
    echo 'Xvfb / xwininfo / xprop not installed'; exit 2
fi
xvfb-run -a -s "-screen 0 1600x900x24" \
  env LIBGL_ALWAYS_SOFTWARE=1 bash -c '
    set -e
    # An EWMH-compliant window manager enables _NET_WM_ICON
    # in GLFW. Xvfb alone lacks the _NET_SUPPORTED property.
    env -u DBUS_SESSION_BUS_ADDRESS metacity --sm-disable --compositor off >/tmp/ogl-branding-metacity.log 2>&1 &
    wm_pid=$!
    trap "kill $wm_pid 2>/dev/null || :; wait $wm_pid 2>/dev/null || :" EXIT
    sleep 2
    ./build/debug/nexverse-viewer >/tmp/ogl-branding-smoke.log 2>&1 &
    viewer_pid=$!
    trap "kill $viewer_pid $wm_pid 2>/dev/null || :; wait $viewer_pid 2>/dev/null || :; wait $wm_pid 2>/dev/null || :" EXIT
    window_id=""
    for i in $(seq 1 25); do
      window_id=$(xwininfo -root -tree |
        awk "/OpenGenesisLINK Viewer/ {print \$1;exit}") || :
      if [[ $window_id == 0x* ]]; then break; fi
      sleep 0.3
    done
    if [[ $window_id != 0x* ]]; then
      echo "no actual Viewer window in virtual X11"; exit 1
    fi
    # Some xprop builds do not print the giant icon CARDINAL array.
    # Verify the actual XGetWindowProperty readback from the viewer.
    # The viewer checks the first 2 CARDINALs and trailing pixel bytes.
    for i in $(seq 1 30); do
      if grep -q "X11 system taskbar icon installed: 128x128"           /tmp/ogl-branding-smoke.log; then break; fi
      sleep 0.3
    done
    wm_class=$(xprop -id "$window_id" WM_CLASS)
    echo "X11_BRAND_CLASS=$wm_class"
    grep "X11 system taskbar icon installed: 128x128"       /tmp/ogl-branding-smoke.log
    if [[ $wm_class != *opengenesislink-viewer* ]]; then exit 1; fi
    grep -q "X11 system taskbar icon installed: 128x128"       /tmp/ogl-branding-smoke.log
    echo "X11_WINDOW_ICON_AND_TASKBAR_CLASS_PASS"
  '
