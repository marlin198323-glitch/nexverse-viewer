# Alpha.365–374 – echte GL-Positionen tiefer Avatar-Linksets

Alpha.374 CI VERIFIZIERT: Featurecommit dc7a7e7114154adaa444ded5ade9e5dfb97f1dce; GitHub Pi5-ARM64-Run 39131738405 SUCCESS exakt für denselben SHA. Mint Debug x64 Vollbuild 358/358, 151/151 native NVIDIA/OpenGL CTests, 140/140 Python, ARM64-kompatibler Headless, isolierter Xvfb Viewerstart und X11 Systemicon/WM_CLASS PASS. Echte 21-Level Avatar-Linkset-Framebuffer-Pixel 4729, 21 HUD-Prim-Ausschlüsse, keine verbliebenen HUD-Pixel; F3 zeigt reale Linktiefe. Echter NexVerse AvatarAppearance/Outfit-/WASD-End-to-End weiterhin OFFEN. Projektstatus 39 % (38,65 % gewichtet); nächster Lauf Alpha.375–384.


18.10.2026 | develop-current | Version 0.2.0-alpha.374 | **39 % gewichtet 38,65 %**.

## Zehn echte Entwicklungsblöcke

| Block | Geprüfte Umsetzung |
|---|---|
| 365 | Neueste GitHub-Übergabe und Server-main `293d1d6` geprüft; keine neue nachgewiesene verpflichtende LLUDP-Änderung. |
| 366 | Fehler im Renderpfad lokalisiert: HUD-Culling löste 128 Vorfahren auf, `scene_pose()` nur 16. Tiefere Linksets verloren den tatsächlichen Avatar-Anker und konnten versetzt erscheinen. |
| 367 | Neue schreibgeschützte `SceneParentChain` mit maximal 128 Prim-Indizes vom Blatt zur Wurzel: erkanntes lokales Avatar-ID-Paar vs. unbekannter Elternknoten getrennt. |
| 368 | Bestehende HUD-Klassifikation auf dieselbe Elternketten-Auflösung umgestellt; keine Änderung am serverseitigen Attachment-Slot-Vertrag. |
| 369 | Produktive OpenGL-`scene_pose()` jetzt iterativ (Root vor Kind) für volle 128 Ebenen; kein willkürlicher 16-Level-Abbruch mehr. |
| 370 | Zyklen, Selbst-Parent, verwaiste Verknüpfungen, unverifizierte Avataranker und Grenzwert 128/129 getestet; ohne unendliche Rekursion und ohne erfundene Serverdaten. |
| 371 | Neuer echter GL-Framebuffer-Pixeltest: synthetisches 21-Prim-Linkset an simuliertem Avatar-Anker ergibt **4.729 geänderte RGB-Pixel** bei maximaler sichtbarer Tiefe **20**. |
| 372 | Echter HUD-Pixeltest mit derselben Hierarchie: alle **21** HUD-Prims ausgeblendet, **0** verbliebene RGB-Pixel; neutrale unsichtbare Sentinel-Szene vermeidet Debug-Testwürfel. |
| 373 | Echte F3-Anzeige: maximale sichtbare Parent-Tiefe und Anzahl tief verschachtelter Objekte. Privater Avatarbericht ergänzt ausschließlich Whitelist-Zähler (keine UUIDs). Neue native CTest-/ARM64-Headless-/Python-Regressionen. |
| 374 | Vollständiger Mint-CMake-Build, native GL-/C++-Tests, Python, Headless, Xvfb-/Fenstericon-Test, GitHub-Push und ARM64-CI auf finalem SHA. |

## Grenzen der Evidenz

Die 4.729 Pixel sind **ein lokaler OpenGL-Fixture-Test mit simuliertem Server-Anker**, kein authentifizierter NexVerse-Live-Test und keine behauptete sichtbare Outfit-Parität. Die Korrektur gilt nur für im Viewer vorhandene und korrekt verknüpfte Prim-Linksets. Tatsächliche Firestorm-Skelettanimation für Attachment-Joints, die eigene Server-Bake/Appearance und bestätigte WASD-Bewegung bleiben offen.

Die bisherige OGL-Welt, Alpha.364-HUD-Filterung, F3-Controls, Icon und adaptive Mesh-Residency bleiben erhalten. Kein Backend-Deploy und keine experimentelle OGLAssetService-Aktivierung.

**Gesamtfortschritt 39 % (38,65 %), nächste zehn Blöcke Alpha.375–384.**
