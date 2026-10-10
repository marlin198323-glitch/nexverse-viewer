#include "nexverse/render/particle_motion.hpp"
#include <cmath>
#include <iostream>

using namespace nexverse::render;

bool near(float actual, float expected, float eps = 0.07f)
{
    return std::abs(actual - expected) < eps;
}

int main()
{
    const RenderVec3 origin{10.0f, 20.0f, 5.0f};
    const RenderVec3 velocity{0.0f, 0.0f, 0.0f};
    const RenderVec3 accel{0.0f, 0.0f, -9.8f};

    const auto gravity = sample_particle_motion(
        origin, velocity, accel, {}, 1.0f, 2.0f, 0U);
    if (!(gravity.position.z < origin.z) ||
        !near(gravity.fraction, 0.5f))
    {
        std::cerr << "Particle gravity interpolation missing\n";
        return 1;
    }

    const auto bounce = sample_particle_motion(
        origin, velocity, accel, {}, 1.0f, 2.0f, particle_bounce);
    if (bounce.position.z < origin.z)
    {
        std::cerr << "Particle bounce crossed source floor\n";
        return 2;
    }

    const RenderVec3 wind{4.0f, 1.0f, 0.0f};
    const auto drifting = sample_particle_motion(
        origin, velocity, {}, wind, 2.0f, 4.0f, particle_wind);
    if (!(drifting.position.x > origin.x + 1.0f) ||
        !(drifting.position.y > origin.y))
    {
        std::cerr << "Particle wind acceleration missing\n";
        return 3;
    }

    const RenderVec3 target{40.0f, 30.0f, 8.0f};
    const auto linear = sample_particle_motion(
        origin, {1.0f, 2.0f, 3.0f}, accel,
        {}, 1.0f, 2.0f, particle_target_linear,
        true, target);
    if (!near(linear.position.x, 25.0f, 0.001f) ||
        !near(linear.position.y, 25.0f, 0.001f) ||
        !near(linear.position.z, 6.5f, 0.001f))
    {
        std::cerr << "Linear particle target placement incorrect\n";
        return 4;
    }
    const auto guided = sample_particle_motion(
        origin, {}, {}, {}, 2.0f, 2.0f,
        particle_target_position, true, target);
    if (!near(guided.position.x, target.x, 0.001f) ||
        !near(guided.position.z, target.z, 0.001f))
    {
        std::cerr << "TargetPos particle did not reach target\n";
        return 5;
    }
    const auto unresolved = sample_particle_motion(
        origin, {}, {}, {}, 2.0f, 2.0f,
        particle_target_position, false, target);
    if (!near(unresolved.position.x, origin.x, 0.001f))
    {
        std::cerr << "Unresolved target incorrectly moved particle\n";
        return 6;
    }
    const auto capped = sample_particle_motion(
        origin, {}, {}, {}, 9999.0f, 1.0f, 0U);
    if (!std::isfinite(capped.position.x) ||
        !near(capped.fraction, 1.0f, 0.001f))
    {
        std::cerr << "Age bound and finite integration failed\n";
        return 7;
    }
    std::cout << "Particle physics, bounce, wind and target tests passed\n";
    return 0;
}
