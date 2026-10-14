# Alpha.106 – Ein-Link COF Mutation mit explizitem Consent und Servernachweis

## Verified upstream
OGL develop-current vor Änderung:
ba138e0c04ca2593cf88b88b9bc97108eef98c63.
OpenGenesisLINK main:
5831913f3dc840aa9661b83fc844546f9f637070.
Firestorm original sources: scripts/messages/message_template.msg
and indra/newview/llviewerinventory.cpp from Firestorm_7.2.4.
Server LLClientView.cs supports LinkInventoryItem Low426 and
RemoveInventoryItem Low270. No new server operator REST/CAPS
is requested; user avatar permissions never obtained from admin APIs.

## Core and UI
F9 > Outfit-Galerie > Aenderungsplan, when the selected
outfit yields exactly ONE missing linked inventory target and
NO removals: an explicit 'Einen COF-Link anlegen' appears.
First click: prepare, never send. Second click within 20s:
re-evaluate login, current simulation, region lease, COF
folder, selected item, existing version and no-delete conditions;
queue exactly ONE reliable Low426. Callback ID never zero.
No automatic repeat, no removal of existing links, no
multi-link mutations. The prior Low270 encoder remains
available to core but is intentionally NOT called by this UI.

Outfit Link Consent lives in
include/nexverse/protocol/firestorm_cof_link_consent.hpp.
Wait at most 18s and poll InventoryDescendents2 within
2-second throttling; continue verification after F9 is closed.
Success means that newer COF folder version contains a
real AT_LINK with asset_id equal to the target and a
real item_id. If not found, say NOT VERIFIED and DO NOT
guess whether the server applied the command.
An ACK is not evidence of successful Wear/Unwear or
updated AvatarAppearance.

'COF pruefen' was corrected: it now uses actual
FetchInventoryDescendents2 for Current Outfit Folder
(FT_CURRENT_OUTFIT=46), not selected Outfit
FetchInventory2. It reports real COF item count
and folder version if owner/region-scoped verified.
Separate selected-target FetchInventory2 stays present.
A monotonically increasing CAPS request serial prevents
accepting an older in-flight response as the result of
a newly pressed COF verify button.

## Automated acceptance
- New firestorm_cof_link_consent_tests.cpp: positive single
  link stage/confirm and server observation; expiry,
  wrong region, stale version, callback zero, bad link
  type, plan with multiple additions or removals, 18s
  non-confirmation, queue failure.
- Extended outfit_gallery_layout_tests.cpp:
  action hitboxes, four monitor aspect ratios.
- Extended firestorm_inventory_fetch_tests.cpp:
  raw mock server Current Outfit Folder version=42,
  actual AT_LINK and folder ID requested in LLSD CAP.
- Older wire/COF planning, LLUDP, 3D Mesh Touch/GUI
  regressions and entire CTest/Python/Linux NVIDIA GL
  release gate.

## Explicit nonclaims
Full Firestorm Outfit 'Wear' and 'Remove', multi-link
atomically consistent mutation, rollback, link callback
delivery, appearance update and direct authenticated
NexVerse end-to-end test NOT IMPLEMENTED/NOT VERIFIED.
This is a verified COF inventory link addition, not
an avatar outfit change. Next priority: safe multi-step
COF+AvatarAppearance reconciliation and real end-user
Outfit controls with complete live acceptance.
