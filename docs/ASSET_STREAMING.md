# OpenGenesisLINK – GPU-Textur-Streaming und Residency

Stand 0.2.0-alpha.61 · Phase 9 (Historie ab alpha.58)
Maßgeblich: projectplan.md

## Ziel und Grenzen

Die bisherige Grenze von 512 RGBA8-Texturen stoppte neue
GetTexture-Anfragen dauerhaft, sobald alle Slots belegt waren.
Ab alpha.57 bleibt die **gleichzeitige** GPU-Residency
auf 512 verschiedene UUIDs begrenzt, kann aber ältere,
nicht mehr relevante Texturen zugunsten neuer verdrängen.

Die Priorisierung wählt zuerst sichtbare Partikelemitter
aus der Region und dann bekannte Texturen der Szene.
Materialtexturen werden beim Scene-Update anhand der
Entfernung zwischen Agent und Objekt sortiert. Es wird
pro 750ms/Scene-Revision neu geplant; maximal acht
Texture-GPU-Ressourcen werden in einem Schritt eviktiert.

GPU-Freigabe erfolgt ausschließlich auf dem OpenGL-Thread
über remove_object_texture / clear_object_textures.
Beim Regionswechsel wird der GPU-Objekttexturcache
invalidiert. Eingehende asynchrone GetTexture-Antworten
werden gegen Region/Epoche und gewünschte Residency
geprüft und nur dann im GPU-Cache angenommen.

## Bekannte Grenzen

- GetTexture nutzt weiterhin den bestehenden Netzwerk-
  Loader und besitzt nur eine aktive Objekttexturanfrage.
- CPU-Decode-/persistenter Diskcache und Streaming-Mip
  bleiben für weitere Phase-9-Blöcke vorgesehen.
- Distanzsortierung von Objektmaterialien wird bei
  Scene-Änderungen aktualisiert, nicht kontinuierlich
  ohne Objektänderung oder laufenden Kamerawechsel.
- Keine Sichtkegel-/Occlusion-Auswertung.
- Ein nachgeladenes eviktiertes Asset erfordert momentan
  einen neuen HTTP-Download. Die sichtbare Welt ist auf
  dem NexVerse-Grid noch nicht live abgenommen.
- Fehlerhafte Mesh-/Textur-/Avatar-Kopplung bleibt auf
  Nutzerwunsch gesondert für die Bugfix-Phase.

## Tests

tests/texture_residency_tests.cpp:
512-Budget, neue priorisierte Partikeltexturen,
Löschen entfernter GPU-Einträge, leere Szene und Null-UUID.

Debug-Build und 30/30 CTests in alpha.57 bestanden.


## Ausbau alpha.58

Der zuvor nur in der GPU vorhandene Texture-Cache erhält einen
256-MiB-Disk-Unterbau. Gespeichert werden decoded RGBA8-Bilder
mit Magic OGLTEX01, Dimensionen und FNV64-Integritäts-Hash.
Dateinamen werden ausschließlich aus syntaktisch geprüften
Texture-UUIDs erstellt; ungültige/veränderte Cachedateien
werden abgelehnt und erneut vom Simulator geladen.
Der Dateizeitstempel wird als LRU-Näherung aktualisiert.

Default-Verzeichnis unter Linux:
~/.cache/opengenesislink/textures-v1/
(bei XDG_CACHE_HOME dort).

TextureDistanceSamples erhalten die Objektposition bei
Scene-Updates und werden alle ca. 750ms relativ zur
aktuellen Avatarposition neu sortiert. MeshDistanceSamples
berücksichtigen neben Entfernung jetzt auch den
Prim-Maßstab als Sichtgrößenheuristik.

Bei Verfügbarkeit der OpenGL-Anisotropie-Erweiterung
werden Objekttexturen mit bis zu 8x anisotroper Filterung
gesampelt; Standard-GPU-Fallback bleibt unverändert.
Periodische Logmeldungen nennen Cache-Hits, HTTP-Loads,
GPU-Texture-Evictions und residenten Slot-Bestand.

Limits:
- Kein synchronisierter OpenSim-Asset-Hash/ETag;
  Cache gilt nur für identische UUIDs als unveränderlich.
- Kein persistenter Mesh-/Animationscache, kein RAM-LRU.
- Keine echte Mip-Stufen-Selektion/Streaming oder Occlusion.
- In-World-Test auf NexVerse weiterhin ausstehend.
- Bekannte Fehlerphase unverändert zurückgestellt.

Neue Tests: texture_disk_cache_tests, texture_distance_tests;
mesh_residency_tests deckt jetzt Size-Weight ab.
Gesamt 32/32 Tests erfolgreich.


## Ausbau alpha.59 – Mesh-RAM-/Disk-Cache · 11.10.2026

Die Phase-9-Assetkette für Meshes lautet: RAM (64 MiB LRU) → Disk
(512 MiB LRU) → authentifizierter GetMesh2/GetMesh-CAPS-Abruf.
Die Caches enthalten dekodierte `MeshGeometry` (Vertices, Indices,
Face-Ranges) und nicht die ursprünglichen LLSD/zlib-Assets.
GetMesh-CAPS-URLs oder Passwörter werden nicht gespeichert.

Der persistente Cache unter
`$XDG_CACHE_HOME/opengenesislink/meshes-v1` oder
`$HOME/.cache/opengenesislink/meshes-v1` verwendet
Magic `OGLMESH1`, Little-Endian, feste Zähl- und Größenlimits
und einen UUID-gebundenen FNV64-Inhaltshash. Die Prüfsumme
ist nicht kryptografisch. Nach Dateigrößen-/Hashfehlern,
Fremd-UUID oder ungültiger Geometrie wird neu heruntergeladen.
Limits: 200.000 Vertices, 1.200.000 Indices, 64 Face-Ranges,
rund 11,2 MiB Geometriedaten je Asset. Symlink-Dateien
werden bei Cache-Lesezugriffen nicht befolgt.

RAM wird durch threadgeschützte echte LRU nach Bytes eviktiert;
Disk durch Last-Write-Time als Access-Näherung. Cachetreffer
bleiben auch nach GPU-Eviction oder Regionswechsel verfügbar.
Bei GPU-Upload gelten weiter die bestehenden Region-Epochen,
aktuelle Residency und 256-Mesh-Slots. Pro Runde werden bis
zu acht entfernte GPU-Meshes freigegeben.

`GetMesh streaming metrics` erfasst erfolgreiche GPU-Uploads
nach Quelle RAM/Disk/Netz, GPU-Evictions und RAM-/GPU-Belegung.
Nicht in die GPU hochgeladene Antworten zählen nicht als
erfolgreicher Netzwerk-GPU-Load.

Tests: `nexverse-mesh-cache-tests` mit Datei-/Hash-Manipulation,
Neustart, UUID-Bindung, Disk-/RAM-LRU, Cache-Fallback und
Fehlerfällen. Gesamt: 33/33 CTests und Linux-GL3.3-Smoke.
Offen: echter authentifizierter NexVerse-Mesh-Sichttest,
LOD-Chunk-/Texture-Mip-Streaming, echte GPU-Bytebudgets,
Asset-Revision/ETag, Rigging und die gesonderte Bugfixphase.


## Ausbau alpha.60 – LOD-Keyspace und Mesh-Residency

**Block A – OpenSim-LOD-Decoder:** High, Medium, Low und Lowest. Gewünschte Stufe zuerst; bei fehlenden/defekten Chunks Fallback auf andere gültige Level. Bestehende LLSD-/zlib-/Mesh-Bounds gelten weiter.

**Block B – Adaptive LOD-Planung:** `plan_mesh_lods` gewichtet Entfernung und Primgröße, dedupliziert mehrfach verwendete Mesh-UUIDs und nutzt Hysterese. Aktualisierung alle etwa 750 ms oder bei Szene-Änderung.

**Block C – LOD-getrennte RAM-/Disk-Caches:** High behält den bisherigen `<uuid>.mesh`-Schlüssel; andere Stufen werden unter `<uuid>.<lod>.mesh` inklusive LOD-gebundener Checksumme gespeichert. Budgets 64 MiB RAM und 512 MiB Disk. Nach Server-Fallback bezeichnet der Cache-Key den angeforderten, nicht zwingend den tatsächlich dekodierten LOD.

**Block D – GPU-Swap und Diagnose:** Mesh-LOD-Upgrades und -Downgrades ohne vorherige Entfernung der Mesh-UUID. GPU lädt zuerst neue Buffers, gibt alte erst nach erfolgreichem Upload frei. Aktuelle Regions-Epoche und Ziel-LOD werden vor Upload geprüft. Neues Log: LOD-Swap-Zähler und geschätzte VBO/EBO-Bytes.

**Verifikation:** C++23-Debug-Build, 34/34 CTests (LOD-Codec/Fallback, Cache-Varianten, Hysterese). OpenGL-Start-Smoke zum Release. Weiter offen: authentifizierter In-World-LOD-Test, GPU-Bytebudget, Texture-Mip-Streaming, Rigging/Animesh und zurückgestellte Avatar-/Bewegungs-/Teleport-Fehler.

Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl60-live.log`. Maßgeblich: `projectplan.md`, Branch `develop-current`.

Die Cache-Budgets gelten für alle LOD-Varianten zusammen. Ein Cachetreffer nach Server-Fallback beweist nur den Wunsch-LOD-Key, nicht den originalen Server-LOD. `gpu_estimated_bytes` sind geschätzte Mesh-VBO/EBO-Bytes und keine tatsächliche VRAM-Messung oder Byte-Grenze. Kein Texture-Mip-Streaming in alpha.60.


## alpha.61 – CPU-Mip-Streaming mit GPU-Texturbytebudget

**Block A – CPU-Mip-Vorstufen:** `texture_mip_image` erzeugt bis zu drei kleinere RGBA8-Basisstufen mit alpha-gewichteter Mittelung (premultiplizierte Farbsummen); NPOT-Bilder werden begrenzt unterstützt. Der Renderer erzeugt darauf weiter gewöhnliche OpenGL-3.3-Mipmapketten. Es wird kein JPEG2000-Progressive- oder HTTP-Chunk-Streaming vorgetäuscht.

**Block B – Distanzabhängige Qualitätswahl:** `plan_texture_mip_levels` wählt 0/1/2/3 je nach Agentabstand (24/64/128 m), mit Hysterese an den Grenzwerten. Mehrfach verwendete Texture-UUIDs verwenden die nächste Objektposition; aktive Partikeltexturen erhalten Vorrang mit Level 0. Die Planung läuft bei Scene-Revision oder spätestens etwa alle 750 ms.

**Block C – Asynchroner Full-Resolution-Cache/Hot-Reload:** HTTP und der bestehende 256-MiB-RGBA8-Disk-Cache speichern ausschließlich Originalbilder. Der asynchrone Worker erstellt daraus die gewünschte CPU-Mipstufe. Ändert sich die Entfernung, kann die vorhandene GPU-Textur erneut hochgeladen und verkleinert oder vergrößert werden, ohne die ursprüngliche GetTexture-Assetdatei erneut laden zu müssen. Region-/Epochenschutz, Residency und veraltete Qualitäts-Requests werden vor dem Upload geprüft.

**Block D – Geplantes 256-MiB-GPU-Texturbytebudget:** Mit `gpu_rgba8_mip_bytes` werden RGBA8-Mipketten berechnet; `plan_texture_byte_budget` kann vor Upload Texturen niedrigerer Priorität zur Eviction bestimmen. Höher priorisierte Texturen werden nicht verdrängt, um weiter entfernte Assets nachzuladen. Kann ein Upload nicht ins Byte-/512-Textur-Slotbudget passen, wird er zurückgestellt statt unkontrolliert zu wachsen. Telemetrie: aktuelle geschätzte Texturbytes, LOD-Swaps, zurückgestellte Uploads und GPU-Evictions.

**Verifikation:** Debug-Build C++23, 35/35 CTests einschließlich RGBA-Alpha, ungerader Dimensionen, Mip-Byteberechnung, Distanzhysterese und Budgetpriorität. OpenGL-3.3-GUI-Smoke nach Versionspflege. Live-In-World-Abnahme mit echtem NexVerse-Login steht aus.

**Abgrenzung:** Die 256 MiB sind ein softwareseitig aus RGBA8-Mipdimensionen berechnetes Texturbudget, keine gemessene oder vollständig durchgesetzte physische VRAM-Gesamtgrenze. Terrain-, UI- und Mesh-GPU-Ressourcen gehören nicht zu diesem Texture-Budget. Bei sehr großen/zahlreichen Bildassets können zusätzliche Inhalte unter Budgetdruck vorerst im Fallback bleiben.

**Offen:** Netzwerk-Chunks und native JPEG2000-Progressive-LOD, effizientes CPU-RAM-Caching für sämtliche Texturlevel, gemeinsames GPU-Bytebudget für Mesh/Terrain/Texture, authentifizierter Streaming-Sichttest; bekannte Avatar-/WASD-/Mesh-Asset-/Teleportfehler bleiben auf Nutzerwunsch für die spätere gesammelte Fehlerbehebung dokumentiert.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl61-live.log`.

Das Budget verhindert anhand der dokumentierten RGBA8-Mipkettenabschätzung die Annahme neuer Texturen über 256 MiB. Durch Treiber-/Allocator-Overhead ist dies keine exakte gesamte VRAM-Nutzung. Für denselben Texture-Key können unter wechselnder Entfernung unterschiedliche Basisresolutionen nacheinander resident werden. `GetTexture`/Disk speichert dennoch ausschließlich vollständige Assets; die GPU-Textur kann ohne Änderung der UUID aktualisiert werden.
