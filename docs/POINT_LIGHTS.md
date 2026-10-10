# OpenGenesisLINK – OpenSim lokale Point-Lights

Stand: 0.2.0-alpha.56 · 10.10.2026
Projektplan: Phase 8 (Prims/Scene Objects, Lichtquellen).

## Protokoll und Daten

OpenSim-Prims können ExtraParams des Typs 0x20
(LightData) tragen. Das Format folgt
OpenMetaverse Primitive.LightData:

- RGB: drei UNORM8-Werte
- Alpha: UNORM8-Intensität
- Float32 LE Radius, Cutoff und Falloff (je 4 Byte)
- insgesamt 16 Byte pro LightData

Sculpt/Mesh-ExtraParams 0x30 können im selben Objekt
vorhanden sein. Beide Blocktypen werden separat gesucht.
Malformed LightData wird abgelehnt; der Prim bleibt erhalten.
ObjectUpdate- und ObjectUpdateCompressed-Parser reichen
die validierten Lichtdaten in das WorldModel durch.
Terse-/unvollständige Updates bewahren vorhandene Werte.

## Grafik

Der Legacy-OpenGL-Renderer verwendet bis zu acht
Punktlichter in Avatar-Nähe (Distanzpriorität).
Die Lichtposition ergibt sich aus der Prim-Position,
bei Kindprims angenähert mit Parent+Offset.
Die Shader berechnen pro Fragment Weltposition,
diffuse Lampert-Beleuchtung und radialen Abfall
auf Basis Radius/Intensität/Falloff.
Fullbright-/unbeleuchtete Flächen sind ausgenommen.

**Bewusst eingeschränkt:** keine Schatten und keine
PBR-Materialien, keine Projektorlichter, keine
Beleuchtung von Terrain, Wasser oder Avataren,
noch keine exakte Parent-Rotation für Linksets.
Der Cutoff-Wert wird seit alpha.56 zusätzlich als
weiche Kern-zu-Rand-Abnahme **approximiert**. Außerdem
gibt es auf glänzenden Oberflächen ein kleines
Blinn-Phong-Highlight. Beide Effekte sind nicht
physikalisch kalibriert.
Diese Implementierung ist keine Zusage vollständiger
Firestorm-Lichtkompatibilität.

## Tests

- OpenSim LightData-Regressionstest in
  tests/viewer_messages_tests.cpp prüft Farbkanäle,
  Intensität, Radius, Cutoff, Falloff, Truncation
  und nicht-finite Radiuswerte.
- CMake-CTest und OpenGL-Starttest.
- Authentifizierter In-World-Test mit aktivierter
  OpenSim-Lichtquelle bleibt offen.

## Spätere Fehlerbehebung

Der Nutzer hat entschieden, zunächst den Funktionsplan
fortzusetzen. Die Avatar-/Bewegungssynchronisation
(alpha.52) und fehlende Mesh-/Texture-Assets
werden als bekannte Fehler separat behandelt.

Referenz:
https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/Primitives/Primitive.cs


## Erweiterung alpha.56

Wenige lokale Punktlichter können den Kontrast von
glänzenden Prims/Meshes erhöhen. Der Viewer überträgt
neben Radius/Falloff nun auch Cutoff an den Fragment-
Shader und wertet spekulare Glanzanteile abhängig vom
Legacy-Shiny-Wert aus. Fullbright bleibt unverändert.
Keine Schatten/Projektion/PBR, In-World-Abnahme offen.
