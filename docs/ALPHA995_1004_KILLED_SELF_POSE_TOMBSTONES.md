# Alpha.995–1004 – KillObject-Avatar darf nicht per altem UDP-Paket wieder aufleben

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | OGL 0.2.0-alpha.1004

## Zehn Entwicklungsblöcke

995. Linux-Mint-Desktop-Commander, GitHub develop-current 29d7cda, sauberer Branch und Remote synchron. CHATUEBERGABE.md, projectplan.md und Alpha.985–994 zuerst gelesen.
996. OpenGenesisLINK-Servermain f56e354694a41adb2334884c21efc45db784cb92 erneut überprüft; kein neues Firestorm/OpenSim-Avatar- oder KillObject-Wire-Protokoll.
997. Konkreten Fehler im echten Full/Compressed ObjectUpdate-Handler ermittelt: nach echtem Self KillObject ist LocalID=0 und High9 Terse sicher, jedoch kann verspätetes Full/Compressed mit eigener UUID dieselbe getötete LocalID neu binden und Self-Position, Kamera und WASD-/Yaw-Beleg fälschlich freigeben.
998. WorldModel hält authentifizierte KillObject-LocalIDs als zusätzliche, nicht gerenderte, agent/session/region-gebundene FIFO-Tombstones (maximal 16 LocalID+Body-Fingerprints) fest.
999. Existierende frühe Eingangsprüfung reject_retired_self_object_pose in beiden tatsächlichen App-Handlern nun auch für getötete LocalIDs: alte exakte Body-TE, fehlende, ungültige oder Placeholder-TE ohne echten neuen Body-Beweis werden VOR LocalID-/Pose-/WASD-Bindung abgewiesen.
1000. Wirklich neue validierte nichtleere Body-TE kann gleiche numerische LocalID neu etablieren. Nach erfolgreicher Annahme dürfen unvollständige normale Compressed/Terse-Body-Updates für diesen aktiven Lease weitergehen, alte exakte Kill-Fingerprints bleiben aber gesperrt.
1001. Bei erneutem Kill derselben LocalID alle noch vorhandenen alten COF-Body-Fingerprints plus aktuellen Kandidaten übernehmen und deduplizieren; FIFO-16-Budget, Reset nach Session-/Region-/Loginwechsel, keine Speicherung im Asset-Cache oder Rendering.
1002. F3 Avatar-Bake-Status erweitert um „Späte KillObject-Posen verworfen“ und „KillObject-IDs gesperrt“, ausschließlich Zahlen und ohne Avatar-/Skin-UUID-Ausgabe.
1003. Native C++-Regression mit echtem Low KillObject Wire-Decode, altem Full-/Compressed-Body, malformed/fehlender TE, echter neuer TE auf gleicher ID, nochmaligem alten Replay, zwei Kill-Revisions-Fingerprints, FIFO 20->16 und Regions-/Sessionreset; Python-Vertrag sichert beide echten Handler vor Bindung/Pose.
1004. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Fortschritt-JSON und Firestorm-/OpenGenesisLINK-/UI-Paritätsmatrizen aktualisiert; vollständiges Mint-x64 CMake/vcpkg/CTests/Python/Xvfb-/Taskleistenicon-Gate vor Push. Pi5-CI für exakte SHA gesondert. Kein Produktivdeploy.

## Grenzen

Ein Simulator kann dieselbe numerische LocalID nach einem Kill theoretisch ohne neue Body-TE wiederverwenden: ohne zusätzliche Epoch-/Wire-Beweise lässt sich dann eine neue Instanz nicht sicher von verspätetem UDP unterscheiden. Der Viewer wählt konservativ die Sperre. Eine wirklich neue valide Body-TE erlaubt einen erneuten Lease; identische alte TE nicht.

Die synthetischen Regressionen und isolierten Xvfb-GUI-Tests ersetzen keine authentifizierte echte NexVerse Live-Avatar-/COF-/Skin-/Outfit-GPU-Pixel-Abnahme und keine Serverbestätigung für WASD/Yaw. Beides bleibt still open. Projektstand 38,65 % gewichtet / 39 % gerundet; nächste Alpha.1005–1014.

## Mint Gate / GitHub / Pi5

Vollständiger Mint-x64-CMake/vcpkg-Debug-Viewer-Build PASS. scripts/mint-viewer-validation.sh EXIT 0: 183/183 native CTests, 477/477 Python-Tests, isolierter Xvfb GUI-Start mit Firestorm-Originalskelett 133 Joints und drei GPU-LLM-Körpermeshes (7186 Dreiecke); X11 WM_CLASS und echtes 128×128 Taskleistenicon PASS. Kein Grid-Login, keine Live-Avatar-/WASD-Abnahme. GitHub-Code-SHA und Pi5-ARM64-Status nach Push separat nachtragen.
