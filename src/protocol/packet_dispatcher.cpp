/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/packet_dispatcher.hpp"

#include <utility>

namespace nexverse::protocol
{
void PacketDispatcher::register_handler(MessageId id, Handler handler)
{
    if (!handler)
    {
        handlers_.erase(id);
        return;
    }
    handlers_[id] = std::move(handler);
}

void PacketDispatcher::clear_handler(MessageId id)
{
    handlers_.erase(id);
}

bool PacketDispatcher::dispatch(const DecodedPacket& packet) const
{
    const auto found = handlers_.find(packet.header.message);
    if (found == handlers_.end())
    {
        return false;
    }

    found->second(packet);
    return true;
}

bool PacketDispatcher::has_handler(MessageId id) const
{
    return handlers_.contains(id);
}

std::size_t PacketDispatcher::handler_count() const noexcept
{
    return handlers_.size();
}
}
