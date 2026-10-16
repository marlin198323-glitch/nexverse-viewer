/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "nexverse/ui/brand_icon.hpp"

#include <png.h>
#include <array>
#include <system_error>

namespace nexverse::ui
{
BrandIcon load_brand_icon_png(const std::filesystem::path& filename)
{
    BrandIcon result;
    png_image image{};
    image.version=PNG_IMAGE_VERSION;
    const auto native=filename.string();
    if(!png_image_begin_read_from_file(&image,native.c_str()))
        return result;
    if(image.width==0U||image.height==0U||
       image.width>1024U||image.height>1024U)
    {
        png_image_free(&image);
        return result;
    }
    image.format=PNG_FORMAT_RGBA;
    result.rgba.resize(PNG_IMAGE_SIZE(image));
    if(!png_image_finish_read(&image,nullptr,result.rgba.data(),0,nullptr))
    {
        png_image_free(&image);
        return {};
    }
    result.width=static_cast<int>(image.width);
    result.height=static_cast<int>(image.height);
    png_image_free(&image);
    return result;
}

std::filesystem::path find_brand_icon(
    const std::filesystem::path& executable_path)
{
    const auto dir=executable_path.parent_path();
    constexpr auto file="opengenesislink-viewer.png";
    const std::array paths={
        dir/"assets"/"branding"/file,
        dir.parent_path()/"assets"/"branding"/file,
        dir.parent_path().parent_path()/"assets"/"branding"/file,
        dir.parent_path()/"share"/"opengenesislink-viewer"/file,
        dir.parent_path()/"share"/"icons"/"hicolor"/"256x256"/"apps"/file,
        dir.parent_path()/"Resources"/file,
        std::filesystem::current_path()/"assets"/"branding"/file,
#ifdef NEXVERSE_BRAND_SOURCE_ICON_PATH
        std::filesystem::path(NEXVERSE_BRAND_SOURCE_ICON_PATH)
#else
        std::filesystem::path()
#endif
    };
    for(const auto& candidate:paths)
    {
        std::error_code ec;
        if(!candidate.empty() && std::filesystem::is_regular_file(candidate,ec))
            return candidate;
    }
    return {};
}
}
