# OpenGenesisLINK – CAPS/EventQueue

Stand: 0.2.0-alpha.80 · 11.10.2026 (technische Historie seit alpha.47)

## Anlass

Im Live-Test von 0.2.0-alpha.46 war die EventQueue zunächst aktiv,
danach traten in etwa 36–40-Sekunden-Abständen wiederkehrende
Warnungen auf. Der Viewer hatte bislang 35 Sekunden HTTP-Total-
Timeout. OpenSimulator hält die EventQueue im Normalfall bis zu
58 Sekunden offen (60-Sekunden-Keepalive minus 2 Sekunden).

OpenSimulator-Referenz:
https://github.com/opensim/opensim/blob/master/OpenSim/Region/ClientStack/Linden/Caps/EventQueue/EventQueueGetModule.cs

## Rückmeldungen und Behandlung

| HTTP/Transport | Klassifikation | Aktion |
|---|---|---|
| 200, gültiges LLSD | Event-Batch | Ack aktualisieren, Events dispatchen, Fehlzähler zurücksetzen |
| 502 | Leere EventQueue | Kein Fehler; Ack bewahren, erneut pollen |
| 404 | Abgelaufene EventQueue | Sofort Seed-CAP `EventQueueGet` neu erfragen, begrenzte Retry-Pause |
| 503, übrige HTTP-Fehler | Fehler | HTTP-Status und Ursache loggen, Backoff |
| TCP-/HTTP-Timeout | Fehler | Ursache loggen, Backoff, periodisch Seed-Neuanfrage |
| 200, defektes LLSD | Protokollfehler | Ursache loggen, Ack unverändert, Backoff |

Bei normalem 502-Idle wird mit 150 ms Abstand neu gepollt, um
einen Hotloop bei falsch konfigurierten Servern zu verhindern.

CAPS-Kommunikation: POST `application/llsd+xml` mit
`ack` (letzte Batch-ID oder undef) und `done=false`.

## Recovery

- Connect-Timeout 8 Sekunden, Poll-Total-Timeout 80 Sekunden.
- Echte Fehlversuche: 1 / 2 / 4 / 8 / 12 Sekunden Retry-Pause.
- Seed-Re-Discovery bei jeder dritten echten Störung oder
  unmittelbar bei HTTP 404.
- Gleiche EventQueue-URL: Ack erhalten. Geänderte URL:
  Ack zurücksetzen und Queue neu aufbauen.
- Gesundes Idle (502) oder gültige Events setzen Fehlzähler zurück.
- Shutdown-Stopp wird während des Retry-Backoffs in
  maximal 100 ms berücksichtigt, nicht erst nach ganzer Pause.
- CAPS-Token und volle Capability-URLs niemals loggen.

## Abnahme und Grenzen

Tests prüfen 502, 404, 503, TCP-Timeout, malformed LLSD,
Ack-Transport, 80-Sekunden-Timeout, Retry-Backoff und
Re-Discovery-Plan. Debug-Build, 23/23 CTest und sechs Sekunden
OpenGL-GUI-Smoke erfolgreich.

**Ein neuer authentifizierter In-World-Langzeittest mit
0.2.0-alpha.47 fehlt noch.**

Der Worker verwendet weiterhin die **ursprüngliche Login-
Seed-Capability**. Ein Regions-/Teleportwechsel kann die alte
EventQueue nicht automatisch durch die Queue einer anderen Region
ersetzen. Dieser vollständige Circuit-/Queue-Wechsel kommt im
separaten Teleport-Lifecycle-Block.


## LLSD-Date-Kompatibilität (0.2.0-alpha.49)

Live alpha.48: Ein HTTP-200-EventQueue-Paket enthielt ein
`<date>`-Feld; der XML-Parser verwarf deshalb den gesamten Batch.
Der LLSD-Typ `date` ist nun im XML- und im Binary-Pfad
('d' plus 8-Byte-Zeitstempel) unterstützt. Das gilt auch für die
optionalen Datumseinträge in Mesh-Asset-Headern. Regressionstests
decken echte Datumswerte, XML-Re-Serialisierung sowie
trunkierte/überlange Werte ab.

Der erste Live-Retest mit alpha.49 muss das Ausbleiben der
`Unsupported LLSD XML type: date`-Meldung bestätigen.


## Alpha.80 – Firestorm Experience-/Inventory-CAPS, ausschließlich serveradvertisierte Funktionen

**Block 1 – geprüfte Firestorm-Seed-CAP-Abdeckung:** `default_seed_capabilities()` fordert beim echten Login-Seed nun zusätzlich `FetchInventoryDescendents2`, `InventoryThumbnailUpload`, `FindExperienceByName`, `GetExperiences`, `AgentExperiences`, `ExperiencePreferences`, `GroupExperiences`, `GetAdminExperiences`, `GetCreatorExperiences`, `IsExperienceAdmin` und `UpdateExperience` an. Vorhandene `FetchInventory2` und `GetExperienceInfo` bleiben. Die Antwort bestimmt die tatsächliche Verfügbarkeit; nicht angebotene CAPS werden nicht simuliert. Der C++-Regressionstest kontrolliert einmalige Anforderung aller relevanten Namen.

**Block 2 – Server-capability-gesteuerte Firestorm-Bereitschaft:** Neue typisierte `FirestormCapsAvailability` mit acht klar definierten CAP-Kategorien (Inventaritem-Fetch, Inventarordner-Fetch, ThumbnailUpload, Owned/Allowed/Permissions/Admin/Update Experiences) berechnet rein aus der geprüften Seed-Antwort die angebotenen Funktionen, nie aus dem Vorhandensein von Beispiel-URLs oder dem Serverproduktnamen. Im produktiven Login-CAPS-Ladebildschirm erscheinen `Startregion: Outfit-Vorschau CAP/- | Experiences CAP/-`, optional die Anzahl eigener/erlaubter Experiences. `CAP` bedeutet *angebotener Endpunkt*, kein erfolgreicher Upload oder Firestorm-Funktionsparität. Der bisherige NexVerse-UI-Stil und die Simulation-Ready-Steuerung bleiben erhalten.

**Block 3 – Read-only Firestorm Experience-Listen, ohne EventQueue zu bremsen:** `parse_agent_experiences` akzeptiert strikt LLSD-Map mit begrenztem `experience_ids`-Array und unterscheidet `purchase`-Feld fehlend von `purchase: 0` (so verlangt es der aktuelle OpenGenesisLINK-Serververtrag für Firestorm). `parse_resident_experiences` validiert `experiences` und `blocked` getrennt. Die OGL-CAPS-Worker-Routine führt die beiden GETs nur bei tatsächlich angebotenen `AgentExperiences`/`GetExperiences` in einer separaten, beim Beenden gejointen Leseaufgabe aus, damit die EventQueue nicht durch diese optionalen Abfragen blockiert wird. Keine Experience-Purchase-/Allow-/Block-/Forget-Verwaltungsaktion erfolgt automatisch; Ergebnisse der ursprünglichen Login-Startregion werden nicht als aktuelle spätere Teleportregion behauptet. Es werden nur Anzahl und CAP-Verfügbarkeit angezeigt; keine Avatar-/Experience-UUID oder Secret-CAP-URL in neuen Diagnoseausgaben.

**Block 4 – Firestorm/OpenGenesisLINK Outfit-/Inventar-Thumbnail-Wire-Vertrag:** `build_thumbnail_begin_request(target,id)` erzeugt nur bei gültiger, nichtleerer Item-/Ordner-UUID das richtige LLSD-XML mit genau einem `item_id` oder `category_id`. `parse_thumbnail_stage` akzeptiert nur autorisierte Antwortformen `{state:upload,uploader:<HTTP(S)-URL>}` bzw. `{state:complete,new_asset:<UUID>}`. Ungültige UUIDs, Dateischemen und unbekannte Zustände werden verworfen. Dies entspricht dem aktuellen OpenGenesisLINK-Servermodul `OpenSim/Region/ClientStack/Linden/Caps/InventoryThumbnailUploadModule.cs`. Die Binär-JPEG2000-Übertragung, Besitz-/Rechteprüfung und endgültige OGL-Inventar-UI sind **nicht implementiert**; der Server selbst bindet One-Time-Uploader an aktive Owner-CAPS. Die serverseitig dokumentierte Firestorm-Outfit-Thumbnail-Cache-Limitation bleibt separat bestehen. `tests/firestorm_compat_tests.cpp` deckt Capability-Abgleich, Listenschema, Größenlimits, LLSD-Serialisierung und Upload-Handshake ab.

**Externe Codequellen, 11.10.2026:** Aktueller OpenGenesisLINK-Server `main` = `6dc73f2adfded7fe5ee94c992daaaba5be2b66c8`, Entwicklungszug 0.9.3.10 Dev / Stable 0.9.3.8, Commit vom 11.10.2026: Experience Manage API Keys können jetzt mit separatem `experiences:viewer:manage`-Scope ausgegeben werden. Dies ist **keine** Viewer-seitige Admin-Key-Nutzung: OGL darf niemals interne World-API-Secrets erhalten. Firestorm offizielle Referenzrelease-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` (Nightly `master` nicht als Release). Relevante Firestorm-Quelle `indra/newview/llfloaterexperiences.cpp` und OGLServer-Dokument `doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md` sowie `FIRESTORM_INVENTORY_THUMBNAIL_09310.md`. Server-/Client-SHAs vor jedem künftigen Featureblock neu vergleichen.

**Nachweis- und Einschränkungsstatus:** Erste vier Firestorm-Vertragsbausteine sind implementiert und lokal synthetisch getestet, aber kein vollständiger Firestorm-Fork und keine vollständige Experience-/Outfit-Benutzerfunktion. Die ursprüngliche reale In-World-Logserie enthält keine frischen Snapshotnachweise, Avatar/WASD/Mesh/EventQueue/Teleport bleiben noch zu integrieren. Weiterhin keine Behauptung einer erfolgreichen NexVerse-Live-Experience-/Inventar-Abnahme. Lizenzentscheidung für tatsächliche Firestorm-LGPL-Source-Übernahme bleibt offen; dieser Commit übernimmt keinen fremden Firestorm-Quellcode.

**Nächste 3–4 Blöcke Alpha.81:** Read-only Experience-Funktionen in die echte OGL-InWorld-Benutzeroberfläche mit regionengebundener Session-/CAP-Lease übernehmen; LLSD Inventar-/Outfit-Thumbnail-Metadaten inklusive Versionswechsel und Rechte abstrahieren; volle Firestorm-Menü-/Funktionsinventur und aktuelle Server-CAPS-Diffs weiter ausfüllen; anschließende kontrollierte Entwicklung von Upload/Experience-Consent mit echter Grid-Permission-/Owner-Abnahme. Firestorm-Parität hat Vorrang vor neuen Renderer-only-Erweiterungen.

Das Login-Seed-CAP-Set ist jetzt eine funktionale Schnittstelle zwischen Firestorm-Funktionsinventur und OpenGenesisLINK-Server. Auch wenn eine Capability auf dem Server im Quellcode vorhanden ist, ist sie für den aktuellen Agenten nicht notwendigerweise beworben. Die Ladebildschirm-Kennung `CAP` heißt nur „serverseitig angeboten“. Abfragen zu Experiences laufen außerhalb des EventQueue-Longpoll-Threads. URLs der CAP-Handler müssen geheim bleiben.
