# OGL v0.4.0.0 – Live-NexVerse-GridInfo: HTTP-Port-80-Login wieder akzeptieren

**30.10.2026 | Linux Mint `jam-MS-7D28` | GitHub `develop-current` | Ausgangs-SHA `4d96238920c9c9a90f814bf36eb66bc7243ef780`**

## Reproduzierter produktionsrelevanter Fehler

Nach dem vorherigen Security-Fix zum NexVerse-only Loginhost verhinderte `trusted_nexverse_login_uri()` alle HTTP-Loginadressen außer `:8002`. Ein tatsächlicher **öffentlicher GET** vom Linux-Mint-Rechner am 30.10.2026 auf `http://hg.stadt-nexverse.de/get_grid_info` antwortete jedoch **HTTP 200**, Gridname **Stadt NexVerse**, Plattform **OpenSim**, Login-Adresse **`http://hg.stadt-nexverse.de/`** — also exakt derselbe vertrauenswürdige Host auf dem impliziten Standard-HTTP-Port **80**. Der HTTPS-Abruf auf 443 schlug mit **Connection refused** fehl. Es wurden keine Zugangsdaten gesendet.

Die alten Viewer-Logs `20:15:54` und `20:16:04` enthielten `GridInfo rejected: login authority is not the NexVerse grid` und `NexVerse GridInfo unavailable and no valid cache exists`. Der vorherige Host-Pinning-Patch wies damit **den richtigen, echten Gridserver selbst ab**. Dies war ein Client-Regressionseintrag, kein Nachweis eines unberechtigten oder fremden Servers.

## Zehn konkret erledigte Entwicklungs-/Prüfblöcke

1. Direkt auf **Linux Mint** `CHATUEBERGABE.md` gelesen; sauberen `develop-current`-Arbeitsbaum und identischen GitHub-HEAD bestätigt; keine Raspberry-Pi-Änderungen.
2. Beide GitHub-CI-Checks des Ausgangscommits `4d962389` als **erfolgreich** geprüft, offene v0.4.0.0-Login-/CAPS-/Avatar-Abnahmen aus `ROADMAP.md` erneut aufgenommen.
3. Die neuen echten Viewer-Logs vom 30.10. auf die GridInfo-Fehlerstellen und den fehlenden Live-Login geprüft, ohne Passwort-, Session- oder Tokenwerte zu übernehmen.
4. Ausschließlich die **öffentliche** GridInfo-URL über den Mint-Rechner abgerufen und daraus Schema, Host und Port des realen XML-RPC-Loginziels ausgelesen: `http://hg.stadt-nexverse.de/` (= Port 80). HTTPS-Port 443 ist derzeit von dort nicht erreichbar.
5. Produktionscode gegen die öffentliche Vorgabe abgeglichen: `trusted_nexverse_login_uri()` blockierte Port 80, obwohl der Host eindeutig die gepinnte NexVerse-Domain war.
6. Die HTTPS/Host-Pinning-Logik **nicht entfernt**, sondern für den gepinnten Host ausschließlich die Legacy-OpenSim-HTTP-Ports **80** (auch ohne explizite Portangabe) und **8002** erlaubt. HTTPS bleibt auf 443 oder 8002 zulässig. Fremde Domains, IP-Literale, Domain-Suffix-Täuschung, URI-Userinfo und andere Ports bleiben verboten.
7. Native `grid_discovery_tests.cpp` um den genauen **realen, öffentlichen** GridInfo-`<login>`-Wert, seine erfolgreiche Live-Zulassung und sichere lokale Cacheübernahme erweitert.
8. Native Regression für expliziten HTTP-Port 80, alte HTTP/8002-Konfiguration, HTTPS/443/8002, Host-Großschreibung und verbotene HTTP/8080/8003-/HTTPS/8443-Ziele ergänzt.
9. Den bisherigen Python-Security-Vertrag auf die jetzt tatsächlich geprüfte zulässige Portmenge aktualisiert und acht weitere Verträge zu Live-GridInfo, Cache-Rückfall, Host-Pinning, Redirect-Sperre und weiterhin fehlender HTTP-Transportvertraulichkeit ergänzt.
10. Vollständigen Linux-Mint-x64-C++-/CTest-/Python-/originalen Firestorm-Xvfb-OpenGL-/GUI-/X11-Systemicon-Gate ausgeführt; `ROADMAP.md`, `projectplan.md`, `CHATUEBERGABE.md`, Fortschritts-JSON und die ursprüngliche Host-Pinning-Dokumentation berichtigt; `develop-current` committen/pushen und GitHub-CI separat beobachten.

## Sicherheit und realer Abnahmeumfang

Der Host bleibt **exakt `hg.stadt-nexverse.de`**, es werden keine anderen Grids zugelassen. Das Testfixture benutzt **öffentliche** Discovery-Daten. Die Remote-Tests haben **keine Passwörter, Cookies oder Login-Requests mit persönlichen Daten** übertragen.

**HTTP/80 ist nicht verschlüsselt.** Host-Pinning verhindert fremde *Login-Domains* in GridInfo, nicht das Mitlesen oder Verändern einer unverschlüsselten HTTP-Verbindung durch einen Netzwerkangreifer. Für einen tatsächlich vertraulichen Login braucht der Gridserver weiterhin ein funktionierendes **HTTPS-XML-RPC-Login** mit gültigem Zertifikat. Der Viewer schaltet nicht eigenmächtig auf einen derzeit nicht erreichbaren HTTPS-Port um.

**Nicht bestätigt:** tatsächlicher Benutzer-Login auf dem Grid, 3/3 sichtbare Avatar-Head-/Upper-/Lower-RGB-Bakes, COF-Outfit, Teleport/Regionswechsel, WASD-/Flug-Readback und reale Shutdown-Dauer nach Login. Die v0.4.0.0-Reifeangabe bleibt daher **38,65 % (39 % gerundet)**, kein `main`-Merge oder Produktiv-Release.
