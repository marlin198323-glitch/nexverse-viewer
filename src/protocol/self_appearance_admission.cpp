/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/self_appearance_admission.hpp"
#include "nexverse/protocol/texture_entry.hpp"

namespace nexverse::protocol
{
bool valid_self_appearance_texture_entry(
    std::span<const std::uint8_t> texture_entry)
{
    // Nonempty data alone is not enough: malformed/truncated TE
    // previously evicted the last authoritative self outfit. Use the
    // exact parser already employed by GetTexture/baked face discovery.
    return parse_texture_entry(texture_entry).has_value();
}
}
