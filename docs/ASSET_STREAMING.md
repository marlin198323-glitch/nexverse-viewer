# OpenGenesisLINK – GPU-Textur-Streaming und Residency

Stand 0.2.0-alpha.57 · Phase 9, Basisblock
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
