# OpenGenesisLINK – OpenSim Materials (Phase 8)

## Stand 0.2.0-alpha.42

Die Viewer-Pipeline unterscheidet **OpenSim Legacy RenderMaterials** von
**PBR/glTF RenderMaterials**. Deren UUIDs sind keine Shaderparameter.

### Vorhandener Pfad

1. Das TextureEntry des WorldModel liefert Default- und per-Face-MaterialID
   sowie getrennte RenderMaterialID (unaufgeloester moderner Materialverweis).
2. Der CAPS-Seed wird wie bisher ausgehandelt. Ist `RenderMaterials`
   vorhanden, wird die URL gespeichert und **asynchron** einmal als GET
   abgefragt, nicht auf dem UI-/Render-Thread.
3. Der Parser erwartet eine LLSD-XML-Map `Zipped` als Binary.
   Ein begrenzter zlib-Inflate dekodiert darin LLSD Binary ohne Header
   zu einer Array-Liste von `{ ID: binary-UUID, Material: map }`.
4. Die Legacy-Materialien werden per UUID in einem lokalen Snapshot gespeichert.
   Ein abgeschlossener Fetch erzwingt einen Scene-Upload, sodass Default-Faces
   und Face-Overrides die Legacy-Alpha-Parameter erhalten.
5. OpenGL rendert die Alpha-Modi 0 (keine Texture-Alpha), 1 (Blend),
   2 (Alpha-Mask-Cutoff) sowie 3 (Emissive-Fallback). Für Mask-Faces
   bleibt Depth-Write eingeschaltet, sofern keine Tint-Transparenz vorliegt.

Sicherheit: Limit 4 MiB LLSD-XML, 2 MiB zlib-Daten, 8 MiB entpackte
LLSD-Daten und 8192 Materialeinträge; ungültige Payloads oder einzelne
Materialdatensätze werden nicht als Renderer-Werte akzeptiert.
Fehlt die Capability oder schlägt der Fetch fehl, gelten unverändert
die TextureEntry-Fallbacks. Keine direkten GPU- oder WorldModel-Zugriffe
im CAPS-Parser.

### Noch offen

- Gezielte POST-Batches und Aktualisierung bei später eintreffenden
  MaterialIDs; der GET-Snapshot wird derzeit nur beim ersten Login
  geladen und kann veralten.
- Legacy-Normal-/Specular-Maps, UV-Transformationen je Material-Kanal,
  ausgebaute Lichtberechnung.
- Neue RenderMaterialIDs: PBR/glTF-Fetch und korrektes Asset-Parsing
  sind **nicht** implementiert. Keine Interpretation einer UUID als PBR-Wert.
- Shader-Verifikation in der realen OpenSim-Welt an alpha-maskierten,
  emissiven, transparenten und gestapelten Prim-Faces.
- Vollständig korrektes OIT/Face-Triangle-Sorting ist noch offen.

Implementierung:
`include/nexverse/protocol/render_materials.hpp`,
`src/protocol/render_materials.cpp`,
`tests/render_materials_tests.cpp`,
`src/app/application.cpp`,
`src/render/opengl_renderer.cpp`.

Protokollreferenz (serverseitige OpenSimulator-Implementierung):
https://github.com/opensim/opensim/blob/master/OpenSim/Region/OptionalModules/Materials/MaterialsModule.cs
