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
Aktuelle Viewer-Version: `0.2.0-alpha.29`.

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
- Firestorm dient ausschließlich als technische Referenz für Viewer-Verhalten, DPI/Scaling, Loading, Kamera und OpenSim-Funktionalität
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
