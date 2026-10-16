#!/usr/bin/env bash
# MPL-2.0 — installs only the user's OGL application menu entry and icons.
# Never changes wallpaper, pinned launchers or other applications.
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
bin="$root/build/debug/nexverse-viewer"
assets="$root/assets/branding"
if [[ ! -x "$bin" ]]; then
    echo "Viewer binary missing: $bin" >&2
    exit 2
fi
for size in 32 48 64 128 256 512; do
    src="$assets/opengenesislink-viewer-$size.png"
    [[ -r "$src" ]] || { echo "Missing icon $src" >&2; exit 2; }
done
target_root="$HOME/.local/share"
desktop="$target_root/applications/opengenesislink-viewer.desktop"
mkdir -p "$target_root/applications"
for size in 32 48 64 128 256 512; do
    path="$target_root/icons/hicolor/$size"x"$size/apps"
    mkdir -p "$path"
    install -m 0644 "$assets/opengenesislink-viewer-$size.png" \
        "$path/opengenesislink-viewer.png"
done
sed -e "s|^Exec=.*|Exec=$bin|" \
    -e "s|^TryExec=.*|TryExec=$bin|" \
    -e "/^Terminal=/i Path=$root" \
    "$root/packaging/linux/opengenesislink-viewer.desktop" >"$desktop"
chmod 0644 "$desktop"
if command -v desktop-file-validate >/dev/null; then
    desktop-file-validate "$desktop"
fi
if command -v update-desktop-database >/dev/null; then
    update-desktop-database "$target_root/applications"
fi
echo "OGL_CINNAMON_DESKTOP_ICON_INSTALLED=$desktop"
echo "OGL_TASKBAR_STARTUP_WM_CLASS=opengenesislink-viewer"
