# OpenGenesisLINK Viewer – OpenSim-Partikelsystem

Stand: 0.2.0-alpha.57 · 10.10.2026
Verbindlicher Plan: projectplan.md, Phase 8.

## Wire-Format

Referenz: OpenMetaverse Primitive.ParticleSystem
https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/Primitives/ParticleSystem.cs

Das Standard-PSBlock enthält 86 Bytes:
68 Systemdaten und 18 ParticleData.
Die erweiterte Form umfasst 94–98 Bytes mit separaten
4-Byte-System-/Data-Längenfeldern und optionalen
Glow-/Blend-Daten. Die Leselogik begrenzt Bytezahlen,
prüft beidseitige Größen und lehnt inkonsistente
Payloads ab. Kein Block ist kein Partikelemitter;
ungültiger Block verhindert nicht die Dekodierung
des übrigen Prims.

ObjectUpdate: PSBlock Variable1 zwischen MediaURL und
ExtraParams. ObjectUpdateCompressed: bei CompressedFlags
Bit 0x08 folgt der 86-Byte-Legacy-Partikelblock
hinter dem ggf. vorhandenen MediaURL/Text/Parent-Block.
Die Daten werden in ObjectState gespeichert.

## Erstes Rendering

Für den Start werden 32 Emitter nach Avatar-Nähe
und maximal 192m Entfernung ausgewählt. Ein separater
GL 3.3 Programmpfad erzeugt bis zu 768 Soft-Sprites
mit positions-/lebensdauerabhängiger Größe und Farbe
in bis zu einem Drawcall pro verwendetem Textur-Asset und\nFallback-Batch. Alpha-Blending, Tiefentest
und inaktive Tiefenschreibungen halten die Partikel
auf Grundlage der 3D-Welt.

Dies ist ein gezielt begrenzter erster Software-
Emitter: zeitbasierte stateless Reproduktion statt
pro Partikel persistenter Physiksimulation. Damit sind
die Parameter und die Shaderpipeline prüfbar, aber
Wind, Kollisions-/Bounce-Reaktionen, Ribbon, Beam,
TargetPos, ObjectRelative, exakte Winkelregeln,
präzise Textur-Ausrichtung, Script-Memory, laufende
Start/Stop-Emitter-Lifetimes und Blend-Modi
noch nicht vollständig implementiert.

Seit alpha.55 wird die Texture-UUID sichtbarer Emitter über die
bereits existierende GetTexture-Capability geladen. Bereits vorhandene
Objekttexturen werden ohne zweiten Download direkt als OpenGL-Textur
wiederverwendet. Die Partikel werden im Shader mit echter RGBA-Textur
multipliziert; falls der Download nicht gelingt, bleibt ein weicher
kreisförmiger Ersatzpartikel sichtbar. Die Auswahl bevorzugt
Partikeltexturen vor regulären Objekttexturen und berücksichtigt das
gemeinsame 512-Texturen-Limit. Es erfolgt noch keine transparente
Partikel-Tiefensortierung oder GPU-Instancing.

## Regressionsabnahme

- 86-Byte-Legacy-Block (Position/Scale/Farbe/Rate)
- Extended-PSBlock mit Glow/Blend
- Abschneiden und ungültige Größen ablehnen
- PSBlock in Full ObjectUpdate
- 86-Byte-PSBlock in ObjectUpdateCompressed
- 27/27 CTests grün; neuer GetTexture-Prioritätstest
- OpenGL-Starttest auf NVIDIA RTX 4070 SUPER grün
- echter NexVerse-In-World-Partikeltest offen

Hinweis: Bekannte Alpha.52-Probleme bei Avatar/WASD und
fehlende Assets werden auf ausdrücklichen Nutzerwunsch
nicht im aktuellen Featureblock behoben.


## 0.2.0-alpha.54 – Phase 8: OpenSim ParticleSystem/PSBlock · 10.10.2026

**Nutzerentscheidung:** Nach projectplan.md die Funktionalität
weiterentwickeln und bekannte Fehler erst später beheben.
Die Priorität bleibt Phase 8 vor neuen großen Folgephasen.

**Geliefert:**
- OpenSim ObjectUpdate PSBlock (Variable1 nach MediaURL) und
  ObjectUpdateCompressed Partikelblock (CompressedFlag 0x08,
  86 Bytes) werden abgegriffen und mit strikter Größenprüfung
  dekodiert; ungültige/fehlende Blöcke blockieren den Prim nicht.
- Protokollkompatibles Legacy-ParticleSystem mit 68-Byte-
  Systemteil und 18-Byte-Partikeldaten (86 Byte) sowie
  erweitertem Format 94–98 Byte mit Größenfeldern,
  Glow-/Blend-Metadaten; Bytes nicht aus C# kopiert, sondern
  eigenständiger C++23-Parser.
- Emissionsparameter: Source/Particle-Lifetime, Burst-Rate,
  BurstCount, BurstRadius, Min/Max-Speed, Beschleunigung,
  Start/End-Farbe, Scale, Textur-UUID (nur Referenz)
  sowie Pattern und Flags.
- Partikelsystem liegt im WorldModel der jeweiligen Prim;
  Terse-/Legacy-Partial-Updates bewahren den bekannten Wert,
  KillObject entfernt den Prim mitsamt Emitter.
- Render-Integration: maximal 32 nahe Emitter in ca. 192m
  Radius, maximal 768 Partikel in einem Drawcall; eigener
  OpenGL-3.3-Shader und Streaming-VBO. Weiche runde Sprites
  mit alphablendfähigem Fade, Z-Test ohne Z-Writes.
  Die Positionen sind deterministisch generierte Grund-
  Partikelpositionen mit Ballistik/Beschleunigung.
- Unterstützte Basispfade: Drop, Explode und grob nach
  oben gerichtete Angle-/Cone-Patterns; keine native
  Target-/Ribbon-/Beam-Logik, Wind, Bounce, Rotationsflags,
  korrekte Emitter-Restart-Events, Textur-Sampling oder
  diskrete persistente Partikel-Physik.
- Smoke-Test: Linux 3440x1440, NVIDIA RTX 4070 SUPER,
  GL 3.3, Welcome-UI lädt, kein Shaderfehler gemeldet.
- CMake-Debug Build und 26/26 CTests; Parser-Regressions-
  tests für Legacy und Extended ParticleSystem,
  invaliden Datenblock, ObjectUpdate und
  ObjectUpdateCompressed mit aktivem PSBlock.
- **Die erste echte In-World-Sichtprüfung ist offen.**
  Simulierte Sprites sind kein fertiger Firestorm-
  Partikelrenderer. Keine Änderungen an der
  bekannten Avatar-/Bewegungssynchronisationslücke.

**Spätere Ausbaustufen im Plan:** Partikeltexturen über
AssetCache, Ziel-Partikel, Ribbon/Beam, Wind/Bounce,
lokal zustandsbehaftete LSL-Emission, Partikel-LOD
und GPU-Instancing. Danach Phase 8 weitere
Material-/Licht-Effekte und Phase 9 Asset-Streaming.

Live-Start: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl54-live.log


## 0.2.0-alpha.55 – Phase 8: Partikeltexturen / GetTexture · 10.10.2026

**Verbindlicher Kurs:** Umsetzung der Funktionen aus projectplan.md
fortsetzen. Die bekannten Avatar-/WASD-/Mesh-/Texturfehler bleiben
auf Nutzerwunsch für die spätere Fehlerphase zurückgestellt.

**Neu:**
- Die in OpenSim ParticleSystem/PSBlock enthaltene Particle
  Texture-UUID wird nicht mehr nur gespeichert: Für sichtbare
  Emitter (maximal 32 im Umkreis) wird die Textur im vorhandenen
  GetTexture-Asset-Loader vor regulären Hintergrund-Objekttexturen
  angefordert und als RGBA8/ggf. JPEG2000 dekodiert.
- Emittierte Bilder teilen denselben 512-Assets-GPU-Cache mit
  Object-TextureEntry; gleiche UUID wird nur einmal angefragt
  und kann für Mesh/Prim und Partikel gleichzeitig genutzt werden.
  Die bereits bekannte Epochen-Prüfung verhindert das Hochladen
  veralteter GetTexture-Replies aus früheren Regionen.
- Neuer Pure-Logic Asset-Priorisierer prüft null/Zero-UUIDs,
  doppelte IDs, bereits geladene und zuvor fehlgeschlagene
  Requests. Bei Bedarf wird auf Objekttexturen zurückgegriffen.
  Eigener automatischer Regressionstest.
- Der Particle-Fragmentshader übernimmt echte Textur-RGBA
  über GL_TEXTURE4 (getrennt nach Textur-UUID gebatcht),
  verrechnet sie mit Start-/End-Farbe und Soft-Mask-Alpha.
  Bei nicht geladenem Asset nutzt er weiterhin die zuvor
  existierende weiche Kreis-Sprite-Darstellung.
- Maximal 768 Partikel/Frame, höchstens 32 Emitter; künftig
  statt eines einzelnen bis zu einem Drawcall je
  verwendeter Partikeltextur plus optional Fallback-Batch.
  Keine eigenen GL-Texturduplikate, kein weiteres HTTP-Subsystem.
- Neue Tests fuer priorisierte Assetwahl, gemeinsame Cache-
  Nutzung, ungültige IDs, geladene und fehlgeschlagene Texturen.
  **27/27 CTests**, CMake-Debug-Build sowie OpenGL-GUI-Smoke
  auf NVIDIA RTX 4070 SUPER ohne Shaderfehler.
- Echte In-World-Sichtabnahme steht noch aus; das vorhandene
  Cachelimit von 512 Assets, transparenzabhängige Sortierung,
  Partikeltextur-Orientierung, diskrete Simulation und
  exakte OpenSim-Partikelmodi bleiben spätere Schritte.
- Gesammelte Avatar-/Bewegungs-/Mesh-Fixphase ausdrücklich
  nicht vorgezogen.

Weitere Planblöcke: Wind-/Bounce-/Target-/Ribbon-Partikel,
Effekt-Lebenszyklen/Blending und komplexere Materialbeleuchtung,
danach Phase 9 Asset-Streaming und übrige Viewerfunktionen.
Protokolldetails: docs/PARTICLES.md.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl55-live.log


## alpha.56 – Vier technische Ausbaublöcke

1. **Wind/Bounce/Partikelphysik:** Die Partikel bewegen sich
   in einem fix gestuften lokalen Modell. Die Beschleunigung
   aus dem PSBlock wirkt auf die Trajektorie, Wind/Bounce
   nur bei gesetzten OpenSim-Flags. Wind kommt derzeit aus
   einem langsamen synthetischen Feld, nicht vom Simulator.
   Bounce findet an der Quelle-Z-Höhe statt, nicht am Boden.
2. **Target, Beam, Ribbon:** TargetUUID aus PSBlock bytes
   52..67; Zielposition bekannter Prims/Avatare wird über
   das WorldModel aufgelöst. TargetPos/TargetLinear führen
   Partikel zum Ziel; Beam erzeugt eine räumliche Punktfolge;
   Ribbon eine zeitlich versetzte Spur aus Sprites. Beides
   sind Zwischenstufen, kein durchgehendes Mesh-Ribbon.
3. **Blend/Sichtung:** 0..9 OpenMetaverse BlendFunc-Werte
   mit Standard SrcAlpha=7 / OneMinusSrcAlpha=9.
   Acht distanzsortierte Bänder mit Textur-/Blend-Batches,
   bis zu 768 Partikel und 192 Batches. Innerhalb eines
   Tiefenbands keine garantierte exakte Tiefensortierung.
4. **Beleuchtung:** Legacy-Point-Light-Cutoff und
   einfache Glanzlichter im Objektshader erweitert.
   Kein Photorealismus/PBR oder echter Light-Projector.

Tests: tests/particle_motion_tests.cpp (Kinematik),
tests/viewer_messages_tests.cpp (TargetUUID/Blend-Enums).
Build 28/28 CTests, OGL 3.3 NVIDIA-Start erfolgreich.
Authentifizierter In-World-Partikeltest weiter offen.


## 0.2.0-alpha.57 – Großrelease: vier zusammenhängende Funktionsblöcke · 10.10.2026

**Block 1 – Emitter-Lebenszyklus / ObjectRelative**
- Emitterstart auf Renderer-Zeitbasis (nicht mehr globaler Partikelphase).
  CRC-Wechsel startet ein emittierendes Prim neu; entfernte Emitter
  werden aus dem Laufzeitstatus entfernt.
- SourceMaxAge stoppt neue Bursts, lässt bereits gestartete Partikel
  innerhalb ihrer eigenen PartMaxAge-Zeit auslaufen.
- Rotationskompatibilität: bei OpenSim SourceFlags ObjectRelative
  (Bit 0) werden die Quellbeschleunigung und Initialgeschwindigkeit
  durch die Objekt-Quaternion transformiert.
- Offene Details: vollständige LSL-Start/Stop- und SimTime-Synchronisation,
  Quelle mit ParentRotation, Simulator-Wind statt synthetischem Wind.

**Block 2 – durchgehende Beam-/Ribbon-Geometrie**
- Neuer Kameraseiten-ausgerichteter Ribbon-Generator mit
  verbundenen Dreiecken statt einer bloßen Punktfolge.
  Separater OpenGL-3.3-Triangle-Shader/-VAO/-VBO, UVs,
  transparente Soft-Edges sowie geteilte GetTexture-Assets.
- Beam folgt dem bekannten Zielpunkt; Ribbons folgen
  einer auf Basis der Partikelkinematik erzeugten Spur.
  Begrenzung 32 Emitter/je 16 Samples und 15 Segmente.
- Offene Details: echte bewegungsabhängige kontinuierliche
  Emitter-Historie, native Ribbon-Rotation und robuste
  Transparenzsortierung aller überlappenden Flächen.

**Block 3 – Partikelmaterial / Glow**
- In Extended PSBlock vorhandener Start/EndGlow wird an den
  GPU-Renderer durchgegeben und für Sprites und Ribbon-Material
  als begrenzte RGB-Aufhellung berücksichtigt; nicht als
  echtes physikalisches HDR- oder PBR-Emissive ausgegeben.
- OpenSim-Blend-Enums 0..9 aus alpha.56 bleiben aktiv.

**Block 4 – Phase 9: wiederverwendbarer Texture-Asset-Cache**
- Frühere harte 512-GPU-Texturen-Grenze wird durch ein
  dynamisches Residency-Budget ersetzt; unbenutzte Texturen
  werden als GL-Texturen freigegeben, höchstens 8 pro
  Planungsrunde (750ms).
- Reihenfolge: nahe Partikelemitter, anschließend Prim-/
  Meshmaterialien; letztere erhalten eine Objekt-Distanz-
  priorisierte Ordnung beim Aufbau der Scene-Revision.
- Neue Region leert den GPU-Objekttexturcache; veraltete
  HTTP-Replies außerhalb von Region/Epoche/Residency werden
  verworfen. Gemeinsames GetTexture-Ladesystem bleibt.
- Grenzen: noch kein persistentes CPU-/Disk-Texture-Cache,
  keine Texture-LOD-/Mip-Streaming-Stufen, keine Occlusion,
  UI/Textur-Distanz-Neusortierung nur bei Scene-Update;
  beim Zurückkehren eviktierter Texturen erneut HTTP möglich.

**Tests:** 30/30 CTests inklusive Lifecycle, Quaternion,
Ribbon-Triangles, Cache-Umschichtung. C++23-Debug-Build
und OpenGL-3.3-Starttest auf NVIDIA RTX 4070 SUPER
ohne gemeldeten Shaderfehler. Der 7-Sekunden-Exit 124
ist der absichtliche Timeout, kein Crash.
**NexVerse In-World-Darstellung bleibt ungeprüft.**

**Auf Nutzerwunsch zurückgestellt:** Avatar-LocalID/
serverbestätigte Bewegung, echte Avatar-Appearance,
defekte/missing Mesh-/Texture-Assets und Teleport.
Kein dieser Fehlerbereiche wird als behoben ausgegeben.

Nächster Großrelease-Block: Asset-Caching-Disk/CPU,
LOD/Prioritätsstufen und weitere OpenSim-Renderer-
Kompatibilität. projectplan.md bleibt verbindlich.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl57-live.log
