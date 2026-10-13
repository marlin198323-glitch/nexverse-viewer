# OpenGenesisLINK Server-Kompatibilitaet: Entscheidungen

Quelle: https://github.com/opengenesislink/opengenesislinkserver.git
Branch main, SHA 011ed5183889ebff72d10a49162b3223830d8ba5,
am 13.10.2026 erneut gelesen. OGL Ausgangsbranch develop-current
Alpha.98 SHA 6c07f2835ec95ebf1eb280255887148b093b5235.

## Pflicht vor JEDEM Entwicklungsblock und vor jedem Release

1. Lokales und GitHub OGL HEAD sowie sauberen Arbeitsstand prüfen.
2. python3 scripts/check_opengenesislink_server.py --strict
   ruft live GitHub-Branch-/Commit-Diff-API auf (12s Timeout).
3. Exitcode 0 bedeutet Current-HEAD entspricht reviewed SHA.
4. Exitcode 2 zeigt neue Commits/Pfade, Diffs zwingend manuell
   mit OGL Code und Firestorm Standard vergleichen, Rechte prüfen.
5. Exitcode 3 bedeutet Github nicht geprüft; keine Scheinbestätigung.
6. Erst nach Review den SHA in
   docs/OPENGENESISLINK_SERVER_BASELINE.json aktualisieren.
7. Prüfen und dokumentieren: Core (Firestorm Standard),
   Plugin (optional, austauschbar) oder Server-only (keine
   doppelte Client-Implementierung).
8. Unmittelbar vor jedem GitHub Push erneut kontrollieren.
9. Die Nutzer-Viewer-Anwendung prüft GitHub nicht bei
   jedem Maus-Klick. Das wäre unsicher und würde Offline-
   Nutzung und Datenschutz beeinträchtigen.

## Nachgeprüfte Features aus OpenGenesisLINK main

| Server-Neuerung | Integration | Viewer-Sichtbarkeit und Abgrenzung |
|---|---|---|
| Standard OpenSim LLUDP/CAPS | KERN | Firestorm Protokoll/UI |
| Nachbarregion llListen Chat-Routing, ChatModule/IWorldComm/NodeAgent | SERVER-ONLY | LLUDP ChatFromViewer bleibt Standard, keine Client-NexBus-Route |
| Whisper/Say/Shout ChatFromViewer | KERN | Alpha.99 drei klickbare Buttons, Drahtwerte 0/1/2, Kanal 0 |
| oglGetRegionDimensions LSL native X/Y/Z | KERN Script-Editor künftig | Server-Script-API, kein Viewer-CAPS für Z; Autocomplete erst mit Editor |
| Stationärer Touch-Hold ca. 100 ms Serverheartbeat | KERN Grab/DeGrab später | Server generiert touch; Viewer darf nicht doppelt synthetisieren |
| InventoryThumbnailUpload und Robust-GETITEM-Verifikation | KERN | vorhandener Outfit-Thumbnail-/CAPS-/LLSD-Cache, Alpha.99 neuer explizit-Empty-Test |
| C# NodeAgent NexBus | SERVER-ONLY | Nur optional Admin-Überwachung könnte separates Modul sein |
| RTX Raytracing | OPTIONAL MODUL | Standard OpenGL bleibt Pflicht |
| Fotorealistischer Regen/Schnee | OPTIONAL MODUL | Standard-Wetter immer unabhängig |
| Zusatz-Outfit-Screenshottools und besondere Text-only-UIs | OPTIONAL MODUL | Basis-Inventar und Chat im Viewer-KERN |

## GitHub-Vergleich – tatsächliche Änderungen am 13.10.2026

Von c29f3e5a auf 86148202: 9 Servercommits, u.a.
OpenSim ChatModule, WorldCommModule, NodeAgent und
Scene.PacketHandlers, LSL oglGetRegionDimensions und
Touch-Hold. Verifizierte Serverdokumentation:
doc/NexVerse/CROSS_REGION_LSL_CHAT_09312.md
doc/NexVerse/OGL_REGION_DIMENSIONS_LSL.md
doc/OpenGenesisLINK/LSL_TOUCH_HOLD_COMPAT.md.

Danach meldete der live Python-Checker 4 weitere
Commits von 86148202 auf 011ed518. Drei betroffene
Pfade: OpenSim/Services/Connectors/Inventory/
XInventoryServicesConnector.cs, Dokumentation
doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md
und CI-Test verify_inventory_thumbnail_upload.py.
Server-Änderung verifiziert vor erfolgreichem Item-
Update den gespeicherten Thumbnailwert durch
GETITEM, statt lokalen Cache-Erfolg als Serverbeweis
zu behandeln. OGL verwendet bereits LLSD-
Thumbnail-ID, versioniertes Cache-Merging mit
gleichversionigem UUID-Replacement und alters-
/Region-Epoch-Schutz. Alpha.99 ergaenzt Regression,
dass explizites Loeschen von fehlendem Metadatafeld
unterschieden wird.
Firestorm 7.2.4 kann laut Serverdokumentation
Thumbnail-Replacements beim Relogin mit
bestehendem Inventory-Cache falsch anzeigen.
Daher nicht pauschal Caches beim Start loeschen.

## Alpha.99 Umsetzung

Chat UI: Fluestern, Sagen, Rufen sichtbar mit
aktiver Hervorhebung, Modal-Mausklick-Bereich,
Status und echter LLUDP-Type-Weitergabe,
nicht nur eigener UI-Text.
Serverseitige Chat-Reichweiten standardmaessig
10/20/100 m, aber via Chat-Konfiguration variabel.
Der Viewer behauptet keine feste Client-Reichweite.
Golden ChatFromViewer-Pakettests für 0/1/2,
Kanal 0 und offene GPU-UI-Regression.

Entscheidung: Neue C# Serverfeatures werden nur
dann im C++ Viewer gespiegelt, wenn Firestorm
ein entsprechendes Client-Protokoll oder GUI-Verhalten
erfordert; die Neubeurteilung geschieht stets anhand
der neuesten source-reviewed SHA.

## Folgeblock

Animation AssetType Fetch/CAPS/cache/Clip-Playback
aus Alpha.98 und dazu funktionierendes Avatar-
Animationsfenster gleichzeitig implementieren.
Touch Grab/DeGrab und LSL-Script-Editor Core
als weitere integrierte UI-/Backend-Arbeit.
NexVerse Login-E2E bleibt bis zu einem realen
authentifizierten Test offen.

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

## GitHub Nachprüfung Alpha.99: IAR GZip Streams

Nochmals 6 Server-Commits entdeckt: 2c7f295 auf
2254d2b3733c5fb171e6e608bc909ff1ef8a7acd
(13.10.2026 20:20:43 UTC). Änderungen in
NexVerse/RegionModules/Archives/
OglIarOperationManager.cs und
doc/NexVerse/IAR_CONSOLE_PROGRESS_09312.md.
Managed Stream-IAR exportiert nun komprimiertes
GZip/TAR und importiert über GZipStream; der
begrenzte Preflight/Archive-Inspector bleibt
vor mutierenden Importen verpflichtend.
**Einordnung SERVER-ONLY**, kein bestehendes
Firestorm-OGL-Viewer-Paket, kein Client-Plugin
für serverseitige Archivströme. Live-Archiv
Export/Import und TOCTOU-/Quota-Gates sind offen.
Die Revision nach Quellenreview in die
Server-Baseline übernommen. Alpha.99 fokussiert
weiter die Viewer-Core-Chatmodi und
korrekte GUI, keinen IAR-Servercode-Port.
