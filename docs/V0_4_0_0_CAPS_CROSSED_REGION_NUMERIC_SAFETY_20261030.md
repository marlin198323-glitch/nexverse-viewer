# OGL Viewer v0.4.0.0 – Sichere CrossedRegion-/Teleport-Simulatorwerte vor Weltübergabe

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Ausgangscommit `83ecb5dd7a0163dffecdb65bda0705c9a64f14fd`.**

## Festgestellter Protokollfehler

`src/protocol/caps_world_events.cpp` akzeptierte aus LLSD-`CrossedRegion`-CAPS-Ereignissen beliebige reelle `Position`- und `LookAt`-Werte, inklusive `NaN`, positivem/negativem Unendlich und extremen `double`-Werten, die beim Cast zu `float` überlaufen. Im Viewer können diese als Agentposition und Blickvektor die neue Region/3D-Kamera ungültig machen.

Außerdem akzeptierte `read_u16()` den ungültigen Simulator-Port 0, während `read_u32()` einen positiven 64-bit-LLSD-Integer > `UINT32_MAX` vor dem Vergleich stillschweigend zu `uint32_t` konvertierte. Bei `read_region_handle()` wurde ein positiver 64-bit-Integer sogar stets auf 32 Bit gekürzt. RegionSizeX/Y konnten bis an die 32-Bit-Grenze durchgelassen werden. Seed-URIs wurden bei CAPS-Simulator-Events nur als nichtleere Strings geprüft.

## Zehn tatsächlich bearbeitete Entwicklungs- und Verifikationsblöcke

1. `CHATUEBERGABE.md` und den maßgebenden `ROADMAP.md` auf dem Linux-Mint-Commander gelesen; `develop-current` und `origin/develop-current` abgeglichen, Arbeitsbaum sauber.
2. Vorherige GitHub-Source- und Full-x64-Build-CI des Ausgangscommits als erfolgreich verifiziert. Aktuellen lokalen Viewer-Log auf Authentifizierung geprüft: kein neuer echter Login.
3. Firestorm-/CAPS-EventQueue-Dispatch, `TeleportFinish`, `CrossedRegion`, `region_handoff_action` und Weltmodell-Region-Transition untersucht; Eingangskriterien von Renderer-/Kamera-Pose und Socket-Zielen geprüft.
4. `read_number()` so korrigiert, dass nur endliche `double`-/Integerwerte innerhalb des `float`-Darstellungsbereichs angenommen werden. Auch nach dem Cast `std::isfinite` prüfen, bevor die neuen Koordinaten in `Vec3` gelangen. Echte negative Koordinaten bleiben erlaubt.
5. Positive 64-Bit-RegionHandles aus LLSD-`integer` vollständig auf `uint64_t` übernehmen statt auf 32 Bit zu kürzen. Acht-Byte-Binär-Handles weiterhin bytegenau verarbeiten.
6. Die gemeinsame `read_u32()`-Routine mit `UINT32_MAX`-Grenzprüfung gegen Integer-Überlauf gehärtet. Simulatorport 0 und Ports über 65535 bleiben ungültig. Beide Prüfungen erfolgen vor einer Socket-Verbindung.
7. Angegebene Regionsabmessungen gegen 0 und den konservativen Wert 16384 m je Achse prüfen, bevor Terrain-/Viewport-Budgets daraus abgeleitet werden. Normale und große OpenSim-Varregions (Test mit 8192 × 4096 m) bleiben unterstützt. Vom Simulator gelieferte Seed-CAPS-URLs zusätzlich mit dem bereits gehärteten HTTP(S)-URI-Validator prüfen.
8. Die native `caps_world_events_tests.cpp` um LLSD-`CrossedRegion`-Fixtures mit NaN, `±Inf`, Float-Überlauf, ungültigem `LookAt`, Ports 0/65536/Überlauf, großen/ungültigen Regionsmaßen, gültigem großem RegionHandle sowie fremden URI-Schemata und URI-Userinfo ergänzt. Echte gültige negative Positionen und großes Varregion-Format bleiben akzeptiert.
9. Zehn neue Python-Vertragsregressionen für Protokoll-Decoder, Endlichkeitsprüfung, Größen- und Portgrenzen, URI-Validator, ursprünglichen Teleport-/CrossedRegion-Event-Dispatch, bestehende ACK-/Replay- sowie Circuit-Handoff-Funktionen ergänzt.
10. Vollständigen Linux-Mint-x64-C++-/CTest-/Python-/originalen Firestorm-Xvfb-OpenGL-/GUI-/X11-Icon-Gate geprüft, Projektplan, Roadmap, Übergabe und Fortschritts-JSON aktualisiert; Commit und Push nach `develop-current`, anschließend GitHub-CI verifizieren.

## Grenzen und echte Live-Abnahme

Die Änderungen verhindern, dass **formal beschädigte oder überlaufende** Simulatorwerte ungeprüft in das Weltmodell gelangen. Sie erzeugen weder neue Server-Positionsdaten noch einen erfolgreichen Regionswechsel. Der feste Gridhost `hg.stadt-nexverse.de` bleibt unverändert. GetTexture-/GetMesh-Limits, Welcome-Seite, kompakte Login-UI und originale Firestorm-Rendererfunktionen werden nicht verändert.

Authentifizierte Abnahme weiterhin offen: echtes Grid-Login, CAPS-Regionswechsel/Teleport, tatsächliches WASD-/Flug-Z-/Yaw-Readback, Head-/Upper-/Lower-Bake-RGB 3/3, COF-Outfit und reale Shutdown-Latenz nach Login.

OpenGenesisLINK-Server upstream am 30.10.2026: SHA `08bf15f306bb4f882f24f32b84eb8df3a3175cea`; letzte Änderung zur Login-Zeitzone, nicht zum CAPS-`CrossedRegion`-Payload.

**Version `v0.4.0.0` IN ARBEIT. Projektfortschritt weiterhin 38,65 % gewichtet (39 % gerundet). Kein `main`-Release und kein Deployment.**
