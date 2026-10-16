#!/usr/bin/env bash
# MPL-2.0: run bounded native C++/Python checks on the Pi without CMake/vcpkg.
set -euo pipefail
cd "$(dirname "$0")/.."
command -v g++ >/dev/null || { echo "g++ missing" >&2; exit 2; }
command -v python3 >/dev/null || { echo "python3 missing" >&2; exit 2; }
tmp="$(mktemp -d)"
trap 'rm -rf -- "$tmp"' EXIT
echo "OGL native: $(uname -m); g++ $(g++ -dumpversion)"
for name in avatar_bake_evidence_tests avatar_bake_stream_priority_tests avatar_bake_preview_tests avatar_bake_mip_priority_tests avatar_gpu_submission_status_tests world_scene_proof_tests outfit_gallery_bake_preview_layout_tests avatar_body_uv_evidence_tests texture_residency_tests firestorm_appearance_verification_tests; do
    g++ -std=c++23 -O0 -Wall -Wextra -Wpedantic -Werror -Iinclude \
        "tests/$name.cpp" -o "$tmp/$name"
    "$tmp/$name"
done
python3 -m unittest discover -s tests -p 'test*.py'
echo "OGL_PI5_NATIVE_HEADLESS_OK (GUI/Full CMake not exercised)"
