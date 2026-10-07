/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/protocol/environment_settings.hpp"

#include <cmath>
#include <string_view>

namespace
{
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;
using nexverse::protocol::Vec3;

bool read_number(const LlsdValue* value, float& out)
{
    if (value == nullptr)
    {
        return false;
    }

    if (value->type == LlsdType::real)
    {
        out = static_cast<float>(value->real_value);
    }
    else if (value->type == LlsdType::integer)
    {
        out = static_cast<float>(value->integer_value);
    }
    else
    {
        return false;
    }

    return std::isfinite(out);
}

bool read_vector(
    const LlsdValue* value,
    float* out,
    std::size_t count)
{
    if (value == nullptr ||
        !value->is_array() ||
        value->array_value.size() < count)
    {
        return false;
    }

    for (std::size_t index = 0U; index < count; ++index)
    {
        if (!read_number(&value->array_value[index], out[index]))
        {
            return false;
        }
    }

    return true;
}

bool read_vec4_as_vec3(const LlsdValue* value, Vec3& out)
{
    float values[4]{};
    if (!read_vector(value, values, 4U))
    {
        return false;
    }

    out = {values[0], values[1], values[2]};
    return true;
}

bool read_scalar_vector(const LlsdValue* value, float& out)
{
    if (read_number(value, out))
    {
        return true;
    }

    if (value == nullptr ||
        !value->is_array() ||
        value->array_value.empty())
    {
        return false;
    }

    return read_number(&value->array_value.front(), out);
}

std::string string_like(const LlsdValue* value)
{
    if (value == nullptr)
    {
        return {};
    }

    if (value->type == LlsdType::string ||
        value->type == LlsdType::uuid ||
        value->type == LlsdType::uri)
    {
        return std::string{value->as_string()};
    }

    return {};
}

const LlsdValue* select_sky_frame(const LlsdValue& root)
{
    if (!root.is_array() || root.array_value.size() < 3U)
    {
        return nullptr;
    }

    const LlsdValue& tracks = root.array_value[1U];
    const LlsdValue& frames = root.array_value[2U];

    if (!frames.is_map() || frames.map_value.empty())
    {
        return nullptr;
    }

    if (tracks.is_array() && !tracks.array_value.empty())
    {
        const LlsdValue& entry = tracks.array_value.front();
        if (entry.is_array() && entry.array_value.size() >= 2U)
        {
            const std::string frame_name =
                string_like(&entry.array_value[1U]);
            if (!frame_name.empty())
            {
                const auto found = frames.map_value.find(frame_name);
                if (found != frames.map_value.end() &&
                    found->second.is_map())
                {
                    return &found->second;
                }
            }
        }
    }

    return &frames.map_value.begin()->second;
}
}

namespace nexverse::protocol
{
std::optional<WindLightEnvironment>
parse_environment_settings(const LlsdValue& value)
{
    if (!value.is_array() || value.array_value.size() < 4U)
    {
        return std::nullopt;
    }

    const LlsdValue* sky = select_sky_frame(value);
    const LlsdValue& water = value.array_value[3U];

    if (sky == nullptr ||
        !sky->is_map() ||
        !water.is_map())
    {
        return std::nullopt;
    }

    WindLightEnvironment result;

    read_vec4_as_vec3(sky->find("ambient"), result.ambient);
    read_vec4_as_vec3(
        sky->find("blue_density"),
        result.blue_density);
    read_vec4_as_vec3(
        sky->find("blue_horizon"),
        result.blue_horizon);
    read_vec4_as_vec3(
        sky->find("sunlight_color"),
        result.sunlight_color);
    read_vec4_as_vec3(
        sky->find("cloud_color"),
        result.cloud_color);

    read_scalar_vector(
        sky->find("haze_density"),
        result.haze_density);
    read_scalar_vector(
        sky->find("haze_horizon"),
        result.haze_horizon);
    read_scalar_vector(
        sky->find("density_multiplier"),
        result.density_multiplier);
    read_scalar_vector(
        sky->find("distance_multiplier"),
        result.distance_multiplier);
    read_scalar_vector(
        sky->find("gamma"),
        result.gamma);
    read_number(
        sky->find("star_brightness"),
        result.star_brightness);
    read_number(
        sky->find("sun_angle"),
        result.sun_angle);
    read_number(
        sky->find("east_angle"),
        result.east_angle);
    read_scalar_vector(
        sky->find("cloud_shadow"),
        result.cloud_shadow);

    result.sky_name = string_like(sky->find("name"));

    read_vec4_as_vec3(
        water.find("waterFogColor"),
        result.water_fog_color);
    read_number(
        water.find("waterFogDensity"),
        result.water_fog_density);
    result.water_normal_map =
        string_like(water.find("normalMap"));

    if (!std::isfinite(result.sun_angle) ||
        !std::isfinite(result.east_angle) ||
        result.gamma <= 0.0f)
    {
        return std::nullopt;
    }

    return result;
}
}
