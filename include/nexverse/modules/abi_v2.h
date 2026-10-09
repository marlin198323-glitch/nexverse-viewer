/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#ifndef OGL_MODULE_ABI_V2_H
#define OGL_MODULE_ABI_V2_H
#include "nexverse/modules/abi.h"

/* V1 header and layouts stay unchanged. V2 extends them by composition. */
#define OGL_MODULE_ABI_VERSION_2 2u
#define OGL_DIAGNOSTICS_VERSION_1 1u

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogl_diagnostics_snapshot_v1 {
    uint32_t struct_size;
    uint32_t version;
    uint64_t frame_index;
    double fps;
    double frame_time_ms;
    uint64_t draw_calls;
    uint64_t triangles;
    uint64_t visible_objects;
    uint64_t ram_total_bytes;
    uint64_t ram_available_bytes;
    uint64_t vram_total_bytes;
    uint64_t vram_available_bytes;
    uint32_t simulation_ready;
    uint32_t reserved;
} ogl_diagnostics_snapshot_v1;

typedef struct ogl_host_api_v2 {
    ogl_host_api base;
    /* Returns 0 for a valid copy, nonzero if no snapshot is published yet. */
    int (*read_diagnostics)(void* context, ogl_diagnostics_snapshot_v1* out);
} ogl_host_api_v2;

typedef struct ogl_module_api_v2 {
    ogl_module_api base;
    /* Main-thread callback, after a diagnostic snapshot has been published. */
    int (*on_frame)(void);
} ogl_module_api_v2;

#ifdef __cplusplus
}
#endif
#endif
