# OpenGenesisLINK Viewer für NexVerse – Arbeitsstand

> `projectplan.md` ist für Architektur, Reihenfolge und Projektumfang immer maßgebend.
> Diese Datei dokumentiert den tatsächlichen Entwicklungsstand und den Wiedereinstiegspunkt.

Stand: 05.10.2026
Lokaler Arbeitsbranch: `develop`
Lokaler Pfad: `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`
GitHub-Repository: `marlin198323-glitch/nexverse-viewer`

## Wiedereinstieg

Wenn der Benutzer in einem neuen Chat `viewer` schreibt:

1. Zuerst lokal über Desktop Commander den Ordner prüfen.
2. `projectplan.md` lesen.
3. Danach `info.md` lesen.
4. `git status` und letzten lokalen Commit prüfen.
5. Lokal weiterentwickeln, kompilieren und testen.
6. GitHub erst für fertige, getestete Entwicklungsblöcke verwenden.
7. Nach jedem größeren Block diese Datei aktualisieren.

## Produkt

Produktname: `OpenGenesisLINK Viewer`
Zielwelt/Grid: NexVerse
Technik: C++23, CMake, vcpkg, GLFW, GLAD, OpenGL 3.3 Core, FreeType.
Lizenz: MPL 2.0.

## Phase 0

Die Desktop-/Renderer-Grundlage ist lauffähig.

Lokaler Grafiktest am 05.10.2026:
- OpenGL 3.3
- GPU: NVIDIA GeForce RTX 4070 SUPER
- GLFW-Fenster startet über `DISPLAY=:0`
- UI-Renderer initialisiert erfolgreich

## Phase 1 – Grid Discovery

Phase 1 ist lokal funktional implementiert.

Vorhanden:
- libcurl HTTP-Transport
- tinyxml2 Parser
- maximale GridInfo-Antwortgröße 256 KiB
- Connect-Timeout 5 Sekunden
- Gesamt-Timeout 10 Sekunden
- nur HTTP/HTTPS
- Redirect-Limit
- Validierung der Login-URI
- unbekannte/additive GridInfo-Felder werden erhalten
- `login` ist Pflichtfeld
- plattformspezifischer Cachepfad
- gültige Live-GridInfo wird gecacht
- ungültige Live-Daten ersetzen keinen gültigen Cache
- Cache-Ersatz mit Backup-/Restore-Logik
- Live/Cache/Fehler-Status
- asynchroner Discovery-Start im grafischen Viewer
- CLI `--grid-info`
- Unit-Tests für Parser und Discovery
- der Discovery-Endpunkt ist fest im Viewer verdrahtet: `http://hg.stadt-nexverse.de/get_grid_info`
- bei jedem Start wird ausschließlich dieser NexVerse-Endpunkt neu abgerufen
- die aktuell vom Server gelieferte XML wird neu ausgewertet und übernommen
- insbesondere wird die jeweils aktuelle `login`-URI aus der XML verwendet
- es gibt keine manuell eintragbare oder alternative Grid-/Login-Adresse im normalen Viewer

Echter NexVerse-Test am 05.10.2026:

```text
GridInfo: LIVE
gridname=Stadt NexVerse
gridnick=Stadt NexVerse
login=http://hg.stadt-nexverse.de:80/
platform=OpenSim
```

Weitere vom Grid gelieferte Felder:
- about
- gatekeeper
- help
- home
- password
- register
- welcome

Der zweite Live-Abruf erkennt korrekt `Changed: no`.

## Tests

Lokaler Debug-Build erfolgreich.

```text
nexverse-core-tests             PASSED
nexverse-grid-info-tests        PASSED
nexverse-grid-discovery-tests   PASSED
nexverse-viewer-version         PASSED
```

4/4 Tests grün.

## Noch offen vor vollständigem Abschluss von Phase 1

- Änderungen noch nicht nach GitHub gepusht.
- Windows- und macOS-CI für diesen lokalen Block noch nicht ausgeführt.
- Desktop-/Installer-Icon ist weiterhin offen.
- Optionaler weiterer URI-Parser-Feinschliff kann später erfolgen, falls reale OpenSim-Grids Sonderfälle liefern.

## UI-Zielbild / Designstand

Am 05.10.2026 wurde die Phase-1-Oberfläche lokal in Richtung des verbindlichen NexVerse-Zielbilds umgebaut.

Neue Struktur:
- OpenGenesisLINK Branding in der Topbar
- NexVerse Grid/Status in der Kopfzeile
- linke Navigation mit Start, Welt, Orte, Freunde, Gruppen, Events, Inventar und Marketplace
- große zentrale World-Preview-Fläche
- rechte Kontextleiste für Grid, Region, Orte, Nähe und Systemstatus
- untere Anmelde-/Aktionsleiste
- vorbereitetes Dock für Chat, Sprechen, Bewegen, Teleport, Inventar und Bauen
- responsive Skalierung für unterschiedliche Fenstergrößen
- keine simulierten Online-Avatare oder Regionen vor echter Serververbindung

Die UI orientiert sich am bereitgestellten NexVerse-Konzeptbild, bleibt aber technisch an den real implementierten Phasen ausgerichtet.

Das bereitgestellte OpenGenesisLINK-Anwendungsicon ist die verbindliche Branding-Quelle für Linux, Windows und macOS. Die plattformspezifische Binär-/Bundle-Einbindung bleibt als Packaging-Schritt offen.

## Phase 2 – OpenSim Login

Phase 2 ist lokal in Arbeit. Der Protokollkern und die UI-Anbindung sind implementiert.

Vorhanden:
- XML-RPC `login_to_simulator` Request
- kompatibles `$1$<md5>` Passwortformat
- Klartextpasswort wird nicht geloggt und nicht per CLI verwendet
- HTTP POST über den bestehenden libcurl-Transport
- 8 Sekunden Connect-Timeout / 30 Sekunden Gesamt-Timeout
- Login-Response-Parser
- Session-Daten für Agent ID, Session ID, Secure Session ID, Circuit Code, Simulator IP/Port, Region X/Y und Seed Capability
- Inventar-Root, Inventar-Skeleton, Library-Root, Library-Skeleton und Library-Owner werden aus der Login-Antwort übernommen
- Buddy-Liste inklusive Rechtewerten wird aus der Login-Antwort übernommen
- Gesten-Grunddaten (`item_id` / `asset_id`) werden übernommen
- Initial-Outfit-Basisdaten werden übernommen
- Login-Flags werden übernommen
- globale OpenSim-Textur-IDs für Cloud/Sun/Moon werden übernommen
- Fehler-/Reason-/Message-Auswertung
- echte Benutzername-/Passwort-Eingabe im Viewer
- Passwortmaskierung
- Mausfokus, Tab, Backspace und Enter
- asynchroner Login, damit die UI nicht blockiert
- Login-Status READY / CONNECTING / CONNECTED / ERROR in der Oberfläche
- nach erfolgreichem Login zeigt die Oberfläche den angemeldeten Avatar und eine aktive OpenSim-Session
- Session-Diagnose zeigt Simulator-IP/-Port sowie Anzahl von Inventarordnern, Buddies und Gesten
- das Passwort wird beim Start des Login-Workers sofort aus dem UI-Zustand entfernt und nach dem Request im Worker überschrieben/geleert
- Unit-Test für Request, Hash, Erfolgsantwort, Fehlerantwort, Bootstrap-/Appearance-Daten und Transportvertrag

Teststand:
- 5/5 lokale Tests grün
- Viewer startet und lädt die Live-GridInfo
- echter Login gegen NexVerse muss jetzt manuell mit einem vorhandenen Benutzerkonto bestätigt werden; Zugangsdaten werden nicht an ChatGPT übermittelt.

## Nächster technischer Schritt

Phase 2 ist abgeschlossen. Der echte Login gegen NexVerse wurde am 05.10.2026 erfolgreich bestätigt.

Bestätigt:
- Login-Status `CONNECTED`
- aktive OpenSim-Session
- Simulator-Ziel aus der echten Login-Antwort wurde übernommen
- Inventar-/Buddy-/Gesten-Bootstrapdaten wurden übernommen
- Viewer bleibt fest an die NexVerse-GridInfo gebunden

## Phase 3 – LLUDP Transport

Phase 3 ist lokal in Arbeit.

Bereits implementiert:
- plattformübergreifender UDP-Socket für Linux/Windows/macOS
- LLUDP Header mit Flags, Big-Endian-Sequenznummer und Extra-Header-Länge
- High/Medium/Low/Fixed Message-ID-Encoding
- kleine Message-Template-Registry für StartPingCheck, CompletePingCheck, UseCircuitCode, CompleteAgentMovement und PacketAck
- UUID-Wire-Encoding
- UseCircuitCode Encoder
- CompleteAgentMovement Encoder
- PacketAck Encoder/Decoder
- appended ACKs in Little-Endian entsprechend LLUDP
- Zerocode-Decoder
- Reliable-Tracking mit Sequenznummern
- Resend mit Resent-Flag
- Retry-Limit
- generisches Byte-Throttling
- MTU 1400 / Payload-Richtwert 1200
- Circuit-State-Machine
- UseCircuitCode wird vor CompleteAgentMovement bestätigt
- StartPingCheck wird mit CompletePingCheck beantwortet
- eingehende Reliable-Pakete werden bestätigt
- 30-Sekunden-Inaktivitäts-Timeout
- begrenzter Reconnect im Viewer
- LLUDP-Status wird live in der Oberfläche angezeigt
- 6/6 lokale Tests grün

Noch offen für Phase 3:
- echter NexVerse-Circuit-Echttest nach Login
- Message-Template-Abdeckung über die Startpakete hinaus erweitern
- vollständige Receive-/Dispatch-Schicht für weitere Simulatorpakete
- Phase 3 erst nach erfolgreichem NexVerse-Echttest als abgeschlossen markieren

Credentials dürfen niemals geloggt oder als Kommandozeilenparameter übernommen werden.
