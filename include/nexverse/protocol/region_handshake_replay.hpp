/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
namespace nexverse::protocol
{
// LLUDP reliable retransmissions can be out of order (not just the
// last inbound sequence). Keep a small, circuit-scoped bounded history.
// Retries remain transport-ACKed; never renegotiate the same packet.
class RegionHandshakeReplay
{
public:
    static constexpr std::size_t capacity=16U;
    [[nodiscard]] bool already_replied(
        std::uint32_t inbound_sequence) const noexcept
    {
        for(std::size_t i=0;i<count_;++i)
            if(history_[i]==inbound_sequence)return true;
        return false;
    }
    void note_replied(std::uint32_t inbound_sequence) noexcept
    {
        if(already_replied(inbound_sequence))return;
        history_[next_]=inbound_sequence;
        next_=(next_+1U)%capacity;
        if(count_<capacity)++count_;
    }
    void reset() noexcept
    {
        history_.fill(0U);count_=0U;next_=0U;
    }
private:
    std::array<std::uint32_t,capacity> history_{};
    std::size_t count_=0U;
    std::size_t next_=0U;
};
}
