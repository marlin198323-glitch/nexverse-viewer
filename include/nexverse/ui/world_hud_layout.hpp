/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once

namespace nexverse::ui
{
struct WorldHudPanelVisibility
{
    bool connection=false;
    bool system=false;
    bool avatar=false;
};
// Cards use fixed scaled y offsets within the right rail. Leave 12 scaled
// points between the last visible card and the anchored 58pt FPS footer.
// Never draw a card over the footer on compact/widescreen window heights.
[[nodiscard]] constexpr WorldHudPanelVisibility world_hud_panel_visibility(
    float rail_height,float scale) noexcept
{
    if(scale<=0.0f||rail_height<=0.0f)return {};
    return {
        rail_height>=(215.0f+12.0f+58.0f)*scale,
        rail_height>=(359.0f+12.0f+58.0f)*scale,
        rail_height>=(471.0f+12.0f+58.0f)*scale
    };
}
}
