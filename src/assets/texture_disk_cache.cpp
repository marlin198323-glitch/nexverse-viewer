/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/assets/texture_disk_cache.hpp"
#include <zlib.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <charconv>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace nexverse::assets
{
namespace
{
constexpr std::uint64_t max_image_bytes = 64ULL * 1024ULL * 1024ULL;
constexpr std::array<char,8> legacy_magic{'O','G','L','T','E','X','0','1'};
constexpr std::array<char,8> compressed_magic{'O','G','L','T','E','X','0','2'};
bool safe_uuid(std::string_view id)
{
    if (id.size()!=36U) return false;
    for(std::size_t i=0;i<id.size();++i)
    {
        if(i==8U||i==13U||i==18U||i==23U)
        {
            if(id[i]!='-')return false;
        }
        else if(!((id[i]>='0'&&id[i]<='9')||
                  (id[i]>='a'&&id[i]<='f')||
                  (id[i]>='A'&&id[i]<='F')))
            return false;
    }
    return true;
}
std::filesystem::path file_for(const std::filesystem::path& dir,
    std::string_view id)
{
    std::string stem{id};
    std::transform(stem.begin(),stem.end(),stem.begin(),[](unsigned char c)
    {return static_cast<char>(std::tolower(c));});
    return dir / (stem+".rgba");
}
std::uint64_t checksum(const ImageRgba8& image)
{
    std::uint64_t hash=14695981039346656037ULL;
    const auto fold=[&](std::uint8_t v)
    {
        hash^=v; hash*=1099511628211ULL;
    };
    for(int i=0;i<4;++i)
    {
        fold(static_cast<std::uint8_t>(image.width>>(i*8)));
        fold(static_cast<std::uint8_t>(image.height>>(i*8)));
    }
    for(const auto pixel:image.pixels)fold(pixel);
    return hash;
}
template<class T>
void write_le(std::ostream& f,T v)
{
    for(std::size_t i=0;i<sizeof(T);++i)
        f.put(static_cast<char>((v>>(i*8))&0xffU));
}
template<class T>
bool read_le(std::istream& f,T& out)
{
    out=0;
    for(std::size_t i=0;i<sizeof(T);++i)
    {
        const int ch=f.get();
        if(ch==EOF)return false;
        out|=static_cast<T>(static_cast<std::uint64_t>(ch)<<(i*8));
    }
    return true;
}
}
std::uint64_t TextureDiskCache::default_budget_bytes() noexcept
{
    constexpr std::uint64_t mib=1024ULL*1024ULL;
    constexpr std::uint64_t fallback=512ULL*mib;
    const char* raw=std::getenv("OGL_TEXTURE_CACHE_MIB");
    if(raw==nullptr||*raw=='\0')return fallback;
    const std::string_view value{raw};
    if(value.size()>4U)return fallback;
    unsigned size=0U;
    const auto [end,error]=std::from_chars(
        value.data(),value.data()+value.size(),size);
    if(error!=std::errc{}||end!=value.data()+value.size()||
       size<128U||size>2048U)return fallback;
    return static_cast<std::uint64_t>(size)*mib;
}
TextureDiskCache::TextureDiskCache(std::filesystem::path directory,
    std::uint64_t budget_bytes)
    : directory_(std::move(directory)),budget_bytes_(budget_bytes)
{}
std::filesystem::path TextureDiskCache::default_directory()
{
    if (const char* xdg=std::getenv("XDG_CACHE_HOME");
        xdg!=nullptr && *xdg!='\0')
    {
        const std::filesystem::path p{xdg};
        if(p.is_absolute())return p/"opengenesislink"/"textures-v1";
    }
    if(const char* home=std::getenv("HOME");
       home!=nullptr && *home!='\0')
        return std::filesystem::path{home}/".cache"/
            "opengenesislink"/"textures-v1";
    return {};
}
bool TextureDiskCache::contains(std::string_view id) const noexcept
{
    if(!safe_uuid(id)||directory_.empty())return false;
    std::error_code error;
    const auto file=file_for(directory_,id);
    const auto size=std::filesystem::file_size(file,error);
    return !error && size>=24U && size<=max_image_bytes+24U;
}
std::optional<ImageRgba8> TextureDiskCache::load(
    std::string_view id,AssetCacheLookup* status) const
{
    if(status)*status=AssetCacheLookup::miss;
    if(!safe_uuid(id))
    {
        if(status)*status=AssetCacheLookup::corrupt;
        return std::nullopt;
    }
    if(directory_.empty())return std::nullopt;
    try
    {
        const auto path=file_for(directory_,id);
        if(!std::filesystem::exists(path)) return std::nullopt;
        if(status)*status=AssetCacheLookup::corrupt;
        if(!std::filesystem::is_regular_file(path) ||
           std::filesystem::file_size(path)>max_image_bytes+24U)
            return std::nullopt;
        std::ifstream f{path,std::ios::binary};
        std::array<char,8> marker{};
        if(!f.read(marker.data(),static_cast<std::streamsize>(marker.size()))||
           (marker!=legacy_magic&&marker!=compressed_magic))
            return std::nullopt;
        ImageRgba8 image;
        std::uint64_t stored_hash=0U;
        if(!read_le(f,image.width)||!read_le(f,image.height)||
           !read_le(f,stored_hash) || image.width==0U||
           image.height==0U||image.width>4096U||image.height>4096U)
            return std::nullopt;
        const std::uint64_t byte_count =
            static_cast<std::uint64_t>(image.width)*image.height*4U;
        const auto file_bytes=std::filesystem::file_size(path);
        if(byte_count>max_image_bytes||file_bytes<24U||
           file_bytes>24U+max_image_bytes ||
           (marker==legacy_magic&&file_bytes!=24U+byte_count))
            return std::nullopt;
        image.pixels.resize(static_cast<std::size_t>(byte_count));
        if(marker==legacy_magic)
        {
            if(!f.read(reinterpret_cast<char*>(image.pixels.data()),
                       static_cast<std::streamsize>(byte_count)))
                return std::nullopt;
        }
        else
        {
            // Defend against malformed/zip-bomb files: the output is
            // bounded by the independently validated image dimensions
            // (at most 64 MiB), and the entire input must be consumed.
            const auto packed_bytes=file_bytes-24U;
            if(packed_bytes==0U)return std::nullopt;
            std::vector<std::uint8_t> packed(
                static_cast<std::size_t>(packed_bytes));
            if(!f.read(reinterpret_cast<char*>(packed.data()),
                       static_cast<std::streamsize>(packed.size())))
                return std::nullopt;
            z_stream stream{};
            stream.next_in=packed.data();
            stream.avail_in=static_cast<uInt>(packed.size());
            stream.next_out=image.pixels.data();
            stream.avail_out=static_cast<uInt>(image.pixels.size());
            if(inflateInit(&stream)!=Z_OK)return std::nullopt;
            const int result=inflate(&stream,Z_FINISH);
            const bool complete=result==Z_STREAM_END&&
                stream.total_out==byte_count&&
                stream.total_in==packed_bytes;
            inflateEnd(&stream);
            if(!complete)return std::nullopt;
        }
        if(!image.valid() || checksum(image)!=stored_hash)
            return std::nullopt;
        // Repack validated legacy RGBA lazily when used. This migrates
        // existing 256 MiB caches without losing assets on upgrade.
        // Save errors do not invalidate a successfully decoded image.
        if(marker==legacy_magic&&byte_count>=4096U)
        {
            f.close();
            (void)store(id,image);
        }
        // Using last-write-time as access time keeps eviction portable.
        std::error_code error;
        std::filesystem::last_write_time(path,
            std::filesystem::file_time_type::clock::now(),error);
        if(status)*status=AssetCacheLookup::hit;
        return image;
    }
    catch(const std::filesystem::filesystem_error&)
    {
        if(status)*status=AssetCacheLookup::io_error;
        return std::nullopt;
    }
    catch(const std::ios_base::failure&)
    {
        if(status)*status=AssetCacheLookup::io_error;
        return std::nullopt;
    }
}
bool TextureDiskCache::store(std::string_view id,const ImageRgba8& image) const
{
    if(!safe_uuid(id)||directory_.empty()||!image.valid() ||
       image.width>4096U||image.height>4096U||
       image.pixels.size()>max_image_bytes || budget_bytes_<image.pixels.size()+24U)
        return false;
    try
    {
        std::filesystem::create_directories(directory_);
        const auto dest=file_for(directory_,id);
        // zlib is already a portable dependency of NexVerse::Assets.
        // Lossless compression preserves every RGBA pixel; on noisy
        // incompressible images retain the legacy raw format.
        std::vector<std::uint8_t> packed;
        bool compressible=false;
        if(image.pixels.size()>=4096U)
        {
            const auto bound=compressBound(
                static_cast<uLong>(image.pixels.size()));
            packed.resize(static_cast<std::size_t>(bound));
            uLongf size=bound;
            if(compress2(packed.data(),&size,image.pixels.data(),
                         static_cast<uLong>(image.pixels.size()),
                         Z_BEST_SPEED)==Z_OK &&
               size<image.pixels.size())
            {
                packed.resize(static_cast<std::size_t>(size));
                compressible=true;
            }
        }
        // Temp is per-call; the sequential GetTexture pipeline owns writes.
        auto temp=dest;
        temp+=".part";
        {
            std::ofstream f{temp,std::ios::binary|std::ios::trunc};
            if(!f)return false;
            const auto& marker=compressible?compressed_magic:legacy_magic;
            f.write(marker.data(),static_cast<std::streamsize>(marker.size()));
            write_le<std::uint32_t>(f,image.width);
            write_le<std::uint32_t>(f,image.height);
            write_le<std::uint64_t>(f,checksum(image));
            if(compressible)
                f.write(reinterpret_cast<const char*>(packed.data()),
                        static_cast<std::streamsize>(packed.size()));
            else
                f.write(reinterpret_cast<const char*>(image.pixels.data()),
                        static_cast<std::streamsize>(image.pixels.size()));
            f.flush();
            if(!f)return false;
        }
        std::filesystem::rename(temp,dest);
        (void)prune();
        return true;
    }
    catch(const std::filesystem::filesystem_error&)
    {return false;}
    catch(const std::ios_base::failure&)
    {return false;}
}
std::size_t TextureDiskCache::prune() const
{
    if(directory_.empty())return 0U;
    struct Entry
    {
        std::filesystem::path path;
        std::filesystem::file_time_type age;
        std::uint64_t size=0U;
    };
    std::vector<Entry> entries;
    std::uint64_t sum=0U;
    try
    {
        if(!std::filesystem::exists(directory_))return 0U;
        for(const auto& item:std::filesystem::directory_iterator(directory_))
        {
            if(!item.is_regular_file()||
               item.path().extension()!=".rgba")continue;
            const auto n=item.file_size();
            entries.push_back({item.path(),item.last_write_time(),n});
            sum+=n;
        }
        if(sum<=budget_bytes_)return 0U;
        std::sort(entries.begin(),entries.end(),[](const auto& a,const auto& b)
        {return a.age!=b.age?a.age<b.age:a.path<b.path;});
        std::size_t removed=0U;
        for(const auto& item:entries)
        {
            if(sum<=budget_bytes_)break;
            std::filesystem::remove(item.path);
            sum-=item.size;
            ++removed;
        }
        return removed;
    }
    catch(const std::filesystem::filesystem_error&)
    {return 0U;}
}
}
