# NexVerse Viewer – Projektplan

Stand: 09.10.2026
Repository: `marlin198323-glitch/nexverse-viewer`
Ziel: Eigenständiger, plattformübergreifender OpenSim-Viewer in C++23 unter MPL 2.0.

---

## 1. Zielbild

Der NexVerse Viewer wird kein Firestorm-Fork. Er wird als eigenständiger Viewer neu entwickelt und kommuniziert direkt mit OpenSimulator über das OpenSim/Linden-Viewer-Protokoll.

Die Architektur wird strikt getrennt:

```text
OpenSimulator
      |
      v
OpenSim Protocol Layer
      |
      v
WorldModel
      |
   +--+---+
   |      |
Renderer  UI
```

Dadurch bleiben Netzwerk, Weltzustand, Darstellung und Benutzeroberfläche voneinander entkoppelt.

Technische Basis:

- Sprache: C++23
- Buildsystem: CMake
- Paketverwaltung: vcpkg
- Lizenz: MPL 2.0
- Zielplattformen:
  - Windows x64
  - Linux x64
  - macOS x64
  - macOS ARM64 / Apple Silicon
- Netzwerk:
  - HTTPS / HTTP
  - OpenSim Login
  - LLSD
  - LLUDP
  - CAPS
- Renderer:
  - zunächst abstrahierte Render-Schnittstelle
  - erste lauffähige Implementierung voraussichtlich OpenGL/Vulkan-kompatibel
  - spätere native Backends ohne Änderung des Protocol-Core möglich

---

## 2. Grundregeln der Entwicklung

1. Der OpenSimulator-Server ist autoritativ.
2. Netzwerkcode darf niemals direkt Renderer-Objekte verändern.
3. Der Renderer darf keine OpenSim-Protokolllogik kennen.
4. Der UI-Code darf nicht direkt LLUDP-Pakete verarbeiten.
5. Jede Netzwerkantwort wird zuerst in typisierte Datenstrukturen übersetzt.
6. Das WorldModel bildet den aktuellen Zustand der Welt ab.
7. Speicherverbrauch wird von Anfang an gemessen und begrenzt.
8. Externe Daten werden validiert und mit Größenlimits verarbeitet.
9. Passwörter werden niemals als Kommandozeilenargumente oder in Logs gespeichert.
10. Neue Module erhalten Tests, bevor die nächste große Schicht darauf aufgebaut wird.
11. Die Viewer-Version wird mit jedem abgeschlossenen Entwicklungsblock fortgeschrieben und niemals künstlich als Release markiert.

### Verbindliche Versionsstrategie

- `0.1.x-alpha`: Protokoll-/Login-/LLUDP-/CAPS-Grundlagen
- `0.2.x-alpha`: WorldModel, Scene-Grundlagen und erste echte Weltdaten
- spätere `0.x`-Alpha-Stände: weitere Funktionsphasen bis zur vollständigen Viewer-Baseline
- `1.0.0-beta.N`: Feature-Complete-Testphase der definierten Release-Baseline
- `1.0.0-rc.N`: Release-Candidate nach bestandenen Plattform-, Grid- und Regressionstests
- `1.0.0`: erste stabile Release-Version

Die konkrete Versionsnummer in `VERSION` wird bei jedem getesteten Entwicklungsblock aktualisiert. Eine stabile Release-Version darf erst gesetzt werden, wenn die für den Release definierten Phasen und Tests vollständig abgeschlossen sind.

---

## 2.1 Firestorm-Funktionsbaseline – verbindlicher Mindestumfang

OpenGenesisLINK Viewer bleibt eine eigenständige Neuimplementierung und wird kein Firestorm-Fork. Funktional soll der Viewer für den normalen NexVerse-Betrieb jedoch mindestens die üblichen Kernfunktionen eines modernen Firestorm/OpenSim-Viewers abdecken.

Diese Liste ist verbindliche Produktanforderung und wird phasenweise umgesetzt.

### Anmeldung, Grid und Session
- fester NexVerse-Gridzugang
- GridInfo-Discovery bei jedem Start
- Login mit Avatarname und Passwort
- Startposition: letzte Position, Zuhause und definierte Region/Position
- Session-Aufbau und Reconnect
- Logout
- mehrere Simulator-/Regionswechsel innerhalb einer Session

### Avatar und Bewegung
- Gehen
- Rennen
- Springen
- Fliegen
- Landen
- Sitzen / Aufstehen
- Teleport
- Autopilot / Zielbewegung soweit vom OpenSim-Protokoll unterstützt
- Mouselook
- Kamera frei bewegen
- Zoom / Orbit / Fokus
- Gesten
- Animationen
- Animation Override / AO-Grundfunktion
- Attachments
- Wearables / Outfit-Basis
- Avatar Appearance aktualisieren

### Welt und Regionen
- Region laden
- Nachbarregionen
- Regionswechsel
- Terrain
- Wasser
- Sky / Environment
- Tageszeit
- Wind-/Cloud-Basisdaten soweit vom Simulator geliefert
- Objekte / Prims
- Sculpt-/Mesh-Objekte
- Texturen und Materialien
- Lichtquellen
- Partikel
- Sitzplätze
- Physics-/Collision-relevante Zustände soweit für den Viewer erforderlich
- Parcel-/Land-Daten
- Estate-/Region-Basisinformationen

### Kommunikation
- Local Chat
- Whisper / Say / Shout
- Instant Messages
- Gruppenchat
- Conference Chat soweit OpenSim unterstützt
- Chat-Verlauf lokal
- Tippstatus
- Busy / Away
- Freundschaftsanfragen
- Freundesliste
- Online-/Offline-Status
- Blockieren / Muten
- Gruppen
- Gruppenmitgliedschaften
- Gruppenrollen und Notices soweit vom Grid unterstützt
- Voice als separates Modul
- Streaming Audio / Parcel Music
- Parcel Media / Web Media

### Inventar
- kompletter Inventarbaum
- Ordner
- Items
- Assets laden
- Copy / Move / Rename / Delete
- Trash
- Lost & Found
- Rezzen aus Inventar
- Take / Take Copy
- Give Inventory
- Attach / Detach
- Wear / Remove
- Outfits
- Gestures
- Landmarks
- Notecards
- Scripts
- Textures
- Sounds
- Animations
- Objects
- Mesh / Asset-Unterstützung entsprechend OpenSim
- Inventory Offers und Accept/Decline

### Karte, Orte und Navigation
- World Map
- Mini Map / Radar
- eigene Position
- Avatarpositionen soweit freigegeben
- Regionen
- Landmarks
- Favoriten
- Home
- Teleport History
- Location Bar
- Koordinaten
- SLurl-/hop-/OpenSim-URI-Verarbeitung soweit für NexVerse relevant

### Freunde, Profile und soziale Funktionen
- Profil anzeigen
- Profilbild
- About / Picks soweit verfügbar
- Freunde hinzufügen / entfernen
- Rechte für Freunde
- Nearby Avatars / Radar
- Gruppenprofil
- Gruppenbeitritt / Austritt
- Notices
- Mute-/Block-Liste

### Bauen und Bearbeiten
- Prims erstellen
- Position / Rotation / Scale
- Link / Unlink
- Duplicate
- Delete
- Take / Return soweit berechtigt
- Texture / Color
- Transparency
- Glow
- Full Bright
- Materialparameter soweit OpenSim unterstützt
- Flexible / Light / Physics-Eigenschaften soweit unterstützt
- Object Contents
- Permissions
- Verkaufspreis / Verkaufseinstellungen
- Script- und Notecard-Inhalte öffnen/bearbeiten
- Object Inspect / Creator / Owner
- Land Build-/Rez-Rechte beachten

### Grundstücke und Land
- Parcelinformationen
- Eigentümer
- Area
- Traffic soweit geliefert
- Access / Ban
- Media
- Music
- Landing Point
- Set Home soweit erlaubt
- About Land
- Buy Land / Abandon / Divide / Join nur sofern vom NexVerse-Grid erlaubt und serverseitig unterstützt

### Suche und Entdecken
- Personen
- Gruppen
- Orte
- Regionen
- Events
- Land
- Marketplace-Anbindung nur über eine dafür vorgesehene NexVerse-/Web-Schnittstelle

### UI-Zustandsregel
- Beim Viewer-Start wird fest `https://stadt-nexverse.de/service/welcome` als eingebettete Welcome-Seite geladen und hinter Login/Ladezustand angezeigt.
- Die Welcome-URL ist Bestandteil des NexVerse-Builds und wird nicht durch eine frei wählbare externe URL ersetzt.
- Ist die Welcome-Seite vorübergehend nicht erreichbar oder liefert einen HTTP-Fehler, bleibt der Viewer bedienbar und verwendet den NexVerse-Fallback-Hintergrund.
- Vor erfolgreichem Login werden nur Branding, Grid-/Verbindungsstatus und die Anmeldung angezeigt.
- Nach erfolgreichem Login, aber vor geladenem WorldModel/Renderer, wird ausschließlich ein Ladezustand für die Simulation angezeigt.
- Navigation, Freunde, Orte, Inventar, Teleport, Bauen, In-World-Toolbar und rechte Kontextleisten dürfen erst sichtbar bzw. bedienbar werden, wenn die Simulation/Region tatsächlich geladen und der WorldModel-Status `ready` ist.
- LLUDP `UDP LIVE` allein reicht ausdrücklich nicht aus, um die In-World-Oberfläche freizuschalten.
- Bewegung und andere In-World-Befehle dürfen erst gesendet werden, wenn die Simulation als bereit markiert ist.

### UI-Standardfunktionen
- Toolbar
- konfigurierbare Panels
- Notifications
- Dialoge
- Preferences
- Audio-Lautstärken
- Voice-Steuerung
- Kamera-Steuerung
- Grafikoptionen
- Netzwerk-/Cacheoptionen
- Tastaturbelegung
- Mausbelegung
- Vollbild / Fenster
- mehrere Auflösungen
- HiDPI
- Dark Theme / NexVerse Branding
- lokalisierbare UI
- Screenshot-Funktion

### Diagnose und Sicherheit
- Netzwerkstatus
- Ping / Packet Loss
- FPS
- Simulatorstatus
- Circuit-/CAPS-Status
- sichere Credential-Behandlung
- keine Passwörter in Logs
- Asset-/Netzwerkgrößenlimits
- Timeouts
- Reconnect
- Crash-/Fehlerdiagnose ohne sensible Daten

### Nicht automatisch Teil der Mindestbaseline

Spezielle Firestorm-Power-User-Erweiterungen wie RLV/RLVa, sehr spezielle Phototools, Firestorm-spezifische Debug-Optionen oder proprietäre Service-Integrationen werden nicht automatisch übernommen. Solche Funktionen kommen nur hinzu, wenn sie für NexVerse ausdrücklich benötigt werden.

Die UI muss nicht wie Firestorm aussehen. Maßgeblich bleibt das definierte OpenGenesisLINK-/NexVerse-Zieldesign. Firestorm dient hier ausschließlich als funktionaler Referenzumfang.

---

## 2.2 OpenGenesisLINK Next-Gen Renderer – verbindliche Zielarchitektur

Diese Renderer-Vorgabe ist ab 05.10.2026 verbindlicher Bestandteil des Projektplans und muss bei allen späteren Entscheidungen zu WorldModel, Scene Layer, Rendering, Vulkan, Materialien, Terrain, Wasser, Atmosphäre, Avataren, Assets, Performance und GPU-Funktionen berücksichtigt werden.

Wichtig: Diese Vorgabe beschreibt die technische und qualitative Zielrichtung. Sie ist ausdrücklich keine Aufforderung, alle Zukunftsfunktionen sofort gleichzeitig zu implementieren. Die Umsetzung erfolgt weiterhin schrittweise nach den Projektphasen und den weiteren Anweisungen im Chat.

### Langfristiges Qualitätsziel

OpenGenesisLINK erhält eine eigene moderne Rendering-Architektur. Bestehende OpenSim-Welten müssen kompatibel bleiben, sollen aber grafisch deutlich hochwertiger dargestellt werden als im klassischen Second-Life-/OpenSim-Viewer.

Second Life beziehungsweise Firestorm dienen für die Grafikqualität nur als Mindestvergleich. Ziel ist keine optische Kopie, sondern eine eigenständige moderne Darstellung mit besserer Beleuchtung, Materialien, Wasser, Terrain, Atmosphäre, Reflexionen, Schatten, Performance und optionalem Hardware-Raytracing.

### Verbindliche Architekturtrennung

Grundprinzip:

```text
OpenSim / OpenGenesisLINK Netzwerkdaten
        ↓
WorldModel / Scene Representation
        ↓
OGLRender Render Abstraction Layer
        ↓
Render Backend
        ↓
GPU
```

Der Netzwerk-/OpenSim-Protokollcode darf nicht direkt von Vulkan oder einem bestimmten Rendering-Backend abhängig sein.

Der bestehende Viewer-Code bleibt zuständig für unter anderem:

- Netzwerk
- OpenSim-Protokoll
- Inventar
- Kommunikation
- Region-Verbindungen
- Asset-System
- Avatar-Daten
- Viewer-Funktionen

Die Renderingarchitektur wird davon sauber getrennt.

### OGLRender

Arbeitstitel der neuen Render-Abstraktionsschicht:

`OGLRender`

Zielstruktur, bei Bedarf später anpassbar:

```text
OGLRender/
    Core/
    Backend/
    Vulkan/
    Scene/
    Materials/
    Lighting/
    Shadows/
    Reflections/
    Terrain/
    Water/
    Environment/
    Avatars/
    Particles/
    PostProcessing/
    RayTracing/
    Upscaling/
    Streaming/
    Debug/
```

### Primäres Render-Backend

Vulkan ist das primäre moderne Render-Backend.

Zielplattformen zunächst:

- Windows x86_64
- Linux x86_64
- Linux ARM64 soweit technisch sinnvoll

Langfristig muss die Render-Abstraktion weitere Backends zulassen. Für macOS kann später Metal geprüft werden; Windows kann später optional DirectX 12 erhalten. Zunächst bleibt der Fokus auf Vulkan.

### Legacy-Renderer und Migration

Der vorhandene/aktuelle Renderer wird während der Entwicklung nicht sofort entfernt.

Übergangsmodell:

```text
Renderer:
- Legacy
- OpenGenesisLINK NextGen
```

Der NextGen-Renderer darf zunächst experimentell parallel existieren. Erst wenn er ausreichend stabil und funktional vollständig ist, wird über die endgültige Ablösung des Legacy-Renderers entschieden.

### OpenSim-Kompatibilität

Bestehende Regionen dürfen nicht neu gebaut werden müssen.

Unterstützt bleiben müssen insbesondere:

- Prims
- Meshes
- Sculpties
- Texturen
- Avatare
- Kleidung
- Attachments
- Terrain
- Wasser
- Partikel
- Lichtquellen
- Alpha-Materialien
- bestehende PBR-Materialien
- Legacy-Materialien

Prinzip:

```text
bestehende OpenSim-Daten
→ OpenGenesisLINK Scene Layer
→ moderner Renderer
```

### Modernes Materialsystem

Das Materialsystem wird langfristig PBR-basiert und glTF-kompatibel im Metallic/Roughness-Modell ausgelegt.

Mindestens vorzubereiten:

- Base Color
- Normal
- Metallic
- Roughness
- Ambient Occlusion
- Emissive
- Alpha
- Height / Displacement

Später optional:

- Clearcoat
- Clearcoat Roughness
- Transmission
- Subsurface Scattering
- Anisotropy
- Sheen

Legacy-Materialien müssen automatisch möglichst sinnvoll auf das neue Materialsystem abgebildet werden.

### Beleuchtung

Langfristig vorzusehen:

- physikalisch plausible Beleuchtung
- HDR Rendering
- Image Based Lighting
- Forward+ oder Clustered Lighting
- dynamische Lichtquellen
- hochwertige Sonnenbeleuchtung
- Point Lights
- Spot Lights
- Reflection Probes
- Ambient Occlusion
- Tone Mapping
- Exposure Control
- kontrollierbares Bloom
- indirekte Beleuchtung soweit technisch sinnvoll

Bestehende OpenSim-Lichtquellen bleiben kompatibel.

### Schatten und Reflexionen

Rasterpfad:

- Cascaded Shadow Maps
- hochwertige Schattenfilterung
- dynamische Schatten
- konfigurierbare Shadow Distance
- konfigurierbare Shadow Resolution
- Reflection Probes
- Screen Space Reflections
- Planar Reflections
- Cubemap-Fallback

Optional mit Raytracing:

- Raytraced Shadows
- Raytraced Reflections
- Raytraced Ambient Occlusion
- später optional RT Global Illumination

### Hardware-Raytracing

Raytracing bleibt ausdrücklich optional. Eine Raytracing-GPU darf niemals Voraussetzung für OpenGenesisLINK sein.

```text
GPU ohne Raytracing
→ Vulkan Raster Rendering

GPU mit Raytracing
→ optional Hybrid Rendering
```

Die Architektur soll BLAS/TLAS berücksichtigen und dynamische OpenSim-Welten effizient behandeln. Statische Geometrie soll wiederverwendet werden; bewegliche Instanzen werden aktualisiert, ohne unnötig die gesamte Beschleunigungsstruktur neu aufzubauen.

Raytracing-Reichweite und Qualitätsstufen bleiben konfigurierbar. Als verbindliche Zielrichtung werden folgende Profile vorgesehen, deren exakte Zuordnung später technisch angepasst werden darf:

```text
Aus
Automatisch
Niedrig  -> RT Shadows
Mittel   -> RT Shadows + RT Reflections
Hoch     -> RT Shadows + RT Reflections + RT AO
Ultra    -> RT Shadows + RT Reflections + RT AO + optional RT GI
```

Raytracing muss außerdem räumlich begrenzbar sein. Nahe Szenenbereiche können Raytracing verwenden, während weiter entfernte Bereiche auf Rasterisierung, Reflection Probes, SSR oder andere Fallbacks zurückfallen. Als mögliche, später abstimmbare Reichweiten gelten 50 m, 100 m, 150 m, 256 m und 512 m.

### Hardwareerkennung

Später automatisch erkennen:

- GPU
- VRAM
- Vulkan-Unterstützung
- relevante Vulkan Extensions
- Hardware-Raytracing
- Upscaling-Unterstützung

Nicht unterstützte Funktionen dürfen nicht angeboten beziehungsweise automatisch deaktiviert werden.

Kein GPU-Hersteller-Lock-in: NVIDIA, AMD und Intel sind grundsätzlich zu berücksichtigen.

### Terrain

Bestehendes OpenSim-Terrain bleibt kompatibel.

Enhanced-Terrain soll langfristig unter anderem vorbereiten:

- Heightmap
- Base Color
- Normal
- Roughness
- AO
- Detail Maps
- Height / Displacement
- Vegetation Mask
- Wetness Mask
- Snow Mask

Mögliche Materialtypen: Gras, Erde, Stein, Sand, Schlamm, Schnee und nasser Untergrund.

### Wasser

Langfristig:

- hochwertige Reflexionen
- Refraction
- Fresnel
- Normal Waves
- Foam
- Tiefenabhängigkeit
- Unterwasser-Fog
- Sonnenreflexion
- Küstenübergänge
- Reflection Probes
- SSR
- optional RT Reflections

Bestehende OpenSim-Wasserhöhen bleiben kompatibel.

### Vegetation

Vorbereiten:

- GPU Instancing
- LOD
- Distance Culling
- Frustum Culling
- GPU Culling
- Wind Animation
- optimierte Foliage Shader

Später optional automatische Grasverteilung und Vegetation Masks.

### Atmosphäre und Environment

Langfristig:

- physikalischer Himmel
- Sonne
- Mond
- Sterne
- atmosphärische Streuung
- volumetrische Wolken
- volumetrischer Nebel
- moderne Tageszeiten

Bestehende EEP-/Windlight-Daten müssen übernommen beziehungsweise übersetzt werden können.

### Wetter

Später mögliche Erweiterung:

- klar
- bewölkt
- Regen
- Gewitter
- Nebel
- Schnee

Wetter kann später Materialien und Umgebung beeinflussen, etwa Wetness/Roughness/Reflexionen. Dieses System wird nicht vorzeitig implementiert.

### Avatar-Rendering

Bestehende OpenSim-/SL-Avatare bleiben vollständig kompatibel.

Langfristige Qualitätsverbesserungen:

- Skin Shader
- bessere Hautdarstellung
- Subsurface Scattering
- Augen
- Haare
- Stoffe
- Schatten
- Animation Interpolation
- Normal Maps
- moderne PBR-Materialien

### Post Processing

Modular vorbereiten:

- HDR
- Tone Mapping
- Exposure
- Bloom
- GTAO / HBAO
- TAA
- SMAA
- Color Grading
- Sharpening
- Depth of Field
- optional Motion Blur
- volumetrische Effekte

Effekte müssen einzeln deaktivierbar sein.

### Upscaling

Herstellerunabhängig vorbereiten:

- FSR

Später optional, nur bei vorhandener Unterstützung:

- DLSS
- XeSS

### GPU- und Streaming-Performance

Der Renderer muss nicht nur schöner, sondern effizienter werden.

Vorbereiten:

- Frustum Culling
- Occlusion Culling
- Distance Culling
- GPU Culling
- Instancing
- LOD
- Texture Streaming
- Mesh Streaming
- später optional Meshlets
- GPU Driven Rendering
- Indirect Draw

Texture Streaming priorisiert sichtbare und nahe Inhalte. Mesh Streaming verwendet passende LOD-Stufen nach Entfernung und Sichtbarkeit.

### Performance-Diagnose

Entwickleransicht langfristig mit mindestens:

- FPS
- Frame Time
- CPU Render Time
- GPU Frame Time
- VRAM Usage
- RAM Usage
- Draw Calls
- Triangles
- sichtbare Objekte
- Texture Memory
- Mesh Memory
- Shadow Time
- Raytracing Time
- Post Processing Time

### Grafikprofile

Langfristig:

- Sehr niedrig
- Niedrig
- Mittel
- Hoch
- Ultra
- Benutzerdefiniert

OpenGenesisLINK soll sowohl auf schwächeren Systemen als auch auf High-End-Gaming-PCs sinnvoll skalieren.

### Serverunabhängigkeit und Enhanced Capabilities

Der neue Renderer darf keine speziellen OpenGenesisLINK-Servererweiterungen voraussetzen.

Normale OpenSim-Region:

```text
Standarddaten
→ Compatibility Rendering
```

OpenGenesisLINK-Region kann später zusätzlich optionale Enhanced Capabilities liefern, beispielsweise:

- OGL_PBR
- OGL_TERRAIN
- OGL_LIGHTING
- OGL_ENVIRONMENT
- OGL_WEATHER

Fehlt eine Capability, muss immer der Standard-OpenSim-Fallback funktionieren.

### Stabilität und Fallback

Kann Vulkan oder der NextGen-Renderer nicht gestartet werden:

```text
Fehler erkennen
→ Logeintrag
→ sinnvoller Fallback
```

Solange ein Legacy-Renderer vorhanden ist, darf auf diesen zurückgefallen werden. Der Viewer darf wegen eines nicht verfügbaren NextGen-Features nicht einfach abstürzen.

### Renderer-Logging

Vorzusehen sind klare Renderer-Logs, beispielsweise:

```text
[OGL-RENDER] Renderer wird initialisiert
[OGL-RENDER] GPU: ...
[OGL-RENDER] VRAM: ...
[OGL-RENDER] Vulkan: ...
[OGL-RENDER] Hardware-Raytracing: Ja/Nein
[OGL-RENDER] Renderer erfolgreich gestartet
```

Fehlerkategorien unter anderem:

```text
[OGL-RENDER-ERROR]
[OGL-VULKAN-ERROR]
[OGL-RT-ERROR]
```

### Schrittweise Umsetzung – verbindlich

Nicht eigenständig alle Zukunftssysteme gleichzeitig bauen.

Insbesondere nicht ohne passende spätere Phase sofort vollständig implementieren:

- Raytracing
- Wetter
- Terrain 2.0
- neue Avatar-Systeme
- Upscaling
- Global Illumination
- neue Server-Protokolle
- sämtliche Post-Processing-Systeme

Vorgehen:

1. Zielarchitektur bei allen Entscheidungen berücksichtigen.
2. Bestehende Architektur analysieren.
3. saubere World-/Scene-/Render-Abstraktionen schaffen.
4. OpenSim-Kompatibilität erhalten.
5. Funktionen phasenweise nach Projektplan und weiteren Chat-Anweisungen implementieren.

Diese Renderer-Vorgabe ist bei jeder späteren Rendering-Phase erneut heranzuziehen. Sie darf nicht dadurch übergangen werden, dass kurzfristig ein einfacherer renderergebundener Netzwerk- oder WorldModel-Weg möglich wäre.


### Abgleich mit der vollständigen Next-Gen-Renderer-Anweisung – 09.10.2026

Die vollständige Renderer-Anweisung wurde erneut gegen diesen Projektplan geprüft. Abschnitt 2.2 bleibt die verbindliche konsolidierte Fassung für dieses Repository.

Dabei gelten zusätzlich ausdrücklich folgende Klarstellungen:

- OpenGenesisLINK ist **kein einfacher Firestorm-Umbau** und der Renderer wird nicht als Firestorm-Renderer-Fork geführt.
- Firestorm/Second Life dienen lediglich als Funktions- beziehungsweise Qualitätsreferenz; die Renderarchitektur bleibt eigenständig.
- `OGLRender` bezeichnet die Render-Abstraktionsschicht und die gemeinsame Scene-/Material-/Lighting-/Water-/Environment-Schnittstelle.
- Vulkan bleibt das primäre moderne Backend.
- Die neue Module Runtime darf Renderer-Backends später als austauschbare Module bereitstellen, ohne die `WorldModel -> Scene -> OGLRender`-Trennung aufzugeben.
- Ein optionales `OGL Vulkan Renderer`- beziehungsweise `OGL RTX Renderer`-Modul implementiert definierte Renderer-Interfaces; Netzwerk, WorldModel und OpenSim-Protokoll dürfen davon nicht abhängen.
- Der vorhandene OpenGL-Renderer bleibt während der Migration der sichere Legacy-/Fallback-Pfad.
- Raytracing bleibt optional; Rasterpfad und Fallbacks müssen vollständig funktionsfähig bleiben.
- PBR, Beleuchtung, Schatten, Reflexionen, Wasser, Terrain, Atmosphäre, Wetter, Post Processing, Streaming und Performance-Optimierung werden schrittweise umgesetzt und nicht als ein einzelner großer Umbau.
- OpenSim-Kompatibilität bleibt Pflicht: bestehende Regionen müssen ohne Neuanlage oder spezielle OGL-Servererweiterungen darstellbar bleiben.
- Enhanced Capabilities erweitern die Darstellung nur optional; fehlt eine OGL-Capability, greift der Standard-OpenSim-Fallback.

Die Renderer-Anweisung und die Plugin-/Modularchitektur ergänzen sich damit: OGLRender definiert die stabile Rendergrenze, während konkrete Backends und High-End-Funktionen später modular geliefert werden können.

---

## 2.3 OpenGenesisLINK Add-on- und Plugin-Architektur – verbindliche Vorgabe

Diese Vorgabe ist ab 09.10.2026 verbindlicher Bestandteil des Projektplans. Neue größere Funktionen müssen so entworfen werden, dass der Viewer-Core nicht unnötig mit optionalen Funktionen gekoppelt wird.

### Offizielle Add-on-Plattform

Die offizielle OpenGenesisLINK Add-on- und Plugin-Plattform ist:

```text
https://addons.opengenesislink.de/
```

Diese Plattform dient als zentrale Quelle für Plugin-Katalog, Metadaten, Versionen, Download-Pakete, Prüfsummen, digitale Signaturen, Kompatibilitätsinformationen und Abhängigkeiten. Webseite und Viewer müssen auf dieselbe Add-on-Registry beziehungsweise denselben Datenbestand zugreifen.

### Plugin Manager gehört zum Viewer-Core

Der Plugin Manager selbst ist kein optionales Plugin. Zum Core gehören mindestens:

```text
Plugin Manager
Plugin Registry Client
Package Installer
Signature Verification
Dependency Resolver
Compatibility Resolver
Plugin Lifecycle Manager
Update Manager
Permission / Capability Manager
```

Der Viewer muss auch ohne optionale Plugins vollständig startfähig und als Basis-Viewer nutzbar bleiben.

### Installationswege

Unterstützt werden die direkte Installation aus dem Viewer über `addons.opengenesislink.de` sowie die manuelle Installation eines heruntergeladenen OpenGenesisLINK Plugin-Pakets. Vorgesehenes Paketformat: `*.oglpkg`.

Ein Paket kann beispielsweise `manifest.json`, Plugin-Binary, Assets, Shader, Lizenz und Signatur enthalten. Die genaue Containerstruktur wird vor Implementierung der Plugin Runtime separat spezifiziert und versioniert.

### Plugin Manifest und stabile IDs

Jedes Plugin benötigt ein maschinenlesbares Manifest mit mindestens ID, Name, Version, Vendor, Plugin-API-Version, Mindest-Viewer-Version, Plattformen, Architekturen, Plugin-Typ, Capabilities, Abhängigkeiten, Entrypoint, Paket-Hash und Signaturmetadaten. Plugin-IDs sind dauerhaft stabil. Beispiel: `org.opengenesislink.renderer.rtx`.

### Plugin API und ABI

OpenGenesisLINK benötigt eine versionierte Plugin API. Native Plugins dürfen nicht direkt auf interne C++-Klassen zugreifen, deren Layout sich ohne API-Versionierung ändern kann. Bevorzugt wird eine schmale C-kompatible ABI oder eine vergleichbar stabile Boundary. Inkompatible Plugins dürfen nicht geladen werden.

### Aktivieren und Deaktivieren

Plugins müssen im Viewer installiert, aktiviert, deaktiviert, aktualisiert und deinstalliert werden können. Zusätzlich müssen Details, Abhängigkeiten und Kompatibilitätsstatus sichtbar sein. Wenn ein Plugin nicht gefahrlos zur Laufzeit entladen werden kann, darf ein Neustart erforderlich sein; der Viewer muss dies klar anzeigen.

### Capability- und Rechte-Modell

Plugins deklarieren benötigte Capabilities. Mindestens vorzusehen sind `ui`, `audio`, `voice`, `network`, `world-read`, `world-edit`, `inventory`, `avatar`, `animation`, `media`, `renderer`, `gpu`, `filesystem-scoped` und `developer`. Besonders privilegierte Plugins, insbesondere Renderer- oder Netzwerkmodule, müssen klar gekennzeichnet sein.

### Vertrauensstufen und Signaturen

Native Plugins sind sicherheitskritisch. Vorgesehene Vertrauensstufen:

```text
OGL Official
Verified Developer
Unverified / Developer Mode
```

Offizielle Pakete müssen digital signiert sein. Paket-Hash und Signatur sind vor Installation beziehungsweise Laden zu prüfen. Manipulierte oder ungültig signierte Pakete werden blockiert. Unsignierte lokale Plugins dürfen nur über einen ausdrücklich aktivierten Developer Mode geladen werden.

### Registry- und Download-Schnittstelle

`addons.opengenesislink.de` benötigt eine maschinenlesbare API. Vorgesehene logische Endpunkte:

```text
/api/v1/addons
/api/v1/addons/{id}
/api/v1/addons/{id}/releases
/api/v1/addons/{id}/releases/{version}
/api/v1/addons/{id}/download/{platform}/{architecture}
```

Die konkrete API darf später angepasst werden, muss aber Katalog, Suche, Kategorien, Detailinformationen, Releases, kompatible Paketauswahl, Download, Update-Erkennung sowie Hash- und Signaturprüfung unterstützen.

### Kategorien

Mindestens vorzusehen: Rendering, Voice, Media, Avatar, Animation/AO, Building, Developer Tools, Accessibility, UI, Social sowie OpenSim/Hypergrid.

### Core versus Plugin

Im Core bleiben mindestens OpenSim Login, Grid Discovery, LLUDP, CAPS, WorldModel, Asset-Grundsystem, Avatar-Grundsystem, Inventar-Grundsystem, UI-Grundsystem, Standard Renderer, Plugin Manager, Registry Client und Plugin Security.

Als Plugins beziehungsweise austauschbare Module eignen sich insbesondere RTX/Ray-Tracing Renderer, alternative Renderer, Voice-Erweiterungen, AO/Animation, RLV/RLVa, Media-Erweiterungen, Advanced Build Tools, Developer Tools, zusätzliche UI-Panels und optionale Social-Funktionen.

### Renderer als besonderer Modultyp

Renderer-Plugins benötigen eine eigene Renderer API. Zielarchitektur:

```text
WorldModel / Scene API
        ↓
Renderer Interface
        ├── Standard OpenGL Renderer
        ├── Vulkan Renderer
        └── RTX / Ray-Tracing Renderer Plugin
```

Das WorldModel darf nicht wissen, welcher Renderer aktiv ist. Ein RTX-/Ray-Tracing-Plugin muss GPU- und API-Capabilities prüfen, beispielsweise Vulkan, `VK_KHR_acceleration_structure` und `VK_KHR_ray_tracing_pipeline`. Ist ein optionaler Renderer nicht nutzbar, muss ein unterstützter Standard-Renderer verfügbar bleiben.

### Abhängigkeiten und Updates

Plugins dürfen versionierte Abhängigkeiten deklarieren. Der Dependency Resolver darf keine teilweise oder inkonsistente Installation hinterlassen. Zirkuläre oder nicht erfüllbare Abhängigkeiten werden abgelehnt. Updates dürfen erst nach erfolgreicher Prüfung von Kompatibilität, Hash, Signatur und Abhängigkeiten installiert werden. Rollback beziehungsweise sichere Wiederherstellung nach fehlgeschlagenem Update ist als spätere Anforderung vorzusehen.

### Plattformen

Die Plugin-Architektur muss mindestens `linux-x64`, `windows-x64`, `macos-arm64` und optional `macos-x64` berücksichtigen. Ein Plugin darf nur angeboten beziehungsweise geladen werden, wenn ein kompatibles Paket für Plattform und Architektur vorhanden ist.

### Entwicklungsregel für kommende Phasen

Ab sofort gilt bei neuen größeren Funktionen:

1. Prüfen, ob die Funktion zwingend zum Core gehört.
2. Falls nicht, eine Plugin-/Modulgrenze bevorzugen.
3. Keine direkten Abhängigkeiten eines Plugins auf private Renderer-, Protocol- oder WorldModel-Implementierungsdetails.
4. Öffentliche Plugin-Schnittstellen versionieren.
5. Plugin-Funktionen dürfen den Viewer ohne Plugin nicht unbrauchbar machen.
6. Sicherheit, Signaturprüfung und Capability-Grenzen haben Vorrang vor maximaler Plugin-Freiheit.
7. `addons.opengenesislink.de` ist die offizielle Registry für veröffentlichte OpenGenesisLINK Add-ons.

Die minimale Module Runtime wird ab sofort vor weiteren großen Viewer-Funktionsblöcken implementiert. Sie ist technische Grundlage des OGL Viewers und kein späteres Add-on. Die Online-Registry, der Add-on-Store, Downloads von `addons.opengenesislink.de`, Signatur-Infrastruktur und veröffentlichte optionale Plugins folgen erst nach dem Viewer-Core beziehungsweise in späteren Phasen.


### Sofortiger Modul-System-Basisblock – vor weiteren großen Viewer-Features

Bevor nach Phase 8 weitere große Systeme wie Inventar, Voice, AO, Media, erweiterte UI oder alternative Renderer eng an den Core gekoppelt werden, wird eine minimale, lokale Module Runtime implementiert.

Dieser Basisblock umfasst verbindlich:

```text
ModuleManager
versionierte Module API
stabile native ABI-Grenze
Manifest Loader
lokale Modul-Erkennung
Modul-Lifecycle
Aktivieren / Deaktivieren
Abhängigkeitsprüfung
Capability-Deklaration
Plattform-/Architekturprüfung
Fehlerisolierung beim Laden
Persistenz des Aktivierungszustands
```

Vorgesehener nativer Lifecycle:

```text
ogl_module_init
ogl_module_start
ogl_module_stop
ogl_module_shutdown
```

Die konkreten Symbolnamen dürfen bei der Implementierung angepasst werden, müssen danach jedoch Teil der versionierten ABI werden.

Lokale Entwicklungsstruktur, beispielhaft:

```text
OpenGenesisLINK/
├── ogl-viewer
├── config/
├── cache/
└── modules/
    └── org.opengenesislink.example/
        ├── manifest.json
        └── module.so / module.dll / module.dylib
```

Zunächst werden Module lokal installiert beziehungsweise aus dem Modulverzeichnis geladen. Noch nicht Bestandteil dieses Basisblocks sind:

```text
Online-Plugin-Store
Download aus dem Viewer
addons.opengenesislink.de API-Client
automatische Online-Updates
Publisher-/Developer-Accounts
Produktions-Signaturservice
RTX-, AO-, Voice- oder andere fertige optionale Plugins
```

Diese Online- und Produktfunktionen bauen später auf derselben Module Runtime auf. Dadurch wird der Viewer jetzt modular vorbereitet, ohne die Fertigstellung der eigentlichen Viewer-Kernfunktionen durch einen kompletten Add-on-Store zu verzögern.


### Geplanter OpenGenesisLINK Plugin-Zielkatalog

Der folgende Katalog ist die verbindliche Zielrichtung für spätere optionale Module. Die Liste definiert Zweck, grobe Verantwortungsgrenze und Prioritätsklasse. Nicht alle Plugins werden sofort umgesetzt.

#### Klasse A – Infrastruktur- und Renderer-Module

1. **OGL Example Module** – Referenz-/Testplugin für ABI, Manifest, Lifecycle, Capabilities, Aktivieren/Deaktivieren und Dependency-Handling.
2. **OGL Vulkan Renderer** – moderner alternativer Raster-Renderer; Grundlage für spätere Ray-Tracing-Funktionen und bessere GPU-Auslastung.
3. **OGL RTX Renderer** – optionales Ray-Tracing-/RTX-Backend mit Raytracing-Schatten, Reflexionen und später GI; nur auf kompatibler Hardware.
4. **OGL Voice** – Spatial Voice, Mikrofon, Gerätewahl, Mute, Avatar-Lautstärke und Positionsaudio.
5. **OGL Media** – erweiterte Media-on-a-Prim-/Browser-/Video-/Codec-Funktionen auf Basis einer stabilen Media-Schnittstelle.
6. **OGL Weather** – realistisches Wettermodul für Regen, Schnee, Nebel, Sturm und Wind mit moderner GPU-basierter Darstellung; spätere RTX-Erweiterungen für nasse Oberflächen, Reflexionen und volumetrische Effekte vorgesehen.

#### Klasse B – Avatar, Animation und Bedienung

7. **OGL Advanced AO** – erweiterter Animation Override mit mehreren Sets und automatischen Zustandswechseln.
8. **OGL RLVa** – optionale RLV/RLVa-Kompatibilität für Attachments und Restriktionsbefehle.
9. **OGL Outfit Manager** – erweiterte Outfit-, Wearable- und Attachment-Verwaltung mit Kategorien, Tags, Favoriten, Suche, zuletzt getragen und optionalen Metadaten.
10. **OGL Outfit Preview** – Screenshot-/Galerie-Erweiterung für Outfits mit manueller oder automatischer Vorschau-Erstellung, Vorschaubildern, Kamera-Presets und lokaler Verknüpfung zum Outfit.
11. **OGL Avatar Tools** – Avatar-Diagnose, Attachment-Übersicht, Animationsstatus, Komplexität und Rendering-Debug.
12. **OGL Animation Studio** – Animation Preview, Trigger, Sequenzen und erweiterte Animationsverwaltung.
13. **OGL Gesture Tools** – Gesten suchen, kategorisieren, testen und Trigger-Konflikte erkennen.
14. **OGL Gamepad** – Controller-Unterstützung für Xbox-/PlayStation-kompatible Geräte mit frei belegbarer Steuerung.
15. **OGL VR** – spätere OpenXR-basierte VR-Unterstützung mit Head Tracking, Controllern und VR-UI.

#### Klasse C – Bauen, Materialien und Assets

16. **OGL Advanced Build Tools** – exakte Transformationen, Alignment, Multi-Edit, Prim-/Linkset-Werkzeuge.
17. **OGL Mesh Tools** – Mesh-Diagnose, LOD, Triangle Count, Bounding Boxes, Material-Slots und Upload-Vorprüfung.
18. **OGL Material Tools** – Legacy- und PBR-Materialbearbeitung, Normal/Roughness/Metallic und Materialvorschau.
19. **OGL Scripting Tools** – erweiterter LSL/OSSL-Editor, Syntax-Highlighting, Script-Logs und Diagnose.
20. **OGL Asset Inspector** – Asset-UUIDs, Texturen, Meshes, Animationen, Sounds, Cache- und Decode-Status.
21. **OGL Export / Backup Tools** – Sicherung erlaubter Einstellungen, Profile, Favoriten und Inhalte unter strikter Beachtung von Copy-/Export-Rechten.

#### Klasse D – Welt, Land und Administration

22. **OGL Map Plus** – zusätzliche Map-Layer, Marker, Favoriten und Teleport-Historie.
23. **OGL Hypergrid Tools** – HG-Favoriten, Grid-Verlauf, Grid-Infos, Hypergrid-Diagnose und Landmark-Verwaltung.
24. **OGL Parcel Tools** – Parcel-Analyse, Media/Music, Access-/Ban-Listen, Objektübersicht und Landstatistiken.
25. **OGL Estate Tools** – Region-/Estate-Administration, Access, Bans, Terrain, Neustarts und Debug-Optionen.
26. **OGL Moderation Tools** – Freeze/Eject/Ban-Hilfen, Abuse-Report-Unterstützung und Moderationsdiagnose; Serverrechte bleiben maßgeblich.

#### Klasse E – UI, Komfort und Kommunikation

27. **OGL UI Themes** – alternative Themes, Farben, Icons und Layout-Presets auf Basis der OGL-UI-API.
28. **OGL Accessibility** – High Contrast, größere UI, Farbsehschwächen-Modi und weitere Barrierefreiheitsfunktionen.
29. **OGL Inventory Plus** – erweiterte Suche, Smart Folders, Duplikaterkennung und Massenoperationen; Basisinventar bleibt Core.
30. **OGL Social Plus** – Kontaktkategorien, Notizen, Filter und optionale soziale Komfortfunktionen; Freunde/IM/Gruppen bleiben Core.
31. **OGL Stream & Radio** – Parcel Music, Icecast/Shoutcast, Stream-Metadaten und Favoriten.
32. **OGL Translator** – optionale Chat-/IM-Übersetzung über konfigurierbare externe Dienste mit klaren Datenschutzhinweisen.
33. **OGL Discord Integration** – optionale Rich Presence/Statusintegration nur nach ausdrücklicher Aktivierung.
34. **OGL Streamer Mode** – Datenschutzmodus für Streams/Videos, z. B. Namen, Chat, Koordinaten und Benachrichtigungen ausblenden.

#### Klasse F – Diagnose, Kreativ- und Entwicklerwerkzeuge

35. **OGL Developer Tools** – LLUDP-, CAPS-, EventQueue-, ObjectUpdate-, TextureEntry-, Asset- und Renderer-Diagnose.
36. **OGL Performance Monitor** – FPS, Frametime, Draw Calls, VRAM, CPU/GPU, Netzwerk und Cache-Telemetrie.
37. **OGL Photography** – freie Kamera, Kamera-Presets, High-Resolution-Screenshots, Post-Processing und spätere DoF-/Machinima-Funktionen.
38. **OGL AI Assistant** – spätere optionale Assistenz für UI-Hilfe, Diagnose und Build-/Scripting-Unterstützung; niemals Pflichtbestandteil des Viewers.

### Verbindliche Core-/Plugin-Trennung für Text-only, Tageszeit und Wetter

#### Text-only Viewer – Core-Betriebsmodus

OpenGenesisLINK erhält einen nativen Text-only-Betriebsmodus. Dieser ist kein Plugin, sondern ein alternativer Core-Modus des Viewers.

Ziel ist ein vollständig nutzbarer OpenSim-/NexVerse-Client ohne aktive 3D-Weltdarstellung. Derselbe Protocol-, Session-, Social-, Inventar- und World-State-Core wird verwendet, der 3D-Renderer wird jedoch nicht beziehungsweise nur minimal initialisiert.

Mindestens vorzusehen:

```text
Login
Grid-/Regionstatus
Chat
IM
Freunde
Gruppen
Inventar
Outfits / Appearance-Basis
Teleport
Landmarks
Benachrichtigungen
Avatar-/Regionstatus in Textform
optionale minimale Karte / Regionsinformation
```

Der Startmodus soll konfigurierbar sein:

```text
Normaler 3D-Modus
Text-only-Modus
```

Der Text-only-Modus muss auch auf Systemen ohne leistungsfähige GPU sinnvoll nutzbar sein. Renderer-spezifische Plugins wie Vulkan oder RTX dürfen dafür keine Voraussetzung sein.

#### Reale Tag-/Nachtzeit – Core Environment-Funktion

Die Anpassung der sichtbaren Tageszeit an reale Zeit gehört in den Environment-Core und nicht in ein Plugin. Sie ist ein lokaler Viewer-Override und darf die vom Simulator gelieferte Region-/EEP-Konfiguration nicht serverseitig verändern.

Vorgesehene Modi:

```text
Region / OpenSim / EEP verwenden
Feste lokale Uhrzeit
Eigener lokaler Tageszyklus
Reale lokale Systemzeit
Reale Zeit mit geografisch berechneter Sonnenposition
```

Standard bleibt die Region-/OpenSim-Vorgabe. Der Benutzer muss einen lokalen Override ausdrücklich aktivieren.

Für `Reale lokale Systemzeit` wird die lokale Uhrzeit des Betriebssystems auf den sichtbaren Tageszyklus abgebildet.

Für die spätere Option `Reale Zeit mit geografisch berechneter Sonnenposition` dürfen Datum, Uhrzeit und ein vom Benutzer gewählter Referenzort verwendet werden, um Sonnenaufgang, Sonnenuntergang und Sonnenstand zu berechnen. Diese Funktion darf nicht ungefragt den physischen Standort des Benutzers ermitteln oder übertragen.

Der Environment-Core muss dafür eine stabile API bereitstellen, über die Renderer und optionale Weather-Plugins den resultierenden Environment-Zustand lesen können.

#### OGL Weather Plugin

Realistischer Regen, Schnee und weitere Wettereffekte werden als optionales `OGL Weather`-Plugin umgesetzt.

Ziel ist ausdrücklich keine einfache, grobe Pixel-/Billboard-Partikeldarstellung. Das Plugin soll eine moderne, skalierbare Wetterpipeline verwenden.

Vorgesehene Funktionen:

```text
Regen
Schnee
Nebel
Sturm
Windböen
unterschiedliche Niederschlagsintensität
weltkoordinatenbasierte Wetterzonen
Entfernungs-/Dichteanpassung um Kamera und Avatar
windabhängige Partikelbewegung
GPU-basierte Partikelsimulation beziehungsweise spätere Compute-Pfade
weiche Übergänge zwischen Wetterzuständen
```

Das Weather-Plugin darf Environment- und Renderer-APIs benutzen, aber keine privaten Renderer-Implementierungsdetails direkt referenzieren.

Vorgesehene Schnittstellen:

```text
Environment API
Weather State API
Wind API
Camera API
Scene Depth / Surface Query API
Renderer Effects API
```

#### Spätere OGL Weather RTX-Erweiterungen

Auf einem kompatiblen Vulkan-/RTX-Renderer können zusätzliche Wettereffekte aktiviert werden. Diese sind optional und dürfen nicht Voraussetzung für normales Wetter sein.

Später vorgesehen:

```text
nasse Oberflächen
Ray-Tracing-Reflexionen auf nassem Boden
realistischere Pfützen
volumetrischer Niederschlag / Nebel
Lichtstreuung im Regen / Schnee
verbesserte Schatten und Beleuchtung während Unwettern
```

Die Architektur soll es ermöglichen, dass `OGL Weather` mit dem Standard-Renderer funktioniert und bei vorhandenem `OGL RTX Renderer` zusätzliche hochwertige Effekte freischaltet.

#### Zusammenspiel von Region, Tageszeit und Wetter

Die Priorität wird klar definiert:

```text
1. Simulator/Region liefert Environment/EEP als Basis
2. optionaler lokaler Tageszeit-Override verändert nur die lokale Darstellung
3. OGL Weather ergänzt lokale Wettereffekte
4. optionale RTX-Funktionen verbessern ausschließlich die Darstellung
```

Ein lokaler Wetter- oder Tageszeit-Override darf keine Regionseinstellungen auf dem OpenSim-Server verändern.

### Verbindliche Outfit-System-Trennung

Das Outfit-System wird bewusst zwischen Core und Plugins getrennt. Der Viewer muss ohne optionale Outfit-Plugins weiterhin vollständig in der Lage sein, den Avatar anzuziehen und Outfits zu verwenden.

#### Outfit-Funktionen im Core

Zum Viewer-Core gehören mindestens:

```text
Wearables anziehen / ausziehen
Attachments anlegen / abnehmen
aktuelles Outfit erkennen
Outfit speichern
Outfit laden / wechseln
Inventar-/Outfit-Ordner-Verknüpfung
serverseitige Appearance-/Outfit-Synchronisation
Basisdarstellung der getragenen Wearables und Attachments
```

Diese Funktionen dürfen niemals davon abhängen, dass `OGL Outfit Manager` oder `OGL Outfit Preview` installiert ist.

#### OGL Outfit Manager Plugin

`OGL Outfit Manager` erweitert die Core-Outfit-API um Komfortfunktionen:

```text
Outfit-Galerie / erweiterte Liste
Kategorien
Tags
Favoriten
Suche und Filter
Notizen pro Outfit
zuletzt getragen
erweiterte Attachment-/Wearable-Übersicht
fehlende Outfit-Bestandteile erkennen
lokale Metadaten pro Outfit
```

Das Plugin speichert nur Zusatzmetadaten. Die eigentliche Outfit-/Appearance-Wahrheit bleibt im Core beziehungsweise im OpenSim-Inventar-/Appearance-System.

#### OGL Outfit Preview Plugin

`OGL Outfit Preview` ist für visuelle Outfit-Vorschauen zuständig. Vorgesehene Funktionen:

```text
Screenshot manuell einem Outfit zuweisen
beim Speichern optional automatisch Vorschau erzeugen
Outfit-Galerie mit Vorschaubildern
Kamera-Presets für Ganzkörper-/Portrait-Aufnahmen
Vorschaubild ersetzen / löschen
mehrere Vorschaubilder optional später
lokale Thumbnail-/Preview-Verwaltung
```

Die Vorschau ist Zusatzinformation und darf das Outfit selbst nicht verändern.

#### Speicherung der Outfit-Previews

Vorschauen werden standardmäßig lokal im Benutzerprofil gespeichert und über eine stabile Outfit-ID beziehungsweise einen stabilen Inventar-/Outfit-Schlüssel referenziert. Beispiel:

```text
user-data/
└── outfits/
    ├── metadata.json
    └── previews/
        ├── <outfit-id>.png
        └── ...
```

Die genaue Dateistruktur darf bei der Implementierung angepasst werden. Sie muss plattformunabhängig sein und darf keine absoluten benutzerspezifischen Pfade im Outfit-Metadatensatz speichern.

#### Screenshot-Erstellung

Für automatische Outfit-Screenshots benötigt das Preview-Plugin ausschließlich freigegebene APIs für Kamera, Frame Capture und Outfit-Identifikation. Es darf nicht direkt auf private Renderer-Datenstrukturen zugreifen.

Vorgesehener Ablauf:

```text
Outfit speichern / auswählen
→ Outfit-ID ermitteln
→ optional Kamera-Preset anwenden
→ Frame Capture anfordern
→ Preview-Datei speichern
→ Preview mit Outfit-ID verknüpfen
→ Galerie aktualisieren
```

Später kann optional eine Cloud-/Sync-Funktion ergänzt werden. Sie gehört nicht zur ersten Outfit-Plugin-Version und darf nur mit ausdrücklicher Benutzeraktivierung Bilder hochladen. Standard ist lokale Speicherung.

### Reihenfolge für die ersten echten Module

Die Module werden nicht in der Reihenfolge der obigen Liste vollständig umgesetzt. Für die Validierung der Module Runtime gilt zunächst folgende Priorität:

```text
1. OGL Example Module
2. OGL Developer Tools
3. OGL Performance Monitor
4. OGL Vulkan Renderer
5. OGL RTX Renderer
```

Erst wenn diese Module die API-/ABI-, Lifecycle-, Capability- und Dependency-Grenzen praktisch bestätigt haben, werden weitere tiefe Module wie Voice, Media, AO oder RLVa aus dem Core heraus beziehungsweise als optionale Erweiterung umgesetzt.

### Grundsatz

Ein Plugin darf nur Funktionen übernehmen, die über eine stabile, versionierte Schnittstelle erreichbar sind. Kernfunktionen wie Login, LLUDP/CAPS, WorldModel, Basis-Avatar, Basis-Inventar, Basis-UI und mindestens ein funktionsfähiger Standard-Renderer bleiben unabhängig von optionalen Plugins verfügbar.

---

# 3. Projektphasen

## Phase 0 – Projektgrundlage

Ziel: Das Repository kann auf Windows, Linux und macOS kompiliert und getestet werden.

Aufgaben:

- MPL-2.0-Lizenz
- CMake-Projekt
- CMakePresets
- vcpkg-Konfiguration
- Basis-Verzeichnisstruktur
- Viewer-Versionierung
- Logging
- Fehlerbehandlung
- Konfigurationssystem
- Unit-Test-Grundlage
- GitHub Actions für Linux, Windows und macOS
- Debug- und Release-Builds

Ergebnis:

```text
nexverse-viewer --version
```

liefert beispielsweise:

```text
NexVerse Viewer 0.1.0-alpha.1
```

Status: ALS NÄCHSTES

---

## Phase 1 – OpenSim Grid Discovery

Ziel: Der Viewer erkennt einen OpenSimulator-Gridserver und verwendet beim Start immer die aktuell vom NexVerse-Grid veröffentlichten Grid-Daten.

### Verbindlicher NexVerse Discovery-Endpunkt

Für NexVerse ist die feste Grid-Adresse:

```text
hg.stadt-nexverse.de
```

Der Discovery-Endpunkt lautet:

```text
http://hg.stadt-nexverse.de/get_grid_info
```

Dieser Endpunkt ist die maßgebliche Quelle für die Grid-Konfiguration des Viewers.

Verbindliche Produktregel für den NexVerse-Build:
- Der Discovery-Endpunkt ist fest im Viewer verdrahtet und kann in der normalen Anwendung nicht geändert werden.
- Es gibt keine Grid-Auswahl und kein Eingabefeld für eine alternative Login-/Grid-Adresse.
- Bei jedem Start wird ausschließlich `http://hg.stadt-nexverse.de/get_grid_info` neu abgefragt.
- Die vom Server aktuell gelieferte `login`-URI und die übrigen GridInfo-Werte werden anschließend verwendet.
- Ändert sich die XML auf dem NexVerse-Server, wird die Änderung beim nächsten Start automatisch übernommen.

### Verhalten bei jedem Viewer-Start

Bei jedem Start des NexVerse Viewers muss vor dem eigentlichen Login automatisch `get_grid_info` abgerufen werden.

Ablauf:

```text
Viewer startet
      |
      v
GET http://hg.stadt-nexverse.de/get_grid_info
      |
      v
XML sicher parsen und validieren
      |
      v
Änderungen gegenüber lokalem Cache erkennen
      |
      v
aktuelle GridInfo übernehmen
      |
      v
Login-Oberfläche / Login-Endpunkt aktualisieren
      |
      v
Benutzer kann sich anmelden
```

Wichtig:

- Die GridInfo darf nicht dauerhaft hart im Viewer dupliziert werden.
- Bei jedem Start wird die XML neu vom Server geladen.
- Änderungen an der XML müssen ohne neues Viewer-Release wirksam werden.
- Der Viewer verwendet immer die aktuell erfolgreich validierten Serverwerte.
- Relevante Änderungen werden im Log protokolliert, jedoch ohne Zugangsdaten oder andere sensible Werte.
- Die letzte erfolgreich validierte GridInfo wird lokal als Cache gespeichert.
- Ist der Discovery-Endpunkt vorübergehend nicht erreichbar, darf der Viewer die letzte gültige gecachte Version verwenden und muss den Offline-/Fallback-Zustand sichtbar kennzeichnen.
- Eine fehlerhafte, unvollständige oder nicht parsebare XML darf niemals eine zuvor gültige Konfiguration überschreiben.
- Netzwerkantworten erhalten Timeout- und Größenlimits.
- Später soll HTTPS verwendet werden, sobald der Discovery-Endpunkt dafür zuverlässig bereitsteht; bis dahin wird der angegebene HTTP-Endpunkt unterstützt.
- Nach erfolgreichem Abruf werden ausschließlich die vom Server gelieferten aktuellen Werte für Login, Welcome, Economy, About, Register, Help, Password und Web-Profile verwendet, sofern vorhanden.

Implementierung:

- HTTP-Client
- URI-Parser
- `GET /get_grid_info`
- XML-Auswertung
- GridInfo-Datenmodell
- Timeouts
- TLS-Prüfung
- Antwortgrößenlimit
- Fehlerbehandlung

Daten unter anderem:

- gridname
- gridnick
- login
- welcome
- economy
- about
- register
- help
- password
- web_profile_url

CLI-Test:

```bash
./nexverse-viewer --grid-info https://grid.example.org
```

Erwartetes Ergebnis:

```text
Grid: NexVerse
Login URI: https://...
Status: compatible
```

---

## Phase 2 – OpenSim Login ✅ abgeschlossen 05.10.2026

Ziel: Ein Benutzer kann sich authentifizieren.

Implementierung:

- Login Request
- LLSD/XML-RPC/HTTP-Verarbeitung entsprechend OpenSim-Endpunkt
- sichere Credential-Verarbeitung
- Session-Datenmodell

Relevante Login-Daten:

- Agent UUID
- Session UUID
- Secure Session UUID
- Circuit Code
- Seed Capability
- Region
- Simulator IP
- Simulator UDP Port
- Inventory Skeleton
- Buddy-/Appearance-Basisdaten

Ergebnis:

```text
LOGIN OK
Agent: ...
Region: ...
Simulator: ...
```

---

## Phase 3 – LLUDP Transport ✅ abgeschlossen 05.10.2026 · NexVerse Circuit-Echttest erfolgreich

Ziel: Stabile Echtzeitverbindung zum Simulator.

Implementierung:

- UDP Socket
- Circuit-Aufbau
- OpenSim/Linden Message Template
- Packet Encoder
- Packet Decoder
- Sequenznummern
- ACK
- Resend
- Reliable Messages
- Packet Throttling
- MTU-Grenzen
- Timeout/Reconnect

Wichtig:

Dieser Layer darf noch keinerlei Renderer enthalten.

---

## Phase 4 – CAPS / HTTP Capabilities ✅ abgeschlossen 05.10.2026 · NexVerse-Echttest erfolgreich

Ziel: Der Viewer verwendet die vom Simulator bereitgestellten Capability-Endpunkte.

Implementierung:

- Seed Capability
- Capability Discovery
- HTTP GET/POST
- LLSD XML/Binary
- EventQueue
- SimulatorFeatures

Später verwendete Bereiche:

- Inventory
- Assets
- Mesh
- Texturen
- Uploads
- Environment
- Appearance

---

## Phase 5 – WorldModel ✅ abgeschlossen · NexVerse-Echttest 06.10.2026

Ziel: Ein rendering-unabhängiges Abbild der Welt.

Struktur:

```text
WorldModel
├── Grid
├── Region
├── Agent
├── Avatar
├── Object
├── Terrain
├── Parcel
└── Environment
```

Das WorldModel wird ausschließlich durch validierte Protocol-Ereignisse aktualisiert.

---

## Phase 6 – Fenster, Input und Renderer-Grundlage ✅ abgeschlossen · visueller Echttest 06.10.2026

Ziel: Erstes echtes Viewer-Fenster.

Funktionen:

- Desktop-Fenster
- Tastatur
- Maus
- Kamera
- Resize
- VSync
- Frame Timing
- Renderer Interface
- Debug Overlay

Erste Darstellung:

- Hintergrund
- Kamera
- Testgeometrie
- FPS
- RAM
- VRAM soweit verfügbar

---

## Phase 7 – Region und Terrain 🚧 in Arbeit seit 06.10.2026

Ziel: Eine echte OpenSim-Region darstellen.

Funktionen:

- Terrain-Daten empfangen
- Terrain Mesh
- OpenSim-Terrainmaterial-Metadaten aus RegionHandshake
- echte Terrain-Detailtexturen über die entdeckte GetTexture-Capability laden
- serverseitige PNG-Konvertierung verwenden, solange kein JPEG2000-Decoder im Viewer benötigt wird
- Terraintexturen asynchron laden und mit sicherem Farb-Fallback darstellen
- Wasser
- Region-Grenzen
- Umgebung
- OpenSim-`EnvironmentSettings`/Legacy-WindLight für Himmel, Sonne, Haze und Wasser
- moderne `ExtEnvironment`/EEP-Unterstützung mit Capability-Priorität, Legacy-Fallback sowie zeitabhängiger zyklischer Daycycle-Auswahl und Interpolation
- progressive Nachladung
- Viewer startet standardmäßig als maximiertes normales Fenster mit nativer Titelleiste und Minimize/Maximize/Close; echter exklusiver Fullscreen nur optional per F11
- UI muss 16:9, 21:9 und 32:9/Ultrawide ohne breite feste Dashboard-Spalten unterstützen
- nach erfolgreichem Login folgt ein eigener Ladebildschirm mit Fortschrittsbalken
- automatischer Zustandswechsel: Login -> Loading -> In-World
- vollständige Navigation/HUD erst bei `WorldModel READY`
- In-World ist die 3D-Welt die Hauptansicht; Dashboard-Flächen dürfen sie nicht verdecken
- In-World-UI nur als kompakte transparente HUD-/Toolbar-Overlays
- die visuelle Designsprache bleibt verbindlich NexVerse/OpenGenesisLINK: Schwarz/Dunkelblau, Cyan/Blau sowie Violett/Magenta
- bestehendes NexVerse-Branding, Kartenstil, Statuschips, Navigation und Toolbar-Stil bleiben maßgebend
- Firestorm darf nur als technische Funktionsreferenz verwendet werden; Firestorm-Skin/XUI/Design darf nicht übernommen werden
- UI-Skalierung muss den eigenen NexVerse-Stil auf 16:9, 21:9 und 32:9 erhalten

---

## Phase 8 – Prims und Scene Objects

Ziel: OpenSim-Objekte sichtbar machen.

Funktionen:

- ObjectUpdate
- Transform
- Rotation
- Scale
- Parent/Child
- Linksets
- Primitive Types
- Object Lifecycle
- Culling

Aktueller Implementierungsstand:
- Full ObjectUpdate liefert Position, Quaternion, Scale, PCode und Parent-ID ins WorldModel
- Scene-Revision überträgt Änderungen gezielt an OGLRender
- Primitive-Shape-Parameter (Path/Profile) werden aus ObjectUpdate dekodiert und im WorldModel erhalten
- sichtbare Grundformen für Box, Zylinder, Prisma, Kugel und Torus vorhanden
- Parent/Child-Transformbasis und Distanz-Culling aktiv
- Compressed-/Terse-Updates erhalten bekannte statische Shape-/Scale-/Parent-Daten
- Taper, Shear, Twist, Path Begin/End sowie Profile Begin/End und Hollow für lineare Prims sind aktiv; Circular-Sweep-Pfade berücksichtigen Revolutions, RadiusOffset, Skew und PathScaleX/Y
- TextureEntry-Default-Face, echte Objekttexturen und vollständige per-Face-Override-Dekodierung sind aktiv; Statische Box-/Cylinder-/Prism- sowie dynamische Sweep-Facegruppen sind vorbereitet; als nächstes: verifizierte Material-/RenderMaterial-Assetauflösung und anschließend schrittweiser PBR-Ausbau

Zunächst einfache Materialien, danach vollständige Texturen/PBR.

---

## Phase 9 – Asset-System

Ziel: Assets sicher und speichereffizient laden.

Asset-Typen:

- Textures
- Meshes
- Animations
- Sounds
- Wearables
- Notecards
- Scripts nur als Inventarinhalt
- weitere OpenSim Asset-Typen

Cache:

```text
RAM Asset Cache
      |
      v
Disk Cache
      |
      v
OpenSim Asset Service / CAPS
```

Vorgaben:

- LRU/ähnliche Eviction
- Größenlimit
- Content-Hash/Versionierung
- keine vollständige Inventar-Vorladung
- sichtbare Assets priorisieren

---

## Phase 10 – Avatar

Ziel: Eigener und fremde Avatare darstellen.

Funktionen:

- Avatar Skeleton
- Shape
- Skin
- Wearables
- Attachments
- Bakes
- Animationen
- Name Tags
- Sit/Stand

---

## Phase 11 – Bewegung

Ziel: Avatar-Steuerung.

Funktionen:

- WASD
- Laufen
- Rennen
- Springen
- Fliegen
- Sitzen
- Kamera
- serverautoritatives Movement
- später Client Prediction / Reconciliation

---

## Phase 12 – Inventar

Funktionen:

- Ordner
- Items
- Suche
- Drag & Drop
- Rez
- Wear
- Attach
- Delete
- Create
- Rename
- Move
- Asset Referenzen

---

## Phase 13 – Kommunikation

Funktionen:

- Local Chat
- IM
- Offline Messages
- Notifications
- Block/Mute
- Freunde
- Presence

---

## Phase 14 – Teleport und Regionen

Funktionen:

- Teleport
- Home
- Landmarks
- Region Crossing
- Map
- Destination Handling
- Fehler-Recovery

---

## Phase 15 – Gruppen und Profile

Funktionen:

- Profile
- Friends
- Groups
- Rollen
- Einladungen
- Notices
- Group Chat

---

## Phase 16 – Build Tools

Funktionen:

- Rez
- Move
- Rotate
- Scale
- Link/Unlink
- Texture
- Material
- Permissions
- Object Properties

---

## Phase 17 – Land / Parcels

Funktionen:

- Parcel Grenzen
- Parcel Informationen
- Access Lists
- Ban Lists
- Media
- Music URL
- Land Settings
- Terrain Editing

---

## Phase 18 – Hypergrid

Ziel: Kompatible Reisen in andere OpenSim-Grids.

Funktionen:

- Hypergrid URI
- Gatekeeper
- Foreign Agent
- Foreign Inventory
- Asset Transfer
- Teleport
- Identity Handling

---

## Phase 19 – Voice

Voice wird als Adapter implementiert und nicht fest in den World-/Protocol-Core eingebaut.

```text
Voice API
├── Provider A
├── Provider B
└── später NexVerse Voice
```

---

## Phase 20 – NexVerse Integration

Optionale NexVerse-spezifische Funktionen:

- Bürgerportal
- Events
- Marketplace
- Support
- Update Service
- News
- Regionsuche
- NexVerse Account Services

Diese Funktionen dürfen die normale OpenSim-Kompatibilität nicht beschädigen.

---

## Phase 21 – Optimierung

Speicher:

- Texture Budget
- Mesh Budget
- Asset Cache Budget
- Region Cache
- Avatar Complexity
- HTTP Buffer Limits
- UI Memory
- Hintergrund-Regionen entladen

Performance:

- Frustum Culling
- Occlusion
- LOD
- Instancing
- Texture Streaming
- Mesh Streaming
- Multithreading
- Worker Queues

Messwerte:

- RAM
- VRAM
- CPU
- GPU
- Netzwerk
- Frame Time
- Asset Download Rate

---

## Phase 22 – Release

Release-Kanäle:

- alpha
- beta
- stable

Plattformpakete:

- Windows Installer
- Linux AppImage oder vergleichbares Paket
- macOS App Bundle / DMG

---

# 4. Branch-Modell

Nach dem ersten Commit:

```text
main
  |
  +-- develop
       |
       +-- feature/grid-info
       +-- feature/login
       +-- feature/lludp
       +-- feature/caps
       +-- ...
```

Regeln:

- `main` = stabiler Stand
- `develop` = aktuelle Entwicklung
- Features in eigenen Branches
- Pull Request nach `develop`
- Releases von `develop` nach `main`

---

# 5. Versionierung

Schema:

```text
MAJOR.MINOR.PATCH-STAGE.N
```

Beispiele:

```text
0.1.0-alpha.1
0.1.0-alpha.2
0.2.0-alpha.1
0.9.0-beta.1
1.0.0
```

---

# 6. Lokales Kompilieren

## Linux – Debian/Ubuntu

### Grundpakete

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  ninja-build \
  git \
  curl \
  zip \
  unzip \
  tar \
  pkg-config
```

vcpkg einmalig installieren:

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

Umgebungsvariable setzen:

```bash
export VCPKG_ROOT="$HOME/vcpkg"
```

Optional dauerhaft:

```bash
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc
source ~/.bashrc
```

Repository holen:

```bash
cd ~
git clone https://github.com/marlin198323-glitch/nexverse-viewer.git
cd nexverse-viewer
```

Später den Entwicklungsbranch verwenden:

```bash
git checkout develop
git pull
```

Debug konfigurieren:

```bash
cmake --preset debug
```

Kompilieren:

```bash
cmake --build --preset debug
```

Tests:

```bash
ctest --preset debug --output-on-failure
```

Viewer starten:

```bash
./build/debug/nexverse-viewer
```

Release Build:

```bash
cmake --preset release
cmake --build --preset release
ctest --preset release --output-on-failure
```

---

## Windows 11

Benötigt:

- Visual Studio 2022 oder neuer
- Desktop development with C++
- CMake
- Git
- vcpkg
- Windows SDK

PowerShell:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

$env:VCPKG_ROOT="C:\vcpkg"

git clone https://github.com/marlin198323-glitch/nexverse-viewer.git
cd nexverse-viewer

git checkout develop

cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

Start:

```powershell
.\build\debug\nexverse-viewer.exe
```

Release:

```powershell
cmake --preset release
cmake --build --preset release
ctest --preset release --output-on-failure
```

---

## macOS

Benötigt:

```bash
xcode-select --install
brew install cmake ninja git
```

vcpkg:

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT="$HOME/vcpkg"
```

Viewer:

```bash
git clone https://github.com/marlin198323-glitch/nexverse-viewer.git
cd nexverse-viewer
git checkout develop

cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

---

# 7. Nach jedem Update testen

Linux:

```bash
cd ~/nexverse-viewer

git checkout develop
git pull

cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure

./build/debug/nexverse-viewer
```

Wenn nur kleine Änderungen vorhanden sind, muss CMake nicht jedes Mal alles neu kompilieren. Der Build ist inkrementell.

---

# 8. Clean Build

Wenn Abhängigkeiten oder CMake-Konfigurationen geändert wurden:

Linux/macOS:

```bash
rm -rf build
cmake --preset debug
cmake --build --preset debug
```

Windows PowerShell:

```powershell
Remove-Item -Recurse -Force build
cmake --preset debug
cmake --build --preset debug
```

---

# 9. Fehlerberichte beim Testen

Für jeden Test benötigen wir mindestens:

```text
Betriebssystem:
CPU:
RAM:
GPU:
Viewer-Version:
Commit:
Grid:
Region:

Fehler:
...

Letzte Logzeilen:
...
```

Später bekommt der Viewer dafür eine automatische Diagnosefunktion.

---

# 10. Aktueller Entwicklungsrahmen und nächste Arbeit

## 10.1 Unmittelbar nächste Arbeit

Der frühere Phase-0-Startplan ist abgeschlossen und wird durch den aktuellen Arbeitsstand ersetzt.

Die unmittelbar nächsten Arbeitsschritte sind:

1. ✅ Module Runtime / ModuleManager und native C-ABI v1 inklusive Manifest, Lifecycle, Abhängigkeiten, Aktivierung und Beispielmodul (`0.2.0-alpha.38`).
2. ✅ Ergänzende kompatible Diagnose-ABI v2 mit read-only Snapshot und Frame-Callback (`0.2.0-alpha.39`).
3. ✅ OGL Developer Tools und OGL Performance Monitor als erste funktionsfähige, optionale Testmodule (`0.2.0-alpha.39`).
4. **JETZT: Phase 8 Rendering** weiterführen, Prim-/Material-/Scene-Funktionalität vervollständigen.
5. Anschließend Meshes, Sculpties, Partikel und Lichtquellen ausbauen.
6. Avatar, Animation, Attachments, Inventar, Outfits, Chat/IM/Freunde/Gruppen, Teleport/Map und Build/Edit schrittweise alltagstauglich machen.
7. Erst danach die größeren NextGen-Blöcke Vulkan, RTX, Weather, Advanced Water/Hydro-Anbindung und weitere Plugins ausbauen.
8. Produktions-Plugin-Sicherheit (Signaturen, Prüfsummen, Isolation und Benutzer-UI) gesondert abschließen, bevor Drittmodule freigegeben werden.

## 10.2 Planungsrahmen für die Entwicklungsdauer

Die folgenden Zeiträume sind **grobe Planungswerte**, keine festen Termine oder Garantien. Sie hängen insbesondere von Testtiefe, Protokollproblemen, Plattformunterschieden, OpenSim-Kompatibilität, Nutzerfeedback, Fehlerkorrekturen und dem tatsächlichen Entwicklungsumfang ab.

Grobe Einzelblöcke:

```text
Module-System-Grundlage                 ca. 1-3 Wochen
Phase 8 Rendering/Prims/Materialien     ca. 3-6 Wochen
Meshes/Sculpties/Partikel/Lichter       ca. 1-2 Monate
Avatar/Bewegung/Animation/Attachments   ca. 1-2 Monate
Inventar/Outfits/Appearance             ca. 1-2 Monate
Chat/IM/Freunde/Gruppen/Profile         ca. 1-2 Monate
Teleport/Map/Minimap/Landmarks          ca. 3-6 Wochen
Build/Edit/Parcel/Estate                 ca. 1-2 Monate
Media/Audio/Voice-Grundfunktionen       ca. 1-2 Monate
Text-only/Settings/Diagnose/Polishing   ca. 1 Monat
Vulkan NextGen Renderer                 ca. 2-4 Monate
RTX/Weather/Hydro/Advanced Water        zusätzliche mehrere Monate
```

Diese Blöcke laufen teilweise überlappend und werden nicht zwingend streng nacheinander umgesetzt.

## 10.3 Produkt-Meilensteine

### Nutzbare Alpha

Ziel: Der Viewer kann sinnvoll in NexVerse genutzt und getestet werden.

Mindestens:

```text
Login
Welt sichtbar
Avatar sichtbar und steuerbar
Chat
Teleport
Basisinventar
Basisoutfits
Region-/Environment-Darstellung
stabile Grundnavigation
```

Planungsrahmen: etwa **3-6 Monate** ab aktuellem Stand bei kontinuierlicher Entwicklung und regelmäßigem Testen.

### Alltagstaugliche Beta

Ziel: Der OGL Viewer kann für einen großen Teil der normalen täglichen Nutzung eingesetzt werden.

Zusätzlich unter anderem:

```text
Inventar weitgehend vollständig
Appearance/Outfits
IM/Freunde/Gruppen
Map/Minimap/Landmarks
Build/Edit
Parcels
Media/Audio-Grundfunktionen
stabilere Performance
Einstellungen
Diagnose
plattformübergreifende Builds
```

Planungsrahmen: ungefähr **6-9 Monate** für eine brauchbare Beta.

### Weitgehender Firestorm-Ersatz im Alltag

Ziel: Die wesentlichen Viewer-Funktionen sind so vollständig und stabil, dass OGL im normalen NexVerse-/OpenSim-Alltag Firestorm weitgehend ersetzen kann.

Planungsrahmen: ungefähr **9-15 Monate**.

### Langfristige vollständige OGL-Plattform

Enthält zusätzlich die großen Zukunftsblöcke:

```text
Vulkan NextGen Renderer
modernes PBR
RTX / Raytracing
modernes Wasser
OGL Weather
Text-only Modus
Plugin-/Add-on-Ökosystem
Outfit Preview
Advanced AO/RLVa/Voice/Media
moderne Avatar-Darstellung
Performance-/Streaming-Systeme
Windows/Linux/macOS Stabilisierung
spätere HydroEngine-Viewer-Anbindung
```

Planungsrahmen für dieses Gesamtziel: ungefähr **12-24 Monate**.

## 10.4 Entwicklungsprinzip

Der Viewer wird nicht erst am Ende nutzbar gemacht. Die Entwicklung erfolgt in nutzbaren Stufen:

```text
Alpha
→ Welt sehen
→ bewegen
→ Avatar
→ Chat
→ Inventar
→ Teleport
→ Outfits
→ Build/Edit
→ Alltagstauglichkeit
→ Beta

danach weiter:
→ Vulkan
→ RTX
→ Advanced Weather
→ Advanced Water / Hydro
→ weitere Plugins
```

Priorität hat zuerst ein stabiler, täglich nutzbarer OpenGenesisLINK Viewer. High-End-Grafik und optionale Erweiterungen werden anschließend auf der bereits vorbereiteten Architektur ausgebaut.


## 10.5 Verbindliche Chat-Übergabe

Für einen Wechsel in einen neuen Entwicklungs-Chat wird im Repository die Datei `CHATUEBERGABE.md` gepflegt.

Ein neuer Chat steigt in dieser Reihenfolge ein:

```text
CHATUEBERGABE.md
→ projectplan.md
→ info.md
→ docs/ARCHITECTURE.md
→ docs/DESIGN.md
→ git status / git log / VERSION
```

`projectplan.md` bleibt für Architektur, Reihenfolge und Umfang maßgebend. `info.md` dokumentiert den tatsächlichen Implementierungsstand. `CHATUEBERGABE.md` bündelt den aktuellen Wiedereinstiegspunkt und darf den Projektplan nicht überschreiben.

Nach jedem größeren getesteten Entwicklungsblock werden mindestens `info.md` und `CHATUEBERGABE.md` aktualisiert. Veraltete Statusangaben in README oder den Architektur-/Design-Dokumenten werden im selben Dokumentationsblock korrigiert.

Wenn der Benutzer in einem neuen Chat nur `viewer` oder `weiter` schreibt und der nächste Arbeitsschritt aus der Übergabe eindeutig ist, wird ohne unnötige Rückfrage am aktuellen lokalen Repository-Stand weitergearbeitet.


## Entwicklungsstand Module Runtime – 09.10.2026

`0.2.0-alpha.38` implementiert den ersten lokalen Module-Runtime-Basisblock
entsprechend Abschnitt „Sofortiger Modul-System-Basisblock“:

- öffentliche native C-ABI v1, Host-Callback und Lifecycle;
- validierter JSON-Manifest-Lader, Plattform-/Architektur-, ABI- und Viewer-Version;
- lokale Module Discovery, deklarierte Capability-Gates, Dependency- und Zyklusprüfung;
- Aktivierung/Deaktivierung, persistenter Status, native Bibliotheksladung,
  Fehler-/Rollback-Pfad und umgekehrter Stop-Lifecycle;
- OGL Example Module mit eigenständigem CMake-Build und automatisierten Tests.

Build/Tests: 20/20 grün, OpenGL-Runtime-Smoke erfolgreich.

**Nicht abgeschlossen** sind Produktions-Signaturprüfung, Hashverifikation,
Sandbox-/Prozessisolation, Capability-Durchsetzung jenseits der Host-ABI,
Modulverwaltungs-UI, Registry, Downloads, Updates und plattformübergreifende
Runtime-Abnahmetests. Native unsignierte Module bleiben daher im Normalbetrieb
blockiert; Developer Mode bedarf ausdrücklicher Aktivierung.

Der nächste Entwicklungsblock ist **OGL Developer Tools**, danach
**OGL Performance Monitor** zur praktischen Validierung erweiterter Host-APIs.
Anschließend Phase 8 fortsetzen. Architektur- und Sicherheitsdetails:
`docs/MODULES.md`. Bestehende langfristige Zielanforderungen bleiben unverändert.


## Phase 8 – Legacy Shiny-Fallback · 09.10.2026 · 0.2.0-alpha.40

- OpenGL-Fragmentshader berücksichtigt OpenSim-Shiny-Stufen 0–3 erstmals
  als begrenzte, oberflächenabhängige Glanzlichtapproximation.
- Default- und per-Face-Materialien führen Shiny an den Shader weiter.
- Fullbright-Flächen erhalten keinen zusätzlichen Glanzlicht-Effekt;
  inaktive Objekt-/Terrainpässe setzen die Uniform auf 0 zurück.
- Bumpiness-Werte werden weiterhin per Material transportiert und begrenzt,
  aber noch nicht als Normal-/Bump-Map gerendert.
- Dies ist ein Legacy-Compatibility-Fallback, kein vollwertiges PBR.
- Build und 21/21 CTest grün. Der nächste Phase-8-Schritt behandelt
  Material-ID-/RenderMaterial-Auswertung und weitere Facegruppen.


## Phase 8 – Alpha-Passes und Material-IDs · 09.10.2026 · 0.2.0-alpha.41

- Object-Rendering in zwei Durchläufe getrennt: zuerst deckende Prim-Faces
  mit Depth-Write, anschließend transparente Faces mit Alpha-Blending und
  deaktiviertem Depth-Write.
- Transparente Objekte werden mit stabiler Reihenfolge nach Abstand zum
  Kamerastandpunkt (fern nach nah) gezeichnet; das WorldModel bleibt unverändert.
- RGBA8-Texturen werden beim GPU-Upload einmalig auf Alpha < 255 geprüft.
  Vollständig deckende Texturen bleiben nun im Opaque-Pass statt pauschal
  als transparent gerendert zu werden.
- Vollständig transparente Fragmente werden im Legacy-GLSL verworfen.
- Die vom TextureEntry gelieferten Default-Material-ID und
  RenderMaterial-ID gehen jetzt auch beim Scene-Object-Transfer nicht verloren;
  die bereits vorhandene Vererbung/Override-Auswertung erhält einen
  erweiterten Regressionstest.
- Neues `NexVerse::Render`-CPU-Modul `material_pass` für testbare
  Alpha-Klassifikation und stabile Sortierung; neuer CTest.
- CMake Debug-Build, 22/22 Tests und OpenGL Runtime-Smoke erfolgreich.
  Welcome/GL initialisieren; ein eingeloggter In-World-Echttest steht aus.
- Grenzen: kein PBR-/Legacy-Material-Asset-Fetch, keine Normalmaps,
  keine exakte Sortierung einzelner transparenter Dreiecke/Faces,
  kein Order-Independent Transparency. Material-/RenderMaterial-UUIDs
  werden bewahrt, aber **noch nicht** als Materials aufgelöst.
- Als Nächstes: überprüfte Material-CAP/Asset-Resolver-Anbindung und
  vollständigeres Face-/Materialhandling, dann weitere Phase-8-Objekttypen.


## Phase 8 – echter Legacy RenderMaterials-CAPS-Pfad · 09.10.2026 · 0.2.0-alpha.42

- Verifiziert an OpenSimulator `MaterialsModule.cs`: die `RenderMaterials`-
  Capability liefert als GET eine `Zipped`-Binary in LLSD-XML;
  darin liegt eine zlib-komprimierte LLSD-Binary-Array mit
  `ID` (16-Byte UUID) und `Material` (LLSD-Map).
- Eigener Protocol-Parser und `RenderMaterialsService` (asynchrones GET
  nach CAPS Discovery), mit begrenzten komprimierten/entpackten Größen,
  8192 Max-Entries, UUID- und Integer-Prüfung, Ablehnung ungültiger Daten.
- Legacy `DiffuseAlphaMode`/ `AlphaMaskCutoff` werden per Material-ID
  an Default-/per-Face-Renderdaten gebunden. Material-Cache-Update erzwingt
  Scene-Neuübertragung ohne Änderung des serverautoritativ gespeicherten Models.
- OpenGL-Shader: Alpha-Mode 0/1/2/3, maskiertes `discard`, Opaque-
  Depthpass für Mask-Faces ohne Tint-Alpha, emissives leichtes Fallback.
- Normal-/Specular-Map-UUIDs werden eingelesen, aber nicht als Texturen
  geladen. Moderne `RenderMaterialID`/PBR werden nicht vorgetäuscht.
- 23/23 CTest erfolgreich; Debug-Build und OpenGL-Runtime-Smoke erfolgreich
  (Welcome geladen, Shader initialisiert). In-World-Sichtprüfung offen.
- Verbleibend: GET-Snapshot ist nicht live aktualisiert; POST-Batch-Abfragen
  bei neuen MaterialIDs, echte Normal-/Specular-Maps, PBR/glTF-Material-
  Assets, Face-/Triangle-Transparenzsortierung und In-World-Tests.
- Siehe `docs/MATERIALS.md` für genaue Grenzen und Schnittstellen.


## Phase 8 – gezielte Legacy-Material-Nachladung · 09.10.2026 · 0.2.0-alpha.43

- An der OpenSimulator-`RenderMaterialsPostCap`-Referenz geprüft:
  POST mit LLSD-XML-`Zipped`, zlib und binärem LLSD-Array aus
  16-Byte-Material-UUIDs; Response verwendet das bestehende
  `ID`/`Material`-Format.
- `RenderMaterialsService::fetch_ids()` und
  `build_render_materials_request()`: höchstens 32 Material-IDs
  je POST-Batch, UUID-Prüfung, Deduplikation, Begrenzungen des
  komprimierten Payloads und ausschließlich angeforderte IDs
  aus der Antwort übernehmen.
- Viewer sammelt Material-IDs aus Default- und per-Face-Renderdaten,
  überspringt Null- und ungültige IDs, wartet auf den initialen
  GET-Snapshot, startet dann asynchrone, sequenzielle POST-Batches.
- Ein erfolgreicher Materialwechsel aktualisiert den lokalen Cache
  und invalidiert die gerenderte Scene-Revision; unveränderte
  Einträge lösen keinen unnötigen Scene-Upload aus.
- Wiederholungsversuche für unbekannte IDs sind auf maximal drei
  Versuche mit zwei Minuten Abstand begrenzt. Ein bekanntes Material
  kann bei späterem Scene-Update nach Ablauf der Schutzfrist
  erneut angefragt werden.
- 23/23 CTest erfolgreich, einschließlich neuer POST-Request-
  Validierung (Payload-Struktur, Deduplikation, URL-/Größenfilter,
  Antwort-Whitelist).
- Ausstehend: Region-/Teleport-übergreifender Material-Lifecycle,
  kontinuierliche serverseitige Materialänderungen ohne Scene-Update,
  echte Normal-/Specular-Texturen und glTF/PBR.

- Laufzeit-Smoke 5s bestätigt OpenGL 3.3, RTX 4070 SUPER, NexVerse-UI und
  geladenen Welcome-Hintergrund; `timeout`-Exit 124 erwartet.


## Phase 8 – Legacy Normal/Specular Mapping · 09.10.2026 · 0.2.0-alpha.44

- OpenSimulator `SOPMaterial.cs` als Protokollreferenz geprüft:
  `NormMap`/`SpecMap`, `Norm/SpecOffsetX/Y`, `RepeatX/Y`,
  `Rotation` werden per 1/10000 skaliert; dazu `SpecColor`,
  `SpecExp` und `EnvIntensity`.
- Die Material-CAPS-Response dekodiert nun diese typisierten Werte
  mit Grenzen und Ablehnung fehlerhafter numerischer Felder.
- Das Application-/Scene-Mapping übernimmt Normal-/Specular-UUIDs
  sowie beide unabhängigen UV-Transformationen. Vorhandene
  GetTexture-PNG-Downloads werden für diese Texturen wiederverwendet,
  ohne einen zweiten Asset-Transport einzuführen.
- OGLRender bindet geladene Maps pro Face auf GL-Texture-Units 5/6,
  verwendet getrennte Repeat/Offset/Rotation-Uniforms, approximiert
  Normalmapping über UV-/Oberflächen-Derivate (TBN-Basis) und ein
  einfaches Legacy-Specular-Highlight aus Map/SpecColor/SpecExp.
- Fehlen Asset oder Materialdaten, bleibt der Legacy-Shiny-Fallback
  erhalten; Fullbright bleibt von zusätzlichen Lichteffekten ausgenommen.
- Kein vollständiges PBR, keine echte Umgebungsspiegelung, keine
  garantierte Texture-/Tangent-Fidelity für alle Primformen.
- CMake Debug-Build erfolgreich, 23/23 CTest erfolgreich; OpenGL-3.3-
  Laufzeitstart auf RTX 4070 SUPER einschließlich Welcome geladen.
  Ein eingeloggter In-World-/Material-Vergleich bleibt offen.
- Als Nächstes: Material-Asset-Caching/Invalidierung beim Region- und
  Teleportwechsel, weitere Renderer-Primitiven, visuelle In-World-Tests.


## Phase 8 – regionsgebundene CAPS-/Material-Caches · 09.10.2026 · 0.2.0-alpha.45

- Das Materialsystem verwendet eine explizite Region-Identität aus
  WorldModel-Handle, Generation und Seed-Capability.
- Bei Login, TeleportFinish und CrossedRegion (inkl. gleicher Handle
  mit neuer Generation) werden Legacy-Material-Cache, offene Material-
  ID-Listen, Retry-/Cooldown-Daten und Texture-Download-Metadaten
  invalidiert; die Szene wird erneut aus dem WorldModel aufgebaut.
- Material- und GetTexture-Capabilities werden anhand der Seed-Capability
  **der jeweils aktuellen Region** separat asynchron neu entdeckt.
  Die bisherige Login-EventQueue bleibt vorerst ein separater Pfad.
- Alle asynchronen Material-GET-/POST- sowie Terrain-/Objekt-Textur-
  Anfragen sind mit einer Region-Epoche versehen. Veraltete Ergebnisse
  werden konsumiert, aber nicht mehr in die neue Region übernommen;
  laufende std::async-Futures werden nicht beim Übergang überschrieben.
- Neues Header-Modul `RegionMaterialScope` und Regressionstests für
  identische Region, Generationserhöhung, Regionshandle- und Seed-Wechsel,
  insbesondere die Zurückweisung von Antworten mit veralteter Epoche.
- **Abgrenzung:** Der vollständige Teleport-/Circuit-Reconnect,
  die Neuanmeldung der gesamten EventQueue sowie eine vollständige
  GPU-Texture-Eviction-Strategie gehören zu weiteren Entwicklungsblöcken.
  Dieser Block isoliert den **Material- und Asset-Abruf** bei Regionswechseln.
- Abnahme: Debug-Build erfolgreich, 23/23 CTest gruen,
  5-Sekunden-OpenGL-Smoke inklusive NexVerse-UI und Welcome erfolgreich.
  Ein eingeloggter In-World-Teleport-Test steht noch aus.


## Phase 8 – zuverlässiger OpenSim-GetTexture-Transport · 10.10.2026 · 0.2.0-alpha.46

**Anlass (Live-Login NexVerse 10.10.2026, 00:13 bis 00:18):**
- Login und LLUDP UDP LIVE, 17 CAPS-Endpunkte, Handshake,
  WorldModel READY, RenderMaterials GET 46 und POST, Terrain/Prim-
  Geometrie nachweislich aus der echten OpenSim-Region empfangen.
- Objekttexturen: 300 Fehlversuche, **0 Erfolgsfälle**. Auch die
  Terrain-Detailtexturen fielen auf Farb-Fallback zurück.
- Die frühere Funktion `TextureAssetLoader::fetch_png` forderte
  ausschließlich `format=png` an. OpenSimulator verwendet nativ
  JPEG2000 (`x-j2c`); eine serverseitige PNG-Konvertierung ist
  nicht immer verfügbar. Die genaue Ursache wurde zuvor nicht geloggt.

**Implementiert:**
- `openjpeg` als vcpkg-Abhängigkeit + explizites `libpng`
  (Linux/Windows/macOS-abhängige Abhängigkeiten vereinheitlicht).
- `TextureAssetLoader::fetch_rgba` fragt bevorzugt PNG ab;
  bei HTTP-/Decoderfehler einmalige Rückfall-Anfrage mit
  `format=x-j2c` für dieselbe UUID. Keine Wiederholung pro Frame;
  bestehender Cache für fehlgeschlagene UUIDs bleibt erhalten.
- Memory-Stream-basierter OpenJPEG-Decoder für J2C-/JP2-Daten,
  unterstützt RGB, RGBA, Graustufen, Alpha, 1 bis 16 Bit und
  einfache Chroma-Subsampling-/YCC-Fälle.
- Begrenzt komprimierte Downloads auf 32 MiB, Abmessungen
  auf 4096x4096, RGBA-Ausgabe auf 64 MiB und verifiziert Metadaten
  vor und nach dem Dekodieren.
- Material- und Terrain-Logs melden jetzt pro UUID die konkreten
  Ursachen von PNG- und J2C-Fehlern; erfolgreiche J2C-Rückfälle
  sind durch die Erfolgsmeldung erkennbar. CAPS-Token/URLs werden
  nicht in Warnmeldungen ausgegeben.
- Erweiterte Tests mit echten 16x16 JPEG2000 J2C/JP2/RGBA-Fixtures,
  fehlerhaftem PNG, 404-HTTP-Fallback und defekten J2C-Payloads.

**Grenzen und offene Abnahme:**
- CMake Debug-Build, 23/23 CTest und 6-Sekunden-OpenGL-3.3-Smoke
  mit RTX 4070 SUPER, NexVerse-UI und Welcome erfolgreich.
  End-to-End-Nachweis mit authentifiziertem NexVerse-Grid und
  sichtbaren Texturen nach Neustart der Anwendung weiterhin offen.
- Die wiederkehrenden CAPS-EventQueue-Poll-Fehler gehören
  in den nächsten separaten Protokollblock, nicht zur Texturdekodierung.
- Progressive JPEG2000-LOD, Asset-Priorisierung, Retry-/LRU-
  Strategie, harte GPU-Speicherverwaltung und modernes PBR folgen.


## CAPS – EventQueue-Lifecycle und Recovery · 10.10.2026 · 0.2.0-alpha.47

**Live-Befund aus 0.2.0-alpha.46:**
- Echtes Login, EventQueue-Live-Batch, 287+ geladene Objekttexturen
  und WorldModel READY. Anschließend CAPS-Poll-Warnungen im
  Abstand von ~36–40 Sekunden bei einem früheren HTTP-Timeout von
  **35 Sekunden**.
- OpenSimulator setzt den EventQueue-Keepalive auf 60 Sekunden
  und hält Polls etwa 58 Sekunden offen, bevor eine leere Queue
  mit HTTP **502 Bad Gateway** quittiert wird. Dieser Status ist
  eine *reguläre* Leermeldung, kein verlorenes Ereignis.
  Referenz: `OpenSim/Region/ClientStack/Linden/Caps/EventQueue/EventQueueGetModule.cs`
  (`VIEWERKEEPALIVE=(60-2)*1000`, `NoEvents` gibt 502 zurück).

**Implementiert:**
- Spezifischer EventQueue-HTTP-Total-Timeout: **80 Sekunden**,
  Connect-Timeout weiterhin 8 Sekunden.
- Unterscheidung:
  - HTTP 200 + gültiges LLSD → Batch und Acknowledgement.
  - HTTP 502 → erwartetes Idle; keine Warnung, Ack erhalten,
    Poll erneut senden; kleines 150-ms-Hotloop-Schutzintervall.
  - HTTP 404 → Endpoint abgelaufen; sofort Seed-CAPS neu abrufen.
  - Andere HTTP-/Transport-/LLSD-Fehler → Diagnose mit HTTP-
    Status, Fehlermeldung, Retry-Zähler und Ack-Zustand.
- Gesunde Batch-/Idle-Antworten resetten die Fehlerzählung.
  Echte Fehler erhalten begrenzte exponentielle Backoffs
  von 1, 2, 4, 8 bis maximal 12 Sekunden.
- Bei jeder dritten echten Störung wird `EventQueueGet`
  aus dem ursprünglichen Login-Seed neu angefragt.
  Ist die Capability-URI unverändert, bleibt der Ack erhalten.
  Bei geändertem Endpoint wird er zurückgesetzt, weil es
  eine andere Queue ist. Werte und Tokens werden nicht geloggt.
- CAPS-Status: `CAPS LIVE`, `CAPS IDLE`, `CAPS RETRY`,
  `CAPS DEGRADED` mit Wiederherstellungsmeldungen.
- Retry-Pausen sind in 100-ms-Schritten stoppbar.

**Testabnahme:**
- CMake Debug-Build erfolgreich, 23/23 automatisierte CTests
  inkl. HTTP 502, 404, 503, Timeout, fehlerhafter LLSD-Antwort,
  Ack-Erhalt, Wiederverbindungs-/Backoff-Entscheidung.
- OpenGL-3.3-Smoke auf NVIDIA RTX 4070 SUPER mit NexVerse-UI
  und Welcome erfolgreich; absichtliches Timeout des Smoke-
  Tests ist kein Absturz.
- **Neuer echter Alpha.47-EventQueue-Langzeittest nach Login
  ausstehend.** Das beobachtete alte 35-Sekunden-Intervall
  sollte nicht wieder als Fehler auftreten.

**Noch offen:**
- Vollständiger Teleport/Regionsübergang inklusive UDP-Circuit-
  Wechsel, Stopp alter EventQueue und Start neuer Region-Queue.
- Langzeit-Metriken, Error-Sampling, Test verschiedener Simulator-
  und Proxy-Laufzeitkonfigurationen, Seed-Neuvergabe bei
  abgelaufener Region, dynamischer Reconnect auf neuer Region.
- Diese Änderung ersetzt *nicht* den bestehenden LLUDP-Circuit.

Details: `docs/CAPS.md`.


## Phase 8/9 – LLSD-Logs + erste OpenSim-Meshobjekte · 10.10.2026 · 0.2.0-alpha.48

**Auslöser und Live-Befund:**
- Alpha.47 lädt 293 Objekttexturen und 4 Terraintexturen.
  Die EventQueue behandelt einen regulären HTTP-502-Leer-Poll
  korrekt; ein initialer HTTP200/LLSD-Integer-Parsefehler erholte
  sich beim nächsten Poll. 9 Texture-Asset-Fehler bleiben:
  6x HTTP 404 (serverseitig kein Asset geliefert), 2x unbekanntes
  Bildformat, 1x defekte JPEG2000-Tile-Daten. Serverassets
  können nicht ohne Assetservice-Zugriff clientseitig ersetzt werden.
- Meshobjekte waren vorher vollständig unterrepräsentiert:
  ObjectUpdate ExtraParams wurden verworfen, kein Mesh-Asset
  abgerufen, keine Mesh-Geometrie an OpenGL übergeben.

**Implementiert (Basis):**
- LLSD XML-Integer: Empty/Whitespace als 0, 64-Bit-Werte
  mit strikter Bereichsprüfung, Pluszeichen möglich; Binary
  LLSD bleibt standardkonform 32 Bit.
- ObjectUpdate-SculptExtraParams: Parametertyp 0x30 mit 17 Byte
  UUID + Sculpt-Type; Mesh-Typ `type & 7 == 5`.
  Die Mesh-ID wird ins WorldModel/SceneObject übergeben.
- GetMesh/GetMesh2 wird über regionsgebundene Seed-CAPS
  entdeckt und per `mesh_id`-Query asynchron heruntergeladen.
- Binärer LLSD-Header mit Blockoffset/Blockgröße;
  zlib/gzip-LOD-Daten; beschränkte Gesamt-/Einzelgröße;
  Position/UV/Index-Dekodierung von 16-Bit-Werten für die
  High-/Medium-LOD, Face-Ranges für Texturen, geometrische
  Dreiecksnormalen, OpenGL-Buffers. Kein lokaler OBJ/DAE-Import.
- CPU/GPU-Asset-Daten getrennt: Decoder off-thread, OpenGL
  Upload ausschließlich auf Render-Thread.
- Zuordnung von Mesh-Asset-UUIDs zu geladenen Mesh-Prims,
  Mesh-Fallback bis Download abgeschlossen, Caches auf
  256 Asset-IDs und regionsepochengeschützte Futures begrenzt.

**Abnahme und offene Grenzen:**
- Debug-Build, 24/24 CTests einschl. synthetischem OpenSim-Mesh,
  malformed LLSD und Invalid-Asset-Tests.
- Ein echter Login mit visueller Mesh-Kontrolle auf NexVerse
  ist **nicht** durch Offline-Tests ersetzt. GetMesh-Antworten,
  Shader-/Face-UVs und Performance müssen in der Welt geprüft werden.
- Compressed-ObjectUpdate initiale Mesh-Parameter noch nicht
  erfasst; bisherige IDs werden bei Partial-Updates bewahrt.
- Rigging/Skinned Mesh/Animesh, Sculpt-Texture-Parametrisierung,
  Mesh-LOD-Distanzstrategie, Direct ViewerAsset, optimierter
  Speicher-LRU, vollständige UV/Normal/Material-Fidelity und
  Teleport-Circuit-Wechsel bleiben eigenständige Blöcke.

Weitere Details: `docs/MESH.md`.


## Phase 8/9 – LLSD-Datum / Mesh-Fehlerbehandlung · 10.10.2026 · alpha.49

**Verbindlicher Ausgangsbefund aus alpha.48:**
- 232 echte Mesh-Uploads an OpenGL, 216 fehlgeschlagen.
- Davon 208 identische Headerfehler `Unknown LLSD binary marker`,
  sieben ungültige Submeshes, ein Mesh HTTP404.
- Objekttexturen 287 erfolgreich/9 fehlend; Terraintexturen 4/4.
- EventQueue HTTP502 normal, ein HTTP200-LLSD-Parsefehler
  `Unsupported LLSD XML type: date`, danach Recovery.

**Geliefert:**
- LLSD XML `date` und binäres `d` im Parser/Serializer.
- Bei unbekanntem Binärtyp Hex-Marker und Byteposition;
  bei defektem Mesh-Header 8 erste Asset-Bytes in Hex und
  Gesamtzahl der Bytes als sichere, begrenzte Diagnostik.
- Default UV [0,1] bei fehlender optionaler TexCoord0Domain.
- Einzelne ungültige Submesh-Faces können übersprungen werden
  (Rollback partieller Indizes/Vertices; gültige Faces bleiben).
- GetMesh2 vor GetMesh, alternativer Mesh-CAP-Abruf bei Fehlschlag,
  begrenzt auf einmalige Wiederholung; epochengeschützter Cache.
- Tests mit Datumsfeldern, optionalen UV-Domains,
  alternativen Capabilities und teilweise beschädigten Mesh-Faces
  erweitern die 24 vorhandenen CTests.

**Abnahme und Prioritäten:**
- Offline-/Build-/GUI-Abnahme separat dokumentieren.
- Kein echter Server-Mesh-Retest dieser Version erfolgt: **208
  Headerfehler nicht als behoben ausgeben**. Nach Login die neuen
  `prefix_hex`-`at offset`-Fehler auslesen und deren Ursache
  anhand des tatsächlichen Binärformats angehen.
- Dann `ObjectUpdateCompressed` Mesh-ExtraParams, Mesh-LOD-
  Streaming, Skinned/Animesh, Avatar und vollständige Texturen
  separat bearbeiten.

Details `docs/MESH.md` und `docs/CAPS.md`.


## alpha.50 – Compressed-ObjectUpdate und Mesh-Prims · 10.10.2026

**Status:** Beim Start dieses Blocks gab es kein aktuelles
/tmp/ogl49-live.log auf der Linux-Maschine. Die 208 vorherigen
Mesh-Headerfehler sind deshalb weiterhin unbestätigt.

**Implementiert:**
- Vollständige ObjectUpdateCompressed-Pakete mit CompressedFlags,
  Owner UUID, optionalen Feldern AngularVelocity, Parent,
  ScratchPad, Text, MediaURL, Particles, Sound und NameValues.
- SculptExtraParams Typ 0x30 nach den optionalen Feldern;
  Mesh-UUID und Sculpt-Typ werden ins WorldModel übernommen.
- Vollständige 23-Byte-Prim-Form und TextureEntry-Daten mit
  32-Bit-Längenfeld und Face-Textur sowie Skalierung/Parent-ID.
- Bounds-Checks und Ablehnung abgeschnittener Compressed-Daten.
- Test: synthetisches vollständiges komprimiertes Mesh-Prim
  samt Scale/Parent/TextureEntry, plus Truncation-Test.
- Formatbeleg: OpenMetaverse ObjectManager.cs,
  ObjectUpdateCompressedHandler:
  https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/ObjectManager.cs

**Noch offen:** echter NexVerse-Live-Test; 208 vorherige
Headerfehler über prefix_hex / Byteoffset analysieren;
Mesh-LOD, Rigging/Animesh, Teleport-Regionswechsel und
fehlende Servertexturen.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl50-live.log


## 0.2.0-alpha.51 – Dynamischer GPU-Mesh-Cache · 10.10.2026

**Ausgangsbefund alpha.50:** Der Live-Test meldete 256 erfolgreiche
Mesh-Uploads, 2 vollständig fehlerhafte Mesh-LODs,
3 teilweise gerettete Submeshes. Durch die Bedingung
loaded_ids.size() < 256 wurden danach keine weiteren
Meshes angefordert. Das ist ein Cache-Limit und nicht
die tatsächliche Anzahl von Meshes der NexVerse-Region.

**Implementiert:**
- Neue pure Mesh-Residency-Planung mit nach Entfernung zum
  Avatar priorisierten Mesh-UUIDs und Duplikatentfernung.
- GPU-Asset-Budget 256. Werden durch Ortswechsel neue Meshes
  relevanter, verdrängen sie zuvor geladene entfernte Meshes.
- 24 Meter Prioritätsbonus für residente Meshes vermeidet
  Cache-Churn bei geringfügiger Bewegung.
- Plan-Neuberechnung bei SceneObject-Änderung oder nach
  maximal 750 ms; GPU-Eviction pro Zyklus auf acht begrenzt.
- Renderer: set_mesh_asset liefert Upload-Erfolg, dazu
  remove_mesh_asset und clear_mesh_assets für explizite
  GL-VAO/VBO/EBO-Freigabe auf dem Renderer-Thread.
- Bei Region-Epochenwechsel werden ältere GPU-Meshes gelöscht.
  Stale Mesh-HTTP-Ergebnisse und nicht mehr priorisierte
  Ergebnisse werden nicht hochgeladen.
- Unbekannte/doppelte UUIDs und nicht-finite Positionen
  werden bei der Auswahl ignoriert; Gleichstände deterministisch.
- Zusätzlicher Testsatz für Bewegung, Eviction, Hysterese,
  Deduplikation, NaN, Nullbudget und 320 Meshes; 25/25 CTests.

**Offen und begrenzt:**
- Echte In-World-Abnahme von alpha.51 bei Ortswechsel.
- Noch keine Frustum-/Occlusion-Priorisierung.
- Mesh-Kindposition nur angenähert mit Parent-Position
  plus lokalem Offset (ohne Rotation).
- Keine persistenten Mesh-Disk-/CPU-LOD-Caches;
  beim Zurückkommen können Downloads erneut erfolgen.
- Zwei vollständig ungültige Mesh-LODs, beschädigte oder
  HTTP404-Textur-Assets; Rigging/Animesh und Teleport
  bleiben eigenständige Entwicklungsblöcke.

Live-Test: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl51-live.log


## alpha.52 – Avatar sichtbar machen und Bewegung · 10.10.2026

**Auslöser:** Der Screenshot der verbundenen Welt zeigt
Terrain/Meshes, aber keinen eigenen Avatar und bislang
keine nachvollziehbare Bewegung. WORLD UDP LIVE beweist
weder den Avatar-Renderpfad noch die Bewegungsfähigkeit.

**Implementiert:**
- OpenGL-Avatar-Proxy-Pfad mit sieben Basis-Meshes für
  Kopf, Torso, Arme, Beine, für eigenen Avatar und bis
  zu 63 weitere Avatar-Datenpunkte. Die Darstellung wird
  pro Frame anhand der Agentposition aktualisiert.
- Die Proxies sind absichtlich keine echten OpenSim-Avatare:
  keine Skin-Bakes, Avatar-Meshes, Skeletons,
  Wearables, Haare, Attachments oder Animationen.
- Neues Keyboard-Control-Modul mit W/S/A/D, linker/rechter
  Pfeiltaste zur Rotation, Shift für schnelleres Bewegen,
  Space für Sprung, C für Abwärtsbewegung, F Flug
  per Toggle. Die AgentUpdate-Pakete enthalten dabei
  korrekte Körper-/Kopf-Quaternionen und Kameraachsen.
- Self-Avatar-LocalID wird aus ObjectUpdate bzw.
  ObjectUpdateCompressed gewonnen, mit dem RegionHandle
  gekoppelt und bei ImprovedTerseObjectUpdate
  zur Aktualisierung der serverbestätigten eigenen
  Avatarposition im WorldModel verwendet.
- Bei Steuerungsänderungen und bestätigten Simulator-
  Positionsupdates werden diagnostische Logzeilen geschrieben.
  Fokusverlust sendet keine gedrückten Bewegungs-Keys.
- Nach erfolgreichem Login wird das Login-Eingabefeld
  nicht mehr durch die Bewegungssteuerung verändert.
- Regressionstests für Steuerbits, Quaternion, Fly-Toggle
  und Steuerungsreset; 26/26 CTests grün.

**Noch offen und nicht als fertig behaupten:**
- Echte NexVerse-In-World-Bewegungsabnahme, insbesondere
  muss die Self-Avatar-LocalID und die Terse-Positions-
  Bestätigung mit neuer Version beobachtet werden.
- Richtige Firestorm-Avatar-Appearance (Skeleton,
  Body Shape, Skin-Bakes, Wearables, Attachments,
  Animationen) sowie Mouselook, Camera-Orbit/Zoom,
  Teleport/Regionswechsel, Collision/Reconciliation.
- Die einfachen Avatar-Proxies sind nur eine
  Übergangsdarstellung bis zu Phase-10-Appearance.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl52-live.log


## 0.2.0-alpha.53 – Phase 8: OpenSim-Point-Lights · 10.10.2026

**Verbindliche Nutzerentscheidung vom 10.10.2026:**
Fortsetzung der Entwicklung entlang der Reihenfolge des
projectplan.md. Die zuletzt untersuchten Avatar-/Bewegungs-
und übrigen Rendering-Asset-Fehler werden **zunächst
nicht behoben**, sondern dokumentiert und für eine
spätere gemeinsame Fehlerbehebungsphase vorgemerkt.
Keine implizite Behauptung, dass die Probleme gelöst seien.

**Jetzt neu in Phase 8 implementiert:**
- Point-Light ExtraParams Blocktyp 0x20, 16 Byte:
  4x RGBA-UNORM8 (Alpha = Intensität), dazu
  little-endian float32 Radius, Cutoff und Falloff.
  Implementierung orientiert sich an libopenmetaverse,
  Primitive.LightData.
- Sichere Bounds/Finite-/Radius-Prüfungen;
  Daten werden in vollständigem ObjectUpdate und
  ObjectUpdateCompressed nach SculptExtraParams separat
  dekodiert. Bei Partial-Updates behalten vorhandene
  Objekte ihre bereits bekannte Lichtdefinition.
- Eigenes Feld im WorldModel für lokale Lichtquellen.
- Die acht nächsten Punktlichter werden aus
  den Objektpositionen pro Frame ausgewählt. Child-Prim-
  Position wird vorläufig mit Parent+Offset angenähert.
- Legacy OGLRender: Weltposition aus uModel-Transform,
  acht Point-Light-Uniforms und diffuse Beleuchtung
  von nicht-fullbright Objekt-/Meshflächen nach
  Abstand, Radius, Intensität und Falloff.
- Terrain/Avatar werden in dieser ersten Version
  noch nicht von den lokalen Punktlichtern beleuchtet.
  Keine dynamischen Schatten, keine Projektoren,
  keine PBR-Energiekonservierung.
- Kompletter Debug-Build, 26/26 CTests bestanden,
  GL-3.3-Smoke auf RTX 4070 SUPER erfolgreich.
  **In-World-Sichtabnahme mit echter Lichtquelle
  steht noch aus**.

**Bekannte Fehler für spätere Fehlerphase (unverändert):**
- Eigene Avatar-LocalID wurde in alpha.52 Live-Log nicht
  gebunden, daher keine serverbestätigten
  Self-Avatar-Positionsupdates / keine verifizierte
  Benutzbarkeit der WASD-Bewegung.
- Eigenes Erscheinungsbild ist bisher nur Proxy,
  echtes Skeleton/Bakes/Wearables fehlen.
- Alpha.52: 252 geladene, vier fehlgeschlagene Meshes;
  neun fehlende/beschädigte Objekttexturen.
- Keine aktuelle Bestätigung einer GPU-Cache-Eviction
  bei tatsächlichem Ortswechsel.

**Fortsetzung Phase 8:** Partikel-Basis, weitere
Lichttypen (Projektor/Material-Licht), Render-Performance
und anschließend Phase 9 bzw. weitere Funktionssysteme.
Maßgeblich bleibt der Plan, nicht die Bugliste.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl53-live.log


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


## 0.2.0-alpha.56 – Großrelease Phase 8 / vier Entwicklungsblöcke · 10.10.2026

Auf Nutzerwunsch werden zukünftig **3–4 zusammenhängende Funktionen
pro Release** umgesetzt, mit einem gemeinsamen Abschluss aus Build,
Tests, Versionspflege, Dokumentation und GitHub-Commit. Einzelne
Zwischentests bleiben zur Qualitätssicherung möglich.

**Block A – Partikelphysik:**
- Neuer testbarer reiner Partikelkinematik-Helfer, mit Begrenzung
  von Lebensdauer und festen Integrationsschritten (48).
- Gravity/PSBlock-Beschleunigung, Wind-Flag 0x008 und Bounce-Flag
  0x004. Wind ist aktuell ein **lokal generiertes Feld** und
  keine behauptete Simulator-Windmessung; Bounce erfolgt auf der
  Höhe der Quellprim, nicht durch Terrain-/Collision-Abfragen.
- Flags für InterpColor (0x001) und InterpScale (0x002) werden
  berücksichtigt statt unbedingter Interpolation.

**Block B – Ziel- und Verbindungseffekte:**
- Target UUID aus dem 68-Byte-PSBlock-Systemteil auslesen;
  bekannte Zielprims, Avatare und eigener Agent werden anhand
  UUID dem Emitter zugeordnet; unbekannte Ziele bleiben
  unaufgelöst, ohne auf erfundene Positionen zu springen.
- TargetPos (0x040) und TargetLinear (0x080) im CPU-Sampler,
  Beam (0x200) als verteilte Partikel entlang einer Strecke
  und Ribbon (0x400) als zeitlich versetzte Punktspur.
- Echte zusammenhängende Ribbon-Flächen, SourceRotation,
  Target-Bewegungs-Prediction und spezifische Formmodi sind
  **noch offen**.

**Block C – Transparenz, Blending, Budget:**
- OpenMetaverse ParticleSystem BlendFunc-Enums 0..9 für
  Extended PSBlock werden validiert und mit OpenGL abgebildet.
  Default SourceAlpha=7, OneMinusSourceAlpha=9.
- Neue distanzbasierte Sortierung in acht Tiefenbändern,
  innerhalb eines Bands Batching nach Blend/Texture.
- Feste Obergrenzen von 768 Partikeln und maximal 192
  Zeichen-Batches schützen vor übergroßer Drawcall-Zahl.
- Das ist nur näherungsweise Alpha-Tiefensortierung; echte
  per-Partikel-Sortierung über alle Texturen ist offen.

**Block D – Beleuchtung/Materialgrundlagen:**
- Bei den bis zu acht Legacy-Point-Lights wird zusätzlich
  zum bestehenden Radius/Falloff der Cutoff-Wert im Shader
  als näherungsweiche Kernausblendung berücksichtigt.
- Lokales Blinn-Phong-Highlight für glänzende,
  nicht-fullbright Objektoberflächen ergänzt.
- Kein vollständiges PBR, keine Projektorlichter, keine
  dynamischen Schatten oder Energieerhaltung.

**Abnahme:** Debug-CMake + C++23 erfolgreich, 28/28 CTests
einschließlich neuer reiner Wind-/Bounce-/Target-Tests,
Parser-Tests für TargetUUID und Blend-Enums;
OpenGL-GUI-Smoke auf NVIDIA RTX 4070 SUPER, Version alpha.56,
NexVerse-Welcome geladen, keine Shader-Fehler (der Exit 124
des Smoke-Tests ist die beabsichtigte 7-Sekunden-Begrenzung).
**Authentifizierter In-World-Sichttest dieser Effekte ausstehend.**

**Zurückgestellte Fehler unverändert:** Eigene Avatar-LocalID
und Bewegungsbestätigung, echte Avatar-Skeleton-/Appearance,
defekte/fehlende Mesh-/Texturassets und Teleport/Region-Handoff.
Keine dieser Fehlergruppen wird in diesem Release als gelöst
ausgegeben.

**Nächste gemeinsame Blöcke:** echte PSBlock-Emitter-Lebenszyklen,
Parent-/ObjectRelative- und Target-Pfade, verbundene Ribbons
und Beam-Geometrie, Asset-Caching und moderne Materialeffekte.
Die Reihenfolge bleibt an projectplan.md gebunden.

Live-Abnahme:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl56-live.log
