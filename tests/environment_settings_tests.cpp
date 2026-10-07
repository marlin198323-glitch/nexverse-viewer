#include "nexverse/protocol/environment_settings.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace
{
nexverse::protocol::LlsdValue vec3(
    double x,
    double y,
    double z)
{
    using nexverse::protocol::LlsdValue;
    return LlsdValue::array({
        LlsdValue::real(x),
        LlsdValue::real(y),
        LlsdValue::real(z)});
}

nexverse::protocol::LlsdValue vec4(
    double x,
    double y,
    double z,
    double w)
{
    using nexverse::protocol::LlsdValue;
    return LlsdValue::array({
        LlsdValue::real(x),
        LlsdValue::real(y),
        LlsdValue::real(z),
        LlsdValue::real(w)});
}

nexverse::protocol::LlsdValue scalar4(double x)
{
    return vec4(x, 0.0, 0.0, 1.0);
}
}

int main()
{
    using namespace nexverse::protocol;

    std::map<std::string, LlsdValue, std::less<>> sky;
    sky["name"] = LlsdValue::string("NexVerse Day");
    sky["ambient"] = vec4(0.31, 0.32, 0.33, 1.0);
    sky["blue_density"] = vec4(0.12, 0.22, 0.38, 1.0);
    sky["blue_horizon"] = vec4(0.25, 0.28, 0.34, 1.0);
    sky["sunlight_color"] = vec4(0.90, 0.82, 0.70, 0.3);
    sky["cloud_color"] = vec4(0.50, 0.51, 0.52, 1.0);
    sky["haze_density"] = scalar4(0.61);
    sky["haze_horizon"] = scalar4(0.17);
    sky["density_multiplier"] = scalar4(0.00021);
    sky["distance_multiplier"] = scalar4(0.95);
    sky["gamma"] = scalar4(1.1);
    sky["star_brightness"] = LlsdValue::real(0.12);
    sky["sun_angle"] = LlsdValue::real(1.1);
    sky["east_angle"] = LlsdValue::real(0.4);
    sky["cloud_shadow"] = scalar4(0.35);

    std::map<std::string, LlsdValue, std::less<>> frames;
    frames["NexVerse Day"] = LlsdValue::map(std::move(sky));

    std::map<std::string, LlsdValue, std::less<>> water;
    water["waterFogColor"] = vec4(0.02, 0.15, 0.25, 1.0);
    water["waterFogDensity"] = LlsdValue::real(12.0);
    water["normalMap"] =
        LlsdValue::uuid("822ded49-9a6c-f61c-cb89-6df54f42cdf4");

    const LlsdValue response = LlsdValue::array({
        LlsdValue::map({}),
        LlsdValue::array({
            LlsdValue::array({
                LlsdValue::real(-1.0),
                LlsdValue::string("NexVerse Day")})
        }),
        LlsdValue::map(std::move(frames)),
        LlsdValue::map(std::move(water))
    });

    const auto parsed = parse_environment_settings(response);
    if (!parsed.has_value())
    {
        std::cerr << "EnvironmentSettings parse failed\n";
        return 1;
    }

    if (parsed->sky_name != "NexVerse Day" ||
        std::abs(parsed->ambient.x - 0.31f) > 0.0001f ||
        std::abs(parsed->sunlight_color.z - 0.70f) > 0.0001f ||
        std::abs(parsed->sun_angle - 1.1f) > 0.0001f ||
        std::abs(parsed->east_angle - 0.4f) > 0.0001f ||
        std::abs(parsed->water_fog_color.y - 0.15f) > 0.0001f ||
        std::abs(parsed->water_fog_density - 12.0f) > 0.0001f ||
        parsed->water_normal_map !=
            "822ded49-9a6c-f61c-cb89-6df54f42cdf4")
    {
        std::cerr << "EnvironmentSettings values mismatch\n";
        return 2;
    }

    if (parse_environment_settings(LlsdValue::map({})).has_value())
    {
        std::cerr << "Invalid legacy environment unexpectedly accepted\n";
        return 3;
    }

    std::map<std::string, LlsdValue, std::less<>> haze;
    haze["ambient"] = vec3(0.41, 0.42, 0.43);
    haze["blue_density"] = vec3(0.18, 0.28, 0.48);
    haze["blue_horizon"] = vec3(0.31, 0.36, 0.52);
    haze["density_multiplier"] = LlsdValue::real(0.00024);
    haze["distance_multiplier"] = LlsdValue::real(1.05);
    haze["haze_density"] = LlsdValue::real(0.52);
    haze["haze_horizon"] = LlsdValue::real(0.14);

    std::map<std::string, LlsdValue, std::less<>> eep_sky;
    eep_sky["name"] = LlsdValue::string("NexVerse EEP Sky");
    eep_sky["type"] = LlsdValue::string("sky");
    eep_sky["legacy_haze"] = LlsdValue::map(std::move(haze));
    eep_sky["sunlight_color"] = vec3(0.95, 0.80, 0.62);
    eep_sky["cloud_color"] = vec3(0.58, 0.59, 0.61);
    eep_sky["gamma"] = LlsdValue::real(1.2);
    eep_sky["star_brightness"] = LlsdValue::real(0.2);
    eep_sky["cloud_shadow"] = LlsdValue::real(0.4);
    eep_sky["sun_rotation"] = vec4(0.0, -0.3824995, 0.0, 0.9239557);

    std::map<std::string, LlsdValue, std::less<>> eep_water;
    eep_water["name"] = LlsdValue::string("NexVerse EEP Water");
    eep_water["type"] = LlsdValue::string("water");
    eep_water["water_fog_color"] = vec3(0.03, 0.17, 0.28);
    eep_water["water_fog_density"] = LlsdValue::real(9.5);
    eep_water["normal_map"] =
        LlsdValue::uuid("822ded49-9a6c-f61c-cb89-6df54f42cdf4");

    std::map<std::string, LlsdValue, std::less<>> eep_frames;
    eep_frames["SkyFrame"] = LlsdValue::map(std::move(eep_sky));
    eep_frames["WaterFrame"] = LlsdValue::map(std::move(eep_water));

    const LlsdValue water_track = LlsdValue::array({
        LlsdValue::map({
            {"key_keyframe", LlsdValue::real(0.0)},
            {"key_name", LlsdValue::string("WaterFrame")}})
    });
    const LlsdValue sky_track = LlsdValue::array({
        LlsdValue::map({
            {"key_keyframe", LlsdValue::real(0.0)},
            {"key_name", LlsdValue::string("SkyFrame")}})
    });

    const LlsdValue ext_response = LlsdValue::map({
        {"success", LlsdValue::boolean(true)},
        {"environment", LlsdValue::map({
            {"day_cycle", LlsdValue::map({
                {"frames", LlsdValue::map(std::move(eep_frames))},
                {"tracks", LlsdValue::array({
                    water_track,
                    sky_track})},
                {"type", LlsdValue::string("daycycle")}})
            }
        })}
    });

    const auto ext = parse_ext_environment_settings(ext_response);
    if (!ext.has_value())
    {
        std::cerr << "ExtEnvironment parse failed\n";
        return 4;
    }

    if (ext->sky_name != "NexVerse EEP Sky" ||
        std::abs(ext->ambient.x - 0.41f) > 0.0001f ||
        std::abs(ext->blue_horizon.z - 0.52f) > 0.0001f ||
        std::abs(ext->sunlight_color.x - 0.95f) > 0.0001f ||
        std::abs(ext->gamma - 1.2f) > 0.0001f ||
        std::abs(ext->water_fog_color.z - 0.28f) > 0.0001f ||
        std::abs(ext->water_fog_density - 9.5f) > 0.0001f ||
        ext->water_normal_map !=
            "822ded49-9a6c-f61c-cb89-6df54f42cdf4")
    {
        std::cerr << "ExtEnvironment values mismatch\n";
        return 5;
    }

    if (parse_ext_environment_settings(LlsdValue::array({})).has_value())
    {
        std::cerr << "Invalid EEP environment unexpectedly accepted\n";
        return 6;
    }

    std::cout << "Environment settings tests passed\n";
    return 0;
}
