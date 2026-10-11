# OpenGenesisLINK – OpenSim-Meshassets (Phase 8/9)

Aktueller Stand: 0.2.0-alpha.70 · 11.10.2026 (Historie ab alpha.48)

## Umfang

**Meshobjekte** im OpenSim-Simulator sind keine lokalen .obj/.dae-Dateien:
sie referenzieren einen serverseitigen UUID-Asset. Dieser Stand
führt einen ersten Pfad von ObjectUpdate → GetMesh → LLSD-LOD →
CPU-Mesh → OpenGL-Vertex-/Indexbuffer und Face-Ranges ein.

## Protokoll

- `ObjectUpdate.ExtraParams`: variabler Byteblock; Entry-Count,
  16-Bit LE Typ, 32-Bit LE Länge, Daten. 0x30 = Sculpt;
  Sculptdaten: 16-Byte Asset-UUID und 1 Byte Typ.
  `type & 0x07 == 0x05` bedeutet Mesh; hohe Bits können
  Mirror/Inversion-/Animesh-Flags enthalten.
- Speicherung im WorldModel; Compressed/Terse-Partial-Updates
  bewahren die bereits empfangene Mesh-UUID.
- Regionsgebundene Capability `GetMesh`, ersatzweise `GetMesh2`.
  Download `?mesh_id=<UUID>`, HTTP GET außerhalb GPU-Thread,
  mit 8s Connect- und 45s Total-Timeout.
- Asset: LLSD Binary-Header-Prefix (Map), `high_lod` oder
  `medium_lod` mit Offset und Size ab Headerende.
  LOD-Zeichenkette ist zlib-/gzip-komprimiertes LLSD Binary
  Array pro Mesh-Face.
- Ein Submesh enthält Position (3 U16 pro Vertex),
  TexCoord0 (2 U16) sowie TriangleList (3 U16 pro Dreieck).
  PositionDomain und TexCoord0Domain bestimmen die Quantisierung.
  Fehlen Positionsdomains, verwendet der Viewer -0.5 bis +0.5.
  Für ein erstes unskinned Rendering werden flache
  Dreiecksnormalen berechnet. Die korrekte Face-Range bleibt
  erhalten und kann Texturmaterialien zugeordnet werden.
- Ab alpha.51 werden bis zu 256 gleichzeitig GPU-residente
  Meshes dynamisch nach Agent-Entfernung ausgewählt; neue,
  nahe Meshes verdrängen entfernte. Region-Epochen halten
  alte HTTP-Replies aus dem neuen Cache fern.

## Grenzen und Fehler

- 32 MiB Maximaldownload, 16 MiB entpackter LOD, begrenzte
  Vertex-/Indexzahlen. Ungültige LLSD-/Index-/Domain-Daten
  werden nicht in GPU-Buffers kopiert.
- Kein vollständiges LOD-Streaming, keine Rigging-Boneweights,
  kein Animesh oder lokale OBJ-/DAE-Datei-Importfunktion.
- Ab alpha.50 werden auch initiale Mesh-Daten im
  ObjectUpdateCompressed erfasst. Live-Test offen.
- Kein permanenter Mesh-Diskcache, keine Distance-LOD, keine
  physikalischen Collision-Meshes und kein vollständiges
  PBR/glTF-Materialsystem.
- Realer visuell überprüfter NexVerse-Mesh-Test steht aus.
  Der erfolgreiche synthetische Unit-Test ist **kein** In-World-Nachweis.

## Technische Dateien

`include/nexverse/assets/mesh_loader.hpp`,
`src/assets/mesh_loader.cpp`,
`include/nexverse/protocol/viewer_messages.hpp`,
`src/protocol/viewer_messages.cpp`,
`include/nexverse/world/world_model.hpp`,
`include/nexverse/render/renderer.hpp`,
`src/render/opengl_renderer.cpp`,
`src/app/application.cpp`,
`tests/mesh_loader_tests.cpp`.

Formatreferenz: https://wiki.secondlife.com/wiki/Mesh/Mesh_Asset_Format
und https://wiki.secondlife.com/wiki/ExtraParams


## 0.2.0-alpha.49 – Formatfehlerdiagnose und Recovery (10.10.2026)

**Live-Ausgangszustand alpha.48:** 448 Requests, 232 erfolgreich,
216 fehlgeschlagen: 208x unbekannter binärer LLSD-Marker
(vorher als Steuerzeichen dargestellt), 7x ungültiges Submesh,
1x HTTP 404. Diese Zahlen sind **vor** alpha.49 ermittelt.

- LLSD-Binary-Datum ('d' + IEEE754-64-Bit-UTC-Timestamp) und
  XML-`date` werden jetzt protokollkonform eingelesen.
- Fehlende `TexCoord0Domain` sind kein Fehler mehr;
  die Standarddomain [0.0, 1.0] wird verwendet.
- Fehlerhafte einzelne Faces werden aus der teilweise aufgebauten
  Geometrie zurückgerollt; gültige weitere Faces bleiben renderbar,
  `skipped_invalid_faces=N` kennzeichnet diese teilweise Rettung.
- Seed-Discovery erfasst `GetMesh2` und `GetMesh`.
  GetMesh2 wird bevorzugt, GetMesh als alternative URL genau einmal
  bei Download-/Decoderfehler verwendet. Gleiche URLs verursachen
  keinen doppelten Request.
- Bei ungültigem LLSD-Header erscheinen Marker in Hex (`0x1d`),
  Byteoffset, Gesamtgröße und die **ersten 8 Bytes als Hex-Präfix**.
  Keine CAPS-URLs/Token und keine vollen Mesh-Assets loggen.
- Die 208 Fälle sind damit besser diagnostizierbar und haben einen
  alternativen Fetch-Weg, **aber eine erfolgreiche Live-Behebung ist
  noch nicht nachgewiesen**. Insbesondere dürfen unbekannte Bytes
  nicht geraten oder blind als gültige Geometrie interpretiert werden.
- Binärer LLSD-Header folgt der öffentlichen Mesh-Spezifikation.
  Clientseitig unbrauchbare Assetinhalte und HTTP 404 erfordern
  bei Bedarf eine Prüfung der Server-Assets.

Abnahme: `ctest --preset debug --output-on-failure` und
`./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl49-live.log`.
Bei erneuten Fehlern die Marker-Offsets/Hex-Präfixe und die
Fehler je Capability prüfen, bevor zusätzliche Decoder-Fallbacks
eingebaut werden. Vollständiges Compressed-ObjectUpdate, Animesh
und Skinning bleiben offen.


## alpha.50 – Compressed-ObjectUpdate und Mesh-Prims · 10.10.2026

**Status:** Beim Start dieses Blocks gab es kein aktuelles
/tmp/ogl49-live.log auf der Linux-Maschine. Die 208 vorherigen
Mesh-Headerfehler sind deshalb weiterhin unbestätigt.

**Implementiert:**
- Vollständige ObjectUpdateCompressed-Pakete mit CompressedFlags,
  Owner UUID, optionalen Feldern AngularVelocity, Parent,
  ScratchPad, Text, MediaURL, Particles, Sound und NameValues.
- SculptExtraParams Typ 0x30 nach den optionalen Feldern;
  Mesh-UUID und Sculpt-Typ werden ins WorldModel übernommen.
- Vollständige 23-Byte-Prim-Form und TextureEntry-Daten mit
  32-Bit-Längenfeld und Face-Textur sowie Skalierung/Parent-ID.
- Bounds-Checks und Ablehnung abgeschnittener Compressed-Daten.
- Test: synthetisches vollständiges komprimiertes Mesh-Prim
  samt Scale/Parent/TextureEntry, plus Truncation-Test.
- Formatbeleg: OpenMetaverse ObjectManager.cs,
  ObjectUpdateCompressedHandler:
  https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/ObjectManager.cs

**Noch offen:** echter NexVerse-Live-Test; 208 vorherige
Headerfehler über prefix_hex / Byteoffset analysieren;
Mesh-LOD, Rigging/Animesh, Teleport-Regionswechsel und
fehlende Servertexturen.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl50-live.log


## 0.2.0-alpha.51 – Dynamischer GPU-Mesh-Cache · 10.10.2026

**Ausgangsbefund alpha.50:** Der Live-Test meldete 256 erfolgreiche
Mesh-Uploads, 2 vollständig fehlerhafte Mesh-LODs,
3 teilweise gerettete Submeshes. Durch die Bedingung
loaded_ids.size() < 256 wurden danach keine weiteren
Meshes angefordert. Das ist ein Cache-Limit und nicht
die tatsächliche Anzahl von Meshes der NexVerse-Region.

**Implementiert:**
- Neue pure Mesh-Residency-Planung mit nach Entfernung zum
  Avatar priorisierten Mesh-UUIDs und Duplikatentfernung.
- GPU-Asset-Budget 256. Werden durch Ortswechsel neue Meshes
  relevanter, verdrängen sie zuvor geladene entfernte Meshes.
- 24 Meter Prioritätsbonus für residente Meshes vermeidet
  Cache-Churn bei geringfügiger Bewegung.
- Plan-Neuberechnung bei SceneObject-Änderung oder nach
  maximal 750 ms; GPU-Eviction pro Zyklus auf acht begrenzt.
- Renderer: set_mesh_asset liefert Upload-Erfolg, dazu
  remove_mesh_asset und clear_mesh_assets für explizite
  GL-VAO/VBO/EBO-Freigabe auf dem Renderer-Thread.
- Bei Region-Epochenwechsel werden ältere GPU-Meshes gelöscht.
  Stale Mesh-HTTP-Ergebnisse und nicht mehr priorisierte
  Ergebnisse werden nicht hochgeladen.
- Unbekannte/doppelte UUIDs und nicht-finite Positionen
  werden bei der Auswahl ignoriert; Gleichstände deterministisch.
- Zusätzlicher Testsatz für Bewegung, Eviction, Hysterese,
  Deduplikation, NaN, Nullbudget und 320 Meshes; 25/25 CTests.

**Offen und begrenzt:**
- Echte In-World-Abnahme von alpha.51 bei Ortswechsel.
- Noch keine Frustum-/Occlusion-Priorisierung.
- Mesh-Kindposition nur angenähert mit Parent-Position
  plus lokalem Offset (ohne Rotation).
- Keine persistenten Mesh-Disk-/CPU-LOD-Caches;
  beim Zurückkommen können Downloads erneut erfolgen.
- Zwei vollständig ungültige Mesh-LODs, beschädigte oder
  HTTP404-Textur-Assets; Rigging/Animesh und Teleport
  bleiben eigenständige Entwicklungsblöcke.

Live-Test: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl51-live.log


## alpha.59 – Mesh-Cache-Pipeline · 11.10.2026

Die neuen Dateien `include/nexverse/assets/mesh_cache.hpp` und
`src/assets/mesh_cache.cpp` kapseln Disk- und RAM-Residency
unabhängig vom OpenGL-Backend. Ein Worker-Future liefert
`CachedMeshResult` mit typisierter Herkunft aus Memory/Disk/Netz.
CPU-Geometrie überlebt dadurch GPU-Mesh-Eviction, sofern sie noch
im 64-MiB-RAM-LRU oder 512-MiB-Disk-LRU enthalten ist.

Die GPU erhält weiterhin nur Daten aus der gültigen
Region-Epoche und Desired-Residency. Keine neue
Mesh-LOD-Decodierung, Rigging, Skinning, Animesh oder
Behebung der zuvor defekten Serverassets. Test:
`tests/mesh_cache_tests.cpp`; 33/33 CTests grün,
OpenGL-Welcome-Smoke erfolgreich; In-World-Test offen.


## alpha.60 – Vier OpenSim-Mesh-LODs, distanzbasierte Auswahl

**Block A – OpenSim-LOD-Decoder:** High, Medium, Low und Lowest. Gewünschte Stufe zuerst; bei fehlenden/defekten Chunks Fallback auf andere gültige Level. Bestehende LLSD-/zlib-/Mesh-Bounds gelten weiter.

**Block B – Adaptive LOD-Planung:** `plan_mesh_lods` gewichtet Entfernung und Primgröße, dedupliziert mehrfach verwendete Mesh-UUIDs und nutzt Hysterese. Aktualisierung alle etwa 750 ms oder bei Szene-Änderung.

**Block C – LOD-getrennte RAM-/Disk-Caches:** High behält den bisherigen `<uuid>.mesh`-Schlüssel; andere Stufen werden unter `<uuid>.<lod>.mesh` inklusive LOD-gebundener Checksumme gespeichert. Budgets 64 MiB RAM und 512 MiB Disk. Nach Server-Fallback bezeichnet der Cache-Key den angeforderten, nicht zwingend den tatsächlich dekodierten LOD.

**Block D – GPU-Swap und Diagnose:** Mesh-LOD-Upgrades und -Downgrades ohne vorherige Entfernung der Mesh-UUID. GPU lädt zuerst neue Buffers, gibt alte erst nach erfolgreichem Upload frei. Aktuelle Regions-Epoche und Ziel-LOD werden vor Upload geprüft. Neues Log: LOD-Swap-Zähler und geschätzte VBO/EBO-Bytes.

**Verifikation:** C++23-Debug-Build, 34/34 CTests (LOD-Codec/Fallback, Cache-Varianten, Hysterese). OpenGL-Start-Smoke zum Release. Weiter offen: authentifizierter In-World-LOD-Test, GPU-Bytebudget, Texture-Mip-Streaming, Rigging/Animesh und zurückgestellte Avatar-/Bewegungs-/Teleport-Fehler.

Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl60-live.log`. Maßgeblich: `projectplan.md`, Branch `develop-current`.

`MeshAssetResult::decoded_lod` enthält beim Netz-Decoding die physische LOD. Der Renderer kann bereits geladene Meshes durch neu hochgeladene LOD-Puffer ersetzen. Szenen-Residency bleibt auf 256 unterschiedliche Mesh-UUIDs begrenzt. Der Wunsch-LOD kann sich beim Regions-/Positionswechsel ändern; veraltete async-Resultate werden verworfen.


## alpha.62 – Kamera-Priorität und GPU-Geometriebudget

**Block A – Kameraabhängige Asset-Priorität:** Ein weicher Blickrichtungsfaktor gewichtet Textur- und Mesh-Asset-Anfragen relativ zur Third-Person-Kamera. Objekte hinter dem Betrachter werden nicht ausgeblendet; sehr nahe Objekte bleiben grundsätzlich hoch priorisiert. Die bestehenden Entfernungssortierungen und GPU-Residency-Grenzen bleiben erhalten. Das ist keine echte Frustum- oder Occlusion-Culling-Implementierung.

**Block B – CPU-RAM-Texturcache:** Ein threadgeschützter 96-MiB-LRU-Cache hält vollständige dekodierte RGBA8-Bilder pro gültiger UUID vor. Der asynchrone GetTexture-Pfad arbeitet RAM → persistenter Diskcache → HTTP/Decoder. Für kleinere GPU-Mip-Basisbilder wird das dekodierte Original aus dem RAM wiederverwendet. Keine Cache-Speicherung von CAPS-URLs, Tokens oder ungültigen Bildern.

**Block C – gemeinsam begrenzte GPU-Mesh-/Objekttexturbytes:** Geschätztes, softwareseitiges gemeinsames Budget von 384 MiB mit zusätzlich jeweils 256 MiB als Maximalgrenze für Mesh-Buffer und Objekttexturen. Vor GPU-Upload werden bestehende Ressourcen und ersetzte LOD-Varianten eingerechnet. Bei Überschreitung wird ein Mesh-Upload zurückgestellt; Texturen können zuerst niedrig priorisierte Texturen verdrängen. Bei freiwerdendem Speicher werden zurückgestellte Anfragen wieder berücksichtigt. Dies ist ausdrücklich keine vollständige VRAM-Bilanz: Terrain, UI, Treiber-Overhead und weitere Ressourcen sind nicht enthalten.

**Block D – Streaming-Diagnose und Regressionen:** 60-s-Metriken für RAM-Treffer, RAM-Größe/Einträge, GPU-Mesh-/Texturbytes, gemeinsames geschätztes Budget und Deferred Counts. Durchschnittliche Zeit vom Asset-Worker-Start bis zur Verarbeitung der fertigen Future (keine reine Netzwerklatenz). Neue Tests für Kamera-Priorität, sicheren gemeinsamen Budget-Rechenweg sowie den parallelen Textur-RAM-LRU-Cache.

**Verifikation:** C++23-Debug-Build und 38/38 automatisierte CTests erfolgreich. OpenGL-3.3-Welcome-Starttest nach Versionspflege ausführen. Kein bestätigter In-World-Test des NexVerse-Asset-Streamings.

**Bewusst offen:** echte Frustum-/Occlusion-Abfrage, globale inklusive Terrain/UI gemessene VRAM-Budgetierung, inkrementeller J2C-Netzstream, Avatar-Skeleton/Bakes, Bewegungs-Synchronisierung und vollständige Teleport-/Circuit-Behandlung. Bekannte Mesh-/Texture-Serverfehler bleiben in der späteren Fehlerbehebungsphase.

**Nächste 3–4 Blöcke:** Asset-Streaming-Queue/Prioritätsregeln, planbares Prefetching mit Limits, Cache-/Regions-Invalidierung und zusätzliche Mess- und In-World-Integrationsdiagnostik.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl62-live.log`. Verbindlicher GitHub-Handoff-Branch: `develop-current`.

Das Mesh-Streaming verwendet zusätzlich zu der bisherigen Distanz/Größe-Priorität die Blickrichtung der Kamera als weichen Faktor. GPU-Mesh-Geometrie erhält ein geschätztes Einzelbudget von 256 MiB und teilt sich ein softwareseitiges Objekt-Asset-Budget von 384 MiB mit Texturen. Ist bei einem neuen oder ersetzenden LOD-Upload kein Platz, bleibt die alte GPU-Geometrie unverändert und die neue Anfrage wird vorübergehend zurückgestellt. Die bestehenden GPU-Slotbegrenzungen und Regions-Epochenprüfungen bleiben bestehen.


## alpha.63 – Mesh-Demand-Pacing

**Block A – GetTexture-/GetMesh-Anfrage-Taktung:** `AssetRequestPacer` taktet neu gestartete Textur-Demand-Worker auf mindestens 125 ms und Mesh-Demand-Worker auf mindestens 180 ms Abstand. Die bereits vorhandene Kamera-/Distanz- und Partikelpriorität bleibt maßgeblich; höchstens je ein aktiver Demand-Worker pro Assettyp und keine künstliche Blockade bereits laufender Requests. Pacer werden bei Regionswechsel zurückgesetzt.

**Block B – begrenztes Texture-Prefetching:** Der Viewer wärmt vor dem eigentlichen GPU-Upload vollständige RGBA8-Bilder aus dem persistenten Texture-Diskcache in den 96-MiB-RAM-LRU vor. Die Auswahl berücksichtigt die bereits priorisierte GPU-Residency-Reihenfolge, ignoriert GPU-residente und bekannte fehlgeschlagene UUIDs und untersucht maximal vier Kandidaten pro Durchgang. Es läuft höchstens ein Prefetch-Worker gleichzeitig, mit mindestens 650 ms Abstand und maximal 48 Prefetch-Kandidaten pro Region. Es finden dabei ausdrücklich keine zusätzlichen HTTP-Anfragen oder GPU-Uploads statt.

**Block C – Regions-Epochen und Cache-Admission:** `TexturePrefetchScope` setzt geprüfte Kandidaten bei neuem `material_scope.epoch()` zurück. Die aktuell gültige Regionsepoche wird atomar an den Prefetch-Worker weitergegeben; ein beim Abschluss veralteter Disk-Read wird vor der Cache-Aufnahme zurückgewiesen. Bestehende GetTexture-/GetMesh-Epochenschutzprüfungen im GPU-Pfad bleiben unangetastet. Diskcache und RAMcache sind UUID-basiert, CAPS-Tokens werden nicht gespeichert. Prefetch-Worker lassen sich nicht während laufender Dateizugriffe zwangsweise abbrechen.

**Block D – Telemetrie und Regressionstests:** Das 60-Sekunden-Streaminglog meldet Demand-Starts je Assettyp sowie Prefetch-Versuche, erfolgreiche RAM-Erwärmungen, Diskcache-Misses und stale Prefetch-Resultate. Neue deterministische Tests für Zeitintervalle, Kandidaten-Limits und Epochensprünge; Integrationstest des echten Disk→RAM-Prefetch-Pfads einschließlich manipuliertem Cachefile. `TextureMemoryCache::contains()` ermöglicht den Kandidatenabgleich ohne teure Bildkopie.

**Verifikation:** C++23-Debug-Build, 40/40 CTests einschließlich neuer Scheduler- und Disk-Prefetch-Tests. Linux OpenGL-3.3-GUI-Starttest nach Versionspflege. Authentifizierter NexVerse-In-World-Streaming-Test ist noch ausstehend.

**Grenzen:** Kein HTTP-Prefetch, kein eigenständiger parallel ladender Mesh-Prefetcher, kein inkrementeller J2C-Assetstream. 384-MiB-Budget bleibt eine Schätzung für Mesh-/Objekttextur-GPU-Ressourcen ohne Terrain/UI und Treiber-Overhead. Unvollständiger Avatar, Bewegung, Teleport und einzelne Mesh-/Texturserverfehler bleiben entsprechend Projektplan für die spätere Bugfixphase dokumentiert.

**Nächste 3–4 Blöcke:** Recovery und Fehlerklassifikation des Asset-Loaders, temporäre Retry-/Backoff-Logik statt permanenter Fail-Sets, robustere Regionsepochen-Aufräumlogik und In-World-Assetmetriken. `projectplan.md` bleibt bindend.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl63-live.log`; Branch `develop-current`.

Der zuvor eingeführte Mesh-LOD-/RAM-/Disk-/GetMesh-Pfad bleibt unverändert; neu gestartete GetMesh-Demand-Futures werden über `AssetRequestPacer` im Abstand von mindestens 180 ms begrenzt. Es gibt weiterhin höchstens einen aktiven Mesh-Download-Worker. Die Kamera-/Größenheuristik, LOD-Hysterese und gemeinsame geschätzte GPU-Byte-Grenze gelten unverändert. In diesem Release existiert kein eigenständiger Mesh-Prefetch-Download.


## alpha.64 – Strukturierte GetMesh-Fehler und LOD-Retries

**Block A – strukturierte Asset-Fehlerklassifikation:** Der GetTexture- und GetMesh-HTTP-Pfad liefert zusätzlich zum Diagnosetext `AssetFailure`: `transient` (Timeout/Transport, 408/425/429/5xx), `missing` (404/410), `invalid` (fehlerhafter Decoder/unerwartete HTTP-4xx) oder `configuration` (400/401/403, fehlender Transport/ungültige CAPS-URI). Bei PNG→J2C beziehungsweise GetMesh2→GetMesh-Fallback bestimmt die kombinierte Fehlerklasse die Wiederholungsstrategie; ein erfolgreicher Fallback gilt als Erfolg.

**Block B – zeitlich begrenzte Retry-/Backoff-Logik:** `AssetRetrySchedule` ersetzt die bisher bis zum Regionswechsel dauerhaft gesperrten Fehler-UUIDs. Temporäre Fehler: nach 2, 4 und 8 Sekunden erneute Versuche, danach fünf Minuten Sperrzeit; HTTP 404/410: zehn Minuten; ungültige Assets: 30 Minuten; Konfigurationsfehler: fünf Minuten. Erfolgreiche GPU-Uploads löschen den zugehörigen Fehlerstatus. Mesh-Fehlerstatus unterscheidet UUID und angeforderten LOD; GPU-Bytebudget-Deferrals bleiben weiterhin getrennt. Die Map ist auf 4096 Einträge begrenzt; bei darüber hinausgehender Kartenfluktuation wird sie geleert.

**Block C – regiongebundene Fehlerverwaltung:** Bei `material_scope.epoch()`-Wechsel werden Mesh- und Textur-Retrytabellen geleert. Bereits laufende Downloads werden nicht auf dem GUI-Thread blockiert oder abgebrochen. Fertige Antworten aus alten Epochen bleiben durch die bestehenden Epochenschutzprüfungen vom GPU-Upload ausgeschlossen; zusätzliche Stale-Counter dokumentieren diese Verwerfungen. Cache-Dateiformate und Mesh-/Texture-Cachebudgets bleiben unverändert.

**Block D – Diagnose und Regressionstests:** Streaminglogs führen `failure_events`, `retry_dispatches`, `backoff_assets`, `transient_failures` und `stale_results` je Mesh-/Textur-Familie. Tests prüfen Klassifikation von HTTP-Antworten, Rechenfolge des Backoffs, Regionsepochen, Zurücksetzen bei Erfolg und beide echten Loader-Fallback-Pfade (404 und 503) mittels Mock-HTTP-Transport.

**Verifikation:** Vollständiger C++23-Debug-Build und 41/41 CTests sind erfolgreich. Nach Versionspflege erfolgen erneuter vollständiger Rebuild und Linux OpenGL-3.3-Welcome-Starttest. Ein authentifizierter NexVerse-In-World-Streaming-Test ist weiterhin nicht ausgeführt.

**Grenzen und offene Arbeiten:** Diese Verbesserung beseitigt nicht die eigentlichen Fehler einzelner Server-Assets. Sie verhindert stattdessen endlose schnelle Retries beziehungsweise permanente lokale Sperren. Tatsächliche Simulator-Synchronisierung, Avatar-LocalID/Appearance, Bewegungsanzeige sowie Regions-Teleport bleiben wie vereinbart für spätere Fehlerblöcke offen. Ein genereller Netzwerkanfrage-Abbruch mittels Cancellation-Token ist nicht implementiert.

**Nächster Release (3–4 Blöcke):** In-World-Integrationsdiagnostik und Echtzeit-Aussagekraft, bessere Unterscheidung von Cache-/Serverfehlern, mögliche asynchrone Netzwerk-Cancellation und Asset-Streaming-Queue-Optimierung; die verbindliche Roadmap bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl64-live.log`; maßgeblicher Branch ist `develop-current`.

Ein Mesh-Ladefehler wird jetzt als `transient`, `missing`, `invalid` oder `configuration` klassifiziert. Die Wiederholungsstrategie wird pro angeforderter Mesh-UUID plus LOD gespeichert, damit ein nicht ladbarer High-LOD nicht alle anderen Qualitätsstufen sperrt. Kommt bei GetMesh2 ein Fehler, wird die vorhandene GetMesh-Fallback-Route weiter benutzt. Die Retry-Information wird bei Regionswechsel zurückgesetzt; alte GetMesh-Resultate dürfen kein GPU-Mesh überschreiben.


## alpha.65 – Mesh-Admission nach Regionsepochen

**Block A – Cache-Miss- und Dateifehlerdiagnose:** `AssetCacheLookup` und die optionale Statusausgabe von `TextureDiskCache::load()` unterscheiden `hit`, `miss`, `corrupt`, `io_error`. Der bereits vorhandene Disk-Cache-Header, die CRC-artige FNV64-Inhaltsprüfung, die 256-MiB-Ressourcengrenze und UUID-Validierung bleiben kompatibel. Texturworker berichten Disk-Misses, beschädigte Einträge und Dateisystemfehler getrennt von HTTP-Versuchen. Diese Quellenklassifikation verändert keine Serverantworten.

**Block B – koordinierte Regions-Epochen im Asset-Worker:** Der bestehende atomare Live-Epochenmarker aus dem Textur-Prefetch wird auch von Demand-GetTexture und GetMesh verwendet. Worker prüfen die Epoche vor Beginn und unmittelbar vor einem bislang nicht gestarteten HTTP-Download. Nach einem bereits laufenden HTTP-Download prüfen sie erneut vor dem Schreiben in den RAM-/Disk-Cache. Die `load_mesh_cached`-Funktion bietet dafür einen optionalen Admission-Callback. Die GPU-Ladepfade verwerfen nach wie vor veraltete Epochen. Bereits laufendes libcurl-HTTP kann noch nicht per Cancellation-Token unterbrochen werden.

**Block C – In-World-Health-Telemetrie:** Nach Aufbau der Simulation schreibt das Rendering alle etwa 30 Sekunden eine Kennzahlenzeile `InWorld asset health` (Regionsepoche, Anzahl der Szenen-Samples, gewünschte vs. bereits GPU-residente UUIDs und Deckungsgrad in Prozent, fehlende Assets, laufende Texture-/Mesh-Worker, Retry-Einträge und geschätzte GPU-Mesh-/Objekttexturbytes). Ohne aktive Simulation erfolgt keine falsche In-World-Bestandsmeldung. Die Coverage-Metrik bedeutet nur UUID-Residency, nicht vollständige grafische Darstellung, Avatar- oder Netzwerk-Korrektheit.

**Block D – Regressionen und Detailzähler:** Der Diskcache-Test prüft saubere Treffer, Miss und beschädigte Datei. Der Meshcache-Test prüft zwei Abbruchsituationen, einmal vor HTTP und einmal nach Abschluss einer künstlich veralteten Downloadfunktion; in beiden Fällen gelangt nichts Falsches in RAM/Disk. Der neue Test `asset_scene_coverage_tests` prüft leere Szenen, deduplizierte UUIDs, teilweise und vollständige Abdeckung. Die GetTexture-/GetMesh-Streaminglogs nennen zusätzlich `skipped_before_http`, `stale_after_http`, `disk_corrupt` und `http_attempts`.

**Teststand:** C++23-Debug-Build und 42/42 CTests nach Implementierung erfolgreich. Für die finale Version erfolgt nochmals vollständiger Rebuild und Linux-OpenGL-3.3-Welcome-Smoke. Es gibt weiterhin keinen authentifizierten NexVerse-In-World-Livetest; die In-World-Kennzahlen sind instrumentiert, aber noch nicht an realen Simulatordaten verifiziert.

**Unverändert offen:** Eigentliche serverseitig fehlende/beschädigte Mesh- und Texturassets, Avatar-/WASD-/Teleport-Funktionsdefizite und die vollständige visuelle Weltabnahme. Die Fehler werden protokolliert, aber nicht fälschlich als bereits behoben markiert. Keine echte libcurl-Abbruch-API in diesem Release.

**Nächste 3–4 Blöcke:** Realistisches Asset-Recovery-Verhalten unter Last, Retry-Schleifen und Prioritäts-Stabilisierung, strukturierte Diagnostics-/Health-Snapshots statt nur Textlogs und Renderer-/Szenenabnahme. `projectplan.md` bleibt verbindlich.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl65-live.log`. Branch: `develop-current`.

Die neue optionale Callback-Guard an `load_mesh_cached` schützt Stale-Replies auch vor einer nachgelagerten Aufnahme in RAM und Disk. Frühzeitig veraltete Mesh-Requests beginnen keine HTTP-Anfrage. Vorhandene Caches bleiben pro UUID/LOD gültig; das Format wurde nicht verändert.


## alpha.66 – GetMesh-/LOD-Retry-Fairness

**Block A – stabile Asset-Auswahl:** `AssetRequestFairness` integriert die bereits vorhandene Kamera-/Entfernungs- und Partikel-Priorisierung mit der pro Asset/LOD geführten Retry-Tabelle. Neue, berechtigte Assets werden innerhalb ihrer bisherigen Reihenfolge bevorzugt; bei einem bereiten Retry wird jeder vierte tatsächliche Dispatch für eine Wiederholungsanfrage reserviert. Bei ausschließlich bereiten Retries werden diese unmittelbar berücksichtigt. Ungültige UUID-Leereinträge, vollständig geladene Assets, Budget-Deferrals und noch aktive Cooldowns bleiben ausgeschlossen. Per-Region-Zähler werden beim Epochensprung zurückgesetzt. Das ist eine kleine Fairness-Schicht vor den bestehenden unabhängigen Textur-/Mesh-Demand-Workern, keine parallele Netzwerk-Multiplexer-Queue.

**Block B – stabile Retry-Zustände unter Last:** `AssetRetrySchedule` setzt bei mehr als 4096 Einträgen nicht mehr die gesamte Retry-Tabelle zurück, sondern verdrängt nur den Eintrag mit dem frühesten Retry-Zeitpunkt. Neue `blocked_count(now)`- und `ready_retry_count(now)`-Metriken trennen noch gesperrte Assets von wieder zulässigen Wiederholungsversuchen. Die bestehenden kurzen 2-/4-/8-Sekunden-Transient-Retries und längeren Cooldowns aus alpha.64 bleiben erhalten. Alte Fehlerzustände werden weiterhin bei einem Regionsepochen-Wechsel invalidiert.

**Block C – strukturierte In-World-Streaming-Snapshots:** Zu den vorhandenen 30-Sekunden-Health-Logs kommt `InWorldAssetSnapshot` mit maschinenlesbarem, numerischem JSON-Schema `ogl.asset.streaming.v1`. Felder: Regionsepoche, Szenensamples, gewünschte/geladene/offene Textur-/Mesh-UUIDs samt Abdeckungsprozent, aktuell laufende Asset-Worker, noch gesperrte und retryfähige Fehler, RAM-Cachebytes und die geschätzte GPU-Mesh-/Objekttexturbelegung inklusive gemeinsamem Softwarebudget. Ohne `simulation_ready` wird kein In-World-Snapshot behauptet; ein leeres Desired-Set ergibt rechnerisch 100 % Coverage und ist kein Nachweis einer vollständigen Welt.

**Block D – Streaming-Integrationstests:** Drei neue CTests: Fairness/Reihenfolge/Duplikate/Deferred-Assets/LOD-Keys, JSON-Schema inklusive leerer Region sowie die gemeinsame Planung aus Texture-Residency, Retry, Byte-Budget, Region-Reset und Health-Snapshot. Zudem ein Stress-Test mit 4098 Failure-Einträgen und den getrennten blocked-/ready-Kennzahlen. C++23-Debug-Build und 45/45 CTests bestanden; vollständiger Release-Rebuild und GUI-Starttest nach Versionserhöhung folgen.

**Wichtige Abgrenzung:** Die neue Integrationstest-Suite prüft die Asset-Planungs- und Budgetkomponenten, nicht einen authentifizierten In-World-Renderer- oder OpenSim-Server-End-to-End-Test. Die bereits bekannten Defizite (Avatar-Körper und Bewegung, fehlende Meshobjekte, fehlerhafte Assetantworten, Teleport und EventQueue) bleiben im Projektplan dokumentiert und werden wie vereinbart in späteren Fehlerbehebungsphasen behandelt. Die GPU-Bytes bleiben geschätzte Mesh-/Objekttexturwerte ohne Terrain-, UI- und Treiber-Overhead.

**Nächster 3–4-Blöcke-Release:** Datengetriebene Auswertung realer Simulatorlogs und Regionen, Entzerrung von Textur-/Mesh-LOD-Swaps, Renderer-GPU-Upload- und Slot-Backpressure sowie reproduzierbare In-World-Testpfade. `projectplan.md` bleibt die maßgebliche Roadmap.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl66-live.log`. Alle Änderungen auf `develop-current`; `develop` nicht automatisch verwenden.

GetMesh benutzt denselben Fairness-Planer und bildet den Retry-Key aus UUID+LOD. Eine volle GPU-Slotliste blockiert neue Slots, aber keine LOD-Ersetzung eines bereits geladenen Meshes. Der alte Budgetschutz bleibt erhalten, neue Health-Snapshots zeigen Desired-/Resident-Mesh-UUIDs, aktuelle InFlight-Werte und GPU-Meshbyte-Schätzung.


## alpha.67 – Stabile Mesh-LOD-Wechsel und Upload-Gate

**Block A – tatsächliches Viewerlog analysiert:** `tools/ogl_asset_log_report.py` (Python 3, Standardbibliothek) fasst das reale `$HOME/.local/state/opengenesislink/viewer.log` sowie künftige `InWorldAssetSnapshot`-Zeilen in einem numerischen, UUID-/CAP-URL-freien JSON-Bericht zusammen. Der historische Stand vom 11.10.2026 enthielt 230 Mesh- und 56 Texture-Fehlermeldungen, davon 42 mit 404 und eine mit 5xx-Muster; 243 weitere sind historische unklassifizierte Meldungen. Validierte In-World-Snapshots waren in dieser Datei noch nicht vorhanden. Dokumentierte Bedienung: `docs/LOG_ANALYSIS.md`.

**Block B – LOD-Dwell gegen Flackern:** Die neue generische `AssetLodDwell<Quality>` verlangt für wechselnde GPU-residente Mesh-LODs und Textur-Basismips eine über 1400 ms stabile Zielstufe. Neue noch nicht GPU-residente Assets werden sofort bedient; aktive Partikeltexturen müssen weiterhin mit höchster Bildauflösung angefordert werden. Die Anti-Oszillationszustände werden bei Regionswechsel geleert und von verschwundenen Scene-IDs bereinigt. Bereits bestehende Distanz-/Größen-LOD-Hysteresen bleiben wirksam.

**Block C – Renderthread-Upload-Backpressure und Slotfreigabe:** Der gemeinsame `AssetGpuUploadGate` hält mindestens 30 ms Abstand zwischen erfolgreichen Objekttextur- und Mesh-GPU-Uploads. Fertige Worker-Futures bleiben bis zum nächsten Upload-Fenster ausstehend; es gehen keine bereits dekodierten Daten verloren. `plan_texture_slot_admission` prüft zusätzlich zur bytebasierten Planung das 512-Slot-Limit: Ist das Limit erreicht, darf ein eingehender, höher priorisierter Textur-UUID einen niedrig priorisierten GPU-Slot ersetzen. Die Budgetrechnung berücksichtigt bereits geplante Byte-Evictions und verhindert die Verdrängung höher priorisierter Assets. Terrain-/UI-GPU-Uploads gehören nicht zur neuen Sperre.

**Block D – Regressionen und Telemetrie:** Zwei neue C++ CTests für LOD-Dwell/Regionsreset und GPU-Upload-Gate/Slot-Zuteilung; ein Python-CTest prüft den Parser mit Legacylogs, gültigem JSON, defektem Snapshot und Sanitizing unbekannter Felder. Die regulären Textur-Streaminglogs führen nun `mip_dwell_pending`, `slot_priority_evictions`, `gpu_uploads`, `gpu_uploaded_bytes`; die Mesh-Streaminglogs führen `lod_dwell_pending`. Ein vollständiger C++23-Debug-Build und 48/48 CTests waren erfolgreich; nach der Versionspflege werden sie erneut und zusätzlich der Linux-GUI-Smoke geprüft.

**Grenzen:** Die aus echten Logs gewonnenen Zahlen beschreiben historische Verbindungs-/Asset-Fehler und keine bestätigte Beseitigung der 404/Decoderprobleme. Neuer Renderthread-Takt reduziert zeitliche Upload-Spitzen, garantiert aber keine fixe Framezeit. Weiterhin ausstehend sind authentifizierte In-World-Abnahme, vollständige Mesh-Objekte, Avatar-Body/Bewegung, Teleport und EventQueue-Fehlerbehebung.

**Nächster Release (3–4 Blöcke):** Datenbasierte Prüfung anhand neuer In-World-Logs und GPU-Diagnose, weitere Upload-Messungen, scene-priority-Feedback und gezielte Render-Integration. `projectplan.md` ist weiter maßgebend; bei ausdrücklicher Nutzerpräferenz Fehlerbehebung separat.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl67-live.log`. GitHub-Branch `develop-current`.

`AssetLodDwell<MeshLod>` verzögert nur den Qualitätswechsel bereits GPU-residenter Meshes. Das bestehende `plan_mesh_lods` bleibt für die Zielstufe zuständig, mit zusätzlicher 1400-ms-Stabilitätszeit im Viewer. Nach Regionswechsel wird der LOD-Zeitstatus gelöscht. Erfolgreiche Mesh-GPU-Uploads nutzen das gemeinsame 30-ms-Renderthread-Upload-Fenster.


## alpha.68 – Mesh-Upload-Messung, Churn-Retention und Renderer-GPU-Test

**Block A – reale CPU-Uploadzeit statt Worker-Wartezeit:** `AssetUploadTelemetry` misst die Wandzeit, die das Render-Thread-Aufrufen von `set_object_texture(...)` und `set_mesh_asset(...)` benötigt. Das feste Ringfenster hält die letzten 64 Uploadversuche und bietet `recent_avg_us`, `recent_p95_us`, `recent_max_us`, `recent_count`; Nullzustand und Überlauf werden konservativ behandelt. Messgröße ist **CPU-seitige OpenGL-Übermittlung**, keine GPU-Fertigstellungszeit, kein `glFinish` oder OpenGL-Timer-Query.

**Block B – adaptives GPU-Upload-Backpressure:** `AssetGpuUploadGate` bleibt bei schnellen Uploads bei mindestens 30 ms Abstand; langsame CPU-Übermittlungen verlängern das nächste Uploadfenster auf etwa 4× beobachtete Millisekunden, bis maximal 150 ms. Der nächstfolgende kurze Upload senkt das Intervall wieder auf den Grundwert. Mesh und Textur teilen sich den Renderthread-Gate. Bereits fertiggeladene Futures bleiben für spätere Frames verfügbar. Terrain/UI und vorbestehende Renderer-Arbeiten unterliegen dem Gate nicht.

**Block C – Szenenpriorisierung und erweiterte Health-Snapshots:** `AssetScenePriorityFeedback` beobachtet alle etwa 750 ms die GPU-gewünschten UUID-Sets, erkennt erhebliche Wechsel (symmetrische Differenz mindestens ein Viertel der vorherigen/neuen Set-Größe) und erhöht für drei stabile Planzyklen den weichen Loaded-Bonus temporär: Mesh von 24 auf 42 gewichtete Meter, Textur von 8 auf 14. Neue näher gelegene Assets bleiben weiterhin auswählbar; keine UUID wird fest gepinnt. Regionsepochen löschen den Zustand. `InWorldAssetSnapshot` behält `ogl.asset.streaming.v1`, ergänzt rein numerische Felder zu CPU-Uploadavg/p95/max, Sampleanzahl, effektiver Uploadsperre und Texture-/Mesh-Prioritäts-Churn. Der vorhandene Log-Analyzer wertet zulässige Felder aus und berechnet Peak-p95 bzw. Peak-Churn; er gibt keine CAPS-URLs/UUIDs aus.

**Block D – echter Renderer-Integrationstest:** `renderer_gpu_asset_integration_tests.cpp` erzeugt einen unsichtbaren echten GLFW/OpenGL-3.3-Kontext, lädt/repliziert/entfernt synthetische Textur und Mesh auf dem GPU-Pfad, prüft ungültige Meshdaten, führt einen Renderer-Frame aus, kontrolliert `visible_objects`, Renderer-Backend und GL-Fehlerfreiheit. Ohne OpenGL-Display überspringt CTest den Test via Exitcode 77; auf dem autorisierten Linux-PC wurde der Test unter `DISPLAY=:0` vollständig ausgeführt. Zwei weitere C++-Tests prüfen Rolling-Telemetrie und Scene-Prioritätsrückkopplung; erweiterte JSON-/Log-/Upload-Gate-Regressionstests. Gesamt: **51/51 CTests bestanden**, inkl. realem OpenGL-Test, anschließend vollständiger Versions-Rebuild/GUI-Smoke.

**Grenzen:** Der Renderer-Integrationstest arbeitet mit synthetischer Geometrie und einem lokalen GPU-Kontext, ohne authentifizierte NexVerse-Region oder echtes Asset. Er belegt noch nicht, dass Avatar-Body/Bewegung, Mesh-Objekte aus dem Simulator, CAPS-EventQueue, Teleport oder alle Weltassets korrekt sind. Diese bekannten Punkte bleiben bis zur vereinbarten späteren Bugfixphase dokumentiert. CPU-Uploadzeiten dürfen nicht als verifizierte GPU-Latenz bezeichnet werden.

**Nächster 3–4-Blöcke-Release:** Wiederholbare Smoke-/GPU-Profiling-Sessions anhand synthetischer Szenen unterschiedlicher Größe, Renderer-Upload-Fehlertransparenz, Scene-Priority-Schutz unter voller Asset-Residency und Ausbau der testbaren Welt-/Mesh-Renderpfade. Maßgeblich bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl68-live.log`. CLI-Logauswertung: `python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"`; Source Branch `develop-current`.

`AssetScenePriorityFeedback` erhöht den Loaded-Bias nur temporär bei erheblichem Desired-Set-Turnover. GPU-Uploads werden auf dem Renderthread auf CPU-Wandzeit gemessen. Der CTest verwendet ein synthetisches Dreieck im echten OpenGL-3.3-Kontext, prüft Re-Upload/Replacements, sichtbare Szene und GL-Fehler; er ersetzt keinen OpenSim-Meshdownload-Test.


## alpha.69 – Mesh-Validierung vor dem GPU-Upload

**Block A – echter synthetischer GPU-Kapazitäts-/Belastungstest:** `tests/renderer_gpu_pressure_tests.cpp` erzeugt im versteckten OpenGL-3.3-Kontext auf dem Linux-Testrechner tatsächlich 512 kleine GPU-Objekttexturen und 256 kleine GPU-Meshes (Renderer-Grenzen). Bei 16, 128, 256 und 512 Texturen wird ein Frame mit den aktuell angelegten Szenenobjekten gezeichnet; Tests prüfen `visible_objects`, `draw_calls`, OpenGL-Fehlerfreiheit, abgewiesene 513. Textur/257. Mesh, korrekte Ersetzung bestehender Ressourcen sowie erneute Aufnahme nach Slot-Freigabe. Beispiel: unter dieser synthetischen Auslastung ca. 35 ms CPU-Gesamttestzeit inklusive Context-Init/Uploads/Draws. Diese Zeit ist ausdrücklich KEIN Benchmark realistischer größerer OpenSim-Assets.

**Block B – verlässliche GetTexture-GPU-Upload-Rückmeldung:** Die Renderer-Schnittstelle `Renderer::set_object_texture` meldet nun `bool` statt `void`. `LegacyOpenGlRenderer` baut Texturen immer erst in einem neuen GL-Namen auf, prüft Dimensionen und GL-Fehler, übernimmt sie nur bei erfolgreichem Upload und löscht den vorherigen Textur-Namen erst danach (transaktionale Ersetzung). Bei fehlerhaften Daten, vollem internen GPU-Slotlimit oder GL-Fehlern kehrt sie `false` zurück, ohne eine vorhandene Textur derselben UUID zu zerstören. Die Anwendung zählt `texture_gpu_upload_failures`, gibt eine Warnung aus, plant einen transienten Retry und trägt fehlgeschlagene Uploads nicht mehr als GPU-resident ein. Überlegungen zur atomaren Freigabe *anderer* prioritätsbedingt zuvor verdrängter Texturen bleiben ausdrücklich offen.

**Block C – robusterer GetMesh-GPU-Pfad:** Schon vor GPU-Submission prüft der Renderer Meshvertex-Stride, Dreiecks-Indexvielfache, jeden Index gegen die tatsächlich vorhandenen Vertices und angegebene Face-Bereiche gegen den Indexbuffer. Der eigentliche GL-Mesh-Upload kontrolliert Fehler und gültige VAO/VBO/EBO-Handles und gibt teilweise angelegte GL-Ressourcen bei Fehlern frei. Vorheriger LOD-Buffer bleibt bei gescheiterter Ersetzung erhalten. Viewer meldet `mesh_gpu_upload_failures`, klassifiziert GPU-Uploadfehler als transient und plant einen Backoff statt eines permanenten `invalid`-Fehlerstatus.

**Block D – Vollauslastung und Logdiagnose:** Der neue CTest `asset_full_residency_tests.cpp` übt 80 neue vorrangige Partikeltexturen bei 512 belegten Slots und eine Kamera-Fahrt über 400 Mesh-UUIDs bei 256 GPU-Slots aus. Er kontrolliert Priorität, Freigabe, Slotlimits und das Verbleiben naher/nützlicher Assets. `InWorldAssetSnapshot` führt zusätzliche Zähler `texture_gpu_upload_failures` und `mesh_gpu_upload_failures` im vorhandenen numerischen Schema `ogl.asset.streaming.v1`; der Python-Loganalysator nimmt sie in die sichere Feldauswahl auf und zeigt die jeweiligen Maximalwerte. JSON-, GL-Renderer- und Log-Regressionstests angepasst. C++23-Debug-Build und **53/53 CTests** inklusive zweier realer OpenGL-Kontexttests erfolgreich; vollständiger Release-Rebuild, Versionstest und GUI-Start folgen.

**Unverändert offene Grenzen:** Ein OpenGL-Test mit kleinen künstlichen Assets validiert Ressourcenhandling und GPU-Slots, aber weder echtes GetMesh/OpenSim-Decoding noch Live-Weltgeometrie, Avatar, Netzwerk-EventQueue, Teleport und In-World-Interaktion. Tests laufen auf echtem GL3.3/NVIDIA-Linux mit `DISPLAY=:0`; ohne GL-Display überspringen sich die beiden GPU-Kontext-CTests per Returncode 77. Shared GPU-Budget von 384 MiB ist eine Software-Schätzung ohne Terrain/UI/Treiber-Speicher. Die atomare Textur-Ersetzung gilt nur für denselben UUID-Slot, nicht für bereits vorab verdrängte andere Slots.

**Nächster Release / verbindlicher Plan:** Schrittweise GPU-Upload-Rollback/Recovery bei Slot-Evictions, realistischere Szenengeometrien/Texture-Mip-Lastprofile, strukturierte Upload-Failure-Gründe und weitere Renderer-Timing-Integration. Avatar/Bewegung/Mesh-/Teleport-/CAPS-Bugfixes wie vom Nutzer vereinbart separat.

Run: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Pressure einzeln: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-pressure-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl69-live.log`; nur `develop-current` nutzen.

Vor GL-Upload werden Indices und Face-Indexbereiche gegen Meshvertex- und Indexbuffer validiert; unvollständige GL-Objekte werden bei Fehlern freigegeben. Im GPU-Stresstest sind 256 Meshes vollständig belegbar und weitere Meshes werden regulär abgelehnt. Mit `renderer_gpu_pressure_tests.cpp` ist dieser Grenzpfad wiederholbar.


## alpha.70 – Strukturierte GetMesh-GPU-Uploadergebnisse

**Block A – texturschonender, transaktionaler GPU-Austausch:** `Renderer::replace_object_texture(uuid, image, evictions)` übernimmt ein komplett neues OpenGL-Textur-Handle erst nach erfolgreichem `glTexImage2D` und `glGenerateMipmap`. Erst danach werden die vorab ausgewählten Eviction-UUIDs gelöscht und die neue Textur eingebunden. Bei Fehlschlag bleiben alle bisherigen UUID-Slots, inklusive der Eviction-Kandidaten, erhalten. Der Viewer spiegelt die Freigaben erst nach erfolgreichem GL-Commit in `object_texture_loaded_ids`, `texture_gpu_bytes`, Mip-Tabellen und GPU-Bytezähler; abgelehnte Uploads zählen nicht als geladene Ressource. Der bisherige bool-Aufruf `set_object_texture` bleibt als Wrapper erhalten.

**Block B – strukturierte GPU-Uploadergebnisse für Texture UND Mesh:** `GpuUploadResult`/`GpuUploadFailure` enthält `success`, `failure`, `gl_error` mit Klassen `none`, `invalid_input`, `capacity`, `unsupported_dimensions`, `allocation`, `driver_error`. Beide Rendererpfade bieten strukturierte Methoden; der bisherige bool-Mesh-Aufruf ist ein Wrapper. Bei Texturen werden GL-Größenlimits sowie GL-Uploadfehler gemeldet, beim Mesh getrennt fehlerhafte Index-/Vertex-/Face-Daten, belegte 256 Mesh-Slots und sonstige Driver-Fehler. Application-Logs nennen nun `gpu_failure` (und bei Texturen `gl_error`), invalid/unsupported nutzt längeren invalid-Cooldown, transient/kapazitätsbedingt kurzzeitige Retries. Eine verlässliche OpenGL-Mesh-Fehlernummer wird im Renderer bei GL-Fehlern noch nicht als separates numerisches Feld exportiert und ist als Folgeschritt dokumentiert.

**Block C – reproduzierbare GPU-Upload-Lastprofile mit großen Assets:** Neuer CTest `renderer_gpu_profiles_tests.cpp` verwendet unsichtbaren ECHTEN OpenGL-3.3-Context und prüft 40 × 64², 20 × 256² und 6 × 1024² RGBA-Texturen sowie Mesh-Vertexgrids 16/32/64 (8/8/4 Instanzen, bis zu 8192 Dreiecke je Grid). Für jede Stufe werden GPU-Uploads, Renderframe, sichtbare Objekte und GL-Fehler geprüft; CPU-Upload-Zeiten (nicht GPU-Fertigstellungszeit) pro Gruppe protokolliert. Beispiel Linux RTX 4070 SUPER am 11.10.2026: Texture 283/369/4604 µs und Mesh 17/61/157 µs je Upload. Diese Zahlen gelten nur für synthetische Assets/diesen Testlauf, nicht für typische Serverdownloads oder FPS.

**Block D – Rollback-/Überbelegungs-Regressionen:** `renderer_gpu_pressure_tests.cpp` prüft weiterhin tatsächliche Belegung 512 Texturen/256 Meshes, jetzt zusätzlich Transaktions-Rollback bei ungültigem Input, fehlender Slotkapazität, unerlaubter Selbsteviction sowie übergroßen Texturen; erfolgreiches Commit entfernt exakt den vorgesehenen GPU-Victim und macht die neue ID resident. Vorhandene Mesh-Überbelegungs- und ungültige-Index-Tests prüfen die passende strukturierte Fehlerklasse. Insgesamt nach Implementierung **54/54 CTests bestanden**, inklusive drei echter OpenGL-Context-Tests; nach finaler Versionspflege erneut Full-Rebuild und GUI-Smoke.

**Wichtige technische Grenze:** Der Transaktionspfad hält den alten GPU-Textur-Slot und die neue Kandidatenressource kurzfristig gleichzeitig im VRAM. Bei sehr knappem physischem VRAM kann die sichere Transaktion scheitern und die alten Texturen behalten – ergebnisabhängig beabsichtigtes Schutzverhalten, aber später mit zusätzlicher Budgetreserve/Backpressure zu optimieren. Netzwerk-CAPS, echte Meshasset-/Avatar-/Bewegungs-/Teleportfehler wurden wie vereinbart nicht als bereits behoben deklariert. Der OpenGL-Integrationstest ist synthetisch, nicht authentifiziert in einer NexVerse-Region.

**Nächste vier Blöcke:** Strukturiertes Mesh-GL-Fehlerdetail, VRAM-Transientbudget/Reserve bei Rollback-Uploads, Instrumentierung echter Weltobjekt-Draws und Aufbau eines reproduzierbaren In-World-Akzeptanztests. `projectplan.md` bleibt verbindlich, und die ausdrücklich aufgeschobenen Avatar/Bewegungs/EventQueue-Bugfixes bleiben späteren Blöcken vorbehalten.

Start: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Profile: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-profiles-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl70-live.log`; nur `develop-current`.

Zusätzlich zu `set_mesh_asset` (bool) gibt es die API `upload_mesh_asset` mit `GpuUploadResult`. Ungültige Meshindices / Face-Grenzen sind `invalid_input`, volle 256 Mesh-Slots sind `capacity`, und offene GL-Fehler sind `driver_error`. Der OpenGL-Shader-/VBO-Pfad bleibt von der eigentlichen CAPS-Dekodierung getrennt; Live-Meshs stehen weiterhin aus.
