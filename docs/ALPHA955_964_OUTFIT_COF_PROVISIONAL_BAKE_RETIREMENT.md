# Alpha.955–964 – Alte provisorische Self-Body-Texturen bei echtem COF-Outfitwechsel entfernen

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | Viewer 0.2.0-alpha.964

## Zehn Entwicklungsblöcke

955. Sauberen GitHub develop-current HEAD 362a1bb mit origin geprüft, CHATUEBERGABE.md und projectplan.md zuerst gelesen; Pi5 Alpha954 Feature-SHA c86977f Run 40008385875 COMPLETED/SUCCESS.
956. OpenGenesisLINK Servermain ccfc644d geprüft; keine neuen Low158-/COF-/LLUDP-Wireänderungen. Firestorm-/OpenSim-Low158 bleibt Autorität für eigenes Skin/Outfit.
957. REALEN Avatar-Outfit-Texturmischfehler gefunden: WorldModel::self_bake_selection nutzt vorläufige eigene ObjectUpdate-TE für fehlende Low158-Head-/Upper-/Lower-Bakes, ließ aber diese TE nach neuer bestätigter COF-Outfitversion bestehen. So konnten noch alte Körpertexturen die neuen Lücken füllen.
958. Beim tatsächlich angenommenen neueren Low158-COF oder geändertem AppearanceVersion-Format ungültige vorläufige ObjectUpdate-Bakes aus vorherigem Outfit verwerfen. Originale autoritative Low158-Texturen und zuvor eingegangene valide Self-Appearance bleiben erhalten.
959. Vorhandenen COF-Highwater vor dem Wechsel berücksichtigen, auch wenn dazwischen eine unversionierte Legacy-Count0-Low158 eintrifft. Kein Reset bei erster modern bestätigter COF, derselben Version, reinem Legacy-Readback oder aufgrund des Rollback-Gates verworfenem älteren Packet.
960. Nach wirklich frischer, region-/agentgebundener ObjectUpdate mit gültiger TE dürfen weiterhin aktuelle provisorische Bakes für fehlende Low158-Slots verwendet werden. Die alte TE wird nur an der wirklichen Outfitgrenze entfernt; kein pauschales Abschalten der Firestorm-Fallbackpipeline.
961. WorldModel-Counter self_object_bake_outfit_invalidations pro Agent/Session/Regiongeneration mit bounded UINT32; Reset bei Login-, Region-, Simulator- und Epochwechsel. Kein Aufzeichnen privater Texture-UUIDs.
962. F3 zeigt 'COF-Wechsel: alte Objekt-Bakes verworfen:N' getrennt von Self-Bake Withdrawals, alten COF-Snapshots, GetTexture-Download und wirklichem GPU-Bake-Erfolg.
963. Native C++23-Mint-WorldModel-Regressionsuite um COF 100→101, gleiche Revision, verspätete abgewiesene Revision, Legacy-Zwischenmeldung, Formatrevision, neues ObjectUpdate und Sitzungsreset erweitert; in Pi5 ARM64 Headless-Worldlinked Testloop ergänzt; neuer Python-Produktionsvertrag prüft F3 und Revisionsevidenz.
964. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS/info, Fortschritt-JSON sowie OpenGenesisLINK-/Firestorm-/UI-Paritätsmatrizen aktualisiert; kompletter Mint-x64-Viewer/CTest/Python/Xvfb/Icon-Gate vor GitHub-Push und exakte Pi5-/Hosted-CI-Code-SHA dokumentieren. Kein Produktivdeploy.

## Live-Nachweis offen

Echter authentifizierter NexVerse-Avatar mit serverseitigen COF-/Low158-/Outfit-/Skin-GPU-Pixeln und bestätigter WASD-/Yaw-Bewegung bleibt still open. Lokale Native-/Xvfb-Tests sind kein echtes Grid-E2E. Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.965–974. Kein Produktivdeploy.

## Vollständiges Mint-x64-Release-Gate bestanden
scripts/mint-viewer-validation.sh EXIT 0:
CMake/vcpkg Linux-x64 Viewer 0.2.0-alpha.964,
183/183 native CTests inklusive WorldModel aktueller COF/Obj-TE
Auswahl, 462/462 Python-Integrationsverträge,
isolierter Xvfb Viewer mit originalem Firestorm-Skelett 133 Gelenke,
3 GPU-LLM-Bodymeshes / 7186 Dreiecke,
X11 WM_CLASS und OGL 128x128-Systemicon PASS.
Keine echte authentifizierte Grid-Live-Session; kein Deploy.

## Exakte GitHub-Code-SHA und CI
Feature SHA 7bf730812ed149f358a84b12ebe8d24ee6cba14a.
Pi5 ARM64 Run 40011135542 zuletzt IN_PROGRESS,
nicht grün bis completed/success.
Hosted Linux x64 Run 40011135336 COMPLETED/FAILURE,
Job 120611503544, runner_id=0, steps=0:
kein gestarteter Compiler wegen fehlender Runner-Zuteilung.
Letzter Pi5 SUCCESS Alpha.954 SHA c86977f
Run 40008385875.
Lokaler Linux Mint voller x64-Gate EXIT 0:
183 native CTests, 462 Python, Xvfb Firestorm LLM
GPU-Körpermeshes und X11-Systemicon 128x128 PASS.
Kein authentifizierter Live-Grid-Login, kein Deploy.
