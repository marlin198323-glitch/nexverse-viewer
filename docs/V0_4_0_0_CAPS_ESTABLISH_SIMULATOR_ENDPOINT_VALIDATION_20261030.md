# OGL Viewer v0.4.0.0 – CAPS EstablishAgentCommunication und Simulator-Socket-Eingaben absichern

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Basis `1fb3324d03850ff6bddc78b00d4195b8e26897e4`**

## Funktionaler Fehler und Geltungsbereich

Während `TeleportFinish` und `CrossedRegion` inzwischen bereits endliche Koordinaten, Simulatorports und CAPS-Seed-URIs prüfen, akzeptierte der benachbarte Parser `parse_establish_agent_communication_event()` jedes nichtleere `sim-ip-and-port` und `seed-capability` als erfolgreich. So wurden Einträge wie ungültige IPv4-Adressen, `host:0`, `host:65536`, ungültige URL-Schemes und URI-Userinfo im Viewer als `CAPS EstablishAgentCommunication ... validated` protokolliert. Derzeit aktiviert dieser Eventhandler **noch keinen zusätzlichen UDP-Circuit**; die alte Erfolgsmeldung suggerierte fälschlich mehr, als der Client tatsächlich durchgeführt hatte.

Der gemeinsam genutzte `read_ip()`-Pfad für textuelle Simulatoradressen in `EnableSimulator`, `TeleportFinish` und `CrossedRegion` nahm zuvor ebenfalls jede nichtleere Zeichenfolge an. Diese wird bei einem wirklichen Handoff später als UDP-Adresse verwendet. Binäre IPv4- und IPv6-LLSD-Adressen hatten bereits korrekte Konvertierung und bleiben unverändert.

## Zehn Entwicklungs-/Prüfblöcke

1. `CHATUEBERGABE.md`, `ROADMAP.md`, aktuellen Linux-Mint-Checkout `develop-current` und GitHub-Remote abgeglichen. Kein Raspberry-Pi-Checkout geändert.
2. GitHub-CI für Basiscommit `1fb3324` geprüft: Source Check **grün**; Full-x64-Build zunächst **in progress**. Späteren Status gesondert prüfen.
3. Neueste Logs überprüft: 30.10.-Läufe zeigen weiterhin isolierten Viewer-Start, Original-Firestorm-Meshes und erfolgreiche öffentliche GridInfo-Discovery, aber keinen authentifizierten Avatar-/Teleport-Test.
4. CAPS-Welt-Eventdecoder sowie `EstablishAgentCommunication`-, `EnableSimulator`-, `TeleportFinish`- und `CrossedRegion`-Weitergabe untersucht. Fehlende Validierungsgrenzen von Seed-URI und Simulator-Endpunkt identifiziert.
5. Gemeinsamen `valid_simulator_host`-Guard implementiert: keine URL-Pfade, Querys, Userinfo, Leerzeichen, Zeilenumbrüche oder ungültige IPv4-Oktette. Legitime DNS-Namen bleiben möglich und werden wie bisher im Transport aufgelöst.
6. `sim-ip-and-port`-Eingaben dürfen nur einen vollständigen `hostname:port`, `IPv4:port` oder geklammerten `[IPv6]:port`-Endpunkt mit Dezimalport **1–65535** enthalten. Strings mit fehlendem Port, Mehrfachport, Überlauf oder Zeichenmüll werden vor Übernahme verworfen.
7. Plattformsparsame IPv6-Literalprüfung eingeführt: genau acht Hexgruppen oder einmal komprimierte `::`-Gruppen; gültige IPv4-Mapped-IPv6-Endungen zählen als zwei Gruppen. Ungültige `:::`, mehrfache Kompression, zu viele Gruppen oder überlange Hexgruppen werden abgelehnt. Binäre 16-Byte-IPv6-Adressen bleiben kompatibel.
8. EAC-Seed muss den bestehenden gehärteten `is_valid_http_uri`-Validator bestehen. Die Anwendungsdiagnose unterscheidet jetzt ausdrücklich **„parsed; no new circuit activated“** von anonymisiertem **„rejected“**. Kein Log neuer Agent-IDs, Seed-CAPS oder Hostnamen.
9. Native `caps_world_events_tests.cpp` mit gültigen und beschädigten IPv4-/IPv6-/DNS-Endpunkten, Portgrenzen, Seed-Schemes, URI-Userinfo, IPv4-Mapped-IPv6, TeleportFinish-/EnableSimulator-/CrossedRegion-Strings und weiter gültigem Binär-IPv6 erweitert. Zwölf Python-Vertragsprüfungen verifizieren konkrete Parser-Grenzen, Quellverkabelung, datensparsame Logs und unveränderte WorldModel-/Region-Sicherungen.
10. Vollständiges Linux-Mint-x64-C++-/CTest-/Python-/Original-Firestorm-Xvfb-OpenGL-/GUI-/X11-Icon-Gate, JSON-/Git-Prüfung, Aktualisierung von `CHATUEBERGABE.md`, `ROADMAP.md`, `projectplan.md` und Fortschritts-JSON. Commit auf `develop-current` erst nach erfolgreicher Abnahme; GitHub-Checks separat kontrollieren.

## Grenzen und Folgearbeit

Das ist **Eingabevalidierung, keine neue Regionsverbindungsfunktion**: Ein erfolgreich geparstes `EstablishAgentCommunication`-Event baut allein noch keinen zweiten Circuit auf. DNS wird syntaktisch, nicht durch Namensauflösung geprüft. UDP-Circuit-Erfolg, tatsächlicher Teleport und Avatar-Bewegung benötigen weiterhin eine echte Anmeldung. Auch Head-/Upper-/Lower-Skin-Bakes 3/3, COF/Outfit und die reale Shutdown-Zeit nach Login sind noch offen.

OpenGenesisLINK-Server Upstream am 30.10.2026: `08bf15f306bb4f882f24f32b84eb8df3a3175cea` (Zeitzonenkonfiguration, kein relevantes CAPS-Protokollupdate). Welcome-UI, Login-Design, originale Firestorm-Funktionen sowie alleiniger Gridhost `hg.stadt-nexverse.de` wurden nicht verändert.

**Version `v0.4.0.0` IN ARBEIT, Projektfortschritt weiterhin 38,65 % gewichtet (39 % gerundet). Kein `main`-Merge, kein Release/Deployment.**
