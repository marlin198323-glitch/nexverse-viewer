#include "nexverse/assets/mesh_lod_policy.hpp"
#include <limits>
#include <iostream>

int main()
{
    using namespace nexverse::assets;
    const std::vector<MeshDistanceSample> s{
        {"a", 10,0,0,1}, {"a",150,0,0,1},
        {"b",35,0,0,1}, {"c",85,0,0,1}, {"d",155,0,0,1},
        {"bad",std::numeric_limits<float>::quiet_NaN(),0,0,1}};
    auto levels=plan_mesh_lods(s,0,0,0);
    if(levels.size()!=4U || levels.at("a")!=MeshLod::high ||
       levels.at("b")!=MeshLod::medium ||
       levels.at("c")!=MeshLod::low ||
       levels.at("d")!=MeshLod::lowest) return 1;
    auto hysteresis=plan_mesh_lods({{"a",22,0,0,1}},0,0,0,levels);
    if(hysteresis.at("a")!=MeshLod::high) return 2;
    hysteresis=plan_mesh_lods({{"a",30,0,0,1}},0,0,0,levels);
    if(hysteresis.at("a")!=MeshLod::medium) return 3;
    hysteresis=plan_mesh_lods({{"b",60,0,0,1}},0,0,0,levels);
    if(hysteresis.at("b")!=MeshLod::medium) return 4;
    hysteresis=plan_mesh_lods({{"c",130,0,0,1}},0,0,0,levels);
    if(hysteresis.at("c")!=MeshLod::low) return 5;
    hysteresis=plan_mesh_lods({{"d",100,0,0,1}},0,0,0,levels);
    if(hysteresis.at("d")!=MeshLod::lowest) return 6;
    auto size=plan_mesh_lods({{"x",80,0,0,16}},0,0,0);
    if(size.at("x")!=MeshLod::medium) return 7; // 80/sqrt(16) = 20
    if(!plan_mesh_lods({},0,0,0,levels).empty())return 8;
    std::cout<<"Mesh LOD selection, reuse, hysteresis and limits OK\n";
    return 0;
}
