# OpenGenesisLINK – OpenSim-Meshassets (Phase 8/9)

Aktueller Stand: 0.2.0-alpha.63 · 11.10.2026 (Historie ab alpha.48)

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
