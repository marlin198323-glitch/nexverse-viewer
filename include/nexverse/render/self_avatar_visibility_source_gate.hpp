/* This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. */
#pragma once
#include <array>
#include <optional>
#include <string>

namespace nexverse::render
{
// Keep the source identity of the *actual* currently rendered self body
// independently from the previous-frame asynchronous OpenGL query result.
// No server identity, UUID or session token is ever logged/exported.
class SelfAvatarVisibilitySourceGate
{
public:
    using BakeIds=std::array<std::string,3U>;
    [[nodiscard]] bool observe(const std::optional<BakeIds>& source)
    {
        if(!initialized_)
        {
            initialized_=true;
            previous_=source;
            return true; // first source also invalidates old GPU queries
        }
        if(previous_==source)return false;
        previous_=source;
        return true;
    }
    void reset() noexcept
    {
        initialized_=false;
        previous_.reset();
    }
private:
    bool initialized_=false;
    std::optional<BakeIds> previous_;
};
}
