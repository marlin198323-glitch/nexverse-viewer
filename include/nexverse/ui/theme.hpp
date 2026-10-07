/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace nexverse::ui
{
struct Color
{
    float r;
    float g;
    float b;
    float a;
};

namespace theme
{
inline constexpr Color background{0.020f, 0.028f, 0.055f, 1.0f};
inline constexpr Color surface{0.040f, 0.060f, 0.100f, 0.94f};
inline constexpr Color surface_soft{0.060f, 0.085f, 0.135f, 0.82f};
inline constexpr Color surface_glass{0.055f, 0.075f, 0.120f, 0.72f};
inline constexpr Color border{0.120f, 0.255f, 0.360f, 0.65f};
inline constexpr Color cyan{0.090f, 0.820f, 0.970f, 1.0f};
inline constexpr Color blue{0.180f, 0.430f, 1.000f, 1.0f};
inline constexpr Color violet{0.610f, 0.260f, 0.970f, 1.0f};
inline constexpr Color magenta{0.920f, 0.220f, 0.800f, 1.0f};
inline constexpr Color text{0.920f, 0.960f, 1.000f, 1.0f};
inline constexpr Color text_muted{0.530f, 0.630f, 0.740f, 1.0f};
inline constexpr Color success{0.180f, 0.850f, 0.570f, 1.0f};
inline constexpr Color warning{1.000f, 0.660f, 0.180f, 1.0f};
}
}
