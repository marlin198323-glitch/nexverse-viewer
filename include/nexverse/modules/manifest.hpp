/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::modules
{
struct Dependency
{
    std::string id;
    std::string min_version;
};

struct Manifest
{
    std::string id;
    std::string name;
    std::string version;
    std::string vendor;
    std::uint32_t api_version = 0;
    std::string minimum_viewer_version;
    std::string type;
    std::vector<std::string> platforms;
    std::vector<std::string> architectures;
    std::vector<std::string> capabilities;
    std::vector<Dependency> dependencies;
    std::string entrypoint;
    std::string package_hash;
    std::string signature;
};

struct ManifestResult
{
    std::optional<Manifest> value;
    std::string error;
    [[nodiscard]] bool ok() const noexcept { return value.has_value(); }
};

[[nodiscard]] ManifestResult parse_manifest(std::string_view json);
[[nodiscard]] bool version_at_least(std::string_view actual, std::string_view minimum);
[[nodiscard]] std::string host_platform();
[[nodiscard]] std::string host_architecture();
}
