# Alpha.102 – echte WorldRay-Auswahl und sichtbarer Objekt-Floater

OGL Ausgangs-HEAD develop-current:
3592d2e9f08daa40cc8ed1ee0e155da9a4887adf.
OpenGenesisLINK Server main:
2cc59ceaada1c6791ac8320d3d5638cae941b13a.
Firestorm_7.2.4 message_template.msg ObjectGrab Low117
und ObjectDeGrab Low119 aus Alpha.101 bleiben verbindlich.

## Geometry
include/nexverse/render/world_object_picker.hpp
setzt Ray aus der gleichen OpenGL RenderCamera
(Forward, Up, 60 Grad vertikaler FOV, Bildschirm-
Aspect, Maus-TopLeft) zusammen. Relative Weltposition
und Richtung werden mit normiertem inversen
Quaternion in den lokalen Objekt-Raum transformiert.
Dort OBB-Slab-Intersection mit Scale/2 und
nächstem positiven Eintrittspunkt. Invalid FOV,
NaN/Infinity, Offscreen, Null/negative Size,
BehindCamera und zu ferne Objekte werden abgewiesen.
Shape-Werkzeug nur BoundingBox, keine exakten
Scults-/Mesh-Faces. Der Client erfindet keine
FaceID/UV/Normal oder serverseitige Ergebnisse.

## Viewer UI and input path
ViewerShellLayout.world_viewport_hit grenzt
tatsächliche 3D-Welt gegen Navigation, Topbar,
Dock, rechte HUD Karten und Modal ab. GLFW
legt Mauskoordinaten ab, liest keine WorldModel-
Daten im Callback. Hauptthread rekonstruiert
Kamera und bis 15m reichende RootPrim-Objekte
unter WorldMutex, führt Raytest aus, selektiert
getroffenes LocalID, aktiviert echtes
authentifiziertes LLUDP ObjectGrab und zeigt
Objekte-Floater. Das 9. Dock-Element Objekte
ist aktiv, ebenso Welt-Menü. Selektiertes
Prim bleibt in sichtbarer Auswahlliste.
Release, kurzer Click im selben Poll-Frame,
Alt-Tab und Regionwechsel sind berücksichtigt.

## Tests and constraints
tests/world_object_picker_tests.cpp:
Rayfront, Nearest, 90°-Quaternion, Invalid,
1280x720, 3440x1440 und 5120x1440.
viewer_shell_tests.cpp: Welt-/HUD Hitbox.
Plus original Wire-Goldens, GL Shell und volle
Projekt-CTest/Python-Testserie.
Noch kein 3D-Mesh-BVH, precise SurfaceInfo,
KindPrim ChildTouch, Live-NexVerse LSL Smoke
und Outfit COF-Wear. Kein vollständiger
Firestorm-Funktionsstand behauptet.

## Serverreview 14.10.2026
Server 0.9.3.12 Dev/kein RC oder Stable:
ES256 signierter OAR/IAR-Freigabeprozess
ist Robust-/NodeAdmin-Funktion, nicht
Viewer-User-CAPS. Keys niemals im UserViewer.
Feature-Entscheidung CORE für Objekt-Touch
und Renderer, SERVER ONLY für Archiv-/Konsole,
optionale Plugins nur Zusatzvisualisierung.


## Alpha.102 zweite Upstream-Nachpruefung: 0.9.3.12 Release-Label

Ein weiterer Server-Commit nach der anfaenglichen
Sechs-Commit-Pruefung: neuer main SHA
1841c4ee8ef580730be7d306023e594380f8b19f
(14.10.2026 16:41:53 UTC), aus 2cc59cea.
Die VersionInfo.cs ist nun auf Flavour.Release
gestellt; Dokumentation RELEASE_0.9.3.12.md
und 09312_RC1_STABLE_ACCEPTANCE.md betonen aber:
**KEIN** allein dadurch publizierter GitHub-Tag,
KEINE nachgewiesene produktive Installation und
KEINE vollstaendige 22-Punkte-Release-Evidence.
Die zuletzt unabhaengig verifizierte/tagged
Stable bleibt v0.9.3.10. Endnutzer-OGL darf
keine Server-Livefreigabe aus dem Code-Label
ableiten. Keine neue Client-LLUDP/CAPS-Funktion
aus diesen Release-Dokumenten abzuleiten.
Server-Baseline-SHA nach Review aktualisiert.
