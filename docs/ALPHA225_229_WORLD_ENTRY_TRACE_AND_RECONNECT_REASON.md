# Alpha.225–229 – Welt-Entry-Verlauf und konkrete Reconnect-Ursachen

Datum: 16.10.2026, Viewer v0.2.0-alpha.229, Linux Mint jam-MS-7D28,
Desktop Commander unter Benutzer jam.
Repository marlin198323-glitch/nexverse-viewer
Branch develop-current, Quellbasis
bd1e0401c443f9bf9eb2f6d4e5de38012146c034.
Raspberry Pi5 wird nur für Self-hosted ARM64-CI verwendet.

## 225 – Fünf echte Entwicklungsblöcke auch nach Chatwechsel
CHATUEBERGABE.md enthält JETZT als ALLERERSTES die verbindliche
Regel: Jedes einfache WEITER bedeutet fünf vollständige,
implementierte Entwicklungsblöcke mit sichtbarer GUI, echtem Code,
Mint Native/NVIDIA OpenGL Build, C++23/Python/Headless, Xvfb-
Vollviewer, GitHub Push und Pi5-CI, nicht bloß fünf Planungspunkte.
AGENTS.md, projectplan.md und info.md wiederholen die Regel vorn.
Gleichzeitig neuer WorldEntryTrace: fixed ring 5 Einträge,
jeder Eintrag hat ausschließlich aus einem Enum ausgewählte
Verbindungsstufe und begrenzte Dauer in Sekunden (max. 3600).
Keine User-/Agent-/Session-IDs oder Netzwerkinformationen in
dieser Tracestruktur; bei fehlender Authentifizierung reset.
Der neueste Eintrag wird bei jeder Live-Abfrage aktualisiert,
frühere Stufen bleiben zur Diagnose erhalten.
Native Test: Dauer, neue Stage, Überlauf, Reset, Zeitklammerung.

## 226 – Erklärung, WARUM Simulator-Reconnect gesperrt ist
Vorher meldete das F3-Fenster nur eine deaktivierte Schaltfläche.
Jetzt liefert loading_retry_reason einen getypten Status:
Login fehlt, LLUDP nicht begonnen, alter Worker läuft noch,
kein finaler Fehler, Stop/Regionsübergang, Simulatorziel
nicht verifiziert, Cooldown läuft oder Retry freigegeben.
Diese Zustände verwenden ausschließlich bereits lokal bekannte
Gates, nicht die eventuell schutzbedürftige Error-Message des
Simulator-Circuits oder dessen IP. Keine automatische Verbindung
durch die neue Information; Sicherheitsprüfungen aus 222 erhalten.
Native Negativfälle inklusive ausdrücklich WAIT statt False-Positive.

## 227 – Private lokale Diagnose mit nachvollziehbarem Verlauf
Das bisherige freiwillige F3-Diagnosereport (POSIX Mode 0600)
enthält jetzt zusätzlich fünf typisierte History-Items mit
Wartezeit pro Stufe, Anzahl manueller Circuit-Neustarts und
festen Retry-Statuscode. Daten werden aus der Live-Session
zum Anforderungszeitpunkt übernommen, NICHT aus Logdateien,
CAPS-URLs, Login-Formularen oder Serverantwortstrings kopiert.
Report-Test schreibt die echte Datei in einen temporären Pfad,
prüft vollständige Felder sowie Nichtvorhandensein von
Passwort, Agent-/SessionID, Chat und Simulator-IP.
Kein unaufgeforderter Hintergrundexport.

## 228 – Reale GUI-Integration + Regressionen
Das existierende Laden-F3 zeigt jetzt dauerhaft:
- letzte zwei Verbindungsstufen mit tatsächlich vergangener Zeit
- konkrete lokal geprüfte Retry-Freigabe-/Sperrursache
- Anzahl manuell ausgelöster LLUDP-Neustarts
Die echte Welt-ID/Region/Agent-Anwesenheit sowie die
bereits implementierten originalen LLUDP-Paketzähler und
GetTexture-/AvatarAppearance-Stufen bleiben erhalten.
Auf dem Ladebildschirm funktioniert das gleiche F3/ESC/S
wie im vorherigen Block; Original-Welcome-Seite und Systemicon
werden NICHT verändert. Python Contract prüft exakt
die reale Event-/GUI-/Bericht-Verdrahtung sowie die dauerhafte
Fünferregel als erste Nachricht der Chatübergabe.
Natives C++ prüft festen Ring, negative Retry-Kriterien
und datensparsamen 0600-Dateiexport.

## 229 – Integration, Tests, Dokumentation, GitHub
Pflicht-Releasegates:
Mint x64 CMake/Ninja Vollbuild und alle NVIDIA-OpenGL/Native
CTest, alle Python Tests, Pi5-kompatibles C++23 Headless,
echter isolierter Xvfb-Vollviewer-GUI-Start ohne DISPLAY=:0
sowie systemweites X11-Taskleisten-Branding (WM_CLASS/ICON).
Danach Commit/Push develop-current auf Mint, Build-SHA
über GitHub vergleichen und separaten Pi5-ARM64-Runnerjob
für GENAU DIESEN SHA erfolgreich abschließen.
Plan, Handoff, info, AGENTS und Kompatibilitätsmatrizen
werden vor und nach CI fortgeschrieben.

Referenz OpenGenesisLINK Server main am 16.10.2026
ec760cacfff2fe505cf51991985553ed434eb121
(fix j2k retry OpenJPEG on CSJ2K failure), vor Entwicklung
verifiziert. Das reviewed_server_sha der Kompatibilitätsmatrix
wird wegen weiter ausstehender Commit-/Patchreviews
nicht unqualifiziert nachgeführt; keine OpenSim-
Wire-Protocol-Neuerfindung durch diese Arbeiten.

Echte NexVerse-Welt und Avatar sowie reales
LLUDP-Wiederverbinden sind weiterhin NICHT als End-to-End
geprüft: Native/GPU-Fixtures und Login-freier Xvfb-Smoke
ersetzen keinen autorisierten Login. Das neue F3-Protokoll
hilft beim späteren Nachweis der blockierenden Pipeline-Stufe.
