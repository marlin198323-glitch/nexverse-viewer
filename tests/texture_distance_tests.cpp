#include "nexverse/assets/texture_distance.hpp"
#include <iostream>
#include <limits>
int main()
{
    using nexverse::assets::TextureDistanceSample;
    using nexverse::assets::rank_texture_distances;
    const std::vector<TextureDistanceSample> samples{
        {"left",0,0,0},{"right",150,0,0},
        {"shared",5,0,0},{"shared",149,0,0},
        {"invalid",std::numeric_limits<float>::quiet_NaN(),0,0}
    };
    const auto left=rank_texture_distances(samples,0,0,0,{});
    const auto right=rank_texture_distances(samples,150,0,0,{});
    if(left.size()!=3U || left[0]!="left" ||
       right.size()!=3U || right[0]!="right" ||
       right[1]!="shared")
    {
        std::cerr<<"Moving avatar failed to reprioritize textures\n";
        return 1;
    }
    const auto stable=rank_texture_distances(
        {{"resident",10,0,0},{"new",8,0,0}},0,0,0,
        {"resident"},6.0f);
    if(stable[0]!="resident")
    {
        std::cerr<<"Texture cache hysteresis failed\n";return 2;
    }
    std::cout<<"Texture location priorities and hysteresis passed\n";
    return 0;
}
