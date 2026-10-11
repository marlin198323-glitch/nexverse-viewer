# OpenGenesisLINK Viewer – Avatar und Bewegung

Stand 0.2.0-alpha.52 vom 10.10.2026

Ein eingeloggter OpenSim-Viewer kann Welt und Meshes anzeigen,
ohne selbst bereits spielbar zu sein. Die bisherige lokale
AgentUpdate-Ausgabe reichte ohne Self-Avatar-Synchronisierung
und Avatar-Renderpfad nicht aus.

## Grundbedienung

W/S: vorwärts und rückwärts.
A/D: seitwärts links und rechts.
Pfeiltasten links/rechts: Avatar drehen.
Pfeiltasten oben/unten: vorwärts/rückwärts.
Shift: schneller.
Leertaste: springen / aufwärts.
C: abwärts.
F: Flugmodus an/aus mit erneutem Tastendruck.
F11: maximierte Ansicht / exklusives Fullscreen umschalten.

Die Steuerung sendet nur bei aktivem LLUDP-Circuit
und fokussiertem Fenster AgentUpdate an den OpenSim-Server.
Positionsdaten werden nicht als serverbestätigt ausgegeben,
solange kein passendes Self-Avatar-Paket eingetroffen ist.

## Avatar-Darstellung

Bis zur echten Avatar-Appearance werden eigene und andere
bekannte Avatare mit einem vereinfachten Humanoid aus
vorhandenen primitiven GPU-Meshes visualisiert. Das macht
Drehung und Position visuell kontrollierbar, ersetzt aber
weder Skeleton, Texturen/Bakes, Kleidung, Mesh-Outfits,
Attachments noch Animationen.

## Status und Live-Test

Zur Abnahme nach der Anmeldung W/A/S/D und Pfeiltasten
betätigen; die Logs sollten dann Folgendes zeigen:
- Avatar controls active
- AgentUpdate controls changed
- Avatar self local-id bound
- Avatar self position synchronized

Fehlt die LocalID oder die Simulator-Positionsbestätigung,
wird die Ursache anhand der Avatar-Update-Pakete untersucht.
Kein Teleport, keine unbegrenzte Fake-Client-Prediction.

Die weitere Reihenfolge steht im verbindlichen projectplan.md.


## alpha.52 – Avatar sichtbar machen und Bewegung · 10.10.2026

**Auslöser:** Der Screenshot der verbundenen Welt zeigt
Terrain/Meshes, aber keinen eigenen Avatar und bislang
keine nachvollziehbare Bewegung. WORLD UDP LIVE beweist
weder den Avatar-Renderpfad noch die Bewegungsfähigkeit.

**Implementiert:**
- OpenGL-Avatar-Proxy-Pfad mit sieben Basis-Meshes für
  Kopf, Torso, Arme, Beine, für eigenen Avatar und bis
  zu 63 weitere Avatar-Datenpunkte. Die Darstellung wird
  pro Frame anhand der Agentposition aktualisiert.
- Die Proxies sind absichtlich keine echten OpenSim-Avatare:
  keine Skin-Bakes, Avatar-Meshes, Skeletons,
  Wearables, Haare, Attachments oder Animationen.
- Neues Keyboard-Control-Modul mit W/S/A/D, linker/rechter
  Pfeiltaste zur Rotation, Shift für schnelleres Bewegen,
  Space für Sprung, C für Abwärtsbewegung, F Flug
  per Toggle. Die AgentUpdate-Pakete enthalten dabei
  korrekte Körper-/Kopf-Quaternionen und Kameraachsen.
- Self-Avatar-LocalID wird aus ObjectUpdate bzw.
  ObjectUpdateCompressed gewonnen, mit dem RegionHandle
  gekoppelt und bei ImprovedTerseObjectUpdate
  zur Aktualisierung der serverbestätigten eigenen
  Avatarposition im WorldModel verwendet.
- Bei Steuerungsänderungen und bestätigten Simulator-
  Positionsupdates werden diagnostische Logzeilen geschrieben.
  Fokusverlust sendet keine gedrückten Bewegungs-Keys.
- Nach erfolgreichem Login wird das Login-Eingabefeld
  nicht mehr durch die Bewegungssteuerung verändert.
- Regressionstests für Steuerbits, Quaternion, Fly-Toggle
  und Steuerungsreset; 26/26 CTests grün.

**Noch offen und nicht als fertig behaupten:**
- Echte NexVerse-In-World-Bewegungsabnahme, insbesondere
  muss die Self-Avatar-LocalID und die Terse-Positions-
  Bestätigung mit neuer Version beobachtet werden.
- Richtige Firestorm-Avatar-Appearance (Skeleton,
  Body Shape, Skin-Bakes, Wearables, Attachments,
  Animationen) sowie Mouselook, Camera-Orbit/Zoom,
  Teleport/Regionswechsel, Collision/Reconciliation.
- Die einfachen Avatar-Proxies sind nur eine
  Übergangsdarstellung bis zu Phase-10-Appearance.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl52-live.log


## Alpha.85 – COF-Wear-Preflight ist kein Avatar-Appearance-Update

**Block 1 – Firestorm-ähnlicher Inventarbaum F10 mit echter Ordnernavigation:** Die neue lesende Inventaransicht `F10` zeigt Ordner aus `LoginSession.inventory_skeleton` sowie live nachgeladene `FetchInventoryDescendents2`-Kategorien (Name, Typ, Parent-UUID, Version). Auf-/Ab-Pfeile und Mausklick markieren Ordner, Enter/Rechts öffnen und Links/Backspace gehen zurück; `F10`/ESC schließen. Die Fenster F8/F9/F10 schließen einander aus, Movement-Tasten werden bei geöffnetem Inventar blockiert. Der generische Fetch-Adapter fragt im Hintergrund jeweils genau einen ausgewählten Ordner mittels gültigem, regionengebundenem Simulator-CAP ab; bis zu 256 abgefragte Ordner und 4096 lokal modellierte Folder verhindern ungebremste Rekursion. `owner_id`, `folder_id`, Parent-Zuordnung, LLSD-Typen/Versionsnummer und aktuelle Regionsepoche werden vor Übernahme geprüft. Auch ungültige/fehlende Capabilities führen nicht zu HTTP-Dauerschleifen. Unterordner außerhalb des Login-Skeleton erscheinen erst nach erfolgreichem Simulator-Fetch, nicht aus erfundenen Daten.

**Block 2 – echter `FetchInventory2`-Einzelitem-Abruf für Outfits:** `firestorm_inventory_items.hpp/.cpp` implementiert die OpenGenesisLINK-konforme `POST`-Anfrage `items:[{item_id,owner_id}]` an die vom aktuellen CAPS-Seed beworbene `FetchInventory2`-URL, gebündelt auf maximal 32 IDs aus der aktuell angezeigten F9-Outfitliste. Die Antwort `agent_id` und `items[].permissions.owner_id` müssen exakt zum angemeldeten Avatar gehören; unbekannte IDs und ungültige UUIDs/LLSD-Typen werden herausgefiltert. Lesen und Sanitizing der Itemnamen, Asset-IDs und Typen sind getestet. Die F9-Auswahl startet einen asynchronen On-Demand-Fetch; dessen Ergebnis bleibt mit Folder und Regionsepoche verknüpft. Ein Fetch-Ergebnis eines zuvor gewählten Ordners darf dem neuen nicht als aufgelöste Referenz untergeschoben werden. Die volle Avatar-/Wearables-Appearance-Pipeline ist noch nicht vorhanden.

**Block 3 – COF-/Outfit-Wear-Vorprüfung statt fehlerhaftem Blind-Wear:** Nach Vergleich mit Firestorm `indra/newview/llappearancemgr.cpp` überprüft `firestorm_outfit_wear_preflight` vorhandene Outfit-Items, Current Outfit Folder (COF, LLFolderType=46), referenzierte LLAssetType-24-Links und unverstandene Folder-Links (Typ 25). F9 zeigt einen expliziten Status für noch nicht geladene Inventare/COF, leere Outfit-Folder, nicht aufgelöste Linkziele und noch fehlende Wearable-/Attachment-/Avatar-Appearance-Synchronisation. `can_wear()` bleibt **absichtlich false**: Firestorm-Wear benötigt COF-Linktransaktionen, tatsächliche Wearable-Assets, Attachment-Regeln und Appearance-Abgleich. Der Server- und Firestorm-Quellvergleich erlaubt keinen erfundenen einzelnen `Wear`-CAP; die vollständige Outfit-Aktion bleibt offen. Ein User-Klick darf keinen falschen Erfolg anzeigen.

**Block 4 – explizit bestätigtes Firestorm Experience-Forget:** `F8` unterstützt bei vorhandener aktueller `ExperiencePreferences`-CAP nun zusätzlich `X` zur Vorbereitung von „Vergessen“, gefolgt von **Enter** zur ausdrücklichen Bestätigung (ESC Abbruch). Der Connector nutzt den OpenGenesisLINK-Vertrag `DELETE` auf `<current-capability>?<UUID>`, gegen ungültige IDs und abgelaufene Regionsepochen geschützt. `IHttpTransport::remove()` ist standardmäßig fail-closed, `CurlHttpTransport` unterbindet Redirects für mutierende DELETE-Requests. Erst wenn die Serverantwort die betroffene ID weder in `experiences` noch in `blocked` enthält, meldet der Viewer einen bestätigten Erfolg. Vorbereitete Aktionen werden auch beim Schließen/Wechseln zwischen F8/F9/F10 verworfen. Tests beweisen keine DELETE ohne gesondertes Enter, korrekten HTTP-Queryvertrag, Servernegativantwort- und Stale-Lease-Schutz. Experience-Erwerb, Rollen-/Admin-Bearbeitung und beliebige Write-CAPs bleiben gesperrt.

**Quelldisziplin, 11.10.2026:** Offizielle Firestorm-Basis `FirestormViewer/phoenix-firestorm`, Branch `Firestorm_7.2.4`, HEAD `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` einschließlich `indra/newview/llappearancemgr.cpp`, `indra/llinventory/llfoldertype.h`, Experience-Floater. Aktueller OpenGenesisLINK-Server `main`, HEAD **`885fa72a10fec770d6dc1220f08766437159a084`** – neuer als Alpha.84 (`efc9b3a`); darin u. a. ExperienceInfo-ID-Fix (`2194527`) und ein interaktives Firestorm-LSL-E2E-Testskript (`885fa72`). Für Inventory/COF sind `OpenSim/Framework/InventoryItemBase.cs` und `OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs` maßgeblich (inkl. `item_id`, `permissions.owner_id`, `agent_id`), für Experience-Forget `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Vor weiteren Codeänderungen immer aktuellen Server-HEAD erneut prüfen; eine lokal vorhandene Testdatei beim Server beweist KEINE vom Viewer vollzogene Live-Abnahme.

**Tests und Grenzen:** Für Alpha.85 wurden neue CTests `firestorm-inventory-items-tests` und `firestorm-inventory-tree-wear-tests` integriert sowie Forget-/COF-/F10-Owner-/Region-Regressionen erweitert. Insgesamt 80 CTests geplant und im finalen Debug-C++23-Versions-Neubau mit echten GPU-Tests zu prüfen. Ein synthetischer HTTP-Fixture-Test ist `tested_synthetic`, nicht `live_verified`. Der GUI-Welcome-Smoke verifiziert nicht die post-login F8/F9/F10-Fenster; echtes NexVerse-InWorld-Testing, Avatar, Bewegung, EventQueue/Teleport, Meshes und Outfit-Wear bleiben als nicht abgenommen markiert.

**Nächste 3–4 Blöcke Alpha.86:** Echte Firestorm-COF-/Wearable-Asset- und Attachment-Transaktion anhand `llappearancemgr.cpp` und OpenGenesisLINK-Serverrechten entwerfen/implementieren und mit Live-Avatar-Readback absichern; den F10-Baum um gezielten Item-Readback und Folder-EventQueue-Änderungen ergänzen; Experience-Consent-/Avatar-/Permission-Dialoge gegen aktuellen Simulator robuster machen; authentifizierte In-World-Abnahme (Avatar, WASD, Mesh und EventQueue) mit realem Grid-Zugang priorisieren. Volle Firestorm-Funktionsparität bleibt die verbindliche Baseline.
