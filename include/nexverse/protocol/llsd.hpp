/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
enum class LlsdType
{
    undefined,
    boolean,
    integer,
    real,
    string,
    uuid,
    uri,
    date,
    binary,
    array,
    map
};

struct LlsdValue
{
    LlsdType type = LlsdType::undefined;
    bool boolean_value = false;
    std::int64_t integer_value = 0;
    double real_value = 0.0;
    std::string string_value;
    std::vector<std::uint8_t> binary_value;
    std::vector<LlsdValue> array_value;
    std::map<std::string, LlsdValue, std::less<>> map_value;

    static LlsdValue boolean(bool value);
    static LlsdValue integer(std::int64_t value);
    static LlsdValue real(double value);
    static LlsdValue string(std::string value);
    static LlsdValue uuid(std::string value);
    static LlsdValue uri(std::string value);
    static LlsdValue date(std::string value);
    static LlsdValue date_seconds(double value);
    static LlsdValue binary(std::vector<std::uint8_t> value);
    static LlsdValue array(std::vector<LlsdValue> value = {});
    static LlsdValue map(std::map<std::string, LlsdValue, std::less<>> value = {});

    [[nodiscard]] const LlsdValue* find(std::string_view key) const;
    [[nodiscard]] std::string_view as_string() const noexcept;
    [[nodiscard]] bool is_map() const noexcept;
    [[nodiscard]] bool is_array() const noexcept;
};

struct LlsdParseResult
{
    std::optional<LlsdValue> value;
    std::string error;

    [[nodiscard]] bool ok() const noexcept
    {
        return value.has_value() && error.empty();
    }
};

[[nodiscard]] std::string serialize_llsd_xml(const LlsdValue& value);
[[nodiscard]] LlsdParseResult parse_llsd_xml(std::string_view xml);

[[nodiscard]] std::vector<std::uint8_t> serialize_llsd_binary(
    const LlsdValue& value,
    bool prepend_header = true);
[[nodiscard]] LlsdParseResult parse_llsd_binary(
    std::span<const std::uint8_t> bytes);
// Mesh assets start with a binary LLSD header followed by raw compressed blocks.
[[nodiscard]] LlsdParseResult parse_llsd_binary_prefix(
    std::span<const std::uint8_t> bytes,
    std::size_t& bytes_consumed);
}
