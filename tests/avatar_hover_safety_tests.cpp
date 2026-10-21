/* MPL-2.0: Simulator Low158 invalid hover must not poison
 * original Firestorm body GL transforms. */
#include "nexverse/render/avatar_hover_safety.hpp"
#include <cmath>
#include <iostream>
#include <limits>
int main()
{
    using nexverse::render::safe_avatar_hover;
    using nexverse::render::avatar_hover_status;
    const auto nan=std::numeric_limits<float>::quiet_NaN();
    const auto pinf=std::numeric_limits<float>::infinity();
    const auto ninf=-pinf;
    for(const auto value:{nan,pinf,ninf})
    {
        const auto offset=safe_avatar_hover(value);
        if(!offset.invalid||offset.meters!=0.0f||
           !std::isfinite(offset.meters))return 1;
    }
    const auto too_high=safe_avatar_hover(999999.0f);
    const auto too_low=safe_avatar_hover(-999999.0f);
    if(too_high.invalid||too_high.meters!=2.0f||
       too_low.invalid||too_low.meters!=-2.0f)return 2;
    const auto normal=safe_avatar_hover(0.3f);
    if(normal.invalid||std::abs(normal.meters-0.3f)>0.00001f||
       !avatar_hover_status(false,0).empty())return 3;
    if(avatar_hover_status(true,0).find("Self 1")==std::string::npos||
       avatar_hover_status(false,2).find("Fremde 2")==std::string::npos||
       avatar_hover_status(true,2).find("Fremde 2")==std::string::npos)
        return 4;
    std::cout<<"OGL_AVATAR_LOW158_HOVER_NAN_INFINITE_GPU_GUARD_PASS\n";
}