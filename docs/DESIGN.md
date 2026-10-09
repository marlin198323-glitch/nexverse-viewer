# OpenGenesisLINK Viewer – Design Direction

## Identität

Eigenständige OpenGenesisLINK-/NexVerse-Oberfläche; keine pixelgenaue Firestorm-/Second-Life-Kopie.

Visuelle Grundsprache:

- dunkles Navy / nahezu Schwarz
- Silber/Weiß
- Cyan / elektrisches Blau
- Violett / Magenta
- reduzierte Glas-/Panel-Flächen
- dominante Weltansicht
- einklappbare Panels
- widescreen- und HiDPI-tauglich

## Start / Login

Der Viewer startet maximiert als normales dekoriertes Desktop-Fenster. Native Minimieren-/Maximieren-/Schließen-Schaltflächen bleiben erhalten. F11 toggelt exklusiven Fullscreen.

Feste NexVerse-Endpunkte:

- Discovery: `http://hg.stadt-nexverse.de/get_grid_info`
- Welcome: `https://stadt-nexverse.de/service/welcome`

Die Welcome-Seite liegt hinter Login/Ladezustand. In-World-Navigation erscheint erst bei tatsächlich bereiter Simulation.

## In-World Layout

```text
+--------------------------------------------------------------------+
| OpenGenesisLINK / Region / Status                     Avatar/Profil |
+--------------------------------------------------------------------+
|                                                                    |
|                            3D WORLD                                |
|                                                                    |
| [Navigation]                                             [Context] |
|                                                                    |
+--------------------------------------------------------------------+
| Chat | Voice | Movement | Teleport | Inventory | Build | Camera    |
+--------------------------------------------------------------------+
```

## Text-only

Als Core-Betriebsmodus geplant. Login, Chat, IM, Freunde, Gruppen, Inventar, Outfits, Teleport, Landmarks und Regionstatus sollen ohne aktive 3D-Weltdarstellung nutzbar sein.

## Outfit UX

Core: Wearables, Attachments, Outfit speichern/laden/wechseln, Appearance Sync.

Plugins:
- `OGL Outfit Manager`: Kategorien, Tags, Favoriten, Suche, Notizen.
- `OGL Outfit Preview`: Outfit-Screenshots, automatische Previews, Galerie, Kamera-Presets.

Previews werden standardmäßig lokal gespeichert. Cloud/Upload später nur opt-in.

## Environment / Weather

Environment-Modi:

- Region / OpenSim / EEP
- feste lokale Uhrzeit
- eigener Tageszyklus
- reale lokale Systemzeit
- später reale Sonnenposition über einen vom Nutzer gewählten Referenzort

`OGL Weather` soll hochwertigen Regen, Schnee, Nebel und Sturm ohne grobe Pixelpartikel-Optik ermöglichen. RTX erweitert nur optional.

## NextGen Grafik

Langfristig: Vulkan, PBR, HDR, moderne Beleuchtung, hochwertige Schatten/Reflexionen, modernes Terrain/Wasser, Atmosphäre, Post Processing, Streaming und optional Raytracing.

Grafikprofile müssen von sehr niedrig bis Ultra/Benutzerdefiniert skalieren.

## Accessibility

HiDPI, UI-Skalierung, Schriftgröße, Tastaturnavigation, Kontrast und plattformspezifische Eingabekonventionen bleiben verbindliche Ziele.

`projectplan.md` ist maßgeblich.
