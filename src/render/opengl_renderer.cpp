/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/render/renderer.hpp"
#include "nexverse/render/terrain_mesh.hpp"

#include "nexverse/core/logger.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numbers>
#include <string>
#include <string_view>
#include <unordered_map>

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

struct MeshData
{
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
};

struct GpuMesh
{
    GLuint vao = 0U;
    GLuint vbo = 0U;
    GLuint ebo = 0U;
    GLsizei index_count = 0;
};

void append_vertex(
    MeshData& mesh,
    float x,
    float y,
    float z)
{
    mesh.vertices.insert(
        mesh.vertices.end(),
        {x, y, z, 0.65f, 0.74f, 0.88f});
}

MeshData make_cylinder_mesh(std::uint32_t segments = 24U)
{
    MeshData mesh;
    segments = std::max<std::uint32_t>(segments, 3U);

    for (std::uint32_t i = 0U; i < segments; ++i)
    {
        const float angle =
            (2.0f * std::numbers::pi_v<float>) * static_cast<float>(i) /
            static_cast<float>(segments);
        const float x = 0.5f * std::cos(angle);
        const float y = 0.5f * std::sin(angle);
        append_vertex(mesh, x, y, -0.5f);
        append_vertex(mesh, x, y, 0.5f);
    }

    const std::uint32_t bottom_center =
        static_cast<std::uint32_t>(mesh.vertices.size() / 6U);
    append_vertex(mesh, 0.0f, 0.0f, -0.5f);
    const std::uint32_t top_center =
        static_cast<std::uint32_t>(mesh.vertices.size() / 6U);
    append_vertex(mesh, 0.0f, 0.0f, 0.5f);

    for (std::uint32_t i = 0U; i < segments; ++i)
    {
        const std::uint32_t next = (i + 1U) % segments;
        const std::uint32_t b0 = i * 2U;
        const std::uint32_t t0 = b0 + 1U;
        const std::uint32_t b1 = next * 2U;
        const std::uint32_t t1 = b1 + 1U;

        mesh.indices.insert(
            mesh.indices.end(),
            {b0, b1, t1, b0, t1, t0});

        mesh.indices.insert(
            mesh.indices.end(),
            {bottom_center, b1, b0});
        mesh.indices.insert(
            mesh.indices.end(),
            {top_center, t0, t1});
    }

    return mesh;
}

MeshData make_prism_mesh()
{
    MeshData mesh;

    append_vertex(mesh, 0.0f, -0.57735f, -0.5f);
    append_vertex(mesh, 0.5f, 0.288675f, -0.5f);
    append_vertex(mesh, -0.5f, 0.288675f, -0.5f);
    append_vertex(mesh, 0.0f, -0.57735f, 0.5f);
    append_vertex(mesh, 0.5f, 0.288675f, 0.5f);
    append_vertex(mesh, -0.5f, 0.288675f, 0.5f);

    mesh.indices = {
        0U, 2U, 1U,
        3U, 4U, 5U,
        0U, 1U, 4U, 0U, 4U, 3U,
        1U, 2U, 5U, 1U, 5U, 4U,
        2U, 0U, 3U, 2U, 3U, 5U};

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
                z);
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
                minor_radius * std::sin(minor_angle));
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

std::uint32_t append_profile_vertex(
    MeshData& mesh,
    ProfilePoint point,
    float z,
    float scale_amount = 1.0f)
{
    const std::uint32_t index =
        static_cast<std::uint32_t>(
            mesh.vertices.size() / 6U);
    append_vertex(
        mesh,
        point.x * scale_amount,
        point.y * scale_amount,
        z);
    return index;
}

void append_quad(
    MeshData& mesh,
    std::uint32_t a,
    std::uint32_t b,
    std::uint32_t c,
    std::uint32_t d)
{
    mesh.indices.insert(
        mesh.indices.end(),
        {a, b, c, a, c, d});
}

MeshData make_linear_profile_mesh(
    RenderPrimitiveKind kind,
    float profile_begin,
    float profile_end,
    float hollow,
    RenderProfileKind hollow_kind)
{
    MeshData mesh;

    const RenderProfileKind outer =
        outer_profile_kind(kind);
    const RenderProfileKind inner =
        hollow_kind == RenderProfileKind::same
            ? outer
            : hollow_kind;

    const float begin =
        std::clamp(profile_begin, 0.0f, 0.9998f);
    const float end =
        std::clamp(
            profile_end,
            begin + 0.0001f,
            1.0f);
    const float hollow_amount =
        std::clamp(hollow, 0.0f, 0.95f);
    const bool has_hollow = hollow_amount > 0.0001f;
    const bool full_profile =
        begin <= 0.0001f && end >= 0.9999f;

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

    std::vector<std::uint32_t> outer_bottom;
    std::vector<std::uint32_t> outer_top;
    std::vector<std::uint32_t> inner_bottom;
    std::vector<std::uint32_t> inner_top;

    outer_bottom.reserve(parameters.size());
    outer_top.reserve(parameters.size());
    inner_bottom.reserve(parameters.size());
    inner_top.reserve(parameters.size());

    for (float parameter : parameters)
    {
        const ProfilePoint outer_point =
            profile_point(outer, parameter);
        outer_bottom.push_back(
            append_profile_vertex(
                mesh,
                outer_point,
                -0.5f));
        outer_top.push_back(
            append_profile_vertex(
                mesh,
                outer_point,
                0.5f));

        if (has_hollow)
        {
            const ProfilePoint inner_point =
                profile_point(inner, parameter);
            inner_bottom.push_back(
                append_profile_vertex(
                    mesh,
                    inner_point,
                    -0.5f,
                    hollow_amount));
            inner_top.push_back(
                append_profile_vertex(
                    mesh,
                    inner_point,
                    0.5f,
                    hollow_amount));
        }
    }

    for (std::size_t index = 0U;
         index + 1U < parameters.size();
         ++index)
    {
        const std::size_t next = index + 1U;

        append_quad(
            mesh,
            outer_bottom[index],
            outer_bottom[next],
            outer_top[next],
            outer_top[index]);

        if (has_hollow)
        {
            append_quad(
                mesh,
                inner_bottom[next],
                inner_bottom[index],
                inner_top[index],
                inner_top[next]);

            append_quad(
                mesh,
                outer_top[index],
                outer_top[next],
                inner_top[next],
                inner_top[index]);

            append_quad(
                mesh,
                outer_bottom[next],
                outer_bottom[index],
                inner_bottom[index],
                inner_bottom[next]);
        }
    }

    if (!has_hollow)
    {
        const std::uint32_t bottom_center =
            append_profile_vertex(
                mesh,
                {0.0f, 0.0f},
                -0.5f);
        const std::uint32_t top_center =
            append_profile_vertex(
                mesh,
                {0.0f, 0.0f},
                0.5f);

        for (std::size_t index = 0U;
             index + 1U < parameters.size();
             ++index)
        {
            const std::size_t next = index + 1U;
            mesh.indices.insert(
                mesh.indices.end(),
                {
                    bottom_center,
                    outer_bottom[next],
                    outer_bottom[index],
                    top_center,
                    outer_top[index],
                    outer_top[next]
                });
        }

        if (!full_profile)
        {
            append_quad(
                mesh,
                bottom_center,
                outer_bottom.front(),
                outer_top.front(),
                top_center);
            append_quad(
                mesh,
                bottom_center,
                top_center,
                outer_top.back(),
                outer_bottom.back());
        }
    }
    else if (!full_profile)
    {
        append_quad(
            mesh,
            outer_bottom.front(),
            inner_bottom.front(),
            inner_top.front(),
            outer_top.front());
        append_quad(
            mesh,
            outer_bottom.back(),
            outer_top.back(),
            inner_top.back(),
            inner_bottom.back());
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
                z_offset + pz);
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

    const bool closed_path =
        path_begin <= 0.0001f &&
        path_end >= 0.9999f &&
        std::abs(
            revolutions -
            std::round(revolutions)) <
            0.0001f;

    if (!closed_path)
    {
        const std::uint32_t start_center =
            static_cast<std::uint32_t>(
                mesh.vertices.size() / 6U);
        append_vertex(
            mesh,
            mesh.vertices[0],
            mesh.vertices[1],
            mesh.vertices[2]);

        const std::uint32_t end_base =
            path_segments * row;
        const std::uint32_t end_center =
            static_cast<std::uint32_t>(
                mesh.vertices.size() / 6U);
        append_vertex(
            mesh,
            mesh.vertices[
                static_cast<std::size_t>(
                    end_base) * 6U],
            mesh.vertices[
                static_cast<std::size_t>(
                    end_base) * 6U + 1U],
            mesh.vertices[
                static_cast<std::size_t>(
                    end_base) * 6U + 2U]);

        for (std::uint32_t profile_index = 0U;
             profile_index < profile_segments;
             ++profile_index)
        {
            mesh.indices.insert(
                mesh.indices.end(),
                {
                    start_center,
                    profile_index + 1U,
                    profile_index
                });

            mesh.indices.insert(
                mesh.indices.end(),
                {
                    end_center,
                    end_base + profile_index,
                    end_base + profile_index + 1U
                });
        }
    }

    return mesh;
}

bool upload_mesh(
    GpuMesh& target,
    const MeshData& source)
{
    if (source.vertices.empty() || source.indices.empty())
    {
        return false;
    }

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

    target.index_count =
        static_cast<GLsizei>(source.indices.size());
    return true;
}

void destroy_mesh(GpuMesh& mesh)
{
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

uniform mat4 uMvp;
uniform bool uUsePrimDeform;
uniform vec2 uPrimPathRange;
uniform vec2 uPrimTaper;
uniform vec2 uPrimShear;
uniform float uPrimTwistBegin;
uniform float uPrimTwistEnd;

out vec3 vColor;
out vec2 vWorldXY;
out vec3 vObjectLocal;
out float vMaterial;

void main()
{
    vec3 localPosition = aPosition;

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
    vMaterial = aMaterial;
}
)GLSL";

constexpr const char* fragment_shader_source = R"GLSL(
#version 330 core
in vec3 vColor;
in vec2 vWorldXY;
in vec3 vObjectLocal;
in float vMaterial;
out vec4 FragColor;

uniform float uAlpha;
uniform bool uUseTerrainTextures;
uniform bool uOverrideColor;
uniform vec3 uOverrideColorValue;
uniform float uObjectGlow;
uniform bool uObjectFullbright;
uniform bool uUseObjectTexture;
uniform vec2 uObjectRepeat;
uniform vec2 uObjectOffset;
uniform float uObjectRotation;
uniform sampler2D uObjectTexture;
uniform vec3 uFogHorizon;
uniform sampler2D uTerrain0;
uniform sampler2D uTerrain1;
uniform sampler2D uTerrain2;
uniform sampler2D uTerrain3;

vec2 objectProjectedUv()
{
    vec3 dx = dFdx(vObjectLocal);
    vec3 dy = dFdy(vObjectLocal);
    vec3 normal = abs(normalize(cross(dx, dy)));

    vec2 uv;
    if (normal.z >= normal.x && normal.z >= normal.y)
    {
        uv = vObjectLocal.xy + vec2(0.5);
    }
    else if (normal.x >= normal.y)
    {
        uv = vObjectLocal.yz + vec2(0.5);
    }
    else
    {
        uv = vObjectLocal.xz + vec2(0.5);
    }

    uv *= uObjectRepeat;

    float c = cos(uObjectRotation);
    float s = sin(uObjectRotation);
    vec2 centered = uv - vec2(0.5);
    uv = mat2(c, -s, s, c) * centered + vec2(0.5);
    uv += uObjectOffset;
    return uv;
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
        base = mix(textureColor, vColor, 0.12);
    }

    if (uOverrideColor)
    {
        base = uOverrideColorValue;

        if (uUseObjectTexture)
        {
            vec4 texel = texture(
                uObjectTexture,
                objectProjectedUv());
            base *= texel.rgb;
            alpha *= texel.a;
        }

        base += vec3(clamp(uObjectGlow, 0.0, 1.0) * 0.35);
    }

    float fog = uObjectFullbright
        ? 0.0
        : smoothstep(0.90, 1.0, gl_FragCoord.z);
    vec3 color = mix(base, uFogHorizon, fog * 0.55);
    FragColor = vec4(color, alpha);
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

        program_ = create_program();
        sky_program_ = create_sky_program();
        if (program_ == 0U || sky_program_ == 0U)
        {
            return false;
        }

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

        if (!upload_mesh(cylinder_mesh_, make_cylinder_mesh()) ||
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
        camera_ = camera;

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
        if (textures.generation == texture_generation_ &&
            textures.material_revision == texture_material_revision_)
        {
            return;
        }

        texture_generation_ = textures.generation;
        texture_material_revision_ = textures.material_revision;
        terrain_textures_ready_ = false;

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

    void set_object_texture(
        const std::string& texture_id,
        const RenderImageRgba8& image) override
    {
        if (texture_id.empty() || !image.valid())
        {
            return;
        }

        constexpr std::size_t max_object_textures = 512U;

        auto found = object_textures_.find(texture_id);
        if (found == object_textures_.end() &&
            object_textures_.size() >= max_object_textures)
        {
            return;
        }

        GLuint texture = 0U;
        if (found != object_textures_.end())
        {
            texture = found->second;
        }
        else
        {
            glGenTextures(1, &texture);
            if (texture == 0U)
            {
                return;
            }
            object_textures_.emplace(texture_id, texture);
        }

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0U);
        glActiveTexture(GL_TEXTURE0);
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

    void begin_frame() override
    {
        diagnostics_.draw_calls = 0U;
        diagnostics_.triangles = 0U;
        diagnostics_.visible_objects = 0U;

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

            const bool use_textures =
                terrain_textures_ready_ &&
                texture_generation_ == terrain_generation_ &&
                texture_material_revision_ == terrain_material_revision_;

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
            glUniform1i(
                glGetUniformLocation(program_, "uUseTerrainTextures"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uOverrideColor"),
                GL_TRUE);

            for (std::size_t index = 0U;
                 index < scene_objects_.size();
                 ++index)
            {
                const RenderSceneObject& object =
                    scene_objects_[index];

                const Mat4 model = scene_model(index);
                if (!scene_object_visible(model, object))
                {
                    continue;
                }

                const Mat4 mvp = multiply(
                    projection,
                    multiply(view, model));

                glUniformMatrix4fv(
                    glGetUniformLocation(program_, "uMvp"),
                    1,
                    GL_FALSE,
                    mvp.value.data());

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

                const bool has_texture_entry =
                    !object.texture_id.empty();
                const bool child =
                    object.parent_id != 0U;

                const float color_r =
                    has_texture_entry
                        ? object.color[0]
                        : (child ? 0.48f : 0.58f);
                const float color_g =
                    has_texture_entry
                        ? object.color[1]
                        : (child ? 0.68f : 0.74f);
                const float color_b =
                    has_texture_entry
                        ? object.color[2]
                        : (child ? 0.82f : 0.88f);
                const float alpha =
                    has_texture_entry
                        ? std::clamp(
                            object.color[3],
                            0.0f,
                            1.0f)
                        : 1.0f;

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
                    has_texture_entry
                        ? std::clamp(
                            object.glow,
                            0.0f,
                            1.0f)
                        : 0.0f);
                glUniform1i(
                    glGetUniformLocation(
                        program_,
                        "uObjectFullbright"),
                    object.fullbright ? GL_TRUE : GL_FALSE);

                bool use_object_texture = false;
                if (has_texture_entry)
                {
                    const auto found_texture =
                        object_textures_.find(object.texture_id);
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
                            object.repeat_u,
                            object.repeat_v);
                        glUniform2f(
                            glGetUniformLocation(
                                program_,
                                "uObjectOffset"),
                            object.offset_u,
                            object.offset_v);
                        glUniform1f(
                            glGetUniformLocation(
                                program_,
                                "uObjectRotation"),
                            object.texture_rotation);
                        use_object_texture = true;
                    }
                }

                glUniform1i(
                    glGetUniformLocation(
                        program_,
                        "uUseObjectTexture"),
                    use_object_texture ? GL_TRUE : GL_FALSE);
                glUniform1f(
                    glGetUniformLocation(
                        program_,
                        "uAlpha"),
                    alpha);

                const bool transparent =
                    alpha < 0.999f ||
                    use_object_texture;

                GLuint object_vao = vao_;
                GLsizei object_index_count = 36;

                if (GpuMesh* sweep_mesh =
                        curved_sweep_mesh_for(object);
                    sweep_mesh != nullptr)
                {
                    object_vao = sweep_mesh->vao;
                    object_index_count =
                        sweep_mesh->index_count;
                }
                else if (GpuMesh* profile_mesh =
                             profile_mesh_for(object);
                         profile_mesh != nullptr)
                {
                    object_vao = profile_mesh->vao;
                    object_index_count =
                        profile_mesh->index_count;
                }
                else
                {
                    switch (object.kind)
                    {
                        case RenderPrimitiveKind::cylinder:
                            object_vao = cylinder_mesh_.vao;
                            object_index_count =
                                cylinder_mesh_.index_count;
                            break;
                        case RenderPrimitiveKind::prism:
                            object_vao = prism_mesh_.vao;
                            object_index_count =
                                prism_mesh_.index_count;
                            break;
                        case RenderPrimitiveKind::sphere:
                            object_vao = sphere_mesh_.vao;
                            object_index_count =
                                sphere_mesh_.index_count;
                            break;
                        case RenderPrimitiveKind::torus:
                            object_vao = torus_mesh_.vao;
                            object_index_count =
                                torus_mesh_.index_count;
                            break;
                        case RenderPrimitiveKind::box:
                        default:
                            break;
                    }
                }

                if (object_vao == 0U ||
                    object_index_count <= 0)
                {
                    continue;
                }

                if (transparent)
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(
                        GL_SRC_ALPHA,
                        GL_ONE_MINUS_SRC_ALPHA);
                    glDepthMask(GL_FALSE);
                }

                glBindVertexArray(object_vao);
                glDrawElements(
                    GL_TRIANGLES,
                    object_index_count,
                    GL_UNSIGNED_INT,
                    nullptr);

                ++diagnostics_.draw_calls;
                diagnostics_.triangles +=
                    static_cast<std::uint64_t>(
                        object_index_count / 3);
                ++diagnostics_.visible_objects;

                if (transparent)
                {
                    glDepthMask(GL_TRUE);
                    glDisable(GL_BLEND);
                }

                if (use_object_texture)
                {
                    glActiveTexture(GL_TEXTURE4);
                    glBindTexture(GL_TEXTURE_2D, 0U);
                    glActiveTexture(GL_TEXTURE0);
                }
            }

            glUniform1f(
                glGetUniformLocation(program_, "uAlpha"),
                1.0f);
            glUniform1f(
                glGetUniformLocation(program_, "uObjectGlow"),
                0.0f);
            glUniform1i(
                glGetUniformLocation(program_, "uObjectFullbright"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uUseObjectTexture"),
                GL_FALSE);
            glBindVertexArray(0U);
            glUniform1i(
                glGetUniformLocation(program_, "uUsePrimDeform"),
                GL_FALSE);
            glUniform1i(
                glGetUniformLocation(program_, "uOverrideColor"),
                GL_FALSE);
        }

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
        refresh_vram();
    }

    void shutdown() override
    {
        destroy_mesh(cylinder_mesh_);
        destroy_mesh(prism_mesh_);
        destroy_mesh(sphere_mesh_);
        destroy_mesh(torus_mesh_);

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

        for (auto& [id, texture] : object_textures_)
        {
            (void)id;
            if (texture != 0U)
            {
                glDeleteTextures(1, &texture);
            }
        }
        object_textures_.clear();

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
        const float dx =
            model.value[12] - camera_.position.x;
        const float dy =
            model.value[13] - camera_.position.y;
        const float dz =
            model.value[14] - camera_.position.z;

        const float distance_squared =
            dx * dx + dy * dy + dz * dz;

        const float radius =
            0.5f * std::sqrt(
                object.scale.x * object.scale.x +
                object.scale.y * object.scale.y +
                object.scale.z * object.scale.z);

        const float limit =
            std::max(camera_.far_clip, 1.0f) +
            std::max(radius, 0.5f);

        return distance_squared <= limit * limit;
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

    GpuMesh cylinder_mesh_;
    GpuMesh prism_mesh_;
    GpuMesh sphere_mesh_;
    GpuMesh torus_mesh_;

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

    GLuint water_vao_ = 0U;
    GLuint water_vbo_ = 0U;
    GLuint water_ebo_ = 0U;

    bool nvx_memory_info_ = false;
    bool test_anchor_initialized_ = false;
    RenderCamera camera_;
    RenderEnvironment environment_;
    std::vector<RenderSceneObject> scene_objects_;
    std::unordered_map<std::uint32_t, std::size_t> scene_object_index_;
    std::unordered_map<std::uint64_t, GpuMesh> profile_mesh_cache_;
    std::unordered_map<std::uint64_t, GpuMesh> curved_sweep_mesh_cache_;
    std::unordered_map<std::string, GLuint> object_textures_;
    RenderVec3 test_anchor_;
    RenderDiagnostics diagnostics_;
};

std::unique_ptr<Renderer> create_legacy_opengl_renderer()
{
    return std::make_unique<LegacyOpenGlRenderer>();
}
}
