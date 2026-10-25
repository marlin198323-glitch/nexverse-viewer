/* MPL-2.0: bounded JPEG2000 metadata diagnosis without user asset IDs. */
#include "nexverse/assets/jpeg2000_metadata_limits.hpp"
#include <iostream>
using namespace nexverse::assets;
int main()
{
    if(!jpeg2000_image_limits_reason(512,512,3).empty()||
       !jpeg2000_image_limits_reason(4096,4096,4).empty()||
       jpeg2000_image_limits_reason(4097,1,3)!="image_dimensions"||
       jpeg2000_image_limits_reason(1,4097,3)!="image_dimensions"||
       jpeg2000_image_limits_reason(0,1,3)!="image_extent"||
       jpeg2000_image_limits_reason(1,1,5)!="channel_count"||
       !jpeg2000_component_limits_reason(128,128,2,2,8).empty()||
       jpeg2000_component_limits_reason(0,1,1,1,8)!="component_extent"||
       jpeg2000_component_limits_reason(4097,1,1,1,8)!="component_dimensions"||
       jpeg2000_component_limits_reason(128,128,0,1,8)!="component_subsampling"||
       jpeg2000_component_limits_reason(128,128,1,1,17)!="component_precision")
       return 1;
    std::cout<<"OGL_J2C_METADATA_LIMIT_REASON_PASS\n";
}
