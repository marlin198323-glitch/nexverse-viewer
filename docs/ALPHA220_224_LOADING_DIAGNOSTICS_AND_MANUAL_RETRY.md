# Alpha.220–224 – Loading/Avatar-Verbindung vor Weltfreigabe prüfen

Datum 16.10.2026; Version 0.2.0-alpha.224. Entwicklungsmaschine
Linux Mint jam-MS-7D28, jam, Remote Desktop Commander;
GitHub marlin198323-glitch/nexverse-viewer develop-current,
Basis d5a7bc4be238166ac9f82a40e900656e013c261d.
Raspberry Pi 5 bleibt ausschließlich Self-hosted ARM64-CI.

## Block 220 – Region/Agent-Status unabhängig von WorldReady
Vorher war das im F3-Report exportierte Feld region_connected nur
dann yes, wenn die gesamte Welt bereit war. Region verbunden,
aber fehlende AgentPosition wurde fälschlich als disconnected
exportiert. Jetzt werden Region/Handle und AgentId/SessionId-Paar
direkt unter WorldModel-Mutex als separate Bool-Fakten gelesen.
Hinzu kommen vier echte LLUDP-Paketzähler und beworbene
CAPS EventQueue-Verfügbarkeit. Der Bericht bleibt
streng datensparsam: keine Passwort-, User-, Session-,
Agent-, URL-, Chat- oder Serveridentifikationswerte.
Eine literale NUL-Byte-Konstante im C++-Quelltext wurde
durch den druckbaren C++-Escape ersetzt.

## Block 221 – Reale F3-Oberfläche während des Ladevorgangs
Nach erfolgreichem Login funktioniert F3 jetzt schon,
bevor die Welt freigegeben wurde. Sichtbarer DIAGNOSE [F3]
Button öffnet über dem bestehenden Welcome-Hintergrund
ein separates modales Verbindungsfenster.
F3/ESC schließen, S/Mausbutton erzeugen bewusst einen
lokalen privaten Textreport im selben User-State-Verzeichnis.
Klicks hinter dem Overlay dürfen nicht in Login-/World-Aktionen
durchrutschen. Der Originalicon- und WebKit-Welcome-Pfad
wurde nicht ersetzt. Layouttests für 1100x700,
1280x720, 1920x1080, 3440x1440 sind hinzugefügt.
Nach WorldReady ist weiter die bestehende F3-
Avatarstatus-Oberfläche maßgeblich.

## Block 222 – Echte manuelle LLUDP-Reconnect-Aktion
Im Lade-Diagnosefenster ist SIMULATOR NEU VERBINDEN
nur bei final fehlgeschlagenem, vollständig
abgeschlossenem bisherigen LLUDP-Worker aktiv.
Voraussetzungen: vorhandener authentifizierter Login,
vorheriger Circuit gestartet, Worker-Future ready,
kein circuit_stop/Handoff, gültiges Simulatorziel,
Region-Endpunkt identisch, Abstand zur vorigen manuellen
Anforderung mindestens 15 Sekunden.
Erst nach einem ausdrücklichen Klick wird der fertige
Future get() aufgerufen, die outbound Queue und
lokale Movement-Feedback/Control-States zurückgesetzt,
dann exakt EIN neuer UseCircuitCode-Worker mit der
authentifizierten Sitzung und dem unveränderten
Regionziel gestartet. Keine erzwungenen Netzwerkwiederholungen
im Hintergrund, kein zweiter Worker, kein neuer
Loginversuch und keine Erfolgsmeldung vor echter Verbindung.
Ohne echte NexVerse-Sitzung nur Policy/Code/Build getestet.

## Block 223 – Ehrliche Welt-Progressanzeige
Vorher konnten optionale CAPS/Terrain/Objektpakete
den Fortschritt erhöhen, obwohl die AgentPosition fehlte.
Die Progressbar bezieht sich jetzt ausschließlich
auf authentifizierte Pflichtmilestones:
Login, aktiver LLUDP-Circuit, RegionHandle,
RegionConnected, AgentId/Session-Bindung und AgentPosition.
Während des Ladezustands nie 100 Prozent.
Ein Stufenzeitmesser wird bei jedem erreichten Schritt
zurückgesetzt; nach 15 s erscheint ein
stufenspezifischer, nicht irreführender Hilfetext.
Report verwendet nur WorldEntryStage Enum und
einen begrenzten Wait-Zähler. Optionale Empfangspakete
bleiben separat sichtbar.

## Block 224 – QA, Repository und Pi5
C++23 native Tests: Lade-Button-Hitboxen und Widescreen,
Circuit-Retry-Policy samt negativen Fällen,
WorldEntry-Timer-/Progress-Stufen,
Report-Enum-Whitelist/Private-File mode 0600.
Python Tests: Quellpfade für F3/ESC/S/Retry,
WorldModel unabhängigen Status und optionalitätsfreien
Progress sowie bestehende Viewer-Regressionen.
Vollständiger Linux-x64 Mint-Build, NVIDIA OpenGL-CTests,
Pi5-Headless-Tests, isolierter Xvfb Viewer-Start und
Cinnamon WM_CLASS/Icon. Doku, Projektplan, Handoff,
GitHub develop-current Push und Runner-Verifikation.

OpenGenesisLINK Server Referenz: main
ec760cacfff2fe505cf51991985553ed434eb121,
OpenJPEG fallback nach CSJ2K-Fehler (16.10.2026).
23 neuere Server-Commits gegenüber reviewed Baseline
4e99976630f1008db9014ea2b86c3723b09e9fe6
sind weiterhin nicht patchweise abgenommen. Reviewed
Pin bleibt unverändert; keine Serverdatei wurde geändert.

Nicht abgenommen: echte Welt-/Avatar-Pixel nach autorisierter
Grid-Anmeldung, reale manuelle LLUDP-Neuverbindung
und Firestorm Mesh/Bake/Animation/COF-End-to-End.
Daher kein behaupteter Live-Erfolg.
