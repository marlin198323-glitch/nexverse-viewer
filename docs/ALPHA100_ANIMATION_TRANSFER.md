# Alpha.100 – Firestorm/OpenGenesisLINK AssetTransfer + GUI

Quellen: OpenGenesisLINK server main
2254d2b3733c5fb171e6e608bc909ff1ef8a7acd,
OpenSim/Region/ClientStack/Linden/UDP/LLClientView.cs
(HandleTransferRequest, MakeAssetRequest, SendAsset,
SendAssetNotFound). FirestormViewer/phoenix-firestorm
Tag Firestorm_7.2.4 message_template.msg.

## Protokoll
Client Low153 TransferRequest: UUID16 TransferID,
Channel S32=2, Source S32=2, Priority F32,
Params Variable2 mit UUID16 AssetID + S32 Animation20.
Server Low154 TransferInfo: UUID16, Channel S32=2,
TargetType S32=0, Status S32, Size S32,
Params Variable2 (at byte32) mit AssetID+Type.
Server High17 TransferPacket: UUID16, Channel S32=2,
Packet S32, Status S32 (0 more, 1 final), Data Variable2
at byte28 (NOT byte32). OpenGenesisLINK Status -2
means asset not found, Size0, Params echoed.

## Security, Assembler and Source of Truth
Request from active, authenticated High20 self
AnimUUID only. Actual CircuitConnection reliable
queue; responses matched by random request UUID.
Bounded epoch-scoped mutex Inbox; 2MiB max,
4096 chunks, conflict/duplicate/size/ID checks,
out-of-order assembly. Only validated whole
binary LLKeyframeMotion v1/legacy 0.1 becomes
a cached immutable clip. No external HTTP
asset URLs or auto-import. Region change
invalidates pending and cache. Max 64 clips.
Server notfound -2 produces real error; Timeout
10 seconds; no endless background retry.

## Actual Playback and Viewer UI
RenderAvatarProxy receives real clipped animation
only after successful transfer and decoding.
Priority prefers the highest loaded active clip,
per agent+AnimUUID+SeqID clock starts when
first selected and resets on region change.
Original Firestorm LLM Head/Upper/Lower skinning
uses the existing GPU bones and textures.
No full Firestorm animation fades/layers/IK yet.

Viewer Avatar>Animationen and Dock>Animationen
list High20 server entries and live status:
not requested, fetching, loaded with joint
count, failed/missing/timeout. Download erneut
is an actual guarded mouse button that permits
one more LLUDP attempt if an error occurred.
Play/Stop has no imaginary backend.

## Test and Release Honesty
tests/animation_asset_transfer_tests.cpp:
Low153/Low154/High17 golden payloads,
complete valid keyframe fixture, missing -2,
out-of-order, duplicate/conflicting chunks,
bad lengths, wrong type/ID and epoch reset.
ViewerShell keyboard/click and GL3.3 tests
plus 3 original LLM weighted GPU draw test.
Live NexVerse authenticated session and
simulator response have NOT been revalidated.

## Alpha.100 wiederholter Server-GitHub-Releaseabgleich

Beim vorletzten Live-GitHub-Preflight neue Server SHA
689c65ce9fa6d8816e8f3c3be3aa4d0e4bab50a3
(13.10.2026 20:25:25 UTC) ermittelt. Ein Commit nach
2254d2b: doc/NexVerse/CROSS_REGION_LSL_CHAT_09312.md,
tools/ci/verify_cross_region_chat.py,
tools/diagnostics/ogl_cross_region_chat_smoke.lsl.
CHAT01/CHAT02 erhalten einen LSL-Testwuerfel fuer
Region-A/Region-B, Chat Kanal0 und /7, Distanzfilter
20m, NexBus, Prozessgrenzen. Dokumentations-/Test-
Aenderung, KEINE neue LLUDP Client-Schnittstelle.
Einordnung SERVER-ONLY/Operator-Test, OGL Standard-
ChatFromViewer unveraendert. Die Smoke-Prozedur wurde
nicht real mit zwei Simulatoren ausgefuehrt und darf
nicht als PASS bewertet werden. Reviewed-SHA nach
Quellvergleich im Baseline-Ledger aktualisiert.
