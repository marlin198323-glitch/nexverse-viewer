# OpenGenesisLINK – GetTexture / JPEG2000

Stand: 0.2.0-alpha.46 · 10.10.2026

## Anlass und belegte Logs

Im Live-Test 0.2.0-alpha.45 vom 10.10.2026 ab 00:13 Uhr wurden
300 Objekttexturfehler und null erfolgreiche Downloads gemeldet.
Gleichzeitig waren Login, LLUDP, WorldModel READY, RenderMaterials
GET/POST, CAPS und Terrain-/Prim-Geometrie aktiv. Der genaue
HTTP- oder Decoderfehler war in dieser Version nicht sichtbar.

## Abrufvertrag

1. Aktuelle regionsgebundene `GetTexture`-Capability verwenden.
2. `GET <cap>?texture_id=<UUID>&format=png` (vorhandene Query-Parameter
   bleiben erhalten). HTTP-2xx-Bytes als PNG zu RGBA8 dekodieren.
3. Scheitert HTTP oder PNG-Dekodierung, denselben Asset-Identifier
   über `format=x-j2c` abrufen und per OpenJPEG dekodieren.
4. J2C-Codestream (Startmarker FF4F) und JP2-Container
   werden beide akzeptiert. Für nicht unterstützte Daten wird
   die präzise Fehlerursache zurückgegeben.
5. Beide Pfade liefern dieselbe `TextureAssetResult`-Struktur,
   die in den bestehenden OpenGL-Texturcache eingespeist wird.
6. Bei Fehlern werden PNG- und J2C-Fehler, die UUID und die
   verfügbare Fallback-Darstellung geloggt. **Keine Capability-
   Token/URLs oder Authentifizierungsdaten protokollieren.**

## Ressourcenbegrenzungen

- Maximal 32 MiB Download/komprimierte Bilddaten je Abruf
- Maximal 4096 × 4096 Pixel und 64 MiB RGBA8-Ausgabe
- 1–4 Kanäle, 1–16 Bit Präzision, kontrollierter Komponentenzugriff
- Begrenzte Netzwerk-Timeouts, Serialisierung je Objekt-Download
- Pro Regions-Epoche fehlgeschlagene IDs nicht im Renderloop
  immer wieder abfragen; bestehende GPU-Asset-Grenze bleibt aktiv

## Regressionstests

`nexverse-texture-loader-tests` testet PNG-Erfolg,
original J2C- und JP2-Dekodierung, RGBA/Alpha,
PNG 404 → J2C-Erfolg, defektes PNG → J2C-Erfolg,
beide Formate HTTP 404 und fehlerhafte JPEG2000-Bilder.
Testbilder sind als kleine verlustlose Byte-Fixtures eingebettet;
Tests benötigen weder Internet noch OpenSim-Zugang.

## Noch offen

Ein realer In-World-Test auf dem authentifizierten NexVerse-Grid
ist **nach** Installation dieser Version erforderlich.
Der nächste Log-Auszug muss mindestens einen erfolgreichen
`Object texture asset loaded (GetTexture x-j2c fallback succeeded)`
oder eine konkrete PNG-/J2C-Fehlermeldung enthalten.
Vorher darf eine erfolgreiche Behebung sämtlicher Welttexturen
nicht behauptet werden.

JPEG2000-LOD/Streaming, permanenter Asset-Diskcache,
Cache-Eviction, Texturpriorisierung und vollständiger
Teleport-/CAPS-/UDP-Reconnect werden separat entwickelt.


## alpha.61 – CPU-Mip-Abstufung nach Entfernung

**Block A – CPU-Mip-Vorstufen:** `texture_mip_image` erzeugt bis zu drei kleinere RGBA8-Basisstufen mit alpha-gewichteter Mittelung (premultiplizierte Farbsummen); NPOT-Bilder werden begrenzt unterstützt. Der Renderer erzeugt darauf weiter gewöhnliche OpenGL-3.3-Mipmapketten. Es wird kein JPEG2000-Progressive- oder HTTP-Chunk-Streaming vorgetäuscht.

**Block B – Distanzabhängige Qualitätswahl:** `plan_texture_mip_levels` wählt 0/1/2/3 je nach Agentabstand (24/64/128 m), mit Hysterese an den Grenzwerten. Mehrfach verwendete Texture-UUIDs verwenden die nächste Objektposition; aktive Partikeltexturen erhalten Vorrang mit Level 0. Die Planung läuft bei Scene-Revision oder spätestens etwa alle 750 ms.

**Block C – Asynchroner Full-Resolution-Cache/Hot-Reload:** HTTP und der bestehende 256-MiB-RGBA8-Disk-Cache speichern ausschließlich Originalbilder. Der asynchrone Worker erstellt daraus die gewünschte CPU-Mipstufe. Ändert sich die Entfernung, kann die vorhandene GPU-Textur erneut hochgeladen und verkleinert oder vergrößert werden, ohne die ursprüngliche GetTexture-Assetdatei erneut laden zu müssen. Region-/Epochenschutz, Residency und veraltete Qualitäts-Requests werden vor dem Upload geprüft.

**Block D – Geplantes 256-MiB-GPU-Texturbytebudget:** Mit `gpu_rgba8_mip_bytes` werden RGBA8-Mipketten berechnet; `plan_texture_byte_budget` kann vor Upload Texturen niedrigerer Priorität zur Eviction bestimmen. Höher priorisierte Texturen werden nicht verdrängt, um weiter entfernte Assets nachzuladen. Kann ein Upload nicht ins Byte-/512-Textur-Slotbudget passen, wird er zurückgestellt statt unkontrolliert zu wachsen. Telemetrie: aktuelle geschätzte Texturbytes, LOD-Swaps, zurückgestellte Uploads und GPU-Evictions.

**Verifikation:** Debug-Build C++23, 35/35 CTests einschließlich RGBA-Alpha, ungerader Dimensionen, Mip-Byteberechnung, Distanzhysterese und Budgetpriorität. OpenGL-3.3-GUI-Smoke nach Versionspflege. Live-In-World-Abnahme mit echtem NexVerse-Login steht aus.

**Abgrenzung:** Die 256 MiB sind ein softwareseitig aus RGBA8-Mipdimensionen berechnetes Texturbudget, keine gemessene oder vollständig durchgesetzte physische VRAM-Gesamtgrenze. Terrain-, UI- und Mesh-GPU-Ressourcen gehören nicht zu diesem Texture-Budget. Bei sehr großen/zahlreichen Bildassets können zusätzliche Inhalte unter Budgetdruck vorerst im Fallback bleiben.

**Offen:** Netzwerk-Chunks und native JPEG2000-Progressive-LOD, effizientes CPU-RAM-Caching für sämtliche Texturlevel, gemeinsames GPU-Bytebudget für Mesh/Terrain/Texture, authentifizierter Streaming-Sichttest; bekannte Avatar-/WASD-/Mesh-Asset-/Teleportfehler bleiben auf Nutzerwunsch für die spätere gesammelte Fehlerbehebung dokumentiert.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl61-live.log`.

Diese Erweiterung verändert nicht `GetTexture`-CAPS oder JPEG2000-Decoder: PNG/J2C werden unverändert als vollständiges RGBA8 dekodiert. Die Mip-Reduktion erfolgt danach im Hintergrundworker aus den vollständigen (gegebenenfalls persistent gecachten) Pixeldaten; Terraintexturen sind zunächst von dieser neuen Objekttextur-Budgetsteuerung ausgenommen.
