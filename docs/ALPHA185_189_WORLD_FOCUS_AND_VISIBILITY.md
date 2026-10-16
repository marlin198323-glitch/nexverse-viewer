# Alpha.185–189 – Freie 3D-Weltansicht/F5 und kameragültige Scene-Draws

Datum: 16.10.2026. OpenGenesisLINK Viewer 0.2.0-alpha.189.
Primärhost Linux Mint jam-MS-7D28 über Desktop Commander, Nutzer jam,
Repository marlin198323-glitch/nexverse-viewer Branch develop-current,
Basis 30c2842c94f9e541a9cd652e1e7590c112544702.
Pi5 nur ARM64 Self-hosted GitHub CI. Keine unaufgeforderten
Viewer-Fenster auf DISPLAY=:0; ausschließlich isolierter Xvfb-Smoke.
Keine echten Grid-/Wear-/Unwear-/Login-Schreibaktionen.

## Alpha.185 – Kamerabezogene Objekt-Sichtbarkeit statt Ghost-Draws
Bisher prüfte OpenGLRenderer::scene_object_visible nur sphärische
Entfernung bis far_clip und Objektmaß. Weltobjekte, die vollständig
HINTER der Kamera lagen, wurden trotzdem als Draw/visible_objects
gezählt, obwohl die GPU sie aus dem Bild schnitt. Eine neue
konservative, header-only C++23-Hilfe
render/scene_forward_visibility.hpp berücksichtigt außerdem den
normalisierten Kamerablickvektor und die Objekt-Bounding-Sphere.
Objekte vollständig hinter der Blickebene werden vor Draw
ausgeschlossen; nahe/übergroße Objekte am Kamerastandort bleiben
sichtbar, außerhalb far_clip werden verworfen. Ungültige
Nicht-Endlich-Werte werden abgewiesen. Kein vollständiges
6-Ebenen-Frustum, das wird nicht behauptet. Das Objektzentrum
kommt weiterhin aus der bestehenden Modellmatrix und Skala.

## Alpha.186 – Nutzbarer HUD-Fokus über F5
Ein neuer read-only Bedienmodus ermöglicht den 3D-Weltblick ohne
große Seitenleisten. F5 schaltet die sichtbare Navigations-
und Diagnoseoberfläche ein/aus, aber nicht Renderer,
Simulatorverbindung oder vorhandene Firestorm-Floater.
Bei ausgeblendetem Dashboard bleibt ein kleines sichtbar
anklickbares "HUD EIN [F5]" rechts oben und ein dezenter
Hinweis "3D-Weltansicht | F5 fuer Navigation" links oben.
Bei eingeblendetem Dashboard erscheint am selben Ort
"HUD AUS [F5]". Kein nicht-funktionierender Platzhalter-
Button. Standardmäßig ist das HUD weiterhin sichtbar.

## Alpha.187 – Mausinteraktion und Widescreen
Neue reine Geometrie-/Inputfunktion
ui/world_hud_toggle.hpp mit stabiler Hit-Region rechts oben
unterhalb der bestehenden Topstatuschips. Bei 16:9,
21:9/32:9 und kompakten Linux-Fenstern bleibt der Knopf
benutzbar. Der GLFW-Mauscallback trifft dieselbe
Geometrie, die gezeichnet wird. Wenn das HUD ausgeblendet
ist, dürfen die unsichtbaren Navigations- und Menühit-Zonen
keine Aktionen auslösen; Klicks gelangen stattdessen
in die bestehende 3D-Welt-Objekt-Auswahl.
Bereits sichtbare Floater und Menüs behalten ihre Eingaben,
und F9/Inventar/Experiences laufen weiter unabhängig.
F5 nutzt die authentifizierte Simulationsbereitschaft als Gate.

## Alpha.188 – Beweisbare Tests (C++23 und GPU)
Neue C++23-Regressionsziele:
nexverse-world-hud-toggle-tests (Widescreen/kompakt/
ein-/ausgeblendete Dashboard-Hit-Zonen, Modal-Fenster)
und nexverse-scene-forward-visibility-tests
(Hinterkamera, far_clip, übergroße Objekte, NaN).
Linux-x64 CMake und Pi5 Headless-CI integriert.
Bestehender versteckter NVIDIA OpenGL Framebuffer-CTest
prüft nun zusätzlich eine Szene hinter dem Kamerablick:
ein echt gerendertes Mesh muss mit rückwärts blickender
Kamera 0 Welt-Draws und 0 sichtbare Objekte liefern,
sowie dieselben Hintergrundpixel wie ohne Mesh.
Die synthetische Testfixture nutzt bewusst ein eigenes
Modellzentrum hinter der Kamera. GPU- und Quelltests
belegen nicht, dass Login/Avatar bereits weltweit
visuell Ende-zu-Ende funktioniert.

## Alpha.189 – Vollbuild, isolierte GUI, Dokumentation und GitHub
CMake/Ninja Linux-x64 Vollbuild, gesamte lokale Native-
/NVIDIA-OpenGL-CTest-Suite und Python/Headless-Regressionen
sind verbindliche Tests vor GitHub-Push. Vollständiger
Viewer-Start ausschließlich im isolierten 1600x900 Xvfb,
kein sichtbarer Start auf dem Nutzerdesktop. Alle
Projectplan/Handoff/AGENTS/Firestorm-/Server-Parity-
Dokumente werden mit diesem Block synchronisiert;
danach Push direkt vom Mint-PC und separater Pi5-CI.
Das letzte bekannte OpenGenesisLINK-Server-main ist
ec760cacfff2fe505cf51991985553ed434eb121,
seit Alpha.184 unverändert. Die reviewed_sha in
docs/OPENGENESISLINK_SERVER_BASELINE.json bleibt
4e99976630f1008db9014ea2b86c3723b09e9fe6,
weil die früheren 22 Upstream-Commits noch nicht
vollständig manuell qualifiziert wurden.

## Offen
Die neue benutzbare Weltansicht und korrektere GL-Zählung
sind kein bestätigter echter NexVerse-Avatar-/Weltframe.
Ein autorisierter Test-Grid-Login, echte CAPS-
EventQueue-/AvatarAppearance-Texturdaten und
Framebuffer-Sichtprüfung sind weiterhin offen.
Firestorm TexLayer11-Bake-Pixelparität, Wear/Unwear
mit COF-Readback/Idempotenz/Rollback sowie weitere
native Windows/macOS-Viewer-Builds bleiben geplant.

## Nächste fünf Blöcke
1. Authentifizierten Testlogin und GL-Weltstatus/Avatar
   mit Zustimmung messen, ohne Zugangsdaten zu loggen.
2. RenderMesh Bounds/Frustum konservativ vervollständigen
   und weltweite Kamera-, Near-/Far-Clip-Prüfung ausbauen.
3. Firestorm eigene AvatarHead/Upper/Lower Pixelprobe
   mit echten Simulator-Asset-UUIDs abgleichen.
4. Outfit/F9 sichtbare Controls weiter mit sicheren
   serververifizierten Wear/Unwear-Transaktionen anbinden.
5. OpenGenesisLINK Server ältere Commits patchweise
   reviewen und vollständig in Integration Matrix führen.
