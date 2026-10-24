# OGL Viewer – Interaktive Projektabnahme

## Direkt auf Linux Mint starten

```bash
cd /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer
python3 scripts/ogl-acceptance.py --build
xdg-open build/acceptance/ABNAHME_DASHBOARD.html
```

Die generierte HTML-Datei arbeitet offline, ohne Webserver, CDN,
Login oder Datenbank. Sie enthält **12 Versionsstufen, 36 Aufgaben,
34 Abnahmekriterien und 7 echte Live-/Plattform-Freigabegates**.

## Ablauf

1. Version auswählen, Suchfeld nutzen, nur offene Punkte filtern.
2. Prüfer hinterlegen und jedes Kriterium als OFFEN, IN_ARBEIT,
   BLOCKIERT oder BESTANDEN manuell dokumentieren.
3. Zu BESTANDEN konkreten Testbeleg und Testzeitpunkt angeben.
4. Abweichungen und reproduzierbare Schritte erfassen.
5. JSON-Protokoll exportieren oder zur gleichen Git-SHA importieren.
   Druckansicht und lokales Browser-Zwischenspeichern sind vorhanden.
6. Exportiertes Protokoll so validieren:

```bash
python3 scripts/ogl-acceptance.py --validate /pfad/zur/ogl-abnahme-v0.4.0.0-xxxx.json
python3 scripts/ogl-version.py --release-check v1.0.0.0
```

**Wichtig:** Browserbewertungen sind manuelle Entwürfe.
Sie verändern weder Projektstatus noch GitHub-Branch,
Release-Freigabe oder echte E2E-Evidence-IDs.
Die reale Endabnahme bleibt bei fehlenden authentifizierten
Grid-/Avatar-/Outfit-/WASD-/UI-/Plattformbeweisen gesperrt.
Der Validator weist unbekannte IDs, vorgetäuschte
Releaseberechtigungen und veraltete Revisionen zurück.
Keine Passwörter, Tokens oder Geheimnisse eintragen.

HTML wird unter build/acceptance erzeugt und nicht mit einer
veralteten SHA ins Git eingecheckt. Nach jedem neuen Commit
neu generieren, alte Reports archivieren.

Quelle: docs/RELEASE_ROADMAP.json und docs/PROJECT_PROGRESS.json.
Verbindlich: docs/GOAL_100_PERCENT_RELEASE_ACCEPTANCE.md
und docs/RELEASE_VERSION_POLICY.md.
