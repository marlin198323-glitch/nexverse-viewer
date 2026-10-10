#include "nexverse/assets/texture_request_priority.hpp"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main()
{
    using nexverse::assets::next_texture_request;
    const std::vector<std::string> particles{
        "", "00000000-0000-0000-0000-000000000000",
        "smoke-texture", "smoke-texture", "fire-texture"};
    const std::vector<std::string> objects{
        "smoke-texture", "wall-texture", "fire-texture"};

    const auto first = next_texture_request(
        particles, objects, {}, {});
    if (!first || *first != "smoke-texture")
    {
        std::cerr << "Particle texture priority incorrect\n";
        return 1;
    }

    const auto second = next_texture_request(
        particles, objects, {"smoke-texture"}, {});
    if (!second || *second != "fire-texture")
    {
        std::cerr << "Repeated/loaded particle texture not skipped\n";
        return 2;
    }

    const auto fallback = next_texture_request(
        particles, objects, {"smoke-texture"}, {"fire-texture"});
    if (!fallback || *fallback != "wall-texture")
    {
        std::cerr << "Object texture fallback was not selected\n";
        return 3;
    }

    const auto none = next_texture_request(
        particles, objects, {"smoke-texture", "wall-texture"},
        {"fire-texture"});
    if (none)
    {
        std::cerr << "Already cached/failed texture requested again\n";
        return 4;
    }

    const auto no_texture = next_texture_request(
        {"", "00000000-0000-0000-0000-000000000000"},
        {}, {}, {});
    if (no_texture)
    {
        std::cerr << "Invalid particle asset UUID was requested\n";
        return 5;
    }

    std::cout << "Particle/object shared GetTexture priorities verified\n";
    return 0;
}
