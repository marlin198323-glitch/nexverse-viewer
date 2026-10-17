/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/render/renderer.hpp"
#include "nexverse/render/scene_forward_visibility.hpp"
#include "nexverse/render/avatar_body_uv_evidence.hpp"
#include "nexverse/render/avatar_skin_weight_pair.hpp"
#include "nexverse/render/avatar_attachment_anchor.hpp"
#include "nexverse/render/terrain_mesh.hpp"
#include "nexverse/render/terrain_texture_readiness.hpp"
#include "nexverse/render/material_pass.hpp"
#include "nexverse/render/particle_motion.hpp"
#include "nexverse/render/particle_emission.hpp"
#include "nexverse/render/ribbon_geometry.hpp"

#include "nexverse/core/logger.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <numbers>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace
{
using nexverse::render::RenderCamera;
using nexverse::render::RenderPrimitiveKind;
using nexverse::render::RenderProfileKind;
using nexverse::render::RenderSceneObject;
using nexverse::render::RenderVec3;

constexpr GLenum gl_gpu_memory_info_total_available_memory_nvx = 0x9048;
constexpr GLenum gl_gpu_memory_info_current_available_vidmem_nvx = 0x9049;

struct Mat4
{
    std::array<float, 16> value{};
};

float dot(RenderVec3 a, RenderVec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

RenderVec3 cross(RenderVec3 a, RenderVec3 b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

RenderVec3 normalize(RenderVec3 v)
{
    const float length = std::sqrt(dot(v, v));
    if (length <= 0.000001f)
    {
        return {};
    }
    return {v.x / length, v.y / length, v.z / length};
}

RenderVec3 add(RenderVec3 a, RenderVec3 b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

RenderVec3 scale(RenderVec3 v, float amount)
{
    return {v.x * amount, v.y * amount, v.z * amount};
}

Mat4 identity()
{
    Mat4 result;
    result.value[0] = 1.0f;
    result.value[5] = 1.0f;
    result.value[10] = 1.0f;
    result.value[15] = 1.0f;
    return result;
}

Mat4 multiply(const Mat4& a, const Mat4& b)
{
    Mat4 result;
    for (std::size_t column = 0U; column < 4U; ++column)
    {
        for (std::size_t row = 0U; row < 4U; ++row)
        {
            float sum = 0.0f;
            for (std::size_t k = 0U; k < 4U; ++k)
            {
                sum +=
                    a.value[k * 4U + row] *
                    b.value[column * 4U + k];
            }
            result.value[column * 4U + row] = sum;
        }
    }
    return result;
}

Mat4 translation(RenderVec3 position)
{
    Mat4 result = identity();
    result.value[12] = position.x;
    result.value[13] = position.y;
    result.value[14] = position.z;
    return result;
}

Mat4 scaling(RenderVec3 value)
{
    Mat4 result = identity();
    result.value[0] = value.x;
    result.value[5] = value.y;
    result.value[10] = value.z;
    return result;
}

Mat4 quaternion_rotation(
    nexverse::render::RenderQuaternion q)
{
    const float length = std::sqrt(
        q.x * q.x +
        q.y * q.y +
        q.z * q.z +
        q.w * q.w);

    if (length <= 0.000001f)
    {
        return identity();
    }

    q.x /= length;
    q.y /= length;
    q.z /= length;
    q.w /= length;

    const float xx = q.x * q.x;
    const float yy = q.y * q.y;
    const float zz = q.z * q.z;
    const float xy = q.x * q.y;
    const float xz = q.x * q.z;
    const float yz = q.y * q.z;
    const float wx = q.w * q.x;
    const float wy = q.w * q.y;
    const float wz = q.w * q.z;

    Mat4 result = identity();
    result.value[0] = 1.0f - 2.0f * (yy + zz);
    result.value[1] = 2.0f * (xy + wz);
    result.value[2] = 2.0f * (xz - wy);

    result.value[4] = 2.0f * (xy - wz);
    result.value[5] = 1.0f - 2.0f * (xx + zz);
    result.value[6] = 2.0f * (yz + wx);

    result.value[8] = 2.0f * (xz + wy);
    result.value[9] = 2.0f * (yz - wx);
    result.value[10] = 1.0f - 2.0f * (xx + yy);
    return result;
}

Mat4 rotation_z(float angle);

Mat4 rotation_y(float angle)
{
    Mat4 result=identity();
    const float c=std::cos(angle),si=std::sin(angle);
    result.value[0]=c;result.value[2]=-si;
    result.value[8]=si;result.value[10]=c;
    return result;
}

// Render a modest, deterministic locomotion pose with real joint weights.
// This is procedural visualization, not playback of server-supplied
// LLAvatarAnimation assets. WorldModel and movement physics are untouched.
std::vector<Mat4> avatar_joint_deformations(
    const nexverse::render::RenderAvatarBindRig& rig,
    nexverse::protocol::AvatarMotion motion,double seconds,
    const nexverse::assets::FirestormKeyframeMotion* keyframe=nullptr)
{
    using nexverse::protocol::AvatarMotion;
    std::vector<Mat4> transforms;
    if(rig.joints.empty()||rig.joints.size()>256U)return transforms;
    transforms.reserve(rig.joints.size());
    const float clip_time=keyframe
        ?nexverse::assets::firestorm_clip_time(
            *keyframe,static_cast<float>(seconds)):0.0f;
    const bool running=motion==AvatarMotion::run;
    const bool walking=motion==AvatarMotion::walk||running;
    const float rate=running?11.0f:7.5f;
    const float step=std::sin(static_cast<float>(
        std::isfinite(seconds)?std::fmod(seconds*rate,100000.0):0.0));
    const float stride=running?0.65f:0.36f;
    for(const auto& joint:rig.joints)
    {
        float yaw=0.0f,pitch=0.0f;
        if(walking)
        {
            if(joint.name=="mHipLeft")pitch=stride*step;
            else if(joint.name=="mHipRight")pitch=-stride*step;
            else if(joint.name=="mKneeLeft")
                pitch=0.48f*std::max(0.0f,-step);
            else if(joint.name=="mKneeRight")
                pitch=0.48f*std::max(0.0f,step);
            else if(joint.name=="mShoulderLeft")pitch=-stride*0.84f*step;
            else if(joint.name=="mShoulderRight")pitch=stride*0.84f*step;
            else if(joint.name=="mElbowLeft")pitch=-0.11f;
            else if(joint.name=="mElbowRight")pitch=0.11f;
            else if(joint.name=="mChest")yaw=0.025f*step;
        }
        else if(motion==AvatarMotion::sit)
        {
            if(joint.name=="mHipLeft"||joint.name=="mHipRight")pitch=1.15f;
            else if(joint.name=="mKneeLeft"||
                    joint.name=="mKneeRight")pitch=-1.15f;
            else if(joint.name=="mChest")pitch=-0.08f;
        }
        else if(motion==AvatarMotion::jump)
        {
            if(joint.name=="mHipLeft"||joint.name=="mHipRight")pitch=0.32f;
            else if(joint.name=="mKneeLeft"||
                    joint.name=="mKneeRight")pitch=-0.48f;
            else if(joint.name=="mShoulderLeft"||
                    joint.name=="mShoulderRight")pitch=-0.38f;
        }
        else if(motion==AvatarMotion::fly||motion==AvatarMotion::hover)
        {
            if(joint.name=="mPelvis"&&motion==AvatarMotion::fly)
                pitch=-0.25f;
            else if(joint.name=="mShoulderLeft"||
                    joint.name=="mShoulderRight")pitch=-0.20f;
            else if(joint.name=="mHipLeft"||joint.name=="mHipRight")
                pitch=-0.10f;
        }
        // Actual LLKeyframeMotion has priority over the procedural
        // fallback for bones that carry real sampled curves.
        Mat4 rotation=multiply(rotation_y(pitch),rotation_z(yaw));
        RenderVec3 local_offset=joint.local;
        if(keyframe)
        {
            for(const auto& track:keyframe->joints)
            {
                if(track.name!=joint.name)continue;
                if(!track.rotations.empty())
                {
                    const auto q=nexverse::assets::sample_firestorm_rotation(
                        track,clip_time);
                    rotation=quaternion_rotation({q.x,q.y,q.z,q.w});
                }
                if(!track.positions.empty()&&joint.name=="mPelvis")
                {
                    const auto p=nexverse::assets::sample_firestorm_position(
                        track,clip_time);
                    local_offset=add(local_offset,{p.x,p.y,p.z});
                }
                break;
            }
        }
        const Mat4 local=multiply(translation(local_offset),rotation);
        const Mat4 world=joint.parent>=0 &&
            static_cast<std::size_t>(joint.parent)<transforms.size()
            ? multiply(transforms[static_cast<std::size_t>(joint.parent)],
                       local):local;
        transforms.push_back(world);
    }
    // Bind-pose translation is removed in the GPU palette upload.
    // This is a recognizable pose until real LLKeyframeMotion assets load.
    return transforms;
}

Mat4 rotation_z(float angle)
{
    Mat4 result = identity();
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    result.value[0] = c;
    result.value[1] = s;
    result.value[4] = -s;
    result.value[5] = c;
    return result;
}

Mat4 rotation_x(float angle)
{
    Mat4 result = identity();
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    result.value[5] = c;
    result.value[6] = s;
    result.value[9] = -s;
    result.value[10] = c;
    return result;
}

Mat4 perspective(
    float vertical_fov_degrees,
    float aspect,
    float near_clip,
    float far_clip)
{
    const float safe_aspect = std::max(aspect, 0.001f);
    const float safe_near = std::max(near_clip, 0.001f);
    const float safe_far = std::max(far_clip, safe_near + 0.001f);
    const float fov =
        std::clamp(vertical_fov_degrees, 20.0f, 120.0f) *
        std::numbers::pi_v<float> / 180.0f;
    const float f = 1.0f / std::tan(fov * 0.5f);

    Mat4 result;
    result.value[0] = f / safe_aspect;
    result.value[5] = f;
    result.value[10] =
        (safe_far + safe_near) / (safe_near - safe_far);
    result.value[11] = -1.0f;
    result.value[14] =
        (2.0f * safe_far * safe_near) / (safe_near - safe_far);
    return result;
}

Mat4 view_matrix(const RenderCamera& camera)
{
    RenderVec3 forward = normalize(camera.forward);
    RenderVec3 up = normalize(camera.up);
    if (dot(forward, forward) <= 0.0f)
    {
        forward = {1.0f, 0.0f, 0.0f};
    }
    if (dot(up, up) <= 0.0f)
    {
        up = {0.0f, 0.0f, 1.0f};
    }

    RenderVec3 right = normalize(cross(forward, up));
    if (dot(right, right) <= 0.0f)
    {
        right = {0.0f, -1.0f, 0.0f};
    }
    up = normalize(cross(right, forward));

    Mat4 result = identity();
    result.value[0] = right.x;
    result.value[1] = up.x;
    result.value[2] = -forward.x;

    result.value[4] = right.y;
    result.value[5] = up.y;
    result.value[6] = -forward.y;

    result.value[8] = right.z;
    result.value[9] = up.z;
    result.value[10] = -forward.z;

    result.value[12] = -dot(right, camera.position);
    result.value[13] = -dot(up, camera.position);
    result.value[14] = dot(forward, camera.position);
    return result;
}

struct MeshFaceRange
{
    std::size_t face_index = 0U;
    std::uint32_t first_index = 0U;
    std::uint32_t index_count = 0U;
};

struct MeshData
{
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<MeshFaceRange> face_ranges;
};

struct GpuMesh
{
    GLuint vao = 0U;
    GLuint vbo = 0U;
    GLuint ebo = 0U;
    GLuint skin_vbo=0U;
    std::vector<std::uint16_t> skin_palette;
    GLsizei index_count = 0;
    std::vector<MeshFaceRange> face_ranges;
};

constexpr std::size_t prim_vertex_stride = 8U;

std::uint32_t append_vertex(
    MeshData& mesh,
    float x,
    float y,
    float z,
    float u = 0.0f,
    float v = 0.0f)
{
    const std::uint32_t index =
        static_cast<std::uint32_t>(
            mesh.vertices.size() / prim_vertex_stride);
    mesh.vertices.insert(
        mesh.vertices.end(),
        {x, y, z, 0.65f, 0.74f, 0.88f, u, v});
    return index;
}

std::uint32_t append_quad_vertices(
    MeshData& mesh,
    std::array<float, 5> a,
    std::array<float, 5> b,
    std::array<float, 5> c,
    std::array<float, 5> d)
{
    const std::uint32_t first =
        append_vertex(mesh, a[0], a[1], a[2], a[3], a[4]);
    append_vertex(mesh, b[0], b[1], b[2], b[3], b[4]);
    append_vertex(mesh, c[0], c[1], c[2], c[3], c[4]);
    append_vertex(mesh, d[0], d[1], d[2], d[3], d[4]);
    mesh.indices.insert(
        mesh.indices.end(),
        {first, first + 1U, first + 2U,
         first, first + 2U, first + 3U});
    return first;
}

template <typename AppendIndices>
void append_face_range(
    MeshData& mesh,
    std::size_t face_index,
    AppendIndices&& append_indices)
{
    const std::size_t first = mesh.indices.size();
    append_indices();
    const std::size_t count = mesh.indices.size() - first;
    if (count == 0U)
    {
        return;
    }

    mesh.face_ranges.push_back({
        face_index,
        static_cast<std::uint32_t>(first),
        static_cast<std::uint32_t>(count)});
}

MeshData make_box_mesh()
{
    MeshData mesh;

    const auto add_face =
        [&](std::size_t face,
            std::array<float, 3> a,
            std::array<float, 3> b,
            std::array<float, 3> c,
            std::array<float, 3> d)
        {
            append_face_range(mesh, face, [&]
            {
                append_quad_vertices(
                    mesh,
                    {a[0], a[1], a[2], 0.0f, 0.0f},
                    {b[0], b[1], b[2], 1.0f, 0.0f},
                    {c[0], c[1], c[2], 1.0f, 1.0f},
                    {d[0], d[1], d[2], 0.0f, 1.0f});
            });
        };

    add_face(0U,
        {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f});
    add_face(1U,
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f});
    add_face(2U,
        {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f});
    add_face(3U,
        {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f});
    add_face(4U,
        {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f});
    add_face(5U,
        {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f});

    return mesh;
}

MeshData make_cylinder_mesh(std::uint32_t segments = 24U)
{
    MeshData mesh;
    segments = std::max<std::uint32_t>(segments, 3U);

    append_face_range(mesh, 0U, [&]
    {
        const auto center =
            append_vertex(mesh, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f);
        for (std::uint32_t i = 0U; i < segments; ++i)
        {
            const std::uint32_t next = (i + 1U) % segments;
            const float a0 =
                2.0f * std::numbers::pi_v<float> *
                static_cast<float>(i) / static_cast<float>(segments);
            const float a1 =
                2.0f * std::numbers::pi_v<float> *
                static_cast<float>(next) / static_cast<float>(segments);
            const float x0 = 0.5f * std::cos(a0);
            const float y0 = 0.5f * std::sin(a0);
            const float x1 = 0.5f * std::cos(a1);
            const float y1 = 0.5f * std::sin(a1);
            const auto v0 =
                append_vertex(mesh, x0, y0, 0.5f, x0 + 0.5f, y0 + 0.5f);
            const auto v1 =
                append_vertex(mesh, x1, y1, 0.5f, x1 + 0.5f, y1 + 0.5f);
            mesh.indices.insert(mesh.indices.end(), {center, v0, v1});
        }
    });

    append_face_range(mesh, 1U, [&]
    {
        for (std::uint32_t i = 0U; i < segments; ++i)
        {
            const float u0 =
                static_cast<float>(i) / static_cast<float>(segments);
            const float u1 =
                static_cast<float>(i + 1U) / static_cast<float>(segments);
            const float a0 = 2.0f * std::numbers::pi_v<float> * u0;
            const float a1 = 2.0f * std::numbers::pi_v<float> * u1;
            const float x0 = 0.5f * std::cos(a0);
            const float y0 = 0.5f * std::sin(a0);
            const float x1 = 0.5f * std::cos(a1);
            const float y1 = 0.5f * std::sin(a1);
            append_quad_vertices(
                mesh,
                {x0, y0, -0.5f, u0, 0.0f},
                {x1, y1, -0.5f, u1, 0.0f},
                {x1, y1, 0.5f, u1, 1.0f},
                {x0, y0, 0.5f, u0, 1.0f});
        }
    });

    append_face_range(mesh, 2U, [&]
    {
        const auto center =
            append_vertex(mesh, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f);
        for (std::uint32_t i = 0U; i < segments; ++i)
        {
            const std::uint32_t next = (i + 1U) % segments;
            const float a0 =
                2.0f * std::numbers::pi_v<float> *
                static_cast<float>(i) / static_cast<float>(segments);
            const float a1 =
                2.0f * std::numbers::pi_v<float> *
                static_cast<float>(next) / static_cast<float>(segments);
            const float x0 = 0.5f * std::cos(a0);
            const float y0 = 0.5f * std::sin(a0);
            const float x1 = 0.5f * std::cos(a1);
            const float y1 = 0.5f * std::sin(a1);
            const auto v0 =
                append_vertex(mesh, x0, y0, -0.5f, x0 + 0.5f, y0 + 0.5f);
            const auto v1 =
                append_vertex(mesh, x1, y1, -0.5f, x1 + 0.5f, y1 + 0.5f);
            mesh.indices.insert(mesh.indices.end(), {center, v1, v0});
        }
    });

    return mesh;
}

MeshData make_prism_mesh()
{
    MeshData mesh;
    constexpr float y_min = -0.577350269f;
    constexpr float y_span = 0.866025404f;

    const std::array<std::array<float, 3>, 3> bottom{{
        {{0.0f, y_min, -0.5f}},
        {{0.5f, 0.288675135f, -0.5f}},
        {{-0.5f, 0.288675135f, -0.5f}}
    }};
    const std::array<std::array<float, 3>, 3> top{{
        {{0.0f, y_min, 0.5f}},
        {{0.5f, 0.288675135f, 0.5f}},
        {{-0.5f, 0.288675135f, 0.5f}}
    }};

    append_face_range(mesh, 0U, [&]
    {
        std::array<std::uint32_t, 3> idx{};
        for (std::size_t i = 0U; i < 3U; ++i)
        {
            idx[i] = append_vertex(
                mesh,
                top[i][0], top[i][1], top[i][2],
                top[i][0] + 0.5f,
                (top[i][1] - y_min) / y_span);
        }
        mesh.indices.insert(mesh.indices.end(), {idx[0], idx[1], idx[2]});
    });

    for (std::size_t side = 0U; side < 3U; ++side)
    {
        const std::size_t next = (side + 1U) % 3U;
        append_face_range(mesh, 1U + side, [&]
        {
            append_quad_vertices(
                mesh,
                {bottom[side][0], bottom[side][1], -0.5f, 0.0f, 0.0f},
                {bottom[next][0], bottom[next][1], -0.5f, 1.0f, 0.0f},
                {top[next][0], top[next][1], 0.5f, 1.0f, 1.0f},
                {top[side][0], top[side][1], 0.5f, 0.0f, 1.0f});
        });
    }

    append_face_range(mesh, 4U, [&]
    {
        std::array<std::uint32_t, 3> idx{};
        for (std::size_t i = 0U; i < 3U; ++i)
        {
            idx[i] = append_vertex(
                mesh,
                bottom[i][0], bottom[i][1], bottom[i][2],
                bottom[i][0] + 0.5f,
                (bottom[i][1] - y_min) / y_span);
        }
        mesh.indices.insert(mesh.indices.end(), {idx[0], idx[2], idx[1]});
    });

    return mesh;
}

MeshData make_sphere_mesh(
    std::uint32_t latitude_segments = 12U,
    std::uint32_t longitude_segments = 24U)
{
    MeshData mesh;
    latitude_segments =
        std::max<std::uint32_t>(latitude_segments, 3U);
    longitude_segments =
        std::max<std::uint32_t>(longitude_segments, 6U);

    for (std::uint32_t lat = 0U;
         lat <= latitude_segments;
         ++lat)
    {
        const float v =
            static_cast<float>(lat) /
            static_cast<float>(latitude_segments);
        const float phi =
            std::numbers::pi_v<float> * v;
        const float z = 0.5f * std::cos(phi);
        const float radius = 0.5f * std::sin(phi);

        for (std::uint32_t lon = 0U;
             lon <= longitude_segments;
             ++lon)
        {
            const float u =
                static_cast<float>(lon) /
                static_cast<float>(longitude_segments);
            const float theta =
                (2.0f * std::numbers::pi_v<float>) * u;
            append_vertex(
                mesh,
                radius * std::cos(theta),
                radius * std::sin(theta),
                z,
                u,
                1.0f - v);
        }
    }

    const std::uint32_t row =
        longitude_segments + 1U;

    for (std::uint32_t lat = 0U;
         lat < latitude_segments;
         ++lat)
    {
        for (std::uint32_t lon = 0U;
             lon < longitude_segments;
             ++lon)
        {
            const std::uint32_t a = lat * row + lon;
            const std::uint32_t b = a + row;

            mesh.indices.insert(
                mesh.indices.end(),
                {a, b, a + 1U, a + 1U, b, b + 1U});
        }
    }

    return mesh;
}

MeshData make_torus_mesh(
    std::uint32_t major_segments = 24U,
    std::uint32_t minor_segments = 12U)
{
    MeshData mesh;
    major_segments =
        std::max<std::uint32_t>(major_segments, 8U);
    minor_segments =
        std::max<std::uint32_t>(minor_segments, 6U);

    constexpr float major_radius = 0.34f;
    constexpr float minor_radius = 0.16f;

    for (std::uint32_t major = 0U;
         major <= major_segments;
         ++major)
    {
        const float u =
            static_cast<float>(major) /
            static_cast<float>(major_segments);
        const float major_angle =
            (2.0f * std::numbers::pi_v<float>) * u;

        const float major_cos = std::cos(major_angle);
        const float major_sin = std::sin(major_angle);

        for (std::uint32_t minor = 0U;
             minor <= minor_segments;
             ++minor)
        {
            const float v =
                static_cast<float>(minor) /
                static_cast<float>(minor_segments);
            const float minor_angle =
                (2.0f * std::numbers::pi_v<float>) * v;

            const float ring =
                major_radius +
                minor_radius * std::cos(minor_angle);

            append_vertex(
                mesh,
                ring * major_cos,
                ring * major_sin,
                minor_radius * std::sin(minor_angle),
                u,
                v);
        }
    }

    const std::uint32_t row =
        minor_segments + 1U;

    for (std::uint32_t major = 0U;
         major < major_segments;
         ++major)
    {
        for (std::uint32_t minor = 0U;
             minor < minor_segments;
             ++minor)
        {
            const std::uint32_t a =
                major * row + minor;
            const std::uint32_t b =
                a + row;

            mesh.indices.insert(
                mesh.indices.end(),
                {a, b, a + 1U, a + 1U, b, b + 1U});
        }
    }

    return mesh;
}

struct ProfilePoint
{
    float x = 0.0f;
    float y = 0.0f;
};

RenderProfileKind outer_profile_kind(RenderPrimitiveKind kind)
{
    switch (kind)
    {
        case RenderPrimitiveKind::cylinder:
            return RenderProfileKind::circle;
        case RenderPrimitiveKind::prism:
            return RenderProfileKind::triangle;
        case RenderPrimitiveKind::box:
        default:
            return RenderProfileKind::square;
    }
}

ProfilePoint profile_point(RenderProfileKind kind, float parameter)
{
    const float u =
        parameter - std::floor(parameter);

    if (kind == RenderProfileKind::circle)
    {
        const float angle =
            -0.5f * std::numbers::pi_v<float> +
            2.0f * std::numbers::pi_v<float> * u;
        return {
            0.5f * std::cos(angle),
            0.5f * std::sin(angle)};
    }

    if (kind == RenderProfileKind::triangle)
    {
        constexpr std::array<ProfilePoint, 3> vertices{{
            {0.0f, -0.577350269f},
            {0.5f, 0.288675135f},
            {-0.5f, 0.288675135f}
        }};

        const float scaled = u * 3.0f;
        const std::size_t side =
            static_cast<std::size_t>(scaled) % 3U;
        const float t =
            scaled - std::floor(scaled);
        const ProfilePoint a = vertices[side];
        const ProfilePoint b =
            vertices[(side + 1U) % vertices.size()];
        return {
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t};
    }

    constexpr std::array<ProfilePoint, 4> vertices{{
        {0.5f, -0.5f},
        {0.5f, 0.5f},
        {-0.5f, 0.5f},
        {-0.5f, -0.5f}
    }};

    const float scaled = u * 4.0f;
    const std::size_t side =
        static_cast<std::size_t>(scaled) % 4U;
    const float t =
        scaled - std::floor(scaled);
    const ProfilePoint a = vertices[side];
    const ProfilePoint b =
        vertices[(side + 1U) % vertices.size()];
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t};
}

void add_profile_breakpoints(
    std::vector<float>& values,
    RenderProfileKind kind,
    float begin,
    float end)
{
    if (kind == RenderProfileKind::square)
    {
        for (float value : {0.25f, 0.5f, 0.75f})
        {
            if (value > begin && value < end)
            {
                values.push_back(value);
            }
        }
    }
    else if (kind == RenderProfileKind::triangle)
    {
        for (float value : {1.0f / 3.0f, 2.0f / 3.0f})
        {
            if (value > begin && value < end)
            {
                values.push_back(value);
            }
        }
    }
}

std::vector<float> profile_parameters(
    RenderProfileKind outer,
    RenderProfileKind inner,
    float begin,
    float end)
{
    std::vector<float> values;
    values.push_back(begin);

    if (outer == RenderProfileKind::circle ||
        inner == RenderProfileKind::circle)
    {
        const float range = std::max(0.0001f, end - begin);
        const std::uint32_t segments =
            std::max<std::uint32_t>(
                4U,
                static_cast<std::uint32_t>(
                    std::ceil(range * 32.0f)));
        for (std::uint32_t index = 1U;
             index < segments;
             ++index)
        {
            values.push_back(
                begin +
                range *
                    static_cast<float>(index) /
                    static_cast<float>(segments));
        }
    }

    add_profile_breakpoints(values, outer, begin, end);
    add_profile_breakpoints(values, inner, begin, end);
    values.push_back(end);

    std::sort(values.begin(), values.end());
    values.erase(
        std::unique(
            values.begin(),
            values.end(),
            [](float a, float b)
            {
                return std::abs(a - b) < 0.000001f;
            }),
        values.end());

    return values;
}

MeshData make_linear_profile_mesh(
    RenderPrimitiveKind kind,
    float profile_begin,
    float profile_end,
    float hollow,
    RenderProfileKind hollow_kind)
{
    MeshData mesh;

    const RenderProfileKind outer = outer_profile_kind(kind);
    const RenderProfileKind inner =
        hollow_kind == RenderProfileKind::same ? outer : hollow_kind;

    const float begin = std::clamp(profile_begin, 0.0f, 0.9998f);
    const float end =
        std::clamp(profile_end, begin + 0.0001f, 1.0f);
    const float hollow_amount = std::clamp(hollow, 0.0f, 0.95f);
    const bool has_hollow = hollow_amount > 0.0001f;
    const bool full_profile = begin <= 0.0001f && end >= 0.9999f;

    const auto parameters =
        profile_parameters(
            outer,
            has_hollow ? inner : outer,
            begin,
            end);
    if (parameters.size() < 2U)
    {
        return mesh;
    }

    auto profile_side_count = [](RenderProfileKind profile)
    {
        switch (profile)
        {
            case RenderProfileKind::triangle: return 3U;
            case RenderProfileKind::square: return 4U;
            case RenderProfileKind::circle:
            case RenderProfileKind::same:
            default: return 1U;
        }
    };

    auto profile_side = [](RenderProfileKind profile, float parameter)
    {
        if (profile == RenderProfileKind::circle ||
            profile == RenderProfileKind::same)
        {
            return 0U;
        }
        const float u =
            std::clamp(
                parameter - std::floor(parameter),
                0.0f,
                0.999999f);
        const std::uint32_t count =
            profile == RenderProfileKind::triangle ? 3U : 4U;
        return std::min<std::uint32_t>(
            count - 1U,
            static_cast<std::uint32_t>(
                std::floor(u * static_cast<float>(count))));
    };

    auto outer_u = [&](float parameter)
    {
        if (outer == RenderProfileKind::circle ||
            outer == RenderProfileKind::same)
        {
            return (parameter - begin) /
                std::max(end - begin, 0.0001f);
        }

        const std::uint32_t count =
            outer == RenderProfileKind::triangle ? 3U : 4U;
        const float scaled =
            parameter * static_cast<float>(count);
        return scaled - std::floor(scaled);
    };

    auto cap_uv = [](ProfilePoint point, float scale_amount)
    {
        return std::array<float, 2>{
            point.x * scale_amount + 0.5f,
            point.y * scale_amount + 0.5f};
    };

    const std::uint32_t outer_side_count =
        profile_side_count(outer);
    std::array<bool, 4> outer_side_present{};
    for (std::size_t i = 0U; i + 1U < parameters.size(); ++i)
    {
        const float mid =
            (parameters[i] + parameters[i + 1U]) * 0.5f;
        outer_side_present[profile_side(outer, mid)] = true;
    }

    append_face_range(mesh, 0U, [&]
    {
        for (std::size_t i = 0U; i + 1U < parameters.size(); ++i)
        {
            const float p0 = parameters[i];
            const float p1 = parameters[i + 1U];
            const ProfilePoint o0 = profile_point(outer, p0);
            const ProfilePoint o1 = profile_point(outer, p1);
            const auto uv0 = cap_uv(o0, 1.0f);
            const auto uv1 = cap_uv(o1, 1.0f);

            if (has_hollow)
            {
                const ProfilePoint h0 = profile_point(inner, p0);
                const ProfilePoint h1 = profile_point(inner, p1);
                const auto huv0 = cap_uv(h0, hollow_amount);
                const auto huv1 = cap_uv(h1, hollow_amount);
                append_quad_vertices(
                    mesh,
                    {o0.x, o0.y, 0.5f, uv0[0], uv0[1]},
                    {o1.x, o1.y, 0.5f, uv1[0], uv1[1]},
                    {h1.x * hollow_amount, h1.y * hollow_amount, 0.5f, huv1[0], huv1[1]},
                    {h0.x * hollow_amount, h0.y * hollow_amount, 0.5f, huv0[0], huv0[1]});
            }
            else
            {
                const auto center =
                    append_vertex(mesh, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f);
                const auto a =
                    append_vertex(mesh, o0.x, o0.y, 0.5f, uv0[0], uv0[1]);
                const auto b =
                    append_vertex(mesh, o1.x, o1.y, 0.5f, uv1[0], uv1[1]);
                mesh.indices.insert(mesh.indices.end(), {center, a, b});
            }
        }
    });

    std::size_t next_face = 1U;

    for (std::uint32_t side = 0U;
         side < outer_side_count;
         ++side)
    {
        if (!outer_side_present[side])
        {
            continue;
        }

        append_face_range(mesh, next_face++, [&]
        {
            for (std::size_t i = 0U;
                 i + 1U < parameters.size();
                 ++i)
            {
                const float p0 = parameters[i];
                const float p1 = parameters[i + 1U];
                const float mid = (p0 + p1) * 0.5f;
                if (profile_side(outer, mid) != side)
                {
                    continue;
                }

                const ProfilePoint a = profile_point(outer, p0);
                const ProfilePoint b = profile_point(outer, p1);
                float u0 = outer_u(p0);
                float u1 = outer_u(p1);

                if (outer != RenderProfileKind::circle &&
                    outer != RenderProfileKind::same &&
                    u1 < u0)
                {
                    u1 = 1.0f;
                }

                append_quad_vertices(
                    mesh,
                    {a.x, a.y, -0.5f, u0, 0.0f},
                    {b.x, b.y, -0.5f, u1, 0.0f},
                    {b.x, b.y, 0.5f, u1, 1.0f},
                    {a.x, a.y, 0.5f, u0, 1.0f});
            }
        });
    }

    if (has_hollow)
    {
        append_face_range(mesh, next_face++, [&]
        {
            for (std::size_t i = 0U;
                 i + 1U < parameters.size();
                 ++i)
            {
                const float p0 = parameters[i];
                const float p1 = parameters[i + 1U];
                const ProfilePoint a = profile_point(inner, p0);
                const ProfilePoint b = profile_point(inner, p1);
                const float u0 =
                    (p0 - begin) /
                    std::max(end - begin, 0.0001f);
                const float u1 =
                    (p1 - begin) /
                    std::max(end - begin, 0.0001f);

                append_quad_vertices(
                    mesh,
                    {b.x * hollow_amount, b.y * hollow_amount, -0.5f, u1, 0.0f},
                    {a.x * hollow_amount, a.y * hollow_amount, -0.5f, u0, 0.0f},
                    {a.x * hollow_amount, a.y * hollow_amount, 0.5f, u0, 1.0f},
                    {b.x * hollow_amount, b.y * hollow_amount, 0.5f, u1, 1.0f});
            }
        });
    }

    append_face_range(mesh, next_face++, [&]
    {
        for (std::size_t i = 0U; i + 1U < parameters.size(); ++i)
        {
            const float p0 = parameters[i];
            const float p1 = parameters[i + 1U];
            const ProfilePoint o0 = profile_point(outer, p0);
            const ProfilePoint o1 = profile_point(outer, p1);
            const auto uv0 = cap_uv(o0, 1.0f);
            const auto uv1 = cap_uv(o1, 1.0f);

            if (has_hollow)
            {
                const ProfilePoint h0 = profile_point(inner, p0);
                const ProfilePoint h1 = profile_point(inner, p1);
                const auto huv0 = cap_uv(h0, hollow_amount);
                const auto huv1 = cap_uv(h1, hollow_amount);
                append_quad_vertices(
                    mesh,
                    {o1.x, o1.y, -0.5f, uv1[0], uv1[1]},
                    {o0.x, o0.y, -0.5f, uv0[0], uv0[1]},
                    {h0.x * hollow_amount, h0.y * hollow_amount, -0.5f, huv0[0], huv0[1]},
                    {h1.x * hollow_amount, h1.y * hollow_amount, -0.5f, huv1[0], huv1[1]});
            }
            else
            {
                const auto center =
                    append_vertex(mesh, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f);
                const auto a =
                    append_vertex(mesh, o1.x, o1.y, -0.5f, uv1[0], uv1[1]);
                const auto b =
                    append_vertex(mesh, o0.x, o0.y, -0.5f, uv0[0], uv0[1]);
                mesh.indices.insert(mesh.indices.end(), {center, a, b});
            }
        }
    });

    if (!full_profile)
    {
        const ProfilePoint begin_outer =
            profile_point(outer, begin);
        const ProfilePoint end_outer =
            profile_point(outer, end);

        if (has_hollow)
        {
            const ProfilePoint begin_inner =
                profile_point(inner, begin);
            const ProfilePoint end_inner =
                profile_point(inner, end);

            append_face_range(mesh, next_face++, [&]
            {
                append_quad_vertices(
                    mesh,
                    {begin_outer.x, begin_outer.y, -0.5f, 0.0f, 0.0f},
                    {begin_inner.x * hollow_amount, begin_inner.y * hollow_amount, -0.5f, 1.0f, 0.0f},
                    {begin_inner.x * hollow_amount, begin_inner.y * hollow_amount, 0.5f, 1.0f, 1.0f},
                    {begin_outer.x, begin_outer.y, 0.5f, 0.0f, 1.0f});
            });

            append_face_range(mesh, next_face++, [&]
            {
                append_quad_vertices(
                    mesh,
                    {end_outer.x, end_outer.y, -0.5f, 0.0f, 0.0f},
                    {end_outer.x, end_outer.y, 0.5f, 0.0f, 1.0f},
                    {end_inner.x * hollow_amount, end_inner.y * hollow_amount, 0.5f, 1.0f, 1.0f},
                    {end_inner.x * hollow_amount, end_inner.y * hollow_amount, -0.5f, 1.0f, 0.0f});
            });
        }
        else
        {
            append_face_range(mesh, next_face++, [&]
            {
                append_quad_vertices(
                    mesh,
                    {0.0f, 0.0f, -0.5f, 1.0f, 0.0f},
                    {begin_outer.x, begin_outer.y, -0.5f, 0.0f, 0.0f},
                    {begin_outer.x, begin_outer.y, 0.5f, 0.0f, 1.0f},
                    {0.0f, 0.0f, 0.5f, 1.0f, 1.0f});
            });

            append_face_range(mesh, next_face++, [&]
            {
                append_quad_vertices(
                    mesh,
                    {0.0f, 0.0f, -0.5f, 1.0f, 0.0f},
                    {0.0f, 0.0f, 0.5f, 1.0f, 1.0f},
                    {end_outer.x, end_outer.y, 0.5f, 0.0f, 1.0f},
                    {end_outer.x, end_outer.y, -0.5f, 0.0f, 0.0f});
            });
        }
    }

    return mesh;
}

std::uint64_t profile_mesh_key(
    const RenderSceneObject& object)
{
    auto quantize = [](float value)
    {
        return static_cast<std::uint64_t>(
            std::clamp(
                std::llround(
                    static_cast<double>(
                        std::clamp(value, 0.0f, 1.0f)) *
                    50000.0),
                0LL,
                50000LL));
    };

    const std::uint64_t kind =
        static_cast<std::uint64_t>(object.kind) & 0x0FU;
    const std::uint64_t hollow_kind =
        static_cast<std::uint64_t>(
            object.hollow_profile) & 0x0FU;
    const std::uint64_t begin =
        quantize(object.profile_begin);
    const std::uint64_t end_cut =
        quantize(1.0f - object.profile_end);
    const std::uint64_t hollow =
        quantize(object.profile_hollow);

    return
        kind |
        (hollow_kind << 4U) |
        (begin << 8U) |
        (end_cut << 24U) |
        (hollow << 40U);
}

bool needs_dynamic_profile_mesh(
    const RenderSceneObject& object)
{
    if (!object.linear_path_deform)
    {
        return false;
    }

    if (object.kind != RenderPrimitiveKind::box &&
        object.kind != RenderPrimitiveKind::cylinder &&
        object.kind != RenderPrimitiveKind::prism)
    {
        return false;
    }

    return
        object.profile_begin > 0.0001f ||
        object.profile_end < 0.9999f ||
        object.profile_hollow > 0.0001f;
}

bool needs_curved_sweep_mesh(
    const RenderSceneObject& object)
{
    return object.curved_path &&
        (object.kind == RenderPrimitiveKind::torus ||
         object.kind == RenderPrimitiveKind::sphere);
}

std::uint64_t curved_sweep_key(
    const RenderSceneObject& object)
{
    auto quantize_signed = [](float value, float scale_value)
    {
        const long long raw = std::llround(
            static_cast<double>(value) *
            static_cast<double>(scale_value));
        return static_cast<std::uint64_t>(
            static_cast<std::uint32_t>(
                static_cast<std::int32_t>(raw)));
    };

    auto quantize_unit = [](float value)
    {
        return static_cast<std::uint64_t>(
            std::clamp(
                std::llround(
                    static_cast<double>(
                        std::clamp(value, 0.0f, 2.0f)) *
                    1000.0),
                0LL,
                2000LL));
    };

    std::uint64_t key =
        static_cast<std::uint64_t>(object.kind) & 0x0FU;
    key ^= quantize_unit(object.path_begin) << 4U;
    key ^= quantize_unit(object.path_end) << 16U;
    key ^= quantize_unit(object.path_hole_x) << 28U;
    key ^= quantize_unit(object.path_hole_y) << 40U;
    key ^= (quantize_signed(object.revolutions, 100.0f) & 0xFFU) << 52U;
    key ^= (quantize_signed(object.radius_offset, 100.0f) & 0x3FU) << 60U;

    key ^= quantize_signed(object.skew, 100.0f);
    key ^= quantize_signed(object.shear_x, 100.0f) << 8U;
    key ^= quantize_signed(object.shear_y, 100.0f) << 16U;
    key ^= quantize_signed(object.taper_x, 100.0f) << 24U;
    key ^= quantize_signed(object.taper_y, 100.0f) << 32U;
    key ^= quantize_signed(object.twist_begin_radians, 100.0f) << 40U;
    key ^= quantize_signed(object.twist_end_radians, 100.0f) << 48U;

    return key;
}

ProfilePoint curved_profile_point(
    RenderPrimitiveKind kind,
    float parameter)
{
    if (kind == RenderPrimitiveKind::sphere)
    {
        const float angle =
            -0.5f * std::numbers::pi_v<float> +
            std::numbers::pi_v<float> *
                std::clamp(parameter, 0.0f, 1.0f);
        return {
            0.5f * std::cos(angle),
            0.5f * std::sin(angle)};
    }

    return profile_point(
        RenderProfileKind::circle,
        parameter);
}

MeshData make_curved_sweep_mesh(
    const RenderSceneObject& object)
{
    MeshData mesh;

    const float revolutions =
        std::clamp(object.revolutions, 0.1f, 4.0f);
    const float path_begin =
        std::clamp(object.path_begin, 0.0f, 0.9998f);
    const float path_end =
        std::clamp(
            object.path_end,
            path_begin + 0.0001f,
            1.0f);

    const float two_pi =
        2.0f * std::numbers::pi_v<float>;
    const float start_angle =
        two_pi * path_begin * revolutions -
        object.shear_y * 0.9f;
    const float end_angle =
        two_pi * path_end * revolutions -
        object.shear_y * 0.9f;

    if (end_angle <= start_angle)
    {
        return mesh;
    }

    constexpr std::uint32_t steps_per_revolution = 24U;
    const std::uint32_t path_segments =
        std::max<std::uint32_t>(
            2U,
            static_cast<std::uint32_t>(
                std::ceil(
                    (end_angle - start_angle) /
                    (two_pi /
                     static_cast<float>(
                         steps_per_revolution)))));

    const std::uint32_t profile_segments =
        object.kind == RenderPrimitiveKind::sphere
            ? 12U
            : 16U;

    const float path_length =
        path_end - path_begin;
    const float total_skew =
        object.skew * 2.0f * path_length;
    const float skew_start =
        path_begin * 2.0f * object.skew -
        object.skew;
    const float y_path_scale =
        object.path_hole_y * 0.5f;
    const float x_shear_factor =
        object.shear_x *
        (0.25f +
         0.5f * (0.5f - object.path_hole_y));
    const float y_shear_compensation =
        1.0f +
        std::abs(object.shear_y) * 0.25f;

    const auto append_layer_vertex =
        [&](float angle,
            float percent_of_path,
            float percent_of_angles,
            float profile_parameter)
        {
            float x_profile_scale =
                (1.0f - std::abs(object.skew)) *
                object.path_hole_x;
            float y_profile_scale =
                object.path_hole_y;

            if (object.taper_x > 0.01f)
            {
                x_profile_scale *=
                    1.0f -
                    percent_of_path *
                        object.taper_x;
            }
            else if (object.taper_x < -0.01f)
            {
                x_profile_scale *=
                    1.0f +
                    (1.0f - percent_of_path) *
                        object.taper_x;
            }

            if (object.taper_y > 0.01f)
            {
                y_profile_scale *=
                    1.0f -
                    percent_of_path *
                        object.taper_y;
            }
            else if (object.taper_y < -0.01f)
            {
                y_profile_scale *=
                    1.0f +
                    (1.0f - percent_of_path) *
                        object.taper_y;
            }

            float radius_scale = 1.0f;
            if (object.radius_offset > 0.001f)
            {
                radius_scale =
                    1.0f -
                    object.radius_offset *
                        percent_of_path;
            }
            else if (object.radius_offset < -0.001f)
            {
                radius_scale =
                    1.0f +
                    object.radius_offset *
                        (1.0f - percent_of_path);
            }

            const float twist =
                object.twist_begin_radians +
                (object.twist_end_radians -
                 object.twist_begin_radians) *
                    percent_of_path;

            const float x_offset =
                0.5f *
                    (skew_start +
                     total_skew * percent_of_angles) +
                std::sin(angle) *
                    x_shear_factor;

            const float path_radius =
                (0.5f - y_path_scale) *
                radius_scale;

            const float y_offset =
                y_shear_compensation *
                std::cos(angle) *
                path_radius;
            const float z_offset =
                std::sin(
                    angle +
                    object.shear_y) *
                path_radius;

            const ProfilePoint profile =
                curved_profile_point(
                    object.kind,
                    profile_parameter);

            float px =
                profile.x *
                x_profile_scale;
            float py =
                profile.y *
                y_profile_scale;
            float pz = 0.0f;

            const float twist_cos =
                std::cos(twist);
            const float twist_sin =
                std::sin(twist);
            const float twisted_x =
                px * twist_cos -
                py * twist_sin;
            const float twisted_y =
                px * twist_sin +
                py * twist_cos;

            const float path_rotation =
                angle + object.shear_y;
            const float path_cos =
                std::cos(path_rotation);
            const float path_sin =
                std::sin(path_rotation);

            px = twisted_x;
            py =
                twisted_y * path_cos -
                pz * path_sin;
            pz =
                twisted_y * path_sin +
                pz * path_cos;

            append_vertex(
                mesh,
                x_offset + px,
                y_offset + py,
                z_offset + pz,
                profile_parameter,
                percent_of_angles);
        };

    for (std::uint32_t path_index = 0U;
         path_index <= path_segments;
         ++path_index)
    {
        const float path_lerp =
            static_cast<float>(path_index) /
            static_cast<float>(path_segments);
        const float angle =
            start_angle +
            (end_angle - start_angle) *
                path_lerp;
        const float percent_of_path =
            angle /
            (two_pi * revolutions);

        for (std::uint32_t profile_index = 0U;
             profile_index <= profile_segments;
             ++profile_index)
        {
            const float profile_parameter =
                static_cast<float>(
                    profile_index) /
                static_cast<float>(
                    profile_segments);

            append_layer_vertex(
                angle,
                percent_of_path,
                path_lerp,
                profile_parameter);
        }
    }

    const std::uint32_t row =
        profile_segments + 1U;

    const bool closed_path =
        path_begin <= 0.0001f &&
        path_end >= 0.9999f &&
        std::abs(
            revolutions -
            std::round(revolutions)) <
            0.0001f;

    auto vertex_position =
        [&](std::uint32_t index)
        {
            const std::size_t base =
                static_cast<std::size_t>(index) *
                prim_vertex_stride;
            return std::array<float, 3>{
                mesh.vertices[base],
                mesh.vertices[base + 1U],
                mesh.vertices[base + 2U]};
        };

    auto cap_center =
        [&](std::uint32_t base)
        {
            std::array<float, 3> center{};
            for (std::uint32_t i = 0U;
                 i <= profile_segments;
                 ++i)
            {
                const auto p =
                    vertex_position(base + i);
                center[0] += p[0];
                center[1] += p[1];
                center[2] += p[2];
            }

            const float inverse =
                1.0f /
                static_cast<float>(
                    profile_segments + 1U);
            center[0] *= inverse;
            center[1] *= inverse;
            center[2] *= inverse;
            return center;
        };

    // Circular-path prims number cut-end before the outside surface.
    // Without a cut the outside surface is face 0.
    const std::size_t outside_face =
        closed_path ? 0U : 1U;

    if (!closed_path)
    {
        append_face_range(mesh, 0U, [&]
        {
            const std::uint32_t end_base =
                path_segments * row;
            const auto center =
                cap_center(end_base);

            for (std::uint32_t profile_index = 0U;
                 profile_index < profile_segments;
                 ++profile_index)
            {
                const float u0 =
                    static_cast<float>(profile_index) /
                    static_cast<float>(profile_segments);
                const float u1 =
                    static_cast<float>(profile_index + 1U) /
                    static_cast<float>(profile_segments);

                const ProfilePoint q0 =
                    curved_profile_point(
                        object.kind,
                        u0);
                const ProfilePoint q1 =
                    curved_profile_point(
                        object.kind,
                        u1);
                const auto p0 =
                    vertex_position(
                        end_base + profile_index);
                const auto p1 =
                    vertex_position(
                        end_base + profile_index + 1U);

                const auto c =
                    append_vertex(
                        mesh,
                        center[0], center[1], center[2],
                        0.5f, 0.5f);
                const auto a =
                    append_vertex(
                        mesh,
                        p0[0], p0[1], p0[2],
                        q0.x + 0.5f,
                        q0.y + 0.5f);
                const auto b =
                    append_vertex(
                        mesh,
                        p1[0], p1[1], p1[2],
                        q1.x + 0.5f,
                        q1.y + 0.5f);

                mesh.indices.insert(
                    mesh.indices.end(),
                    {c, a, b});
            }
        });
    }

    append_face_range(mesh, outside_face, [&]
    {
        for (std::uint32_t path_index = 0U;
             path_index < path_segments;
             ++path_index)
        {
            for (std::uint32_t profile_index = 0U;
                 profile_index < profile_segments;
                 ++profile_index)
            {
                const std::uint32_t a =
                    path_index * row +
                    profile_index;
                const std::uint32_t b =
                    a + row;

                mesh.indices.insert(
                    mesh.indices.end(),
                    {
                        a,
                        b,
                        a + 1U,
                        a + 1U,
                        b,
                        b + 1U
                    });
            }
        }
    });

    if (!closed_path)
    {
        append_face_range(mesh, 2U, [&]
        {
            const auto center =
                cap_center(0U);

            for (std::uint32_t profile_index = 0U;
                 profile_index < profile_segments;
                 ++profile_index)
            {
                const float u0 =
                    static_cast<float>(profile_index) /
                    static_cast<float>(profile_segments);
                const float u1 =
                    static_cast<float>(profile_index + 1U) /
                    static_cast<float>(profile_segments);

                const ProfilePoint q0 =
                    curved_profile_point(
                        object.kind,
                        u0);
                const ProfilePoint q1 =
                    curved_profile_point(
                        object.kind,
                        u1);
                const auto p0 =
                    vertex_position(profile_index);
                const auto p1 =
                    vertex_position(profile_index + 1U);

                const auto c =
                    append_vertex(
                        mesh,
                        center[0], center[1], center[2],
                        0.5f, 0.5f);
                const auto a =
                    append_vertex(
                        mesh,
                        p0[0], p0[1], p0[2],
                        q0.x + 0.5f,
                        q0.y + 0.5f);
                const auto b =
                    append_vertex(
                        mesh,
                        p1[0], p1[1], p1[2],
                        q1.x + 0.5f,
                        q1.y + 0.5f);

                mesh.indices.insert(
                    mesh.indices.end(),
                    {c, b, a});
            }
        });
    }

    return mesh;
}

bool upload_mesh(
    GpuMesh& target,
    const MeshData& source,
    GLenum* reported_error=nullptr)
{
    if(reported_error)*reported_error=GL_NO_ERROR;
    if (source.vertices.empty() || source.indices.empty())
    {
        return false;
    }

    for(int i=0;i<8 && glGetError()!=GL_NO_ERROR;++i) {}
    glGenVertexArrays(1, &target.vao);
    glGenBuffers(1, &target.vbo);
    glGenBuffers(1, &target.ebo);

    glBindVertexArray(target.vao);
    glBindBuffer(GL_ARRAY_BUFFER, target.vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            source.vertices.size() * sizeof(float)),
        source.vertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, target.ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            source.indices.size() * sizeof(std::uint32_t)),
        source.indices.data(),
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0U);
    glVertexAttribPointer(
        0U,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(prim_vertex_stride * sizeof(float)),
        nullptr);

    glEnableVertexAttribArray(1U);
    glVertexAttribPointer(
        1U,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(prim_vertex_stride * sizeof(float)),
        reinterpret_cast<void*>(3U * sizeof(float)));

    glEnableVertexAttribArray(3U);
    glVertexAttribPointer(
        3U,
        2,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(prim_vertex_stride * sizeof(float)),
        reinterpret_cast<void*>(6U * sizeof(float)));

    glBindVertexArray(0U);
    glBindBuffer(GL_ARRAY_BUFFER, 0U);

    target.index_count =
        static_cast<GLsizei>(source.indices.size());
    target.face_ranges = source.face_ranges;
    const auto upload_error=glGetError();
    if(reported_error)*reported_error=upload_error;
    if(upload_error!=GL_NO_ERROR ||
        target.vao==0U || target.vbo==0U || target.ebo==0U)
    {
        if(target.ebo!=0U)glDeleteBuffers(1,&target.ebo);
        if(target.vbo!=0U)glDeleteBuffers(1,&target.vbo);
        if(target.vao!=0U)glDeleteVertexArrays(1,&target.vao);
        target={};
        nexverse::core::warning(
            "OGLRender mesh GPU upload failed; gl_error="+
            std::to_string(upload_error));
        return false;
    }
    return true;
}

void destroy_mesh(GpuMesh& mesh)
{
    if(mesh.skin_vbo!=0U)glDeleteBuffers(1,&mesh.skin_vbo);
    if (mesh.ebo != 0U)
    {
        glDeleteBuffers(1, &mesh.ebo);
    }
    if (mesh.vbo != 0U)
    {
        glDeleteBuffers(1, &mesh.vbo);
    }
    if (mesh.vao != 0U)
    {
        glDeleteVertexArrays(1, &mesh.vao);
    }
    mesh = {};
}

constexpr const char* vertex_shader_source = R"GLSL(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aMaterial;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in float aAvatarJointWeight;
uniform bool uAvatarSkinEnabled;
uniform int uAvatarJointCount;
uniform mat4 uAvatarJointPalette[32];

uniform mat4 uMvp;
uniform mat4 uModel;
uniform bool uUsePrimDeform;
uniform vec2 uPrimPathRange;
uniform vec2 uPrimTaper;
uniform vec2 uPrimShear;
uniform float uPrimTwistBegin;
uniform float uPrimTwistEnd;

out vec3 vColor;
out vec2 vWorldXY;
out vec3 vObjectLocal;
out vec3 vPointLightWorldPosition;
out float vMaterial;
out vec2 vTexCoord;

void main()
{
    vec3 localPosition = aPosition;
    if(uAvatarSkinEnabled)
    {
        // Last palette index uses last two valid joints, factor 1.
        float weight=clamp(aAvatarJointWeight,0.0,
                           float(uAvatarJointCount-1));
        int base=min(int(floor(weight)),uAvatarJointCount-2);
        float factor=clamp(weight-float(base),0.0,1.0);
        vec3 p0=(uAvatarJointPalette[base]*
                 vec4(aPosition,1.0)).xyz;
        vec3 p1=(uAvatarJointPalette[base+1]*
                 vec4(aPosition,1.0)).xyz;
        localPosition=mix(p0,p1,factor);
    }

    if (uUsePrimDeform)
    {
        float basePathT = clamp(aPosition.z + 0.5, 0.0, 1.0);
        float pathT = mix(
            uPrimPathRange.x,
            uPrimPathRange.y,
            basePathT);

        localPosition.z = pathT - 0.5;

        vec2 taperScale = max(
            vec2(0.01),
            vec2(1.0) - uPrimTaper * pathT);
        localPosition.xy *= taperScale;

        float twist = mix(
            uPrimTwistBegin,
            uPrimTwistEnd,
            pathT);
        float twistCos = cos(twist);
        float twistSin = sin(twist);
        localPosition.xy = mat2(
            twistCos, -twistSin,
            twistSin, twistCos) * localPosition.xy;

        localPosition.xy +=
            uPrimShear * (pathT - 0.5);
    }

    gl_Position = uMvp * vec4(localPosition, 1.0);
    vColor = aColor;
    vWorldXY = localPosition.xy;
    vObjectLocal = localPosition;
    vPointLightWorldPosition = (uModel * vec4(localPosition, 1.0)).xyz;
    vMaterial = aMaterial;
    vTexCoord = aTexCoord;
}
)GLSL";

constexpr const char* fragment_shader_source = R"GLSL(
#version 330 core
in vec3 vColor;
in vec2 vWorldXY;
in vec3 vObjectLocal;
in vec3 vPointLightWorldPosition;
in float vMaterial;
in vec2 vTexCoord;
out vec4 FragColor;

uniform float uAlpha;
uniform vec3 uCameraWorld;
uniform int uPointLightCount;
uniform vec4 uPointLightPositions[8]; // xyz world position, w radius
uniform vec4 uPointLightColors[8];    // RGB, intensity
uniform vec4 uPointLightFalloffs[8];  // x falloff
uniform bool uUseTerrainTextures;
uniform bool uOverrideColor;
uniform vec3 uOverrideColorValue;
uniform float uObjectGlow;
uniform int uObjectShiny;
uniform int uObjectBumpiness;
uniform int uObjectAlphaMode;
uniform float uObjectAlphaCutoff;
uniform bool uObjectFullbright;
uniform bool uUseObjectTexture;
uniform bool uObjectTexGenPlanar;
uniform vec3 uObjectScale;
uniform vec2 uObjectRepeat;
uniform vec2 uObjectOffset;
uniform float uObjectRotation;
uniform sampler2D uObjectTexture;
uniform bool uUseLegacyNormalMap;
uniform bool uUseLegacySpecularMap;
uniform sampler2D uLegacyNormalMap;
uniform sampler2D uLegacySpecularMap;
uniform vec2 uLegacyNormalRepeat;
uniform vec2 uLegacyNormalOffset;
uniform float uLegacyNormalRotation;
uniform vec2 uLegacySpecularRepeat;
uniform vec2 uLegacySpecularOffset;
uniform float uLegacySpecularRotation;
uniform vec3 uLegacySpecularColor;
uniform float uLegacySpecularExponent;
uniform float uLegacyEnvironmentIntensity;
uniform vec3 uFogHorizon;
uniform sampler2D uTerrain0;
uniform sampler2D uTerrain1;
uniform sampler2D uTerrain2;
uniform sampler2D uTerrain3;

vec2 objectBaseUv()
{
    vec2 uv = vTexCoord;

    if (uObjectTexGenPlanar)
    {
        vec3 dx = dFdx(vObjectLocal);
        vec3 dy = dFdy(vObjectLocal);
        vec3 normal = abs(normalize(cross(dx, dy)));

        if (normal.z >= normal.x && normal.z >= normal.y)
        {
            uv =
                vObjectLocal.xy *
                uObjectScale.xy *
                2.0 +
                vec2(0.5);
        }
        else if (normal.x >= normal.y)
        {
            uv =
                vObjectLocal.yz *
                uObjectScale.yz *
                2.0 +
                vec2(0.5);
        }
        else
        {
            uv =
                vObjectLocal.xz *
                uObjectScale.xz *
                2.0 +
                vec2(0.5);
        }
    }

    return uv;
}

// OpenSim legacy maps each have an independent UV transform; the source
// values were stored as fixed-point integers with scale 1/10000.
vec2 legacyTransformUv(vec2 rawUv, vec2 repeat, vec2 offset, float rotation)
{
    vec2 uv = rawUv * repeat;
    float c = cos(rotation);
    float si = sin(rotation);
    uv = mat2(c, -si, si, c) * (uv - vec2(0.5)) + vec2(0.5);
    return uv + offset;
}

vec2 objectTextureUv()
{
    return legacyTransformUv(objectBaseUv(), uObjectRepeat,
                             uObjectOffset, uObjectRotation);
}

vec3 terrainTextureColor(vec2 uv, float material)
{
    float t = clamp(material, 0.0, 1.0);
    if (t < 0.333333)
    {
        float local = t / 0.333333;
        return mix(
            texture(uTerrain0, uv).rgb,
            texture(uTerrain1, uv).rgb,
            local);
    }
    if (t < 0.666667)
    {
        float local = (t - 0.333333) / 0.333334;
        return mix(
            texture(uTerrain1, uv).rgb,
            texture(uTerrain2, uv).rgb,
            local);
    }

    float local = (t - 0.666667) / 0.333333;
    return mix(
        texture(uTerrain2, uv).rgb,
        texture(uTerrain3, uv).rgb,
        local);
}

void main()
{
    vec3 base = vColor;
    float alpha = uAlpha;

    if (uUseTerrainTextures)
    {
        vec2 uv = vWorldXY / 12.0;
        vec3 textureColor = terrainTextureColor(uv, vMaterial);
        // Keep source terrain textures prominent instead of a gray wash.
        base = mix(textureColor, vColor, 0.035);
    }

    if (uOverrideColor)
    {
        base = uOverrideColorValue;

        if (uUseObjectTexture)
        {
            vec4 texel = texture(
                uObjectTexture,
                objectTextureUv());
            base *= texel.rgb;
            if (uObjectAlphaMode == 2)
            {
                if (texel.a <= uObjectAlphaCutoff) discard;
            }
            else if (uObjectAlphaMode == 1)
            {
                alpha *= texel.a;
            }
            else if (uObjectAlphaMode == 3)
            {
                base += texel.rgb * texel.a * 0.15;
            }
        }

        // Legacy per-face normal/specular maps. Derivatives approximate a
        // local tangent frame; PBR energy conservation is not provided here.
        // Missing assets always fall back to the earlier flat surface.
        if (!uObjectFullbright &&
            (uObjectShiny > 0 || uUseLegacyNormalMap ||
             uUseLegacySpecularMap))
        {
            vec3 dpdx = dFdx(vObjectLocal);
            vec3 dpdy = dFdy(vObjectLocal);
            vec3 geometric = cross(dpdx, dpdy);
            vec3 normal = normalize(geometric + vec3(1e-10));
            if (!gl_FrontFacing) normal = -normal;
            if (uUseLegacyNormalMap)
            {
                vec2 normalUv = legacyTransformUv(
                    objectBaseUv(), uLegacyNormalRepeat,
                    uLegacyNormalOffset, uLegacyNormalRotation);
                vec2 duvdx = dFdx(normalUv);
                vec2 duvdy = dFdy(normalUv);
                float det = duvdx.x * duvdy.y - duvdx.y * duvdy.x;
                if (abs(det) > 1e-8)
                {
                    vec3 tangent = normalize(
                        dpdx * duvdy.y - dpdy * duvdx.y);
                    tangent = normalize(tangent -
                        normal * dot(normal, tangent));
                    vec3 bitangent = normalize(cross(normal, tangent)) *
                        sign(det);
                    vec3 sampleNormal = texture(uLegacyNormalMap, normalUv).xyz *
                        2.0 - 1.0;
                    normal = normalize(mat3(tangent, bitangent, normal) *
                        sampleNormal);
                }
                float diffuse = max(dot(normal,
                    normalize(vec3(0.32, 0.4, 0.86))), 0.0);
                base *= 0.68 + 0.32 * diffuse;
            }

            vec3 lightDir = normalize(vec3(0.32, 0.4, 0.86));
            float facing = max(dot(normal, lightDir), 0.0);
            if (uUseLegacySpecularMap)
            {
                vec2 specUv = legacyTransformUv(
                    objectBaseUv(), uLegacySpecularRepeat,
                    uLegacySpecularOffset, uLegacySpecularRotation);
                vec4 spec = texture(uLegacySpecularMap, specUv);
                float exponent = mix(4.0, 128.0,
                    clamp(uLegacySpecularExponent, 0.0, 1.0));
                float gloss = pow(facing, exponent);
                base += spec.rgb * uLegacySpecularColor *
                    (gloss * (0.1 + 0.9 * spec.a) *
                     (0.25 + 0.75 * uLegacyEnvironmentIntensity));
            }
            else if (uObjectShiny > 0)
            {
                float gloss = pow(facing,
                    mix(10.0, 55.0, float(uObjectShiny) / 3.0));
                base += vec3(gloss * float(uObjectShiny) * 0.11);
            }
        }
        // First OpenSim legacy point-light pass; no shadows or PBR.
        // Only non-fullbright objects receive dynamic light shading.
        if (!uObjectFullbright && uPointLightCount > 0)
        {
            vec3 normal = cross(
                dFdx(vPointLightWorldPosition),
                dFdy(vPointLightWorldPosition));
            normal = normalize(normal + vec3(1e-9));
            if (!gl_FrontFacing) normal = -normal;
            vec3 illumination = vec3(0.0);
            for (int i = 0; i < 8; ++i)
            {
                if (i >= uPointLightCount) break;
                vec3 toward = uPointLightPositions[i].xyz -
                    vPointLightWorldPosition;
                float distanceToLight = length(toward);
                float radius = max(uPointLightPositions[i].w, 0.01);
                if (distanceToLight >= radius) continue;
                float diffuse = max(
                    dot(normal, toward / max(distanceToLight, 0.0001)),
                    0.0);
                float attenuation = 1.0 - distanceToLight / radius;
                float falloff = max(uPointLightFalloffs[i].x, 0.05);
                // Legacy cutoff defines the core versus penumbra
                // approximately; it is not a shadow-casting spotlight.
                float cutoff = clamp(uPointLightFalloffs[i].y, 0.0, 0.90);
                attenuation = pow(attenuation, 1.0 + falloff);
                attenuation *= smoothstep(cutoff, 1.0, attenuation);
                vec3 incident = toward / max(distanceToLight, 0.0001);
                vec3 viewDir = normalize(uCameraWorld - vPointLightWorldPosition);
                vec3 halfVec = normalize(incident + viewDir);
                float specular = uObjectShiny > 0 ?
                    pow(max(dot(normal, halfVec), 0.0),
                        mix(10.0, 64.0, float(uObjectShiny)/3.0)) *
                        float(uObjectShiny)/3.0 : 0.0;
                illumination += uPointLightColors[i].rgb *
                    uPointLightColors[i].a *
                    (diffuse + 0.25 * specular) * attenuation;
            }
            base *= 1.0 + min(illumination, vec3(1.5));
        }
        // Classic bumpiness values are not normal-map texture assets.
        base += vec3(clamp(uObjectGlow, 0.0, 1.0) * 0.35);
    }

    float fog = uObjectFullbright
        ? 0.0
        : smoothstep(0.90, 1.0, gl_FragCoord.z);
    vec3 color = mix(base, uFogHorizon, fog * 0.55);
    if (alpha < 0.004) discard;
    FragColor = vec4(color, alpha);
}
)GLSL";

// Dedicated soft-sprite shader: native points rendered as Gaussian
// circles, avoiding the hard square pixels of old particle renderers.
constexpr const char* particle_vertex_shader_source = R"GLSL(
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in float aRadius;
uniform mat4 uViewProjection;
uniform vec3 uCamera;
uniform float uViewportHeight;
out vec4 vParticleColor;
void main()
{
    gl_Position = uViewProjection * vec4(aPosition, 1.0);
    float distanceToCamera = max(distance(aPosition, uCamera), 0.1);
    gl_PointSize = clamp(aRadius * uViewportHeight /
        distanceToCamera, 2.0, 96.0);
    vParticleColor = aColor;
}
)GLSL";

constexpr const char* ribbon_vertex_shader_source = R"GLSL(
#version 330 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec4 aColor;
layout(location=2) in vec2 aUv;
uniform mat4 uViewProjection;
out vec4 vColor;
out vec2 vUv;
void main()
{
    gl_Position = uViewProjection * vec4(aPosition,1.0);
    vColor = aColor;
    vUv = aUv;
}
)GLSL";
constexpr const char* ribbon_fragment_shader_source = R"GLSL(
#version 330 core
in vec4 vColor;
in vec2 vUv;
uniform bool uTextured;
uniform sampler2D uRibbonTexture;
out vec4 FragColor;
void main()
{
    vec4 image = uTextured ? texture(uRibbonTexture,vUv) : vec4(1.0);
    float edge = 1.0-smoothstep(0.72,1.0,abs(vUv.x*2.0-1.0));
    FragColor = vec4(vColor.rgb*image.rgb,vColor.a*image.a*edge);
}
)GLSL";

constexpr const char* particle_fragment_shader_source = R"GLSL(
#version 330 core
in vec4 vParticleColor;
uniform sampler2D uParticleTexture;
uniform bool uParticleTextured;
out vec4 FragColor;
void main()
{
    vec2 xy = gl_PointCoord - vec2(0.5);
    float r = length(xy) * 2.0;
    if (r >= 1.0) discard;
    // The generated soft disc remains the fallback for missing assets.
    float soft = exp(-4.0 * r * r) * (1.0 - smoothstep(0.7, 1.0, r));
    vec4 sampleColor = uParticleTextured
        ? texture(uParticleTexture, gl_PointCoord) : vec4(1.0);
    FragColor = vec4(vParticleColor.rgb * sampleColor.rgb,
        vParticleColor.a * sampleColor.a * soft);
}
)GLSL";

constexpr const char* sky_vertex_shader_source = R"GLSL(
#version 330 core
out vec2 vUv;

void main()
{
    vec2 p;
    if (gl_VertexID == 0)
    {
        p = vec2(-1.0, -1.0);
    }
    else if (gl_VertexID == 1)
    {
        p = vec2(3.0, -1.0);
    }
    else
    {
        p = vec2(-1.0, 3.0);
    }

    vUv = p * 0.5 + 0.5;
    gl_Position = vec4(p, 0.0, 1.0);
}
)GLSL";

constexpr const char* sky_fragment_shader_source = R"GLSL(
#version 330 core
in vec2 vUv;
out vec4 FragColor;

uniform float uTime;
uniform vec3 uHorizon;
uniform vec3 uZenith;
uniform vec3 uSunColor;
uniform vec2 uSunPosition;
uniform float uGamma;
uniform float uHazeDensity;

void main()
{
    float y = clamp(vUv.y, 0.0, 1.0);
    float gradientPower = clamp(0.62 + uHazeDensity * 0.20, 0.55, 1.15);
    vec3 sky = mix(uHorizon, uZenith, pow(y, gradientPower));

    float sunDist = distance(vUv, uSunPosition);
    float sun = 1.0 - smoothstep(0.018, 0.070, sunDist);
    float glow = 1.0 - smoothstep(0.05, 0.24, sunDist);
    sky += uSunColor * sun * 0.85;
    sky += uSunColor * glow * 0.10;

    float subtle = 0.006 * sin(uTime * 0.08);
    sky += vec3(subtle);

    float safeGamma = max(uGamma, 0.1);
    sky = pow(max(sky, vec3(0.0)), vec3(1.0 / safeGamma));
    FragColor = vec4(sky, 1.0);
}
)GLSL";

std::string shader_log(GLuint shader)
{
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    return log;
}

std::string program_log(GLuint program)
{
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    return log;
}

GLuint compile_shader(GLenum type, const char* source)
{
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        nexverse::core::error(
            "Legacy renderer shader compile failed: " + shader_log(shader));
        glDeleteShader(shader);
        return 0U;
    }

    return shader;
}

GLuint create_program_from(
    const char* vertex_source,
    const char* fragment_source)
{
    const GLuint vertex =
        compile_shader(GL_VERTEX_SHADER, vertex_source);
    if (vertex == 0U)
    {
        return 0U;
    }

    const GLuint fragment =
        compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (fragment == 0U)
    {
        glDeleteShader(vertex);
        return 0U;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        nexverse::core::error(
            "Legacy renderer program link failed: " + program_log(program));
        glDeleteProgram(program);
        return 0U;
    }

    return program;
}

GLuint create_program()
{
    return create_program_from(
        vertex_shader_source,
        fragment_shader_source);
}

GLuint create_sky_program()
{
    return create_program_from(
        sky_vertex_shader_source,
        sky_fragment_shader_source);
}

bool has_extension(std::string_view name)
{
    GLint count = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);

    for (GLint index = 0; index < count; ++index)
    {
        const auto* value = reinterpret_cast<const char*>(
            glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(index)));
        if (value != nullptr && name == value)
        {
            return true;
        }
    }

    return false;
}
}

namespace nexverse::render
{
class LegacyOpenGlRenderer final : public Renderer
{
public:
    ~LegacyOpenGlRenderer() override
    {
        shutdown();
    }

    bool initialize(int width, int height) override
    {
        width_ = width > 0 ? width : 1;
        height_ = height > 0 ? height : 1;
        // GL_EXT_texture_filter_anisotropic is optional on OpenGL 3.3.
        // Query extensions rather than blindly passing enum 0x84FE.
        GLint extension_count = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);
        for (GLint i = 0; i < extension_count; ++i)
        {
            const auto* name = glGetStringi(
                GL_EXTENSIONS, static_cast<GLuint>(i));
            if (name != nullptr &&
                std::string_view{reinterpret_cast<const char*>(name)} ==
                    "GL_EXT_texture_filter_anisotropic")
            {
                GLfloat limit = 1.0f;
                glGetFloatv(0x84FFU, &limit);
                if (std::isfinite(limit) && limit >= 1.0f)
                    texture_anisotropy_ = std::min(limit, 8.0f);
                break;
            }
        }

        program_ = create_program();
        sky_program_ = create_sky_program();
        particle_program_ = create_program_from(
            particle_vertex_shader_source,
            particle_fragment_shader_source);
        ribbon_program_ = create_program_from(
            ribbon_vertex_shader_source,
            ribbon_fragment_shader_source);
        if (program_ == 0U || sky_program_ == 0U ||
            particle_program_ == 0U || ribbon_program_ == 0U)
        {
            return false;
        }
        // OpenGL 3.3 core, two nonblocking queries per original body
        // mesh Head/Upper/Lower. Individual slots cannot overwrite each
        // other's result; query results are polled without GPU fences.
        glGenQueries(static_cast<GLsizei>(
            self_avatar_visibility_queries_.size()),
            self_avatar_visibility_queries_.data());
        if(std::any_of(self_avatar_visibility_queries_.begin(),
            self_avatar_visibility_queries_.end(),
            [](GLuint id){return id==0U;}))
            return false;

        glGenVertexArrays(1, &particle_vao_);
        glGenBuffers(1, &particle_vbo_);
        glBindVertexArray(particle_vao_);
        glBindBuffer(GL_ARRAY_BUFFER, particle_vbo_);
        constexpr GLsizei stride = 8 * static_cast<GLsizei>(sizeof(float));
        glVertexAttribPointer(0U, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(0U);
        glVertexAttribPointer(1U, 4, GL_FLOAT, GL_FALSE, stride,
            reinterpret_cast<void*>(3U * sizeof(float)));
        glEnableVertexAttribArray(1U);
        glVertexAttribPointer(2U, 1, GL_FLOAT, GL_FALSE, stride,
            reinterpret_cast<void*>(7U * sizeof(float)));
        glEnableVertexAttribArray(2U);
        glBindVertexArray(0U);
        glBindBuffer(GL_ARRAY_BUFFER, 0U);

        glGenVertexArrays(1, &ribbon_vao_);
        glGenBuffers(1, &ribbon_vbo_);
        glBindVertexArray(ribbon_vao_);
        glBindBuffer(GL_ARRAY_BUFFER, ribbon_vbo_);
        constexpr GLsizei ribbon_stride = 9*
            static_cast<GLsizei>(sizeof(float));
        glVertexAttribPointer(0U,3,GL_FLOAT,GL_FALSE,ribbon_stride,nullptr);
        glEnableVertexAttribArray(0U);
        glVertexAttribPointer(1U,4,GL_FLOAT,GL_FALSE,ribbon_stride,
            reinterpret_cast<void*>(3U*sizeof(float)));
        glEnableVertexAttribArray(1U);
        glVertexAttribPointer(2U,2,GL_FLOAT,GL_FALSE,ribbon_stride,
            reinterpret_cast<void*>(7U*sizeof(float)));
        glEnableVertexAttribArray(2U);
        glBindVertexArray(0U);
        glBindBuffer(GL_ARRAY_BUFFER,0U);
        glGenVertexArrays(1, &sky_vao_);

        constexpr std::array<float, 48> vertices{
            -0.50f, -0.50f, -0.50f, 0.05f, 0.85f, 1.00f,
             0.50f, -0.50f, -0.50f, 0.65f, 0.15f, 1.00f,
             0.50f,  0.50f, -0.50f, 1.00f, 0.25f, 0.55f,
            -0.50f,  0.50f, -0.50f, 0.10f, 1.00f, 0.55f,
            -0.50f, -0.50f,  0.50f, 1.00f, 0.65f, 0.10f,
             0.50f, -0.50f,  0.50f, 0.20f, 0.55f, 1.00f,
             0.50f,  0.50f,  0.50f, 1.00f, 0.15f, 0.70f,
            -0.50f,  0.50f,  0.50f, 0.35f, 0.95f, 1.00f
        };

        constexpr std::array<std::uint32_t, 36> indices{
            0U, 1U, 2U, 2U, 3U, 0U,
            4U, 6U, 5U, 6U, 4U, 7U,
            0U, 4U, 5U, 5U, 1U, 0U,
            3U, 2U, 6U, 6U, 7U, 3U,
            1U, 5U, 6U, 6U, 2U, 1U,
            0U, 3U, 7U, 7U, 4U, 0U
        };

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(vertices)),
            vertices.data(),
            GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(indices)),
            indices.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(0U);
        glVertexAttribPointer(
            0U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(6U * sizeof(float)),
            nullptr);

        glEnableVertexAttribArray(1U);
        glVertexAttribPointer(
            1U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(6U * sizeof(float)),
            reinterpret_cast<void*>(3U * sizeof(float)));

        glBindVertexArray(0U);
        glBindBuffer(GL_ARRAY_BUFFER, 0U);

        if (!upload_mesh(box_mesh_, make_box_mesh()) ||
            !upload_mesh(cylinder_mesh_, make_cylinder_mesh()) ||
            !upload_mesh(prism_mesh_, make_prism_mesh()) ||
            !upload_mesh(sphere_mesh_, make_sphere_mesh()) ||
            !upload_mesh(torus_mesh_, make_torus_mesh()))
        {
            core::error(
                "OGLRender primitive base mesh initialization failed");
            return false;
        }

        diagnostics_.backend = "Legacy OpenGL 3.3";
        if (const auto* gpu =
                reinterpret_cast<const char*>(glGetString(GL_RENDERER));
            gpu != nullptr)
        {
            diagnostics_.gpu = gpu;
        }

        nvx_memory_info_ = has_extension("GL_NVX_gpu_memory_info");
        refresh_vram();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        core::info("OGLRender legacy OpenGL backend initialized");
        return true;
    }

    void resize(int width, int height) override
    {
        width_ = width > 0 ? width : 1;
        height_ = height > 0 ? height : 1;
    }

    void set_camera(const RenderCamera& camera) override
    {
        // Previous-frame occlusion queries must not be treated as
        // evidence for a NEW camera position or viewing direction.
        // Detect real pose changes rather than invalidating on the
        // normal same-pose set_camera call each rendered frame.
        const auto changed=[](float a,float b) noexcept
        {
            return !std::isfinite(a)||!std::isfinite(b)||
                   std::abs(a-b)>0.0001f;
        };
        const auto vec_changed=[&](RenderVec3 a,RenderVec3 b)
        {
            return changed(a.x,b.x)||changed(a.y,b.y)||changed(a.z,b.z);
        };
        if(camera_valid_&&(vec_changed(camera_.position,camera.position)||
           vec_changed(camera_.forward,camera.forward)||
           vec_changed(camera_.left,camera.left)||
           vec_changed(camera_.up,camera.up)||
           changed(camera_.vertical_fov_degrees,camera.vertical_fov_degrees)))
            reset_self_avatar_visibility();
        camera_=camera;
        camera_valid_=true;

        if (!test_anchor_initialized_)
        {
            RenderVec3 forward = normalize(camera_.forward);
            if (dot(forward, forward) <= 0.0f)
            {
                forward = {1.0f, 0.0f, 0.0f};
            }

            test_anchor_ = add(
                camera_.position,
                scale(forward, 6.0f));
            test_anchor_initialized_ = true;
        }
    }

    void set_terrain(const RenderTerrain& terrain) override
    {
        if (terrain.generation == terrain_generation_ &&
            terrain.revision == terrain_revision_ &&
            terrain.material_revision == terrain_material_revision_)
        {
            return;
        }

        terrain_generation_ = terrain.generation;
        terrain_revision_ = terrain.revision;
        terrain_material_revision_ = terrain.material_revision;
        terrain_index_count_ = 0U;

        const TerrainMesh mesh = build_terrain_mesh(
            terrain.heights,
            terrain.valid_patches,
            terrain.width,
            terrain.height,
            terrain.patch_size,
            terrain.terrain_start_height,
            terrain.terrain_height_range);

        if (mesh.vertices.empty() || mesh.indices.empty())
        {
            return;
        }

        if (terrain_vao_ == 0U)
        {
            glGenVertexArrays(1, &terrain_vao_);
            glGenBuffers(1, &terrain_vbo_);
            glGenBuffers(1, &terrain_ebo_);
        }

        glBindVertexArray(terrain_vao_);
        glBindBuffer(GL_ARRAY_BUFFER, terrain_vbo_);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                mesh.vertices.size() * sizeof(TerrainVertex)),
            mesh.vertices.data(),
            GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                mesh.indices.size() * sizeof(std::uint32_t)),
            mesh.indices.data(),
            GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0U);
        glVertexAttribPointer(
            0U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(sizeof(TerrainVertex)),
            nullptr);

        glEnableVertexAttribArray(1U);
        glVertexAttribPointer(
            1U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(sizeof(TerrainVertex)),
            reinterpret_cast<void*>(offsetof(TerrainVertex, r)));

        glEnableVertexAttribArray(2U);
        glVertexAttribPointer(
            2U,
            1,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(sizeof(TerrainVertex)),
            reinterpret_cast<void*>(offsetof(TerrainVertex, material)));

        glBindVertexArray(0U);
        terrain_index_count_ =
            static_cast<std::uint32_t>(mesh.indices.size());

        if (water_vao_ == 0U)
        {
            glGenVertexArrays(1, &water_vao_);
            glGenBuffers(1, &water_vbo_);
            glGenBuffers(1, &water_ebo_);
        }

        const float max_x =
            static_cast<float>(terrain.width > 0U ? terrain.width - 1U : 0U);
        const float max_y =
            static_cast<float>(terrain.height > 0U ? terrain.height - 1U : 0U);
        const float z = terrain.water_height;

        const std::array<TerrainVertex, 4> water_vertices{{
            {0.0f, 0.0f, z, 0.05f, 0.38f, 0.68f},
            {max_x, 0.0f, z, 0.05f, 0.38f, 0.68f},
            {max_x, max_y, z, 0.08f, 0.48f, 0.78f},
            {0.0f, max_y, z, 0.08f, 0.48f, 0.78f}
        }};
        constexpr std::array<std::uint32_t, 6> water_indices{
            0U, 1U, 2U, 0U, 2U, 3U
        };

        glBindVertexArray(water_vao_);
        glBindBuffer(GL_ARRAY_BUFFER, water_vbo_);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(water_vertices)),
            water_vertices.data(),
            GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, water_ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(water_indices)),
            water_indices.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(0U);
        glVertexAttribPointer(
            0U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(sizeof(TerrainVertex)),
            nullptr);
        glEnableVertexAttribArray(1U);
        glVertexAttribPointer(
            1U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(sizeof(TerrainVertex)),
            reinterpret_cast<void*>(offsetof(TerrainVertex, r)));

        glBindVertexArray(0U);

        core::info(
            "OGLRender terrain mesh updated; triangles=" +
            std::to_string(terrain_index_count_ / 3U));
    }

    void set_terrain_textures(
        const RenderTerrainTextures& textures) override
    {
        const auto actual_count=static_cast<std::uint32_t>(
            std::count_if(textures.detail.begin(),textures.detail.end(),
                          [](const RenderImageRgba8& img)
                          {return img.valid();}));
        const bool same_revision=
            textures.generation==texture_generation_ &&
            textures.material_revision==texture_material_revision_;
        // Important: the first partially loaded material upload does NOT
        // finish the current revision. Allow more real layers within it;
        // never downgrade an already better GL set.
        if(same_revision&&actual_count<=terrain_real_detail_layers_)return;

        texture_generation_ = textures.generation;
        texture_material_revision_ = textures.material_revision;
        terrain_textures_ready_ = false;
        terrain_real_detail_layers_=0U;

        static constexpr std::array<std::array<std::uint8_t, 4>, 4>
            fallback_colors{{
                {{90U, 78U, 52U, 255U}},
                {{52U, 126U, 58U, 255U}},
                {{104U, 96U, 68U, 255U}},
                {{164U, 164U, 170U, 255U}}
            }};

        if (terrain_textures_[0] == 0U)
        {
            glGenTextures(
                static_cast<GLsizei>(terrain_textures_.size()),
                terrain_textures_.data());
        }

        bool any_real = false;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (std::size_t index = 0U;
             index < terrain_textures_.size();
             ++index)
        {
            glActiveTexture(
                static_cast<GLenum>(GL_TEXTURE0 + index));
            glBindTexture(
                GL_TEXTURE_2D,
                terrain_textures_[index]);

            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_S,
                GL_REPEAT);
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_T,
                GL_REPEAT);
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);

            const auto& image = textures.detail[index];
            if (image.valid())
            {
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RGBA8,
                    static_cast<GLsizei>(image.width),
                    static_cast<GLsizei>(image.height),
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    image.pixels.data());
                any_real = true;
                ++terrain_real_detail_layers_;
            }
            else
            {
                const auto& fallback =
                    fallback_colors[index];
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RGBA8,
                    1,
                    1,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    fallback.data());
            }

            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glBindTexture(GL_TEXTURE_2D, 0U);
        glActiveTexture(GL_TEXTURE0);

        terrain_textures_ready_ = any_real;

        if (terrain_textures_ready_)
        {
            core::info(
                "OGLRender terrain texture set uploaded");
        }
    }

    void remove_object_texture(const std::string& id) override
    {
        const auto it=object_textures_.find(id);
        if(it==object_textures_.end())return;
        if(it->second!=0U)glDeleteTextures(1,&it->second);
        object_textures_.erase(it);
        object_texture_alpha_.erase(id);
    }
    [[nodiscard]] unsigned int avatar_bake_preview_handle(
        const std::string& server_bake_id) const override
    {
        if(server_bake_id.empty())return 0U;
        const auto it=object_textures_.find(server_bake_id);
        return it==object_textures_.end()?0U:it->second;
    }
    void clear_object_textures() override
    {
        for(auto& [id,texture]:object_textures_)
        {
            (void)id;
            if(texture!=0U)glDeleteTextures(1,&texture);
        }
        object_textures_.clear();
        object_texture_alpha_.clear();
    }
    [[nodiscard]] bool set_object_texture(
        const std::string& texture_id,
        const RenderImageRgba8& image) override
    {
        return replace_object_texture(texture_id,image,{}).success;
    }

    [[nodiscard]] GpuUploadResult replace_object_texture(
        const std::string& texture_id,
        const RenderImageRgba8& image,
        const std::vector<std::string>& evictions) override
    {
        if(texture_id.empty() || !image.valid())
            return {false,GpuUploadFailure::invalid_input,0U};

        constexpr std::size_t max_object_textures=512U;
        const auto found=object_textures_.find(texture_id);
        // Count only actual, distinct victim slots; reject attempts to
        // replace the incoming UUID as an "eviction".
        std::unordered_set<std::string> victims;
        for(const auto& id:evictions)
        {
            if(id==texture_id)
                return {false,GpuUploadFailure::invalid_input,0U};
            if(object_textures_.contains(id))victims.insert(id);
        }
        if(found==object_textures_.end() &&
           object_textures_.size()-victims.size() >= max_object_textures)
            return {false,GpuUploadFailure::capacity,0U};

        GLint max_side=0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max_side);
        if(max_side<=0 ||
           image.width>static_cast<std::uint32_t>(max_side) ||
           image.height>static_cast<std::uint32_t>(max_side))
            return {false,GpuUploadFailure::unsupported_dimensions,0U};

        // Important: old textures stay live through the entire GL upload.
        // This temporarily requires extra VRAM for one candidate texture.
        for(int i=0;i<8 && glGetError()!=GL_NO_ERROR;++i) {}
        GLuint candidate=0U;
        glGenTextures(1,&candidate);
        if(candidate==0U)
            return {false,GpuUploadFailure::allocation,0U};

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,candidate);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        if(texture_anisotropy_>1.0f)
            glTexParameterf(GL_TEXTURE_2D,0x84FEU,texture_anisotropy_);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,
            static_cast<GLsizei>(image.width),
            static_cast<GLsizei>(image.height),0,GL_RGBA,
            GL_UNSIGNED_BYTE,image.pixels.data());
        GLenum error=glGetError();
        if(error==GL_NO_ERROR)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            error=glGetError();
        }
        glBindTexture(GL_TEXTURE_2D,0U);
        glActiveTexture(GL_TEXTURE0);
        if(error!=GL_NO_ERROR)
        {
            glDeleteTextures(1,&candidate);
            return {false,
                error==GL_OUT_OF_MEMORY ?
                    GpuUploadFailure::allocation :
                    GpuUploadFailure::driver_error,
                static_cast<unsigned>(error)};
        }

        // Commit happens only after a fully successful GPU upload.
        for(const auto& victim:victims)
            remove_object_texture(victim);
        if(found!=object_textures_.end())
        {
            const GLuint previous=found->second;
            found->second=candidate;
            if(previous!=0U)glDeleteTextures(1,&previous);
        }
        else object_textures_.emplace(texture_id,candidate);
        object_texture_alpha_[texture_id]=texture_has_alpha(image);
        return {true,GpuUploadFailure::none,0U};
    }

    [[nodiscard]] bool set_mesh_asset(
        const std::string& mesh_id,
        const RenderMeshGeometry& geometry) override
    {
        return upload_mesh_asset(mesh_id,geometry).success;
    }
    [[nodiscard]] GpuUploadResult upload_mesh_asset(
        const std::string& mesh_id,
        const RenderMeshGeometry& geometry) override
    {
        if (mesh_id.empty() || geometry.vertices.empty() ||
            geometry.indices.empty() || geometry.vertices.size()%8U!=0U ||
            geometry.indices.size()%3U!=0U)
            return {false,GpuUploadFailure::invalid_input,0U};
        const auto vertex_count=geometry.vertices.size()/8U;
        if (std::any_of(geometry.indices.begin(),geometry.indices.end(),
                [vertex_count](std::uint32_t index)
                {return index>=vertex_count;}))
            return {false,GpuUploadFailure::invalid_input,0U};
        if (std::any_of(geometry.faces.begin(),geometry.faces.end(),
                [&geometry](const RenderMeshFace& face)
                {
                    return face.first_index>geometry.indices.size() ||
                        face.index_count >
                            geometry.indices.size()-face.first_index;
                }))
            return {false,GpuUploadFailure::invalid_input,0U};
        const auto old = mesh_assets_.find(mesh_id);
        if (old == mesh_assets_.end() && mesh_assets_.size() >= 256U)
            return {false,GpuUploadFailure::capacity,0U};
        MeshData mesh;
        mesh.vertices = geometry.vertices;
        mesh.indices = geometry.indices;
        for (const auto& face : geometry.faces)
            mesh.face_ranges.push_back({
                face.face, face.first_index, face.index_count});
        GpuMesh gpu;
        GLenum gpu_error=GL_NO_ERROR;
        if(!upload_mesh(gpu,mesh,&gpu_error))
            return {false,
                gpu_error==GL_OUT_OF_MEMORY
                    ? GpuUploadFailure::allocation
                    : GpuUploadFailure::driver_error,
                static_cast<unsigned>(gpu_error)};
        // Atomic logical replacement: upload first; destroy previous GPU
        // buffers only after the new VAO/VBO/EBO were created successfully.
        // This keeps the old LOD visible if a new upload fails.
        if (old != mesh_assets_.end())
        {
            destroy_mesh(old->second);
            old->second = std::move(gpu);
        }
        else mesh_assets_.emplace(mesh_id, std::move(gpu));
        nexverse::core::info(
            "OGLRender mesh geometry uploaded: triangles=" +
            std::to_string(geometry.indices.size() / 3U));
        return {true,GpuUploadFailure::none,0U};
    }

    void remove_mesh_asset(const std::string& mesh_id) override
    {
        const auto it = mesh_assets_.find(mesh_id);
        if (it == mesh_assets_.end()) return;
        destroy_mesh(it->second);
        mesh_assets_.erase(it);
    }

    void clear_mesh_assets() override
    {
        for (auto& [id, mesh] : mesh_assets_)
        {
            (void)id;
            destroy_mesh(mesh);
        }
        mesh_assets_.clear();
    }

    void set_environment(
        const RenderEnvironment& environment) override
    {
        environment_ = environment;
    }

    void set_scene_objects(
        const std::vector<RenderSceneObject>& objects) override
    {
        constexpr std::size_t max_scene_objects = 50000U;

        scene_objects_.assign(
            objects.begin(),
            objects.begin() +
                static_cast<std::ptrdiff_t>(
                    std::min(objects.size(), max_scene_objects)));

        scene_object_index_.clear();
        scene_object_index_.reserve(scene_objects_.size());

        for (std::size_t index = 0U;
             index < scene_objects_.size();
             ++index)
        {
            if (scene_objects_[index].local_id != 0U)
            {
                scene_object_index_[
                    scene_objects_[index].local_id] = index;
            }
        }
    }

    void set_particle_emitters(
        const std::vector<RenderParticleEmitter>& emitters) override
    {
        constexpr std::size_t max_emitters = 32U;
        particle_emitters_.assign(emitters.begin(),
            emitters.begin() + static_cast<std::ptrdiff_t>(
                std::min(emitters.size(), max_emitters)));
        std::unordered_set<std::uint32_t> active;
        for (const auto& emitter : particle_emitters_)
        {
            active.insert(emitter.local_id);
            auto it = particle_start_times_.find(emitter.local_id);
            if (it != particle_start_times_.end() &&
                it->second.first != emitter.crc)
                particle_start_times_.erase(it);
        }
        for (auto it = particle_start_times_.begin();
             it != particle_start_times_.end();)
        {
            if (!active.contains(it->first))
                it = particle_start_times_.erase(it);
            else
                ++it;
        }
    }

    void set_point_lights(
        const std::vector<RenderPointLight>& lights) override
    {
        constexpr std::size_t max_active_lights = 8U;
        point_lights_.assign(
            lights.begin(),
            lights.begin() + static_cast<std::ptrdiff_t>(
                std::min(lights.size(), max_active_lights)));
    }

    [[nodiscard]] bool set_avatar_body_mesh(
        std::size_t slot,const RenderMeshGeometry& geometry) override
    {
        if(slot>=avatar_body_meshes_.size() ||
           geometry.vertices.size()<24U||
           geometry.vertices.size()%8U!=0U||
           geometry.indices.size()<3U||
           geometry.indices.size()%3U!=0U)
            return false;
        if(!audit_avatar_body_uv(
                geometry.vertices,geometry.indices).safe_for_gpu)
            return false;
        if(!geometry.skin_weights.empty())
        {
            if(geometry.skin_weights.size()!=geometry.vertices.size()/8U ||
               geometry.skin_palette.size()<2U ||
               geometry.skin_palette.size()>32U ||
               std::any_of(geometry.skin_weights.begin(),
                           geometry.skin_weights.end(),
                  [&](float weight)
                  {
                      return !resolve_avatar_skin_weight(
                          weight,geometry.skin_palette.size()).valid;
                  }))
                return false;
        }
        MeshData source;
        source.vertices=geometry.vertices;
        source.indices=geometry.indices;
        GpuMesh candidate;
        if(!upload_mesh(candidate,source))
            return false;
        if(!geometry.skin_weights.empty())
        {
            glBindVertexArray(candidate.vao);
            glGenBuffers(1,&candidate.skin_vbo);
            glBindBuffer(GL_ARRAY_BUFFER,candidate.skin_vbo);
            glBufferData(GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(
                    geometry.skin_weights.size()*sizeof(float)),
                geometry.skin_weights.data(),GL_STATIC_DRAW);
            glEnableVertexAttribArray(4U);
            glVertexAttribPointer(4U,1,GL_FLOAT,GL_FALSE,
                                  sizeof(float),nullptr);
            glBindVertexArray(0U);
            glBindBuffer(GL_ARRAY_BUFFER,0U);
            if(glGetError()!=GL_NO_ERROR)
            {
                destroy_mesh(candidate);
                return false;
            }
            candidate.skin_palette=geometry.skin_palette;
        }
        destroy_mesh(avatar_body_meshes_[slot]);
        avatar_body_meshes_[slot]=std::move(candidate);
        return true;
    }

    void reset_self_avatar_visibility() override
    {
        ++self_avatar_visibility_epoch_;
        self_avatar_samples_known_=false;
        self_avatar_samples_passed_=false;
        self_avatar_part_samples_known_.fill(false);
        self_avatar_part_samples_passed_.fill(false);
        self_avatar_part_resolved_sequence_.fill(0U);
        // Do not reuse unresolved query objects until the GPU reports
        // ready; their epoch tags prevent stale region results.
    }

    void set_avatar_bind_rig(
        const RenderAvatarBindRig& rig) override
    {
        avatar_bind_rig_=rig;
    }

    void set_avatar_proxies(
        const std::vector<RenderAvatarProxy>& avatars) override
    {
        constexpr std::size_t max_avatar_proxies = 64U;
        avatar_proxies_.assign(
            avatars.begin(),
            avatars.begin() + static_cast<std::ptrdiff_t>(
                std::min(avatars.size(), max_avatar_proxies)));
    }

    void begin_frame() override
    {
        diagnostics_.draw_calls = 0U;
        diagnostics_.triangles = 0U;
        diagnostics_.visible_objects = 0U;
        // Terrain GL telemetry is PER FRAME just like all scene counters.
        // A prior textured draw must not make today's fallback look loaded.
        diagnostics_.terrain_textured_draw_calls=0U;
        diagnostics_.terrain_fallback_draw_calls=0U;
        diagnostics_.terrain_real_detail_layers=0U;
        diagnostics_.scene_draw_calls = 0U;
        diagnostics_.scene_mesh_draw_calls = 0U;
        diagnostics_.scene_prim_draw_calls = 0U;
        diagnostics_.scene_opaque_draw_calls = 0U;
        diagnostics_.scene_alpha_draw_calls = 0U;
        diagnostics_.scene_alpha_mask_draw_calls = 0U;
        diagnostics_.scene_missing_mesh_draw_calls = 0U;
        diagnostics_.scene_missing_mesh_objects = 0U;
        diagnostics_.scene_missing_mesh_triangles = 0U;
        diagnostics_.avatar_proxy_draw_calls = 0U;
        diagnostics_.self_avatar_proxy_draw_calls = 0U;
        diagnostics_.avatar_baked_texture_draw_calls = 0U;
        diagnostics_.avatar_anatomical_mesh_draw_calls = 0U;
        diagnostics_.avatar_weighted_skin_draw_calls = 0U;
        diagnostics_.avatar_locally_animated_skin_draw_calls = 0U;
        diagnostics_.avatar_server_selected_skin_draw_calls = 0U;
        diagnostics_.avatar_keyframe_skin_draw_calls = 0U;
        diagnostics_.self_avatar_weighted_skin_draw_calls = 0U;
        diagnostics_.self_avatar_keyframe_skin_draw_calls=0U;
        diagnostics_.self_avatar_server_selected_skin_draw_calls=0U;
        diagnostics_.self_avatar_anatomical_mesh_draw_calls = 0U;
        diagnostics_.self_avatar_baked_texture_draw_calls = 0U;
        diagnostics_.self_avatar_body_bake_gl_draws.fill(0U);
        // Query availability is polled before GL_QUERY_RESULT to avoid
        // stalls. Two concurrent slots per body part are versioned so
        // an older GPU result cannot overtake a newer resolved result.
        for(std::size_t i=0U;i<self_avatar_visibility_queries_.size();++i)
        {
            if(!self_avatar_visibility_pending_[i])continue;
            GLuint available=GL_FALSE;
            glGetQueryObjectuiv(self_avatar_visibility_queries_[i],
                GL_QUERY_RESULT_AVAILABLE,&available);
            if(available!=GL_TRUE)continue;
            GLuint passed=GL_FALSE;
            glGetQueryObjectuiv(self_avatar_visibility_queries_[i],
                GL_QUERY_RESULT,&passed);
            self_avatar_visibility_pending_[i]=false;
            const std::size_t part=i/2U;
            if(self_avatar_visibility_query_epoch_[i]!=
               self_avatar_visibility_epoch_ ||
               self_avatar_visibility_query_sequence_[i]<
               self_avatar_part_resolved_sequence_[part])continue;
            self_avatar_part_resolved_sequence_[part]=
                self_avatar_visibility_query_sequence_[i];
            self_avatar_part_samples_known_[part]=true;
            self_avatar_part_samples_passed_[part]=passed!=GL_FALSE;
        }
        self_avatar_samples_known_=std::any_of(
            self_avatar_part_samples_known_.begin(),
            self_avatar_part_samples_known_.end(),
            [](bool known){return known;});
        self_avatar_samples_passed_=std::any_of(
            self_avatar_part_samples_passed_.begin(),
            self_avatar_part_samples_passed_.end(),
            [](bool visible){return visible;});
        diagnostics_.self_avatar_visible_samples_known=
            self_avatar_samples_known_;
        diagnostics_.self_avatar_visible_samples_passed=
            self_avatar_samples_passed_;
        diagnostics_.self_avatar_part_samples_known=
            self_avatar_part_samples_known_;
        diagnostics_.self_avatar_part_samples_passed=
            self_avatar_part_samples_passed_;

        glViewport(0, 0, width_, height_);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.008f, 0.018f, 0.035f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void render_world(double time_seconds) override
    {
        if (program_ == 0U || sky_program_ == 0U)
        {
            return;
        }

        const RenderEnvironment& env = environment_;
        const RenderVec3 horizon = env.available
            ? env.blue_horizon
            : RenderVec3{0.42f, 0.58f, 0.82f};
        const RenderVec3 zenith = env.available
            ? RenderVec3{
                std::clamp(env.blue_density.x * 0.55f, 0.02f, 1.5f),
                std::clamp(env.blue_density.y * 0.55f, 0.02f, 1.5f),
                std::clamp(env.blue_density.z * 0.55f, 0.02f, 1.5f)}
            : RenderVec3{0.08f, 0.18f, 0.38f};
        const RenderVec3 sun_color = env.available
            ? env.sunlight_color
            : RenderVec3{1.00f, 0.84f, 0.55f};

        const float sun_x = std::clamp(
            0.5f + 0.42f * std::cos(env.east_angle),
            0.06f,
            0.94f);
        const float sun_y = std::clamp(
            0.5f + 0.42f * std::sin(env.sun_angle),
            0.06f,
            0.94f);

        glDisable(GL_DEPTH_TEST);
        glUseProgram(sky_program_);
        glUniform1f(
            glGetUniformLocation(sky_program_, "uTime"),
            static_cast<float>(time_seconds));
        glUniform3f(
            glGetUniformLocation(sky_program_, "uHorizon"),
            horizon.x,
            horizon.y,
            horizon.z);
        glUniform3f(
            glGetUniformLocation(sky_program_, "uZenith"),
            zenith.x,
            zenith.y,
            zenith.z);
        glUniform3f(
            glGetUniformLocation(sky_program_, "uSunColor"),
            sun_color.x,
            sun_color.y,
            sun_color.z);
        glUniform2f(
            glGetUniformLocation(sky_program_, "uSunPosition"),
            sun_x,
            sun_y);
        glUniform1f(
            glGetUniformLocation(sky_program_, "uGamma"),
            env.available ? env.gamma : 1.0f);
        glUniform1f(
            glGetUniformLocation(sky_program_, "uHazeDensity"),
            env.available ? env.haze_density : 0.70f);
        glBindVertexArray(sky_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0U);
        glUseProgram(0U);
        glEnable(GL_DEPTH_TEST);

        diagnostics_.draw_calls += 1U;
        diagnostics_.triangles += 1U;

        const float aspect =
            static_cast<float>(width_) /
            static_cast<float>(height_ > 0 ? height_ : 1);

        const Mat4 projection = perspective(
            camera_.vertical_fov_degrees,
            aspect,
            camera_.near_clip,
            camera_.far_clip);
        const Mat4 view = view_matrix(camera_);

        glUseProgram(program_);
        glUniformMatrix4fv(
            glGetUniformLocation(program_, "uModel"),
            1, GL_FALSE, identity().value.data());
        glUniform1i(
            glGetUniformLocation(program_, "uPointLightCount"), 0);
        glUniform1f(
            glGetUniformLocation(program_, "uAlpha"),
            1.0f);
        glUniform1i(
            glGetUniformLocation(program_, "uUseTerrainTextures"),
            GL_FALSE);
        glUniform1i(
            glGetUniformLocation(program_, "uUsePrimDeform"),
            GL_FALSE);
        glUniform1i(
            glGetUniformLocation(program_, "uOverrideColor"),
            GL_FALSE);
        glUniform3f(
            glGetUniformLocation(program_, "uFogHorizon"),
            horizon.x,
            horizon.y,
            horizon.z);

        if (terrain_index_count_ > 0U && terrain_vao_ != 0U)
        {
            const Mat4 mvp = multiply(projection, view);
            glUniformMatrix4fv(
                glGetUniformLocation(program_, "uMvp"),
                1,
                GL_FALSE,
                mvp.value.data());

            const bool use_textures=terrain_detail_gpu_ready(
                terrain_textures_ready_,texture_generation_,
                terrain_generation_,texture_material_revision_,
                terrain_material_revision_);

            if (use_textures)
            {
                static constexpr std::array<const char*, 4> sampler_names{
                    "uTerrain0",
                    "uTerrain1",
                    "uTerrain2",
                    "uTerrain3"};

                for (std::size_t index = 0U;
                     index < terrain_textures_.size();
                     ++index)
                {
                    glActiveTexture(
                        static_cast<GLenum>(GL_TEXTURE0 + index));
                    glBindTexture(
                        GL_TEXTURE_2D,
                        terrain_textures_[index]);
                    glUniform1i(
                        glGetUniformLocation(
                            program_,
                            sampler_names[index]),
                        static_cast<GLint>(index));
                }

                glUniform1i(
                    glGetUniformLocation(
                        program_,
                        "uUseTerrainTextures"),
                    GL_TRUE);
            }

            glBindVertexArray(terrain_vao_);
            glDrawElements(
                GL_TRIANGLES,
                static_cast<GLsizei>(terrain_index_count_),
                GL_UNSIGNED_INT,
                nullptr);

            diagnostics_.draw_calls += 1U;
            diagnostics_.triangles += terrain_index_count_ / 3U;
            if(use_textures)
            {
                ++diagnostics_.terrain_textured_draw_calls;
                diagnostics_.terrain_real_detail_layers=
                    terrain_real_detail_layers_;
            }
            else
                ++diagnostics_.terrain_fallback_draw_calls;

            glUniform1i(
                glGetUniformLocation(
                    program_,
                    "uUseTerrainTextures"),
                GL_FALSE);
            glActiveTexture(GL_TEXTURE0);

            if (water_vao_ != 0U)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDepthMask(GL_FALSE);

                glUniform1f(
                    glGetUniformLocation(program_, "uAlpha"),
                    0.32f);
                glUniform1i(
                    glGetUniformLocation(program_, "uOverrideColor"),
                    GL_TRUE);
                glUniform3f(
                    glGetUniformLocation(program_, "uOverrideColorValue"),
                    env.water_fog_color.x,
                    env.water_fog_color.y,
                    env.water_fog_color.z);

                glBindVertexArray(water_vao_);
                glDrawElements(
                    GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    nullptr);
                diagnostics_.draw_calls += 1U;
                diagnostics_.triangles += 2U;

                glDepthMask(GL_TRUE);
                glUniform1f(
                    glGetUniformLocation(program_, "uAlpha"),
                    1.0f);
                glUniform1i(
                    glGetUniformLocation(program_, "uOverrideColor"),
                    GL_FALSE);

                glLineWidth(2.0f);
                glDrawArrays(GL_LINE_LOOP, 0, 4);
                diagnostics_.draw_calls += 1U;

                glDisable(GL_BLEND);
            }

            glBindVertexArray(0U);
        }

        if (vao_ != 0U && !scene_objects_.empty())
        {
            const GLsizei active_light_count = static_cast<GLsizei>(
                std::min<std::size_t>(point_lights_.size(), 8U));
            std::array<float, 32U> positions{};
            std::array<float, 32U> colors{};
            std::array<float, 32U> falloffs{};
            for (GLsizei i = 0; i < active_light_count; ++i)
            {
                const auto& light = point_lights_[
                    static_cast<std::size_t>(i)];
                const auto k = static_cast<std::size_t>(i) * 4U;
                positions[k] = light.position.x;
                positions[k+1U] = light.position.y;
                positions[k+2U] = light.position.z;
                positions[k+3U] = std::max(light.radius, 0.01f);
                colors[k] = light.color.x;
                colors[k+1U] = light.color.y;
                colors[k+2U] = light.color.z;
                colors[k+3U] = light.intensity;
                falloffs[k] = light.falloff;
                falloffs[k + 1U] = light.cutoff;
            }
            glUniform3f(
                glGetUniformLocation(program_, "uCameraWorld"),
                camera_.position.x, camera_.position.y,
                camera_.position.z);
            glUniform1i(
                glGetUniformLocation(program_, "uPointLightCount"), active_light_count);
            glUniform4fv(
                glGetUniformLocation(program_, "uPointLightPositions[0]"),
                active_light_count, positions.data());
            glUniform4fv(
                glGetUniformLocation(program_, "uPointLightColors[0]"),
                active_light_count, colors.data());
            glUniform4fv(
                glGetUniformLocation(program_, "uPointLightFalloffs[0]"),
                active_light_count, falloffs.data());
            glUniform1i(
                glGetUniformLocation(program_, "uUseTerrainTextures"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uOverrideColor"),
                GL_TRUE);

            std::vector<VisibleObjectDistance> visible_objects;
            visible_objects.reserve(scene_objects_.size());
            for (std::size_t index = 0U; index < scene_objects_.size(); ++index)
            {
                // Firestorm HUD points (31-38) must never draw as world
                // objects at avatar coordinates. A proper HUD renderer is
                // a separate UI phase; do not leak an offscreen HUD here.
                const auto& entity=scene_objects_[index];
                bool hud=false;
                if(!entity.object_id.empty() && entity.parent_id!=0U)
                {
                    if(const auto* avatar=find_attachment_avatar_parent(
                           avatar_proxies_,entity.parent_id))
                    {
                        const auto attached=avatar->attachments.find(
                            entity.object_id);
                        if(attached!=avatar->attachments.end() &&
                           attached->second<avatar_bind_rig_.is_hud.size())
                            hud=avatar_bind_rig_.is_hud[attached->second];
                    }
                }
                if(hud)continue;
                const Mat4 model = scene_model(index);
                if (!scene_object_visible(model, scene_objects_[index]))
                    continue;
                const float dx = model.value[12] - camera_.position.x;
                const float dy = model.value[13] - camera_.position.y;
                const float dz = model.value[14] - camera_.position.z;
                visible_objects.push_back({index, dx * dx + dy * dy + dz * dz});
            }
            diagnostics_.visible_objects += visible_objects.size();
            std::unordered_set<std::size_t> missing_mesh_object_indices;

            // Draw all opaque surfaces with depth writes before alpha surfaces.
            // Transparent objects are drawn back-to-front, without depth writes.
            for (unsigned pass = 0U; pass < 2U; ++pass)
            {
                if (pass == 1U)
                {
                    sort_transparent_objects(visible_objects);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glDepthMask(GL_FALSE);
                }

                for (const auto& draw : visible_objects)
                {
                const std::size_t index = draw.index;
                const RenderSceneObject& object = scene_objects_[index];
                const Mat4 model = scene_model(index);

                const Mat4 mvp = multiply(
                    projection,
                    multiply(view, model));

                glUniformMatrix4fv(
                    glGetUniformLocation(program_, "uMvp"),
                    1,
                    GL_FALSE,
                    mvp.value.data());
                glUniformMatrix4fv(
                    glGetUniformLocation(program_, "uModel"),
                    1, GL_FALSE, model.value.data());

                glUniform1i(
                    glGetUniformLocation(program_, "uUsePrimDeform"),
                    object.linear_path_deform ? GL_TRUE : GL_FALSE);
                glUniform2f(
                    glGetUniformLocation(program_, "uPrimPathRange"),
                    object.path_begin,
                    object.path_end);
                glUniform2f(
                    glGetUniformLocation(program_, "uPrimTaper"),
                    object.taper_x,
                    object.taper_y);
                glUniform2f(
                    glGetUniformLocation(program_, "uPrimShear"),
                    object.shear_x,
                    object.shear_y);
                glUniform1f(
                    glGetUniformLocation(program_, "uPrimTwistBegin"),
                    object.twist_begin_radians);
                glUniform1f(
                    glGetUniformLocation(program_, "uPrimTwistEnd"),
                    object.twist_end_radians);

                GLuint object_vao = vao_;
                GLsizei object_index_count = 36;
                const std::vector<MeshFaceRange>* object_face_ranges =
                    nullptr;
                const bool using_uploaded_mesh =
                    !object.mesh_id.empty() &&
                    mesh_assets_.contains(object.mesh_id);

                if (using_uploaded_mesh)
                {
                    auto& mesh = mesh_assets_.at(object.mesh_id);
                    object_vao = mesh.vao;
                    object_index_count = mesh.index_count;
                    object_face_ranges = &mesh.face_ranges;
                }
                else if (GpuMesh* sweep_mesh =
                             object.mesh_id.empty()
                                 ? curved_sweep_mesh_for(object) : nullptr;
                    sweep_mesh != nullptr)
                {
                    object_vao = sweep_mesh->vao;
                    object_index_count =
                        sweep_mesh->index_count;
                    object_face_ranges =
                        &sweep_mesh->face_ranges;
                }
                else if (GpuMesh* profile_mesh =
                             profile_mesh_for(object);
                         profile_mesh != nullptr)
                {
                    object_vao = profile_mesh->vao;
                    object_index_count =
                        profile_mesh->index_count;
                    object_face_ranges =
                        &profile_mesh->face_ranges;
                }
                else
                {
                    GpuMesh* static_mesh = nullptr;
                    switch (object.kind)
                    {
                        case RenderPrimitiveKind::cylinder:
                            static_mesh = &cylinder_mesh_;
                            break;
                        case RenderPrimitiveKind::prism:
                            static_mesh = &prism_mesh_;
                            break;
                        case RenderPrimitiveKind::sphere:
                            static_mesh = &sphere_mesh_;
                            break;
                        case RenderPrimitiveKind::torus:
                            static_mesh = &torus_mesh_;
                            break;
                        case RenderPrimitiveKind::box:
                            static_mesh = &box_mesh_;
                            break;
                        default:
                            break;
                    }

                    if (static_mesh != nullptr)
                    {
                        object_vao = static_mesh->vao;
                        object_index_count =
                            static_mesh->index_count;
                        object_face_ranges =
                            &static_mesh->face_ranges;
                    }
                }

                if (object_vao == 0U ||
                    object_index_count <= 0)
                {
                    continue;
                }

                const bool child =
                    object.parent_id != 0U;

                glUniform3f(
                    glGetUniformLocation(
                        program_,
                        "uObjectScale"),
                    std::max(object.scale.x, 0.01f),
                    std::max(object.scale.y, 0.01f),
                    std::max(object.scale.z, 0.01f));

                auto draw_material_range =
                    [&](const RenderFaceMaterial* face_material,
                        GLsizei count,
                        std::size_t first_index)
                    {
                        const bool has_material =
                            face_material != nullptr ||
                            !object.texture_id.empty();

                        const std::string& texture_id =
                            face_material != nullptr
                                ? face_material->texture_id
                                : object.texture_id;

                        const auto& color =
                            face_material != nullptr
                                ? face_material->color
                                : object.color;

                        const float repeat_u =
                            face_material != nullptr
                                ? face_material->repeat_u
                                : object.repeat_u;
                        const float repeat_v =
                            face_material != nullptr
                                ? face_material->repeat_v
                                : object.repeat_v;
                        const float offset_u =
                            face_material != nullptr
                                ? face_material->offset_u
                                : object.offset_u;
                        const float offset_v =
                            face_material != nullptr
                                ? face_material->offset_v
                                : object.offset_v;
                        const float rotation =
                            face_material != nullptr
                                ? face_material->rotation
                                : object.texture_rotation;
                        const bool fullbright =
                            face_material != nullptr
                                ? face_material->fullbright
                                : object.fullbright;
                        const bool texgen_planar =
                            face_material != nullptr
                                ? face_material->texgen_planar
                                : object.texgen_planar;
                        const unsigned shiny =
                            face_material != nullptr
                                ? face_material->shiny
                                : object.shiny;
                        const unsigned bumpiness =
                            face_material != nullptr
                                ? face_material->bumpiness
                                : object.bumpiness;
                        const unsigned alpha_mode =
                            face_material != nullptr
                                ? face_material->diffuse_alpha_mode
                                : object.diffuse_alpha_mode;
                        const unsigned alpha_cutoff =
                            face_material != nullptr
                                ? face_material->alpha_mask_cutoff
                                : object.alpha_mask_cutoff;
                        const float glow =
                            face_material != nullptr
                                ? face_material->glow
                                : object.glow;
                        const RenderLegacyAppearance& legacy =
                            face_material != nullptr
                                ? face_material->legacy
                                : object.legacy;

                        const float color_r =
                            has_material
                                ? color[0]
                                : (child ? 0.48f : 0.58f);
                        const float color_g =
                            has_material
                                ? color[1]
                                : (child ? 0.68f : 0.74f);
                        const float color_b =
                            has_material
                                ? color[2]
                                : (child ? 0.82f : 0.88f);
                        const float alpha =
                            has_material
                                ? std::clamp(
                                    color[3],
                                    0.0f,
                                    1.0f)
                                : 1.0f;

                        bool loaded_texture_has_alpha = false;
                        if (!texture_id.empty())
                        {
                            const auto found_alpha = object_texture_alpha_.find(texture_id);
                            loaded_texture_has_alpha =
                                found_alpha != object_texture_alpha_.end() &&
                                found_alpha->second;
                        }
                        const bool transparent = face_material_requires_blending(
                            alpha,loaded_texture_has_alpha,alpha_mode);
                        if (transparent != (pass == 1U)) return;

                        glUniform3f(
                            glGetUniformLocation(
                                program_,
                                "uOverrideColorValue"),
                            color_r,
                            color_g,
                            color_b);
                        glUniform1f(
                            glGetUniformLocation(
                                program_,
                                "uObjectGlow"),
                            has_material
                                ? std::clamp(
                                    glow,
                                    0.0f,
                                    1.0f)
                                : 0.0f);
                        glUniform1i(
                            glGetUniformLocation(program_, "uObjectShiny"),
                            static_cast<GLint>(std::min(shiny, 3U)));
                        glUniform1i(
                            glGetUniformLocation(program_, "uObjectBumpiness"),
                            static_cast<GLint>(std::min(bumpiness, 31U)));
                        glUniform1i(
                            glGetUniformLocation(program_, "uObjectAlphaMode"),
                            static_cast<GLint>(std::min(alpha_mode, 3U)));
                        glUniform1f(
                            glGetUniformLocation(program_, "uObjectAlphaCutoff"),
                            static_cast<float>(alpha_cutoff) / 255.0f);
                        glUniform1i(
                            glGetUniformLocation(
                                program_,
                                "uObjectFullbright"),
                            fullbright ? GL_TRUE : GL_FALSE);

                        glUniform1i(
                            glGetUniformLocation(
                                program_,
                                "uObjectTexGenPlanar"),
                            texgen_planar
                                ? GL_TRUE
                                : GL_FALSE);

                        bool use_object_texture = false;
                        if (!texture_id.empty())
                        {
                            const auto found_texture =
                                object_textures_.find(texture_id);
                            if (found_texture != object_textures_.end() &&
                                found_texture->second != 0U)
                            {
                                glActiveTexture(GL_TEXTURE4);
                                glBindTexture(
                                    GL_TEXTURE_2D,
                                    found_texture->second);
                                glUniform1i(
                                    glGetUniformLocation(
                                        program_,
                                        "uObjectTexture"),
                                    4);
                                glUniform2f(
                                    glGetUniformLocation(
                                        program_,
                                        "uObjectRepeat"),
                                    repeat_u,
                                    repeat_v);
                                glUniform2f(
                                    glGetUniformLocation(
                                        program_,
                                        "uObjectOffset"),
                                    offset_u,
                                    offset_v);
                                glUniform1f(
                                    glGetUniformLocation(
                                        program_,
                                        "uObjectRotation"),
                                    rotation);
                                use_object_texture = true;
                            }
                        }

                        glUniform1i(
                            glGetUniformLocation(
                                program_,
                                "uUseObjectTexture"),
                            use_object_texture
                                ? GL_TRUE
                                : GL_FALSE);
                        glUniform1f(
                            glGetUniformLocation(
                                program_,
                                "uAlpha"),
                            alpha);

                        const auto bind_legacy_map =
                            [&](const RenderLegacyMap& map, GLenum unit,
                                std::string_view sampler,
                                std::string_view enabled,
                                std::string_view repeat_uniform,
                                std::string_view offset_uniform,
                                std::string_view rotation_uniform) -> bool
                            {
                                const auto found = object_textures_.find(
                                    map.texture_id);
                                const bool loaded = !map.texture_id.empty() &&
                                    found != object_textures_.end() &&
                                    found->second != 0U;
                                glUniform1i(
                                    glGetUniformLocation(program_,
                                        std::string{enabled}.c_str()),
                                    loaded ? GL_TRUE : GL_FALSE);
                                if (!loaded) return false;
                                glActiveTexture(unit);
                                glBindTexture(GL_TEXTURE_2D, found->second);
                                glUniform1i(
                                    glGetUniformLocation(program_,
                                        std::string{sampler}.c_str()),
                                    unit == GL_TEXTURE5 ? 5 : 6);
                                glUniform2f(
                                    glGetUniformLocation(program_,
                                        std::string{repeat_uniform}.c_str()),
                                    map.repeat[0], map.repeat[1]);
                                glUniform2f(
                                    glGetUniformLocation(program_,
                                        std::string{offset_uniform}.c_str()),
                                    map.offset[0], map.offset[1]);
                                glUniform1f(
                                    glGetUniformLocation(program_,
                                        std::string{rotation_uniform}.c_str()),
                                    map.rotation);
                                return true;
                            };
                        const bool use_normal = bind_legacy_map(
                            legacy.normal, GL_TEXTURE5,
                            "uLegacyNormalMap", "uUseLegacyNormalMap",
                            "uLegacyNormalRepeat", "uLegacyNormalOffset",
                            "uLegacyNormalRotation");
                        const bool use_specular = bind_legacy_map(
                            legacy.specular, GL_TEXTURE6,
                            "uLegacySpecularMap", "uUseLegacySpecularMap",
                            "uLegacySpecularRepeat", "uLegacySpecularOffset",
                            "uLegacySpecularRotation");
                        glUniform3f(
                            glGetUniformLocation(program_,
                                "uLegacySpecularColor"),
                            legacy.specular_color[0],
                            legacy.specular_color[1],
                            legacy.specular_color[2]);
                        glUniform1f(
                            glGetUniformLocation(program_,
                                "uLegacySpecularExponent"),
                            static_cast<float>(legacy.specular_exponent) /
                                255.0f);
                        glUniform1f(
                            glGetUniformLocation(program_,
                                "uLegacyEnvironmentIntensity"),
                            static_cast<float>(legacy.environment_intensity) /
                                255.0f);

                        glDrawElements(
                            GL_TRIANGLES,
                            count,
                            GL_UNSIGNED_INT,
                            reinterpret_cast<const void*>(
                                first_index *
                                sizeof(std::uint32_t)));

                        ++diagnostics_.draw_calls;
                        ++diagnostics_.scene_draw_calls;
                        if(pass==1U)++diagnostics_.scene_alpha_draw_calls;
                        else ++diagnostics_.scene_opaque_draw_calls;
                        if(alpha_mode==2U)
                            ++diagnostics_.scene_alpha_mask_draw_calls;
                        if(using_uploaded_mesh)
                            ++diagnostics_.scene_mesh_draw_calls;
                        else
                        {
                            ++diagnostics_.scene_prim_draw_calls;
                            if(!object.mesh_id.empty())
                            {
                                ++diagnostics_.scene_missing_mesh_draw_calls;
                                diagnostics_.scene_missing_mesh_triangles+=
                                    static_cast<std::uint64_t>(count/3);
                                if(missing_mesh_object_indices.insert(index).second)
                                    ++diagnostics_.scene_missing_mesh_objects;
                            }
                        }
                        diagnostics_.triangles +=
                            static_cast<std::uint64_t>(
                                count / 3);

                        if (use_normal)
                        {
                            glActiveTexture(GL_TEXTURE5);
                            glBindTexture(GL_TEXTURE_2D, 0U);
                        }
                        if (use_specular)
                        {
                            glActiveTexture(GL_TEXTURE6);
                            glBindTexture(GL_TEXTURE_2D, 0U);
                        }
                        glActiveTexture(GL_TEXTURE0);
                        if (use_object_texture)
                        {
                            glActiveTexture(GL_TEXTURE4);
                            glBindTexture(
                                GL_TEXTURE_2D,
                                0U);
                            glActiveTexture(GL_TEXTURE0);
                        }
                    };

                glBindVertexArray(object_vao);

                if (object_face_ranges != nullptr &&
                    !object_face_ranges->empty())
                {
                    for (const MeshFaceRange& range :
                         *object_face_ranges)
                    {
                        const RenderFaceMaterial* material =
                            range.face_index <
                                    object.face_materials.size() &&
                            object.face_materials[range.face_index]
                                .has_value()
                                ? &*object.face_materials[range.face_index]
                                : nullptr;

                        draw_material_range(
                            material,
                            static_cast<GLsizei>(range.index_count),
                            range.first_index);
                    }
                }
                else
                {
                    draw_material_range(
                        nullptr,
                        object_index_count,
                        0U);
                }

                } // scene object
            } // opaque / translucent pass
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
            glUniform1i(
                glGetUniformLocation(program_, "uPointLightCount"), 0);

            glUniform1f(
                glGetUniformLocation(program_, "uAlpha"),
                1.0f);
            glUniform1f(
                glGetUniformLocation(program_, "uObjectGlow"),
                0.0f);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectShiny"), 0);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectBumpiness"), 0);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectAlphaMode"), 1);
            glUniform1f(
                glGetUniformLocation(program_, "uObjectAlphaCutoff"), 0.0f);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectFullbright"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUseObjectTexture"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUseLegacyNormalMap"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUseLegacySpecularMap"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectTexGenPlanar"),
                GL_FALSE);
            glBindVertexArray(0U);
            glUniform1i(
                glGetUniformLocation(program_, "uUsePrimDeform"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uOverrideColor"),
                GL_FALSE);
        }

        // Avatar proxy rendering is deliberately separate from object
        // meshes so it follows each server-authoritative avatar transform
        // every frame without rebuilding the scene-object cache.
        if (!avatar_proxies_.empty())
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
            glUniform1i(
                glGetUniformLocation(program_, "uUseTerrainTextures"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUseObjectTexture"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUsePrimDeform"),
                GL_FALSE);
            // Original .llm UVs are NEVER generated using a world-object
            // planar mapping, even when the prior scene used texgen.
            glUniform1i(
                glGetUniformLocation(program_, "uObjectTexGenPlanar"),
                GL_FALSE);
            glUniform3f(
                glGetUniformLocation(program_, "uObjectScale"),
                1.0f,1.0f,1.0f);
            glUniform1i(
                glGetUniformLocation(program_, "uUseLegacyNormalMap"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUseLegacySpecularMap"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectAlphaMode"), 1);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectFullbright"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectShiny"), 0);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectBumpiness"), 0);
            glUniform1f(
                glGetUniformLocation(program_, "uObjectGlow"), 0.0f);
            glUniform1f(
                glGetUniformLocation(program_, "uAlpha"), 1.0f);
            glUniform1i(
                glGetUniformLocation(program_, "uOverrideColor"),
                GL_TRUE);
            struct Part
            {
                RenderVec3 offset;
                RenderVec3 size;
                bool round = false;
                unsigned color = 0U;
            };
            // Simple clearly non-final humanoid with shoulders, head,
            // torso and split legs; it is not an OpenSim avatar skin.
            const std::array<Part, 7U> parts{{
                {{0.0f, 0.0f, 1.58f}, {0.34f, 0.34f, 0.34f}, true, 0U},
                {{0.0f, 0.0f, 1.11f}, {0.37f, 0.54f, 0.68f}, false, 1U},
                {{0.0f, -0.40f, 1.08f}, {0.22f, 0.23f, 0.65f}, false, 2U},
                {{0.0f,  0.40f, 1.08f}, {0.22f, 0.23f, 0.65f}, false, 2U},
                {{0.0f, -0.15f, 0.43f}, {0.25f, 0.24f, 0.79f}, false, 3U},
                {{0.0f,  0.15f, 0.43f}, {0.25f, 0.24f, 0.79f}, false, 3U},
                {{0.0f, 0.0f, 0.77f}, {0.33f, 0.46f, 0.24f}, false, 1U}
            }};
            const bool anatomical_ready=std::all_of(
                avatar_body_meshes_.begin(),avatar_body_meshes_.end(),
                [](const GpuMesh& mesh)
                {return mesh.vao!=0U&&mesh.index_count>0;});
            for (const auto& avatar : avatar_proxies_)
            {
                const Mat4 parent = multiply(
                    translation(avatar.position),
                    quaternion_rotation(avatar.rotation));
                if(anatomical_ready)
                {
                    const auto motion=avatar.motion!=
                        nexverse::protocol::AvatarMotion::idle
                            ? avatar.motion
                            : (avatar.moving
                                ? nexverse::protocol::AvatarMotion::walk
                                : nexverse::protocol::AvatarMotion::idle);
                    const double animation_time=
                        avatar.keyframe_clip &&
                        avatar.keyframe_elapsed_seconds>=0.0f
                            ?static_cast<double>(
                                 avatar.keyframe_elapsed_seconds)
                            :time_seconds;
                    const auto posed=avatar_joint_deformations(
                        avatar_bind_rig_,motion,animation_time,
                        avatar.keyframe_clip.get());
                    // Disable until a specific valid body mesh/palette is
                    // bound, keeping unrelated scene objects unaffected.
                    glUniform1i(glGetUniformLocation(
                        program_,"uAvatarSkinEnabled"),GL_FALSE);
                    // Actual Firestorm avatar_head/upper/lower .llm
                    // meshes have bind-pose world-relative coordinates,
                    // preserved original UVs and normals. Future stages
                    // add skeleton animation/weight-driven deformation;
                    // the original body's triangles already render now.
                    const Mat4 mvp=multiply(
                        projection,multiply(view,parent));
                    glUniformMatrix4fv(
                        glGetUniformLocation(program_,"uMvp"),
                        1,GL_FALSE,mvp.value.data());
                    glUniformMatrix4fv(
                        glGetUniformLocation(program_,"uModel"),
                        1,GL_FALSE,parent.value.data());
                    // Query each of the three original body meshes
                    // separately, never counting the seven-part proxy.
                    // Only one GL_ANY_SAMPLES_PASSED can be active at once.
                    for(std::size_t slot=0U;slot<3U;++slot)
                    {
                        const auto& gpu=avatar_body_meshes_[slot];
                        bool skin_valid=!posed.empty() &&
                            gpu.skin_vbo!=0U &&
                            gpu.skin_palette.size()>=2U &&
                            gpu.skin_palette.size()<=32U;
                        if(skin_valid)
                        {
                            std::array<Mat4,32U> matrices;
                            matrices.fill(identity());
                            for(std::size_t j=0U;
                                j<gpu.skin_palette.size();++j)
                            {
                                const auto bone=gpu.skin_palette[j];
                                if(bone>=posed.size())
                                {
                                    skin_valid=false;
                                    break;
                                }
                                const auto& rest=
                                    avatar_bind_rig_.joints[bone].bind_world;
                                matrices[j]=multiply(posed[bone],
                                    translation({-rest.x,-rest.y,-rest.z}));
                            }
                            if(skin_valid)
                                glUniformMatrix4fv(glGetUniformLocation(
                                    program_,"uAvatarJointPalette[0]"),
                                    32,GL_FALSE,matrices[0].value.data());
                        }
                        glUniform1i(glGetUniformLocation(
                            program_,"uAvatarJointCount"),
                            skin_valid?static_cast<GLint>(
                                gpu.skin_palette.size()):0);
                        glUniform1i(glGetUniformLocation(
                            program_,"uAvatarSkinEnabled"),
                            skin_valid?GL_TRUE:GL_FALSE);
                        const auto& id=avatar.baked_texture_ids[slot];
                        bool baked=false;
                        if(!id.empty())
                        {
                            const auto found=object_textures_.find(id);
                            if(found!=object_textures_.end() &&
                               found->second!=0U)
                            {
                                glActiveTexture(GL_TEXTURE4);
                                glBindTexture(GL_TEXTURE_2D,found->second);
                                glUniform1i(glGetUniformLocation(
                                    program_,"uObjectTexture"),4);
                                baked=true;
                            }
                        }
                        glUniform1i(glGetUniformLocation(
                            program_,"uUseObjectTexture"),
                            baked?GL_TRUE:GL_FALSE);
                        glUniform2f(glGetUniformLocation(
                            program_,"uObjectRepeat"),1.0f,1.0f);
                        glUniform2f(glGetUniformLocation(
                            program_,"uObjectOffset"),0.0f,0.0f);
                        glUniform1f(glGetUniformLocation(
                            program_,"uObjectRotation"),0.0f);
                        glUniform1i(glGetUniformLocation(
                            program_,"uObjectAlphaMode"),2);
                        glUniform1f(glGetUniformLocation(
                            program_,"uObjectAlphaCutoff"),0.015f);
                        const RenderVec3 tint=baked
                            ? RenderVec3{1.0f,1.0f,1.0f}
                            : (slot==1U ? RenderVec3{0.70f,0.67f,0.63f}
                               :RenderVec3{0.83f,0.73f,0.68f});
                        const float shade=avatar.is_self?1.0f:0.75f;
                        glUniform3f(glGetUniformLocation(
                            program_,"uOverrideColorValue"),
                            tint.x*shade,tint.y*shade,tint.z*shade);
                        int visibility_slot=-1;
                        if(avatar.is_self)
                            for(std::size_t i=slot*2U;i<slot*2U+2U;++i)
                                if(!self_avatar_visibility_pending_[i])
                                {
                                    visibility_slot=static_cast<int>(i);
                                    break;
                                }
                        if(visibility_slot>=0)
                            glBeginQuery(GL_ANY_SAMPLES_PASSED,
                                self_avatar_visibility_queries_[
                                    static_cast<std::size_t>(visibility_slot)]);
                        glBindVertexArray(gpu.vao);
                        glDrawElements(GL_TRIANGLES,gpu.index_count,
                                       GL_UNSIGNED_INT,nullptr);
                        if(visibility_slot>=0)
                        {
                            glEndQuery(GL_ANY_SAMPLES_PASSED);
                            const auto i=static_cast<std::size_t>(visibility_slot);
                            self_avatar_visibility_pending_[i]=true;
                            self_avatar_visibility_query_epoch_[i]=
                                self_avatar_visibility_epoch_;
                            self_avatar_visibility_query_sequence_[i]=
                                ++self_avatar_query_sequence_;
                        }
                        ++diagnostics_.draw_calls;
                        ++diagnostics_.avatar_anatomical_mesh_draw_calls;
                        if(avatar.is_self)
                            ++diagnostics_.self_avatar_anatomical_mesh_draw_calls;
                        if(skin_valid)
                        {
                            ++diagnostics_.avatar_weighted_skin_draw_calls;
                            if(motion!=nexverse::protocol::AvatarMotion::idle)
                                ++diagnostics_.avatar_locally_animated_skin_draw_calls;
                            if(avatar.animation_from_server)
                                ++diagnostics_.avatar_server_selected_skin_draw_calls;
                            if(avatar.keyframe_clip)
                                ++diagnostics_.avatar_keyframe_skin_draw_calls;
                            if(avatar.is_self)
                            {
                                ++diagnostics_.self_avatar_weighted_skin_draw_calls;
                                if(avatar.animation_from_server)
                                    ++diagnostics_.self_avatar_server_selected_skin_draw_calls;
                                if(avatar.keyframe_clip)
                                    ++diagnostics_.self_avatar_keyframe_skin_draw_calls;
                            }
                        }
                        if(baked)
                        {
                            ++diagnostics_.avatar_baked_texture_draw_calls;
                            if(avatar.is_self)
                            {
                                ++diagnostics_.self_avatar_baked_texture_draw_calls;
                                // Only original Firestorm .llm anatomical
                                // draws; never count 7-part proxy fallback.
                                ++diagnostics_.self_avatar_body_bake_gl_draws[slot];
                            }
                        }
                        diagnostics_.triangles+=static_cast<std::uint64_t>(
                            gpu.index_count/3);
                    }
                    glUniform1i(glGetUniformLocation(
                        program_,"uAvatarSkinEnabled"),GL_FALSE);
                    continue;
                }
                for (const auto& part : parts)
                {
                    const GpuMesh& gpu = part.round
                        ? sphere_mesh_ : box_mesh_;
                    if (gpu.vao == 0U || gpu.index_count <= 0) continue;
                    const Mat4 model = multiply(
                        parent,
                        multiply(translation(part.offset),
                                 scaling(part.size)));
                    const Mat4 mvp = multiply(
                        projection, multiply(view, model));
                    glUniformMatrix4fv(
                        glGetUniformLocation(program_, "uMvp"),
                        1, GL_FALSE, mvp.value.data());
                    const std::array<RenderVec3, 4> colors{{
                        {0.81f, 0.75f, 0.68f},
                        {0.08f, 0.64f, 0.76f},
                        {0.16f, 0.21f, 0.33f},
                        {0.08f, 0.13f, 0.24f}
                    }};
                    // Transition toward baked Firestorm appearance using
                    // the already resident GetTexture OpenGL cache. Head
                    // -> baked face 8; torso/arms -> 9; legs -> 10. This
                    // is NOT UV-correct skeleton skinning or outfit wear.
                    const std::size_t baked_slot=part.color==0U?0U:
                        part.color==3U?2U:1U;
                    const auto& baked_id=avatar.baked_texture_ids[baked_slot];
                    bool baked=false;
                    if(!baked_id.empty())
                    {
                        const auto tex=object_textures_.find(baked_id);
                        if(tex!=object_textures_.end() && tex->second!=0U)
                        {
                            glActiveTexture(GL_TEXTURE4);
                            glBindTexture(GL_TEXTURE_2D,tex->second);
                            glUniform1i(glGetUniformLocation(
                                program_,"uObjectTexture"),4);
                            baked=true;
                        }
                    }
                    glUniform1i(glGetUniformLocation(
                        program_,"uUseObjectTexture"),
                        baked?GL_TRUE:GL_FALSE);
                    glUniform2f(glGetUniformLocation(
                        program_,"uObjectRepeat"),1.0f,1.0f);
                    glUniform2f(glGetUniformLocation(
                        program_,"uObjectOffset"),0.0f,0.0f);
                    glUniform1f(glGetUniformLocation(
                        program_,"uObjectRotation"),0.0f);
                    glUniform1i(glGetUniformLocation(
                        program_,"uObjectAlphaMode"),2);
                    glUniform1f(glGetUniformLocation(
                        program_,"uObjectAlphaCutoff"),0.015f);
                    const auto tint = baked
                        ? RenderVec3{1.0f,1.0f,1.0f} : colors[part.color];
                    const float shade = avatar.is_self ? 1.0f : 0.72f;
                    glUniform3f(
                        glGetUniformLocation(program_,
                            "uOverrideColorValue"),
                        tint.x * shade, tint.y * shade, tint.z * shade);
                    glBindVertexArray(gpu.vao);
                    glDrawElements(
                        GL_TRIANGLES, gpu.index_count,
                        GL_UNSIGNED_INT, nullptr);
                    diagnostics_.draw_calls++;
                    ++diagnostics_.avatar_proxy_draw_calls;
                    if(avatar.is_self)
                        ++diagnostics_.self_avatar_proxy_draw_calls;
                    if(baked)
                    {
                        ++diagnostics_.avatar_baked_texture_draw_calls;
                        if(avatar.is_self)
                            ++diagnostics_.self_avatar_baked_texture_draw_calls;
                    }
                    diagnostics_.triangles +=
                        static_cast<std::uint64_t>(gpu.index_count / 3);
                }
            }
            glBindVertexArray(0U);
            glUniform1i(glGetUniformLocation(
                program_,"uAvatarSkinEnabled"),GL_FALSE);
            glUniform1i(glGetUniformLocation(
                program_,"uUseObjectTexture"),GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uOverrideColor"),
                GL_FALSE);
            glActiveTexture(GL_TEXTURE0);
        }

        draw_soft_particles(time_seconds, multiply(projection, view));
        if (terrain_index_count_ > 0U || !scene_objects_.empty())
        {
            glUseProgram(0U);
            return;
        }

        if (vao_ == 0U || !test_anchor_initialized_)
        {
            glUseProgram(0U);
            return;
        }

        const float angle =
            static_cast<float>(
                std::fmod(time_seconds * 0.45, 2.0 * std::numbers::pi));

        const Mat4 model = multiply(
            translation(test_anchor_),
            multiply(
                rotation_z(angle),
                rotation_x(angle * 0.6f)));
        const Mat4 mvp =
            multiply(projection, multiply(view, model));

        glUniformMatrix4fv(
            glGetUniformLocation(program_, "uMvp"),
            1,
            GL_FALSE,
            mvp.value.data());

        glBindVertexArray(vao_);
        glDrawElements(
            GL_TRIANGLES,
            36,
            GL_UNSIGNED_INT,
            nullptr);
        glBindVertexArray(0U);
        glUseProgram(0U);

        diagnostics_.draw_calls += 1U;
        diagnostics_.triangles += 12U;
    }

    void end_frame() override
    {
        // If original body drawing stopped, any old "visible" query is
        // no longer current-frame evidence (e.g. region change, proxy).
        if(diagnostics_.self_avatar_anatomical_mesh_draw_calls==0U)
        {
            const bool pending=std::any_of(
                self_avatar_visibility_pending_.begin(),
                self_avatar_visibility_pending_.end(),
                [](bool active){return active;});
            if(self_avatar_samples_known_||pending)
                reset_self_avatar_visibility();
            diagnostics_.self_avatar_visible_samples_known=false;
            diagnostics_.self_avatar_visible_samples_passed=false;
            diagnostics_.self_avatar_part_samples_known.fill(false);
            diagnostics_.self_avatar_part_samples_passed.fill(false);
        }
        refresh_vram();
    }

    void shutdown() override
    {
        if(self_avatar_visibility_queries_[0]!=0U)
            glDeleteQueries(static_cast<GLsizei>(
                self_avatar_visibility_queries_.size()),
                self_avatar_visibility_queries_.data());
        self_avatar_visibility_queries_.fill(0U);
        self_avatar_visibility_pending_.fill(false);
        self_avatar_part_samples_known_.fill(false);
        self_avatar_part_samples_passed_.fill(false);
        self_avatar_part_resolved_sequence_.fill(0U);
        self_avatar_samples_known_=false;
        self_avatar_samples_passed_=false;
        if (ribbon_vbo_ != 0U)
            glDeleteBuffers(1, &ribbon_vbo_);
        if (ribbon_vao_ != 0U)
            glDeleteVertexArrays(1, &ribbon_vao_);
        if (ribbon_program_ != 0U)
            glDeleteProgram(ribbon_program_);
        ribbon_vbo_=0U;
        ribbon_vao_=0U;
        ribbon_program_=0U;
        if (particle_vbo_ != 0U)
            glDeleteBuffers(1, &particle_vbo_);
        if (particle_vao_ != 0U)
            glDeleteVertexArrays(1, &particle_vao_);
        if (particle_program_ != 0U)
            glDeleteProgram(particle_program_);
        particle_vbo_ = 0U;
        particle_vao_ = 0U;
        particle_program_ = 0U;
        destroy_mesh(box_mesh_);
        destroy_mesh(cylinder_mesh_);
        destroy_mesh(prism_mesh_);
        destroy_mesh(sphere_mesh_);
        destroy_mesh(torus_mesh_);
        for(auto& body:avatar_body_meshes_)destroy_mesh(body);

        for (auto& [key, mesh] : profile_mesh_cache_)
        {
            (void)key;
            destroy_mesh(mesh);
        }
        profile_mesh_cache_.clear();

        for (auto& [key, mesh] : curved_sweep_mesh_cache_)
        {
            (void)key;
            destroy_mesh(mesh);
        }
        curved_sweep_mesh_cache_.clear();

        clear_mesh_assets();

        clear_object_textures();

        if (terrain_textures_[0] != 0U)
        {
            glDeleteTextures(
                static_cast<GLsizei>(terrain_textures_.size()),
                terrain_textures_.data());
            terrain_textures_.fill(0U);
        }

        if (terrain_ebo_ != 0U)
        {
            glDeleteBuffers(1, &terrain_ebo_);
            terrain_ebo_ = 0U;
        }
        if (terrain_vbo_ != 0U)
        {
            glDeleteBuffers(1, &terrain_vbo_);
            terrain_vbo_ = 0U;
        }
        if (terrain_vao_ != 0U)
        {
            glDeleteVertexArrays(1, &terrain_vao_);
            terrain_vao_ = 0U;
        }
        if (water_ebo_ != 0U)
        {
            glDeleteBuffers(1, &water_ebo_);
            water_ebo_ = 0U;
        }
        if (water_vbo_ != 0U)
        {
            glDeleteBuffers(1, &water_vbo_);
            water_vbo_ = 0U;
        }
        if (water_vao_ != 0U)
        {
            glDeleteVertexArrays(1, &water_vao_);
            water_vao_ = 0U;
        }

        if (ebo_ != 0U)
        {
            glDeleteBuffers(1, &ebo_);
            ebo_ = 0U;
        }
        if (vbo_ != 0U)
        {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0U;
        }
        if (vao_ != 0U)
        {
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0U;
        }
        if (sky_vao_ != 0U)
        {
            glDeleteVertexArrays(1, &sky_vao_);
            sky_vao_ = 0U;
        }
        if (sky_program_ != 0U)
        {
            glDeleteProgram(sky_program_);
            sky_program_ = 0U;
        }
        if (program_ != 0U)
        {
            glDeleteProgram(program_);
            program_ = 0U;
        }
    }

    [[nodiscard]] RenderDiagnostics diagnostics() const override
    {
        return diagnostics_;
    }

private:
    // This stage renders a deterministic bounded approximation of
    // OpenSim source particles. A full stateful LSL particle engine,
    // texture blending, and target tracking remain separate phases.
    void draw_soft_particles(double seconds, const Mat4& view_projection)
    {
        if (particle_emitters_.empty() || particle_program_ == 0U ||
            particle_vao_ == 0U || !std::isfinite(seconds))
            return;

        // Eight distance bands, rendered back to front. Within each band
        // particles are grouped by GPU texture and blend function.
        // Bounded storage and GL calls avoid pathological emitter storms.
        using BatchKey = std::tuple<int, std::uint8_t, std::uint8_t,
                                    std::string>;
        std::map<BatchKey, std::vector<float>, std::greater<>> batches;
        constexpr std::size_t max_particles = 768U;
        constexpr std::size_t max_batches = 192U;
        const auto hash01 = [](std::uint32_t seed)
        {
            seed ^= seed >> 16U;
            seed *= 0x7feb352dU;
            seed ^= seed >> 15U;
            seed *= 0x846ca68bU;
            seed ^= seed >> 16U;
            return static_cast<float>(seed & 0xffffU) / 65535.0f;
        };
        const render::RenderVec3 local_wind{
            std::sin(static_cast<float>(seconds) * 0.13f) * 1.2f,
            std::cos(static_cast<float>(seconds) * 0.09f) * 0.75f,
            0.0f};
        struct RibbonDraw
        {
            std::string texture_id;
            std::uint8_t src=7U;
            std::uint8_t dst=9U;
            std::vector<float> vertices;
        };
        std::vector<RibbonDraw> ribbons;
        std::size_t count = 0U;
        for (const auto& emitter : particle_emitters_)
        {
            if (count >= max_particles) break;
            if (emitter.max_age <= 0.0f || emitter.burst_rate <= 0.0f ||
                emitter.burst_count == 0U) continue;
            auto [state, inserted] = particle_start_times_.try_emplace(
                emitter.local_id, emitter.crc, seconds);
            (void)inserted;
            const float elapsed = static_cast<float>(
                std::max(0.0, seconds - state->second.second));
            if ((emitter.flags & (particle_ribbon|particle_beam)) != 0U)
            {
                std::vector<RenderVec3> points;
                constexpr std::size_t segments = 15U;
                points.reserve(segments+1U);
                const bool beam = (emitter.flags & particle_beam) != 0U &&
                    emitter.target_available;
                const float limit = std::min(elapsed,emitter.max_age);
                if (beam || limit >= 0.05f)
                {
                    for(std::size_t j=0U;j<=segments;++j)
                    {
                        const float t=static_cast<float>(j)/
                            static_cast<float>(segments);
                        if(beam)
                        {
                            points.push_back({
                                std::lerp(emitter.position.x,emitter.target_position.x,t),
                                std::lerp(emitter.position.y,emitter.target_position.y,t),
                                std::lerp(emitter.position.z,emitter.target_position.z,t)});
                        }
                        else
                        {
                            RenderVec3 acceleration=emitter.acceleration;
                            RenderVec3 velocity{0.0f,0.0f,
                                0.5f*(emitter.speed_min+emitter.speed_max)};
                            if ((emitter.source_flags & 0x01U)!=0U)
                            {
                                acceleration=rotate_particle_vector(
                                    emitter.rotation,acceleration);
                                velocity=rotate_particle_vector(
                                    emitter.rotation,velocity);
                            }
                            const auto sample=sample_particle_motion(
                                emitter.position,velocity,acceleration,
                                local_wind,limit*t,emitter.max_age,emitter.flags,
                                emitter.target_available,emitter.target_position);
                            points.push_back(sample.position);
                        }
                    }
                    auto ribbon_color=emitter.start_color;
                    const float glow=std::clamp(emitter.start_glow,0.0f,1.0f);
                    for (std::size_t c=0U;c<3U;++c)
                        ribbon_color[c]=std::min(1.0f,
                            ribbon_color[c]*(1.0f+glow*0.65f));
                    const auto triangles=build_ribbon_triangles(
                        points,camera_.position,
                        std::clamp(emitter.start_scale,0.02f,2.0f),
                        ribbon_color);
                    if (!triangles.empty() && ribbons.size()<32U)
                        ribbons.push_back({emitter.texture_id,
                            emitter.blend_source,emitter.blend_destination,
                            triangles});
                    if (!triangles.empty())
                        continue; // connected ribbon replaces point sprites
                }
            }

            const auto found = object_textures_.find(emitter.texture_id);
            const std::string texture_key =
                found == object_textures_.end() ? "" : emitter.texture_id;
            const float interval = std::max(0.04f, emitter.burst_rate);
            const auto bursts = static_cast<std::size_t>(
                std::clamp(std::ceil(emitter.max_age / interval),
                    1.0f, 32.0f));
            const auto per_burst = std::min<std::size_t>(
                emitter.burst_count, 16U);
            for (std::size_t burst = 0U; burst < bursts; ++burst)
            {
                if (count >= max_particles) break;
                const auto burst_age = particle_burst_age(
                    elapsed, interval, emitter.max_age,
                    emitter.source_max_age, burst);
                if (!burst_age.has_value()) continue;
                const float age = *burst_age;
                for (std::size_t n = 0U; n < per_burst; ++n)
                {
                    if (count >= max_particles) break;
                    const auto seed = emitter.local_id ^
                        (static_cast<std::uint32_t>(burst) * 9301U) ^
                        (static_cast<std::uint32_t>(n) * 49297U);
                    const float a = hash01(seed + 11U) * 6.2831853f;
                    const float z = hash01(seed + 37U) * 2.0f - 1.0f;
                    const float radial = std::sqrt(std::max(0.0f,
                        1.0f - z * z));
                    const float speed = std::lerp(
                        emitter.speed_min, emitter.speed_max,
                        hash01(seed + 67U));
                    const float radius = emitter.burst_radius *
                        hash01(seed + 93U);
                    RenderVec3 dir{
                        std::cos(a) * radial, std::sin(a) * radial, z};
                    if ((emitter.pattern & 0x01U) != 0U)
                        dir = {0.0f, 0.0f, 0.0f};
                    else if ((emitter.pattern & 0x0cU) != 0U)
                        dir.z = std::abs(z);

                    // Beam: samples lie on the source-to-target span.
                    // Ribbon: samples form a trail of successive ages;
                    // native connected triangle ribbons remain future work.
                    float particle_age = age;
                    if (emitter.target_available &&
                        (emitter.flags & particle_beam) != 0U)
                    {
                        particle_age = emitter.max_age *
                            (static_cast<float>(n) + 0.5f) /
                            static_cast<float>(per_burst);
                    }
                    else if ((emitter.flags & particle_ribbon) != 0U)
                    {
                        particle_age = std::max(0.0f, age -
                            0.045f * static_cast<float>(n));
                    }
                    RenderVec3 velocity{
                        speed * dir.x, speed * dir.y, speed * dir.z};
                    RenderVec3 acceleration = emitter.acceleration;
                    if ((emitter.source_flags & 0x01U) != 0U)
                    {
                        // ObjectRelative lives in system flags (bit 0);
                        // data flags represent particle behavior.
                        velocity = rotate_particle_vector(
                            emitter.rotation, velocity);
                        acceleration = rotate_particle_vector(
                            emitter.rotation, acceleration);
                    }
                    const auto sample = sample_particle_motion(
                        emitter.position, velocity,
                        acceleration, local_wind,
                        particle_age, emitter.max_age, emitter.flags,
                        emitter.target_available,
                        emitter.target_position);
                    RenderVec3 pos = sample.position;
                    if (emitter.target_available &&
                        (emitter.flags & particle_beam) != 0U)
                    {
                        const float t = sample.fraction;
                        pos = {
                            std::lerp(emitter.position.x,
                                emitter.target_position.x, t) +
                                radius * dir.x * 0.08f,
                            std::lerp(emitter.position.y,
                                emitter.target_position.y, t) +
                                radius * dir.y * 0.08f,
                            std::lerp(emitter.position.z,
                                emitter.target_position.z, t) +
                                radius * dir.z * 0.08f};
                    }
                    if (!std::isfinite(pos.x) || !std::isfinite(pos.y) ||
                        !std::isfinite(pos.z)) continue;
                    const auto t = sample.fraction;
                    const bool lerp_color =
                        (emitter.flags & particle_interp_color) != 0U;
                    const bool lerp_scale =
                        (emitter.flags & particle_interp_scale) != 0U;
                    const float glow=std::clamp(std::lerp(
                        emitter.start_glow,emitter.end_glow,t),
                        0.0f,1.0f);
                    const auto lerped_color = [&](std::size_t channel)
                    {
                        const float base=lerp_color
                            ? std::lerp(emitter.start_color[channel],
                                emitter.end_color[channel], t)
                            : emitter.start_color[channel];
                        // Legacy emissive particles retain their own
                        // brightness without full HDR/PBR.
                        return channel==3U ? base :
                            std::min(1.0f,base*(1.0f+glow*0.65f));
                    };
                    const float size = std::clamp(
                        lerp_scale ? std::lerp(
                            emitter.start_scale, emitter.end_scale, t)
                            : emitter.start_scale,
                        0.02f, 4.0f);

                    const float dx = camera_.position.x - pos.x;
                    const float dy = camera_.position.y - pos.y;
                    const float dz = camera_.position.z - pos.z;
                    const float distance = std::sqrt(
                        dx*dx + dy*dy + dz*dz);
                    if (!std::isfinite(distance)) continue;
                    const int band = std::clamp(
                        static_cast<int>(distance / 24.0f), 0, 7);
                    const BatchKey key{
                        band, emitter.blend_source,
                        emitter.blend_destination, texture_key};
                    if (!batches.contains(key) &&
                        batches.size() >= max_batches) continue;
                    auto& vertices = batches[key];
                    vertices.insert(vertices.end(),
                        {pos.x, pos.y, pos.z,
                         lerped_color(0U), lerped_color(1U),
                         lerped_color(2U), lerped_color(3U), size});
                    ++count;
                }
            }
        }
        if (count == 0U && ribbons.empty()) return;

        constexpr std::array<GLenum, 10U> blend_modes{{
            GL_ONE, GL_ZERO, GL_DST_COLOR, GL_SRC_COLOR,
            GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR,
            GL_DST_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA
        }};
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glUseProgram(particle_program_);
        glUniformMatrix4fv(
            glGetUniformLocation(particle_program_, "uViewProjection"),
            1, GL_FALSE, view_projection.value.data());
        glUniform3f(
            glGetUniformLocation(particle_program_, "uCamera"),
            camera_.position.x, camera_.position.y, camera_.position.z);
        glUniform1f(
            glGetUniformLocation(particle_program_, "uViewportHeight"),
            static_cast<float>(height_));
        glUniform1i(
            glGetUniformLocation(particle_program_, "uParticleTexture"), 4);
        glBindVertexArray(particle_vao_);
        glBindBuffer(GL_ARRAY_BUFFER, particle_vbo_);
        glActiveTexture(GL_TEXTURE4);
        for (const auto& [key, vertices] : batches)
        {
            if (vertices.empty()) continue;
            const auto& [band, src, dst, texture_id] = key;
            (void)band;
            if (src >= blend_modes.size() || dst >= blend_modes.size())
                continue;
            glBlendFunc(blend_modes[src], blend_modes[dst]);
            const auto found = object_textures_.find(texture_id);
            const bool textured = !texture_id.empty() &&
                found != object_textures_.end() && found->second != 0U;
            glUniform1i(
                glGetUniformLocation(particle_program_, "uParticleTextured"),
                textured ? GL_TRUE : GL_FALSE);
            glBindTexture(GL_TEXTURE_2D, textured ? found->second : 0U);
            glBufferData(GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                vertices.data(), GL_STREAM_DRAW);
            glDrawArrays(GL_POINTS, 0,
                static_cast<GLsizei>(vertices.size() / 8U));
            ++diagnostics_.draw_calls;
        }
        glBindTexture(GL_TEXTURE_2D, 0U);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(0U);
        glBindBuffer(GL_ARRAY_BUFFER, 0U);
        if (!ribbons.empty() && ribbon_program_ != 0U)
        {
            glActiveTexture(GL_TEXTURE4);
            glUseProgram(ribbon_program_);
            glUniformMatrix4fv(
                glGetUniformLocation(ribbon_program_,"uViewProjection"),
                1,GL_FALSE,view_projection.value.data());
            glUniform1i(
                glGetUniformLocation(ribbon_program_,"uRibbonTexture"),4);
            glBindVertexArray(ribbon_vao_);
            glBindBuffer(GL_ARRAY_BUFFER,ribbon_vbo_);
            for(const auto& ribbon:ribbons)
            {
                if(ribbon.src>=blend_modes.size() ||
                   ribbon.dst>=blend_modes.size())continue;
                glBlendFunc(blend_modes[ribbon.src],blend_modes[ribbon.dst]);
                const auto it=object_textures_.find(ribbon.texture_id);
                const bool textured=it!=object_textures_.end() &&
                    it->second!=0U;
                glUniform1i(
                    glGetUniformLocation(ribbon_program_,"uTextured"),
                    textured ? GL_TRUE:GL_FALSE);
                glBindTexture(GL_TEXTURE_2D,textured?it->second:0U);
                glBufferData(GL_ARRAY_BUFFER,
                    static_cast<GLsizeiptr>(ribbon.vertices.size()*sizeof(float)),
                    ribbon.vertices.data(),GL_STREAM_DRAW);
                glDrawArrays(GL_TRIANGLES,0,
                    static_cast<GLsizei>(ribbon.vertices.size()/9U));
                ++diagnostics_.draw_calls;
                diagnostics_.triangles+=ribbon.vertices.size()/27U;
            }
            glBindTexture(GL_TEXTURE_2D,0U);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(0U);
            glBindBuffer(GL_ARRAY_BUFFER,0U);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_PROGRAM_POINT_SIZE);
        glUseProgram(program_);
    }

    [[nodiscard]] Mat4 scene_pose(
        std::size_t index,
        std::size_t depth = 0U) const
    {
        if (index >= scene_objects_.size())
        {
            return identity();
        }

        const RenderSceneObject& object =
            scene_objects_[index];

        const Mat4 local_pose = multiply(
            translation(object.position),
            quaternion_rotation(object.rotation));

        if (object.parent_id == 0U || depth >= 16U)
        {
            return local_pose;
        }

        const auto found =
            scene_object_index_.find(object.parent_id);
        if (found == scene_object_index_.end() ||
            found->second == index)
        {
            // OpenSim attachments use the avatar's simulator local-id as
            // their root parent. The avatar is intentionally not a prim
            // in scene_object_index_. Resolve the child through the live
            // self/remote avatar transform to follow WASD/teleport movement.
            // Not a skeletal attachment point yet: skeleton/rig is pending.
            if(const auto* avatar=find_attachment_avatar_parent(
                   avatar_proxies_,object.parent_id))
            {
                Mat4 root=multiply(
                    translation(avatar->position),
                    quaternion_rotation(avatar->rotation));
                if(!object.object_id.empty())
                {
                    const auto it=avatar->attachments.find(object.object_id);
                    if(it!=avatar->attachments.end() &&
                       it->second<avatar_bind_rig_.available.size() &&
                       avatar_bind_rig_.available[it->second] &&
                       !avatar_bind_rig_.is_hud[it->second])
                        root=multiply(root,translation(
                            avatar_bind_rig_.attachment_offsets[it->second]));
                }
                return multiply(root,local_pose);
            }
            return local_pose;
        }

        return multiply(
            scene_pose(found->second, depth + 1U),
            local_pose);
    }

    [[nodiscard]] Mat4 scene_model(
        std::size_t index) const
    {
        if (index >= scene_objects_.size())
        {
            return identity();
        }

        const RenderSceneObject& object =
            scene_objects_[index];

        return multiply(
            scene_pose(index),
            scaling({
                std::max(object.scale.x, 0.01f),
                std::max(object.scale.y, 0.01f),
                std::max(object.scale.z, 0.01f)}));
    }

    [[nodiscard]] GpuMesh* curved_sweep_mesh_for(
        const RenderSceneObject& object)
    {
        if (!needs_curved_sweep_mesh(object))
        {
            return nullptr;
        }

        const std::uint64_t key =
            curved_sweep_key(object);

        if (auto found = curved_sweep_mesh_cache_.find(key);
            found != curved_sweep_mesh_cache_.end())
        {
            return &found->second;
        }

        constexpr std::size_t max_curved_sweep_meshes = 256U;
        if (curved_sweep_mesh_cache_.size() >=
            max_curved_sweep_meshes)
        {
            return nullptr;
        }

        const MeshData source =
            make_curved_sweep_mesh(object);

        GpuMesh gpu;
        if (!upload_mesh(gpu, source))
        {
            return nullptr;
        }

        auto [inserted, ok] =
            curved_sweep_mesh_cache_.emplace(
                key,
                std::move(gpu));
        if (!ok)
        {
            return nullptr;
        }

        return &inserted->second;
    }

    [[nodiscard]] GpuMesh* profile_mesh_for(
        const RenderSceneObject& object)
    {
        if (!needs_dynamic_profile_mesh(object))
        {
            return nullptr;
        }

        const std::uint64_t key =
            profile_mesh_key(object);

        if (auto found = profile_mesh_cache_.find(key);
            found != profile_mesh_cache_.end())
        {
            return &found->second;
        }

        constexpr std::size_t max_profile_meshes = 512U;
        if (profile_mesh_cache_.size() >= max_profile_meshes)
        {
            return nullptr;
        }

        const MeshData source =
            make_linear_profile_mesh(
                object.kind,
                object.profile_begin,
                object.profile_end,
                object.profile_hollow,
                object.hollow_profile);

        GpuMesh gpu;
        if (!upload_mesh(gpu, source))
        {
            return nullptr;
        }

        auto [inserted, ok] =
            profile_mesh_cache_.emplace(
                key,
                std::move(gpu));
        if (!ok)
        {
            return nullptr;
        }

        return &inserted->second;
    }

    [[nodiscard]] bool scene_object_visible(
        const Mat4& model,
        const RenderSceneObject& object) const
    {
        return scene_forward_visible(
            model.value[12]-camera_.position.x,
            model.value[13]-camera_.position.y,
            model.value[14]-camera_.position.z,
            object.scale.x,object.scale.y,object.scale.z,
            camera_.forward.x,camera_.forward.y,camera_.forward.z,
            camera_.far_clip);
    }

    void refresh_vram()
    {
        if (!nvx_memory_info_)
        {
            return;
        }

        GLint total_kib = 0;
        GLint available_kib = 0;
        glGetIntegerv(
            gl_gpu_memory_info_total_available_memory_nvx,
            &total_kib);
        glGetIntegerv(
            gl_gpu_memory_info_current_available_vidmem_nvx,
            &available_kib);

        if (total_kib > 0)
        {
            diagnostics_.vram_total_bytes =
                static_cast<std::uint64_t>(total_kib) * 1024U;
        }
        if (available_kib > 0)
        {
            diagnostics_.vram_available_bytes =
                static_cast<std::uint64_t>(available_kib) * 1024U;
        }
    }

    int width_ = 1;
    int height_ = 1;
    GLuint program_ = 0U;
    GLuint sky_program_ = 0U;
    GLuint sky_vao_ = 0U;
    GLuint vao_ = 0U;
    GLuint vbo_ = 0U;
    GLuint ebo_ = 0U;

    GpuMesh box_mesh_;
    GpuMesh cylinder_mesh_;
    GpuMesh prism_mesh_;
    GpuMesh sphere_mesh_;
    GpuMesh torus_mesh_;
    std::array<GpuMesh,3U> avatar_body_meshes_{};
    std::array<GLuint,6U> self_avatar_visibility_queries_{};
    std::array<bool,6U> self_avatar_visibility_pending_{};
    std::array<std::uint64_t,6U> self_avatar_visibility_query_epoch_{};
    std::array<std::uint64_t,6U> self_avatar_visibility_query_sequence_{};
    std::array<std::uint64_t,3U> self_avatar_part_resolved_sequence_{};
    std::array<bool,3U> self_avatar_part_samples_known_{};
    std::array<bool,3U> self_avatar_part_samples_passed_{};
    std::uint64_t self_avatar_query_sequence_=0U;
    std::uint64_t self_avatar_visibility_epoch_=1U;
    bool self_avatar_samples_known_=false;
    bool self_avatar_samples_passed_=false;
    bool camera_valid_=false;

    GLuint terrain_vao_ = 0U;
    GLuint terrain_vbo_ = 0U;
    GLuint terrain_ebo_ = 0U;
    std::uint32_t terrain_index_count_ = 0U;
    std::uint32_t terrain_generation_ = 0U;
    std::uint32_t terrain_revision_ = 0U;
    std::uint32_t terrain_material_revision_ = 0U;

    std::array<GLuint, 4> terrain_textures_{};
    std::uint32_t texture_generation_ = 0U;
    std::uint32_t texture_material_revision_ = 0U;
    bool terrain_textures_ready_ = false;
    std::uint32_t terrain_real_detail_layers_=0U;

    GLuint water_vao_ = 0U;
    GLuint water_vbo_ = 0U;
    GLuint water_ebo_ = 0U;

    GLfloat texture_anisotropy_ = 1.0f;
    bool nvx_memory_info_ = false;
    bool test_anchor_initialized_ = false;
    RenderCamera camera_;
    RenderEnvironment environment_;
    std::vector<RenderSceneObject> scene_objects_;
    std::vector<RenderAvatarProxy> avatar_proxies_;
    RenderAvatarBindRig avatar_bind_rig_{};
    std::vector<RenderPointLight> point_lights_;
    std::vector<RenderParticleEmitter> particle_emitters_;
    std::unordered_map<std::uint32_t,
        std::pair<std::uint32_t, double>> particle_start_times_;
    GLuint particle_program_ = 0U;
    GLuint particle_vao_ = 0U;
    GLuint particle_vbo_ = 0U;
    GLuint ribbon_program_ = 0U;
    GLuint ribbon_vao_ = 0U;
    GLuint ribbon_vbo_ = 0U;
    std::unordered_map<std::uint32_t, std::size_t> scene_object_index_;
    std::unordered_map<std::uint64_t, GpuMesh> profile_mesh_cache_;
    std::unordered_map<std::uint64_t, GpuMesh> curved_sweep_mesh_cache_;
    std::unordered_map<std::string, GpuMesh> mesh_assets_;
    std::unordered_map<std::string, GLuint> object_textures_;
    std::unordered_map<std::string, bool> object_texture_alpha_;
    RenderVec3 test_anchor_;
    RenderDiagnostics diagnostics_;
};

std::unique_ptr<Renderer> create_legacy_opengl_renderer()
{
    return std::make_unique<LegacyOpenGlRenderer>();
}
}
