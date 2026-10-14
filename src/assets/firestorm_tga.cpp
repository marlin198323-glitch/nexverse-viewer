/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_tga.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <system_error>
namespace nexverse::assets
{
FirestormTgaResult decode_firestorm_tga(
    std::span<const std::uint8_t> bytes)
{
    const auto fail=[](const char* reason)->FirestormTgaResult {
        return {{},reason};
    };
    if(bytes.size()<18U||bytes.size()>16U*1024U*1024U)
        return fail("TGA size invalid");
    const auto color_map=bytes[1U];
    const auto type=bytes[2U];
    const auto pixel_bits=bytes[16U];
    const auto origin=bytes[17U];
    if(color_map!=0U||(type!=2U&&type!=3U&&type!=10U&&type!=11U)||
       (type==3U||type==11U?pixel_bits!=8U:
           (pixel_bits!=24U&&pixel_bits!=32U))||
       (origin&0x10U)!=0U)
        return fail("Unsupported Firestorm TGA format");
    const auto width=static_cast<unsigned>(bytes[12U])+
        (static_cast<unsigned>(bytes[13U])<<8U);
    const auto height=static_cast<unsigned>(bytes[14U])+
        (static_cast<unsigned>(bytes[15U])<<8U);
    if(width==0U||height==0U||width>2048U||height>2048U||
       width*height>4194304U)
        return fail("TGA dimensions outside limit");
    const auto stride=static_cast<std::size_t>(pixel_bits/8U);
    std::size_t index=18U+bytes[0U];
    if(index>bytes.size())return fail("Invalid TGA image ID");
    const auto pixels=static_cast<std::size_t>(width)*height;
    ImageRgba8 result{
        width,height,std::vector<std::uint8_t>(pixels*4U,0U)};
    std::size_t written=0U;
    const auto read_pixel=[&](std::array<std::uint8_t,4>& px)->bool{
        if(index+stride>bytes.size())return false;
        if(stride==1U)
            px={bytes[index],bytes[index],bytes[index],255U};
        else
            px={bytes[index+2U],bytes[index+1U],bytes[index],
                stride==4U?bytes[index+3U]:static_cast<std::uint8_t>(255U)};
        index+=stride;
        return true;
    };
    const auto put=[&](const std::array<std::uint8_t,4>& px)->bool{
        if(written>=pixels)return false;
        const auto col=written%width,row=written/width;
        const auto dest_row=(origin&0x20U)!=0U?row:height-1U-row;
        const auto offset=(dest_row*width+col)*4U;
        std::copy(px.begin(),px.end(),result.pixels.begin()+
                  static_cast<std::ptrdiff_t>(offset));
        ++written;
        return true;
    };
    if(type==2U||type==3U)
    {
        while(written<pixels)
        {
            std::array<std::uint8_t,4> px{};
            if(!read_pixel(px)||!put(px))return fail("Truncated raw TGA");
        }
    }
    else while(written<pixels)
    {
        if(index>=bytes.size())return fail("Truncated RLE TGA packet");
        const auto packet=bytes[index++];
        const auto count=static_cast<std::size_t>(packet&0x7fU)+1U;
        if(count>pixels-written)return fail("RLE packet exceeds TGA image");
        if((packet&0x80U)!=0U)
        {
            std::array<std::uint8_t,4> px{};
            if(!read_pixel(px))return fail("RLE pixel truncated");
            for(std::size_t j=0;j<count;++j)(void)put(px);
        }
        else for(std::size_t j=0;j<count;++j)
        {
            std::array<std::uint8_t,4> px{};
            if(!read_pixel(px)||!put(px))
                return fail("Raw RLE pixels truncated");
        }
    }
    return {std::move(result),{}};
}
FirestormTgaResult load_firestorm_character_tga(
    const std::filesystem::path& root,const std::string& name)
{
    if(name.empty()||name.size()>120U||
       name.find_first_not_of(
           "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
           "0123456789_-." )!=std::string::npos||
       name=="."||name==".."||
       name.find("..")!=std::string::npos||
       !name.ends_with(".tga"))
        return {{}, "Invalid local Firestorm TGA filename"};
    const auto path=root/name;
    std::error_code ec;
    const auto bytes=std::filesystem::file_size(path,ec);
    if(ec||bytes<18U||bytes>16U*1024U*1024U)
        return {{}, "Firestorm TGA absent/oversized"};
    std::ifstream stream{path,std::ios::binary};
    if(!stream)return {{}, "Firestorm TGA not readable"};
    std::vector<std::uint8_t> data{
        std::istreambuf_iterator<char>{stream},
        std::istreambuf_iterator<char>{}};
    return decode_firestorm_tga(data);
}
}
