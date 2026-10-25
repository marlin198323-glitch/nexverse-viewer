/* Mozilla Public License, v. 2.0 */
#pragma once
#include <cstddef>
#include <string_view>
namespace nexverse::assets
{
// Payload-based format recognition is required because some OpenSim
// GetTexture endpoints serve a codestream even for format=png.
// This is only a gate for the existing size-limited OpenJPEG decoder,
// never proof that the image itself is valid.
[[nodiscard]] constexpr bool is_native_jpeg2000_payload(
    std::string_view bytes) noexcept
{
    const bool codestream=bytes.size()>=2U &&
        static_cast<unsigned char>(bytes[0])==0xffU &&
        static_cast<unsigned char>(bytes[1])==0x4fU;
    constexpr std::string_view signature{
        "\x00\x00\x00\x0c\x6a\x50\x20\x20\x0d\x0a\x87\x0a",12U};
    const bool boxed_jp2=bytes.size()>=signature.size() &&
        bytes.substr(0U,signature.size())==signature;
    return codestream||boxed_jp2;
}
}
