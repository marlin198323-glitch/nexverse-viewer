#include "nexverse/render/particle_emission.hpp"
#include "nexverse/render/ribbon_geometry.hpp"
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>

using namespace nexverse::render;

int main()
{
    // Births stop after SourceMaxAge, but still-living particles continue.
    const auto initial = particle_burst_age(0.4f,0.5f,2.0f,1.0f,0U);
    const auto latter = particle_burst_age(1.4f,0.5f,2.0f,1.0f,1U);
    const auto stopped = particle_burst_age(1.4f,0.5f,2.0f,1.0f,0U);
    if (!initial || !latter || stopped ||
        std::abs(*initial-0.4f)>0.0001f ||
        std::abs(*latter-0.9f)>0.0001f)
    {
        std::cerr << "SourceMaxAge emission lifecycle incorrect\n";
        return 1;
    }
    if (particle_burst_age(-1.0f,1.0f,1.0f,0.0f,0U) ||
        particle_burst_age(6.0f,1.0f,2.0f,0.0f,8U))
    {
        std::cerr << "Invalid or out-of-window birth accepted\n";
        return 2;
    }

    const float z=std::sin(std::numbers::pi_v<float>/4.0f);
    const float w=std::cos(std::numbers::pi_v<float>/4.0f);
    const auto rotated = rotate_particle_vector(
        {0.0f,0.0f,z,w},{1.0f,0.0f,0.0f});
    if (std::abs(rotated.x)>0.0002f ||
        std::abs(rotated.y-1.0f)>0.0002f)
    {
        std::cerr << "Object-relative quaternion rotation incorrect\n";
        return 3;
    }

    const std::vector<RenderVec3> points{
        {0.0f,0.0f,0.0f},
        {1.0f,0.0f,0.0f},
        {2.0f,0.5f,0.0f}};
    const auto ribbon = build_ribbon_triangles(
        points,{1.0f,2.0f,2.0f},0.2f,
        {1.0f,0.4f,0.2f,0.75f});
    if (ribbon.size()!=108U)
    {
        std::cerr << "Connected ribbon must contain four triangles\n";
        return 4;
    }
    for (float value:ribbon)
        if (!std::isfinite(value))
        {
            std::cerr << "Ribbon contains nonfinite vertex attribute\n";
            return 5;
        }
    if (!build_ribbon_triangles(points,{0.0f,0.0f,2.0f},0.0f,
        {1.0f,1.0f,1.0f,1.0f}).empty())
    {
        std::cerr << "Invalid ribbon width accepted\n";
        return 6;
    }
    std::cout << "Particle lifetimes, ObjectRelative and connected ribbons OK\n";
    return 0;
}
