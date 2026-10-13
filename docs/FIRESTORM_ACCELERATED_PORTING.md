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
