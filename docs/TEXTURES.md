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
