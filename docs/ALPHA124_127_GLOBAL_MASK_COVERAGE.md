# Alpha.124–127 – Firestorm global colors, multiple masks and 11 bake input regions

Development date: 2026-10-14. Source OGL develop-current HEAD 1020c5b4d508bacbece159acaac2f7fc6de882af. Target 0.2.0-alpha.127. Firestorm reference Firestorm_7.2.4 original avatar_lad.xml / lltexlayer.cpp / lltexlayerparams.cpp.

### 124 – Global palette definitions
The bounded original Firestorm avatar_lad.xml parser also reads true skin_color, hair_color, eye_color param_color palettes, strict RGBAs, IDs, owners, operations and bounds. Invalid/missing/mixed owner, unknown/duplicate global color and excessive palettes fail closed. Original Firestorm asset bytes are never checked into GitHub.

### 125 – Owner-bound color evaluation
Global colors can be resolved only from real authenticated original Firestorm LLWearable visual params, partitioned by wearable type: skin=1, hair=2, eyes=3. Duplicate conflicting weights within the same type remain unresolved. Unavailable colors never silently become default palette values. F9 local preview consumes resolved global colors with the existing fixed and local tints.

### 126 – Ordered additive/multiplicative alpha masks
The existing read-only CPU RGBA diagnostic compositor supports several validated local param_alpha TGA resources, applying ADD and MULTIPLY in original XML order. Missing weights or assets, initial multiply (needs previous render-target alpha), nonzero alpha domain, malformed images, unknown morphological/bump/visibility effects fail closed. This is not pixel-identical Firestorm GPU morphology or a valid simulator bake.

### 127 – Real viewer diagnostics and all eleven groups
A read-only input-coverage audit accounts for 11 original Firestorm layer regions, availability of authenticated downloaded source images, absent images and complex rules. The existing clickable F9 color/mask action displays Global/Multi counts and coverage totals alongside Head/Upper/Lower local previews. Coverage never approves a baked upload. No AgentSetAppearance or AgentIsNowWearing send was added.

New C++ tests verify authentic 3 global color definitions, owner type isolation, missing/malformed values, two-mask ordering and multi-region fail-closed state. Previous TGA/alpha test updated so nonzero domain is no longer approximated. Source-level coverage contains an explicit upload_authorized() == false gate. 106/106 CTests succeeded prior to final release gate.

### Server baseline review
GitHub upstream #210 SHA 4e99976630f1008db9014ea2b86c3723b09e9fe6, compared patch for UserManagementModule.cs, HGFriendsService.cs, UserAgentService.cs, docs and CI. Fixes negative FriendsServerURI caching and delivery to ALL verified online local Hypergrid friends during relog. No new simulator-to-viewer online/offline packet fields. Classification SERVER_ONLY_HYPERGRID_FRIENDS_DELIVERY_NO_NEW_VIEWER_WIRE. A real two-grid Hypergrid reconnect smoke is still outstanding.

### Not implemented
Full original Firestorm texture baking (GPU/pixel parity), domain/visibility/bump/morph rules, cache IDs, simulator upload/ack, 3D outfit skin and authoritative multi-item wear/unwear with rollback, authenticated NexVerse in-world acceptance. These remain prioritized; no claim of finished avatar wardrobe.
