# OpenGenesisLINK – OpenSim-Meshassets (Phase 8/9)

Stand 0.2.0-alpha.48 · 10.10.2026

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
- Ein Asset wird genau einmal pro Region angefragt. Der Cache
  darf höchstens 256 verschiedene Meshes enthalten. Ältere
  regionale HTTP-Replies werden anhand der Generation/Epoche
  verworfen.

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
