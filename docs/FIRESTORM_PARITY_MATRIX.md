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
