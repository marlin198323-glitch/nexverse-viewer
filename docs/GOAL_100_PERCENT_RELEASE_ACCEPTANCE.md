# OGL Viewer – Abnahmekriterien bis 100 %

Stand 24.10.2026, Branch develop-current, Version 0.2.0-alpha.1144, aktueller gewichteter Fortschritt 38,65 %.

## Verbindliche Freigabe

Der Viewer wird auf Firestorm-Basis für OpenSim und OpenGenesisLINK entwickelt. Grüne synthetische Rendering-/Unit-Tests allein sind keine vollständige Abnahme. Es darf keinen produktiven Release und keine 100-%-Kennzeichnung geben, solange echte Grid-, Bedienungs- und Plattformtests fehlen.

Der lokale Abschlussprüfer python3 scripts/ogl-release-readiness.py bewertet alle sieben Bereiche in docs/PROJECT_PROGRESS.json und verlangt sieben explizite echte End-to-End-Nachweise in final_release_evidence. Aktueller Exitcode 2 = BLOCKED. Ein Release benötigt exakte getestete GitHub-Commit-SHA und ausdrückliche Benutzerfreigabe.

## P0 – Avatar, Skin, Outfit und Authentifizierung

1. Authentifizierter Grid-Login in die reale NexVerse-Region; echte gültige Head/Upper/Lower TextureEntry-Bakes mit GetTexture/LLUDP-Assetnachweis, Decoding und **drei echten RGB-Framebufferpixelproben**. Xvfb-Testfarben zählen nicht als Live-Endbeweis.
2. Original-CurrentOutfitFolder-Wearables beim normalen Login ohne F9 lesen (automatischer nur-lesender FetchInventory2/LLWearable-Pfad ab Alpha.1144); **noch offen** sind vollständige Skin-/Shirt-/Pants-/Hair-/Tattoo-/Alpha-/Universal-Layer-Komposition, verifizierte sichtbare Outfits und vollständige Skinned-Mesh-Attachments.
3. Firestorm Shape einschließlich Bone-/Skeleton-/nichtlinearer Driver-/Anim-Parameter und echte Remote-Avatar-Parität. Aktuelle lokale Morphmesh-Vorschau allein reicht nicht.
4. WASD, Laufen, Springen, Fliegen, Yaw und Steuerung erst nach tatsächlichem **Server-Positions-/Rotations-Readback** als erfolgreich melden. Gesendete AgentUpdate-Kontrollbits reichen nicht.
5. Avatar-/Simulator-/CAPS-Lease sicher bei Teleport, Regionwechsel, Logout und Neu-Login; alte Skin/Outfit-Daten dürfen nicht in neue Sessions übergehen.

## P1 – GUI und Firestorm-Bedienfunktionen

6. Menüs, Fenster, Schaltflächen, Modals, Suche/Tabellen und verknüpfte Funktionen vollständig mit echter Backendaktion testen. Fullscreen-Start, Minimieren, Maximieren, Beenden, Widescreen und originales OGL-X11-Systemicon.
7. Freunde/Gruppen, Profile, IM/Chat, Inventory CRUD, Teleport/Karte, Parzellen/Land und Bauen/Bearbeiten so weit wie der Server es anbietet; zu jedem Klick End-to-End-Nachweis.
8. Welt/Terrain/Himmel/Wasser, Mesh-LOD/Materialien/JPEG2000, Szenenobjekte, HUD- und Inworld-Attachments bei realen Simulatorassets inklusive Performance/Fehlerlog.

## Release und CI

9. Linux-x64-GUI auf realer GPU sowie Windows/macOS GUI/Installer mit dokumentierter MPL-2.0-Quellen- und Asset-Lizenzlage, Security- und Lasttests.
10. Für EXAKTE Feature- und Release-SHAs Linux und Pi5 ARM64 CI erfolgreich abgeschlossen, Build-Artefakte geprüft; Runner-Prozess alleine bedeutet keine erfolgreiche CI. Produktion nur nach ausdrücklicher Freigabe.

## Aktuell real implementiert (Alpha.1144)

Erster echter Datenpfad für Autostart des originalen COF-Items-/LLWearable-Lesens ohne F9 nach authentifiziertem Login; ein Versuch pro exaktem Owner/Region/COF/Version, begrenzt auf 64 Schlüssel je Session. Initial fehlende COF-Ordnerdaten lösen höchstens alle 30 Sekunden einen bestehenden read-only CAPS-Refresh aus. Original-UUID-/Wearable-/Owner-/Epoch-/COF-Sicherheitsprüfungen und manuelles F9 bleiben. F3 kennzeichnet laufende Items, Assetstatus und fehlende Live-Endnachweise. Neuer authentifizierter Live-Grid-E2E-Erfolg noch NICHT erbracht. Der gewichtete Projektfortschritt bleibt 38,65 %.
