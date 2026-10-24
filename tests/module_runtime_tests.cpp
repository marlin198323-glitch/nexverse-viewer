/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/manifest.hpp"
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
void require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}
std::string read(const fs::path& file)
{
    std::ifstream in(file, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}
void write(const fs::path& file, const std::string& content)
{
    std::ofstream out(file, std::ios::trunc);
    out << content;
    require(bool(out), "Cannot create test manifest");
}
void replace_one(std::string& source, const std::string& old, const std::string& replacement)
{
    const auto where = source.find(old);
    require(where != std::string::npos, "Test manifest layout changed");
    source.replace(where, old.size(), replacement);
}
}
int main(int argc, char** argv)
{
    if (argc != 2) { std::cerr << "Missing build module directory\n"; return 1; }
    const auto source = fs::path(argv[1]);
    const auto manifest = read(source / "manifest.json");
    const auto parsed = parse_manifest(manifest);
    require(parsed.ok(), "Valid example manifest rejected");
    require(parsed.value->id == "org.opengenesislink.example", "Incorrect ID");
    require(version_at_least("0.2.0-alpha.38", "0.2.0-alpha.37"), "Version compare broken");
    require(!version_at_least("0.2.0-alpha.37", "0.2.0-alpha.38"), "Alpha comparison broken");
    require(version_at_least("0.2.0", "0.2.0-alpha.38"), "Release ordering broken");
    require(!version_at_least("0.1.9", "0.2.0"), "Major/minor comparison broken");
    // Four-part public versions and old alpha manifests must coexist.
    require(version_at_least("v0.3.0.0", "0.2.0-alpha.37"),
            "New public vX.Y.Z.W must meet legacy module minimum");
    require(version_at_least("v1.0.0.1", "v1.0.0.0"),
            "Four-part revision compare broken");
    require(!version_at_least("v1.0.0.0", "v1.0.0.1"),
            "Older four-part release accepted for newer minimum");
    require(!version_at_least("v1.0.0.0.1", "v1.0.0.0"),
            "Malformed five-part version accepted");

    std::string malformed = manifest;
    replace_one(malformed, "\"entrypoint\": \"ogl_example_module", "\"entrypoint\": \"../ogl_example_module");
    require(!parse_manifest(malformed).ok(), "Directory traversal entrypoint accepted");
    malformed = manifest;
    replace_one(malformed, "\"api_version\": 1", "\"api_version\": 3");
    require(!parse_manifest(malformed).ok(), "Incompatible API accepted");
    require(!parse_manifest("{\"id\":\"a\",\"id\":\"b\"}").ok(), "Duplicate JSON key accepted");
    require(!parse_manifest(std::string(65537, 'x')).ok(), "Oversized manifest accepted");

    const auto test_root = fs::temp_directory_path() /
        ("ogl-modules-test-" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    const auto moddir = test_root / "modules" / "org.opengenesislink.example";
    fs::create_directories(moddir);
    struct Cleanup { fs::path path; ~Cleanup() { std::error_code ec; fs::remove_all(path, ec); } } cleanup{test_root};
    write(moddir / "manifest.json", manifest);
    const auto binary = parsed.value->entrypoint;
    fs::copy_file(source / binary, moddir / binary);

    std::string error;
    ModuleOptions denied{test_root / "modules", test_root / "state.txt", false, {"developer"}};
    {
        ModuleManager manager(denied);
        manager.discover();
        require(manager.modules().size() == 1, "Example module not discovered");
        require(manager.modules().at(parsed.value->id).state == ModuleState::blocked,
                "Unsigned module not blocked by default");
        require(!manager.set_enabled(parsed.value->id, true, error),
                "Unsigned module enabled without developer mode");
    }
    ModuleOptions opts{test_root / "modules", test_root / "state.txt", true, {"developer"}};
    {
        ModuleManager manager(opts);
        manager.discover();
        require(manager.modules().at(parsed.value->id).state == ModuleState::disabled,
                "Module should be disabled on first install");
        require(manager.set_enabled(parsed.value->id, true, error), "Could not enable module");
        require(manager.start_enabled(error), "Could not start module");
        require(manager.modules().at(parsed.value->id).state == ModuleState::running,
                "Module failed to reach running state");
        require(manager.set_enabled(parsed.value->id, true, error),
                "Idempotent enable failed");
        require(manager.modules().at(parsed.value->id).state == ModuleState::running,
                "Idempotent enable reset a running module");
        require(manager.set_enabled(parsed.value->id, false, error), "Could not disable module");
        require(manager.modules().at(parsed.value->id).state == ModuleState::disabled,
                "Module remained active after disable");
    }
    {
        ModuleManager manager(opts);
        manager.discover();
        require(!manager.modules().at(parsed.value->id).enabled,
                "Disabled state was not persisted");
        require(manager.set_enabled(parsed.value->id, true, error), "Re-enable failed");
    }
    {
        ModuleManager manager(opts);
        manager.discover();
        require(manager.modules().at(parsed.value->id).enabled, "Enabled state not persisted");
        std::string with_missing_dependency = manifest;
        replace_one(with_missing_dependency, "\"dependencies\": []",
                    "\"dependencies\": [{\"id\":\"org.opengenesislink.notinstalled\",\"min_version\":\"0.1.0\"}]");
        write(moddir / "manifest.json", with_missing_dependency);
        manager.discover();
        require(!manager.start_enabled(error), "Missing dependency accepted");
        require(error.find("Dependency missing") != std::string::npos,
                "Dependency failure reason missing");
        write(moddir / "manifest.json", manifest);
    }
    {
        ModuleManager manager(opts);
        manager.discover();
        require(manager.start_enabled(error), "Restart after restored manifest failed");
        manager.stop_all();
    }
    std::cout << "Module manifest / ABI / lifecycle / persistence tests passed\n";
    return 0;
}
