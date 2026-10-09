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
