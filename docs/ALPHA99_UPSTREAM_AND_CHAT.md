# Alpha.99 – dynamischer Servercheck und Viewer-Chat

Entwicklungsreferenz: OpenGenesisLINK Server main
011ed5183889ebff72d10a49162b3223830d8ba5.
OGL Viewer vorher Alpha.98 develop-current SHA
6c07f2835ec95ebf1eb280255887148b093b5235.

OGL muss bei JEDER Programmieraktion, besonders vor
Featureblöcken und Release-Commits, den Live-HEAD
beider Repositories prüfen. Script:
python3 scripts/check_opengenesislink_server.py --strict
Lokal fehlender Internetzugang/alte SHA ist
ein ehrlicher Review-Blocker, kein stiller Pass.
SHA-Ledger in OPENGENESISLINK_SERVER_BASELINE.json,
Matrix in OPENGENESISLINK_INTEGRATION_MATRIX.md.

Neu entdeckte 9 + 4 Servercommits:
ChatModule/WorldComm/NodeAgent Neighbor Script
llListen, LSL oglGetRegionDimensions(), stationäres
Touch-Hold serverheartbeat, autorisierter Robust
InventoryThumbnail GETITEM Readback. Nach Review:
Firestorm Standard Chat, Inventory, Scripting Editor
und Grab/DeGrab bleiben OGL-CORE.
NexBus Event-Routing/Server-LSL gehört nur
in den SERVER. Verbesserte Wetter/RTX-Funktionen
optional als Plugin ohne Pflichtabhängigkeit.

Viewer Alpha.99 realisiert:
- Chatfenster zeigt drei auswählbare Buttons:
  Fluestern, Sagen, Rufen.
- Klick setzt Live ViewerShellState.chat_mode,
  aktive Farbe und Meldungsstatus.
- Beim Senden wird LocalChatMessage mit
  LocalChatType whisper/normal/shout 0/1/2
  in dieselbe authentifizierte OutboundCommandQueue
  gelegt; LLUDP ChatFromViewer bleibt Standard.
- Kanal 0, regionale Reichweite und CrossRegion
  Chat-Verteilung bleiben Serververantwortung.
- ViewerShell Maus-Hitboxen und echte LLUDP-
  Paketbytes im Golden-Test sowie GPU-Shell-Test.
- Outfit Thumbnail Synchronizer bekommt Regression:
  explizit leere Thumbnail-ID ist anders als
  fehlendes Feld. Keine vollständige Löschen-
  des-Cache-beim-Neustart-Scheinlösung.

Sichtbare UI und Backend bleiben gekoppelt;
nächste Stufe ist Animation-Asset-Fetch + GUI-
Playbackstatus, nicht bloß C++ ohne Viewer-Fenster.

Security: User-/Shop-/Grid-Secrets nie in Logs,
Server C# niemals blind in Client portieren,
Keine neue NexBus-Verbindung für Viewer,
kein GitHub Polling durch jeden Endnutzerklick.

## Zweiter erneuter GitHub-Check direkt vor Alpha.99-Release

Beim verpflichtenden Release-Check von Server SHA 011ed518
meldete GitHub erneut **4 weitere Commits** bis SHA
2c7f2953681eda3fe0a66700e38621f31077d822
(13.10.2026 20:15:28 UTC). Betroffene Dateien
ausschliesslich Dokumentation:
doc/NexVerse/09312_RC1_STABLE_ACCEPTANCE.md,
doc/NexVerse/LSL_PARITY_09311_09312.md,
doc/NexVerse/ROADMAP.md.

**Release-Sicherheitsentscheidung:** OpenGenesisLINK
main ist im Stand 0.9.3.12 DEVELOPMENT,
**NICHT RC1 oder Stable**. Die veröffentlichte
Stable ist v0.9.3.10 (12.10.2026).
22 geplante Operator-/InWorld-Gates beginnen
NOT_RUN, kein CI-Gruen als Live-PASS ausgeben.
Insbesondere Native-OGL Login, Voice, COF/
Outfits, Firestorm Simulator-Login/Teleport,
Chat über Regionsgrenzen, Touch-Hold und
Inventory-Thumbnail-Reconnect sind nicht
durch Alpha.99-InWorld-Tests bestätigt.
OpenGenesisLINK Roadmap verschiebt OGL
Experiences 2.0, Pathfinding 2.0 und
My Outfits 2.0 bis NACH 0.9.4.0 Generation 1.
Das betrifft diese erweiterten OGL-Features,
nicht die Pflicht zu Firestorm Standard-
Funktionsparität im Basiskern.
Keine neue C++-Protokollcodeänderung nötig,
da der Upstream-Diff nur .md-Dateien umfasst.
Geprüften SHA nach manuellem Review aktualisiert.
