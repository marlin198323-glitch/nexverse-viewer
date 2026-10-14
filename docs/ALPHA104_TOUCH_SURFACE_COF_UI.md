# Alpha.104 – Firestorm ObjectTouch SurfaceInfo und COF Readback

Source audit: OGL develop-current ac9384d358fe96ea8897c1f753fab7c51e57869a; OpenGenesisLINK main 1841c4ee8ef580730be7d306023e594380f8b19f; Firestorm_7.2.4 lltoolgrab.cpp und message_template.msg.

## Wire / geometry / safety
Authenticated reliable ObjectGrab Low117 and ObjectDeGrab Low119 carry optional SurfaceInfo (VariableCount 1): UVCoord Vec3 (12), STCoord Vec3 (12), FaceIndex S32 (4), Position Vec3 (12), Normal Vec3 (12), Binormal Vec3 (12), total 64 bytes. Client sends Count0 on invalid, unknown or planar-mapped face. For a resident GetMesh triangle, barycentric UV, geometric world normal, tangent-space world binormal and per-face TextureEntry transformation yield validated surface values. The same surface is retained for matching DeGrab and invalidated by region change. GUI indicates actual send, not server acceptance.

## Outfits
F9 gallery offers actual Aktualisieren and COF pruefen controls, using owner- and RegionEpoch-scoped InventoryDescendents2/FetchInventory2. No parallel pending request and min 2 seconds between manual verify requests. Readback status reports success with item count, pending, invalid session, or failure; previous resolved items are invalidated. No CreateInventoryLink, RemoveInventoryItem, Wear/Unwear or Appearance mutation yet. These must not be falsely advertised.

## Validation
Golden tests for 64-byte SurfaceInfo order in Low117/119, validation rejects, Triangle ray normal and binormal, mouse hit regions on standard and ultrawide windows, full CTest/Python and GPU GUI smoke. No new authenticated live simulator touch or COF mutation has been exercised. Next task: safe COF transactional write + readback/appearance and matching functional Wear/Unwear GUI.

## Alpha.104 verpflichtender GitHub Release-Preflight

Vor GitHub Release am 14.10.2026 wurde neuer
OpenGenesisLINK main SHA
5831913f3dc840aa9661b83fc844546f9f637070
gegen 1841c4ee entdeckt: 1 Commit, 6 Dateien.
Begutachtet OglApiDocsAccess.cs, NexApiDocsPage.cs,
NexVerseWorldApiConnector.cs und
doc/OpenGenesisLINK/WORLD_API_CONTROL_CENTER.md
sowie CI. Admin Anmeldung:
GET /api/v1/docs ist nur Login Shell,
GET /api/v1/docs/panel liefert Dashboard
nur mit HTTPS, authentifiziertem GridUser,
aktuellem LoginAllowed, UserLevel>=200,
AdminAll Scope und servervalidiertem Token.
Machine Keys sind kein Admin Login.
**SERVER-ONLY_OPERATOR_ADMIN_UI**, keine
neuen OpenSim LLUDP/CAPS Viewer-Pakete,
kein Admin-ControlCenter mit Adminbearer im
Endnutzer-OGL Viewer. OpenGenesisLINK ist
Serverprodukt, NexVerse ein konkretes Grid;
Legacy Namespaces und Header bleiben nur fuer
Abwaertskompatibilitaet. Live Admin
Login/Deployment ist nicht automatisch getestet.
Reviewed SHA in Baseline aktualisiert.
