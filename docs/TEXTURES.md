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


## alpha.62 – RAM-Texturcache und Sichtkegel-Streaming

**Block A – Kameraabhängige Asset-Priorität:** Ein weicher Blickrichtungsfaktor gewichtet Textur- und Mesh-Asset-Anfragen relativ zur Third-Person-Kamera. Objekte hinter dem Betrachter werden nicht ausgeblendet; sehr nahe Objekte bleiben grundsätzlich hoch priorisiert. Die bestehenden Entfernungssortierungen und GPU-Residency-Grenzen bleiben erhalten. Das ist keine echte Frustum- oder Occlusion-Culling-Implementierung.

**Block B – CPU-RAM-Texturcache:** Ein threadgeschützter 96-MiB-LRU-Cache hält vollständige dekodierte RGBA8-Bilder pro gültiger UUID vor. Der asynchrone GetTexture-Pfad arbeitet RAM → persistenter Diskcache → HTTP/Decoder. Für kleinere GPU-Mip-Basisbilder wird das dekodierte Original aus dem RAM wiederverwendet. Keine Cache-Speicherung von CAPS-URLs, Tokens oder ungültigen Bildern.

**Block C – gemeinsam begrenzte GPU-Mesh-/Objekttexturbytes:** Geschätztes, softwareseitiges gemeinsames Budget von 384 MiB mit zusätzlich jeweils 256 MiB als Maximalgrenze für Mesh-Buffer und Objekttexturen. Vor GPU-Upload werden bestehende Ressourcen und ersetzte LOD-Varianten eingerechnet. Bei Überschreitung wird ein Mesh-Upload zurückgestellt; Texturen können zuerst niedrig priorisierte Texturen verdrängen. Bei freiwerdendem Speicher werden zurückgestellte Anfragen wieder berücksichtigt. Dies ist ausdrücklich keine vollständige VRAM-Bilanz: Terrain, UI, Treiber-Overhead und weitere Ressourcen sind nicht enthalten.

**Block D – Streaming-Diagnose und Regressionen:** 60-s-Metriken für RAM-Treffer, RAM-Größe/Einträge, GPU-Mesh-/Texturbytes, gemeinsames geschätztes Budget und Deferred Counts. Durchschnittliche Zeit vom Asset-Worker-Start bis zur Verarbeitung der fertigen Future (keine reine Netzwerklatenz). Neue Tests für Kamera-Priorität, sicheren gemeinsamen Budget-Rechenweg sowie den parallelen Textur-RAM-LRU-Cache.

**Verifikation:** C++23-Debug-Build und 38/38 automatisierte CTests erfolgreich. OpenGL-3.3-Welcome-Starttest nach Versionspflege ausführen. Kein bestätigter In-World-Test des NexVerse-Asset-Streamings.

**Bewusst offen:** echte Frustum-/Occlusion-Abfrage, globale inklusive Terrain/UI gemessene VRAM-Budgetierung, inkrementeller J2C-Netzstream, Avatar-Skeleton/Bakes, Bewegungs-Synchronisierung und vollständige Teleport-/Circuit-Behandlung. Bekannte Mesh-/Texture-Serverfehler bleiben in der späteren Fehlerbehebungsphase.

**Nächste 3–4 Blöcke:** Asset-Streaming-Queue/Prioritätsregeln, planbares Prefetching mit Limits, Cache-/Regions-Invalidierung und zusätzliche Mess- und In-World-Integrationsdiagnostik.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl62-live.log`. Verbindlicher GitHub-Handoff-Branch: `develop-current`.

`TextureMemoryCache` ist ein 96-MiB-C++-LRU-Cache mit Mutex und kanonischen UUIDs. Cacheinhalt ist das vollständige RGBA8-Bild (nicht die GPU-Mip-Vorstufe). Reihenfolge: RAM-Treffer, dann persistentes `textures-v1`, schließlich GetTexture. Die Kamera-Priorität beeinflusst die Auswahlreihenfolge, ohne fehlende Bilddaten oder Materialdefaults zu erfinden.
