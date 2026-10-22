# Alpha.815–824 – Manuelle Outfit-BAKE-Pruefung pro authentifizierter Quelle

22.10.2026 | Linux Mint jam-MS-7D28 | develop-current
Version 0.2.0-alpha.824 | Projekt 39 % (gewichtet 38,65 %).

## Zehn Entwicklungsblöcke

815. Sauberen Github Viewer-Branch develop-current HEAD 60aa8d8,
CHATUEBERGABE.md und projectplan.md vor der Arbeit gelesen;
vorige Pi5 Alpha.814 Code-SHA f2b8094 Run 39833603079 SUCCESS.
816. Aktuellen OpenGenesisLINK Servermain 7396897d geprüft:
kein neuer COF/Low158/GetTexture/Low382-Vertrag.
817. ECHTEN F9-Fehler gefunden: AvatarBakeRecheck erlaubte
höchstens drei manuelle Texture-Replans PRO REGION und blieb
auch bei neuen, authentifizierten Outfits dauerhaft gesperrt.
818. Replan-Budget nun pro gültiger, nichtleerer Head/Upper/Lower
Server-Body-Bake-Quelle max. 3 Versuche; echte geänderte
Low158/ObjTE-Quelle darf in selber Region wieder manuell laden.
819. Zusätzliche Obergrenze 12 manuelle Replans pro Regions-Epoch;
Outfitwechsel setzt weder globalen Zähler noch 12s-Cooldown
zurück und kann Download-/Fehler-Backoff nicht umgehen.
820. Das echte F9-Hauptloop speist AvatarBakeRecheck nun mit
aktuellen authentifizierten Self-Bake-IDs und Session-/Region-
Lease. Leere, Firestorm-Placeholder, NULL und fremde Session
resetten das Budget nicht.
821. F9 zeigt klar Outfit-Limit vs. gesamtes Regions-Limit.
CAP/Stream/In-Flight/Retry/VRAM-Pacing und sämtliche
serverautoritativen Avatar-TE-Regeln bleiben unverändert.
822. Native C++-Tests decken ausgeschöpftes Outfit A, neues
Outfit B, ungültige/Placeholder/fremde Quellen, total 12,
beibehaltenen 12s-Delay, neue Region und Legacy-Backoff ab.
823. Python-Realsource-Vertrag sichert den echten F9-Bindepunkt,
beide Limits, Validitätskontrollen und Versionsfortsetzung;
vollständiger Mint-x64-Build + 176 native CTests + Python +
Xvfb GUI + echtes OGL-Systemicon als Release-Gate.
824. VERSION/CHATUEBERGABE.md/projectplan.md/AGENTS/info,
Fortschritt JSON, Server-/Firestorm-/UI-Paritätsmatrizen
aktualisieren; nach vollständigem grünen Gate Code nach
GitHub develop-current pushen und exakte CI-SHA vermerken.

## Weiter offen
Dies sind lokale Neuprüfungen echter Server-Bake-Assets, KEIN
authentifizierter NexVerse-Avatar, keine bestätigten Low158-
COF-Outfits/Skin-Pixel und kein serverbestätigter WASD-Livetest.
P0 Inworld-End-to-End weiter OFFEN. Fortschritt 39 %.
Nächster Block Alpha.825–834. Kein Produktionsdeploy.

## Release-Gate Linux Mint x64
scripts/mint-viewer-validation.sh EXIT 0:
vollständiger CMake/vcpkg-x64-Debug-Build,
176/176 native CTests (einschließlich
AvatarBakeRecheck-Quellenkontingent/Regionscap),
400/400 Python-Tests, isolierter Xvfb Viewer
0.2.0-alpha.824, 133 Original-Firestorm-Joints,
drei Original-Körpermeshes GPU-resident
(7.186 Dreiecke), korrektes WM_CLASS und
OGL-X11-Taskleisten-Icon 128x128 PASS.
Authentifizierter Grid-Avatar/Outfit/Skin/WASD
damit NICHT nachgewiesen. Kein Produktivdeploy.
