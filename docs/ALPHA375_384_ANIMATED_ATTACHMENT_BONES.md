# Alpha.375–384 – echte Firestorm Bone-Attachment-Animation (18.10.2026)

Branch develop-current | Version 0.2.0-alpha.384 | Gesamtfortschritt **39 % (38,65 % gewichtet)**. Live NexVerse Self-AvatarAppearance/Bake/Outfit und bestätigte WASD-Bewegung bleiben **OFFEN**.

| Block | Ergebnis / Abnahmekriterium |
|---|---|
| 375 | GitHub HEAD, Übergabe, Projektfortschritt und Upstream OpenGenesisLINK main 293d1d6 geprüft; kein neuer Viewer-Pflichtvertrag. |
| 376 | Fehler: Kleidungs- und Attachment-Prims nutzten nur starre Bind-Position statt der tatsächlich gerenderten Avatar-Joints. |
| 377 | Firestorm avatar_lad.xml behält die lokale Gelenkposition und validierten Joint-Index (Index+1, 0 für HUD/Root) zusätzlich zum vorhandenen Bind-Offset. |
| 378 | Original-Joint-Mapping durch Firestorm-Rig -> Application -> RenderAvatarBindRig übernommen; Fallback-Bind-Offsets bleiben. |
| 379 | Pro gerendertem Frame werden per Avatar höchstens 256 Joint-Matrizen einmal berechnet, maximal 64 Avatar-Proxys; gleiche Bewegungs-/LLKeyframe-Quelle wie GPU-Körperskinning. |
| 380 | Scene-Pose der realen Welt-Linksets hängt an der passenden Joint-Matrix plus lokalen Attachment-Offset; fehlende/virtuelle Joints bleiben am originalen statischen Anker. |
| 381 | F3 Avatarstatus um aktuelle Bone-Pose-Anzahl erweitert, ohne behauptete Server-Animation. |
| 382 | Privater Avatarbericht enthält nur numerischen Bone-Pose-Zähler, keine UUIDs, Namen, Logins oder Assetinhalte. |
| 383 | Originales Firestorm-Skelett und 21-Level-Linksets regressionsgeprüft; echte OpenGL-Framebuffer-Änderung nach bewegtem Gelenk nachgewiesen; ohne Bone 0 Delta und HUD 0 Pixel; Python-Contracts. |
| 384 | Vollbuild Mint x64, CTest/NVIDIA-GL, Python, ARM64-kompatibler Headless-Test, isolierter Xvfb-Viewer/Systemicon, GitHub-Push und Pi5 CI für denselben SHA (CI nur nach tatsächlicher Bestätigung als erfolgreich markieren). |

## GPU-Beleg, keine erfundene Grid-Parität

Der isolierte NVIDIA-GL-Fixture-Test mit einem simulierten Avatar-Anker beweist **1.108 geänderte RGB-Objektpixel** zwischen Laufzeit 0.00 und 0.22 Sekunden. Dabei wird für jeden Frame die Hintergrund- und Avatar-Baseline entfernt; ein ungültiger Gelenkindex ergibt **0** unerwartete Änderung, und HUD-Prims ergeben **0** Weltpixel. Dieses lokale Testresultat ist **kein** authentifizierter NexVerse-Server-Avatar und kein kompletter Firestorm-Outfit-/Skelett-/Cloth-Nachweis.

Die Bone-Animation benutzt bereits gerenderte lokale oder tatsächlich geladene Firestorm-Keyframe-Motions. Fehlt ein echter Server-Keyframe, bleibt der vorhandene visuelle Procedural-Fallback. Keine zusätzlichen Serverpakete, keine generierten Bake-UUIDs, keine Aktivierung experimenteller Plugins und kein Produktionsdeploy.

**Nächste 10 Blöcke Alpha.385–394:** echte AvatarAppearance-Live-Fehlerphase anhand neuer Sitzungslogs bestimmen, SSA-/Skin-/Bake-Readback und bestätigte WASD-Position mit Live-Evidenz priorisieren, gleichzeitig vollständige sichtbare Viewer-GUI/Firestorm-Funktionsabdeckung ausbauen.
