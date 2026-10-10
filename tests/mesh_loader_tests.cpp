#include "nexverse/assets/mesh_loader.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <zlib.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace
{
using nexverse::protocol::LlsdValue;

std::string small_mesh(bool with_uv_domain = true,
                       bool invalid_first_face = false)
{
    const std::vector<std::uint8_t> positions{
        0U, 0U, 0U, 0U, 0U, 0U,
        255U, 255U, 0U, 0U, 0U, 0U,
        0U, 0U, 255U, 255U, 0U, 0U};
    const std::vector<std::uint8_t> triangles{0U, 0U, 1U, 0U, 2U, 0U};
    const std::vector<std::uint8_t> uv{
        0U, 0U, 0U, 0U,
        255U, 255U, 0U, 0U,
        0U, 0U, 255U, 255U};

    const auto domain = LlsdValue::map({
        {"Min", LlsdValue::array({
            LlsdValue::real(0.0), LlsdValue::real(0.0)})},
        {"Max", LlsdValue::array({
            LlsdValue::real(1.0), LlsdValue::real(1.0)})}});

    std::map<std::string, LlsdValue, std::less<>> part{
        {"Position", LlsdValue::binary(positions)},
        {"TriangleList", LlsdValue::binary(triangles)},
        {"TexCoord0", LlsdValue::binary(uv)}};
    if (with_uv_domain) part.emplace("TexCoord0Domain", domain);
    std::vector<LlsdValue> faces;
    if (invalid_first_face)
        faces.push_back(LlsdValue::map({
            {"Position", LlsdValue::binary({0U, 0U})}}));
    faces.push_back(LlsdValue::map(std::move(part)));
    const auto lod = nexverse::protocol::serialize_llsd_binary(
        LlsdValue::array(std::move(faces)), false);

    std::vector<std::uint8_t> zipped(compressBound(
        static_cast<uLong>(lod.size())));
    uLongf count = static_cast<uLongf>(zipped.size());
    if (compress2(
            zipped.data(), &count, lod.data(),
            static_cast<uLong>(lod.size()), Z_BEST_SPEED) != Z_OK)
        return {};
    zipped.resize(static_cast<std::size_t>(count));

    const auto header = nexverse::protocol::serialize_llsd_binary(
        LlsdValue::map({
            {"version", LlsdValue::integer(1)},
            {"date", LlsdValue::date_seconds(1712345678.5)},
            {"high_lod", LlsdValue::map({
                {"offset", LlsdValue::integer(0)},
                {"size", LlsdValue::integer(
                    static_cast<std::int64_t>(zipped.size()))}})}}), false);

    std::string result(header.begin(), header.end());
    result.append(
        reinterpret_cast<const char*>(zipped.data()), zipped.size());
    return result;
}

class FakeTransport final : public nexverse::net::IHttpTransport
{
public:
    std::string body;
    std::string last_url;
    int code = 200;
    std::string bad_getmesh2;

    nexverse::net::HttpResponse get(
        std::string_view url, std::size_t,
        std::chrono::milliseconds, std::chrono::milliseconds) override
    {
        last_url.assign(url);
        if (!bad_getmesh2.empty() &&
            url.find("GetMesh2") != std::string_view::npos)
            return {200, bad_getmesh2, {}};
        if (code != 200)
            return {code, {}, "HTTP server returned status " +
                std::to_string(code)};
        return {code, body, {}};
    }

    nexverse::net::HttpResponse post(
        std::string_view, std::string_view, std::string_view,
        std::size_t, std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        return {405, {}, "not supported"};
    }
};
}

int main()
{
    using namespace nexverse::assets;
    constexpr std::string_view id =
        "11111111-2222-3333-4444-555555555555";
    const auto uri = build_get_mesh_uri(
        "https://caps.example.test/GetMesh", id);
    if (uri != "https://caps.example.test/GetMesh?mesh_id="
               "11111111-2222-3333-4444-555555555555" ||
        !build_get_mesh_uri("https://caps.example.test/GetMesh",
                            "not-a-uuid").empty())
    {
        std::cerr << "GetMesh URI mismatch\n";
        return 1;
    }

    const std::string bytes = small_mesh();
    const auto raw = std::span{
        reinterpret_cast<const std::uint8_t*>(bytes.data()), bytes.size()};
    std::size_t consumed = 0U;
    const auto prefix = nexverse::protocol::parse_llsd_binary_prefix(raw, consumed);
    if (!prefix.ok() || !prefix.value->is_map() ||
        consumed >= bytes.size() ||
        nexverse::protocol::parse_llsd_binary(raw).ok())
    {
        std::cerr << "Mesh LLSD header prefix boundaries incorrect\n";
        return 6;
    }
    const auto decoded = decode_mesh_asset(id, bytes);
    if (!decoded.success || !decoded.geometry.valid() ||
        decoded.geometry.indices.size() != 3U ||
        decoded.geometry.vertices.size() != 24U ||
        decoded.geometry.faces.size() != 1U ||
        decoded.geometry.faces[0].face != 0U ||
        decoded.geometry.faces[0].index_count != 3U)
    {
        std::cerr << "Mesh LOD decoding failed: " << decoded.message << '\n';
        return 2;
    }

    const auto transport = std::make_shared<FakeTransport>();
    transport->body = bytes;
    MeshAssetLoader loader{transport};
    const auto fetched = loader.fetch(
        "https://caps.example.test/GetMesh", id);
    if (!fetched.success || transport->last_url != uri)
    {
        std::cerr << "GetMesh fetch failed: " << fetched.message << '\n';
        return 3;
    }

    const auto omitted_domain = decode_mesh_asset(id, small_mesh(false));
    if (!omitted_domain.success ||
        omitted_domain.geometry.indices.size() != 3U)
    {
        std::cerr << "Mesh missing optional UV domain was rejected: "
                  << omitted_domain.message << '\n';
        return 7;
    }

    const auto partially_valid = decode_mesh_asset(id, small_mesh(true, true));
    if (!partially_valid.success ||
        partially_valid.geometry.indices.size() != 3U ||
        partially_valid.geometry.faces.size() != 1U ||
        partially_valid.geometry.faces[0].face != 1U ||
        partially_valid.message.find("skipped_invalid_faces=1") ==
            std::string::npos)
    {
        std::cerr << "Valid faces not preserved after damaged submesh: "
                  << partially_valid.message << '\n';
        return 9;
    }

    transport->bad_getmesh2 = "invalid";
    const auto alternate = loader.fetch(
        "https://caps.example.test/GetMesh2", id,
        "https://caps.example.test/GetMesh");
    if (!alternate.success ||
        transport->last_url.find("GetMesh?mesh_id=") ==
            std::string::npos ||
        alternate.message.find("alternate GetMesh") == std::string::npos)
    {
        std::cerr << "Secondary Mesh capability recovery failed: "
                  << alternate.message << '\n';
        return 8;
    }
    transport->bad_getmesh2.clear();

    transport->code = 404;
    const auto unavailable = loader.fetch(
        "https://caps.example.test/GetMesh", id);
    if (unavailable.success ||
        unavailable.message.find("404") == std::string::npos)
    {
        std::cerr << "GetMesh HTTP 404 not propagated\n";
        return 4;
    }
    if (decode_mesh_asset(id, "invalid").success ||
        decode_mesh_asset(id, bytes.substr(0U, 30U)).success ||
        decode_mesh_asset(id, bytes.substr(0U, bytes.size() - 5U)).success)
    {
        std::cerr << "Malformed mesh was accepted\n";
        return 5;
    }

    std::cout << "OpenSim mesh high-LOD and GetMesh tests passed\n";
    return 0;
}
