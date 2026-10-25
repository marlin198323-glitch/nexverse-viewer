/* This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0.
 */
#include "nexverse/assets/jpeg2000_payload_signature.hpp"
#include "jpeg2000_fixture.hpp"
#include <iostream>
#include <string_view>
int main()
{
    using nexverse::assets::is_native_jpeg2000_payload;
    if(!is_native_jpeg2000_payload(std::string_view{
           reinterpret_cast<const char*>(j2c_fixture),
           sizeof(j2c_fixture)}) ||
       !is_native_jpeg2000_payload(std::string_view{
           reinterpret_cast<const char*>(jp2_fixture),
           sizeof(jp2_fixture)}) ||
       is_native_jpeg2000_payload("") ||
       is_native_jpeg2000_payload("not PNG nor JPEG2000") ||
       is_native_jpeg2000_payload(std::string_view{
           "\x89PNG\r\n\x1a\n",8}) ||
       is_native_jpeg2000_payload(std::string_view{
           "\x00\x00\x00\x0c\x6a\x50",6}))
        return 1;
    // A partial J2C SOC is only a tentative MIME match. The caller
    // must still run OpenJPEG, then fall back on decode failure.
    if(!is_native_jpeg2000_payload(std::string_view{"\xff\x4f",2}))
        return 2;
    std::cout<<"OGL_JPEG2000_PAYLOAD_SIGNATURE_PASS\n";
}
