/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/module_manager.hpp"
#include "nexverse/core/logger.hpp"
#include "nexverse/core/version.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <stdexcept>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace nexverse::modules
{
namespace
{
std::filesystem::path config_root()
{
#ifdef _WIN32
    if (const char* appdata = std::getenv("APPDATA"))
        return std::filesystem::path(appdata) / "OpenGenesisLINK";
#else
    if (const char* xdg = std::getenv("XDG_CONFIG_HOME"))
        if (*xdg != 0) return std::filesystem::path(xdg) / "opengenesislink";
    if (const char* home = std::getenv("HOME"))
    {
#ifdef __APPLE__
        return std::filesystem::path(home) / "Library/Application Support/OpenGenesisLINK";
#else
        return std::filesystem::path(home) / ".config/opengenesislink";
#endif
    }
#endif
    return {};
}
bool contains(const std::vector<std::string>& a, const std::string& x)
{
    return std::find(a.begin(), a.end(), x) != a.end();
}
void close_lib(void* handle) noexcept
{
    if (!handle) return;
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif
}
void* open_lib(const std::filesystem::path& path)
{
#ifdef _WIN32
    return static_cast<void*>(LoadLibraryW(path.c_str()));
#else
    return dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
}
ogl_module_get_api_fn lookup(void* handle)
{
#ifdef _WIN32
    return reinterpret_cast<ogl_module_get_api_fn>(
        GetProcAddress(static_cast<HMODULE>(handle), OGL_MODULE_ENTRY_SYMBOL));
#else
    return reinterpret_cast<ogl_module_get_api_fn>(dlsym(handle, OGL_MODULE_ENTRY_SYMBOL));
#endif
}
std::string library_error()
{
#ifdef _WIN32
    return "LoadLibrary/GetProcAddress failed (Windows error " + std::to_string(GetLastError()) + ")";
#else
    const char* err = dlerror();
    return err ? std::string(err) : "Dynamic library load failed";
#endif
}
}

ModuleOptions default_module_options()
{
    auto root = config_root();
    if (root.empty()) return {};
    ModuleOptions opts;
    opts.modules_directory = root / "modules";
    opts.state_file = root / "enabled-modules.txt";
    return opts;
}

void ModuleManager::host_log(void*, int level, const char* message)
{
    if (!message) return;
    const std::string text = std::string("[OGL-MODULE] ") + message;
    if (level >= 3) core::error(text);
    else if (level == 2) core::warning(text);
    else core::info(text);
}
ModuleManager::ModuleManager(ModuleOptions options) : options_(std::move(options))
{
    host_api_ = {sizeof(ogl_host_api), OGL_MODULE_ABI_VERSION, this, &ModuleManager::host_log};
    host_api_v2_ = {{sizeof(ogl_host_api_v2), OGL_MODULE_ABI_VERSION_2,
                     this, &ModuleManager::host_log}, &ModuleManager::read_diagnostics};
}
ModuleManager::~ModuleManager() { stop_all(); }

const std::map<std::string, ModuleInfo, std::less<>>& ModuleManager::modules() const noexcept
{
    return modules_;
}
void ModuleManager::read_state()
{
    enabled_.clear();
    if (options_.state_file.empty()) return;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(options_.state_file, ec)) return;
    if (std::filesystem::file_size(options_.state_file, ec) > 16384U || ec)
    {
        core::warning("[OGL-MODULE] Ignoring oversized activation file");
        return;
    }
    std::ifstream in(options_.state_file);
    std::string id;
    while (std::getline(in, id))
    {
        if (!id.empty() && id.back() == '\r') id.pop_back();
        if (!id.empty() && id.front() != '#') enabled_.insert(id);
    }
}
bool ModuleManager::write_state(std::string& error) const
{
    if (options_.state_file.empty()) { error = "Module state path is not configured"; return false; }
    std::error_code ec;
    std::filesystem::create_directories(options_.state_file.parent_path(), ec);
    if (ec) { error = "Cannot create module state directory: " + ec.message(); return false; }
    const auto temp = std::filesystem::path(options_.state_file.string() + ".tmp");
    {
        std::ofstream out(temp, std::ios::trunc);
        if (!out) { error = "Cannot write module state"; return false; }
        for (const auto& id : enabled_) out << id << '\n';
        out.flush();
        if (!out) { error = "Cannot flush module state"; return false; }
    }
#ifdef _WIN32
    if (!MoveFileExW(temp.c_str(), options_.state_file.c_str(),
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        error = "Cannot atomically replace module state"; return false;
    }
#else
    std::filesystem::rename(temp, options_.state_file, ec);
    if (ec) { error = "Cannot replace module state: " + ec.message(); return false; }
#endif
    return true;
}
bool ModuleManager::compatible(ModuleInfo& info)
{
    const auto& m = info.manifest;
    auto block = [&](std::string message)
    {
        info.state = ModuleState::blocked;
        info.error = std::move(message);
        return false;
    };
    if (m.api_version != OGL_MODULE_ABI_VERSION &&
        m.api_version != OGL_MODULE_ABI_VERSION_2)
        return block("Module ABI is incompatible");
    if (!version_at_least(core::version(), m.minimum_viewer_version))
        return block("Viewer version is below module requirement");
    if (!contains(m.platforms, host_platform()) ||
        !contains(m.architectures, host_architecture()))
        return block("Module does not support this platform/architecture");
    // The production signature verifier does not exist yet: never call signed
    // packages verified or let signature metadata bypass this restriction.
    if (!m.signature.empty() || !m.package_hash.empty())
        return block("Package signatures/hashes not yet supported by local runtime");
    if (!options_.developer_mode)
        return block("Unsigned native modules require explicit developer mode");
    for (const auto& cap : m.capabilities)
        if (!options_.allowed_capabilities.contains(cap))
            return block("Module capability not granted: " + cap);
    std::error_code ec;
    const auto lib = info.directory / m.entrypoint;
    if (std::filesystem::is_symlink(std::filesystem::symlink_status(lib, ec)) ||
        !std::filesystem::is_regular_file(lib, ec) || ec)
        return block("Module binary is missing or is a symlink");
    info.state = info.enabled ? ModuleState::ready : ModuleState::disabled;
    info.error.clear();
    return true;
}
void ModuleManager::discover()
{
    stop_all();
    modules_.clear();
    read_state();
    if (options_.modules_directory.empty()) return;
    std::error_code ec;
    if (!std::filesystem::is_directory(options_.modules_directory, ec)) return;

    std::filesystem::directory_iterator it(options_.modules_directory, ec), end;
    for (; !ec && it != end; it.increment(ec))
    {
        if (it->is_symlink(ec) || !it->is_directory(ec)) continue;
        const auto directory = it->path();
        const auto manifest_path = directory / "manifest.json";
        if (std::filesystem::is_symlink(std::filesystem::symlink_status(manifest_path, ec)) ||
            !std::filesystem::is_regular_file(manifest_path, ec) || ec)
        {
            ec.clear();
            continue;
        }
        const auto size = std::filesystem::file_size(manifest_path, ec);
        if (ec || size > 65536U) { ec.clear(); continue; }
        std::ifstream in(manifest_path, std::ios::binary);
        if (!in) continue;
        std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        auto parsed = parse_manifest(text);
        if (!parsed.ok())
        {
            core::warning("[OGL-MODULE] Invalid manifest: " + directory.filename().string() +
                          ": " + parsed.error);
            continue;
        }
        auto m = std::move(*parsed.value);
        if (directory.filename() != m.id)
        {
            core::warning("[OGL-MODULE] Directory does not match module ID: " + m.id);
            continue;
        }
        ModuleInfo info;
        info.enabled = enabled_.contains(m.id);
        info.directory = directory;
        info.manifest = std::move(m);
        (void)compatible(info);
        const auto id = info.manifest.id;
        modules_.emplace(id, std::move(info));
    }
    if (ec) core::warning("[OGL-MODULE] Directory scan failed: " + ec.message());
}

bool ModuleManager::set_enabled(std::string_view id, bool enabled, std::string& error)
{
    error.clear();
    auto it = modules_.find(id);
    if (it == modules_.end()) { error = "Module not installed"; return false; }
    auto& info = it->second;
    if (info.enabled == enabled) return true;
    if (enabled && !compatible(info)) { error = info.error; return false; }
    if (!enabled)
    {
        for (const auto& [other_id, other] : modules_)
            if (other_id != id && other.enabled)
                for (const auto& dep : other.manifest.dependencies)
                    if (dep.id == id)
                    {
                        error = "Module required by enabled dependency: " + other_id;
                        return false;
                    }
    }
    const auto old_enabled = enabled_;
    if (enabled) enabled_.insert(info.manifest.id);
    else enabled_.erase(info.manifest.id);
    if (!write_state(error))
    {
        enabled_ = old_enabled;
        return false;
    }
    if (!enabled) unload_one(id);
    info.enabled = enabled;
    info.state = enabled ? ModuleState::ready : ModuleState::disabled;
    info.error.clear();
    return true;
}
bool ModuleManager::load_one(std::string_view id, std::string& error)
{
    auto& info = modules_.at(std::string(id));
    if (!compatible(info)) { error = info.error; return false; }
    const auto path = info.directory / info.manifest.entrypoint;
    void* handle = open_lib(path);
    if (!handle)
    {
        error = "Could not load native module: " + library_error();
        info.state = ModuleState::failed;
        info.error = error;
        return false;
    }
    const auto entry = lookup(handle);
    const ogl_module_api* api = nullptr;
    try { if (entry) api = entry(info.manifest.api_version); }
    catch (...) { /* An untrusted plugin violated the no-exceptions ABI contract. */ }
    if (!api || api->struct_size < sizeof(ogl_module_api) ||
        api->abi_version != info.manifest.api_version || !api->module_id ||
        std::string_view(api->module_id) != id || !api->init ||
        !api->start || !api->stop || !api->shutdown)
    {
        close_lib(handle);
        error = "Module entrypoint or ABI contract invalid";
        info.state = ModuleState::failed;
        info.error = error;
        return false;
    }
    if (info.manifest.api_version == OGL_MODULE_ABI_VERSION_2 &&
        (api->struct_size < sizeof(ogl_module_api_v2) ||
         !reinterpret_cast<const ogl_module_api_v2*>(api)->on_frame))
    {
        close_lib(handle);
        error = "V2 module has no valid on_frame callback";
        info.state = ModuleState::failed;
        info.error = error;
        return false;
    }
    const ogl_host_api* host = info.manifest.api_version == OGL_MODULE_ABI_VERSION_2
        ? &host_api_v2_.base : &host_api_;
    int result = -1;
    try { result = api->init(host); }
    catch (...) { result = -1; }
    if (result != 0)
    {
        close_lib(handle);
        error = "Module initialization failed";
        info.state = ModuleState::failed;
        info.error = error;
        return false;
    }
    loaded_.emplace(std::string(id), Loaded{handle, api});
    load_order_.push_back(std::string(id));
    info.state = ModuleState::initialized;
    return true;
}

bool ModuleManager::start_one(const std::string& id,
                              std::set<std::string>& visiting,
                              std::set<std::string>& finished, std::string& error)
{
    if (finished.contains(id)) return true;
    if (!visiting.insert(id).second)
    {
        error = "Circular module dependency detected: " + id;
        return false;
    }
    auto it = modules_.find(id);
    if (it == modules_.end() || !it->second.enabled)
    {
        error = "Required module is missing or disabled: " + id;
        visiting.erase(id);
        return false;
    }
    auto& info = it->second;
    if (!compatible(info))
    {
        error = id + ": " + info.error;
        visiting.erase(id);
        return false;
    }
    for (const auto& dep : info.manifest.dependencies)
    {
        const auto other = modules_.find(dep.id);
        if (other == modules_.end() ||
            !version_at_least(other->second.manifest.version, dep.min_version))
        {
            error = "Dependency missing or version mismatch: " + dep.id;
            visiting.erase(id);
            return false;
        }
        if (!start_one(dep.id, visiting, finished, error))
        {
            visiting.erase(id);
            return false;
        }
    }
    if (!loaded_.contains(id) && !load_one(id, error))
    {
        visiting.erase(id);
        return false;
    }
    int result = -1;
    try { result = loaded_.at(id).api->start(); }
    catch (...) { result = -1; }
    if (result != 0)
    {
        error = "Module start failed: " + id;
        info.state = ModuleState::failed;
        info.error = error;
        visiting.erase(id);
        return false;
    }
    info.state = ModuleState::running;
    visiting.erase(id);
    finished.insert(id);
    core::info("[OGL-MODULE] Started " + id);
    return true;
}
bool ModuleManager::start_enabled(std::string& error)
{
    error.clear();
    // Always shut down first: lifecycle calls are never duplicated on restart.
    stop_all();
    std::set<std::string> visiting, finished;
    for (const auto& [id, info] : modules_)
    {
        if (!info.enabled) continue;
        if (!start_one(id, visiting, finished, error))
        {
            core::warning("[OGL-MODULE] " + error);
            stop_all(); // Roll back everything started in this batch.
            return false;
        }
    }
    return true;
}
void ModuleManager::unload_one(std::string_view id) noexcept
{
    const auto it = loaded_.find(id);
    if (it == loaded_.end()) return;
    auto& info = modules_.at(std::string(id));
    if (info.state == ModuleState::running || info.state == ModuleState::failed)
    {
        try { it->second.api->stop(); } catch (...) {}
    }
    try { it->second.api->shutdown(); } catch (...) {}
    close_lib(it->second.handle);
    info.state = info.enabled ? ModuleState::ready : ModuleState::disabled;
    loaded_.erase(it);
    load_order_.erase(std::remove(load_order_.begin(), load_order_.end(), id),
                      load_order_.end());
}
void ModuleManager::stop_all() noexcept
{
    while (!load_order_.empty()) unload_one(load_order_.back());
}
void ModuleManager::update_diagnostics(const ogl_diagnostics_snapshot_v1& value) noexcept
{
    if (value.struct_size < sizeof(ogl_diagnostics_snapshot_v1) ||
        value.version != OGL_DIAGNOSTICS_VERSION_1) return;
    diagnostics_ = value;
    diagnostics_.struct_size = sizeof(diagnostics_);
    diagnostics_.version = OGL_DIAGNOSTICS_VERSION_1;
    diagnostics_ready_ = true;
}
int ModuleManager::read_diagnostics(void* context, ogl_diagnostics_snapshot_v1* out)
{
    if (!context || !out || out->struct_size < sizeof(ogl_diagnostics_snapshot_v1) ||
        out->version != OGL_DIAGNOSTICS_VERSION_1) return 1;
    auto& manager = *static_cast<ModuleManager*>(context);
    if (!manager.diagnostics_ready_) return 2;
    *out = manager.diagnostics_;
    return 0;
}
std::size_t ModuleManager::tick() noexcept
{
    std::size_t invoked = 0;
    for (const auto& id : load_order_)
    {
        const auto it = loaded_.find(id);
        if (it == loaded_.end() || modules_.at(id).state != ModuleState::running ||
            it->second.api->abi_version != OGL_MODULE_ABI_VERSION_2) continue;
        const auto* api = reinterpret_cast<const ogl_module_api_v2*>(it->second.api);
        int result = -1;
        try { result = api->on_frame(); } catch (...) { result = -1; }
        if (result != 0)
        {
            core::error("[OGL-MODULE] Frame callback failed: " + id);
            stop_all();
            break;
        }
        ++invoked;
    }
    return invoked;
}
}
