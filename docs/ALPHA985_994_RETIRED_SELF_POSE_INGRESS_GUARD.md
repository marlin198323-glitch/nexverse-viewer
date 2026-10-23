# Alpha.985–994 – alte Avatar-LocalID vor WASD-/Pose-Bindung abweisen

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | OGL 0.2.0-alpha.994

## Zehn echte Entwicklungsblöcke

985. GitHub develop-current HEAD e08732c, sauberer Linux-Mint-Checkout und Remote synchron. Zuerst CHATUEBERGABE.md, projectplan.md und vorige Alpha.975–984-Dokumentation gelesen.
986. OpenGenesisLINK-Servermain erneut exakt f56e354694a41adb2334884c21efc45db784cb92 geprüft. Änderung nur World API Admin-Machine-Key-Scopes; Firestorm/OpenSim Avatar-TE unverändert.
987. Fehler im Full ObjectUpdate-Empfang festgestellt: application.cpp bindet Self-LocalID und leert Movement-/Turn-Feedback bevor die im WorldModel implementierte alte COF-Body-TE-Sperre aufgerufen wird.
988. Dieselbe Reihenfolge auch in ObjectUpdateCompressed festgestellt und korrigiert; die Terse-Pipeline ist bereits an eine autorisierte aktuelle Self-LocalID gebunden und bleibt unverändert.
989. Neue authentifizierte WorldModel::reject_retired_self_object_pose-Vorprüfung für aktuelle AgentID/Session, RegionHandle und nichtleere LocalID; vor beiden Rebind-Pfaden, Self-Pose und WASD-/Yaw-Evidenz.
990. Exakte alte LocalID+Head/Upper/Lower-TE zuverlässig verworfen; alte LocalID mit abwesender, ungültiger oder lediglich leeren Bake-TE ist kein neuer Lease-Beweis. Tatsächlich andere gültige, nichtleere neue TE ist weiter erlaubt, einschließlich späterer echter Wiederverwendung einer LocalID.
991. Bereits neu verifizierter aktiver Self-LocalID-Kandidat darf unvollständige Folgemeldungen behalten; alte vollständige Outfit-TE kann ihn dennoch nicht verdrängen. Rejection-Zähler uint32 saturierend, Reset bei WorldModel reset, Regionswechsel, Login-/Sessionwechsel.
992. F3 Avatar-Bake-Status zeigt die Anzahl „Alte Avatar-Pose vor WASD verworfen“ neben altem Bake-Replay- und History-Zähler, ausschließlich als datensparsamer Zähler.
993. Echter nativer WorldModel-Test mit authentifiziertem COF-100/101, verspätetem Full-Body/fehlendem und malformed TE, frischem TE auf gleicher LocalID, neuer LocalID, erneutem Replay und Neuanmeldung. Python-Realpath-Contract sichert Reihenfolge in Full/Compressed-Handlern und F3-Anzeige.
994. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Fortschritts-JSON und Firestorm-/Server-/UI-Paritätsmatrizen aktualisiert. Vollständiges Mint-x64-CMake/vcpkg/CTests/Python/Xvfb-/Taskleistenicon-Release-Gate vor Code-Push; Pi5-ARM64-CI danach prüfen. Kein Produktivdeploy.

## Gültigkeitsgrenzen

Native C++-Tests und Xvfb sind keine authentifizierte NexVerse-Live-Avatar-/COF-/Outfit-/Skin-GPU-Pixel- und keine serverbestätigte WASD-/Yaw-End-to-End-Abnahme. Alle entsprechenden Live-E2E-Ziele bleiben still open. Gesamtfortschritt 38,65 % gewichtet / 39 % gerundet; nächste zehn Blöcke Alpha.995–1004.

## Build und CI

Vollständiger Linux-Mint-x64-CMake/vcpkg-Debug-Viewer-Build durchgeführt. Nach Anpassung der persistierten Zehn-Blöcke-Übergabeprüfung: scripts/mint-viewer-validation.sh --tests-only EXIT 0, 183/183 native CTests, 474/474 Python-Vertragstests, isoliertes Xvfb-Startfenster mit Firestorm-Original-Skelett 133 Joints und drei GPU-LLM-Körpermeshes (7186 Dreiecke) sowie OGL-X11 WM_CLASS und echtes 128x128-Taskleistenicon PASS. Der Test ist kein authentifizierter Live-Grid-Avatar-/WASD-End-to-End-Beweis. Exakt verifizierter GitHub-Code-Commit c12920ff5cc737969dd3f651d7db8f698227f238 auf develop-current. Auf Pi5 s359142 ist der Runner.Listener und ein OGL Runner.Worker aktiv; GitHub-Statusabfrage zeigte noch keine Commit-Status-Einträge und keine PR-Workflow-Runs. Dies belegt keinen abgeschlossenen CI-Erfolg: Pi5 ARM64 zur exakten Feature-SHA NOCH UNVERIFIZIERT (NICHT als GRUEN markieren). Hosted Linux x64 ohne verifizierten Compile-/Runner-Nachweis. Kein Produktionsdeploy.
