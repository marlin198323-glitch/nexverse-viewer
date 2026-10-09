/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/render_materials.hpp"
#include "nexverse/protocol/region_material_scope.hpp"
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
    int post_calls = 0;
    std::string post_body;
    std::string post_type;
    nexverse::net::HttpResponse get(
        std::string_view uri, std::size_t,
        std::chrono::milliseconds, std::chrono::milliseconds) override
    {
        if (uri != "https://caps.example/materials") return {404, "", ""};
        ++calls;
        return {200, payload, {}};
    }
    nexverse::net::HttpResponse post(
        std::string_view uri, std::string_view content_type, std::string_view body,
        std::size_t, std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        if (uri != "https://caps.example/materials")
            return {404, "", ""};
        ++post_calls;
        post_body.assign(body);
        post_type.assign(content_type);
        return {200, payload, {}};
    }
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
        {"SpecMap", LlsdValue::uuid("bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb")},
        {"NormOffsetX", LlsdValue::integer(-2500)},
        {"NormOffsetY", LlsdValue::integer(3750)},
        {"NormRepeatX", LlsdValue::integer(20000)},
        {"NormRepeatY", LlsdValue::integer(5000)},
        {"NormRotation", LlsdValue::integer(15708)},
        {"SpecOffsetX", LlsdValue::integer(1250)},
        {"SpecOffsetY", LlsdValue::integer(-1000)},
        {"SpecRepeatX", LlsdValue::integer(30000)},
        {"SpecRepeatY", LlsdValue::integer(10000)},
        {"SpecRotation", LlsdValue::integer(-31416)},
        {"SpecColor", LlsdValue::array({
            LlsdValue::integer(128), LlsdValue::integer(64),
            LlsdValue::integer(255), LlsdValue::integer(255)})},
        {"SpecExp", LlsdValue::integer(200)},
        {"EnvIntensity", LlsdValue::integer(80)}});
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
               found->second.specular_id == "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb" &&
               found->second.normal_transform.offset_x == -0.25f &&
               found->second.normal_transform.offset_y == 0.375f &&
               found->second.normal_transform.repeat_x == 2.0f &&
               found->second.normal_transform.repeat_y == 0.5f &&
               found->second.normal_transform.rotation > 1.5707f &&
               found->second.normal_transform.rotation < 1.5709f &&
               found->second.specular_transform.offset_x == 0.125f &&
               found->second.specular_transform.offset_y == -0.1f &&
               found->second.specular_transform.repeat_x == 3.0f &&
               found->second.specular_transform.repeat_y == 1.0f &&
               found->second.specular_transform.rotation < -3.1415f &&
               found->second.specular_transform.rotation > -3.1417f &&
               found->second.specular_color[0] > 0.501f &&
               found->second.specular_color[0] < 0.503f &&
               found->second.specular_color[1] > 0.250f &&
               found->second.specular_color[1] < 0.252f &&
               found->second.specular_color[2] == 1.0f &&
               found->second.specular_exponent == 200U &&
               found->second.environment_intensity == 80U,
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
    const auto request = nexverse::protocol::build_render_materials_request({
        "AABBCCDD-0102-0304-0506-0708090A0B0C",
        "aabbccdd-0102-0304-0506-0708090a0b0c"});
    if (!check(!request.empty(), "Cannot encode targeted POST batch")) return 11;
    const auto outer = nexverse::protocol::parse_llsd_xml(request);
    if (!check(outer.ok(), "POST wrapper LLSD is invalid")) return 12;
    const auto* wrapped = outer.value->find("Zipped");
    if (!check(wrapped && wrapped->type == nexverse::protocol::LlsdType::binary,
               "POST is missing Zipped binary")) return 13;
    std::array<std::uint8_t, 4096> uncompressed{};
    uLongf uncompressed_size = uncompressed.size();
    if (!check(uncompress(uncompressed.data(), &uncompressed_size,
                         wrapped->binary_value.data(),
                         wrapped->binary_value.size()) == Z_OK,
               "POST compression invalid")) return 14;
    const auto ids = nexverse::protocol::parse_llsd_binary(
        std::span<const std::uint8_t>(uncompressed.data(), uncompressed_size));
    if (!check(ids.ok() && ids.value->is_array() &&
               ids.value->array_value.size() == 1U &&
               ids.value->array_value.front().type ==
                   nexverse::protocol::LlsdType::binary &&
               ids.value->array_value.front().binary_value == id,
               "POST body must contain deduplicated 16-byte binary UUIDs"))
        return 15;
    if (!check(service.fetch_ids("https://caps.example/materials", {
                   "aabbccdd-0102-0304-0506-0708090a0b0c"}).success &&
               fake->post_calls == 1 && fake->post_body.size() > 0U &&
               fake->post_type == "application/llsd+xml",
               "POST capability request was not sent correctly")) return 16;
    if (!check(!service.fetch_ids("file:///etc/shadow", {
                   "aabbccdd-0102-0304-0506-0708090a0b0c"}).success &&
               fake->post_calls == 1, "POST allowed unsafe capability URI"))
        return 17;
    if (!check(nexverse::protocol::build_render_materials_request({}).empty() &&
               nexverse::protocol::build_render_materials_request({
                   "this-is-not-an-id"}).empty() &&
               nexverse::protocol::build_render_materials_request({
                   "00000000-0000-0000-0000-000000000000"}).empty() &&
               nexverse::protocol::build_render_materials_request(
                   std::vector<std::string>(
                       nexverse::protocol::max_render_materials_batch_ids + 1U,
                       "aabbccdd-0102-0304-0506-0708090a0b0c")).empty(),
               "Invalid/overlarge POST batch accepted")) return 18;
    if (!check(nexverse::protocol::is_valid_legacy_material_id(
                   "aabbccdd-0102-0304-0506-0708090a0b0c") &&
               !nexverse::protocol::is_valid_legacy_material_id(
                   "00000000-0000-0000-0000-000000000000"),
               "Material UUID normalization check failed")) return 19;
    // A capable server may return a larger material list than requested.
    // POST results must be filtered to the requested UUIDs.
    const auto unrelated = LlsdValue::map({
        {"ID", LlsdValue::binary(std::vector<std::uint8_t>(16U, 0x1fU))},
        {"Material", material}});
    fake->payload = zipped(LlsdValue::array({item, unrelated}));
    const auto filtered = service.fetch_ids("https://caps.example/materials",
        {"aabbccdd-0102-0304-0506-0708090a0b0c"});
    if (!check(filtered.success && filtered.materials.size() == 1U &&
               filtered.materials.contains("aabbccdd-0102-0304-0506-0708090a0b0c"),
               "POST returned unrequested material definitions")) return 20;
    const auto invalid_transform = LlsdValue::map({
        {"ID", LlsdValue::binary(id)},
        {"Material", LlsdValue::map({
            {"NormRepeatX", LlsdValue::integer(2000000)}})}});
    if (!check(parse_render_materials_response(
        zipped(LlsdValue::array({invalid_transform}))).materials.empty(),
        "Oversized texture transform accepted")) return 21;
    const auto invalid_color = LlsdValue::map({
        {"ID", LlsdValue::binary(id)},
        {"Material", LlsdValue::map({
            {"SpecColor", LlsdValue::array({
                LlsdValue::integer(256),
                LlsdValue::integer(0),
                LlsdValue::integer(0),
                LlsdValue::integer(255)})}})}});
    if (!check(parse_render_materials_response(
        zipped(LlsdValue::array({invalid_color}))).materials.empty(),
        "Oversized specular color accepted")) return 22;
    const auto invalid_exponent = LlsdValue::map({
        {"ID", LlsdValue::binary(id)},
        {"Material", LlsdValue::map({
            {"SpecExp", LlsdValue::integer(-1)}})}});
    if (!check(parse_render_materials_response(
        zipped(LlsdValue::array({invalid_exponent}))).materials.empty(),
        "Negative specular exponent accepted")) return 23;
    // A late GET, POST, or texture response is only valid in the region
    // and capability seed which initiated it. A handshake update with
    // unchanged identity must not clear the cache.
    nexverse::protocol::RegionMaterialScope scope;
    if (!check(scope.update(100U, 0U, "https://caps.example/seed-a"),
               "Initial region scope was not established")) return 24;
    const auto first_epoch = scope.epoch();
    if (!check(scope.matches(first_epoch) &&
               !scope.update(100U, 0U, "https://caps.example/seed-a") &&
               scope.matches(first_epoch),
               "Same region handshake invalidated material scope")) return 25;
    if (!check(scope.update(100U, 1U, "https://caps.example/seed-a") &&
               !scope.matches(first_epoch),
               "Same-handle teleport generation failed to reject stale reply"))
        return 26;
    const auto teleport_epoch = scope.epoch();
    if (!check(scope.update(200U, 1U, "https://caps.example/seed-a") &&
               !scope.matches(teleport_epoch),
               "Crossed-region handle failed to invalidate cache")) return 27;
    const auto crossed_epoch = scope.epoch();
    if (!check(scope.update(200U, 1U, "https://caps.example/seed-b") &&
               !scope.matches(crossed_epoch) &&
               scope.seed_capability() == "https://caps.example/seed-b",
               "Changed seed did not invalidate material scope")) return 28;
    if (!check(scope.matches(scope.epoch()),
               "Current material response epoch rejected")) return 29;
    std::cout << "OpenSim RenderMaterials region scope and cache lifetimes passed\n";
}
