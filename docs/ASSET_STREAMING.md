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
