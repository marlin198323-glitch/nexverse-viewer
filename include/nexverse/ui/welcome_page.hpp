/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::ui
{
inline constexpr std::string_view nexverse_welcome_url =
    "https://stadt-nexverse.de/service/welcome";

struct WelcomeFrame
{
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> rgba;
    bool ready = false;
    bool failed = false;
    // Increments even when the snapshot retains its previous dimensions.
    // The GL uploader must compare this, not only width/height.
    std::uint64_t generation = 0U;
    std::string status;
};

class WelcomePage
{
public:
    WelcomePage();
    ~WelcomePage();

    WelcomePage(const WelcomePage&) = delete;
    WelcomePage& operator=(const WelcomePage&) = delete;

    bool initialize(int width, int height);
    void resize(int width, int height);
    void pump();
    [[nodiscard]] const WelcomeFrame& frame() const noexcept;

private:
    struct Impl;
    Impl* impl_ = nullptr;
};
}
