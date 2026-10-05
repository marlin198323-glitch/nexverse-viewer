/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/caps.hpp"

#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
class CapsEventDispatcher
{
public:
    using Handler = std::function<void(const EventQueueEvent&)>;

    void register_handler(std::string message, Handler handler);
    void clear_handler(std::string_view message);
    void set_fallback_handler(Handler handler);

    [[nodiscard]] bool dispatch(const EventQueueEvent& event) const;
    [[nodiscard]] bool has_handler(std::string_view message) const;
    [[nodiscard]] std::size_t handler_count() const noexcept;

private:
    std::map<std::string, Handler, std::less<>> handlers_;
    Handler fallback_handler_;
};
}
