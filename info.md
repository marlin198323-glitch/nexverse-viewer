# OpenGenesisLINK Viewer für NexVerse – Arbeitsstand

> `projectplan.md` ist für Architektur, Reihenfolge und Projektumfang immer maßgebend.
> Diese Datei dokumentiert den tatsächlichen Entwicklungsstand und den Wiedereinstiegspunkt.

Stand: 11.10.2026
Lokaler Arbeitsbranch: `develop-current`
GitHub-Handoff-Branch: `develop-current`
Hinweis: Remote-`develop` besitzt derzeit eine ältere divergierende Historie und ist nicht der maßgebliche Handoff-Stand.
Lokaler Pfad: `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`
GitHub-Repository: `marlin198323-glitch/nexverse-viewer`

## Wiedereinstieg

Wenn der Benutzer in einem neuen Chat `viewer` schreibt:

1. Zuerst lokal über Desktop Commander den Ordner prüfen.
2. `CHATUEBERGABE.md` lesen.
3. `projectplan.md` lesen.
4. Danach `info.md` lesen.
5. `git status`, `git log` und `VERSION` prüfen.
6. Lokal weiterentwickeln, kompilieren und testen.
7. GitHub erst für fertige, getestete Entwicklungsblöcke verwenden.
8. Nach jedem größeren Block `info.md` und `CHATUEBERGABE.md` aktualisieren.

## Produkt

Produktname: `OpenGenesisLINK Viewer`
Zielwelt/Grid: NexVerse
Technik: C++23, CMake, vcpkg, GLFW, GLAD, OpenGL 3.3 Core, FreeType.
Lizenz: MPL 2.0.
Aktuelle Viewer-Version: `0.2.0-alpha.85`.

Versionsregel:
- Entwicklungsstände werden fortlaufend versioniert.
- Phase 5 startet mit `0.2.0-alpha.1`.
- Beta/RC werden erst nach vollständiger Release-Baseline verwendet.
- stabile Zielversion des ersten Releases ist `1.0.0`.

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
- `LayerData` wird jetzt strukturell validiert (Layer-Typ + 16-Bit-Datenlänge + Payload-Grenze)
- nur Land (`0x4C`) und LandExtended (`0x4D`) markieren WorldModel-Terrain als verfügbar
- Wind/sonstige Layer können `simulation_ready` nicht auslösen
- gültiges Terrain erhöht eine Terrain-Revision im WorldModel
- persistente Diagnose meldet einmalig `WorldModel terrain stream validated`
- erster Ready-Übergang meldet `WorldModel READY; simulation UI may unlock`
- `CoarseLocationUpdate` wird typisiert dekodiert und aktualisiert den Avatar-Snapshot im WorldModel
- `KillObject` wird typisiert dekodiert und entfernt lokale Objekt-IDs aus dem WorldModel
- `ImprovedTerseObjectUpdate` aktualisiert Position/Rotation von bekannten Avataren und Objekten
- `ObjectUpdate` wird bounds-geprüft dekodiert; UUID, LocalID, PCode, ParentID, Position und Rotation werden übernommen
- eigener Agent erhält seine echte Position/Rotation aus seinem Avatar-`ObjectUpdate`
- Diagnose meldet erste echte Streams: `WorldModel coarse avatar stream validated`, `WorldModel full object stream validated`, `WorldModel terse object stream validated`
- `ObjectUpdateCompressed` wird typisiert dekodiert und übernimmt UUID, LocalID, PCode, Position und Rotation ins WorldModel
- `ObjectUpdateCached` wird typisiert als LocalID/CRC/UpdateFlags validiert; ohne eigenen Objektcache werden daraus bewusst keine erfundenen Weltattribute erzeugt
- zusätzliche Diagnosen: `WorldModel compressed object stream validated` und `WorldModel cached object stream validated`
- `AgentDataUpdate` wird typisiert dekodiert und übernimmt Vorname, Nachname, Gruppentitel, aktive Gruppen-ID, Gruppenrechte und Gruppenname in einen getrennten `AgentProfileState`
- Session-/Bewegungszustand und Profil-/Gruppendaten bleiben im WorldModel bewusst getrennt
- `AvatarAppearance` wird typisiert dekodiert und speichert Trial-Flag, TextureEntry, Visual-Parameter sowie Appearance-/COF-Versionen im Avatar-Zustand
- vorhandene Position/Rotation/LocalID bleiben bei Appearance-Updates erhalten
- Renderer-/Materialinterpretation erfolgt bewusst erst in späteren Phasen
- `EnvironmentSettings` wird aus der echten CAPS-Discovery übernommen
- generischer CAPS-LLSD-GET akzeptiert valide Map- und Array-Antworten für OpenSim-Kompatibilität
- gültige EnvironmentSettings erhöhen `EnvironmentState.revision` und setzen `available=true`
- Diagnose: `CAPS EnvironmentSettings loaded successfully`
- `ParcelProperties` High 23 ist als bekannte LLUDP-Nachricht registriert
- vollständiger bounds-geprüfter Parcel-Decoder übernimmt LocalID, Owner, Fläche, Flags, Verkaufspreis, Name/Beschreibung sowie Music-/Media-URL
- `ParcelState.available=true` wird erst nach vollständig validiertem Paket gesetzt
- Diagnose für Echttest: `WorldModel parcel stream validated`
- echter Terrain-Codec für OpenSim-LayerData implementiert
- GroupHeader, PatchHeader, Quantisierung, Patch-ID und DCT-Koeffizienten werden bitgenau dekodiert
- 16×16-IDCT erzeugt echte Höhenwerte je Terrain-Patch
- WorldModel hält jetzt ein 256×256-Höhenfeld, Patch-Gültigkeit und Patch-Abdeckung
- `simulation_ready` wird erst durch tatsächlich dekodierte Terrain-Patches freigegeben
- neuer `nexverse-terrain-codec-tests` Test
- typisierte EventQueue-World-Parser für `EnableSimulator`, `EstablishAgentCommunication`, `TeleportFinish` und `CrossedRegion`
- RegionHandle-Binary wird 64-Bit-sicher dekodiert; IPv4/IPv6-Simulatoradressen, Port und Regionsgröße werden validiert
- `TeleportFinish` und `CrossedRegion` starten eine echte WorldModel-Regionstransition
- beim Regionswechsel werden Terrain, Parcel, Environment, Avatare und Objekte der alten Region invalidiert
- Region `generation` wird bei jedem World-Wechsel erhöht
- alter LLUDP-Circuit darf eine neue Region nicht mehr fälschlich als verbunden markieren; Simulator-IP/Port müssen zum Login-Circuit passen
- neuer `nexverse-caps-world-events-tests` Test
- `RegionHandshake` Low 148 wird vollständig bounds-geprüft dekodiert
- WorldModel übernimmt autoritativ SimName, Region UUID, RegionFlags, ExtendedFlags, RegionProtocols, SimAccess und Wasserhöhe
- `AgentMovementComplete` Low 250 wird typisiert dekodiert
- Agent-/Session-ID und RegionHandle müssen zum aktuellen WorldModel passen, bevor Position übernommen wird
- Agentposition und Region-Agentposition werden aus `AgentMovementComplete` synchronisiert; ChannelVersion wird in RegionState gespeichert
- Diagnosen: `WorldModel RegionHandshake metadata validated` und `WorldModel AgentMovementComplete validated`
- spatial LLUDP-Updates werden nur noch für die aktuell verbundene Region akzeptiert
- ObjectUpdate/Compressed/Cached/Terse müssen zusätzlich mit ihrem RegionHandle zum WorldModel passen
- Terrain, Parcel, KillObject, CoarseLocation und AvatarAppearance werden während einer nicht verbundenen Regionstransition verworfen
- `WorldSnapshot` liefert eine immutable Kopie von Grid, Region, Agent, Profil, Avataren, Objekten, Terrain, Parcel und Environment
- Snapshot bleibt nach späteren WorldModel-Mutationen stabil und bildet die Übergabegrenze für Renderer/UI
- Phase 5 am 06.10.2026 mit `0.2.0-alpha.11` im echten NexVerse erfolgreich abgenommen
- Teilabnahme im echten NexVerse mit `0.2.0-alpha.7` bereits erfolgreich: Login, LLUDP `UDP LIVE`, CAPS/SimulatorFeatures, EnvironmentSettings, Full/Cached/Terse Object Streams, Terrain-Stream, Coarse-Avatar-Stream, EventQueue und `WorldModel READY`
- Echttest `0.2.0-alpha.11`: RegionHandshake-Metadaten validiert, AgentMovementComplete validiert, `UDP LIVE`, CAPS 16 Endpunkte, SimulatorFeatures OK, EnvironmentSettings OK, Full/Terse/Cached Object Streams validiert, Terrain-Höhendaten validiert, EventQueue LIVE und WorldModel READY
- READY-Abnahmelog: `terrain_patches=40; objects=49; avatars=0; parcel=-; environment=OK`; `parcel=-` bedeutet nur, dass in diesem Lauf keine `ParcelProperties`-Nachricht vom Simulator eingetroffen ist – Decoder und Parcel-State bleiben implementiert
- aktuelle Binärversion bestätigt: `OpenGenesisLINK Viewer 0.2.0-alpha.11`
- kompletter Debug-Build nach Phase-5-Abschlussblock: 15/15 Tests grün

Phase 6 gestartet:
- bestehende GLFW-Fenster-, Tastatur-, Maus-, Resize- und VSync-Grundlage bestätigt
- Frame-Time und geglättete FPS werden im Hauptloop gemessen
- kleines In-World-Debug-Overlay zeigt FPS und Framezeit
- AgentUpdate-Kamera verwendet keine fest codierten Achsen mehr
- Kameraachsen werden aus dem aktuellen Agent-Quaternion des WorldModel abgeleitet
- Kamerazentrum folgt der autoritativen Agentposition
- aktueller Phase-6-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.12`
- kompletter Debug-Build: 15/15 Tests grün
- eigenständiges `NexVerse::Render`-Modul mit backend-neutralem `Renderer`-Interface angelegt
- erstes Legacy-Backend: OpenGL 3.3, getrennt von UI und Protocol
- Backend übernimmt Viewport/Clear und rendert sichtbare rotierende RGB-Testgeometrie
- UI malt bei `simulation_ready` nur noch als Overlay über den Welt-Renderer; Login/Ladezustände behalten den NexVerse-Hintergrund
- Renderer-Diagnose liefert Backend, GPU, Draw Calls und Triangle Count
- NVIDIA-VRAM wird optional über `GL_NVX_gpu_memory_info` gelesen; bei nicht unterstützten GPUs bleibt VRAM sauber `n/a`
- System-RAM-Diagnose ergänzt
- Debug-Overlay zeigt FPS, Framezeit, Draw Calls, Triangles, RAM und VRAM
- aktueller Phase-6-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.13`
- kompletter Debug-Build nach Renderer-Interface-/Diagnoseblock: 15/15 Tests grün
- Renderer-Interface um backend-neutrale `RenderCamera` erweitert
- Legacy-OpenGL-Backend rendert jetzt einen echten 3D-Testwürfel mit Model-/View-/Projection-Matrix statt Clip-Space-Demo
- Testwürfel wird beim ersten gültigen Kamerastand einige Meter vor dem Agenten in Weltkoordinaten verankert
- Renderer-Kamera verwendet dieselbe WorldModel-Agentposition/-ausrichtung wie der an den Simulator gesendete AgentUpdate-Kamerastand
- System-RAM-Metriken werden im Core-Test auf plausible Werte geprüft
- damit sind alle im Projektplan definierten Phase-6-Funktionen implementiert
- aktueller Phase-6-Abschlussstand: `OpenGenesisLINK Viewer 0.2.0-alpha.14`
- visueller NexVerse-Echttest am 06.10.2026 erfolgreich: 3D-Würfel sichtbar, Renderer-Kamera aktiv, Debug-Overlay mit FPS/Frame/RAM/VRAM/Draw/Triangles sichtbar

Phase 7 gestartet:
- echtes OpenSim-Terrain wird aus dem 256x256-WorldModel-Höhenfeld in ein GPU-Mesh überführt
- Mesh berücksichtigt nur validierte Terrain-Patches und wächst progressiv mit neuen LayerData-Patches
- Patch-Grenzen werden zu einem zusammenhängenden Höhenmesh verbunden
- Terrain verwendet zunächst höhenbasierte Debug-Farbgebung; echte Bodentexturen folgen im Asset-/Materialpfad
- Wasserfläche wird auf der vom RegionHandshake gelieferten Wasserhöhe gerendert
- sichtbare Regiongrenze wird als Linienrahmen gerendert
- bei Regionswechsel wird das alte Terrain-Mesh über Region-generation/revision verworfen
- ohne Terrain bleibt der Phase-6-Würfel als Fallback sichtbar
- neuer CPU-Test `nexverse-terrain-mesh-tests` prüft partielle und progressive Mesh-Erweiterung
- aktueller sichtbarer Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.15`
- kompletter Debug-Build: 16/16 Tests grün
- Viewer startet standardmäßig im echten Fullscreen-Modus des primären Monitors; bei fehlender Monitor-Mode gibt es einen maximierten Fallback
- Widescreen-/Ultrawide-HUD skaliert primär über die vertikale Auflösung, damit die 3D-Welt auf 21:9/32:9 nicht durch breite Seitenpanels verloren geht
- nach erfolgreichem Login erscheint jetzt ein eigener Fullscreen-Ladebildschirm statt des Dashboard-Zwischenzustands
- Ladebildschirm zeigt Fortschrittsbalken und einzelne Schritte für Grid, LLUDP, CAPS, Terrain und Objekte
- bei `WorldModel READY` wechselt der Viewer automatisch in die 3D-Welt
- im In-World-Zustand wird das große Dashboard vollständig ausgeblendet
- stattdessen gibt es nur ein schmales transparentes World-HUD mit Titel/Grid, Session, Aktionen und Diagnose
- aktueller UI-/Fullscreen-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.16`
- kompletter Debug-Build nach Fullscreen-/Loading-/World-HUD-Block: 16/16 Tests grün
- `0.2.0-alpha.17`: Schriftfehler im In-World-Fullscreen behoben; UI-Renderer aktiviert Blending für jeden Shape-/Image-/Text-Pass selbst
- Ursache der Blockschrift war vom World-Renderer deaktiviertes `GL_BLEND`, nicht fehlende Fontdaten
- In-World-HUD wieder auf die ursprüngliche NexVerse-Designsprache zurückgeführt: Branding-Leiste oben, kompakte Navigation links, Statuskarten rechts, Aktionsleiste unten
- 3D-Welt bleibt Hauptfläche; Seitenelemente sind transparent und bewusst kompakter als das ursprüngliche Dashboard
- Aktualisiert 11.10.2026: Firestorm dient als vollständige Funktions- und Implementierungsbaseline einschließlich Viewer-Verhalten, DPI/Scaling, Loading, Kamera und OpenSim-/OpenGenesisLINK-Kompatibilität (eigener aktueller Code noch ohne echten Firestorm-Source-Fork)
- Firestorm-Skin/Style/XUI wird nicht übernommen; NexVerse-Design und Farben bleiben verbindlich
- aktueller UI-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.17`
- kompletter Debug-Build nach Schrift-/NexVerse-HUD-Fix: 16/16 Tests grün
- `0.2.0-alpha.18`: Viewer startet nicht mehr im exklusiven Monitor-Fullscreen, sondern als normales dekoriertes Fenster und wird direkt maximiert
- native Fenstersteuerung bleibt verfügbar: Minimieren, Maximieren/Wiederherstellen und Schließen
- `F11` schaltet optional in echten exklusiven Fullscreen und mit erneutem `F11` zurück in den maximierten Fenstermodus
- Fenster bleibt resizable und behält die Widescreen-/Ultrawide-Skalierung
- aktueller Fenster-/UI-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.18`
- kompletter Debug-Build nach Window-Mode-/F11-Block: 16/16 Tests grün
- `0.2.0-alpha.19`: Renderer-Kamera vom LLUDP-AgentUpdate-Kamerastand getrennt
- In-World-Renderer verwendet jetzt eine Third-Person-Kamera hinter und oberhalb des Agenten mit Blick auf den Avatar-/Bodenbereich
- Simulator erhält weiterhin den bisherigen protokollseitigen AgentUpdate-Kamerastand
- Legacy-Renderer besitzt einen eigenen Himmelspass vor Terrain/Wasser
- Himmel-Fallback rendert Horizont-/Zenit-Verlauf, Sonnenlicht und leichten atmosphärischen Farbverlauf
- Terrain-Fragmentshader blendet in großer Tiefe leicht in die Horizontfarbe und erzeugt damit sichtbare atmosphärische Distanz
- dieser Himmel ist ein OpenSim-Kompatibilitätsfallback; exakte EEP-/Environment-Parameter folgen separat
- aktueller sichtbarer Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.19`
- kompletter Debug-Build nach Kamera-/Himmel-/Fog-Block: 16/16 Tests grün
- `0.2.0-alpha.20`: RegionHandshake dekodiert jetzt die 4 TerrainBase-UUIDs, 4 TerrainDetail-UUIDs sowie TerrainStartHeight00/01/10/11 und TerrainHeightRange00/01/10/11
- Terrainmaterial-Metadaten liegen getrennt im WorldModel-`TerrainState` und besitzen eine eigene Revision
- Regionswechsel invalidiert damit automatisch auch alte Terrainmaterialien
- Renderer-Mesh wird bei Material-Revision neu aufgebaut, auch wenn sich das Höhenfeld selbst nicht geändert hat
- vorläufige Terrain-Farbmischung verwendet jetzt die echten OpenSim-Start-/Range-Höhen statt globalem Min/Max
- RegionHandshake- und Terrain-Mesh-Regressionsfälle auf die neuen Materialdaten erweitert
- aktueller Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.20`
- kompletter Debug-Build nach Terrainmaterial-Metadatenblock: 16/16 Tests grün
- `0.2.0-alpha.21`: neues eigenständiges `NexVerse::Assets`-Modul
- OpenSim-`GetTexture` wird mit `texture_id=<UUID>&format=png` verwendet; Capability-URL bleibt intern und wird nicht geloggt
- PNG-Texturen werden mit libpng bounds-geprüft in RGBA8 dekodiert
- Downloadlimit 32 MiB und maximale Texturgröße 4096x4096 verhindern unkontrollierte Assetgrößen
- neuer `nexverse-texture-loader-tests` Test mit echtem Binär-PNG und Mock-HTTP-Transport
- Terrain-Detailtexturen werden pro Region/Materialrevision asynchron geladen, ohne EventQueue oder Rendering zu blockieren
- Renderer übernimmt vier Terrain-Detailtexturen auf die GPU und mischt sie anhand des echten OpenSim-Materialfaktors
- fehlende Einzeltexturen erhalten sichere 1x1-Fallbackfarben; ohne echte Assets bleibt der bisherige Terrain-Farbmodus aktiv
- Regionswechsel/Materialrevision verhindern, dass verspätete Texturergebnisse in eine neue Region übernommen werden
- aktueller sichtbarer Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.21`
- kompletter Debug-Build nach GetTexture-/PNG-/GPU-Terraintexturblock: 17/17 Tests grün
- `0.2.0-alpha.22`: eigener Parser für OpenSim-`EnvironmentSettings`/Legacy-WindLight-LLSD ergänzt
- Sky-Track und referenzierter Sky-Frame werden aus dem 4-Element-WindLight-Array typisiert ausgewertet
- WorldModel-`EnvironmentState` enthält jetzt Ambient, BlueDensity, BlueHorizon, SunlightColor, Haze, Gamma, Sun/East Angle, Cloud-Werte und WaterFog-Daten
- ungültige oder unbekannte EnvironmentSettings aktivieren den Renderer-Fallback statt einen fehlerhaften Environment-State
- Legacy-Renderer erhält Environment-Snapshots revisionsbasiert aus dem WorldModel
- Himmel verwendet Regions-Horizont-/Zenitfarbe, Sonnenfarbe, Sonnenposition, Gamma und Haze
- Terrain-Fog verwendet die Regions-Horizontfarbe
- Wasser verwendet die Regions-`waterFogColor` als sichtbare Basisfarbe
- neuer `nexverse-environment-settings-tests` Regressionstest
- aktueller sichtbarer Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.22`
- kompletter Debug-Build nach Environment-/WindLight-Block: 18/18 Tests grün
- `0.2.0-alpha.23`: Seed-Capability-Liste um `ExtEnvironment` erweitert
- `ExtEnvironment` wird gegenüber Legacy-`EnvironmentSettings` bevorzugt
- EEP-Parser unterstützt `environment -> day_cycle -> frames/tracks`
- Water- und Sky-Frame werden über `key_name` aus den jeweiligen EEP-Tracks aufgelöst
- EEP-`legacy_haze`, SunlightColor, CloudColor, Gamma, StarBrightness und CloudShadow werden übernommen
- `sun_rotation` wird als Quaternion normalisiert und in Sun/East Angle für den bestehenden Rendererpfad umgerechnet
- EEP-Wasser übernimmt `water_fog_color`, `water_fog_density` und `normal_map`
- bei fehlender/fehlerhafter `ExtEnvironment`-Antwort erfolgt automatisch Fallback auf `EnvironmentSettings`
- EEP und Legacy-WindLight schreiben in denselben WorldModel-`EnvironmentState`
- Environment-Regressionsuite deckt jetzt Legacy-WindLight und modernes EEP ab
- aktueller Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.23`
- kompletter Debug-Build nach ExtEnvironment-/EEP-Fallback-Block: 18/18 Tests grün
- `0.2.0-alpha.24`: EEP-Daycycle wird zeitabhängig nach OpenSim-Semantik gesampelt
- Daycycle-Zeit: `((UnixTime + day_offset) % day_length) / day_length`
- Sky- und Water-Tracks werden nach `key_keyframe` sortiert und zyklisch über 0.0/1.0 hinweg ausgewertet
- vorheriger/nächster Keyframe werden inklusive Mitternachts-Wrap bestimmt
- Vektor-/Skalarwerte werden linear interpoliert; Sun/East Angle verwenden winkelbewusste Wrap-Interpolation
- WaterFog-Farbe und WaterFog-Dichte werden ebenfalls zwischen Water-Keyframes interpoliert
- neue deterministische Regression: day_length=100, Unix=25 ergibt exakt 50% zwischen Keyframes 0.0 und 0.5
- erfolgreicher `ExtEnvironment`-Payload wird intern gehalten und nicht erneut per HTTP geladen
- laufende Session sampelt EEP lokal ungefähr alle 2,5 Sekunden neu und schreibt eine neue Environment-Revision ins WorldModel
- Himmel, Sonne, Haze, Terrain-Fog und Wasser können sich dadurch während der Session mit dem Region-Daycycle verändern
- aktueller Phase-7-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.24`
- kompletter Debug-Build nach EEP-Daycycle-Live-Sampling: 18/18 Tests grün
- `0.2.0-alpha.25`: Phase 8 gestartet; echte OpenSim-Prims werden vom WorldModel an OGLRender übertragen
- `ObjectUpdate` dekodiert Scale X/Y/Z jetzt statt die 12 Bytes zu überspringen
- WorldModel-`ObjectState` enthält PCode, Parent-ID, Position, Rotation und Scale
- eigene `object_revision` sorgt dafür, dass Scene-Objekte nur bei Änderungen/Regionswechsel neu an den Renderer übertragen werden
- Compressed-/Terse-Updates erhalten bekannte statische Objektwerte wie Scale/Parent/PCode aus dem bisherigen WorldModel-Zustand
- OGLRender besitzt backendneutrale `RenderSceneObject`-/`RenderQuaternion`-Strukturen; OpenSim-PCode bleibt außerhalb des Renderers
- Prim-Basisdarstellung verwendet ein korrekt normiertes 1x1x1-Box-Mesh mit echter Position, Quaternion-Rotation und Scale
- Parent/Child-Transformationen werden rekursiv bis maximal 16 Ebenen aufgelöst; Root-Scale beeinflusst Child-Offsets nicht
- Scene-Objekte werden gegen den Kamera-Far-Clip distanzbasiert gecullt
- `KillObject` entfernt Objekte weiterhin aus dem WorldModel und erhöht nun die Scene-Revision
- HUD-Diagnose zeigt zusätzlich `Obj <sichtbar>`
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.25`
- kompletter Debug-Build nach erstem Prim-/Scene-Renderblock: 18/18 Tests grün
- `0.2.0-alpha.26`: OpenSim-`PrimitiveShapeData` wird vollständig aus `ObjectUpdate` übernommen
- Shape-Klassifizierung mappt gerade Pfade + Kreisprofil auf Zylinder, Dreiecksprofile auf Prisma und quadratische Profile auf Box
- gekrümmte Pfade mit Halbkreisprofil werden als Kugel behandelt; andere gekrümmte Pfade als Torus
- unbekannte/noch nicht unterstützte Shape-Kombinationen fallen sicher auf Box zurück
- OGLRender besitzt getrennte GPU-Meshes für Box, Zylinder, Prisma, Kugel und Torus
- echte Position, Quaternion-Rotation, Scale und Parent/Child-Transformation bleiben für alle Grundformen aktiv
- Compressed-/Terse-Updates behalten die zuletzt bekannte Shape-Geometrie aus dem WorldModel
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.26`
- kompletter Debug-Build nach Prim-Grundformenblock: 18/18 Tests grün
- `0.2.0-alpha.27`: lineare OpenSim-Prims unterstützen jetzt Taper, Shear und Twist direkt im Vertexshader
- `PathTaperX/Y` wird nach OpenSim-Semantik mit `raw * 0.01` skaliert
- `PathShearX/Y` wird als signiertes 8-Bit-Feld interpretiert und mit `0.01` skaliert
- `PathTwistBegin/End` wird als `raw * PI * 0.01` in Bogenmaß umgesetzt
- Taper skaliert den Querschnitt entlang des linearen Pfads
- Twist interpoliert zwischen Begin/End und rotiert den Querschnitt pro Vertex
- Shear verschiebt den Querschnitt entlang der Pfadachse
- Deformation wird nur für gerade/flexible Pfade aktiviert; Kugel/Torus bleiben bis zum Sweep-Geometrieausbau unverändert
- Terrain-/Wasser-Shaderpfade bleiben durch `uUsePrimDeform=false` unbeeinflusst
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.27`
- kompletter Debug-Build nach Taper-/Shear-/Twist-Block: 18/18 Tests grün
- `0.2.0-alpha.28`: lineare Prims unterstützen jetzt `PathBegin`/`PathEnd`
- OpenSim-Skalierung: `PathBegin = raw * 0.00002`, `PathEnd = 1 - raw * 0.00002`
- der Basis-Querschnitt wird im Vertexshader auf den tatsächlich sichtbaren Pfadbereich abgebildet
- Taper, Twist und Shear verwenden anschließend den echten Pfadparameter statt immer 0..1
- dadurch stimmen abgeschnittene lineare Box-/Zylinder-/Prisma-Pfade deutlich besser mit OpenSim überein
- ungültige/überlappende Bereiche werden auf einen minimal gültigen Pfadbereich geklemmt
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.28`
- kompletter Debug-Build nach PathBegin-/PathEnd-Block: 18/18 Tests grün
- `0.2.0-alpha.29`: lineare Primprofile unterstützen jetzt `ProfileBegin`, `ProfileEnd` und `ProfileHollow`
- OpenSim-Skalierung: Profile Begin/End/Hollow verwenden `raw * 0.00002`; Hollow wird auf maximal 0.95 begrenzt
- Quadrat-, Kreis- und Dreiecksprofile werden als echte Querschnittskonturen generiert
- Profile-Cuts erzeugen echte Schnittflächen statt nur Fragment-Clipping
- Hollow erzeugt Außenwand, Innenwand sowie obere/untere Ringflächen
- Hollow-Profile `Same`, `Circle`, `Square` und `Triangle` werden berücksichtigt
- dynamische Profilmeshes werden nach Shape/Begin/End/Hollow gecacht
- GPU-Profilcache ist auf 512 eindeutige Kombinationen begrenzt; darüber bleibt der sichere Standardmesh-Fallback
- Profilmesh-Cache wird beim Renderer-Shutdown vollständig freigegeben
- Taper/Shear/Twist/PathCut wirken weiterhin auf das generierte Profilmesh
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.29`
- kompletter Debug-Build nach ProfileCut-/Hollow-Block: 18/18 Tests grün
- `0.2.0-alpha.30`: gekrümmte OpenSim-Circular-Paths verwenden jetzt dynamische Sweep-Geometrie statt ausschließlich statischer Torus-/Sphere-Meshes
- `PathRadiusOffset = raw * 0.01`, `PathRevolutions = 1.0 + raw * 0.015`, `PathSkew = raw * 0.01`
- `PathScaleX/Y` wird nach OpenSim als `(200 - raw) * 0.01` in die Circular-Path-Querschnittsgröße übernommen
- Circular-Path-Berechnung übernimmt die wesentlichen OpenSim-`PrimMesher`-Terme für x/y-ProfileScale, RadiusScale, Skew, TopShear-X/Y, Path-Winkel und Twist
- PathBegin/End begrenzen den tatsächlichen Sweep-Bogen
- Revolutions steuert die Anzahl der Umläufe des Sweep-Pfads
- RadiusOffset verändert den Pfadradius entlang des Sweeps
- Skew verschiebt den Circular Path über seine Länge
- Taper/Shear/Twist werden auch auf gekrümmte Sweep-Layer angewendet
- Torus nutzt ein geschlossenes Kreisprofil; Sphere nutzt ein Halbkreisprofil über denselben Circular-Path-Mechanismus
- gekrümmte Sweep-Meshes werden in einem separaten GPU-Cache mit maximal 256 eindeutigen Kombinationen gehalten
- Sweep-Mesh-Cache wird beim Renderer-Shutdown vollständig freigegeben
- bei Cache-Limit/Generierungsfehler bleibt das vorhandene statische Torus-/Sphere-Fallback verfügbar
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.30`
- kompletter Debug-Build nach Circular-Sweep-/Revolutions-/RadiusOffset-/Skew-Block: 18/18 Tests grün
- `0.2.0-alpha.31`: eigenes Protocol-Modul für OpenSim/Linden-`TextureEntry` ergänzt
- Full-`ObjectUpdate` liest den `TextureEntry`-Variable2-Block jetzt autoritativ ein statt ihn zu überspringen
- Roh-`TextureEntry` wird im WorldModel erhalten; Compressed-/Terse-Updates übernehmen den zuletzt bekannten Materialzustand
- Default-Face-Parser dekodiert Texture UUID, invertierte RGBA-Farbe, RepeatU/V, OffsetU/V, Rotation, BasicMaterial, Media, Glow sowie optionale Material-/RenderMaterial-UUIDs
- TextureEntry-Face-Bitfield-Sektionen werden bounds-geprüft übersprungen; per-Face-Overrides folgen separat
- Renderer übernimmt Default-Face-Farbe und Alpha statt der bisherigen Debugfarbe
- transparente Default-Faces aktivieren Alpha-Blending nur für den betreffenden Drawcall; GL-Depth-/Blend-State wird anschließend wiederhergestellt
- Glow wird als erster sichtbarer Materialparameter berücksichtigt
- Fullbright und Shiny werden aus dem BasicMaterial-Byte dekodiert und im Render-State erhalten
- neuer `nexverse-texture-entry-tests` Binärtest sowie Full-ObjectUpdate-Regression mit echter TextureEntry
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.31`
- kompletter Debug-Build nach TextureEntry-/Default-Materialblock: 19/19 Tests grün
- `0.2.0-alpha.32`: echte Default-Face-Objekttexturen werden über die bereits entdeckte OpenSim-`GetTexture`-Capability geladen
- Objekttextur-Downloads verwenden dieselbe bounds-geprüfte PNG/RGBA8-Assetpipeline wie Terraintexturen
- aktuelle Scene-Texture-UUIDs werden pro Object-Revision gesammelt, sortiert und dedupliziert
- Objekttexturen werden seriell im Hintergrund geladen; bereits geladene/fehlgeschlagene UUIDs werden innerhalb der Session nicht erneut angefordert
- Objekttextur-GPU-Cache ist auf 512 eindeutige Assets begrenzt
- Renderer bindet Default-Face-Texture, RepeatU/V, OffsetU/V und Rotation pro Objekt
- TextureEntry-Farbe multipliziert die geladene Textur statt durch Debugfarbe ersetzt zu werden
- Fullbright deaktiviert den Distanz-Fog für den betreffenden Prim-Drawcall
- Glow bleibt aktiv und wird nach dem Texture-Multiply addiert
- PNG-Alpha aktiviert Blending auch dann, wenn das TextureEntry-Farb-Alpha selbst 1.0 ist
- Textur-GPU-Ressourcen werden beim Renderer-Shutdown freigegeben
- bei fehlgeschlagenem Assetdownload bleibt automatisch das vorhandene Default-Farbmaterial sichtbar
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.32`
- kompletter Debug-Build nach ObjectTexture-/GetTexture-/GPU-Block: 19/19 Tests grün
- `0.2.0-alpha.33`: `TextureEntry` wird jetzt vollständig mit Face-Bitmasken dekodiert statt Override-Sektionen nur zu überspringen
- jede Property-Sektion (Texture UUID, Farbe, Repeat, Offset, Rotation, Material, Media, Glow, Material-/RenderMaterial-UUID) kann gezielt einzelne Faces überschreiben
- per-Face-Zustände werden erst nach vollständigem Parse aus dem kompletten Default-Face abgeleitet, damit nicht überschriebene Eigenschaften korrekt geerbt werden
- `TextureEntry::face(index)` liefert transparent Default oder Face-Override
- Full-`ObjectUpdate` parst TextureEntry einmalig und hält den vollständigen Zustand im WorldModel
- bestehender `default_texture`-Pfad bleibt API-kompatibel
- Compressed-/Terse-Updates können den zuletzt bekannten vollständigen Face-Materialzustand weitertragen
- Face-Bitfield-Lesung ist auf 64 Faces und maximal 10 Varint-Bytes bounds-geprüft
- Regressionstest prüft getrennte Texture-/Color-/Glow-Overrides auf Face 0/1/2 inklusive korrekter Default-Vererbung
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.33`
- kompletter Debug-Build nach vollständigem TextureEntry-Face-Parser: 19/19 Tests grün
- `0.2.0-alpha.34`: aufgelöste TextureEntry-Face-Overrides werden vollständig in `RenderSceneObject::face_materials[64]` übertragen
- jedes Face-Material enthält Texture UUID, Farbe/Alpha, Repeat, Offset, Rotation, Fullbright, Shiny, Glow sowie Material-/RenderMaterial-UUID
- Override-Texture-UUIDs werden zusammen mit der Default-Texture in die deduplizierte GetTexture-Assetliste aufgenommen
- Standard-Boxen werden jetzt in sechs getrennten Indexgruppen gezeichnet
- OpenSim-Face-Mapping für Standard-Box: Top=0, Outer=1..4, Bottom=5
- die lokale Box-EBO-Reihenfolge Bottom/Top/Outer1..4 wird auf die OpenSim-Face-Nummern umgesetzt
- jedes Box-Face kann dadurch eigene Farbe, Alpha, Glow, Fullbright, Texture, Repeat, Offset und Rotation verwenden
- Texture-/Alpha-Blending und GPU-Texture-Binding erfolgen pro Face-Drawcall und werden danach sauber zurückgesetzt
- dynamische Profile, Cylinder/Prism und Circular-Sweeps bleiben vorerst beim Default-Face bis deren Facegruppen explizit markiert sind
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.34`
- kompletter Debug-Build nach erstem Renderer-Face-Mapping: 19/19 Tests grün
- `0.2.0-alpha.35`: Renderer-Meshes führen jetzt explizite OpenSim-Face-Ranges bis in den GPU-Mesh-Cache mit
- Standard-Zylinder werden nach OpenSim als Top=0, Outside=1, Bottom=2 getrennt materialisiert
- Standard-Prismen werden als Top=0, drei Außenflächen=1..3 und Bottom=4 getrennt materialisiert
- dynamische lineare Profile erzeugen ihre Face-Nummern aus den tatsächlich vorhandenen Profilflächen; durch Profile-Cuts vollständig entfernte Außenflächen verbrauchen keine Face-ID
- Hollow-Innenwand wird als genau ein eigenes OpenSim-Face geführt, unabhängig von der inneren Profilform
- Profile-Cut Begin/End erhalten nach Top/Außenflächen/Hollow/Bottom jeweils eigene Face-Ranges
- dynamische Circular-Sweep-Meshes trennen bei offenem Pfad Cut-End, Outside und Cut-Begin; bei geschlossenem Sweep bleibt Outside Face 0
- per-Face TextureEntry-Materialien werden nun auch für diese GPU-Mesh-Ranges beim Drawcall ausgewählt
- Default-Material bleibt Fallback, falls für eine Face-ID kein Override vorhanden ist
- kompletter Debug-Build nach erweitertem Prim-Face-Mapping: 19/19 Tests grün
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.35`
- `0.2.0-alpha.36`: Prim-Meshes besitzen jetzt explizite UV-Koordinaten pro Vertex statt shaderseitiger Normalen-Projektion
- Box-Mesh verwendet 24 getrennte Face-Vertices, damit jede OpenSim-Fläche eigene UV-Seams besitzt
- Zylinder: Top/Bottom verwenden planare UVs; Outside verwendet Umfang-U und Höhen-V mit sauberer 0/1-Seam
- Prisma: Top/Bottom verwenden planare Profil-UVs; jede der drei Außenflächen besitzt eigene 0..1-UVs
- dynamische lineare Profile erzeugen Face-spezifische UV-Vertices für Top, Outer, Hollow, Bottom und Profile-Cut Begin/End
- Hollow-Innenwand verwendet einen eigenen umlaufenden U-Kanal und Pfad-V
- Circular-Sweep-Außenflächen verwenden Profilparameter als U und Sweep-Pfad als V
- offene Circular-Sweep-Caps besitzen eigene Planar-UV-Seams und teilen keine Außenflächen-UVs mehr
- statische Sphere-/Torus-Fallbackmeshes besitzen ebenfalls sphärische bzw. toroidale UV-Grundkoordinaten
- TextureEntry RepeatU/V, OffsetU/V und Rotation werden nun auf den echten Prim-UVs angewendet
- alter `objectProjectedUv()`-Pfad wurde entfernt
- Laufzeittest des OpenGL-3.3-Shaders auf NVIDIA GeForce RTX 4070 SUPER erfolgreich
- kompletter Debug-Build nach Prim-UV-Block: 19/19 Tests grün
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.36`
- `0.2.0-alpha.37`: TextureEntry-Material-/Media-Bits werden zentral über Hilfsmethoden ausgewertet
- Fullbright, Shiny und Bumpiness werden aus dem Material-Byte getrennt erhalten
- Media-on-a-Prim und TexGen werden aus dem Media-Byte getrennt erhalten
- TexGen Planar wird bis in Default- und per-Face-RenderMaterial übertragen
- Planar-Mapping verwendet die lokale Face-Ebene und die echte Objektgröße
- OpenSim/SL-Semantik für Planar-Repeats wird umgesetzt: Repeat-Werte gelten pro 0,5 Meter statt pro Face
- Default-TexGen verwendet weiterhin die expliziten Face-UVs aus alpha.36
- RepeatU/V, OffsetU/V und Rotation werden nach Auswahl des Mapping-Modus angewendet
- Bumpiness und Media-Presence liegen nun im Renderer-Vertrag für die folgenden Material-/Media-Phasen bereit
- TextureEntry-Regressionsfall prüft Fullbright, Shiny, Bumpiness, Media und Planar-TexGen gemeinsam
- OpenGL-Shader-Laufzeittest auf NVIDIA GeForce RTX 4070 SUPER erfolgreich
- kompletter Debug-Build nach TexGen-/Media-Bit-Block: 19/19 Tests grün
- aktueller Phase-8-Stand: `OpenGenesisLINK Viewer 0.2.0-alpha.37`

Nicht mehr Phase 3, sondern spätere Phasen:
- CAPS/EventQueue/SimulatorFeatures → Phase 4
- WorldModel-Dekodierung und Weltzustände → Phase 5
- sichtbare Region/Terrain/Avatar/Objekte → Renderer-/World-Phasen
- Chat-/Teleport-/Movement-UI bleibt bis `simulation_ready` gesperrt

Credentials dürfen niemals geloggt oder als Kommandozeilenparameter übernommen werden.

## Verbindliche Funktionsbaseline

**Aktualisierung 11.10.2026:** Firestorm ist nicht mehr bloß Mindestmaßstab für übliche Kernfunktionen, sondern die verbindliche vollständige Feature-/Verhaltens- und Implementierungsreferenz des OGL Viewers. Serverseitige OpenGenesisLINK-/OpenSim-Funktionalität ist dabei autoritativ; neue OGL-Funktionen dürfen die vorhandene Firestorm-Funktionalität nur dokumentiert anpassen/ergänzen. Eigener C++23-Code ist noch kein Firestorm-Quellcode-Fork; Lizenz- und Portierstrategie bleibt explizit offen. Vollständige Regeln unter `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`.

Verbindlich eingeplant sind unter anderem Bewegung/Flug/Sitzen/Teleport, Kamera/Mouselook, Local Chat/IM/Gruppen, Freunde/Profile/Radar, Inventar, Attachments/Outfits/Gesten/AO, Karte/Minimap/Landmarks, Bauen/Object Edit, Parcel/Land, Media/Music/Voice, Environment sowie Diagnose-/Preferences-Funktionen.

Auch spezielle Firestorm-Funktionen wie RLV/RLVa, erweiterte Phototools und Power-User-Werkzeuge gehören zur verbindlichen Inventur. Gründe für Zurückstellung oder fehlende Grid-Fähigkeiten werden je Funktion offen dokumentiert, nicht pauschal ausgeschlossen.


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


## Verbindliche Add-on-/Plugin-Architektur – 09.10.2026

- `projectplan.md` Abschnitt 2.3 ergänzt.
- Offizielle Add-on-Plattform: `https://addons.opengenesislink.de/`.
- Plugin Manager, Registry Client, Paketinstallation, Signaturprüfung, Abhängigkeitsauflösung, Lifecycle, Updates und Capability-Prüfung gehören zum Viewer-Core.
- Vorgesehenes Paketformat: `.oglpkg`.
- Webseite und Viewer verwenden dieselbe Registry beziehungsweise denselben Release-Datenbestand.
- Native Plugins erhalten eine kontrollierte, versionierte ABI-Grenze.
- Vertrauensstufen: OGL Official, Verified Developer, Unverified/Developer Mode.
- Signatur- und Hashprüfung sind für offizielle Pakete verbindlich.
- Plugins können installiert, aktiviert, deaktiviert, aktualisiert und deinstalliert werden.
- Renderer werden als besonderer Modultyp vorbereitet; WorldModel/Scene bleiben rendererunabhängig.
- Ziel: Standard Renderer im Core, optionale Vulkan-/RTX-/Ray-Tracing-Renderer über klar definierte Renderer-Schnittstelle.
- Kommende größere Systeme müssen ab sofort auf Core-vs-Plugin-Grenzen geprüft werden.


## Modul-System-Reihenfolge – 09.10.2026

- Korrektur zur bisherigen Planung: Die minimale Module Runtime wird nicht erst später gebaut, sondern als nächster Architekturblock vor weiteren großen Viewer-Features.
- Jetzt: ModuleManager, versionierte Module API/ABI, Manifest Loader, lokales Laden, Lifecycle, Aktivieren/Deaktivieren, Dependencies, Capabilities, Plattformprüfung und persistenter Aktivierungszustand.
- Später: `addons.opengenesislink.de` Registry/API, Store-UI, Online-Download, automatische Updates, Produktions-Signaturservice und konkrete optionale Plugins wie RTX/AO/Voice.
- Ziel: OGL Viewer zuerst vollständig weiterbauen, aber ab jetzt auf einer echten modularen Grundlage.


## Plugin-Zielkatalog – 09.10.2026

- `projectplan.md` enthält jetzt einen verbindlichen Zielkatalog mit 38 geplanten Plugin-/Modulideen.
- Kategorien: Infrastruktur/Renderer, Avatar/Animation, Build/Material/Assets, Welt/Land/Admin, UI/Komfort/Kommunikation sowie Diagnose/Kreativ/Developer.
- Erste Runtime-Validierung: OGL Example Module -> Developer Tools -> Performance Monitor -> Vulkan Renderer -> RTX Renderer.
- Voice, Media, AO, RLVa und weitere tiefe Module folgen erst, wenn API/ABI, Lifecycle, Capabilities und Dependencies durch die ersten Module praktisch bestätigt sind.


## Outfit-System Core/Plugin-Trennung – 09.10.2026

- Outfit-Basis bleibt verbindlich im Core: Wearables, Attachments, aktuelles Outfit, Speichern/Laden/Wechseln, Inventar-Verknüpfung und Appearance-Synchronisation.
- `OGL Outfit Manager` wird Komfort-Plugin für Galerie/Liste, Kategorien, Tags, Favoriten, Suche, Notizen und lokale Zusatzmetadaten.
- Neues Plugin `OGL Outfit Preview` übernimmt Outfit-Screenshots, automatische Vorschau-Erstellung, Kamera-Presets und lokale Thumbnail-/Galerie-Verwaltung.
- Outfit-Screenshots werden standardmäßig lokal gespeichert; ein späterer Upload/Sync darf nur explizit aktiviert werden.
- Preview-Plugin nutzt ausschließlich freigegebene Kamera-/Frame-Capture-/Outfit-APIs und keine privaten Renderer-Strukturen.


## Text-only / reale Tageszeit / Wetter – 09.10.2026

- Text-only wird als Core-Betriebsmodus geplant, nicht als Plugin.
- Text-only nutzt denselben Login-/Protocol-/Social-/Inventar-Core, kann aber ohne aktive 3D-Weltdarstellung arbeiten.
- Reale Tag-/Nachtzeit wird als lokaler Environment-Core-Override geplant; Standard bleibt Region/OpenSim/EEP.
- Vorgesehen: Region/EEP, feste Uhrzeit, eigener Zyklus, reale Systemzeit sowie später geografisch berechnete Sonnenposition.
- `OGL Weather` neu im Plugin-Katalog: realistischer Regen, Schnee, Nebel, Sturm und Wind mit moderner GPU-basierter Darstellung.
- Spätere RTX-Integration: nasse Oberflächen, Reflexionen, Pfützen, volumetrische Effekte und verbesserte Wetterbeleuchtung.
- Lokale Wetter-/Zeit-Overrides verändern niemals serverseitige Regionseinstellungen.


## Next-Gen-Renderer-Anweisung erneut abgeglichen – 09.10.2026

- Vollständige aktuelle Next-Gen-Renderer-Anweisung gegen `projectplan.md` Abschnitt 2.2 geprüft.
- Bestehender Plan deckte den Großteil bereits ab; keine widersprüchliche Neuarchitektur erforderlich.
- Ergänzt: konkrete RT-Qualitätsprofile (Aus/Auto/Niedrig/Mittel/Hoch/Ultra) und mögliche RT-Reichweiten 50/100/150/256/512 m.
- Ergänzt: klare Abstimmung `OGLRender` mit der neuen Module Runtime; Renderer-Backends dürfen später Module sein, WorldModel/Protocol bleiben backendunabhängig.
- Klarstellung: eigener OGL Viewer, kein Firestorm-Fork; Firestorm/Second Life nur Referenz.


## Entwicklungszeit und aktuelle Reihenfolge – 09.10.2026

- Alter Phase-0-Abschnitt `Unmittelbar nächste Arbeit` im Projektplan ersetzt; er entsprach nicht mehr dem aktuellen Stand.
- Nächster Block bleibt Module Runtime / ModuleManager, danach Phase 8 und die weiteren alltagstauglichen Viewer-Systeme.
- Zeitplanung ausdrücklich nur als grober Rahmen: nutzbare Alpha ca. 3-6 Monate, brauchbare Beta ca. 6-9 Monate, weitgehender Firestorm-Ersatz ca. 9-15 Monate, vollständige langfristige OGL-Plattform ca. 12-24 Monate.
- Vulkan/RTX/Weather/Hydro und weitere High-End-/Plugin-Funktionen kommen nach dem alltagstauglichen Core, nicht vor ihm.

## Chat-Übergabe / neuer Chat – 09.10.2026

- Neue Datei `CHATUEBERGABE.md` ist ab sofort der primäre Wiedereinstiegspunkt.
- Alle projektbezogenen Markdown-Dateien wurden am 09.10.2026 auf den aktuellen Stand konsolidiert.
- Aktuelle Viewer-Version: `0.2.0-alpha.37`.
- Letzter vollständig geprüfter Codeblock: 19/19 Tests grün plus OpenGL Runtime-Smoke.
- Nächster Entwicklungsblock: Module Runtime / ModuleManager.
- Nach jedem größeren Block `info.md` und `CHATUEBERGABE.md` gemeinsam aktualisieren.

## GitHub-Handoff-Branch – 09.10.2026

- Der vollständige aktuelle lokale Stand wurde sicher auf GitHub als Branch `develop-current` veröffentlicht.
- Grund: GitHub-`develop` enthält 46 ältere Remote-only-Commits aus einer divergierenden Historie; ein Force-Push wurde bewusst vermieden.
- Neuer Chat soll für den aktuellen Viewer-Stand **`develop-current`** verwenden.
- Eine spätere bewusste Bereinigung/Reconciliation von `develop` kann separat erfolgen.


## Module Runtime / ModuleManager Basis – 09.10.2026 · 0.2.0-alpha.38

- Branch `develop-current`; bestehende OpenSim-/Render-Funktionalität unangetastet.
- Neues C++23-Librarytarget `NexVerse::Modules` mit nativer C-ABI v1
  (`ogl_module_get_api`, Host-Log-Callback, init/start/stop/shutdown).
- Lokaler JSON Manifest Loader mit Größenbegrenzungen, Pflichtfeldern, validierten
  IDs, Versionsvergleich, restriktivem Entrypoint und Duplicate-Key-Erkennung.
- ModuleManager erkennt lokale Module, prüft Viewer-/ABI-/Plattform- und
  Architekturkompatibilität, Capabilities, Abhängigkeiten und Zyklen.
- Aktivierung/Deaktivierung mit persistentem Text-State und atomarem Schreiben.
- Native Binary-Lader für Windows/macOS/Linux, Stop-/Shutdown-Reihenfolge und
  Rollback bei Batch-Startfehlern.
- Sicherheit: Unsigned immer blockiert, außer expliziter Developer Mode plus
  Capability-Freigabe; Signaturen/Hashes noch nicht verifizierbar und entsprechend
  blockiert. Kein Sicherheitsversprechen einer Sandbox.
- Neues CMake-Beispielmodul `OGL Example Module` für die ABI-Regressionsprüfung.
  Wird nicht im produktiven Viewer installiert oder automatisch aktiviert.
- `docs/MODULES.md` dokumentiert ABI-Vertrag, Installation, Einschränkungen.
- Debug-Build erfolgreich und 20/20 Tests grün, neuer `nexverse-module-runtime-tests`.
- OpenGL Runtime-Smoke erfolgreich (6s Laufzeit; Timeout 124 erwartet),
  NVIDIA RTX 4070 SUPER erkannt, NexVerse Welcome geladen.
- Nächster Block: Host-Developer-Tools/Diagnose als echtes Modul, danach
  Performance Monitor; dann priorisierte Viewer-Core-Phase 8.
- Nicht erledigt: produktive Signatur- und Hash-Verifikation, sichere
  Drittmodul-Isolation, Nutzer-Modul-UI, Add-on-Registry/Download,
  plattformübergreifende Runtime-Abnahme, Renderer-/WorldModel-Host-APIs.


## Diagnose-Plugins / Module ABI v2 – 09.10.2026 · 0.2.0-alpha.39

- ABI v1 unverändert; `abi_v2.h` ergänzt ein versioniertes, read-only
  `ogl_diagnostics_snapshot_v1`, Host-Callback und Main-Thread Frame-Callback.
- `ModuleManager` akzeptiert API v1/v2, veröffentlicht Diagnose-Snapshots und
  dispatcht Frames nur an aktive ABI-v2-Module. Fehler führen zum Stop aller
  Module im Batch; Abhängigkeiten und Developer-Mode-Sperren bleiben erhalten.
- Zwei separat gebaute native Module:
  `org.opengenesislink.developer-tools` und
  `org.opengenesislink.performance-monitor` (Dependency auf Developer Tools).
- Developer Tools protokolliert World-Ready, Frameindex, Draw Calls, Triangles,
  sichtbare Objekte; Performance Monitor loggt nach jeweils 300 gültigen Frames
  FPS, min/avg/max der geglätteten Framezeit und freien RAM/VRAM.
- Viewer-Loop veröffentlicht die tatsächlich erfassten Render-/RAM-Metriken;
  die Diagnoseschnittstelle verleiht **keine** Write-Rechte auf WorldModel/Render.
- Expliziter Start mit `--dev-modules` und lokal vorinstalliertem, aktiviertem
  Modul; Default bleibt aus. Modul-Lifecycle / Signaturen nicht produktionsreif.
- CMake Debug Build erfolgreich; 21/21 Tests grün, inklusive ABI-v1/v2,
  Default-Deny, Capability-Deny, Dependencies, Frame-Dispatch, Persistenz.
- Echter OpenGL-Starttest: `Developer Tools running`, `Performance Monitor
  running`, Frame-/Weltstatus-Log und Performance-Kennzahlen bestätigt;
  Welcome-Seite lädt normal. Timeout-Code 124 durch 7s Smoke erwartet.
- Nächste maßgebliche Arbeit: Phase 8 Prim-/Material-/Scene-Darstellung
  vervollständigen, danach Meshes/Sculpties/Partikel/Lichter.
- Offen: Modulverwaltungs-UI, signierter Vertrauenspfad, Prozessisolation,
  plattformübergreifende Abnahme, echte GPU Frame-Time-Metrik und integrierte
  Developer-UI-Panels. Kein RTX/Vulkan-Vorgriff.


## Phase 8 – Legacy Shiny-Fallback · 09.10.2026 · 0.2.0-alpha.40

- OpenGL-Fragmentshader berücksichtigt OpenSim-Shiny-Stufen 0–3 erstmals
  als begrenzte, oberflächenabhängige Glanzlichtapproximation.
- Default- und per-Face-Materialien führen Shiny an den Shader weiter.
- Fullbright-Flächen erhalten keinen zusätzlichen Glanzlicht-Effekt;
  inaktive Objekt-/Terrainpässe setzen die Uniform auf 0 zurück.
- Bumpiness-Werte werden weiterhin per Material transportiert und begrenzt,
  aber noch nicht als Normal-/Bump-Map gerendert.
- Dies ist ein Legacy-Compatibility-Fallback, kein vollwertiges PBR.
- Build und 21/21 CTest grün. Der nächste Phase-8-Schritt behandelt
  Material-ID-/RenderMaterial-Auswertung und weitere Facegruppen.


## Phase 8 – Alpha-Passes und Material-IDs · 09.10.2026 · 0.2.0-alpha.41

- Object-Rendering in zwei Durchläufe getrennt: zuerst deckende Prim-Faces
  mit Depth-Write, anschließend transparente Faces mit Alpha-Blending und
  deaktiviertem Depth-Write.
- Transparente Objekte werden mit stabiler Reihenfolge nach Abstand zum
  Kamerastandpunkt (fern nach nah) gezeichnet; das WorldModel bleibt unverändert.
- RGBA8-Texturen werden beim GPU-Upload einmalig auf Alpha < 255 geprüft.
  Vollständig deckende Texturen bleiben nun im Opaque-Pass statt pauschal
  als transparent gerendert zu werden.
- Vollständig transparente Fragmente werden im Legacy-GLSL verworfen.
- Die vom TextureEntry gelieferten Default-Material-ID und
  RenderMaterial-ID gehen jetzt auch beim Scene-Object-Transfer nicht verloren;
  die bereits vorhandene Vererbung/Override-Auswertung erhält einen
  erweiterten Regressionstest.
- Neues `NexVerse::Render`-CPU-Modul `material_pass` für testbare
  Alpha-Klassifikation und stabile Sortierung; neuer CTest.
- CMake Debug-Build, 22/22 Tests und OpenGL Runtime-Smoke erfolgreich.
  Welcome/GL initialisieren; ein eingeloggter In-World-Echttest steht aus.
- Grenzen: kein PBR-/Legacy-Material-Asset-Fetch, keine Normalmaps,
  keine exakte Sortierung einzelner transparenter Dreiecke/Faces,
  kein Order-Independent Transparency. Material-/RenderMaterial-UUIDs
  werden bewahrt, aber **noch nicht** als Materials aufgelöst.
- Als Nächstes: überprüfte Material-CAP/Asset-Resolver-Anbindung und
  vollständigeres Face-/Materialhandling, dann weitere Phase-8-Objekttypen.


## Phase 8 – echter Legacy RenderMaterials-CAPS-Pfad · 09.10.2026 · 0.2.0-alpha.42

- Verifiziert an OpenSimulator `MaterialsModule.cs`: die `RenderMaterials`-
  Capability liefert als GET eine `Zipped`-Binary in LLSD-XML;
  darin liegt eine zlib-komprimierte LLSD-Binary-Array mit
  `ID` (16-Byte UUID) und `Material` (LLSD-Map).
- Eigener Protocol-Parser und `RenderMaterialsService` (asynchrones GET
  nach CAPS Discovery), mit begrenzten komprimierten/entpackten Größen,
  8192 Max-Entries, UUID- und Integer-Prüfung, Ablehnung ungültiger Daten.
- Legacy `DiffuseAlphaMode`/ `AlphaMaskCutoff` werden per Material-ID
  an Default-/per-Face-Renderdaten gebunden. Material-Cache-Update erzwingt
  Scene-Neuübertragung ohne Änderung des serverautoritativ gespeicherten Models.
- OpenGL-Shader: Alpha-Mode 0/1/2/3, maskiertes `discard`, Opaque-
  Depthpass für Mask-Faces ohne Tint-Alpha, emissives leichtes Fallback.
- Normal-/Specular-Map-UUIDs werden eingelesen, aber nicht als Texturen
  geladen. Moderne `RenderMaterialID`/PBR werden nicht vorgetäuscht.
- 23/23 CTest erfolgreich; Debug-Build und OpenGL-Runtime-Smoke erfolgreich
  (Welcome geladen, Shader initialisiert). In-World-Sichtprüfung offen.
- Verbleibend: GET-Snapshot ist nicht live aktualisiert; POST-Batch-Abfragen
  bei neuen MaterialIDs, echte Normal-/Specular-Maps, PBR/glTF-Material-
  Assets, Face-/Triangle-Transparenzsortierung und In-World-Tests.
- Siehe `docs/MATERIALS.md` für genaue Grenzen und Schnittstellen.


## Phase 8 – gezielte Legacy-Material-Nachladung · 09.10.2026 · 0.2.0-alpha.43

- An der OpenSimulator-`RenderMaterialsPostCap`-Referenz geprüft:
  POST mit LLSD-XML-`Zipped`, zlib und binärem LLSD-Array aus
  16-Byte-Material-UUIDs; Response verwendet das bestehende
  `ID`/`Material`-Format.
- `RenderMaterialsService::fetch_ids()` und
  `build_render_materials_request()`: höchstens 32 Material-IDs
  je POST-Batch, UUID-Prüfung, Deduplikation, Begrenzungen des
  komprimierten Payloads und ausschließlich angeforderte IDs
  aus der Antwort übernehmen.
- Viewer sammelt Material-IDs aus Default- und per-Face-Renderdaten,
  überspringt Null- und ungültige IDs, wartet auf den initialen
  GET-Snapshot, startet dann asynchrone, sequenzielle POST-Batches.
- Ein erfolgreicher Materialwechsel aktualisiert den lokalen Cache
  und invalidiert die gerenderte Scene-Revision; unveränderte
  Einträge lösen keinen unnötigen Scene-Upload aus.
- Wiederholungsversuche für unbekannte IDs sind auf maximal drei
  Versuche mit zwei Minuten Abstand begrenzt. Ein bekanntes Material
  kann bei späterem Scene-Update nach Ablauf der Schutzfrist
  erneut angefragt werden.
- 23/23 CTest erfolgreich, einschließlich neuer POST-Request-
  Validierung (Payload-Struktur, Deduplikation, URL-/Größenfilter,
  Antwort-Whitelist).
- Ausstehend: Region-/Teleport-übergreifender Material-Lifecycle,
  kontinuierliche serverseitige Materialänderungen ohne Scene-Update,
  echte Normal-/Specular-Texturen und glTF/PBR.

- Laufzeit-Smoke 5s bestätigt OpenGL 3.3, RTX 4070 SUPER, NexVerse-UI und
  geladenen Welcome-Hintergrund; `timeout`-Exit 124 erwartet.


## Phase 8 – Legacy Normal/Specular Mapping · 09.10.2026 · 0.2.0-alpha.44

- OpenSimulator `SOPMaterial.cs` als Protokollreferenz geprüft:
  `NormMap`/`SpecMap`, `Norm/SpecOffsetX/Y`, `RepeatX/Y`,
  `Rotation` werden per 1/10000 skaliert; dazu `SpecColor`,
  `SpecExp` und `EnvIntensity`.
- Die Material-CAPS-Response dekodiert nun diese typisierten Werte
  mit Grenzen und Ablehnung fehlerhafter numerischer Felder.
- Das Application-/Scene-Mapping übernimmt Normal-/Specular-UUIDs
  sowie beide unabhängigen UV-Transformationen. Vorhandene
  GetTexture-PNG-Downloads werden für diese Texturen wiederverwendet,
  ohne einen zweiten Asset-Transport einzuführen.
- OGLRender bindet geladene Maps pro Face auf GL-Texture-Units 5/6,
  verwendet getrennte Repeat/Offset/Rotation-Uniforms, approximiert
  Normalmapping über UV-/Oberflächen-Derivate (TBN-Basis) und ein
  einfaches Legacy-Specular-Highlight aus Map/SpecColor/SpecExp.
- Fehlen Asset oder Materialdaten, bleibt der Legacy-Shiny-Fallback
  erhalten; Fullbright bleibt von zusätzlichen Lichteffekten ausgenommen.
- Kein vollständiges PBR, keine echte Umgebungsspiegelung, keine
  garantierte Texture-/Tangent-Fidelity für alle Primformen.
- CMake Debug-Build erfolgreich, 23/23 CTest erfolgreich; OpenGL-3.3-
  Laufzeitstart auf RTX 4070 SUPER einschließlich Welcome geladen.
  Ein eingeloggter In-World-/Material-Vergleich bleibt offen.
- Als Nächstes: Material-Asset-Caching/Invalidierung beim Region- und
  Teleportwechsel, weitere Renderer-Primitiven, visuelle In-World-Tests.


## Phase 8 – regionsgebundene CAPS-/Material-Caches · 09.10.2026 · 0.2.0-alpha.45

- Das Materialsystem verwendet eine explizite Region-Identität aus
  WorldModel-Handle, Generation und Seed-Capability.
- Bei Login, TeleportFinish und CrossedRegion (inkl. gleicher Handle
  mit neuer Generation) werden Legacy-Material-Cache, offene Material-
  ID-Listen, Retry-/Cooldown-Daten und Texture-Download-Metadaten
  invalidiert; die Szene wird erneut aus dem WorldModel aufgebaut.
- Material- und GetTexture-Capabilities werden anhand der Seed-Capability
  **der jeweils aktuellen Region** separat asynchron neu entdeckt.
  Die bisherige Login-EventQueue bleibt vorerst ein separater Pfad.
- Alle asynchronen Material-GET-/POST- sowie Terrain-/Objekt-Textur-
  Anfragen sind mit einer Region-Epoche versehen. Veraltete Ergebnisse
  werden konsumiert, aber nicht mehr in die neue Region übernommen;
  laufende std::async-Futures werden nicht beim Übergang überschrieben.
- Neues Header-Modul `RegionMaterialScope` und Regressionstests für
  identische Region, Generationserhöhung, Regionshandle- und Seed-Wechsel,
  insbesondere die Zurückweisung von Antworten mit veralteter Epoche.
- **Abgrenzung:** Der vollständige Teleport-/Circuit-Reconnect,
  die Neuanmeldung der gesamten EventQueue sowie eine vollständige
  GPU-Texture-Eviction-Strategie gehören zu weiteren Entwicklungsblöcken.
  Dieser Block isoliert den **Material- und Asset-Abruf** bei Regionswechseln.
- Abnahme: Debug-Build erfolgreich, 23/23 CTest gruen,
  5-Sekunden-OpenGL-Smoke inklusive NexVerse-UI und Welcome erfolgreich.
  Ein eingeloggter In-World-Teleport-Test steht noch aus.


## Phase 8 – zuverlässiger OpenSim-GetTexture-Transport · 10.10.2026 · 0.2.0-alpha.46

**Anlass (Live-Login NexVerse 10.10.2026, 00:13 bis 00:18):**
- Login und LLUDP UDP LIVE, 17 CAPS-Endpunkte, Handshake,
  WorldModel READY, RenderMaterials GET 46 und POST, Terrain/Prim-
  Geometrie nachweislich aus der echten OpenSim-Region empfangen.
- Objekttexturen: 300 Fehlversuche, **0 Erfolgsfälle**. Auch die
  Terrain-Detailtexturen fielen auf Farb-Fallback zurück.
- Die frühere Funktion `TextureAssetLoader::fetch_png` forderte
  ausschließlich `format=png` an. OpenSimulator verwendet nativ
  JPEG2000 (`x-j2c`); eine serverseitige PNG-Konvertierung ist
  nicht immer verfügbar. Die genaue Ursache wurde zuvor nicht geloggt.

**Implementiert:**
- `openjpeg` als vcpkg-Abhängigkeit + explizites `libpng`
  (Linux/Windows/macOS-abhängige Abhängigkeiten vereinheitlicht).
- `TextureAssetLoader::fetch_rgba` fragt bevorzugt PNG ab;
  bei HTTP-/Decoderfehler einmalige Rückfall-Anfrage mit
  `format=x-j2c` für dieselbe UUID. Keine Wiederholung pro Frame;
  bestehender Cache für fehlgeschlagene UUIDs bleibt erhalten.
- Memory-Stream-basierter OpenJPEG-Decoder für J2C-/JP2-Daten,
  unterstützt RGB, RGBA, Graustufen, Alpha, 1 bis 16 Bit und
  einfache Chroma-Subsampling-/YCC-Fälle.
- Begrenzt komprimierte Downloads auf 32 MiB, Abmessungen
  auf 4096x4096, RGBA-Ausgabe auf 64 MiB und verifiziert Metadaten
  vor und nach dem Dekodieren.
- Material- und Terrain-Logs melden jetzt pro UUID die konkreten
  Ursachen von PNG- und J2C-Fehlern; erfolgreiche J2C-Rückfälle
  sind durch die Erfolgsmeldung erkennbar. CAPS-Token/URLs werden
  nicht in Warnmeldungen ausgegeben.
- Erweiterte Tests mit echten 16x16 JPEG2000 J2C/JP2/RGBA-Fixtures,
  fehlerhaftem PNG, 404-HTTP-Fallback und defekten J2C-Payloads.

**Grenzen und offene Abnahme:**
- CMake Debug-Build, 23/23 CTest und 6-Sekunden-OpenGL-3.3-Smoke
  mit RTX 4070 SUPER, NexVerse-UI und Welcome erfolgreich.
  End-to-End-Nachweis mit authentifiziertem NexVerse-Grid und
  sichtbaren Texturen nach Neustart der Anwendung weiterhin offen.
- Die wiederkehrenden CAPS-EventQueue-Poll-Fehler gehören
  in den nächsten separaten Protokollblock, nicht zur Texturdekodierung.
- Progressive JPEG2000-LOD, Asset-Priorisierung, Retry-/LRU-
  Strategie, harte GPU-Speicherverwaltung und modernes PBR folgen.


## CAPS – EventQueue-Lifecycle und Recovery · 10.10.2026 · 0.2.0-alpha.47

**Live-Befund aus 0.2.0-alpha.46:**
- Echtes Login, EventQueue-Live-Batch, 287+ geladene Objekttexturen
  und WorldModel READY. Anschließend CAPS-Poll-Warnungen im
  Abstand von ~36–40 Sekunden bei einem früheren HTTP-Timeout von
  **35 Sekunden**.
- OpenSimulator setzt den EventQueue-Keepalive auf 60 Sekunden
  und hält Polls etwa 58 Sekunden offen, bevor eine leere Queue
  mit HTTP **502 Bad Gateway** quittiert wird. Dieser Status ist
  eine *reguläre* Leermeldung, kein verlorenes Ereignis.
  Referenz: `OpenSim/Region/ClientStack/Linden/Caps/EventQueue/EventQueueGetModule.cs`
  (`VIEWERKEEPALIVE=(60-2)*1000`, `NoEvents` gibt 502 zurück).

**Implementiert:**
- Spezifischer EventQueue-HTTP-Total-Timeout: **80 Sekunden**,
  Connect-Timeout weiterhin 8 Sekunden.
- Unterscheidung:
  - HTTP 200 + gültiges LLSD → Batch und Acknowledgement.
  - HTTP 502 → erwartetes Idle; keine Warnung, Ack erhalten,
    Poll erneut senden; kleines 150-ms-Hotloop-Schutzintervall.
  - HTTP 404 → Endpoint abgelaufen; sofort Seed-CAPS neu abrufen.
  - Andere HTTP-/Transport-/LLSD-Fehler → Diagnose mit HTTP-
    Status, Fehlermeldung, Retry-Zähler und Ack-Zustand.
- Gesunde Batch-/Idle-Antworten resetten die Fehlerzählung.
  Echte Fehler erhalten begrenzte exponentielle Backoffs
  von 1, 2, 4, 8 bis maximal 12 Sekunden.
- Bei jeder dritten echten Störung wird `EventQueueGet`
  aus dem ursprünglichen Login-Seed neu angefragt.
  Ist die Capability-URI unverändert, bleibt der Ack erhalten.
  Bei geändertem Endpoint wird er zurückgesetzt, weil es
  eine andere Queue ist. Werte und Tokens werden nicht geloggt.
- CAPS-Status: `CAPS LIVE`, `CAPS IDLE`, `CAPS RETRY`,
  `CAPS DEGRADED` mit Wiederherstellungsmeldungen.
- Retry-Pausen sind in 100-ms-Schritten stoppbar.

**Testabnahme:**
- CMake Debug-Build erfolgreich, 23/23 automatisierte CTests
  inkl. HTTP 502, 404, 503, Timeout, fehlerhafter LLSD-Antwort,
  Ack-Erhalt, Wiederverbindungs-/Backoff-Entscheidung.
- OpenGL-3.3-Smoke auf NVIDIA RTX 4070 SUPER mit NexVerse-UI
  und Welcome erfolgreich; absichtliches Timeout des Smoke-
  Tests ist kein Absturz.
- **Neuer echter Alpha.47-EventQueue-Langzeittest nach Login
  ausstehend.** Das beobachtete alte 35-Sekunden-Intervall
  sollte nicht wieder als Fehler auftreten.

**Noch offen:**
- Vollständiger Teleport/Regionsübergang inklusive UDP-Circuit-
  Wechsel, Stopp alter EventQueue und Start neuer Region-Queue.
- Langzeit-Metriken, Error-Sampling, Test verschiedener Simulator-
  und Proxy-Laufzeitkonfigurationen, Seed-Neuvergabe bei
  abgelaufener Region, dynamischer Reconnect auf neuer Region.
- Diese Änderung ersetzt *nicht* den bestehenden LLUDP-Circuit.

Details: `docs/CAPS.md`.


## Phase 8/9 – LLSD-Logs + erste OpenSim-Meshobjekte · 10.10.2026 · 0.2.0-alpha.48

**Auslöser und Live-Befund:**
- Alpha.47 lädt 293 Objekttexturen und 4 Terraintexturen.
  Die EventQueue behandelt einen regulären HTTP-502-Leer-Poll
  korrekt; ein initialer HTTP200/LLSD-Integer-Parsefehler erholte
  sich beim nächsten Poll. 9 Texture-Asset-Fehler bleiben:
  6x HTTP 404 (serverseitig kein Asset geliefert), 2x unbekanntes
  Bildformat, 1x defekte JPEG2000-Tile-Daten. Serverassets
  können nicht ohne Assetservice-Zugriff clientseitig ersetzt werden.
- Meshobjekte waren vorher vollständig unterrepräsentiert:
  ObjectUpdate ExtraParams wurden verworfen, kein Mesh-Asset
  abgerufen, keine Mesh-Geometrie an OpenGL übergeben.

**Implementiert (Basis):**
- LLSD XML-Integer: Empty/Whitespace als 0, 64-Bit-Werte
  mit strikter Bereichsprüfung, Pluszeichen möglich; Binary
  LLSD bleibt standardkonform 32 Bit.
- ObjectUpdate-SculptExtraParams: Parametertyp 0x30 mit 17 Byte
  UUID + Sculpt-Type; Mesh-Typ `type & 7 == 5`.
  Die Mesh-ID wird ins WorldModel/SceneObject übergeben.
- GetMesh/GetMesh2 wird über regionsgebundene Seed-CAPS
  entdeckt und per `mesh_id`-Query asynchron heruntergeladen.
- Binärer LLSD-Header mit Blockoffset/Blockgröße;
  zlib/gzip-LOD-Daten; beschränkte Gesamt-/Einzelgröße;
  Position/UV/Index-Dekodierung von 16-Bit-Werten für die
  High-/Medium-LOD, Face-Ranges für Texturen, geometrische
  Dreiecksnormalen, OpenGL-Buffers. Kein lokaler OBJ/DAE-Import.
- CPU/GPU-Asset-Daten getrennt: Decoder off-thread, OpenGL
  Upload ausschließlich auf Render-Thread.
- Zuordnung von Mesh-Asset-UUIDs zu geladenen Mesh-Prims,
  Mesh-Fallback bis Download abgeschlossen, Caches auf
  256 Asset-IDs und regionsepochengeschützte Futures begrenzt.

**Abnahme und offene Grenzen:**
- Debug-Build, 24/24 CTests einschl. synthetischem OpenSim-Mesh,
  malformed LLSD und Invalid-Asset-Tests.
- Ein echter Login mit visueller Mesh-Kontrolle auf NexVerse
  ist **nicht** durch Offline-Tests ersetzt. GetMesh-Antworten,
  Shader-/Face-UVs und Performance müssen in der Welt geprüft werden.
- Compressed-ObjectUpdate initiale Mesh-Parameter noch nicht
  erfasst; bisherige IDs werden bei Partial-Updates bewahrt.
- Rigging/Skinned Mesh/Animesh, Sculpt-Texture-Parametrisierung,
  Mesh-LOD-Distanzstrategie, Direct ViewerAsset, optimierter
  Speicher-LRU, vollständige UV/Normal/Material-Fidelity und
  Teleport-Circuit-Wechsel bleiben eigenständige Blöcke.

Weitere Details: `docs/MESH.md`.


## Phase 8/9 – LLSD-Datum / Mesh-Fehlerbehandlung · 10.10.2026 · alpha.49

**Verbindlicher Ausgangsbefund aus alpha.48:**
- 232 echte Mesh-Uploads an OpenGL, 216 fehlgeschlagen.
- Davon 208 identische Headerfehler `Unknown LLSD binary marker`,
  sieben ungültige Submeshes, ein Mesh HTTP404.
- Objekttexturen 287 erfolgreich/9 fehlend; Terraintexturen 4/4.
- EventQueue HTTP502 normal, ein HTTP200-LLSD-Parsefehler
  `Unsupported LLSD XML type: date`, danach Recovery.

**Geliefert:**
- LLSD XML `date` und binäres `d` im Parser/Serializer.
- Bei unbekanntem Binärtyp Hex-Marker und Byteposition;
  bei defektem Mesh-Header 8 erste Asset-Bytes in Hex und
  Gesamtzahl der Bytes als sichere, begrenzte Diagnostik.
- Default UV [0,1] bei fehlender optionaler TexCoord0Domain.
- Einzelne ungültige Submesh-Faces können übersprungen werden
  (Rollback partieller Indizes/Vertices; gültige Faces bleiben).
- GetMesh2 vor GetMesh, alternativer Mesh-CAP-Abruf bei Fehlschlag,
  begrenzt auf einmalige Wiederholung; epochengeschützter Cache.
- Tests mit Datumsfeldern, optionalen UV-Domains,
  alternativen Capabilities und teilweise beschädigten Mesh-Faces
  erweitern die 24 vorhandenen CTests.

**Abnahme und Prioritäten:**
- Offline-/Build-/GUI-Abnahme separat dokumentieren.
- Kein echter Server-Mesh-Retest dieser Version erfolgt: **208
  Headerfehler nicht als behoben ausgeben**. Nach Login die neuen
  `prefix_hex`-`at offset`-Fehler auslesen und deren Ursache
  anhand des tatsächlichen Binärformats angehen.
- Dann `ObjectUpdateCompressed` Mesh-ExtraParams, Mesh-LOD-
  Streaming, Skinned/Animesh, Avatar und vollständige Texturen
  separat bearbeiten.

Details `docs/MESH.md` und `docs/CAPS.md`.


## alpha.50 – Compressed-ObjectUpdate und Mesh-Prims · 10.10.2026

**Status:** Beim Start dieses Blocks gab es kein aktuelles
/tmp/ogl49-live.log auf der Linux-Maschine. Die 208 vorherigen
Mesh-Headerfehler sind deshalb weiterhin unbestätigt.

**Implementiert:**
- Vollständige ObjectUpdateCompressed-Pakete mit CompressedFlags,
  Owner UUID, optionalen Feldern AngularVelocity, Parent,
  ScratchPad, Text, MediaURL, Particles, Sound und NameValues.
- SculptExtraParams Typ 0x30 nach den optionalen Feldern;
  Mesh-UUID und Sculpt-Typ werden ins WorldModel übernommen.
- Vollständige 23-Byte-Prim-Form und TextureEntry-Daten mit
  32-Bit-Längenfeld und Face-Textur sowie Skalierung/Parent-ID.
- Bounds-Checks und Ablehnung abgeschnittener Compressed-Daten.
- Test: synthetisches vollständiges komprimiertes Mesh-Prim
  samt Scale/Parent/TextureEntry, plus Truncation-Test.
- Formatbeleg: OpenMetaverse ObjectManager.cs,
  ObjectUpdateCompressedHandler:
  https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/ObjectManager.cs

**Noch offen:** echter NexVerse-Live-Test; 208 vorherige
Headerfehler über prefix_hex / Byteoffset analysieren;
Mesh-LOD, Rigging/Animesh, Teleport-Regionswechsel und
fehlende Servertexturen.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl50-live.log


## 0.2.0-alpha.51 – Dynamischer GPU-Mesh-Cache · 10.10.2026

**Ausgangsbefund alpha.50:** Der Live-Test meldete 256 erfolgreiche
Mesh-Uploads, 2 vollständig fehlerhafte Mesh-LODs,
3 teilweise gerettete Submeshes. Durch die Bedingung
loaded_ids.size() < 256 wurden danach keine weiteren
Meshes angefordert. Das ist ein Cache-Limit und nicht
die tatsächliche Anzahl von Meshes der NexVerse-Region.

**Implementiert:**
- Neue pure Mesh-Residency-Planung mit nach Entfernung zum
  Avatar priorisierten Mesh-UUIDs und Duplikatentfernung.
- GPU-Asset-Budget 256. Werden durch Ortswechsel neue Meshes
  relevanter, verdrängen sie zuvor geladene entfernte Meshes.
- 24 Meter Prioritätsbonus für residente Meshes vermeidet
  Cache-Churn bei geringfügiger Bewegung.
- Plan-Neuberechnung bei SceneObject-Änderung oder nach
  maximal 750 ms; GPU-Eviction pro Zyklus auf acht begrenzt.
- Renderer: set_mesh_asset liefert Upload-Erfolg, dazu
  remove_mesh_asset und clear_mesh_assets für explizite
  GL-VAO/VBO/EBO-Freigabe auf dem Renderer-Thread.
- Bei Region-Epochenwechsel werden ältere GPU-Meshes gelöscht.
  Stale Mesh-HTTP-Ergebnisse und nicht mehr priorisierte
  Ergebnisse werden nicht hochgeladen.
- Unbekannte/doppelte UUIDs und nicht-finite Positionen
  werden bei der Auswahl ignoriert; Gleichstände deterministisch.
- Zusätzlicher Testsatz für Bewegung, Eviction, Hysterese,
  Deduplikation, NaN, Nullbudget und 320 Meshes; 25/25 CTests.

**Offen und begrenzt:**
- Echte In-World-Abnahme von alpha.51 bei Ortswechsel.
- Noch keine Frustum-/Occlusion-Priorisierung.
- Mesh-Kindposition nur angenähert mit Parent-Position
  plus lokalem Offset (ohne Rotation).
- Keine persistenten Mesh-Disk-/CPU-LOD-Caches;
  beim Zurückkommen können Downloads erneut erfolgen.
- Zwei vollständig ungültige Mesh-LODs, beschädigte oder
  HTTP404-Textur-Assets; Rigging/Animesh und Teleport
  bleiben eigenständige Entwicklungsblöcke.

Live-Test: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl51-live.log


## alpha.52 – Avatar sichtbar machen und Bewegung · 10.10.2026

**Auslöser:** Der Screenshot der verbundenen Welt zeigt
Terrain/Meshes, aber keinen eigenen Avatar und bislang
keine nachvollziehbare Bewegung. WORLD UDP LIVE beweist
weder den Avatar-Renderpfad noch die Bewegungsfähigkeit.

**Implementiert:**
- OpenGL-Avatar-Proxy-Pfad mit sieben Basis-Meshes für
  Kopf, Torso, Arme, Beine, für eigenen Avatar und bis
  zu 63 weitere Avatar-Datenpunkte. Die Darstellung wird
  pro Frame anhand der Agentposition aktualisiert.
- Die Proxies sind absichtlich keine echten OpenSim-Avatare:
  keine Skin-Bakes, Avatar-Meshes, Skeletons,
  Wearables, Haare, Attachments oder Animationen.
- Neues Keyboard-Control-Modul mit W/S/A/D, linker/rechter
  Pfeiltaste zur Rotation, Shift für schnelleres Bewegen,
  Space für Sprung, C für Abwärtsbewegung, F Flug
  per Toggle. Die AgentUpdate-Pakete enthalten dabei
  korrekte Körper-/Kopf-Quaternionen und Kameraachsen.
- Self-Avatar-LocalID wird aus ObjectUpdate bzw.
  ObjectUpdateCompressed gewonnen, mit dem RegionHandle
  gekoppelt und bei ImprovedTerseObjectUpdate
  zur Aktualisierung der serverbestätigten eigenen
  Avatarposition im WorldModel verwendet.
- Bei Steuerungsänderungen und bestätigten Simulator-
  Positionsupdates werden diagnostische Logzeilen geschrieben.
  Fokusverlust sendet keine gedrückten Bewegungs-Keys.
- Nach erfolgreichem Login wird das Login-Eingabefeld
  nicht mehr durch die Bewegungssteuerung verändert.
- Regressionstests für Steuerbits, Quaternion, Fly-Toggle
  und Steuerungsreset; 26/26 CTests grün.

**Noch offen und nicht als fertig behaupten:**
- Echte NexVerse-In-World-Bewegungsabnahme, insbesondere
  muss die Self-Avatar-LocalID und die Terse-Positions-
  Bestätigung mit neuer Version beobachtet werden.
- Richtige Firestorm-Avatar-Appearance (Skeleton,
  Body Shape, Skin-Bakes, Wearables, Attachments,
  Animationen) sowie Mouselook, Camera-Orbit/Zoom,
  Teleport/Regionswechsel, Collision/Reconciliation.
- Die einfachen Avatar-Proxies sind nur eine
  Übergangsdarstellung bis zu Phase-10-Appearance.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl52-live.log


## 0.2.0-alpha.53 – Phase 8: OpenSim-Point-Lights · 10.10.2026

**Verbindliche Nutzerentscheidung vom 10.10.2026:**
Fortsetzung der Entwicklung entlang der Reihenfolge des
projectplan.md. Die zuletzt untersuchten Avatar-/Bewegungs-
und übrigen Rendering-Asset-Fehler werden **zunächst
nicht behoben**, sondern dokumentiert und für eine
spätere gemeinsame Fehlerbehebungsphase vorgemerkt.
Keine implizite Behauptung, dass die Probleme gelöst seien.

**Jetzt neu in Phase 8 implementiert:**
- Point-Light ExtraParams Blocktyp 0x20, 16 Byte:
  4x RGBA-UNORM8 (Alpha = Intensität), dazu
  little-endian float32 Radius, Cutoff und Falloff.
  Implementierung orientiert sich an libopenmetaverse,
  Primitive.LightData.
- Sichere Bounds/Finite-/Radius-Prüfungen;
  Daten werden in vollständigem ObjectUpdate und
  ObjectUpdateCompressed nach SculptExtraParams separat
  dekodiert. Bei Partial-Updates behalten vorhandene
  Objekte ihre bereits bekannte Lichtdefinition.
- Eigenes Feld im WorldModel für lokale Lichtquellen.
- Die acht nächsten Punktlichter werden aus
  den Objektpositionen pro Frame ausgewählt. Child-Prim-
  Position wird vorläufig mit Parent+Offset angenähert.
- Legacy OGLRender: Weltposition aus uModel-Transform,
  acht Point-Light-Uniforms und diffuse Beleuchtung
  von nicht-fullbright Objekt-/Meshflächen nach
  Abstand, Radius, Intensität und Falloff.
- Terrain/Avatar werden in dieser ersten Version
  noch nicht von den lokalen Punktlichtern beleuchtet.
  Keine dynamischen Schatten, keine Projektoren,
  keine PBR-Energiekonservierung.
- Kompletter Debug-Build, 26/26 CTests bestanden,
  GL-3.3-Smoke auf RTX 4070 SUPER erfolgreich.
  **In-World-Sichtabnahme mit echter Lichtquelle
  steht noch aus**.

**Bekannte Fehler für spätere Fehlerphase (unverändert):**
- Eigene Avatar-LocalID wurde in alpha.52 Live-Log nicht
  gebunden, daher keine serverbestätigten
  Self-Avatar-Positionsupdates / keine verifizierte
  Benutzbarkeit der WASD-Bewegung.
- Eigenes Erscheinungsbild ist bisher nur Proxy,
  echtes Skeleton/Bakes/Wearables fehlen.
- Alpha.52: 252 geladene, vier fehlgeschlagene Meshes;
  neun fehlende/beschädigte Objekttexturen.
- Keine aktuelle Bestätigung einer GPU-Cache-Eviction
  bei tatsächlichem Ortswechsel.

**Fortsetzung Phase 8:** Partikel-Basis, weitere
Lichttypen (Projektor/Material-Licht), Render-Performance
und anschließend Phase 9 bzw. weitere Funktionssysteme.
Maßgeblich bleibt der Plan, nicht die Bugliste.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl53-live.log


## 0.2.0-alpha.54 – Phase 8: OpenSim ParticleSystem/PSBlock · 10.10.2026

**Nutzerentscheidung:** Nach projectplan.md die Funktionalität
weiterentwickeln und bekannte Fehler erst später beheben.
Die Priorität bleibt Phase 8 vor neuen großen Folgephasen.

**Geliefert:**
- OpenSim ObjectUpdate PSBlock (Variable1 nach MediaURL) und
  ObjectUpdateCompressed Partikelblock (CompressedFlag 0x08,
  86 Bytes) werden abgegriffen und mit strikter Größenprüfung
  dekodiert; ungültige/fehlende Blöcke blockieren den Prim nicht.
- Protokollkompatibles Legacy-ParticleSystem mit 68-Byte-
  Systemteil und 18-Byte-Partikeldaten (86 Byte) sowie
  erweitertem Format 94–98 Byte mit Größenfeldern,
  Glow-/Blend-Metadaten; Bytes nicht aus C# kopiert, sondern
  eigenständiger C++23-Parser.
- Emissionsparameter: Source/Particle-Lifetime, Burst-Rate,
  BurstCount, BurstRadius, Min/Max-Speed, Beschleunigung,
  Start/End-Farbe, Scale, Textur-UUID (nur Referenz)
  sowie Pattern und Flags.
- Partikelsystem liegt im WorldModel der jeweiligen Prim;
  Terse-/Legacy-Partial-Updates bewahren den bekannten Wert,
  KillObject entfernt den Prim mitsamt Emitter.
- Render-Integration: maximal 32 nahe Emitter in ca. 192m
  Radius, maximal 768 Partikel in einem Drawcall; eigener
  OpenGL-3.3-Shader und Streaming-VBO. Weiche runde Sprites
  mit alphablendfähigem Fade, Z-Test ohne Z-Writes.
  Die Positionen sind deterministisch generierte Grund-
  Partikelpositionen mit Ballistik/Beschleunigung.
- Unterstützte Basispfade: Drop, Explode und grob nach
  oben gerichtete Angle-/Cone-Patterns; keine native
  Target-/Ribbon-/Beam-Logik, Wind, Bounce, Rotationsflags,
  korrekte Emitter-Restart-Events, Textur-Sampling oder
  diskrete persistente Partikel-Physik.
- Smoke-Test: Linux 3440x1440, NVIDIA RTX 4070 SUPER,
  GL 3.3, Welcome-UI lädt, kein Shaderfehler gemeldet.
- CMake-Debug Build und 26/26 CTests; Parser-Regressions-
  tests für Legacy und Extended ParticleSystem,
  invaliden Datenblock, ObjectUpdate und
  ObjectUpdateCompressed mit aktivem PSBlock.
- **Die erste echte In-World-Sichtprüfung ist offen.**
  Simulierte Sprites sind kein fertiger Firestorm-
  Partikelrenderer. Keine Änderungen an der
  bekannten Avatar-/Bewegungssynchronisationslücke.

**Spätere Ausbaustufen im Plan:** Partikeltexturen über
AssetCache, Ziel-Partikel, Ribbon/Beam, Wind/Bounce,
lokal zustandsbehaftete LSL-Emission, Partikel-LOD
und GPU-Instancing. Danach Phase 8 weitere
Material-/Licht-Effekte und Phase 9 Asset-Streaming.

Live-Start: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl54-live.log


## 0.2.0-alpha.55 – Phase 8: Partikeltexturen / GetTexture · 10.10.2026

**Verbindlicher Kurs:** Umsetzung der Funktionen aus projectplan.md
fortsetzen. Die bekannten Avatar-/WASD-/Mesh-/Texturfehler bleiben
auf Nutzerwunsch für die spätere Fehlerphase zurückgestellt.

**Neu:**
- Die in OpenSim ParticleSystem/PSBlock enthaltene Particle
  Texture-UUID wird nicht mehr nur gespeichert: Für sichtbare
  Emitter (maximal 32 im Umkreis) wird die Textur im vorhandenen
  GetTexture-Asset-Loader vor regulären Hintergrund-Objekttexturen
  angefordert und als RGBA8/ggf. JPEG2000 dekodiert.
- Emittierte Bilder teilen denselben 512-Assets-GPU-Cache mit
  Object-TextureEntry; gleiche UUID wird nur einmal angefragt
  und kann für Mesh/Prim und Partikel gleichzeitig genutzt werden.
  Die bereits bekannte Epochen-Prüfung verhindert das Hochladen
  veralteter GetTexture-Replies aus früheren Regionen.
- Neuer Pure-Logic Asset-Priorisierer prüft null/Zero-UUIDs,
  doppelte IDs, bereits geladene und zuvor fehlgeschlagene
  Requests. Bei Bedarf wird auf Objekttexturen zurückgegriffen.
  Eigener automatischer Regressionstest.
- Der Particle-Fragmentshader übernimmt echte Textur-RGBA
  über GL_TEXTURE4 (getrennt nach Textur-UUID gebatcht),
  verrechnet sie mit Start-/End-Farbe und Soft-Mask-Alpha.
  Bei nicht geladenem Asset nutzt er weiterhin die zuvor
  existierende weiche Kreis-Sprite-Darstellung.
- Maximal 768 Partikel/Frame, höchstens 32 Emitter; künftig
  statt eines einzelnen bis zu einem Drawcall je
  verwendeter Partikeltextur plus optional Fallback-Batch.
  Keine eigenen GL-Texturduplikate, kein weiteres HTTP-Subsystem.
- Neue Tests fuer priorisierte Assetwahl, gemeinsame Cache-
  Nutzung, ungültige IDs, geladene und fehlgeschlagene Texturen.
  **27/27 CTests**, CMake-Debug-Build sowie OpenGL-GUI-Smoke
  auf NVIDIA RTX 4070 SUPER ohne Shaderfehler.
- Echte In-World-Sichtabnahme steht noch aus; das vorhandene
  Cachelimit von 512 Assets, transparenzabhängige Sortierung,
  Partikeltextur-Orientierung, diskrete Simulation und
  exakte OpenSim-Partikelmodi bleiben spätere Schritte.
- Gesammelte Avatar-/Bewegungs-/Mesh-Fixphase ausdrücklich
  nicht vorgezogen.

Weitere Planblöcke: Wind-/Bounce-/Target-/Ribbon-Partikel,
Effekt-Lebenszyklen/Blending und komplexere Materialbeleuchtung,
danach Phase 9 Asset-Streaming und übrige Viewerfunktionen.
Protokolldetails: docs/PARTICLES.md.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl55-live.log


## 0.2.0-alpha.56 – Großrelease Phase 8 / vier Entwicklungsblöcke · 10.10.2026

Auf Nutzerwunsch werden zukünftig **3–4 zusammenhängende Funktionen
pro Release** umgesetzt, mit einem gemeinsamen Abschluss aus Build,
Tests, Versionspflege, Dokumentation und GitHub-Commit. Einzelne
Zwischentests bleiben zur Qualitätssicherung möglich.

**Block A – Partikelphysik:**
- Neuer testbarer reiner Partikelkinematik-Helfer, mit Begrenzung
  von Lebensdauer und festen Integrationsschritten (48).
- Gravity/PSBlock-Beschleunigung, Wind-Flag 0x008 und Bounce-Flag
  0x004. Wind ist aktuell ein **lokal generiertes Feld** und
  keine behauptete Simulator-Windmessung; Bounce erfolgt auf der
  Höhe der Quellprim, nicht durch Terrain-/Collision-Abfragen.
- Flags für InterpColor (0x001) und InterpScale (0x002) werden
  berücksichtigt statt unbedingter Interpolation.

**Block B – Ziel- und Verbindungseffekte:**
- Target UUID aus dem 68-Byte-PSBlock-Systemteil auslesen;
  bekannte Zielprims, Avatare und eigener Agent werden anhand
  UUID dem Emitter zugeordnet; unbekannte Ziele bleiben
  unaufgelöst, ohne auf erfundene Positionen zu springen.
- TargetPos (0x040) und TargetLinear (0x080) im CPU-Sampler,
  Beam (0x200) als verteilte Partikel entlang einer Strecke
  und Ribbon (0x400) als zeitlich versetzte Punktspur.
- Echte zusammenhängende Ribbon-Flächen, SourceRotation,
  Target-Bewegungs-Prediction und spezifische Formmodi sind
  **noch offen**.

**Block C – Transparenz, Blending, Budget:**
- OpenMetaverse ParticleSystem BlendFunc-Enums 0..9 für
  Extended PSBlock werden validiert und mit OpenGL abgebildet.
  Default SourceAlpha=7, OneMinusSourceAlpha=9.
- Neue distanzbasierte Sortierung in acht Tiefenbändern,
  innerhalb eines Bands Batching nach Blend/Texture.
- Feste Obergrenzen von 768 Partikeln und maximal 192
  Zeichen-Batches schützen vor übergroßer Drawcall-Zahl.
- Das ist nur näherungsweise Alpha-Tiefensortierung; echte
  per-Partikel-Sortierung über alle Texturen ist offen.

**Block D – Beleuchtung/Materialgrundlagen:**
- Bei den bis zu acht Legacy-Point-Lights wird zusätzlich
  zum bestehenden Radius/Falloff der Cutoff-Wert im Shader
  als näherungsweiche Kernausblendung berücksichtigt.
- Lokales Blinn-Phong-Highlight für glänzende,
  nicht-fullbright Objektoberflächen ergänzt.
- Kein vollständiges PBR, keine Projektorlichter, keine
  dynamischen Schatten oder Energieerhaltung.

**Abnahme:** Debug-CMake + C++23 erfolgreich, 28/28 CTests
einschließlich neuer reiner Wind-/Bounce-/Target-Tests,
Parser-Tests für TargetUUID und Blend-Enums;
OpenGL-GUI-Smoke auf NVIDIA RTX 4070 SUPER, Version alpha.56,
NexVerse-Welcome geladen, keine Shader-Fehler (der Exit 124
des Smoke-Tests ist die beabsichtigte 7-Sekunden-Begrenzung).
**Authentifizierter In-World-Sichttest dieser Effekte ausstehend.**

**Zurückgestellte Fehler unverändert:** Eigene Avatar-LocalID
und Bewegungsbestätigung, echte Avatar-Skeleton-/Appearance,
defekte/fehlende Mesh-/Texturassets und Teleport/Region-Handoff.
Keine dieser Fehlergruppen wird in diesem Release als gelöst
ausgegeben.

**Nächste gemeinsame Blöcke:** echte PSBlock-Emitter-Lebenszyklen,
Parent-/ObjectRelative- und Target-Pfade, verbundene Ribbons
und Beam-Geometrie, Asset-Caching und moderne Materialeffekte.
Die Reihenfolge bleibt an projectplan.md gebunden.

Live-Abnahme:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl56-live.log


## 0.2.0-alpha.57 – Großrelease: vier zusammenhängende Funktionsblöcke · 10.10.2026

**Block 1 – Emitter-Lebenszyklus / ObjectRelative**
- Emitterstart auf Renderer-Zeitbasis (nicht mehr globaler Partikelphase).
  CRC-Wechsel startet ein emittierendes Prim neu; entfernte Emitter
  werden aus dem Laufzeitstatus entfernt.
- SourceMaxAge stoppt neue Bursts, lässt bereits gestartete Partikel
  innerhalb ihrer eigenen PartMaxAge-Zeit auslaufen.
- Rotationskompatibilität: bei OpenSim SourceFlags ObjectRelative
  (Bit 0) werden die Quellbeschleunigung und Initialgeschwindigkeit
  durch die Objekt-Quaternion transformiert.
- Offene Details: vollständige LSL-Start/Stop- und SimTime-Synchronisation,
  Quelle mit ParentRotation, Simulator-Wind statt synthetischem Wind.

**Block 2 – durchgehende Beam-/Ribbon-Geometrie**
- Neuer Kameraseiten-ausgerichteter Ribbon-Generator mit
  verbundenen Dreiecken statt einer bloßen Punktfolge.
  Separater OpenGL-3.3-Triangle-Shader/-VAO/-VBO, UVs,
  transparente Soft-Edges sowie geteilte GetTexture-Assets.
- Beam folgt dem bekannten Zielpunkt; Ribbons folgen
  einer auf Basis der Partikelkinematik erzeugten Spur.
  Begrenzung 32 Emitter/je 16 Samples und 15 Segmente.
- Offene Details: echte bewegungsabhängige kontinuierliche
  Emitter-Historie, native Ribbon-Rotation und robuste
  Transparenzsortierung aller überlappenden Flächen.

**Block 3 – Partikelmaterial / Glow**
- In Extended PSBlock vorhandener Start/EndGlow wird an den
  GPU-Renderer durchgegeben und für Sprites und Ribbon-Material
  als begrenzte RGB-Aufhellung berücksichtigt; nicht als
  echtes physikalisches HDR- oder PBR-Emissive ausgegeben.
- OpenSim-Blend-Enums 0..9 aus alpha.56 bleiben aktiv.

**Block 4 – Phase 9: wiederverwendbarer Texture-Asset-Cache**
- Frühere harte 512-GPU-Texturen-Grenze wird durch ein
  dynamisches Residency-Budget ersetzt; unbenutzte Texturen
  werden als GL-Texturen freigegeben, höchstens 8 pro
  Planungsrunde (750ms).
- Reihenfolge: nahe Partikelemitter, anschließend Prim-/
  Meshmaterialien; letztere erhalten eine Objekt-Distanz-
  priorisierte Ordnung beim Aufbau der Scene-Revision.
- Neue Region leert den GPU-Objekttexturcache; veraltete
  HTTP-Replies außerhalb von Region/Epoche/Residency werden
  verworfen. Gemeinsames GetTexture-Ladesystem bleibt.
- Grenzen: noch kein persistentes CPU-/Disk-Texture-Cache,
  keine Texture-LOD-/Mip-Streaming-Stufen, keine Occlusion,
  UI/Textur-Distanz-Neusortierung nur bei Scene-Update;
  beim Zurückkehren eviktierter Texturen erneut HTTP möglich.

**Tests:** 30/30 CTests inklusive Lifecycle, Quaternion,
Ribbon-Triangles, Cache-Umschichtung. C++23-Debug-Build
und OpenGL-3.3-Starttest auf NVIDIA RTX 4070 SUPER
ohne gemeldeten Shaderfehler. Der 7-Sekunden-Exit 124
ist der absichtliche Timeout, kein Crash.
**NexVerse In-World-Darstellung bleibt ungeprüft.**

**Auf Nutzerwunsch zurückgestellt:** Avatar-LocalID/
serverbestätigte Bewegung, echte Avatar-Appearance,
defekte/missing Mesh-/Texture-Assets und Teleport.
Kein dieser Fehlerbereiche wird als behoben ausgegeben.

Nächster Großrelease-Block: Asset-Caching-Disk/CPU,
LOD/Prioritätsstufen und weitere OpenSim-Renderer-
Kompatibilität. projectplan.md bleibt verbindlich.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl57-live.log


## 0.2.0-alpha.58 – Phase 9 Großrelease (vier Blöcke) · 10.10.2026

Nutzerentscheid: Jeweils drei bis vier vollständige Entwicklungsblöcke
pro Release; bekannte Avatar-/Bewegungs-/Mesh-Asset-Fehler bis
zur späteren Bugfixphase zurückstellen. Verbindlich: projectplan.md.

**Block A – Persistenter Texture-Disk-Cache:**
- TextureDiskCache lädt/speichert validierte RGBA8-Assetdaten
  unter $XDG_CACHE_HOME/opengenesislink/textures-v1 bzw.
  $HOME/.cache/opengenesislink/textures-v1.
- 8-Byte-Version-Magic, Width/Height, FNV-64-Inhaltshash über
  Dimensionen und Pixeldaten, Größen-/Bounds-Prüfungen, sichere
  UUID-Dateinamen, temp-Datei + Umbenennung.
- 256 MiB Diskbudget, 64 MiB maximales Einzelbild,
  least-recently-used-ähnliche Eviction anhand File-MTime;
  auf Cache-Miss/Fälschung/IO-Problem normaler GetTexture-Download.
- Cache wird im Hintergrund-Texture-Future geprüft; bei Erfolg
  wird HTTP übersprungen. Kein persistenter Mesh-/Animationscache
  in diesem Release, keine echte Hash-/Version-Aushandlung mit
  dem OpenSim-Asset-Service.

**Block B – Dynamische Texturprioritäten:**
- Texturpositionen werden bei Object-/Scene-Update für Default,
  Face sowie Normal-/Specular-Materialien gesammelt.
- Alle 750ms/Scene-Revisionsänderung werden sie ausgehend von
  der aktuellen Avatarposition nach Entfernung neu sortiert.
- Identische UUIDs werden zusammengefasst; bereits GPU-geladene
  Assets erhalten 8m Hysterese gegen Thrashing.
- Partikel-Assetpriorität sowie 512 gleichzeitige GPU-Texturen,
  begrenzte GL-Eviction und Regions-Epochenprüfungen bleiben.

**Block C – Mesh-Sichtgrößenpriorität:**
- MeshDistanceSample führt zusätzlich den maximalen
  Prim-Scale-Durchmesser; Priorisierung gewichtet
  distance / sqrt(clamp(diameter,0.5,16)).
- Größe beeinflusst ausschließlich Assetabruf/Priorität,
  **noch nicht** echte Mesh-LOD-Stufen, View-Frustum-/Occlusion-
  Culling oder Mesh-Vertex-Dekodierung. Defekte Meshes werden
  nicht als repariert bezeichnet.

**Block D – GPU-Sampling und Asset-Diagnose:**
- Bei unterstützter GL_EXT_texture_filter_anisotropic
  aktiviert das Legacy-GL-Backend für Objekttexturen
  bis zu 8x anisotrope Filterung zusätzlich zum Mipmap-Sampler.
  Auf inkompatiblen GPUs bleibt das alte Verhalten.
- Log alle ~60s: kumulative Disk-Cache-Hits, HTTP-Downloads,
  GPU-Evictions, residente Texturen und GPU-Slotbudget.
  Die Zähler sind Sitzungsstatistik, keine serverbestätigte
  Bandbreitenmessung.

**Verifikation:**
- CMake Debug C++23 erfolgreich; 32/32 CTests.
- Neue Tests für persistente Cacheintegrität, manipulierte
  Cache-Dateien, Neustart-/LRU-Verhalten, bewegungsabhängige
  Texture-Distance-Ordering/Hysterese und größenbasierte
  Mesh-Priorität.
- Linux NVIDIA GeForce RTX 4070 SUPER OpenGL 3.3 GUI-Smoke
  bei 3440x1440 mit Welcome-Seite, keine gemeldeten GL-
  Shaderfehler; Exitcode 124 war beabsichtigtes 7s-Timeout.
- **Authentifizierte In-World-Streaming-Abnahme ausstehend.**

**Bekannte, absichtlich unangetastete Probleme:**
Self-Avatar-LocalID/Bewegung, echtes Avatar-Skeleton/Bakes,
noch fehlerhafte/fehlende Mesh-/Texturassets, Teleport.
Keiner dieser Punkte wird als geschlossen markiert.

**Nächste Großrelease-Ideen gemäß Phase 9:** Mesh-Disk-Cache,
RAM-Assetcache, Chunk-/Mip-Streaming-Priorität, GPU/CPU-
Speicherbudget und anschließend Avatar-Funktionssysteme.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl58-live.log


## 0.2.0-alpha.59 – Phase 9 Mesh Cache · 11.10.2026

**Block A – persistente Mesh-Dateien:** 512 MiB Diskbudget,
Versionmagic OGLMESH1, Little-Endian, UUID-gebundene FNV64-
Integritätsprüfung, Größen-/Index-/Face-Checks und MTime-LRU.
Keine CAPS-Token/URL-Cachedaten.

**Block B – Mesh-RAM-LRU:** threadgeschützte CPU-Geometry-
Residency mit 64 MiB Bytebudget und LRU-Eviction. Durch
GPU-Eviction entfernte Meshes können ohne HTTP erneut
aus dem RAM geladen werden.

**Block C – Integration:** asynchroner Ladepfad RAM → Disk →
GetMesh2/GetMesh; Region-/Epochenschutz und GPU-Budget 256
Meshes (max. acht Evictions je Planung) unverändert.

**Block D – Telemetrie und Tests:** getypte Cacheherkunft,
60s-Metriken über erfolgreiche GPU-Uploads nach Quelle,
CPU-RAM-Bytes und GPU-Slots. Manipulations-, Neustart-,
UUID-, LRU-/Budget- und Fehler-Fallback-Tests.
CMake Debug C++23, **33/33 CTests bestanden**,
OpenGL 3.3 GUI-Starttest auf NVIDIA RTX 4070 SUPER mit
NexVerse-Welcome erfolgreich; Timeout 124 erwartungsgemäß.

**Noch offen:** In-World-Abnahme, echtes Mesh-LOD-Streaming,
GPU/CPU-Bytebudget und Texture-Mip-Streaming.
Avatar-LocalID, WASD, Skeleton/Bakes, defekte Server-Mesh-/
Texturassets und Regions-Teleport bleiben auf Nutzerwunsch
bis zur Bugfixphase zurückgestellt.
`projectplan.md` bleibt verbindlich.
Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl59-live.log`.

Code: `assets/mesh_cache.hpp/.cpp`, `tests/mesh_cache_tests.cpp`,
`src/app/application.cpp`. Weitere Details:
`docs/ASSET_STREAMING.md`, `docs/MESH.md`.
Handoff: `develop-current` und nicht divergentes `develop`.


## 0.2.0-alpha.60 – Dynamische Mesh-LODs · 11.10.2026

**Block A – OpenSim-LOD-Decoder:** High, Medium, Low und Lowest. Gewünschte Stufe zuerst; bei fehlenden/defekten Chunks Fallback auf andere gültige Level. Bestehende LLSD-/zlib-/Mesh-Bounds gelten weiter.

**Block B – Adaptive LOD-Planung:** `plan_mesh_lods` gewichtet Entfernung und Primgröße, dedupliziert mehrfach verwendete Mesh-UUIDs und nutzt Hysterese. Aktualisierung alle etwa 750 ms oder bei Szene-Änderung.

**Block C – LOD-getrennte RAM-/Disk-Caches:** High behält den bisherigen `<uuid>.mesh`-Schlüssel; andere Stufen werden unter `<uuid>.<lod>.mesh` inklusive LOD-gebundener Checksumme gespeichert. Budgets 64 MiB RAM und 512 MiB Disk. Nach Server-Fallback bezeichnet der Cache-Key den angeforderten, nicht zwingend den tatsächlich dekodierten LOD.

**Block D – GPU-Swap und Diagnose:** Mesh-LOD-Upgrades und -Downgrades ohne vorherige Entfernung der Mesh-UUID. GPU lädt zuerst neue Buffers, gibt alte erst nach erfolgreichem Upload frei. Aktuelle Regions-Epoche und Ziel-LOD werden vor Upload geprüft. Neues Log: LOD-Swap-Zähler und geschätzte VBO/EBO-Bytes.

**Verifikation:** C++23-Debug-Build, 34/34 CTests (LOD-Codec/Fallback, Cache-Varianten, Hysterese). OpenGL-Start-Smoke zum Release. Weiter offen: authentifizierter In-World-LOD-Test, GPU-Bytebudget, Texture-Mip-Streaming, Rigging/Animesh und zurückgestellte Avatar-/Bewegungs-/Teleport-Fehler.

Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl60-live.log`. Maßgeblich: `projectplan.md`, Branch `develop-current`.

Quellen: `src/assets/mesh_loader.cpp`, `src/assets/mesh_cache.cpp`, `include/nexverse/assets/mesh_lod_policy.hpp`, `src/app/application.cpp`, `src/render/opengl_renderer.cpp`. Tests `mesh_loader_tests`, `mesh_cache_tests`, `mesh_lod_policy_tests`.


## 0.2.0-alpha.61 – Phase 9, Textur-Mip-Streaming · 11.10.2026

**Block A – CPU-Mip-Vorstufen:** `texture_mip_image` erzeugt bis zu drei kleinere RGBA8-Basisstufen mit alpha-gewichteter Mittelung (premultiplizierte Farbsummen); NPOT-Bilder werden begrenzt unterstützt. Der Renderer erzeugt darauf weiter gewöhnliche OpenGL-3.3-Mipmapketten. Es wird kein JPEG2000-Progressive- oder HTTP-Chunk-Streaming vorgetäuscht.

**Block B – Distanzabhängige Qualitätswahl:** `plan_texture_mip_levels` wählt 0/1/2/3 je nach Agentabstand (24/64/128 m), mit Hysterese an den Grenzwerten. Mehrfach verwendete Texture-UUIDs verwenden die nächste Objektposition; aktive Partikeltexturen erhalten Vorrang mit Level 0. Die Planung läuft bei Scene-Revision oder spätestens etwa alle 750 ms.

**Block C – Asynchroner Full-Resolution-Cache/Hot-Reload:** HTTP und der bestehende 256-MiB-RGBA8-Disk-Cache speichern ausschließlich Originalbilder. Der asynchrone Worker erstellt daraus die gewünschte CPU-Mipstufe. Ändert sich die Entfernung, kann die vorhandene GPU-Textur erneut hochgeladen und verkleinert oder vergrößert werden, ohne die ursprüngliche GetTexture-Assetdatei erneut laden zu müssen. Region-/Epochenschutz, Residency und veraltete Qualitäts-Requests werden vor dem Upload geprüft.

**Block D – Geplantes 256-MiB-GPU-Texturbytebudget:** Mit `gpu_rgba8_mip_bytes` werden RGBA8-Mipketten berechnet; `plan_texture_byte_budget` kann vor Upload Texturen niedrigerer Priorität zur Eviction bestimmen. Höher priorisierte Texturen werden nicht verdrängt, um weiter entfernte Assets nachzuladen. Kann ein Upload nicht ins Byte-/512-Textur-Slotbudget passen, wird er zurückgestellt statt unkontrolliert zu wachsen. Telemetrie: aktuelle geschätzte Texturbytes, LOD-Swaps, zurückgestellte Uploads und GPU-Evictions.

**Verifikation:** Debug-Build C++23, 35/35 CTests einschließlich RGBA-Alpha, ungerader Dimensionen, Mip-Byteberechnung, Distanzhysterese und Budgetpriorität. OpenGL-3.3-GUI-Smoke nach Versionspflege. Live-In-World-Abnahme mit echtem NexVerse-Login steht aus.

**Abgrenzung:** Die 256 MiB sind ein softwareseitig aus RGBA8-Mipdimensionen berechnetes Texturbudget, keine gemessene oder vollständig durchgesetzte physische VRAM-Gesamtgrenze. Terrain-, UI- und Mesh-GPU-Ressourcen gehören nicht zu diesem Texture-Budget. Bei sehr großen/zahlreichen Bildassets können zusätzliche Inhalte unter Budgetdruck vorerst im Fallback bleiben.

**Offen:** Netzwerk-Chunks und native JPEG2000-Progressive-LOD, effizientes CPU-RAM-Caching für sämtliche Texturlevel, gemeinsames GPU-Bytebudget für Mesh/Terrain/Texture, authentifizierter Streaming-Sichttest; bekannte Avatar-/WASD-/Mesh-Asset-/Teleportfehler bleiben auf Nutzerwunsch für die spätere gesammelte Fehlerbehebung dokumentiert.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl61-live.log`.

Code: `include/nexverse/assets/texture_mip.hpp`, `src/assets/texture_mip.cpp`, `src/app/application.cpp`, `CMakeLists.txt`. Tests: `tests/texture_mip_tests.cpp`; bisher 35/35 CTests bestanden. Branch `develop-current` bleibt Handoff.


## 0.2.0-alpha.62 – Asset-Streaming-Priorität und Caches · 11.10.2026

**Block A – Kameraabhängige Asset-Priorität:** Ein weicher Blickrichtungsfaktor gewichtet Textur- und Mesh-Asset-Anfragen relativ zur Third-Person-Kamera. Objekte hinter dem Betrachter werden nicht ausgeblendet; sehr nahe Objekte bleiben grundsätzlich hoch priorisiert. Die bestehenden Entfernungssortierungen und GPU-Residency-Grenzen bleiben erhalten. Das ist keine echte Frustum- oder Occlusion-Culling-Implementierung.

**Block B – CPU-RAM-Texturcache:** Ein threadgeschützter 96-MiB-LRU-Cache hält vollständige dekodierte RGBA8-Bilder pro gültiger UUID vor. Der asynchrone GetTexture-Pfad arbeitet RAM → persistenter Diskcache → HTTP/Decoder. Für kleinere GPU-Mip-Basisbilder wird das dekodierte Original aus dem RAM wiederverwendet. Keine Cache-Speicherung von CAPS-URLs, Tokens oder ungültigen Bildern.

**Block C – gemeinsam begrenzte GPU-Mesh-/Objekttexturbytes:** Geschätztes, softwareseitiges gemeinsames Budget von 384 MiB mit zusätzlich jeweils 256 MiB als Maximalgrenze für Mesh-Buffer und Objekttexturen. Vor GPU-Upload werden bestehende Ressourcen und ersetzte LOD-Varianten eingerechnet. Bei Überschreitung wird ein Mesh-Upload zurückgestellt; Texturen können zuerst niedrig priorisierte Texturen verdrängen. Bei freiwerdendem Speicher werden zurückgestellte Anfragen wieder berücksichtigt. Dies ist ausdrücklich keine vollständige VRAM-Bilanz: Terrain, UI, Treiber-Overhead und weitere Ressourcen sind nicht enthalten.

**Block D – Streaming-Diagnose und Regressionen:** 60-s-Metriken für RAM-Treffer, RAM-Größe/Einträge, GPU-Mesh-/Texturbytes, gemeinsames geschätztes Budget und Deferred Counts. Durchschnittliche Zeit vom Asset-Worker-Start bis zur Verarbeitung der fertigen Future (keine reine Netzwerklatenz). Neue Tests für Kamera-Priorität, sicheren gemeinsamen Budget-Rechenweg sowie den parallelen Textur-RAM-LRU-Cache.

**Verifikation:** C++23-Debug-Build und 38/38 automatisierte CTests erfolgreich. OpenGL-3.3-Welcome-Starttest nach Versionspflege ausführen. Kein bestätigter In-World-Test des NexVerse-Asset-Streamings.

**Bewusst offen:** echte Frustum-/Occlusion-Abfrage, globale inklusive Terrain/UI gemessene VRAM-Budgetierung, inkrementeller J2C-Netzstream, Avatar-Skeleton/Bakes, Bewegungs-Synchronisierung und vollständige Teleport-/Circuit-Behandlung. Bekannte Mesh-/Texture-Serverfehler bleiben in der späteren Fehlerbehebungsphase.

**Nächste 3–4 Blöcke:** Asset-Streaming-Queue/Prioritätsregeln, planbares Prefetching mit Limits, Cache-/Regions-Invalidierung und zusätzliche Mess- und In-World-Integrationsdiagnostik.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl62-live.log`. Verbindlicher GitHub-Handoff-Branch: `develop-current`.

Code: `include/nexverse/assets/asset_view_priority.hpp`, `include/nexverse/assets/asset_gpu_budget.hpp`, `include/nexverse/assets/texture_memory_cache.hpp`, `src/assets/texture_memory_cache.cpp`, `src/app/application.cpp`; angepasste Planer `texture_distance.hpp` und `mesh_residency.hpp`. Neue Tests `asset_view_priority_tests.cpp`, `asset_gpu_budget_tests.cpp`, `texture_memory_cache_tests.cpp`. Der GitHub-Branch `develop` ist weiterhin nicht der maßgebliche Handoff.


## 0.2.0-alpha.63 – Asset-Pacing und diskbasiertes Prefetching · 11.10.2026

**Block A – GetTexture-/GetMesh-Anfrage-Taktung:** `AssetRequestPacer` taktet neu gestartete Textur-Demand-Worker auf mindestens 125 ms und Mesh-Demand-Worker auf mindestens 180 ms Abstand. Die bereits vorhandene Kamera-/Distanz- und Partikelpriorität bleibt maßgeblich; höchstens je ein aktiver Demand-Worker pro Assettyp und keine künstliche Blockade bereits laufender Requests. Pacer werden bei Regionswechsel zurückgesetzt.

**Block B – begrenztes Texture-Prefetching:** Der Viewer wärmt vor dem eigentlichen GPU-Upload vollständige RGBA8-Bilder aus dem persistenten Texture-Diskcache in den 96-MiB-RAM-LRU vor. Die Auswahl berücksichtigt die bereits priorisierte GPU-Residency-Reihenfolge, ignoriert GPU-residente und bekannte fehlgeschlagene UUIDs und untersucht maximal vier Kandidaten pro Durchgang. Es läuft höchstens ein Prefetch-Worker gleichzeitig, mit mindestens 650 ms Abstand und maximal 48 Prefetch-Kandidaten pro Region. Es finden dabei ausdrücklich keine zusätzlichen HTTP-Anfragen oder GPU-Uploads statt.

**Block C – Regions-Epochen und Cache-Admission:** `TexturePrefetchScope` setzt geprüfte Kandidaten bei neuem `material_scope.epoch()` zurück. Die aktuell gültige Regionsepoche wird atomar an den Prefetch-Worker weitergegeben; ein beim Abschluss veralteter Disk-Read wird vor der Cache-Aufnahme zurückgewiesen. Bestehende GetTexture-/GetMesh-Epochenschutzprüfungen im GPU-Pfad bleiben unangetastet. Diskcache und RAMcache sind UUID-basiert, CAPS-Tokens werden nicht gespeichert. Prefetch-Worker lassen sich nicht während laufender Dateizugriffe zwangsweise abbrechen.

**Block D – Telemetrie und Regressionstests:** Das 60-Sekunden-Streaminglog meldet Demand-Starts je Assettyp sowie Prefetch-Versuche, erfolgreiche RAM-Erwärmungen, Diskcache-Misses und stale Prefetch-Resultate. Neue deterministische Tests für Zeitintervalle, Kandidaten-Limits und Epochensprünge; Integrationstest des echten Disk→RAM-Prefetch-Pfads einschließlich manipuliertem Cachefile. `TextureMemoryCache::contains()` ermöglicht den Kandidatenabgleich ohne teure Bildkopie.

**Verifikation:** C++23-Debug-Build, 40/40 CTests einschließlich neuer Scheduler- und Disk-Prefetch-Tests. Linux OpenGL-3.3-GUI-Starttest nach Versionspflege. Authentifizierter NexVerse-In-World-Streaming-Test ist noch ausstehend.

**Grenzen:** Kein HTTP-Prefetch, kein eigenständiger parallel ladender Mesh-Prefetcher, kein inkrementeller J2C-Assetstream. 384-MiB-Budget bleibt eine Schätzung für Mesh-/Objekttextur-GPU-Ressourcen ohne Terrain/UI und Treiber-Overhead. Unvollständiger Avatar, Bewegung, Teleport und einzelne Mesh-/Texturserverfehler bleiben entsprechend Projektplan für die spätere Bugfixphase dokumentiert.

**Nächste 3–4 Blöcke:** Recovery und Fehlerklassifikation des Asset-Loaders, temporäre Retry-/Backoff-Logik statt permanenter Fail-Sets, robustere Regionsepochen-Aufräumlogik und In-World-Assetmetriken. `projectplan.md` bleibt bindend.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl63-live.log`; Branch `develop-current`.

Wichtige neue Dateien: `include/nexverse/assets/asset_stream_scheduler.hpp`, `include/nexverse/assets/texture_prefetch.hpp`; relevante Änderungen in `src/app/application.cpp`, `include/nexverse/assets/texture_memory_cache.hpp`, `src/assets/texture_memory_cache.cpp`. CMake: `asset_stream_scheduler_tests.cpp`, `texture_prefetch_tests.cpp`. Wiederaufnahme immer über GitHub-Branch `develop-current`; `develop` ist nicht maßgebend.


## 0.2.0-alpha.64 – Asset-Fehlerklassen und Retry-Cooldown · 11.10.2026

**Block A – strukturierte Asset-Fehlerklassifikation:** Der GetTexture- und GetMesh-HTTP-Pfad liefert zusätzlich zum Diagnosetext `AssetFailure`: `transient` (Timeout/Transport, 408/425/429/5xx), `missing` (404/410), `invalid` (fehlerhafter Decoder/unerwartete HTTP-4xx) oder `configuration` (400/401/403, fehlender Transport/ungültige CAPS-URI). Bei PNG→J2C beziehungsweise GetMesh2→GetMesh-Fallback bestimmt die kombinierte Fehlerklasse die Wiederholungsstrategie; ein erfolgreicher Fallback gilt als Erfolg.

**Block B – zeitlich begrenzte Retry-/Backoff-Logik:** `AssetRetrySchedule` ersetzt die bisher bis zum Regionswechsel dauerhaft gesperrten Fehler-UUIDs. Temporäre Fehler: nach 2, 4 und 8 Sekunden erneute Versuche, danach fünf Minuten Sperrzeit; HTTP 404/410: zehn Minuten; ungültige Assets: 30 Minuten; Konfigurationsfehler: fünf Minuten. Erfolgreiche GPU-Uploads löschen den zugehörigen Fehlerstatus. Mesh-Fehlerstatus unterscheidet UUID und angeforderten LOD; GPU-Bytebudget-Deferrals bleiben weiterhin getrennt. Die Map ist auf 4096 Einträge begrenzt; bei darüber hinausgehender Kartenfluktuation wird sie geleert.

**Block C – regiongebundene Fehlerverwaltung:** Bei `material_scope.epoch()`-Wechsel werden Mesh- und Textur-Retrytabellen geleert. Bereits laufende Downloads werden nicht auf dem GUI-Thread blockiert oder abgebrochen. Fertige Antworten aus alten Epochen bleiben durch die bestehenden Epochenschutzprüfungen vom GPU-Upload ausgeschlossen; zusätzliche Stale-Counter dokumentieren diese Verwerfungen. Cache-Dateiformate und Mesh-/Texture-Cachebudgets bleiben unverändert.

**Block D – Diagnose und Regressionstests:** Streaminglogs führen `failure_events`, `retry_dispatches`, `backoff_assets`, `transient_failures` und `stale_results` je Mesh-/Textur-Familie. Tests prüfen Klassifikation von HTTP-Antworten, Rechenfolge des Backoffs, Regionsepochen, Zurücksetzen bei Erfolg und beide echten Loader-Fallback-Pfade (404 und 503) mittels Mock-HTTP-Transport.

**Verifikation:** Vollständiger C++23-Debug-Build und 41/41 CTests sind erfolgreich. Nach Versionspflege erfolgen erneuter vollständiger Rebuild und Linux OpenGL-3.3-Welcome-Starttest. Ein authentifizierter NexVerse-In-World-Streaming-Test ist weiterhin nicht ausgeführt.

**Grenzen und offene Arbeiten:** Diese Verbesserung beseitigt nicht die eigentlichen Fehler einzelner Server-Assets. Sie verhindert stattdessen endlose schnelle Retries beziehungsweise permanente lokale Sperren. Tatsächliche Simulator-Synchronisierung, Avatar-LocalID/Appearance, Bewegungsanzeige sowie Regions-Teleport bleiben wie vereinbart für spätere Fehlerblöcke offen. Ein genereller Netzwerkanfrage-Abbruch mittels Cancellation-Token ist nicht implementiert.

**Nächster Release (3–4 Blöcke):** In-World-Integrationsdiagnostik und Echtzeit-Aussagekraft, bessere Unterscheidung von Cache-/Serverfehlern, mögliche asynchrone Netzwerk-Cancellation und Asset-Streaming-Queue-Optimierung; die verbindliche Roadmap bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl64-live.log`; maßgeblicher Branch ist `develop-current`.

Neue Typen: `include/nexverse/assets/asset_failure.hpp`, `AssetFailure`, `AssetRetrySchedule`. Angepasst: `src/assets/texture_loader.cpp`, `src/assets/mesh_loader.cpp`, `src/app/application.cpp`, CMake. Neue Tests: `tests/asset_failure_tests.cpp`, außerdem Mock-Transport-Regressionen in `texture_loader_tests.cpp` und `mesh_loader_tests.cpp`.


## 0.2.0-alpha.65 – In-World-Asset-Diagnostik · 11.10.2026

**Block A – Cache-Miss- und Dateifehlerdiagnose:** `AssetCacheLookup` und die optionale Statusausgabe von `TextureDiskCache::load()` unterscheiden `hit`, `miss`, `corrupt`, `io_error`. Der bereits vorhandene Disk-Cache-Header, die CRC-artige FNV64-Inhaltsprüfung, die 256-MiB-Ressourcengrenze und UUID-Validierung bleiben kompatibel. Texturworker berichten Disk-Misses, beschädigte Einträge und Dateisystemfehler getrennt von HTTP-Versuchen. Diese Quellenklassifikation verändert keine Serverantworten.

**Block B – koordinierte Regions-Epochen im Asset-Worker:** Der bestehende atomare Live-Epochenmarker aus dem Textur-Prefetch wird auch von Demand-GetTexture und GetMesh verwendet. Worker prüfen die Epoche vor Beginn und unmittelbar vor einem bislang nicht gestarteten HTTP-Download. Nach einem bereits laufenden HTTP-Download prüfen sie erneut vor dem Schreiben in den RAM-/Disk-Cache. Die `load_mesh_cached`-Funktion bietet dafür einen optionalen Admission-Callback. Die GPU-Ladepfade verwerfen nach wie vor veraltete Epochen. Bereits laufendes libcurl-HTTP kann noch nicht per Cancellation-Token unterbrochen werden.

**Block C – In-World-Health-Telemetrie:** Nach Aufbau der Simulation schreibt das Rendering alle etwa 30 Sekunden eine Kennzahlenzeile `InWorld asset health` (Regionsepoche, Anzahl der Szenen-Samples, gewünschte vs. bereits GPU-residente UUIDs und Deckungsgrad in Prozent, fehlende Assets, laufende Texture-/Mesh-Worker, Retry-Einträge und geschätzte GPU-Mesh-/Objekttexturbytes). Ohne aktive Simulation erfolgt keine falsche In-World-Bestandsmeldung. Die Coverage-Metrik bedeutet nur UUID-Residency, nicht vollständige grafische Darstellung, Avatar- oder Netzwerk-Korrektheit.

**Block D – Regressionen und Detailzähler:** Der Diskcache-Test prüft saubere Treffer, Miss und beschädigte Datei. Der Meshcache-Test prüft zwei Abbruchsituationen, einmal vor HTTP und einmal nach Abschluss einer künstlich veralteten Downloadfunktion; in beiden Fällen gelangt nichts Falsches in RAM/Disk. Der neue Test `asset_scene_coverage_tests` prüft leere Szenen, deduplizierte UUIDs, teilweise und vollständige Abdeckung. Die GetTexture-/GetMesh-Streaminglogs nennen zusätzlich `skipped_before_http`, `stale_after_http`, `disk_corrupt` und `http_attempts`.

**Teststand:** C++23-Debug-Build und 42/42 CTests nach Implementierung erfolgreich. Für die finale Version erfolgt nochmals vollständiger Rebuild und Linux-OpenGL-3.3-Welcome-Smoke. Es gibt weiterhin keinen authentifizierten NexVerse-In-World-Livetest; die In-World-Kennzahlen sind instrumentiert, aber noch nicht an realen Simulatordaten verifiziert.

**Unverändert offen:** Eigentliche serverseitig fehlende/beschädigte Mesh- und Texturassets, Avatar-/WASD-/Teleport-Funktionsdefizite und die vollständige visuelle Weltabnahme. Die Fehler werden protokolliert, aber nicht fälschlich als bereits behoben markiert. Keine echte libcurl-Abbruch-API in diesem Release.

**Nächste 3–4 Blöcke:** Realistisches Asset-Recovery-Verhalten unter Last, Retry-Schleifen und Prioritäts-Stabilisierung, strukturierte Diagnostics-/Health-Snapshots statt nur Textlogs und Renderer-/Szenenabnahme. `projectplan.md` bleibt verbindlich.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl65-live.log`. Branch: `develop-current`.

Neu: `include/nexverse/assets/asset_cache_lookup.hpp`, `asset_scene_coverage.hpp`, `tests/asset_scene_coverage_tests.cpp`. Geändert: Textur-/Mesh-Diskcache, TextureAssetResult, MeshCache-Admission, `src/app/application.cpp`, CMake und Regressionstests. GitHub-Handoff weiterhin ausschließlich `develop-current`.


## 0.2.0-alpha.66 – Streaming-Queue-Fairness und Snapshot · 11.10.2026

**Block A – stabile Asset-Auswahl:** `AssetRequestFairness` integriert die bereits vorhandene Kamera-/Entfernungs- und Partikel-Priorisierung mit der pro Asset/LOD geführten Retry-Tabelle. Neue, berechtigte Assets werden innerhalb ihrer bisherigen Reihenfolge bevorzugt; bei einem bereiten Retry wird jeder vierte tatsächliche Dispatch für eine Wiederholungsanfrage reserviert. Bei ausschließlich bereiten Retries werden diese unmittelbar berücksichtigt. Ungültige UUID-Leereinträge, vollständig geladene Assets, Budget-Deferrals und noch aktive Cooldowns bleiben ausgeschlossen. Per-Region-Zähler werden beim Epochensprung zurückgesetzt. Das ist eine kleine Fairness-Schicht vor den bestehenden unabhängigen Textur-/Mesh-Demand-Workern, keine parallele Netzwerk-Multiplexer-Queue.

**Block B – stabile Retry-Zustände unter Last:** `AssetRetrySchedule` setzt bei mehr als 4096 Einträgen nicht mehr die gesamte Retry-Tabelle zurück, sondern verdrängt nur den Eintrag mit dem frühesten Retry-Zeitpunkt. Neue `blocked_count(now)`- und `ready_retry_count(now)`-Metriken trennen noch gesperrte Assets von wieder zulässigen Wiederholungsversuchen. Die bestehenden kurzen 2-/4-/8-Sekunden-Transient-Retries und längeren Cooldowns aus alpha.64 bleiben erhalten. Alte Fehlerzustände werden weiterhin bei einem Regionsepochen-Wechsel invalidiert.

**Block C – strukturierte In-World-Streaming-Snapshots:** Zu den vorhandenen 30-Sekunden-Health-Logs kommt `InWorldAssetSnapshot` mit maschinenlesbarem, numerischem JSON-Schema `ogl.asset.streaming.v1`. Felder: Regionsepoche, Szenensamples, gewünschte/geladene/offene Textur-/Mesh-UUIDs samt Abdeckungsprozent, aktuell laufende Asset-Worker, noch gesperrte und retryfähige Fehler, RAM-Cachebytes und die geschätzte GPU-Mesh-/Objekttexturbelegung inklusive gemeinsamem Softwarebudget. Ohne `simulation_ready` wird kein In-World-Snapshot behauptet; ein leeres Desired-Set ergibt rechnerisch 100 % Coverage und ist kein Nachweis einer vollständigen Welt.

**Block D – Streaming-Integrationstests:** Drei neue CTests: Fairness/Reihenfolge/Duplikate/Deferred-Assets/LOD-Keys, JSON-Schema inklusive leerer Region sowie die gemeinsame Planung aus Texture-Residency, Retry, Byte-Budget, Region-Reset und Health-Snapshot. Zudem ein Stress-Test mit 4098 Failure-Einträgen und den getrennten blocked-/ready-Kennzahlen. C++23-Debug-Build und 45/45 CTests bestanden; vollständiger Release-Rebuild und GUI-Starttest nach Versionserhöhung folgen.

**Wichtige Abgrenzung:** Die neue Integrationstest-Suite prüft die Asset-Planungs- und Budgetkomponenten, nicht einen authentifizierten In-World-Renderer- oder OpenSim-Server-End-to-End-Test. Die bereits bekannten Defizite (Avatar-Körper und Bewegung, fehlende Meshobjekte, fehlerhafte Assetantworten, Teleport und EventQueue) bleiben im Projektplan dokumentiert und werden wie vereinbart in späteren Fehlerbehebungsphasen behandelt. Die GPU-Bytes bleiben geschätzte Mesh-/Objekttexturwerte ohne Terrain-, UI- und Treiber-Overhead.

**Nächster 3–4-Blöcke-Release:** Datengetriebene Auswertung realer Simulatorlogs und Regionen, Entzerrung von Textur-/Mesh-LOD-Swaps, Renderer-GPU-Upload- und Slot-Backpressure sowie reproduzierbare In-World-Testpfade. `projectplan.md` bleibt die maßgebliche Roadmap.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl66-live.log`. Alle Änderungen auf `develop-current`; `develop` nicht automatisch verwenden.

Neu: `include/nexverse/assets/asset_request_fairness.hpp`, `asset_streaming_snapshot.hpp`, `tests/asset_request_fairness_tests.cpp`, `asset_streaming_snapshot_tests.cpp`, `asset_stream_integration_tests.cpp`. Geändert: Retry-Map, `src/app/application.cpp`, CMake. Letzter Live-Build und Commit sind auf GitHub-Branch `develop-current` zu prüfen.


## 0.2.0-alpha.67 – Anti-LOD-Flackern und GPU-Uploads · 11.10.2026

**Block A – tatsächliches Viewerlog analysiert:** `tools/ogl_asset_log_report.py` (Python 3, Standardbibliothek) fasst das reale `$HOME/.local/state/opengenesislink/viewer.log` sowie künftige `InWorldAssetSnapshot`-Zeilen in einem numerischen, UUID-/CAP-URL-freien JSON-Bericht zusammen. Der historische Stand vom 11.10.2026 enthielt 230 Mesh- und 56 Texture-Fehlermeldungen, davon 42 mit 404 und eine mit 5xx-Muster; 243 weitere sind historische unklassifizierte Meldungen. Validierte In-World-Snapshots waren in dieser Datei noch nicht vorhanden. Dokumentierte Bedienung: `docs/LOG_ANALYSIS.md`.

**Block B – LOD-Dwell gegen Flackern:** Die neue generische `AssetLodDwell<Quality>` verlangt für wechselnde GPU-residente Mesh-LODs und Textur-Basismips eine über 1400 ms stabile Zielstufe. Neue noch nicht GPU-residente Assets werden sofort bedient; aktive Partikeltexturen müssen weiterhin mit höchster Bildauflösung angefordert werden. Die Anti-Oszillationszustände werden bei Regionswechsel geleert und von verschwundenen Scene-IDs bereinigt. Bereits bestehende Distanz-/Größen-LOD-Hysteresen bleiben wirksam.

**Block C – Renderthread-Upload-Backpressure und Slotfreigabe:** Der gemeinsame `AssetGpuUploadGate` hält mindestens 30 ms Abstand zwischen erfolgreichen Objekttextur- und Mesh-GPU-Uploads. Fertige Worker-Futures bleiben bis zum nächsten Upload-Fenster ausstehend; es gehen keine bereits dekodierten Daten verloren. `plan_texture_slot_admission` prüft zusätzlich zur bytebasierten Planung das 512-Slot-Limit: Ist das Limit erreicht, darf ein eingehender, höher priorisierter Textur-UUID einen niedrig priorisierten GPU-Slot ersetzen. Die Budgetrechnung berücksichtigt bereits geplante Byte-Evictions und verhindert die Verdrängung höher priorisierter Assets. Terrain-/UI-GPU-Uploads gehören nicht zur neuen Sperre.

**Block D – Regressionen und Telemetrie:** Zwei neue C++ CTests für LOD-Dwell/Regionsreset und GPU-Upload-Gate/Slot-Zuteilung; ein Python-CTest prüft den Parser mit Legacylogs, gültigem JSON, defektem Snapshot und Sanitizing unbekannter Felder. Die regulären Textur-Streaminglogs führen nun `mip_dwell_pending`, `slot_priority_evictions`, `gpu_uploads`, `gpu_uploaded_bytes`; die Mesh-Streaminglogs führen `lod_dwell_pending`. Ein vollständiger C++23-Debug-Build und 48/48 CTests waren erfolgreich; nach der Versionspflege werden sie erneut und zusätzlich der Linux-GUI-Smoke geprüft.

**Grenzen:** Die aus echten Logs gewonnenen Zahlen beschreiben historische Verbindungs-/Asset-Fehler und keine bestätigte Beseitigung der 404/Decoderprobleme. Neuer Renderthread-Takt reduziert zeitliche Upload-Spitzen, garantiert aber keine fixe Framezeit. Weiterhin ausstehend sind authentifizierte In-World-Abnahme, vollständige Mesh-Objekte, Avatar-Body/Bewegung, Teleport und EventQueue-Fehlerbehebung.

**Nächster Release (3–4 Blöcke):** Datenbasierte Prüfung anhand neuer In-World-Logs und GPU-Diagnose, weitere Upload-Messungen, scene-priority-Feedback und gezielte Render-Integration. `projectplan.md` ist weiter maßgebend; bei ausdrücklicher Nutzerpräferenz Fehlerbehebung separat.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl67-live.log`. GitHub-Branch `develop-current`.

Neue Dateien: `include/nexverse/assets/asset_lod_dwell.hpp`, `asset_gpu_upload_gate.hpp`, `tests/asset_lod_dwell_tests.cpp`, `asset_gpu_upload_gate_tests.cpp`, `tools/ogl_asset_log_report.py`, `tests/test_ogl_asset_log_report.py`, `docs/LOG_ANALYSIS.md`; geändert: `src/app/application.cpp`, `CMakeLists.txt`.


## 0.2.0-alpha.68 – GPU-CPU-Metrik und Renderer-Integration · 11.10.2026

**Block A – reale CPU-Uploadzeit statt Worker-Wartezeit:** `AssetUploadTelemetry` misst die Wandzeit, die das Render-Thread-Aufrufen von `set_object_texture(...)` und `set_mesh_asset(...)` benötigt. Das feste Ringfenster hält die letzten 64 Uploadversuche und bietet `recent_avg_us`, `recent_p95_us`, `recent_max_us`, `recent_count`; Nullzustand und Überlauf werden konservativ behandelt. Messgröße ist **CPU-seitige OpenGL-Übermittlung**, keine GPU-Fertigstellungszeit, kein `glFinish` oder OpenGL-Timer-Query.

**Block B – adaptives GPU-Upload-Backpressure:** `AssetGpuUploadGate` bleibt bei schnellen Uploads bei mindestens 30 ms Abstand; langsame CPU-Übermittlungen verlängern das nächste Uploadfenster auf etwa 4× beobachtete Millisekunden, bis maximal 150 ms. Der nächstfolgende kurze Upload senkt das Intervall wieder auf den Grundwert. Mesh und Textur teilen sich den Renderthread-Gate. Bereits fertiggeladene Futures bleiben für spätere Frames verfügbar. Terrain/UI und vorbestehende Renderer-Arbeiten unterliegen dem Gate nicht.

**Block C – Szenenpriorisierung und erweiterte Health-Snapshots:** `AssetScenePriorityFeedback` beobachtet alle etwa 750 ms die GPU-gewünschten UUID-Sets, erkennt erhebliche Wechsel (symmetrische Differenz mindestens ein Viertel der vorherigen/neuen Set-Größe) und erhöht für drei stabile Planzyklen den weichen Loaded-Bonus temporär: Mesh von 24 auf 42 gewichtete Meter, Textur von 8 auf 14. Neue näher gelegene Assets bleiben weiterhin auswählbar; keine UUID wird fest gepinnt. Regionsepochen löschen den Zustand. `InWorldAssetSnapshot` behält `ogl.asset.streaming.v1`, ergänzt rein numerische Felder zu CPU-Uploadavg/p95/max, Sampleanzahl, effektiver Uploadsperre und Texture-/Mesh-Prioritäts-Churn. Der vorhandene Log-Analyzer wertet zulässige Felder aus und berechnet Peak-p95 bzw. Peak-Churn; er gibt keine CAPS-URLs/UUIDs aus.

**Block D – echter Renderer-Integrationstest:** `renderer_gpu_asset_integration_tests.cpp` erzeugt einen unsichtbaren echten GLFW/OpenGL-3.3-Kontext, lädt/repliziert/entfernt synthetische Textur und Mesh auf dem GPU-Pfad, prüft ungültige Meshdaten, führt einen Renderer-Frame aus, kontrolliert `visible_objects`, Renderer-Backend und GL-Fehlerfreiheit. Ohne OpenGL-Display überspringt CTest den Test via Exitcode 77; auf dem autorisierten Linux-PC wurde der Test unter `DISPLAY=:0` vollständig ausgeführt. Zwei weitere C++-Tests prüfen Rolling-Telemetrie und Scene-Prioritätsrückkopplung; erweiterte JSON-/Log-/Upload-Gate-Regressionstests. Gesamt: **51/51 CTests bestanden**, inkl. realem OpenGL-Test, anschließend vollständiger Versions-Rebuild/GUI-Smoke.

**Grenzen:** Der Renderer-Integrationstest arbeitet mit synthetischer Geometrie und einem lokalen GPU-Kontext, ohne authentifizierte NexVerse-Region oder echtes Asset. Er belegt noch nicht, dass Avatar-Body/Bewegung, Mesh-Objekte aus dem Simulator, CAPS-EventQueue, Teleport oder alle Weltassets korrekt sind. Diese bekannten Punkte bleiben bis zur vereinbarten späteren Bugfixphase dokumentiert. CPU-Uploadzeiten dürfen nicht als verifizierte GPU-Latenz bezeichnet werden.

**Nächster 3–4-Blöcke-Release:** Wiederholbare Smoke-/GPU-Profiling-Sessions anhand synthetischer Szenen unterschiedlicher Größe, Renderer-Upload-Fehlertransparenz, Scene-Priority-Schutz unter voller Asset-Residency und Ausbau der testbaren Welt-/Mesh-Renderpfade. Maßgeblich bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl68-live.log`. CLI-Logauswertung: `python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"`; Source Branch `develop-current`.

Neue Dateien: `include/nexverse/assets/asset_upload_telemetry.hpp`, `asset_scene_priority_feedback.hpp`, `tests/asset_upload_telemetry_tests.cpp`, `asset_scene_priority_feedback_tests.cpp`, `renderer_gpu_asset_integration_tests.cpp`. Angepasst: `asset_gpu_upload_gate.hpp`, `asset_streaming_snapshot.hpp`, `src/app/application.cpp`, Python-Logauswertung, CMake, Tests. Source/Docs gemeinsam nach GitHub `develop-current` pushen.


## 0.2.0-alpha.69 – GPU-Upload-Verifikation und Vollauslastung · 11.10.2026

**Block A – echter synthetischer GPU-Kapazitäts-/Belastungstest:** `tests/renderer_gpu_pressure_tests.cpp` erzeugt im versteckten OpenGL-3.3-Kontext auf dem Linux-Testrechner tatsächlich 512 kleine GPU-Objekttexturen und 256 kleine GPU-Meshes (Renderer-Grenzen). Bei 16, 128, 256 und 512 Texturen wird ein Frame mit den aktuell angelegten Szenenobjekten gezeichnet; Tests prüfen `visible_objects`, `draw_calls`, OpenGL-Fehlerfreiheit, abgewiesene 513. Textur/257. Mesh, korrekte Ersetzung bestehender Ressourcen sowie erneute Aufnahme nach Slot-Freigabe. Beispiel: unter dieser synthetischen Auslastung ca. 35 ms CPU-Gesamttestzeit inklusive Context-Init/Uploads/Draws. Diese Zeit ist ausdrücklich KEIN Benchmark realistischer größerer OpenSim-Assets.

**Block B – verlässliche GetTexture-GPU-Upload-Rückmeldung:** Die Renderer-Schnittstelle `Renderer::set_object_texture` meldet nun `bool` statt `void`. `LegacyOpenGlRenderer` baut Texturen immer erst in einem neuen GL-Namen auf, prüft Dimensionen und GL-Fehler, übernimmt sie nur bei erfolgreichem Upload und löscht den vorherigen Textur-Namen erst danach (transaktionale Ersetzung). Bei fehlerhaften Daten, vollem internen GPU-Slotlimit oder GL-Fehlern kehrt sie `false` zurück, ohne eine vorhandene Textur derselben UUID zu zerstören. Die Anwendung zählt `texture_gpu_upload_failures`, gibt eine Warnung aus, plant einen transienten Retry und trägt fehlgeschlagene Uploads nicht mehr als GPU-resident ein. Überlegungen zur atomaren Freigabe *anderer* prioritätsbedingt zuvor verdrängter Texturen bleiben ausdrücklich offen.

**Block C – robusterer GetMesh-GPU-Pfad:** Schon vor GPU-Submission prüft der Renderer Meshvertex-Stride, Dreiecks-Indexvielfache, jeden Index gegen die tatsächlich vorhandenen Vertices und angegebene Face-Bereiche gegen den Indexbuffer. Der eigentliche GL-Mesh-Upload kontrolliert Fehler und gültige VAO/VBO/EBO-Handles und gibt teilweise angelegte GL-Ressourcen bei Fehlern frei. Vorheriger LOD-Buffer bleibt bei gescheiterter Ersetzung erhalten. Viewer meldet `mesh_gpu_upload_failures`, klassifiziert GPU-Uploadfehler als transient und plant einen Backoff statt eines permanenten `invalid`-Fehlerstatus.

**Block D – Vollauslastung und Logdiagnose:** Der neue CTest `asset_full_residency_tests.cpp` übt 80 neue vorrangige Partikeltexturen bei 512 belegten Slots und eine Kamera-Fahrt über 400 Mesh-UUIDs bei 256 GPU-Slots aus. Er kontrolliert Priorität, Freigabe, Slotlimits und das Verbleiben naher/nützlicher Assets. `InWorldAssetSnapshot` führt zusätzliche Zähler `texture_gpu_upload_failures` und `mesh_gpu_upload_failures` im vorhandenen numerischen Schema `ogl.asset.streaming.v1`; der Python-Loganalysator nimmt sie in die sichere Feldauswahl auf und zeigt die jeweiligen Maximalwerte. JSON-, GL-Renderer- und Log-Regressionstests angepasst. C++23-Debug-Build und **53/53 CTests** inklusive zweier realer OpenGL-Kontexttests erfolgreich; vollständiger Release-Rebuild, Versionstest und GUI-Start folgen.

**Unverändert offene Grenzen:** Ein OpenGL-Test mit kleinen künstlichen Assets validiert Ressourcenhandling und GPU-Slots, aber weder echtes GetMesh/OpenSim-Decoding noch Live-Weltgeometrie, Avatar, Netzwerk-EventQueue, Teleport und In-World-Interaktion. Tests laufen auf echtem GL3.3/NVIDIA-Linux mit `DISPLAY=:0`; ohne GL-Display überspringen sich die beiden GPU-Kontext-CTests per Returncode 77. Shared GPU-Budget von 384 MiB ist eine Software-Schätzung ohne Terrain/UI/Treiber-Speicher. Die atomare Textur-Ersetzung gilt nur für denselben UUID-Slot, nicht für bereits vorab verdrängte andere Slots.

**Nächster Release / verbindlicher Plan:** Schrittweise GPU-Upload-Rollback/Recovery bei Slot-Evictions, realistischere Szenengeometrien/Texture-Mip-Lastprofile, strukturierte Upload-Failure-Gründe und weitere Renderer-Timing-Integration. Avatar/Bewegung/Mesh-/Teleport-/CAPS-Bugfixes wie vom Nutzer vereinbart separat.

Run: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Pressure einzeln: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-pressure-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl69-live.log`; nur `develop-current` nutzen.

Neu: `tests/renderer_gpu_pressure_tests.cpp`, `tests/asset_full_residency_tests.cpp`. Modifiziert: Renderer-Vertrag, OpenGL-Asset-Upload, `src/app/application.cpp`, CMake/Tests/Log-Analyser. Alle Änderungen als ein Release auf `develop-current` committen; keine alternativen Branches mergen.


## 0.2.0-alpha.70 – GPU-Textur-Transaktionen und Lastprofile · 11.10.2026

**Block A – texturschonender, transaktionaler GPU-Austausch:** `Renderer::replace_object_texture(uuid, image, evictions)` übernimmt ein komplett neues OpenGL-Textur-Handle erst nach erfolgreichem `glTexImage2D` und `glGenerateMipmap`. Erst danach werden die vorab ausgewählten Eviction-UUIDs gelöscht und die neue Textur eingebunden. Bei Fehlschlag bleiben alle bisherigen UUID-Slots, inklusive der Eviction-Kandidaten, erhalten. Der Viewer spiegelt die Freigaben erst nach erfolgreichem GL-Commit in `object_texture_loaded_ids`, `texture_gpu_bytes`, Mip-Tabellen und GPU-Bytezähler; abgelehnte Uploads zählen nicht als geladene Ressource. Der bisherige bool-Aufruf `set_object_texture` bleibt als Wrapper erhalten.

**Block B – strukturierte GPU-Uploadergebnisse für Texture UND Mesh:** `GpuUploadResult`/`GpuUploadFailure` enthält `success`, `failure`, `gl_error` mit Klassen `none`, `invalid_input`, `capacity`, `unsupported_dimensions`, `allocation`, `driver_error`. Beide Rendererpfade bieten strukturierte Methoden; der bisherige bool-Mesh-Aufruf ist ein Wrapper. Bei Texturen werden GL-Größenlimits sowie GL-Uploadfehler gemeldet, beim Mesh getrennt fehlerhafte Index-/Vertex-/Face-Daten, belegte 256 Mesh-Slots und sonstige Driver-Fehler. Application-Logs nennen nun `gpu_failure` (und bei Texturen `gl_error`), invalid/unsupported nutzt längeren invalid-Cooldown, transient/kapazitätsbedingt kurzzeitige Retries. Eine verlässliche OpenGL-Mesh-Fehlernummer wird im Renderer bei GL-Fehlern noch nicht als separates numerisches Feld exportiert und ist als Folgeschritt dokumentiert.

**Block C – reproduzierbare GPU-Upload-Lastprofile mit großen Assets:** Neuer CTest `renderer_gpu_profiles_tests.cpp` verwendet unsichtbaren ECHTEN OpenGL-3.3-Context und prüft 40 × 64², 20 × 256² und 6 × 1024² RGBA-Texturen sowie Mesh-Vertexgrids 16/32/64 (8/8/4 Instanzen, bis zu 8192 Dreiecke je Grid). Für jede Stufe werden GPU-Uploads, Renderframe, sichtbare Objekte und GL-Fehler geprüft; CPU-Upload-Zeiten (nicht GPU-Fertigstellungszeit) pro Gruppe protokolliert. Beispiel Linux RTX 4070 SUPER am 11.10.2026: Texture 283/369/4604 µs und Mesh 17/61/157 µs je Upload. Diese Zahlen gelten nur für synthetische Assets/diesen Testlauf, nicht für typische Serverdownloads oder FPS.

**Block D – Rollback-/Überbelegungs-Regressionen:** `renderer_gpu_pressure_tests.cpp` prüft weiterhin tatsächliche Belegung 512 Texturen/256 Meshes, jetzt zusätzlich Transaktions-Rollback bei ungültigem Input, fehlender Slotkapazität, unerlaubter Selbsteviction sowie übergroßen Texturen; erfolgreiches Commit entfernt exakt den vorgesehenen GPU-Victim und macht die neue ID resident. Vorhandene Mesh-Überbelegungs- und ungültige-Index-Tests prüfen die passende strukturierte Fehlerklasse. Insgesamt nach Implementierung **54/54 CTests bestanden**, inklusive drei echter OpenGL-Context-Tests; nach finaler Versionspflege erneut Full-Rebuild und GUI-Smoke.

**Wichtige technische Grenze:** Der Transaktionspfad hält den alten GPU-Textur-Slot und die neue Kandidatenressource kurzfristig gleichzeitig im VRAM. Bei sehr knappem physischem VRAM kann die sichere Transaktion scheitern und die alten Texturen behalten – ergebnisabhängig beabsichtigtes Schutzverhalten, aber später mit zusätzlicher Budgetreserve/Backpressure zu optimieren. Netzwerk-CAPS, echte Meshasset-/Avatar-/Bewegungs-/Teleportfehler wurden wie vereinbart nicht als bereits behoben deklariert. Der OpenGL-Integrationstest ist synthetisch, nicht authentifiziert in einer NexVerse-Region.

**Nächste vier Blöcke:** Strukturiertes Mesh-GL-Fehlerdetail, VRAM-Transientbudget/Reserve bei Rollback-Uploads, Instrumentierung echter Weltobjekt-Draws und Aufbau eines reproduzierbaren In-World-Akzeptanztests. `projectplan.md` bleibt verbindlich, und die ausdrücklich aufgeschobenen Avatar/Bewegungs/EventQueue-Bugfixes bleiben späteren Blöcken vorbehalten.

Start: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Profile: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-profiles-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl70-live.log`; nur `develop-current`.

Änderungen: Renderer `GpuUploadResult` und `replace_object_texture` plus `upload_mesh_asset`, `src/render/opengl_renderer.cpp`, `src/app/application.cpp`, CMake, GPU-Pressuretests. Neuer realer GL-Test `tests/renderer_gpu_profiles_tests.cpp`. GitHub `develop-current` ist der maßgebliche Release-Branch.


## 0.2.0-alpha.71 – tatsächliche Mesh-Drawcalls und nichtdestruktive GPU-Reserve

**Block A – konkrete GetMesh-OpenGL-Fehlernummern:** Der vorhandene GL-Uploadhelfer `upload_mesh` kann nun über einen optionalen `GLenum*` die tatsächlich gemeldete OpenGL-Fehlernummer an `Renderer::upload_mesh_asset` weiterreichen. `GpuUploadResult.gl_error` ist bei GL-Fehlern gesetzt; `GL_OUT_OF_MEMORY` wird als `allocation`, andere GL-Fehler als `driver_error` klassifiziert. Der Viewer protokolliert bei gescheiterten Mesh-Uploads `gpu_failure` und `gl_error` zusammen mit Retry-Delay. Kein GL-Fehler/fehlgeschlagene Handle-Anlage kann weiterhin `driver_error` mit `gl_error=0` ergeben; diese semantische Grenze ist beabsichtigt.

**Block B – transaktionale GPU-Textur-Staging-Reserve:** Neue reine C++-Hilfsfunktion `assets::gpu_staging_budget(texture_resident_bytes,mesh_resident_bytes,incoming_bytes,shared_budget_bytes)`. Sie prüft vor einer Kandidaten-Upload-Transaktion den kurzzeitigen geschätzten Höchstbedarf, während Altressourcen für Rollback noch auf der GPU liegen. Sicherheitsabstand: `min(16 MiB, shared_budget / 32)`; bei 384 MiB geteiltem Assetbudget also 12 MiB. Überlauf und Nullbudget werden sicher behandelt. Ein unzureichendes Staging-Budget führt zur nichtdestruktiven Deferral statt zum sofortigen Entfernen alter Texturen; zusätzliches Diagnosefeld `texture_gpu_staging_deferrals` und Warnlog mit numerischen Bytes. Die Angaben sind Software-Schätzungen, **keine** Garantie real freien VRAMs einschließlich Terrain/UI/Driver-Overhead.

**Block C – echte Weltobjekt-Renderzähler:** `RenderDiagnostics` liefert jetzt zusätzlich `scene_draw_calls`, `scene_mesh_draw_calls`, `scene_prim_draw_calls`. Zähler werden je Frame zurückgesetzt und ausschließlich beim tatsächlich ausgeführten glDrawElements aus der Weltobjekt-/Face-Schleife fortgeschrieben; Sky, Terrain, Wasser, Avatar-Proxies, UI zählen nicht dazu. Mesh-Zähler erfordern ein tatsächlich vorhandenes GPU-Mesh-Handle für die Mesh-ID, sonst zählt der tatsächlich gerenderte Fallback als Prim. `InWorldAssetSnapshot` enthält das letzte beendete Frame als `previous_frame_*`, wodurch In-World-Protokolle zwischen Asset-Residency und wirklichen Mesh-Draws unterscheiden. Synthetische Linux-OpenGL-Tests bestätigen Mesh UND Prim sowie die Summe aller Szene-Draws.

**Block D – reproduzierbarer In-World-Akzeptanzprüfer:** `tools/ogl_inworld_acceptance.py` wertet sanitizte Berichte des bestehenden `ogl_asset_log_report.py` aus; erforderlich sind mindestens 3 **aufeinanderfolgende Snapshots derselben Regionsepoche**, aktive Region, nichtleere gewünschte Textur-/Meshsets, mindestens 75 % Coverage beider Familien und ein tatsächlich ausgeführter Scene- sowie GPU-Mesh-Draw im neuesten Snapshot. Nur dann liefert die CLI `evidence_pass`/Exitcode 0; ansonsten `not_verified`/Exitcode 2. JSON liefert pro Kriterium booleschen Prüfstatus; keine UUID/CAPS-URL wird ausgegeben. Zwei zusätzliche CTests für GPU-Reserve und Akzeptanzlogik, erweitert um Regionsepochenwechsel, ergänzen die Renderer-/Snapshot-Regressionen. C++23-Debug und 56/56 CTests wurden nach Implementierung erfolgreich geprüft.

**Lokale Ist-Daten / korrekte Abgrenzung:** Die autorisierte Datei `$HOME/.local/state/opengenesislink/viewer.log` enthielt am 11.10.2026 weiterhin 0 valide `InWorldAssetSnapshot`-Zeilen; der neue Akzeptanzprüfer lieferte folgerichtig `not_verified` mit Exitcode 2. Die Datei verzeichnet historische 230 Mesh- und 56 Texture-Fehler (42 mit 404-Muster, eine mit 5xx-Muster); daraus kann keine aktuelle in-world Rendering-Freigabe abgeleitet werden. Auch `evidence_pass` allein würde Avatarsteuerung, Teleport und EventQueue **nicht** bestätigen. Diese Problemfelder bleiben wie vereinbart in einer späteren Bugfixphase.

**Nächste zusammenhängende Releaseblöcke (alpha.72):** Verbesserte In-World-Snapshot-Serienvalidierung mit Zeitstempeln, konkreter Renderer-Frame-Mesh-Face-Abnahme, GPU-Staging-Deferral-Lebenszyklus/Recovery sowie weiteres Mesh- und Texture-Streaming-Feedback. `projectplan.md` bleibt verbindlich.

CLI: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`; CMake: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Branch ausschließlich `develop-current`.

Neu: `include/nexverse/assets/gpu_staging_budget.hpp`, `tests/gpu_staging_budget_tests.cpp`, `tools/ogl_inworld_acceptance.py`, `tests/test_ogl_inworld_acceptance.py`. Geändert: Renderer, Application, Snapshot, Python-Loganalyse, CMake und GL-/Snapshot-Tests. Endstand nur dann auf GitHub `develop-current` sichern, wenn Release-Rebuild + alle Tests + GUI-Smoke bestanden.


## 0.2.0-alpha.72 – Akzeptanzserien und GPU-Streaming-Recovery

**Block A – belastbare Snapshotserien statt altem Log-Zufall:** `AssetStreamingSnapshot` erzeugt bei `simulation_ready` ergänzend `session_started_unix_ms` (Prozesskennung als Startzeit), `emitted_unix_ms` und monoton steigende `sequence`; Schema `ogl.asset.streaming.v1` bleibt additive-only. Der Parser `tools/ogl_asset_log_report.py` zählt als neueste zusammenhängende Serie nur Snapshots derselben Session UND Regionsepoche mit lückenloser Sequenz und realistischem zeitlichen Abstand 15–90 s. Der CLI-Akzeptanzprüfer `tools/ogl_inworld_acceptance.py` nutzt `ogl.inworld.acceptance.v2` und prüft zusätzlich den Echtzeitstatus: letzter Snapshot maximal 180 s alt (10 s Uhr-Toleranz), 3 zusammenhängende Snapshots, nichtleere Desired-Textur- und Mesh-Sets, jeweils mindestens 75 % Residency und tatsächlich ausgeführte Scene- bzw. Mesh-Draws. `--max-age-seconds`, `--min-snapshots`, `--min-coverage` sind konfigurierbar. Ohne die neuen Zeitfelder oder bei historischen Logs ist das Ergebnis absichtlich `not_verified` (Exit 2), kein erfundener Liveerfolg.

**Block B – realer GPU-Mesh-Face-Nachweis:** OpenGL-RenderDiagnostics unterscheidet nun die erfassten `scene_mesh_draw_calls` (wirklich hochgeladenes Mesh) und `scene_missing_mesh_draw_calls`: Letztere zählen echte Prim-Fallback-Draws für Objekte, die eine Mesh-UUID anfordern, aber keine passende GPU-Meshressource besitzen. Ein erweiteter echter GLFW/OpenGL-3.3-Test lädt 2 getrennte Face-Indexbereiche, prüft exakt 2 GPU-Mesh-Drawcalls plus Prim-/Missing-Mesh-Fallback sowie den Frame-Reset nach dem Entfernen der Meshressource. Das In-World-Snapshotfeld `previous_frame_scene_missing_mesh_draw_calls` und der sanitized Log-Report `peak_missing_mesh_fallback_draws` liefern eine konkrete Anzeige der bisher fehlenden Meshobjekte, aber noch keine Fehlerbehebung des OpenSim-Meshdownloads.

**Block C – GPU-Staging-Deferrals werden wieder freigegeben:** `GpuStagingRecovery` speichert zurückgestellte Textur-UUIDs mit dem zugehörigen geschätzten Texture-/Mesh-Byteverbrauch und kontrollierter Cooldownzeit. Ein erneuter Versuch wird ermöglicht, sobald eine der beiden Assetfamilien weniger GPU-Speicher benötigt oder der Retry-Zeitpunkt erreicht ist; wiederholte Deferrals verwenden je UUID steigende 2/4/8/16/32/64-s-Cooldowns, ohne beim Freigeben der Retry-Anfrage die Historie zu verlieren. Erfolgreiche Uploads löschen die Historiereihe; bei Regionswechsel Reset; bei geänderter Desired-Residency verschwundene Assets werden bereinigt. Laufende Worker werden nicht zwangsweise unterbrochen. `texture_gpu_staging_waiting` / `texture_gpu_staging_released` und die bereits vorhandenen Deferrals gehen in In-World-JSON/Streaminglogs und den sicheren Report ein.

**Block D – Regressionen/Streaming-Feedback:** Ein neuer C++-CTest prüft Memory-Drop-Retry, Cooldown-Aufstieg, Recovery, Resolution, Prune und Region-Reset. Der Python-Akzeptanz-CTest prüft fehlende, veraltete und synthetische valide Serien sowie Sessionwechsel, Epochensprünge, Sequenzlücken und unrealistische Abstände; die Renderer-Integration prüft die Multi-Face-Geometrie, tatsächliche GL-Zeichnungen und fehlende Mesh-GPU-Residency. Erwartet sind 57/57 CTests inkl. dreier echter GL-Context-Tests. Die numerische Log-Whitelist wird um die neuen Snapshotfelder erweitert, ohne Asset-UUIDs/CAPS-URLs in Ausgaben aufzunehmen.

**Lokaler Status am 11.10.2026:** Die autorisierte `$HOME/.local/state/opengenesislink/viewer.log` enthält noch 0 neue `InWorldAssetSnapshot`-Zeilen. Der überarbeitete CLI-Prüfer liefert `not_verified`/Exitcode 2. Das ist technisch korrekt und darf nicht als abgeschlossene Live-NexVerse-Regions-, Avatar-, Mesh- oder Bewegungsabnahme dargestellt werden. Bekannte EventQueue-/Teleport-/Avatarprobleme bleiben wie gewünscht auf dem späteren Bugfix-Pfad. Außerdem sind GPU-Bytegrenzen nur Software-Schätzungen und keine Treiber-VRAM-Garantie.

**Folgerelease Alpha.73 / 3–4 Blöcke:** GPU-Streaming-Retry-Integration und Budget-Pressure-Regressionen mit realem GL-Kontext, weitere Missing-Mesh-/Face-Fallback-Statistik, wiederholbare Langzeit-OpenGL-Szenentests sowie echte In-World-Log-Akzeptanz sobald Regionenzugang/Assets bereit sind. Maßgeblich ist weiter `projectplan.md`.

CLI auf dem autorisierten Linux-PC: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"` (Exitcode 0 nur für neueste gültige Live-Daten). Branch ausschließlich `develop-current`.

Neu: `include/nexverse/assets/gpu_staging_recovery.hpp`, `tests/gpu_staging_recovery_tests.cpp`; weitere Änderungen in Renderer/Application/AssetStreamingSnapshot/Python-CLI-CMake-Tests. Lokaler/Tracking/GitHub-Stand nach dem Push kontrollieren. `projectplan.md` maßgeblich.


## 0.2.0-alpha.73 – GPU Full-Slot Streaming & Mesh-Fallback-Profiling · 11.10.2026

**Block A – vollständig belegte GetTexture-GPU-Slots korrekt behandeln:** Die Demand-Anfrageprüfung vor HTTP blockierte bisher jede neue Asset-UUID, sobald bereits 512 Texturen GPU-resident waren, obwohl die bestehende `plan_texture_slot_admission` danach bereits eine niedriger priorisierte Textur verdrängen kann. In `src/app/application.cpp` wird bei voller Tabelle jetzt eine rangbewusste Preflight-Slotzulassung (gewünschte und zur Räumung markierte Resident-UUIDs) durchgeführt; nur unmögliche Transfers werden deferred. Nach dem Download prüfen Bytebudget, Staging-Reserve, Slotpriorität und transaktionaler Renderer-Commit weiterhin die tatsächliche Machbarkeit. Unit-Test erweitert für 512 belegte Slots und die Nichtverdrängung höher priorisierter Objekte.

**Block B – tatsächlicher OpenGL-Soak-Test mit GPU-Pressure und Recovery:** `tests/renderer_gpu_soak_tests.cpp` erstellt einen versteckten echten OpenGL-3.3-Kontext mit 24 Weltobjekten, davon 20 gültige GPU-Meshes und vier bewusst nicht hochgeladene Mesh-UUIDs, plus acht echte GPU-Texturen. In 160 gerenderten Frames ersetzt es wiederholt Texturen, ersetzt GPU-Meshes und kontrolliert pro Frame GL-Fehlerfreiheit, korrekte Drawcall-Summe, vorhandene GPU-Mesh-Draws und stabile Zahl fehlender Meshobjekte. 40 gezielte *simulierte* Software-Staging-Druckereignisse werden mit `gpu_staging_budget`/`GpuStagingRecovery` freigegeben; 160 Transaktionen tatsächlich im Renderer ausgeführt. Beispiel auf der Linux RTX 4070 SUPER: 43 ms CPU-Laufzeit für diese kleine synthetische Szene, kein FPS- oder realistischer Simulatorperformance-Benchmark. Ohne GL-Kontext überspringt sich der CTest (Exit 77).

**Block C – präzise Mesh-Fallback-Metriken:** Der Renderer zählt zusätzlich zu bisherigen pro Frame ausgeführten Missing-Mesh-Fallback-Drawcalls jetzt **einzigartige betroffene Szenenobjekte** und die wirklich als Ersatz gezeichneten **Dreiecke**. Da eine Region pro Objekt mehrere Materialfaces und transparente/opaque Renderpasses besitzt, vermeidet ein Frame-lokales Set eine Überzählung der Objekte. Die numerischen `InWorldAssetSnapshot`-Keys `previous_frame_scene_missing_mesh_objects` und `previous_frame_scene_missing_mesh_triangles` sowie der sanitizte Logbericht zeigen das Ergebnis. Reale OpenGL-Multi-Face- und Mesh-Entfernungs-Tests prüfen diese Zähler; fehlende OpenSim-Meshdownloads werden damit diagnostiziert, nicht behauptet repariert.

**Block D – verschärfte In-World-Abnahme und Streaming-Regressionen:** `tools/ogl_inworld_acceptance.py` verlangt nun standardmäßig null zuletzt tatsächlich gerenderte Missing-Mesh-Fallback-Draws, zusätzlich zu frischer Session-/Epoch-/Zeitserie, nichtleerer Region, >=75 % Texture- und Mesh-Residency und tatsächlichen GPU-Mesh-Draws. Nur für bewusst teilweise bereite Szenen kann der Aufrufer `--max-missing-mesh-draws N` setzen. Fehlende Diagnosefelder gelten nicht als null und können keinen Pass erzeugen. Python-Cases prüfen strikten Status und Override; C++-Regressionen GPU-Full-Residency/Byte-Slot-Vorabzulassung/Snapshot werden mit GL-Tests ausgeführt. Insgesamt **58/58 CTests bestanden** (erster vollständiger Entwicklungsbuild).

**Abnahmegrenze:** Auf der autorisierten Linux-Workstation ist die frühere `viewer.log` weiterhin ohne aktuelle In-World-Snapshotserie (`not_verified`), insbesondere ohne Prüfung echter NexVerse-Mesh-, Avatar- und Simulatorinteraktionen. Synthetische Tests ersetzen keinen erfolgreichen Simulatorbeitritt. Seit Alpha.70 bewusst später zu beheben: fehlender Avatar, WASD/Bewegung, Teleport, CAPS EventQueue, teilweise fehlende Meshobjekte. Die aktuelle Arbeit führt den maßgeblichen `projectplan.md` ohne ungefragte Fehlerbehebungsphase fort.

**Nächste 3–4 Releaseblöcke Alpha.74:** Zeitliche Priorität und aktive GPU-Budget-Deferral-Diagnose bei geteiltem Tex-/Mesh-Speicher, Wiederverwendung von Assets über Regions-/LOD-Wechsel, erweitertes realistisches GL-Szenen-/Materialtestprofil und automatische Prüfung wirklich neuer Regionlogs sobald echte Server- und Loginbedingungen erfüllt sind.

Lokaler Debug/Soak: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-soak-tests`. In-World-Prüfer: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repo `marlin198323-glitch/nexverse-viewer`, einzig maßgeblicher Branch `develop-current`.

Neu: `tests/renderer_gpu_soak_tests.cpp`; Änderungen: `src/app/application.cpp`, `src/render/opengl_renderer.cpp`, `include/nexverse/render/renderer.hpp`, `asset_streaming_snapshot.hpp`, Python-Analyzer/Acceptance, CMake und GL-/Snapshot-/Residency-Tests. Wiedereinstieg über `projectplan.md`; lokalen/Tracking/GitHub-Commit vergleichen.


## 0.2.0-alpha.74 – Kombiniertes GPU-Budget und CPU-Asset-Reuse · 11.10.2026

**Block A – geteiltes Tex-/Mesh-GPU-Budget und Mesh-Retry-Recovery:** Mesh-Anfragen, deren dekodierte GPU-Geometrie das kombinierte Budget überschreitet, werden nicht mehr unbegrenzt allein durch `mesh_gpu_budget_deferred_ids` blockiert. Neuer `GpuStagingRecovery` auch für Meshes, mit Key `UUID.LOD`, progressivem 2/4/8/16/32/64-s-Cooldown und vorgezogener Re-Evaluierung nach sinkendem Texture- oder Mesh-Residency-Byteverbrauch. Bei Regionswechsel Reset, bei geänderter Scene-Desired-Liste Prune, nach erfolgreichem Mesh-Upload Resolution. Dieselbe Recovery wird nun auch bei nicht passendem Texture-Byte-/Slotplan verwendet, nicht nur bei Staging-Reserveverletzung. Numeric Snapshotfelder `mesh_gpu_budget_waiting`, `mesh_gpu_budget_released`, plus bestehende Textur-Deferrals. Risiken der nur geschätzten (Terrain/UI/Driver nicht eingeschlossenen) GPU-Bytegrenzen bleiben.

**Block B – tatsächliche wiederverwendete CPU-Assets, keine falsche GPU-Hot-Reuse-Behauptung:** Die bisherigen Mesh-/Texture-RAM- und Disk-Caches bleiben regionsübergreifend im Speicher bzw. auf Festplatte bestehen; GPU-Handles werden aus Sicherheitsgründen weiterhin bei Regionswechsel geräumt. Der neue begrenzte `AssetReuseHistory` (max. 4096 UUIDs/Familie) beobachtet ausschließlich erfolgreiche erneute GPU-Uploads und zählt sie als `cached_region_reuse` oder `cached_lod_swap` NUR, wenn der Worker ausdrücklich einen RAM-/Disk-Cache-Hit meldete. Texture-Mipqualität und Mesh-LOD werden pro UUID erfasst, unabhängig vom Cache-Tier. Snapshotfelder für `texture_cpu_cached_region_reuse`, `texture_cpu_cached_lod_reuse`, `mesh_cpu_cached_region_reuse`, `mesh_cpu_cached_lod_reuse`; gleichartige Streaming-Logs. Keine Gewähr, dass dieselbe UUID bei Regionwechsel auf einem fremden Server existiert oder identische GPU-Handles weiterverwendet werden.

**Block C – langlebiger realer OpenGL-Material-/Szenentest:** Der bereits echte GLFW/OpenGL-3.3-Soak mit 160 Frames, 160 transaktionalen Texture-Replacements, 40 simulierten Staging-Deferrals, 20 GPU-Meshes plus vier absichtlich fehlenden Meshobjekten testet jetzt abwechselnd opake und teiltransparente Texturen, per-Face-Alpha-Tints, Fullbright, Shininess sowie Repeat-/Offset-Materialparameter. Szenenmaterial wird in jedem Frame aktualisiert. Echte OpenGL-Frames, `scene_mesh_draw_calls`, `scene_prim_draw_calls`, `scene_missing_mesh_objects`, GL-Fehlerfreiheit und GPU-Assetreplacements werden kontrolliert. Das ist eine synthetische GPU-/Legacy-Material-Regressionsprüfung, keine vollständige PBR- oder NexVerse-InWorld-Visualabnahme.

**Block D – End-to-End GPU-Pressure/Queue-Diagnostik und reproduzierbare Tests:** Neuer CTest `asset_gpu_pressure_pipeline_tests.cpp` simuliert gemeinsam beanspruchtes 384-MiB-Softwarebudget: eine blockierte Mesh-Anfrage wird unmittelbar nach 12-MiB-Texturfreigabe wieder möglich, Staging schützt alte GPU-Assets, Textur-Cooldown läuft ab, Regionswechsel leert Deferrals, RAM-/Disk-Hit bleibt auf Cacheebene für folgende Region nachvollziehbar. Neuer Unit-Test `asset_reuse_history_tests.cpp` kontrolliert bounded History, Regionswechsel, Qualitätswechsel und Nicht-Cache-Fälle. Der Log-Analyser nimmt neue numerische Snapshotfelder in die Whitelist auf und gibt Spitzenwerte der mesh-budget-Warteschlange frei, ohne UUIDs/CAPS-Adressen/Secrets auszugeben; Python-Test prüft dies. Damit **60/60 CTests** im ersten vollständigen Debug-Testlauf.

**Lokale Live-Abgrenzung 11.10.2026:** Der unverändert mit der autorisierten Workstation vorliegende `$HOME/.local/state/opengenesislink/viewer.log` enthält noch keine gültigen `InWorldAssetSnapshot`-Serien; die CLI `tools/ogl_inworld_acceptance.py` ergibt `not_verified`/Exitcode 2 (0 geeignete Snapshots). Keine verifizierte Aussage zur vollständigen NexVerse-Welt, Avatar-Body, WASD/Bewegung, Teleport, fehlenden realen Meshobjekten oder EventQueue. Diese bereits bekannten Themen verbleiben auf der ausdrücklich später geplanten Fehlerbehebungsphase.

**Nächster geplanter Stand alpha.75:** Noch gezielteres CPU/GPU-Cache-Residency-Management mit eindeutiger Kennzeichnung von HTTP-/RAM-/Disk-Treffern und anhaltender Budgetlast, stärkere GL-Legacy-Material-/Multi-Face-Szenen, prüfbare Wiederverbindung nach Regionenwechsel und aussagekräftige Auswertung echter In-World-Testlogs, sobald Authentifizierung und Regionsdaten verfügbar sind. `projectplan.md` bleibt die verbindliche Roadmap.

Repo `marlin198323-glitch/nexverse-viewer`, aktiver Branch `develop-current`, lokale Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Standard: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; anschließend GUI-Smoke und GitHub-SHA-Vergleich.

Neue Quellen `include/nexverse/assets/asset_reuse_history.hpp`, `tests/asset_reuse_history_tests.cpp`, `tests/asset_gpu_pressure_pipeline_tests.cpp`; Hauptintegration `src/app/application.cpp`; erweitert `tests/renderer_gpu_soak_tests.cpp`, `asset_streaming_snapshot.hpp`, `tools/ogl_asset_log_report.py` und CMake.


## 0.2.0-alpha.75 – LRU RAM/Disk-Reuse, GPU-Deferred- und GL-Regiontest · 11.10.2026

**Block A – deterministic bounded CPU-Asset-Reuse (RAM vs Disk):** `include/nexverse/assets/asset_reuse_history.hpp` was changed from arbitrary `unordered_map.begin()` eviction to real LRU with a `std::list` and O(1)-average lookup/touch/evict. The 4096 UUID/family limit is maintained. New `AssetReuseHistory::Tier {network,memory,disk}` distinguishes actual reported successful memory/disk-backed GPU reuploads at a later RegionEpoch or different texture mip / mesh LOD; counters `region_ram_hits`, `region_disk_hits`, `lod_ram_hits`, `lod_disk_hits` and `evictions`, plus prior combined hit counts. Application maps the texture loader's explicit `RAM texture cache hit` / `Disk texture cache hit` and mesh `MeshCacheTier` into this tier. New numeric `InWorldAssetSnapshot` and safe log parser fields distinguish the cache provenance (including history LRU evictions). This is CPU-/disk-cache reuse; GPU textures and VBOs remain invalidated on region changes.

**Block B – targeted GPU Deferred/Retry lifecycle:** `GpuStagingRecovery::synchronize(blocked, desired)` prunes obsolete blocked UUIDs in lockstep with active desired sets, **preserving cooldown for still-required UUIDs** and unblocking orphans that have no recovery deadline; a corresponding regression test verifies this. Texture plan changes no longer indiscriminately clear all deferred IDs; specific changed mip-levels release their own state, and full-slot preflight rejection explicitly adds a timed recovery record. Mesh desired-Lod set changes use synchronized `UUID.LOD` deferred keys rather than a blanket clear. Existing actual GPU-memory-eviction code can still trigger early retry, and its software VRAM estimates still exclude other driver allocations. No unauthenticated network or EventQueue behavior is asserted fixed.

**Block C – real native GL3.3 three-face region transition test:** New `tests/renderer_region_transition_tests.cpp` creates a hidden actual GLFW/OpenGL 3.3 context. It uploads three independently materialized faces, including opaque texture, blended tint and alpha-textured face. It verifies three GPU mesh Face-Draws and no missing-Mesh fallback. It then simulates a region change by clearing all GPU meshes and textures, validates correct Prim fallback without any stale GPU mesh, and reuploads the SAME immutable UUID with refreshed texture bytes; a new rendered frame must again show 3 GPU Mesh Face-Draws and 0 missing mesh objects. All stages check `glGetError()`; skip code 77 if no GL context. The test is synthetic, not a real authenticated OpenSim region reconnect.

**Block D – series-wide latest-session In-World acceptance:** The log analyzer now computes only from the most recent contiguous valid `(session_started_unix_ms, region_epoch, sequence, 15–90 s interval)` series `latest_region_min_texture_coverage_pct`, `latest_region_min_mesh_coverage_pct`, `latest_region_min_mesh_draw_calls`, `latest_region_max_missing_mesh_fallback_draws` and `latest_region_max_mesh_budget_waiting`. `tools/ogl_inworld_acceptance.py` now rejects an apparently healthy latest frame if the current series contains an earlier low-coverage frame, absent required mesh-draw evidence, or missing-mesh fallback. Missing fields result in `not_verified`, not assumed zeros. New dedicated `tests/test_ogl_inworld_transition_report.py` tests healthy series, degraded middle frame, missing mesh in earlier frame, missing telemetry field and session/epoch boundaries. End-to-end C++ / OpenGL and Python regression count **62 CTests**, all passed before final version rebuild.

**Proven live status:** The authorized workstation's `$HOME/.local/state/opengenesislink/viewer.log` still contains **zero** valid current `InWorldAssetSnapshot` samples; acceptance returns `not_verified` with exit code 2. It is not possible to claim a real NexVerse Region/Avatar/Movement/Teleport/EventQueue acceptance from the GL tests or these logs. Those user-deferred issues remain in the project roadmap for later troubleshooting.

**Next large release alpha.76 (3–4 blocks):** cache-pressure eviction and persistent asset-tier observations, region-switch cancellation and stale HTTP event contract tests (without assuming live simulator connectivity), wider material/texture transparency scenario coverage, explicit failure diagnosis of missing Real Mesh UUIDs once real region snapshots exist. Keep the planned staged sequence in `projectplan.md`; don't silently move debugging of WASD, avatar, Teleport or CAPS queue ahead.

Build: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Live check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Private GitHub repo `marlin198323-glitch/nexverse-viewer`, branch ONLY `develop-current`.

Changed `asset_reuse_history.hpp`, `gpu_staging_recovery.hpp`, `asset_streaming_snapshot.hpp`, `src/app/application.cpp`, `tools/ogl_asset_log_report.py`, `tools/ogl_inworld_acceptance.py`, CMake and regression tests. New tests: `tests/renderer_region_transition_tests.cpp`, `tests/test_ogl_inworld_transition_report.py`.


## 0.2.0-alpha.76 – RAM-Cache-Druck, verspätete Regionen-HTTP-Antworten und Materialtests

**Block A – echte CPU-RAM-Cache-Belastungstransparenz:** Die beiden bestehenden LRU-Caches `TextureMemoryCache` und `MeshMemoryCache` erfassen nun direkt `hits`, `misses`, `evictions`, `bytes` und `entries`. Der Zugriff auf die Statistiken ist mutexgeschützt, die Evictionzählung erfolgt ausschließlich beim wirklichen Verdrängen eines älteren Eintrags. Ein neuer gemeinsamer Header `asset_memory_cache_stats.hpp` hält das numerische Messformat. Der Viewer überträgt je Familie Treffer, Fehlzugriffe und Verdrängungen in `InWorldAssetSnapshot` (`texture_ram_hits`, `texture_ram_misses`, `texture_ram_evictions`, `mesh_ram_hits`, `mesh_ram_misses`, `mesh_ram_evictions`). Der sanitizte Python-Logbericht berücksichtigt die Felder und berechnet `peak_texture_ram_evictions` / `peak_mesh_ram_evictions`. Vorhandene CPU-RAM-Cachetests prüfen die neuen Zähler und Speichergrenzen.

**Block B – kontrolliert verspätete HTTP-Antwort beim Regionswechsel:** Neuer nativer Test `tests/region_http_epoch_contract_tests.cpp`: Eine asynchrone, simulierte GetMesh-HTTP-Antwort wird nach Beginn des Requests in Region A gezielt zurückgehalten. Nach dem Übergang zu B wird die alte Antwort freigegeben und muss an der vorhandenen Epochensperre verworfen werden, ohne die Mesh-Disk-/RAM-Caches zu befüllen. Anschließend wird ein neuer Request im aktiven B akzeptiert. Auch der Rückwechsel B → A erhält eine neue Epoche und erlaubt einen authentifizierten Cachetreffer, ohne dass die alte Antwort wieder gültig wird. Dies prüft reale asynchrone Steuerlogik mit kontrolliertem Transportcallback und existierender `RegionMaterialScope`/`load_mesh_cached`-Integration, jedoch keinen echten Login/HTTP-Endpunkt.

**Block C – umfangreicherer nativer OpenGL-3.3-Material-/Multiface-Test:** `renderer_region_transition_tests.cpp` prüft weiterhin GPU-Freigabe und Wiederaufbau derselben Mesh-UUID nach einem Regionswechsel. Ergänzend rendert der echte versteckte GLFW/OpenGL-3.3-Kontext **48 Frames mit zwei Objekten und jeweils drei Mesh-Faces**. Face-Tint/Alpha, transparente Textur, Fullbright, Glow, UV-Rotation, UV-Repeat/Offset und Planar-Texturmodus wechseln. Pro Frame müssen sechs GPU-Mesh-Face-Draws, null Missing-Mesh-Objekte, konsistente Drawcall-Summen und GL-Fehlerfreiheit vorliegen. Diese synthetischen Renderer-Regressionen prüfen technische Pfade, nicht die visuelle Pixelgenauigkeit einer realen OpenSim-Region.

**Block D – kategorisierte Missing-Mesh-/GetTexture-Ausfälle:** Neues `AssetFailureTelemetry` mit numerischen Zählern `missing`, `transient`, `invalid`, `configuration`, ohne UUID/CAPS-Adresse. Der reale Applikationspfad zählt kategorisiert fehlgeschlagene Texture- und Mesh-Downloads bzw. GPU-Rejections (mit bestehenden AssetFailure-Regeln); bei erfolgreichen GPU-Uploads wird keine Fehlerzählung erhöht. `InWorldAssetSnapshot` ergänzt `mesh_fail_{missing,transient,invalid,configuration}` und `texture_fail_{missing,transient,invalid,configuration}`. Der vorhandene sichere Python-Analyzer nimmt die Felder auf und meldet `max_mesh_missing_events`, `max_mesh_transient_events`; Tests kontrollieren Kategorieabbildung, Snapshotformat und bestehende Cacheauswertung. Zusammen **64/64 CTests** im Entwicklungsbuild, darunter native GLFW/OpenGL-Tests und der neu hinzugefügte zeitversetzte Regions-HTTP-Test.

**Bewertungsgrenzen / Ist-Status:** Ein Simulator-HTTP-404 / 410 wird getrennt als `missing` dokumentiert, Retry-/Serverfehler als `transient` und Fehlkonfiguration separat. Diese Kategorien erlauben später eine gezielte Ursachenanalyse der realen fehlenden Meshobjekte, beheben aber noch nicht den OpenSim-Netzwerkdownload oder die CAPS-EventQueue. Der autorisierte echte lokale `viewer.log` enthält am 11.10.2026 weiterhin 0 geeignete `InWorldAssetSnapshot`-Zeilen; In-World-Prüfer meldet `not_verified`/Exitcode 2. Ohne authentifizierte Region kann weder Avatar, Bewegung, Teleport noch die Bildqualität vollständig abgenommen werden. Diese bekannten Probleme bleiben wie vereinbart für die spätere Bugfixphase vorgesehen.

**Nächste 3–4 koordinierte Blöcke für Alpha.77:** Auswertung von Cachemiss-Raten und Prioritätskontrolle unter längerer Streaminglast, zuverlässige stale-Response-Trennung auch für Texturdownloads/Prefetch in isolierten Regressionstests, Mesh-Face-/Material-Transparenz-Routing samt GPU-Speicherlast und Diagnose tatsächlicher OpenSim-Regiondaten sobald verfügbar. Der verbindliche `projectplan.md` bestimmt weiterhin die Reihenfolge.

Build/Test auf der autorisierten Workstation: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.

Neu: `asset_memory_cache_stats.hpp`, `asset_failure_telemetry.hpp`, `region_http_epoch_contract_tests.cpp`, `asset_failure_telemetry_tests.cpp`. Geändert: echte Mesh-/Texture-RAM-Cache-Implementierungen, `src/app/application.cpp`, `asset_streaming_snapshot.hpp`, GL-Regionstest, Cachetests, CMake, Analyzer. Alle Dateien gemeinsam nach `develop-current` committen.


## 0.2.0-alpha.77 – Cache-Pressure, GetTexture-Regionguard und GPU-Alpha-Faces

**Block A – 30-s-Intervallanalyse echter RAM-LRU-Belastung:** Neues `AssetCachePressureWindow` berechnet nicht nur kumulative, sondern delta-basierte Hitquote und Anzahl echter Evictions pro Asset-RAM-Cache-Sampling-Intervall. `under_pressure` wird nur bei mindestens 8 neuen Cache-Misses, mindestens 2 Verdrängungen und einer Trefferrate unter 50 % aktiviert. Der Renderer beobachtet Textur- und Mesh-RAM-Cache getrennt und veröffentlicht `texture_ram_hit_rate_pct`, `texture_ram_interval_evictions`, `texture_ram_pressure` und entsprechende `mesh_*`-Werte in `ogl.asset.streaming.v1`. Bei aktivem Texturcache-Druck setzt das freiwillige **Disk-only-Texture-Prefetching** aus, während GetTexture-Anforderungen benötigter sichtbarer Weltassets weiterhin starten dürfen. Regionswechsel resetten die Fenster. Cache-Pressure-CTest deckt Druck, Recovery, Nulländerung und Counter-Reset ab; die Schwellwerte sind konservativ und noch nicht live kalibriert.

**Block B – GetTexture-Worker mit überprüfbarer Regionsantwortsperre:** Die bisher nur in `src/app/application.cpp` kopierte RAM/Disk/HTTP-Mip-Laderoutine ist in `include/nexverse/assets/texture_cached_request.hpp` als `load_texture_cached_scoped` extrahiert; die App verwendet diese unmittelbar mit dem existierenden authentifizierten `TextureAssetLoader`, den CPU-/Disk-Caches, gewünschtem Mip und dem atomischen Regionsepochen-Accept-Callback. Vor HTTP und nach einer Antwort wird die Epoche kontrolliert; eine verspätete HTTP-Antwort wird **vor** der RAM-/Disk-Cache-Übernahme verworfen. Zusätzlich wird eine vom angefragten UUID abweichende erfolgreiche Antwort als `invalid` abgelehnt. Der neue native CTest `texture_http_epoch_contract_tests.cpp` hält eine simulierte HTTP-Antwort gezielt bis nach dem A→B→C-Wechsel zurück, prüft keine stale Cache-Befüllung, erfolgreiche Antwort der aktuellen Region, GPU-Mip-reduziertes Arbeitsergebnis bei weiterhin voller RAM-Assetauflösung, RAM- und Disk-Treffer. Das ist ein kontrollierter Transportvertragstest, keine externe Netzabnahme.

**Block C – echte OpenGL Opaque-vs-Alpha-Face-Telemetrie:** `RenderDiagnostics` zählt bei tatsächlich ausgeführten Szene-`glDrawElements` nun getrennt `scene_opaque_draw_calls` und `scene_alpha_draw_calls`, die sich genau auf `scene_draw_calls` summieren müssen. Beide Zahlen werden beim Frame-Beginn zurückgesetzt und als `previous_frame_scene_opaque_draw_calls` / `previous_frame_scene_alpha_draw_calls` in In-World-Health-Snapshots aufgenommen. Die echten GLFW/OpenGL-3.3-Tests `renderer_region_transition_tests.cpp` (2 Meshobjekte, je 3 Materialfaces, 48 wechselnde Materialframes) und `renderer_gpu_soak_tests.cpp` (160 Frames mit Texture-Updates und fehlenden Meshes) fordern nun tatsächliche Draws in BEIDEN Durchläufen und exakte Summengleichheit. GPU-/Materialfunktionalität im synthetischen GL-Kontext bewiesen, nicht Pixelidentität mit dem Live-Simulator.

**Block D – datensparsame Ursachen-/Missing-Asset-Diagnostik:** Neues `AssetMissingIdCounter` speichert maximal 4096 verschiedene intern beobachtete UUID-Strings pro Region/Familie und zählt nur `AssetFailure::missing`, nicht Transient-/GPU-Failures; bei vollem Set werden Overflow-Ereignisse separat gezählt. Regionswechsel setzt die ID-Zählung zurück. Die App zählt so `mesh_missing_unique`, `mesh_missing_unique_overflow`, `texture_missing_unique`, `texture_missing_unique_overflow` und schreibt nur deren numerische Werte in `ogl.asset.streaming.v1`, niemals die eigentlichen UUIDs/CAPS-Adressen. Der sanitizte Python-Analyzer berechnet auf der neuesten gültigen Region-Snapshot-Serie `latest_region_mesh_missing_events_delta`, `latest_region_texture_missing_events_delta`, `latest_region_peak_missing_mesh_unique`, `latest_region_peak_texture_ram_pressure`, `latest_region_peak_mesh_ram_pressure` und die minimalen Cache-Hitraten. `ogl_inworld_acceptance.py` enthält diese Befunde unter `observations`, verändert jedoch keine bisher gültigen Nachweiskriterien. Zusätzliche C++- und Python-Tests prüfen Bounded/Reset/Overflow, zeitliche Serienaggregation und keinen Secret-Leak. Insgesamt **67/67 CTests** im Entwicklungsbuild erfolgreich.

**Live-Status / Abgrenzung 11.10.2026:** Die echte lokale `$HOME/.local/state/opengenesislink/viewer.log` auf der autorisierten Workstation enthält weiterhin 0 gültige aktuelle `InWorldAssetSnapshot`-Zeilen. `tools/ogl_inworld_acceptance.py` liefert `not_verified`, Exitcode 2. Der Code kann ohne authentifizierten Regionsbeitritt nicht als visuelle Abnahme von Meshes, Avatar, Welt, WASD/Bewegung, Teleport oder CAPS-EventQueue gelten. Diese bereits bekannten Mängel bleiben wie vereinbart für die spätere Fehlerbehebungs- und In-World-Abnahmephase dokumentiert. Hinweis: Der begrenzte Missing-ID-Counter meldet bei Sättigung eine Untergrenze unterschiedlicher UUIDs plus Overflows, keine exakte unbeschränkte Kardinalität.

**Nächster geplanter Release alpha.78, 3–4 Blöcke:** Cache-Pressure-Werte gegen fortlaufende Simulator-Samples validieren; transparentes Texture-/Mesh-Fehlverhalten mit vollständigem Regionswechsel und Asset-Staging prüfen; zusätzliche GPU-Szenenfälle für unterschiedliche Alpha-Mask-/Blending-Kontrakte und eine aussagekräftige Diagnose der wirklich vom Server fehlenden Assets sobald Live-Daten vorliegen. Maßgeblich bleibt `projectplan.md`.

Debug-Kommandos: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, nur Branch `develop-current`.

Neue Header `asset_cache_pressure_window.hpp`, `asset_missing_id_counter.hpp`, `texture_cached_request.hpp` und neue Tests `asset_cache_pressure_window_tests.cpp`, `asset_missing_id_counter_tests.cpp`, `texture_http_epoch_contract_tests.cpp`. Änderungen in `src/app/application.cpp`, Renderer, StreamingSnapshot, CMake und Log-Analyzer/-Acceptance samt GL-/Python-Regressionen. Auf GitHub nur `develop-current` committen.


## 0.2.0-alpha.78 – Cache-Hysterese und Alpha-Mask-Rendering

**Block A – RAM-Cache-Druck ohne Prefetch-Pendeln:** `AssetCachePressureWindow` benötigt nach aktivem Texture-/Mesh-Thrashing jetzt zwei aufeinanderfolgende unkritische 30-s-Auswertungsfenster, bevor `under_pressure` zurückgesetzt wird. Dadurch pausiert das rein optionale Texture-Disk-Prefetch auch nach einem zufällig ruhigen Einzelintervall weiter; erforderliche GetTexture-Netzwerkrequests bleiben unabhängig. Ergänzter Unit-Test für die Hysterese und neuer End-to-End-C++-Test `asset_cache_pressure_lru_tests.cpp`, der tatsächlich `TextureMemoryCache` und `MeshMemoryCache` mit jeweils nur zwei residenten Assets über 24 wechselnde UUIDs belastet, echte Hits/Misses/Evictions beobachtet und anschließend die zwei stabilen Recovery-Intervalle sowie den regionsübergreifend erhaltenen RAM-Inhalt kontrolliert.

**Block B – veraltetes Disk-Prefetch während Regionswechsel:** Produktionsfunktion `prefetch_texture_disk_to_memory` delegiert jetzt an einen separat kontrollierbaren `prefetch_texture_disk_to_memory_scoped`-Vertrag. Der integrierte `texture_prefetch_tests.cpp` hält eine asynchrone, gültige Disk-Leseantwort zwischen Start und Cache-Aufnahme an, wechselt dann die Regionsepoche und gibt den Worker frei. Das alte Texture-Bild darf nicht in die RAM-LRU übernommen werden. GetTexture-Hauptpfad und zuvor bestehende HTTP-Regionsepochen-Sperren bleiben unverändert. Das ist ein deterministischer Worker-/Cache-Vertragstest, kein echter Grid-Download.

**Block C – OpenGL3.3-Alpha-Mask-vs-Blend-Routing:** Neue zentrale CPU-Routingfunktion `face_material_requires_blending(tint_alpha,loaded_texture_has_alpha,mode)`. AlphaMode=1 berücksichtigt Texture-Alpha für den transparenten Pass; Mode=2 behandelt Texture-Alpha als GPU-Discard-/Mask-Bedingung im grundsätzlich opaken Pass, sofern Tint-Alpha 1 ist; Mode 0/3 lösen nicht allein wegen Texture-Alpha einen Blend-Pass aus. Der Shader führt `discard` bei AlphaMode 2 wie zuvor aus. Neuer tatsächlich ausgeführter Frame-Zähler `scene_alpha_mask_draw_calls`, zusätzlich zu bestehenden `scene_alpha_draw_calls` und `scene_opaque_draw_calls`, wird in `RenderDiagnostics` und im anonymisierten `InWorldAssetSnapshot.previous_frame_scene_alpha_mask_draw_calls` ausgegeben. Die echten OpenGL-Regionstransitions-/Mesh-Face-Tests prüfen Mask-Faces und sowohl opaque als auch blended Drawcalls; `material_pass_tests.cpp` enthält alle vier Moduskontrakte.

**Block D – realistische Cache-Druck- und Renderer-Sitzungsanalyse:** `tools/ogl_asset_log_report.py` wertet neueste zusammenhängende gültige Region-Snapshotserien aus und berichtet `latest_region_texture_ram_pressure_samples`, `latest_region_mesh_ram_pressure_samples`, `latest_region_peak_alpha_draw_calls`, `latest_region_peak_alpha_mask_draw_calls`, `latest_region_peak_gpu_upload_p95_us`. `tools/ogl_inworld_acceptance.py` ergänzt die sicheren Werte unter `observations`; sie sind Diagnose, nicht automatisch ein weiterer erfolgreicher Abnahmebeweis. Python-Regressionen belegen Anonymisierung und korrekte serielle Metrik. **68/68 CTests** vor finalem Release-Rebuild bestanden, einschließlich echter GLFW/OpenGL-Szenen und CPU-LRU-Stress.

**Ist-/Abnahmegrenzen:** Autorisierte Linux-Workstation `$HOME/.local/state/opengenesislink/viewer.log`: weiterhin **0** gültige aktuelle `InWorldAssetSnapshot`-Einträge, `ogl_inworld_acceptance.py` meldet `not_verified`, Exitcode 2. Daher keine echte NexVerse-Region-, Avatar-, WASD/Bewegungs-, Mesh-Vollständigkeits-, Teleport- oder CAPS-EventQueue-Freigabe. Diese bekannten Themen bleiben wie vereinbart für die spätere Live-Integrations- und Fehlerbehebungsphase dokumentiert. Alle aktuell getesteten Alpha-/Staging- und Cache-Szenen sind synthetisch, auch wenn der tatsächliche NVIDIA-OpenGL3.3-Treiber verwendet wurde.

**Nächster 3–4-Blöcke-Release Alpha.79:** Transparenz-/Maske-Sichtbarkeit zusätzlich mit reproduzierbarer Pixel-/Framebuffer-Abnahme prüfen, kombiniertes Texture-/Mesh-Staging über mehrere Regionswechsel noch strenger validieren, Ressourcen-Counter/Hysterese bei typischen Cachegrößen profilieren und In-World-Akzeptanzmeldungen für echte Simulatorlogs verfeinern sobald aktuelle Regionsdaten vorliegen. Maßgeblich bleibt `projectplan.md`.

GitHub: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (alten divergenten `develop` nicht mergen). Tests `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Diagnose `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`.

Geändert: `asset_cache_pressure_window.hpp`, `texture_prefetch.hpp`, `render/material_pass.hpp`, `src/render/material_pass.cpp`, `src/render/opengl_renderer.cpp`, `src/app/application.cpp`, `asset_streaming_snapshot.hpp`, CMake und Renderer-/Material-/Prefetch-/Analyzer-Tests. Neu: `tests/asset_cache_pressure_lru_tests.cpp`. Vollständige Tests/GUI/Docs/Push gemeinsam freigeben.


## 0.2.0-alpha.79 – Framebuffer-Alpha-Mask-Test und Recovery über GPU-Regionsepochen

**Block A – visuelle Pixel-/Framebuffer-Abnahme für Alpha, AlphaMask und opake Meshfaces:** Neuer echter GL3.3-GPU-CTest `tests/renderer_alpha_framebuffer_tests.cpp` erstellt ein verstecktes GLFW-Fenster, lädt ein GPU-Mesh und eine 2×2-Texture und liest nach `render_world()` den zentralen tatsächlich gerenderten RGBA-Pixel mit `glReadPixels`. Geprüft werden ein opakes rotes Face, vollständiger AlphaMask-Discard bei AlphaMode 2/Cutoff 128, anschließend restaurierte Opakheit und Blend-Rendering bei 50 % Alpha; Framepass-Zähler müssen mit der Sichtbarkeit übereinstimmen. Die maskierte Pixelreferenz ist ein vollständig transparenter AlphaMode-1-Face im **gleichen aktiven Szene-Rendermodus**, nicht ein No-Scene-Frame, da diese unterschiedliche Hintergrundpfade nehmen. Der neue GL-Test läuft auf dem echten NVIDIA OpenGL 3.3-Treiber und bestand. Eine synthetische Einzelpixelabnahme ist keine vollständige PBR-/OpenSim-Bildqualitätsfreigabe.

**Block B – drei vollständige GPU-Regionswechsel unter Mesh-/Texture-Staginglast:** Der bestehende tatsächliche GLFW/OpenGL-3.3-Test `tests/renderer_gpu_soak_tests.cpp` führt weiter 160 Frame-Draws, 160 transaktionale Texture-Replacements und 40 künstlich erzeugte Staging-Deferrals aus. Zusätzlich entfernt er bei Frame 40, 80 und 120 alle GPU-Texture- und Mesh-Ressourcen, setzt die Region-bezogene `GpuStagingRecovery` zurück und verifiziert vor dem Wiederaufbau **null GPU-Mesh-Draws, 24 fehlende GPU-Mesh-Objekte** und keine wartenden alten Staging-Einträge. Anschließend werden alle 20 gültigen synthetischen GPU-Meshes und acht Texturen frisch geladen; die vier absichtlich unbereit gehaltenen Meshobjekte bleiben als echte Fallbacks sichtbar. Alle Frame-/Passsummen und GL-Fehlerprüfungen müssen bestehen. Das testet GPU-Regions-Lebenszyklus und Upload-Recovery, nicht die Netz-/Simulatorintegration.

**Block C – Cache-Kapazitätsmatrix für echte Texture-/Mesh-RAM-LRUs:** Neuer C++-CTest `tests/asset_cache_capacity_profiles_tests.cpp` verwendet die realen `TextureMemoryCache`-/`MeshMemoryCache`-Implementierungen mit 8, 64 und 128 zulässigen Asset-Slots. Über sechs deterministische Zugriffswellen mit 96 verschiedenen UUIDs werden 1152 Zugriffe je Familie und Profil durchgeführt, inklusive bevorzugt wiederholten Hotspots, Hit/Miss und LRU-Evictions sowie Kapazitätsgrenzen. Beispiel-Testlauf: 8 Slots 383 Hits/769 Misses/761 Evictions; 64 Slots 875/277/213; 128 Slots 1056/96/0 pro Assetfamilie. Diese Testwerte sind reproduzierbare synthetische Zugriffsmuster und kein kalibriertes Profil realer OpenSim-Assetgrößen/Hardwarelatzenzen. Für alle drei Größen sind triviale Fehlzustände, unkontrolliertes Wachstum und fehlende Wiederverwendung abgesichert.

**Block D – konkrete, datensparsame In-World-Fehlerdiagnose:** `tools/ogl_inworld_acceptance.py` erweitert das bisherige `ogl.inworld.acceptance.v2` um `failure_codes` und `next_check`. Aus den existierenden konkreten Health-Checks werden nur feste sichere Diagnosecodes erzeugt, beispielsweise `no_current_region_snapshots`, `insufficient_region_samples`, `texture_residency_incomplete`, `gpu_mesh_draws_missing`, `mesh_fallback_still_drawn`; niemals UUIDs, CAP-URLs oder Rohlogs. Der empfohlene nächste Prüfpunkt priorisiert fehlende Live-Regionensnapshots, veraltete/unterbrochene Serien, nichtleere Asset-Demandsets, niedrige Residency und echte Mesh-/Fallback-Draws. `evidence_pass` erhält die leere Failure-Liste. Python-Regressionen prüfen fehlende Daten, valide Serien, veraltete/unterbrochene Daten und kritische Missing-Mesh-Fälle. Im aktuellen echten Viewerlog: **0** gültige InWorld-Snapshots, `not_verified`, `failure_codes=['no_current_region_snapshots']`, CLI-Exitcode 2. Der fehlende Live-Test wird ausdrücklich nicht als Fehlerbehebung oder als bestanden ausgegeben.

**Gesamtprüfung:** Debug-C++23-CMake-Build, reale OpenGL-Regressionen und **70/70 CTests** im Entwicklungsstand bestanden. Nach der Versionspflege vollständiger Neuaufbau und GUI-Welcome-Smoke auf NVIDIA RTX 4070 SUPER vorgesehen; GitHub-Push ausschließlich nach bestandener Endprüfung auf `develop-current`.

**Nächste vier geplante Releaseblöcke Alpha.80:** Erweiterte Pixelreferenzen für Mehrfach-Overlays und AlphaSort, kontrollierter GPU-Rollback-/Uploadfehler-Regionswechsel, Cache-/Residency-Priorisierung nach messbarer Druckphase und tatsächliche In-World-Test-Protokollierung sobald erfolgreiche Authentifizierung/Regionsdaten vorliegen. Bekannte Avatar-/WASD-/Teleport-/CAPS-EventQueue-Probleme bleiben wie vereinbart bis zur späteren Integrations-/Bugfixphase zurückgestellt; `projectplan.md` bleibt maßgeblich.

CLI auf autorisiertem Linux-Desktop: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Live-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.

Neu: `tests/renderer_alpha_framebuffer_tests.cpp`, `tests/asset_cache_capacity_profiles_tests.cpp`; erweitert: `tests/renderer_gpu_soak_tests.cpp`, `tools/ogl_inworld_acceptance.py`, Python-Tests, CMake. Vier große Blöcke in einem Release, Datenfluss und UI bleiben unverändert.


## Alpha.80 – erster überprüfbarer Firestorm-Funktionsparitätsblock

**Block 1 – geprüfte Firestorm-Seed-CAP-Abdeckung:** `default_seed_capabilities()` fordert beim echten Login-Seed nun zusätzlich `FetchInventoryDescendents2`, `InventoryThumbnailUpload`, `FindExperienceByName`, `GetExperiences`, `AgentExperiences`, `ExperiencePreferences`, `GroupExperiences`, `GetAdminExperiences`, `GetCreatorExperiences`, `IsExperienceAdmin` und `UpdateExperience` an. Vorhandene `FetchInventory2` und `GetExperienceInfo` bleiben. Die Antwort bestimmt die tatsächliche Verfügbarkeit; nicht angebotene CAPS werden nicht simuliert. Der C++-Regressionstest kontrolliert einmalige Anforderung aller relevanten Namen.

**Block 2 – Server-capability-gesteuerte Firestorm-Bereitschaft:** Neue typisierte `FirestormCapsAvailability` mit acht klar definierten CAP-Kategorien (Inventaritem-Fetch, Inventarordner-Fetch, ThumbnailUpload, Owned/Allowed/Permissions/Admin/Update Experiences) berechnet rein aus der geprüften Seed-Antwort die angebotenen Funktionen, nie aus dem Vorhandensein von Beispiel-URLs oder dem Serverproduktnamen. Im produktiven Login-CAPS-Ladebildschirm erscheinen `Startregion: Outfit-Vorschau CAP/- | Experiences CAP/-`, optional die Anzahl eigener/erlaubter Experiences. `CAP` bedeutet *angebotener Endpunkt*, kein erfolgreicher Upload oder Firestorm-Funktionsparität. Der bisherige NexVerse-UI-Stil und die Simulation-Ready-Steuerung bleiben erhalten.

**Block 3 – Read-only Firestorm Experience-Listen, ohne EventQueue zu bremsen:** `parse_agent_experiences` akzeptiert strikt LLSD-Map mit begrenztem `experience_ids`-Array und unterscheidet `purchase`-Feld fehlend von `purchase: 0` (so verlangt es der aktuelle OpenGenesisLINK-Serververtrag für Firestorm). `parse_resident_experiences` validiert `experiences` und `blocked` getrennt. Die OGL-CAPS-Worker-Routine führt die beiden GETs nur bei tatsächlich angebotenen `AgentExperiences`/`GetExperiences` in einer separaten, beim Beenden gejointen Leseaufgabe aus, damit die EventQueue nicht durch diese optionalen Abfragen blockiert wird. Keine Experience-Purchase-/Allow-/Block-/Forget-Verwaltungsaktion erfolgt automatisch; Ergebnisse der ursprünglichen Login-Startregion werden nicht als aktuelle spätere Teleportregion behauptet. Es werden nur Anzahl und CAP-Verfügbarkeit angezeigt; keine Avatar-/Experience-UUID oder Secret-CAP-URL in neuen Diagnoseausgaben.

**Block 4 – Firestorm/OpenGenesisLINK Outfit-/Inventar-Thumbnail-Wire-Vertrag:** `build_thumbnail_begin_request(target,id)` erzeugt nur bei gültiger, nichtleerer Item-/Ordner-UUID das richtige LLSD-XML mit genau einem `item_id` oder `category_id`. `parse_thumbnail_stage` akzeptiert nur autorisierte Antwortformen `{state:upload,uploader:<HTTP(S)-URL>}` bzw. `{state:complete,new_asset:<UUID>}`. Ungültige UUIDs, Dateischemen und unbekannte Zustände werden verworfen. Dies entspricht dem aktuellen OpenGenesisLINK-Servermodul `OpenSim/Region/ClientStack/Linden/Caps/InventoryThumbnailUploadModule.cs`. Die Binär-JPEG2000-Übertragung, Besitz-/Rechteprüfung und endgültige OGL-Inventar-UI sind **nicht implementiert**; der Server selbst bindet One-Time-Uploader an aktive Owner-CAPS. Die serverseitig dokumentierte Firestorm-Outfit-Thumbnail-Cache-Limitation bleibt separat bestehen. `tests/firestorm_compat_tests.cpp` deckt Capability-Abgleich, Listenschema, Größenlimits, LLSD-Serialisierung und Upload-Handshake ab.

**Externe Codequellen, 11.10.2026:** Aktueller OpenGenesisLINK-Server `main` = `6dc73f2adfded7fe5ee94c992daaaba5be2b66c8`, Entwicklungszug 0.9.3.10 Dev / Stable 0.9.3.8, Commit vom 11.10.2026: Experience Manage API Keys können jetzt mit separatem `experiences:viewer:manage`-Scope ausgegeben werden. Dies ist **keine** Viewer-seitige Admin-Key-Nutzung: OGL darf niemals interne World-API-Secrets erhalten. Firestorm offizielle Referenzrelease-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` (Nightly `master` nicht als Release). Relevante Firestorm-Quelle `indra/newview/llfloaterexperiences.cpp` und OGLServer-Dokument `doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md` sowie `FIRESTORM_INVENTORY_THUMBNAIL_09310.md`. Server-/Client-SHAs vor jedem künftigen Featureblock neu vergleichen.

**Nachweis- und Einschränkungsstatus:** Erste vier Firestorm-Vertragsbausteine sind implementiert und lokal synthetisch getestet, aber kein vollständiger Firestorm-Fork und keine vollständige Experience-/Outfit-Benutzerfunktion. Die ursprüngliche reale In-World-Logserie enthält keine frischen Snapshotnachweise, Avatar/WASD/Mesh/EventQueue/Teleport bleiben noch zu integrieren. Weiterhin keine Behauptung einer erfolgreichen NexVerse-Live-Experience-/Inventar-Abnahme. Lizenzentscheidung für tatsächliche Firestorm-LGPL-Source-Übernahme bleibt offen; dieser Commit übernimmt keinen fremden Firestorm-Quellcode.

**Nächste 3–4 Blöcke Alpha.81:** Read-only Experience-Funktionen in die echte OGL-InWorld-Benutzeroberfläche mit regionengebundener Session-/CAP-Lease übernehmen; LLSD Inventar-/Outfit-Thumbnail-Metadaten inklusive Versionswechsel und Rechte abstrahieren; volle Firestorm-Menü-/Funktionsinventur und aktuelle Server-CAPS-Diffs weiter ausfüllen; anschließende kontrollierte Entwicklung von Upload/Experience-Consent mit echter Grid-Permission-/Owner-Abnahme. Firestorm-Parität hat Vorrang vor neuen Renderer-only-Erweiterungen.


## 0.2.0-alpha.81 – erste bedienbare Firestorm-Experience-Ansicht

Vier zusammengehörende Firestorm-Paritätsblöcke:

1. **Bedienbare Experience-Ansicht:** Nach authentifizierter und bereiter Simulation öffnet F8 einen Floater im NexVerse-Design. Eigene/Erlaubte/Blockierte Experiences lassen sich über Maus oder die Tasten 1–3 auswählen, ESC/F8 schließt. Die Liste enthält echte Simulator-UUID-Präfixe mit ausdrücklich noch fehlenden Anzeigenamen statt erfundener Namen. Fehlende CAPS, noch nicht geladene Daten und leere Listen werden getrennt angezeigt. Solange die Ansicht geöffnet ist, löst WASD keine Avatarbewegung aus. Schreiben, Experience erwerben, Allow/Block/Forget bleiben gesperrt. Der Viewport vor erfolgreichem Simulator-Eintritt bleibt frei von der Experience-UI.

2. **Region-scoped Experience-Leseergebnisse:** FirestormRegionLease bindet Regionhandle, WorldModel-Generation und Seed-Identität an einen threadsicheren Epochentoken. Bei Regionswechsel werden alte CAP-Advert-Flags, Counts und Experience-UUID-Listen gelöscht, der Floater geschlossen. In-flight AgentExperiences/GetExperiences-Antworten werden nach dem Wechsel auch bei gültiger HTTP-200-Antwort verworfen; Lease-Check erneut unter Caps-Mutex. Die Endpoints und UUIDs dürfen nicht in Statuslogs erscheinen. Der EventQueue-Hauptworker bleibt unabhängig. Neuer deterministischer asynchroner CTest belegt die späte GET-Ablehnung. Grenze: Neues Experience-CAPS-Seed-Discovery nach Teleport ist noch nicht umgesetzt, daher wird absichtlich keine alte Liste weiter angezeigt.

3. **Inventar-/Outfit-Metadatenvertrag:** Der MPL-2.0-Adapter parse_inventory_thumbnail_metadata liest die OpenGenesisLINK-LLSD thumbnail.asset_id aus FetchInventory2/FetchInventoryDescendents2, unterscheidet fehlendes Feld, ausdrücklich leere (Zero-UUID) Vorschau und gültige Thumbnail-UUID, lehnt defekte LLSD-Strukturen ab. InventoryFolderThumbnailCache speichert zu Folder-UUID die Version und Asset-ID, lehnt ältere Folderversionen ab und akzeptiert geänderte Thumbnail-IDs auch bei gleicher Version, um die dokumentierte Firestorm-7.2.4-Outfit-Gallery-Caching-Einschränkung nicht blind zu übernehmen. Neuer CTest belegt leere, alte, ersetzte und ungültige Metadaten. Grenze: noch kein echter Inventar-GET/Outfit-Gallery-UI, kein JPEG2000-Upload, keine reale Server-Readback-Abnahme.

4. **Konkrete Funktionsparität und Verträge:** Offizielle Firestorm-Releasequelle Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d; belegt über indra/newview/llfloaterexperiences.cpp (Tabs, GetExperiences/AgentExperiences, Acquire-Button). Aktueller OpenGenesisLINK Server main @ 6dc73f2adfded7fe5ee94c992daaaba5be2b66c8; Belege doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md, doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md und serverseitige Thumbnail/Experience-Implementierung. Versionsgrenzen: OpenGenesisLINK 0.9.3.10 Dev und Firestorm 7.2.4.80712. Die Firestorm-Funktions-/Verhaltensbaseline bleibt verbindlich, der bestehende OGL-C++23-Viewer ist noch kein Firestorm-Code-Fork.

**Nachweise und Grenze:** 73/73 lokale CTests nach Integration, einschließlich zweier neuer Firestorm-Regressionen und realen GPU-Tests. Ein bestandener synthetischer Lease-/Metadatentest ist KEIN NexVerse-In-World-Test. Eine tatsächliche authentifizierte Regionsabnahme für diesen Stand fehlt weiterhin. Der F8-Floater wird nur im Simulationszustand gezeichnet; Login-Welcome-Smoke allein prüft keine Interaktion nach einem realen Simulatorbeitritt.

**Alpha.82-Priorität:** neue Regions-Experience-CAPS nach Teleport entdecken, Experience-Namen/Profile mit GetExperienceInfo lesen, servergebundene FetchInventory2/FetchInventoryDescendents2-Read-only-Outfit-Gallery aufbauen sowie Write/Consent/Upload nach separatem Owner-/Permissions-/Grid-Vertrag kontrolliert vorbereiten. Firestorm-Parität zuerst, Zusatzrenderer nachrangig.


## 0.2.0-alpha.82 – Firestorm-Funktionserweiterung auf echten Simulator-Verträgen

**Block A – dynamische Firestorm-CAPS nach Region-/Teleportwechsel:** Neue Protokollroutine fetch_firestorm_region entdeckt AgentExperiences, GetExperiences, GetExperienceInfo, FetchInventory2/FetchInventoryDescendents2 und InventoryThumbnailUpload anhand des echten aktuellen Simulator-Seeds. Die Routine holt autorisierte Experience-Listen ausschließlich lesend; CAP-Verfügbarkeit bedeutet keinen bestätigten Betrieb und keine Schreibberechtigung. In der App laufen Region-Read-Worker unabhängig vom bestehenden Login-CAPS/EventQueue-Longpoll, mit maximal vier ausstehenden Anfragen und nichtblockierender Future-Abholung; bei erschöpften Slots wird der aktuellste Regionsrequest nach Freigabe erneut begonnen. Ergebnisse werden nur für den noch gültigen FirestormRegionLease-Token unter Caps-Mutex übernommen. Alte UUIDs/Capabilities werden beim Regionswechsel zuerst verworfen. Wichtig: Die allgemeine Simulatorverbindung/EventQueue-Reconnect wird dadurch nicht automatisch repariert; die Verfügbarkeit einer neuen Region setzt eine vom WorldModel gelieferte gültige Seed-CAP voraus.

**Block B – echte Experience-Anzeigenamen über Firestorm GetExperienceInfo:** Authentifizierte read-only Profile werden optional vom pro Region angebotenen GetExperienceInfo-Endpoint über die Firestorm-Query /id/?page_size=12&public_id=UUID abgefragt. Nur tatsächliche IDs aus eigenen/erlaubten/blockierten Listen, maximal zwölf, werden angefragt. Der LLSD-Parser verlangt experience_keys[] mit public_id und name, validiert UUID, begrenzt Namenslänge auf 128 Zeichen und lehnt Steuerzeichen ab. F8 zeigt bei Erfolg den vom Simulator gelieferten Experience-Namen; bei fehlender/nicht unterstützter CAP bleibt die eindeutige Kennzeichnung ID XXXXXXXX (Name nicht geladen). Regionsepoche schützt auch Profilresultate, keine CAP-URLs, Experience-UUIDs oder Admin-API-Keys in neuen strukturierten Logs.

**Block C – lesende Outfit-Galerie aus echten OpenSim-Loginordnern:** F9 öffnet nach erfolgreichem Eintritt in eine Simulation ein Fenster im NexVerse-Design. Firestorms echte LLFolderType-Codes FT_MY_OUTFITS=48 und FT_OUTFIT=47 identifizieren aus der vom OpenSim-Login gelieferten inventory-skeleton die Outfit-Ordner. Die ersten zehn realen Ordnernamen werden aus maximal 256 akzeptierten UUID-validierten Einträgen angezeigt; ungültige und doppelte IDs werden ignoriert, Steuercodes in Titeln bereinigt. F8- und F9-Ansichten schließen einander aus, ESC/F9 schließt die Outfit-Ansicht, während offener Ansicht werden WASD-/Movement-Tasten nicht an den Agenten übergeben. Keine erfundene Vorschau oder Behauptung eines funktionierenden Wear/Rez. **Grenze:** Die Anzeige stammt derzeit aus dem Login-Skeleton; der aktive FetchInventory2/FetchInventoryDescendents2-Readback, Asset-Mesh-/JPEG2000-Vorschauen, COF-Verknüpfung, Outfits anziehen und Rechteprüfung bleiben offen.

**Block D – überprüfbarer Firestorm-/OpenGenesisLINK-Wire-Vertrag:** Der vollständige serverseitige Snapshot vom 11.10.2026 nutzt OpenGenesisLINK main fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b (neuer als alpha.81 6dc73f2). Offizielle Firestorm-Release-Referenz Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d. Implementierungsquellen: Firestorm indra/llmessage/llexperiencecache.cpp und .h (public_id, experience_keys, /id/), indra/llinventory/llfoldertype.h (46/47/48); OGL-Server NexVerse/RegionModules/Experiences/NexExperienceModule.cs mit GetExperienceInfo via variable path und getrennten Permission-/Manage-Keys. Der OGL-Viewer besitzt und benötigt keine zentralen World-API-Keys. Neuer nativer HTTP-Fixture-Test simuliert Seed->Listen->Profilaufrufe und verspäteten Region-Seed nach dem Wechsel, neuer Outfit-Gallery-Test validiert Firestorm-Ordner-Typen, Duplikate, Sanitizing und echte Login-Ordnerdaten. Die Tests beweisen kontrollierten OGL-Vertrag, KEINE authentifizierte Live-Regionsabnahme oder vollständigen Firestorm-Quellfork.

**Abnahmegrenzen:** Die neue F8-/F9-Bedienung ist nur während WorldModel.ready sichtbar; GUI-Welcome-Smoketest ohne echte NexVerse-Simulation belegt daher keinen Live-UI-Nachweis. Noch keine echte InWorldAssetSnapshot-Serie im bisherigen Log. EventQueue, Avatar, tatsächliche WASD-Bewegung, Upload, Wearables/COF, Marketplace/Groups/Voice und sonstige Firestorm-Funktionen bleiben gemäß verbindlicher Paritätsmatrix als noch nicht nachgewiesen/implementiert gekennzeichnet. Nach Versions-Neubau volle Tests und NVIDIA-OpenGL-Smoke durchführen.

**Nächste vier geplante Blöcke Alpha.83:** echten FetchInventory2-/FetchInventoryDescendents2-Readback der Outfitfolder mit serveradvertisierter CAPS-POST-Struktur, Thumbnail.asset_id und diskretem Cache-Vertrag implementieren; Outfit-Vorschauen aus Assetpipeline als Bilder anzeigen, keine leeren Platzhalter; Experience-Auswahl mit Profildetails (Name, Beschreibung, Berechtigungen) vervollständigen; EventQueue/Teleport-Region-CAPS- und Firestorm-Features mit echter autorisierter Simulator-Sitzung regressionsprüfen. Funktionen nur bei tatsächlichen Serverrechten aktivieren.


## 0.2.0-alpha.83 – Firestorm-Inventar-/Experience-Funktionen

**Block 1 – echter Firestorm-/OpenGenesisLINK-Inventarabruf per CAPS-POST:** `firestorm_inventory_fetch.hpp/.cpp` erstellt aus dem authentifizierten OpenSim-Login-Agenten und den ermittelten Outfit-Foldern ein LLSD-`FetchInventoryDescendents2`-POST mit `folders[]`, `folder_id`, `owner_id`, `sort_order`, `fetch_folders`, `fetch_items`. Bis 16 Outfits und ihre gültigen übergeordneten Folder werden doppelfrei abgefragt, weil nur die *Parent-Categories* die Thumbnail-Metadaten der Outfit-Ordner liefern. Antworten werden auf erlaubte Folder-/Parent-UUIDs, nichtnegative Versionen, max. 64 Folder-/1024-Category-Entries und den aktuellen FirestormRegionLease-Epochentoken begrenzt. Gelesen werden `categories[].thumbnail.asset_id` und versionsgebundene Foldermetadaten. Keine künstlichen Inventarordner, fremden Owner oder URL-Leaks. Der Zugriff bleibt ausschließlich lesend und nur über eine vom aktuellen Simulator beworbene Capability.

**Block 2 – echte Outfit-Vorschaubilder durch vorhandene GetTexture-/JPEG2000-/PNG-Pipeline:** Nach erfolgreichem, epochentreuem Inventar-Readback stellt die Produktions-App die vom Server gelieferten `thumbnail.asset_id` IDs und den tatsächlich beworbenen `GetTexture`-CAP bereit. Bis zu acht unterschiedliche, tatsächlich vorhandene Bilder werden in einer auf drei gleichzeitige Ladevorgänge begrenzten asynchronen Pipeline über den vorhandenen `assets::TextureAssetLoader::fetch_rgba` (PNG mit JPEG2000-Fallback) dekodiert und erst auf dem OpenGL-UI-Thread über `UiRenderer::create_rgba_texture` in sichtbare F9-Vorschau-Bilder umgewandelt. Auflösung für UI-Upload höchstens 1024×1024; Asset-ID und Regionen-Epoche werden unmittelbar vor GPU-Upload und beim Zeichnen kontrolliert. Beim Regionswechsel werden neue/alte Tasks, ID-Zuordnungen und GL-Textures getrennt behandelt; alte GL-Handles werden gelöscht, late replies verworfen. Fehler oder vom Grid nicht angebotene Caps lassen den ehrlichen Text `Vorschau -` stehen. Kein Bitmap wird erfunden und kein Firestorm-Thumbnail-Upload vorgetäuscht.

**Block 3 – Firestorm Experience-Profildetails und Benutzerselektion:** Die bestehende F8-Ansicht nutzt nun `ExperienceProfile` aus den vom Server über `GetExperienceInfo /id/?page_size=12&public_id=...` tatsächlich gelieferten LLSD-Objekten. Neben dem Namen werden Beschreibung (maximal 160 Zeichen zur Darstellung aus max. 4096), Firestorm-Maturity-Codes 13/21/42 und ein Aktivstatus aus Property-Bit 6 verarbeitet; alle Texte werden auf Länge und Steuerzeichen geprüft. Ein einzelner Experience-Eintrag ist im vorhandenen Viewer-Design per Maus oder Pfeiltasten markierbar; 1/2/3 wechseln zwischen eigenen/erlaubten/blockierten Listen. Zurückhaltender Fallback, wenn kein aktuelles Profil verfügbar ist. Keine automatische Zustimmung, kein Kauf, keine Write- oder Admin-API-Key-Nutzung.

**Block 4 – geprüfter Server-/Firestorm-Vertrag und Regressionen:** Offizieller Firestorm-Release-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` mit `indra/llinventory/llfoldertype.h` (Outfits 47 / My Outfits 48) und `indra/llmessage/llexperiencecache.cpp` (GetExperienceInfo). Offizieller OpenGenesisLINK-Server `main` @ `fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b`, insbesondere `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs`, `OpenSim/Region/ClientStack/Linden/Caps/FetchInventory2Module.cs` und `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Neuer CTest `firestorm_inventory_fetch_tests.cpp` simuliert Owner-/Folder-POST, echte Parent-Category-Thumbnail-LLSD, gesperrte Altregion-Requests und ungültige CAP-URLs. Erweiterter `firestorm_region_read_tests.cpp` testet Region-Seed -> Read-only Experience-Listen -> Profile mit Beschreibung/Maturity -> Folder-/Thumbnail-Response und Texture-CAP, ohne echte Avatar-Authentifizierung zu behaupten. Galerie-Skeleton-Test verwirft auch ungültige Parent-UUIDs. Debug C++23 und insgesamt 76 CTests nach finalem Versions-Neubau vollständig prüfen.

**Strikte Status-/Abnahmegrenze:** `tested_synthetic` für die protokollierten Antworten, Bilddekodierung und GL-Uploadpfade; **nicht** `live_verified`, weil aktuell keine authentifizierte NexVerse-Simulator-/Avatar-/Outfit-Sitzung vorliegt. Kein vollständiger Firestorm-Code-Fork, nur testbare eigene Adapter entsprechend Firestorm-Featurebasis. Die bestehende allgemeine Teleport-/EventQueue- und Avatar-/WASD-Integration ist weiterhin offen. F8/F9 sind nur nach `simulation_ready` sichtbar; Welcome-GUI-Smoketest belegt die neue UI nicht. Vollständiges COF-/Wearables-/Outfitwechsel-Verhalten, Folder-Events und Berechtigungen fehlen. Die verbesserte Cache-Metadatenroutine darf nicht als Nachweis ausgegebener Thumbnail-Bilder auf einem echten Grid missverstanden werden.

**Nächste 3–4 Blöcke Alpha.84:** kontinuierliche Firestorm-Inventarordner-Synchronisierung und `FetchInventory2`-Items samt COF-Lesezustand (nicht nur Login-Skeleton); bedienbare Outfit-Details/Tragen mit Rechte- und Permission-Checks gegen Serverprotokoll; Experience-Berechtigungsdialog mit expliziter Nutzeraktion und aktuellem Simulator-Lease; danach eine echte NexVerse-In-World-Testabnahme inklusive Avatar, Bewegung, Regionswechsel und den noch offenen EventQueue-/Mesh-Problemen. Vollständige Firestorm-Parität bleibt vorrangig.


## 0.2.0-alpha.84 – COF, F9-Details und Experience-Berechtigungen

**Block 1 – Firestorm COF und wirkliche Outfit-Inventargegenstände (lesend):** Der `FetchInventoryDescendents2`-Reader liest jetzt neben den Parent-Thumbnail-`categories` die im Simulatorantwort-`items[]` enthaltenen Inventargegenstände pro Outfit-Folder. UUID, Asset-ID, Name, Asset-/Inventory-Type werden als typisierte `OutfitInventoryItem`-Einträge nur aus gültigen und begrenzten LLSD-Maps übernommen (max. 512 Antwortitems, höchstens 256 verwendete pro Ordner, Namen max. 128 Zeichen mit Entfernung von Steuerzeichen). Der separate Current Outfit Folder (COF, Firestorm Typ 46) wird aus der authentifizierten OpenSim-Login-Skeleton ermittelt und mit dem gleichen Owner-gebundenen CAPS-POST abgefragt, auch wenn keine gespeicherten Outfits vorhanden sind. Der Owner der Serverantwort wird kanonisch als UUID-Bytes gegen den eingeloggten Agenten geprüft; fremde/ältere Antworten dürfen keine Anzeigedaten ersetzen. **Keine Wearables-/COF-Mutation**, kein unberechtigter Asset-Upload.

**Block 2 – versionsgebundene, inkrementelle Inventarsynchronisierung:** Neue `protocol::FirestormInventorySync` hält Folderversion, Items und Thumbnail-Asset-IDs ausschließlich innerhalb der Regionsepoche und verwirft alte Versionen, unvollständige/fehlende Thumbnail-Maps sowie verspätete Regionsantworten. Ein gleichversionierter, aber wirklich geänderter Thumbnail-Asset-ID-Link aktualisiert die Galerie. Solange F9 geöffnet ist, prüft die Anwendung im 30-Sekunden-Abstand mit einem **maximal einen** Hintergrund-Future den bereits beworbenen `FetchInventoryDescendents2`-CAP erneut. Render-/EventQueue-Threads warten nicht auf diese Requests. Die alte GL-Vorschau wird nur bei echter Asset-ID-Änderung freigegeben, danach wieder geladen. Neuer deterministischer `firestorm_inventory_sync_tests.cpp` prüft Version-Ordering, unveränderte/ersetzte Thumbnails, ignorierte Legacy-Omissions und Region-Epoch-Reset. **Keine echte EventQueue-basierte Folder-Notifikation**, Polling während sichtbarer Galerie ist ein sicherer Zwischenstand.

**Block 3 – bedienbare F9-Outfit-Details inklusive COF-Readback:** Im bestehenden NexVerse-Look lässt sich einer der ersten zehn in der Liste sichtbaren Outfit-Folder per Maus oder Auf-/Ab-Taste auswählen. Ein rechtes Detailfeld zeigt dessen tatsächlich vom Server gelesene Gegenstandsnamen (bis sechs), Anzahl und den separaten aktuellen COF-Linkstand, sofern dieser geladen wurde. Fehlt die Owner-CAP oder ein aktueller Abruf, steht ausdrücklich „Inhalt noch nicht geladen“. Existing PNG/JPEG2000-GetTexture-Thumbnail-Darstellung bleibt aktiver Bildpfad; fehlende Bilder bleiben unbestätigt statt gefälscht. **Outfit anziehen, BOM, Wearables, Animationen und synchronisierte Avatar-Appearance sind noch offen**, solange ihre kompletten Firestorm-LLUDP-/CAPS-Rechte und eine echte NexVerse-Simulation fehlen.

**Block 4 – Firestorm ExperiencePermissions mit ausdrücklicher Nutzereinwilligung:** Der neue `ExperiencePermissionConsent`-Vertrag ermöglicht im F8-Fenster bei vorhandener, aktueller `ExperiencePreferences`-CAP genau `A` (= Erlauben vorbereiten) oder `B` (= Blockieren vorbereiten), gefolgt von `Enter` zur **separaten Bestätigung**. `Escape`, Regionswechsel oder veraltete Epochs verwerfen die Vorbereitung. Erst nach Bestätigung startet eine asynchrone, owner-/Region-Lease-gebundene `HTTP PUT`-Operation mit offizieller LLSD-Map `{experience_uuid:{permission:Allow|Block}}`. OpenGenesisLINK liefert die bestätigten `experiences`/`blocked`-Listen; nur wenn die gewählte ID genau im Zielset und nicht im Gegenset steht, zeigt OGL eine erfolgreiche Änderung an. Die generische HTTP-Abstraktion besitzt dafür eine fail-closed PUT-Methode; libcurl-Weiterleitungen für mutierende PUT-Requests sind gesperrt. Die anderen Firestorm-Aktionen `Forget`/DELETE, Purchase, Experience-Editor und Adminrechte bleiben **nicht freigeschaltet**. Neuer Test prüft weder versehentliche Writes noch falsche Erfolgsbestätigungen: ohne explizite Enter-Bestätigung null PUT-Requests, epoch-bound stale reject, negative Serverantwort, erlauben/blockieren.

**Server-/Firestorm-Quellenprüfung (11.10.2026):** Offizielle Firestorm-Releasequelle `FirestormViewer/phoenix-firestorm` Branch `Firestorm_7.2.4` bei `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d`. OpenGenesisLINK Server `main` nun **`efc9b3a73ce04f6f57a1958c29ec0a439ed76d0f`** (neuer als alpha.83 `fcac896e`): Source `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs` mit `items`, `categories.thumbnail.asset_id`, `owner_id`, `version`; `NexVerse/RegionModules/Experiences/NexExperienceModule.cs` mit leasegebundenem `ExperiencePreferences` GET/PUT/DELETE, nur Avatar-/CAP-gebundene Berechtigungen. Server-HEAD entfernt OGL-Pathfinding aus 0.9.3.10-Dev-Scope: Firestorm-Pathfinding bleibt gewünschte Baseline, aber kann auf diesem Server derzeit **nicht als kompatible NavMesh-Funktion** behauptet werden. Neues Feature nur bei Server-Vertrag und später mit tatsächlichen Rights-/Live-Checks.

**Evidenz und Grenzen:** Versionsbuild und sämtliche 78 lokalen CTests inklusive realer GPU-Tests sowie Linux-GUI-Smoke sind vor GitHub-Release erneut durchzuführen. `tested_synthetic` beschreibt bestätigten OGL-Wire-Code und reproduzierbare Tests, **nicht** erfolgreiche Live-Outfit-Wear, Consent, Thumbnail-Erneuerung oder In-World-Avatar-/EventQueue-Funktion. Ungeprüfte bzw. fehlende Firestorm-Bereiche bleiben in der Matrix ausdrücklich offen. UI-Nachweise aus dem Welcome-Ladebildschirm belegen F8/F9 erst in einer echten `simulation_ready`-Sitzung **nicht**.

**Nächste 3–4 Blöcke Alpha.85:** COF-/Outfit-Wear- und Appearance-/Attachment-Protokoll gegen Firestorm-Source und OpenGenesisLINK-Rechte nachbauen; Firestorm-Inventar-Baum samt On-Demand FetchInventory2 und Ordner-Event-Diffs ausbauen, danach optionales Permissions-Forget mit Bestätigung und Lease; fehlende EventQueue-/Avatar-/Mesh-/WASD-Integration gegen authentifizierte NexVerse-Grid-Sitzung live prüfen. Der vollständige Firestorm-Funktionskatalog bleibt verpflichtend.


## 0.2.0-alpha.85 – Firestorm Inventar- und Experience-Funktionen

**Block 1 – Firestorm-ähnlicher Inventarbaum F10 mit echter Ordnernavigation:** Die neue lesende Inventaransicht `F10` zeigt Ordner aus `LoginSession.inventory_skeleton` sowie live nachgeladene `FetchInventoryDescendents2`-Kategorien (Name, Typ, Parent-UUID, Version). Auf-/Ab-Pfeile und Mausklick markieren Ordner, Enter/Rechts öffnen und Links/Backspace gehen zurück; `F10`/ESC schließen. Die Fenster F8/F9/F10 schließen einander aus, Movement-Tasten werden bei geöffnetem Inventar blockiert. Der generische Fetch-Adapter fragt im Hintergrund jeweils genau einen ausgewählten Ordner mittels gültigem, regionengebundenem Simulator-CAP ab; bis zu 256 abgefragte Ordner und 4096 lokal modellierte Folder verhindern ungebremste Rekursion. `owner_id`, `folder_id`, Parent-Zuordnung, LLSD-Typen/Versionsnummer und aktuelle Regionsepoche werden vor Übernahme geprüft. Auch ungültige/fehlende Capabilities führen nicht zu HTTP-Dauerschleifen. Unterordner außerhalb des Login-Skeleton erscheinen erst nach erfolgreichem Simulator-Fetch, nicht aus erfundenen Daten.

**Block 2 – echter `FetchInventory2`-Einzelitem-Abruf für Outfits:** `firestorm_inventory_items.hpp/.cpp` implementiert die OpenGenesisLINK-konforme `POST`-Anfrage `items:[{item_id,owner_id}]` an die vom aktuellen CAPS-Seed beworbene `FetchInventory2`-URL, gebündelt auf maximal 32 IDs aus der aktuell angezeigten F9-Outfitliste. Die Antwort `agent_id` und `items[].permissions.owner_id` müssen exakt zum angemeldeten Avatar gehören; unbekannte IDs und ungültige UUIDs/LLSD-Typen werden herausgefiltert. Lesen und Sanitizing der Itemnamen, Asset-IDs und Typen sind getestet. Die F9-Auswahl startet einen asynchronen On-Demand-Fetch; dessen Ergebnis bleibt mit Folder und Regionsepoche verknüpft. Ein Fetch-Ergebnis eines zuvor gewählten Ordners darf dem neuen nicht als aufgelöste Referenz untergeschoben werden. Die volle Avatar-/Wearables-Appearance-Pipeline ist noch nicht vorhanden.

**Block 3 – COF-/Outfit-Wear-Vorprüfung statt fehlerhaftem Blind-Wear:** Nach Vergleich mit Firestorm `indra/newview/llappearancemgr.cpp` überprüft `firestorm_outfit_wear_preflight` vorhandene Outfit-Items, Current Outfit Folder (COF, LLFolderType=46), referenzierte LLAssetType-24-Links und unverstandene Folder-Links (Typ 25). F9 zeigt einen expliziten Status für noch nicht geladene Inventare/COF, leere Outfit-Folder, nicht aufgelöste Linkziele und noch fehlende Wearable-/Attachment-/Avatar-Appearance-Synchronisation. `can_wear()` bleibt **absichtlich false**: Firestorm-Wear benötigt COF-Linktransaktionen, tatsächliche Wearable-Assets, Attachment-Regeln und Appearance-Abgleich. Der Server- und Firestorm-Quellvergleich erlaubt keinen erfundenen einzelnen `Wear`-CAP; die vollständige Outfit-Aktion bleibt offen. Ein User-Klick darf keinen falschen Erfolg anzeigen.

**Block 4 – explizit bestätigtes Firestorm Experience-Forget:** `F8` unterstützt bei vorhandener aktueller `ExperiencePreferences`-CAP nun zusätzlich `X` zur Vorbereitung von „Vergessen“, gefolgt von **Enter** zur ausdrücklichen Bestätigung (ESC Abbruch). Der Connector nutzt den OpenGenesisLINK-Vertrag `DELETE` auf `<current-capability>?<UUID>`, gegen ungültige IDs und abgelaufene Regionsepochen geschützt. `IHttpTransport::remove()` ist standardmäßig fail-closed, `CurlHttpTransport` unterbindet Redirects für mutierende DELETE-Requests. Erst wenn die Serverantwort die betroffene ID weder in `experiences` noch in `blocked` enthält, meldet der Viewer einen bestätigten Erfolg. Vorbereitete Aktionen werden auch beim Schließen/Wechseln zwischen F8/F9/F10 verworfen. Tests beweisen keine DELETE ohne gesondertes Enter, korrekten HTTP-Queryvertrag, Servernegativantwort- und Stale-Lease-Schutz. Experience-Erwerb, Rollen-/Admin-Bearbeitung und beliebige Write-CAPs bleiben gesperrt.

**Quelldisziplin, 11.10.2026:** Offizielle Firestorm-Basis `FirestormViewer/phoenix-firestorm`, Branch `Firestorm_7.2.4`, HEAD `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` einschließlich `indra/newview/llappearancemgr.cpp`, `indra/llinventory/llfoldertype.h`, Experience-Floater. Aktueller OpenGenesisLINK-Server `main`, HEAD **`885fa72a10fec770d6dc1220f08766437159a084`** – neuer als Alpha.84 (`efc9b3a`); darin u. a. ExperienceInfo-ID-Fix (`2194527`) und ein interaktives Firestorm-LSL-E2E-Testskript (`885fa72`). Für Inventory/COF sind `OpenSim/Framework/InventoryItemBase.cs` und `OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs` maßgeblich (inkl. `item_id`, `permissions.owner_id`, `agent_id`), für Experience-Forget `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Vor weiteren Codeänderungen immer aktuellen Server-HEAD erneut prüfen; eine lokal vorhandene Testdatei beim Server beweist KEINE vom Viewer vollzogene Live-Abnahme.

**Tests und Grenzen:** Für Alpha.85 wurden neue CTests `firestorm-inventory-items-tests` und `firestorm-inventory-tree-wear-tests` integriert sowie Forget-/COF-/F10-Owner-/Region-Regressionen erweitert. Insgesamt 80 CTests geplant und im finalen Debug-C++23-Versions-Neubau mit echten GPU-Tests zu prüfen. Ein synthetischer HTTP-Fixture-Test ist `tested_synthetic`, nicht `live_verified`. Der GUI-Welcome-Smoke verifiziert nicht die post-login F8/F9/F10-Fenster; echtes NexVerse-InWorld-Testing, Avatar, Bewegung, EventQueue/Teleport, Meshes und Outfit-Wear bleiben als nicht abgenommen markiert.

**Nächste 3–4 Blöcke Alpha.86:** Echte Firestorm-COF-/Wearable-Asset- und Attachment-Transaktion anhand `llappearancemgr.cpp` und OpenGenesisLINK-Serverrechten entwerfen/implementieren und mit Live-Avatar-Readback absichern; den F10-Baum um gezielten Item-Readback und Folder-EventQueue-Änderungen ergänzen; Experience-Consent-/Avatar-/Permission-Dialoge gegen aktuellen Simulator robuster machen; authentifizierte In-World-Abnahme (Avatar, WASD, Mesh und EventQueue) mit realem Grid-Zugang priorisieren. Volle Firestorm-Funktionsparität bleibt die verbindliche Baseline.


## 0.2.0-alpha.86 – Firestorm F10 Inventar, Items, Detailabruf · 11.10.2026

Block 1 – Lesender F10-Folder-Readback: FetchInventoryDescendents2 sendet
fetch_folders=true und fetch_items=true an die aktuelle vom Simulator
beworbene CAP. Antwort: authentifizierte owner_id, angefragte folder_id,
version >= 0, items[]; pro Item UUID, parent_id des aktiven Folders,
permissions.owner_id des angemeldeten Agenten, gültige Typen und bereinigte
Namen. Maximal 512 zurückgegebene / 256 dargestellte Einträge je Folder.
Keine serverseitige Wear- oder Inventory-Mutation.

Block 2 – versionsgebundene Sync und Lease: InventoryFolderContents hält
Items und Server-Folderversion zusammen. Older-version Responses werden
verworfen; neue leere Arrays können Einträge entfernen. Region-Lease
schützt gegen alte HTTP-Resultate; Regionswechsel invalidiert alle
Folder-Inhalte, Pollzeiten und Detailansichten. Maximal ein asynchroner
F10-Folder-Fetch gleichzeitig; 30-Sekunden-Polling für sichtbaren Folder,
R führt einen manuellen Refresh aus. Der InventoryTree bleibt auf 4096
Folder begrenzt; bestehende Nodes werden beim Limit noch aktualisiert.

Block 3 – F10 UI und Einzelitem: links reale Unterordner, rechts echte
Itemnamen aus dem CAPS-Readback, keine erfundenen Gegenstände. J/K und
Mausklick wählen Item; I fordert via aktueller ownergebundener
FetchInventory2-CAP nur die ausgewählte UUID an. Ergebnis passt
ausschließlich zur aktuellen Selection, Folder-ID und Regionsepoche.
F10 bleibt read-only; Auf/Ab/Enter navigieren den Ordnerbaum.

Block 4 – nachprüfbarer Protokollvertrag: Servercode
OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs,
OpenSim/Framework/InventoryItemBase.cs::ToLLSDxml und
OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs
zeigen die erforderlichen item_id, parent_id, permissions.owner_id,
version und item-/folder-Strukturen. Offizieller Server main
885fa72a10fec770d6dc1220f08766437159a084 und Firestorm Releasebranch
Firestorm_7.2.4 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d
vor diesem Entwicklungsblock verifiziert. Deterministische
Regressionen prüfen Owner-/Parent-Spoofing, versionsgebundene Updates,
empty Items, Fetch-Requestvertrag und Region-Invalidierung.

WICHTIG: Build/CTest/OpenGL-Welcome-Smoke werden lokal durchgeführt.
Sie ergeben nur tested_synthetic, keine bestätigte Live-Anmeldung und
keinen Firestorm-Vollfork. F10-Readback, Outfit-Wear, Avatar, WASD,
Teleport, Meshes, EventQueue und COF-Transaktionen müssen am
authentifizierten NexVerse-Grid weiter geprüft werden.

Nächste Blöcke: Inventar-Folder-Event-Diffs nach vom aktuellen Server
tatsächlich ausgesendetem CAPS/UDP-Vertrag; Firestorm COF/Wearables/
Attachment-/Appearance-Rechtemodell und Lifecycle; interaktive InWorld-
Abnahme inkl. Avatar, Bewegung, Mesh und EventQueue. Projektplan und
Firestorm-Funktionsbaseline bleiben verbindlich.
Branch: develop-current, Repo: marlin198323-glitch/nexverse-viewer.


## 0.2.0-alpha.87 – Firestorm/OpenSim Inventory EventQueue und Rechte · 11.10.2026

**Block 1 – echter Server-EventQueue-Vertrag:** Die OpenGenesisLINK
EventQueueGetHandlers.cs sendet BulkUpdateInventory mit AgentData.AgentID,
ItemData.ItemID/FolderID/OwnerID und FolderData.FolderID/ParentID.
Null-UUID-Folderdaten sind beim Item-Event Dummy-Records, kein
Inventarordner. Der neue Parser firestorm_inventory_events.cpp prüft
den authentifizierten Agent, einzelne Itembesitzer, UUID-Formate sowie
Arraygrenzen bis 512. Ungültige/fremde Daten werden nicht übernommen.

**Block 2 – Thread-/Regionsgrenzen:** Eine mutexgeschützte
InventoryEventInbox übergibt bis zu 256 eindeutige Folder-IDs vom
EventQueue-Netzwerkthread an den OpenGL-Main-Thread. Bei Overflow
wird ein begrenzter vollständiger erneuter read-only Abruf markiert.
Alte Regionsepochen werden abgelehnt. Ein veralteter Login-
EventQueue-Poll wird nach Teleport/Crossing nicht weiter betrieben.
Die vollständige neue LLUDP-/CAPS-EventQueue-Anmeldung ist noch offen.

**Block 3 – F9/F10 inkrementelle Aktualisierung:** Echte Eventhints
invalidieren betroffene F10-Folder und markieren diese für einen
erneuten CAPS-Request. Eine Invalidierungsrevision schützt die
asynchronen F10-Antworten vor einem neueren Ereignis. Für F9 werden
veraltete FetchInventoryDescendents2-Antworten ebenso abgelehnt;
eine begrenzte read-only Refresh-Anfrage folgt einem Ereignis
frühestens zwei Sekunden nach der letzten Abfrage. Kein Event wird
direkt als Inventardaten oder ein erfolgreicher Wear-Vorgang benutzt.

**Block 4 – F10 Item-Folder- und Auswahlprüfung:** FetchInventory2
validiert optional die parent_id eines im ausgewählten F10-Folder
angefragten Items zusätzlich zu Besitzer und Region. Eine ungültige
erwartete Folder-UUID blockiert bereits vor HTTP. Verschobene Items
oder verspätete Ergebnisse dürfen nicht als aktuelle Itemdetails
unter anderem ausgewähltem Item erscheinen. F9 bleibt ownergebunden.

Die neuen Negativtests pruefen synthetische EventQueue-Responses mit
Fremd-Owner, falschen Foldern, Dummy-Foldern, falschem Parent, zu
grossen Arrays, Regionsepoche und Overflow. Debug C++23 plus nun
81 CTests sind vor Release komplett erneut auszuführen. Ein
bestandenes CTest ist nur tested_synthetic, nicht live_verified.

Geprüfte Codequelle OpenGenesisLINK main:
ed37b9db720924c1d7105ea499a8c96430e9b7ea.
Gegenüber Alpha.86 nur ein Economy-/Hypergrid-Commit; Inventory-
Protokoll unverändert. Firestorm offizieller Releasebranch
Firestorm_7.2.4:
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
OGL ist weiterhin ein eigenständiger C++23-Viewer, kein vollständiger
Firestorm-Quellcodefork. Firestorm-Funktionsparität ist verbindlich.

Nächste Blöcke: Regionswechsel einschließlich neuem Simulator-
Circuit und CAPS/EventQueue-Lifecycle; authentifizierte Avatar-,
WASD- und Mesh-InWorld-Abnahme; sonstige Inventory-LLUDP-Änderungs-
und Löschpfade; COF-/Wearables-/Attachment-Protokoll mit tatsächlichen
Serverrechten und einem Avatar-Appearance-Readback. Projektplan
und Firestorm-Paritätsmatrix bleiben verbindlich.
GitHub Branch: develop-current (nicht develop).


## 0.2.0-alpha.88 – Region-Handoff, LLUDP/CAPS EventQueue · 11.10.2026

**Block 1 – Firestorm/OpenSim Region LLUDP handoff:** Die vorher nur mit
der Loginregion verbundene CircuitConnection wird beim Wechsel von
simulator_ip/port aus einem geprüften TeleportFinish-/CrossedRegion-
WorldModel-Ziel gestoppt. Nur nach Abschluss des bisherigen
Circuit-Futures werden der gemeinsame Outbound-Command-Queue und
lokale Movement-/Avatar-Identitäten zurückgesetzt und eine neue
CircuitConnection zum Ziel mit unverändert authentifizierter
AgentID/SessionID/CircuitCode gestartet. Neue Verbindung führt
UseCircuitCode, CompleteAgentMovement und RegionHandshake erneut aus.
Dabei läuft absichtlich nur EIN LLUDP-Worker zur gleichen Zeit;
Routen mit ungültigem Port/IP bleiben gesperrt. Ohne bestätigtes
AgentMovementComplete bleibt die Welt nicht SIMULATION READY.

**Block 2 – neue, regiongebundene EventQueueGet:** Der vorherige
Login-EventQueue-Worker beendet sein Polling, sobald WorldModel-
Regionhandle/Seed oder die FirestormRegionLease nicht mehr passen.
region_event_queue.cpp entdeckt den vom ZIELSIMULATOR über dessen
Seed-CAP angebotenen EventQueueGet-Endpunkt erst nach aktiver
passender LLUDP-Verbindung. Der Worker führt OpenSim-Long-Polling mit
separater ack-Sequenz, HTTP-502-Idle-Behandlung, Retry-/404-Seed-
Wiederabfrage und Ack-Reset nur bei neuer Endpunkt-URL aus. Nur
aktuelle Epoch-Events gehen an CapsEventDispatcher und die
ownergebundene Inventar-Inbox. In-flight alte Polls laufen aus und
werden ohne UI-Thread-Join am Frame-Ende entsorgt. Maximal vier
neue EQ-Futures gleichzeitig; beim Shutdown gejoint.

**Block 3 – Schutz gegen alte Regionen-Pakete und HTTP-Rückläufer:**
Der UDP-Paketcallback prüft nun vor jedem WorldModel-Dispatch die
aktuelle WorldModel-simulator_ip/port gegen die tatsächlich benutzte
CircuitConnection sowie das Stop-Flag. Stale UDP-Daten aus der
alten Region verändern somit nach Teleport nicht die aktuelle Welt.
Der Login-CAPS-Worker verwirft nach Regionswechsel alte Pollergebnisse,
einschließlich späte EnvironmentSettings/ExtEnvironment-Änderungen.
Bei LLUDP-Neustart werden Avatar-LocalID/RegionHandle unter
WorldModel-Mutex und Movement-Controller zurückgesetzt; alte
Outbound-Befehle werden nicht an den neuen Simulator gesendet.

**Block 4 – reproduzierbare Ablauf-/Protokolltests:** Neuer
RegionCircuitHandoff-Vertrag mit Aktionen unchanged, wait, launch,
invalid_destination sowie source-gebundener Packet-Guard.
Neue CTests region-circuit-handoff-tests und
region-event-queue-tests mit einer synthetischen CAPS-HTTP-Fixture
prüfen SeedCAP, ack, Duplicate-Batch, HTTP404-CAP-Refresh mit
eigenständigem ack, sowie Epoch-/Stop-Invalidierung. Zusätzlich
werden sämtliche bisherigen CTests samt realen GPU-Tests bei
DISPLAY=:0 und ein GUI-Welcome-Smoke durchgeführt.

Quellreferenzen am 11.10.2026 neu verglichen:
OpenGenesisLINK Server main:
f9ee61580a1b6aaf1d420b545ffce883b3dade0c,
seit Alpha.87 nur Wirtschaft/Hypergrid-Bank-Payout, keine
Änderung an Region-Caps/EventQueueGetHandlers.cs;
offizieller Firestorm Releasebranch Firestorm_7.2.4:
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.

**Grenzen:** Der neue mehrregionale UDP/CAPS-Lifecycle ist
tested_synthetic und gebaut; ein realer Teleport zu einem
zweiten NexVerse-Simulator, aktueller Avatar, WASD, Mesh-Skinning,
Inventar-Mutationen und COF-Wear sind weiterhin not_live_verified.
Firestorm bleibt vollständige Funktionsbaseline; OGL nutzt bislang
eigene C++23-Module und ist noch kein Firestorm-Codefork.
Eine EventQueue-Bestätigung allein ist noch keine Authentifizierung
einer vollständigen Avatar-Appearance.

**Nächste Blöcke:** Live-Login/Teleport und Avatar-Render/Movement
auf dem laufenden NexVerse/OpenGenesisLINK-Grid abnehmen; LLUDP-
Inventar-RemoveInventoryItem/RemoveInventoryFolder als serverseitige
Readback-Invalidierung anbinden; COF-/Wearables-/Attachments und
Meshes/Skeleton bis zum echten Avatar-Live-Readback vervollständigen;
anschließend weitere Firestorm-Parität nach projectplan.md.
Branch develop-current, OGL-Repository marlin198323-glitch/nexverse-viewer.


## 0.2.0-alpha.89 – echtes AgentUpdate-Quaternion, WASD/Avatar-Abnahme und Asset-Readback · 11.10.2026

**Block 1 – entscheidender OpenSim-WASD-Protokollfix:** Die aktuelle,
live angemeldete Alpha.88 hatte korrekt erkannte ControlFlags 1/2,
aber neun identische Simulator-Positionsantworten auf
(151.568924,193.071442,22.558577). Ursache im Viewer-Quellcode:
encode_agent_update serialisierte BodyRotation UND HeadRotation mit
je 16 Byte statt dem Linden-Message-Template-Typ LLQuaternion mit
je 12 Byte. Dadurch standen State, Camera-Vektoren und ControlFlags
auf dem Wire um zusammen acht Bytes versetzt; OpenSim konnte den
Tastenbits nicht das beabsichtigte ControlFlags-Feld zuordnen.
AgentUpdate High 4 hat jetzt genau 114 Byte Nutzdaten; ControlFlags
LE-U32 liegt bei Offset 109, Flags beim letzten Byte. Ein
bytegenauer Regressionstest prüft die Zielbytes und verhindet einen
künftigen stillen Offset-Regression. Die lokale Tastenerkennung
(WASD, Pfeiltasten, Shift, F) bleibt unverändert. Ob OpenSim danach
wirklich Positionsänderungen meldet, ist ohne eine frische
authentifizierte Sitzung **noch nicht live_verified**.

**Block 2 – Regionswechsel READY repariert:** Alpha.88 prüfte
world_model.region().simulator_ip/port irrtümlich weiter gegen
active_session->simulator_ip/port von der initialen Login-Region.
Das führte nach erfolgreichem Destination-Circuit zu einer
permanenten SIMULATION-NOT-READY-Sperre. Jetzt prüft
region_circuit_ready explizit den aktiven
circuit_target_ip/port und den Stop-Status. Die Unit-Tests prüfen
die Destination nach Wechsel, noch gestoppte und fremde Circuits.

**Block 3 – sichtbarer Avatar-Proxy als echte GPU-Aktivität
nachweisbar:** OGLRenderer zeichnete bereits eine klar provisorische
7-Teile-Selbstavatarfigur, zählte diese aber nie getrennt.
RenderDiagnostics enthält jetzt avatar_proxy_draw_calls und
self_avatar_proxy_draw_calls für tatsächlich ausgeführte
glDrawElements, über Frame-Reset abgesichert. Eine echte
GL3.3/GPU-Regression prüft sieben Self-Drawcalls, null nach
Entfernen und unveränderte Mesh-Szenezähler. Die laufende
InWorld-Anwendung meldet einmal pro Region erst nach
tatsächlichem Self-Proxy-Renderdraw eine Diagnose, deutlich
markiert als **KEIN** gebackener/skinnierter Firestorm-Avatar.
Die frühere WorldModel-Zeile avatars=0 betrifft allein die
Liste anderer Avatare, nicht die SelfAvatar-ID.

**Block 4 – Asset-Live-Auswertung und warmes Szenenfenster:**
Die echte Alpha.88-Sitzung am 11.10.2026 enthält 7
zusammenhängende InWorldAssetSnapshots. Letzter Snapshot:
252/256 Mesh-Assets (98%), 286/295 Texturen (96%),
vier fehlgeschlagene Meshes (1 missing, 3 invalid),
neun fehlgeschlagene Texturen (6 missing, 3 invalid);
keine GPU-Upload-Failures, dafür bei 200 Objekten noch
964 Fallback-Mesh-Drawcalls. Der Decoder versucht bereits
die anderen verfügbaren LODs und den alternativen GetMesh-CAP,
deshalb sind die echten Defekte/HTTP404 derzeit NICHT mit
synthetischen Geometrien zu überdecken. Vor dem Live-Grid-Abgleich
bleiben diese Assets serverseitig zu prüfen.

Der bestehende Acceptance-Prüfer benutzte bisher für die
Steady-State-Assetabnahme das Minimum der GESAMTEN Regionsepoche,
einschließlich der anfänglichen 2%-Downloadphase. Nun verwendet
er bei unveränderter Session-/Regions-/Sequenz-/Zeitvalidierung
genau die letzten konfigurierten N (normalerweise 3)
aufeinanderfolgenden, aktuellsten Numerik-Snapshots und verlangt
innerhalb dieses Fensters alle Draw-, Coverage- und
Fallback-Grenzen. Frühere Peaks bleiben im historischen Report.
Der Log-Report gibt dafür ausschließlich eine begrenzte,
numerische (max. 64), datensparsame Snapshot-Reihe aus. 15
Python-Regressionstests inklusive Warm-up, Stale-Sample,
fehlende GPU-Draws und unvollständige Fallback-Serien erfolgreich.
Die alte Alpha.88-Session bleibt mangels Frische und aufgrund
tatsächlich fehlender Meshes **not_verified**.

**Quellen:** Linden AgentUpdate Message-Layout:
https://wiki.secondlife.com/wiki/AgentUpdate;
LLQuaternion Wire 12 statt 16 Byte:
https://wiki.secondlife.com/wiki/Message_Layout;
offizieller Firestorm Releasebranch Firestorm_7.2.4
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d;
OpenGenesisLINK Server main
16d219f142926c7e00327e4ca91ecb1f8fc213e6.
Die letzte Server-Änderung modifizierte LSL/Economy,
nicht das AgentUpdate-Wire-Format.

**Weiter offen:** Eine neue echte Alpha.89-Login-Session
muss Bewegung anhand sich ändernder Serverposition,
sichtbaren SelfAvatar-Proxy im Live-Renderer sowie
aktualisierte Textur/Mesh-Asset-Readbacks nachweisen.
Skinned Avatare, Appearance/Bake, COF/Wearables/Attachments,
Regionen-Teleport End-to-End bleiben unvollständig.
Firestorm-Funktionsumfang weiterhin verbindliche Baseline;
eigene C++23-Viewer-Implementierung weiterhin kein
Firestorm-Quellcodefork.

GitHub Branch: develop-current; Repo:
marlin198323-glitch/nexverse-viewer. Bei Folgeturn
CHATUEBERGABE.md vor dem Projektplan lesen.


## Alpha.90 – LLUDP-Inventarentfernung und sichere Cache-Aktualisierung (13.10.2026)

Vier zusammenhaengende Aenderungsbloecke sind im GitHub-Quellstand vorbereitet:
1. Linden-/Firestorm-MessageIds Low 270 RemoveInventoryItem, 276 RemoveInventoryFolder und 284 RemoveInventoryObjects wurden explizit in die LLUDP-Protokollregistrierung aufgenommen.
2. Neue strikte Pruefung von AgentID + SessionID (UUID), je 1-Byte Variable-Blockzaehler und 16-Byte Inventory-IDs (hoechstens 64). Falsche Sitzung, ungültige Record-IDs, Restbytes, abgeschnittene und uebergrosse Pakete werden verworfen. Dies sind Hinweise vom verbundenen Simulator, keine direkten Inventar-Mutationen.
3. Im Live-PacketDispatcher werden solche Nachrichten mit der aktuell verbundenen WorldModel-Sitzung verglichen, anschliessend ueber die regionsgebundene FirestormRegionLease als full_refresh in dieselbe InventoryEventInbox geleitet. F9/Galerie, F10-Ordner und F10-Einzelitem-Details werden ueber den bestehenden MainThread/CAPS-Readback-Pfad invalidiert. Ohne verifizierte Folder-Parent-Kante wird nicht geraten oder geloescht. Wiederholte Hinweise werden zusammengefasst; alte Epochen verworfen.
4. Tests wurden erweitert: LLUDP-MessageIds, Single-/Dual-Variable-Block-Layout, gueltige/ungueltige Agent-/Session-IDs, Empty/Overlength/Truncated/Zero UUID, Inbox Full-Refresh, Regionepoch und Koaleszierung.

**Wichtige Einschraenkung:** Der autorisierte Linux-Rechner jam-MS-7D28 war am 13.10.2026 offline (Desktop Commander seit ca. 46 Stunden nicht verbunden). Deshalb wurde dieser Alpha.90-Quellcodeblock per GitHub-Repository aktualisiert, aber es fand **kein** lokaler C++23-Debug-Build, kein CTest, kein GUI-Test und keine authentifizierte InWorld-Abnahme statt. Der Block ist STATUS **committed_unbuilt**, nicht tested_synthetic oder live_verified. Ungetestete Beta-/Produktiv-Freigaben bleiben verboten; Build/Test beim naechsten verfuegbaren Desktop-Kontakt zuerst nachholen.

**Quellen:** FirestormViewer/phoenix-firestorm, branch Firestorm_7.2.4, scripts/messages/message_template.msg (RemoveInventoryItem 270, RemoveInventoryFolder 276, RemoveInventoryObjects 284). OpenGenesisLINK Server main 953063775933b6d7fab4f746d055587b8d74f2ea (IClientInventory.cs und Scene.Inventory.cs als Sendepfade). Der neue Server hat zudem ein optionales OGLViewerAdvisorModule; Viewerkennung `OpenGenesisLINK Viewer` bleibt kompatibel, sagt aber nichts ueber Funktionsparitaet aus.

**Naechste Prioritaet:** Build/Test/Paketlayout live pruefen, Alpha.89 AgentUpdate-WASD-Positionsdelta aus neuer Live-Session pruefen, SelfAvatar/Appearance und Mesh-Skinning, danach vollstaendige Inventar-COF/Wear-Transaktionen und End-to-End Teleport. Firestorm bleibt Funktionsbaseline, nicht austauschbar.


## 0.2.0-alpha.91 – Build-Hotfix, avatar server motion evidence und LLUDP-Regression · 13.10.2026

**Block 1 – Alpha.90 erstmals echt auf Linux gebaut:** Am 13.10.2026
war Desktop Commander auf jam-MS-7D28 wieder erreichbar.
Der vorher per GitHub veröffentlichte unbuilt-Stand Alpha.90 enthielt
einen Compilerfehler in firestorm_inventory_events_tests.cpp:
ein lokales SESSION-Lambda-Standardargument. Die UUID-Konstante
wurde in den Namespace-Scope verschoben. Anschließend liefen
83/83 CTests mit echter NVIDIA-GL-Umgebung erfolgreich.
Damit ist der unveränderte Alpha.90-Paketpfad erstmals gebaut
und synthetisch abgenommen. Der alte Hinweis committed_unbuilt
bleibt als historischer Zwischenstatus erhalten; Alpha.91 löst ihn ab.

**Block 2 – WASD-Reaktion des Simulators endlich getrennt messen:**
Neue header-only, mutex-geschützte
protocol::MovementFeedbackMonitor verarbeitet unidirektionale
AgentUpdate-WASD-ControlFlags auf dem UI-Thread und echte
serverautoritierte Self-ImprovedTerseObjectUpdate-Positionen
auf dem LLUDP-Worker. Nach wenigstens 25 cm Netto-Positionsänderung
in einer zusammenhängenden Bewegungsanforderung wird genau ein
"Avatar motion VERIFIED" protokolliert (nur serverbasierte
Positionsreaktion, ausdrücklich nicht Avatar-Bake/Animation).
Nach mindestens 4 Sekunden und 3 Terse-Samples ohne
Positionsänderung wird genau eine präzise WARN-Protokollmeldung
ausgegeben, keine voreilige Bewegungsfreigabe.
Gegensinnige Tasten und reine Rotation zählen NICHT als
Fortbewegung. Bei neuer Eingabe und beim Regionen-/Login-Circuit-
Reset werden alte Ergebnisse nicht fortgeschrieben. Der Monitor
manipuliert WorldModel oder Network-Payload nie. Deterministische
Tests prüfen Tasten, Stillstand, bestätigten Positionsdelta,
Warnungs-/Bestätigungs-Deduplizierung, neue Eingaben und
Teleport-Reset. **Eine frische authentifizierte Alpha.91-
InWorld-Session wurde nicht durchgeführt**, keine echte WASD-
Freigabe behauptet.

**Block 3 – zusätzliche sichere Server-Inventory-Varianten:**
Die Alpha.90-Parser für Firestorm RemoveInventoryItem (Low 270),
RemoveInventoryFolder (Low 276), RemoveInventoryObjects (Low 284)
wurden um Live-ähnliche Grenzwert-Regressionen für 64 gültige
UUIDs, 65 ungültige, zwei Variablenblöcke mit insgesamt 64
UUIDs, unpassende Nachrichtentypen und gebrochene Blocklängen
erweitert. Selbst valide Meldungen sind weiterhin nur
Readback-Invalidierungen; nicht die Freigabe, Servergegenstände
ohne Rückleseprüfung zu löschen.

**Block 4 – kompletter Firestorm-AgentUpdate-Wire-Golden-Test:**
Der bisherige Test prüfte 114-Byte-Nutzdaten und den
ControlFlags-Offset. Jetzt werden zusätzlich BodyRotation
LLQuaternion x/y/z, HeadRotation x/y/z, State, CameraCenter
und CameraAt, FarClip sowie Flags bytegenau gegen Firestorms
High-4-Layout geprüft. Dadurch fällt erneut eine
versehentliche 16-Byte-Quaternion oder vertauschte Camera-
und Controls-Position bei CI/CTest sofort auf.

**Aktuelle Quellreferenzen:** FirestormViewer/phoenix-firestorm
Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
OpenGenesisLINK/opengenesislinkserver main @
953063775933b6d7fab4f746d055587b8d74f2ea.
Auf Serverseite gibt es inzwischen unter anderem
OGLViewerAdvisor und IAR/Portal-Protokolle; diese sind nicht
mit einem vollständig implementierten Avatar-Wear/Appearance-
Protokoll gleichzusetzen.

**Abnahme:** Alpha.91 C++23 Build und alle CTests samt GPU-
Tests auf DISPLAY=:0 durchführen; Python Regression und GUI
Welcome-Smoke prüfen. Neues Logszenario hat nach 11.10.2026
bislang keine authentifizierte Sitzung. Nicht freigegeben:
echte NexVerse-WASD-/Teleport-Live-Abnahme, Firestorm Avatar
Bakes/Skeleton/Outfit-Wear/COF, fehlende Assets und originale
Firestorm-Vollparität.

**Folgepriorität:** Neue OGL-Login-Session starten, WASD
mindestens fünf Sekunden halten und auf die neue Avatar-
Motion-Diagnose prüfen; danach Position, SelfAvatar und
Textures/Mesh-Readback; echte zwei-Regionen-Circuit-/CAPS-
Teleports; serverseitige OGL-IAR/Appearance-Spezifikationen
für Wearables/COF und Attachments vergleichen.
Branch develop-current, Repository
marlin198323-glitch/nexverse-viewer, projectplan.md maßgeblich.
