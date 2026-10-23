#!/usr/bin/env bash
# MPL-2.0: run bounded native C++/Python checks on the Pi without CMake/vcpkg.
set -euo pipefail
cd "$(dirname "$0")/.."
command -v g++ >/dev/null || { echo "g++ missing" >&2; exit 2; }
command -v python3 >/dev/null || { echo "python3 missing" >&2; exit 2; }
tmp="$(mktemp -d)"
trap 'rm -rf -- "$tmp"' EXIT
echo "OGL native: $(uname -m); g++ $(g++ -dumpversion)"
for name in local_heading_preview_tests avatar_proxy_pose_integrity_tests avatar_hover_projection_tests avatar_hover_safety_tests login_tos_consent_tests simulator_scene_validity_tests self_lease_feedback_tests scene_parent_chain_tests attachment_scene_classification_tests movement_feedback_status_tests mesh_residency_target_tests avatar_skin_weight_pair_tests self_appearance_admission_tests self_appearance_ack_receipt_tests avatar_bake_evidence_tests avatar_bake_recheck_tests avatar_part_visibility_tests avatar_bake_stream_priority_tests avatar_bake_preview_tests avatar_bake_mip_priority_tests avatar_gpu_submission_status_tests avatar_pipeline_proof_tests avatar_appearance_runtime_tests avatar_visual_truth_tests mesh_quality_control_tests mesh_quality_evidence_tests terrain_texture_readiness_tests terrain_layer_coverage_tests terrain_detail_retry_tests terrain_recovery_status_tests avatar_view_evidence_tests world_entry_stage_tests world_entry_trace_tests loading_diagnostics_layout_tests loading_circuit_retry_policy_tests asset_stream_proof_tests third_person_camera_tests camera_zoom_controls_tests camera_orbit_controls_tests camera_avatar_focus_tests scene_forward_visibility_tests world_hud_toggle_tests world_hud_layout_tests world_scene_proof_tests outfit_gallery_bake_preview_layout_tests avatar_body_uv_evidence_tests texture_residency_tests firestorm_appearance_verification_tests; do
    g++ -std=c++23 -O0 -Wall -Wextra -Wpedantic -Werror -Iinclude \
        "tests/$name.cpp" -o "$tmp/$name"
    "$tmp/$name"
done
# Compile production WorldModel/LLUUDP and TextureEntry sources once
# on ARM64, then link native regression entrypoints. Warnings are fatal.
world_objects=()
for source in src/world/world_model.cpp src/protocol/viewer_messages.cpp     src/protocol/lludp.cpp src/protocol/texture_entry.cpp     src/protocol/self_appearance_admission.cpp; do
    obj="$tmp/$(basename "$source" .cpp).o"
    g++ -std=c++23 -O0 -Wall -Wextra -Wpedantic -Werror         -pthread -Iinclude -c "$source" -o "$obj"
    world_objects+=("$obj")
done
for name in prim_localid_reuse_tests prim_uuid_relocation_tests     avatar_localid_reuse_tests kill_avatar_linkset_tests     self_avatar_kill_binding_tests world_model_tests self_avatar_pose_authority_tests self_agent_high4_pose_gate_tests self_agent_movement_look_at_tests early_avatar_readback_tests; do
    g++ -std=c++23 -O0 -Wall -Wextra -Wpedantic -Werror         -pthread -Iinclude "tests/$name.cpp" "${world_objects[@]}"         -o "$tmp/$name"
    "$tmp/$name"
done
python3 -m unittest discover -s tests -p 'test*.py'
echo "OGL_PI5_NATIVE_HEADLESS_OK (GUI/Full CMake not exercised)"
