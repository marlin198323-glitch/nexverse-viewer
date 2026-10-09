/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/render_materials.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <zlib.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
using nexverse::protocol::LlsdValue;
using nexverse::protocol::RenderMaterialsService;
using nexverse::protocol::parse_render_materials_response;
class FakeTransport final : public nexverse::net::IHttpTransport
{
public:
    std::string payload;
    int calls = 0;
    nexverse::net::HttpResponse get(
        std::string_view uri, std::size_t,
        std::chrono::milliseconds, std::chrono::milliseconds) override
    {
        if (uri != "https://caps.example/materials") return {404, "", ""};
        ++calls;
        return {200, payload, {}};
    }
    nexverse::net::HttpResponse post(
        std::string_view, std::string_view, std::string_view,
        std::size_t, std::chrono::milliseconds,
        std::chrono::milliseconds) override { return {405, "", ""}; }
};

bool check(bool condition, const char* message)
{
    if (!condition) std::cerr << message << '\n';
    return condition;
}

std::string zipped(const LlsdValue& value)
{
    const auto data = nexverse::protocol::serialize_llsd_binary(value, false);
    uLongf max = compressBound(static_cast<uLong>(data.size()));
    std::vector<std::uint8_t> compressed(max);
    if (compress2(compressed.data(), &max, data.data(),
                  static_cast<uLong>(data.size()), Z_BEST_SPEED) != Z_OK)
        return {};
    compressed.resize(max);
    return nexverse::protocol::serialize_llsd_xml(LlsdValue::map(
        {{"Zipped", LlsdValue::binary(std::move(compressed))}}));
}
}

int main()
{
    std::vector<std::uint8_t> id = {
        0xaa,0xbb,0xcc,0xdd,0x01,0x02,0x03,0x04,
        0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c};
    const auto material = LlsdValue::map({
        {"DiffuseAlphaMode", LlsdValue::integer(2)},
        {"AlphaMaskCutoff", LlsdValue::integer(128)},
        {"NormMap", LlsdValue::uuid("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa")},
        {"SpecMap", LlsdValue::uuid("bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb")}});
    const auto item = LlsdValue::map({
        {"ID", LlsdValue::binary(id)}, {"Material", material}});
    const auto xml = zipped(LlsdValue::array({item}));
    if (!check(!xml.empty(), "Cannot create zipped material fixture")) return 1;
    const auto parsed = parse_render_materials_response(xml);
    if (!check(parsed.success && parsed.materials.size() == 1U,
               "Valid compressed GET materials rejected")) return 2;
    const auto found = parsed.materials.find(
        "aabbccdd-0102-0304-0506-0708090a0b0c");
    if (!check(found != parsed.materials.end() &&
               found->second.diffuse_alpha_mode == 2U &&
               found->second.alpha_mask_cutoff == 128U &&
               found->second.normal_id == "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa" &&
               found->second.specular_id == "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb",
               "RenderMaterials field decoding incorrect")) return 3;
    auto fake = std::make_shared<FakeTransport>();
    fake->payload = xml;
    RenderMaterialsService service{fake};
    if (!check(service.fetch_all("https://caps.example/materials").success &&
               fake->calls == 1, "CAPS material GET not invoked")) return 4;
    if (!check(!service.fetch_all("file:///etc/passwd").success &&
               fake->calls == 1, "Unsafe CAPS URL accepted")) return 5;
    if (!check(!parse_render_materials_response(
           "<llsd><map><key>Zipped</key><binary>!!!!</binary></map></llsd>").success,
           "Invalid base64 accepted")) return 6;
    if (!check(!parse_render_materials_response(zipped(LlsdValue::map())).success,
               "Non-array LLSD accepted")) return 7;
    if (!check(!parse_render_materials_response(xml.substr(0, xml.size()/2)).success,
               "Truncated payload accepted")) return 8;
    const auto huge = LlsdValue::string(std::string(9U*1024U*1024U, 'x'));
    if (!check(!parse_render_materials_response(zipped(huge)).success,
               "Decompression bomb limit bypassed")) return 9;
    auto invalid = LlsdValue::map({
        {"ID", LlsdValue::binary(id)},
        {"Material", LlsdValue::map({{"DiffuseAlphaMode",LlsdValue::integer(7)}})}});
    if (!check(parse_render_materials_response(zipped(LlsdValue::array({invalid}))).materials.empty(),
               "Out-of-range alpha mode accepted")) return 10;
    std::cout << "OpenSim RenderMaterials capability decoding passed\n";
}
