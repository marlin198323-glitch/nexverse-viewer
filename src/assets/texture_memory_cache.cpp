/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/assets/texture_memory_cache.hpp"
#include <algorithm>
#include <iterator>
namespace nexverse::assets
{
namespace {
bool uuid(std::string_view id) noexcept
{
    if(id.size()!=36U)return false;
    for(std::size_t i=0;i<id.size();++i)
    {
        if(i==8U||i==13U||i==18U||i==23U)
        {if(id[i]!='-')return false;}
        else if(!((id[i]>='0'&&id[i]<='9')||
                  (id[i]>='a'&&id[i]<='f')||
                  (id[i]>='A'&&id[i]<='F')))return false;
    }
    return true;
}
std::string canonical(std::string_view id)
{
    std::string key{id};
    std::transform(key.begin(),key.end(),key.begin(),
        [](unsigned char c){return static_cast<char>(
            c>='A' && c<='F' ? c-'A'+'a' : c);});
    return key;
}
}
TextureMemoryCache::TextureMemoryCache(std::size_t budget)
    : budget_(budget) {}
std::optional<ImageRgba8> TextureMemoryCache::load(std::string_view id)
{
    if(!uuid(id))return std::nullopt;
    std::lock_guard guard{mutex_};
    const auto it=entries_.find(canonical(id));
    if(it==entries_.end())return std::nullopt;
    age_.splice(age_.end(),age_,it->second.age);
    return it->second.image;
}
bool TextureMemoryCache::contains(std::string_view id) const
{
    if(!uuid(id))return false;
    std::lock_guard guard{mutex_};
    return entries_.contains(canonical(id));
}
bool TextureMemoryCache::store(std::string_view id,
    const ImageRgba8& image)
{
    if(!uuid(id)||!image.valid()||image.width>4096U||
       image.height>4096U || image.pixels.size()>budget_)
        return false;
    const std::string key=canonical(id);
    const std::size_t size=image.pixels.size();
    std::lock_guard guard{mutex_};
    if(const auto it=entries_.find(key);it!=entries_.end())
    {
        used_-=it->second.bytes;
        age_.erase(it->second.age);
        entries_.erase(it);
    }
    while(used_>budget_-size && !age_.empty())
    {
        const std::string oldest=age_.front();
        used_-=entries_.at(oldest).bytes;
        entries_.erase(oldest);
        age_.pop_front();
    }
    age_.push_back(key);
    entries_.emplace(key,Entry{image,size,std::prev(age_.end())});
    used_+=size;
    return true;
}
std::size_t TextureMemoryCache::bytes() const
{
    std::lock_guard guard{mutex_};
    return used_;
}
std::size_t TextureMemoryCache::count() const
{
    std::lock_guard guard{mutex_};
    return entries_.size();
}
}
