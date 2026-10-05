/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/outbound_queue.hpp"

#include <algorithm>
#include <utility>

namespace nexverse::protocol
{
OutboundCommandQueue::OutboundCommandQueue(std::size_t max_pending)
    : max_pending_{std::max<std::size_t>(max_pending, 1U)}
{
}

bool OutboundCommandQueue::push(OutboundCommand command)
{
    std::scoped_lock lock{mutex_};
    if (queue_.size() >= max_pending_)
    {
        return false;
    }

    queue_.push_back(std::move(command));
    return true;
}

std::optional<OutboundCommand> OutboundCommandQueue::try_pop()
{
    std::scoped_lock lock{mutex_};
    if (queue_.empty())
    {
        return std::nullopt;
    }

    OutboundCommand command = std::move(queue_.front());
    queue_.pop_front();
    return command;
}

std::size_t OutboundCommandQueue::size() const
{
    std::scoped_lock lock{mutex_};
    return queue_.size();
}

void OutboundCommandQueue::clear()
{
    std::scoped_lock lock{mutex_};
    queue_.clear();
}
}
