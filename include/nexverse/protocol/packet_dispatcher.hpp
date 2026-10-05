/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/lludp.hpp"

#include <cstddef>
#include <functional>
#include <map>
#include <string>

namespace nexverse::protocol
{
struct MessageIdLess
{
    bool operator()(const MessageId& left, const MessageId& right) const noexcept
    {
        if (left.frequency != right.frequency)
        {
            return static_cast<int>(left.frequency) < static_cast<int>(right.frequency);
        }
        return left.value < right.value;
    }
};

class PacketDispatcher
{
public:
    using Handler = std::function<void(const DecodedPacket&)>;

    void register_handler(MessageId id, Handler handler);
    void clear_handler(MessageId id);

    [[nodiscard]] bool dispatch(const DecodedPacket& packet) const;
    [[nodiscard]] bool has_handler(MessageId id) const;
    [[nodiscard]] std::size_t handler_count() const noexcept;

private:
    std::map<MessageId, Handler, MessageIdLess> handlers_;
};
}
