# Alpha.101 – Firestorm Touch und OpenGenesisLINK GUI

Upstream checked before changes:
OpenGenesisLINK Server main at
689c65ce9fa6d8816e8f3c3be3aa4d0e4bab50a3,
OGL develop-current 2820c5d87271bd4fdafbc7c93181be305ee80b59.
References: OpenGenesisLINK LLClientView.cs
HandleObjectGrab/HandleObjectDeGrab,
doc/OpenGenesisLINK/LSL_TOUCH_HOLD_COMPAT.md,
Firestorm_7.2.4 scripts/messages/message_template.msg.

Firestorm Low117 ObjectGrab and Low119 ObjectDeGrab
are sent reliably with real logged-in AgentID,
SessionID and selected simulator LocalID.
No explicit face picking is claimed. The SurfaceInfo
variable block is empty and GrabOffset zero;
server LLClientView supports empty touchArgs.
Repeated LSL touch() during a stationary
hold is produced server-side by the region heartbeat,
not client frame updates.

Visible UI: Welt > Objekte beruehren opens a
NexVerse floater showing at most five nearby root
prim IDs <=15m. Rows are selectable, BERUEHREN
button holds grab while left mouse remains
pressed and sends DeGrab on release.
Release outside the window and ESC are handled,
old selection reset on region change, LocalID
proximity/root/presence revalidated on queueing.
Only verified connected-world objects offered.
No fake interactive placeholders or client-side
touch event synthesis.

CHAT01/CHAT02 companion: Firestorm slash channel
parser handles /7 Beep and /-1729 Private,
sends correct S32 Channel to authentic
ChatFromViewer, and surfaces requested
channel to the user. Cross-region script delivery
is server-only; tests of two simulator nodes
remain NOT_RUN.

Regression: tests/object_touch_tests.cpp
validates original LLUDP Low117/119 header,
agent/session bytes, local ID, offsets/empty
SurfaceInfo, fail-closed zero/invalid IDs,
standard slash-channel parsing.
viewer_shell_tests.cpp covers menu, selectable
rows, button hitboxes for screen sizes, and
viewer_shell_gl_tests.cpp draws real floater.

Known gaps: world-mouse Raycast/FaceIndex/UV,
child prim touch target, avatar permission
surface semantics, authenticated NexVerse
live LSL script check and full COF Outfit UI.
Do not claim server touch_start/touch/touch_end
was observed until real inworld test.
