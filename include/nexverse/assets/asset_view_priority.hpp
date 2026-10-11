/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cmath>

namespace nexverse::assets
{
struct AssetCameraHint
{
    float x=0.0f,y=0.0f,z=0.0f;
    float forward_x=1.0f,forward_y=0.0f,forward_z=0.0f;
};
// Soft ranking only: no object is excluded by this view-direction hint.
// Nearby objects never suffer a large off-axis penalty.
[[nodiscard]] inline double view_priority_multiplier(
    const AssetCameraHint& camera, float x, float y, float z) noexcept
{
    if (!std::isfinite(camera.x)||!std::isfinite(camera.y)||
        !std::isfinite(camera.z)||!std::isfinite(x)||
        !std::isfinite(y)||!std::isfinite(z)||
        !std::isfinite(camera.forward_x)||!std::isfinite(camera.forward_y)||
        !std::isfinite(camera.forward_z)) return 1.0;
    const double dx=static_cast<double>(x)-camera.x;
    const double dy=static_cast<double>(y)-camera.y;
    const double dz=static_cast<double>(z)-camera.z;
    const double distance=std::hypot(dx,dy,dz);
    const double forward=std::hypot(
        static_cast<double>(camera.forward_x),
        static_cast<double>(camera.forward_y),
        static_cast<double>(camera.forward_z));
    if (distance<=12.0 || forward<=0.000001 ||
        !std::isfinite(distance) || !std::isfinite(forward)) return 1.0;
    const double dot=(dx*camera.forward_x+dy*camera.forward_y+
                      dz*camera.forward_z)/(distance*forward);
    const double off_axis=std::clamp((0.3-dot)/1.3,0.0,1.0);
    return 1.0+1.75*off_axis*std::clamp((distance-12.0)/40.0,0.0,1.0);
}
}
