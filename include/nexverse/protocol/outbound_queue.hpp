/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/viewer_messages.hpp"

#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <variant>

namespace nexverse::protocol
{
using OutboundCommand = std::variant<
    AgentUpdateData,
    LocalChatMessage,
    TeleportLocation>;

class OutboundCommandQueue
{
public:
    explicit OutboundCommandQueue(std::size_t max_pending = 256U);

    [[nodiscard]] bool push(OutboundCommand command);
    [[nodiscard]] std::optional<OutboundCommand> try_pop();
    [[nodiscard]] std::size_t size() const;
    void clear();

private:
    mutable std::mutex mutex_;
    std::deque<OutboundCommand> queue_;
    std::size_t max_pending_;
};
}
