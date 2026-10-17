Alpha.315–324 | Viewer 0.2.0-alpha.324 | **39 %** gewichtet 38,65 %. Echtes original Firestorm LLM Skinning korrigiert: Vertexshader verwendet pro Head/Upper/Lower die tatsächliche GPU JointCount, letzter Index N-1 wird korrekt als Paar N-2/N-1 statt nicht initialisierte Folgematrix behandelt. Grenzwertprüfer im Upload, F3 zeigt tatsächliche Weight- und Clip-GL Draws. Kein Live-Grid-Outfit-/Bake-Pixel-E2E belegt. OpenGenesisLINK Server main 7c6b0cf ohne nötige Änderung, OGLAssetService disabled-by-default. Nächste **Alpha.325–334**.

---

Alpha.305–314: Autoritative SelfAppearance im WorldModel und realen Low158-Handler über aktiven Owner/Region, kein Trial, TextureEntry/VisualParams. Echte bestätigte Avatar-Skin wird von späteren Preview-/Teilpaketen nicht zerstört. F3-/privater Export Diagnose. Upstream OpenGenesisLINK 7c6b0cf, optionales Server-Addon disabled-by-default, echte Grid-Pixel-E2E noch OFFEN. 39 % Gesamtfortschritt. Nächste Alpha.315–324.

---

## Alpha.295–304, 17.10.2026 – SelfAvatarAppearance: Reliable ACK vs echtes Serverpaket

- Nur der ReliableTracker-bestätigte ACK der jüngsten SSA-RegionHandshakeReply-Sequenz wird als Simulator-ACK gezeigt; späte/duplizierte/falsche ACKs zählen nicht. Neuer RegionHandshake/Session-Start entwertet alte Receipt-Daten.
- Original F3 Avatar-/Bake-UI unterscheidet gesendetes Bit2, bestätigtes ACK, 15s fehlendes tatsächliches Self-Paket und verifiziertes WorldModel-Self/aktuelle Region. Grüner Zustand ausschließlich aus AvatarPipelineProof mit Login/Regionsidentität.
- Privater Diagnoseexport: ACK-Boolean und auf 120 Sekunden begrenzte lokale Wartezeit, keine UUIDs/Serveradressen/Chats.
- OpenGenesisLINK Server main 7c6b0cf7 nur optionales OGLAssetService Buildplugin, keine aktivierte Serverschnittstelle. **39 %** Gesamtfortschritt. Nächste 10 Blöcke **Alpha.305–314**. Echter Grid-Self/Outfit/Bake-Pixelbeweis noch NOT_RUN.


---

## Alpha.285–294 (17.10.2026) – Firestorm Original SelfAvatarAppearance Handshake

- Echte CircuitConnection versendet nun RegionHandshakeReply mit SSA-Flag Bit 2 (0x00000004), bisher Flags0. Verified gegen Second Life viewer main llviewerregion.cpp / llviewerregion.h. Bits0/1 bleiben deaktiviert, kein implementierter LL-Objektcache.
- F3 Avatar und Bake-Seiten zeigen getrennt lokalen, zuverlässigen SSA-Sendepfad und tatsächlich empfangene Self-Serverdaten. Kein Avatar-/Bake-Upload durch diese Änderung. Privatdiagnose enthält nur send-Bool.
- OpenGenesisLINK Server main 7c6b0cf7 ist lediglich OGLAssetService plugin loading build fix, weiterhin disabled-by-default; kein zwingender CAP- oder Backend-Wechsel.
- Tests: Native LLUDP-Golden-Wire, UI, Export, Mint NVIDIA GL/Python/Headless/Xvfb und Pi5-CI; Live-NexVerse-SelfAppearance-/Outfit-End-to-End OFFEN. **39 %**. Als nächstes **Alpha.295–304**.


---

## 17.10.2026 / Alpha.275–284 – unabhängiger original-LLM GPU Depth-Proof

- Head/Upper/Lower: drei getrennte GL_ANY_SAMPLES_PASSED-Queries (je zwei asynchrone Slots), Epoch-Tags und monotone Ergebnis-Sequenzen, OR-Gesamtergebnis statt Last-Query-Wins; null wenn originale LLM-Draws fehlen.
- Sichtbare F3-Seiten zeigen pro Body-Part getrennt Server-TE-Referenz, GPU-Residenz, echten Original-BakeGL-Draw und GPU-Depth-Status V/0/?/-. F3 manuelle Textdiagnose enthält ausschließlich sechs Bool-Felder.
- OpenGenesisLINK main 7c6b0cf7252189707066f09ad66c732642b27af9 – optionaler OGLAssetService-Build/Konstruktor-Fix, addon disabled-by-default; keine neue bestätigte Avatar-/CAPS-Wire-Anforderung.
- Tests: C++/NVIDIA-GL/Mint-Xvfb-Headless/Python synthetisch. Kein echter OpenSim/NexVerse-Outfit/Avatar-End-to-End-Test. Gesamtfortschritt **39 %**, nächste Alpha.285–294.

Neuester Serverreview 17.10.2026: OpenGenesisLINK main 5fcfc7f486660c8c27586e69eefc62cf0b9d22a1 (+2 Commits nach 8ec7ea; einzig bin/Robust.HG.OGLAsset.ini.example). Dieses vollständige Hypergrid-ROBUST-Beispiel ist EXPERIMENTAL, DISABLED BY DEFAULT und ausdrücklich nur für isoliertes Staging mit eigener DB/Assets/Secrets. Kein Laufzeit-Protokoll- oder Viewer-CAP-Wechsel, KEIN automatisches Aktivieren/Deployment.

## 17.10.2026 / Alpha.265–274 – Server-Bake-Inspektor und GetTexture-Replanung

- Viewer-Core liest ausschließlich authentifizierte Self-AvatarAppearance-TE-Bakes in der aktuellen Region: 11 Slots mit realer GPU-Residenz, 3 Head/Upper/Lower Original-LLM-GL-Draws und unabhängigen asynchronen GPU-Depth-Werten. Source- und Native-Tests belegen die Datenwege, kein echter NexVerse-Pixelvergleich.
- F3 Viewer-GUI wechselt per B/echtem Button Übersicht/2 Bake-Seiten, R plant bei expliziter Nutzeraktion höchstens 3-mal pro Epoche (12s Cooldown) die tatsächlich fehlenden, bereits fälligen Bakes über den bestehenden GetTexture-Stream neu ein.
- OpenGenesisLINK Server main 8ec7ea255901260abdeabfc5f17eeb850930d851 mit deaktiviertem AssetService-Addon: keine bestätigte neue AvatarAppearance/GetTexture-Protokollaktion.
- Status synthetisch getestet, Live-Grid/Outfit/Bake/Pixels NOT_RUN; Gesamtfortschritt **39 %**. Nächster Lauf Alpha.275–284.

Neuester Serverreview 17.10.2026: main 8ec7ea255901260abdeabfc5f17eeb850930d851 (+6 Commits seit bf55a17524a9f3f175c56d2adeaa4d39083f446f), ausschließlich OGL-AssetService-Addon-Build/CI, Legacy FSAssetService bleibt aktiv; keine neue Viewer-CAP/LLUDP-Laufzeitanforderung.

## 17.10.2026 – Alpha.255–264 / Terrain-Retry-F7 (synthetischer Nachweis)

- Viewer-Core (application.cpp, terrain_detail_retry.hpp): material-/regionsgebundener manueller GetTexture-Nachladeversuch; vier automatische Versuche, 30s Backoff, max. zwei bewusst freigegebene manuelle Budgets, nur fehlende Texturlayer.
- Viewer-UI (viewer_shell.hpp, viewer_shell_render.cpp): sichtbarer, realer Regions-F7-Button, Shortcut T und vier getrennte Layer-/CAP-/Backoff-Stufen. Layout- und Native-Tests belegen die Handlung, **nicht** den echten Grid-Inworld-Download.
- Upstream opengenesislinkserver main bf55a17524a9: OGL AssetService Addon disabled-by-default, Betreiber-/Serverfunktion; aktuelles GetTexture-CAP-Verhalten NICHT ungeprüft ersetzen, kein Admin-Key in Viewer.
- Status tested_synthetic für Policy/GUI; tatsächliche Welt-/Avatar-/Outfit-/Terrainpixel und Firestorm-Parität NOT_RUN.
- OGL Gesamtfortschritt **39 %**, unverändert. Nächste zehn Blöcke Alpha.265–274.

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

## Alpha.109

Alpha.109 Server main SHA 2b7c19c9396eb2f75fd60a41a33a05655909da6b Precheck gruen. Firestorm Wearable Subtype Flags und Original LLUDP Low383/84 als User-Viewer-Core Paketformate, aber keine mutierenden Appearance-Befehle bevor Original Wearable/Bake-Daten korrekt sind. Keine Betreiber-API und keine AdminKeys im User Viewer.

## Alpha.110

Alpha.110 OpenGenesisLINK main 2b7c19c9396eb2f75fd60a41a33a05655909da6b zu Beginn geprüft, keine neuen Commits. OpenSim Legacy Asset Transfer Request Low153 Type5 Clothing/13 Bodypart, Info154/Packet High17, owner-scoped COF Inventory CAPS. Original-Userdaten nur vom Simulator und aktuellem Grid. Keine Operator-Einrichtung/Robust Admin Keys oder nicht vorhandener AssetHTTP-Endpunkt erfunden.

## Alpha.110 aktualisierte Live-Server-Prüfung (14.10.2026)

Nach Entwicklung, vor Release-Gate, neues OpenGenesisLINK main SHA 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94, Commit #208 ('simple GridUser Level 200 Control Center login'); Altstand 2b7c19c9396eb2f75fd60a41a33a05655909da6b. GitHub Commit-Patches und Quellcode MANUELL geprüft: NexApiDocsPage.cs, NexVerseWorldApiConnector.cs, OglApiDocsAccess.cs sowie die beiden CI-Checks. Nur 1 Commit/5 Dateien. Operator Control Center wechselt von eigener '/api/v1/auth/admin/session' plus vorheriger AdminAll/TLS-Proxy-Absicherung zum normalen GridUser '/api/v1/auth/session' mit serverseitigem UserLevel >=200. TrustedProxyAllowlist/TLS-Sondercheck wurden aus dem Admin-Dashboard-Flow entfernt.

Klassifizierung: SERVER_ONLY_OPERATOR_ADMIN_AUTH_REVIEW, **NICHT** im normalen OGL Viewer implementieren, keine Admin-Tokens oder WorldAPI Operator-Einstellungen im Viewer. Kein Änderung am OpenSim Firestorm LLUDP TransferRequest153/Info154/Packet17 oder User-Inventory-CAPS. Sicherheitshinweis: Server-Betreiber muss bei diesem vereinfachten Dashboard-Login echte HTTPS-Terminierung und Rollen-/UserLevel-Grenze vor Exposition prüfen; das wird durch eine Viewer-Regression nicht automatisch abgedeckt. Server-Baseline nach Review auf 5d9c616... gesetzt; strikter Check anschließend erneut.

## Alpha.111

Alpha111 Server-Referenz main 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94, #208 OperatorAdminLogin bleibt SERVER_ONLY. User-VIEWER nutzt bereits vorhandenes authentifiziertes GetTexture Region CAPS mit PNG/X-J2C fallback, per Wearable Original UUID und COF/Epoch-Guard. Keine Admin-Token/secret APIs.

## Alpha.112-115

Alpha112-115 Server main 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94 live verifiziert. Viewer-Core liest Firestorm XML lokal und nutzt nur bereits authentifizierte GetTexture Daten, kein Serveroperator Rest, kein Asset Write, kein AvatarBake CacheID Fake. #208 Control Center bleibt Server-Only.

## Alpha.112–115 Live-Serverreview nach Implementierung (14.10.2026)

Der OpenGenesisLINK Server main ist vor Release auf Commit 20a7fd765527807fd9d7887965b5f5f28d42d68f (#209) vorgerückt, zuvor 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94. Alle neun geänderten Dateien und GitHub-Patches überprüft: Control Center v2, WorldAPI Metadaten für 0.9.3.12 als Release (früher 0.9.3.12 entwicklung), Version-aware ChatGPT Guidance, Operator Diagnose/CI, BaseHttpServer malformed XML-RPC => HTTP400 statt impliziter Erfolgsantwort, HG InstantMessage Log-Throttle nur für unauflösbare Empfänger OHNE Aussetzen der eigentlichen UAS/Zustellversuche.

Entscheidung SERVER_ONLY_WORLDAPI_OPERATOR_DIAGNOSTICS_AND_HG_XMLRPC: Keine User-Viewer LLUDP Transfer/GetTexture/COF Änderungen, kein neues API/Secret im OGL Viewer. Weiterhin OpenGenesisLINK=Server-Produkt, NexVerse=konkretes Grid. Die Rollenprüfung UserLevel >=200 ist nur für den Serveradministrator; darf NICHT in die Anmeldung des normalen Viewers einfließen. Release gilt als serverseitige Metadatenangabe, nicht als Live-NexVerse-Abnahme einer fertig gebackenen Avatar-Kleidung. Baseline aktualisiert auf 20a7fd... und strikten GitHub-Check anschließend wiederholen.

## Alpha.116-119

Alpha116-119 OpenGenesisLINK main 20a7fd765527807fd9d7887965b5f5f28d42d68f in Baseline. Original Firestorm TGA lokal, User COF/auth Firestorm LLWearable/visual params in Viewer, kein API-Bedarf oder Operator-/WorldAPI-Secret; noch keine zulässige Server-Bake-Mutation.

## Alpha.120-123

Alpha120–123 server reference main 20a7fd765527807fd9d7887965b5f5f28d42d68f prechecked, #208-209 server-only. No changes to authenticated user LLUDP or GetTexture CAPS contract. New Viewer-only local color math stays separate from server-supplied AvatarAppearance baked TextureEntry IDs; never send unsupported AgentSetAppearance/BakeIDs or operator tokens.


### Alpha.124–127: original Firestorm global color & multi-mask diagnostics (2026-10-14)
Real avatar_lad.xml global skin/hair/eye palettes and only authenticated actual LLWearable type-owned parameters; two-plus-mask ADD/MULTIPLY order. Unsupported domain/morph/bump/visibility fails closed, F9 shows color/mask/11-region input counts. No pixel-identical Firestorm bake, no server upload or 3D clothing proof. New tests + projectplan, docs/ALPHA124_127_GLOBAL_MASK_COVERAGE.md.

Upstream OpenGenesisLINK 4e999766 (#210): Hypergrid friends reconnect status/negative cache are SERVER_ONLY; existing avatar online/offline viewer packets unchanged. Full viewer friends GUI compatibility remains CORE backlog, not separate admin credentials.

## 15.10.2026 – Alpha.128–131 Firestorm/OGL-Kompatibilität

F9 zeigt nach aktivem Appearance-Klick elf originale Server-TE-Bakes getrennt von GPU-Residency und Head/Upper/Lower. Kein vollständiger Bake, Upload oder Wear. Details in docs/ALPHA128_131_PI5_BAKE_EVIDENCE.md. Upstream main d31c0316 unreviewed (22 Commits, >=300 Compare-Dateien), Pin bleibt alt; 2 native C++23 und 18 Python Pi-Tests PASS, CMake/GPU nicht verfügbar.

## Alpha.132–135 (15.10.2026)

Server-echte Avatar Head/Upper/Lower BakedTextureIDs werden vor Welttexturen priorisiert. F9 zeigt nach explizitem Klick Read-only GPU-Residency live. Vollständige Firestorm-Bake-Pixelparität und Wear/Upload noch offen. Vier native Pi C++ plus 18 Python PASS; Upstream strict weiter BLOCKIERT. Details in docs/ALPHA132_135_AVATAR_BAKE_STREAMING.md.

## 15.10.2026 – Alpha.136–139 / Renderer + F9

Original Firestorm Avatar-Body-UVs werden vor GL-Upload validiert,
per-frame Head/Upper/Lower-Bake-GL-Draws pro originalem LLM-Körpermesh
gezählt, und in F9 als reine Draw-Provenance angezeigt. Kein Pixel-
oder Wear-Nachweis. Native Pi5 C++-Tests 5/5, Python 22/22.
Vollständiger Grafiktest NICHT erfolgt; OpenGenesisLINK Server-Pin
bleibt bis vollständiger Einzelcommitprüfung unverändert.
Siehe docs/ALPHA136_139_GPU_BODY_UV_PROOF.md.

## 15.10.2026 – Alpha.140–144 (Core/Firestorm/GUI/Server-Gate)

Read-only reale GPU-Server-Bake-Vorschauen Head/Upper/Lower in F9,
korrekter Shader-UV-State-Reset, Body Quell-MIP0 Priorität ohne
VRAM-Budgetumgehung. Ein Live-Inventar von 22 OpenGenesisLINK Commits
und 1276 File-Einträgen ist vollständig; manuelle Prüfung und
Kompatibilitäts-Pin stehen weiterhin aus. Native Pi7 C++23, Python28
PASS. Full GUI/GPU-Firestorm-Parität NICHT bestätigt.
Siehe docs/ALPHA140_144_GPU_BAKE_PREVIEW_AND_UPSTREAM_AUDIT.md.

## 15.10.2026 – Alpha.145–149

Identity-bound server Head/Upper/Lower, klickbare große F9 GPU Vorschau, responsive Layout und Region-/Logout-Reset. Firestorm Baked-Pixel und echter Wear/Unwear weiter OFFEN. Pi 8 C++23 und 31 Python PASS, voller GL-/GUI-Build NICHT GETESTET. Upstream main d31c0316 bleibt wegen unvollständigem Codepatchreview gesperrt; neue Live API Abfrage erhielt HTTP 403. Details: docs/ALPHA145_149_AUTH_F9_SERVER_REVIEW.md.

## 16.10.2026: Alpha.150-154

Original Firestorm 3-part body resource auto-discovery on Linux Mint. Original LLM GPU 7,186 triangles, 133 joints, 55 attachments loaded in isolated GUI smoke. Native NVIDIA GPU tests and all 113/113 CTest cases, 33/33 Python pass. J2C/JP2 decoder contract checked against new upstream server OpenJPEG fallback ec760cac, reviewed SHA not advanced. Inworld Avatar/Outfit/COF readback NOT yet proven. Detailed report: docs/ALPHA150_154_LINUX_MINT_FIRESTORM_GPU_BUILD.md.

## 16.10.2026 – Alpha.155-159

Original Nightly and Release Firestorm character assets all 96 byte-identical, nightly user-cache autodiscovery. Read-only F9 GPU Server-Bakes immediately visible and show 0/3..3/3 status. GL3.3 actual texture pixel readback and stale handle eviction verified on NVIDIA GPU. Login screen displays actual original body GPU-ready or fallback status. Mint full Build and native CTest 113/113, Python 34/34 PASS. Inworld Avatar/Outfit readback not verified. Upstream main ec760cac J2K fallback classified; reviewed baseline not changed. See docs/ALPHA155_159_NIGHTLY_F9_GPU_UI.md.

## 16.10.2026 – Alpha.160–164 (isolierte GUI, Avatar HUD)

Alle automatischen GUI-Viewer-Starttests ausschließlich Xvfb, keine sichtbaren Programme auf Nutzerdesktop. Inworld UI meldet jetzt tatsächliche Original-LLM-, Proxy- und Self-Head/Upper/Lower-Bake-Draws statt nur initialisierte Ressourcen; F9 und MIP0 wählen SelfAvatar per find_if (unabhängig von Vektor-Reihenfolge). Native x64 CTest 114/114, Python 38/38 PASS, echter Xvfb GUI-Smoke mit 133 Firestorm Joints und 7.186 Triangles PASS. Kein authentifizierter Simulator-Avatar oder echtes Wear/COF-End-to-End bestätigt. OpenGenesisLINK main ec760cac unverändert, server review bleibt offen. Siehe docs/ALPHA160_164_ISOLATED_GUI_AND_AVATAR_HUD.md.

## 16.10.2026 – Alpha.165–169

Simulator-authentifizierte SceneReady tritt nun vor später TerrainLayerData auf. Volle Terrain-Readiness bleibt ein separater Vertrag. Inworld HUD zeigt echte WorldModel Objekte und GL-Drawevidence sowie Terrain 0..256, CAPS Lifecycle ohne falsches LIVE. Native WorldModel, HUD und NVIDIA-GL Tests prüfen Szene und Meshdraw vor erstem Terrain-Paket. Inworld Login/Avatar und COF Wear/Unwear weiter offen, Server-Review-Pin bleibt unverändert. Siehe docs/ALPHA165_169_EARLY_SCENE_AND_WORLD_DIAG.md.

## 16.10.2026 – Alpha.170–174

AvatarPipelineProof liefert read-only authentifizierte Agent/Region AvatarAppearance Server-TE/Bake und GPU-Residency sowie tatsächlich im Frame gezeichnete Original-LLM/BakeGL-Werte. Neue sichtbare Inworld-Karte mit H/U/L und Packet-Missing Status; positive Farbe nur bei kompletten 3 Texturen und 3 originalen Meshdraws. Überlappte CAPS-/Systemstatuskarten repariert; enge Fenster ohne überdeckte FPS-Leiste. GPU-Regression mit tatsächlichen GL-Submissions und Eviction, Native Header-Pi5-Tests. Kein vollständiger E2E Grid-Login, kein bestätigtes Outfit-Wear, Server-Reviewed-SHA bleibt blockiert. Details: docs/ALPHA170_174_AVATAR_PIPELINE_GUI.md.

## 16.10.2026 – Alpha.175–179

Inworld-Status zeigt WorldModel/GL und nun Textur/Mesh GPU-Residency mit eindeutigen aktuellen UUIDs getrennt. Responsive Netzwerk-/System-/Avatar-Karten überdecken keine FPS-Leiste bei niedrigen Fensterhöhen; Tests an Layoutbreakpoints. GL3.3 Nvidia Regression bestätigt echte Texture-Residency-zu-Eviction-Verläufe. Firestorm originale Body-/Bake-/Outfit-Pipeline bleibt Basis, keine Inworld-E2E-Freigabe. Server reviewed SHA unverändert ec760cac main als Referenz. Siehe docs/ALPHA175_179_WORLD_ASSET_STREAM_AND_RESPONSIVE_HUD.md.

## 16.10.2026 – Alpha.180–184

Die OpenGenesisLINK/NexVerse Third-Person-Kamera normalisiert ungültige/nicht-unitäre Agent-Quaternions und begrenzt unplausible Serverpositionen auf einen endlichen lokalen Fallback. Inworld GUI signalisiert Kamera-Fallback; Original-Firestorm-/World-GL-Mesh bei normalem und ungültigem Quaternion-Fallback mit echten GPU-RGBA Framebuffer-Pixeln auf verstecktem NVIDIA OpenGL3.3 getestet, Terrain-LayerData nicht vorausgesetzt. Ein echter eingeloggter Avatar und endgültige Firestorm-Bake-/Outfit-/Szenenparität sind weiterhin NICHT bestätigt. OpenGenesisLINK main ec760cac, Server-reviewed-SHA unverändert. Details: docs/ALPHA180_184_THIRD_PERSON_CAMERA_AND_GL_FRAMEBUFFER.md.

## 16.10.2026 – Alpha.185–189

Firestorm-kompatible Weltbedienung erweitert: direkt anklickbarer HUD AUS/AN F5 Weltfokus mit erhaltenem 3D-Renderer, F9 und Floater-Funktionen. Hidden Hud verhindert Ghost-Bedienflächen und leitet Weltklicks zur bestehenden 3D-Objektauswahl. Das OpenGL-Backend verwirft vollständig hinter der Kamera liegende Szenenobjekte statt sie als Welt-Draws zu zählen; NVIDIA-GL Framebuffer/Gegenblick-Regression plus native C++23 Widescreen/Renderer-Tests. Echter authentifizierter Grid-/Avatar-Framebuffer steht weiterhin aus. Upstream ReviewedSHA gesperrt. Siehe docs/ALPHA185_189_WORLD_FOCUS_AND_VISIBILITY.md.

## 16.10.2026 – Alpha.190–194

Firestorm-kompatibler Third-Person-View-Zoom nun per Wheel, Keyboard und echten anklickbaren +/- Button mit Live-Meterzahl von 3..24m. Renderer, Raypick und Texture-/Mesh-Priorität nutzen dieselbe Pose, ohne Simulator-Movement zu verändern. F9 Read-only Server-Bake-Vorschau hat sichtbare B-Taste. NVIDIA-OpenGL3.3-Framebuffer-Pixeltest bestätigt reale Near-/Far-Zoomwirkung am synthetischen Weltmesh. Echter Grid-/Avatar-Pixelbeleg und komplettes COF Wear weiter offen; Server-ReviewedSHA unverändert. Siehe docs/ALPHA190_194_CAMERA_ZOOM_AND_F9_BAKES.md.

## 16.10.2026 – Alpha.195–199

Echter lokaler Avatar-Kameraorbit: Front/Seite/Hinten per V/[ ] und drei sichtbare, klickbare Schalter. Firestorm-basierter GL-Renderer und WorldRay-Picking nutzen gleiche Pose, Avatar bleibt simulatorauthentisch. NaN-/Inf-Zoom-Normierungsfehler behoben. Native Mint-/Pi5-C++-Tests und echte NVIDIA-OpenGL-Pixelprobe aus Front- und Rueckansicht mit synthetischen Meshes. Echter NexVerse-Login mit sichtbarem Avatar und vollstaendige 11-Bake-/Outfit-Paritaet weiterhin ausstehend. Server reviewed SHA unveraendert. Details: docs/ALPHA195_199_AVATAR_CAMERA_ORBIT.md.

## 16.10.2026 – Alpha.200–204 Branding

Das vom Nutzer bereitgestellte Original-OG-Viewer-Icon wird als echtes GLFW/X11-Cinnamon-Taskleistenicon und OpenGL-Logo oben links im Login verwendet. Linux hicolor Starter, Windows ICO und macOS ICNS stehen als native Ressourcen bereit. Vorhandener WelcomePage WebKit Browser und Grid-Login bleiben unverändert; nur Navy/Cyan-Details frischer gestaltet. Keine Full-UI-Mockups oder Server/Avatar-/Wear-Protokolländerungen. Siehe docs/ALPHA200_204_BRANDING_SYSTEM_ICON_AND_LOGIN.md.

## 16.10.2026 – Alpha.205–209

Kamera-Blickfeld-Projektion anhand aktueller AgentPosition, verifizierter Avatar-Render-GL-Draws und Originalicon in der 3D-Welt-HUD-Kopfzeile. Loading-Overlay ermittelt konkreten fehlenden Welt-Entry-Schritt (LLUDP, Region oder Agent), ohne optionales Terrain/CAPS als Voraussetzung zu behaupten. F5 meldet ausschließlich fehlenden Avatar im leeren Weltfokus. Live Grid-Avatar-/Pixel-Parität weiter unbewiesen; keine COF- oder Server-Änderung. Details: docs/ALPHA205_209_WORLD_ENTRY_AND_AVATAR_VIEW_EVIDENCE.md.

## 16.10.2026 – Alpha.210–214

Verbindlicher fünf-Blöcke-Durchlauf je WEITER. Echter OpenGL-3.3 Query für Front/Upper/Lower Original-LLM Self-Avatar-Draws: GPU-Depth-Fragment ja/nein/unknown statt irreführender Draw-Proof, async+RegionEpoch sicher, Native GPU-Regression. Echte interaktive F4 AVATAR Fokus-Taste und Button neben den bisherigen Orbit-/Zoom-Controls (8m, frontal), in beiden F5-Modi. WELCOME und Originalicon bleiben erhalten. Kein NexVerse-Live-Grid-/Appearance-Pixel-E2E-Claim. Dokumentation: docs/ALPHA210_214_FIVE_BLOCK_RULE_AVATAR_SAMPLES.md.

## 16.10.2026 – Alpha.215–219

GPU-Avatar-Mesh-Fragmente dürfen nach Kamerabewegung/Originalmesh-Ausfall nicht als aktuelle Werte weiterlaufen. F3 (bestehender Avatarstatus) zeigt aktuelle Kamera-/Original-Linden-Mesh-/ServerBake-/Depth-Samples und hat echten [S]-Bericht-Button; lokaler Export mit POSIX 0600 und Whitelist ohne Identität, UUIDs oder Geheimnisse. F4 Kamera, Login, original OGL-Icon, WebKit-Welcome, F9 Outfit, und bestehende Server-LLUDP unverändert. Kein tatsächlicher NexVerse-Avatar-End-to-End-Beweis. Details: docs/ALPHA215_219_AVATAR_DIAGNOSIS_AND_QUERY_FRESHNESS.md.

## 16.10.2026 – Alpha.220–224

F3 WorldEntry-Diagnose vor WorldReady, echte getrennte LLUDP/Region/Agent/CAPS-Flags, private Datei ohne Secrets, anklickbarer manueller Circuit-Retry nur nach vollständig fehlgeschlagenem Worker und bekanntem Simulatorziel. Loading-Progress aus Pflichtmilestones, nicht CAPS/Terrain. Original-NexVerse-WebKit-Welcome und Firestorm-Outfits/UI bleiben unangetastet. Kein echter Grid-Avatar/Retry-E2E. Details docs/ALPHA220_224_LOADING_DIAGNOSTICS_AND_MANUAL_RETRY.md.

## 16.10.2026 – Alpha.225–229

Fünf echte Entwicklungsblöcke pro WEITER im neuen Chat dauerhaft verbindlich; pro Session max. fünf WorldEntryStage-Enum-Werte mit Haltezeit in F3 (nur vertraulichkeitsfreie Fakten), LLUDP Retry-Grund aus tatsächlicher abgeschlossener Circuit-Worker-/Region-Endpoint-/Cooldown-Policy und privatem explizitem POSIX 0600-Diagnoseexport. Klickbarer F3-Ladestatus und Original Welcome bleiben erhalten. Kein echter NexVerse-Welt-/Avatar- oder Retry-End-to-End-Beleg. Bericht docs/ALPHA225_229_WORLD_ENTRY_TRACE_AND_RECONNECT_REASON.md.

## 16.10.2026 – Alpha.230–234

Self-AvatarAppearance realer LLUDP-Handler-Status, GL-Terrain-Shader Textur/Fallback-Draw sichtbar, F2 DETAIL/SPAREN steuert MeshLoD Hysterese und Prioritaet inklusive Neuplanung, Kamera auf 5.5m und groessere HUD Statusschriften. Welcome/Icon bleiben unveraendert. Echter ServerBake/Welt-Pixelbeweis weiterhin OFFEN. Bericht docs/ALPHA230_234_VISIBLE_WORLD_AVATAR_MESH_UI.md.

## 16.10.2026 – Alpha.235–244

Verbindlich zehn echte Entwicklungsblöcke je WEITER und
Projektstatus 38 % unverändert. Neue echte Self-Avatar-
BodyBake-/Keyframe-GL-Diagnosen, 4/4 Terrain-RGBA-Layer
gegen 1x1-Ersatz, exakte Mesh-UUID+LOD-Matches auf der GPU
und zusätzliche Anzeigen in HUD, F3 Avatar/Objekte und
privatem 0600-Diagnoseexport. Weiterhin KEINE bestätigte
vollständige Firestorm-/NexVerse-Live-Avatar-, Terrain-
oder Serveranimations-Parität. Original-Welcome/Icon erhalten.
Details: docs/ALPHA235_244_VISUAL_EVIDENCE.md.


## 17.10.2026 – Alpha.245–254

Echte Terrain-GetTexture-Partial-Recovery mit maximal
vier Backoff-Versuchen pro Materialregion, kumulierte
bereits echte RGBA-Pixel, fortschreitender OpenGL-Upload
ohne Materialrevisionswechsel und ohne Downgrade.
Vorher fehlerhaft kummulierte Terrain-Draw-Zähler jetzt
pro Frame korrekt; NVIDIA-GPU-Test statt bloßer
Netzwerkzähler. F7 Region zeigt Retry/Mesh, Self-Avatar-
Appearance-Zähler nach Regionen getrennt, Animationen-
Fenster und Logs zeigen echte Keyframe-Skin-Nutzung.
Neuer OpenGenesisLINK-Server-Mesh-Upload-LLSD-Patch
d8f4426 ist nur auf Uploads bezogen, nicht GetMesh.
Project-Status 39 % als gewichtete Schätzung; echte
NexVerse-Firestorm-Avatar-/Welt-Pixelparität weiterhin OFFEN.
Siehe docs/ALPHA245_254_TERRAIN_RECOVERY_ANIMATION.md.

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
