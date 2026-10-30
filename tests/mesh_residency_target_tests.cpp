/* MPL-2.0 */
#include "nexverse/assets/mesh_residency_target.hpp"
#include <iostream>
using nexverse::assets::mesh_resident_count_target;
int main()
{
    static_assert(nexverse::assets::max_mesh_resident_assets==384U);
    static_assert(nexverse::assets::economy_mesh_resident_assets==256U);
    static_assert(mesh_resident_count_target(0U,true)==256U);
    static_assert(mesh_resident_count_target(200U,true)==256U);
    static_assert(mesh_resident_count_target(292U,true)==292U);
    static_assert(mesh_resident_count_target(300U,true)==300U);
    static_assert(mesh_resident_count_target(600U,true)==384U);
    static_assert(mesh_resident_count_target(600U,false)==256U);
    if(mesh_resident_count_target(292U,true)!=292U) return 1;
    std::cout<<"OGL_ADAPTIVE_MESH_CAP_PASS\n";
}
