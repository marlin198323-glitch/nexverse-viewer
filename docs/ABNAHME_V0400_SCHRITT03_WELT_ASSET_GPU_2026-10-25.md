# OGL Viewer v0.4.0.0 – geführte Abnahme Schritt 3 Welt/F3

**25.10.2026 – Teilabnahme mit Fehlern, kein kompletter Live-Endnachweis.**

## Beobachtung auf zwei Nutzerscreenshots 15:45–15:46

- Anmeldestatus Grid LIVE, Login CONNECTED, UDP LIVE; Terrain,
  Gebäudeteile und andere Weltobjekte werden dargestellt.
- Große weiße/graue Flächen, mehrere unvollständige Meshobjekte.
- Eigene Avatar-Skin und vollständige Outfit-Darstellung nicht bestätigt.
- Einige linke Navigationspunkte bleiben unvollständig.

## Laufzeit-Evidenz (Linux Mint, nur zusammengefasste Befunde)

Session 25.10.2026, 15:40:08–15:47:59, Viewer Log
`/.local/state/opengenesislink/viewer.log:

- 784 mal GetMesh GPU upload failed: gpu_failure=capacity, gl_error=0.
- 43 GetTexture failed (u.a. JPEG2000-Grenzprüfungen/HTTP fehlende Assets).
- 2 GetMesh failed: Mesh LOD contains no valid renderable triangles.
- Login erfolgreich, CAPS Seed 19 Endpunkte, UDP authenticated
  AgentMovementComplete, erste EventQueue und WorldModel Terrains/Objekte.
- Keine tatsächlich verifizierte 3/3 Head/Upper/Lower-Skin-Bake/COF-
  Appearance in der untersuchten Session.

## Ursache P0: GPU-Mesh-Slot-Kapazität

In `src/render/opengl_renderer.cpp` verwirft
`upload_mesh_asset()` neue Meshes, sobald
`mesh_assets_.size() >= 256U`. Daraus entstehen
`GpuUploadFailure::capacity`-Fehler ohne GL-Fehler.
Das ist eine **harte Renderer-Asset-Grenze**, kein Beweis,
dass der physische Grafikspeicher erschöpft ist.

P0 `OGL-WORLD-003-MESH-CAPACITY`: Sichtbarkeit/LOD/Priorisierung
und budgetierte GPU-Cache-Eviction mit sicheren Buffer-Freigaben
implementieren, gegen Region >256 Mesh-IDs und original
Firestorm vergleichen. Nicht einfach unendlich erhöhen.

P1 `OGL-WORLD-003-TEXTURE-DECODE`: JPEG2000-/HTTP-Asset-
Fehler differenziert prüfen und defekte Assets melden.

P1 `OGL-WORLD-003-MESH-INVALID`: zwei Mesh-LOD mit nicht
renderbaren Dreiecken untersuchen.

P0 `OGL-AVATAR-003-APPEARANCE`: echtes Skin-/COF-
Outfit-Ende-zu-Ende bleibt separat offen.

## Abnahmestatus und nächste Aktion

Schritt 3: Welt geladen, Rendering **fehlerhaft**.
Manueller Abnahme-Review mit Status FEHLER und identischer
Quell-Git-SHA unter build/acceptance/.
Offizielle Release-Evidenz und Roadmap-Abnahme weiterhin OFFEN.

Schritt 4: Avatarfenster mit X schließen, 3D-Welt fokussieren,
W etwa 2 Sekunden drücken; Nutzer meldet tatsächliche
Bewegung, Entwickler prüft Server-Positions-Readback.
Kein main-Merge, Tag oder Produktivdeployment.
