#include "nexverse/assets/mesh_residency.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>
#include <vector>

using nexverse::assets::MeshDistanceSample;
using nexverse::assets::plan_mesh_residency;

int main()
{
    std::vector<MeshDistanceSample> objects;
    objects.reserve(320U);
    for (int i = 0; i < 320; ++i)
    {
        objects.push_back({
            "mesh-" + std::to_string(i),
            static_cast<float>(i), 0.0f, 0.0f});
    }
    // A linked prim using the same mesh must not waste GPU budget.
    objects.push_back({"mesh-0", 900.0f, 0.0f, 0.0f});
    objects.push_back({"bad-nan", std::numeric_limits<float>::quiet_NaN(), 0, 0});
    objects.push_back({"", 0, 0, 0});

    auto initial = plan_mesh_residency(objects, 0.0f, 0.0f, 0.0f, {}, 256U);
    if (initial.prioritized_ids.size() != 256U ||
        initial.desired_ids.size() != 256U ||
        !initial.desired_ids.contains("mesh-0") ||
        !initial.desired_ids.contains("mesh-255") ||
        initial.desired_ids.contains("mesh-319") ||
        initial.desired_ids.contains("bad-nan") ||
        !initial.eviction_ids.empty())
    {
        std::cerr << "Initial proximity priorities incorrect\n";
        return 1;
    }

    std::unordered_set<std::string> loaded{
        initial.desired_ids.begin(), initial.desired_ids.end()};
    auto after_move =
        plan_mesh_residency(objects, 319.0f, 0.0f, 0.0f, loaded, 256U);
    if (after_move.prioritized_ids.size() != 256U ||
        !after_move.desired_ids.contains("mesh-319") ||
        after_move.desired_ids.contains("mesh-0") ||
        after_move.eviction_ids.empty())
    {
        std::cerr << "Moving avatar did not evict distant assets\n";
        return 2;
    }
    for (const auto& id : after_move.eviction_ids)
    {
        if (!loaded.contains(id) || after_move.desired_ids.contains(id))
        {
            std::cerr << "Evicted an active or nonexistent asset\n";
            return 3;
        }
    }

    // A distance change much smaller than the cache hysteresis must
    // not churn already-resident assets at the selection boundary.
    auto stable = plan_mesh_residency(
        {{"resident", 99.0f, 0.0f, 0.0f},
         {"new", 95.0f, 0.0f, 0.0f}},
        0.0f, 0.0f, 0.0f, {"resident"}, 1U, 12.0f);
    if (stable.prioritized_ids.size() != 1U ||
        stable.prioritized_ids[0] != "resident" ||
        !stable.eviction_ids.empty())
    {
        std::cerr << "Loaded mesh hysteresis failed\n";
        return 4;
    }
    auto far = plan_mesh_residency(
        {{"resident", 199.0f, 0.0f, 0.0f},
         {"new", 95.0f, 0.0f, 0.0f}},
        0.0f, 0.0f, 0.0f, {"resident"}, 1U, 12.0f);
    if (far.prioritized_ids[0] != "new" ||
        far.eviction_ids.size() != 1U ||
        far.eviction_ids[0] != "resident")
    {
        std::cerr << "Distant mesh did not become evictable\n";
        return 5;
    }

    const auto none = plan_mesh_residency(
        objects, 0, 0, 0, {"unrelated"}, 0U);
    if (!none.prioritized_ids.empty() ||
        none.eviction_ids.size() != 1U ||
        none.eviction_ids[0] != "unrelated")
    {
        std::cerr << "Zero residency budget not enforced\n";
        return 6;
    }

    auto tie = plan_mesh_residency(
        {{"b", 1, 0, 0}, {"a", -1, 0, 0}},
        0, 0, 0, {}, 2U);
    if (tie.prioritized_ids[0] != "a" ||
        tie.prioritized_ids[1] != "b")
    {
        std::cerr << "Priorities not deterministic on equal distance\n";
        return 7;
    }

    // Apparent size can outweigh pure distance; this changes asset
    // priority, not the actual OpenSim LOD decoder.
    const auto size_rank=plan_mesh_residency({
        {"tiny-near",15,0,0,0.5f},
        {"large-far",25,0,0,16.0f}},
        0,0,0,{},1U);
    if(size_rank.prioritized_ids[0]!="large-far")
    {
        std::cerr << "Screen-size mesh asset heuristic failed\n";
        return 8;
    }

    std::cout << "Mesh residency, movement, dedup and eviction tests passed\n";
    return 0;
}
