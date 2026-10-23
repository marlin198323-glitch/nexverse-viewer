# Alpha.1135–1144 – Original-COF-/LLWearable-Autostart nach Login statt F9-Abhängigkeit

24.10.2026 | Linux Mint jam-MS-7D28 Desktop Commander | GitHub develop-current | Version 0.2.0-alpha.1144

## Zehn Entwicklungsblöcke

1135. Auf Linux Mint jam-MS-7D28 GitHub-Branch develop-current, Lokal/Origin fd6a0acd996667cd3c895e55bf5f267653b2ce77 sauber geprüft. CHATUEBERGABE.md zuerst, projectplan.md danach gelesen; Servermain debdcce15d6175a3adc1cd81fd5bd0715317080b.
1136. Hauptlücke ermittelt: originaler Owner-COF-FetchInventory2-/LLWearable-Transfer und aktuelle Self-Shape waren bisher an manuelles F9-Outfit-Fenster gebunden, nicht an regulären authentifizierten Login.
1137. Bounded C++-FirestormAutoCofRead-Guard eingebaut: Autoabfrage nur bei nonempty Owner/COF, gültiger Region-Epoch und Version, maximal einmal pro exaktem Owner/Region/COF/Version, 64-Schlüssel-Deckel, Reset bei Regionswechsel. Manueller F9-Retry bleibt.
1138. Normale Welt-Frame-Verarbeitung startet den originalen READ ONLY FetchInventory2 auch bei geschlossenem F9, ausschließlich authentifiziert, simulatorbereit und bei passendem Lease, CAP, aktueller COF-Version und 1..32 UUID-validierten Originalitems. Keine COF-Mutation oder AgentSetAppearance.
1139. Bei vollständig passendem FetchInventory2-Readback und WearableReadiness ready_for_appearance_pipeline folgt automatisch die bestehende sequenzielle original LLWearable-Assettransfer-Pipeline für 1..16 Wearables. Typ-/ID-/Owner-/Epoch-/COF-Version-/Assetfehler-Gates bleiben.
1140. Blockierte/ungültige Automatikversuche räumen temporäre Flags ab; Regionwechsel setzt Zustand zurück. Keine stillschweigende automatische Wiederholung im Frame-Loop. F9 weiterhin ausdrücklich als manuelle Diagnose und Retry.
1141. Fehlende initiale COF-Ordner-CAPS-Daten werden auch ohne F9 über den bereits vorhandenen owner-/regiongebundenen read-only Inventory-Refresh nachgefragt, nie häufiger als alle 30 Sekunden.
1142. F3-Status zeigt nur im passenden Owner-/Region-/COF-Kontext den tatsächlichen Item-/Wearable-Fortschritt x/y oder fehlende Daten und erinnert an noch fehlenden Live-Outfit-/Skin-Bake-Beleg. Keine Avatar-/Session-/Asset-UUIDs ausgegeben.
1143. Native C++-Tests überprüfen Owner/Region/COF-Version, Wiederbesuch desselben Schlüssels, Invalid-Inputs und Reset. Acht Python-Tests sichern die automatische Auth-/Readiness-/Asset-/CAPS-/F3-/Datenschutzkette und die streng gesperrte Freigabe.
1144. Streng blockierender kompletter Release-Evidence-Prüfer scripts/ogl-release-readiness.py und docs/GOAL_100_PERCENT_RELEASE_ACCEPTANCE.md mit zehn verbindlichen Restarbeiten. VERSION/Projektplan/CHATUEBERGABE/AGENTS/info/JSON/Paritätsmatrizen aktualisiert. Gesamter Mint-CMake/vcpkg-x64-Build, native CTests/Python und isoliertes Original-Firestorm-OpenGL-/GUI-/128px-X11-Icon-Gate vor GitHub-Push, Pi5-CI nur separat bestätigen. Kein Produktivdeployment.

## Technisch belegtes Ergebnis und Grenzen

Der bisherige reine F9-Bedarf für automatische Original-COF-Daten ist im Code entfernt. Die eigentliche Shape-Upload- und Wearable-Pipeline bleibt in denselben authentifizierten Checks; die native Admission wurde getestet. Nicht belegt ist der echte Login einer Grid-Sitzung unter dieser Version mit komplettem echten RGB-Skin-/Outfit-Bake. Echte Server-WASD/Yaw-Acknowledgements, Makeup/Outfit-Komposition, alle Firestorm-GUI-Funktionen sowie Windows/macOS-Release fehlen weiter. Fortschritt 38,65 % gewichtet, ca. 39 % gerundet, keine 100-%-Freigabe.

## QA

Original-Firestorm C++-Autoadmission-Regression und acht neue Python-Regressionen PASS. Vollständiger Linux-Mint-x64 CMake/vcpkg-Debug-Viewer-Build PASS; 183/183 native CTests inklusive Original-Firestorm-OpenGL-Framebuffer, 544/544 Python Tests und isolierter Xvfb-GUI-Start mit originalem 133-Joint-Firestorm-Rig/3 gewichteten GPU-LLM-Bodymeshes/7186 Dreiecken sowie WM_CLASS und 128x128 X11-Taskleistenicon PASS. scripts/mint-viewer-validation.sh --tests-only EXIT 0. Der gesonderte scripts/ogl-release-readiness.py beendet erwartungsgemäß mit EXIT 2 / FINAL RELEASE BLOCKED bei 38,65% und sieben fehlenden echten Live-E2E- beziehungsweise Plattformnachweisen. Keine echte authentifizierte Live-Grid-Sitzung im Test. Exakte Feature-GitHub-SHA und Pi5 ARM64-CI-Status nach Push ergänzen. Nächste Alpha.1145–1154, Fokus echte Grid-/COF-/Skin-/Outfit-/WASD-End-to-End-Blocker.
