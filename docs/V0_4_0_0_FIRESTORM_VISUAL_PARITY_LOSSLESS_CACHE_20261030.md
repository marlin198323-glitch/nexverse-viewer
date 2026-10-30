# OGL Viewer v0.4.0.0 – Firestorm-Bildvergleich, dauerhaft komprimierter Weltcache, Render-Parität

**30. Oktober 2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Basis `a12e8e37d054e03095e5fd24b25bd9900f31542f`.**

## Tatsächlicher Bildvergleich Firestorm vs. OGL

Zwei echte Screenshots des Nutzers von **22:43:38 (Firestorm)** und **22:54:26 (OGL Viewer)** derselben winterlichen Region zeigen erhebliche Qualitätsunterschiede. Der native Firestorm rendert gepflasterte Gehwege, zahlreiche weit entfernte Bäume und Gebäude, prägnante material- und objektbezogene Schatten, dynamisches Tageslicht mit sichtbarem Himmel, sehr viel detailliertere Fassaden und die weiter entfernte Umwelt. Der OGL Viewer zeigt überwiegend eine großflächig graue Bodenfläche, fehlende/noch nicht gezeichnete Vegetation und wenige Schatten. Die Kamerapositionen und Beleuchtungszeiten sind nicht identisch; die beiden Screenshots erlauben keine exakten Pixel-Prozentwerte. Sie zeigen aber, dass eine hohe Anzahl heruntergeladener Textur-IDs keineswegs Firestorm-Parität beweist.

Die authentifizierten Logs vom **22:54:13 Uhr** zeigen **221 GetTexture-Netzwerkversuche, 216 erfolgreich geladen und 0 Treffer im vorhandenen Diskcache** (27 im CPU-RAM, 45 frühzeitige Prefetch-Versuche ohne Diskhit). Bei 22:54 lagen 196/236 derzeit priorisierte Textur-IDs auf der GPU, davon nur 16 auf vollständigem Mip0 und 180 reduziert. Vier bis fünf Texturassets scheitern am Server HTTP 404. Der OGL-Avatar hat weiterhin **0/3 Head-/Upper-/Lower-Bake-IDs**. Der Screenshot ist damit ein repräsentativer **unvollständiger** Renderer-Stand, nicht der angestrebte Endzustand.

## Bestehender Cache und tatsächliche Ursache

Ein mehrstufiger Cache existiert bereits. `TextureMemoryCache` hält dekodierte Bilder begrenzt im RAM, `TextureDiskCache` speichert bislang komplett unkomprimierte RGBA-Dateien mit eigenem Budget, `MeshDiskCache` hält nach UUID validierte Mesh-Assets über Neustarts. Der entsprechende sichere Workerpfad `load_texture_cached_scoped` prüft vor HTTP die RAM- und Diskcaches, verwirft alle alten Regionsantworten und verwendet **keine** Credentials/CAPS-URLs als Dateinamen. Er bleibt unverändert.

Ein realer read-only Inventurcheck ergab **81 unkomprimierte Texturen = 255,7 MiB** (davon 61 × 1024×1024 RGBA zu 4 MiB) bei bisherigem Defaultbudget **256 MiB**, und **1.722 Meshdateien = 254,5 MiB**. Das zu kleine Budget kann bei Wechsel zu vielen anderen Texturen wertvolle alte Weltassets verdrängen. Cache-Netzwerk-Hits sind daher vor allem beim ersten Öffnen einer neuen Szene gering.

## Zehn abgearbeitete Implementierungs- und Testblöcke

1. Zuerst `CHATUEBERGABE.md`, Roadmap und Git `develop-current` auf Linux Mint geprüft, anschließend die zwei echten Nutzer-Screenshots fachlich analysiert. **Keine Bilder erzeugt und keine Firestorm-Oberflächen nachgezeichnet.**
2. Die aktuellen Anmelde-, Texture-, Mesh-, Terrain-, Shader- und Avatar-Logs zeitlich und mit denselben Szenenobjekten verglichen. `GetTexture streaming metrics`, echte `InWorldAssetSnapshot`- und `scene_gl_...`-Zähler statt visueller Vermutungen verwendet.
3. Das echte Firestorm-Installationsverzeichnis samt persistierendem `~/.firestorm_x64`-Cache mit der bestehenden OGL-Architektur verglichen. Dabei nachgewiesen, dass OGL bereits 515 MiB auf Platte hält, die auf alten RGBA-Daten beruhen.
4. `TextureDiskCache` in **bestehender Datei und bestehendem API-Pfad** um `OGLTEX02` erweitert: verlustfreie zlib-Kompression `Z_BEST_SPEED` für nur tatsächlich platzsparende Texturen, andernfalls altes `OGLTEX01`-Format. Dieselbe validierte Bildgröße (maximal 4096² / 64 MiB RGBA) und FNV64-Prüfsumme bleiben verpflichtend. Schon vorhandene zlib-Dependency `ZLIB::ZLIB` verwendet, keine neuen Binärdateien oder GPU-Herstellerbindung.
5. Komprimierte Leseoperationen gegen Zipbomben geschützt: überprüfte Größe aus 24-Byte-Header, nur passende Header-Magic, präzise `inflate(Z_FINISH)`-Dekodierung und **vollständiger** Verbrauch von Eingangsbytes plus vollständig gefüllter Ausgabe. Ungültige, manipulierte, zusätzliche oder unvollständige Datensätze werden ausschließlich als `corrupt` abgewiesen; nie als valide Asset-Bilder oder schwebende Nutzer-Credentials übernommen.
6. **Abwärtskompatible Migration** auf demselben bestehenden Cachepfad implementiert: Bei erfolgreichem Prüfsummen-Load einer alten `OGLTEX01`-Textur über 4 KiB wird die Datei bei nächster Nutzung durch komprimierte Fassung ersetzt, solange diese kleiner ist. Gespeicherte UUIDs und Pixelwerte bleiben identisch. Fehlgeschlagene Speicheroperationen dürfen die bereits geladene Welttextur nicht unbrauchbar machen. Alte ungenutzte Dateien werden nicht gelöscht.
7. Universellen Default des reinen **Festplatten-Texturcaches** von 256 auf **512 MiB** erhöht; `OGL_TEXTURE_CACHE_MIB` erlaubt **128 bis 2048 MiB**. Ungültige oder zu große Eingaben verwenden sicher 512 MiB. Keine Veränderung an OpenGL VRAM, Shader-/Texture-Decode-/Mip-Auswahl, LLUDP, HTTP-Pacing, Avatar-Konten oder Region-Gates. Der standardmäßige Meshcache bleibt erhalten.
8. Neues schreibfreies Diagnosewerkzeug `scripts/ogl-cache-status.py`: zeigt nur Cache-Budget, Anzahl komprimierter/alter RGBA-Texturen, tatsächliche und ursprüngliche Bytezahl sowie Meshanzahl/-größe; kein Dateiname, Accountname, CAPS-Token oder UUID wird veröffentlicht. Eine kleine read-only Stichprobe von **zwölf echten 1024²-Texturen** ergab für zlib Level 1 eine **Median-Dateigröße von 24,6 %** des unkomprimierten RGBA (Bereich 2,4–48,3 %). **Dies ist keine allgemeine Performance-Garantie**, sondern ein konkreter, ohne Dateiveränderungen gemessener Wert.
9. Native `texture_disk_cache_tests.cpp` um realistische **1024² RGBA**-Bitgleichheit nach Speichern/Neustart, `OGLTEX02`-Magic, echte Kompressionsrate, Live-`OGLTEX01`-Migration, an komprimierte Datei angehängte Fremdbytes sowie Budgetgrenzen (128, 2048, fehlerhafte Eingaben) ergänzt. Zwölf Python-Vertragsprüfungen für Cacheformat, Datei-/Datenintegrität, Worker-Transport, read-only CLI, Sicherheit und Unabhängigkeit vom GPU-Hersteller.
10. Vollständige Linux-Mint-x64-Prüfung `scripts/mint-viewer-validation.sh` einschließlich aller nativen CTests, Python-Tests, original Firestorm-basiertem Xvfb-/OpenGL-/GUI-Test und X11-Systemicon; Dokumentation/Handover/Roadmap und Fortschritt synchronisieren, geprüften Commit auf `develop-current` pushen und GitHub-CI überprüfen. Kein Main-Merge.

## Bedienung und Nachweis

Read-only Cache-Zustand:

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
```

Optionaler kleiner Cache, Beispiel vor dem Viewerstart:

```bash
OGL_TEXTURE_CACHE_MIB=256 ./build/debug/nexverse-viewer
```

Ohne Variable verwendet der Viewer 512 MiB. Es werden **keine** GPU-Budgets damit überschrieben; gespeicherte Texturen bleiben beim Neustart erhalten, solange der Speicherplatz und Cache-LRU sie zulassen. Erst wenn die Weltbilder beim nächsten Login durch die aktuelle Binary wiederbenutzt werden, erscheinen `OGLTEX02`-Dateien und die Anzahl der `disk_hits` steigt. Die 81 bestehenden Dateinachweise werden **nicht unaufgefordert im laufenden Konto-/Gridbetrieb überschrieben oder gelöscht**.

## Ab hier Firestorm-Parität schrittweise, nicht nur Zähler

**P0-A: Tatsächliche Material-/UV-Abbildung** – aktuell sichtbare Floor-/Plaza-/Gebäudeflächen mit denselben TexEntry-/Materialparametern wie Firestorm rendern. Die vorhandene GL-Diagnose kennt `scene_gl_textured_draws` und `scene_gl_requested_texture_fallback_draws`, aber noch keine Bildflächen-/UV-/Materialvollständigkeit. Die wichtigsten flächenmäßig sichtbaren Fallbacks im Screenshot ermitteln, bevor man blind Texturqualität hochstellt.

**P0-B: Texture-Mip-/LOD-Wahl nach Bildschirmpräsenz** – Distanz- und GPU-Budget schon vorhanden; Mip3-Ferne ist für große sichtbare Bodenflächen möglicherweise zu grob. Bevor Mip0 für alle geladen wird, Screen-Space-/Flächenpriorität, VRAM-Budget, Nähe, Hysterese und Hardwareprofil gemeinsam optimieren.

**P0-C: Bäume, Alpha/Vegetation, Licht/Schatten** – Firestorm-Vergleich zeigt fehlende entfernte Bäume, matte Außenflächen, fehlende Schatten und flachen Himmel. GetMesh/Objektselektion, Alpha-Maskierung, Normal-/Specular-/PBR-Materialien, Gelände-Schichten, Tageslicht und Licht-Schatten-Daten mit Firestorm der **gleichen Kamera und Region** vergleichen.

**P0-D: Avatar-Skin/Outfit** – selbst beim erfolgreichen Login fehlen serverseitig weiterhin alle drei notwendigen Body-Bake-Verweise (Head/Upper/Lower) und COF ist unvollständig. Ohne Bakes keine optische Avatar-Parität behaupten.

**Abnahme:** Zwei Viewer der gleichen Region bei möglichst ähnlicher Tageszeit, Position, Ausrichtung, FOV und Bildgröße vergleichen; `InWorldAssetSnapshot` und konkrete Screenshot-Differenzen gegenüberstellen. Die Zahl von heruntergeladenen UUIDs ist keine Freigabe für die Grafikqualität. Der höhere und komprimierte Cache verbessert die **Wiederverwendung und möglicherweise die Startzeit**, aber rendert fehlende Geometrie, UVs, Beleuchtung oder serverseitig fehlende Bakes nicht von selbst.

**Zielversion `v0.4.0.0` IN ARBEIT | Projektfortschritt 38,65 % gewichtet ≈ 39 % | Kein `main`-Merge, kein Release.**
