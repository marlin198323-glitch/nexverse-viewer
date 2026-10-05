/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/terrain_codec.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>

namespace
{
constexpr float oo_sqrt2 = 0.7071067811865475244f;
constexpr std::uint32_t end_of_patches = 97U;
constexpr std::uint32_t expected_stride = 264U;

class BitReader
{
public:
    explicit BitReader(std::span<const std::uint8_t> data)
        : data_{data}
    {
    }

    bool read_msb(std::uint32_t count, std::uint32_t& value)
    {
        if (count > 32U || remaining_bits() < count)
        {
            return false;
        }

        value = 0U;
        for (std::uint32_t index = 0U; index < count; ++index)
        {
            const std::size_t byte_index = bit_offset_ / 8U;
            const std::size_t bit_index = bit_offset_ % 8U;
            const std::uint8_t mask =
                static_cast<std::uint8_t>(0x80U >> bit_index);
            value = (value << 1U) |
                ((data_[byte_index] & mask) != 0U ? 1U : 0U);
            ++bit_offset_;
        }
        return true;
    }

    bool read_packed(std::uint32_t count, std::uint32_t& value)
    {
        if (count > 32U)
        {
            return false;
        }

        value = 0U;
        std::uint32_t remaining = count;
        std::uint32_t shift = 0U;

        while (remaining > 0U)
        {
            const std::uint32_t chunk = std::min<std::uint32_t>(8U, remaining);
            std::uint32_t part = 0U;
            if (!read_msb(chunk, part))
            {
                return false;
            }

            value |= part << shift;
            shift += 8U;
            remaining -= chunk;
        }

        return true;
    }

    bool read_float(float& value)
    {
        std::uint32_t bits = 0U;
        if (!read_packed(32U, bits))
        {
            return false;
        }
        value = std::bit_cast<float>(bits);
        return std::isfinite(value);
    }

private:
    [[nodiscard]] std::size_t remaining_bits() const noexcept
    {
        return data_.size() * 8U - bit_offset_;
    }

    std::span<const std::uint8_t> data_;
    std::size_t bit_offset_ = 0U;
};

std::array<int, nexverse::protocol::terrain_patch_samples> build_copy_matrix()
{
    std::array<int, nexverse::protocol::terrain_patch_samples> matrix{};

    bool diagonal = false;
    bool right = true;
    int i = 0;
    int j = 0;
    int count = 0;

    while (i < 16 && j < 16)
    {
        matrix[static_cast<std::size_t>(j * 16 + i)] = count++;

        if (!diagonal)
        {
            if (right)
            {
                if (i < 15) ++i;
                else ++j;
                right = false;
                diagonal = true;
            }
            else
            {
                if (j < 15) ++j;
                else ++i;
                right = true;
                diagonal = true;
            }
        }
        else if (right)
        {
            ++i;
            --j;
            if (i == 15 || j == 0) diagonal = false;
        }
        else
        {
            --i;
            ++j;
            if (j == 15 || i == 0) diagonal = false;
        }
    }

    return matrix;
}

const std::array<int, nexverse::protocol::terrain_patch_samples>& copy_matrix()
{
    static const auto value = build_copy_matrix();
    return value;
}

const std::array<float, nexverse::protocol::terrain_patch_samples>& dequantize_table()
{
    static const auto table = []
    {
        std::array<float, nexverse::protocol::terrain_patch_samples> value{};
        for (std::size_t y = 0U; y < 16U; ++y)
        {
            for (std::size_t x = 0U; x < 16U; ++x)
            {
                value[y * 16U + x] =
                    1.0f + 2.0f * static_cast<float>(x + y);
            }
        }
        return value;
    }();
    return table;
}

const std::array<float, nexverse::protocol::terrain_patch_samples>& cosine_table()
{
    static const auto table = []
    {
        std::array<float, nexverse::protocol::terrain_patch_samples> value{};
        const float half_pi_over_size =
            std::numbers::pi_v<float> * 0.5f / 16.0f;

        for (std::size_t u = 0U; u < 16U; ++u)
        {
            for (std::size_t n = 0U; n < 16U; ++n)
            {
                value[u * 16U + n] = std::cos(
                    (2.0f * static_cast<float>(n) + 1.0f) *
                    static_cast<float>(u) *
                    half_pi_over_size);
            }
        }
        return value;
    }();
    return table;
}

bool decode_coefficients(
    BitReader& reader,
    std::uint32_t word_bits,
    std::array<int, nexverse::protocol::terrain_patch_samples>& coefficients)
{
    if (word_bits < 2U || word_bits > 17U)
    {
        return false;
    }

    coefficients.fill(0);

    for (std::size_t index = 0U; index < coefficients.size(); ++index)
    {
        std::uint32_t marker = 0U;
        if (!reader.read_packed(1U, marker))
        {
            return false;
        }

        if (marker == 0U)
        {
            continue;
        }

        std::uint32_t value_or_eob = 0U;
        if (!reader.read_packed(1U, value_or_eob))
        {
            return false;
        }

        if (value_or_eob == 0U)
        {
            return true;
        }

        std::uint32_t negative = 0U;
        std::uint32_t magnitude = 0U;
        if (!reader.read_packed(1U, negative) ||
            !reader.read_packed(word_bits, magnitude))
        {
            return false;
        }

        if (magnitude > static_cast<std::uint32_t>(std::numeric_limits<int>::max()))
        {
            return false;
        }

        coefficients[index] =
            negative != 0U
                ? -static_cast<int>(magnitude)
                : static_cast<int>(magnitude);
    }

    return true;
}

std::array<float, nexverse::protocol::terrain_patch_samples> decompress_patch(
    const std::array<int, nexverse::protocol::terrain_patch_samples>& coefficients,
    std::uint8_t quant_word_bits,
    std::uint16_t range,
    float dc_offset)
{
    std::array<float, nexverse::protocol::terrain_patch_samples> block{};
    std::array<float, nexverse::protocol::terrain_patch_samples> temp{};
    std::array<float, nexverse::protocol::terrain_patch_samples> output{};

    const auto& copy = copy_matrix();
    const auto& dequant = dequantize_table();
    const auto& cosines = cosine_table();

    for (std::size_t index = 0U; index < block.size(); ++index)
    {
        block[index] =
            static_cast<float>(coefficients[static_cast<std::size_t>(copy[index])]) *
            dequant[index];
    }

    for (std::size_t column = 0U; column < 16U; ++column)
    {
        for (std::size_t n = 0U; n < 16U; ++n)
        {
            float total = oo_sqrt2 * block[column];
            for (std::size_t u = 1U; u < 16U; ++u)
            {
                total +=
                    block[u * 16U + column] *
                    cosines[u * 16U + n];
            }
            temp[n * 16U + column] = total;
        }
    }

    for (std::size_t line = 0U; line < 16U; ++line)
    {
        const std::size_t base = line * 16U;
        for (std::size_t n = 0U; n < 16U; ++n)
        {
            float total = oo_sqrt2 * temp[base];
            for (std::size_t u = 1U; u < 16U; ++u)
            {
                total +=
                    temp[base + u] *
                    cosines[u * 16U + n];
            }
            block[base + n] = total * (2.0f / 16.0f);
        }
    }

    const int prequant = static_cast<int>(quant_word_bits >> 4U) + 2;
    const float quantize = std::ldexp(1.0f, prequant);
    const float mult = static_cast<float>(range) / quantize;
    const float add =
        mult * std::ldexp(1.0f, prequant - 1) + dc_offset;

    for (std::size_t index = 0U; index < output.size(); ++index)
    {
        output[index] = block[index] * mult + add;
    }

    return output;
}
}

namespace nexverse::protocol
{
TerrainDecodeResult decode_terrain_layer(
    std::span<const std::uint8_t> data,
    std::uint8_t expected_layer_type)
{
    if (data.size() < 5U)
    {
        return {false, {}, "Terrain layer is too short"};
    }

    BitReader reader{data};

    std::uint32_t stride = 0U;
    std::uint32_t patch_size = 0U;
    std::uint32_t layer_type = 0U;

    if (!reader.read_packed(16U, stride) ||
        !reader.read_packed(8U, patch_size) ||
        !reader.read_packed(8U, layer_type))
    {
        return {false, {}, "Terrain group header is truncated"};
    }

    if (stride != expected_stride ||
        patch_size != terrain_patch_size ||
        layer_type != expected_layer_type)
    {
        return {false, {}, "Terrain group header is unsupported"};
    }

    TerrainDecodeResult result;
    result.success = true;
    result.message = "Terrain layer decoded";

    std::array<bool, terrain_patches_per_edge * terrain_patches_per_edge> seen{};

    while (true)
    {
        std::uint32_t quant_word_bits_raw = 0U;
        if (!reader.read_packed(8U, quant_word_bits_raw))
        {
            return {false, {}, "Terrain patch header is truncated"};
        }

        if (quant_word_bits_raw == end_of_patches)
        {
            break;
        }

        float dc_offset = 0.0f;
        std::uint32_t range_raw = 0U;
        std::uint32_t patch_id = 0U;

        if (!reader.read_float(dc_offset) ||
            !reader.read_packed(16U, range_raw) ||
            !reader.read_packed(10U, patch_id))
        {
            return {false, {}, "Terrain patch metadata is truncated"};
        }

        const std::uint32_t word_bits =
            (quant_word_bits_raw & 0x0FU) + 2U;
        const std::uint32_t x = patch_id >> 5U;
        const std::uint32_t y = patch_id & 0x1FU;

        if (x >= terrain_patches_per_edge ||
            y >= terrain_patches_per_edge ||
            range_raw == 0U ||
            range_raw > std::numeric_limits<std::uint16_t>::max())
        {
            return {false, {}, "Terrain patch metadata is invalid"};
        }

        const std::size_t slot =
            static_cast<std::size_t>(y) * terrain_patches_per_edge +
            static_cast<std::size_t>(x);
        if (seen[slot])
        {
            return {false, {}, "Terrain layer contains duplicate patch coordinates"};
        }
        seen[slot] = true;

        std::array<int, terrain_patch_samples> coefficients{};
        if (!decode_coefficients(reader, word_bits, coefficients))
        {
            return {false, {}, "Terrain patch coefficients are invalid"};
        }

        TerrainPatch patch;
        patch.x = static_cast<std::uint8_t>(x);
        patch.y = static_cast<std::uint8_t>(y);
        patch.heights = decompress_patch(
            coefficients,
            static_cast<std::uint8_t>(quant_word_bits_raw),
            static_cast<std::uint16_t>(range_raw),
            dc_offset);

        if (!std::all_of(
                patch.heights.begin(),
                patch.heights.end(),
                [](float value) { return std::isfinite(value); }))
        {
            return {false, {}, "Terrain patch produced non-finite heights"};
        }

        result.patches.push_back(std::move(patch));

        if (result.patches.size() >
            terrain_patches_per_edge * terrain_patches_per_edge)
        {
            return {false, {}, "Terrain layer contains too many patches"};
        }
    }

    if (result.patches.empty())
    {
        return {false, {}, "Terrain layer contains no patches"};
    }

    return result;
}
}
