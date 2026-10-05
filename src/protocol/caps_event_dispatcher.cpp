/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/caps_event_dispatcher.hpp"

#include <utility>

namespace nexverse::protocol
{
void CapsEventDispatcher::register_handler(
    std::string message,
    Handler handler)
{
    if (message.empty())
    {
        return;
    }

    if (!handler)
    {
        handlers_.erase(message);
        return;
    }

    handlers_[std::move(message)] = std::move(handler);
}

void CapsEventDispatcher::clear_handler(std::string_view message)
{
    handlers_.erase(std::string{message});
}

void CapsEventDispatcher::set_fallback_handler(Handler handler)
{
    fallback_handler_ = std::move(handler);
}

bool CapsEventDispatcher::dispatch(const EventQueueEvent& event) const
{
    const auto found = handlers_.find(event.message);
    if (found != handlers_.end())
    {
        found->second(event);
        return true;
    }

    if (fallback_handler_)
    {
        fallback_handler_(event);
        return true;
    }

    return false;
}

bool CapsEventDispatcher::has_handler(std::string_view message) const
{
    return handlers_.contains(message);
}

std::size_t CapsEventDispatcher::handler_count() const noexcept
{
    return handlers_.size();
}
}
