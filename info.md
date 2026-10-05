[Reading 259 lines from start (total: 259 lines, 0 remaining)]

[Reading 212 lines from start (total: 212 lines, 0 remaining)]

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
- kleine Message-Template-Registry für StartPingCheck, CompletePingCheck, UseCircuitCode, RegionHandshake, RegionHandshakeReply, CompleteAgentMovement, AgentMovementComplete und PacketAck
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
- RegionHandshake wird erkannt und mit zuverlässigem RegionHandshakeReply beantwortet
- AgentMovementComplete markiert den Circuit als wirklich aktiv
- StartPingCheck wird mit CompletePingCheck beantwortet
- eingehende Reliable-Pakete werden bestätigt
- 30-Sekunden-Inaktivitäts-Timeout
- begrenzter Reconnect im Viewer
- LLUDP-Status wird live in der Oberfläche angezeigt
- Statuswechsel werden gezielt geloggt, damit Circuit-Fehler im Echttest lokalisierbar sind
- 9/9 lokale Tests grün

Echttest am 05.10.2026 erfolgreich bestätigt:
- `SOCKET` – UDP-Socket zum echten NexVerse-Simulator geöffnet
- `CIRCUIT` – `UseCircuitCode` gesendet
- `MOVEMENT` – `UseCircuitCode` bestätigt und `CompleteAgentMovement` gesendet
- `REGION` – `RegionHandshake` empfangen und `RegionHandshakeReply` gesendet
- `UDP LIVE` – `AgentMovementComplete` empfangen; Circuit aktiv
- sauberer Shutdown bestätigt

Zusätzlich vorhanden:
- generischer `PacketDispatcher` mit registrierbaren Handlern
- jedes dekodierte Circuit-Paket kann nach der Transportbehandlung über einen Packet-Callback an spätere WorldModel-/Chat-/Movement-/Object-Module weitergereicht werden
- typisierte `AgentUpdate`-Nachricht für Bewegung/Kamera
- typisierte `ChatFromViewer`-Nachricht für Local Chat
- `ChatFromSimulator`-Decoder und registrierter Dispatcher-Handler
- typisierte `TeleportLocationRequest`-Nachricht
- Decoder für `TeleportProgress` und `TeleportFailed`
- thread-sichere begrenzte Outbound-Queue zwischen UI/Input und LLUDP-Thread
- `W/S/A/D`, Leertaste, `C`, Pfeil links/rechts und Shift sind als `AgentUpdate`-Steuerflags vorbereitet
- AgentUpdate ist auf 10 Hz ausgelegt, wird aber erst gesendet, wenn `simulation_ready` aktiv ist
- 9/9 lokale Tests grün

Welcome-/Startseite:
- feste eingebettete URL: `https://stadt-nexverse.de/service/welcome`
- Linux verwendet WebKitGTK und rendert die Seite intern als OpenGL-Hintergrundtextur
- die Welcome-Seite liegt hinter Login bzw. Simulations-Ladezustand
- bei HTTP-/Ladefehlern bleibt der Viewer nutzbar und zeigt den NexVerse-Fallback-Hintergrund
- aktueller Serverstand am 05.10.2026: die feste URL liefert HTTP 404, die Viewer-Integration selbst funktioniert und wartet auf eine erreichbare Seite
- Plattformabstraktion ist vorhanden; Windows WebView2 und macOS WKWebView bleiben für die jeweiligen Plattform-Builds umzusetzen

UI-Zustandslogik aktualisiert:
- vor Login nur Anmeldung und Verbindungsstatus
- nach Login/UDP-Circuit nur `Simulation wird geladen`
- Navigation, In-World-Dock, rechte Welt-/Freunde-/Region-Panels und Bewegungsinput bleiben gesperrt, bis ein zukünftiger WorldModel-/Renderer-Status `simulation_ready` liefert
- `UDP LIVE` allein schaltet die Viewer-Navigation nicht mehr frei

Phase 3 ist abgeschlossen.

Abschlussstand:
- echter NexVerse-LLUDP-Circuit erfolgreich bestätigt
- UDP, Sequenzen, ACK, Resend, Reliable, Throttling, MTU, Timeout und Reconnect vorhanden
- PacketDispatcher mit Fallback vorhanden
- typisierte Outbound-Nachrichten für AgentUpdate, Local Chat und Teleport vorhanden
- zentrale eingehende Weltpakete sind im Message-Template registriert: LayerData, ObjectUpdate, ObjectUpdateCompressed, ObjectUpdateCached, ImprovedTerseObjectUpdate, KillObject, CoarseLocationUpdate, AvatarAppearance und AgentDataUpdate
- Ladebildschirm zählt Terrain-, Objekt-, Avatar- und sonstige Pakete, ohne die In-World-UI vorzeitig freizuschalten
- 9/9 lokale Tests grün

## Phase 4 – CAPS / HTTP Capabilities

Phase 4 ist lokal in Arbeit.

Bereits implementiert:
- generisches LLSD-Datenmodell
- LLSD XML Serialize/Parse
- LLSD Binary Serialize/Parse
- Größen- und Nesting-Limits
- Seed-Capability-Request als `application/llsd+xml`
- Capability-Discovery aus der Login-`seed_capability`
- nur validierte HTTP/HTTPS Capability-URLs werden übernommen
- Standard-Capability-Liste für EventQueue, SimulatorFeatures, Assets, Mesh, Inventory, Appearance und weitere Viewer-Bereiche
- `SimulatorFeatures` Abruf und LLSD-Auswertung
- EventQueue-Long-Poll mit `ack`/`done`
- EventQueue-Batch-Parser
- eigener `CapsEventDispatcher` mit typisierten Message-Namen und Fallback
- automatischer CAPS-Worker startet nach erfolgreichem Login
- Capability-URLs werden nicht geloggt
- Ladebildschirm zeigt CAPS-Status, Endpoint-Anzahl, SimulatorFeatures-Status und Eventanzahl
- sichere Phase-4-Diagnoselogs ohne Capability-URLs oder Zugangsdaten:
  - Seed-Discovery erfolgreich/fehlgeschlagen plus Anzahl akzeptierter Endpunkte
  - SimulatorFeatures erfolgreich/fehlgeschlagen/nicht angeboten
  - EventQueueGet vorhanden/nicht angeboten
  - erster erfolgreicher EventQueue-Batch wird einmalig als `CAPS EventQueue live` geloggt
  - EventQueue-Retries werden mit Retry-Zähler geloggt
- 12/12 lokale Tests grün nach dem Diagnoseblock

NexVerse-Echttest am 05.10.2026:
- Seed-Capability erfolgreich: 16 valide Capability-Endpunkte erkannt
- `EventQueueGet` wird vom Simulator angeboten
- EventQueue-Long-Poll erfolgreich: erster echter Batch empfangen, Status `CAPS LIVE`
- LLUDP blieb parallel `UDP LIVE`
- `SimulatorFeatures` wird angeboten, der Abruf ist im Echttest jedoch noch fehlgeschlagen
- Diagnose wurde erweitert: der nächste Testlauf protokolliert den konkreten HTTP-/LLSD-Fehler ohne Capability-URL

SimulatorFeatures-Fehleranalyse am 05.10.2026:
- Echttest meldete zunächst `LLSD real is invalid`
- zweiter Diagnose-Echttest zeigte den konkreten gültigen NexVerse-Wert `11.001100540161133`
- Ursache: `std::strtod` war locale-abhängig und lief unter `de_DE`; LLSD verwendet unabhängig von der System-Locale Punktnotation
- LLSD-`real` wird jetzt locale-unabhängig mit `std::from_chars` geparst
- OpenSim-kompatible leere Real-Werte `<real/>` werden als `0.0` akzeptiert
- führender/nachgestellter XML-Whitespace bei Real-Werten wird akzeptiert
- Regressionstests für `<real/>`, `<real>  1.5000  </real>` und den echten NexVerse-Wert `11.001100540161133` ergänzt
- kompletter Debug-Build erfolgreich, 12/12 Tests grün
- persistente Viewer-Logdatei ergänzt, damit Echttests unabhängig vom Startweg auswertbar sind
  - Linux: `~/.local/state/opengenesislink/viewer.log` bzw. `$XDG_STATE_HOME/opengenesislink/viewer.log`
  - Windows: `%LOCALAPPDATA%\\OpenGenesisLINK\\viewer.log`
  - macOS: `~/Library/Logs/OpenGenesisLINK/viewer.log`
- Logger schreibt weiterhin auf Konsole und zusätzlich sofort in die Logdatei

Phase 4 abgeschlossen am 05.10.2026:
- finaler NexVerse-Echttest erfolgreich
- Seed-Capability: 16 valide Endpunkte
- `SimulatorFeatures`: erfolgreich geladen
- `EventQueueGet`: verfügbar und echter Long-Poll-Batch empfangen
- LLUDP parallel `UDP LIVE`
- Phase 4 ist damit vollständig abgeschlossen

Aktuell:
- Phase 5 – WorldModel ist gestartet
- eigenes Modul `NexVerse::World` angelegt
- renderer-unabhängiges `WorldModel` mit Grid, Region, Agent, Avatar, Object, Terrain, Parcel und Environment
- WorldModel akzeptiert nur typisierte/validierte Zustandsupdates; keine LLUDP-/CAPS-Rohdaten und keine Renderer-Abhängigkeit
- Login initialisiert Grid/Region/Agent im WorldModel
- LLUDP-Circuit aktualisiert ausschließlich den verbunden/anwesend-Zustand von Region und Agent
- `simulation_ready` wird jetzt aus `WorldModel::ready()` abgeleitet
- wichtig: Login + `UDP LIVE` reichen weiterhin nicht; `ready()` verlangt zusätzlich validierte Weltdaten (derzeit Terrain)
- neuer `nexverse-world-model-tests` Test
- kompletter Debug-Build nach Phase-5-Grundblock: 13/13 Tests grün

Nicht mehr Phase 3, sondern spätere Phasen:
- CAPS/EventQueue/SimulatorFeatures → Phase 4
- WorldModel-Dekodierung und Weltzustände → Phase 5
- sichtbare Region/Terrain/Avatar/Objekte → Renderer-/World-Phasen
- Chat-/Teleport-/Movement-UI bleibt bis `simulation_ready` gesperrt

Credentials dürfen niemals geloggt oder als Kommandozeilenparameter übernommen werden.

[executed on device: jam-MS-7D28 (3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d)]
## Verbindliche Funktionsbaseline

Ab 05.10.2026 gilt Firestorm funktional als Mindestmaßstab für die üblichen Viewer-Kernfunktionen. OpenGenesisLINK bleibt technisch und gestalterisch eine eigenständige Implementierung.

Verbindlich eingeplant sind unter anderem Bewegung/Flug/Sitzen/Teleport, Kamera/Mouselook, Local Chat/IM/Gruppen, Freunde/Profile/Radar, Inventar, Attachments/Outfits/Gesten/AO, Karte/Minimap/Landmarks, Bauen/Object Edit, Parcel/Land, Media/Music/Voice, Environment sowie Diagnose-/Preferences-Funktionen.

Spezielle Firestorm-Erweiterungen wie RLV/RLVa oder sehr spezielle Firestorm-Power-User-Werkzeuge sind nicht automatisch Teil der Mindestbaseline und werden nur bei Bedarf ergänzt.

[executed on device: jam-MS-7D28 (3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d)]

## Next-Gen Renderer Vorgabe

Die vom Nutzer bereitgestellte „OpenGenesisLINK NextGen Renderer Anweisung“ ist als verbindliche technische Zielrichtung in `projectplan.md` unter Abschnitt 2.2 integriert.

Für spätere Arbeit gilt:
- Netzwerk/OpenSim bleibt vom Renderer entkoppelt.
- WorldModel/Scene Layer ist die Grenze zwischen Protokolldaten und Rendering.
- Vulkan ist das primäre NextGen-Backend.
- OpenSim-Kompatibilität bleibt Pflicht.
- OGLRender wird als modulare Render-Abstraktion vorbereitet.
- Legacy-/NextGen-Parallelbetrieb bleibt während der Migration erlaubt.
- PBR, modernes Lighting, Schatten, Reflexionen, Terrain, Wasser, Atmosphäre, Avatar-Rendering, Streaming, GPU-Culling und Performance-Diagnose werden schrittweise aufgebaut.
- Raytracing, Upscaling, Wetter, GI und Enhanced Server Capabilities sind Zukunftsmodule und dürfen nicht ohne passende spätere Phase vorschnell vollständig umgesetzt werden.
- Bei jeder Rendering-/WorldModel-Entscheidung ist Abschnitt 2.2 des `projectplan.md` erneut heranzuziehen.
