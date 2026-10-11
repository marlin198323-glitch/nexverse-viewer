/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/asset_scene_coverage.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
namespace nexverse::assets
{
// Fixed, numeric-only JSON schema: safe to parse in a log collector.
// GPU coverage means UUID residency only, not verified visual completeness.
struct AssetStreamingSnapshot
{
    std::uint64_t region_epoch=0U;
    std::uint64_t session_started_unix_ms=0U;
    std::uint64_t emitted_unix_ms=0U;
    std::uint64_t sequence=0U;
    std::size_t texture_scene_samples=0U;
    std::size_t mesh_scene_samples=0U;
    AssetSceneCoverage textures{};
    AssetSceneCoverage meshes{};
    std::size_t texture_in_flight=0U;
    std::size_t mesh_in_flight=0U;
    std::size_t texture_retry_blocked=0U;
    std::size_t mesh_retry_blocked=0U;
    std::size_t texture_ready_retries=0U;
    std::size_t mesh_ready_retries=0U;
    std::size_t texture_ram_bytes=0U;
    std::size_t mesh_ram_bytes=0U;
    std::size_t texture_ram_hits=0U;
    std::size_t texture_ram_misses=0U;
    std::size_t texture_ram_evictions=0U;
    std::size_t texture_ram_hit_rate_pct=100U;
    std::size_t texture_ram_interval_evictions=0U;
    std::size_t texture_ram_pressure=0U;
    std::size_t mesh_ram_hits=0U;
    std::size_t mesh_ram_misses=0U;
    std::size_t mesh_ram_evictions=0U;
    std::size_t mesh_ram_hit_rate_pct=100U;
    std::size_t mesh_ram_interval_evictions=0U;
    std::size_t mesh_ram_pressure=0U;
    std::size_t gpu_texture_bytes=0U;
    std::size_t gpu_mesh_bytes=0U;
    std::size_t gpu_shared_budget_bytes=0U;
    std::size_t gpu_upload_cpu_samples=0U;
    std::uint64_t gpu_upload_cpu_avg_us=0U;
    std::uint64_t gpu_upload_cpu_p95_us=0U;
    std::uint64_t gpu_upload_cpu_max_us=0U;
    std::size_t gpu_upload_pacing_ms=0U;
    std::size_t texture_gpu_upload_failures=0U;
    std::size_t texture_gpu_staging_deferrals=0U;
    std::size_t texture_gpu_staging_waiting=0U;
    std::size_t texture_gpu_staging_released=0U;
    std::size_t mesh_gpu_budget_waiting=0U;
    std::size_t mesh_gpu_budget_released=0U;
    std::size_t texture_cpu_cached_region_reuse=0U;
    std::size_t texture_cpu_cached_lod_reuse=0U;
    std::size_t mesh_cpu_cached_region_reuse=0U;
    std::size_t mesh_cpu_cached_lod_reuse=0U;
    std::size_t texture_cpu_cached_region_ram=0U;
    std::size_t texture_cpu_cached_region_disk=0U;
    std::size_t texture_cpu_cached_lod_ram=0U;
    std::size_t texture_cpu_cached_lod_disk=0U;
    std::size_t mesh_cpu_cached_region_ram=0U;
    std::size_t mesh_cpu_cached_region_disk=0U;
    std::size_t mesh_cpu_cached_lod_ram=0U;
    std::size_t mesh_cpu_cached_lod_disk=0U;
    std::size_t texture_cpu_reuse_history_evictions=0U;
    std::size_t mesh_cpu_reuse_history_evictions=0U;
    std::size_t mesh_gpu_upload_failures=0U;
    std::size_t mesh_fail_missing=0U;
    std::size_t mesh_fail_transient=0U;
    std::size_t mesh_fail_invalid=0U;
    std::size_t mesh_fail_configuration=0U;
    std::size_t mesh_missing_unique=0U;
    std::size_t mesh_missing_unique_overflow=0U;
    std::size_t texture_fail_missing=0U;
    std::size_t texture_fail_transient=0U;
    std::size_t texture_fail_invalid=0U;
    std::size_t texture_fail_configuration=0U;
    std::size_t texture_missing_unique=0U;
    std::size_t texture_missing_unique_overflow=0U;
    std::size_t texture_priority_churn=0U;
    std::size_t mesh_priority_churn=0U;
    std::uint64_t previous_frame_draw_calls=0U;
    std::uint64_t previous_frame_visible_objects=0U;
    std::uint64_t previous_frame_scene_draw_calls=0U;
    std::uint64_t previous_frame_scene_mesh_draw_calls=0U;
    std::uint64_t previous_frame_scene_prim_draw_calls=0U;
    std::uint64_t previous_frame_scene_opaque_draw_calls=0U;
    std::uint64_t previous_frame_scene_alpha_draw_calls=0U;
    std::uint64_t previous_frame_scene_alpha_mask_draw_calls=0U;
    std::uint64_t previous_frame_scene_missing_mesh_draw_calls=0U;
    std::uint64_t previous_frame_scene_missing_mesh_objects=0U;
    std::uint64_t previous_frame_scene_missing_mesh_triangles=0U;

    [[nodiscard]] std::string json() const
    {
        std::string out=R"({"schema":"ogl.asset.streaming.v1")";
        const auto field=[&](const char* key,std::uint64_t value)
        {
            out+=",\"";
            out+=key;
            out+="\":";
            out+=std::to_string(value);
        };
        field("region_epoch",region_epoch);
        field("session_started_unix_ms",session_started_unix_ms);
        field("emitted_unix_ms",emitted_unix_ms);
        field("sequence",sequence);
        field("texture_scene_samples",texture_scene_samples);
        field("mesh_scene_samples",mesh_scene_samples);
        field("textures_wanted",textures.requested);
        field("textures_resident",textures.resident);
        field("textures_pending",textures.pending);
        field("texture_coverage_pct",textures.percentage());
        field("meshes_wanted",meshes.requested);
        field("meshes_resident",meshes.resident);
        field("meshes_pending",meshes.pending);
        field("mesh_coverage_pct",meshes.percentage());
        field("texture_in_flight",texture_in_flight);
        field("mesh_in_flight",mesh_in_flight);
        field("texture_retry_blocked",texture_retry_blocked);
        field("mesh_retry_blocked",mesh_retry_blocked);
        field("texture_ready_retries",texture_ready_retries);
        field("mesh_ready_retries",mesh_ready_retries);
        field("texture_ram_bytes",texture_ram_bytes);
        field("mesh_ram_bytes",mesh_ram_bytes);
        field("texture_ram_hits",texture_ram_hits);
        field("texture_ram_misses",texture_ram_misses);
        field("texture_ram_evictions",texture_ram_evictions);
        field("texture_ram_hit_rate_pct",texture_ram_hit_rate_pct);
        field("texture_ram_interval_evictions",texture_ram_interval_evictions);
        field("texture_ram_pressure",texture_ram_pressure);
        field("mesh_ram_hits",mesh_ram_hits);
        field("mesh_ram_misses",mesh_ram_misses);
        field("mesh_ram_evictions",mesh_ram_evictions);
        field("mesh_ram_hit_rate_pct",mesh_ram_hit_rate_pct);
        field("mesh_ram_interval_evictions",mesh_ram_interval_evictions);
        field("mesh_ram_pressure",mesh_ram_pressure);
        field("gpu_texture_bytes",gpu_texture_bytes);
        field("gpu_mesh_bytes",gpu_mesh_bytes);
        field("gpu_shared_budget_bytes",gpu_shared_budget_bytes);
        field("gpu_upload_cpu_samples",gpu_upload_cpu_samples);
        field("gpu_upload_cpu_avg_us",gpu_upload_cpu_avg_us);
        field("gpu_upload_cpu_p95_us",gpu_upload_cpu_p95_us);
        field("gpu_upload_cpu_max_us",gpu_upload_cpu_max_us);
        field("gpu_upload_pacing_ms",gpu_upload_pacing_ms);
        field("texture_gpu_upload_failures",texture_gpu_upload_failures);
        field("texture_gpu_staging_deferrals",texture_gpu_staging_deferrals);
        field("texture_gpu_staging_waiting",texture_gpu_staging_waiting);
        field("texture_gpu_staging_released",texture_gpu_staging_released);
        field("mesh_gpu_budget_waiting",mesh_gpu_budget_waiting);
        field("mesh_gpu_budget_released",mesh_gpu_budget_released);
        field("texture_cpu_cached_region_reuse",texture_cpu_cached_region_reuse);
        field("texture_cpu_cached_lod_reuse",texture_cpu_cached_lod_reuse);
        field("mesh_cpu_cached_region_reuse",mesh_cpu_cached_region_reuse);
        field("mesh_cpu_cached_lod_reuse",mesh_cpu_cached_lod_reuse);
        field("texture_cpu_cached_region_ram",texture_cpu_cached_region_ram);
        field("texture_cpu_cached_region_disk",texture_cpu_cached_region_disk);
        field("texture_cpu_cached_lod_ram",texture_cpu_cached_lod_ram);
        field("texture_cpu_cached_lod_disk",texture_cpu_cached_lod_disk);
        field("mesh_cpu_cached_region_ram",mesh_cpu_cached_region_ram);
        field("mesh_cpu_cached_region_disk",mesh_cpu_cached_region_disk);
        field("mesh_cpu_cached_lod_ram",mesh_cpu_cached_lod_ram);
        field("mesh_cpu_cached_lod_disk",mesh_cpu_cached_lod_disk);
        field("texture_cpu_reuse_history_evictions",
            texture_cpu_reuse_history_evictions);
        field("mesh_cpu_reuse_history_evictions",
            mesh_cpu_reuse_history_evictions);
        field("mesh_gpu_upload_failures",mesh_gpu_upload_failures);
        field("mesh_fail_missing",mesh_fail_missing);
        field("mesh_fail_transient",mesh_fail_transient);
        field("mesh_fail_invalid",mesh_fail_invalid);
        field("mesh_fail_configuration",mesh_fail_configuration);
        field("mesh_missing_unique",mesh_missing_unique);
        field("mesh_missing_unique_overflow",mesh_missing_unique_overflow);
        field("texture_fail_missing",texture_fail_missing);
        field("texture_fail_transient",texture_fail_transient);
        field("texture_fail_invalid",texture_fail_invalid);
        field("texture_fail_configuration",texture_fail_configuration);
        field("texture_missing_unique",texture_missing_unique);
        field("texture_missing_unique_overflow",texture_missing_unique_overflow);
        field("texture_priority_churn",texture_priority_churn);
        field("mesh_priority_churn",mesh_priority_churn);
        field("previous_frame_draw_calls",previous_frame_draw_calls);
        field("previous_frame_visible_objects",
            previous_frame_visible_objects);
        field("previous_frame_scene_draw_calls",
            previous_frame_scene_draw_calls);
        field("previous_frame_scene_mesh_draw_calls",
            previous_frame_scene_mesh_draw_calls);
        field("previous_frame_scene_prim_draw_calls",
            previous_frame_scene_prim_draw_calls);
        field("previous_frame_scene_opaque_draw_calls",
            previous_frame_scene_opaque_draw_calls);
        field("previous_frame_scene_alpha_draw_calls",
            previous_frame_scene_alpha_draw_calls);
        field("previous_frame_scene_alpha_mask_draw_calls",
            previous_frame_scene_alpha_mask_draw_calls);
        field("previous_frame_scene_missing_mesh_draw_calls",
            previous_frame_scene_missing_mesh_draw_calls);
        field("previous_frame_scene_missing_mesh_objects",
            previous_frame_scene_missing_mesh_objects);
        field("previous_frame_scene_missing_mesh_triangles",
            previous_frame_scene_missing_mesh_triangles);
        out+="}";
        return out;
    }
};
}
