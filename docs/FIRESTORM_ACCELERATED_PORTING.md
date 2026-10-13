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
