/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/abi.h"

#ifndef OGL_EXAMPLE_ID
#define OGL_EXAMPLE_ID "org.opengenesislink.example"
#endif
#ifdef _WIN32
#define OGL_EXPORT extern "C" __declspec(dllexport)
#else
#define OGL_EXPORT extern "C" __attribute__((visibility("default")))
#endif

namespace
{
const ogl_host_api* host_api = nullptr;
bool initialized = false;
bool running = false;

int init(const ogl_host_api* host)
{
    if (!host || host->abi_version != OGL_MODULE_ABI_VERSION ||
        host->struct_size < sizeof(ogl_host_api) || !host->log) return 1;
    host_api = host;
    initialized = true;
    host_api->log(host_api->context, 1, "OGL Example Module initialized");
    return 0;
}
int start()
{
    if (!initialized || running) return 2;
    running = true;
    host_api->log(host_api->context, 1, "OGL Example Module running");
    return 0;
}
void stop() { running = false; }
void shutdown() { running = false; initialized = false; host_api = nullptr; }

const ogl_module_api api = {
    sizeof(ogl_module_api), OGL_MODULE_ABI_VERSION, OGL_EXAMPLE_ID,
    &init, &start, &stop, &shutdown
};
}
OGL_EXPORT const ogl_module_api* ogl_module_get_api(uint32_t host_version)
{
    return host_version == OGL_MODULE_ABI_VERSION ? &api : nullptr;
}
