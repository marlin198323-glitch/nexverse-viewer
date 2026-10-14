# Alpha.109 – Firestorm Wearables und Viewer F9 UI

Source baseline: OGL develop-current 984fbabfaf03135b4c399caacedc81317ca9a95d, OpenGenesisLINK Server main 2b7c19c9396eb2f75fd60a41a33a05655909da6b, Firestorm_7.2.4 llwearabletype.h/.cpp, llinventorydefines.h and message_template.msg.

Original inventory subtype is item flags & 0x0000ff; seventeen wearable types Shape0 Skin1 Hair2 Eyes3 Shirt4 Pants5 Shoes6 Socks7 Jacket8 Gloves9 Undershirt10 Underpants11 Skirt12 Alpha13 Tattoo14 Physics15 Universal16. Missing flags are unknown, not Shape0. COF Link flags are not trusted for the original target; the original inventory object is fetched by owner and region scoped FetchInventory2 with COF version/readback guard. Invalid target, multiple bodyparts/layer collisions or missing subtype fail closed.

Original Firestorm Low383 AgentIsNowWearing and Low84 AgentSetAppearance have bounded protocol encoders with golden byte tests, UUID and cache/size/texture/visual param validation. No real sending to simulator until all wearable assets, textures, layers and AvatarAppearance are ready and validated.

F9 Wearables pruefen reads actual linked original items via FetchInventory2 (32 max), populates the visual 17-slot inventory panel and read-only status; invalid on region or COF update. All actions use core OpenSim/Firestorm compatibility, no server admin keys/REST privileges. UI hitboxes validated on 800x480 through 5120x1440.

Not yet complete: wearable asset decoding, layered bakes, actual avatar morph+GPU wearing, multi-item full Outfit Tragen/Ablegen with rollback, authenticated real NexVerse Inworld test.
