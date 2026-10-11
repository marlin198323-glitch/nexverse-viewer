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
Aktuelle Viewer-Version: `0.2.0-alpha.67`.

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

Ab 05.10.2026 gilt Firestorm funktional als Mindestmaßstab für die üblichen Viewer-Kernfunktionen. OpenGenesisLINK bleibt technisch und gestalterisch eine eigenständige Implementierung.

Verbindlich eingeplant sind unter anderem Bewegung/Flug/Sitzen/Teleport, Kamera/Mouselook, Local Chat/IM/Gruppen, Freunde/Profile/Radar, Inventar, Attachments/Outfits/Gesten/AO, Karte/Minimap/Landmarks, Bauen/Object Edit, Parcel/Land, Media/Music/Voice, Environment sowie Diagnose-/Preferences-Funktionen.

Spezielle Firestorm-Erweiterungen wie RLV/RLVa oder sehr spezielle Firestorm-Power-User-Werkzeuge sind nicht automatisch Teil der Mindestbaseline und werden nur bei Bedarf ergänzt.


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
