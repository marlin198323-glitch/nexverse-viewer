#!/usr/bin/env bash
# MPL-2.0: validate OpenGenesisLINK Viewer on Linux Mint x64.
# Never deploy and never open the interactive user display.
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"
if [[ "$(uname -s)" != Linux || "$(uname -m)" != x86_64 ]]; then
  echo "OGL Mint validation needs Linux x64; Pi ARM64 stays CI only." >&2
  exit 2
fi
if (( $# > 0 )); then
  if [[ "$1" != "--tests-only" || $# -ne 1 ]]; then
    echo "Usage: $0 [--tests-only]" >&2; exit 2
  fi
fi
if ! printenv VCPKG_ROOT >/dev/null; then
  export VCPKG_ROOT="$HOME/vcpkg"
fi
if [[ ! -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]]; then
  echo "Missing vcpkg toolchain: $VCPKG_ROOT" >&2; exit 2
fi
echo "OGL_MINT_HOST: $(hostname) x86_64"
echo "OGL_MINT_SHA: $(git rev-parse HEAD)"
echo "OGL_MINT_VERSION: $(cat VERSION)"
echo "GUI checks use isolated Xvfb; never open interactive desktop."
if (( $# == 0 )); then
  cmake --preset debug -DNEXVERSE_BUILD_TESTS=ON
  cmake --build --preset debug --parallel 8
fi
if [[ ! -x build/debug/nexverse-viewer ]]; then
  echo "Missing real x64 viewer executable." >&2; exit 1
fi
ctest --test-dir build/debug --parallel 8 --output-on-failure
python3 -m unittest discover -s tests -p 'test*.py'
bash scripts/mint-isolated-gui-smoke.sh
bash scripts/test-mint-window-branding.sh
echo "OGL_MINT_X64_FULL_VALIDATION_PASS"
echo "NO_LIVE_GRID_LOGIN_OR_AVATAR_SKIN_COF_WASD_CLAIM"
