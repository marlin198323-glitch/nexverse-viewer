# Firestorm ↔ OpenGenesisLINK ↔ OGL – Paritätsmatrix

Stand: 11.10.2026. Dies ist die **offene, inkrementell zu verifizierende** Soll-/Ist-Matrix. Keine pauschalen Erfolgsmeldungen!

Primärdokument: [FIRESTORM_COMPATIBILITY_CONTRACT.md](FIRESTORM_COMPATIBILITY_CONTRACT.md).

## Vergleichsquellen (nur historischer Snapshot dieser Aufnahme)

| Quelle | Versions-/Vergleichsstand | Konsequenz |
|---|---|---|
| Firestorm | offizieller Release-Branch `Firestorm_7.2.4` (vor Implementierung SHA und Nachfolgereleases neu prüfen) | gesamter Funktionsumfang / Verhalten / erforderliche UI-Aktionen |
| OpenGenesisLINK Server | `main` @ `cb3ebb6ff05160bf9bdb20e78372a68d7b723512` | CAPS, LLUDP, LLSD, LSL, Berechtigungen und tatsächliche Features |
| Server Stable | `0.9.3.8` | Verfügbarkeit im Stable separat verifizieren |
| Server Dev | `0.9.3.10 Dev` | Dev-Code oder grüne Tests sind **keine** Bestätigung auf laufendem NexVerse-Grid |
| OGL Viewer | `0.2.0-alpha.79`, `develop-current` @ `96e1418d076d1d2ca9cf44e9d4ba7d2f1ddd6c21` | Login-/Renderer-/Cache-Grundlagen vorhanden, kein vollständiger Firestorm-Fork und keine vollständige Simulatorabnahme |

Vor jedem neuen Funktionsblock **Server und Firestorm neu abfragen**, Vergleichs-SHA in Änderungen vermerken; diese Tabelle niemals als automatisch aktuellen HEAD ansehen.

## Feature-Familien – Soll und evidenzbasiertes Ist

| Firestorm-Featuregruppe | OGL-Status der Vollparität | Prüfnachweis / weitere Aktion |
|---|---|---|
| Gridverwaltung, Login und Session | `uninventoried` | pro Firestorm-Funktion CAP/LLUDP-/Login-Pfad inventarisieren, echtes Grid testen |
| Bewegung, Kamera, Sitzen, Fliegen, Teleport, Autopilot | `uninventoried` | aktuelle C++-Movement-Tests ersetzen keine avatarbasierte Live-Abnahme |
| Vollständige Avatar-Appearance, Skin, BOM, Mesh Bodies, AO, Gesten und Animationen | `uninventoried` | bisher fehlender sichtbar vollständiger Avatar und Wearables/COF |
| HUD, Attachments, Outfits und Avatar-Skinnning | `uninventoried` | Outfit-/Appearance-Nachrichten und Skin-/Skeleton-/LOD-Verhalten vergleichen |
| Inventar, Drag/Drop, Permissions, Suche, Trash, Upload und Gallery | `uninventoried` | Server-`InventoryThumbnailUpload` vorhanden im Dev; Cache-Limitation beachten |
| Chat/IM/Voice/Groups/Friends/Profiles/Search | `uninventoried` | pro Modul Server- und Clientimplementierung prüfen; OGLVoice in Server-Dev deferred |
| Regionen, Karten, Minimap, Land, Estate und Hypergrid | `uninventoried` | Rechte, Objekt-/Parzellenstatus und Grid-Kompatibilität nachweisen |
| Build-/Objekt-/Face-/Script-Editing und Import | `uninventoried` | Firestorm-Editoraktionen, Simulatormitteilungen und Rechte inventarisieren |
| Modernes World-Rendering, Texture/Mesh, Media, Licht, EEP/PBR/Alpha | `tested_synthetic` für **ausgewählte Rendererpfade**, **nicht** Vollparität | OpenGL-Pixel-, GPU- und Cachetests; In-World-Assets unverifiziert |
| Snapshot, Film, Phototools, Preferences, Debug-Settings | `uninventoried` | einschließlich früher ausgenommener Power-User-Funktionen |
| RLV/RLVa und vergleichbare erweiterte Viewerwerkzeuge | `uninventoried` | sichere bewusst steuerbare Unterstützung und Rechte-/Protokollprüfung |
| Experience-Zustimmung, Preferences, Allow/Block/Forget | `uninventoried` | Server dokumentiert Dev-Handler; echte Firestorm-/Avatar-/CAPS-Live-Abnahme offen |
| Pathfinding/Navigation/NavMesh | `server_unsupported` für komplette Firestorm-Havok/NavMesh-Parität im geprüften 0.9.3.10-Dev-Scope | Server bietet native Teilfunktionen; keine erfundenen `RetrieveNavMeshSrc`-Payloads |
| OGLVoice als zentrale neue OpenGenesisLINK-Funktion | `deferred_with_reason` laut geprüftem Server-Roadmap-Scope | nicht als vorhandenes Firestorm-Voice-Feature verwechseln |
| Wirtschaft, Geld, Marketplace, Gruppenrechte, sonstige Grid-Dienste | `uninventoried` | erst Gridfähigkeit und Berechtigungen prüfen, danach OGL-Funktion liefern |

**Regel:** `uninventoried` bedeutet fehlender vollständiger Einzelnachweis, nicht automatisch, dass keinerlei OGL-Code existiert. Einzelnachweise und Ausnahmen müssen aufgeschlüsselt werden. `tested_synthetic` beweist nur die genannten Testpfade. `server_unsupported`/ `deferred_with_reason` gilt nur für den dokumentierten, zeitlich gebundenen Serverumfang.

## Pflichtschema für Einzel-Funktionen

| Firestorm-Funktion / Menüpfad | Firestorm Branch + Commit + Datei | OpenGenesisLINK Server Commit + API/CAPS/LLUDP | OGL Datei/Funktion | Status | Tests | Abweichung |
|---|---|---|---|---|---|---|
| *Pro Firestorm-Aktion ergänzen; noch nicht inventarisiert* | – | – | – | `uninventoried` | – | – |

## Nächste vier Arbeitsblöcke (nach dem Prioritätsentscheid)

1. Funktionsinventar des aktuellen offiziellen Firestorm-Release-Branches **inklusive** Power-User-Menüs aus Originalquellen erstellen; Ergebnis in Einzelzeilen der obigen Matrix festhalten.
2. Aktuellen OpenGenesisLINK-`main`-HEAD und Servercode zu LLUDP/CAPS/Login/Inventory/Appearance/Groups/Experience/Voice gegen Firestorm-Verhalten prüfen; Live-Fähigkeit getrennt notieren.
3. Geeignete Firestorm-Source-Module auf rechtlich/technisch mögliche Übernahme, API-Adapter und Projekt-Lizenzfolgen bewerten; nicht ungeprüft Quellcode als OGL/MPL ausgeben.
4. Funktionale Lücken zuerst schließen, neue Renderer-/Pluginfunktionen weiterhin in separaten Blöcken, sofern sie Firestorm-Parität und Serververtrag nicht verdrängen.


## Alpha.80 – konkrete Einzelfunktionen und Nachweise

**Block 1 – geprüfte Firestorm-Seed-CAP-Abdeckung:** `default_seed_capabilities()` fordert beim echten Login-Seed nun zusätzlich `FetchInventoryDescendents2`, `InventoryThumbnailUpload`, `FindExperienceByName`, `GetExperiences`, `AgentExperiences`, `ExperiencePreferences`, `GroupExperiences`, `GetAdminExperiences`, `GetCreatorExperiences`, `IsExperienceAdmin` und `UpdateExperience` an. Vorhandene `FetchInventory2` und `GetExperienceInfo` bleiben. Die Antwort bestimmt die tatsächliche Verfügbarkeit; nicht angebotene CAPS werden nicht simuliert. Der C++-Regressionstest kontrolliert einmalige Anforderung aller relevanten Namen.

**Block 2 – Server-capability-gesteuerte Firestorm-Bereitschaft:** Neue typisierte `FirestormCapsAvailability` mit acht klar definierten CAP-Kategorien (Inventaritem-Fetch, Inventarordner-Fetch, ThumbnailUpload, Owned/Allowed/Permissions/Admin/Update Experiences) berechnet rein aus der geprüften Seed-Antwort die angebotenen Funktionen, nie aus dem Vorhandensein von Beispiel-URLs oder dem Serverproduktnamen. Im produktiven Login-CAPS-Ladebildschirm erscheinen `Startregion: Outfit-Vorschau CAP/- | Experiences CAP/-`, optional die Anzahl eigener/erlaubter Experiences. `CAP` bedeutet *angebotener Endpunkt*, kein erfolgreicher Upload oder Firestorm-Funktionsparität. Der bisherige NexVerse-UI-Stil und die Simulation-Ready-Steuerung bleiben erhalten.

**Block 3 – Read-only Firestorm Experience-Listen, ohne EventQueue zu bremsen:** `parse_agent_experiences` akzeptiert strikt LLSD-Map mit begrenztem `experience_ids`-Array und unterscheidet `purchase`-Feld fehlend von `purchase: 0` (so verlangt es der aktuelle OpenGenesisLINK-Serververtrag für Firestorm). `parse_resident_experiences` validiert `experiences` und `blocked` getrennt. Die OGL-CAPS-Worker-Routine führt die beiden GETs nur bei tatsächlich angebotenen `AgentExperiences`/`GetExperiences` in einer separaten, beim Beenden gejointen Leseaufgabe aus, damit die EventQueue nicht durch diese optionalen Abfragen blockiert wird. Keine Experience-Purchase-/Allow-/Block-/Forget-Verwaltungsaktion erfolgt automatisch; Ergebnisse der ursprünglichen Login-Startregion werden nicht als aktuelle spätere Teleportregion behauptet. Es werden nur Anzahl und CAP-Verfügbarkeit angezeigt; keine Avatar-/Experience-UUID oder Secret-CAP-URL in neuen Diagnoseausgaben.

**Block 4 – Firestorm/OpenGenesisLINK Outfit-/Inventar-Thumbnail-Wire-Vertrag:** `build_thumbnail_begin_request(target,id)` erzeugt nur bei gültiger, nichtleerer Item-/Ordner-UUID das richtige LLSD-XML mit genau einem `item_id` oder `category_id`. `parse_thumbnail_stage` akzeptiert nur autorisierte Antwortformen `{state:upload,uploader:<HTTP(S)-URL>}` bzw. `{state:complete,new_asset:<UUID>}`. Ungültige UUIDs, Dateischemen und unbekannte Zustände werden verworfen. Dies entspricht dem aktuellen OpenGenesisLINK-Servermodul `OpenSim/Region/ClientStack/Linden/Caps/InventoryThumbnailUploadModule.cs`. Die Binär-JPEG2000-Übertragung, Besitz-/Rechteprüfung und endgültige OGL-Inventar-UI sind **nicht implementiert**; der Server selbst bindet One-Time-Uploader an aktive Owner-CAPS. Die serverseitig dokumentierte Firestorm-Outfit-Thumbnail-Cache-Limitation bleibt separat bestehen. `tests/firestorm_compat_tests.cpp` deckt Capability-Abgleich, Listenschema, Größenlimits, LLSD-Serialisierung und Upload-Handshake ab.

**Externe Codequellen, 11.10.2026:** Aktueller OpenGenesisLINK-Server `main` = `6dc73f2adfded7fe5ee94c992daaaba5be2b66c8`, Entwicklungszug 0.9.3.10 Dev / Stable 0.9.3.8, Commit vom 11.10.2026: Experience Manage API Keys können jetzt mit separatem `experiences:viewer:manage`-Scope ausgegeben werden. Dies ist **keine** Viewer-seitige Admin-Key-Nutzung: OGL darf niemals interne World-API-Secrets erhalten. Firestorm offizielle Referenzrelease-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` (Nightly `master` nicht als Release). Relevante Firestorm-Quelle `indra/newview/llfloaterexperiences.cpp` und OGLServer-Dokument `doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md` sowie `FIRESTORM_INVENTORY_THUMBNAIL_09310.md`. Server-/Client-SHAs vor jedem künftigen Featureblock neu vergleichen.

**Nachweis- und Einschränkungsstatus:** Erste vier Firestorm-Vertragsbausteine sind implementiert und lokal synthetisch getestet, aber kein vollständiger Firestorm-Fork und keine vollständige Experience-/Outfit-Benutzerfunktion. Die ursprüngliche reale In-World-Logserie enthält keine frischen Snapshotnachweise, Avatar/WASD/Mesh/EventQueue/Teleport bleiben noch zu integrieren. Weiterhin keine Behauptung einer erfolgreichen NexVerse-Live-Experience-/Inventar-Abnahme. Lizenzentscheidung für tatsächliche Firestorm-LGPL-Source-Übernahme bleibt offen; dieser Commit übernimmt keinen fremden Firestorm-Quellcode.

**Nächste 3–4 Blöcke Alpha.81:** Read-only Experience-Funktionen in die echte OGL-InWorld-Benutzeroberfläche mit regionengebundener Session-/CAP-Lease übernehmen; LLSD Inventar-/Outfit-Thumbnail-Metadaten inklusive Versionswechsel und Rechte abstrahieren; volle Firestorm-Menü-/Funktionsinventur und aktuelle Server-CAPS-Diffs weiter ausfüllen; anschließende kontrollierte Entwicklung von Upload/Experience-Consent mit echter Grid-Permission-/Owner-Abnahme. Firestorm-Parität hat Vorrang vor neuen Renderer-only-Erweiterungen.

| Firestorm-Einzelfunktion | Firestorm Source @ 10bd3c9f | OpenGenesisLINK Server @ 6dc73f2 | OGL Alpha.80 | Status und Grenze |
|---|---|---|---|---|
| Experiences: eigene Listen lesen | `indra/newview/llfloaterexperiences.cpp` / `AgentExperiences` | `NexExperienceModule.cs`, GET `AgentExperiences`, `experience_ids`, optional `purchase` | `firestorm_compat.cpp`, `src/app/application.cpp`, optionaler GET | `tested_synthetic`; Startregion, keine UI-Verwaltung |
| Experiences: Allow-/Block-Listen lesen | derselbe Floater / `GetExperiences` | `NexExperienceModule.cs`, GET Lists | `parse_resident_experiences`, GET nur nach Seedangebot | `tested_synthetic`; Login-Ladebildschirm mit Count, keine Einwilligung/Entsperraktion |
| Outfit-/Inventory-Thumbnail-Up/Down-CAP-Discovery | Inventar/Outfit im Firestorm-Viewer | `InventoryThumbnailUploadModule.cs`, Seed CAP optional | `default_seed_capabilities`, `firestorm_caps_availability` | `tested_synthetic` CAP-Adverts; kein vollständiger Upload |
| Thumbnail-Upload Start-/Finish-Control LLSD | Firestorm Inventory Snapshot Flow | `InventoryThumbnailUploadModule.cs`, `state=upload/complete` | `build_thumbnail_begin_request`, `parse_thumbnail_stage` | `tested_synthetic` Wire-Control, kein JPEG2000-Upload und kein Eigentumsnachweis |
| Firestorm Experience Acquire/Manage | `llfloaterexperiences.cpp`, Acquire-Button | `FIRESTORM_EXPERIENCE_ACQUIRE_09310.md`, ViewerManage-Key serverseitig | `purchase`-Existenz lesbar; keine Schreibfunktion | `deferred_with_reason` bis UI/Owner-/Server-Live-Tests |


## Alpha.81 – weitere quellgebundene Einzelfunktions-Nachweise

| Firestorm Einzelfunktion | Server-/Clientbeleg | Alpha.81 Umsetzung | Status |
|---|---|---|---|
| Experiences: Eigene, Erlaubte, Blockierte | Firestorm llfloaterexperiences.cpp; Server NexExperienceModule.cs | F8-Floater, Maustabs/Tasten 1-3, lesende Arrays | tested_synthetic, keine Live-Regionsabnahme |
| Experience-GET nach Regionswechsel | Firestorm aktueller gAgent-Regionsbezug, Server regiongebundene CAP-Leases | FirestormRegionLease, alte IDs/Flags löschen, verspätete GETs verwerfen | tested_synthetic, neue Discovery noch offen |
| Outfit-Thumbnail-Feld | Server FetchInventory2/FetchInventoryDescendents2 thumbnail.asset_id | parse_inventory_thumbnail_metadata | tested_synthetic, nicht an aktiven Inventory-GET angebunden |
| Outfit-Thumbnail ersetzen | Serverpersistenz, Firestorm 7.2.4 bekannte INV01-Cache-Grenze | InventoryFolderThumbnailCache Versions-/UUID-Revalidierung | tested_synthetic, komplette Gallery offen |
| Kaufen, Consent, Allow/Block/Forget, Upload | Firestorm-Floater und Server Owner-/CAP-Vertrag | nicht freigeschaltet | deferred_with_reason bis Live-Permissions-Prüfung |

Vier zusammengehörende Firestorm-Paritätsblöcke:

1. **Bedienbare Experience-Ansicht:** Nach authentifizierter und bereiter Simulation öffnet F8 einen Floater im NexVerse-Design. Eigene/Erlaubte/Blockierte Experiences lassen sich über Maus oder die Tasten 1–3 auswählen, ESC/F8 schließt. Die Liste enthält echte Simulator-UUID-Präfixe mit ausdrücklich noch fehlenden Anzeigenamen statt erfundener Namen. Fehlende CAPS, noch nicht geladene Daten und leere Listen werden getrennt angezeigt. Solange die Ansicht geöffnet ist, löst WASD keine Avatarbewegung aus. Schreiben, Experience erwerben, Allow/Block/Forget bleiben gesperrt. Der Viewport vor erfolgreichem Simulator-Eintritt bleibt frei von der Experience-UI.

2. **Region-scoped Experience-Leseergebnisse:** FirestormRegionLease bindet Regionhandle, WorldModel-Generation und Seed-Identität an einen threadsicheren Epochentoken. Bei Regionswechsel werden alte CAP-Advert-Flags, Counts und Experience-UUID-Listen gelöscht, der Floater geschlossen. In-flight AgentExperiences/GetExperiences-Antworten werden nach dem Wechsel auch bei gültiger HTTP-200-Antwort verworfen; Lease-Check erneut unter Caps-Mutex. Die Endpoints und UUIDs dürfen nicht in Statuslogs erscheinen. Der EventQueue-Hauptworker bleibt unabhängig. Neuer deterministischer asynchroner CTest belegt die späte GET-Ablehnung. Grenze: Neues Experience-CAPS-Seed-Discovery nach Teleport ist noch nicht umgesetzt, daher wird absichtlich keine alte Liste weiter angezeigt.

3. **Inventar-/Outfit-Metadatenvertrag:** Der MPL-2.0-Adapter parse_inventory_thumbnail_metadata liest die OpenGenesisLINK-LLSD thumbnail.asset_id aus FetchInventory2/FetchInventoryDescendents2, unterscheidet fehlendes Feld, ausdrücklich leere (Zero-UUID) Vorschau und gültige Thumbnail-UUID, lehnt defekte LLSD-Strukturen ab. InventoryFolderThumbnailCache speichert zu Folder-UUID die Version und Asset-ID, lehnt ältere Folderversionen ab und akzeptiert geänderte Thumbnail-IDs auch bei gleicher Version, um die dokumentierte Firestorm-7.2.4-Outfit-Gallery-Caching-Einschränkung nicht blind zu übernehmen. Neuer CTest belegt leere, alte, ersetzte und ungültige Metadaten. Grenze: noch kein echter Inventar-GET/Outfit-Gallery-UI, kein JPEG2000-Upload, keine reale Server-Readback-Abnahme.

4. **Konkrete Funktionsparität und Verträge:** Offizielle Firestorm-Releasequelle Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d; belegt über indra/newview/llfloaterexperiences.cpp (Tabs, GetExperiences/AgentExperiences, Acquire-Button). Aktueller OpenGenesisLINK Server main @ 6dc73f2adfded7fe5ee94c992daaaba5be2b66c8; Belege doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md, doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md und serverseitige Thumbnail/Experience-Implementierung. Versionsgrenzen: OpenGenesisLINK 0.9.3.10 Dev und Firestorm 7.2.4.80712. Die Firestorm-Funktions-/Verhaltensbaseline bleibt verbindlich, der bestehende OGL-C++23-Viewer ist noch kein Firestorm-Code-Fork.

**Nachweise und Grenze:** 73/73 lokale CTests nach Integration, einschließlich zweier neuer Firestorm-Regressionen und realen GPU-Tests. Ein bestandener synthetischer Lease-/Metadatentest ist KEIN NexVerse-In-World-Test. Eine tatsächliche authentifizierte Regionsabnahme für diesen Stand fehlt weiterhin. Der F8-Floater wird nur im Simulationszustand gezeichnet; Login-Welcome-Smoke allein prüft keine Interaktion nach einem realen Simulatorbeitritt.

**Alpha.82-Priorität:** neue Regions-Experience-CAPS nach Teleport entdecken, Experience-Namen/Profile mit GetExperienceInfo lesen, servergebundene FetchInventory2/FetchInventoryDescendents2-Read-only-Outfit-Gallery aufbauen sowie Write/Consent/Upload nach separatem Owner-/Permissions-/Grid-Vertrag kontrolliert vorbereiten. Firestorm-Parität zuerst, Zusatzrenderer nachrangig.


## Alpha.82 – neuer offizieller Source-Stand und Einzelparität

| Firestorm-Funktion | Firestorm-/Serverbeleg | OGL alpha.82 | Status |
|---|---|---|---|
| Experiences bei Teleport/Regionwechsel | Firestorm regiongebundenes getCapability, Server Capability-Leases | Neues Seed-CAPS-Fetch pro WorldModel-Epoche, eigener Read-Worker, staleness-safe Commit | tested_synthetic; EventQueue selbst noch nicht live verifiziert |
| Experience-Profile mit realem Anzeigenamen | Firestorm llexperiencecache /id/?page_size=...&public_id=... und experience_keys; Server GetExperienceInfo-Variable-Path | Read-only Profilbatch bis zu 12 IDs, F8 zeigt authentifizierte Namen oder explizite ID-Fallbacks | tested_synthetic; echte Anzeigenamen nur falls CAP antwortet |
| My Outfits und Outfit-Folder aus Inventar | Firestorm LLFolderType 48/47, OpenSim XML-RPC inventory-skeleton | F9 read-only Outfit-Galerie nach Login, echte Ordnernamen und validierte UUID | tested_synthetic; dynamischer FetchInventory-Readback offen |
| Outfit-Thumbnail und Wear | OGL-Server FetchInventoryDescendents2 mit thumbnail.asset_id, serverabhängiger Cache | Bestehender LLSD-Parser/Version-Cache; F9 weist fehlende Vorschau ausdrücklich aus | deferred_with_reason: kein Bildfetch/Tragen |
| Consent, Allow/Block, Purchase, Admin | OpenGenesisLINK NexExperienceModule mit Permission/Manage-CAPS und separaten Server-Keys | keine Schreiboperationen | deferred_with_reason bis Avatar-/CAP-Rechte und Liveabnahme |

**Block A – dynamische Firestorm-CAPS nach Region-/Teleportwechsel:** Neue Protokollroutine fetch_firestorm_region entdeckt AgentExperiences, GetExperiences, GetExperienceInfo, FetchInventory2/FetchInventoryDescendents2 und InventoryThumbnailUpload anhand des echten aktuellen Simulator-Seeds. Die Routine holt autorisierte Experience-Listen ausschließlich lesend; CAP-Verfügbarkeit bedeutet keinen bestätigten Betrieb und keine Schreibberechtigung. In der App laufen Region-Read-Worker unabhängig vom bestehenden Login-CAPS/EventQueue-Longpoll, mit maximal vier ausstehenden Anfragen und nichtblockierender Future-Abholung; bei erschöpften Slots wird der aktuellste Regionsrequest nach Freigabe erneut begonnen. Ergebnisse werden nur für den noch gültigen FirestormRegionLease-Token unter Caps-Mutex übernommen. Alte UUIDs/Capabilities werden beim Regionswechsel zuerst verworfen. Wichtig: Die allgemeine Simulatorverbindung/EventQueue-Reconnect wird dadurch nicht automatisch repariert; die Verfügbarkeit einer neuen Region setzt eine vom WorldModel gelieferte gültige Seed-CAP voraus.

**Block B – echte Experience-Anzeigenamen über Firestorm GetExperienceInfo:** Authentifizierte read-only Profile werden optional vom pro Region angebotenen GetExperienceInfo-Endpoint über die Firestorm-Query /id/?page_size=12&public_id=UUID abgefragt. Nur tatsächliche IDs aus eigenen/erlaubten/blockierten Listen, maximal zwölf, werden angefragt. Der LLSD-Parser verlangt experience_keys[] mit public_id und name, validiert UUID, begrenzt Namenslänge auf 128 Zeichen und lehnt Steuerzeichen ab. F8 zeigt bei Erfolg den vom Simulator gelieferten Experience-Namen; bei fehlender/nicht unterstützter CAP bleibt die eindeutige Kennzeichnung ID XXXXXXXX (Name nicht geladen). Regionsepoche schützt auch Profilresultate, keine CAP-URLs, Experience-UUIDs oder Admin-API-Keys in neuen strukturierten Logs.

**Block C – lesende Outfit-Galerie aus echten OpenSim-Loginordnern:** F9 öffnet nach erfolgreichem Eintritt in eine Simulation ein Fenster im NexVerse-Design. Firestorms echte LLFolderType-Codes FT_MY_OUTFITS=48 und FT_OUTFIT=47 identifizieren aus der vom OpenSim-Login gelieferten inventory-skeleton die Outfit-Ordner. Die ersten zehn realen Ordnernamen werden aus maximal 256 akzeptierten UUID-validierten Einträgen angezeigt; ungültige und doppelte IDs werden ignoriert, Steuercodes in Titeln bereinigt. F8- und F9-Ansichten schließen einander aus, ESC/F9 schließt die Outfit-Ansicht, während offener Ansicht werden WASD-/Movement-Tasten nicht an den Agenten übergeben. Keine erfundene Vorschau oder Behauptung eines funktionierenden Wear/Rez. **Grenze:** Die Anzeige stammt derzeit aus dem Login-Skeleton; der aktive FetchInventory2/FetchInventoryDescendents2-Readback, Asset-Mesh-/JPEG2000-Vorschauen, COF-Verknüpfung, Outfits anziehen und Rechteprüfung bleiben offen.

**Block D – überprüfbarer Firestorm-/OpenGenesisLINK-Wire-Vertrag:** Der vollständige serverseitige Snapshot vom 11.10.2026 nutzt OpenGenesisLINK main fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b (neuer als alpha.81 6dc73f2). Offizielle Firestorm-Release-Referenz Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d. Implementierungsquellen: Firestorm indra/llmessage/llexperiencecache.cpp und .h (public_id, experience_keys, /id/), indra/llinventory/llfoldertype.h (46/47/48); OGL-Server NexVerse/RegionModules/Experiences/NexExperienceModule.cs mit GetExperienceInfo via variable path und getrennten Permission-/Manage-Keys. Der OGL-Viewer besitzt und benötigt keine zentralen World-API-Keys. Neuer nativer HTTP-Fixture-Test simuliert Seed->Listen->Profilaufrufe und verspäteten Region-Seed nach dem Wechsel, neuer Outfit-Gallery-Test validiert Firestorm-Ordner-Typen, Duplikate, Sanitizing und echte Login-Ordnerdaten. Die Tests beweisen kontrollierten OGL-Vertrag, KEINE authentifizierte Live-Regionsabnahme oder vollständigen Firestorm-Quellfork.

**Abnahmegrenzen:** Die neue F8-/F9-Bedienung ist nur während WorldModel.ready sichtbar; GUI-Welcome-Smoketest ohne echte NexVerse-Simulation belegt daher keinen Live-UI-Nachweis. Noch keine echte InWorldAssetSnapshot-Serie im bisherigen Log. EventQueue, Avatar, tatsächliche WASD-Bewegung, Upload, Wearables/COF, Marketplace/Groups/Voice und sonstige Firestorm-Funktionen bleiben gemäß verbindlicher Paritätsmatrix als noch nicht nachgewiesen/implementiert gekennzeichnet. Nach Versions-Neubau volle Tests und NVIDIA-OpenGL-Smoke durchführen.

**Nächste vier geplante Blöcke Alpha.83:** echten FetchInventory2-/FetchInventoryDescendents2-Readback der Outfitfolder mit serveradvertisierter CAPS-POST-Struktur, Thumbnail.asset_id und diskretem Cache-Vertrag implementieren; Outfit-Vorschauen aus Assetpipeline als Bilder anzeigen, keine leeren Platzhalter; Experience-Auswahl mit Profildetails (Name, Beschreibung, Berechtigungen) vervollständigen; EventQueue/Teleport-Region-CAPS- und Firestorm-Features mit echter autorisierter Simulator-Sitzung regressionsprüfen. Funktionen nur bei tatsächlichen Serverrechten aktivieren.


## Alpha.83 – konkrete Firestorm-/Server-Feature-Verträge

| Firestorm-Funktionsbereich | Offizieller Server-/Firestorm-Beleg | OGL Alpha.83 | Status |
|---|---|---|---|
| InventoryDescendents2-Fetch inkl. MyOutfits Parent | Server FetchInvDescHandler.cs, Firestorm LLFolderType 47/48 | Read-only LLSD-POST von aktuellen CAPS, Owner-ID aus Login, whitelist-UUID-Folder, Parent-Category/Thumbnail Parse | `tested_synthetic`; Live-CAP-Owner noch nicht geprüft |
| Outfit-Thumbnail via JPEG2000/PNG | Server category.thumbnail.asset_id und GetTexture CAP | Async Bild-Download max. 8, GL-UI-Upload bis 1024², sichtbare F9-Vorschau bei Erfolg, alte Region/Asset rejected | `tested_synthetic`; tatsächliche Grid-Bilder noch `not_verified` |
| Experience-Profildetails + Auswahl | Server ViewerExperience fields Name/Description/Maturity/Properties; Firestorm LLExperienceCache | Selektierbare F8-Einträge, Description, Maturity, Enabled; keine Mutationsfunktion | `tested_synthetic`; Server-Live-/Owner-Checks offen |
| Outfit Tragen/COF, Permission-Edits, Experience-Consent/Acquire | Firestorm Viewer-Wearables/Inventory/Experience-Manager und Server-Permission-CAPS | nicht freigeschaltet | `deferred_with_reason` bis tatsächliche Grid-Rechte-/Avatar-Integration |

**Block 1 – echter Firestorm-/OpenGenesisLINK-Inventarabruf per CAPS-POST:** `firestorm_inventory_fetch.hpp/.cpp` erstellt aus dem authentifizierten OpenSim-Login-Agenten und den ermittelten Outfit-Foldern ein LLSD-`FetchInventoryDescendents2`-POST mit `folders[]`, `folder_id`, `owner_id`, `sort_order`, `fetch_folders`, `fetch_items`. Bis 16 Outfits und ihre gültigen übergeordneten Folder werden doppelfrei abgefragt, weil nur die *Parent-Categories* die Thumbnail-Metadaten der Outfit-Ordner liefern. Antworten werden auf erlaubte Folder-/Parent-UUIDs, nichtnegative Versionen, max. 64 Folder-/1024-Category-Entries und den aktuellen FirestormRegionLease-Epochentoken begrenzt. Gelesen werden `categories[].thumbnail.asset_id` und versionsgebundene Foldermetadaten. Keine künstlichen Inventarordner, fremden Owner oder URL-Leaks. Der Zugriff bleibt ausschließlich lesend und nur über eine vom aktuellen Simulator beworbene Capability.

**Block 2 – echte Outfit-Vorschaubilder durch vorhandene GetTexture-/JPEG2000-/PNG-Pipeline:** Nach erfolgreichem, epochentreuem Inventar-Readback stellt die Produktions-App die vom Server gelieferten `thumbnail.asset_id` IDs und den tatsächlich beworbenen `GetTexture`-CAP bereit. Bis zu acht unterschiedliche, tatsächlich vorhandene Bilder werden in einer auf drei gleichzeitige Ladevorgänge begrenzten asynchronen Pipeline über den vorhandenen `assets::TextureAssetLoader::fetch_rgba` (PNG mit JPEG2000-Fallback) dekodiert und erst auf dem OpenGL-UI-Thread über `UiRenderer::create_rgba_texture` in sichtbare F9-Vorschau-Bilder umgewandelt. Auflösung für UI-Upload höchstens 1024×1024; Asset-ID und Regionen-Epoche werden unmittelbar vor GPU-Upload und beim Zeichnen kontrolliert. Beim Regionswechsel werden neue/alte Tasks, ID-Zuordnungen und GL-Textures getrennt behandelt; alte GL-Handles werden gelöscht, late replies verworfen. Fehler oder vom Grid nicht angebotene Caps lassen den ehrlichen Text `Vorschau -` stehen. Kein Bitmap wird erfunden und kein Firestorm-Thumbnail-Upload vorgetäuscht.

**Block 3 – Firestorm Experience-Profildetails und Benutzerselektion:** Die bestehende F8-Ansicht nutzt nun `ExperienceProfile` aus den vom Server über `GetExperienceInfo /id/?page_size=12&public_id=...` tatsächlich gelieferten LLSD-Objekten. Neben dem Namen werden Beschreibung (maximal 160 Zeichen zur Darstellung aus max. 4096), Firestorm-Maturity-Codes 13/21/42 und ein Aktivstatus aus Property-Bit 6 verarbeitet; alle Texte werden auf Länge und Steuerzeichen geprüft. Ein einzelner Experience-Eintrag ist im vorhandenen Viewer-Design per Maus oder Pfeiltasten markierbar; 1/2/3 wechseln zwischen eigenen/erlaubten/blockierten Listen. Zurückhaltender Fallback, wenn kein aktuelles Profil verfügbar ist. Keine automatische Zustimmung, kein Kauf, keine Write- oder Admin-API-Key-Nutzung.

**Block 4 – geprüfter Server-/Firestorm-Vertrag und Regressionen:** Offizieller Firestorm-Release-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` mit `indra/llinventory/llfoldertype.h` (Outfits 47 / My Outfits 48) und `indra/llmessage/llexperiencecache.cpp` (GetExperienceInfo). Offizieller OpenGenesisLINK-Server `main` @ `fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b`, insbesondere `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs`, `OpenSim/Region/ClientStack/Linden/Caps/FetchInventory2Module.cs` und `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Neuer CTest `firestorm_inventory_fetch_tests.cpp` simuliert Owner-/Folder-POST, echte Parent-Category-Thumbnail-LLSD, gesperrte Altregion-Requests und ungültige CAP-URLs. Erweiterter `firestorm_region_read_tests.cpp` testet Region-Seed -> Read-only Experience-Listen -> Profile mit Beschreibung/Maturity -> Folder-/Thumbnail-Response und Texture-CAP, ohne echte Avatar-Authentifizierung zu behaupten. Galerie-Skeleton-Test verwirft auch ungültige Parent-UUIDs. Debug C++23 und insgesamt 76 CTests nach finalem Versions-Neubau vollständig prüfen.

**Strikte Status-/Abnahmegrenze:** `tested_synthetic` für die protokollierten Antworten, Bilddekodierung und GL-Uploadpfade; **nicht** `live_verified`, weil aktuell keine authentifizierte NexVerse-Simulator-/Avatar-/Outfit-Sitzung vorliegt. Kein vollständiger Firestorm-Code-Fork, nur testbare eigene Adapter entsprechend Firestorm-Featurebasis. Die bestehende allgemeine Teleport-/EventQueue- und Avatar-/WASD-Integration ist weiterhin offen. F8/F9 sind nur nach `simulation_ready` sichtbar; Welcome-GUI-Smoketest belegt die neue UI nicht. Vollständiges COF-/Wearables-/Outfitwechsel-Verhalten, Folder-Events und Berechtigungen fehlen. Die verbesserte Cache-Metadatenroutine darf nicht als Nachweis ausgegebener Thumbnail-Bilder auf einem echten Grid missverstanden werden.

**Nächste 3–4 Blöcke Alpha.84:** kontinuierliche Firestorm-Inventarordner-Synchronisierung und `FetchInventory2`-Items samt COF-Lesezustand (nicht nur Login-Skeleton); bedienbare Outfit-Details/Tragen mit Rechte- und Permission-Checks gegen Serverprotokoll; Experience-Berechtigungsdialog mit expliziter Nutzeraktion und aktuellem Simulator-Lease; danach eine echte NexVerse-In-World-Testabnahme inklusive Avatar, Bewegung, Regionswechsel und den noch offenen EventQueue-/Mesh-Problemen. Vollständige Firestorm-Parität bleibt vorrangig.


## Alpha.84 – neue Firestorm-/Server-Funktionsnachweise

| Firestorm-Funktion | Server-/Firestorm-Vertrag | OGL Alpha.84 | Status |
|---|---|---|---|
| Outfit-Inventareinträge/COF | Firestorm FolderTypes 46/47/48; `FetchInvDescHandler` Items/Owner/Version | Owner-UUID-validiertes LLSD-Readback, `OutfitInventoryItem[]`, COF-Ordner aus Login | `tested_synthetic`, Wear noch offen |
| Outfit-Änderungen während F9 geöffnet | Firestorm Inventory event/Version, OpenSim Folderversion | 30s CAPS-Polling, `FirestormInventorySync` verwirft stale Folder und Epoch, aktualisiert Thumbnail-GL-Zuordnung | `tested_synthetic`; EventQueue-Inventarmeldungen noch offen |
| Outfit-Inhalt/Namen/COF-Status | Server `items[]` aus authentifiziertem Viewer-CAP | auswählbare F9-Liste mit bis zu sechs realen Itemtiteln und COF-Linkanzahl | `tested_synthetic`; echte Wearables nicht geladen |
| Experience Allow/Block | Server `ExperiencePreferences`: PUT LLSD UUID -> permission, GET `experiences`/`blocked`, aktuelle Avatarlease | F8 A/B vorbereiten, separate Enter-Bestätigung, ESC Abbruch, async PUT, verifizierte Listen | `tested_synthetic`; keine Live-Owner-Consent-Abnahme |
| Experience Forget/Acquire/Edit | Server DELETE/Purchase/Manage nur bei CAP und Auth | nicht aktiviert | `deferred_with_reason` |
| OpenGenesisLINK Pathfinding/Havok NavMesh | aktueller Server main `efc9b3a` entfernt OGL Pathfinding aus 0.9.3.10 Dev | keine erfundenen NavMesh-Endpunkte | `server_unsupported` für Server-HEAD; Firestorm-Inventurpflicht bleibt |

**Block 1 – Firestorm COF und wirkliche Outfit-Inventargegenstände (lesend):** Der `FetchInventoryDescendents2`-Reader liest jetzt neben den Parent-Thumbnail-`categories` die im Simulatorantwort-`items[]` enthaltenen Inventargegenstände pro Outfit-Folder. UUID, Asset-ID, Name, Asset-/Inventory-Type werden als typisierte `OutfitInventoryItem`-Einträge nur aus gültigen und begrenzten LLSD-Maps übernommen (max. 512 Antwortitems, höchstens 256 verwendete pro Ordner, Namen max. 128 Zeichen mit Entfernung von Steuerzeichen). Der separate Current Outfit Folder (COF, Firestorm Typ 46) wird aus der authentifizierten OpenSim-Login-Skeleton ermittelt und mit dem gleichen Owner-gebundenen CAPS-POST abgefragt, auch wenn keine gespeicherten Outfits vorhanden sind. Der Owner der Serverantwort wird kanonisch als UUID-Bytes gegen den eingeloggten Agenten geprüft; fremde/ältere Antworten dürfen keine Anzeigedaten ersetzen. **Keine Wearables-/COF-Mutation**, kein unberechtigter Asset-Upload.

**Block 2 – versionsgebundene, inkrementelle Inventarsynchronisierung:** Neue `protocol::FirestormInventorySync` hält Folderversion, Items und Thumbnail-Asset-IDs ausschließlich innerhalb der Regionsepoche und verwirft alte Versionen, unvollständige/fehlende Thumbnail-Maps sowie verspätete Regionsantworten. Ein gleichversionierter, aber wirklich geänderter Thumbnail-Asset-ID-Link aktualisiert die Galerie. Solange F9 geöffnet ist, prüft die Anwendung im 30-Sekunden-Abstand mit einem **maximal einen** Hintergrund-Future den bereits beworbenen `FetchInventoryDescendents2`-CAP erneut. Render-/EventQueue-Threads warten nicht auf diese Requests. Die alte GL-Vorschau wird nur bei echter Asset-ID-Änderung freigegeben, danach wieder geladen. Neuer deterministischer `firestorm_inventory_sync_tests.cpp` prüft Version-Ordering, unveränderte/ersetzte Thumbnails, ignorierte Legacy-Omissions und Region-Epoch-Reset. **Keine echte EventQueue-basierte Folder-Notifikation**, Polling während sichtbarer Galerie ist ein sicherer Zwischenstand.

**Block 3 – bedienbare F9-Outfit-Details inklusive COF-Readback:** Im bestehenden NexVerse-Look lässt sich einer der ersten zehn in der Liste sichtbaren Outfit-Folder per Maus oder Auf-/Ab-Taste auswählen. Ein rechtes Detailfeld zeigt dessen tatsächlich vom Server gelesene Gegenstandsnamen (bis sechs), Anzahl und den separaten aktuellen COF-Linkstand, sofern dieser geladen wurde. Fehlt die Owner-CAP oder ein aktueller Abruf, steht ausdrücklich „Inhalt noch nicht geladen“. Existing PNG/JPEG2000-GetTexture-Thumbnail-Darstellung bleibt aktiver Bildpfad; fehlende Bilder bleiben unbestätigt statt gefälscht. **Outfit anziehen, BOM, Wearables, Animationen und synchronisierte Avatar-Appearance sind noch offen**, solange ihre kompletten Firestorm-LLUDP-/CAPS-Rechte und eine echte NexVerse-Simulation fehlen.

**Block 4 – Firestorm ExperiencePermissions mit ausdrücklicher Nutzereinwilligung:** Der neue `ExperiencePermissionConsent`-Vertrag ermöglicht im F8-Fenster bei vorhandener, aktueller `ExperiencePreferences`-CAP genau `A` (= Erlauben vorbereiten) oder `B` (= Blockieren vorbereiten), gefolgt von `Enter` zur **separaten Bestätigung**. `Escape`, Regionswechsel oder veraltete Epochs verwerfen die Vorbereitung. Erst nach Bestätigung startet eine asynchrone, owner-/Region-Lease-gebundene `HTTP PUT`-Operation mit offizieller LLSD-Map `{experience_uuid:{permission:Allow|Block}}`. OpenGenesisLINK liefert die bestätigten `experiences`/`blocked`-Listen; nur wenn die gewählte ID genau im Zielset und nicht im Gegenset steht, zeigt OGL eine erfolgreiche Änderung an. Die generische HTTP-Abstraktion besitzt dafür eine fail-closed PUT-Methode; libcurl-Weiterleitungen für mutierende PUT-Requests sind gesperrt. Die anderen Firestorm-Aktionen `Forget`/DELETE, Purchase, Experience-Editor und Adminrechte bleiben **nicht freigeschaltet**. Neuer Test prüft weder versehentliche Writes noch falsche Erfolgsbestätigungen: ohne explizite Enter-Bestätigung null PUT-Requests, epoch-bound stale reject, negative Serverantwort, erlauben/blockieren.

**Server-/Firestorm-Quellenprüfung (11.10.2026):** Offizielle Firestorm-Releasequelle `FirestormViewer/phoenix-firestorm` Branch `Firestorm_7.2.4` bei `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d`. OpenGenesisLINK Server `main` nun **`efc9b3a73ce04f6f57a1958c29ec0a439ed76d0f`** (neuer als alpha.83 `fcac896e`): Source `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs` mit `items`, `categories.thumbnail.asset_id`, `owner_id`, `version`; `NexVerse/RegionModules/Experiences/NexExperienceModule.cs` mit leasegebundenem `ExperiencePreferences` GET/PUT/DELETE, nur Avatar-/CAP-gebundene Berechtigungen. Server-HEAD entfernt OGL-Pathfinding aus 0.9.3.10-Dev-Scope: Firestorm-Pathfinding bleibt gewünschte Baseline, aber kann auf diesem Server derzeit **nicht als kompatible NavMesh-Funktion** behauptet werden. Neues Feature nur bei Server-Vertrag und später mit tatsächlichen Rights-/Live-Checks.

**Evidenz und Grenzen:** Versionsbuild und sämtliche 78 lokalen CTests inklusive realer GPU-Tests sowie Linux-GUI-Smoke sind vor GitHub-Release erneut durchzuführen. `tested_synthetic` beschreibt bestätigten OGL-Wire-Code und reproduzierbare Tests, **nicht** erfolgreiche Live-Outfit-Wear, Consent, Thumbnail-Erneuerung oder In-World-Avatar-/EventQueue-Funktion. Ungeprüfte bzw. fehlende Firestorm-Bereiche bleiben in der Matrix ausdrücklich offen. UI-Nachweise aus dem Welcome-Ladebildschirm belegen F8/F9 erst in einer echten `simulation_ready`-Sitzung **nicht**.

**Nächste 3–4 Blöcke Alpha.85:** COF-/Outfit-Wear- und Appearance-/Attachment-Protokoll gegen Firestorm-Source und OpenGenesisLINK-Rechte nachbauen; Firestorm-Inventar-Baum samt On-Demand FetchInventory2 und Ordner-Event-Diffs ausbauen, danach optionales Permissions-Forget mit Bestätigung und Lease; fehlende EventQueue-/Avatar-/Mesh-/WASD-Integration gegen authentifizierte NexVerse-Grid-Sitzung live prüfen. Der vollständige Firestorm-Funktionskatalog bleibt verpflichtend.


## Alpha.85 – neue Firestorm-Einzelfunktions-Nachweise

| Firestorm-Basisfunktion | Firestorm-/OpenGenesisLINK-Nachweis | OGL alpha.85 | Evidenz |
|---|---|---|---|
| Allgemeiner Inventarbaum | `LLInventoryModel`, OpenSim InventorySkeleton / `FetchInventoryDescendents2` | F10: Login-Folder, echte untergeordnete `categories` on demand, Navigation mit Tastatur/Maus | `tested_synthetic`, nicht Live |
| Inventargegenstände einzeln | Server `FetchInventory2Handler.cs`: items/agent_id/permissions.owner_id | Produktions-F9-CAP-POST max. 32, UUID-/Owner-/Region-Lease Prüfung | `tested_synthetic`, reale Avatar-Items nicht geprüft |
| Outfit-Wear-/COF-Readiness | Firestorm `llappearancemgr.cpp`: COF-Links, Wearable Fetch, Avatar Update | Typisierte Link-/COF-/Unresolved-Vorprüfung im F9 | `tested_synthetic`; tatsächliches Tragen `deferred_with_reason` |
| Experience-Berechtigung Forget | Server `NexExperienceModule.cs`: `DELETE ?<UUID>` auf avatar-scoped `ExperiencePreferences` | F8 X + Enter, Cancel beim Fensterwechsel; Server-getreue Listenabnahme | `tested_synthetic`; Live-Consent noch offen |
| Autoritative Permissions, Purchase, Manage, vollständiger Wear | Firestorm-/Server-Verträge | Kein implizites Mutieren, kein automatischer Erfolg | `deferred_with_reason` |

**Block 1 – Firestorm-ähnlicher Inventarbaum F10 mit echter Ordnernavigation:** Die neue lesende Inventaransicht `F10` zeigt Ordner aus `LoginSession.inventory_skeleton` sowie live nachgeladene `FetchInventoryDescendents2`-Kategorien (Name, Typ, Parent-UUID, Version). Auf-/Ab-Pfeile und Mausklick markieren Ordner, Enter/Rechts öffnen und Links/Backspace gehen zurück; `F10`/ESC schließen. Die Fenster F8/F9/F10 schließen einander aus, Movement-Tasten werden bei geöffnetem Inventar blockiert. Der generische Fetch-Adapter fragt im Hintergrund jeweils genau einen ausgewählten Ordner mittels gültigem, regionengebundenem Simulator-CAP ab; bis zu 256 abgefragte Ordner und 4096 lokal modellierte Folder verhindern ungebremste Rekursion. `owner_id`, `folder_id`, Parent-Zuordnung, LLSD-Typen/Versionsnummer und aktuelle Regionsepoche werden vor Übernahme geprüft. Auch ungültige/fehlende Capabilities führen nicht zu HTTP-Dauerschleifen. Unterordner außerhalb des Login-Skeleton erscheinen erst nach erfolgreichem Simulator-Fetch, nicht aus erfundenen Daten.

**Block 2 – echter `FetchInventory2`-Einzelitem-Abruf für Outfits:** `firestorm_inventory_items.hpp/.cpp` implementiert die OpenGenesisLINK-konforme `POST`-Anfrage `items:[{item_id,owner_id}]` an die vom aktuellen CAPS-Seed beworbene `FetchInventory2`-URL, gebündelt auf maximal 32 IDs aus der aktuell angezeigten F9-Outfitliste. Die Antwort `agent_id` und `items[].permissions.owner_id` müssen exakt zum angemeldeten Avatar gehören; unbekannte IDs und ungültige UUIDs/LLSD-Typen werden herausgefiltert. Lesen und Sanitizing der Itemnamen, Asset-IDs und Typen sind getestet. Die F9-Auswahl startet einen asynchronen On-Demand-Fetch; dessen Ergebnis bleibt mit Folder und Regionsepoche verknüpft. Ein Fetch-Ergebnis eines zuvor gewählten Ordners darf dem neuen nicht als aufgelöste Referenz untergeschoben werden. Die volle Avatar-/Wearables-Appearance-Pipeline ist noch nicht vorhanden.

**Block 3 – COF-/Outfit-Wear-Vorprüfung statt fehlerhaftem Blind-Wear:** Nach Vergleich mit Firestorm `indra/newview/llappearancemgr.cpp` überprüft `firestorm_outfit_wear_preflight` vorhandene Outfit-Items, Current Outfit Folder (COF, LLFolderType=46), referenzierte LLAssetType-24-Links und unverstandene Folder-Links (Typ 25). F9 zeigt einen expliziten Status für noch nicht geladene Inventare/COF, leere Outfit-Folder, nicht aufgelöste Linkziele und noch fehlende Wearable-/Attachment-/Avatar-Appearance-Synchronisation. `can_wear()` bleibt **absichtlich false**: Firestorm-Wear benötigt COF-Linktransaktionen, tatsächliche Wearable-Assets, Attachment-Regeln und Appearance-Abgleich. Der Server- und Firestorm-Quellvergleich erlaubt keinen erfundenen einzelnen `Wear`-CAP; die vollständige Outfit-Aktion bleibt offen. Ein User-Klick darf keinen falschen Erfolg anzeigen.

**Block 4 – explizit bestätigtes Firestorm Experience-Forget:** `F8` unterstützt bei vorhandener aktueller `ExperiencePreferences`-CAP nun zusätzlich `X` zur Vorbereitung von „Vergessen“, gefolgt von **Enter** zur ausdrücklichen Bestätigung (ESC Abbruch). Der Connector nutzt den OpenGenesisLINK-Vertrag `DELETE` auf `<current-capability>?<UUID>`, gegen ungültige IDs und abgelaufene Regionsepochen geschützt. `IHttpTransport::remove()` ist standardmäßig fail-closed, `CurlHttpTransport` unterbindet Redirects für mutierende DELETE-Requests. Erst wenn die Serverantwort die betroffene ID weder in `experiences` noch in `blocked` enthält, meldet der Viewer einen bestätigten Erfolg. Vorbereitete Aktionen werden auch beim Schließen/Wechseln zwischen F8/F9/F10 verworfen. Tests beweisen keine DELETE ohne gesondertes Enter, korrekten HTTP-Queryvertrag, Servernegativantwort- und Stale-Lease-Schutz. Experience-Erwerb, Rollen-/Admin-Bearbeitung und beliebige Write-CAPs bleiben gesperrt.

**Quelldisziplin, 11.10.2026:** Offizielle Firestorm-Basis `FirestormViewer/phoenix-firestorm`, Branch `Firestorm_7.2.4`, HEAD `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` einschließlich `indra/newview/llappearancemgr.cpp`, `indra/llinventory/llfoldertype.h`, Experience-Floater. Aktueller OpenGenesisLINK-Server `main`, HEAD **`885fa72a10fec770d6dc1220f08766437159a084`** – neuer als Alpha.84 (`efc9b3a`); darin u. a. ExperienceInfo-ID-Fix (`2194527`) und ein interaktives Firestorm-LSL-E2E-Testskript (`885fa72`). Für Inventory/COF sind `OpenSim/Framework/InventoryItemBase.cs` und `OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs` maßgeblich (inkl. `item_id`, `permissions.owner_id`, `agent_id`), für Experience-Forget `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Vor weiteren Codeänderungen immer aktuellen Server-HEAD erneut prüfen; eine lokal vorhandene Testdatei beim Server beweist KEINE vom Viewer vollzogene Live-Abnahme.

**Tests und Grenzen:** Für Alpha.85 wurden neue CTests `firestorm-inventory-items-tests` und `firestorm-inventory-tree-wear-tests` integriert sowie Forget-/COF-/F10-Owner-/Region-Regressionen erweitert. Insgesamt 80 CTests geplant und im finalen Debug-C++23-Versions-Neubau mit echten GPU-Tests zu prüfen. Ein synthetischer HTTP-Fixture-Test ist `tested_synthetic`, nicht `live_verified`. Der GUI-Welcome-Smoke verifiziert nicht die post-login F8/F9/F10-Fenster; echtes NexVerse-InWorld-Testing, Avatar, Bewegung, EventQueue/Teleport, Meshes und Outfit-Wear bleiben als nicht abgenommen markiert.

**Nächste 3–4 Blöcke Alpha.86:** Echte Firestorm-COF-/Wearable-Asset- und Attachment-Transaktion anhand `llappearancemgr.cpp` und OpenGenesisLINK-Serverrechten entwerfen/implementieren und mit Live-Avatar-Readback absichern; den F10-Baum um gezielten Item-Readback und Folder-EventQueue-Änderungen ergänzen; Experience-Consent-/Avatar-/Permission-Dialoge gegen aktuellen Simulator robuster machen; authentifizierte In-World-Abnahme (Avatar, WASD, Mesh und EventQueue) mit realem Grid-Zugang priorisieren. Volle Firestorm-Funktionsparität bleibt die verbindliche Baseline.


## Alpha.86 – F10 Inventory / FetchInventory2 (11.10.2026)

| Firestorm-Funktionsbereich | Implementiert | Status |
|---|---|---|
| InventoryDescendents2 Items im F10 | Eigentümer-/Folder-geprüfter Items-Readback (max. 256) und reale Gegenstandsnamen | tested_synthetic; Live noch offen |
| Folder Sync | Versionen, 30s Read-Polling, R-Refresh und Region-Cache-Reset | tested_synthetic; EventQueue-Diffs offen |
| Itemdetails | I liest per FetchInventory2 nur gewähltes Item, Lease-/Folder-/Selection-Guard | tested_synthetic; InWorld offen |
| Firestorm COF/Outfit Wear/Appearance | nicht freigeschaltet; vollständige Auth-/Rechte-/Attachment-Transaktion nötig | not_implemented |

Aktuell abgeglichene Quellen: OpenGenesisLINK main
885fa72a10fec770d6dc1220f08766437159a084 und offizieller
Firestorm Releasebranch Firestorm_7.2.4
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.


## Alpha.87 – Server-Inventory BulkUpdateInventory

| Firestorm-Einzelfunktion | Protokoll / OGL | Status |
|---|---|---|
| CAPS BulkUpdateInventory Event | Validierter AgentData/ItemData/FolderData Eventparser | tested_synthetic, Live unbestätigt |
| Folder Cache-Invalidierung | Owner- und epochgebundene MainThread Inbox mit begrenztem Speicher | tested_synthetic |
| F10 Inhaltsaktualisierung | Dirty Folder und HTTP Invalidation-Revision; CAPS nachladen | tested_synthetic |
| F9 Outfit-Galerie-Aktualisierung | Stale Reply Ablehnung und read-only Refresh mit Coalescing | tested_synthetic |
| F10 Einzelitem Parent-Validierung | FetchInventory2 mit geprüftem erwarteten parent_id | tested_synthetic |
| Teleport auf neue Region | Alter EventQueue-Poll wird invalidiert | partial: Neuer Circuit/EventQueue offen |
| Vollständige Firestorm-Inventory-Edits und Outfit-Wear | Noch keine Wear-/COF-Mutation | not_implemented |

Quellen: OpenGenesisLINK main ed37b9db720924c1d7105ea499a8c96430e9b7ea;
Firestorm Firestorm_7.2.4 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.


## Alpha.88 – Region-Circuit und EventQueue-Handoff

| Firestorm-Einzelfunktion | OGL-Implementierung | Abnahme |
|---|---|---|
| Teleport/CrossedRegion: Destination LLUDP | Alter Circuit-Stop, Future-Join vor neuer UseCircuitCode/CompleteAgentMovement-Sequenz | tested_synthetic; echter Regionswechsel fehlt |
| Regionsbezogenes EventQueueGet | Ziel-CAP-Discovery erst nach aktivem LLUDP, Ack-/Idle-/404-/Retry-Lifecycle | tested_synthetic; echte neue EventQueue noch unbestätigt |
| Alte Regionen-Pakete nach Übergang | Aktueller Simulator-Endpunkt und Stop-Flag vor WorldModel-Paketdispatch | tested_synthetic |
| Stale Login-Environment-HTTP | Epoch-Prüfung vor Render-WorldModel-Schreibzugriff | tested_synthetic |
| OGL-Simulator-UI nach Handoff | WorldModel READY nur bei passender aktiver Circuit-Verbindung | code path vorhanden, live nicht bestätigt |
| Meshes, Avatar/WASD und vollwertige Wearables | kein Full Firestorm-Live-Beleg | not_live_verified |

Offizieller Firestorm Branch Firestorm_7.2.4
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d;
OpenGenesisLINK main f9ee61580a1b6aaf1d420b545ffce883b3dade0c.


## Alpha.89 – WASD-Wire und Avatar-Proxy, reale Live-Log-Belege

| Firestorm-Funktion | OGL-Änderung | Akzeptanz |
|---|---|---|
| AgentUpdate und WASD | LLQuaternion 12 Byte; vollständige 114-Byte-Payload, U32-ControlFlags Offset 109 | tested_synthetic; echte Bewegung nach Neustart noch offen |
| Regionswechsel READY | Aktueller LLUDP Circuit-Zielendpunkt statt initialem Login-Simulator | tested_synthetic; echter Teleport offen |
| Self-Avatar Rendern | Physische OpenGL-Self-Proxy-Draws separat gezählt und GPU-getestet | tested_gpu; skinned Appearance weiterhin fehlt |
| Mesh/Textur Readback | Tatsächlicher Assetstatus aus sieben vorherigen InWorld-Snapshots | live_logged in Alpha.88, verbleibende Fehlassets |
| InWorld-Abnahme | Last-N-konsekutives Steady-State-Fenster ohne Cold-Start-Verzerrung | tested_fixture; reales vollständiges Acceptance weiterhin not_verified |

Referenz Firestorm Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d,
OpenGenesisLINK main @ 16d219f142926c7e00327e4ca91ecb1f8fc213e6.


## Alpha.90 – Legacy LLUDP Inventarentfernung (committed_unbuilt)

| Funktion | OGL-Pfad | Abnahmestatus |
|---|---|---|
| RemoveInventoryItem Low 270 | Agent-/Session-gepruefte Nachricht -> full CAPS resync | committed_unbuilt |
| RemoveInventoryFolder Low 276 | Agent-/Session-gepruefte Nachricht -> full CAPS resync | committed_unbuilt |
| RemoveInventoryObjects Low 284 | zwei begrenzte Variable-Record-Arrays -> full CAPS resync | committed_unbuilt |
| F9/F10 Readback statt voreiliger Loeschung | bestehende epoch-sichere Inbox/MainThread-Invalidierung | Code integriert; Live-Abnahme offen |
| Wear, COF, Bakes und echtes Avatar-Outfit | nicht implementiert | not_implemented |

Firestorm message_template.msg (Branch Firestorm_7.2.4) und OpenGenesisLINK main 953063775933b6d7fab4f746d055587b8d74f2ea herangezogen. Da Linux-Desktop offline, Build und CTests noch ausstehend.


## Alpha.91 – Firestorm-Kompatibilitätsnachweise (13.10.2026)

| Firestorm-Funktion | OGL-Entwicklung | Status |
|---|---|---|
| AgentUpdate High 4 | Vollständige Body-/HeadQuaternion 12 Byte, State, Camera-/Far-/Controls-/Flags-Golden-Offsets | lokal CTest, Realgrid noch offen |
| Bewegung / Positionsantwort | Neuer thread-sicherer WASD/Terse-Deltamonitor mit 4-s/3-Samples-Stillstandwarnung | lokal CTest, Live noch offen |
| RemoveInventoryItem/Folder/Objects | Agent-/Session-gebundene Readback-Invalidierung, 64/65-ID-Grenze | lokal CTest; Live noch offen |
| Regions-/Login-Reset | Movement-Monitor wird beim Circuit-Neustart zurückgesetzt | Source-Test, echter Teleport offen |
| Avatar Bake/Skeleton/Outfits/Attachments | kein vollständiger Firestorm-Ersatz | not_implemented |

Quellen Firestorm Firestorm_7.2.4 @
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d;
OpenGenesisLINK main @
953063775933b6d7fab4f746d055587b8d74f2ea.


## Alpha.92 – Firestorm Low 158 / Appearance- und Avatar-Pose-Handoff

| Bereich | OGL-Implementierung | Abnahme |
|---|---|---|
| AvatarAppearance Sender/IsTrial | gültige Nachricht, eigene AgentID getrennt, remote appearance Readback | tested_synthetic; Live offen |
| AppearanceData (COF und Flags) | read-only gespeichert, bei ObjectUpdate-Pose erhalten | tested_synthetic, Wear-Transaktion fehlt |
| AppearanceHover Variable | LLVector3-Float-Prüfung, Proxy-Z-Versatz -2m..2m | code+fixture; echtes Körper-Skinning fehlt |
| AttachmentBlock Variable | LLUUID/AttachmentPoint geparst, begrenzt, gespeichert | tested_synthetic, 3D-Attachments fehlen |
| WorldModel-Pose | gemeinsamer update_avatar_transform für Full/Compressed/Coarse/Terse | tested_synthetic, echte Regionen offen |
| SelfAppearance | getrennte WorldSnapshot-Daten, kein Avatar-Geisterproxy | tested_synthetic, Live-Readback offen |
| Vollständiger Firestorm-Avatar | gebackene Texturen, Skin, Skeleton, Animation, Outfit-Wear | not_implemented |

Quellen: Firestorm Firestorm_7.2.4 scripts/messages/message_template.msg;
OpenGenesisLINK main c29f3e5a70ccc0971b22371f1d13fd1de23be240.
OGL nutzt Firestorm als Standard-Funktionsbaseline.


## Alpha.93 – Firestorm baked Avatar-Texturen und Attachment-Root

| Funktion | OGL-Implementierung | Abnahme |
|---|---|---|
| Firestorm Avatar-Bake-Faces 8/9/10, übrige 8 Slots | 11 UUID Face-Indizes, 3 sichtbare; nur echte explizite Overwrites | Unit-Tests; kein NexVerse-Livebeleg |
| Baked Skin GetTexture | Priorisiert wie prim/material assets, gemeinsamer Budget-/Mip-/Retry-/GPU-Pfad | Pipeline code+tests; Grid offen |
| Baked Head/Upper/Lower | Tatsächliche OpenGL-Textur-Shader-Samples an Proxy | GPU test; noch nicht SL-Skinned |
| Fallback bei fehlenden Bakes | Farbproxy bleibt sichtbar, kein Blinddownload von IMG_USE_BAKED-* | GPU test + parser fixture |
| Avatar parent local-id | Attached Prim Root folgt Self-/Remote-Avatar Pose | CPU lookup test; Live offen |
| Firestorm-Skeleton/RigMesh, richtige Bake-UV und AttachmentPoint | Weiterhin kein echtes Skinning oder Bones | not_implemented |
| COF/Wear und AvatarAnimation | Weiterhin keine vollständigen Aktionen | not_implemented |

Firestorm-Releasespezifikation: LLAvatarAppearanceDefines ETextureIndex
und IMG_USE_BAKED_* in Branch Firestorm_7.2.4.
OpenGenesisLINK Server main c29f3e5a70ccc0971b22371f1d13fd1de23be240.


## Alpha.94 – echte Firestorm-Body- und Skeleton-Assetintegration

| Standardfunktion | OGL-Stand | Beweis / Grenze |
|---|---|---|
| Linden Binary Mesh 1.0 | original .llm lesen, bound-checks | real unit tests |
| Head/Upper/Lower | 3 Originalmeshes, 4320 Vertices, 7186 Dreiecke | OpenGL Real-Asset-Tests; kein animiertes Skinning |
| Original UV / Baked Face 8/9/10 | bekannte GetTexture-Shaderbindung | GPU resident/fallback verified |
| Original Firestorm-Skeleton | 133 Bones/Jointnames und statische Bind-Pose | Real XML reader test; keine Live-Skinning |
| Attachment-Bind-Points | 55 Slots, echte UUID/LID-Bindung | Parser/GPU tests, live pending |
| HUD Attachments | mScreen slots 31-38 nicht als 3D-Weltobjekte | noch kein 2D-HUD |
| InWorld Bewegung, Wear, COF, real rigging | weiter ausstehend | not_live_verified |
| Full Firestorm Source Parity | NICHT full-fork; Originalressourcen integriert | weitere Quelle-/Lizenz-Pruefung |

## Alpha.95 – echtes Firestorm VertexWeight-Skinning (synthetic acceptance)

| Feature | Implementiert | Noch offen |
|---|---|---|
| LLJointRenderData Palette | Head 3, Upper 15, Lower 9 inkl. BASE Ancestors | Firestorm joint rest-rotation/full animations |
| Original VertexSkin F32 | GPU VBO attribute 4, Float-Gewichtsmischung im GLSL | Normal-/Tangent-Skinning |
| Skeleton Parent Pose | 133 Joins dynamisch zusammengesetzt | volles Bone-Transform-System |
| Self Walk | Prozedural aus Controls, nie Serverposition vorgetaeuscht | echte AvatarAnimation E2E |
| Remote Walk | Nur gemessene Server-Positionsdeltas | AvatarAnimation Playback |
| GL Ressourcen | SkinVBO Cleanup, nicht-avatar State Isolation | grosse Last/Live GPU |
| Outfit/COF/Shape Morph | Unveraendert offen | not_implemented |

## Alpha.96 – authoritative animation wire and built-in motion selection

| Firestorm baseline | Implementiert | Evidence and missing |
|---|---|---|
| AvatarAnimation High20 | Authenticated circuit, complete bounded decoder | Golden LLUDP fixture |
| AnimationList/SequenceID | Full replace, signed S32, duplicate-max | Unit tests |
| AnimationSourceList/PhysicalEvent | Raw readback only | Block count/length tests, not executed |
| Self vs remote | Strict local avatar, invisible pending 64, epoch reset | WorldModel synthetic |
| Standard ANIM_AGENT UUIDs | Walk/Run/Jump/Fly/Sit/Hover recognized | Source-based UUID assertions |
| Real GL skeleton | Original 3 LLM weight meshes draw six provisional poses | NVIDIA GL regression |
| Keyframe priority/morph/asset playback | Not implemented | Cannot claim full Firestorm animation |

## Alpha.97 – GUI-Pflicht: Firestorm-Funktionsbaseline sichtbar im Viewer

| Firestorm-Oberfläche | OGL real verknüpft | Einschränkung |
|---|---|---|
| Top-Menüs & Toolbar | 5 Dropdown-Menüs, aktive untere Aktionsleiste | Noch nicht alle Firestorm-Menüpunkte |
| Avatar | Live Avatar-, Appearance- und Animations-Readback-Floater | Kein COF/Wear-Write |
| Kommunikation | LocalChatWindow / LLUDP send/receive | IM/Friends/Voice fehlen |
| Welt/Orte | Region, regionale Mini-Karte, Teleport-XYZ | keine globale Webmap oder LMs |
| Inventar/Outfits | Bestehende CAPS-Viewer-Floater + klickbare Buttons | Wear/preflight nur lesend |
| Erlebnisse | Bestehender CAPS Experience-Floater | keine ungeprüften Kaufaktionen |
| Ansicht | Hilfefenster, F11 Fullscreen | Preferences-Umfang unvollständig |
| Unimplementierte Menüs | Freunde/Gruppen/Events/Marketplace inaktiv | kein falsches "funktioniert" |
| Responsive Test & GL | 3 Größen, tatsächliche Modal-/Texturzeichnung | authentifiziertes NexVerse E2E offen |

Ab jetzt doc/VIEWER_UI_PARITY_PLAN.md als Pflichtdokument
zusammen mit Projektplan bei jedem Backend-/UI-Block aktualisieren.

## Alpha.98 – KeyframeMotion und Animations-Viewer

| Firestorm Feature | OGL Entwicklung | Fehlender Rest |
|---|---|---|
| LLKeyframeMotion v1.0 | LE-Daten, Quat/Position, Zeiten, Loop | IK-Constraints |
| Legacy LLKeyframeMotion v0.1 | F32 Euler-/Positionskurven | volle Asset-Paritaet |
| Kurven-Sampling | SLERP/linear/LoopInOut | Fade- und Priority-Blending |
| Original LLM GPU | echter Clip sampled in JointMatrices | Live-Asset-Fetch |
| Avatar > Animationen / Dock | echte High20 UUID/Sequence-Daten | manueller Play/Stop |
| Responsive ViewerShell | 16:9, 21:9, 32:9, echter GL-Test | Login-E2E |

## Alpha.99 – GitHub-Upstream-Check, Chat-UI und Core-Abgrenzung

| Firestorm/OpenSim Feature | OGL-Status | Einordnung |
|---|---|---|
| Viewer Chat whisper/normal/shout 0/1/2 | Drei klickbare Modi, LLUDP-Golden-Test | Core |
| Cross-region llListen Eventzustellung | Keine neuen Clientpakete erforderlich | Server-only |
| oglGetRegionDimensions LSL | Editor-/Autocomplete-Kandidat, kein Client-CAPS | Core später |
| Touch-Hold GrabUpdate/DeGrab | Status offen, Server wiederholt touch | Core später |
| InventoryThumbnailUpload und Cache | Bereits Kern/Version/UUID-Logic, neuer Clear-vs-Missing-Test | Core |
| RTX und hochwertige optische Effekte | Separate Erweiterungen | Optionales Modul |

Ein GitHub-Preflight aus scripts/check_opengenesislink_server.py
ist vor und nach jedem Entwicklungsblock Pflicht.
Pin-Revision + Entscheidungen stehen in
docs/OPENGENESISLINK_INTEGRATION_MATRIX.md.

## Alpha.100 – Animationsasset-Transfer im OGL Core

Low153 TransferRequest über echte CircuitQueue, Low154 TransferInfo
und High17 TransferPacket mit streng geprüftem Status und Payload.
Notfound -2, Epoch-Reset, bounded Out-of-Order Rekonstruktion,
Clip-Decoder und GPU-Laufzeit pro High20-Sequence vorhanden.
Avatar-Animationsfenster zeigt echte Download-/Fehler-
und Clip-Jointanzahl, Download erneut per Button.
E2E gegen einen authentifizierten Server ist weiterhin offen.

## Alpha.101 – Prim Touch/Grab und Chatkanal

| Firestorm Standard | OGL Core | Noch offen |
|---|---|---|
| ObjectGrab Low117 | reales LLUDP Reliable, SurfaceInfo Count 0 | exakte Face-/UV-Parameter |
| ObjectDeGrab Low119 | reales LLUDP Reliable bei Maus-Release | Live-Skript-Eventnachweis |
| OpenGenesisLINK stationaeres Touch-Hold | Viewer muss nur Grab halten | echtes LSL TouchSmoke durchfuehren |
| Welt -> Objekte beruehren | Live WorldModel Root-Prims bis 15m | 3D Ray Picking und Child-Prim |
| Chat /7 Beep | Parser fuer Signed-Channel im ChatFromViewer | zwei Regionen CHAT01/CHAT02 noch NICHT verifiziert |
| UI/Backend gemeinsam | aktiver Button und sichtbarer Status | COF-Wear und ScriptEditor folgen |

## Alpha.102 – Direct World Picking und Server Dev Review

- 3D WorldClick: CameraRay, Quaternion-OBB, echte Low117/119.
- Sichtbare UI: Welt->Objekte und unteres Dock Objekte,
  Live-RootPrim-Highlight und Status.
- Fokusverlust/Alt-Tab: aktiven Grab kontrolliert aufheben.
- Exakte Mesh-Face-/UV-/ChildPrim-Interaktion: OFFEN.
- OpenGenesisLINK 0.9.3.12 ist DEV-Testversion,
  keine Freigabe als RC oder Stable.
- Neue Robust OAR/IAR ES256 Feature-Control ist
  SERVER-ONLY Admin/API, NICHT User-Viewer-Plugin.

## Alpha.103 – Face-/UV-Picking und Outfit-Ladeaktion

| Feature | Alpha.103 | Noch offen |
|---|---|---|
| CameraRay und GetMesh Triangles | echte indizierte Triangle/UV/Face Treffer | große Mesh BVH |
| Fehlende Mesh-Dreiecke | keine Box-Phantomtreffer, wenn Mesh resident | Sculpt/Prim Geometrie exakt |
| LLUDP ObjectGrab | weiterhin zuverlässig Low117 | geprüfte SurfaceInfo Normal/ST/Binormal |
| Sichtbares Weltfenster | Face/UV oder BoundingBox-Näherung | Child-Prim |
| Outfit Galerie | aktueller authentifizierter Read-Only Refresh-Button | COF Wear/Unwear |

## Alpha.104

Alpha.104: Original ObjectGrab/DeGrab SurfaceInfo VariableCount1 mit 64 Bytes (UV/ST/Face/Position/Normal/Binormal) aus Mesh/GetMesh/TextureEntry, sonst Count0. Anzeige im Objektfenster. Outfit F9 COF pruefen authentifiziert FetchInventory2, kein Wear/Unwear.

## Alpha.105

LLUDP LinkInventoryItem Low426/RemoveInventoryItem Low270 sind technisch im Viewer-Circuit. GUI: COF-Aenderungsplan aus Readback mit fail-closed Konflikterkennung, keine stummen Mutationen. Wear/Unwear/Apearance weiterhin unvollstaendig. Reconciliation verlangt neue Serverversion+RegionEpoch+exakte Link-Zielmenge.

## Alpha.106

| Firestorm Feature | Alpha.106 OGL | Rest |
|---|---|---|
| Ein einzelner COF-Link | Zwei-Klick-Consent, RELIABLE LLUDP 426, Server COF Readback | volle Outfit-Transaktion |
| COF-Version kontrollieren | Current Outfit Folder direkt mit InventoryDescendents2 | atomare Versioncompare am Server |
| Ergebnis bestaetigen | Hoehere Serverversion und exaktes AT_LINK Ziel | AvatarAppearance/GPU |
| F9 Viewer-Button | Link anlegen mit Confirm und Status | Tragen/Ablegen Mehrfachoperationen |
| Fehlschlag | 18s Timeout, kein Auto-Resend/Delete | rollback bei Teilfehlerserie |
| Alte CAPS Antworten | Request-Seriennummer fuer COF-Pruefen | Live Netztest |

## Alpha.107

Firestorm Low426: zwei bis vier reine ADD-LINK Operationen seriell nur nach Auth-CAPS Readback je Schritt. Low270: genau ein COF-LINK-REMOVE mit double consent und exact leftover verification. Kein kombinierter COF Wear/Unwear, kein automatisch reversibler Multi-Link Rollback, keine bestätigte AvatarAppearance.

## Alpha.108

Firestorm AvatarAppearance Low158 AppearanceData CofVersion wird jetzt als read-only zweite Bestätigungsebene nach Low426/270 COF-Readback herangezogen: exakte Version, Agent, RegionEpoch, nicht-trial, neue Paketseriennummer, nichtleere TextureEntry/VisualParam. F9 Status und echter Appearance prüfen Button. Vollständiges AgentIsNowWearing Low383 / AgentSetAppearance Low84 mit Wearable Flags, Baking und Avatar GPU-Übernahme weiterhin OFFEN.

## Alpha.109

Alpha.109: 17 originale LLWearableType 0..16 mit subtype = real InventoryFlags & 0xff aus authentifiziertem FetchInventory2/InventoryDescendents2, missing != SHAPE0. Original Low383 AgentIsNowWearing und Low84 AgentSetAppearance Byte-Encoder implementiert und registriert, aber bis echte Assets/Bakes vorhanden NICHT versendet. F9 Wearables pruefen und 17 Slotzeilen mit echten COF Originalitemdaten, kein nachweislich getragener Avatar.

## Alpha.110

Alpha.110: Original LLWearable v1 Text-Asset-Parser (permissions/sale/type/VisualParams/local Textures), strenge Bounds. Authentifizierter Firestorm Low153 Clothing5/Bodypart13 Transfer via Circuit mit TransferInfo154 und TransferPacket High17, RegionEpoch/TransferID. F9 Benutzeraktion 16 Wearables seriell, Original COF Version/Subtype geprüft, 12s Timeout und Status. 45 Original TE Indices, elf baked outputs, lokale Input->Bake Referenzgruppen. Nicht GPU-Bake, nicht Outfit-Wear.

## Alpha.111

Alpha111: F9 authentifizierte originale lokale Wearable Texture Input UUIDs via vorhandenes GetTexture PNG + OpenJPEG native J2C laden. Je 17 Original Wearable-Slots echte OpenGL RGBA8 Thumbnail Preview, max 12 deduplizierte Quellen 128px. Korrektes Original 45 TextureEntry Owner-/Bake-Source-Mapping, elf Baked Outputs ausgeschlossen. Kein Avatar-Wear/Bake.

## Alpha.112-115

Alpha112-115: original lokal installiertes Firestorm avatar_lad.xml 11 original LayerSets, 89 geordnete Layer und 3 simple Inputs gelesen; Shader/TexLayer Compiler mit Grenzen und Masken/TGA/Bump/Color-Erkennung. Source-over nur safe local RGBA8 und 3 F9 Head/Upper/Lower Teilgrafiken. VOLLSTÄNDIGER GPU Bake und Wear/Unwear weiterhin offen.

## Alpha.116-119

Alpha116-119: Original Firestorm Character RLE11 TGA Alpha und RLE10 RGB inklusive Bounds gelesen, param_alpha id/wearable/domain/multiply/skip XML Metadaten erfasst. Nur owner-verifizierte LLWearable visual parameter TGA mask source-over als lokale, unvollständige Head/Upper/Lower F9 GL Vorschau. Keine Firestorm vollständige TexLayer/Avatar Bake oder COF Tragen/Ablegen.

## Alpha.120-123

Alpha120–123 original Firestorm TexLayer Color XML 99 param_color/71 colored layers, including 25-step palettes. Fixed RGBA, GlobalColor name (not guessed), Add/Multiply/Blend with original LLWearable visual param weights and alpha TGA mask in conservative RGBA preview, GUI F9 Farben/Masken. Server AvatarAppearance TextureEntry has 11 nonplaceholder baked IDs counted separately. No complete Bake, no Outfit-Wear.


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
