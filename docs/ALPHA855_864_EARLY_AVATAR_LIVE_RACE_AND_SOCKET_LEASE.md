# Alpha.855-864 - FastRez: pending Low158 cannot overwrite later live COF

23.10.2026 | Mint jam-MS-7D28 | develop-current
Viewer 0.2.0-alpha.864 | 39% (weighted 38.65%)

## Ten completed development blocks
855. Verified clean develop-current HEAD e0accb2; read CHATUEBERGABE.md and projectplan.md first; Pi5 Alpha854 SHA 584ea15 Run 39975553846 SUCCESS.
856. Rechecked server main 08f95e7 vs 8b3d80e; only new #285 WebRTC/Voice deployment. No new avatar/COF/Low382/WASD protocol.
857. Identified actual race: staged early valid Low158 could flush AFTER a newer directly accepted live Low158, replacing real current skin, especially when the later packet used legacy COF=0.
858. WorldModel flush allows staged Low158 only while no other current authorized self_appearance is accepted. A valid directly admitted packet always wins over older staged data, irrespective of COF format.
859. Bound early self Low158 and Low382 to exact simulator IP and UDP port as well as Agent, Session, RegionHandle and generation; same region handle can reconnect via another simulator socket.
860. Preserved strict TE/VisualParams, preview rejection, stage boundedness (one per type), login/logout/region reset and authenticated readback admission.
861. Added read-only WorldModel early_self_readbacks_pending status for F3. Pending is neither COF success nor GPU skin proof.
862. Wired visible F3 avatar handshake status: Low158 gepuffert (warte Circuit) and Low382 gepuffert (warte Circuit), under existing world mutex, without exposing private asset/session identifiers.
863. Expanded native C++ regression: staged COF17 versus directly admitted COF42, staged Low382 serial7 versus live serial9, unversioned legacy direct reply versus old stage, and socket port 9000 versus 9001 under same handle. Python production contract, CMake and Pi5 headless native test maintained.
864. Version, handoff, projectplan, parity docs and progress JSON updated. Full local Mint x64 CMake/CTest/Python/GUI/icon release gate before git push; exact Pi5 and hosted x64 CI tracked by code SHA.

P0 remains OPEN: no authenticated real NexVerse avatar/COF/skin/outfit GPU frame nor server confirmed WASD/yaw live test. No production deploy. Next Alpha.865-874.

## Completed Linux Mint x64 release gate
scripts/mint-viewer-validation.sh EXIT 0, full CMake/vcpkg
Viewer 0.2.0-alpha.864, 178/178 native CTests,
416/416 Python contracts, isolated Xvfb Viewer
with original Firestorm 133 bind joints, three
GPU resident Body meshes / 7186 triangles and
X11 taskbar WM_CLASS and system icon 128x128 PASS.
No authenticated NexVerse Grid inworld session.
