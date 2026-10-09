/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/abi_v2.h"
#include "nexverse/modules/module_manager.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;
using namespace nexverse::modules;

namespace
{
void check(bool ok, const char* message)
{
    if (!ok) throw std::runtime_error(message);
}
void copy_module(const fs::path& src, const fs::path& dst)
{
    fs::create_directories(dst);
    const auto manifest_src = src / "manifest.json";
    std::ifstream in(manifest_src);
    const std::string json{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    const auto parsed = parse_manifest(json);
    check(parsed.ok(), "Invalid built test manifest");
    fs::copy_file(manifest_src, dst / "manifest.json");
    fs::copy_file(src / parsed.value->entrypoint, dst / parsed.value->entrypoint);
}
}
int main(int argc, char** argv)
{
    if (argc != 2) { std::cerr << "Usage: developer_modules_tests <build modules>\n"; return 1; }
    const fs::path source{argv[1]};
    const auto temp = fs::temp_directory_path() /
        ("ogl-developer-tests-" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    struct Remove { fs::path p; ~Remove() { std::error_code ec; fs::remove_all(p, ec); } } cleanup{temp};
    const auto base = temp / "modules";
    const std::string dev = "org.opengenesislink.developer-tools";
    const std::string perf = "org.opengenesislink.performance-monitor";
    const std::string example = "org.opengenesislink.example";
    for (const auto& id : {dev, perf, example})
        copy_module(source / id, base / id);

    auto dev_manifest = parse_manifest([&] {
        std::ifstream in(base / dev / "manifest.json");
        return std::string{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    }());
    check(dev_manifest.ok() && dev_manifest.value->api_version == OGL_MODULE_ABI_VERSION_2,
          "Developer Tools ABI v2 manifest rejected");
    check(dev_manifest.value->capabilities.size() == 1 &&
          dev_manifest.value->capabilities.front() == "developer",
          "Developer Tools capability declaration invalid");

    const fs::path state = temp / "enabled.txt";
    ModuleOptions opts{base, state, true, {"developer"}};
    {
        ModuleOptions denied{base, temp / "denied.txt", false, {"developer"}};
        ModuleManager manager(denied);
        manager.discover();
        check(manager.modules().size() == 3, "Did not discover all v1/v2 test modules");
        std::string error;
        check(!manager.set_enabled(dev, true, error),
              "V2 unsigned module activated outside developer mode");
    }
    {
        ModuleOptions denied_caps{base, temp / "denied-caps.txt", true, {}};
        ModuleManager manager(denied_caps);
        manager.discover();
        std::string error;
        check(!manager.set_enabled(dev, true, error),
              "V2 module ignored denied capability");
    }
    std::string error;
    {
        ModuleManager manager(opts);
        manager.discover();
        check(manager.set_enabled(perf, true, error), "Cannot activate performance module");
        check(!manager.start_enabled(error), "Disabled dependency was accepted");
        check(error.find("disabled") != std::string::npos,
              "Dependency rejection did not describe disabled dependency");
        check(manager.set_enabled(dev, true, error), "Cannot activate developer module");
        check(manager.set_enabled(example, true, error), "Cannot activate legacy module");
        check(manager.start_enabled(error), "Cannot start ABI v1 + v2 modules");
        check(manager.modules().at(dev).state == ModuleState::running,
              "Developer Tools not running");
        check(manager.modules().at(perf).state == ModuleState::running,
              "Performance Monitor not running");
        check(manager.modules().at(example).state == ModuleState::running,
              "ABI v1 backwards compatibility broken");

        // No diagnostics until first published snapshot: callbacks still safe.
        check(manager.tick() == 2, "Only v2 modules should receive frame callback");
        ogl_diagnostics_snapshot_v1 snapshot{};
        snapshot.struct_size = sizeof(snapshot);
        snapshot.version = OGL_DIAGNOSTICS_VERSION_1;
        snapshot.fps = 60.0;
        snapshot.frame_time_ms = 16.67;
        snapshot.draw_calls = 120;
        snapshot.triangles = 4500;
        snapshot.visible_objects = 70;
        snapshot.ram_total_bytes = 32ULL * 1024ULL * 1024ULL * 1024ULL;
        snapshot.ram_available_bytes = 16ULL * 1024ULL * 1024ULL * 1024ULL;
        snapshot.simulation_ready = 1;
        for (std::uint64_t i = 1; i <= 305; ++i)
        {
            snapshot.frame_index = i;
            manager.update_diagnostics(snapshot);
            check(manager.tick() == 2, "Frame diagnostics dispatch failed");
        }
        check(!manager.set_enabled(dev, false, error),
              "Dependency protection failed during disable");
        check(manager.set_enabled(perf, false, error), "Cannot disable performance module");
        check(manager.set_enabled(dev, false, error), "Cannot disable developer tools");
        check(manager.set_enabled(example, false, error), "Cannot disable v1 example");
    }
    {
        ModuleManager manager(opts);
        manager.discover();
        check(!manager.modules().at(dev).enabled && !manager.modules().at(perf).enabled,
              "Enabled state was not persisted");
        check(manager.start_enabled(error), "Empty enabled module set should start");
        check(manager.tick() == 0, "Unexpected callbacks after disable");
    }
    std::cout << "ABI v1/v2 Developer Tools and Performance Monitor lifecycle passed\n";
    return 0;
}
