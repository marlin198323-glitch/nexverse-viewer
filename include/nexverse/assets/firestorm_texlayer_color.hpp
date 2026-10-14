/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_texlayer_manifest.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <optional>
namespace nexverse::assets
{
// Firestorm_7.2.4 LLTexLayer::calculateTexLayerColor and
// LLTexLayerParamColor::getNetColor port, restricted to known REAL worn
// visual-parameter values. Skin/hair/eye global colors need independently
// resolved avatar global-color params; we do not invent them.
[[nodiscard]] inline std::optional<std::array<float,4>>
evaluate_firestorm_local_layer_tint(
    const FirestormTexLayer& layer,
    const std::map<int,float>& worn_weights,
    const std::map<std::string,std::array<float,4>>& global_colors={})
{
    std::array<float,4> color{};
    if(layer.has_global_color||!layer.global_color_name.empty())
    {
        const auto found=global_colors.find(layer.global_color_name);
        if(found==global_colors.end())return std::nullopt;
        color=found->second;
        for(const auto component:color)
            if(!std::isfinite(component)||component<0.0f||component>1.0f)
                return std::nullopt;
    }
    else if(layer.fixed_rgba)
        for(std::size_t i=0U;i<4U;++i)
            color[i]=static_cast<float>((*layer.fixed_rgba)[i])/255.0f;
    else if(layer.color_params.empty())
        color.fill(1.0f);
    // When color parameters exist without fixed or global color,
    // Firestorm findNetColor initializes the accumulator to ZERO.
    for(const auto& param:layer.color_params)
    {
        const auto it=worn_weights.find(param.id);
        if(it==worn_weights.end()||param.palette.empty()||
           !std::isfinite(it->second)||param.minimum>=param.maximum||
           it->second<param.minimum-0.00001f||
           it->second>param.maximum+0.00001f)
            return std::nullopt;
        const auto normalized=std::clamp(
            (it->second-param.minimum)/(param.maximum-param.minimum),
            0.0f,1.0f);
        const float position=normalized*
            static_cast<float>(param.palette.size()-1U);
        const auto start=std::min(
            static_cast<std::size_t>(position),
            param.palette.size()-1U);
        const auto end=std::min(start+1U,param.palette.size()-1U);
        const auto weight=position-static_cast<float>(start);
        for(std::size_t i=0U;i<4U;++i)
        {
            const auto c0=static_cast<float>(param.palette[start][i])/255.0f;
            const auto c1=static_cast<float>(param.palette[end][i])/255.0f;
            const auto sample=c0*(1.0f-weight)+c1*weight;
            switch(param.operation)
            {
                case FirestormColorOperation::add:
                    color[i]+=sample;
                    break;
                case FirestormColorOperation::multiply:
                    color[i]*=sample;
                    break;
                case FirestormColorOperation::blend:
                    color[i]=color[i]*(1.0f-normalized)+
                        sample*normalized;
                    break;
            }
        }
    }
    for(auto& component:color)
    {
        if(!std::isfinite(component))return std::nullopt;
        component=std::clamp(component,0.0f,1.0f);
    }
    return color;
}
// Only authenticated original LLWearables of the matching Firestorm
// wearable type may supply skin/hair/eye visual parameters. Missing or
// conflicting owner-specific values keep the global color unresolved.
[[nodiscard]] inline std::map<std::string,std::array<float,4>>
evaluate_firestorm_global_colors(
    const FirestormTexLayerManifest& manifest,
    const std::map<int,std::map<int,float>>& by_wearable_type)
{
    std::map<std::string,std::array<float,4>> colors;
    if(!manifest.valid())return colors;
    for(const auto& [name,global]:manifest.global_colors)
    {
        const auto weights=by_wearable_type.find(global.wearable_type);
        if(weights==by_wearable_type.end())continue;
        FirestormTexLayer definition;
        definition.color_params=global.color_params;
        const auto color=evaluate_firestorm_local_layer_tint(
            definition,weights->second);
        if(color)colors.emplace(name,*color);
    }
    return colors;
}
}
