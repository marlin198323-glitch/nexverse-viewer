# Firestorm-Funktionsparität – verbindlicher Vertrag für OGL Viewer

**Beschluss:** 11.10.2026. Maßgebend: `projectplan.md`, Abschnitt 2.1.
**Produkt:** OpenGenesisLINK Viewer (NexVerse-Design). **Status:** Zielvertrag, keine Behauptung bereits bestehender Vollparität.

## 1. Quellen- und Prioritätsordnung

1. **OpenGenesisLINK Server ist autoritativ** für tatsächlich vorhandene Funktionen, CAPS, LLUDP/LLSD, LSL, Objekt- und Avatarzustand, Sicherheits-/Rechteprüfung, Grid/Region/Robust und serverseitig veröffentlichte Featureflags. Repository: https://github.com/opengenesislink/opengenesislinkserver (Branch `main`). Vor jeder Featureimplementierung und vor jedem Release den aktuellen Branchstand samt Commit-SHA neu abfragen und passende aktuelle Serverdokumente und Codepfade lesen. Version und SHA je Kompatibilitätstest dokumentieren.
2. **Firestorm ist die vollständige Funktions- und Bedienungsbaseline sowie primäre Quellcode-/Implementierungsreferenz.** Quelle: https://github.com/FirestormViewer/phoenix-firestorm . Referenz ist ein veröffentlichter, protokollierter Release-Branch (bei Erstellung existiert `Firestorm_7.2.4`); der jeweils aktuelle Release-Branch ist zu prüfen. `master` ist laut Projekt Nightly und wird nicht ungeprüft als Produktionscode übernommen.
3. **OpenSimulator/Second-Life-Protokoll** bleibt der Wire-Kompatibilitätsvertrag, soweit vom tatsächlich benutzten OpenGenesisLINK-Server unterstützt. Bestehende SL-/OpenSim-Funktionen behalten Semantik, Events, LSL-Konstanten und Berechtigungen. Proprietäre SL-/Havok-Daten oder nicht verfügbare Serverfunktionen werden nicht simuliert.
4. **NexVerse-Produktvorgaben** betreffen eigenständiges Design, modernes Rendering, plattformübergreifende Bedienung, Erweiterbarkeit und optional neue Plugins. Neue Funktionen dürfen die Firestorm-Baseline nicht unbemerkt verdrängen.

**Quellenstand der Erstaufnahme:** OpenGenesisLINK `main` = `cb3ebb6ff05160bf9bdb20e78372a68d7b723512`, 11.10.2026, Releasezug `0.9.3.10 Dev`, letzter Stable `0.9.3.8`. Dieser SHA ist nur ein historischer Audit-Snapshot; *nicht dauerhaft anpinnen*. Firestorm-Referenzrelease zum Start: `Firestorm_7.2.4`, ebenfalls vor Umsetzung auf Aktualität prüfen.

## 2. Funktionsprinzip: zuerst Firestorm erhalten, dann anpassen, zuletzt ergänzen

- Alle in der referenzierten Firestorm-Version vorhandenen normalen **und erweiterten** Bedienfunktionen gehören zunächst in die Funktionsinventur. Frühere pauschale Ausnahmen (RLV/RLVa, Phototools, spezielle Debug-Werkzeuge) sind aufgehoben. Eine besondere Funktion darf nur mit dokumentierter Begründung und offenem Prüfstatus zurückgestellt werden, niemals stillschweigend entfallen.
- Existierende Viewer-Funktionen in ihrem Nutzerverhalten zunächst beibehalten bzw. funktionsgleich implementieren. Anpassungen erfolgen nur für OpenSim/OpenGenesisLINK-Protokoll, lokale Rechte, Sicherheit, Betriebssysteme oder die eigene Oberfläche. OGL-spezifische Funktionen werden als Erweiterung bzw. klar dokumentierter Umbau ergänzt.
- Der bisherige OGL-Viewer ist gegenwärtig eine **eigenständige C++23-Neuimplementierung**, kein tatsächlicher Firestorm-Source-Fork. Die gewünschte Firestorm-Quellcodebasis ist deshalb ein *noch offener Architektur-/Migrationsauftrag*. Keine Datei, kein Modul und kein Release darf ohne Quellcodeprüfung als „aus Firestorm übernommen“ bezeichnet werden.
- Source-Reuse/Portieren/Forken darf nach technischer Machbarkeit und Lizenzprüfung erfolgen. Firestorm stammt aus der LGPL-Codebasis; Bestandteile, Änderungs-/Quellcodeveröffentlichung, Copyright-Hinweise und Drittkomponentenrechte sind einzuhalten. Bestehende MPL-2.0-Lizenz der eigenen OGL-Dateien wird nicht ohne Prüfung pauschal auf Firestorm-Code übertragen. Für Weiterverwendung nur nachvollziehbar lizenzierte Releasequellen verwenden. Keine proprietären Assets/Skin-/Trademark-/Havok-Bestandteile ungeprüft übernehmen.
- **Die eigene NexVerse-UI bleibt verbindlich.** Firestorm-Funktionen müssen dort mindestens gleichwertig erreichbar sein; Firestorm-Skin/XUI muss nicht kopiert werden. Vorschaltlogik, bestehendes Login/Welcome, Loading-Screen, Widescreen, Native Window Controls und Phasentrennung bleiben bestehen.

## 3. Verbindliche Inventur – alle Firestorm-Funktionsgruppen

Jeder Bereich wird in der späteren Matrix auf *konkrete* Firestorm-Menüs, Command/Codepfade, Wire-CAPS/LLUDP/Serverfunktionen, OGL-Implementierung, Tests und dokumentierten Status heruntergebrochen. Diese Übersicht ist ein **Sollkatalog**, keine Implementierungsbehauptung.

| Bereich | Vollständigkeitsanforderung |
|---|---|
| Grid, Login, Session | Grids/Discovery, MFA soweit unterstützt, Namen, Startpositionen, Favoriten, sichere Sitzungen, Reconnect, Logout, Multi-Simulator/Hypergrid |
| Welt, Bewegung, Kamera | vollständige Bewegung/Flug/Landen/Sitzen, Tastatur/Mouselook, Steuerungsoptionen, Kamera/Orbit/Focus, Autopilot, TP-Angebote/Requests, Landmarken, Regionswechsel |
| Avatar & Appearance | Skeleton, Shape/Skin/Hair/Eyes, BOM, Wearables/Attachments, Animesh, AO, Gesten/Animationen, Kleidung/Outfits, COF, Appearance-Rebake, Mesh-Bodies, Avatar-LOD |
| Welt-/Objektrendering | Terrain/Water/Sky/EEP, Prim/Mesh/Sculpt, Texturen, Alpha/Mask/PBR/Materialien, Partikel, Animationen, Licht/Schatten, Media-on-Prim, Caches |
| Inventar & Upload | Items/Folders, Copy/Move/Link, Permissions/Rez/Take, Upload von unterstützten Mesh-/Texture-/Sound-/Animation-Assets, Thumbnails/Outfit-Galerie, Trash/Restore |
| Kommunikation | Local/Nearby Chat, Gruppenchat, IM, Friends, Profile, Calling Cards, Notifications, Mute/Block, Voice/Music/Media soweit server- und betrieblich verfügbar |
| Sozialfunktionen | Profile, Gruppen/Rollen/Einladungen, Kontaktstatus, Radar/People/Nearby, Offline-Messages, Picks/Classifieds, Events und Search |
| Bau-/Objektbearbeitung | Prims/Meshes erstellen, Link/Unlink, Position/Rotation/Skalierung, Objekt-/Material-/Texture-Faces, Script-/Contents/Permissions, Editieren/Kopieren, Import/Export soweit rechtmäßig |
| Land, Estate & Navigation | Parzellen, Kauf/Verkauf, Estate-Rechte, Ban/Access, Region/Estate-Tools, Karte/Minimap, Teleports, Waypoints, Pathfinding soweit serverseitig |
| Wirtschaft & Commerce | Geldsaldo/Zahlungen/Transaktionen, Marketplace/Verkauf/Vermietung und Optionsmenüs, wenn Grid-Backend und Berechtigungen diese bereitstellen |
| Fotografie & Medien | Snapshot, Phototools, WindLight/EEP, Tageszeit, Kameraoptionen, Videostream, Media- und Soundeinstellungen, lokale Exporte |
| Power-User-Features | RLV/RLVa (nur erlaubtes, bewusst aktivierbares Verhalten), Debug-Settings, Konsole/Statistik, Kamera- und Editierhilfen, UI-/Chat-/Avatarpräferenzen |
| Sicherheit, Diagnostik & UI | Datenschutz, Crash-/Asset-/Netzwerkdiagnosen, Packetloss/Ping/FPS, Logs ohne Geheimnisse, Lokalisierung, Accessibility, Shortcuts, Settings |
| Grid-spezifische Erweiterungen | Serveradvertisierte NexExperiences/Consent, InventoryThumbnailUpload, Such-/Gruppen-/Landdienste, NV$ und optionale OpenGenesisLINK-Funktionen |

**Nicht vergessen:** Die echte Zuordnung pro Menü/Funktion ist erst nach kontrolliertem Firestorm-Sourcescan und auf echter OpenGenesisLINK-Region verifiziert. Der Funktionskatalog ist bewusst umfangreich, um jede vorhandene Firestorm-Funktion zu erfassen.

## 4. Verbindliches Verhalten bei Serverunterschieden

- Quelle der Wahrheit sind echtes Feature-Discovery, Sim-Feature-LLSD, CAP-Seed und Live-/Servercode. **Nie** allein aufgrund eines ähnlich benannten Serversystems eine funktionierende Viewer-Funktion behaupten.
- Firestorm-Funktion und OpenGenesisLINK-Server-Cap vorhanden: originalgetreues Nutzerverhalten, Berechtigungs- und Fehlermeldungsabnahme.
- Funktion vorhanden, aber aktuelle Grid-Version bietet keine Cap/Rechte: UI zeigt einen erklärbaren „vom Grid nicht unterstützt“/deaktiviert-Status oder sicheres Fallback; kein unsichtbares Entfernen aus der Funktionsinventur und kein Dummy-CAP/erfundene URL.
- OpenGenesisLINK-spezifische zusätzliche Capability: isolierter Adapter/Plugin oder OpenGenesisLINK-spezifisches Modul, keine schleichende Änderung von SL/OpenSim-Paketsemantik.
- **Nicht unterstützte** oder verschobene Serverfunktionen sind als `server_unsupported` beziehungsweise `deferred` zu dokumentieren. Beispielsweise dokumentiert die 0.9.3.10-Server-Roadmap die Verschiebung von OGLVoice und vollständigem Firestorm-Havok-NavMesh; NexExperiences verlangt noch echte Firestorm-Live-Abnahmen.
- **Inventar-Sonderfall:** `InventoryThumbnailUpload` ist laut Server 0.9.3.10 Dev serverseitig vorhanden, hat jedoch einen im Referenz-Firestorm getesteten Caching-Grenzfall beim Ersetzen von Outfit-Ordner-Vorschaubildern. OGL soll diesen Fall separat regressionstesten; nicht behaupten, Firestorm habe ihn bereits vollständig behoben.

## 5. Verbindliche Nachweismatrix und Status

Für **jede** Firestorm-Funktion eine Zeile mit:
`Kategorie | Firestorm-Release-Branch/Commit/Quelldatei | Server-main-SHA/Cap oder Packet/LSL/Featureflag | OGL-Modul | Status | automatischer Test | NexVerse-Live-Abnahme | Abweichung`.

Zulässige Statuswerte: `uninventoried`, `planned`, `in_progress`, `implemented_untested`, `tested_synthetic`, `live_verified`, `server_unsupported`, `deferred_with_reason`. **Keine** automatische Einstufung als `live_verified` nach erfolgreichen Unit-/GL-Pixeltests.

Pro Release:
1. Vor Arbeitsbeginn Server-`main`-HEAD prüfen und im Change-Log eine neue feste Vergleichs-SHA festhalten; Firestorm-Release-Quelle auf Aktualität prüfen.
2. Bestehende Featuregruppen aus Inventur und Protokoll-/API-Differenz gegen Server-Code priorisieren. Firestorm-Parität vor beliebigen Zusatzfunktionen.
3. Implementieren/portieren, Build/C++-Tests und echte Simulator-/Firestorm-Vergleichstests voneinander getrennt dokumentieren.
4. Paritätsmatrix/Projektplan/Chatübergabe aktualisieren; Regressionen und Lizenzhinweise prüfen; erst dann Release-Commit.
5. Bei Unklarheit oder fehlender Laufzeitverbindung `unverified` beibehalten; nicht behaupten, der OGL-Viewer sei vollständiger Firestorm-Ersatz.

## 6. Primärreferenzen

- Firestorm-Quellcode: https://github.com/FirestormViewer/phoenix-firestorm
- OpenGenesisLINK Server: https://github.com/opengenesislink/opengenesislinkserver
- Server-Roadmap: https://github.com/opengenesislink/opengenesislinkserver/blob/main/doc/NexVerse/ROADMAP.md
- Server Pathfinding/Experiences: https://github.com/opengenesislink/opengenesislinkserver/blob/main/doc/NexVerse/PATHFINDING_EXPERIENCES_FULL_PARITY_09310.md
- Server Inventory/Outfit-Caching: https://github.com/opengenesislink/opengenesislinkserver/blob/main/doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md


## Implementierungsstand Alpha.80 – erster Feature-Adapter

**Block 1 – geprüfte Firestorm-Seed-CAP-Abdeckung:** `default_seed_capabilities()` fordert beim echten Login-Seed nun zusätzlich `FetchInventoryDescendents2`, `InventoryThumbnailUpload`, `FindExperienceByName`, `GetExperiences`, `AgentExperiences`, `ExperiencePreferences`, `GroupExperiences`, `GetAdminExperiences`, `GetCreatorExperiences`, `IsExperienceAdmin` und `UpdateExperience` an. Vorhandene `FetchInventory2` und `GetExperienceInfo` bleiben. Die Antwort bestimmt die tatsächliche Verfügbarkeit; nicht angebotene CAPS werden nicht simuliert. Der C++-Regressionstest kontrolliert einmalige Anforderung aller relevanten Namen.

**Block 2 – Server-capability-gesteuerte Firestorm-Bereitschaft:** Neue typisierte `FirestormCapsAvailability` mit acht klar definierten CAP-Kategorien (Inventaritem-Fetch, Inventarordner-Fetch, ThumbnailUpload, Owned/Allowed/Permissions/Admin/Update Experiences) berechnet rein aus der geprüften Seed-Antwort die angebotenen Funktionen, nie aus dem Vorhandensein von Beispiel-URLs oder dem Serverproduktnamen. Im produktiven Login-CAPS-Ladebildschirm erscheinen `Startregion: Outfit-Vorschau CAP/- | Experiences CAP/-`, optional die Anzahl eigener/erlaubter Experiences. `CAP` bedeutet *angebotener Endpunkt*, kein erfolgreicher Upload oder Firestorm-Funktionsparität. Der bisherige NexVerse-UI-Stil und die Simulation-Ready-Steuerung bleiben erhalten.

**Block 3 – Read-only Firestorm Experience-Listen, ohne EventQueue zu bremsen:** `parse_agent_experiences` akzeptiert strikt LLSD-Map mit begrenztem `experience_ids`-Array und unterscheidet `purchase`-Feld fehlend von `purchase: 0` (so verlangt es der aktuelle OpenGenesisLINK-Serververtrag für Firestorm). `parse_resident_experiences` validiert `experiences` und `blocked` getrennt. Die OGL-CAPS-Worker-Routine führt die beiden GETs nur bei tatsächlich angebotenen `AgentExperiences`/`GetExperiences` in einer separaten, beim Beenden gejointen Leseaufgabe aus, damit die EventQueue nicht durch diese optionalen Abfragen blockiert wird. Keine Experience-Purchase-/Allow-/Block-/Forget-Verwaltungsaktion erfolgt automatisch; Ergebnisse der ursprünglichen Login-Startregion werden nicht als aktuelle spätere Teleportregion behauptet. Es werden nur Anzahl und CAP-Verfügbarkeit angezeigt; keine Avatar-/Experience-UUID oder Secret-CAP-URL in neuen Diagnoseausgaben.

**Block 4 – Firestorm/OpenGenesisLINK Outfit-/Inventar-Thumbnail-Wire-Vertrag:** `build_thumbnail_begin_request(target,id)` erzeugt nur bei gültiger, nichtleerer Item-/Ordner-UUID das richtige LLSD-XML mit genau einem `item_id` oder `category_id`. `parse_thumbnail_stage` akzeptiert nur autorisierte Antwortformen `{state:upload,uploader:<HTTP(S)-URL>}` bzw. `{state:complete,new_asset:<UUID>}`. Ungültige UUIDs, Dateischemen und unbekannte Zustände werden verworfen. Dies entspricht dem aktuellen OpenGenesisLINK-Servermodul `OpenSim/Region/ClientStack/Linden/Caps/InventoryThumbnailUploadModule.cs`. Die Binär-JPEG2000-Übertragung, Besitz-/Rechteprüfung und endgültige OGL-Inventar-UI sind **nicht implementiert**; der Server selbst bindet One-Time-Uploader an aktive Owner-CAPS. Die serverseitig dokumentierte Firestorm-Outfit-Thumbnail-Cache-Limitation bleibt separat bestehen. `tests/firestorm_compat_tests.cpp` deckt Capability-Abgleich, Listenschema, Größenlimits, LLSD-Serialisierung und Upload-Handshake ab.

**Externe Codequellen, 11.10.2026:** Aktueller OpenGenesisLINK-Server `main` = `6dc73f2adfded7fe5ee94c992daaaba5be2b66c8`, Entwicklungszug 0.9.3.10 Dev / Stable 0.9.3.8, Commit vom 11.10.2026: Experience Manage API Keys können jetzt mit separatem `experiences:viewer:manage`-Scope ausgegeben werden. Dies ist **keine** Viewer-seitige Admin-Key-Nutzung: OGL darf niemals interne World-API-Secrets erhalten. Firestorm offizielle Referenzrelease-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` (Nightly `master` nicht als Release). Relevante Firestorm-Quelle `indra/newview/llfloaterexperiences.cpp` und OGLServer-Dokument `doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md` sowie `FIRESTORM_INVENTORY_THUMBNAIL_09310.md`. Server-/Client-SHAs vor jedem künftigen Featureblock neu vergleichen.

**Nachweis- und Einschränkungsstatus:** Erste vier Firestorm-Vertragsbausteine sind implementiert und lokal synthetisch getestet, aber kein vollständiger Firestorm-Fork und keine vollständige Experience-/Outfit-Benutzerfunktion. Die ursprüngliche reale In-World-Logserie enthält keine frischen Snapshotnachweise, Avatar/WASD/Mesh/EventQueue/Teleport bleiben noch zu integrieren. Weiterhin keine Behauptung einer erfolgreichen NexVerse-Live-Experience-/Inventar-Abnahme. Lizenzentscheidung für tatsächliche Firestorm-LGPL-Source-Übernahme bleibt offen; dieser Commit übernimmt keinen fremden Firestorm-Quellcode.

**Nächste 3–4 Blöcke Alpha.81:** Read-only Experience-Funktionen in die echte OGL-InWorld-Benutzeroberfläche mit regionengebundener Session-/CAP-Lease übernehmen; LLSD Inventar-/Outfit-Thumbnail-Metadaten inklusive Versionswechsel und Rechte abstrahieren; volle Firestorm-Menü-/Funktionsinventur und aktuelle Server-CAPS-Diffs weiter ausfüllen; anschließende kontrollierte Entwicklung von Upload/Experience-Consent mit echter Grid-Permission-/Owner-Abnahme. Firestorm-Parität hat Vorrang vor neuen Renderer-only-Erweiterungen.

Die Zielvorgabe „Firestorm als Basis“ bleibt auch nach diesem Protokoll-/Seed-Block offen. Nur ein subset serveradvertisierter Funktionen ist tatsächlich als OGL-Protokoll implementiert; Lizenz- und UI-Quellportierung wird gesondert geprüft.


## Implementierungsfortschritt Alpha.81

Vier zusammengehörende Firestorm-Paritätsblöcke:

1. **Bedienbare Experience-Ansicht:** Nach authentifizierter und bereiter Simulation öffnet F8 einen Floater im NexVerse-Design. Eigene/Erlaubte/Blockierte Experiences lassen sich über Maus oder die Tasten 1–3 auswählen, ESC/F8 schließt. Die Liste enthält echte Simulator-UUID-Präfixe mit ausdrücklich noch fehlenden Anzeigenamen statt erfundener Namen. Fehlende CAPS, noch nicht geladene Daten und leere Listen werden getrennt angezeigt. Solange die Ansicht geöffnet ist, löst WASD keine Avatarbewegung aus. Schreiben, Experience erwerben, Allow/Block/Forget bleiben gesperrt. Der Viewport vor erfolgreichem Simulator-Eintritt bleibt frei von der Experience-UI.

2. **Region-scoped Experience-Leseergebnisse:** FirestormRegionLease bindet Regionhandle, WorldModel-Generation und Seed-Identität an einen threadsicheren Epochentoken. Bei Regionswechsel werden alte CAP-Advert-Flags, Counts und Experience-UUID-Listen gelöscht, der Floater geschlossen. In-flight AgentExperiences/GetExperiences-Antworten werden nach dem Wechsel auch bei gültiger HTTP-200-Antwort verworfen; Lease-Check erneut unter Caps-Mutex. Die Endpoints und UUIDs dürfen nicht in Statuslogs erscheinen. Der EventQueue-Hauptworker bleibt unabhängig. Neuer deterministischer asynchroner CTest belegt die späte GET-Ablehnung. Grenze: Neues Experience-CAPS-Seed-Discovery nach Teleport ist noch nicht umgesetzt, daher wird absichtlich keine alte Liste weiter angezeigt.

3. **Inventar-/Outfit-Metadatenvertrag:** Der MPL-2.0-Adapter parse_inventory_thumbnail_metadata liest die OpenGenesisLINK-LLSD thumbnail.asset_id aus FetchInventory2/FetchInventoryDescendents2, unterscheidet fehlendes Feld, ausdrücklich leere (Zero-UUID) Vorschau und gültige Thumbnail-UUID, lehnt defekte LLSD-Strukturen ab. InventoryFolderThumbnailCache speichert zu Folder-UUID die Version und Asset-ID, lehnt ältere Folderversionen ab und akzeptiert geänderte Thumbnail-IDs auch bei gleicher Version, um die dokumentierte Firestorm-7.2.4-Outfit-Gallery-Caching-Einschränkung nicht blind zu übernehmen. Neuer CTest belegt leere, alte, ersetzte und ungültige Metadaten. Grenze: noch kein echter Inventar-GET/Outfit-Gallery-UI, kein JPEG2000-Upload, keine reale Server-Readback-Abnahme.

4. **Konkrete Funktionsparität und Verträge:** Offizielle Firestorm-Releasequelle Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d; belegt über indra/newview/llfloaterexperiences.cpp (Tabs, GetExperiences/AgentExperiences, Acquire-Button). Aktueller OpenGenesisLINK Server main @ 6dc73f2adfded7fe5ee94c992daaaba5be2b66c8; Belege doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md, doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md und serverseitige Thumbnail/Experience-Implementierung. Versionsgrenzen: OpenGenesisLINK 0.9.3.10 Dev und Firestorm 7.2.4.80712. Die Firestorm-Funktions-/Verhaltensbaseline bleibt verbindlich, der bestehende OGL-C++23-Viewer ist noch kein Firestorm-Code-Fork.

**Nachweise und Grenze:** 73/73 lokale CTests nach Integration, einschließlich zweier neuer Firestorm-Regressionen und realen GPU-Tests. Ein bestandener synthetischer Lease-/Metadatentest ist KEIN NexVerse-In-World-Test. Eine tatsächliche authentifizierte Regionsabnahme für diesen Stand fehlt weiterhin. Der F8-Floater wird nur im Simulationszustand gezeichnet; Login-Welcome-Smoke allein prüft keine Interaktion nach einem realen Simulatorbeitritt.

**Alpha.82-Priorität:** neue Regions-Experience-CAPS nach Teleport entdecken, Experience-Namen/Profile mit GetExperienceInfo lesen, servergebundene FetchInventory2/FetchInventoryDescendents2-Read-only-Outfit-Gallery aufbauen sowie Write/Consent/Upload nach separatem Owner-/Permissions-/Grid-Vertrag kontrolliert vorbereiten. Firestorm-Parität zuerst, Zusatzrenderer nachrangig.


## Stand Alpha.82: weitere Firestorm-basierte Funktionen

**Block A – dynamische Firestorm-CAPS nach Region-/Teleportwechsel:** Neue Protokollroutine fetch_firestorm_region entdeckt AgentExperiences, GetExperiences, GetExperienceInfo, FetchInventory2/FetchInventoryDescendents2 und InventoryThumbnailUpload anhand des echten aktuellen Simulator-Seeds. Die Routine holt autorisierte Experience-Listen ausschließlich lesend; CAP-Verfügbarkeit bedeutet keinen bestätigten Betrieb und keine Schreibberechtigung. In der App laufen Region-Read-Worker unabhängig vom bestehenden Login-CAPS/EventQueue-Longpoll, mit maximal vier ausstehenden Anfragen und nichtblockierender Future-Abholung; bei erschöpften Slots wird der aktuellste Regionsrequest nach Freigabe erneut begonnen. Ergebnisse werden nur für den noch gültigen FirestormRegionLease-Token unter Caps-Mutex übernommen. Alte UUIDs/Capabilities werden beim Regionswechsel zuerst verworfen. Wichtig: Die allgemeine Simulatorverbindung/EventQueue-Reconnect wird dadurch nicht automatisch repariert; die Verfügbarkeit einer neuen Region setzt eine vom WorldModel gelieferte gültige Seed-CAP voraus.

**Block B – echte Experience-Anzeigenamen über Firestorm GetExperienceInfo:** Authentifizierte read-only Profile werden optional vom pro Region angebotenen GetExperienceInfo-Endpoint über die Firestorm-Query /id/?page_size=12&public_id=UUID abgefragt. Nur tatsächliche IDs aus eigenen/erlaubten/blockierten Listen, maximal zwölf, werden angefragt. Der LLSD-Parser verlangt experience_keys[] mit public_id und name, validiert UUID, begrenzt Namenslänge auf 128 Zeichen und lehnt Steuerzeichen ab. F8 zeigt bei Erfolg den vom Simulator gelieferten Experience-Namen; bei fehlender/nicht unterstützter CAP bleibt die eindeutige Kennzeichnung ID XXXXXXXX (Name nicht geladen). Regionsepoche schützt auch Profilresultate, keine CAP-URLs, Experience-UUIDs oder Admin-API-Keys in neuen strukturierten Logs.

**Block C – lesende Outfit-Galerie aus echten OpenSim-Loginordnern:** F9 öffnet nach erfolgreichem Eintritt in eine Simulation ein Fenster im NexVerse-Design. Firestorms echte LLFolderType-Codes FT_MY_OUTFITS=48 und FT_OUTFIT=47 identifizieren aus der vom OpenSim-Login gelieferten inventory-skeleton die Outfit-Ordner. Die ersten zehn realen Ordnernamen werden aus maximal 256 akzeptierten UUID-validierten Einträgen angezeigt; ungültige und doppelte IDs werden ignoriert, Steuercodes in Titeln bereinigt. F8- und F9-Ansichten schließen einander aus, ESC/F9 schließt die Outfit-Ansicht, während offener Ansicht werden WASD-/Movement-Tasten nicht an den Agenten übergeben. Keine erfundene Vorschau oder Behauptung eines funktionierenden Wear/Rez. **Grenze:** Die Anzeige stammt derzeit aus dem Login-Skeleton; der aktive FetchInventory2/FetchInventoryDescendents2-Readback, Asset-Mesh-/JPEG2000-Vorschauen, COF-Verknüpfung, Outfits anziehen und Rechteprüfung bleiben offen.

**Block D – überprüfbarer Firestorm-/OpenGenesisLINK-Wire-Vertrag:** Der vollständige serverseitige Snapshot vom 11.10.2026 nutzt OpenGenesisLINK main fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b (neuer als alpha.81 6dc73f2). Offizielle Firestorm-Release-Referenz Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d. Implementierungsquellen: Firestorm indra/llmessage/llexperiencecache.cpp und .h (public_id, experience_keys, /id/), indra/llinventory/llfoldertype.h (46/47/48); OGL-Server NexVerse/RegionModules/Experiences/NexExperienceModule.cs mit GetExperienceInfo via variable path und getrennten Permission-/Manage-Keys. Der OGL-Viewer besitzt und benötigt keine zentralen World-API-Keys. Neuer nativer HTTP-Fixture-Test simuliert Seed->Listen->Profilaufrufe und verspäteten Region-Seed nach dem Wechsel, neuer Outfit-Gallery-Test validiert Firestorm-Ordner-Typen, Duplikate, Sanitizing und echte Login-Ordnerdaten. Die Tests beweisen kontrollierten OGL-Vertrag, KEINE authentifizierte Live-Regionsabnahme oder vollständigen Firestorm-Quellfork.

**Abnahmegrenzen:** Die neue F8-/F9-Bedienung ist nur während WorldModel.ready sichtbar; GUI-Welcome-Smoketest ohne echte NexVerse-Simulation belegt daher keinen Live-UI-Nachweis. Noch keine echte InWorldAssetSnapshot-Serie im bisherigen Log. EventQueue, Avatar, tatsächliche WASD-Bewegung, Upload, Wearables/COF, Marketplace/Groups/Voice und sonstige Firestorm-Funktionen bleiben gemäß verbindlicher Paritätsmatrix als noch nicht nachgewiesen/implementiert gekennzeichnet. Nach Versions-Neubau volle Tests und NVIDIA-OpenGL-Smoke durchführen.

**Nächste vier geplante Blöcke Alpha.83:** echten FetchInventory2-/FetchInventoryDescendents2-Readback der Outfitfolder mit serveradvertisierter CAPS-POST-Struktur, Thumbnail.asset_id und diskretem Cache-Vertrag implementieren; Outfit-Vorschauen aus Assetpipeline als Bilder anzeigen, keine leeren Platzhalter; Experience-Auswahl mit Profildetails (Name, Beschreibung, Berechtigungen) vervollständigen; EventQueue/Teleport-Region-CAPS- und Firestorm-Features mit echter autorisierter Simulator-Sitzung regressionsprüfen. Funktionen nur bei tatsächlichen Serverrechten aktivieren.


## Implementierungsstand Alpha.83: Inventory-Fetch und Experience-Profildetails

**Block 1 – echter Firestorm-/OpenGenesisLINK-Inventarabruf per CAPS-POST:** `firestorm_inventory_fetch.hpp/.cpp` erstellt aus dem authentifizierten OpenSim-Login-Agenten und den ermittelten Outfit-Foldern ein LLSD-`FetchInventoryDescendents2`-POST mit `folders[]`, `folder_id`, `owner_id`, `sort_order`, `fetch_folders`, `fetch_items`. Bis 16 Outfits und ihre gültigen übergeordneten Folder werden doppelfrei abgefragt, weil nur die *Parent-Categories* die Thumbnail-Metadaten der Outfit-Ordner liefern. Antworten werden auf erlaubte Folder-/Parent-UUIDs, nichtnegative Versionen, max. 64 Folder-/1024-Category-Entries und den aktuellen FirestormRegionLease-Epochentoken begrenzt. Gelesen werden `categories[].thumbnail.asset_id` und versionsgebundene Foldermetadaten. Keine künstlichen Inventarordner, fremden Owner oder URL-Leaks. Der Zugriff bleibt ausschließlich lesend und nur über eine vom aktuellen Simulator beworbene Capability.

**Block 2 – echte Outfit-Vorschaubilder durch vorhandene GetTexture-/JPEG2000-/PNG-Pipeline:** Nach erfolgreichem, epochentreuem Inventar-Readback stellt die Produktions-App die vom Server gelieferten `thumbnail.asset_id` IDs und den tatsächlich beworbenen `GetTexture`-CAP bereit. Bis zu acht unterschiedliche, tatsächlich vorhandene Bilder werden in einer auf drei gleichzeitige Ladevorgänge begrenzten asynchronen Pipeline über den vorhandenen `assets::TextureAssetLoader::fetch_rgba` (PNG mit JPEG2000-Fallback) dekodiert und erst auf dem OpenGL-UI-Thread über `UiRenderer::create_rgba_texture` in sichtbare F9-Vorschau-Bilder umgewandelt. Auflösung für UI-Upload höchstens 1024×1024; Asset-ID und Regionen-Epoche werden unmittelbar vor GPU-Upload und beim Zeichnen kontrolliert. Beim Regionswechsel werden neue/alte Tasks, ID-Zuordnungen und GL-Textures getrennt behandelt; alte GL-Handles werden gelöscht, late replies verworfen. Fehler oder vom Grid nicht angebotene Caps lassen den ehrlichen Text `Vorschau -` stehen. Kein Bitmap wird erfunden und kein Firestorm-Thumbnail-Upload vorgetäuscht.

**Block 3 – Firestorm Experience-Profildetails und Benutzerselektion:** Die bestehende F8-Ansicht nutzt nun `ExperienceProfile` aus den vom Server über `GetExperienceInfo /id/?page_size=12&public_id=...` tatsächlich gelieferten LLSD-Objekten. Neben dem Namen werden Beschreibung (maximal 160 Zeichen zur Darstellung aus max. 4096), Firestorm-Maturity-Codes 13/21/42 und ein Aktivstatus aus Property-Bit 6 verarbeitet; alle Texte werden auf Länge und Steuerzeichen geprüft. Ein einzelner Experience-Eintrag ist im vorhandenen Viewer-Design per Maus oder Pfeiltasten markierbar; 1/2/3 wechseln zwischen eigenen/erlaubten/blockierten Listen. Zurückhaltender Fallback, wenn kein aktuelles Profil verfügbar ist. Keine automatische Zustimmung, kein Kauf, keine Write- oder Admin-API-Key-Nutzung.

**Block 4 – geprüfter Server-/Firestorm-Vertrag und Regressionen:** Offizieller Firestorm-Release-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` mit `indra/llinventory/llfoldertype.h` (Outfits 47 / My Outfits 48) und `indra/llmessage/llexperiencecache.cpp` (GetExperienceInfo). Offizieller OpenGenesisLINK-Server `main` @ `fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b`, insbesondere `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs`, `OpenSim/Region/ClientStack/Linden/Caps/FetchInventory2Module.cs` und `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Neuer CTest `firestorm_inventory_fetch_tests.cpp` simuliert Owner-/Folder-POST, echte Parent-Category-Thumbnail-LLSD, gesperrte Altregion-Requests und ungültige CAP-URLs. Erweiterter `firestorm_region_read_tests.cpp` testet Region-Seed -> Read-only Experience-Listen -> Profile mit Beschreibung/Maturity -> Folder-/Thumbnail-Response und Texture-CAP, ohne echte Avatar-Authentifizierung zu behaupten. Galerie-Skeleton-Test verwirft auch ungültige Parent-UUIDs. Debug C++23 und insgesamt 76 CTests nach finalem Versions-Neubau vollständig prüfen.

**Strikte Status-/Abnahmegrenze:** `tested_synthetic` für die protokollierten Antworten, Bilddekodierung und GL-Uploadpfade; **nicht** `live_verified`, weil aktuell keine authentifizierte NexVerse-Simulator-/Avatar-/Outfit-Sitzung vorliegt. Kein vollständiger Firestorm-Code-Fork, nur testbare eigene Adapter entsprechend Firestorm-Featurebasis. Die bestehende allgemeine Teleport-/EventQueue- und Avatar-/WASD-Integration ist weiterhin offen. F8/F9 sind nur nach `simulation_ready` sichtbar; Welcome-GUI-Smoketest belegt die neue UI nicht. Vollständiges COF-/Wearables-/Outfitwechsel-Verhalten, Folder-Events und Berechtigungen fehlen. Die verbesserte Cache-Metadatenroutine darf nicht als Nachweis ausgegebener Thumbnail-Bilder auf einem echten Grid missverstanden werden.

**Nächste 3–4 Blöcke Alpha.84:** kontinuierliche Firestorm-Inventarordner-Synchronisierung und `FetchInventory2`-Items samt COF-Lesezustand (nicht nur Login-Skeleton); bedienbare Outfit-Details/Tragen mit Rechte- und Permission-Checks gegen Serverprotokoll; Experience-Berechtigungsdialog mit expliziter Nutzeraktion und aktuellem Simulator-Lease; danach eine echte NexVerse-In-World-Testabnahme inklusive Avatar, Bewegung, Regionswechsel und den noch offenen EventQueue-/Mesh-Problemen. Vollständige Firestorm-Parität bleibt vorrangig.


## Stand Alpha.84 – COF/Outfit und ExperiencePermissions

**Block 1 – Firestorm COF und wirkliche Outfit-Inventargegenstände (lesend):** Der `FetchInventoryDescendents2`-Reader liest jetzt neben den Parent-Thumbnail-`categories` die im Simulatorantwort-`items[]` enthaltenen Inventargegenstände pro Outfit-Folder. UUID, Asset-ID, Name, Asset-/Inventory-Type werden als typisierte `OutfitInventoryItem`-Einträge nur aus gültigen und begrenzten LLSD-Maps übernommen (max. 512 Antwortitems, höchstens 256 verwendete pro Ordner, Namen max. 128 Zeichen mit Entfernung von Steuerzeichen). Der separate Current Outfit Folder (COF, Firestorm Typ 46) wird aus der authentifizierten OpenSim-Login-Skeleton ermittelt und mit dem gleichen Owner-gebundenen CAPS-POST abgefragt, auch wenn keine gespeicherten Outfits vorhanden sind. Der Owner der Serverantwort wird kanonisch als UUID-Bytes gegen den eingeloggten Agenten geprüft; fremde/ältere Antworten dürfen keine Anzeigedaten ersetzen. **Keine Wearables-/COF-Mutation**, kein unberechtigter Asset-Upload.

**Block 2 – versionsgebundene, inkrementelle Inventarsynchronisierung:** Neue `protocol::FirestormInventorySync` hält Folderversion, Items und Thumbnail-Asset-IDs ausschließlich innerhalb der Regionsepoche und verwirft alte Versionen, unvollständige/fehlende Thumbnail-Maps sowie verspätete Regionsantworten. Ein gleichversionierter, aber wirklich geänderter Thumbnail-Asset-ID-Link aktualisiert die Galerie. Solange F9 geöffnet ist, prüft die Anwendung im 30-Sekunden-Abstand mit einem **maximal einen** Hintergrund-Future den bereits beworbenen `FetchInventoryDescendents2`-CAP erneut. Render-/EventQueue-Threads warten nicht auf diese Requests. Die alte GL-Vorschau wird nur bei echter Asset-ID-Änderung freigegeben, danach wieder geladen. Neuer deterministischer `firestorm_inventory_sync_tests.cpp` prüft Version-Ordering, unveränderte/ersetzte Thumbnails, ignorierte Legacy-Omissions und Region-Epoch-Reset. **Keine echte EventQueue-basierte Folder-Notifikation**, Polling während sichtbarer Galerie ist ein sicherer Zwischenstand.

**Block 3 – bedienbare F9-Outfit-Details inklusive COF-Readback:** Im bestehenden NexVerse-Look lässt sich einer der ersten zehn in der Liste sichtbaren Outfit-Folder per Maus oder Auf-/Ab-Taste auswählen. Ein rechtes Detailfeld zeigt dessen tatsächlich vom Server gelesene Gegenstandsnamen (bis sechs), Anzahl und den separaten aktuellen COF-Linkstand, sofern dieser geladen wurde. Fehlt die Owner-CAP oder ein aktueller Abruf, steht ausdrücklich „Inhalt noch nicht geladen“. Existing PNG/JPEG2000-GetTexture-Thumbnail-Darstellung bleibt aktiver Bildpfad; fehlende Bilder bleiben unbestätigt statt gefälscht. **Outfit anziehen, BOM, Wearables, Animationen und synchronisierte Avatar-Appearance sind noch offen**, solange ihre kompletten Firestorm-LLUDP-/CAPS-Rechte und eine echte NexVerse-Simulation fehlen.

**Block 4 – Firestorm ExperiencePermissions mit ausdrücklicher Nutzereinwilligung:** Der neue `ExperiencePermissionConsent`-Vertrag ermöglicht im F8-Fenster bei vorhandener, aktueller `ExperiencePreferences`-CAP genau `A` (= Erlauben vorbereiten) oder `B` (= Blockieren vorbereiten), gefolgt von `Enter` zur **separaten Bestätigung**. `Escape`, Regionswechsel oder veraltete Epochs verwerfen die Vorbereitung. Erst nach Bestätigung startet eine asynchrone, owner-/Region-Lease-gebundene `HTTP PUT`-Operation mit offizieller LLSD-Map `{experience_uuid:{permission:Allow|Block}}`. OpenGenesisLINK liefert die bestätigten `experiences`/`blocked`-Listen; nur wenn die gewählte ID genau im Zielset und nicht im Gegenset steht, zeigt OGL eine erfolgreiche Änderung an. Die generische HTTP-Abstraktion besitzt dafür eine fail-closed PUT-Methode; libcurl-Weiterleitungen für mutierende PUT-Requests sind gesperrt. Die anderen Firestorm-Aktionen `Forget`/DELETE, Purchase, Experience-Editor und Adminrechte bleiben **nicht freigeschaltet**. Neuer Test prüft weder versehentliche Writes noch falsche Erfolgsbestätigungen: ohne explizite Enter-Bestätigung null PUT-Requests, epoch-bound stale reject, negative Serverantwort, erlauben/blockieren.

**Server-/Firestorm-Quellenprüfung (11.10.2026):** Offizielle Firestorm-Releasequelle `FirestormViewer/phoenix-firestorm` Branch `Firestorm_7.2.4` bei `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d`. OpenGenesisLINK Server `main` nun **`efc9b3a73ce04f6f57a1958c29ec0a439ed76d0f`** (neuer als alpha.83 `fcac896e`): Source `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs` mit `items`, `categories.thumbnail.asset_id`, `owner_id`, `version`; `NexVerse/RegionModules/Experiences/NexExperienceModule.cs` mit leasegebundenem `ExperiencePreferences` GET/PUT/DELETE, nur Avatar-/CAP-gebundene Berechtigungen. Server-HEAD entfernt OGL-Pathfinding aus 0.9.3.10-Dev-Scope: Firestorm-Pathfinding bleibt gewünschte Baseline, aber kann auf diesem Server derzeit **nicht als kompatible NavMesh-Funktion** behauptet werden. Neues Feature nur bei Server-Vertrag und später mit tatsächlichen Rights-/Live-Checks.

**Evidenz und Grenzen:** Versionsbuild und sämtliche 78 lokalen CTests inklusive realer GPU-Tests sowie Linux-GUI-Smoke sind vor GitHub-Release erneut durchzuführen. `tested_synthetic` beschreibt bestätigten OGL-Wire-Code und reproduzierbare Tests, **nicht** erfolgreiche Live-Outfit-Wear, Consent, Thumbnail-Erneuerung oder In-World-Avatar-/EventQueue-Funktion. Ungeprüfte bzw. fehlende Firestorm-Bereiche bleiben in der Matrix ausdrücklich offen. UI-Nachweise aus dem Welcome-Ladebildschirm belegen F8/F9 erst in einer echten `simulation_ready`-Sitzung **nicht**.

**Nächste 3–4 Blöcke Alpha.85:** COF-/Outfit-Wear- und Appearance-/Attachment-Protokoll gegen Firestorm-Source und OpenGenesisLINK-Rechte nachbauen; Firestorm-Inventar-Baum samt On-Demand FetchInventory2 und Ordner-Event-Diffs ausbauen, danach optionales Permissions-Forget mit Bestätigung und Lease; fehlende EventQueue-/Avatar-/Mesh-/WASD-Integration gegen authentifizierte NexVerse-Grid-Sitzung live prüfen. Der vollständige Firestorm-Funktionskatalog bleibt verpflichtend.


## Alpha.85 – Firestorm-Quellvertrag zu Inventory, COF und Experience Forget

**Block 1 – Firestorm-ähnlicher Inventarbaum F10 mit echter Ordnernavigation:** Die neue lesende Inventaransicht `F10` zeigt Ordner aus `LoginSession.inventory_skeleton` sowie live nachgeladene `FetchInventoryDescendents2`-Kategorien (Name, Typ, Parent-UUID, Version). Auf-/Ab-Pfeile und Mausklick markieren Ordner, Enter/Rechts öffnen und Links/Backspace gehen zurück; `F10`/ESC schließen. Die Fenster F8/F9/F10 schließen einander aus, Movement-Tasten werden bei geöffnetem Inventar blockiert. Der generische Fetch-Adapter fragt im Hintergrund jeweils genau einen ausgewählten Ordner mittels gültigem, regionengebundenem Simulator-CAP ab; bis zu 256 abgefragte Ordner und 4096 lokal modellierte Folder verhindern ungebremste Rekursion. `owner_id`, `folder_id`, Parent-Zuordnung, LLSD-Typen/Versionsnummer und aktuelle Regionsepoche werden vor Übernahme geprüft. Auch ungültige/fehlende Capabilities führen nicht zu HTTP-Dauerschleifen. Unterordner außerhalb des Login-Skeleton erscheinen erst nach erfolgreichem Simulator-Fetch, nicht aus erfundenen Daten.

**Block 2 – echter `FetchInventory2`-Einzelitem-Abruf für Outfits:** `firestorm_inventory_items.hpp/.cpp` implementiert die OpenGenesisLINK-konforme `POST`-Anfrage `items:[{item_id,owner_id}]` an die vom aktuellen CAPS-Seed beworbene `FetchInventory2`-URL, gebündelt auf maximal 32 IDs aus der aktuell angezeigten F9-Outfitliste. Die Antwort `agent_id` und `items[].permissions.owner_id` müssen exakt zum angemeldeten Avatar gehören; unbekannte IDs und ungültige UUIDs/LLSD-Typen werden herausgefiltert. Lesen und Sanitizing der Itemnamen, Asset-IDs und Typen sind getestet. Die F9-Auswahl startet einen asynchronen On-Demand-Fetch; dessen Ergebnis bleibt mit Folder und Regionsepoche verknüpft. Ein Fetch-Ergebnis eines zuvor gewählten Ordners darf dem neuen nicht als aufgelöste Referenz untergeschoben werden. Die volle Avatar-/Wearables-Appearance-Pipeline ist noch nicht vorhanden.

**Block 3 – COF-/Outfit-Wear-Vorprüfung statt fehlerhaftem Blind-Wear:** Nach Vergleich mit Firestorm `indra/newview/llappearancemgr.cpp` überprüft `firestorm_outfit_wear_preflight` vorhandene Outfit-Items, Current Outfit Folder (COF, LLFolderType=46), referenzierte LLAssetType-24-Links und unverstandene Folder-Links (Typ 25). F9 zeigt einen expliziten Status für noch nicht geladene Inventare/COF, leere Outfit-Folder, nicht aufgelöste Linkziele und noch fehlende Wearable-/Attachment-/Avatar-Appearance-Synchronisation. `can_wear()` bleibt **absichtlich false**: Firestorm-Wear benötigt COF-Linktransaktionen, tatsächliche Wearable-Assets, Attachment-Regeln und Appearance-Abgleich. Der Server- und Firestorm-Quellvergleich erlaubt keinen erfundenen einzelnen `Wear`-CAP; die vollständige Outfit-Aktion bleibt offen. Ein User-Klick darf keinen falschen Erfolg anzeigen.

**Block 4 – explizit bestätigtes Firestorm Experience-Forget:** `F8` unterstützt bei vorhandener aktueller `ExperiencePreferences`-CAP nun zusätzlich `X` zur Vorbereitung von „Vergessen“, gefolgt von **Enter** zur ausdrücklichen Bestätigung (ESC Abbruch). Der Connector nutzt den OpenGenesisLINK-Vertrag `DELETE` auf `<current-capability>?<UUID>`, gegen ungültige IDs und abgelaufene Regionsepochen geschützt. `IHttpTransport::remove()` ist standardmäßig fail-closed, `CurlHttpTransport` unterbindet Redirects für mutierende DELETE-Requests. Erst wenn die Serverantwort die betroffene ID weder in `experiences` noch in `blocked` enthält, meldet der Viewer einen bestätigten Erfolg. Vorbereitete Aktionen werden auch beim Schließen/Wechseln zwischen F8/F9/F10 verworfen. Tests beweisen keine DELETE ohne gesondertes Enter, korrekten HTTP-Queryvertrag, Servernegativantwort- und Stale-Lease-Schutz. Experience-Erwerb, Rollen-/Admin-Bearbeitung und beliebige Write-CAPs bleiben gesperrt.

**Quelldisziplin, 11.10.2026:** Offizielle Firestorm-Basis `FirestormViewer/phoenix-firestorm`, Branch `Firestorm_7.2.4`, HEAD `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` einschließlich `indra/newview/llappearancemgr.cpp`, `indra/llinventory/llfoldertype.h`, Experience-Floater. Aktueller OpenGenesisLINK-Server `main`, HEAD **`885fa72a10fec770d6dc1220f08766437159a084`** – neuer als Alpha.84 (`efc9b3a`); darin u. a. ExperienceInfo-ID-Fix (`2194527`) und ein interaktives Firestorm-LSL-E2E-Testskript (`885fa72`). Für Inventory/COF sind `OpenSim/Framework/InventoryItemBase.cs` und `OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs` maßgeblich (inkl. `item_id`, `permissions.owner_id`, `agent_id`), für Experience-Forget `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Vor weiteren Codeänderungen immer aktuellen Server-HEAD erneut prüfen; eine lokal vorhandene Testdatei beim Server beweist KEINE vom Viewer vollzogene Live-Abnahme.

**Tests und Grenzen:** Für Alpha.85 wurden neue CTests `firestorm-inventory-items-tests` und `firestorm-inventory-tree-wear-tests` integriert sowie Forget-/COF-/F10-Owner-/Region-Regressionen erweitert. Insgesamt 80 CTests geplant und im finalen Debug-C++23-Versions-Neubau mit echten GPU-Tests zu prüfen. Ein synthetischer HTTP-Fixture-Test ist `tested_synthetic`, nicht `live_verified`. Der GUI-Welcome-Smoke verifiziert nicht die post-login F8/F9/F10-Fenster; echtes NexVerse-InWorld-Testing, Avatar, Bewegung, EventQueue/Teleport, Meshes und Outfit-Wear bleiben als nicht abgenommen markiert.

**Nächste 3–4 Blöcke Alpha.86:** Echte Firestorm-COF-/Wearable-Asset- und Attachment-Transaktion anhand `llappearancemgr.cpp` und OpenGenesisLINK-Serverrechten entwerfen/implementieren und mit Live-Avatar-Readback absichern; den F10-Baum um gezielten Item-Readback und Folder-EventQueue-Änderungen ergänzen; Experience-Consent-/Avatar-/Permission-Dialoge gegen aktuellen Simulator robuster machen; authentifizierte In-World-Abnahme (Avatar, WASD, Mesh und EventQueue) mit realem Grid-Zugang priorisieren. Volle Firestorm-Funktionsparität bleibt die verbindliche Baseline.


## Alpha.87 – OpenGenesisLINK Inventory Event-Vertrag

EventQueueGetHandlers.cs, OpenGenesisLINK main
ed37b9db720924c1d7105ea499a8c96430e9b7ea, sendet
BulkUpdateInventory als AgentData-AgentID, ItemData-ItemID/FolderID/
OwnerID und ggf. FolderData-FolderID/ParentID. Die synthetische
Null-UUID-FolderData beim Item-Event ist zu ignorieren. OGL akzeptiert
die Nachricht nur als epochgebundenes Readback-Invalidierungssignal;
der serverautorisierte CAPS-GET/POST liefert den eigentlichen Zustand.
FetchInventory2 liest im F10 zusätzlich die parent_id. Der Viewer
führt keine automatische Inventory- oder COF-Mutation aus; ein neuer
Region-Circuit/EventQueue-Wiederaufbau bleibt noch unvollständig.


## Alpha.88 – LLUDP/CAPS Region-Handoff

Die vom aktuellen Server ausgelieferten CrossedRegion/TeleportFinish-
WorldModel-Ziele binden neue simulator_ip/port und seed_capability.
Ein neues LLUDP CircuitConnection-Handshake mit dem bereits
authentifizierten Agent und CircuitCode darf erst erfolgen, wenn
der alte Socket/Worker vollständig beendet ist. Sobald die neue
LLUDP-Session ACTIVE ist, darf EventQueueGet ausschließlich über
das Seed-CAP derselben aktuellen Region entdeckt werden. EventQueue-
ack gilt pro tatsächlichem EventQueueGet-Endpunkt, nach 404 und
neu entdecktem URI wieder ohne alte ack. Stale Epoch-/UDP-Pakete
und Login-Environment-CAPS-Antworten dürfen die neue Welt nicht
überschreiben. Dies ist derzeit nur fixture-validiert, nicht
auf realen NexVerse-Teleports abgenommen. OpenGenesisLINK Server
f9ee61580a1b6aaf1d420b545ffce883b3dade0c.


## Alpha.89 – AgentUpdate-Linden-Wire und Renderer-Beweislage

AgentUpdate High 4, NotTrusted Zerocoded: AgentData = AgentID
LLUUID 16 + SessionID LLUUID 16 + BodyRotation LLQuaternion 12 +
HeadRotation LLQuaternion 12 + State U8 1 + CameraCenter/At/Left/Up
je LLVector3 12 + Far F32 4 + ControlFlags U32 4 + Flags U8 1.
Gesamt 114 Byte Nutzdaten, ControlFlags bei Offset 109.
LLQuaternion ist eine normierte Vierkomponenten-Quaternion,
aber das Simulatorprotokoll überträgt nur x/y/z.
OpenSim-Bewegung bleibt serverautoritativ; ein kontrollierter
InWorld-Live-Test ist für die reale Positionsänderung erforderlich.
Der GL-Self-Avatar-Proxy ist nur eine Ersatzgeometrie; die
Firestorm-Bake-/Appearance-/Rigging-Kette bleibt noch offen.
Quellen: wiki.secondlife.com/wiki/AgentUpdate und
wiki.secondlife.com/wiki/Message_Layout.


## Alpha.90 – Server-origin LLUDP RemoveInventory semantics

RemoveInventoryItem (Low 270) und RemoveInventoryFolder (Low 276) haben je AgentData Single (AgentID UUID16, SessionID UUID16) und eine Variable-Liste mit U8 count, count*LLUUID16. RemoveInventoryObjects (Low 284) hat AgentData Single, dann FolderData Variable und ItemData Variable; jeder Listenzaehler U8. Obergrenze OGL 64 IDs, keine Null-UUIDs, keine Daten hinter dem letzten Block. Jede Nachricht ist nur ein Readback-Invalidierungshinweis fuer die authentifizierte aktuelle WorldModel-Sitzung, keine lokal autorisierte Delete-Bestaetigung. Bei unbekanntem Parent immer vollstaendiger CAPS-Resync. Einspeisung erfolgt region-epoch-geprueft ueber InventoryEventInbox. Ohne Linux-Build bleibt Alpha.90 committed_unbuilt.


## Alpha.91 – SelfAvatar-Motion nicht mit Avatar-Appearance verwechseln

AgentUpdate wird mit dem unveränderten Linden-High-4-Wire-Layout
gesendet. Nur ein Simulator-ImprovedTerseObjectUpdate mit der
gebundenen SelfAvatar-LocalID, passendem Regionhandle und
mindestens 0.25 m Positionsdelta nach WASD zählt als
beobachtete Bewegung. Drei weiterhin stationäre Terse-Samples
und vier Sekunden halten den Fall ohne Bestätigung fest.
Die Messung verwendet keine Pixel-/Avatar-Skeleton-
Eigenschaften und ist kein Ersatz für einen Firestorm-
Wearables-/Bake-/Animation-Readback. Neue lokale Golden-Tests
prüfen auch State, Camera- und FarClip-Offsets der unveränderten
114-Byte-AgentUpdate-Nutzdaten.


## Alpha.92 – Trusted AvatarAppearance Low 158 vollen Tail lesen

Offizielle Firestorm-Message-Order: Sender Single ID LLUUID,
IsTrial BOOL; ObjectData Single TextureEntry Variable2;
VisualParam Variable U8; AppearanceData Variable
AppearanceVersion U8, CofVersion S32, Flags U32;
AppearanceHover Variable HoverHeight LLVector3 (3*F32);
AttachmentBlock Variable ID LLUUID, AttachmentPoint U8.
OGL akzeptiert vollen Tail mit geprüften Blockgrenzen und
den legacy OpenSim-No-Tail-Fall. Fehlt bei vorhandenem Hover-
Zähler der abschließende Attachment-Zähler, ist das Paket
ungültig. Avatar-COF- und Attachment-Wire-Blöcke sind
Readback-Metadaten, keine legitime Bestätigung von Wear,
Bake, Attach oder einem voll geriggten Renderer.
Appearance wird pro authentifizierter SelfAgentID getrennt
gespeichert und bei Regionsepochenwechsel verworfen;
Pose-only-Pakete löschen keine Textur-/COF-Daten mehr.


## Alpha.93 – echte BakedTextures nur aus expliziten TextureEntry-Faces

Firestorm ETextureIndex Baked Head 8, Upper 9, Lower 10,
Eyes 11, Skirt 19, Hair 20, LeftArm 40, LeftLeg 41,
Aux1/2/3 42/43/44. TextureEntry-Default-Face oder
Farb-/UV-Override sind KEIN Nachweis einer Bake.
Der Decoder besitzt nun eigene texture_id_overridden[64]
Indikatoren. NullUUIDs und IMG_USE_BAKED_*
Placeholder-IDs dürfen keine GetTexture-Aufträge
auslösen. Die gültigen Head/Upper/Lower-UUIDs nehmen
an der bestehenden GPU-Asset-Budgetverwaltung teil.
OpenGL-Proxy-Drawcalls mit tatsächlichem Texture-Sampling
werden separat gezählt. Ein UV-korrektes Mesh-Skinning
und Firestorm-Rig/Körper/COF steht weiter aus.
Ein vom Simulator bestätigter AvatarAttachment-
ParentLocalID ist im Renderer Parent-Transform,
noch nicht LLAvatarJoint-AttachmentPoint.


## Alpha.94 – Originalresource Trust Boundary

Nur lokale, benutzerkontrollierte Firestorm-Laufzeitdateien
Linden Binary Mesh 1.0 und avatar_skeleton.xml/avatar_lad.xml
werden benutzt. No automatic asset download, no hidden
redistributed Firestorm blobs, no script execution through
file names. Strenge Binaer- und XML-Datenbudgets,
Endianness, Finiteness, Count und Index checks.
Full 3-Mesh-GPU-Upload required to replace 7-part fallback.
AttachmentPoint nur bei verifizierter Avatar-LocalID und
serverseitiger Objekt-UUID; mScreen-Objekte NICHT im
3D-Weltraum zeichnen. Rig hat nur Bind-Pose, Skinning
bleibt offen.

## Alpha.95 – Real Joint Palette / GPU Weight Trust

Der Linden-Binary-.llm-SkinWeight ist ein F32-Index mit
1-basierter RenderJoint-Palette aus Firestorm LLAvatarJointMesh.
Es duerfen nur Skeleton-Joints aus der validierten BASE-
Ancestor-Palette verwendet werden. Der GPU-Shader mischt
zwei benachbarte Matrizen nach fractional(weight).
Die Palette ist auf 32 Matrizen begrenzt. Bei malformed
Gewicht/Count wird weder GPU-Mesh noch anderer Render-
State korrumpiert. Prozedurale Animation bedeutet KEINE
Bewegungsbestaetigung des OpenSim Servers.

## Alpha.96 – AvatarAnimation High20 authoritative readback

Simulator->Viewer High20 has UUID Sender, AnimationList
(UUID+signed S32), AnimationSourceList (UUID), and
PhysicalAvatarEventList (U8 byte length followed by data).
It has no separate agent/session or region-handle fields.
Current authenticated circuit and connected WorldModel
provide the trust boundary. Each message REPLACES active
animations for exactly that sender, including empty stop.
Late remote animations pending up to 64 entries do NOT
create visible world actors. Firestorm animation-standard
UUIDs select only temporary local poses, NOT the real
animation asset track, priority and blend.

## Alpha.97 – UI und Serverrechte sind eine gemeinsame Schnittstelle

Firestorm-Funktionsentwicklung darf nicht losgelöst von deren
Viewer-Menü/Floater/Tastatur-/Maustasten-Funktion erfolgen.
Bereits implementierte CAPS-/WorldModel-Funktionen benötigen
sichtbare und korrekt verdrahtete GUI-EntryPoints.
Chat/Teleport gelten als ausdrückliche Nutzer-Requests
im authentifizierten Weltkontext, keine automatischen Sends.
Region-Karte zeigt nur real bekannte lokale Positionen.
Backends ohne Rechte/Antwort bleiben sichtbar deaktiviert.
Gemischte UI- und Netzwerk-/Renderer-Tests sind
für zukünftige Blöcke Pflicht.

## Alpha.98 – KeyframeMotion v1.0/0.1 und UI

Originalformat gemaess Firestorm 7.2.4
LLKeyframeMotion.cpp: U16 Version/Sub, S32 Priority,
F32 Duration, CString Emote, F32 LoopIn/LoopOut,
S32 Loop, F32 EaseIn/EaseOut, U32 HandPose und
U32 JointCount. Jointname Cstring, Priority S32,
S32 KeyCount, bei v1.0 U16 quantisierte
Time/QuaternionVec3XYZ und PositionXYZ;
bei v0.1 float Zeiten und EulerXYZ/PoseXYZ.
Der aktuelle Decoder lehnt IK/Constraint-Clips ab.
High20 AnimUUID ist kein heruntergeladener Clip.
Die GUI zeigt UUID/Sequence nur als readback,
manueller Start/Stop deaktiviert, bis Rechte
und tatsächlicher Assetdownload implementiert sind.

## Alpha.99 – geprüfter OpenGenesisLINK SHA pro Entwicklungsaktion

Source-of-truth: opengenesislink/opengenesislinkserver
main, reviewed SHA 011ed5183889ebff72d10a49162b3223830d8ba5.
Vor Entwicklung und Release mit
python3 scripts/check_opengenesislink_server.py --strict
live prüfen. Neue Commits müssen anhand der echten
C#-Quellen und LSL/CAPS-Protokolle bewertet werden.
CrossRegion LSL Chat bleibt serverintern,
die Standard-Wire-Typen Whisper=0, Say=1,
Shout=2 bleiben clientseitig OpenSim-kompatibel;
Server Chat distances sind konfigurierbar.
Thumbnail-Item-GETITEM-Verifikation ist
Serverautorisierung, vorhandener Client-Cache
bleibt region-epoch-/UUID-gebunden; kein
pauschales Cache-Leeren bei jedem Start.
Nur Firestorm-Basis- und korrekte Clientfeatures
in Kern integrieren, RTX/Wetter optional Module.

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

## Alpha.100 – serverkompatible TransferWire statt erfundener GetAsset-CAP

Geprüfter OpenGenesisLINK Server: LLClientView.cs aktuelle main.
TransferRequest Low153: UUID16 + Channel2 + Source2 + Priority
+ Variable2(UUID16 Asset, S32 Type20).
Info Low154 Variable2-Offset32, High17 Packet Variable2-Offset28.
OpenGenesisLINK fehlendes Asset: Info Status -2 Size0
und 20-Byte Params echo. RegionEpoch/TransferID sind
Pflichtgrenzen; Fremd-/Alt-Circuit-Pakete nicht übernehmen.
Keine Animation-Payload im High20 AnimationList.

## Alpha.101 – Touch-Quellvertrag (OpenGenesisLINK)

Firestorm scripts/messages/message_template.msg:
ObjectGrab Low117 AgentData UUID AgentID+SessionID,
ObjectData LocalID U32 GrabOffset LLVector3,
SurfaceInfo Variable Count; ObjectDeGrab Low119
AgentData UUID+SessionID, LocalID U32,
SurfaceInfo Variable Count.
OpenGenesisLINK LLClientView.cs HandleObjectGrab/
HandleObjectDeGrab erlaubt leerer SurfaceInfo-Blocks
und reicht LocalID an OnGrabObject/OnDeGrabObject.
Tatsächlicher Server-Skript-Heartbeat sendet
touch() wiederholt solange Grab besteht, der
Viewer sendet KEINE duplizierenden GrabUpdate
nur für stationaere Maus.
Objektliste nur aus RegionWorldModel und bei
Grab nochmal LocalID/Root/Proximity pruefen,
Regionswechsel invalidiert.
ChatFromViewer Skriptkanal /7 wird S32 Channel7,
kein clientseitiges CrossRegionChat-NexBus.

## Alpha.102 – Firestorm WorldRay und Feature-Gate-Rechte

KameraProjection Top-Left Cursor, Aspect und FOV
entsprechen OpenGL WorldRenderer. Weltklicks
werden nicht durch Viewer-HUD weitergegeben.
BoundingBox-Treffer sind keine Triangle-Face-
Intersections; LLUDP Grab SurfaceInfo bleibt
leer, bis reale UV/FaceNormal vorhanden sind.
Signierte OGL Feature Control zum Archivieren
ist ausschließlich Admin-Serverfunktion:
keine ES256 PrivateKeys, keine NexBus/Robust
Operatorzugänge in normalem Viewer.


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

## Alpha.103 – Serverreferenz und GetMesh Faceinfo

Der bereits implementierte GetMesh CAP liefert
Vertices xyz/nxyz/uv, Triangles und FaceRanges.
CPU Picking hält eine streng begrenzte,
GPU-resident-kohärente Kopie. Ein Ray-OBB
Broadphase-Test wird bei residentem Mesh durch
Möller-Trumbore Dreiecksschnitt ersetzt, mit
baryzentrischer UV-Interpolation und FaceIndex.
Das sichtbare UI ist keine Freigabe,
Face/ST/Position/Normal/Binormal als LLUDP
SurfaceInfo zu senden. Bis alle Felder
komplett validiert sind bleibt SurfaceInfo
im echten ObjectGrab/DeGrab leer.
Outfit-Refresh ist CAP read-only, echte
COF-Mutationen weiter gesperrt.

## Alpha.104

Alpha.104: Wire-Layout aus Firestorm lltoolgrab.cpp und message_template.msg Low117/119. SurfaceInfo als ein 64-Byte Eintrag: UV Vector3, ST Vector3, Face S32, Position/Normal/Binormal Vector3. Server LLClientView HandleObjectGrab/DeGrab liest diese Daten. Nur validierter TriangleHit plus Texturface liefert Daten; Count0 bleibt Fallback. Outfit COF weiter read-only.

## Alpha.105

Firestorm 7.2.4 llviewerinventory.cpp/message_template.msg LinkInventoryItem Low426 (Agent, CallbackID, FolderID, null TransactionID, OldItemID, Type24, InvType, Name/Desc Variable1) und RemoveInventoryItem Low270 (Agent und 1 ItemID). OpenGenesisLINK LLClientView Handler bestaetigt Wire-Struktur. Implementierte Wire-Nachrichten sind nicht automatisch komplette COF-Wear-Mutation; keine UI-Sendeberechtigung bis serverbestätigter Transaktionsfreigabe.

## Alpha.106

Original LinkInventoryItem Low426 aus Firestorm 7.2.4
wird nur fuer EINEN ADD-LINK ohne bestehende
Loeschungen mit zweitem Click innerhalb 20s
in die Circuit-Queue gestellt. COF muss zum
Owner und RegionEpoch passen. Nach
Queue wartet der Viewer maximal 18s auf
owner-scoped InventoryDescendents2 Current
Outfit Folder mit neuerer Serverversion und
passendem Link-Target (asset_id). LLUDP ACK
ist KEINE erfolgreiche Appearance-Bestätigung.
Unerkannter Erfolg wird nicht auto-retry'd.
COF-Pruefen verwendet nun wirklich
InventoryDescendents2 und neue Request-
Seriennummer statt selected-outfit
FetchInventory2. Kein voller Wear- oder
Unwear-API Support. Admin Keys bleiben server-only.

## Alpha.107

Original Firestorm LLUDP LinkInventoryItem Low426 und RemoveInventoryItem Low270: im Core verwendbar nur mit owner-gebundenem COF-Readback, Version/RegionEpoch, explizitem zwei-Klick-Consent und Timeout. Mutationsserien ohne Loeschung 2-4, Link-Remove strikt einer und bei nichtleerem COF. Unbestimmtes Ergebnis erfordert manuellen Server-Refresh vor neuem Write.
