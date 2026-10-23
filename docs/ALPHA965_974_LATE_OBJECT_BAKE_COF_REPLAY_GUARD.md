# Alpha.965–974 – Veraltete ObjectUpdate-Body-TE nach COF-Wechsel nicht wieder einsetzen

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | OGL Viewer 0.2.0-alpha.974

## Zehn Entwicklungsblöcke

965. GitHub develop-current HEAD 2d40f37, Arbeitsverzeichnis sauber und origin identisch; zuerst CHATUEBERGABE.md und projectplan.md gelesen. Pi5 Alpha964 exakte Feature-SHA 7bf7308 Run 40011135542 COMPLETED/SUCCESS.
966. OpenGenesisLINK Servermain ccfc644d erneut geprüft: keine neuen Low158/Avatar-ObjectUpdate-Firestorm-Wireänderungen; bestehende Firestorm-Bake-Mesh-Vorgaben erhalten.
967. ECHTEN verbleibenden COF-Quellen-Race gefunden: Nach Alpha964-Widerruf eines veralteten provisorischen ObjectUpdate TE konnte dasselbe ältere UDP ObjectUpdate-Paket im selben Region-Circuit erneut eintreffen und alte Body-Texturen wiederherstellen; bei neuerem LocalID sogar die aktuelle Textur verwerfen.
968. WorldModel hält nach authentisch akzeptiertem COF-/AppearanceVersion-Outfitwechsel genau eine zurückgezogene Self-ObjectUpdate-Provenienz (LocalID und 3 baked UUIDs) als REINEN Ablehnungs-Fingerprint. Sie erscheint weder in Renderbakes noch im GetTexture-HTTP-/GPU-Plan.
969. Exakt alte UUID-Tripel + altes Self-LocalID werden in der echten observe_self_object_bakes-Pipeline nach erfolgreichem TextureEntry-Decode und vor Akzeptanz als UDP-Replay zurückgewiesen. Fremder Agent, Region oder LocalID=0 bleiben wie zuvor abgewiesen; echte Low158 bleibt Autorität.
970. Der zuvor gebundene aktuelle LocalID verliert seine echten aktuellen Self-Bakes nicht, nur weil eine verspätete alte ObjectUpdate-Nachricht des zurückgezogenen LocalID wieder eintrifft. Duplikate können beliebig versetzt erscheinen, solange die alte Outfit-Provenienz gilt.
971. Eine tatsächlich andere komplette neue Server-TE und eine neue gültige Self-LocalID dürfen weiterhin fehlende Head/Upper/Lower-Faces ergänzen. Der alte Ablehnungs-Fingerprint bleibt selbst nach solchen neuen echten Daten aktiv, damit ein nochmals späteres altes UDP-Paket den neueren Skin nicht überschreibt.
972. Bei neuem Login/Agent, Region-/Simulatorwechsel, WorldModel-Reset und echtem passenden KillObject wird der Ablehnungs-Fingerprint zurückgesetzt. Eigener saturierender, sitzungsbezogener Ablehnungszähler (0..UINT32_MAX), ohne UUID-/Session-Logging.
973. F3 meldet Alte Objekt-Bakes erneut verworfen N getrennt von COF-Wechsel, serverseitigem Low158-Bake, GetTexture und GPU-Pixeln. Native echte Self-WorldModel-/TextureEntry-Teststrecke um 100→101, spätes Doppelpaket, neue LocalID, anderes TE mit gleichem LocalID, erneutes verspätetes Paket und Neuanmeldung erweitert; Mint/Pi5 Worldlinked und neuer Python-Realpath-Vertrag.
974. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Fortschritts-JSON und Firestorm-/Server-/UI-Paritätsmatrizen aktualisiert. Voller Linux-Mint-x64 CMake/vcpkg/CTests/Python/Xvfb-GUI/Taskbar-Icon-Gate vor Code-GitHub-Push; Pi5/Hosted-CI pro SHA separat dokumentiert. Kein Produktivdeploy.

## Reale Live-Abnahme weiter offen

Der tatsächliche Login in das NexVerse-Grid mit AvatarAppearance/COF-/Outfit-/Skin-GPU-Pixeln und serverbestätigter WASD/Yaw-Bewegung wurde nicht in einer authentifizierten Livesitzung nachgewiesen. Isoliertes Mint-GUI und nativer Packet-Replay-Test liefern keinen Live-Beweis. Gesamtprojekt 39 % (38,65 % gewichtet); nächster Abschnitt Alpha.975–984. Kein Produktivdeploy.

## Verifizierter vollständiger Linux-Mint-x64-Release-Gate
scripts/mint-viewer-validation.sh EXIT 0.
Viewer 0.2.0-alpha.974 kompletter CMake/vcpkg Linux-x64-Build;
183/183 native CTests, 467/467 Python; isolierter
Xvfb-GUI mit Original Firestorm Avatar-Bind-Skeleton
133 Joints, drei GPU-LLM-Körpermeshes/7186 Dreiecke,
OGL X11 WM_CLASS und Taskleisten-Systemicon 128x128 PASS.
Kein authentifizierter Live-NexVerse-Avatar/COF/Skin/WASD,
kein Produktivdeploy.

## Exakte Code-SHA / GitHub CI
Feature SHA 4798bff70bb86abed265229b6f31c255ab00d355.
Pi5 ARM64 Source Check Run 40012744294 IN_PROGRESS beim
letzten Abruf; kein Pi5-Gruen vor completed/success.
Hosted Linux-x64 Run 40012744160 COMPLETED/FAILURE,
Job 120616981110 runner_id 0 steps 0, kein gestarteter Compiler.
Letzter Pi5 SUCCESS Alpha.964 Code-SHA 7bf7308
Run 40011135542. Lokaler Mint-x64-Gate EXIT 0:
183 CTests, 467 Python, Firestorm Xvfb Original-
GPU-Körpermeshes und OGL X11 Taskleisten-Icon.
Kein Live-Grid-E2E und kein Produktivdeploy.
