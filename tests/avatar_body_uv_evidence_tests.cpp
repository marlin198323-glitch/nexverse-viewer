/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/render/avatar_body_uv_evidence.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace nexverse::render;

int main()
{
    // Exact eight-float original Firestorm vertex interleave: xyz, nxyz, uv.
    std::vector<float> triangle{
        0,0,0, 0,0,1, 0.0f,0.0f,
        1,0,0, 0,0,1, 1.0f,0.0f,
        0,1,0, 0,0,1, 0.0f,1.0f};
    const std::vector<std::uint32_t> faces{0,1,2};
    const auto good=audit_avatar_body_uv(triangle,faces);
    if(!good.safe_for_gpu||good.vertex_count!=3U||
       good.triangle_count!=1U||
       good.min_uv[0]!=0.0f||good.max_uv[1]!=1.0f)return 1;

    // Preserve genuine UVs outside [0,1] (Firestorm repeat semantics).
    triangle[6]=-0.5f; triangle[15]=1.5f;
    if(!audit_avatar_body_uv(triangle,faces).safe_for_gpu)return 2;
    triangle[6]=std::numeric_limits<float>::infinity();
    if(audit_avatar_body_uv(triangle,faces).safe_for_gpu)return 3;
    triangle[6]=0.0f;
    triangle[3]=std::numeric_limits<float>::quiet_NaN();
    if(audit_avatar_body_uv(triangle,faces).safe_for_gpu)return 4;
    triangle[3]=0.0f;
    triangle[6]=150.0f;
    if(audit_avatar_body_uv(triangle,faces).safe_for_gpu)return 5;
    triangle[6]=0.0f;triangle[15]=1.5f;
    const std::vector<std::uint32_t> oob_faces{0U,1U,3U};
    const std::vector<std::uint32_t> short_faces{0U,1U};
    if(audit_avatar_body_uv(triangle,oob_faces).safe_for_gpu)return 6;
    if(audit_avatar_body_uv(triangle,short_faces).safe_for_gpu)return 7;
    if(audit_avatar_body_uv({},faces).safe_for_gpu)return 8;
    triangle.resize(23U);
    if(audit_avatar_body_uv(triangle,faces).safe_for_gpu)return 9;

    // F9 display can only say "Draw" for original-mesh textured GPU
    // submissions, never Firestorm bake pixel parity / asset-upload.
    std::array<std::uint64_t,3U> draws{2U,0U,1U};
    if(avatar_gl_body_symbols(draws)!=std::array<char,3U>{'G','-','G'})
        return 10;
    if(describe_avatar_gl_body_draws(draws,false).find("Appearance pruefen")
       ==std::string::npos)return 11;
    const auto active=describe_avatar_gl_body_draws(draws,true);
    if(active.find("H:G U:- L:G")==std::string::npos||
       active.find("keine Pixelparitaet")==std::string::npos)
        return 12;
    std::cout<<"Firestorm body UV audit and actual 3-slot GL proof PASS\n";
    return 0;
}
