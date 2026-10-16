# OGL Alpha.245-254: Terrain recovery and self-avatar animation visibility

Stand 17.10.2026. Linux Mint jam-MS-7D28, branch develop-current.
Raspberry Pi5 pi5-ogl only CI. Version 0.2.0-alpha.254.

## The ten integrated blocks

245. Verified latest OpenGenesisLINK Server main
d8f4426110379b39be7c0f45161f7c0c6b5d811e.
Exactly one additional upstream change since ec760cac:
server mesh-UPLOAD NewAgentInventory/physics LLSD validation;
the Viewer GET Mesh download protocol is unaffected. The
reviewed-server baseline must not be auto-advanced.

246. TerrainDetailRetry bounded to four attempts: 2, 5, 15
seconds backoff, generation/material epoch isolation, no requests
for empty UUID or already-successful layer.

247. Current-generation RGBA texture cache merges only newly
successful layers; failed requests never discard already valid
layer pixels, stale previous-region transfers are rejected.

248. GL set_terrain_textures accepts more real layers within
the SAME material revision, skips identical reuploads, refuses
downgrades; revised server material invalidates old GPU state.

249. Fixed real bug: prior terrain textured and fallback GL draw
counters were cumulative ACROSS frames. They now reset per
frame with the other renderer diagnostics, so the HUD does
not report stale success. Hidden NVIDIA OpenGL regression
verifies 1/4 -> 2/4 within the same material revision,
refuses downgrade, and resets correctly at new material epoch.

250. AvatarAppearance received/malformed/foreign/self counters
are reset across simulator epoch changes; private F3 diagnostics
exports only numeric evidence for the current region.

251. F7 Region window now shows genuine terrain GPU shader/
layer coverage and bounded retry progress or exhausted state.
No false 4/4 text when 1x1 fallback is present.

252. Logged only after actual GL skin draws that the SELF
Firestorm keyframe asset is used. Existing Animations viewer
window shows whether server-selected GL skin is really used.

253. C++23 tests for terrain state and UI, live hidden-GPU
promotion test, Python wiring regressions, privacy validation
and continuity of all existing tests.

254. Full Linux Mint x64 CMake/Ninja viewer, NVIDIA OpenGL CTests,
Python, ARM64 compatible headless, isolated Xvfb viewer smoke
and original Linux taskbar icon, GitHub commit/push and Pi5 CI;
update projectplan, AGENTS, handoff, info and three matrices.

## Weighted overall progress

World renderer goes from 40 to 43 percent of its category
(category weight 20 percent), asset/material streaming from
45 to 47 percent (category weight 15 percent). Therefore
37.75 becomes 38.65 percent and displays as 39 percent.
This increase is backed by a real functional fix and hidden
NVIDIA OpenGL tests, NOT by counting 10 release numbers.
Progress script and project JSON remain authoritative.

## Open, not E2E-proven

Last actual simulator logs had AvatarAnimation SELF,
and decoded LLKeyframeMotion with 19 joints, but no accepted
current Self AvatarAppearance. Full Head/Upper/Lower server
bakes, outfits, terrain Firestorm pixel matching, high LOD
for every world object, real inworld animation and attachment
fidelity remain UNPROVEN by this build. No user desktop
DISPLAY=:0 launch; GUI smoke only isolated Xvfb.
Original NexVerse login icon, system icon and Welcome page
remain unchanged. Next 10-block release: Alpha.255-264.

## Ergänzender aktueller Upstream-Abgleich – 17.10.2026

Unmittelbar vor der GitHub-Freigabe des Viewer-Alpha.254
stand OpenGenesisLINK Server main bereits auf
bf55a17524a9f3f175c56d2adeaa4d39083f446f:
86 weitere Commits seit dem in Block 245 gesehenen
d8f4426, zusammengefasst unter dem Merge
"OGL AssetService buildable compatibility addon (#231)".
Quelländerungen vor allem in addons/OGLAssetService
(.NET 8), Cache, Offline-FSAssets-Migration, MariaDB-/
PostgreSQL-Hushaudits, Redis-/Snapshot-Sicherheitsgates
und CI. Das Addon ist disabled-by-default in der
OpenSim-Hauptintegration. Es ist KEIN pauschaler Austausch
der Viewer-CAPS/GetTexture/GetMesh-Schnittstelle.
Daher separater SERVER-Addon-/API-Kompatibilitätsprüfpunkt
im nächsten Zehnerdurchlauf, keine ungeprüfte Umschaltung,
kein Prod-Deploy und kein Vorschub reviewed_sha.

Nächster Viewer-Lauf Alpha.255–264: tatsächliche API-
Exposition im Grid prüfen, optionales serverseitiges
Addon nur nach autorisiertem Health/Read-Only-Vertrag
integrieren, Firestorm-Netzwerk-/Avatar- und Weltzustände
weiter behandeln.
