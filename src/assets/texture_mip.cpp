/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/assets/texture_mip.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <string_view>
#include <unordered_map>

namespace nexverse::assets
{
std::optional<ImageRgba8> texture_mip_image(
    const ImageRgba8& source, std::uint8_t level)
{
    if (!source.valid() || source.width > 4096U ||
        source.height > 4096U || level > 12U)
        return std::nullopt;
    ImageRgba8 current = source;
    for (std::uint8_t step = 0U;
         step < level && (current.width > 1U || current.height > 1U);
         ++step)
    {
        ImageRgba8 next;
        next.width = (current.width + 1U)/2U;
        next.height = (current.height + 1U)/2U;
        next.pixels.resize(static_cast<std::size_t>(next.width) *
                           next.height * 4U);
        for (std::uint32_t y = 0U; y < next.height; ++y)
        {
            for (std::uint32_t x = 0U; x < next.width; ++x)
            {
                std::uint32_t alpha_sum = 0U;
                std::array<std::uint32_t,3> premult{};
                for (std::uint32_t j = 0U; j < 2U; ++j)
                for (std::uint32_t i = 0U; i < 2U; ++i)
                {
                    const auto sx = std::min(2U*x+i,current.width-1U);
                    const auto sy = std::min(2U*y+j,current.height-1U);
                    const auto off = (static_cast<std::size_t>(sy) *
                        current.width + sx)*4U;
                    const std::uint32_t a = current.pixels[off+3U];
                    alpha_sum += a;
                    for (unsigned c = 0U; c < 3U; ++c)
                        premult[c] += current.pixels[off+c]*a;
                }
                const auto dst = (static_cast<std::size_t>(y)*next.width+x)*4U;
                for (unsigned c = 0U; c < 3U; ++c)
                    next.pixels[dst+c] = static_cast<std::uint8_t>(
                        alpha_sum ? (premult[c]+alpha_sum/2U)/alpha_sum : 0U);
                next.pixels[dst+3U] = static_cast<std::uint8_t>(
                    (alpha_sum+2U)/4U);
            }
        }
        current = std::move(next);
    }
    return current;
}
std::size_t gpu_rgba8_mip_bytes(std::uint32_t width,
                                std::uint32_t height) noexcept
{
    if (width==0U || height==0U || width>4096U || height>4096U)
        return 0U;
    std::size_t bytes=0U;
    while (true)
    {
        bytes += static_cast<std::size_t>(width)*height*4U;
        if (width==1U && height==1U) return bytes;
        width = std::max(1U,width/2U);
        height = std::max(1U,height/2U);
    }
}
std::unordered_map<std::string,std::uint8_t> plan_texture_mip_levels(
    const std::vector<TextureDistanceSample>& samples,
    float x,float y,float z,
    const std::unordered_map<std::string,std::uint8_t>& previous,
    const std::unordered_set<std::string>& particle_ids)
{
    std::unordered_map<std::string,double> nearest;
    if (std::isfinite(x) && std::isfinite(y) && std::isfinite(z))
    for (const auto& s:samples)
    {
        if (s.id.empty() || !std::isfinite(s.x) ||
            !std::isfinite(s.y) || !std::isfinite(s.z))
            continue;
        const double dist = std::hypot(
            static_cast<double>(s.x)-x,
            static_cast<double>(s.y)-y,
            static_cast<double>(s.z)-z);
        if (!std::isfinite(dist)) continue;
        auto [it,new_key] = nearest.try_emplace(s.id,dist);
        if (!new_key && dist<it->second) it->second=dist;
    }
    std::unordered_map<std::string,std::uint8_t> levels;
    levels.reserve(nearest.size()+particle_ids.size());
    for (const auto& [id,dist]:nearest)
    {
        // Prefer original detail for nearby streets/building faces.
        // Existing GPU byte budgets and actual driver admission still
        // cap memory use on small as well as large graphics cards.
        std::uint8_t level = dist < 48.0 ? 0U :
                             dist < 96.0 ? 1U :
                             dist < 176.0 ? 2U : 3U;
        if (const auto it = previous.find(id);it != previous.end())
        {
            switch(it->second)
            {
            case 0U: if (dist < 56.0) level=0U;break;
            case 1U: if (dist >= 40.0 && dist < 112.0) level=1U;break;
            case 2U: if (dist >= 82.0 && dist < 200.0) level=2U;break;
            case 3U: if (dist >= 160.0) level=3U;break;
            default: break;
            }
        }
        levels[id] = level;
    }
    for (const auto& id:particle_ids)
        if (!id.empty()) levels[id] = 0U;
    return levels;
}
TextureByteBudgetPlan plan_texture_byte_budget(
    const std::vector<std::string>& prioritized,
    const std::unordered_map<std::string,std::size_t>& resident_bytes,
    std::string_view incoming_id,std::size_t incoming_bytes,
    std::size_t budget)
{
    TextureByteBudgetPlan plan;
    if (incoming_id.empty() || incoming_bytes==0U ||
        incoming_bytes>budget) return plan;
    std::size_t used=0U;
    for(const auto& [id,size]:resident_bytes)
        if (id!=incoming_id)
        {
            if (size>std::numeric_limits<std::size_t>::max()-used)
                used=std::numeric_limits<std::size_t>::max();
            else used+=size;
        }
    if (used<=budget-incoming_bytes)
    {
        plan.fits=true;
        plan.remaining_bytes=budget-(used+incoming_bytes);
        return plan;
    }
    std::unordered_map<std::string,std::size_t> rank;
    for (std::size_t i=0U;i<prioritized.size();++i)
        rank.try_emplace(prioritized[i],i);
    std::vector<std::string> candidates;
    for(const auto& [id,size]:resident_bytes)
        if (id!=incoming_id && size>0U) candidates.push_back(id);
    std::sort(candidates.begin(),candidates.end(),
        [&](const auto& a,const auto& b)
        {
            const auto ai=rank.find(a),bi=rank.find(b);
            const auto ar=ai==rank.end()?prioritized.size():ai->second;
            const auto br=bi==rank.end()?prioritized.size():bi->second;
            if(ar!=br)return ar>br;
            return a<b;
        });
    const auto in = rank.find(std::string{incoming_id});
    const auto incoming_rank = in==rank.end()?prioritized.size():in->second;
    for(const auto& id:candidates)
    {
        const auto it = rank.find(id);
        const auto candidate_rank =
            it==rank.end()?prioritized.size():it->second;
        // Never evict a more important texture to stream a distant one.
        if (candidate_rank<=incoming_rank) continue;
        const auto size=resident_bytes.at(id);
        used=used>=size ? used-size : 0U;
        plan.eviction_ids.push_back(id);
        if(used<=budget-incoming_bytes)
        {
            plan.fits=true;
            plan.remaining_bytes=budget-(used+incoming_bytes);
            return plan;
        }
    }
    return plan;
}
}
