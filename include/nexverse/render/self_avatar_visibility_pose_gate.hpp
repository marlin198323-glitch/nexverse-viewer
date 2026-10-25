/* This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0.
 */
#pragma once
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>

namespace nexverse::render
{
// An asynchronous GL visibility result refers to geometry at a specific
// position and orientation. Server movement without camera movement
// must retire it too. Inputs are audited and quaternion-normalized
// by sanitize_avatar_proxy_pose before observe() is called.
class SelfAvatarVisibilityPoseGate
{
public:
    // x,y,z,qx,qy,qz,qw. No avatar IDs or network information.
    using Pose=std::array<float,7U>;
    [[nodiscard]] bool observe(const std::optional<Pose>& pose)
    {
        if(!initialized_)
        {
            initialized_=true;
            previous_=pose;
            return true;
        }
        if(!previous_ || !pose)
        {
            if(previous_==pose)return false;
            previous_=pose;
            return true;
        }
        // Anchor to the last invalidation, not the previous incoming
        // frame: small frame-to-frame movement must accumulate.
        double position_delta_squared=0.0;
        for(std::size_t i=0U;i<3U;++i)
        {
            const double delta=double((*pose)[i])-(*previous_)[i];
            position_delta_squared+=delta*delta;
        }
        // q and -q are identical rotations. Dot is well-defined for
        // finite, normalized quaternions accepted by the renderer.
        double q_dot=0.0;
        for(std::size_t i=3U;i<7U;++i)
            q_dot+=double((*pose)[i])*(*previous_)[i];
        constexpr double minimum_displacement_squared=0.05*0.05;
        constexpr double minimum_rotation_dot=0.99985; // ~2 degrees
        if(position_delta_squared<=minimum_displacement_squared &&
           std::abs(q_dot)>=minimum_rotation_dot)
            return false;
        previous_=pose;
        return true;
    }
    void reset() noexcept
    {
        initialized_=false;
        previous_.reset();
    }
private:
    bool initialized_=false;
    std::optional<Pose> previous_;
};
}
