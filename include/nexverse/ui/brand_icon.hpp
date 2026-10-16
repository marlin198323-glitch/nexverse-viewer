/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <filesystem>
#include <vector>

namespace nexverse::ui
{
struct BrandIcon
{
    int width=0;
    int height=0;
    std::vector<unsigned char> rgba;
    [[nodiscard]] bool valid() const noexcept
    {
        return width>0 && height>0 &&
            rgba.size()==static_cast<std::size_t>(width)*
                static_cast<std::size_t>(height)*4U;
    }
};
[[nodiscard]] BrandIcon load_brand_icon_png(const std::filesystem::path& filename);
[[nodiscard]] std::filesystem::path find_brand_icon(
    const std::filesystem::path& executable_path);
}
