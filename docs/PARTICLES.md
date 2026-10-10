# OpenGenesisLINK Viewer – OpenSim-Partikelsystem

Stand: 0.2.0-alpha.54 · 10.10.2026
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
in einem Drawcall. Alpha-Blending, Tiefentest
und inaktive Tiefenschreibungen halten die Partikel
auf Grundlage der 3D-Welt.

Dies ist ein gezielt begrenzter erster Software-
Emitter: zeitbasierte stateless Reproduktion statt
pro Partikel persistenter Physiksimulation. Damit sind
die Parameter und die Shaderpipeline prüfbar, aber
Wind, Kollisions-/Bounce-Reaktionen, Ribbon, Beam,
TargetPos, ObjectRelative, exakte Winkelregeln,
Textur-UV/Sampling, Script-Memory, laufende
Start/Stop-Emitter-Lifetimes und Blend-Modi
noch nicht vollständig implementiert.

Die Texture UUID wird für spätere Asset-Nachladung
mitgeführt. Kein tatsächliches LSL-Particle-
Asset wird durch diese Version vorab geladen.

## Regressionsabnahme

- 86-Byte-Legacy-Block (Position/Scale/Farbe/Rate)
- Extended-PSBlock mit Glow/Blend
- Abschneiden und ungültige Größen ablehnen
- PSBlock in Full ObjectUpdate
- 86-Byte-PSBlock in ObjectUpdateCompressed
- 26/26 CTests grün
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
