# OGL Viewer v0.4.0.0 – NexVerse-Loginhost aus ungesicherter GridInfo-Datenquelle fest binden

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Ausgangs-Commit `f164a9c70cc451732270f7e23baa2d16459272d6`.**

## Reproduzierter Sicherheitsfehler

Der OpenGenesisLINK Viewer ist ausdrücklich ausschließlich für `hg.stadt-nexverse.de` bestimmt. Seine GridInfo-Discovery fragt derzeit `http://hg.stadt-nexverse.de/get_grid_info` über HTTP ab. Auch die lokale Cachedatei ist eine nicht vertrauenswürdige Eingabe. Bisher wurde jede allgemeingültige HTTP(S)-Adresse aus dem XML-Feld `<login>` als aktiver XML-RPC-Endpunkt übernommen. Ein manipulierter HTTP-Response oder bearbeiteter Cache konnte daher ein externes `https://evil.invalid/login` als **password-bearing** Login-Ziel speichern. Der vorhandene `is_valid_http_uri`-Check verhindert zwar URI-Userinfo, verbietet aber keinen fremden Host. Die vorhandene libcurl-Redirect-Sperre schützt nicht, wenn bereits die ursprüngliche Ziel-URI vom untrusted GridInfo stammt.

## Zehn bearbeitete Entwicklungs-/Testblöcke

1. Per **Linux-Mint-Commander** `CHATUEBERGABE.md` als Übergabequelle gelesen, aktuellen `develop-current`-HEAD und `origin/develop-current` überprüft: `f164a9c`, lokaler Arbeitsbaum sauber.
2. GitHub-CI des Ausgangscommits geprüft: Source Check bereits **PASS**; vollständiger GitHub-Linux-x64-Build anfangs noch **in progress**.
3. Neueste lokalen 30.10.-Viewer-Logs geprüft: kein authentifizierter Login und keine `AgentMovementComplete`-Abnahme. Öffentlicher HTTPS-Aufruf `hg.stadt-nexverse.de/get_grid_info` vom Mint-Rechner derzeit **nicht erreichbar**; daraus keine Live-Grid-Behauptung.
4. `src/protocol/grid_info.cpp`, `grid_discovery.cpp`, `GridDiscoveryService::refresh`, `load_cache`, aktive Loginzielübernahme in `application.cpp` und `CurlHttpTransport` vollständig überprüft. Hostwechsel durch syntaktisch gültiges XML als konkreten lokalen Sicherheitsfehler reproduzierbar gefunden.
5. Eingeführt: `trusted_nexverse_login_uri` prüft eine bereits allgemeinvalidierte HTTP(S)-URI gegen den **exakten NexVerse-Hostname**, ASCII-case-insensitiv. Fremde Domains, Suffix-Täuschungen, IP-Literale und unzulässige Ports werden abgelehnt. Erlaubt ist der dokumentierte OpenSim-XML-RPC-HTTP-Port **8002**; für HTTPS zusätzlich der Standardport 443 oder 8002.
6. Die Zulässigkeitsprüfung wird **vor jeder Übernahme einer live gelieferten Login-URI und vor jedem Laden einer gecachten URI** ausgeführt. Eine manipulierte Live-GridInfo darf die letzte valide Konfiguration nicht ersetzen. Ein bearbeiteter Cache bleibt auch bei Netzausfall unwirksam.
7. Keine untrusted URL, Query-Daten oder Hostdetails im neuen Fehlerlog. Das bisherige vollständige `Active OpenSim login URI: ...`-Logging entfernt und durch anonymisierte Zulassungsmeldung ersetzt. `CURLOPT_FOLLOWLOCATION=0` bleibt aktiv.
8. `grid_discovery_tests.cpp` mit **neun fremden oder gespooften Hosts/Ports**, gültiger Host-Großschreibung, HTTPS-Varianten, Offline-Cache-Poisoning und Wiederherstellung eines gültigen Caches erweitert. Native GridInfo-, Discovery- und Login-Tests bestanden.
9. Zehn Python-Struktur-/Integrationsverträge für Live- und Cache-Gate, Redirect-Sperre, Portgrenzen, XML-RPC-Credential-URL, anonymisierte Meldungen und unveränderten generischen CAPS-URI-Validator ergänzt.
10. Gesamtes Linux-Mint-C++-/CTest-/Python-/Firestorm-Xvfb-OpenGL-/GUI-/X11-Systemicon-Gate ausgeführt, danach `ROADMAP.md`, `projectplan.md`, `CHATUEBERGABE.md` und Fortschritts-JSON aktualisiert, GitHub-Commit gepusht und CI separat geprüft.

## Sicherheitsgrenzen

**Host-Pinning macht HTTP nicht zu TLS.** Der aktuelle dokumentierte XML-RPC-Login-Port ist HTTP/8002. Ein Angreifer auf dem Transportweg kann unverschlüsselten HTTP-Verkehr weiter beobachten oder manipulieren. Der Fix verhindert insbesondere die ungewollte Übernahme einer *anderen Domain* und fremder Ports, nicht sämtliche Netzwerkangriffe. Für vertrauliche Zugangsdaten ist ein serverseitig unterstützter HTTPS-XML-RPC-Endpunkt mit verifiziertem Zertifikat die längerfristig notwendige Lösung; eine automatische HTTP-zu-HTTPS-Umschaltung ohne Servernachweis würde die bestehende Anmeldung möglicherweise unterbrechen.

Die generische `GridInfo`-XML-Funktion und `is_valid_http_uri` bleiben für LLUDP/CAPS-URL-Validierungen unverändert; die Domänenbindung gilt bewusst nur im **NexVerse-only GridDiscoveryService**. Kein weiteres Grid freigeschaltet.

## Noch offene v0.4.0.0-Abnahmen

Authentifizierter Login gegen den aktuellen Gridserver, Region/Teleport, WASD-/Flug-Readback, GetTexture/GetMesh, echte Head-/Upper-/Lower-Body-Bakes mit finalen RGB-Pixeln und COF-Outfit sowie Shutdown-Zeit nach Anmeldung. Die in diesem Block getestete Sicherheit des Zielhost-Imports ist ein lokaler nachgewiesener Schutz, kein bestandener Live-Grid-Login.

Upstream OpenGenesisLINK-Server am 30.10.2026: Commit `08bf15f306bb4f882f24f32b84eb8df3a3175cea` (Login-Zeitzone); kein für diese Client-URI-Validierung notwendiger Protokollumbau.

**Zielversion `v0.4.0.0` IN ARBEIT. Fortschritt unverändert 38,65 % gewichtet (39 % gerundet). Kein `main`-Merge und kein Release/Deployment.**
