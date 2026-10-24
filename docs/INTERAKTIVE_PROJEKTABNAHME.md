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

## Geführte Abnahme für Nutzer ohne technische Kenntnisse

Ganz oben im Dashboard: **Geführte Abnahme – wir machen das gemeinsam**.
Du musst keine Logs verstehen. Acht geführte Prüfschritte:

| Schritt | Deine Aufgabe | Meine technische Prüfung |
| --- | --- | --- |
| 1. Start | Viewer starten, Loginfenster suchen | Programmstart und UI |
| 2. Login | Nur im Viewer anmelden | Authentifizierung und CAPS |
| 3. Welt | Boden und Objekte sehen, F3 öffnen | Region und EventQueue |
| 4. WASD | W/S/A/D je kurz drücken | Serverposition |
| 5. Drehung | Pfeil links/rechts | Server-Yaw |
| 6. Sprung/Flug | Leertaste und Flugmodus (F) | Höhen-Readback |
| 7. Teleport | Bekannten Teleport wählen | Region-/CAPS-Wechsel |
| 8. Re-Login | Viewer schließen und erneut anmelden | Session-Cleanup |

Pro Schritt nur **Funktioniert**, **Fehler / geht nicht** oder
**Noch nicht möglich** anklicken, bei Fehlern kurz beschreiben,
was auf dem Bildschirm sichtbar war, dann **Nächster Schritt**.
Ein Screenshot ohne Passwörter oder Tokens ist hilfreich.
Ich prüfe die echten Logs, Simulatorantworten und Quelltests.
Du musst NICHT selbst entscheiden, ob ein offizieller
End-to-End-Nachweis ausreichend ist.

Die acht Schritte liegen in `docs/ACCEPTANCE_GUIDED_V0400.json`.
Die Antworten werden lokal und JSON-exportierbar gespeichert.
Das Dashboard führt keine Releases aus und kein manueller Klick
schließt die sieben offiziellen Live-Release-Kriterien ab.

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

## Geführte Abnahme (v0.4.0.0) – QA nachgewiesen

Die echte Chromium-UI wurde im isolierten Headless-Modus bedient:
Geführter Test #1 mit dem Button Funktioniert, Speicherung, Export
der Antwort und unveränderter Releasefreigabe: PASS.
Die Import-/Export-Validierung lehnt unbekannte Schritte,
unzulässige Statuswerte, falsche Versions-/Git-SHA und
erfundene Releaseberechtigungen ab. Der geführte Testplan
besteht aus 8 eindeutigen, auf die Roadmap referenzierten Schritten.

Lokale Linux-Mint-Abnahme: 185/185 native CTests, 647/647
Python-Tests und original Firestorm-Xvfb-GPU-/GUI-/128px-X11-
Taskleistenicon PASS, vollständiger CMake/vcpkg-x64-Debug-Build.
Das Dashboard kann per scripts/ogl-acceptance.py --build
zur aktuell geprüften Git-SHA neu erstellt werden.

**Nicht abgenommen:** Aktuelle authentifizierte Live-Grid-Anmeldung,
CAPS-/Teleport-Region-Endtest, serverbestätigte WASD-/Yaw-/Höhen-
bewegung; weiterhin v0.4.0.0 IN_ARBEIT und v1 RELEASE BLOCKED.
Der Nutzer soll nur die acht visuellen Testschritte und Beobachtungen
liefern; die technischen Beweise bleiben Aufgabe der Entwicklung.
