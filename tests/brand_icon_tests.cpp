/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "nexverse/ui/brand_icon.hpp"
#include <filesystem>
#include <iostream>
int main()
{
    namespace fs=std::filesystem;
    const auto binary=fs::current_path()/"build/debug/nexverse-viewer";
    const auto located=nexverse::ui::find_brand_icon(binary);
    if(located.empty() || !fs::exists(located))return 1;
    const auto logo=nexverse::ui::load_brand_icon_png(located);
    if(!logo.valid() || logo.width!=256 || logo.height!=256)return 2;
    if(logo.rgba[3]!=0U)return 3;
    const auto middle=(128U*256U+128U)*4U+3U;
    if(logo.rgba[middle]!=255U)return 4;
    if(nexverse::ui::load_brand_icon_png("missing-branding.png").valid())
        return 5;
    std::cout<<"Original branded PNG correctly decoded from project/build asset path PASS\n";
    return 0;
}
