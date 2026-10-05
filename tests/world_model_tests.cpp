#include "nexverse/world/world_model.hpp"

#include <iostream>
#include <string>

int main()
{
    using namespace nexverse::world;

    WorldModel world;
    if (world.ready())
    {
        std::cerr << "Fresh WorldModel unexpectedly ready\n";
        return 1;
    }

    world.set_grid({"NexVerse", "http://hg.stadt-nexverse.de:80/"});
    world.apply_region({
        0x0000010000000200ULL,
        "NexVerse",
        {128.0f, 128.0f, 25.0f},
        true});
    world.apply_agent({
        "11111111-2222-3333-4444-555555555555",
        "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",
        {128.0f, 128.0f, 25.0f},
        {},
        true});

    if (world.ready())
    {
        std::cerr << "WorldModel became ready before validated world data\n";
        return 2;
    }

    nexverse::protocol::TerrainPatch terrain_patch;
    terrain_patch.x = 2U;
    terrain_patch.y = 3U;
    terrain_patch.heights.fill(42.25f);
    world.apply_terrain_patch(terrain_patch);

    const std::size_t terrain_sample =
        (3U * 16U + 5U) * 256U +
        (2U * 16U + 7U);

    if (!world.ready() ||
        world.terrain().valid_patch_count != 1U ||
        !world.terrain().valid_patches[3U * 16U + 2U] ||
        world.terrain().heights[terrain_sample] != 42.25f)
    {
        std::cerr << "Validated terrain patch did not populate WorldModel\n";
        return 3;
    }

    AvatarState avatar;
    avatar.id = "avatar-1";
    avatar.name = "Test Avatar";
    avatar.position = {130.0f, 128.0f, 25.0f};
    world.upsert_avatar(std::move(avatar));
    world.upsert_object({
        42U,
        "object-uuid",
        {132.0f, 128.0f, 25.0f},
        {}});

    if (world.avatars().size() != 1U || world.objects().size() != 1U)
    {
        std::cerr << "World entity upsert failed\n";
        return 4;
    }

    world.remove_avatar("avatar-1");
    world.remove_object(42U);
    if (!world.avatars().empty() || !world.objects().empty())
    {
        std::cerr << "World entity removal failed\n";
        return 5;
    }

    world.reset();
    if (world.ready() || world.region().connected || world.agent().present)
    {
        std::cerr << "WorldModel reset failed\n";
        return 6;
    }

    std::cout << "WorldModel foundation tests passed\n";
    return 0;
}
