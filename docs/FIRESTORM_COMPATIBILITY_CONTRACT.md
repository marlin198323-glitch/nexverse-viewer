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
