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

## Alpha.100 – GUI und LLUDP AnimationAsset synchron

Avatar>Animationen / Dock>Animationen: sichtbar und klickbar,
aktuelle AnimID/Sequence, tatsächlicher Transfer-Status,
gültiger Clip und Jointanzahl, Fehler und Retry-Button.
Nur bei Fehler erneut laden; kein vermeintlicher Start/Stop.
Viewport/GL Regression und Protokoll-Golden-Test, Live noch offen.

## Alpha.101 – sichtbare Objektberuehrung und Kanal-Chat

Neue Bedienoberfläche im NexVerse Viewer:
Welt > Objekte beruehren (echte Root-Prims <=15m),
auswählbare Liste mit LocalID und Weltabstand.
Klick auf BERUEHREN führt ObjectGrab Low117
aus, gehaltene GLFW-Maustaste wird ohne
künstliches Timer-spammen vom OpenGenesisLINK
Server als Touch-Hold behandelt; Release
führt ObjectDeGrab Low119 aus. Status im
Floater und ViewerShellLayout Hitboxen, GL3.3 Test.
Zusätzlich akzeptiert Lokaler Chat jetzt
Firestorm /7 Beep, /-1729 Private,
korrekt gesendeter S32 ChatChannel, und
meldet den Kanal in der UI.
Noch keine direkte Welt-Ray-Auswahl,
echter NexVerse Login-Smoketest offen.

## Alpha.102 – Direktklick in die Welt statt nur Liste

Freier Weltklick wählt ein echtes Root-Prim mittels
CameraRay und gedrehter BoundingBox. GUI schaltet
auf Objekte-Floater und selektiert das Objekt
sichtbar. Unteres Dock erhält Objekte-Knopf.
Mausloslassen und Fokusverlust werden von
echtem ObjectDeGrab abgedeckt. HUD-/Overlay-
Flächen lösen keine Weltaktionen aus.
Vollständige Mesh-Face-ID/UV-Interaktion
bleibt offen, kein Fake-Play/Touch-Resultat.

## Alpha.103 – Mesh-Face im Objektfenster und echter Outfit-Refresh

Welt > Objekte beruehren zeigt bei wirklich residentem
GetMesh FaceID/UV aus Triangle-Raycast, ansonsten
ausdrücklich BoundingBox-Näherung. Kein Phantomtreffer
in Lücken eines vorhandenen Meshes.
F9 Outfit Galerie besitzt einen neuen nutzbaren
Aktualisieren-Button mit Auth-CAPS
InventoryDescendents2 read-only, Besitzer-/
RegionEpoch-Check und >=2s Throttle. Kein
Tragen/Abnehmen, bis echte COF-Mutation steht.

## Alpha.104

Alpha.104: Welt-Objektfenster zeigt, ob gueltige SurfaceInfo an Simulator gesendet wurde. F9 Outfit-Galerie hat funktionierende Klicks auf Aktualisieren und COF pruefen inkl begrenztem FetchInventory2 und echtem Antwortstatus. Kein unechter Wear-Button.

## Alpha.105

F9 Outfit Galerie: neuer echter Aenderungsplan-Button und rechtseitige serverbasierte COF Diff-Vorschau mit +/−/=, Version und Listeneintraegen. Automatisch invalidiert bei neuen Inventory-CAPS/Region und manuellem COF-Verify. Keine aktiven Tragen/Ablegen Buttons ohne server-validierte Wear-Mutation.

## Alpha.106

Im F9 Outfit-Floater ist nun unter Aenderungsplan eine echte,
aber bewusst auf ein einzelnes ADD-LINK begrenzte Aktion verfuegbar.
Erster Klick: vorbereiten. Zweiter Klick innerhalb 20s:
authentifiziertes RELIABLE LinkInventoryItem an COF.
Der sichtbare Zustand unterscheidet Queued,
COF-serverseitig per echter InventoryDescendents2-
Versions+Linkpruefung bestaetigt und 'nicht
bestaetigt'. Alle Varianten mit
Delete/Mehrfachaenderung bleiben UI-gesperrt,
keine falschen 'Tragen' und 'Ablegen'-Buttons.
'COF pruefen' fragt jetzt den CURRENT OUTFIT
FOLDER statt nur das ausgewaehlte Outfit ab.
Alte in-flight CAPS sind nicht als neue
Benutzerabfrage erlaubt.

## Alpha.107

F9 Outfit-Galerie hat echte BatchAdd (2-4 Eintraege) und Single-Unlink Buttons im COF-Aenderungsplan, jeweils mit zweitem Bestätigen-Klick. Anzeige von COF-Versions-/AT_LINK-Readback, Fortschritt N/M, Abbruch und Unsicherheit. Komplett Tragen/Ablegen mangels AvatarAppearance-End-to-End noch NICHT freigeschaltet.

## Alpha.108

AvatarAppearance pruefen Button neu im F9 Outfit Floater: eigener echter Read-Only Check aus servergesendetem self AvatarAppearance CofVersion und aktueller COF InventoryDescendents2-Version. Begleitender automatischer Post-Mutation Evidence-Status (Pending/Stale/Newer/Matched/Timeout), keine Behauptung von sichtbarer Avatar-Bake. Outfit-Fenster 675 skalierte px und gesonderter unterer Status-Footer ohne Ueberlappung der zehn Ordner-Zeilen; gemeinsamer Layout-Code fuer Maus+Zeichnung, getestet 800x480 bis 5120x1440.

## Alpha.109

Alpha.109: F9 Outfit-Galerie zeigt jetzt echten klickbaren Button Wearables pruefen, lädt Originalitems des aktuellen COF ueber authentifizierte FetchInventory2 CAPS, zeigt rechtes Panel Shape/Skin/Hair/Eyes...Universal (17 Slotzeilen) und Anzahl. Stale COF/Region invalidiert Ansicht. Status nennt reale Validierungsfehler und keine visuelle Avatar-Wear-Zusage. Kleine und Widescreen Monitorhitboxen getestet.

## Alpha.110

Alpha.110: F9 Outfit-Viewer mit funktionalem links platziertem "Original-Wearable-Assets laden". Download nur nach authentifiziertem F9 Wearables pruefen auf gültigem unverändertem COF, max 16, seriell, 12s Timeout. Anzeige Original LLWearable Param-/TextureID und Head/Upper/Lower usw. Referenzstatus plus Fehler ohne falsches Tragen. Hitbox mit COF Batch getrennt, Fenster von 800x480 bis 5120x1440.

## Alpha.111

Alpha111 F9 zwei echte Buttons (Wearable-Assets laden, Texturen laden) und 17 Wearable Slot Mini-Bilder aus realen decodierten lokalen PNG/J2C Quellen, eigener Status-Footer ohne Overlap. Hitbox bei 800x480 bis 5120x1440. Wird bei COF/Regionsänderung korrekt freigegeben; volles Outfit Wear und Body Bake bleiben offen.

## Alpha.112-115

Alpha112-115 F9 neue Buttons TexLayer lesen und Teillagen mischen, neben bestehenden Wearable-Assets laden/Texturen laden, plus 3 Head/Upper/Lower partielle GL Farblagenbilder und neue TexLayer Footer Statuszeile. 800x480 bis Ultrawide Klickflächen geprüft. Vollständige Masken bleiben als nicht unterstützt sichtbar.

## Alpha.116-119

Alpha116-119 F9 Button Masken/Teillagen verwendet geprüfte lokale TGA-Masken und original Wearable Visual Param IDs. In 3 sichtbaren Head/Upper/Lower Preview-Feldern erhält man alpha-gesteuerte Teilbilder statt bloßer Textur-Overlays; Status weist mask counts und skipped/fehlende auf. Preview keine vollständig getragene Kleidung, kein wahrer Bake.

## Alpha.120-123

Alpha120–123 visible F9 TexLayer lesen reports count of color layers and params; Farben/Masken action creates authentic local-weight tinted/masked partial head/upper/lower previews and shows missing/color/mask blockers. Existing Appearance pruefen now displays server-origin nonplaceholder AvatarAppearance Baked TE UUID count out of 11 plus CAPS/Simulator COF version comparison; no fake wear-success.


### Alpha.124–127: original Firestorm global color & multi-mask diagnostics (2026-10-14)
Real avatar_lad.xml global skin/hair/eye palettes and only authenticated actual LLWearable type-owned parameters; two-plus-mask ADD/MULTIPLY order. Unsupported domain/morph/bump/visibility fails closed, F9 shows color/mask/11-region input counts. No pixel-identical Firestorm bake, no server upload or 3D clothing proof. New tests + projectplan, docs/ALPHA124_127_GLOBAL_MASK_COVERAGE.md.

Upstream OpenGenesisLINK 4e999766 (#210): Hypergrid friends reconnect status/negative cache are SERVER_ONLY; existing avatar online/offline viewer packets unchanged. Full viewer friends GUI compatibility remains CORE backlog, not separate admin credentials.

## 15.10.2026 – Alpha.128–131 Firestorm/OGL-Kompatibilität

F9 zeigt nach aktivem Appearance-Klick elf originale Server-TE-Bakes getrennt von GPU-Residency und Head/Upper/Lower. Kein vollständiger Bake, Upload oder Wear. Details in docs/ALPHA128_131_PI5_BAKE_EVIDENCE.md. Upstream main d31c0316 unreviewed (22 Commits, >=300 Compare-Dateien), Pin bleibt alt; 2 native C++23 und 18 Python Pi-Tests PASS, CMake/GPU nicht verfügbar.

## Alpha.132–135 (15.10.2026)

Server-echte Avatar Head/Upper/Lower BakedTextureIDs werden vor Welttexturen priorisiert. F9 zeigt nach explizitem Klick Read-only GPU-Residency live. Vollständige Firestorm-Bake-Pixelparität und Wear/Upload noch offen. Vier native Pi C++ plus 18 Python PASS; Upstream strict weiter BLOCKIERT. Details in docs/ALPHA132_135_AVATAR_BAKE_STREAMING.md.

## 15.10.2026 – Alpha.136–139 / Renderer + F9

Original Firestorm Avatar-Body-UVs werden vor GL-Upload validiert,
per-frame Head/Upper/Lower-Bake-GL-Draws pro originalem LLM-Körpermesh
gezählt, und in F9 als reine Draw-Provenance angezeigt. Kein Pixel-
oder Wear-Nachweis. Native Pi5 C++-Tests 5/5, Python 22/22.
Vollständiger Grafiktest NICHT erfolgt; OpenGenesisLINK Server-Pin
bleibt bis vollständiger Einzelcommitprüfung unverändert.
Siehe docs/ALPHA136_139_GPU_BODY_UV_PROOF.md.

## 15.10.2026 – Alpha.140–144 (Core/Firestorm/GUI/Server-Gate)

Read-only reale GPU-Server-Bake-Vorschauen Head/Upper/Lower in F9,
korrekter Shader-UV-State-Reset, Body Quell-MIP0 Priorität ohne
VRAM-Budgetumgehung. Ein Live-Inventar von 22 OpenGenesisLINK Commits
und 1276 File-Einträgen ist vollständig; manuelle Prüfung und
Kompatibilitäts-Pin stehen weiterhin aus. Native Pi7 C++23, Python28
PASS. Full GUI/GPU-Firestorm-Parität NICHT bestätigt.
Siehe docs/ALPHA140_144_GPU_BAKE_PREVIEW_AND_UPSTREAM_AUDIT.md.

## 15.10.2026 – Alpha.145–149

Identity-bound server Head/Upper/Lower, klickbare große F9 GPU Vorschau, responsive Layout und Region-/Logout-Reset. Firestorm Baked-Pixel und echter Wear/Unwear weiter OFFEN. Pi 8 C++23 und 31 Python PASS, voller GL-/GUI-Build NICHT GETESTET. Upstream main d31c0316 bleibt wegen unvollständigem Codepatchreview gesperrt; neue Live API Abfrage erhielt HTTP 403. Details: docs/ALPHA145_149_AUTH_F9_SERVER_REVIEW.md.

## 16.10.2026: Alpha.150-154

Original Firestorm 3-part body resource auto-discovery on Linux Mint. Original LLM GPU 7,186 triangles, 133 joints, 55 attachments loaded in isolated GUI smoke. Native NVIDIA GPU tests and all 113/113 CTest cases, 33/33 Python pass. J2C/JP2 decoder contract checked against new upstream server OpenJPEG fallback ec760cac, reviewed SHA not advanced. Inworld Avatar/Outfit/COF readback NOT yet proven. Detailed report: docs/ALPHA150_154_LINUX_MINT_FIRESTORM_GPU_BUILD.md.

## 16.10.2026 – Alpha.155-159

Original Nightly and Release Firestorm character assets all 96 byte-identical, nightly user-cache autodiscovery. Read-only F9 GPU Server-Bakes immediately visible and show 0/3..3/3 status. GL3.3 actual texture pixel readback and stale handle eviction verified on NVIDIA GPU. Login screen displays actual original body GPU-ready or fallback status. Mint full Build and native CTest 113/113, Python 34/34 PASS. Inworld Avatar/Outfit readback not verified. Upstream main ec760cac J2K fallback classified; reviewed baseline not changed. See docs/ALPHA155_159_NIGHTLY_F9_GPU_UI.md.

## 16.10.2026 – Alpha.160–164 (isolierte GUI, Avatar HUD)

Alle automatischen GUI-Viewer-Starttests ausschließlich Xvfb, keine sichtbaren Programme auf Nutzerdesktop. Inworld UI meldet jetzt tatsächliche Original-LLM-, Proxy- und Self-Head/Upper/Lower-Bake-Draws statt nur initialisierte Ressourcen; F9 und MIP0 wählen SelfAvatar per find_if (unabhängig von Vektor-Reihenfolge). Native x64 CTest 114/114, Python 38/38 PASS, echter Xvfb GUI-Smoke mit 133 Firestorm Joints und 7.186 Triangles PASS. Kein authentifizierter Simulator-Avatar oder echtes Wear/COF-End-to-End bestätigt. OpenGenesisLINK main ec760cac unverändert, server review bleibt offen. Siehe docs/ALPHA160_164_ISOLATED_GUI_AND_AVATAR_HUD.md.

## 16.10.2026 – Alpha.165–169

Simulator-authentifizierte SceneReady tritt nun vor später TerrainLayerData auf. Volle Terrain-Readiness bleibt ein separater Vertrag. Inworld HUD zeigt echte WorldModel Objekte und GL-Drawevidence sowie Terrain 0..256, CAPS Lifecycle ohne falsches LIVE. Native WorldModel, HUD und NVIDIA-GL Tests prüfen Szene und Meshdraw vor erstem Terrain-Paket. Inworld Login/Avatar und COF Wear/Unwear weiter offen, Server-Review-Pin bleibt unverändert. Siehe docs/ALPHA165_169_EARLY_SCENE_AND_WORLD_DIAG.md.

## 16.10.2026 – Alpha.170–174

AvatarPipelineProof liefert read-only authentifizierte Agent/Region AvatarAppearance Server-TE/Bake und GPU-Residency sowie tatsächlich im Frame gezeichnete Original-LLM/BakeGL-Werte. Neue sichtbare Inworld-Karte mit H/U/L und Packet-Missing Status; positive Farbe nur bei kompletten 3 Texturen und 3 originalen Meshdraws. Überlappte CAPS-/Systemstatuskarten repariert; enge Fenster ohne überdeckte FPS-Leiste. GPU-Regression mit tatsächlichen GL-Submissions und Eviction, Native Header-Pi5-Tests. Kein vollständiger E2E Grid-Login, kein bestätigtes Outfit-Wear, Server-Reviewed-SHA bleibt blockiert. Details: docs/ALPHA170_174_AVATAR_PIPELINE_GUI.md.

## 16.10.2026 – Alpha.175–179

Inworld-Status zeigt WorldModel/GL und nun Textur/Mesh GPU-Residency mit eindeutigen aktuellen UUIDs getrennt. Responsive Netzwerk-/System-/Avatar-Karten überdecken keine FPS-Leiste bei niedrigen Fensterhöhen; Tests an Layoutbreakpoints. GL3.3 Nvidia Regression bestätigt echte Texture-Residency-zu-Eviction-Verläufe. Firestorm originale Body-/Bake-/Outfit-Pipeline bleibt Basis, keine Inworld-E2E-Freigabe. Server reviewed SHA unverändert ec760cac main als Referenz. Siehe docs/ALPHA175_179_WORLD_ASSET_STREAM_AND_RESPONSIVE_HUD.md.

## 16.10.2026 – Alpha.180–184

Die OpenGenesisLINK/NexVerse Third-Person-Kamera normalisiert ungültige/nicht-unitäre Agent-Quaternions und begrenzt unplausible Serverpositionen auf einen endlichen lokalen Fallback. Inworld GUI signalisiert Kamera-Fallback; Original-Firestorm-/World-GL-Mesh bei normalem und ungültigem Quaternion-Fallback mit echten GPU-RGBA Framebuffer-Pixeln auf verstecktem NVIDIA OpenGL3.3 getestet, Terrain-LayerData nicht vorausgesetzt. Ein echter eingeloggter Avatar und endgültige Firestorm-Bake-/Outfit-/Szenenparität sind weiterhin NICHT bestätigt. OpenGenesisLINK main ec760cac, Server-reviewed-SHA unverändert. Details: docs/ALPHA180_184_THIRD_PERSON_CAMERA_AND_GL_FRAMEBUFFER.md.

## 16.10.2026 – Alpha.185–189

Firestorm-kompatible Weltbedienung erweitert: direkt anklickbarer HUD AUS/AN F5 Weltfokus mit erhaltenem 3D-Renderer, F9 und Floater-Funktionen. Hidden Hud verhindert Ghost-Bedienflächen und leitet Weltklicks zur bestehenden 3D-Objektauswahl. Das OpenGL-Backend verwirft vollständig hinter der Kamera liegende Szenenobjekte statt sie als Welt-Draws zu zählen; NVIDIA-GL Framebuffer/Gegenblick-Regression plus native C++23 Widescreen/Renderer-Tests. Echter authentifizierter Grid-/Avatar-Framebuffer steht weiterhin aus. Upstream ReviewedSHA gesperrt. Siehe docs/ALPHA185_189_WORLD_FOCUS_AND_VISIBILITY.md.

## 16.10.2026 – Alpha.190–194

Firestorm-kompatibler Third-Person-View-Zoom nun per Wheel, Keyboard und echten anklickbaren +/- Button mit Live-Meterzahl von 3..24m. Renderer, Raypick und Texture-/Mesh-Priorität nutzen dieselbe Pose, ohne Simulator-Movement zu verändern. F9 Read-only Server-Bake-Vorschau hat sichtbare B-Taste. NVIDIA-OpenGL3.3-Framebuffer-Pixeltest bestätigt reale Near-/Far-Zoomwirkung am synthetischen Weltmesh. Echter Grid-/Avatar-Pixelbeleg und komplettes COF Wear weiter offen; Server-ReviewedSHA unverändert. Siehe docs/ALPHA190_194_CAMERA_ZOOM_AND_F9_BAKES.md.

## 16.10.2026 – Alpha.195–199

Echter lokaler Avatar-Kameraorbit: Front/Seite/Hinten per V/[ ] und drei sichtbare, klickbare Schalter. Firestorm-basierter GL-Renderer und WorldRay-Picking nutzen gleiche Pose, Avatar bleibt simulatorauthentisch. NaN-/Inf-Zoom-Normierungsfehler behoben. Native Mint-/Pi5-C++-Tests und echte NVIDIA-OpenGL-Pixelprobe aus Front- und Rueckansicht mit synthetischen Meshes. Echter NexVerse-Login mit sichtbarem Avatar und vollstaendige 11-Bake-/Outfit-Paritaet weiterhin ausstehend. Server reviewed SHA unveraendert. Details: docs/ALPHA195_199_AVATAR_CAMERA_ORBIT.md.

## 16.10.2026 – Alpha.200–204 Branding

Das vom Nutzer bereitgestellte Original-OG-Viewer-Icon wird als echtes GLFW/X11-Cinnamon-Taskleistenicon und OpenGL-Logo oben links im Login verwendet. Linux hicolor Starter, Windows ICO und macOS ICNS stehen als native Ressourcen bereit. Vorhandener WelcomePage WebKit Browser und Grid-Login bleiben unverändert; nur Navy/Cyan-Details frischer gestaltet. Keine Full-UI-Mockups oder Server/Avatar-/Wear-Protokolländerungen. Siehe docs/ALPHA200_204_BRANDING_SYSTEM_ICON_AND_LOGIN.md.

## 16.10.2026 – Alpha.205–209

Kamera-Blickfeld-Projektion anhand aktueller AgentPosition, verifizierter Avatar-Render-GL-Draws und Originalicon in der 3D-Welt-HUD-Kopfzeile. Loading-Overlay ermittelt konkreten fehlenden Welt-Entry-Schritt (LLUDP, Region oder Agent), ohne optionales Terrain/CAPS als Voraussetzung zu behaupten. F5 meldet ausschließlich fehlenden Avatar im leeren Weltfokus. Live Grid-Avatar-/Pixel-Parität weiter unbewiesen; keine COF- oder Server-Änderung. Details: docs/ALPHA205_209_WORLD_ENTRY_AND_AVATAR_VIEW_EVIDENCE.md.

## 16.10.2026 – Alpha.210–214

Verbindlicher fünf-Blöcke-Durchlauf je WEITER. Echter OpenGL-3.3 Query für Front/Upper/Lower Original-LLM Self-Avatar-Draws: GPU-Depth-Fragment ja/nein/unknown statt irreführender Draw-Proof, async+RegionEpoch sicher, Native GPU-Regression. Echte interaktive F4 AVATAR Fokus-Taste und Button neben den bisherigen Orbit-/Zoom-Controls (8m, frontal), in beiden F5-Modi. WELCOME und Originalicon bleiben erhalten. Kein NexVerse-Live-Grid-/Appearance-Pixel-E2E-Claim. Dokumentation: docs/ALPHA210_214_FIVE_BLOCK_RULE_AVATAR_SAMPLES.md.

## 16.10.2026 – Alpha.215–219

GPU-Avatar-Mesh-Fragmente dürfen nach Kamerabewegung/Originalmesh-Ausfall nicht als aktuelle Werte weiterlaufen. F3 (bestehender Avatarstatus) zeigt aktuelle Kamera-/Original-Linden-Mesh-/ServerBake-/Depth-Samples und hat echten [S]-Bericht-Button; lokaler Export mit POSIX 0600 und Whitelist ohne Identität, UUIDs oder Geheimnisse. F4 Kamera, Login, original OGL-Icon, WebKit-Welcome, F9 Outfit, und bestehende Server-LLUDP unverändert. Kein tatsächlicher NexVerse-Avatar-End-to-End-Beweis. Details: docs/ALPHA215_219_AVATAR_DIAGNOSIS_AND_QUERY_FRESHNESS.md.

## 16.10.2026 – Alpha.220–224

F3 WorldEntry-Diagnose vor WorldReady, echte getrennte LLUDP/Region/Agent/CAPS-Flags, private Datei ohne Secrets, anklickbarer manueller Circuit-Retry nur nach vollständig fehlgeschlagenem Worker und bekanntem Simulatorziel. Loading-Progress aus Pflichtmilestones, nicht CAPS/Terrain. Original-NexVerse-WebKit-Welcome und Firestorm-Outfits/UI bleiben unangetastet. Kein echter Grid-Avatar/Retry-E2E. Details docs/ALPHA220_224_LOADING_DIAGNOSTICS_AND_MANUAL_RETRY.md.

## 16.10.2026 – Alpha.225–229

Fünf echte Entwicklungsblöcke pro WEITER im neuen Chat dauerhaft verbindlich; pro Session max. fünf WorldEntryStage-Enum-Werte mit Haltezeit in F3 (nur vertraulichkeitsfreie Fakten), LLUDP Retry-Grund aus tatsächlicher abgeschlossener Circuit-Worker-/Region-Endpoint-/Cooldown-Policy und privatem explizitem POSIX 0600-Diagnoseexport. Klickbarer F3-Ladestatus und Original Welcome bleiben erhalten. Kein echter NexVerse-Welt-/Avatar- oder Retry-End-to-End-Beleg. Bericht docs/ALPHA225_229_WORLD_ENTRY_TRACE_AND_RECONNECT_REASON.md.

## 16.10.2026 – Alpha.230–234

Self-AvatarAppearance realer LLUDP-Handler-Status, GL-Terrain-Shader Textur/Fallback-Draw sichtbar, F2 DETAIL/SPAREN steuert MeshLoD Hysterese und Prioritaet inklusive Neuplanung, Kamera auf 5.5m und groessere HUD Statusschriften. Welcome/Icon bleiben unveraendert. Echter ServerBake/Welt-Pixelbeweis weiterhin OFFEN. Bericht docs/ALPHA230_234_VISIBLE_WORLD_AVATAR_MESH_UI.md.

## 16.10.2026 – Alpha.235–244

Verbindlich zehn echte Entwicklungsblöcke je WEITER und
Projektstatus 38 % unverändert. Neue echte Self-Avatar-
BodyBake-/Keyframe-GL-Diagnosen, 4/4 Terrain-RGBA-Layer
gegen 1x1-Ersatz, exakte Mesh-UUID+LOD-Matches auf der GPU
und zusätzliche Anzeigen in HUD, F3 Avatar/Objekte und
privatem 0600-Diagnoseexport. Weiterhin KEINE bestätigte
vollständige Firestorm-/NexVerse-Live-Avatar-, Terrain-
oder Serveranimations-Parität. Original-Welcome/Icon erhalten.
Details: docs/ALPHA235_244_VISUAL_EVIDENCE.md.


## 17.10.2026 – Alpha.245–254

Echte Terrain-GetTexture-Partial-Recovery mit maximal
vier Backoff-Versuchen pro Materialregion, kumulierte
bereits echte RGBA-Pixel, fortschreitender OpenGL-Upload
ohne Materialrevisionswechsel und ohne Downgrade.
Vorher fehlerhaft kummulierte Terrain-Draw-Zähler jetzt
pro Frame korrekt; NVIDIA-GPU-Test statt bloßer
Netzwerkzähler. F7 Region zeigt Retry/Mesh, Self-Avatar-
Appearance-Zähler nach Regionen getrennt, Animationen-
Fenster und Logs zeigen echte Keyframe-Skin-Nutzung.
Neuer OpenGenesisLINK-Server-Mesh-Upload-LLSD-Patch
d8f4426 ist nur auf Uploads bezogen, nicht GetMesh.
Project-Status 39 % als gewichtete Schätzung; echte
NexVerse-Firestorm-Avatar-/Welt-Pixelparität weiterhin OFFEN.
Siehe docs/ALPHA245_254_TERRAIN_RECOVERY_ANIMATION.md.

## Alpha.645–654 – F7-ID-Lebensdauer und Bewegungsevidenz
F7 zeigt nun tatsächliche KillObject-Entfernungen und implizite
Avatar-/Prim-LocalID-Rebinds als unterschiedliche, vom WorldModel
stammende Zahlen. Echter Self-Kill oder neue LocalID entwertet vorhandene
WASD-/Yaw-Erfolgsbestätigung. Vorhandener funktionierender F7-Floater,
keine Fake-Buttons. Inworld-Pixeltest weiter offen.

## Alpha.655–664 – F7 zeigt belastbare Szenenintegrität

F7 Region/Simulator hat jetzt drei getrennte Diagnosezeilen: empfangener
KillObject Avatar/Prims mit Avatar-ID-Neubindung; Prim-LocalID/UUID-Rebind
und zurückgewiesene ungültige Szenendaten; kombinierte echte Terrain-/
Layer-Nachladeinformationen. Labels passen in die 85-Zeichen-Grenze,
Fehlerzähler erscheint als Warnung. Kein Fake-Button. Tatsächliche
Session-/Region-Werte, nicht erfundene Render-Erfolge. High9, Full- und
Compressed-Self-Avatar-Posen müssen vor WASD-/Kamera-Evidenz gültig sein.
Grafischer Live-Xvfb-/NexVerse-Avatar-Frame steht noch aus.

## Alpha.665–674 – Bürgerordnung vor Weltbeitritt
Das originale Prelogin zeigt bei echter reason=tos-Antwort
ein modales Bürgerordnungsfenster mit offiziellen URL-Button,
ausdrücklichem Zustimmen (vorher deaktiviert) und Ablehnen,
einschließlich ESC und blockiertem Background-Login. Nach
Zustimmung wird für denselben Avatar/Grid das Passwort
neu verlangt. 1100x700, 1280x800 und 3440x1440 HitTests
vorhanden. Kein nachgebauter Fake-Webview und keine
stillschweigende Legal-Akzeptanz. Authentifizierter Browser-/
Simulator-End-to-End-Pixeltest noch offen.

## Alpha.675–684 – Modal-Fokus und echter Linux-x64-UI-Build
Prelogin-TOS-Dialog reagiert nun auf Tab, Shift+Tab, Enter,
Numpad-Enter, Leertaste und ESC mit sichtbarer cyanfarbener
Fokusmarkierung. Nicht freigeschaltete Zustimmung wird übersprungen.
Escape lehnt ausdrücklich ab und schließt nicht mehr das Viewerfenster.
Browserstarter bereinigt POSIX-Kindprozess; Windows benötigt shell32.
Neuer gehosteter Linux-x64-GUI-CMake-Workflow auf develop-current
baut den echten Viewer. Erfolgreicher UI-Framebuffer-/Pixeltest,
Login und vollständig sichtbarer Avatar noch offen.

## Alpha.685–694 – Avatar HUD zeigt fehlerhaften Hover
Die vorhandene Avatar-Pipeline zeigt nun beim Auftreten von
nicht endlichen Hover-Serverwerten einen knappen Warnstatus
Hover korrigiert: Self 1 / Fremde N statt eines irreführenden
Erfolgs. Die Werte stammen aus den wirklich vorbereiteten
aktuellen AvatarProxy-Daten pro Frame. Bei gültigen Daten
bleibt die gewohnte Appearance-Anzeige unverändert.
Keine Fake-Inworld-Daten, kein angeblich fertiger Skin.

## Alpha.695–704 – echte Linux-Mint-GUI-Build-/Icon-Prüfung
Primäres Arbeitsgerät jetzt Linux Mint Desktop Commander jam-MS-7D28
mit RTX 4070 SUPER und CMake/vcpkg. Lokaler kompletter x64-Debug-Viewer
kompiliert; 174 native CTests (inkl. GL und Firestorm Avatar),
Xvfb Viewer Start mit 133 originalen Skelett-Joints, drei originalen
Bindpose Meshes und UI-Renderer, WM_CLASS opengenesislink-viewer +
echtem Taskleisten-Icon 128x128 BESTANDEN. Scripts:
scripts/mint-viewer-validation.sh, mint-isolated-gui-smoke.sh und
test-mint-window-branding.sh. Kein Start im interaktiven Desktop
während automatisierter Tests, kein belegtes NexVerse Live-Avatar-E2E.

## Alpha.705–714 – Bürgerordnung-Modal verwendet aktuelle URL
Aktueller Serverpfad /service/tos jetzt in realer sichtbarer
Modal-Textzeile, externem Browser und Copy-Fallback. Alter
Server /buergerverordnung nutzt nur seine bekannte offizielle
Legacy-Version. Beide URLs sind zuvor auf einen fixen
HTTPS-Wert normalisiert, nicht aus der rohen XMLRPC-Antwort
übernommen. Passwort-Neueingabe und manuelle Zustimmung bleiben.

## Alpha.715–724 – F3 Avatar-Pose entspricht tatsächlicher GPU-Position
Echter GL-Renderer prüft Self/Remote AvatarProxy vor Original-Firestorm-
Mesh-Draw gegen nichtfinite/unplausible Weltposition und ungültige
Quaternionen. Das UI zeigt Self-Drop, Remote-Drops und
reparierte Rotation aus aktuellen RenderDiagnostics pro Frame an;
es darf einen verworfenen Self-Avatar nicht als sichtbar ausweisen.
Frustumstatus verwendet die tatsächliche Low158-Hover-
bereinigte Proxyposition statt unverschobene Agent-Physikposition.
Nativer x64/Mint Original-LLM-Framebufferpixeltest, kein Grid-Live-Avatar.

## Alpha.725–734 – Bewegung und Avatar-Antwort ohne Fake-Erfolge
Mit der echten WASD-/Tastatursteuerung wird bei kaputter
Simulatorposition kein AgentUpdate versendet. Eine warnende
Logzeile erfolgt nur einmal bis zur Wiederherstellung;
MovementFeedbackMonitor wird zurückgesetzt und meldet
keine erfundene Serverbewegung. Bei validem Serverpose
läuft der bisherige Firestorm-artige WASD-/Yaw-/Fly-
Eingabepfad weiter. Willkommen-Seite stimmt mit aktueller
NexVerse GridInfo überein; authentifizierte Inworld-Kontrolle
bleibt ein gesonderter Abnahmetest.

## Alpha.735–744 – Einzelner Avatarname ohne Zwangsnachnamen
Echtes GLFW-Loginfeld lässt jetzt neben Vorname Nachname
auch einen einteiligen Kontonamen zu. Der Parser ermittelt
die Form, der LoginService sendet dafür das vom aktuellen
OpenGenesisLINK Server #268 unterstützte XML-RPC username.
Validierungsfehler verlangen keinen künstlichen Nachnamen.
Ausdrücklicher TOS-Dialog und Passwortschutz unverändert.
Xvfb GUI-Smoke ist kein authentifizierter Avatar-GPU-Test.

## Alpha.745–754 – Skin-Nachprüfung und tatsächliche GPU-Zulassung
Die im Viewer vorhandene manuelle
Avatar-GetTexture-Nachprüfung sowie automatische
Bake-Refresher berücksichtigen jetzt ausschließlich
Self-Bake-Quellen von wirklich GPU-zugelassenen
Avatarpositionen. Bei Server-NaN kann das UI keine
falschen Nachprüfungs- oder Full-Mip-Bereitschaften
mehr aus Avatar-Assets ableiten, die nicht gerendert
werden dürfen. Normale valide Bakes laufen durch
bestehendes GetTexture-/Mip-/VRAM-System weiter.
Die optische Live-Inworld-Abnahme bleibt offen.

## Alpha.755–764 – Avatar-Outfit nicht durch COF-Replay wechseln
Die bestehenden Skin-/Avatarstatus-Fenster spiegeln reale
WorldModel/Renderer-Daten. Neuer Backend-Fix verhindert,
dass verspätete Low158-Nachrichten mit älterem COF plus
trailing unbekannter COF-Version die aktuelle Skin
überschreiben. Sichtbar kontrolliert durch Original-
Body-OpenGL-Framebuffer-Regression. Kein künstlicher
Outfit-Success-Toast; Livetest noch offen.

## Alpha.765–774 – visuelle Skin-Konsistenz bei Low158-Formatwechsel
Das bestehende Viewer-F3-/Avatar-Diagnosepanel nutzt die
tatsächlich vom WorldModel akzeptierten Low158-Daten.
Eine alte Outfitversion aus einer früheren
AppearanceVersion-Reihe wird nun ebenfalls verworfen,
wenn zwischenzeitlich eine andere Formatserie aktiv war.
Die vorhandene Zählung verworfener COF-Pakete steigt
tatsächlich; keine gefälschte Live-Skin-Success-Anzeige.
Realer Xvfb GL Original-Firestorm-Körperpixeltest;
sichtbarer eingeloggter Inworld-Avatar weiter offen.

## Alpha.775–784 – aktuelle F9-Wearables trotz 32-Bit-Wrap
Der vorhandene Wearables-/COF-Diagnosebereich erhält
den letzten authentifizierten Low382-Snapshot jetzt
auch nach SerialNum-Überlauf, statt dauerhaft alte
Readback-Daten anzuzeigen. Kein gefälschter
Avatar-Skin-Erfolg aus Wearables allein; native
Low382-Datagramme und Modelzustand getestet.
Echte NexVerse-Live-Avatar-Abnahme weiter offen.

## Alpha.785–794 – Lokale Turn-Vorschau statt falscher Serverdrehung
Das ThirdPerson-Kamerabild kann bei einem echten
eingereihten Drehkommando kurz auf die eigene
Wunschrotation reagieren. Die Original-Firestorm-
Self-Renderkopie darf denselben max. 350-ms-Heading-
Preview verwenden. F3-Serverbestätigungsfelder beruhen
dagegen weiterhin ausschließlich auf echtem Simulator-
Readback. Sichtbarer authentifizierter Live-Test offen.

## Alpha.795–804 – Statusanzeige nur für tatsächlich gequeue-te WASD-Steuerung
Die Self-Avatar-Walk-Fallback-Animation beruht auf
last_reported_control_flags. Zuvor war dieser Status
vor erfolgreichem Queue.push aktualisiert worden; eine
mit zuverlässigen Nachrichten gefüllte Queue konnte
daher fälschlich einen laufenden Avatar melden.
Jetzt Flag/Log/Preview/MovementFeedback erst nach erfolgreichem
Queue-Push, ansonsten Rücknahme lokaler Turn/Fly-Zustände.
Die lokale Kamera passt sich nach einer echten serverseitigen
Headingänderung ohne gehaltene Drehtaste wieder der
authentischen WorldModel-Rotation an.

## Alpha.805–814: F3 Skin-Warnung bei altem GPU-Texturstand
Das bestehende F3 Avatarfenster erhält eine reale
Rückmeldung, wenn Low158 die aktuellen Bake-IDs liefert,
OpenGL aber noch ein anderes Head/Upper/Lower-Texture
zeichnet. Das ist eine Wartestufe, kein grüner
aktueller Outfitnachweis. Für provisorische Objekt-TE
wird derselbe Vergleich gesondert angewendet.


## Alpha.815–824 – Visible F9 outfit/recheck status
When a new genuine outfit is selected, the Bake Recheck button
may return if actual current textures are missing and CAP,
backoff and inflight gates allow it. Different warning
labels distinguish 3/outfit vs. total 12/region. No button
may bypass real server ownership or asset cooldown.


## Alpha.825–834 – F9 manuell und Live ohne COF-Fehlanzeige
F9-COF-Status zeigt bei aktuellem mehrteiligen
Low158-Paket mit einer gültigen COF-Version und
trailing COF=0 nicht länger irrtümlich COF 0.
Der manuelle Button und die alle 750ms aktualisierte
Live-Anzeige greifen nun auf denselben selektierten
Wert wie der echte WorldModel-Antirollback-Guard zu.
Bei fehlender COF-Revision bleibt OFFEN angezeigt.


## Alpha.835–844 – F3 Verbindung nicht als eigenen Avatar verwechseln
Der Avatar-Pose-Indikator und der datensparsame
Diagnosebericht nutzen die gleiche bestätigte Pose
wie das eigene Original-Firestorm-Renderproxy.
Ein grüner Circuit kann weiterhin Weltstreaming ermöglichen,
aber ohne AgentMovementComplete/Self-ObjectUpdate
bleibt der Status AgentPosition fehlt sichtbar.
Nach Region-/Sessionwechsel wird erneut authentifiziert.


## Alpha.845–854 – F3 früher Avatar/Outfit-Readback
F3 Avatarstatus unterscheidet sofort erhaltene
Low158/Low382-Paketzähler von tatsächlich im WorldModel
akzeptierter Self-Appearance. Falls Serverpakete vor
UI-CircuitState::active eintreffen, erfolgt die gültige
Übernahme nach Freigabe und erst dann erscheinen
COF-/Outfit-Status und aktualisierte F3-Accepted-Zähler.
Keine Anzeige von Skin-GPU-Success vor echtem GL-Readback.

## Alpha.855-864 F3 early Low158/Low382 pending visibility
F3 reports Low158 gepuffert (warte Circuit) and
Low382 gepuffert (warte Circuit) from locked WorldModel.
Pending delivery is separate from authenticated avatar
appearance, COF, GPU resident skin or original body GL draws.
No new interactive login or network requests.

## Alpha.865-874 - F3-Verbindungs-/Avatar-Pose nach Socket-Reconnect
F3/Pose-Anzeige folgt bestehendem WorldModel und zeigt nach echtem IP-/Portwechsel ohne neue Simulator-Position keine falsche bestehende Eigendargestellung. Keine neuen Bedienelemente oder unrealistischen Erfolgs-Badges. Identischer Socket und unvollstaendiger Zwischenhandshake sollen keine unnoetigen visuellen Zuruecksetzungen verursachen.

## Alpha.875-884: F3 attachments/movement and stale self root
F3 self-avatar attachment counts and original Firestorm
body attachments use only local IDs authenticated by the
current region generation, avoiding phantom own attachments
after reconnect. User-sent AgentUpdate remains speculative;
High9 readback only confirms WASD after a valid current
Self LocalID binding. No live E2E claimed.

## Alpha.885–894 – F3 WASD wartet auf tatsächliche Simulatorposition
Aktiver Circuit ohne gültigen aktuellen Selbst-Posebeweis zeigt
WASD: warte auf bestaetigte Simulatorposition statt einer
irreführenden Kein-Richtungsbefehl-Ausgabe. Das ist bewusst
kein simuliertes WASD-Ergebnis, sondern eine Diagnose, warum
MovementController und High4-Queue noch nicht senden.

## Alpha.895–904: F3 schützt gegen erfundenen Avatar-Drehwinkel
Bestehende F3 WASD-wartet-Simulatorposition-Anzeige bleibt aktiv, wenn
AgentMovementComplete Koordinaten liefert, aber keine nutzbare horizontale
LookAt-Blickrichtung. Kein künstlich aus Default-Quaternion abgeleiteter
serverbestätigter Avatar-Heading oder Bewegungserfolg.
