# NexVerse Viewer – Projektplan

Stand: 11.10.2026
Repository: `marlin198323-glitch/nexverse-viewer`
Ziel: Plattformübergreifender OpenGenesisLINK-Viewer mit vollständiger Firestorm-Funktionsbaseline und OpenGenesisLINK-/OpenSim-Serverkompatibilität; aktueller eigener C++23-Code unter MPL 2.0, eventuelle Firestorm-Quellcodeübernahme nur nach Lizenzprüfung.

---

## 1. Zielbild

**Vorrangige Nutzerentscheidung vom 11.10.2026:** Firestorm ist die vollständige Funktions-, Bedienungs- und Quellcode-/Implementierungsreferenz des OGL Viewers. ALLE in der offiziellen Firestorm-Releasebaseline verfügbaren Funktionen gehören in die Paritätsinventur, auch Power-User-Funktionen. Vorhandene Funktionen werden funktional übernommen/portiert und nur für OpenGenesisLINK-/OpenSim-Protokolle, Rechte, Sicherheit oder das eigene NexVerse-Design angepasst. Neue Features kommen ergänzend hinzu; kein stillschweigender Funktionsverlust.

**Ist-/Soll-Abgrenzung:** Der gegenwärtige OGL-Code ist noch eine eigenständige C++23-Neuimplementierung, kein auf Firestorm-Source aufgebauter Fork. Ob Firestorm-Module direkt portiert oder eine Teilmigration vorgenommen werden, ist gesondert technisch und lizenzrechtlich zu bewerten. Der Ist-Zustand darf nicht als bereits vollständige Firestorm-Codebasis ausgegeben werden. Für Quellcodeübernahmen offizielle Firestorm-Releasebranches und deren LGPL-/Drittanbieterbedingungen beachten.

**Verbindliche Source-of-Truth:** OpenGenesisLINK-Server `https://github.com/opengenesislink/opengenesislinkserver` auf aktuellem `main` ist der primäre Protokoll-, CAPS-, Datenmodell- und Berechtigungsvertrag. **Vor jedem Entwicklungsblock neu HEAD/Servercode prüfen** und die verglichene SHA dokumentieren. Firestorm als Funktions- und Client-Quellreferenz: `https://github.com/FirestormViewer/phoenix-firestorm`; nicht blind einen Nightly-Branch als Release implementieren. Details: `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`.

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

## 2.1 Firestorm-Funktionsbaseline – verbindlicher vollständiger Zielkatalog

**Neufassung vom 11.10.2026 mit Vorrang vor älteren Mindestumfangs-/Ausnahmeregeln:** Firestorm ist der verbindliche **vollständige Funktionskatalog**; nicht nur die üblichen Kernfunktionen. Bestehende Firestorm-Funktionen, einschließlich erweiterter Funktionen, werden aufgelistet und auf OGL übertragen, an OpenGenesisLINK-/OpenSim-Serververträge angepasst und erst danach durch eigene Funktionen erweitert. Ein Feature darf ausschließlich begründet auf `server_unsupported` oder `deferred_with_reason` stehen; es wird nicht stillschweigend weggelassen.

Das aktuelle C++23-Repository ist noch keine direkte Firestorm-Source-Migration. Quellcodeportierung/Forkstrategie bedarf eines separaten Lizenz- und Architekturchecks, ohne das Funktionsparitätsziel abzuschwächen. Die folgende historische Kernfunktionsliste bleibt als erste Untermenge erhalten; **maßgebend für den vollständigen Umfang ist `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md` und die einzeln nachzuweisende `docs/FIRESTORM_PARITY_MATRIX.md`.**

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

### Firestorm-Erweiterungen sind Teil der Gesamtinventur

Auch RLV/RLVa, Phototools, spezielle Kamera-/Debug-Werkzeuge und weitere Firestorm-Power-User-Funktionen müssen in die vollständige Paritätsmatrix aufgenommen und – soweit Rechte, Plattform, Protokoll und Lizenzen dies zulassen – implementiert werden. Serverseitig nicht bereitgestellte Funktionen bleiben als überprüfbar begründete Einschränkung sichtbar. Kein automatischer Ausschluss dieser Kategorien mehr.

Das eigenständige OpenGenesisLINK-/NexVerse-UI-Design bleibt bestehen: Funktionsgleichheit bedeutet nicht die unveränderte Übernahme des Firestorm-Skins oder geschützter Marken-/Drittanbieterassets. Firestorm ist **Funktions- UND Implementierungsreferenz**.

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
- **Aktualisierte Priorität 11.10.2026:** Firestorm dient als vollständige Funktions- und Quellcode-/Implementierungsreferenz. Die aktuelle modulare OGL-Renderarchitektur bleibt bis zu einer dokumentierten Migrationsentscheidung getrennt; bestehende Firestorm-Funktionen dürfen bei der Modernisierung nicht verloren gehen.
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
- Firestorm dient als vollständige Funktions- und technische Quellcodereferenz (Lizenz-/Release-Branch-Vorbehalt); das eigens festgelegte OGL-/NexVerse-Design bleibt erhalten. Firestorm-Skin/XUI/Design muss nicht übernommen werden
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


## 0.2.0-alpha.57 – Großrelease: vier zusammenhängende Funktionsblöcke · 10.10.2026

**Block 1 – Emitter-Lebenszyklus / ObjectRelative**
- Emitterstart auf Renderer-Zeitbasis (nicht mehr globaler Partikelphase).
  CRC-Wechsel startet ein emittierendes Prim neu; entfernte Emitter
  werden aus dem Laufzeitstatus entfernt.
- SourceMaxAge stoppt neue Bursts, lässt bereits gestartete Partikel
  innerhalb ihrer eigenen PartMaxAge-Zeit auslaufen.
- Rotationskompatibilität: bei OpenSim SourceFlags ObjectRelative
  (Bit 0) werden die Quellbeschleunigung und Initialgeschwindigkeit
  durch die Objekt-Quaternion transformiert.
- Offene Details: vollständige LSL-Start/Stop- und SimTime-Synchronisation,
  Quelle mit ParentRotation, Simulator-Wind statt synthetischem Wind.

**Block 2 – durchgehende Beam-/Ribbon-Geometrie**
- Neuer Kameraseiten-ausgerichteter Ribbon-Generator mit
  verbundenen Dreiecken statt einer bloßen Punktfolge.
  Separater OpenGL-3.3-Triangle-Shader/-VAO/-VBO, UVs,
  transparente Soft-Edges sowie geteilte GetTexture-Assets.
- Beam folgt dem bekannten Zielpunkt; Ribbons folgen
  einer auf Basis der Partikelkinematik erzeugten Spur.
  Begrenzung 32 Emitter/je 16 Samples und 15 Segmente.
- Offene Details: echte bewegungsabhängige kontinuierliche
  Emitter-Historie, native Ribbon-Rotation und robuste
  Transparenzsortierung aller überlappenden Flächen.

**Block 3 – Partikelmaterial / Glow**
- In Extended PSBlock vorhandener Start/EndGlow wird an den
  GPU-Renderer durchgegeben und für Sprites und Ribbon-Material
  als begrenzte RGB-Aufhellung berücksichtigt; nicht als
  echtes physikalisches HDR- oder PBR-Emissive ausgegeben.
- OpenSim-Blend-Enums 0..9 aus alpha.56 bleiben aktiv.

**Block 4 – Phase 9: wiederverwendbarer Texture-Asset-Cache**
- Frühere harte 512-GPU-Texturen-Grenze wird durch ein
  dynamisches Residency-Budget ersetzt; unbenutzte Texturen
  werden als GL-Texturen freigegeben, höchstens 8 pro
  Planungsrunde (750ms).
- Reihenfolge: nahe Partikelemitter, anschließend Prim-/
  Meshmaterialien; letztere erhalten eine Objekt-Distanz-
  priorisierte Ordnung beim Aufbau der Scene-Revision.
- Neue Region leert den GPU-Objekttexturcache; veraltete
  HTTP-Replies außerhalb von Region/Epoche/Residency werden
  verworfen. Gemeinsames GetTexture-Ladesystem bleibt.
- Grenzen: noch kein persistentes CPU-/Disk-Texture-Cache,
  keine Texture-LOD-/Mip-Streaming-Stufen, keine Occlusion,
  UI/Textur-Distanz-Neusortierung nur bei Scene-Update;
  beim Zurückkehren eviktierter Texturen erneut HTTP möglich.

**Tests:** 30/30 CTests inklusive Lifecycle, Quaternion,
Ribbon-Triangles, Cache-Umschichtung. C++23-Debug-Build
und OpenGL-3.3-Starttest auf NVIDIA RTX 4070 SUPER
ohne gemeldeten Shaderfehler. Der 7-Sekunden-Exit 124
ist der absichtliche Timeout, kein Crash.
**NexVerse In-World-Darstellung bleibt ungeprüft.**

**Auf Nutzerwunsch zurückgestellt:** Avatar-LocalID/
serverbestätigte Bewegung, echte Avatar-Appearance,
defekte/missing Mesh-/Texture-Assets und Teleport.
Kein dieser Fehlerbereiche wird als behoben ausgegeben.

Nächster Großrelease-Block: Asset-Caching-Disk/CPU,
LOD/Prioritätsstufen und weitere OpenSim-Renderer-
Kompatibilität. projectplan.md bleibt verbindlich.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl57-live.log


## 0.2.0-alpha.58 – Phase 9 Großrelease (vier Blöcke) · 10.10.2026

Nutzerentscheid: Jeweils drei bis vier vollständige Entwicklungsblöcke
pro Release; bekannte Avatar-/Bewegungs-/Mesh-Asset-Fehler bis
zur späteren Bugfixphase zurückstellen. Verbindlich: projectplan.md.

**Block A – Persistenter Texture-Disk-Cache:**
- TextureDiskCache lädt/speichert validierte RGBA8-Assetdaten
  unter $XDG_CACHE_HOME/opengenesislink/textures-v1 bzw.
  $HOME/.cache/opengenesislink/textures-v1.
- 8-Byte-Version-Magic, Width/Height, FNV-64-Inhaltshash über
  Dimensionen und Pixeldaten, Größen-/Bounds-Prüfungen, sichere
  UUID-Dateinamen, temp-Datei + Umbenennung.
- 256 MiB Diskbudget, 64 MiB maximales Einzelbild,
  least-recently-used-ähnliche Eviction anhand File-MTime;
  auf Cache-Miss/Fälschung/IO-Problem normaler GetTexture-Download.
- Cache wird im Hintergrund-Texture-Future geprüft; bei Erfolg
  wird HTTP übersprungen. Kein persistenter Mesh-/Animationscache
  in diesem Release, keine echte Hash-/Version-Aushandlung mit
  dem OpenSim-Asset-Service.

**Block B – Dynamische Texturprioritäten:**
- Texturpositionen werden bei Object-/Scene-Update für Default,
  Face sowie Normal-/Specular-Materialien gesammelt.
- Alle 750ms/Scene-Revisionsänderung werden sie ausgehend von
  der aktuellen Avatarposition nach Entfernung neu sortiert.
- Identische UUIDs werden zusammengefasst; bereits GPU-geladene
  Assets erhalten 8m Hysterese gegen Thrashing.
- Partikel-Assetpriorität sowie 512 gleichzeitige GPU-Texturen,
  begrenzte GL-Eviction und Regions-Epochenprüfungen bleiben.

**Block C – Mesh-Sichtgrößenpriorität:**
- MeshDistanceSample führt zusätzlich den maximalen
  Prim-Scale-Durchmesser; Priorisierung gewichtet
  distance / sqrt(clamp(diameter,0.5,16)).
- Größe beeinflusst ausschließlich Assetabruf/Priorität,
  **noch nicht** echte Mesh-LOD-Stufen, View-Frustum-/Occlusion-
  Culling oder Mesh-Vertex-Dekodierung. Defekte Meshes werden
  nicht als repariert bezeichnet.

**Block D – GPU-Sampling und Asset-Diagnose:**
- Bei unterstützter GL_EXT_texture_filter_anisotropic
  aktiviert das Legacy-GL-Backend für Objekttexturen
  bis zu 8x anisotrope Filterung zusätzlich zum Mipmap-Sampler.
  Auf inkompatiblen GPUs bleibt das alte Verhalten.
- Log alle ~60s: kumulative Disk-Cache-Hits, HTTP-Downloads,
  GPU-Evictions, residente Texturen und GPU-Slotbudget.
  Die Zähler sind Sitzungsstatistik, keine serverbestätigte
  Bandbreitenmessung.

**Verifikation:**
- CMake Debug C++23 erfolgreich; 32/32 CTests.
- Neue Tests für persistente Cacheintegrität, manipulierte
  Cache-Dateien, Neustart-/LRU-Verhalten, bewegungsabhängige
  Texture-Distance-Ordering/Hysterese und größenbasierte
  Mesh-Priorität.
- Linux NVIDIA GeForce RTX 4070 SUPER OpenGL 3.3 GUI-Smoke
  bei 3440x1440 mit Welcome-Seite, keine gemeldeten GL-
  Shaderfehler; Exitcode 124 war beabsichtigtes 7s-Timeout.
- **Authentifizierte In-World-Streaming-Abnahme ausstehend.**

**Bekannte, absichtlich unangetastete Probleme:**
Self-Avatar-LocalID/Bewegung, echtes Avatar-Skeleton/Bakes,
noch fehlerhafte/fehlende Mesh-/Texturassets, Teleport.
Keiner dieser Punkte wird als geschlossen markiert.

**Nächste Großrelease-Ideen gemäß Phase 9:** Mesh-Disk-Cache,
RAM-Assetcache, Chunk-/Mip-Streaming-Priorität, GPU/CPU-
Speicherbudget und anschließend Avatar-Funktionssysteme.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl58-live.log


## 0.2.0-alpha.59 – Phase-9-Großrelease (vier Blöcke) · 11.10.2026

**Block A – persistente Mesh-Dateien:** 512 MiB Diskbudget,
Versionmagic OGLMESH1, Little-Endian, UUID-gebundene FNV64-
Integritätsprüfung, Größen-/Index-/Face-Checks und MTime-LRU.
Keine CAPS-Token/URL-Cachedaten.

**Block B – Mesh-RAM-LRU:** threadgeschützte CPU-Geometry-
Residency mit 64 MiB Bytebudget und LRU-Eviction. Durch
GPU-Eviction entfernte Meshes können ohne HTTP erneut
aus dem RAM geladen werden.

**Block C – Integration:** asynchroner Ladepfad RAM → Disk →
GetMesh2/GetMesh; Region-/Epochenschutz und GPU-Budget 256
Meshes (max. acht Evictions je Planung) unverändert.

**Block D – Telemetrie und Tests:** getypte Cacheherkunft,
60s-Metriken über erfolgreiche GPU-Uploads nach Quelle,
CPU-RAM-Bytes und GPU-Slots. Manipulations-, Neustart-,
UUID-, LRU-/Budget- und Fehler-Fallback-Tests.
CMake Debug C++23, **33/33 CTests bestanden**,
OpenGL 3.3 GUI-Starttest auf NVIDIA RTX 4070 SUPER mit
NexVerse-Welcome erfolgreich; Timeout 124 erwartungsgemäß.

**Noch offen:** In-World-Abnahme, echtes Mesh-LOD-Streaming,
GPU/CPU-Bytebudget und Texture-Mip-Streaming.
Avatar-LocalID, WASD, Skeleton/Bakes, defekte Server-Mesh-/
Texturassets und Regions-Teleport bleiben auf Nutzerwunsch
bis zur Bugfixphase zurückgestellt.
`projectplan.md` bleibt verbindlich.
Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl59-live.log`.


## 0.2.0-alpha.60 – Phase 9, vier LOD-Streaming-Blöcke · 11.10.2026

**Block A – OpenSim-LOD-Decoder:** High, Medium, Low und Lowest. Gewünschte Stufe zuerst; bei fehlenden/defekten Chunks Fallback auf andere gültige Level. Bestehende LLSD-/zlib-/Mesh-Bounds gelten weiter.

**Block B – Adaptive LOD-Planung:** `plan_mesh_lods` gewichtet Entfernung und Primgröße, dedupliziert mehrfach verwendete Mesh-UUIDs und nutzt Hysterese. Aktualisierung alle etwa 750 ms oder bei Szene-Änderung.

**Block C – LOD-getrennte RAM-/Disk-Caches:** High behält den bisherigen `<uuid>.mesh`-Schlüssel; andere Stufen werden unter `<uuid>.<lod>.mesh` inklusive LOD-gebundener Checksumme gespeichert. Budgets 64 MiB RAM und 512 MiB Disk. Nach Server-Fallback bezeichnet der Cache-Key den angeforderten, nicht zwingend den tatsächlich dekodierten LOD.

**Block D – GPU-Swap und Diagnose:** Mesh-LOD-Upgrades und -Downgrades ohne vorherige Entfernung der Mesh-UUID. GPU lädt zuerst neue Buffers, gibt alte erst nach erfolgreichem Upload frei. Aktuelle Regions-Epoche und Ziel-LOD werden vor Upload geprüft. Neues Log: LOD-Swap-Zähler und geschätzte VBO/EBO-Bytes.

**Verifikation:** C++23-Debug-Build, 34/34 CTests (LOD-Codec/Fallback, Cache-Varianten, Hysterese). OpenGL-Start-Smoke zum Release. Weiter offen: authentifizierter In-World-LOD-Test, GPU-Bytebudget, Texture-Mip-Streaming, Rigging/Animesh und zurückgestellte Avatar-/Bewegungs-/Teleport-Fehler.

Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl60-live.log`. Maßgeblich: `projectplan.md`, Branch `develop-current`.


## 0.2.0-alpha.61 – Phase 9, vier Texture-Mip-/GPU-Budget-Blöcke · 11.10.2026

**Block A – CPU-Mip-Vorstufen:** `texture_mip_image` erzeugt bis zu drei kleinere RGBA8-Basisstufen mit alpha-gewichteter Mittelung (premultiplizierte Farbsummen); NPOT-Bilder werden begrenzt unterstützt. Der Renderer erzeugt darauf weiter gewöhnliche OpenGL-3.3-Mipmapketten. Es wird kein JPEG2000-Progressive- oder HTTP-Chunk-Streaming vorgetäuscht.

**Block B – Distanzabhängige Qualitätswahl:** `plan_texture_mip_levels` wählt 0/1/2/3 je nach Agentabstand (24/64/128 m), mit Hysterese an den Grenzwerten. Mehrfach verwendete Texture-UUIDs verwenden die nächste Objektposition; aktive Partikeltexturen erhalten Vorrang mit Level 0. Die Planung läuft bei Scene-Revision oder spätestens etwa alle 750 ms.

**Block C – Asynchroner Full-Resolution-Cache/Hot-Reload:** HTTP und der bestehende 256-MiB-RGBA8-Disk-Cache speichern ausschließlich Originalbilder. Der asynchrone Worker erstellt daraus die gewünschte CPU-Mipstufe. Ändert sich die Entfernung, kann die vorhandene GPU-Textur erneut hochgeladen und verkleinert oder vergrößert werden, ohne die ursprüngliche GetTexture-Assetdatei erneut laden zu müssen. Region-/Epochenschutz, Residency und veraltete Qualitäts-Requests werden vor dem Upload geprüft.

**Block D – Geplantes 256-MiB-GPU-Texturbytebudget:** Mit `gpu_rgba8_mip_bytes` werden RGBA8-Mipketten berechnet; `plan_texture_byte_budget` kann vor Upload Texturen niedrigerer Priorität zur Eviction bestimmen. Höher priorisierte Texturen werden nicht verdrängt, um weiter entfernte Assets nachzuladen. Kann ein Upload nicht ins Byte-/512-Textur-Slotbudget passen, wird er zurückgestellt statt unkontrolliert zu wachsen. Telemetrie: aktuelle geschätzte Texturbytes, LOD-Swaps, zurückgestellte Uploads und GPU-Evictions.

**Verifikation:** Debug-Build C++23, 35/35 CTests einschließlich RGBA-Alpha, ungerader Dimensionen, Mip-Byteberechnung, Distanzhysterese und Budgetpriorität. OpenGL-3.3-GUI-Smoke nach Versionspflege. Live-In-World-Abnahme mit echtem NexVerse-Login steht aus.

**Abgrenzung:** Die 256 MiB sind ein softwareseitig aus RGBA8-Mipdimensionen berechnetes Texturbudget, keine gemessene oder vollständig durchgesetzte physische VRAM-Gesamtgrenze. Terrain-, UI- und Mesh-GPU-Ressourcen gehören nicht zu diesem Texture-Budget. Bei sehr großen/zahlreichen Bildassets können zusätzliche Inhalte unter Budgetdruck vorerst im Fallback bleiben.

**Offen:** Netzwerk-Chunks und native JPEG2000-Progressive-LOD, effizientes CPU-RAM-Caching für sämtliche Texturlevel, gemeinsames GPU-Bytebudget für Mesh/Terrain/Texture, authentifizierter Streaming-Sichttest; bekannte Avatar-/WASD-/Mesh-Asset-/Teleportfehler bleiben auf Nutzerwunsch für die spätere gesammelte Fehlerbehebung dokumentiert.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl61-live.log`.

Maßgeblich ist weiterhin `projectplan.md`. Kein Vulkan-/RTX-/Hydro-Vorgriff.


## 0.2.0-alpha.62 – Phase 9, vier Streaming-Bausteine · 11.10.2026

**Block A – Kameraabhängige Asset-Priorität:** Ein weicher Blickrichtungsfaktor gewichtet Textur- und Mesh-Asset-Anfragen relativ zur Third-Person-Kamera. Objekte hinter dem Betrachter werden nicht ausgeblendet; sehr nahe Objekte bleiben grundsätzlich hoch priorisiert. Die bestehenden Entfernungssortierungen und GPU-Residency-Grenzen bleiben erhalten. Das ist keine echte Frustum- oder Occlusion-Culling-Implementierung.

**Block B – CPU-RAM-Texturcache:** Ein threadgeschützter 96-MiB-LRU-Cache hält vollständige dekodierte RGBA8-Bilder pro gültiger UUID vor. Der asynchrone GetTexture-Pfad arbeitet RAM → persistenter Diskcache → HTTP/Decoder. Für kleinere GPU-Mip-Basisbilder wird das dekodierte Original aus dem RAM wiederverwendet. Keine Cache-Speicherung von CAPS-URLs, Tokens oder ungültigen Bildern.

**Block C – gemeinsam begrenzte GPU-Mesh-/Objekttexturbytes:** Geschätztes, softwareseitiges gemeinsames Budget von 384 MiB mit zusätzlich jeweils 256 MiB als Maximalgrenze für Mesh-Buffer und Objekttexturen. Vor GPU-Upload werden bestehende Ressourcen und ersetzte LOD-Varianten eingerechnet. Bei Überschreitung wird ein Mesh-Upload zurückgestellt; Texturen können zuerst niedrig priorisierte Texturen verdrängen. Bei freiwerdendem Speicher werden zurückgestellte Anfragen wieder berücksichtigt. Dies ist ausdrücklich keine vollständige VRAM-Bilanz: Terrain, UI, Treiber-Overhead und weitere Ressourcen sind nicht enthalten.

**Block D – Streaming-Diagnose und Regressionen:** 60-s-Metriken für RAM-Treffer, RAM-Größe/Einträge, GPU-Mesh-/Texturbytes, gemeinsames geschätztes Budget und Deferred Counts. Durchschnittliche Zeit vom Asset-Worker-Start bis zur Verarbeitung der fertigen Future (keine reine Netzwerklatenz). Neue Tests für Kamera-Priorität, sicheren gemeinsamen Budget-Rechenweg sowie den parallelen Textur-RAM-LRU-Cache.

**Verifikation:** C++23-Debug-Build und 38/38 automatisierte CTests erfolgreich. OpenGL-3.3-Welcome-Starttest nach Versionspflege ausführen. Kein bestätigter In-World-Test des NexVerse-Asset-Streamings.

**Bewusst offen:** echte Frustum-/Occlusion-Abfrage, globale inklusive Terrain/UI gemessene VRAM-Budgetierung, inkrementeller J2C-Netzstream, Avatar-Skeleton/Bakes, Bewegungs-Synchronisierung und vollständige Teleport-/Circuit-Behandlung. Bekannte Mesh-/Texture-Serverfehler bleiben in der späteren Fehlerbehebungsphase.

**Nächste 3–4 Blöcke:** Asset-Streaming-Queue/Prioritätsregeln, planbares Prefetching mit Limits, Cache-/Regions-Invalidierung und zusätzliche Mess- und In-World-Integrationsdiagnostik.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl62-live.log`. Verbindlicher GitHub-Handoff-Branch: `develop-current`.


## 0.2.0-alpha.63 – Phase 9, vier Asset-Streaming-Blöcke · 11.10.2026

**Block A – GetTexture-/GetMesh-Anfrage-Taktung:** `AssetRequestPacer` taktet neu gestartete Textur-Demand-Worker auf mindestens 125 ms und Mesh-Demand-Worker auf mindestens 180 ms Abstand. Die bereits vorhandene Kamera-/Distanz- und Partikelpriorität bleibt maßgeblich; höchstens je ein aktiver Demand-Worker pro Assettyp und keine künstliche Blockade bereits laufender Requests. Pacer werden bei Regionswechsel zurückgesetzt.

**Block B – begrenztes Texture-Prefetching:** Der Viewer wärmt vor dem eigentlichen GPU-Upload vollständige RGBA8-Bilder aus dem persistenten Texture-Diskcache in den 96-MiB-RAM-LRU vor. Die Auswahl berücksichtigt die bereits priorisierte GPU-Residency-Reihenfolge, ignoriert GPU-residente und bekannte fehlgeschlagene UUIDs und untersucht maximal vier Kandidaten pro Durchgang. Es läuft höchstens ein Prefetch-Worker gleichzeitig, mit mindestens 650 ms Abstand und maximal 48 Prefetch-Kandidaten pro Region. Es finden dabei ausdrücklich keine zusätzlichen HTTP-Anfragen oder GPU-Uploads statt.

**Block C – Regions-Epochen und Cache-Admission:** `TexturePrefetchScope` setzt geprüfte Kandidaten bei neuem `material_scope.epoch()` zurück. Die aktuell gültige Regionsepoche wird atomar an den Prefetch-Worker weitergegeben; ein beim Abschluss veralteter Disk-Read wird vor der Cache-Aufnahme zurückgewiesen. Bestehende GetTexture-/GetMesh-Epochenschutzprüfungen im GPU-Pfad bleiben unangetastet. Diskcache und RAMcache sind UUID-basiert, CAPS-Tokens werden nicht gespeichert. Prefetch-Worker lassen sich nicht während laufender Dateizugriffe zwangsweise abbrechen.

**Block D – Telemetrie und Regressionstests:** Das 60-Sekunden-Streaminglog meldet Demand-Starts je Assettyp sowie Prefetch-Versuche, erfolgreiche RAM-Erwärmungen, Diskcache-Misses und stale Prefetch-Resultate. Neue deterministische Tests für Zeitintervalle, Kandidaten-Limits und Epochensprünge; Integrationstest des echten Disk→RAM-Prefetch-Pfads einschließlich manipuliertem Cachefile. `TextureMemoryCache::contains()` ermöglicht den Kandidatenabgleich ohne teure Bildkopie.

**Verifikation:** C++23-Debug-Build, 40/40 CTests einschließlich neuer Scheduler- und Disk-Prefetch-Tests. Linux OpenGL-3.3-GUI-Starttest nach Versionspflege. Authentifizierter NexVerse-In-World-Streaming-Test ist noch ausstehend.

**Grenzen:** Kein HTTP-Prefetch, kein eigenständiger parallel ladender Mesh-Prefetcher, kein inkrementeller J2C-Assetstream. 384-MiB-Budget bleibt eine Schätzung für Mesh-/Objekttextur-GPU-Ressourcen ohne Terrain/UI und Treiber-Overhead. Unvollständiger Avatar, Bewegung, Teleport und einzelne Mesh-/Texturserverfehler bleiben entsprechend Projektplan für die spätere Bugfixphase dokumentiert.

**Nächste 3–4 Blöcke:** Recovery und Fehlerklassifikation des Asset-Loaders, temporäre Retry-/Backoff-Logik statt permanenter Fail-Sets, robustere Regionsepochen-Aufräumlogik und In-World-Assetmetriken. `projectplan.md` bleibt bindend.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl63-live.log`; Branch `develop-current`.


## 0.2.0-alpha.64 – Phase 9, vier Asset-Recovery-Blöcke · 11.10.2026

**Block A – strukturierte Asset-Fehlerklassifikation:** Der GetTexture- und GetMesh-HTTP-Pfad liefert zusätzlich zum Diagnosetext `AssetFailure`: `transient` (Timeout/Transport, 408/425/429/5xx), `missing` (404/410), `invalid` (fehlerhafter Decoder/unerwartete HTTP-4xx) oder `configuration` (400/401/403, fehlender Transport/ungültige CAPS-URI). Bei PNG→J2C beziehungsweise GetMesh2→GetMesh-Fallback bestimmt die kombinierte Fehlerklasse die Wiederholungsstrategie; ein erfolgreicher Fallback gilt als Erfolg.

**Block B – zeitlich begrenzte Retry-/Backoff-Logik:** `AssetRetrySchedule` ersetzt die bisher bis zum Regionswechsel dauerhaft gesperrten Fehler-UUIDs. Temporäre Fehler: nach 2, 4 und 8 Sekunden erneute Versuche, danach fünf Minuten Sperrzeit; HTTP 404/410: zehn Minuten; ungültige Assets: 30 Minuten; Konfigurationsfehler: fünf Minuten. Erfolgreiche GPU-Uploads löschen den zugehörigen Fehlerstatus. Mesh-Fehlerstatus unterscheidet UUID und angeforderten LOD; GPU-Bytebudget-Deferrals bleiben weiterhin getrennt. Die Map ist auf 4096 Einträge begrenzt; bei darüber hinausgehender Kartenfluktuation wird sie geleert.

**Block C – regiongebundene Fehlerverwaltung:** Bei `material_scope.epoch()`-Wechsel werden Mesh- und Textur-Retrytabellen geleert. Bereits laufende Downloads werden nicht auf dem GUI-Thread blockiert oder abgebrochen. Fertige Antworten aus alten Epochen bleiben durch die bestehenden Epochenschutzprüfungen vom GPU-Upload ausgeschlossen; zusätzliche Stale-Counter dokumentieren diese Verwerfungen. Cache-Dateiformate und Mesh-/Texture-Cachebudgets bleiben unverändert.

**Block D – Diagnose und Regressionstests:** Streaminglogs führen `failure_events`, `retry_dispatches`, `backoff_assets`, `transient_failures` und `stale_results` je Mesh-/Textur-Familie. Tests prüfen Klassifikation von HTTP-Antworten, Rechenfolge des Backoffs, Regionsepochen, Zurücksetzen bei Erfolg und beide echten Loader-Fallback-Pfade (404 und 503) mittels Mock-HTTP-Transport.

**Verifikation:** Vollständiger C++23-Debug-Build und 41/41 CTests sind erfolgreich. Nach Versionspflege erfolgen erneuter vollständiger Rebuild und Linux OpenGL-3.3-Welcome-Starttest. Ein authentifizierter NexVerse-In-World-Streaming-Test ist weiterhin nicht ausgeführt.

**Grenzen und offene Arbeiten:** Diese Verbesserung beseitigt nicht die eigentlichen Fehler einzelner Server-Assets. Sie verhindert stattdessen endlose schnelle Retries beziehungsweise permanente lokale Sperren. Tatsächliche Simulator-Synchronisierung, Avatar-LocalID/Appearance, Bewegungsanzeige sowie Regions-Teleport bleiben wie vereinbart für spätere Fehlerblöcke offen. Ein genereller Netzwerkanfrage-Abbruch mittels Cancellation-Token ist nicht implementiert.

**Nächster Release (3–4 Blöcke):** In-World-Integrationsdiagnostik und Echtzeit-Aussagekraft, bessere Unterscheidung von Cache-/Serverfehlern, mögliche asynchrone Netzwerk-Cancellation und Asset-Streaming-Queue-Optimierung; die verbindliche Roadmap bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl64-live.log`; maßgeblicher Branch ist `develop-current`.


## 0.2.0-alpha.65 – Phase 9, vier In-World-/Asset-Diagnose-Blöcke · 11.10.2026

**Block A – Cache-Miss- und Dateifehlerdiagnose:** `AssetCacheLookup` und die optionale Statusausgabe von `TextureDiskCache::load()` unterscheiden `hit`, `miss`, `corrupt`, `io_error`. Der bereits vorhandene Disk-Cache-Header, die CRC-artige FNV64-Inhaltsprüfung, die 256-MiB-Ressourcengrenze und UUID-Validierung bleiben kompatibel. Texturworker berichten Disk-Misses, beschädigte Einträge und Dateisystemfehler getrennt von HTTP-Versuchen. Diese Quellenklassifikation verändert keine Serverantworten.

**Block B – koordinierte Regions-Epochen im Asset-Worker:** Der bestehende atomare Live-Epochenmarker aus dem Textur-Prefetch wird auch von Demand-GetTexture und GetMesh verwendet. Worker prüfen die Epoche vor Beginn und unmittelbar vor einem bislang nicht gestarteten HTTP-Download. Nach einem bereits laufenden HTTP-Download prüfen sie erneut vor dem Schreiben in den RAM-/Disk-Cache. Die `load_mesh_cached`-Funktion bietet dafür einen optionalen Admission-Callback. Die GPU-Ladepfade verwerfen nach wie vor veraltete Epochen. Bereits laufendes libcurl-HTTP kann noch nicht per Cancellation-Token unterbrochen werden.

**Block C – In-World-Health-Telemetrie:** Nach Aufbau der Simulation schreibt das Rendering alle etwa 30 Sekunden eine Kennzahlenzeile `InWorld asset health` (Regionsepoche, Anzahl der Szenen-Samples, gewünschte vs. bereits GPU-residente UUIDs und Deckungsgrad in Prozent, fehlende Assets, laufende Texture-/Mesh-Worker, Retry-Einträge und geschätzte GPU-Mesh-/Objekttexturbytes). Ohne aktive Simulation erfolgt keine falsche In-World-Bestandsmeldung. Die Coverage-Metrik bedeutet nur UUID-Residency, nicht vollständige grafische Darstellung, Avatar- oder Netzwerk-Korrektheit.

**Block D – Regressionen und Detailzähler:** Der Diskcache-Test prüft saubere Treffer, Miss und beschädigte Datei. Der Meshcache-Test prüft zwei Abbruchsituationen, einmal vor HTTP und einmal nach Abschluss einer künstlich veralteten Downloadfunktion; in beiden Fällen gelangt nichts Falsches in RAM/Disk. Der neue Test `asset_scene_coverage_tests` prüft leere Szenen, deduplizierte UUIDs, teilweise und vollständige Abdeckung. Die GetTexture-/GetMesh-Streaminglogs nennen zusätzlich `skipped_before_http`, `stale_after_http`, `disk_corrupt` und `http_attempts`.

**Teststand:** C++23-Debug-Build und 42/42 CTests nach Implementierung erfolgreich. Für die finale Version erfolgt nochmals vollständiger Rebuild und Linux-OpenGL-3.3-Welcome-Smoke. Es gibt weiterhin keinen authentifizierten NexVerse-In-World-Livetest; die In-World-Kennzahlen sind instrumentiert, aber noch nicht an realen Simulatordaten verifiziert.

**Unverändert offen:** Eigentliche serverseitig fehlende/beschädigte Mesh- und Texturassets, Avatar-/WASD-/Teleport-Funktionsdefizite und die vollständige visuelle Weltabnahme. Die Fehler werden protokolliert, aber nicht fälschlich als bereits behoben markiert. Keine echte libcurl-Abbruch-API in diesem Release.

**Nächste 3–4 Blöcke:** Realistisches Asset-Recovery-Verhalten unter Last, Retry-Schleifen und Prioritäts-Stabilisierung, strukturierte Diagnostics-/Health-Snapshots statt nur Textlogs und Renderer-/Szenenabnahme. `projectplan.md` bleibt verbindlich.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl65-live.log`. Branch: `develop-current`.


## 0.2.0-alpha.66 – Phase 9, vier Streaming-/Diagnose-Blöcke · 11.10.2026

**Block A – stabile Asset-Auswahl:** `AssetRequestFairness` integriert die bereits vorhandene Kamera-/Entfernungs- und Partikel-Priorisierung mit der pro Asset/LOD geführten Retry-Tabelle. Neue, berechtigte Assets werden innerhalb ihrer bisherigen Reihenfolge bevorzugt; bei einem bereiten Retry wird jeder vierte tatsächliche Dispatch für eine Wiederholungsanfrage reserviert. Bei ausschließlich bereiten Retries werden diese unmittelbar berücksichtigt. Ungültige UUID-Leereinträge, vollständig geladene Assets, Budget-Deferrals und noch aktive Cooldowns bleiben ausgeschlossen. Per-Region-Zähler werden beim Epochensprung zurückgesetzt. Das ist eine kleine Fairness-Schicht vor den bestehenden unabhängigen Textur-/Mesh-Demand-Workern, keine parallele Netzwerk-Multiplexer-Queue.

**Block B – stabile Retry-Zustände unter Last:** `AssetRetrySchedule` setzt bei mehr als 4096 Einträgen nicht mehr die gesamte Retry-Tabelle zurück, sondern verdrängt nur den Eintrag mit dem frühesten Retry-Zeitpunkt. Neue `blocked_count(now)`- und `ready_retry_count(now)`-Metriken trennen noch gesperrte Assets von wieder zulässigen Wiederholungsversuchen. Die bestehenden kurzen 2-/4-/8-Sekunden-Transient-Retries und längeren Cooldowns aus alpha.64 bleiben erhalten. Alte Fehlerzustände werden weiterhin bei einem Regionsepochen-Wechsel invalidiert.

**Block C – strukturierte In-World-Streaming-Snapshots:** Zu den vorhandenen 30-Sekunden-Health-Logs kommt `InWorldAssetSnapshot` mit maschinenlesbarem, numerischem JSON-Schema `ogl.asset.streaming.v1`. Felder: Regionsepoche, Szenensamples, gewünschte/geladene/offene Textur-/Mesh-UUIDs samt Abdeckungsprozent, aktuell laufende Asset-Worker, noch gesperrte und retryfähige Fehler, RAM-Cachebytes und die geschätzte GPU-Mesh-/Objekttexturbelegung inklusive gemeinsamem Softwarebudget. Ohne `simulation_ready` wird kein In-World-Snapshot behauptet; ein leeres Desired-Set ergibt rechnerisch 100 % Coverage und ist kein Nachweis einer vollständigen Welt.

**Block D – Streaming-Integrationstests:** Drei neue CTests: Fairness/Reihenfolge/Duplikate/Deferred-Assets/LOD-Keys, JSON-Schema inklusive leerer Region sowie die gemeinsame Planung aus Texture-Residency, Retry, Byte-Budget, Region-Reset und Health-Snapshot. Zudem ein Stress-Test mit 4098 Failure-Einträgen und den getrennten blocked-/ready-Kennzahlen. C++23-Debug-Build und 45/45 CTests bestanden; vollständiger Release-Rebuild und GUI-Starttest nach Versionserhöhung folgen.

**Wichtige Abgrenzung:** Die neue Integrationstest-Suite prüft die Asset-Planungs- und Budgetkomponenten, nicht einen authentifizierten In-World-Renderer- oder OpenSim-Server-End-to-End-Test. Die bereits bekannten Defizite (Avatar-Körper und Bewegung, fehlende Meshobjekte, fehlerhafte Assetantworten, Teleport und EventQueue) bleiben im Projektplan dokumentiert und werden wie vereinbart in späteren Fehlerbehebungsphasen behandelt. Die GPU-Bytes bleiben geschätzte Mesh-/Objekttexturwerte ohne Terrain-, UI- und Treiber-Overhead.

**Nächster 3–4-Blöcke-Release:** Datengetriebene Auswertung realer Simulatorlogs und Regionen, Entzerrung von Textur-/Mesh-LOD-Swaps, Renderer-GPU-Upload- und Slot-Backpressure sowie reproduzierbare In-World-Testpfade. `projectplan.md` bleibt die maßgebliche Roadmap.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl66-live.log`. Alle Änderungen auf `develop-current`; `develop` nicht automatisch verwenden.


## 0.2.0-alpha.67 – Phase 9, vier LOD-/GPU-/Loganalyse-Blöcke · 11.10.2026

**Block A – tatsächliches Viewerlog analysiert:** `tools/ogl_asset_log_report.py` (Python 3, Standardbibliothek) fasst das reale `$HOME/.local/state/opengenesislink/viewer.log` sowie künftige `InWorldAssetSnapshot`-Zeilen in einem numerischen, UUID-/CAP-URL-freien JSON-Bericht zusammen. Der historische Stand vom 11.10.2026 enthielt 230 Mesh- und 56 Texture-Fehlermeldungen, davon 42 mit 404 und eine mit 5xx-Muster; 243 weitere sind historische unklassifizierte Meldungen. Validierte In-World-Snapshots waren in dieser Datei noch nicht vorhanden. Dokumentierte Bedienung: `docs/LOG_ANALYSIS.md`.

**Block B – LOD-Dwell gegen Flackern:** Die neue generische `AssetLodDwell<Quality>` verlangt für wechselnde GPU-residente Mesh-LODs und Textur-Basismips eine über 1400 ms stabile Zielstufe. Neue noch nicht GPU-residente Assets werden sofort bedient; aktive Partikeltexturen müssen weiterhin mit höchster Bildauflösung angefordert werden. Die Anti-Oszillationszustände werden bei Regionswechsel geleert und von verschwundenen Scene-IDs bereinigt. Bereits bestehende Distanz-/Größen-LOD-Hysteresen bleiben wirksam.

**Block C – Renderthread-Upload-Backpressure und Slotfreigabe:** Der gemeinsame `AssetGpuUploadGate` hält mindestens 30 ms Abstand zwischen erfolgreichen Objekttextur- und Mesh-GPU-Uploads. Fertige Worker-Futures bleiben bis zum nächsten Upload-Fenster ausstehend; es gehen keine bereits dekodierten Daten verloren. `plan_texture_slot_admission` prüft zusätzlich zur bytebasierten Planung das 512-Slot-Limit: Ist das Limit erreicht, darf ein eingehender, höher priorisierter Textur-UUID einen niedrig priorisierten GPU-Slot ersetzen. Die Budgetrechnung berücksichtigt bereits geplante Byte-Evictions und verhindert die Verdrängung höher priorisierter Assets. Terrain-/UI-GPU-Uploads gehören nicht zur neuen Sperre.

**Block D – Regressionen und Telemetrie:** Zwei neue C++ CTests für LOD-Dwell/Regionsreset und GPU-Upload-Gate/Slot-Zuteilung; ein Python-CTest prüft den Parser mit Legacylogs, gültigem JSON, defektem Snapshot und Sanitizing unbekannter Felder. Die regulären Textur-Streaminglogs führen nun `mip_dwell_pending`, `slot_priority_evictions`, `gpu_uploads`, `gpu_uploaded_bytes`; die Mesh-Streaminglogs führen `lod_dwell_pending`. Ein vollständiger C++23-Debug-Build und 48/48 CTests waren erfolgreich; nach der Versionspflege werden sie erneut und zusätzlich der Linux-GUI-Smoke geprüft.

**Grenzen:** Die aus echten Logs gewonnenen Zahlen beschreiben historische Verbindungs-/Asset-Fehler und keine bestätigte Beseitigung der 404/Decoderprobleme. Neuer Renderthread-Takt reduziert zeitliche Upload-Spitzen, garantiert aber keine fixe Framezeit. Weiterhin ausstehend sind authentifizierte In-World-Abnahme, vollständige Mesh-Objekte, Avatar-Body/Bewegung, Teleport und EventQueue-Fehlerbehebung.

**Nächster Release (3–4 Blöcke):** Datenbasierte Prüfung anhand neuer In-World-Logs und GPU-Diagnose, weitere Upload-Messungen, scene-priority-Feedback und gezielte Render-Integration. `projectplan.md` ist weiter maßgebend; bei ausdrücklicher Nutzerpräferenz Fehlerbehebung separat.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl67-live.log`. GitHub-Branch `develop-current`.


## 0.2.0-alpha.68 – Phase 9, vier GPU-/Scene-Feedback-/Renderer-Blöcke · 11.10.2026

**Block A – reale CPU-Uploadzeit statt Worker-Wartezeit:** `AssetUploadTelemetry` misst die Wandzeit, die das Render-Thread-Aufrufen von `set_object_texture(...)` und `set_mesh_asset(...)` benötigt. Das feste Ringfenster hält die letzten 64 Uploadversuche und bietet `recent_avg_us`, `recent_p95_us`, `recent_max_us`, `recent_count`; Nullzustand und Überlauf werden konservativ behandelt. Messgröße ist **CPU-seitige OpenGL-Übermittlung**, keine GPU-Fertigstellungszeit, kein `glFinish` oder OpenGL-Timer-Query.

**Block B – adaptives GPU-Upload-Backpressure:** `AssetGpuUploadGate` bleibt bei schnellen Uploads bei mindestens 30 ms Abstand; langsame CPU-Übermittlungen verlängern das nächste Uploadfenster auf etwa 4× beobachtete Millisekunden, bis maximal 150 ms. Der nächstfolgende kurze Upload senkt das Intervall wieder auf den Grundwert. Mesh und Textur teilen sich den Renderthread-Gate. Bereits fertiggeladene Futures bleiben für spätere Frames verfügbar. Terrain/UI und vorbestehende Renderer-Arbeiten unterliegen dem Gate nicht.

**Block C – Szenenpriorisierung und erweiterte Health-Snapshots:** `AssetScenePriorityFeedback` beobachtet alle etwa 750 ms die GPU-gewünschten UUID-Sets, erkennt erhebliche Wechsel (symmetrische Differenz mindestens ein Viertel der vorherigen/neuen Set-Größe) und erhöht für drei stabile Planzyklen den weichen Loaded-Bonus temporär: Mesh von 24 auf 42 gewichtete Meter, Textur von 8 auf 14. Neue näher gelegene Assets bleiben weiterhin auswählbar; keine UUID wird fest gepinnt. Regionsepochen löschen den Zustand. `InWorldAssetSnapshot` behält `ogl.asset.streaming.v1`, ergänzt rein numerische Felder zu CPU-Uploadavg/p95/max, Sampleanzahl, effektiver Uploadsperre und Texture-/Mesh-Prioritäts-Churn. Der vorhandene Log-Analyzer wertet zulässige Felder aus und berechnet Peak-p95 bzw. Peak-Churn; er gibt keine CAPS-URLs/UUIDs aus.

**Block D – echter Renderer-Integrationstest:** `renderer_gpu_asset_integration_tests.cpp` erzeugt einen unsichtbaren echten GLFW/OpenGL-3.3-Kontext, lädt/repliziert/entfernt synthetische Textur und Mesh auf dem GPU-Pfad, prüft ungültige Meshdaten, führt einen Renderer-Frame aus, kontrolliert `visible_objects`, Renderer-Backend und GL-Fehlerfreiheit. Ohne OpenGL-Display überspringt CTest den Test via Exitcode 77; auf dem autorisierten Linux-PC wurde der Test unter `DISPLAY=:0` vollständig ausgeführt. Zwei weitere C++-Tests prüfen Rolling-Telemetrie und Scene-Prioritätsrückkopplung; erweiterte JSON-/Log-/Upload-Gate-Regressionstests. Gesamt: **51/51 CTests bestanden**, inkl. realem OpenGL-Test, anschließend vollständiger Versions-Rebuild/GUI-Smoke.

**Grenzen:** Der Renderer-Integrationstest arbeitet mit synthetischer Geometrie und einem lokalen GPU-Kontext, ohne authentifizierte NexVerse-Region oder echtes Asset. Er belegt noch nicht, dass Avatar-Body/Bewegung, Mesh-Objekte aus dem Simulator, CAPS-EventQueue, Teleport oder alle Weltassets korrekt sind. Diese bekannten Punkte bleiben bis zur vereinbarten späteren Bugfixphase dokumentiert. CPU-Uploadzeiten dürfen nicht als verifizierte GPU-Latenz bezeichnet werden.

**Nächster 3–4-Blöcke-Release:** Wiederholbare Smoke-/GPU-Profiling-Sessions anhand synthetischer Szenen unterschiedlicher Größe, Renderer-Upload-Fehlertransparenz, Scene-Priority-Schutz unter voller Asset-Residency und Ausbau der testbaren Welt-/Mesh-Renderpfade. Maßgeblich bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl68-live.log`. CLI-Logauswertung: `python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"`; Source Branch `develop-current`.


## 0.2.0-alpha.69 – Phase 9, vier GPU-Residency-/Fehlerdiagnose-Blöcke · 11.10.2026

**Block A – echter synthetischer GPU-Kapazitäts-/Belastungstest:** `tests/renderer_gpu_pressure_tests.cpp` erzeugt im versteckten OpenGL-3.3-Kontext auf dem Linux-Testrechner tatsächlich 512 kleine GPU-Objekttexturen und 256 kleine GPU-Meshes (Renderer-Grenzen). Bei 16, 128, 256 und 512 Texturen wird ein Frame mit den aktuell angelegten Szenenobjekten gezeichnet; Tests prüfen `visible_objects`, `draw_calls`, OpenGL-Fehlerfreiheit, abgewiesene 513. Textur/257. Mesh, korrekte Ersetzung bestehender Ressourcen sowie erneute Aufnahme nach Slot-Freigabe. Beispiel: unter dieser synthetischen Auslastung ca. 35 ms CPU-Gesamttestzeit inklusive Context-Init/Uploads/Draws. Diese Zeit ist ausdrücklich KEIN Benchmark realistischer größerer OpenSim-Assets.

**Block B – verlässliche GetTexture-GPU-Upload-Rückmeldung:** Die Renderer-Schnittstelle `Renderer::set_object_texture` meldet nun `bool` statt `void`. `LegacyOpenGlRenderer` baut Texturen immer erst in einem neuen GL-Namen auf, prüft Dimensionen und GL-Fehler, übernimmt sie nur bei erfolgreichem Upload und löscht den vorherigen Textur-Namen erst danach (transaktionale Ersetzung). Bei fehlerhaften Daten, vollem internen GPU-Slotlimit oder GL-Fehlern kehrt sie `false` zurück, ohne eine vorhandene Textur derselben UUID zu zerstören. Die Anwendung zählt `texture_gpu_upload_failures`, gibt eine Warnung aus, plant einen transienten Retry und trägt fehlgeschlagene Uploads nicht mehr als GPU-resident ein. Überlegungen zur atomaren Freigabe *anderer* prioritätsbedingt zuvor verdrängter Texturen bleiben ausdrücklich offen.

**Block C – robusterer GetMesh-GPU-Pfad:** Schon vor GPU-Submission prüft der Renderer Meshvertex-Stride, Dreiecks-Indexvielfache, jeden Index gegen die tatsächlich vorhandenen Vertices und angegebene Face-Bereiche gegen den Indexbuffer. Der eigentliche GL-Mesh-Upload kontrolliert Fehler und gültige VAO/VBO/EBO-Handles und gibt teilweise angelegte GL-Ressourcen bei Fehlern frei. Vorheriger LOD-Buffer bleibt bei gescheiterter Ersetzung erhalten. Viewer meldet `mesh_gpu_upload_failures`, klassifiziert GPU-Uploadfehler als transient und plant einen Backoff statt eines permanenten `invalid`-Fehlerstatus.

**Block D – Vollauslastung und Logdiagnose:** Der neue CTest `asset_full_residency_tests.cpp` übt 80 neue vorrangige Partikeltexturen bei 512 belegten Slots und eine Kamera-Fahrt über 400 Mesh-UUIDs bei 256 GPU-Slots aus. Er kontrolliert Priorität, Freigabe, Slotlimits und das Verbleiben naher/nützlicher Assets. `InWorldAssetSnapshot` führt zusätzliche Zähler `texture_gpu_upload_failures` und `mesh_gpu_upload_failures` im vorhandenen numerischen Schema `ogl.asset.streaming.v1`; der Python-Loganalysator nimmt sie in die sichere Feldauswahl auf und zeigt die jeweiligen Maximalwerte. JSON-, GL-Renderer- und Log-Regressionstests angepasst. C++23-Debug-Build und **53/53 CTests** inklusive zweier realer OpenGL-Kontexttests erfolgreich; vollständiger Release-Rebuild, Versionstest und GUI-Start folgen.

**Unverändert offene Grenzen:** Ein OpenGL-Test mit kleinen künstlichen Assets validiert Ressourcenhandling und GPU-Slots, aber weder echtes GetMesh/OpenSim-Decoding noch Live-Weltgeometrie, Avatar, Netzwerk-EventQueue, Teleport und In-World-Interaktion. Tests laufen auf echtem GL3.3/NVIDIA-Linux mit `DISPLAY=:0`; ohne GL-Display überspringen sich die beiden GPU-Kontext-CTests per Returncode 77. Shared GPU-Budget von 384 MiB ist eine Software-Schätzung ohne Terrain/UI/Treiber-Speicher. Die atomare Textur-Ersetzung gilt nur für denselben UUID-Slot, nicht für bereits vorab verdrängte andere Slots.

**Nächster Release / verbindlicher Plan:** Schrittweise GPU-Upload-Rollback/Recovery bei Slot-Evictions, realistischere Szenengeometrien/Texture-Mip-Lastprofile, strukturierte Upload-Failure-Gründe und weitere Renderer-Timing-Integration. Avatar/Bewegung/Mesh-/Teleport-/CAPS-Bugfixes wie vom Nutzer vereinbart separat.

Run: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Pressure einzeln: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-pressure-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl69-live.log`; nur `develop-current` nutzen.


## 0.2.0-alpha.70 – Phase 9, vier GPU-Transaktion-/Profiling-Blöcke · 11.10.2026

**Block A – texturschonender, transaktionaler GPU-Austausch:** `Renderer::replace_object_texture(uuid, image, evictions)` übernimmt ein komplett neues OpenGL-Textur-Handle erst nach erfolgreichem `glTexImage2D` und `glGenerateMipmap`. Erst danach werden die vorab ausgewählten Eviction-UUIDs gelöscht und die neue Textur eingebunden. Bei Fehlschlag bleiben alle bisherigen UUID-Slots, inklusive der Eviction-Kandidaten, erhalten. Der Viewer spiegelt die Freigaben erst nach erfolgreichem GL-Commit in `object_texture_loaded_ids`, `texture_gpu_bytes`, Mip-Tabellen und GPU-Bytezähler; abgelehnte Uploads zählen nicht als geladene Ressource. Der bisherige bool-Aufruf `set_object_texture` bleibt als Wrapper erhalten.

**Block B – strukturierte GPU-Uploadergebnisse für Texture UND Mesh:** `GpuUploadResult`/`GpuUploadFailure` enthält `success`, `failure`, `gl_error` mit Klassen `none`, `invalid_input`, `capacity`, `unsupported_dimensions`, `allocation`, `driver_error`. Beide Rendererpfade bieten strukturierte Methoden; der bisherige bool-Mesh-Aufruf ist ein Wrapper. Bei Texturen werden GL-Größenlimits sowie GL-Uploadfehler gemeldet, beim Mesh getrennt fehlerhafte Index-/Vertex-/Face-Daten, belegte 256 Mesh-Slots und sonstige Driver-Fehler. Application-Logs nennen nun `gpu_failure` (und bei Texturen `gl_error`), invalid/unsupported nutzt längeren invalid-Cooldown, transient/kapazitätsbedingt kurzzeitige Retries. Eine verlässliche OpenGL-Mesh-Fehlernummer wird im Renderer bei GL-Fehlern noch nicht als separates numerisches Feld exportiert und ist als Folgeschritt dokumentiert.

**Block C – reproduzierbare GPU-Upload-Lastprofile mit großen Assets:** Neuer CTest `renderer_gpu_profiles_tests.cpp` verwendet unsichtbaren ECHTEN OpenGL-3.3-Context und prüft 40 × 64², 20 × 256² und 6 × 1024² RGBA-Texturen sowie Mesh-Vertexgrids 16/32/64 (8/8/4 Instanzen, bis zu 8192 Dreiecke je Grid). Für jede Stufe werden GPU-Uploads, Renderframe, sichtbare Objekte und GL-Fehler geprüft; CPU-Upload-Zeiten (nicht GPU-Fertigstellungszeit) pro Gruppe protokolliert. Beispiel Linux RTX 4070 SUPER am 11.10.2026: Texture 283/369/4604 µs und Mesh 17/61/157 µs je Upload. Diese Zahlen gelten nur für synthetische Assets/diesen Testlauf, nicht für typische Serverdownloads oder FPS.

**Block D – Rollback-/Überbelegungs-Regressionen:** `renderer_gpu_pressure_tests.cpp` prüft weiterhin tatsächliche Belegung 512 Texturen/256 Meshes, jetzt zusätzlich Transaktions-Rollback bei ungültigem Input, fehlender Slotkapazität, unerlaubter Selbsteviction sowie übergroßen Texturen; erfolgreiches Commit entfernt exakt den vorgesehenen GPU-Victim und macht die neue ID resident. Vorhandene Mesh-Überbelegungs- und ungültige-Index-Tests prüfen die passende strukturierte Fehlerklasse. Insgesamt nach Implementierung **54/54 CTests bestanden**, inklusive drei echter OpenGL-Context-Tests; nach finaler Versionspflege erneut Full-Rebuild und GUI-Smoke.

**Wichtige technische Grenze:** Der Transaktionspfad hält den alten GPU-Textur-Slot und die neue Kandidatenressource kurzfristig gleichzeitig im VRAM. Bei sehr knappem physischem VRAM kann die sichere Transaktion scheitern und die alten Texturen behalten – ergebnisabhängig beabsichtigtes Schutzverhalten, aber später mit zusätzlicher Budgetreserve/Backpressure zu optimieren. Netzwerk-CAPS, echte Meshasset-/Avatar-/Bewegungs-/Teleportfehler wurden wie vereinbart nicht als bereits behoben deklariert. Der OpenGL-Integrationstest ist synthetisch, nicht authentifiziert in einer NexVerse-Region.

**Nächste vier Blöcke:** Strukturiertes Mesh-GL-Fehlerdetail, VRAM-Transientbudget/Reserve bei Rollback-Uploads, Instrumentierung echter Weltobjekt-Draws und Aufbau eines reproduzierbaren In-World-Akzeptanztests. `projectplan.md` bleibt verbindlich, und die ausdrücklich aufgeschobenen Avatar/Bewegungs/EventQueue-Bugfixes bleiben späteren Blöcken vorbehalten.

Start: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Profile: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-profiles-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl70-live.log`; nur `develop-current`.


## 0.2.0-alpha.71 – Phase 9, Mesh-GL-/VRAM-/Draw-/Akzeptanz-Blöcke · 11.10.2026

**Block A – konkrete GetMesh-OpenGL-Fehlernummern:** Der vorhandene GL-Uploadhelfer `upload_mesh` kann nun über einen optionalen `GLenum*` die tatsächlich gemeldete OpenGL-Fehlernummer an `Renderer::upload_mesh_asset` weiterreichen. `GpuUploadResult.gl_error` ist bei GL-Fehlern gesetzt; `GL_OUT_OF_MEMORY` wird als `allocation`, andere GL-Fehler als `driver_error` klassifiziert. Der Viewer protokolliert bei gescheiterten Mesh-Uploads `gpu_failure` und `gl_error` zusammen mit Retry-Delay. Kein GL-Fehler/fehlgeschlagene Handle-Anlage kann weiterhin `driver_error` mit `gl_error=0` ergeben; diese semantische Grenze ist beabsichtigt.

**Block B – transaktionale GPU-Textur-Staging-Reserve:** Neue reine C++-Hilfsfunktion `assets::gpu_staging_budget(texture_resident_bytes,mesh_resident_bytes,incoming_bytes,shared_budget_bytes)`. Sie prüft vor einer Kandidaten-Upload-Transaktion den kurzzeitigen geschätzten Höchstbedarf, während Altressourcen für Rollback noch auf der GPU liegen. Sicherheitsabstand: `min(16 MiB, shared_budget / 32)`; bei 384 MiB geteiltem Assetbudget also 12 MiB. Überlauf und Nullbudget werden sicher behandelt. Ein unzureichendes Staging-Budget führt zur nichtdestruktiven Deferral statt zum sofortigen Entfernen alter Texturen; zusätzliches Diagnosefeld `texture_gpu_staging_deferrals` und Warnlog mit numerischen Bytes. Die Angaben sind Software-Schätzungen, **keine** Garantie real freien VRAMs einschließlich Terrain/UI/Driver-Overhead.

**Block C – echte Weltobjekt-Renderzähler:** `RenderDiagnostics` liefert jetzt zusätzlich `scene_draw_calls`, `scene_mesh_draw_calls`, `scene_prim_draw_calls`. Zähler werden je Frame zurückgesetzt und ausschließlich beim tatsächlich ausgeführten glDrawElements aus der Weltobjekt-/Face-Schleife fortgeschrieben; Sky, Terrain, Wasser, Avatar-Proxies, UI zählen nicht dazu. Mesh-Zähler erfordern ein tatsächlich vorhandenes GPU-Mesh-Handle für die Mesh-ID, sonst zählt der tatsächlich gerenderte Fallback als Prim. `InWorldAssetSnapshot` enthält das letzte beendete Frame als `previous_frame_*`, wodurch In-World-Protokolle zwischen Asset-Residency und wirklichen Mesh-Draws unterscheiden. Synthetische Linux-OpenGL-Tests bestätigen Mesh UND Prim sowie die Summe aller Szene-Draws.

**Block D – reproduzierbarer In-World-Akzeptanzprüfer:** `tools/ogl_inworld_acceptance.py` wertet sanitizte Berichte des bestehenden `ogl_asset_log_report.py` aus; erforderlich sind mindestens 3 **aufeinanderfolgende Snapshots derselben Regionsepoche**, aktive Region, nichtleere gewünschte Textur-/Meshsets, mindestens 75 % Coverage beider Familien und ein tatsächlich ausgeführter Scene- sowie GPU-Mesh-Draw im neuesten Snapshot. Nur dann liefert die CLI `evidence_pass`/Exitcode 0; ansonsten `not_verified`/Exitcode 2. JSON liefert pro Kriterium booleschen Prüfstatus; keine UUID/CAPS-URL wird ausgegeben. Zwei zusätzliche CTests für GPU-Reserve und Akzeptanzlogik, erweitert um Regionsepochenwechsel, ergänzen die Renderer-/Snapshot-Regressionen. C++23-Debug und 56/56 CTests wurden nach Implementierung erfolgreich geprüft.

**Lokale Ist-Daten / korrekte Abgrenzung:** Die autorisierte Datei `$HOME/.local/state/opengenesislink/viewer.log` enthielt am 11.10.2026 weiterhin 0 valide `InWorldAssetSnapshot`-Zeilen; der neue Akzeptanzprüfer lieferte folgerichtig `not_verified` mit Exitcode 2. Die Datei verzeichnet historische 230 Mesh- und 56 Texture-Fehler (42 mit 404-Muster, eine mit 5xx-Muster); daraus kann keine aktuelle in-world Rendering-Freigabe abgeleitet werden. Auch `evidence_pass` allein würde Avatarsteuerung, Teleport und EventQueue **nicht** bestätigen. Diese Problemfelder bleiben wie vereinbart in einer späteren Bugfixphase.

**Nächste zusammenhängende Releaseblöcke (alpha.72):** Verbesserte In-World-Snapshot-Serienvalidierung mit Zeitstempeln, konkreter Renderer-Frame-Mesh-Face-Abnahme, GPU-Staging-Deferral-Lebenszyklus/Recovery sowie weiteres Mesh- und Texture-Streaming-Feedback. `projectplan.md` bleibt verbindlich.

CLI: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`; CMake: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Branch ausschließlich `develop-current`.


## 0.2.0-alpha.72 – Phase 9, vier Snapshot-/Face-/Recovery-Blöcke · 11.10.2026

**Block A – belastbare Snapshotserien statt altem Log-Zufall:** `AssetStreamingSnapshot` erzeugt bei `simulation_ready` ergänzend `session_started_unix_ms` (Prozesskennung als Startzeit), `emitted_unix_ms` und monoton steigende `sequence`; Schema `ogl.asset.streaming.v1` bleibt additive-only. Der Parser `tools/ogl_asset_log_report.py` zählt als neueste zusammenhängende Serie nur Snapshots derselben Session UND Regionsepoche mit lückenloser Sequenz und realistischem zeitlichen Abstand 15–90 s. Der CLI-Akzeptanzprüfer `tools/ogl_inworld_acceptance.py` nutzt `ogl.inworld.acceptance.v2` und prüft zusätzlich den Echtzeitstatus: letzter Snapshot maximal 180 s alt (10 s Uhr-Toleranz), 3 zusammenhängende Snapshots, nichtleere Desired-Textur- und Mesh-Sets, jeweils mindestens 75 % Residency und tatsächlich ausgeführte Scene- bzw. Mesh-Draws. `--max-age-seconds`, `--min-snapshots`, `--min-coverage` sind konfigurierbar. Ohne die neuen Zeitfelder oder bei historischen Logs ist das Ergebnis absichtlich `not_verified` (Exit 2), kein erfundener Liveerfolg.

**Block B – realer GPU-Mesh-Face-Nachweis:** OpenGL-RenderDiagnostics unterscheidet nun die erfassten `scene_mesh_draw_calls` (wirklich hochgeladenes Mesh) und `scene_missing_mesh_draw_calls`: Letztere zählen echte Prim-Fallback-Draws für Objekte, die eine Mesh-UUID anfordern, aber keine passende GPU-Meshressource besitzen. Ein erweiteter echter GLFW/OpenGL-3.3-Test lädt 2 getrennte Face-Indexbereiche, prüft exakt 2 GPU-Mesh-Drawcalls plus Prim-/Missing-Mesh-Fallback sowie den Frame-Reset nach dem Entfernen der Meshressource. Das In-World-Snapshotfeld `previous_frame_scene_missing_mesh_draw_calls` und der sanitized Log-Report `peak_missing_mesh_fallback_draws` liefern eine konkrete Anzeige der bisher fehlenden Meshobjekte, aber noch keine Fehlerbehebung des OpenSim-Meshdownloads.

**Block C – GPU-Staging-Deferrals werden wieder freigegeben:** `GpuStagingRecovery` speichert zurückgestellte Textur-UUIDs mit dem zugehörigen geschätzten Texture-/Mesh-Byteverbrauch und kontrollierter Cooldownzeit. Ein erneuter Versuch wird ermöglicht, sobald eine der beiden Assetfamilien weniger GPU-Speicher benötigt oder der Retry-Zeitpunkt erreicht ist; wiederholte Deferrals verwenden je UUID steigende 2/4/8/16/32/64-s-Cooldowns, ohne beim Freigeben der Retry-Anfrage die Historie zu verlieren. Erfolgreiche Uploads löschen die Historiereihe; bei Regionswechsel Reset; bei geänderter Desired-Residency verschwundene Assets werden bereinigt. Laufende Worker werden nicht zwangsweise unterbrochen. `texture_gpu_staging_waiting` / `texture_gpu_staging_released` und die bereits vorhandenen Deferrals gehen in In-World-JSON/Streaminglogs und den sicheren Report ein.

**Block D – Regressionen/Streaming-Feedback:** Ein neuer C++-CTest prüft Memory-Drop-Retry, Cooldown-Aufstieg, Recovery, Resolution, Prune und Region-Reset. Der Python-Akzeptanz-CTest prüft fehlende, veraltete und synthetische valide Serien sowie Sessionwechsel, Epochensprünge, Sequenzlücken und unrealistische Abstände; die Renderer-Integration prüft die Multi-Face-Geometrie, tatsächliche GL-Zeichnungen und fehlende Mesh-GPU-Residency. Erwartet sind 57/57 CTests inkl. dreier echter GL-Context-Tests. Die numerische Log-Whitelist wird um die neuen Snapshotfelder erweitert, ohne Asset-UUIDs/CAPS-URLs in Ausgaben aufzunehmen.

**Lokaler Status am 11.10.2026:** Die autorisierte `$HOME/.local/state/opengenesislink/viewer.log` enthält noch 0 neue `InWorldAssetSnapshot`-Zeilen. Der überarbeitete CLI-Prüfer liefert `not_verified`/Exitcode 2. Das ist technisch korrekt und darf nicht als abgeschlossene Live-NexVerse-Regions-, Avatar-, Mesh- oder Bewegungsabnahme dargestellt werden. Bekannte EventQueue-/Teleport-/Avatarprobleme bleiben wie gewünscht auf dem späteren Bugfix-Pfad. Außerdem sind GPU-Bytegrenzen nur Software-Schätzungen und keine Treiber-VRAM-Garantie.

**Folgerelease Alpha.73 / 3–4 Blöcke:** GPU-Streaming-Retry-Integration und Budget-Pressure-Regressionen mit realem GL-Kontext, weitere Missing-Mesh-/Face-Fallback-Statistik, wiederholbare Langzeit-OpenGL-Szenentests sowie echte In-World-Log-Akzeptanz sobald Regionenzugang/Assets bereit sind. Maßgeblich ist weiter `projectplan.md`.

CLI auf dem autorisierten Linux-PC: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"` (Exitcode 0 nur für neueste gültige Live-Daten). Branch ausschließlich `develop-current`.


## 0.2.0-alpha.73 – Phase 9, vier GPU-Full-Slot-/Soak-/Missing-Mesh-Blöcke · 11.10.2026

**Block A – vollständig belegte GetTexture-GPU-Slots korrekt behandeln:** Die Demand-Anfrageprüfung vor HTTP blockierte bisher jede neue Asset-UUID, sobald bereits 512 Texturen GPU-resident waren, obwohl die bestehende `plan_texture_slot_admission` danach bereits eine niedriger priorisierte Textur verdrängen kann. In `src/app/application.cpp` wird bei voller Tabelle jetzt eine rangbewusste Preflight-Slotzulassung (gewünschte und zur Räumung markierte Resident-UUIDs) durchgeführt; nur unmögliche Transfers werden deferred. Nach dem Download prüfen Bytebudget, Staging-Reserve, Slotpriorität und transaktionaler Renderer-Commit weiterhin die tatsächliche Machbarkeit. Unit-Test erweitert für 512 belegte Slots und die Nichtverdrängung höher priorisierter Objekte.

**Block B – tatsächlicher OpenGL-Soak-Test mit GPU-Pressure und Recovery:** `tests/renderer_gpu_soak_tests.cpp` erstellt einen versteckten echten OpenGL-3.3-Kontext mit 24 Weltobjekten, davon 20 gültige GPU-Meshes und vier bewusst nicht hochgeladene Mesh-UUIDs, plus acht echte GPU-Texturen. In 160 gerenderten Frames ersetzt es wiederholt Texturen, ersetzt GPU-Meshes und kontrolliert pro Frame GL-Fehlerfreiheit, korrekte Drawcall-Summe, vorhandene GPU-Mesh-Draws und stabile Zahl fehlender Meshobjekte. 40 gezielte *simulierte* Software-Staging-Druckereignisse werden mit `gpu_staging_budget`/`GpuStagingRecovery` freigegeben; 160 Transaktionen tatsächlich im Renderer ausgeführt. Beispiel auf der Linux RTX 4070 SUPER: 43 ms CPU-Laufzeit für diese kleine synthetische Szene, kein FPS- oder realistischer Simulatorperformance-Benchmark. Ohne GL-Kontext überspringt sich der CTest (Exit 77).

**Block C – präzise Mesh-Fallback-Metriken:** Der Renderer zählt zusätzlich zu bisherigen pro Frame ausgeführten Missing-Mesh-Fallback-Drawcalls jetzt **einzigartige betroffene Szenenobjekte** und die wirklich als Ersatz gezeichneten **Dreiecke**. Da eine Region pro Objekt mehrere Materialfaces und transparente/opaque Renderpasses besitzt, vermeidet ein Frame-lokales Set eine Überzählung der Objekte. Die numerischen `InWorldAssetSnapshot`-Keys `previous_frame_scene_missing_mesh_objects` und `previous_frame_scene_missing_mesh_triangles` sowie der sanitizte Logbericht zeigen das Ergebnis. Reale OpenGL-Multi-Face- und Mesh-Entfernungs-Tests prüfen diese Zähler; fehlende OpenSim-Meshdownloads werden damit diagnostiziert, nicht behauptet repariert.

**Block D – verschärfte In-World-Abnahme und Streaming-Regressionen:** `tools/ogl_inworld_acceptance.py` verlangt nun standardmäßig null zuletzt tatsächlich gerenderte Missing-Mesh-Fallback-Draws, zusätzlich zu frischer Session-/Epoch-/Zeitserie, nichtleerer Region, >=75 % Texture- und Mesh-Residency und tatsächlichen GPU-Mesh-Draws. Nur für bewusst teilweise bereite Szenen kann der Aufrufer `--max-missing-mesh-draws N` setzen. Fehlende Diagnosefelder gelten nicht als null und können keinen Pass erzeugen. Python-Cases prüfen strikten Status und Override; C++-Regressionen GPU-Full-Residency/Byte-Slot-Vorabzulassung/Snapshot werden mit GL-Tests ausgeführt. Insgesamt **58/58 CTests bestanden** (erster vollständiger Entwicklungsbuild).

**Abnahmegrenze:** Auf der autorisierten Linux-Workstation ist die frühere `viewer.log` weiterhin ohne aktuelle In-World-Snapshotserie (`not_verified`), insbesondere ohne Prüfung echter NexVerse-Mesh-, Avatar- und Simulatorinteraktionen. Synthetische Tests ersetzen keinen erfolgreichen Simulatorbeitritt. Seit Alpha.70 bewusst später zu beheben: fehlender Avatar, WASD/Bewegung, Teleport, CAPS EventQueue, teilweise fehlende Meshobjekte. Die aktuelle Arbeit führt den maßgeblichen `projectplan.md` ohne ungefragte Fehlerbehebungsphase fort.

**Nächste 3–4 Releaseblöcke Alpha.74:** Zeitliche Priorität und aktive GPU-Budget-Deferral-Diagnose bei geteiltem Tex-/Mesh-Speicher, Wiederverwendung von Assets über Regions-/LOD-Wechsel, erweitertes realistisches GL-Szenen-/Materialtestprofil und automatische Prüfung wirklich neuer Regionlogs sobald echte Server- und Loginbedingungen erfüllt sind.

Lokaler Debug/Soak: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-soak-tests`. In-World-Prüfer: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repo `marlin198323-glitch/nexverse-viewer`, einzig maßgeblicher Branch `develop-current`.


## 0.2.0-alpha.74 – Phase 9, vier GPU-Budget-/Cache-/Material-Blöcke · 11.10.2026

**Block A – geteiltes Tex-/Mesh-GPU-Budget und Mesh-Retry-Recovery:** Mesh-Anfragen, deren dekodierte GPU-Geometrie das kombinierte Budget überschreitet, werden nicht mehr unbegrenzt allein durch `mesh_gpu_budget_deferred_ids` blockiert. Neuer `GpuStagingRecovery` auch für Meshes, mit Key `UUID.LOD`, progressivem 2/4/8/16/32/64-s-Cooldown und vorgezogener Re-Evaluierung nach sinkendem Texture- oder Mesh-Residency-Byteverbrauch. Bei Regionswechsel Reset, bei geänderter Scene-Desired-Liste Prune, nach erfolgreichem Mesh-Upload Resolution. Dieselbe Recovery wird nun auch bei nicht passendem Texture-Byte-/Slotplan verwendet, nicht nur bei Staging-Reserveverletzung. Numeric Snapshotfelder `mesh_gpu_budget_waiting`, `mesh_gpu_budget_released`, plus bestehende Textur-Deferrals. Risiken der nur geschätzten (Terrain/UI/Driver nicht eingeschlossenen) GPU-Bytegrenzen bleiben.

**Block B – tatsächliche wiederverwendete CPU-Assets, keine falsche GPU-Hot-Reuse-Behauptung:** Die bisherigen Mesh-/Texture-RAM- und Disk-Caches bleiben regionsübergreifend im Speicher bzw. auf Festplatte bestehen; GPU-Handles werden aus Sicherheitsgründen weiterhin bei Regionswechsel geräumt. Der neue begrenzte `AssetReuseHistory` (max. 4096 UUIDs/Familie) beobachtet ausschließlich erfolgreiche erneute GPU-Uploads und zählt sie als `cached_region_reuse` oder `cached_lod_swap` NUR, wenn der Worker ausdrücklich einen RAM-/Disk-Cache-Hit meldete. Texture-Mipqualität und Mesh-LOD werden pro UUID erfasst, unabhängig vom Cache-Tier. Snapshotfelder für `texture_cpu_cached_region_reuse`, `texture_cpu_cached_lod_reuse`, `mesh_cpu_cached_region_reuse`, `mesh_cpu_cached_lod_reuse`; gleichartige Streaming-Logs. Keine Gewähr, dass dieselbe UUID bei Regionwechsel auf einem fremden Server existiert oder identische GPU-Handles weiterverwendet werden.

**Block C – langlebiger realer OpenGL-Material-/Szenentest:** Der bereits echte GLFW/OpenGL-3.3-Soak mit 160 Frames, 160 transaktionalen Texture-Replacements, 40 simulierten Staging-Deferrals, 20 GPU-Meshes plus vier absichtlich fehlenden Meshobjekten testet jetzt abwechselnd opake und teiltransparente Texturen, per-Face-Alpha-Tints, Fullbright, Shininess sowie Repeat-/Offset-Materialparameter. Szenenmaterial wird in jedem Frame aktualisiert. Echte OpenGL-Frames, `scene_mesh_draw_calls`, `scene_prim_draw_calls`, `scene_missing_mesh_objects`, GL-Fehlerfreiheit und GPU-Assetreplacements werden kontrolliert. Das ist eine synthetische GPU-/Legacy-Material-Regressionsprüfung, keine vollständige PBR- oder NexVerse-InWorld-Visualabnahme.

**Block D – End-to-End GPU-Pressure/Queue-Diagnostik und reproduzierbare Tests:** Neuer CTest `asset_gpu_pressure_pipeline_tests.cpp` simuliert gemeinsam beanspruchtes 384-MiB-Softwarebudget: eine blockierte Mesh-Anfrage wird unmittelbar nach 12-MiB-Texturfreigabe wieder möglich, Staging schützt alte GPU-Assets, Textur-Cooldown läuft ab, Regionswechsel leert Deferrals, RAM-/Disk-Hit bleibt auf Cacheebene für folgende Region nachvollziehbar. Neuer Unit-Test `asset_reuse_history_tests.cpp` kontrolliert bounded History, Regionswechsel, Qualitätswechsel und Nicht-Cache-Fälle. Der Log-Analyser nimmt neue numerische Snapshotfelder in die Whitelist auf und gibt Spitzenwerte der mesh-budget-Warteschlange frei, ohne UUIDs/CAPS-Adressen/Secrets auszugeben; Python-Test prüft dies. Damit **60/60 CTests** im ersten vollständigen Debug-Testlauf.

**Lokale Live-Abgrenzung 11.10.2026:** Der unverändert mit der autorisierten Workstation vorliegende `$HOME/.local/state/opengenesislink/viewer.log` enthält noch keine gültigen `InWorldAssetSnapshot`-Serien; die CLI `tools/ogl_inworld_acceptance.py` ergibt `not_verified`/Exitcode 2 (0 geeignete Snapshots). Keine verifizierte Aussage zur vollständigen NexVerse-Welt, Avatar-Body, WASD/Bewegung, Teleport, fehlenden realen Meshobjekten oder EventQueue. Diese bereits bekannten Themen verbleiben auf der ausdrücklich später geplanten Fehlerbehebungsphase.

**Nächster geplanter Stand alpha.75:** Noch gezielteres CPU/GPU-Cache-Residency-Management mit eindeutiger Kennzeichnung von HTTP-/RAM-/Disk-Treffern und anhaltender Budgetlast, stärkere GL-Legacy-Material-/Multi-Face-Szenen, prüfbare Wiederverbindung nach Regionenwechsel und aussagekräftige Auswertung echter In-World-Testlogs, sobald Authentifizierung und Regionsdaten verfügbar sind. `projectplan.md` bleibt die verbindliche Roadmap.

Repo `marlin198323-glitch/nexverse-viewer`, aktiver Branch `develop-current`, lokale Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Standard: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; anschließend GUI-Smoke und GitHub-SHA-Vergleich.


## 0.2.0-alpha.75 – Phase 9, vier CPU-Cache-/GPU-Deferred-/Region-GL-/Akzeptanz-Blöcke · 11.10.2026

**Block A – deterministic bounded CPU-Asset-Reuse (RAM vs Disk):** `include/nexverse/assets/asset_reuse_history.hpp` was changed from arbitrary `unordered_map.begin()` eviction to real LRU with a `std::list` and O(1)-average lookup/touch/evict. The 4096 UUID/family limit is maintained. New `AssetReuseHistory::Tier {network,memory,disk}` distinguishes actual reported successful memory/disk-backed GPU reuploads at a later RegionEpoch or different texture mip / mesh LOD; counters `region_ram_hits`, `region_disk_hits`, `lod_ram_hits`, `lod_disk_hits` and `evictions`, plus prior combined hit counts. Application maps the texture loader's explicit `RAM texture cache hit` / `Disk texture cache hit` and mesh `MeshCacheTier` into this tier. New numeric `InWorldAssetSnapshot` and safe log parser fields distinguish the cache provenance (including history LRU evictions). This is CPU-/disk-cache reuse; GPU textures and VBOs remain invalidated on region changes.

**Block B – targeted GPU Deferred/Retry lifecycle:** `GpuStagingRecovery::synchronize(blocked, desired)` prunes obsolete blocked UUIDs in lockstep with active desired sets, **preserving cooldown for still-required UUIDs** and unblocking orphans that have no recovery deadline; a corresponding regression test verifies this. Texture plan changes no longer indiscriminately clear all deferred IDs; specific changed mip-levels release their own state, and full-slot preflight rejection explicitly adds a timed recovery record. Mesh desired-Lod set changes use synchronized `UUID.LOD` deferred keys rather than a blanket clear. Existing actual GPU-memory-eviction code can still trigger early retry, and its software VRAM estimates still exclude other driver allocations. No unauthenticated network or EventQueue behavior is asserted fixed.

**Block C – real native GL3.3 three-face region transition test:** New `tests/renderer_region_transition_tests.cpp` creates a hidden actual GLFW/OpenGL 3.3 context. It uploads three independently materialized faces, including opaque texture, blended tint and alpha-textured face. It verifies three GPU mesh Face-Draws and no missing-Mesh fallback. It then simulates a region change by clearing all GPU meshes and textures, validates correct Prim fallback without any stale GPU mesh, and reuploads the SAME immutable UUID with refreshed texture bytes; a new rendered frame must again show 3 GPU Mesh Face-Draws and 0 missing mesh objects. All stages check `glGetError()`; skip code 77 if no GL context. The test is synthetic, not a real authenticated OpenSim region reconnect.

**Block D – series-wide latest-session In-World acceptance:** The log analyzer now computes only from the most recent contiguous valid `(session_started_unix_ms, region_epoch, sequence, 15–90 s interval)` series `latest_region_min_texture_coverage_pct`, `latest_region_min_mesh_coverage_pct`, `latest_region_min_mesh_draw_calls`, `latest_region_max_missing_mesh_fallback_draws` and `latest_region_max_mesh_budget_waiting`. `tools/ogl_inworld_acceptance.py` now rejects an apparently healthy latest frame if the current series contains an earlier low-coverage frame, absent required mesh-draw evidence, or missing-mesh fallback. Missing fields result in `not_verified`, not assumed zeros. New dedicated `tests/test_ogl_inworld_transition_report.py` tests healthy series, degraded middle frame, missing mesh in earlier frame, missing telemetry field and session/epoch boundaries. End-to-end C++ / OpenGL and Python regression count **62 CTests**, all passed before final version rebuild.

**Proven live status:** The authorized workstation's `$HOME/.local/state/opengenesislink/viewer.log` still contains **zero** valid current `InWorldAssetSnapshot` samples; acceptance returns `not_verified` with exit code 2. It is not possible to claim a real NexVerse Region/Avatar/Movement/Teleport/EventQueue acceptance from the GL tests or these logs. Those user-deferred issues remain in the project roadmap for later troubleshooting.

**Next large release alpha.76 (3–4 blocks):** cache-pressure eviction and persistent asset-tier observations, region-switch cancellation and stale HTTP event contract tests (without assuming live simulator connectivity), wider material/texture transparency scenario coverage, explicit failure diagnosis of missing Real Mesh UUIDs once real region snapshots exist. Keep the planned staged sequence in `projectplan.md`; don't silently move debugging of WASD, avatar, Teleport or CAPS queue ahead.

Build: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Live check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Private GitHub repo `marlin198323-glitch/nexverse-viewer`, branch ONLY `develop-current`.


## 0.2.0-alpha.76 – Phase 9, vier Cache-/RegionsHTTP-/OpenGL-/Fehleranalyse-Blöcke · 11.10.2026

**Block A – echte CPU-RAM-Cache-Belastungstransparenz:** Die beiden bestehenden LRU-Caches `TextureMemoryCache` und `MeshMemoryCache` erfassen nun direkt `hits`, `misses`, `evictions`, `bytes` und `entries`. Der Zugriff auf die Statistiken ist mutexgeschützt, die Evictionzählung erfolgt ausschließlich beim wirklichen Verdrängen eines älteren Eintrags. Ein neuer gemeinsamer Header `asset_memory_cache_stats.hpp` hält das numerische Messformat. Der Viewer überträgt je Familie Treffer, Fehlzugriffe und Verdrängungen in `InWorldAssetSnapshot` (`texture_ram_hits`, `texture_ram_misses`, `texture_ram_evictions`, `mesh_ram_hits`, `mesh_ram_misses`, `mesh_ram_evictions`). Der sanitizte Python-Logbericht berücksichtigt die Felder und berechnet `peak_texture_ram_evictions` / `peak_mesh_ram_evictions`. Vorhandene CPU-RAM-Cachetests prüfen die neuen Zähler und Speichergrenzen.

**Block B – kontrolliert verspätete HTTP-Antwort beim Regionswechsel:** Neuer nativer Test `tests/region_http_epoch_contract_tests.cpp`: Eine asynchrone, simulierte GetMesh-HTTP-Antwort wird nach Beginn des Requests in Region A gezielt zurückgehalten. Nach dem Übergang zu B wird die alte Antwort freigegeben und muss an der vorhandenen Epochensperre verworfen werden, ohne die Mesh-Disk-/RAM-Caches zu befüllen. Anschließend wird ein neuer Request im aktiven B akzeptiert. Auch der Rückwechsel B → A erhält eine neue Epoche und erlaubt einen authentifizierten Cachetreffer, ohne dass die alte Antwort wieder gültig wird. Dies prüft reale asynchrone Steuerlogik mit kontrolliertem Transportcallback und existierender `RegionMaterialScope`/`load_mesh_cached`-Integration, jedoch keinen echten Login/HTTP-Endpunkt.

**Block C – umfangreicherer nativer OpenGL-3.3-Material-/Multiface-Test:** `renderer_region_transition_tests.cpp` prüft weiterhin GPU-Freigabe und Wiederaufbau derselben Mesh-UUID nach einem Regionswechsel. Ergänzend rendert der echte versteckte GLFW/OpenGL-3.3-Kontext **48 Frames mit zwei Objekten und jeweils drei Mesh-Faces**. Face-Tint/Alpha, transparente Textur, Fullbright, Glow, UV-Rotation, UV-Repeat/Offset und Planar-Texturmodus wechseln. Pro Frame müssen sechs GPU-Mesh-Face-Draws, null Missing-Mesh-Objekte, konsistente Drawcall-Summen und GL-Fehlerfreiheit vorliegen. Diese synthetischen Renderer-Regressionen prüfen technische Pfade, nicht die visuelle Pixelgenauigkeit einer realen OpenSim-Region.

**Block D – kategorisierte Missing-Mesh-/GetTexture-Ausfälle:** Neues `AssetFailureTelemetry` mit numerischen Zählern `missing`, `transient`, `invalid`, `configuration`, ohne UUID/CAPS-Adresse. Der reale Applikationspfad zählt kategorisiert fehlgeschlagene Texture- und Mesh-Downloads bzw. GPU-Rejections (mit bestehenden AssetFailure-Regeln); bei erfolgreichen GPU-Uploads wird keine Fehlerzählung erhöht. `InWorldAssetSnapshot` ergänzt `mesh_fail_{missing,transient,invalid,configuration}` und `texture_fail_{missing,transient,invalid,configuration}`. Der vorhandene sichere Python-Analyzer nimmt die Felder auf und meldet `max_mesh_missing_events`, `max_mesh_transient_events`; Tests kontrollieren Kategorieabbildung, Snapshotformat und bestehende Cacheauswertung. Zusammen **64/64 CTests** im Entwicklungsbuild, darunter native GLFW/OpenGL-Tests und der neu hinzugefügte zeitversetzte Regions-HTTP-Test.

**Bewertungsgrenzen / Ist-Status:** Ein Simulator-HTTP-404 / 410 wird getrennt als `missing` dokumentiert, Retry-/Serverfehler als `transient` und Fehlkonfiguration separat. Diese Kategorien erlauben später eine gezielte Ursachenanalyse der realen fehlenden Meshobjekte, beheben aber noch nicht den OpenSim-Netzwerkdownload oder die CAPS-EventQueue. Der autorisierte echte lokale `viewer.log` enthält am 11.10.2026 weiterhin 0 geeignete `InWorldAssetSnapshot`-Zeilen; In-World-Prüfer meldet `not_verified`/Exitcode 2. Ohne authentifizierte Region kann weder Avatar, Bewegung, Teleport noch die Bildqualität vollständig abgenommen werden. Diese bekannten Probleme bleiben wie vereinbart für die spätere Bugfixphase vorgesehen.

**Nächste 3–4 koordinierte Blöcke für Alpha.77:** Auswertung von Cachemiss-Raten und Prioritätskontrolle unter längerer Streaminglast, zuverlässige stale-Response-Trennung auch für Texturdownloads/Prefetch in isolierten Regressionstests, Mesh-Face-/Material-Transparenz-Routing samt GPU-Speicherlast und Diagnose tatsächlicher OpenSim-Regiondaten sobald verfügbar. Der verbindliche `projectplan.md` bestimmt weiterhin die Reihenfolge.

Build/Test auf der autorisierten Workstation: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.


## 0.2.0-alpha.77 – Phase 9, vier Cachepressure-/Texture-HTTP-/GL-/Missing-Asset-Blöcke · 11.10.2026

**Block A – 30-s-Intervallanalyse echter RAM-LRU-Belastung:** Neues `AssetCachePressureWindow` berechnet nicht nur kumulative, sondern delta-basierte Hitquote und Anzahl echter Evictions pro Asset-RAM-Cache-Sampling-Intervall. `under_pressure` wird nur bei mindestens 8 neuen Cache-Misses, mindestens 2 Verdrängungen und einer Trefferrate unter 50 % aktiviert. Der Renderer beobachtet Textur- und Mesh-RAM-Cache getrennt und veröffentlicht `texture_ram_hit_rate_pct`, `texture_ram_interval_evictions`, `texture_ram_pressure` und entsprechende `mesh_*`-Werte in `ogl.asset.streaming.v1`. Bei aktivem Texturcache-Druck setzt das freiwillige **Disk-only-Texture-Prefetching** aus, während GetTexture-Anforderungen benötigter sichtbarer Weltassets weiterhin starten dürfen. Regionswechsel resetten die Fenster. Cache-Pressure-CTest deckt Druck, Recovery, Nulländerung und Counter-Reset ab; die Schwellwerte sind konservativ und noch nicht live kalibriert.

**Block B – GetTexture-Worker mit überprüfbarer Regionsantwortsperre:** Die bisher nur in `src/app/application.cpp` kopierte RAM/Disk/HTTP-Mip-Laderoutine ist in `include/nexverse/assets/texture_cached_request.hpp` als `load_texture_cached_scoped` extrahiert; die App verwendet diese unmittelbar mit dem existierenden authentifizierten `TextureAssetLoader`, den CPU-/Disk-Caches, gewünschtem Mip und dem atomischen Regionsepochen-Accept-Callback. Vor HTTP und nach einer Antwort wird die Epoche kontrolliert; eine verspätete HTTP-Antwort wird **vor** der RAM-/Disk-Cache-Übernahme verworfen. Zusätzlich wird eine vom angefragten UUID abweichende erfolgreiche Antwort als `invalid` abgelehnt. Der neue native CTest `texture_http_epoch_contract_tests.cpp` hält eine simulierte HTTP-Antwort gezielt bis nach dem A→B→C-Wechsel zurück, prüft keine stale Cache-Befüllung, erfolgreiche Antwort der aktuellen Region, GPU-Mip-reduziertes Arbeitsergebnis bei weiterhin voller RAM-Assetauflösung, RAM- und Disk-Treffer. Das ist ein kontrollierter Transportvertragstest, keine externe Netzabnahme.

**Block C – echte OpenGL Opaque-vs-Alpha-Face-Telemetrie:** `RenderDiagnostics` zählt bei tatsächlich ausgeführten Szene-`glDrawElements` nun getrennt `scene_opaque_draw_calls` und `scene_alpha_draw_calls`, die sich genau auf `scene_draw_calls` summieren müssen. Beide Zahlen werden beim Frame-Beginn zurückgesetzt und als `previous_frame_scene_opaque_draw_calls` / `previous_frame_scene_alpha_draw_calls` in In-World-Health-Snapshots aufgenommen. Die echten GLFW/OpenGL-3.3-Tests `renderer_region_transition_tests.cpp` (2 Meshobjekte, je 3 Materialfaces, 48 wechselnde Materialframes) und `renderer_gpu_soak_tests.cpp` (160 Frames mit Texture-Updates und fehlenden Meshes) fordern nun tatsächliche Draws in BEIDEN Durchläufen und exakte Summengleichheit. GPU-/Materialfunktionalität im synthetischen GL-Kontext bewiesen, nicht Pixelidentität mit dem Live-Simulator.

**Block D – datensparsame Ursachen-/Missing-Asset-Diagnostik:** Neues `AssetMissingIdCounter` speichert maximal 4096 verschiedene intern beobachtete UUID-Strings pro Region/Familie und zählt nur `AssetFailure::missing`, nicht Transient-/GPU-Failures; bei vollem Set werden Overflow-Ereignisse separat gezählt. Regionswechsel setzt die ID-Zählung zurück. Die App zählt so `mesh_missing_unique`, `mesh_missing_unique_overflow`, `texture_missing_unique`, `texture_missing_unique_overflow` und schreibt nur deren numerische Werte in `ogl.asset.streaming.v1`, niemals die eigentlichen UUIDs/CAPS-Adressen. Der sanitizte Python-Analyzer berechnet auf der neuesten gültigen Region-Snapshot-Serie `latest_region_mesh_missing_events_delta`, `latest_region_texture_missing_events_delta`, `latest_region_peak_missing_mesh_unique`, `latest_region_peak_texture_ram_pressure`, `latest_region_peak_mesh_ram_pressure` und die minimalen Cache-Hitraten. `ogl_inworld_acceptance.py` enthält diese Befunde unter `observations`, verändert jedoch keine bisher gültigen Nachweiskriterien. Zusätzliche C++- und Python-Tests prüfen Bounded/Reset/Overflow, zeitliche Serienaggregation und keinen Secret-Leak. Insgesamt **67/67 CTests** im Entwicklungsbuild erfolgreich.

**Live-Status / Abgrenzung 11.10.2026:** Die echte lokale `$HOME/.local/state/opengenesislink/viewer.log` auf der autorisierten Workstation enthält weiterhin 0 gültige aktuelle `InWorldAssetSnapshot`-Zeilen. `tools/ogl_inworld_acceptance.py` liefert `not_verified`, Exitcode 2. Der Code kann ohne authentifizierten Regionsbeitritt nicht als visuelle Abnahme von Meshes, Avatar, Welt, WASD/Bewegung, Teleport oder CAPS-EventQueue gelten. Diese bereits bekannten Mängel bleiben wie vereinbart für die spätere Fehlerbehebungs- und In-World-Abnahmephase dokumentiert. Hinweis: Der begrenzte Missing-ID-Counter meldet bei Sättigung eine Untergrenze unterschiedlicher UUIDs plus Overflows, keine exakte unbeschränkte Kardinalität.

**Nächster geplanter Release alpha.78, 3–4 Blöcke:** Cache-Pressure-Werte gegen fortlaufende Simulator-Samples validieren; transparentes Texture-/Mesh-Fehlverhalten mit vollständigem Regionswechsel und Asset-Staging prüfen; zusätzliche GPU-Szenenfälle für unterschiedliche Alpha-Mask-/Blending-Kontrakte und eine aussagekräftige Diagnose der wirklich vom Server fehlenden Assets sobald Live-Daten vorliegen. Maßgeblich bleibt `projectplan.md`.

Debug-Kommandos: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, nur Branch `develop-current`.


## 0.2.0-alpha.78 – Phase 9, vier Cache-Hysterese-/Stale-Prefetch-/AlphaMask-/Analyseblöcke · 11.10.2026

**Block A – RAM-Cache-Druck ohne Prefetch-Pendeln:** `AssetCachePressureWindow` benötigt nach aktivem Texture-/Mesh-Thrashing jetzt zwei aufeinanderfolgende unkritische 30-s-Auswertungsfenster, bevor `under_pressure` zurückgesetzt wird. Dadurch pausiert das rein optionale Texture-Disk-Prefetch auch nach einem zufällig ruhigen Einzelintervall weiter; erforderliche GetTexture-Netzwerkrequests bleiben unabhängig. Ergänzter Unit-Test für die Hysterese und neuer End-to-End-C++-Test `asset_cache_pressure_lru_tests.cpp`, der tatsächlich `TextureMemoryCache` und `MeshMemoryCache` mit jeweils nur zwei residenten Assets über 24 wechselnde UUIDs belastet, echte Hits/Misses/Evictions beobachtet und anschließend die zwei stabilen Recovery-Intervalle sowie den regionsübergreifend erhaltenen RAM-Inhalt kontrolliert.

**Block B – veraltetes Disk-Prefetch während Regionswechsel:** Produktionsfunktion `prefetch_texture_disk_to_memory` delegiert jetzt an einen separat kontrollierbaren `prefetch_texture_disk_to_memory_scoped`-Vertrag. Der integrierte `texture_prefetch_tests.cpp` hält eine asynchrone, gültige Disk-Leseantwort zwischen Start und Cache-Aufnahme an, wechselt dann die Regionsepoche und gibt den Worker frei. Das alte Texture-Bild darf nicht in die RAM-LRU übernommen werden. GetTexture-Hauptpfad und zuvor bestehende HTTP-Regionsepochen-Sperren bleiben unverändert. Das ist ein deterministischer Worker-/Cache-Vertragstest, kein echter Grid-Download.

**Block C – OpenGL3.3-Alpha-Mask-vs-Blend-Routing:** Neue zentrale CPU-Routingfunktion `face_material_requires_blending(tint_alpha,loaded_texture_has_alpha,mode)`. AlphaMode=1 berücksichtigt Texture-Alpha für den transparenten Pass; Mode=2 behandelt Texture-Alpha als GPU-Discard-/Mask-Bedingung im grundsätzlich opaken Pass, sofern Tint-Alpha 1 ist; Mode 0/3 lösen nicht allein wegen Texture-Alpha einen Blend-Pass aus. Der Shader führt `discard` bei AlphaMode 2 wie zuvor aus. Neuer tatsächlich ausgeführter Frame-Zähler `scene_alpha_mask_draw_calls`, zusätzlich zu bestehenden `scene_alpha_draw_calls` und `scene_opaque_draw_calls`, wird in `RenderDiagnostics` und im anonymisierten `InWorldAssetSnapshot.previous_frame_scene_alpha_mask_draw_calls` ausgegeben. Die echten OpenGL-Regionstransitions-/Mesh-Face-Tests prüfen Mask-Faces und sowohl opaque als auch blended Drawcalls; `material_pass_tests.cpp` enthält alle vier Moduskontrakte.

**Block D – realistische Cache-Druck- und Renderer-Sitzungsanalyse:** `tools/ogl_asset_log_report.py` wertet neueste zusammenhängende gültige Region-Snapshotserien aus und berichtet `latest_region_texture_ram_pressure_samples`, `latest_region_mesh_ram_pressure_samples`, `latest_region_peak_alpha_draw_calls`, `latest_region_peak_alpha_mask_draw_calls`, `latest_region_peak_gpu_upload_p95_us`. `tools/ogl_inworld_acceptance.py` ergänzt die sicheren Werte unter `observations`; sie sind Diagnose, nicht automatisch ein weiterer erfolgreicher Abnahmebeweis. Python-Regressionen belegen Anonymisierung und korrekte serielle Metrik. **68/68 CTests** vor finalem Release-Rebuild bestanden, einschließlich echter GLFW/OpenGL-Szenen und CPU-LRU-Stress.

**Ist-/Abnahmegrenzen:** Autorisierte Linux-Workstation `$HOME/.local/state/opengenesislink/viewer.log`: weiterhin **0** gültige aktuelle `InWorldAssetSnapshot`-Einträge, `ogl_inworld_acceptance.py` meldet `not_verified`, Exitcode 2. Daher keine echte NexVerse-Region-, Avatar-, WASD/Bewegungs-, Mesh-Vollständigkeits-, Teleport- oder CAPS-EventQueue-Freigabe. Diese bekannten Themen bleiben wie vereinbart für die spätere Live-Integrations- und Fehlerbehebungsphase dokumentiert. Alle aktuell getesteten Alpha-/Staging- und Cache-Szenen sind synthetisch, auch wenn der tatsächliche NVIDIA-OpenGL3.3-Treiber verwendet wurde.

**Nächster 3–4-Blöcke-Release Alpha.79:** Transparenz-/Maske-Sichtbarkeit zusätzlich mit reproduzierbarer Pixel-/Framebuffer-Abnahme prüfen, kombiniertes Texture-/Mesh-Staging über mehrere Regionswechsel noch strenger validieren, Ressourcen-Counter/Hysterese bei typischen Cachegrößen profilieren und In-World-Akzeptanzmeldungen für echte Simulatorlogs verfeinern sobald aktuelle Regionsdaten vorliegen. Maßgeblich bleibt `projectplan.md`.

GitHub: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (alten divergenten `develop` nicht mergen). Tests `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Diagnose `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`.


## 0.2.0-alpha.79 – Phase 9, vier GPU-Pixel-/Region-/Cache-Matrix-/Live-Diagnose-Blöcke · 11.10.2026

**Block A – visuelle Pixel-/Framebuffer-Abnahme für Alpha, AlphaMask und opake Meshfaces:** Neuer echter GL3.3-GPU-CTest `tests/renderer_alpha_framebuffer_tests.cpp` erstellt ein verstecktes GLFW-Fenster, lädt ein GPU-Mesh und eine 2×2-Texture und liest nach `render_world()` den zentralen tatsächlich gerenderten RGBA-Pixel mit `glReadPixels`. Geprüft werden ein opakes rotes Face, vollständiger AlphaMask-Discard bei AlphaMode 2/Cutoff 128, anschließend restaurierte Opakheit und Blend-Rendering bei 50 % Alpha; Framepass-Zähler müssen mit der Sichtbarkeit übereinstimmen. Die maskierte Pixelreferenz ist ein vollständig transparenter AlphaMode-1-Face im **gleichen aktiven Szene-Rendermodus**, nicht ein No-Scene-Frame, da diese unterschiedliche Hintergrundpfade nehmen. Der neue GL-Test läuft auf dem echten NVIDIA OpenGL 3.3-Treiber und bestand. Eine synthetische Einzelpixelabnahme ist keine vollständige PBR-/OpenSim-Bildqualitätsfreigabe.

**Block B – drei vollständige GPU-Regionswechsel unter Mesh-/Texture-Staginglast:** Der bestehende tatsächliche GLFW/OpenGL-3.3-Test `tests/renderer_gpu_soak_tests.cpp` führt weiter 160 Frame-Draws, 160 transaktionale Texture-Replacements und 40 künstlich erzeugte Staging-Deferrals aus. Zusätzlich entfernt er bei Frame 40, 80 und 120 alle GPU-Texture- und Mesh-Ressourcen, setzt die Region-bezogene `GpuStagingRecovery` zurück und verifiziert vor dem Wiederaufbau **null GPU-Mesh-Draws, 24 fehlende GPU-Mesh-Objekte** und keine wartenden alten Staging-Einträge. Anschließend werden alle 20 gültigen synthetischen GPU-Meshes und acht Texturen frisch geladen; die vier absichtlich unbereit gehaltenen Meshobjekte bleiben als echte Fallbacks sichtbar. Alle Frame-/Passsummen und GL-Fehlerprüfungen müssen bestehen. Das testet GPU-Regions-Lebenszyklus und Upload-Recovery, nicht die Netz-/Simulatorintegration.

**Block C – Cache-Kapazitätsmatrix für echte Texture-/Mesh-RAM-LRUs:** Neuer C++-CTest `tests/asset_cache_capacity_profiles_tests.cpp` verwendet die realen `TextureMemoryCache`-/`MeshMemoryCache`-Implementierungen mit 8, 64 und 128 zulässigen Asset-Slots. Über sechs deterministische Zugriffswellen mit 96 verschiedenen UUIDs werden 1152 Zugriffe je Familie und Profil durchgeführt, inklusive bevorzugt wiederholten Hotspots, Hit/Miss und LRU-Evictions sowie Kapazitätsgrenzen. Beispiel-Testlauf: 8 Slots 383 Hits/769 Misses/761 Evictions; 64 Slots 875/277/213; 128 Slots 1056/96/0 pro Assetfamilie. Diese Testwerte sind reproduzierbare synthetische Zugriffsmuster und kein kalibriertes Profil realer OpenSim-Assetgrößen/Hardwarelatzenzen. Für alle drei Größen sind triviale Fehlzustände, unkontrolliertes Wachstum und fehlende Wiederverwendung abgesichert.

**Block D – konkrete, datensparsame In-World-Fehlerdiagnose:** `tools/ogl_inworld_acceptance.py` erweitert das bisherige `ogl.inworld.acceptance.v2` um `failure_codes` und `next_check`. Aus den existierenden konkreten Health-Checks werden nur feste sichere Diagnosecodes erzeugt, beispielsweise `no_current_region_snapshots`, `insufficient_region_samples`, `texture_residency_incomplete`, `gpu_mesh_draws_missing`, `mesh_fallback_still_drawn`; niemals UUIDs, CAP-URLs oder Rohlogs. Der empfohlene nächste Prüfpunkt priorisiert fehlende Live-Regionensnapshots, veraltete/unterbrochene Serien, nichtleere Asset-Demandsets, niedrige Residency und echte Mesh-/Fallback-Draws. `evidence_pass` erhält die leere Failure-Liste. Python-Regressionen prüfen fehlende Daten, valide Serien, veraltete/unterbrochene Daten und kritische Missing-Mesh-Fälle. Im aktuellen echten Viewerlog: **0** gültige InWorld-Snapshots, `not_verified`, `failure_codes=['no_current_region_snapshots']`, CLI-Exitcode 2. Der fehlende Live-Test wird ausdrücklich nicht als Fehlerbehebung oder als bestanden ausgegeben.

**Gesamtprüfung:** Debug-C++23-CMake-Build, reale OpenGL-Regressionen und **70/70 CTests** im Entwicklungsstand bestanden. Nach der Versionspflege vollständiger Neuaufbau und GUI-Welcome-Smoke auf NVIDIA RTX 4070 SUPER vorgesehen; GitHub-Push ausschließlich nach bestandener Endprüfung auf `develop-current`.

**Nächste vier geplante Releaseblöcke Alpha.80:** Erweiterte Pixelreferenzen für Mehrfach-Overlays und AlphaSort, kontrollierter GPU-Rollback-/Uploadfehler-Regionswechsel, Cache-/Residency-Priorisierung nach messbarer Druckphase und tatsächliche In-World-Test-Protokollierung sobald erfolgreiche Authentifizierung/Regionsdaten vorliegen. Bekannte Avatar-/WASD-/Teleport-/CAPS-EventQueue-Probleme bleiben wie vereinbart bis zur späteren Integrations-/Bugfixphase zurückgestellt; `projectplan.md` bleibt maßgeblich.

CLI auf autorisiertem Linux-Desktop: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Live-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.\n

## Architekturpriorisierung ab 11.10.2026 – Firestorm-Parität vor unkoordinierten OGL-Sonderfunktionen

Diese Neupriorisierung hat Vorrang vor älteren Passagen, die Firestorm nur als minimale Kernfunktionsreferenz oder besondere Firestorm-Werkzeuge als generell optional beschrieben. **Jedes Firestorm-Release-Feature wird einzeln inventarisiert**. Fehlende Server-CAPS/LLUDP-/LSL-Operationen müssen anhand des **aktuellen** OpenGenesisLINK-`main` nachgewiesen und als begrenzte Kompatibilität dokumentiert werden. Neue OGL-Plugins, Shader und UX-Erweiterungen dürfen die vollständige Firestorm-Baseline nur ergänzen oder gezielt umbauen.

**Verbindliche Referenzdokumente:** `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md` (Regeln, Quellen und Parität), `docs/FIRESTORM_PARITY_MATRIX.md` (offene Feature-Matrix, Status, Serverabweichungen). **Nächster Block nach alpha.79:** zuerst Firestorm-Feature-/Quellinventur und Current-Server-CAPS-/Feature-Diff; danach geeignete Port-/Adapter-Strategie samt Lizenzprüfung und gezielte Implementierung fehlender Benutzerfunktionen. Der bisherige Alpha.80-Renderer-Plan bleibt bestehen, aber darf die Firestorm-/OpenGenesisLINK-Paritätsaufgaben nicht verdrängen.

**Quellenprüfung dieser Entscheidung:** OpenGenesisLINK `main` bei `cb3ebb6ff05160bf9bdb20e78372a68d7b723512` am 11.10.2026; Stable `0.9.3.8`, Dev `0.9.3.10`. Dieser dokumentierte SHA ersetzt keine künftige HEAD-Abfrage. Offizieller Firestorm-Releasebranch als erste Inventurreferenz: `Firestorm_7.2.4`; neuere Releases vor den jeweiligen Umsetzungen prüfen. **Versionsnummer `0.2.0-alpha.79` bleibt bei dieser reinen Planungsänderung unverändert; kein falscher Release-Fortschritt.**


## 0.2.0-alpha.80 – Firestorm-CAPS, Experiences und Outfit-Thumbnail-Protokoll · 11.10.2026

**Block 1 – geprüfte Firestorm-Seed-CAP-Abdeckung:** `default_seed_capabilities()` fordert beim echten Login-Seed nun zusätzlich `FetchInventoryDescendents2`, `InventoryThumbnailUpload`, `FindExperienceByName`, `GetExperiences`, `AgentExperiences`, `ExperiencePreferences`, `GroupExperiences`, `GetAdminExperiences`, `GetCreatorExperiences`, `IsExperienceAdmin` und `UpdateExperience` an. Vorhandene `FetchInventory2` und `GetExperienceInfo` bleiben. Die Antwort bestimmt die tatsächliche Verfügbarkeit; nicht angebotene CAPS werden nicht simuliert. Der C++-Regressionstest kontrolliert einmalige Anforderung aller relevanten Namen.

**Block 2 – Server-capability-gesteuerte Firestorm-Bereitschaft:** Neue typisierte `FirestormCapsAvailability` mit acht klar definierten CAP-Kategorien (Inventaritem-Fetch, Inventarordner-Fetch, ThumbnailUpload, Owned/Allowed/Permissions/Admin/Update Experiences) berechnet rein aus der geprüften Seed-Antwort die angebotenen Funktionen, nie aus dem Vorhandensein von Beispiel-URLs oder dem Serverproduktnamen. Im produktiven Login-CAPS-Ladebildschirm erscheinen `Startregion: Outfit-Vorschau CAP/- | Experiences CAP/-`, optional die Anzahl eigener/erlaubter Experiences. `CAP` bedeutet *angebotener Endpunkt*, kein erfolgreicher Upload oder Firestorm-Funktionsparität. Der bisherige NexVerse-UI-Stil und die Simulation-Ready-Steuerung bleiben erhalten.

**Block 3 – Read-only Firestorm Experience-Listen, ohne EventQueue zu bremsen:** `parse_agent_experiences` akzeptiert strikt LLSD-Map mit begrenztem `experience_ids`-Array und unterscheidet `purchase`-Feld fehlend von `purchase: 0` (so verlangt es der aktuelle OpenGenesisLINK-Serververtrag für Firestorm). `parse_resident_experiences` validiert `experiences` und `blocked` getrennt. Die OGL-CAPS-Worker-Routine führt die beiden GETs nur bei tatsächlich angebotenen `AgentExperiences`/`GetExperiences` in einer separaten, beim Beenden gejointen Leseaufgabe aus, damit die EventQueue nicht durch diese optionalen Abfragen blockiert wird. Keine Experience-Purchase-/Allow-/Block-/Forget-Verwaltungsaktion erfolgt automatisch; Ergebnisse der ursprünglichen Login-Startregion werden nicht als aktuelle spätere Teleportregion behauptet. Es werden nur Anzahl und CAP-Verfügbarkeit angezeigt; keine Avatar-/Experience-UUID oder Secret-CAP-URL in neuen Diagnoseausgaben.

**Block 4 – Firestorm/OpenGenesisLINK Outfit-/Inventar-Thumbnail-Wire-Vertrag:** `build_thumbnail_begin_request(target,id)` erzeugt nur bei gültiger, nichtleerer Item-/Ordner-UUID das richtige LLSD-XML mit genau einem `item_id` oder `category_id`. `parse_thumbnail_stage` akzeptiert nur autorisierte Antwortformen `{state:upload,uploader:<HTTP(S)-URL>}` bzw. `{state:complete,new_asset:<UUID>}`. Ungültige UUIDs, Dateischemen und unbekannte Zustände werden verworfen. Dies entspricht dem aktuellen OpenGenesisLINK-Servermodul `OpenSim/Region/ClientStack/Linden/Caps/InventoryThumbnailUploadModule.cs`. Die Binär-JPEG2000-Übertragung, Besitz-/Rechteprüfung und endgültige OGL-Inventar-UI sind **nicht implementiert**; der Server selbst bindet One-Time-Uploader an aktive Owner-CAPS. Die serverseitig dokumentierte Firestorm-Outfit-Thumbnail-Cache-Limitation bleibt separat bestehen. `tests/firestorm_compat_tests.cpp` deckt Capability-Abgleich, Listenschema, Größenlimits, LLSD-Serialisierung und Upload-Handshake ab.

**Externe Codequellen, 11.10.2026:** Aktueller OpenGenesisLINK-Server `main` = `6dc73f2adfded7fe5ee94c992daaaba5be2b66c8`, Entwicklungszug 0.9.3.10 Dev / Stable 0.9.3.8, Commit vom 11.10.2026: Experience Manage API Keys können jetzt mit separatem `experiences:viewer:manage`-Scope ausgegeben werden. Dies ist **keine** Viewer-seitige Admin-Key-Nutzung: OGL darf niemals interne World-API-Secrets erhalten. Firestorm offizielle Referenzrelease-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` (Nightly `master` nicht als Release). Relevante Firestorm-Quelle `indra/newview/llfloaterexperiences.cpp` und OGLServer-Dokument `doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md` sowie `FIRESTORM_INVENTORY_THUMBNAIL_09310.md`. Server-/Client-SHAs vor jedem künftigen Featureblock neu vergleichen.

**Nachweis- und Einschränkungsstatus:** Erste vier Firestorm-Vertragsbausteine sind implementiert und lokal synthetisch getestet, aber kein vollständiger Firestorm-Fork und keine vollständige Experience-/Outfit-Benutzerfunktion. Die ursprüngliche reale In-World-Logserie enthält keine frischen Snapshotnachweise, Avatar/WASD/Mesh/EventQueue/Teleport bleiben noch zu integrieren. Weiterhin keine Behauptung einer erfolgreichen NexVerse-Live-Experience-/Inventar-Abnahme. Lizenzentscheidung für tatsächliche Firestorm-LGPL-Source-Übernahme bleibt offen; dieser Commit übernimmt keinen fremden Firestorm-Quellcode.

**Nächste 3–4 Blöcke Alpha.81:** Read-only Experience-Funktionen in die echte OGL-InWorld-Benutzeroberfläche mit regionengebundener Session-/CAP-Lease übernehmen; LLSD Inventar-/Outfit-Thumbnail-Metadaten inklusive Versionswechsel und Rechte abstrahieren; volle Firestorm-Menü-/Funktionsinventur und aktuelle Server-CAPS-Diffs weiter ausfüllen; anschließende kontrollierte Entwicklung von Upload/Experience-Consent mit echter Grid-Permission-/Owner-Abnahme. Firestorm-Parität hat Vorrang vor neuen Renderer-only-Erweiterungen.


## 0.2.0-alpha.81 – Firestorm Experience-Floater, Regions-Lease und Outfit-Metadaten · 11.10.2026

Vier zusammengehörende Firestorm-Paritätsblöcke:

1. **Bedienbare Experience-Ansicht:** Nach authentifizierter und bereiter Simulation öffnet F8 einen Floater im NexVerse-Design. Eigene/Erlaubte/Blockierte Experiences lassen sich über Maus oder die Tasten 1–3 auswählen, ESC/F8 schließt. Die Liste enthält echte Simulator-UUID-Präfixe mit ausdrücklich noch fehlenden Anzeigenamen statt erfundener Namen. Fehlende CAPS, noch nicht geladene Daten und leere Listen werden getrennt angezeigt. Solange die Ansicht geöffnet ist, löst WASD keine Avatarbewegung aus. Schreiben, Experience erwerben, Allow/Block/Forget bleiben gesperrt. Der Viewport vor erfolgreichem Simulator-Eintritt bleibt frei von der Experience-UI.

2. **Region-scoped Experience-Leseergebnisse:** FirestormRegionLease bindet Regionhandle, WorldModel-Generation und Seed-Identität an einen threadsicheren Epochentoken. Bei Regionswechsel werden alte CAP-Advert-Flags, Counts und Experience-UUID-Listen gelöscht, der Floater geschlossen. In-flight AgentExperiences/GetExperiences-Antworten werden nach dem Wechsel auch bei gültiger HTTP-200-Antwort verworfen; Lease-Check erneut unter Caps-Mutex. Die Endpoints und UUIDs dürfen nicht in Statuslogs erscheinen. Der EventQueue-Hauptworker bleibt unabhängig. Neuer deterministischer asynchroner CTest belegt die späte GET-Ablehnung. Grenze: Neues Experience-CAPS-Seed-Discovery nach Teleport ist noch nicht umgesetzt, daher wird absichtlich keine alte Liste weiter angezeigt.

3. **Inventar-/Outfit-Metadatenvertrag:** Der MPL-2.0-Adapter parse_inventory_thumbnail_metadata liest die OpenGenesisLINK-LLSD thumbnail.asset_id aus FetchInventory2/FetchInventoryDescendents2, unterscheidet fehlendes Feld, ausdrücklich leere (Zero-UUID) Vorschau und gültige Thumbnail-UUID, lehnt defekte LLSD-Strukturen ab. InventoryFolderThumbnailCache speichert zu Folder-UUID die Version und Asset-ID, lehnt ältere Folderversionen ab und akzeptiert geänderte Thumbnail-IDs auch bei gleicher Version, um die dokumentierte Firestorm-7.2.4-Outfit-Gallery-Caching-Einschränkung nicht blind zu übernehmen. Neuer CTest belegt leere, alte, ersetzte und ungültige Metadaten. Grenze: noch kein echter Inventar-GET/Outfit-Gallery-UI, kein JPEG2000-Upload, keine reale Server-Readback-Abnahme.

4. **Konkrete Funktionsparität und Verträge:** Offizielle Firestorm-Releasequelle Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d; belegt über indra/newview/llfloaterexperiences.cpp (Tabs, GetExperiences/AgentExperiences, Acquire-Button). Aktueller OpenGenesisLINK Server main @ 6dc73f2adfded7fe5ee94c992daaaba5be2b66c8; Belege doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md, doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md und serverseitige Thumbnail/Experience-Implementierung. Versionsgrenzen: OpenGenesisLINK 0.9.3.10 Dev und Firestorm 7.2.4.80712. Die Firestorm-Funktions-/Verhaltensbaseline bleibt verbindlich, der bestehende OGL-C++23-Viewer ist noch kein Firestorm-Code-Fork.

**Nachweise und Grenze:** 73/73 lokale CTests nach Integration, einschließlich zweier neuer Firestorm-Regressionen und realen GPU-Tests. Ein bestandener synthetischer Lease-/Metadatentest ist KEIN NexVerse-In-World-Test. Eine tatsächliche authentifizierte Regionsabnahme für diesen Stand fehlt weiterhin. Der F8-Floater wird nur im Simulationszustand gezeichnet; Login-Welcome-Smoke allein prüft keine Interaktion nach einem realen Simulatorbeitritt.

**Alpha.82-Priorität:** neue Regions-Experience-CAPS nach Teleport entdecken, Experience-Namen/Profile mit GetExperienceInfo lesen, servergebundene FetchInventory2/FetchInventoryDescendents2-Read-only-Outfit-Gallery aufbauen sowie Write/Consent/Upload nach separatem Owner-/Permissions-/Grid-Vertrag kontrolliert vorbereiten. Firestorm-Parität zuerst, Zusatzrenderer nachrangig.


## 0.2.0-alpha.82 – Region-CAPS, Experience-Profile und Login-Outfit-Galerie · 11.10.2026

**Block A – dynamische Firestorm-CAPS nach Region-/Teleportwechsel:** Neue Protokollroutine fetch_firestorm_region entdeckt AgentExperiences, GetExperiences, GetExperienceInfo, FetchInventory2/FetchInventoryDescendents2 und InventoryThumbnailUpload anhand des echten aktuellen Simulator-Seeds. Die Routine holt autorisierte Experience-Listen ausschließlich lesend; CAP-Verfügbarkeit bedeutet keinen bestätigten Betrieb und keine Schreibberechtigung. In der App laufen Region-Read-Worker unabhängig vom bestehenden Login-CAPS/EventQueue-Longpoll, mit maximal vier ausstehenden Anfragen und nichtblockierender Future-Abholung; bei erschöpften Slots wird der aktuellste Regionsrequest nach Freigabe erneut begonnen. Ergebnisse werden nur für den noch gültigen FirestormRegionLease-Token unter Caps-Mutex übernommen. Alte UUIDs/Capabilities werden beim Regionswechsel zuerst verworfen. Wichtig: Die allgemeine Simulatorverbindung/EventQueue-Reconnect wird dadurch nicht automatisch repariert; die Verfügbarkeit einer neuen Region setzt eine vom WorldModel gelieferte gültige Seed-CAP voraus.

**Block B – echte Experience-Anzeigenamen über Firestorm GetExperienceInfo:** Authentifizierte read-only Profile werden optional vom pro Region angebotenen GetExperienceInfo-Endpoint über die Firestorm-Query /id/?page_size=12&public_id=UUID abgefragt. Nur tatsächliche IDs aus eigenen/erlaubten/blockierten Listen, maximal zwölf, werden angefragt. Der LLSD-Parser verlangt experience_keys[] mit public_id und name, validiert UUID, begrenzt Namenslänge auf 128 Zeichen und lehnt Steuerzeichen ab. F8 zeigt bei Erfolg den vom Simulator gelieferten Experience-Namen; bei fehlender/nicht unterstützter CAP bleibt die eindeutige Kennzeichnung ID XXXXXXXX (Name nicht geladen). Regionsepoche schützt auch Profilresultate, keine CAP-URLs, Experience-UUIDs oder Admin-API-Keys in neuen strukturierten Logs.

**Block C – lesende Outfit-Galerie aus echten OpenSim-Loginordnern:** F9 öffnet nach erfolgreichem Eintritt in eine Simulation ein Fenster im NexVerse-Design. Firestorms echte LLFolderType-Codes FT_MY_OUTFITS=48 und FT_OUTFIT=47 identifizieren aus der vom OpenSim-Login gelieferten inventory-skeleton die Outfit-Ordner. Die ersten zehn realen Ordnernamen werden aus maximal 256 akzeptierten UUID-validierten Einträgen angezeigt; ungültige und doppelte IDs werden ignoriert, Steuercodes in Titeln bereinigt. F8- und F9-Ansichten schließen einander aus, ESC/F9 schließt die Outfit-Ansicht, während offener Ansicht werden WASD-/Movement-Tasten nicht an den Agenten übergeben. Keine erfundene Vorschau oder Behauptung eines funktionierenden Wear/Rez. **Grenze:** Die Anzeige stammt derzeit aus dem Login-Skeleton; der aktive FetchInventory2/FetchInventoryDescendents2-Readback, Asset-Mesh-/JPEG2000-Vorschauen, COF-Verknüpfung, Outfits anziehen und Rechteprüfung bleiben offen.

**Block D – überprüfbarer Firestorm-/OpenGenesisLINK-Wire-Vertrag:** Der vollständige serverseitige Snapshot vom 11.10.2026 nutzt OpenGenesisLINK main fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b (neuer als alpha.81 6dc73f2). Offizielle Firestorm-Release-Referenz Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d. Implementierungsquellen: Firestorm indra/llmessage/llexperiencecache.cpp und .h (public_id, experience_keys, /id/), indra/llinventory/llfoldertype.h (46/47/48); OGL-Server NexVerse/RegionModules/Experiences/NexExperienceModule.cs mit GetExperienceInfo via variable path und getrennten Permission-/Manage-Keys. Der OGL-Viewer besitzt und benötigt keine zentralen World-API-Keys. Neuer nativer HTTP-Fixture-Test simuliert Seed->Listen->Profilaufrufe und verspäteten Region-Seed nach dem Wechsel, neuer Outfit-Gallery-Test validiert Firestorm-Ordner-Typen, Duplikate, Sanitizing und echte Login-Ordnerdaten. Die Tests beweisen kontrollierten OGL-Vertrag, KEINE authentifizierte Live-Regionsabnahme oder vollständigen Firestorm-Quellfork.

**Abnahmegrenzen:** Die neue F8-/F9-Bedienung ist nur während WorldModel.ready sichtbar; GUI-Welcome-Smoketest ohne echte NexVerse-Simulation belegt daher keinen Live-UI-Nachweis. Noch keine echte InWorldAssetSnapshot-Serie im bisherigen Log. EventQueue, Avatar, tatsächliche WASD-Bewegung, Upload, Wearables/COF, Marketplace/Groups/Voice und sonstige Firestorm-Funktionen bleiben gemäß verbindlicher Paritätsmatrix als noch nicht nachgewiesen/implementiert gekennzeichnet. Nach Versions-Neubau volle Tests und NVIDIA-OpenGL-Smoke durchführen.

**Nächste vier geplante Blöcke Alpha.83:** echten FetchInventory2-/FetchInventoryDescendents2-Readback der Outfitfolder mit serveradvertisierter CAPS-POST-Struktur, Thumbnail.asset_id und diskretem Cache-Vertrag implementieren; Outfit-Vorschauen aus Assetpipeline als Bilder anzeigen, keine leeren Platzhalter; Experience-Auswahl mit Profildetails (Name, Beschreibung, Berechtigungen) vervollständigen; EventQueue/Teleport-Region-CAPS- und Firestorm-Features mit echter autorisierter Simulator-Sitzung regressionsprüfen. Funktionen nur bei tatsächlichen Serverrechten aktivieren.
