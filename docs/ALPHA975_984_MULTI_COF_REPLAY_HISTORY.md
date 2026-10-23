# Alpha.975–984 – mehrere alte COF-Body-TE sicher sperren

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | 0.2.0-alpha.984

## Zehn Entwicklungsblöcke

975. GitHub develop-current a67c18f mit sauberem Arbeitsbranch geprüft, zuerst CHATUEBERGABE.md und projectplan.md gelesen. Linux Mint primär.
976. OpenGenesisLINK Servermain f56e354 (#287) geprüft: World-API-Machine-Key-Scopes, keine neuen Firestorm-Avatar-/LLUDP-Body-Bake-Wire-Anforderungen.
977. Konkreten Fehler nach Alpha.974 gefunden: Bei aufeinanderfolgenden COF-Outfitwechseln wird der einzelne Replay-Fingerprint ersetzt; noch ältere UDP-Body-TE-Pakete können wieder als neu gelten.
978. WorldModel hält bis zu acht alte LocalID/Head/Upper/Lower-Fingerprints nur als Replay-Sperren. Doppelte retired-Fingerprints nicht erneut speichern, keine GPU-/HTTP-/Renderverwendung.
979. Vor Annahme einer eigenen TextureEntry wird über alle retired-Fingerprints geprüft. Echtes neues TextureEntry bleibt zulässig; autoritative Low158 bleibt unverändert.
980. Späte Pakete aus zwei oder mehr vorherigen COF-Outfits können den aktuellen Self-Bake-Kandidaten nicht mehr löschen oder alte Texturen reaktivieren.
981. Begrenzter FIFO-Speicher acht, Entfernen genau passender LocalIDs bei KillObject, Reset bei Login-/Region-/Session-Wechsel. Kein historischer UUID-Export.
982. F3-Diagnose zeigt zusätzlich die Anzahl der aktiven alten Outfit-TE-Sperren; bestehender Replay-Ablehnungszähler bleibt unabhängig von echtem GPU-Bake-Nachweis.
983. Native C++-Regression für COF-Kette, Multi-Replay, FIFO-Kapazität, selektives KillObject und neue Sitzung; Realpath-Python-Contract angepasst/ergänzt.
984. VERSION, Projektplan, Übergabe, AGENTS/info, Fortschritt-JSON und Paritätsmatrizen aktualisiert; Mint-Vollbuild/CTests/Python/Xvfb-Icon-Gate vor Push, Pi5-CI danach. Kein Produktivdeploy.

## Live-Grenzen

Die Tests ersetzen nicht den authentifizierten echten NexVerse Live-Avatar-/COF-/Outfit-/Skin-GPU-Pixelnachweis und keine serverbestätigte WASD-/Yaw-Bewegung. Diese Abnahme ist weiter offen. Projektstatus 38,65 % gewichtet / 39 % gerundet; nächste zehn Entwicklungsblöcke Alpha.985–994.

## Releaseprüfung

Zielgerichteter nativer C++-WorldModel-Test sowie neuer und angepasster Python-Contract: PASS.
Mint x64: scripts/mint-viewer-validation.sh voller CMake/vcpkg-Debug-Build abgeschlossen. Nach Anpassung der Versions-/Übergabe-Contracts: scripts/mint-viewer-validation.sh --tests-only EXIT 0; 183/183 native CTests, 470/470 Python-Tests; isolierter Xvfb-GUI-Start mit Firestorm 133-Joint-Skelett und drei GPU-LLM-Körpermeshes/7186 Dreiecken; X11 WM_CLASS und Taskleisten-Icon 128x128 PASS. Exakte GitHub-Code-SHA 8a6faf4b4baee3d267f1bf23d5f3576d82d9c80b nach Mint-Gate gepusht. GitHub-Connector meldet keine Commit-Status-Einträge und keine PR-Workflow-Runs; dies ist kein Beweis für abgeschlossene Pi5-ARM64-CI. Pi5-Status zur exakten Feature-SHA UNVERIFIZIERT. Nicht als GRUEN melden. Keine Release- oder Produktivfreigabe.
