# Alpha.875-884 - Eigene High9-Avatar-LocalID an Simulatorgeneration binden

23.10.2026 | Linux Mint Desktop Commander jam-MS-7D28
Branch develop-current | OGL 0.2.0-alpha.884 | Projekt 39 %

## Zehn Entwicklungsblöcke
875. GitHub develop-current/HEAD e4368a6 sauber mit origin verglichen; CHATUEBERGABE.md und projectplan.md vor Änderungen gelesen. Pi5 CI der vorigen Alpha.874 Code-SHA 6eabf2f Run 39979417843 COMPLETED SUCCESS.
876. OpenGenesisLINK Servermain 08f95e7 geprüft, keine neuen AvatarAppearance/High9/WASD-Wireanforderungen. Firestorm- und Original-OpenSim High9 bleibt UUID-freies ImprovedTerseObjectUpdate.
877. REALEN Identitätsfehler in src/app/application.cpp festgestellt: self_avatar_local_id war nur an RegionHandle gebunden. Ein Simulatorwechsel mit demselben RegionHandle invalidiert seit Alpha.874 zwar das WorldModel, aber der alte numerische Self-LocalID blieb im Hauptloop und den Low9-Objektupdates zunächst bestehen. Ein recyceltes High9-LocalID des neuen Simulators konnte so als alter eigener Avatar gelten.
878. Reines MPL-2.0-Header-Modul include/nexverse/world/self_avatar_localid_lease.hpp ergänzt: numerische eigene LocalID nur gültig, wenn RegionHandle UND WorldModel-Generation mit dem authentifizierten Full/Compressed-ObjectUpdate-Bind übereinstimmen. Generation 0 beim initialen Login ist ausdrücklich erlaubt.
879. Die zwei echten Full-/Compressed-ObjectUpdate-Self-Pfade binden local ID, RegionHandle und Generation zusammen. Auch wenn ID und Handle im neuen Simulator gleich bleiben, wird nach Generationwechsel der alte Bewegungs-/Drehbeweis zurückgesetzt.
880. High9 ImprovedTerseObjectUpdate benötigt jetzt die komplette Generation-gültige Selbstbindung statt nur LocalID und Handle. Fremde/alte numerische IDs liefern keinen Fake-Self-Pose/WASD-/Yaw-Erfolg.
881. KillObject darf den eigenen Avatar nur dann löschen, wenn das LocalID-Lease zur aktuellen Regionsgeneration gehört. Login-Reset, expliziter Circuit-Handoff und eigener KillObject setzen auch das neue Generation-Feld zurück.
882. Eigenes Firestorm-Body-Renderproxy sowie drei reale Attachment-/F3-/Avatar-Diagnose-Reconciliation-Pfade filtern self_avatar_local_id auf die aktuelle Generation. Veraltete eigene LocalID kann nicht länger Remote-Anhänge fälschlich als Selbstattachments einordnen.
883. Hauptschleife invalidiert stale Selbstbindung beim Wechsel des Firestorm-Regionepochs, bewahrt aber eine echte schon zuvor vom UDP-Thread unter der NEUEN Generation gebundene LocalID. Native C++-Regression in self_avatar_pose_authority_tests und early_avatar_readback_tests prueft echte gleichbleibende numerische ID bei neuem Simulatorport, Generation 0 Erstlogin, stale High9 und neue Full-Bind. Python-Quellvertrag prüft den echten Application-/Renderer-/Kill-Pfad.
884. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS, info, Fortschritt-JSON sowie Server-/Firestorm-/UI-Matrizen aktualisieren. Vollständiger Mint-x64-Viewer-Build, native CTests, Python-Suite, isoliertes Xvfb-GUI und OGL-Taskleistenicon vor Commit/Push. CI pro Feature-SHA und local/hosted getrennt. Kein Deploy.

## Live-Abnahme noch offen
Diese Änderung verhindert die falsche Selbstidentifikation alter High9-LocalIDs nach Reconnect. Sie beweist keinen echten eingeloggten NexVerse-Avatar mit COF, Skin und Kleidungs-Bakes sowie simulatorbestätigtem WASD/Yaw-Live-E2E. Gesamtfortschritt 39 %, nächste zehn Blöcke Alpha.885-894.

## Vollständig grüner lokaler Mint-x64-Release-Gate
scripts/mint-viewer-validation.sh EXIT 0:
CMake/vcpkg Debug Linux-x64 Viewer 0.2.0-alpha.884,
178/178 native CTests einschliesslich echtem WorldModel
same-simulator-handle/new-port und High9 LocalID-Generationstest,
424/424 Python-Contracts, isolierter Xvfb-GUI-Start,
Original Firestorm 133 Skeleton Joints/drei GPU-LLM-Bodymeshes
7186 Dreiecke und X11 WM_CLASS/128px OGL Taskbar-Icon PASS.
Kein Live-NexVerse-Login, kein Deploy.

## Exakte GitHub-Code-SHA und getrennte CI-Ergebnisse
Code SHA 7e43a2aca847525fa0a4a9cdaaec49db6e0283d7.
Pi5 Source Check Run 39982171338 IN_PROGRESS beim Abruf.
Hosted Linux-x64 Run 39982171352 COMPLETED/FAILURE,
Job 120511984008, runner_id 0, null ausgeführte Schritte:
kein gestarteter Compiler. Letzter Pi5-Erfolg Alpha.874
SHA 6eabf2f Run 39979417843. Separat lokaler Mint-x64-Gate
Exit 0: 178 native CTests, 424 Python, Xvfb GUI und OGL Icon.
Live-Avatar E2E weiterhin offen.
