# OGL v0.4.0.0 – Login-CAPS/Seed-Wiederentdeckung und EventQueue nach HTTP 404

**30.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current` | Ausgangs-SHA `970110168eb02dd004957647dd0b1fa1117d814d`**

## Fehlerursache

Nach dem vorherigen Fix für die **Region**-EventQueue stellte eine Codeprüfung fest, dass der separat laufende **Login**-EventQueue-Worker denselben Fehler noch enthielt: War die EventQueue nach HTTP 404 abgelaufen und die anschließende Seed-Neuentdeckung lieferte HTTP 503 oder kein `EventQueueGet`, fragte er weiterhin den bereits bestätigten abgelaufenen Endpoint ab. Anders als bei einer späteren Region springt die RegionRestartGate bei der **ursprünglichen Login-Epoche** absichtlich nicht an. Der Zustand konnte daher bis zum nächsten Login festhängen.

Ein zweiter funktionaler Ausfall lag beim ersten Login vor: Die Login-Seed-CAPS wurde einmalig abgefragt, und eine vorübergehende HTTP-Fehlantwort beendete alle CAPS für die gesamte Anmeldung. Selbst eine erfolgreiche Seed-Discovery ohne `EventQueueGet` beendete den Worker dauerhaft, obwohl der Simulator die Long-Poll-CAP erst nach der Circuit-Initialisierung bereitstellen kann.

## Zehn bearbeitete Entwicklungs-/Verifikationsblöcke

1. Aktiven **Linux-Mint-Commander**, sauberen `develop-current`-Checkout, `CHATUEBERGABE.md`, `ROADMAP.md`, `projectplan.md` und den letzten GitHub-Stand gelesen.
2. Beide GitHub-Checks des vorherigen Commits `9701101` verifiziert: Source Check und kompletter Linux-x64-Viewer-Build erfolgreich. Kein `main`-Merge.
3. Historische Viewer-Logs geprüft: Aktuelle Läufe enthalten keine neue authentifizierte CAPS-/Avatar-Sitzung, deshalb keine erfundenen Live-Grid-Aussagen.
4. Den Login-/Region-CAPS-Lebenszyklus, `EventQueueRecovery`, initiale Seed-Discovery, Cancel-Signal und RegionLease-Grenzen analysiert. Den Rückfall auf eine abgelaufene Login-Queue bestätigt.
5. Bei HTTP 404 und gescheiterter Seed-Neuentdeckung die tote Login-Queue-URI sofort verwerfen, ACK und 64er Replay-Historie leeren, EventQueue-Status auf `CAPS RETRY` setzen. Es werden keine weiteren POSTs an die ungültige URI geschickt.
6. Den Login-Worker bei fehlender URI nicht beenden: Er fragt stattdessen ausschließlich die aktuelle authentifizierte Seed-CAPS wiederholt ab, mit bestehendem 1/2/4/8/12-Sekunden-Backoff, maximal 100-ms-Cancel-Schritten und RegionLease-Abbruch.
7. Wenn der Simulator die Queue erneut bereitstellt, werden ACK/Replay-Cursor und initialer Live-Status neu aufgebaut. Die alte URI wird auch bei gleicher URI erst wieder verwendet, wenn sie tatsächlich aus dem gültigen Seed zurückgegeben wurde.
8. Auch die erste Seed-CAPS-Discovery nach erfolgreichem Login darf vorübergehend fehlschlagen. Sie wird mit dem vorhandenen begrenzten Recovery-Timer wiederholt, ohne Secret-POSTs umzuleiten oder einen alten Regionstatus wiederzubeleben. Initial noch fehlendes `EventQueueGet` benutzt denselben Wiederanlaufpfad.
9. Native HTTP-Mock-Regressionen in `caps_tests.cpp`: HTTP 503 → noch fehlendes `EventQueueGet` → später gültige Seed-CAPS → HTTP 404 der alten Queue → neuer Cursor und Events. Neun zusätzliche Python-Quellverträge kontrollieren die tatsächliche Worker-Verkabelung, Quell-Lebensdauer, ACK-Reset, Datenschutz und 502-Long-Poll. Anonyme Logwarnungen enthalten keine Seed-URL oder Antwort-Body.
10. Vollständiger Linux-Mint-Build, native CTests, Python-Testreihe und isolierte Firestorm-Xvfb-/OpenGL-/GUI-/X11-Icon-Abnahme, anschließend Projektplan/Status/Chatübergabe synchronisieren, Commit nach `develop-current` und GitHub-CI separat prüfen.

## Abnahmegrenzen

Die HTTP-/ACK-/Seed-Fallbacks sind automatisiert geprüft. Eine konkrete authentifizierte NexVerse-Sitzung mit echtem Seed-CAPS-Ausfall und anschließendem Simulator-Queue-Reconnect wurde **nicht** durchgeführt. Vollständig offen bleiben: echtes Login/Relog/Teleport-/Region-Crossing-E2E, serverseitiges WASD/Flug-/Z-Readback, COF-/Wearables, drei aktuelle Head-/Upper-/Lower-Texturen plus sichtbarer Skin-RGB-Endbeweis und Shutdown-Zeit nach Login.

**v0.4.0.0 IN ARBEIT, Fortschritt 38,65 % (39 % gerundet). Kein `main`-Merge, kein Release und keine Produktivinstallation.**
