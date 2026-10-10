#include "nexverse/protocol/llsd.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main()
{
    using nexverse::protocol::parse_llsd_xml;
    for (const auto value : {
        "<llsd><integer/></llsd>",
        "<llsd><integer>   </integer></llsd>",
        "<llsd><integer> 4294967295 </integer></llsd>",
        "<llsd><integer>+42</integer></llsd>"})
    {
        const auto parsed = parse_llsd_xml(value);
        if (!parsed.ok() ||
            parsed.value->type != nexverse::protocol::LlsdType::integer)
        {
            std::cerr << "LLSD XML integral compatibility failed\n";
            return 90;
        }
    }
    const auto wide_integer =
        parse_llsd_xml("<llsd><integer>4294967295</integer></llsd>");
    if (!wide_integer.ok() ||
        wide_integer.value->integer_value != 4294967295LL ||
        parse_llsd_xml("<llsd><integer>18446744073709551616</integer></llsd>").ok() ||
        parse_llsd_xml("<llsd><integer>12abc</integer></llsd>").ok())
    {
        std::cerr << "LLSD XML 64-bit range validation failed\n";
        return 91;
    }

    using namespace nexverse::protocol;

    constexpr std::string_view event_with_date =
        "<llsd><map><key>id</key><integer>7</integer>"
        "<key>timestamp</key><date>2026-10-10T08:04:03.125Z</date>"
        "</map></llsd>";
    const auto dated_event = parse_llsd_xml(event_with_date);
    if (!dated_event.ok() || !dated_event.value->is_map() ||
        !dated_event.value->find("timestamp") ||
        dated_event.value->find("timestamp")->type != LlsdType::date ||
        dated_event.value->find("timestamp")->string_value !=
            "2026-10-10T08:04:03.125Z" ||
        serialize_llsd_xml(*dated_event.value).find("<date>") ==
            std::string::npos)
    {
        std::cerr << "OpenSim EventQueue date XML compatibility failed\n";
        return 92;
    }
    const auto binary_date = serialize_llsd_binary(
        LlsdValue::map({{"date", LlsdValue::date_seconds(1712345678.5)}}));
    const auto decoded_date = parse_llsd_binary(binary_date);
    if (!decoded_date.ok() || !decoded_date.value->find("date") ||
        decoded_date.value->find("date")->type != LlsdType::date ||
        decoded_date.value->find("date")->real_value != 1712345678.5)
    {
        std::cerr << "Mesh LLSD date binary compatibility failed\n";
        return 93;
    }
    std::vector<std::uint8_t> truncated_date{'d', 0x40U};
    if (parse_llsd_binary(truncated_date).ok() ||
        parse_llsd_xml("<llsd><date>" +
            std::string(160U, 'x') + "</date></llsd>").ok())
    {
        std::cerr << "Malformed LLSD date accepted\n";
        return 94;
    }

    LlsdValue root = LlsdValue::map({
        {"name", LlsdValue::string("NexVerse")},
        {"enabled", LlsdValue::boolean(true)},
        {"count", LlsdValue::integer(42)},
        {"ratio", LlsdValue::real(1.25)},
        {"agent", LlsdValue::uuid("11111111-2222-3333-4444-555555555555")},
        {"cap", LlsdValue::uri("https://caps.example.test/seed")},
        {"blob", LlsdValue::binary({0x01U, 0x02U, 0xFEU})},
        {"list", LlsdValue::array({
            LlsdValue::string("one"),
            LlsdValue::integer(-7)})}
    });

    const std::string xml = serialize_llsd_xml(root);
    const auto parsed_xml = parse_llsd_xml(xml);
    if (!parsed_xml.ok() || !parsed_xml.value->is_map())
    {
        std::cerr << "LLSD XML roundtrip failed\n";
        return 1;
    }

    const auto* xml_name = parsed_xml.value->find("name");
    const auto* xml_cap = parsed_xml.value->find("cap");
    const auto* xml_blob = parsed_xml.value->find("blob");
    const auto* xml_list = parsed_xml.value->find("list");
    if (xml_name == nullptr || xml_name->as_string() != "NexVerse" ||
        xml_cap == nullptr || xml_cap->as_string() != "https://caps.example.test/seed" ||
        xml_blob == nullptr || xml_blob->binary_value.size() != 3U ||
        xml_list == nullptr || !xml_list->is_array() || xml_list->array_value.size() != 2U)
    {
        std::cerr << "LLSD XML values mismatch\n";
        return 2;
    }

    const auto binary = serialize_llsd_binary(root);
    const auto parsed_binary = parse_llsd_binary(binary);
    if (!parsed_binary.ok() || !parsed_binary.value->is_map())
    {
        std::cerr << "LLSD binary roundtrip failed\n";
        return 3;
    }

    const auto* binary_agent = parsed_binary.value->find("agent");
    const auto* binary_count = parsed_binary.value->find("count");
    if (binary_agent == nullptr ||
        binary_agent->as_string() != "11111111-2222-3333-4444-555555555555" ||
        binary_count == nullptr ||
        binary_count->integer_value != 42)
    {
        std::cerr << "LLSD binary values mismatch\n";
        return 4;
    }

    constexpr std::string_view external_xml =
        "<llsd><map>"
        "<key>EventQueueGet</key><uri>https://caps.example.test/event</uri>"
        "<key>SimulatorFeatures</key><uri>https://caps.example.test/features</uri>"
        "</map></llsd>";

    const auto external = parse_llsd_xml(external_xml);
    if (!external.ok() ||
        external.value->find("EventQueueGet") == nullptr ||
        external.value->find("SimulatorFeatures") == nullptr)
    {
        std::cerr << "LLSD capability map parse failed\n";
        return 5;
    }

    constexpr std::string_view opensim_reals_xml =
        "<llsd><map>"
        "<key>empty</key><real/>"
        "<key>spaced</key><real>  1.5000  </real>"
        "<key>nexverse</key><real>11.001100540161133</real>"
        "</map></llsd>";

    const auto opensim_reals = parse_llsd_xml(opensim_reals_xml);
    if (!opensim_reals.ok() ||
        opensim_reals.value->find("empty") == nullptr ||
        opensim_reals.value->find("empty")->real_value != 0.0 ||
        opensim_reals.value->find("spaced") == nullptr ||
        std::abs(opensim_reals.value->find("spaced")->real_value - 1.5) > 0.000001 ||
        opensim_reals.value->find("nexverse") == nullptr ||
        std::abs(
            opensim_reals.value->find("nexverse")->real_value -
            11.001100540161133) > 0.000000000001)
    {
        std::cerr << "OpenSim LLSD real compatibility failed\n";
        return 6;
    }

    const std::vector<std::uint8_t> invalid{'[', 0U, 0U, 0U, 1U, 's'};
    if (parse_llsd_binary(invalid).ok())
    {
        std::cerr << "Invalid LLSD binary unexpectedly accepted\n";
        return 7;
    }

    std::cout << "LLSD XML and binary tests passed\n";
    return 0;
}
