# Alpha.210–214 – Verbindliche fünf Blöcke, echte GL-Avatar-Fragmente und Avatar-Fokus

Stand: 16.10.2026. OGL 0.2.0-alpha.214.
Mint Desktop Commander: jam-MS-7D28, User jam;
Repo marlin198323-glitch/nexverse-viewer,
Branch develop-current, Basis d15258ccde7b21084b872507e44647ed393a2d40.
Pi5 wird ausschließlich als ARM64 GitHub Actions Runner verwendet.
GitHub Upstream OpenGenesisLINK main ec760cacfff2fe505cf51991985553ed434eb121,
geprüft über GitHub und unverändert gegenüber alpha.209.
Pinnte reviewed_sha 4e99976630f1008db9014ea2b86c3723b09e9fe6
bleibt ungeändert, solange 22 ältere Server-Commits nicht komplett
patchweise reviewt sind.

## Alpha.210 – Verbindliche Fünferregel, Hand-off und Baseline
Der Nutzer verlangt bei jedem WEITER immer fünf echte Arbeitsblöcke.
Als dauerhaft geltende verbindliche Policy am ANFANG von projectplan.md
eingetragen, inklusive Definition der Block-Fertigstellung, paralleler
API-/Renderer-/sichtbarer UI-Arbeit, Tests und GitHub-/Mint-/Pi-CI.
Die gleiche Pflicht wird in AGENTS.md, info.md und CHATUEBERGABE.md
auch für neue Chat-Kontexte fortgeschrieben.
Blockiert wird offen dokumentiert, nie zu fertig erklärt.

## Alpha.211 – GPU hat den Avatar wirklich in den Tiefentest gebracht?
Der seit alpha.209 vorhandene Self-Avatar-Proof zählte nur GL-Draws
beziehungsweise geometrisches Kamerablickfeld. Neu:
GL_ANY_SAMPLES_PASSED-Abfrage umfasst die tatsächlichen 3
originalen Firestorm .llm Head-/Upper-/Lower-GL-Draws für den
eigenen Avatar, keinesfalls Platzhalter-Proxies.
Zwei OpenGL Query-Objekte werden nur einmal beim Rendererstart
angelegt, asynchron über GL_QUERY_RESULT_AVAILABLE
gepollt und beim Shutdown freigegeben. Der normale Renderloop
ruft weder glFinish noch einen blockierenden Query-Readback auf.
Ein negatives Ergebnis kann daher nun zwischen
vorhandenen Draws und 0 überlebenden Tiefentest-Fragmenten
unterscheiden. Wenn die GPU noch nicht geantwortet hat,
wird explizit UNKNOWN gemeldet statt fälschlich 0.
GL-Query-Ergebnis betrifft einen früheren Rendererframe,
nicht den endgültig zusammengesetzten UI-Framebuffer.

## Alpha.212 – Simulator-Epoch und HUD-Status korrekt
Beim Wechsel der FirestormRegionLease Epoch wird die
Avatar-Fragment-Probe durch reset_self_avatar_visibility
sofort invalidiert. Noch ausstehende OpenGL Queries werden
nicht abrupt wiederverwendet, ihre Resultate aus der alten
Region aber verworfen, sobald die GPU sie abliefert.
AvatarPipelineProof erhält getrennt von Apparance/Textur
die Query-Verfügbarkeit und das Fragments-Signal.
Im tatsächlichen Avatar-HUD und optional im F5-Weltfokus
erscheinen jetzt die Zustände:
- keine aktuelle Region/Agentposition
- kein Original-LLM-Draw
- GPU-Abfrage läuft
- mind. ein sichtbares Originalmesh-Depth-Fragment
- 0 Depth-Fragmente (verdecktes oder nicht sichtbares Mesh)
Diese Anzeige ist ehrlicher als reine DrawCalls und keine
Behauptung, dass originale Outfit-/Bake-Texturen fertig wären.
Keine zusätzlichen Netzwerkpakete, keine Account-Änderung.

## Alpha.213 – Echter interaktiver AVATAR-Fokus
Oben rechts direkt neben Zoom/Orbit/F5 im bestehenden
Welt-HUD (auch im F5-Weltfokus) neuer sichtbarer
AVATAR [F4] Button. Er setzt dieselbe lokale Renderer-/
Picking-/Streaming-Kamerapose auf einen stabilen
8-Meter-Abstand und frontal orbit 180 Grad zurück.
F4-Tastatur und gezeichneter Mausbutton routen in
die identische echte Aktion. Authentifizierte
simulatorbereite WorldScene ist erforderlich,
keine Aktion bei offenen F9/F10/Experiences/Chatfenstern.
Kamera ändert nur lokale ThirdPerson-Pose,
nicht AvatarAgent/LLUDP, nicht OpenGenesisLINK-Server.
Die Button-Geometrie ist für 1100x700 bis
3440x1440 getestet und kollidiert nicht mit Orbitbuttons.

## Alpha.214 – GPU-Integration/Regression, CI, Doku
Native NVIDIA-OpenGL-Integration in
renderer_region_transition_tests.cpp beweist mit
GL_QUERY_RESULT_AVAILABLE für das echte Drei-Part-Mesh,
dass eine Query wirklich beantwortet wird und eine
hinter die Kamera versetzte Self-Avatar-Instanz
trotz GL-Draws 0 Fragmente meldet. Testfixture nutzt
glFinish ausschließlich für deterministisches GPU
Retirement; Produktionscode bleibt nonblocking.
Region-reset liefert UNKNOWN und wirft Alt-Region
nicht als neue Avatar-Visibility aus.
Neue C++23-Test camera_avatar_focus_tests prüft
Fokus-Pose/HitRects und keinen Overlaykonflikt;
Pi5 Headless Script führt ihn ebenfalls aus.
Python Contract schützt GLUT/GL-Query Lifecycle
und echtes GUI-Event-Routing.
Verbindlich vor Push voller Mint-x64 Debug Build,
native+NVIDIA-GL-Tests, Python suite, Pi5-compatible
headless + Xvfb actual full viewer/Taskbar icon;
anschließend GitHub commit/push und Pi5 Runner grün.

## Nach wie vor BLOCKIERT durch fehlenden autorisierten Live-Test
Der eigene Avatar und die NexVerse-Welt wurden nach
echtem eingeloggtem Grid-Login noch NICHT visuell nachgewiesen.
Die GL-Query kann tiefe Fragmente eines synthetischen
Testmeshes prüfen, aber keine vollständige Firestorm-
Bake-Komposition oder Echt-Server-Wear/Unwear beweisen.
COF, Appearance TextureEntry, Animationen, EventQueue
und JWT/Session-Grid-Lifecycle bleiben weiter im Projektplan.
Es wurden keine Grid-Passwörter benötigt, keine Viewer-
Testfenster auf dem echten Linux-Desktop geöffnet.
