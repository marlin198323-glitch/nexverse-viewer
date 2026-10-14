# Firestorm Integration – OGL Viewer beschleunigt

Firestorm ist verbindlicher Funktions-/UX-/Protokollstandard.
OpenGenesisLINK Server ist die OpenSim-Gegenstellenreferenz.
OGL behält das bisherige NexVerse-Welcome-/Widescreen-Design.

## Vorgehensweise fuer "weiter"
Mehrere zusammenhaengende Funktionsmodule (Richtwert 8–12)
mit EINEM getesteten Releaseblock statt isolierter
Miniversionen bearbeiten. Eigenentwicklungen, die bei
Firestorm vorhanden sind, zuerst auf direkte
Wiederverwendung von Format/Ressource/konkretem Modul
pruefen, dann Tests/GitHub/Plan und Uebergabe aktualisieren.
Nicht blind komplette Firestorm-/Linden-Quelltexte
kopieren: LLAppearance-Referenzcode unterliegt teils
LGPL-2.1. OGL hat MPL-2.0 und benoetigt fuer
Quellcodeuebernahmen eine konkrete Abhaengigkeits-
und Lizenzpruefung pro Modul.

## Erreichte Original-Firestorm-Integration in Alpha.94
- Linden Binary Mesh 1.0: Head, Upper, Lower ORIGINAL
- 4320 echte Vertices, 7186 Dreiecke
- echte Normals, UV, JointNames, SkinWeights erhalten
- OpenGL-Body mit drei Original-GPU-Meshes
- 133 Firestorm-Skeleton-Joints und 55 Attachment-Slots
- Avatar-Parent-/Attachment-UUID-/LocalID-Verankerung
- HUD Attachments nicht in 3D rendern
- fehlende Quellen auf anderen OS: 7-Teile-Fallback
- belastbare Linux-Unit/GPU-Tests und GUI-Smoke

## Weiter offen
VertexWeight-Deformation, AvatarAnimation, korrektes
joint rotation mesh skinning, MorphTargets, Outfits/COF,
echter Avatar-Wear-Workflow, HUD-UI, gepackte Assets
fuer Windows/Mac, GPU/Simulator Live-Sichtabnahme,
Mesh-404 und Teleport. Ohne Login nie live_verified.

## Referenz
FirestormViewer/phoenix-firestorm Firestorm_7.2.4
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
Original Character XML/LLM Assets aus installierter
lokaler Firestorm-Laufzeit.

## Alpha.95 – echte GPU-Vertexweight-Deformation

Die aus Firestorm Original-Dateien extrahierten 1-basierten
MeshWeight-Werte werden auf die mit BASE Ancestors
erweiterte Firestorm-LLJointRenderData-Palette abgebildet.
Der GLSL3.3 Shader erhaelt zusaetzliche SkinVBOs und
pro Avatar berechnete Skeleton-Matrizen. Prozedurale
Walk-Poses fuer eigene und serverseitig bewegt
beobachtete fremde Avatare sind bereits in der
GPU-Pipeline testbar. Live-Git-Server-Animationen,
Appearance Morphs und COF bleiben Folgearbeit.

## Alpha.96 – server AvatarAnimation into original-weight GPU

A single integrated block adds a bounded High20 wire
decoder, world animation snapshots/early-remote buffering,
known Firestorm animation UUID matching and several
procedural GPU poses while preserving original LLM
meshes, GLSL weights and baked UVs. Actual LLKeyframeMotion
assets, loop/priority/blend and custom animations remain
the next critical milestone. Source references and wire
tests: docs/ALPHA96_SERVER_ANIMATION.md.

## Alpha.97 – parallele optische Viewer-Entwicklung verpflichtend

Ab jetzt zu jedem technischen Paket denselben visuellen
Funktionsumfang implementieren: Firestorm-konformer
Menüeintrag, NexVerse-Style-Floater, klickbare Buttons,
funktionierender Eingabe-/Backendhandler, ehrlicher Status,
GUI-/GPU-Regressionsprüfung und Widescreen.
Keine alleinige Backend-Arbeit bis die GUI Wochen später
nachgereicht wird. Details docs/VIEWER_UI_PARITY_PLAN.md.

## Alpha.98 – Animation-Asset-Pfad plus GUI gleichzeitig

Ein echter LLKeyframeMotion-Binärdecoder, F32/U16-Curves,
Quaternion-SLERP/Loop, original Firestorm GPU JointMatrices
bei bereitgestelltem Clip sowie Avatar>Animationen-Menue,
anklickbare Live-Animationsliste und GPU-Tests
wurden gemeinsam integriert. Fehlender Server-Asset-
Download und Priority-/Blend-Vollwiedergabe bleiben
naechster Block zusammen mit UI-Status/Fehleraktionen.

## Alpha.99 – parallele GitHub Server-Audits

Neuer verpflichtender Entwicklungszyklus:
OGL-GitHub-SHA prüfen, Server-GitHub-main live
mit --strict gegen reviewed SHA vergleichen,
neue Commits klassifizieren, Core/Modul/
Server-only bewerten, im selben Block C++,
sichtbare GUI/Handler, Tests, Projektplan und
GitHub-Commit. Checker erkennt neue Commits,
aber ändert Reviewed-SHA niemals ungeprüft.
Server-Chat-Modi und InventoryThumbnail-Cache
als VIEWER-CORE, LSL Neighborzustellung
SERVER-ONLY. OpenGenesisLINK-Native LSL-
Funktionen erst bei integriertem Scripteditor
als Editorfunktionen ergänzen.

## Alpha.100 – UI und echte AnimationAsset Transfers gleichzeitig

Original Firestorm AssetTransfer Low153/154 High17 im
LLUDP Core, AssetType Animation20 und validierte Bytes
zum LLKeyframeMotion-Decoder und Original-LLM GPU.
Im gleichen Release GUI-Transfer-/Fehler-/Retry-Funktion
und Test auf echte Klickwege. Prioritäten/Sit/Layer-
Mixing nach wie vor schrittweise zu portieren.

## Alpha.101 – Firestorm Grab/DeGrab plus sichtbar Bedienelement

Backend fuer LLUDP ObjectGrab und ObjectDeGrab
mit echtem Maustasten-Halten/-Loslassen und
naher Live-Root-Prim-Liste im Welt-Menue
im selben Entwicklungsblock programmiert.
Server hat neues stationaeres Touch-Hold,
daher kein OGL Touch-Timer. Firestorm
Skript-Kanalchat /7 umgesetzt.
OpenGL/Maus/PacketGolden Tests gemeinsam;
echtes InWorld noch offen.

## Alpha.102 – Firestorm-ähnlicher 3D-Klick mit GUI

Die OGL-Kamera kann im sichtbaren Weltfenster
echte Root-Prims direkt wählen und damit
die vorhanden verifizierten ObjectGrab/
ObjectDeGrab-LLUDP-Funktionen auslösen.
NexVerse-Dock/Floater verknüpft die Auswahl
sichtbar. Geometry ist noch OBB, kein
dreiecksgenaues Firestorm Mesh-Picking.

## Alpha.103 – Mesh-Geometrie und GUI zeitgleich

Realer GetMesh Uploadpfad speist zugleich GPU
und bounded CPU-Ray-Geometrie. Worldklick
meldet exakte Mesh-Face und UV bei vorhandenen
Assets, sonst klar gekennzeichnete Näherung.
Parallel ergänzt echter F9-Outfit-Galerie-
Aktualisieren-Button einen bestehenden
authenticated InventoryDescendents2-Backend-
Pfad; keine Dummy-COF-Wear-Aktionen.

## Alpha.104

Alpha.104: Original Firestorm ObjectGrab/ObjectDeGrab SurfaceInfo komplett fuer echte Mesh-Hits; Weltfenster mit sichtbarem Sendestatus. Outfit F9 GUI parallel mit COF pruefen und echten Caps FetchInventory2 read-only, Wear-Transaktion noch offen.
