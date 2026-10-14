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

## Alpha.100 – Client/Server Quellenlage

Serverquelle OpenGenesisLINK LLClientView.cs main
SHA 2254d2b3733c5fb171e6e608bc909ff1ef8a7acd.
MakeAssetRequest via TransferRequest SourceType.Asset2,
SendAsset Info/Packet, -2 bei missing. Viewer-CORE:
LLUDP Transfer, validierter Clip-Cache und Avatar-Floater.
Kein C# Code im Viewer, keine direkte AssetService
URL und kein optionales Pflichtplugin.
Vor Release Hauptbranch erneut überprüfen.

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

## Alpha.101 – Source-Review Touch-Hold und ScriptChannel

Geprüfter Server-HEAD vor Entwicklung
689c65ce9fa6d8816e8f3c3be3aa4d0e4bab50a3.
OpenGenesisLINK LLClientView.cs
HandleObjectGrab, HandleObjectDeGrab und
doc/OpenGenesisLINK/LSL_TOUCH_HOLD_COMPAT.md:
Die Stationaer-Touch-Wiederholung (~100ms) ist
serverseitig. OGL implementiert nur die echten
clientseitigen ObjectGrab/DeGrab-Pakete und
lebendige Fensteroberfläche, nicht einen
Simulator-C#-Timer als Viewer-Plugin.
CHAT01/CHAT02: /7 Beep muss korrekten
ChatFromViewer S32 Channel 7 senden;
NexBus Routing bleibt Server-only.

## Source Review 14.10.2026 – sechs Upstream-Commits

Alter SHA 689c65ce9fa6d8816e8f3c3be3aa4d0e4bab50a3
Neuer SHA 2cc59ceaada1c6791ac8320d3d5638cae941b13a.
Sechs Commits zu OGLFeatureControlApi.cs,
OglArchiveFeatureGate.cs, deutschem Konsolenhelp,
NexBus Regressionen, IAR/OAR Archiver und
0.9.3.12 Development-Kennzeichnung.
VersionInfo.cs bestätigt Flavour.Dev.

Robust schützt GET /api/v1/feature-control/nodes
und PATCH für NodeID mit UserLevel >=200 und
Server-Authentifizierung. Signierte ES256
Kurzzeit-Freigaben für OAR1/OAR2/IAR4/IAR8
werden serverseitig durchgesetzt (fail closed).
KEINE Viewer-Protokolländerung; kein
Endnutzer-Plugin und keine Admin-Keys im Viewer.
Normales Objektpicking und Berührung bleiben
OpenSim/Firestorm Viewer Core.


## Alpha.102 zweite Upstream-Nachpruefung: 0.9.3.12 Release-Label

Ein weiterer Server-Commit nach der anfaenglichen
Sechs-Commit-Pruefung: neuer main SHA
1841c4ee8ef580730be7d306023e594380f8b19f
(14.10.2026 16:41:53 UTC), aus 2cc59cea.
Die VersionInfo.cs ist nun auf Flavour.Release
gestellt; Dokumentation RELEASE_0.9.3.12.md
und 09312_RC1_STABLE_ACCEPTANCE.md betonen aber:
**KEIN** allein dadurch publizierter GitHub-Tag,
KEINE nachgewiesene produktive Installation und
KEINE vollstaendige 22-Punkte-Release-Evidence.
Die zuletzt unabhaengig verifizierte/tagged
Stable bleibt v0.9.3.10. Endnutzer-OGL darf
keine Server-Livefreigabe aus dem Code-Label
ableiten. Keine neue Client-LLUDP/CAPS-Funktion
aus diesen Release-Dokumenten abzuleiten.
Server-Baseline-SHA nach Review aktualisiert.

## Alpha.103 – Core/Modes Integration

Reviewed OpenGenesisLINK main SHA
1841c4ee8ef580730be7d306023e594380f8b19f
vor Entwicklung; erneut vor Veröffentlichung
aktuell prüfen. Core: bereits vorhandene
GetMesh, InventoryDescendents2 und
ObjectGrab/DeGrab Firestorm-Kompatibilität.
Keine neue Server-CAPS für Face Picking.
Server-only: OAR/IAR Betreiberfeatureflags,
ES256 private Keys, NexBus. Optional:
Hardware-RTX oder hochwertige Wettermodule.

## Alpha.104

Alpha.104: Server main 1841c4ee8ef580730be7d306023e594380f8b19f erneut vor Entwicklung geprueft, vor Release erneut pruefen. SurfaceTouchEventArgs auf Server ist protokollkompatibles Viewer Core Feature. Serverinterne Admin-ES256 OAR/IAR weiter server-only. Outfit Readback via FetchInventory2 Core, COF-Mutationen nicht vorgetaeuscht.

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

## Alpha.105

Alpha.105 Live-Review Server main 5831913f3dc840aa9661b83fc844546f9f637070. OpenSim OnLinkInventoryItem und OnRemoveInventoryItem sind Viewer-Core-Protokolle. Server-only OAR/IAR FeatureControl, geschuetztes ControlCenter mit UserLevel>=200 und dessen Keys bleiben ausserhalb des Endnutzer-Viewers. Neuer COF-Diff/Preview fail-closed, kein produktiver Avatarwechsel bestaetigt.

## Alpha.106

Alpha.106 server main SHA
5831913f3dc840aa9661b83fc844546f9f637070
bei Beginn verifiziert. Original LLClientView
OnLinkInventoryItem Low426 und owner-scoped
InventoryDescendents2 aus aktuellem Simulator
sind Core-Protokollfunktionen, keine
Robust-Admin-Sonder-CAPS. Viewer-Linkwrite
nur mit explizitem 2-Click Consent, exakt
einem add, keinem remove und anschliessendem
COF Folder Version+Target Readback.
Kein AvatarAppearance-Erfolg ohne Livebeleg.
Operator Docs/OAR Gates bleiben Server-only.

## Alpha.107

OpenGenesisLINK Server main 5831913f3dc840aa9661b83fc844546f9f637070 zu Beginn Alpha.107 erneut geprüft, keine neuen Commits. Original LLClientView Low426/270 und InventoryDescendents2 bleiben Viewer-Core. OAR/IAR ES256 und geschütztes AdminPanel UserLevel>=200 weiter SERVER_ONLY. Noch kein LiveGrid Wear-Beleg.

## Alpha.107 zusätzlicher Release-Serververgleich (14.10.2026)

Während der Alpha.107 Release-Pflichtprüfung erschien ein neuer OpenGenesisLINK-Server Commit: 2b7c19c9396eb2f75fd60a41a33a05655909da6b (17:50:43 UTC, #207). Vorher 5831913f3dc840aa9661b83fc844546f9f637070. Fünf Dateien manuell geprüft: OglApiDocsAccess.cs, NexVerseWorldApiConnector.cs, Robust.ini.example, Robust.HG.ini.example und tools/ci/verify_ogl_docs_admin_ui.py.

Inhalt: abgesicherte AdminDocsTrustedProxyAddresses für TLS-Termination, verifizierte Peer-IP (Loopback oder explizite Allowlist), exakter X-Forwarded-Proto https, keine Annahme untrusted X-Forwarded-For. Anonyme Dashboard-Zugriffe bleiben 401; bei Admin-Bearer kann Proxy-HTTPS-Konfigurationsfehler 426 statt irreführendem 401 gemeldet werden. Owner/GridUser UserLevel >=200 und AdminAll Scope bleiben zwingend. Klassifikation SERVER_ONLY_OPERATOR_ADMIN_SECURITY: weder neue LLUDP/CAPS-Benutzer-Viewer Funktion noch OGL Plugin, kein Token oder Admin-Schlüssel im Desktop Viewer.

Source Review abgeschlossen; geprüften SHA-Pin auf 2b7c19c... aktualisiert. Release-Gate danach erneut ausführen.

## Alpha.108

Server main 2b7c19c9396eb2f75fd60a41a33a05655909da6b zu Alpha108 Beginn geprüft. OpenGenesisLINK/OpenSim Firestorm AvatarAppearance Low158 als serverseitig gesendete Viewer Core Nachricht, original AppearanceData CofVersion/TextureEntry/VisualParam. Weder neue Server-CAPS noch Admin-API benötigt. User Avatar Appearance End-to-End wird nicht vorgetäuscht; server-only AdminProxy-/ES256 Schlüssel bleiben ausserhalb Viewer.
