/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#ifndef OGL_MODULE_ABI_H
#define OGL_MODULE_ABI_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ABI v1 is a frozen native boundary. Do not change layout without bumping version. */
#define OGL_MODULE_ABI_VERSION 1u
#define OGL_MODULE_ENTRY_SYMBOL "ogl_module_get_api"

typedef struct ogl_host_api {
    uint32_t struct_size;
    uint32_t abi_version;
    void* context;
    /* message is a NUL-terminated UTF-8 string, valid only during the call. */
    void (*log)(void* context, int level, const char* message);
} ogl_host_api;

typedef struct ogl_module_api {
    uint32_t struct_size;
    uint32_t abi_version;
    const char* module_id;
    /* Return 0 for success; never throw across the ABI. */
    int (*init)(const ogl_host_api* host);
    int (*start)(void);
    void (*stop)(void);
    void (*shutdown)(void);
} ogl_module_api;

/* Export this exact symbol with C linkage from a native module. */
typedef const ogl_module_api* (*ogl_module_get_api_fn)(uint32_t host_abi_version);

#ifdef __cplusplus
}
#endif
#endif
