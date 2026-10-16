# Alpha.150-154: Linux Mint desktop, GPU and original Firestorm avatar

16.10.2026. Repository marlin198323-glitch/nexverse-viewer; branch
develop-current. Primary device jam-MS-7D28, Desktop Commander ID
3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d, user jam, checkout
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer.
Pi5 device s359142.srv.nexvortex.de is ONLY the GitHub ARM64 CI runner.

## Alpha.150: Desktop checkout recovery
The Mint checkout was clean but still at 0.2.0-alpha.127. Fetched GitHub
and applied a verified git merge --ff-only for 12 commits to Alpha.149.
No discarded local edits. CMake 3.28.3, Ninja, GCC 13.3, vcpkg present,
RTX 4070 SUPER OpenGL 4.6 NVIDIA GPU driver available.

## Alpha.151: Complete native x64 build/tests
Built the actual viewer executable with CMake/Ninja and all test targets.
Full CTest succeeded 112/112 initially and 113/113 after the new
locator test; zero failures. Python unittest discovery 33/33 passed.
Hardware OpenGL renderer framebuffer/region/mesh GPU tests ran in a
hidden GLFW context on the actual NVIDIA display, without logging in.

## Alpha.152: Real GPU baking/JPEG2000 integration checks
Native GPU region transition CTest now asserts three anatomical original
Head/Upper/Lower GPU draw submissions and valid read-only preview name.
After removing texture, it asserts zero baked GL draws and no dangling
preview GL handle. OpenJPEG fixture regression verifies identical J2C
codestream and boxed JP2 RGBA results. Upstream
opengenesislink/opengenesislinkserver main
ec760cacfff2fe505cf51991985553ed434eb121 (16.10.2026) added one
server-only change to J2KDecoderModule.cs: try OpenJPEG layer boundary
decoding after CSJ2K exception. The viewer already uses OpenJPEG.
No viewer-facing CAPS/LLUDP change was observed in this new commit.
Other 22 earlier upstream commits remain incompletely reviewed;
previous reviewed SHA 4e99976630f1008db9014ea2b86c3723b09e9fe6
MUST NOT be advanced without comprehensive review.

## Alpha.153: Native compiler and version issues
Fixed Firestorm wearable integer lambda name shadowing, Pending object
constructor initialization ordering, visual_param ID shadowing,
missing avatar RenderAvatarProxy aggregate initialization, and
unsafe 64-bit COF inventory version narrowing to uint32.
Added COF large-version test. Added CMAKE_CONFIGURE_DEPENDS for VERSION,
so Ninja rebuilds the displayed version instead of retaining stale
Alpha.149. Verified --version prints 0.2.0-alpha.154.
Old unrelated test fixture initializer warnings still exist.

## Alpha.154: Original Firestorm avatar autoload and GUI window smoke
First 1600x900 isolated GUI startup worked but warned that Firestorm
character assets were unavailable and showed only proxy geometry.
Real installed Firestorm Release 7.2.4 character resources were found
on Mint under Schreibtisch/Phoenix-FirestormOS-Releasex64_AVX2-7-2-4-80712.
Nightly 7.2.5 currently exists as a tar.xz archive in Downloads but
is NOT yet extracted.
find_firestorm_avatar_character_directory now scans one directory
level of locally extracted Phoenix-FirestormOS/Firestorm directories
under Desktop/Schreibtisch/Downloads, preferring Nightly over Release.
NEXVERSE_AVATAR_ASSETS explicit override takes precedence.
No third-party Firestorm assets are copied to the project or GitHub.
Added deterministic native temp-home test for auto discovery, fallback
and explicit override.
Verified real Firestorm files: 133 rig joints, 55 attachment slots,
4,320 vertices, 7,186 triangles across original Head/Upper/Lower LLMs.
GUI initialization now reports Firestorm ORIGINAL body meshes GPU
resident, 7,186 triangles, Welcome background loaded, GridInfo LIVE.
Isolated GUI test ran 8 seconds on virtual Mesa 4.5 llvmpipe display
to avoid interrupting the user's desktop; separate framebuffer/GPU
CTests used the NVIDIA RTX 4070 SUPER display.

## Still open
A real authenticated avatar/world appearance after login in NexVerse
has NOT been verified; the preview and body bind meshes do not prove
Firestorm 11-region TexLayer pixel parity, outfit wear/unwear or
server-confirmed COF mutation. No simulator production writes and no
real user account credentials were used.

## Next 5 blocks
1. Compare installed Firestorm Release 7.2.4 and locally unpacked
Nightly 7.2.5 character assets, without distributing them.
2. Authenticated test-grid world and avatar region-event/render checks
with explicit test identity; visually verify world mesh and movement.
3. Actual F9 Appearance/GPU/COF data validation on RTX desktop.
4. Authorize safe outfit wear/unwear with readback and rollback.
5. Finish older 22 upstream code patch compatibility reviews.
