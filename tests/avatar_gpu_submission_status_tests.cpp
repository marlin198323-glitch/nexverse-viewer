/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/render/avatar_gpu_submission_status.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
using nexverse::render::describe_avatar_gpu_submission;
int main()
{
    if(describe_avatar_gpu_submission(0U,0U,{})!="Avatar GL: kein Body-Draw")
        return 1;
    if(describe_avatar_gpu_submission(0U,7U,{})!="Avatar GL: Ersatz-Mesh (kein LLM)")
        return 2;
    if(describe_avatar_gpu_submission(3U,0U,{1U,1U,1U})!=
       "LLM GL 3/3 | Bake H:G U:G L:G")return 3;
    if(describe_avatar_gpu_submission(1U,7U,{0U,5U,0U})!=
       "LLM GL 1/3 | Bake H:- U:G L:-")return 4;
    if(describe_avatar_gpu_submission(100U,0U,{})!=
       "LLM GL 3/3 | Bake H:- U:- L:-")return 5;
    std::cout<<"Avatar HUD GPU submissions, missing and proxy states PASS\n";
    return 0;
}
