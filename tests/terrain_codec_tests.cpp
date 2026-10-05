#include "nexverse/protocol/terrain_codec.hpp"
#include "nexverse/protocol/viewer_messages.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace
{
class BitWriter
{
public:
    void pack_msb(std::uint32_t value, std::uint32_t count)
    {
        for (std::uint32_t i = 0U; i < count; ++i)
        {
            const std::uint32_t shift = count - 1U - i;
            const bool bit = ((value >> shift) & 1U) != 0U;
            if (bit_offset_ % 8U == 0U)
            {
                data_.push_back(0U);
            }
            if (bit)
            {
                data_.back() |= static_cast<std::uint8_t>(
                    0x80U >> (bit_offset_ % 8U));
            }
            ++bit_offset_;
        }
    }

    void pack_packed(std::uint32_t value, std::uint32_t count)
    {
        std::uint32_t remaining = count;
        std::uint32_t shift = 0U;
        while (remaining > 0U)
        {
            const std::uint32_t chunk = remaining > 8U ? 8U : remaining;
            const std::uint32_t mask =
                chunk == 32U ? 0xFFFFFFFFU : ((1U << chunk) - 1U);
            pack_msb((value >> shift) & mask, chunk);
            shift += 8U;
            remaining -= chunk;
        }
    }

    void pack_float(float value)
    {
        pack_packed(std::bit_cast<std::uint32_t>(value), 32U);
    }

    [[nodiscard]] const std::vector<std::uint8_t>& data() const { return data_; }

private:
    std::vector<std::uint8_t> data_;
    std::size_t bit_offset_ = 0U;
};
}

int main()
{
    using namespace nexverse::protocol;

    BitWriter writer;
    writer.pack_packed(264U, 16U);
    writer.pack_packed(16U, 8U);
    writer.pack_packed(layer_type_land, 8U);

    writer.pack_packed(0x80U, 8U);
    writer.pack_float(9.5f);
    writer.pack_packed(1U, 16U);
    writer.pack_packed((2U << 5U) | 3U, 10U);
    writer.pack_msb(0b10U, 2U);
    writer.pack_packed(97U, 8U);

    const auto decoded = decode_terrain_layer(
        writer.data(),
        layer_type_land);

    if (!decoded.success ||
        decoded.patches.size() != 1U ||
        decoded.patches[0].x != 2U ||
        decoded.patches[0].y != 3U)
    {
        std::cerr << "Terrain codec structural decode failed\n";
        return 1;
    }

    for (float height : decoded.patches[0].heights)
    {
        if (std::abs(height - 10.0f) > 0.0001f)
        {
            std::cerr << "Terrain codec height decode failed\n";
            return 2;
        }
    }

    std::cout << "Terrain codec tests passed\n";
    return 0;
}
