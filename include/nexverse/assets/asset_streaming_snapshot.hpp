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
    std::size_t mesh_gpu_upload_failures=0U;
    std::size_t texture_priority_churn=0U;
    std::size_t mesh_priority_churn=0U;
    std::uint64_t previous_frame_draw_calls=0U;
    std::uint64_t previous_frame_visible_objects=0U;
    std::uint64_t previous_frame_scene_draw_calls=0U;
    std::uint64_t previous_frame_scene_mesh_draw_calls=0U;
    std::uint64_t previous_frame_scene_prim_draw_calls=0U;
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
        field("mesh_gpu_upload_failures",mesh_gpu_upload_failures);
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
