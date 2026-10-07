/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/protocol/environment_settings.hpp"

#include <algorithm>
#include <cmath>
#include <string_view>

namespace
{
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;
using nexverse::protocol::Vec3;

constexpr float two_pi = 6.28318530717958647692f;

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

bool read_vec3_flexible(const LlsdValue* value, Vec3& out)
{
    float values[3]{};
    if (!read_vector(value, values, 3U))
    {
        return false;
    }

    out = {values[0], values[1], values[2]};
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

const LlsdValue* select_legacy_sky_frame(const LlsdValue& root)
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

const LlsdValue* select_eep_frame(
    const LlsdValue& frames,
    const LlsdValue* track)
{
    if (!frames.is_map() || frames.map_value.empty())
    {
        return nullptr;
    }

    if (track != nullptr &&
        track->is_array() &&
        !track->array_value.empty())
    {
        const LlsdValue& entry = track->array_value.front();
        if (entry.is_map())
        {
            const std::string frame_name =
                string_like(entry.find("key_name"));
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

    return nullptr;
}

void quaternion_to_angles(
    const LlsdValue* value,
    float& sun_angle,
    float& east_angle)
{
    float q[4]{};
    if (!read_vector(value, q, 4U))
    {
        return;
    }

    const float norm = std::sqrt(
        q[0] * q[0] +
        q[1] * q[1] +
        q[2] * q[2] +
        q[3] * q[3]);
    if (norm <= 0.000001f)
    {
        return;
    }

    for (float& component : q)
    {
        component /= norm;
    }

    const float vx =
        2.0f * (q[0] * q[0] + q[3] * q[3]) - 1.0f;
    const float vy =
        2.0f * (q[0] * q[1] + q[2] * q[3]);
    const float vz =
        2.0f * (q[0] * q[2] - q[1] * q[3]);

    sun_angle = std::asin(std::clamp(vz, -1.0f, 1.0f));
    east_angle = -std::atan2(vy, vx);

    if (sun_angle < 0.0f)
    {
        sun_angle += two_pi;
    }
    if (east_angle < 0.0f)
    {
        east_angle += two_pi;
    }
}

bool validate_environment(
    const nexverse::protocol::WindLightEnvironment& result)
{
    return
        std::isfinite(result.sun_angle) &&
        std::isfinite(result.east_angle) &&
        std::isfinite(result.gamma) &&
        result.gamma > 0.0f;
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

    const LlsdValue* sky = select_legacy_sky_frame(value);
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

    if (!validate_environment(result))
    {
        return std::nullopt;
    }

    return result;
}

std::optional<WindLightEnvironment>
parse_ext_environment_settings(const LlsdValue& value)
{
    if (!value.is_map())
    {
        return std::nullopt;
    }

    const LlsdValue* environment = value.find("environment");
    if (environment == nullptr || !environment->is_map())
    {
        return std::nullopt;
    }

    const LlsdValue* day_cycle = environment->find("day_cycle");
    if (day_cycle == nullptr || !day_cycle->is_map())
    {
        return std::nullopt;
    }

    const LlsdValue* frames = day_cycle->find("frames");
    const LlsdValue* tracks = day_cycle->find("tracks");
    if (frames == nullptr ||
        !frames->is_map() ||
        tracks == nullptr ||
        !tracks->is_array())
    {
        return std::nullopt;
    }

    const LlsdValue* water_track =
        tracks->array_value.size() > 0U
            ? &tracks->array_value[0U]
            : nullptr;
    const LlsdValue* sky_track =
        tracks->array_value.size() > 1U
            ? &tracks->array_value[1U]
            : nullptr;

    const LlsdValue* water =
        select_eep_frame(*frames, water_track);
    const LlsdValue* sky =
        select_eep_frame(*frames, sky_track);

    if (water == nullptr ||
        sky == nullptr ||
        !water->is_map() ||
        !sky->is_map())
    {
        return std::nullopt;
    }

    WindLightEnvironment result;
    result.sky_name = string_like(sky->find("name"));

    if (const LlsdValue* haze = sky->find("legacy_haze");
        haze != nullptr && haze->is_map())
    {
        read_vec3_flexible(
            haze->find("ambient"),
            result.ambient);
        read_vec3_flexible(
            haze->find("blue_density"),
            result.blue_density);
        read_vec3_flexible(
            haze->find("blue_horizon"),
            result.blue_horizon);
        read_number(
            haze->find("density_multiplier"),
            result.density_multiplier);
        read_number(
            haze->find("distance_multiplier"),
            result.distance_multiplier);
        read_number(
            haze->find("haze_density"),
            result.haze_density);
        read_number(
            haze->find("haze_horizon"),
            result.haze_horizon);
    }

    read_vec3_flexible(
        sky->find("sunlight_color"),
        result.sunlight_color);
    read_vec3_flexible(
        sky->find("cloud_color"),
        result.cloud_color);
    read_number(
        sky->find("gamma"),
        result.gamma);
    read_number(
        sky->find("star_brightness"),
        result.star_brightness);
    read_number(
        sky->find("cloud_shadow"),
        result.cloud_shadow);

    quaternion_to_angles(
        sky->find("sun_rotation"),
        result.sun_angle,
        result.east_angle);

    read_vec3_flexible(
        water->find("water_fog_color"),
        result.water_fog_color);
    read_number(
        water->find("water_fog_density"),
        result.water_fog_density);
    result.water_normal_map =
        string_like(water->find("normal_map"));

    if (!validate_environment(result))
    {
        return std::nullopt;
    }

    return result;
}
}
