# Alpha.103 – echte GetMesh-Dreiecke und Outfit UI

Quelle vor Entwicklungsbeginn:
OpenGenesisLINK Server main
1841c4ee8ef580730be7d306023e594380f8b19f
und OGL Viewer develop-current
2dc9cf7e7f9d72de292eaa89c251019812eda4e2.

## Rendering und 3D Picking
CPU-GetMesh-Pick-Cache wird nur nach
erfolgreichem GPU Upload befüllt, ist
auf 24MiB beschränkt und wird bei
GPU Eviction / Regionswechsel bereinigt.
Originales interleaved xyz/nxyz/uv,
indizierte Dreiecke und FaceRanges
werden in lokalen Objektstrahl
transformiert. Nearest positive
Möller-Trumbore Treffer berechnet
FaceID und baryzentrisches UV.
Resident Mesh und fehlendes Dreieck
ergibt keinen OBB-Falschhit;
nicht-residentes Mesh nutzt
markierte OBB-Näherung.
Noch kein vollständiger Firestorm
Mesh BVH für sehr große Assets.
Die LLUDP-ObjectGrab-SurfaceInfo bleibt
leer, solange auch World-Position,
Normal, Binormal und ST korrekt
abgenommen sind. UV-Anzeige in der GUI
ist keine serverseitige TouchFace-Abnahme.

## Outfit Galerie
F9/Outfits-Fenster zeigt den neuen
Button 'Aktualisieren'. Klick setzt
force-refresh für das vorhandene
Read-Only InventoryDescendents2
unter Avatar/RegionEpoch-/Timeoutcheck
und Minimum-Intervall >=2s.
Nur read-only Snapshots; COF Wear/Unwear
wird nicht ohne gültige vollständige
Mutationspipeline ausgeführt.

## Regression und Abnahme
Tests: Framecamera-OBB (Alpha.102),
Triangle-Golden Face7 UV=(0.5,0.5),
kein Treffer in Meshloch, korruptes
Indextriplett und nichtfinite Vertices.
C++/Python Gesamt-Tests und GPU
Linux Smoke vor Github Push.
Keine neue authentifizierte
NexVerse Inworld-Login- oder
echte Outfit-Wear-End-to-End-Abnahme.
