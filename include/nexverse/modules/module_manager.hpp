/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/modules/abi_v2.h"
#include "nexverse/modules/manifest.hpp"
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::modules
{
enum class ModuleState { disabled, blocked, ready, initialized, running, failed };

struct ModuleInfo
{
    Manifest manifest;
    std::filesystem::path directory;
    ModuleState state = ModuleState::disabled;
    bool enabled = false;
    std::string error;
};

struct ModuleOptions
{
    std::filesystem::path modules_directory;
    std::filesystem::path state_file;
    bool developer_mode = false; // Explicit opt-in for unsigned, in-process native code.
    std::set<std::string, std::less<>> allowed_capabilities;
};

[[nodiscard]] ModuleOptions default_module_options();

class ModuleManager
{
public:
    explicit ModuleManager(ModuleOptions options);
    ~ModuleManager();
    ModuleManager(const ModuleManager&) = delete;
    ModuleManager& operator=(const ModuleManager&) = delete;

    void discover();
    [[nodiscard]] const std::map<std::string, ModuleInfo, std::less<>>& modules() const noexcept;
    [[nodiscard]] bool set_enabled(std::string_view id, bool enabled, std::string& error);
    [[nodiscard]] bool start_enabled(std::string& error);
    void stop_all() noexcept;
    void update_diagnostics(const ogl_diagnostics_snapshot_v1& snapshot) noexcept;
    [[nodiscard]] std::size_t tick() noexcept;

private:
    struct Loaded
    {
        void* handle = nullptr;
        const ogl_module_api* api = nullptr;
    };
    ModuleOptions options_;
    std::map<std::string, ModuleInfo, std::less<>> modules_;
    std::map<std::string, Loaded, std::less<>> loaded_;
    std::vector<std::string> load_order_;
    std::set<std::string, std::less<>> enabled_;
    ogl_host_api host_api_{};
    ogl_host_api_v2 host_api_v2_{};
    ogl_diagnostics_snapshot_v1 diagnostics_{};
    bool diagnostics_ready_ = false;

    void read_state();
    [[nodiscard]] bool write_state(std::string& error) const;
    [[nodiscard]] bool compatible(ModuleInfo& info);
    [[nodiscard]] bool load_one(std::string_view id, std::string& error);
    [[nodiscard]] bool start_one(const std::string& id, std::set<std::string>& visiting,
                                 std::set<std::string>& finished, std::string& error);
    void unload_one(std::string_view id) noexcept;
    static void host_log(void* context, int level, const char* message);
    static int read_diagnostics(void* context, ogl_diagnostics_snapshot_v1* out);
};
}
