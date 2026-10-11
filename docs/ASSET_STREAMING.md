# OpenGenesisLINK – GPU-Textur-Streaming und Residency

Stand 0.2.0-alpha.58 · Phase 9, Basisblock
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
