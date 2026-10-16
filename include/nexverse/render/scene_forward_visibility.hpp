/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cmath>
namespace nexverse::render
{
// Conservative object visibility: never count scene draws wholly behind
// the authenticated camera or beyond the far clip. The object bounding
// sphere preserves near/large objects straddling the view plane.
// This is NOT full six-plane frustum culling.
[[nodiscard]] inline bool scene_forward_visible(
    float dx,float dy,float dz,float scale_x,float scale_y,float scale_z,
    float forward_x,float forward_y,float forward_z,float far_clip) noexcept
{
    if(!std::isfinite(dx)||!std::isfinite(dy)||!std::isfinite(dz)||
       !std::isfinite(scale_x)||!std::isfinite(scale_y)||
       !std::isfinite(scale_z)||!std::isfinite(far_clip))
        return false;
    const double radius=0.5*std::hypot(
        std::hypot(double(scale_x),double(scale_y)),double(scale_z));
    const double far=std::max(1.0,double(far_clip));
    const double limit=far+std::max(radius,0.5);
    if(double(dx)*dx+double(dy)*dy+double(dz)*dz>limit*limit)
        return false;
    const double f2=double(forward_x)*forward_x+
        double(forward_y)*forward_y+double(forward_z)*forward_z;
    if(!std::isfinite(f2)||f2<1e-12)
        return true; // do not reject when camera heading is unavailable
    const double depth=(double(dx)*forward_x+
        double(dy)*forward_y+double(dz)*forward_z)/std::sqrt(f2);
    return depth>=-radius;
}
}
