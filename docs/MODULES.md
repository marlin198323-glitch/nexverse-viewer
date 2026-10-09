# OpenGenesisLINK – Module Runtime (ABI v1)

Stand: 09.10.2026, `0.2.0-alpha.39`

Die lokale Modulbasis ist in `NexVerse::Modules` implementiert. Sie gehört
zum Viewer-Core und lädt standardmäßig **keinen ausführbaren Fremdcode**.

## ABI-Vertrag

Öffentliche C-Grenze: `include/nexverse/modules/abi.h`.

- `OGL_MODULE_ABI_VERSION = 1`
- Export: `ogl_module_get_api(uint32_t host_abi_version)`
- `ogl_module_api`: `init`, `start`, `stop`, `shutdown`
- `ogl_host_api`: ABI-Version, Strukturgröße, Kontext und Log-Callback
- Funktionszeiger und Structs sind versioniert; keine STL-Typen oder C++-Exceptions
  über die ABI-Grenze
- Alle Strings sind nullterminiertes UTF-8, Lebensdauer nach ABI-Vertrag
- Callback-Werte: `0` Erfolg, sonst Fehler

Die ABI v1 ist eine **minimale Lifecycle-ABI**, noch keine Renderer-, UI-,
WorldModel-, Inventar- oder Netzwerk-API. Diese folgen als separat versionierte
Host-Function-Tables.

## Lokale Struktur

```text
<config-root>/
  enabled-modules.txt
  modules/
    org.opengenesislink.example/
      manifest.json
      ogl_example_module.so    # plattformspezifisch
```

Linux: `${XDG_CONFIG_HOME:-~/.config}/opengenesislink`.
macOS: `~/Library/Application Support/OpenGenesisLINK`.
Windows: `%APPDATA%/OpenGenesisLINK`.

Das Beispielmodul wird für die Tests nach
`build/debug/modules/org.opengenesislink.example/` gebaut. Es wird **nicht**
automatisch installiert oder aktiviert.

## Erweiterung ABI v2 (seit 0.2.0-alpha.39)

`include/nexverse/modules/abi_v2.h` führt separate und versionierte Strukturen
ein, ohne das ABI-v1-Layout zu verändern:

- `ogl_host_api_v2`: Basis-Host-API plus `read_diagnostics`.
- `ogl_diagnostics_snapshot_v1`: Frameindex, FPS, geglättete Framezeit,
  Draw Calls, Triangles, sichtbare Objekte, verfügbarer/gesamter RAM,
  VRAM und WorldModel-Ready.
- `ogl_module_api_v2`: Basis-Modul-Lifecycle plus Main-Thread-`on_frame`.
- Manifest `api_version: 1` oder `2`; bestehende v1-Module bleiben lauffähig.
- Diagnosen liefern eine Kopie des zuletzt veröffentlichten Snapshots; die
  Modul-API darf kein privates WorldModel-/Renderer-Objekt verändern.

`OGL Developer Tools` protokolliert Frame-/World-Status; `OGL Performance
Monitor` protokolliert alle 300 gültigen Samples durchschnittliche,
minimale und maximale **geglättete** Framezeit sowie RAM-/VRAM-Werte. Der
Monitor setzt Developer Tools als aktivierte Abhängigkeit voraus.

Beide Module sind zunächst **Entwicklertests mit Logging** und noch keine
grafischen Tools oder vollständige GPU-Profilinglösung.

## Entwicklerstart (ausschließlich für vertrauenswürdigen Eigenbau)

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
cmake --build --preset debug -j2
base="${XDG_CONFIG_HOME:-$HOME/.config}/opengenesislink"
mkdir -p "$base/modules"
cp -a build/debug/modules/org.opengenesislink.developer-tools "$base/modules/"
cp -a build/debug/modules/org.opengenesislink.performance-monitor "$base/modules/"
printf '%s\n' 'org.opengenesislink.developer-tools' \
  'org.opengenesislink.performance-monitor' > "$base/enabled-modules.txt"
./build/debug/nexverse-viewer --dev-modules
```

Das Startargument `--dev-modules` ist eine **ausdrückliche
Sicherheitsentscheidung**, keine Standardeinstellung. Wer diesem Modus
zustimmt, erlaubt zuvor aktivierten lokalen Modulen die Ausführung von
nativem Code mit den Rechten des Viewers. Nie fremde/unbekannte Module
in diesen Ordner kopieren.

## Manifest

Pflichtfelder: `id`, `name`, `version`, `vendor`, `api_version`,
`minimum_viewer_version`, `type`, `platforms`, `architectures`,
`capabilities`, `dependencies`, `entrypoint`.

`package_hash` und `signature` sind vorbereitete optionale Metadatenfelder.
Der Manifestparser akzeptiert maximal 64 KiB und begrenzte Arrays, prüft IDs,
Versionsformate und verhindert absolute/übergeordnete Entrypoint-Pfade.
Plattform und Architektur werden vor dem Laden überprüft.

## Sicherheit und Einschränkungen

- **Standardbetrieb:** Nicht signierte Module werden nicht geladen.
- Signatur-/Hashprüfung ist noch nicht implementiert. Pakete mit solchen
  Metadaten werden vorsorglich blockiert; sie gelten nicht als verifiziert.
- **Entwicklerbetrieb:** Eine Hostintegration muss `developer_mode=true`
  und jede benötigte Capability explizit in `allowed_capabilities` setzen.
  Der normale Viewer aktiviert diesen Modus nicht.
- Die Capability-Liste ist bislang eine Zulassungsprüfung, **keine Sandbox**.
  Native Module laufen im Viewer-Prozess und können außerhalb der deklarierten
  Host-Callbacks auf Betriebssystemressourcen zugreifen. Daher nur
  vertrauenswürdige Testmodule in isolierter Entwicklung laden.
- Symlinks bei Modulordner, Manifest und Binary werden verworfen.
- Abhängigkeiten werden vor Initialisierung geprüft (inkl. Mindestversion und
  Zyklen). Fehler beim Batch-Start lösen ein Rollback der gestarteten Module aus.
- Beim Entladen werden Abhängigkeiten in umgekehrter Startreihenfolge gestoppt.
- Aktivierungszustände werden in einer einfachen Textdatei persistiert, über
  temporäre Datei und atomaren Austausch aktualisiert.

## Tests

```bash
cmake --preset debug
cmake --build --preset debug -j2
ctest --preset debug --output-on-failure
```

`nexverse-module-runtime-tests` prüft Parsen/Fehlerfälle, ABI-Lifecycle,
Aktivieren/Deaktivieren, sichere Default-Einstellung, Persistenz und fehlende
Dependencies. Vor größeren Erweiterungen sind Windows/macOS-CI,
Signaturprüfung, Sandbox-Konzept, sichere Moduleinstellungen und UI-Freigabe
gesondert zu bearbeiten.

OGL Developer Tools und OGL Performance Monitor sind als reine Diagnose-Testmodule
implementiert. Nächster Featureblock ist Phase 8 (Prims/Materialien/Scene).
Vulkan/RTX erst nach abgesicherter Host-/Renderer-API und Core-Fortschritt.
