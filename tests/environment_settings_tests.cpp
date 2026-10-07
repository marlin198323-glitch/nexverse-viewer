#include "nexverse/protocol/environment_settings.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace
{
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
        std::cerr << "Invalid environment unexpectedly accepted\n";
        return 3;
    }

    std::cout << "Environment settings tests passed\n";
    return 0;
}
