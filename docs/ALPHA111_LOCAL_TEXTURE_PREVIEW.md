# Alpha.111 – OGL genuine Firestorm local textures and visible F9 previews

Reference: OGL develop-current before block 997d174bbbf0c39d9842b2314e5b63868aa5c494; OpenGenesisLINK Server main 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94; Firestorm_7.2.4 indra/llappearance/llavatarappearancedefines.h/.cpp. Original OpenSim per-region authenticated GetTexture CAPS and existing OGL TextureAssetLoader with PNG and OpenJPEG J2C fallback.

New firestorm_wearable_texture_plan.hpp selects at most 12 unique valid local texture UUIDs from owner-scoped COF-verifiable LLWearable assets. Validates each of 45 original ETextureIndex entries against Firestorm LLWearableType owner and 11 baked-local input groups; excludes 11 baked output entries. Delayed worker replies must match expected texture UUID, RegionEpoch, current authenticated user, COF ID and server COF version. Nonmatching results never become GPU textures.

F9 UI now has two distinct real buttons Wearable-Assets laden and Texturen laden, visible local texture PNG/JPEG2000 RGBA mini thumbnails in 17 Firestorm wearable slot rows, GetTexture status in 10-row footer. Existing texture_mip_image downsamples to <=128x128 before max12 OpenGL resources are created on GUI thread; handles are released on COF/region/new fetch/shutdown. Corrected old bug that merely re-reading the same COF version used to erase all prior LLWearable progress.

Tests cover deduplication, wrong wearer type TE, baked outputs, invalid UUID, 12 limit, stale COF/epoch and REAL J2C -> RGBA -> mip, plus six viewport hitbox checks. Whole C++ build, all CTest, Python tests and actual Linux GPU GL3.3 smoke must pass.

Not a baked wearable: this is real LOCAL INPUT image preview only. Do not describe it as Firestorm layer-composited texture, loaded mesh body outfit, server-applied AvatarAppearance, or authenticated NexusVerse real user E2E. Further work: Firestorm avatar_lad.xml layer stack, alpha mask compositing, self avatar GPU body texture and server supported bake/upload/cache and true Wear/Unwear.
