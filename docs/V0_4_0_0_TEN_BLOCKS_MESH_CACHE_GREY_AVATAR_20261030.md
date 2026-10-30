# OGL v0.4.0.0 – 10 Blöcke: Mesh-Cache, Streaming, grauer Avatar

## Tatsächlicher Ausgangszustand (30.10.2026, letzte Live-Logs)

- Letzter belegter Welt-Snapshot: 268/280 GPU-residente Texturen (95 %); 12 fehlen; 16/268 als Mip0, 252/268 reduziert. Ein weiterer, gerade begonnener Login war noch ohne voll geladene Welt.
- 1.723 bereits persistierte Mesh-Dateien, 254,6 MiB; 277 verlustfrei komprimierte Texturen, 352 MiB auf Platte.
- Mesh-Downloads trotz Cache bislang seriell (180 ms Dispatcher). GetTexture seit vorherigem Commit maximal vier parallele Downloads.
- Server gibt für manche Texturen und Meshes echte HTTP 404 zurück. Ohne Daten kann weder Cache noch der GPU-Renderer die Bilder reparieren.
- Eigener Avatar: Low158 0/3 Server-Body-Bake-Refs, GPU 0/3; Low382 liefert fünf Wearable-Einträge, kein Nachweis für vollständiges COF / Head-/Upper-/Lower-TextureEntry. Viewer darf keine künstliche Kleidung oder Bake-UUIDs behaupten.
- OpenGenesisLINK-Server-Upstream main zuletzt `1b8fb1c`, unverändert bei dieser Prüfung.

## Die zehn implementierten Blöcke

1. **Diagnose und Vorbedingungen**: GitHub-Handover, Upstream, echte Login-Sitzung, Textur-/Mesh-Missing und Avatar-Graufall inventarisiert.
2. **Mesh-Parallelisierung**: Drei concurrent GetMesh/RAM-/Disk-/HTTP-Futures, je eigene ID, LOD, Regionsepoche und Startzeit.
3. **Cache-Fairness & Dedup**: Laufende UUIDs werden nicht doppelt heruntergeladen; zwei von drei Dispatches bevorzugen bereits disk-/RAM-cachierte Assets, jede dritte berücksichtigt die weltweite Prioritätenliste für neue Assets.
4. **Regions-/GPU-Sicherheit**: Originale GetMesh-Sicherungen (Region, gewünschte LOD, Byte-/VRAM-Budget, GPU-Upload-Gate, Backoff) bleiben auf dem Render-Thread. Alle asynchronen Worker werden beim Beenden verbunden.
5. **Parallel-sicherer Mesh-Diskcache**: Eindeutige Schreibdateien pro Worker und atomare Veröffentlichung statt gemeinsamer `.part`; realer 12-facher nativer Konkurrenztest; Validierung vor GPU.
6. **Größerer Mesh-Diskcache**: Von 512 MiB auf 1024 MiB; `OGL_MESH_CACHE_MIB` erlaubt 128–4096 MiB, ungültige Werte fallen auf 1024 MiB zurück; kein Formatbruch / Verlust der 1.723 alten Cache-Dateien.
7. **Mesh-RAM-Warmcache**: 64 MiB auf 96 MiB erhöht; thread-sicheres UUID+LOD-`contains` und kostengünstige Disk-Metadatenprüfung.
8. **Cache-Status korrigiert**: `scripts/ogl-cache-status.py` zeigt nun tatsächliche 1024 MiB Textur- und Mesh-Disk-Budgets sowie ausschließlich vollständige `.mesh`-Dateien (keine `.part.*`-Zwischenstände).
9. **Grauer Avatar – wahrheitsgemäße Diagnose**: Sichtbares 0/3- bzw. Teil-Bake-UI statt pauschalem "Stream belegt"; read-only `scripts/ogl-asset-diagnose.py` zeigt Session, GPU-Residenz, 404-Ereignisse und Bake-Abdeckung ohne private UUIDs und ohne Web-Anfragen. F9 COF/Outfit wird als Prüfung genannt, nicht als fälschliche Server-Reparatur.
10. **Regressionen & CI-Reparatur**: 12 neue Python-Verträge, native 12-Thread-Mesh-Teststrecke sowie Korrektur der vorherigen Linux-x64-Source-Check-Gate-Fehler (verbindliche 10-Block-Kopfzeile und geänderte Mesh-Future-Shutdown-Namen), Dokumentation in Roadmap und Übergabe.

## Benutzerstart (unverändert)

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach einem echten Login zusätzlich: `python3 scripts/ogl-asset-diagnose.py`.

## Lokal reproduzierbar bestandene Prüfungen

- Linux-Mint-x64 CMake/vcpkg Full Viewer Build: **PASS**.
- Native CTest: **195/195 PASS**, inklusive zwölf zeitgleicher Meshcache-Schreiboperationen und GPU-Prüfungen.
- Python-Unittest: **956/956 PASS**, davon zwölf neue Mesh-/Avatar-/Status-Verträge.
- `bash scripts/pi5-headless-tests.sh`: **PASS**, gleiche Source-Check-Teststrecke wie GitHub, **956/956 Python**, ohne echten Login.
- `bash scripts/test-mint-window-branding.sh`: **X11/Systemicon PASS**, GUI-Start ohne authentifizierte Welt.
- `git diff --check` und maschinenlesbare Release-Roadmap: PASS.

## Nicht als gelöst behaupten

- Kein Nachher-Benchmark mit eingeloggter Region oder sichtbarer Mesh-/Textur-Qualität. Drei gleichzeitige Mesh-Worker sind installiert, ihre reale Beschleunigung ist noch zu messen.
- Fehlende Server-Meshes/Texturen mit HTTP 404 benötigen gültige Asset-Repository-/Simulator-Daten.
- Das grau/nackte Aussehen ist ohne echte Head/Upper/Lower-Bakes weiterhin möglich. Server-Bake-Erzeugung, COF-/Outfit-Zustand und gültige Attachment-Daten müssen separat in einer echten Sitzung nachgewiesen werden. Kein blindes mutierendes AgentSetAppearance.
- v0.4.0.0 bleibt IN ARBEIT, Fortschritt 38,65 % (~39 %), kein Release-/main-Merge.
