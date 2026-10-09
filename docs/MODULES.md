# OpenGenesisLINK – Module Runtime (ABI v1)

Stand: 09.10.2026, `0.2.0-alpha.38`

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

Nächste Module: OGL Developer Tools, OGL Performance Monitor. Vulkan/RTX erst
nach abgesicherter Host-/Renderer-API und Core-Fortschritt.
