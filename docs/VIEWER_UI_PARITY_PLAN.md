# OGL Viewer – verbindlicher UI- und Funktionsparitätsplan

Stand: 13.10.2026 / Alpha.97. Der Plan ist ab sofort
bei jedem "weiter" parallel zu projectplan.md fortzuschreiben.
Menüs, Fenster und klickbare Funktionen sind nicht mehr
nachrangig. NexVerse-Widescreen-Dark/Cyan/Violett-Design
bleibt stabil, Firestorm liefert Standard-UX/Featurestruktur,
OpenGenesisLINK/OpenSim das Protokoll.

## UI / tatsächliche Funktion (nur nach Weltbereitschaft)

| Menü/Funktion | Oberfläche | Verknüpfte Aktion | Stand |
|---|---|---|---|
| Oben Avatar / Kommunikation / Welt / Ansicht / Hilfe | Menüleisten, Dropdown und visueller Fokus | ViewerShell::hit / Router | locally_tested |
| Seitennavigation Welt | Anklickbarer Eintrag | Live Region-Floater mit RegionState | locally_tested |
| Seitennavigation Orte | Anklickbarer Eintrag | Teleport-Ziel-Floater | locally_tested |
| Seitennavigation Inventar | Anklickbarer Eintrag | vorhandener InventoryTree/CAPS-Floater | locally_tested |
| Freunde, Gruppen, Events, Marketplace | visuell "IN ARBEIT", nicht klickbar | keine unsichere Platzhalteraktion | disabled_pending_backend |
| Dock Chat | Aktiver Button / Chat-Fenster | LocalChatMessage -> Circuit ChatFromViewer | locally_tested; live_pending |
| Eingehender Chat | Chat-History im Floater | ChatFromSimulator, max 48 Zeilen, Mutex | locally_tested; live_pending |
| Dock Bewegen | Button / Hilfe | realer WASD/AgentUpdate-Controlpfad | locally_tested; live_pending |
| Dock Teleport | Button / XYZ-Felder | regionale TeleportLocationRequest via OutboundQueue | locally_tested; live_pending |
| Dock Inventar | Button / Tree-Floater | tatsächliche Inventar-Skeleton-/Items-CAPS | locally_tested; live_pending |
| Inventar öffnen/zurück/aktualisieren | sichtbare Buttons | Ordnernavigation / vorhandenes FetchFlag | locally_tested; live_pending |
| Dock Outfits | Button / Outfit-Floater | vorhandener COF/Outfit-Readback (nur lesend) | locally_tested; live_pending |
| Dock Erlebnisse | Button / Experience-Floater | bestehende Experience-CAPS / read-only | locally_tested; live_pending |
| Dock Karte | Karte-Fenster | nur lokale bekannte Avatarpositionen/Regiongröße | locally_tested; NOT grid-wide map |
| Avatar | Avatar-Floater | tatsächliche Appearance/Animation/Attachments | locally_tested; live_pending |
| Ansicht/Einstellungen | Optionsfenster | F11 und Klick Vollbild/Fenster | locally_tested |
| Hilfe | Steuerung/Funktionstasten | aktuelle Tastenkürzel als Bedienhilfe | locally_tested |
| F6/F7/F8/F9/F10/F11 | Tastatur & ESC | zugehöriges UI öffnen/schließen | locally_tested |
| Vollständige Firestorm-UI | große Oberfläche/Floater/DragDrop/RLV | nach Feature-Priorität zu implementieren | not_implemented |

## Ab sofort bei jedem Funktionsblock als Pflicht

1. Backend-/Protokoll-Feature und WorldModel bauen und testen.
2. Firestorm-Menü/Floater, Label, sichtbaren Button und Klickbereich
   im selben Entwicklungsblock implementieren.
3. Echte Daten im Fenster anzeigen und Aktivierungen an konkrete
   Outbound-, Capability- oder lokale UI-Handler verknüpfen.
4. Fehler-/Loading-/Erfolgsstatus ehrlich abgrenzen,
   InWorld-Rechte kontrollieren, Buttons bei nicht bereitem
   Backend sichtbar deaktivieren.
5. Mit Maus und Tastatur testen: Öffnen, Schließen, ESC,
   modale Eingabe, keine WASD-Leaks, Fenstergrößen 16:9/21:9/32:9.
6. Unit-, GPU- und UI-Smoketests + NexVerse-authentifizierter
   End-to-End Test sobald möglich; GitHub und .md aktualisieren.

## Nächste UI/Backend-Paare (Alpha.98 ff.)

- Animationen: LLKeyframeMotion-Clipdecoder und Avatar-Animations-
  Fenster mit Clip-/Status-/Priorität und Start/Stop nur
  wenn echte Protokoll-/Berechtigungssemantik vorliegt.
- Wearables/COF: Firestorm Inventar-Itemdetails, Wearable-
  Overlay und expliziter Tragen/Entfernen-Button mit
  verifizierter COF-Mutation, niemals Blind-Wear.
- Kontakt/Gruppe: Presence/IM/Event-CAPS und Freundes-/Gruppen-
  Liste, aktive Chat-/Invite-Floater, erst dann Menü freigeben.
- Bauen: ObjectEditing-Permissions/Rez-Undo plus Build-Tools
  und Kontextmenus, erst dann Aktivierung im Viewer.
- Teleport: Karte/SLURL/LM-Listen mit sicheren
  RegionResolve- und TeleportFinish-Simulatornachweisen.
- Viewer-Preferences: Grafik-/Sound-/Widescreen-/Video-
  Einstellungsfloater mit echter Änderung und Persistenz,
  statt scheinbarer Schalter.

## Technische Abnahme Alpha.97

- Gemeinsame Layout/HitTest-Einheit: ViewerShellLayout.
- Pixelzeichnung: src/ui/viewer_shell_render.cpp, OpenGL3.3.
- Keyboard/Maustasten: Application InputContext, auch Legacy-Floater.
- OutboundQueue sendet Chat und Teleport nur bei eigener
  aktiver Session/Welt mit ausdrücklichem Klick/Enter.
- tests/viewer_shell_tests.cpp prüft Menüs, Rail, Dock,
  Modal, Tastaturziel und XYZ-Validierung für drei Viewports.
- tests/viewer_shell_gl_tests.cpp rendert Chat, Region, Karte,
  Avatar, Settings und alle anderen Fenster in echtem GPU-Test.
- Ein echter Grid-Live UI-Durchlauf (NexVerse Login/World)
  ist erforderlich und steht noch aus.

## Alpha.98 – parallele Keyframe-/Animationsfenster-Entwicklung

- Sichtbare GUI: Avatar > Animationen und Animationen-Dock.
- Klick auf Zeile: echte aktive Server-AnimUUID und
  signierte SequenceID aus WorldModel anzeigen und auswählen.
- Nicht vorgetaeuscht: Animationsasset/Start/Stop steht als
  "noch nicht geladen" bzw. nur lesend, solange no CAP-Fetch.
- Parser: Firestorm v1.0/v0.1, Curve/Loop/Slerp-Kern implementiert.
- Renderer: wenn ein realer Clip extern bereitgestellt wird,
  Original-LLM-Avatar/GPU-Skeleton kurvenbasiert verformbar.
- Tests: ViewerShellLayout (3 Groessen), OpenGL Floater,
  Golden-Clip-Decoder sowie GPU echtes Firestorm LLM.
- E2E Grid-Login/Animation Asset-Download noch offen.
- Alpha.99: Download/Cache/Fade-Priority-Playback nur mit
  gleichzeitiger UI-Status-/Fehler-/Clip-Anzeige weiterentwickeln.

## Alpha.99 – echte OpenSim-Chat-Modi als sichtbare Bedienelemente

Der Chat-Floater besitzt nun die drei echten
Schaltflächen Fluestern, Sagen, Rufen, mit
aktivem Farbzustand und auswählbaren Hitboxen.
App::outbound_queue sendet tatsächlich
LocalChatType whisper 0, normal 1 oder shout 2.
Der Server entscheidet ChatRange und
Cross-Region-llListen-Weiterleitung. Keine
unsichtbaren Standard-Buttons mehr.
Menü, Chat-Eingabe, Modal-Fokus und
OpenGL/LLUDP-Tests prüfen die Integration.
Vor jedem weiteren UI-/Featureblock
aktuellen OpenGenesisLINK main SHA
und OGL Github develop-current SHA prüfen.
Siehe docs/OPENGENESISLINK_INTEGRATION_MATRIX.md.
