# OGL Viewer – Abnahmekriterien bis 100 %

Stand 24.10.2026, Branch develop-current, Version 0.2.0-alpha.1174, aktueller gewichteter Fortschritt 38,65 %.

## Verbindliche Freigabe

Der Viewer wird auf Firestorm-Basis für OpenSim und OpenGenesisLINK entwickelt. Grüne synthetische Rendering-/Unit-Tests allein sind keine vollständige Abnahme. Es darf keinen produktiven Release und keine 100-%-Kennzeichnung geben, solange echte Grid-, Bedienungs- und Plattformtests fehlen.

Der lokale Abschlussprüfer python3 scripts/ogl-release-readiness.py bewertet alle sieben Bereiche in docs/PROJECT_PROGRESS.json und verlangt sieben explizite echte End-to-End-Nachweise in final_release_evidence. Aktueller Exitcode 2 = BLOCKED. Ein Release benötigt exakte getestete GitHub-Commit-SHA und ausdrückliche Benutzerfreigabe.

## P0 – Avatar, Skin, Outfit und Authentifizierung

1. Authentifizierter Grid-Login in die reale NexVerse-Region; echte gültige Head/Upper/Lower TextureEntry-Bakes mit GetTexture/LLUDP-Assetnachweis, Decoding und **drei echten RGB-Framebufferpixelproben**. Xvfb-Testfarben zählen nicht als Live-Endbeweis.
2. Original-CurrentOutfitFolder-Wearables beim normalen Login ohne F9 lesen (automatischer nur-lesender FetchInventory2/LLWearable-Pfad ab Alpha.1144); **noch offen** sind vollständige Skin-/Shirt-/Pants-/Hair-/Tattoo-/Alpha-/Universal-Layer-Komposition, verifizierte sichtbare Outfits und vollständige Skinned-Mesh-Attachments.
3. Firestorm Shape einschließlich Bone-/Skeleton-/nichtlinearer Driver-/Anim-Parameter und echte Remote-Avatar-Parität. Aktuelle lokale Morphmesh-Vorschau allein reicht nicht.
4. WASD, Laufen, Springen, Fliegen, Yaw und Steuerung erst nach tatsächlichem **Server-Positions-/Rotations-Readback** als erfolgreich melden. Gesendete AgentUpdate-Kontrollbits reichen nicht. Seit Alpha.1174 muss die Position zudem mind. 0,25m in die per erfolgreich versendetem AgentUpdate Quaternion+ControlFlags befohlene Welt-Bewegungsrichtung projiziert werden; Gegen-/Querbewegung, Richtungswechsel und Stale/Teleport dürfen keine positive Bewegung quittieren. Auch diese lokale Code-/Unit-Abnahme ersetzt nicht den echten Grid-Live-WASD-/Yaw-Endtest.
5. Avatar-/Simulator-/CAPS-Lease sicher bei Teleport, Regionwechsel, Logout und Neu-Login; alte Skin/Outfit-Daten dürfen nicht in neue Sessions übergehen.

## P1 – GUI und Firestorm-Bedienfunktionen

6. Menüs, Fenster, Schaltflächen, Modals, Suche/Tabellen und verknüpfte Funktionen vollständig mit echter Backendaktion testen. Fullscreen-Start, Minimieren, Maximieren, Beenden, Widescreen und originales OGL-X11-Systemicon.
7. Freunde/Gruppen, Profile, IM/Chat, Inventory CRUD, Teleport/Karte, Parzellen/Land und Bauen/Bearbeiten so weit wie der Server es anbietet; zu jedem Klick End-to-End-Nachweis.
8. Welt/Terrain/Himmel/Wasser, Mesh-LOD/Materialien/JPEG2000, Szenenobjekte, HUD- und Inworld-Attachments bei realen Simulatorassets inklusive Performance/Fehlerlog.

## Release und CI

9. Linux-x64-GUI auf realer GPU sowie Windows/macOS GUI/Installer mit dokumentierter MPL-2.0-Quellen- und Asset-Lizenzlage, Security- und Lasttests.
10. Für EXAKTE Feature- und Release-SHAs Linux und Pi5 ARM64 CI erfolgreich abgeschlossen, Build-Artefakte geprüft; Runner-Prozess alleine bedeutet keine erfolgreiche CI. Produktion nur nach ausdrücklicher Freigabe.

## Aktuell real implementiert (Alpha.1174)

Erster echter Datenpfad für Autostart des originalen COF-Items-/LLWearable-Lesens ohne F9 nach authentifiziertem Login; ein Versuch pro exaktem Owner/Region/COF/Version, begrenzt auf 64 Schlüssel je Session. Initial fehlende COF-Ordnerdaten lösen höchstens alle 30 Sekunden einen bestehenden read-only CAPS-Refresh aus. Original-UUID-/Wearable-/Owner-/Epoch-/COF-Sicherheitsprüfungen und manuelles F9 bleiben. Ab Alpha.1154 folgt nach dem echten fehlerfrei abgeschlossenen automatischen Original-LLWearable-Readback auch der bestehende begrenzte read-only GetTexture-Abruf mit maximal zwölf deduplizierten lokalen Original-RGBA-Quellen bei höchstens 128x128 Vorschaugröße und anschließendem originalen Firestorm-LAD-/TGA-/Globalfarben-/Masken-/Layer-Teilcompositor ohne F9. Seit Alpha.1164 werden diese lokalen Original-Firestorm-LAD-/COF-/RGBA-Teilfarben erstmals **tatsächlich auf den originalen gewichteten Head/Upper/Lower-Meshes des EIGENEN Avatars** gezeichnet, wenn keine echte serverseitige Bake-Textur dafür GPU-resident ist. Die echten Head/Upper/Lower-Simulator-Bakes haben immer Vorrang; fremde Avatare bekommen keine lokalen Previewfarben, beim Region-/COF-Wechsel wird diese lokale GPU-Vorschau gelöscht. Der echte Xvfb-Original-LLM-Framebuffer-Test bestätigt 3398 veränderte eigene RGB-Bildpunkte, 0 Restpixel nach Reset und drei getrennte lokale GL-Draws; die Quellen im Test waren ausdrücklich synthetisch. Die GPU-Bilder bleiben lokale unvollständige Teilvorschauen, KEIN fertiger serverseitig bestätigter Outfit-/Head-/Upper-/Lower-Bake. F3 kennzeichnet laufende Items, Assetstatus, lokale RGBA-Quelle x/y und weiterhin fehlende Live-Endnachweise. Neuer authentifizierter Live-Grid-E2E-Erfolg noch NICHT erbracht. Der gewichtete Projektfortschritt bleibt 38,65 %.
