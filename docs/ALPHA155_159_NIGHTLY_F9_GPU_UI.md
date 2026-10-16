# Alpha.155-159 – Firestorm Nightly, F9, GL Pixel, UI und Server

Datum: 16.10.2026. Primärer Entwicklungsrechner Linux Mint
jam-MS-7D28 über Desktop Commander als jam. Raspberry Pi 5 nur
Self-hosted GitHub ARM64-CI, kein interaktiver Entwickler.
Repository marlin198323-glitch/nexverse-viewer, develop-current.
Basis d49a50f552c4534b7290e0e86bed0c407feae463 (Alpha.154).
Kein produktiver Server-Deploy und keine echten Account-Mutationen.

## Alpha.155 – Original Firestorm Nightly-Assets geprüft
Firestorm 7.2.5 Nightly aus lokalem 191-MB-Archiv, NUR die 96
character-Dateien in eigenes ~/.cache/nexverse-viewer/
firestorm-nightly-7-2-5-81754/character entpackt. Kein Asset
im GitHub-Repository abgelegt. SHA256 für alle 96 Dateien:
Nightly und entpackte Firestorm Release 7.2.4 zu 96/96 identisch,
einschließlich original Head/Upper/Lower LLM und Skelett/LAD XML.
Der Viewer sucht versionierte Nightly-Cache-Ordner vor entpackten
Desktop-/Schreibtisch-Releases, vollständige Datei-Prüfung;
NEXVERSE_AVATAR_ASSETS hat Vorrang. Neuer C++ Locator-Test
prüft Cache, Desktop Nightly, Release und expliziten Override
sowie unvollständigen Nightly-Fallback.

## Alpha.156 – Direkte read-only F9-GPU-Bake-Vorschau
Der geklickte Server-Bakes-Tab braucht kein zusätzliches
kontinuierliches Appearance-Diagnoseflag mehr. Er zeigt den
tatsächlich vorhandenen GPU-Head/Upper/Lower-Status 0/3 bis 3/3.
Authentifizierte Self-ID, aktive Region, Epoch und vorhandene
GPU-Textur-UUIDs bleiben erforderlich. Keine erfundenen Bilder,
keine neue Netzwerkabfrage oder Wear-/COF-/Upload-Operation.

## Alpha.157 – GL3.3 Pixel-Readback und GPU-Lifetime
Renderer Region-Transition-Ctest auf NVIDIA OpenGL liest mit
glGetTexImage den tatsächlichen RGBA8-Mip0-Texturinhalt und
vergleicht die Pixel mit dem hochgeladenen 2x2 Testasset.
GL ActiveTexture/Binding wird danach wiederhergestellt.
Zusätzlich werden Head/Upper/Lower Body-Draw-Provenienz,
glIsTexture und GL-Preview-Handle-Invalidierung nach Eviction
geprüft. Reale Test-Texture != authentifizierter Grid-Bake.

## Alpha.158 – Neuer Servercommit gesondert klassifiziert
OpenGenesisLINK Server main
ec760cacfff2fe505cf51991985553ed434eb121, 16.10.2026.
Änderung J2KDecoderModule.cs: bei CSJ2K-Ausnahme werden
die Layer-Grenzen serverseitig mit OpenJPEG erneut versucht.
Im Viewer-Kompatibilitätsprüfer eigene Kategorie
SERVER_J2K_LAYER_BOUNDARY_FALLBACK_REVIEW, mit Test.
Viewer JPEG2000 J2C/JP2-Pixelparität bereits zuvor verifiziert.
Kein neuer Viewer-CAPS-/LLUDP-Nachrichtentyp im einzelnen Commit.
Ältere 22 Servercommits NICHT komplett manuell beurteilt;
docs/OPENGENESISLINK_SERVER_BASELINE.json reviewed_sha
4e99976630f1008db9014ea2b86c3723b09e9fe6 unverändert;
Strict-Gate weiterhin gesperrt.

## Alpha.159 – Sichtbarer Avatar-Buildstatus und Regressionen
Beim Login zeigt die GUI nun die tatsächlich erfasste
Startbedingung: Firestorm 3/3 GPU-bereit oder provisorische
Geometrie statt eine erfolgreiche Avatar-Anmeldung zu behaupten.
Version 0.2.0-alpha.159. Kompletter x86_64 Debug Ninja-
Build einschließlich ausführbarem Viewer PASS, CTest 113/113 PASS
inklusive RTX 4070 SUPER Nvidia-GL-Tests, Python 34/34 PASS.
Isolierter, zeitbegrenzter Xvfb-Start 1600x900 mit Mesa llvmpipe
initialisiert Skelett 133 Joints, 55 Attachments, Originalkörper
7186 Mesh-Dreiecke GPU-resident, UI und Welcome-Hintergrund.
Der künstliche Timeout ist erwartetes Testende, kein Crash.

## Noch nicht getestet/abgeschlossen
Echt eingeloggter eigener Avatar im NexVerse-Grid, authentifizierte
simulatorseitige Texturpixel und F9-COF-Readbacks; Wear/Unwear
mit serverbestätigtem Rollback und tatsächliche visuelle
Weltdarstellung/Avatarbewegung sind weiter offen.
Ältere Testfixture-Compilerwarnungen sind bekannt. Keine
erfundenen Inworld-Nachweise.

## Nächste fünf Prioritäten
1. Autorisierte authentifizierte Test-Grid-Login-Sitzung: CAPS,
   RegionEventQueue, WorldModel, Objekt-/Avatar-Renderdiagnostik.
2. Echte Server-Head/Upper/Lower Bake-UV- und Alpha-Pixeltests.
3. Wear/Unwear mit explizitem Consent, serverseitigem COF-Readback,
   Idempotenz und möglichem Rollback.
4. Firestorm GUI-/Menü-/Navigation-Parität parallel zur Funktion.
5. Verbleibende Upstream-Server-Commits manuell prüfen, bevor
   der Server-Kompatibilitäts-SHA geändert wird.
