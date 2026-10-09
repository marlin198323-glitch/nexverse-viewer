# OpenGenesisLINK – OpenSim Materials (Phase 8)

## Stand 0.2.0-alpha.45

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
5. Bei späteren Scene-Updates werden Legacy-MaterialIDs gesammelt. Unbekannte
   IDs werden nach abgeschlossenem initialem GET in asynchronen POST-Batches
   von maximal 32 UUIDs abgefragt: LLSD-XML `Zipped` mit zlib-komprimiertem
   LLSD-Binary-Array von je 16 UUID-Bytes. Doppelte IDs werden entfernt;
   zurückkommende Materialdefinitionen werden auf angefragte IDs begrenzt.
6. Ist eine nachgeladene Definition anders, wird der Material-Cache ersetzt
   und die Render-Scene neu aufgebaut. Bei unveränderten Definitionen ist
   kein Re-Upload erforderlich. Unbekannte IDs werden höchstens dreimal
   mit einem Abstand von zwei Minuten angefragt; bekannte IDs können
   bei späteren Scene-Updates nach dem Cooldown erneut abgefragt werden.
7. `NormMap` und `SpecMap` werden zusammen mit separaten
   `Offset/Repeat/Rotation`-Werten (OpenSim-Festkommaformat: 1/10000)
   als Scene-Materialdaten gespeichert. Die bestehende GetTexture-PNG-
   Pipeline lädt sie in den GPU-Asset-Cache. OGLRender sampelt beide
   Maps pro Face getrennt: Normalmapping per approximativer TBN-Basis
   und vereinfachte Specular-Beleuchtung mit `SpecColor`, `SpecExp`
   und `EnvIntensity`. Fehlen Maps, gilt der Legacy-Fallback.
8. OpenGL rendert die Alpha-Modi 0 (keine Texture-Alpha), 1 (Blend),
   2 (Alpha-Mask-Cutoff) sowie 3 (Emissive-Fallback). Für Mask-Faces
   bleibt Depth-Write eingeschaltet, sofern keine Tint-Transparenz vorliegt.

Sicherheit: Limit 4 MiB LLSD-XML, 2 MiB zlib-Daten, 8 MiB entpackte
LLSD-Daten und 8192 Materialeinträge; ungültige Payloads oder einzelne
Materialdatensätze werden nicht als Renderer-Werte akzeptiert.
Fehlt die Capability oder schlägt der Fetch fehl, gelten unverändert
die TextureEntry-Fallbacks. POST-Requests werden ausschließlich über
per Seed erhaltene HTTP(S)-Capability-URIs gesendet und außerhalb des
Render-Threads ausgeführt. Keine direkten GPU- oder WorldModel-Zugriffe
im CAPS-Parser.

### Regionswechsel / Anfrage-Epochen (0.2.0-alpha.45)

- Die Material-/Texture-URL wird pro aktivem WorldModel-Region-Seed
  asynchron aus dem Seed-CAPS ermittelt.
- Der Scope-Schlüssel besteht aus Regionshandle, Region-Generation und
  Seed-Capability. Ein Wechsel invalidiert MaterialIDs, Cooldowns,
  ausstehende IDs und die Texture-Download-Metadaten der alten Region.
- Noch laufende HTTP-Anfragen werden nicht synchron unterbrochen
  oder destruktiv ersetzt. Ihre Ergebnisse enthalten eine lokale
  Anfrage-Epoche und werden beim Eintreffen verworfen, falls diese
  nicht mehr zur aktiven Region passt.
- Die bereits vorhandene Login-EventQueue läuft unabhängig. Diese
  Änderung ersetzt noch **keinen vollständigen Teleport-Protokoll-
  Reconnect** mit neuem UDP-Circuit und neuer EventQueue.
- Bereits auf der GPU liegende UUID-Texturen werden hier nicht
  vollständig evicted; ein Ressourcen-/LRU-Pfad folgt.

### Noch offen

- Der initiale GET-Snapshot wird nur beim ersten Login geladen.
  Materialänderungen ohne Scene-Update werden nicht proaktiv erkannt;
  der Region-/Teleport-Lifecycle muss für Cache/Capability-Wechsel
  noch vervollständigt werden.
- Legacy-Normal-/Specular-Maps werden jetzt über GetTexture nachgeladen,
  pro Face mit unabhängigen UV-Transformationen gesampelt und im
  OpenGL-3.3-Shader vereinfacht beleuchtet. Noch offen sind exakte
  Tangentendaten für alle Primformen, zusätzliche Legacy-Lichtanteile,
  tatsächliche Umgebungsspiegelungen und GPU-Sichttests in-world.
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


Legacy-Datenreferenz (Offset, Repeat, Rotation, SpecColor, Exp):
https://github.com/opensim/opensim/blob/master/OpenSim/Region/Framework/Scenes/SOPMaterial.cs

Es handelt sich um **Legacy-Mapping, nicht glTF/PBR**.
Ein visuell verifizierter In-World-Test an Originalmaterialien fehlt.
