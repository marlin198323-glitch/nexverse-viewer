/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/abi_v2.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>

#if defined(_WIN32)
#define OGL_EXPORT extern "C" __declspec(dllexport)
#else
#define OGL_EXPORT extern "C" __attribute__((visibility("default")))
#endif
namespace {
const ogl_host_api_v2* host = nullptr;
bool running = false;
uint32_t samples = 0;
double frame_sum = 0;
double frame_min = std::numeric_limits<double>::max();
double frame_max = 0;

void reset()
{
    samples = 0;
    frame_sum = 0;
    frame_min = std::numeric_limits<double>::max();
    frame_max = 0;
}
int init(const ogl_host_api* base)
{
    if (!base || base->abi_version != OGL_MODULE_ABI_VERSION_2 ||
        base->struct_size < sizeof(ogl_host_api_v2)) return 1;
    const auto* expanded = reinterpret_cast<const ogl_host_api_v2*>(base);
    if (!expanded->read_diagnostics || !expanded->base.log) return 2;
    host = expanded;
    return 0;
}
int start()
{
    if (!host || running) return 1;
    reset();
    running = true;
    host->base.log(host->base.context, 1, "OGL Performance Monitor running");
    return 0;
}
int on_frame()
{
    if (!running || !host) return 1;
    ogl_diagnostics_snapshot_v1 snapshot{};
    snapshot.struct_size = sizeof(snapshot);
    snapshot.version = OGL_DIAGNOSTICS_VERSION_1;
    if (host->read_diagnostics(host->base.context, &snapshot) != 0) return 0;
    const double ms = snapshot.frame_time_ms;
    if (!std::isfinite(ms) || ms <= 0 || ms > 1000) return 0;
    ++samples;
    frame_sum += ms;
    frame_min = std::min(frame_min, ms);
    frame_max = std::max(frame_max, ms);
    if (samples == 300)
    {
        const double average = frame_sum / static_cast<double>(samples);
        char line[256];
        std::snprintf(line, sizeof(line),
            "Performance: FPS %.1f | frame avg/min/max %.2f/%.2f/%.2f ms | RAM free %llu MiB | VRAM free %llu MiB",
            1000.0 / average, average, frame_min, frame_max,
            static_cast<unsigned long long>(snapshot.ram_available_bytes / 1048576ULL),
            static_cast<unsigned long long>(snapshot.vram_available_bytes / 1048576ULL));
        host->base.log(host->base.context, 1, line);
        reset();
    }
    return 0;
}
void stop() { running = false; }
void shutdown() { running = false; host = nullptr; reset(); }
const ogl_module_api_v2 api = {
    {sizeof(ogl_module_api_v2), OGL_MODULE_ABI_VERSION_2,
     "org.opengenesislink.performance-monitor", &init, &start, &stop, &shutdown},
    &on_frame
};
}
OGL_EXPORT const ogl_module_api* ogl_module_get_api(uint32_t version)
{
    return version == OGL_MODULE_ABI_VERSION_2 ? &api.base : nullptr;
}
