/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/protocol/environment_settings.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <vector>

namespace
{
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;
using nexverse::protocol::Vec3;
using nexverse::protocol::WindLightEnvironment;

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

bool read_i64(const LlsdValue* value, std::int64_t& out)
{
    if (value == nullptr)
    {
        return false;
    }

    if (value->type == LlsdType::integer)
    {
        out = value->integer_value;
        return true;
    }

    if (value->type == LlsdType::real &&
        std::isfinite(value->real_value))
    {
        out = static_cast<std::int64_t>(value->real_value);
        return true;
    }

    return false;
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

struct TrackEntry
{
    float time = 0.0f;
    const LlsdValue* frame = nullptr;
};

struct TrackSample
{
    const LlsdValue* first = nullptr;
    const LlsdValue* second = nullptr;
    float mix = 0.0f;
};

std::vector<TrackEntry> collect_track(
    const LlsdValue& frames,
    const LlsdValue* track)
{
    std::vector<TrackEntry> entries;

    if (!frames.is_map() ||
        track == nullptr ||
        !track->is_array())
    {
        return entries;
    }

    for (const auto& entry : track->array_value)
    {
        if (!entry.is_map())
        {
            continue;
        }

        float keyframe = 0.0f;
        if (!read_number(entry.find("key_keyframe"), keyframe))
        {
            continue;
        }

        const std::string frame_name =
            string_like(entry.find("key_name"));
        if (frame_name.empty())
        {
            continue;
        }

        const auto found = frames.map_value.find(frame_name);
        if (found == frames.map_value.end() ||
            !found->second.is_map())
        {
            continue;
        }

        entries.push_back({
            std::clamp(keyframe, 0.0f, 1.0f),
            &found->second});
    }

    std::sort(
        entries.begin(),
        entries.end(),
        [](const TrackEntry& a, const TrackEntry& b)
        {
            return a.time < b.time;
        });

    return entries;
}

TrackSample sample_track(
    const std::vector<TrackEntry>& track,
    float day_fraction)
{
    TrackSample sample;

    if (track.empty())
    {
        return sample;
    }

    if (track.size() == 1U)
    {
        sample.first = track.front().frame;
        return sample;
    }

    const float day = std::clamp(day_fraction, 0.0f, 1.0f);

    std::size_t next_index = 0U;
    while (next_index < track.size() &&
           track[next_index].time <= day)
    {
        ++next_index;
    }

    float first_time = 0.0f;
    float second_time = 0.0f;
    float sample_time = day;

    if (next_index == 0U || next_index == track.size())
    {
        sample.first = track.back().frame;
        sample.second = track.front().frame;
        first_time = track.back().time;
        second_time = track.front().time + 1.0f;

        if (sample_time < first_time)
        {
            sample_time += 1.0f;
        }
    }
    else
    {
        sample.first = track[next_index - 1U].frame;
        sample.second = track[next_index].frame;
        first_time = track[next_index - 1U].time;
        second_time = track[next_index].time;
    }

    const float span = second_time - first_time;
    if (span <= 0.000001f)
    {
        sample.second = nullptr;
        sample.mix = 0.0f;
        return sample;
    }

    sample.mix = std::clamp(
        (sample_time - first_time) / span,
        0.0f,
        1.0f);
    return sample;
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

void parse_eep_sky(
    const LlsdValue& sky,
    WindLightEnvironment& result)
{
    if (const LlsdValue* haze = sky.find("legacy_haze");
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
        sky.find("sunlight_color"),
        result.sunlight_color);
    read_vec3_flexible(
        sky.find("cloud_color"),
        result.cloud_color);
    read_number(
        sky.find("gamma"),
        result.gamma);
    read_number(
        sky.find("star_brightness"),
        result.star_brightness);
    read_number(
        sky.find("cloud_shadow"),
        result.cloud_shadow);

    quaternion_to_angles(
        sky.find("sun_rotation"),
        result.sun_angle,
        result.east_angle);

    result.sky_name =
        string_like(sky.find("name"));
}

void parse_eep_water(
    const LlsdValue& water,
    WindLightEnvironment& result)
{
    read_vec3_flexible(
        water.find("water_fog_color"),
        result.water_fog_color);
    read_number(
        water.find("water_fog_density"),
        result.water_fog_density);
    result.water_normal_map =
        string_like(water.find("normal_map"));
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

Vec3 lerp(Vec3 a, Vec3 b, float t)
{
    return {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t)};
}

float lerp_angle(float a, float b, float t)
{
    float delta = std::fmod(b - a, two_pi);
    if (delta > 3.14159265358979323846f)
    {
        delta -= two_pi;
    }
    else if (delta < -3.14159265358979323846f)
    {
        delta += two_pi;
    }

    float result = a + delta * t;
    result = std::fmod(result, two_pi);
    if (result < 0.0f)
    {
        result += two_pi;
    }
    return result;
}

WindLightEnvironment blend_environment(
    const WindLightEnvironment& a,
    const WindLightEnvironment& b,
    float t)
{
    WindLightEnvironment result;
    result.ambient = lerp(a.ambient, b.ambient, t);
    result.blue_density = lerp(a.blue_density, b.blue_density, t);
    result.blue_horizon = lerp(a.blue_horizon, b.blue_horizon, t);
    result.sunlight_color = lerp(a.sunlight_color, b.sunlight_color, t);
    result.cloud_color = lerp(a.cloud_color, b.cloud_color, t);
    result.water_fog_color =
        lerp(a.water_fog_color, b.water_fog_color, t);

    result.haze_density = lerp(a.haze_density, b.haze_density, t);
    result.haze_horizon = lerp(a.haze_horizon, b.haze_horizon, t);
    result.density_multiplier =
        lerp(a.density_multiplier, b.density_multiplier, t);
    result.distance_multiplier =
        lerp(a.distance_multiplier, b.distance_multiplier, t);
    result.gamma = lerp(a.gamma, b.gamma, t);
    result.star_brightness =
        lerp(a.star_brightness, b.star_brightness, t);
    result.sun_angle =
        lerp_angle(a.sun_angle, b.sun_angle, t);
    result.east_angle =
        lerp_angle(a.east_angle, b.east_angle, t);
    result.cloud_shadow =
        lerp(a.cloud_shadow, b.cloud_shadow, t);
    result.water_fog_density =
        lerp(a.water_fog_density, b.water_fog_density, t);

    result.sky_name =
        t < 0.5f ? a.sky_name : b.sky_name;
    result.water_normal_map =
        t < 0.5f ? a.water_normal_map : b.water_normal_map;
    return result;
}

bool validate_environment(const WindLightEnvironment& result)
{
    return
        std::isfinite(result.sun_angle) &&
        std::isfinite(result.east_angle) &&
        std::isfinite(result.gamma) &&
        result.gamma > 0.0f;
}

float environment_day_fraction(
    const LlsdValue& environment,
    std::int64_t unix_seconds)
{
    std::int64_t day_length = 14400;
    std::int64_t day_offset = 57600;

    read_i64(environment.find("day_length"), day_length);
    read_i64(environment.find("day_offset"), day_offset);

    if (day_length <= 0)
    {
        day_length = 14400;
    }

    std::int64_t position =
        (unix_seconds + day_offset) % day_length;
    if (position < 0)
    {
        position += day_length;
    }

    return static_cast<float>(
        static_cast<double>(position) /
        static_cast<double>(day_length));
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
parse_ext_environment_settings_at(
    const LlsdValue& value,
    std::int64_t unix_seconds)
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

    const float day_fraction =
        environment_day_fraction(*environment, unix_seconds);

    const LlsdValue* water_track =
        tracks->array_value.size() > 0U
            ? &tracks->array_value[0U]
            : nullptr;
    const LlsdValue* sky_track =
        tracks->array_value.size() > 1U
            ? &tracks->array_value[1U]
            : nullptr;

    const auto water_entries =
        collect_track(*frames, water_track);
    const auto sky_entries =
        collect_track(*frames, sky_track);

    const TrackSample water_sample =
        sample_track(water_entries, day_fraction);
    const TrackSample sky_sample =
        sample_track(sky_entries, day_fraction);

    if (water_sample.first == nullptr ||
        sky_sample.first == nullptr)
    {
        return std::nullopt;
    }

    WindLightEnvironment first;
    parse_eep_sky(*sky_sample.first, first);
    parse_eep_water(*water_sample.first, first);

    WindLightEnvironment result = first;

    if (sky_sample.second != nullptr)
    {
        WindLightEnvironment second = first;
        parse_eep_sky(*sky_sample.second, second);
        result = blend_environment(
            result,
            second,
            sky_sample.mix);
    }

    if (water_sample.second != nullptr)
    {
        WindLightEnvironment second = result;
        parse_eep_water(*water_sample.second, second);

        result.water_fog_color = lerp(
            result.water_fog_color,
            second.water_fog_color,
            water_sample.mix);
        result.water_fog_density = lerp(
            result.water_fog_density,
            second.water_fog_density,
            water_sample.mix);
        result.water_normal_map =
            water_sample.mix < 0.5f
                ? result.water_normal_map
                : second.water_normal_map;
    }

    if (!validate_environment(result))
    {
        return std::nullopt;
    }

    return result;
}

std::optional<WindLightEnvironment>
parse_ext_environment_settings(const LlsdValue& value)
{
    const auto now =
        std::chrono::system_clock::now().time_since_epoch();
    const auto seconds =
        std::chrono::duration_cast<std::chrono::seconds>(now).count();

    return parse_ext_environment_settings_at(
        value,
        seconds);
}
}
