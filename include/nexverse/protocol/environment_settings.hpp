/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/llsd.hpp"
#include "nexverse/protocol/viewer_messages.hpp"

#include <optional>
#include <string>

namespace nexverse::protocol
{
struct WindLightEnvironment
{
    Vec3 ambient{0.35f, 0.35f, 0.35f};
    Vec3 blue_density{0.12f, 0.22f, 0.38f};
    Vec3 blue_horizon{0.25f, 0.25f, 0.32f};
    Vec3 sunlight_color{0.72f, 0.76f, 0.90f};
    Vec3 cloud_color{0.41f, 0.41f, 0.41f};
    Vec3 water_fog_color{0.0156f, 0.149f, 0.2509f};

    float haze_density = 0.70f;
    float haze_horizon = 0.19f;
    float density_multiplier = 0.00018f;
    float distance_multiplier = 0.8f;
    float gamma = 1.0f;
    float star_brightness = 0.0f;
    float sun_angle = 0.317f;
    float east_angle = 0.0f;
    float cloud_shadow = 0.27f;
    float water_fog_density = 10.0f;

    std::string sky_name;
    std::string water_normal_map;
};

[[nodiscard]] std::optional<WindLightEnvironment>
parse_environment_settings(const LlsdValue& value);

[[nodiscard]] std::optional<WindLightEnvironment>
parse_ext_environment_settings(const LlsdValue& value);
}
