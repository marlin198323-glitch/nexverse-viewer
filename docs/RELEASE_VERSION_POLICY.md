# OGL Viewer – verbindliche Versions- und Release-Regeln

## Format und Quelle

Die einzige aktuelle öffentliche Entwicklungsversionsnummer steht in `VERSION`:
**v0.3.0.0** (noch nicht released). Format: `vMAJOR.MINOR.PATCH.REVISION`.

- MAJOR: neue stabile Produktgeneration; v1 = erste vollständig abgenommene Firestorm-kompatible Version.
- MINOR: fachlich großer Versionsmeilenstein (v0.4 Verbindung, v0.5 Avatar usw.).
- PATCH: größere Wartungs- oder Erweiterungsgruppe innerhalb einer MINOR-Version.
- REVISION: nachfolgende kleine Fehlerbehebung; nach v1.0.0.0 kommt explizit v1.0.0.1.
- Historische `alpha.NNNN`-IDs bleiben als Entwicklungsaufzeichnungen, nicht als Releaseversionen.
- CMake `project()`, `core::version()`, GUI, Logs und Smoke verwenden dieselbe Version, ohne doppeltes Präfix.
- Version = Entwicklungsziel, nicht automatisch 100 % oder bereits auf main released.

## /goal vX.Y.Z.W

Ein versionsgenauer ` /goal vX.Y.Z.W fertigstellen ` bezieht sich auf
`docs/RELEASE_ROADMAP.json` und alle darin definierten Aufgaben,
Vorgängerversionen und Abnahmebelege. Interne zehn Entwicklungsblöcke
können weiter benutzt werden, ohne jede Gruppe als neue Releaseversion auszugeben.

Für v1.0.0.0 und später gilt: erst dann auf main veröffentlichen, wenn
ALLE nachfolgenden Bedingungen erfüllt sind:

1. Alle Aufgaben und Abhängigkeiten der Zielversion FERTIG und Kriterien GETESTET.
2. Für v1.0.0.0 Fortschritt 100 %, sieben verifizierte echte `final_release_evidence`-IDs in `docs/PROJECT_PROGRESS.json` und `scripts/ogl-release-readiness.py` Exit 0.
3. Echte NexVerse/OpenGenesisLINK-Grid-E2E für 3/3 Head/Upper/Lower RGB-Bakes, tatsächlich sichtbares COF/Outfit, serverbestätigte Bewegung/Yaw/Jump/Fly, vollständige Firestorm-GUI und Rendering/Materialien.
4. Gleiche EXAKTE getestete Release-Code-SHA auf Linux x64, Windows und macOS mit GUI/Installern; CI, Security, Lizenzen, Update/Rollback bestanden.
5. Getesteten Release-Branch bzw. `develop-current` per PR nach `main`; Branchschutz/Review beachten; nach erfolgreichem Merge annotierten `vX.Y.Z.W`-Tag und GitHub-Release mit Artefakten publizieren.
6. Wenn ein Beweis oder benötigte Berechtigung fehlt: Status BLOCKIERT, kein main-Merge, kein Tag, kein falscher Release.

Der Befehl `/goal v1.0.0.0 ... und releasen` autorisiert einen
**konditionalen** Release nach vollständiger Abnahme, nicht das
Überspringen von GitHub-Rechten, Sicherheitsprüfungen oder Live-Grid-Beweisen.

## Prüfen

`python3 scripts/ogl-version.py --check`
`python3 scripts/ogl-version.py --target v1.0.0.0`
`python3 scripts/ogl-version.py --release-check v1.0.0.0` (derzeit Exit 2 BLOCKED)
`python3 scripts/project-progress.py`
`bash scripts/mint-viewer-validation.sh`

Meilensteine: `ROADMAP.md` und `docs/RELEASE_ROADMAP.json`.
Vorheriger Umfang: `docs/PROJEKT_AUDIT_2026-10-24.md`.
