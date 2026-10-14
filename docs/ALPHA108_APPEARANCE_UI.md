# Alpha.108 – COF zu Server AvatarAppearance, echte F9-Anzeige

## Provenance
OGL Viewer develop-current before block: c6b19b7e7dc86643643e40499d8e967cba722ac4.
OpenGenesisLINK Server main baseline: 2b7c19c9396eb2f75fd60a41a33a05655909da6b.
Firestorm_7.2.4 script messages: AvatarAppearance Trusted Low158 includes Sender ID, IsTrial, ObjectData TextureEntry, VisualParam and AppearanceData AppearanceVersion, CofVersion, Flags.
The user Viewer MUST NOT load WorldAPI admin token, ES256 keys or provide fake Appearance packets.

## Real evidence levels
1. User double-consents and queues one Low426/270, or valid serial Batch.
2. Owner-scoped InventoryDescendents2 returns new COF folder Version and exact intended links (from Alpha106/107).
3. PacketDispatcher accepts an authenticated SELF AvatarAppearance after the start of that mutation, on same simulator circuit/RegionEpoch. AppearanceData CofVersion exactly equals the server COF version confirmed in step 2, IsTrial=false, and both original TextureEntry and VisualParams nonempty. The status labels this matching server message, not a baked/rendered outfit.
4. AvatarBake GPU textures, cached wearables, morphs, visible avatar body and live in-world Wear/Unwear MUST be validated separately.

New CofAppearanceVerifier in protocol header has dedicated explicit state, 120s COF waiting guard, 30s post-COF Appearance guard, packet serial guards and wrong-user/region filters. Existing WorldModel self appearance is the source of truth; observer runs under current world mutex. On region/session transition invalidate state. Do not automatically resend inventory mutation or AgentSetAppearance.

## GUI
Outfit Galerie F9 has a real clickable Appearance pruefen control, comparing its most recent own Simulator AvatarAppearance CofVersion with the currently read CAPS COF version. It reports no packet, missing AppearanceData, user mismatch, version mismatch or matching version but bake unproven. During mutations the footer shows a separate state machine status for evidence.
Because added statuses overlapped the ten outfit rows, the responsive floater was enlarged to 675 scaled pixels. Input hitboxes and renderer share OutfitGalleryLayout for 800x480, 1024x600, 1280x720, 1920x1080, 3440x1440 and 5120x1440.

## Tests and nonclaims
firestorm_appearance_verification_tests: valid server version, missing packet, wrong self UUID, trial, absent AppearanceData, stale/newer CofVersion, missing texture and params, earlier packet serial, timeout and region crossing. outfit_gallery_layout_tests: new button click and footer nonoverlap for six window sizes. CTest/Python and real Linux NVIDIA GL3.3 GUI smoke required. No authenticated NexVerse Wear/Unwear/E2E or new avatar bake has been demonstrated.
