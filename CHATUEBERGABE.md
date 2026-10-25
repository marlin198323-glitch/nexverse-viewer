**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); ROADMAP.md maßgebend. OGL v0.4.0.0 IN ARBEIT, Fortschritt 39 % (38,65 % gewichtet), kein main-Merge. DESIGN-ABNAHME: WebKit-Welcome-URL unverändert, responsiver proportionaler WebKit-Zoom für 2048x838, kompakte Login-/Header-Geometrie und kein cyanfarbener Fokusrahmen.**

**OGL v0.4.0.0 – FEHLERBEHEBUNG SNAPSHOT/RESIZE (25.10.2026):** 10 zusammenhängende Bugfix-/Regressionstestblöcke auf Linux Mint jam-MS-7D28: WebKit-Generation gegen verspätete Frames, genau ein Pending-Snapshot, sichere Request-Maße, gedrosselte 3 Wiederholungen, Resize-Invaliderung, 30-s-Datenaktualisierung, OpenGL-Textur-Neuübertragung bei gleicher Auflösung, sicheres Ende asynchroner Callbacks und echte X11-Resizes bei 1100x640/1910x790/1280x720/1648x928. **188/188 native CTests, 675/675 Python-Tests und original Firestorm Xvfb OpenGL-GUI/X11-Icon PASS.** Authentifizierte Avatar-/CAPS-/WASD-/Shutdown-Live-Tests weiterhin OFFEN. Dokumentiert in docs/V0_4_0_0_WELCOME_SNAPSHOT_ERROR_REPAIR_2026_10_25.md. Gesamtprojekt **39 % (38,65 % gewichtet)**; Version v0.4.0.0 IN_ARBEIT, main unverändert.

**VISUELLE NACHPRÜFUNG 25.10.2026 (v0.4.0.0):** 1672x941 Referenz + 2048x838 echter Mint-Screenshot mit isolierten Viewer-Renders bei 1648x928 und 2048x838 verglichen. WebKit-Welcome-Inhalte unverändert; die Verzerrung der Grafik korrigiert durch Screenshot in echter Contentfläche und bei niedrigem UltraWide uniformen Browserzoom. Hero-NexVerse-Logo UND News-Karten sichtbar. Native Header/Login-Felder feinere Konturen ohne Fokusglow, korrekt balanciert, Beschriftungen lesbarer. **187/187 native CTests, 671/671 Python und Firestorm-Xvfb-OpenGL/GUI/X11-Icon PASS**, reale Live-Grid-Abnahme bleibt OFFEN. Alle Details: docs/V0_4_0_0_VISUAL_REFERENCE_RECHECK_2026_10_25.md. **Projektfortschritt 39 % (38,65 %)**; kein Release oder main-Merge.

**UPDATE 25.10.2026 – KOMPAKTER NEON-LOGIN + SHUTDOWN-PROFILING (v0.4.0.0):** Direkt auf Linux Mint jam-MS-7D28 implementiert: Header 58 px, Footer 60 px, Logininputs 205×32 px, Welt-betreten 180×32 px (1648×928), Fokus ohne äußere Umrandung. Welcome-URL unverändert; WebKit-Zoom für Ultrawide; C++-Hitboxes und Renderer teilen Layout. LLUDP-Retry-Sleep bei Stop in 50-ms-Schritten abbrechbar; gesamter async-Worker-Drain mit phasenspezifischen Logs und realem Gesamtzeit-Marker nach GLFW-Ende. Volles Mint-Gate 187/187 native CTests, 667/667 Python, Firestorm-Original-GPU-/Xvfb-GUI-/128px-X11-Icon PASS. Echter eingeloggter NexVerse-Exit weiterhin UNVERIFIZIERT; kein main-Merge, kein Produktionsrelease. Versionsziel v0.4.0.0 IN_ARBEIT, 38,65 % (39 %). Details docs/V0_4_0_0_MINI_LOGIN_AND_MEASURED_SHUTDOWN.md.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE (ZEHN ECHTE ENTWICKLUNGSBLÖCKE); BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE; öffentliches Versionformat vMAJOR.MINOR.PATCH.REVISION. AKTUELL v0.4.0.0 (IN ARBEIT, nicht released), Gesamtfortschritt 39 % (38,65 % gewichtet), NÄCHSTES ZIEL v0.4.0.0; Planung ROADMAP.md.**

**AKTIVE ROADMAP v0.4.0.0 – GRID/LOGIN/CAPS SECURE TRANSPORT (10 ECHTE ENTWICKLUNGSBLÖCKE):** Exakte auf GitHub bestätigte und lokal vollständig getestete Code-SHA a4441680e3b261d4754afe2d2ca1e6e9e713758e (GitHub-CI für diese SHA UNVERIFIZIERT). v0.3.0.0 als geprüfte Baseline FERTIG, v0.4.0.0 IN ARBEIT. XML-RPC-/CAPS-/GridInfo-URI-Validator verweigert UserInfo-Spoof, defekte Ports/Hostsegmente, Steuerzeichen und Redirect-Manipulationen; LoginService verweigert ungültige Ziele ohne sensitives POST. CAPS-URLs aus Seed LLSD werden geprüft, manipulierte GridInfo erhält keinen Cacheersatz. libcurl folgt keinem 30x-Redirect (GET Bearer-URL-Token und POST Passwort-Hash/LLSD-Body bleiben geschützt). Ein echter Loopback-libcurl-307-HTTP-Test plus native GridDiscovery/Login/CAPS-Tests und zehn Python-Quellverträge. Mint-x64-CMake/vcpkg-Build 428/428 Schritte, 185/185 native CTests, 628/628 Python, echter Original-Firestorm-Xvfb-GPU-/GUI-/128px-X11-Icon-Test PASS. Offen: authentifizierter aktueller Live-Grid-Login/SSO, CAPS-/Region/Teleport-Reconnect, tatsächlich serverbestätigte WASD/Yaw/Jump/Flight. T01 IN_ARBEIT, T02/T03 und A01/A02/A03 OFFEN, Fortschritt weiter 38,65 %, kein main-Merge, keine v1-Freigabe. Details `docs/V0_4_0_0_GRID_LOGIN_CAPS_SECURITY_BATCH01.md`.

**/goal v1.0.0.1 – RELEASE BLOCKIERT (24.10.2026):** v1.0.0.0 wurde NICHT auf main veröffentlicht. GitHub develop-current vor diesem Durchgang: d1e28f4074e4a405fecd1fedf8b5245f77e9d727; origin/main: f4d4a93ecb9b032e0fa675b42cadb8c7ecff80c0. Kein stabiler Vorgänger-Tag/GitHub-Release oder späterer reproduzierbarer Hotfix-Bug. Fortschritt 38,65 % (39 %). Neue v1.0.0.1-Release-Sperre kontrolliert annotierten Vorgängertag, exakte Remote-Tag-Identität, main-Vorfahren, v1.0.0.0-VERSION und übereinstimmenden Release-Beleg; 11 isolierte neue Git-Regressionsprüfungen PASS; vollständiger Mint-Gate 184/184 native CTests, 617/617 Python, Firestorm-Xvfb-GPU-/GUI-/X11-Icon PASS. VERSION bleibt v0.3.0.0, nächstes funktionales Ziel v0.4.0.0. Kein main-Merge, Tag oder Deployment. Details `docs/V1_0_0_1_RELEASE_BLOCKERS_2026-10-24.md` und `docs/RELEASE_VERSION_POLICY.md`.

**NEUER /goal-VERTRAG:** `/goal vX.Y.Z.W fertigstellen` erledigt alle Aufgaben und Abnahmekriterien der Zielversion inklusive Vorgänger. `/goal v1.0.0.0 komplett fertigstellen und releasen` bedeutet vollständige Firestorm-/Grid-Parität, 100 % und sieben echte Endbeweise; erst dann Release-PR nach `main`, Merge, Tag `v1.0.0.0` und GitHub Release. `/goal v1.0.0.1 fertigstellen` ist die erste Wartung nach veröffentlichtem v1.0.0.0. Quellen: `VERSION`, `ROADMAP.md`, `docs/RELEASE_ROADMAP.json`, `docs/RELEASE_VERSION_POLICY.md`, `docs/PROJEKT_AUDIT_2026-10-24.md`. Die bisherigen Alpha.1–1194-Berichte bleiben NUR HISTORIE und keine öffentlichen Versionsstufen. Kein main-Merge/Tag ohne echten erfolgreichen Gate-Test.

**VERBINDLICH FÜR NÄCHSTEN CHAT:** Erst `CHATUEBERGABE.md`, dann `ROADMAP.md`/`docs/RELEASE_ROADMAP.json`, danach `projectplan.md` lesen. Entwicklung auf Linux Mint jam-MS-7D28 im Branch develop-current, 10 echte Entwicklungsblöcke je Durchgang; die nächsten sind Teil des Versionszieles v0.4.0.0. Alte Version 0.2.0-alpha.1194 ist nur Implementierungshistorie; erster stabiler Release v1.0.0.0 noch offen. Das Skript `scripts/ogl-version.py --release-check v1.0.0.0` muss bis zu allen fehlenden Live-E2E- und Plattformbeweisen mit Exit 2 blockieren. Bisherige technische Alpha1194-Gates 184 native CTests/594 Python/FIrestorm-GPU-Xvfb bestanden, aber keine vollständige End-to-End-Abnahme.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); /goal OGL Viewer VOLLSTAENDIG FERTIGSTELLEN. Viewer 0.2.0-alpha.1194, Projektfortschritt ca. 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1195–1204.**

**ALPHA.1194 – ECHTER ANKLICKBARER FLUGMODUS IM VIEWER-MOVE-FENSTER:** GitHub-verifizierte Feature-Code-SHA 4cc9bc01606e2090d2c2adc080f57cc4bfc879c0 (Mint-x64-Build 426 Schritte, 184/184 native CTests, 594/594 Python, original Firestorm-Xvfb-GPU-/GUI-/128px-X11-Icon PASS, CI für genau diese SHA UNVERIFIZIERT). Nur Linux Mint Desktop Commander jam-MS-7D28, GitHub develop-current. Das bereits vorhandene Move-Fenster verfügt nicht mehr ausschließlich über eine F-Tastenanleitung: neu ist eine sichtbare echte klickbare Anforderung Flugmodus aktivieren/beenden (204x42 Pixel skaliert), die exakt wie die vorhandene Taste F über MovementKeys/MovementController den tatsächlichen LLUDP AgentUpdate-control_fly-Modus in die originale OutboundCommandQueue übergibt. Nur aktuelle authentifizierte Simulator-SELF-Position mit gültigem Session-/Agent-/Epoch-Readback autorisiert die Aktion; sonst deaktiviert. Queue-Erfolg aktualisiert ausschließlich den lokalen angeforderten Flugstatus, bei Queue-Failure Rollback und Request erhalten. Bei Logout/ungültiger Pose UI-Flags zurücksetzen. F3/Alpha1184 bewerten echten Flug weiter nur nach tatsächlichem Server-Z-Readback, niemals als UI-Klick-Beweis. Voller Mint-x64-Debug-CMake/vcpkg-Build 426 Schritte, 184/184 native CTests, 594/594 Python und original Firestorm-Xvfb-GPU-/GUI-/128px-X11-Icon-Gate PASS. Native ViewerShell-Layout-Tests 1100/1920/3440 und echter isolierter Xvfb-GL-Render-Test + 10 Python-Verträge. Kein neuer auth-Grid-Live-Flight-, Avatar-Skin-/Outfit-3/3-RGB- oder finaler WASD-Endtest; 39 %, Release BLOCKED, kein Deploy. Details docs/ALPHA1185_1194_CLICKABLE_FLY_MOVEMENT_UI.md.

**VERBINDLICH:** Nur Linux Mint Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer develop-current. Pi5 nur ARM64-CI. Vor Push vollständiges Mint-CMake/vcpkg-x64/native 184 CTests/Python/Xvfb-original Firestorm GPU/GUI/128px-X11-Systemicon-Gate. Die nächste Phase Alpha.1195–1204 muss P0 Avatar-RGB-Skin/COF-/Outfit-/WASD-/Fly-Live-E2E und P1 vollständige sichtbare Firestorm-Funktionen priorisieren. Keine 100%-Freigabe ohne echte Nachweise.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); /goal OGL Viewer VOLLSTAENDIG FERTIGSTELLEN. Version 0.2.0-alpha.1184, Gesamtfortschritt ca. 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1185–1194.**

**ALPHA.1184 – SERVER-HÖHEN-READBACK FÜR JUMP/FLY/DOWN:** GitHub-bestätigte Feature-Code-SHA f3558b9d83cf3d2e16cdf8c5505a35f6431e71dc (voller Mint-x64-Debug-CMake/vcpkg-Build 426 Schritte, 184/184 native CTests, 584/584 Python und isolierter original Firestorm-GPU-/GUI-/128px-X11-Icon-Test PASS; Pi5 ARM64-CI UNVERIFIZIERT, Runner im letzten Prozesscheck nicht erkennbar). Entwicklung Linux Mint Desktop Commander jam-MS-7D28, Branch develop-current. Unabhängiger thread-safe VerticalMovementFeedbackMonitor verarbeitet die tatsächlich erfolgreich per UDP gesendeten ursprünglichen Jump/Up-/Down-/Fly-Steuerbits und verlangt für eine reale Bestätigung mindestens 0,25m Z-Positionsänderung in genau dieser Richtung in authentifizierten self-only Simulatorpositionen aus High9/Terse, Full ObjectUpdate oder Compressed ObjectUpdate. Queue-only, ungültige Serverdaten, vor Sendung empfangene Antworten, Teleport >=32m, Gravitation in Gegenrichtung und rein horizontale Bewegung beweisen kein Steigen. Flightmodus- und Up/Down-Wechsel widerrufen alte Antworten; Stillstand/gegenläufige Bewegungen werden gesondert ausgewiesen. Resets bei allen neun bestehenden Movement-Leases; F3 zeigt echten Höhenstatus, nie einen erdachten Flug-Erfolg. Originales Horizontal-WASD-/Turn-Readback unverändert. Eigene native CTest-Regression und zehn Python-Integrationsverträge. Vollständiger Linux-Mint-CMake/vcpkg-Build 426/426 Schritte, 184/184 native CTests, 584/584 Python, isoliertes Original-Firestorm-Xvfb-GPU-/GUI-/128px-X11-Icon-Gate PASS. Upstream d93b491318e10ad236ad906c71d995067eb470c4 unverändert (Material/PBR, kein neuer Movement-Wire-Bedarf). KEIN authentifizierter Grid-Flight-/Jump- oder finaler RGB-Skin-/COF-Outfit-/WASD-Live-E2E-Beweis. Release weiterhin BLOCKED; kein Deploy, 39 % Fortschritt. Details docs/ALPHA1175_1184_REAL_VERTICAL_MOVEMENT_READBACK.md.

**VERBINDLICH:** Code nur Linux Mint jam-MS-7D28 Desktop Commander im develop-current. Pi5 nur CI. Vor Push vollständiger Mint-x64-Debug/CMake/vcpkg/nativer CTest/Python/Firestorm-Xvfb-OpenGL-GUI/128px-X11-Taskleistenicon-Nachweis. 100 % erst bei vollständigem Firestorm-Funktionsumfang und echten dokumentierten sieben E2E-Nachweisen gemäß docs/GOAL_100_PERCENT_RELEASE_ACCEPTANCE.md. Nächste Alpha.1185–1194, reale Outfit-/Skin-/World-/WASD-/Flight-E2E und UI-Funktionsparität weiter P0.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); /goal OGL Viewer VOLLSTAENDIG FERTIGSTELLEN, ohne unbewiesene 100%-Freigabe. Viewer 0.2.0-alpha.1174, Gesamtfortschritt ca. 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1175–1184.**

**ALPHA.1174 – ECHTE SOCKET-VERSENDETE AGENTUPDATE-RICHTUNG MIT SERVERPOSITION ABGLEICHEN:** GitHub-verifizierte exakte Feature-Code-SHA 6a41e5314bba7987e54ab0db5c8b15acd0d7648a (Mint-x64-Debug-CMake/vcpkg-Build 424 Schritte, 183/183 native CTests, 574/574 Python und isoliertes Firestorm-Xvfb-GPU-/GUI-/128px-X11-Icon PASS; Pi5 ARM64-CI für genau diese SHA UNVERIFIZIERT). Linux Mint Desktop Commander jam-MS-7D28, GitHub develop-current. Eine reale Socket-Send-Callback gibt nun das tatsächlich gesendete AgentUpdate-BodyRotation-Quaternion zusammen mit den Controls an den Monitor. Der ursprünglich zu großzügige 0,25m-Horizontalbewegungs-Beweis akzeptiert jetzt nur mindestens 0,25m POSITIVE Projektion in die wirklich übertragene WASD-/Strafe-/Yaw-Richtung. Falsche/seitliche/Z-only-Bewegung, ungültiges Quaternion, Teleport/Positionssprung, pre-send/Stale oder ersetzte Queue können NICHT als bestätigtes WASD gelten. W→S-/Strafe-Wechsel resetten alte Server-Belege; kleine laufende Yaw-Korrekturen unterbrechen die Bewegung nicht fortlaufend. F3 unterscheidet echte falsche/querläufige Serverbewegung vom Stillstand oder bestätigten Richtungsreadback. Gesamter Mint-x64-CMake/vcpkg-Debug-Build mit 424 Schritten, 183/183 native CTests, 574/574 Python, Original-Firestorm-Xvfb-GPU-/GUI-/128px-X11-Taskleistenicon PASS. Native Richtung/Rotation/Reverse/Stale-Regressionsbeweise und zehn Python-Integrationstests. OpenGenesisLINK Upstream d93b491318e10ad236ad906c71d995067eb470c4 (neue PBR-Material-/glTF-Indexsicherheit, kein neuer LLUDP-Wire-Standard). Kein authentifizierter neuer Grid-WASD-/Yaw-Live-E2E und kein 3/3-RGB-Skin-/COF-Outfit-Live-Test, Release BLOCKED, Fortschritt weiter 39%, KEIN Deploy. Details docs/ALPHA1165_1174_ACTUAL_SENT_AGENTUPDATE_DIRECTION_PROOF.md.

**VERBINDLICH:** Nur Linux Mint Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, GitHub Branch develop-current; Pi5 nur ARM64-CI. Auftrag /goal heißt tatsächliche Firestorm-Funktionsparität und vollständige reale Live-Grid-E2E-Nachweise, nicht nur Unit Tests. Vor jedem Feature-Push volles Mint-x64-CMake/vcpkg-Build/native CTests/Python/Xvfb-Original-Firestorm-GPU-/GUI-/128px-X11-Icon-Gate. Nächste 10 Blöcke Alpha.1175–1184, Schwerpunkt Avatar-/Outfit-/WASD-/UI-E2E. release-readiness bleibt EXIT 2 ohne echte Belege.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); /goal OGL Viewer VOLLSTAENDIG FERTIGSTELLEN. Viewer 0.2.0-alpha.1164, Gesamtfortschritt ca. 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1165–1174.**

**ALPHA.1164 – ECHTE ORIGINALE FIRESTORM-KOPF/OBER-/UNTERKÖRPER-COF-FARBEN AM EIGENEN AVATAR:** GitHub-verifizierte Feature-Code-SHA 6aa8618bb8434d7746b2f52527d130a2ac4ee7b4 (Mint-x64-CMake/vcpkg-Build 424 Schritte, native 183/183 CTests, 564/564 Python und echtes Original-Firestorm-Xvfb-GPU-/GUI-/128px-X11-Taskleistenicon PASS; Pi5 ARM64 CI für exakt diese SHA UNVERIFIZIERT). Linux Mint Desktop Commander jam-MS-7D28, GitHub develop-current. Die bisher bloß in F9 lokalen RGB/Masken/LAD-Layer-Preview-Bilder werden nun auf den wirklichen ORIGINAL-Firestorm-LLM-Head/Upper/Lower-GPU-Meshes des EIGENEN AVATARS gezeichnet, wenn jeweils keine reale serverseitige TextureEntry-Bake-RGBA auf GPU resident ist. Erst echter Server-Bake vor lokalem Partial-Preview; keine Fake-Bake-GL-Counter. 3 getrennte transaktionale, maximal 128x128 eigene RGBA-GPU-Namen, keine Remote-Avatar-Änderungen, kein Ghost bei Logout/Region/COF-Wechsel und kompletter Cleanup bei Neulayer. Vollständiger Mint-x64-CMake/vcpkg-Build/183 native CTests/564 Python und Xvfb-Original-Firestorm-GPU-/GUI-/128px-X11-Icon-Test PASS. F3 meldet wirklich gezeichnete lokale Head/Upper/Lower-GL-Calls als 'unvollständig, kein Server-Bake'. Echter Firestorm-Xvfb-OpenGL-Framebuffer: 3398 sichtbar veränderte lokale Self-Body-Pixel, GL-Draws 1/1/1, Serverbake-Vorrang getestet, Remote 0, Reset 0; echte Original-LLM-Meshes und synthetische Test-RGBA. Keine authentifizierte Grid-Live-3/3-Skin-/Outfit-/WASD-/Yaw-E2E-Abnahme; gesamtes Produkt weiterhin nur 39 %, Release-Gate BLOCKED, KEIN Deploy. Details docs/ALPHA1155_1164_REAL_SELF_FIRESTORM_LOCAL_COF_RGB_RENDER.md.

**VERBINDLICH:** Nur Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer develop-current, Pi5 ausschließlich ARM64-CI. Vollständiger Mint-x64-CMake/vcpkg-Build/CTest/Python/Xvfb-GUI/echter Original-Firestorm-GPU-/128px-X11-Icon-Test vor Push; 100 % erst bei realen 7 E2E-Nachweisen gemäß docs/GOAL_100_PERCENT_RELEASE_ACCEPTANCE.md. Nächste zehn Blöcke Alpha.1165–1174 mit Fokus echter Skin-/Outfit-/WASD-Endbeweis und Firestorm-Funktionsparität.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); /goal OGL Viewer VOLLSTAENDIG FERTIGSTELLEN. Viewer 0.2.0-alpha.1154, Projektfortschritt ca. 39 % (38,65 % gewichtet), NAECHSTE Alpha.1155–1164.**

**ALPHA.1154 – AUTHENTIFIZIERTE AUTO-COF-WEARABLE-/GETTEXTURE-/LAD-LAYER-PIPELINE OHNE F9:** Exakte von GitHub verifizierte Feature-Code-SHA 96827c2aa9a55b193457acb4c528a001895430b9 (Mint-x64-CMake/vcpkg-Build 424/424, native 183/183 CTests, 554/554 Python und echtes isoliertes Firestorm-Original-Xvfb-OpenGL-GUI-/128px-X11-Icon-Gate PASS; Pi5 ARM64 CI UNVERIFIZIERT). Linux Mint Desktop Commander jam-MS-7D28, GitHub develop-current. Aus der zuvor nur bis original COF-LLWearable reichenden Auto-Login-Pipeline folgt jetzt auch das originale begrenzte 12-Quellen-/128x128-GetTexture-RGBA-Fetching und die lokale Firestorm-LAD-/TGA-/Masken-/Farb-Layer-Compositor-Vorschau OHNE manuelle F9-Klicks. Nur ein wirkliches vollständig gelesenes ursprünglich AUTO-authentifiziertes COF-Assetset, exakt owner-/epoch-/COF-versions-gebunden und fehlerfrei, berechtigt diese nächste Stufe; manueller F9-Retry bleibt unabhängig. Alte und ungültige GPU-/Future-/COF-Quellen fallen bei Region-/COF-Wechsel aus; null gültige RGBA-Quellen -> Fail-closed statt Fake-Skin. F3 meldet aktuelle lokale x/y GetTexture-Quellen und lokale Layer-Vorschau, KEIN Simulator-Head-/Upper-/Lower-Bake-Beweis. Zehn neue Python-Verträge, bestehende originale Nativtests und gesamter Mint-x64-Debug-Build/183 native CTests/554 Python/Xvfb-Firestorm-GPU-GUI/128px-X11-Icon-Test PASS. Neuester Servermain e63c586aec7d84761754a4cf31b300f22d602076. KEIN neuer authentifizierter Grid-Live-3/3-Skin-/Outfit-/WASD-/Yaw-Endtest; echte 100-%-Freigabe bleibt BLOCKED, kein Deploy. Details docs/ALPHA1145_1154_AUTOMATIC_COF_LOCAL_TEXTURE_COMPOSITION.md. Gesamtfortschritt bleibt ca. 39 %.

**VERBINDLICH:** NUR Linux Mint Desktop Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current (Pi5 nur ARM64-CI). Bei jedem weiteren Arbeitslauf zehn echte Entwicklungsblöcke, Schwerpunkt echter Avatar RGB-Skin/Outfit-Final-Render, Server-WASD-/Yaw-Readback und UI-Vollständigkeit. Vor Feature-Push voller Mint-CMake/vcpkg-x64-Build, 183+ native CTests, Python, echte Firestorm-Xvfb-OpenGL-GPU-/GUI-/128px-X11-Icon-Tests. Releasegate scripts/ogl-release-readiness.py bleibt EXIT 2 bis alle sieben realen E2E-Evidenzen vorliegen. Nächste zehn Blöcke Alpha.1155–1164.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE (ZEHN ECHTE ENTWICKLUNGSBLÖCKE); /goal OGL Viewer VOLLSTAENDIG FERTIGSTELLEN, nicht bei Morph-Teilerfolgen stehenbleiben. Viewer 0.2.0-alpha.1144, Projektfortschritt ca. 39 % (38,65 % gewichtet), NAECHSTE Alpha.1145–1154.**

**ALPHA.1144 – AUTOMATISCHER AUTHENTIFIZIERTER ORIGINAL-COF-DATENABRUF OHNE F9:** GitHub-verifizierte exakte Feature-SHA 3c9745a10692bc3ce3cc619a602c38cdc3dc87ba (vollständiger Linux-Mint-x64-Build, 183/183 native CTests, 544/544 Python und echter Original-Firestorm-Xvfb-GPU-/GUI-/128px-X11-Taskleistenicon-Test PASS; Pi5 ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28 Desktop Commander, GitHub develop-current. Der bislang nur mit geöffnetem Outfit-F9-Fenster mögliche authentifizierte FetchInventory2-Originalitem-/LLWearable-Assetdownload startet jetzt nach gültigem Login selbsttätig auf dem echten aktuellen COF mit vorhandenen Owner-/Epoch-/Regionlease-/COF-Version-/UUID-/Wearable-Typ-Gates, maximal einmal je exaktem Owner/Region/COF/Version mit 64-Schlüssel-Limit. Bei fehlendem initialen COF-Metadatenabruf read-only CAPS-Fallback im bestehenden 30-Sekunden-Intervall. Abgelehnte Anfragen und Regionswechsel setzen temporäre Automatikflags zurück, F9 bleibt manuelle Retry-Option. F3 zeigt Original-COF-Readback und Assetfortschritt und ausdrücklich noch fehlendes echtes Skin-/Outfit-E2E. Native Guard- und acht Python-Tests, vollstaendiger Mint-x64-Build, 183/183 native CTests, 544/544 Python und Xvfb-GUI/Original-Firestorm-GPU/128px-X11-Icon-Gate PASS; eigenes strenges Freigabe-Gate scripts/ogl-release-readiness.py (aktuell RELEASE BLOCKED) sowie konkreter 100-Prozent-Abnahmekatalog docs/GOAL_100_PERCENT_RELEASE_ACCEPTANCE.md hinzugefügt. Das ist eine echte Verbesserung der Login-Avatarpipeline, aber noch KEIN Live-Grid-3/3-RGB-Skin/Outfit/WASD-Yaw-E2E. Fortschritt 39 %, kein Produktionsdeployment. Details docs/ALPHA1135_1144_AUTOMATIC_COF_LOGIN_BOOTSTRAP.md.

**VERBINDLICH:** Arbeit ausschließlich Linux Mint Desktop Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Pi5 nur ARM64-CI. Vor GitHub-Feature-Push voller Mint-x64-CMake/vcpkg-Build/CTest/Python/Firestorm-Xvfb-GPU-/GUI-/128px-Taskleistenicon-Gate; keine Produktionsfreigabe solange Release-Readiness-Exitcode 2. Bei Fortsetzung Fokus auf wirkliches Live-Grid-Avatar-Outfit-/RGB-Skin-/WASD-Yaw-End-to-End statt weiterer bloßer Debug-Morphs. Nächste zehn Blöcke Alpha.1145–1154.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1134, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1135–1144.**

**ALPHA.1134 – ORIGINAL-FIRESTORM SHAPE-SLIDER-DRIVER-MORPHS BIS GPU:** Von GitHub verifizierte Feature-Code-SHA 50a9e497df0267fcf1fa69a214e24a232ed692cf (424/424 Mint-x64-CMake/vcpkg-Build, 183/183 native CTests, 536/536 Python, isolierter Xvfb-Firestorm-OpenGL-Framebuffer/GUI-/128px-X11-Icon-Test PASS; Pi5 ARM64 CI UNVERIFIZIERT). Linux Mint jam-MS-7D28 Desktop Commander, GitHub develop-current. Der originale Firestorm-LAD-Dynamikkatalog liest jetzt für die aktuellen authentifizierten COF-LLWearable-Shape-Regler echte param_driver -> driven-Morph-ID-Zuordnungen statt nur direktes param_morph. Beispielsweise Shape-ID2 Nose_Big_Out und ID35 Big_Ears steuern originale Firestorm-LLM-Kopf-Morph-Ziele 20002/20035; zugehörige Skelett-Ziele 30002/30035 bleiben bewusst OFFEN, ebenso nichtlineare Driver-Intervalle. Nur identische originale Boundaries, gleiche Original-LLM-Morph-Namen und endliche Shape-Werte zulässig; vorhandene Owner-/Regionlease-/COF-Version-Sperren unverändert. Alle eigenen Original-Self-Mesh-GPU-Preview-Kopien werden bei Stale-COF/Agent/Region weiterhin verworfen, fremde Avatare unverändert. GUI meldet tatsächliche DriverMorph-Treffer und 'Bone-Driver offen, lokal, kein Bake-Beweis'. Echter Xvfb-OpenGL-Framebuffer Original-Firestorm-LLM: 45811 sichtbare Head-Pixel-Unterschiede, 0 Remote-Pixel und vollständige Rücksetzung. Vollständiger Mint-x64-CMake/vcpkg-Build 424 Bauschritte, 183/183 native CTests, 536/536 Python, Xvfb-GUI/Firestorm-GPU/128px-X11-Taskleistenicon PASS. Kein voller Firestorm-Bone-/Makeup-/Outfit-/Live-Skin-/WASD-E2E-Nachweis. Projekt 39%, kein Deployment. Details docs/ALPHA1125_1134_ORIGINAL_FIRESTORM_SHAPE_DRIVER_MORPHS.md.

**VERBINDLICH:** Entwicklung NUR Linux Mint Desktop Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, GitHub Branch develop-current. Pi5 AUSSCHLIESSLICH ARM64-CI. Bei JEDEM 'weiter' genau zehn echte Blöcke, voller Mint-CMake/vcpkg-x64-Build/CTests/Python/echtes Xvfb-Firestorm-GPU-/GUI-/128px-X11-Icon-Gate vor Code-Push. Nächste Blöcke Alpha.1135–1144.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1124, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1125–1134.**

**ALPHA.1124 – ORIGINAL FIRESTORM-LAD DYNAMISCHER COF-SHAPE-KATALOG:** GitHub-verifizierte Feature-SHA fe00f149f15bfdaf2b2a2ffba8a2c75f0d69150a (Mint-x64-Debug-CMake/vcpkg-Build 424 Schritte, 183/183 native CTests, 531/531 Python, original Firestorm Xvfb-GPU-/GUI-/128px-X11-Icon-Gate PASS; Pi5 ARM64-CI UNVERIFIZIERT). Entwicklung NUR auf Linux Mint Desktop Commander jam-MS-7D28 develop-current. Fest acht Shape-IDs abgelöst durch sicheren Laufzeitkatalog der original lokal installierten avatar_lad.xml und vier LLM-MorphTarget-Listen; begrenzte Dateigröße, Original-ID/Name/Bereich und finite Parameter, kein Erraten unbekannter Low158-Bytes. Aktueller authentifizierter COF-LLWearable-Shape-Owner-/Epoch-/Folder-/Version-Filter bleibt. Zusätzlich original Fat_Torso id634, Fat_Legs id635: Original Head3/Wimpern4/Upper3/Lower3 Morphs. Eigene Morph-Mesh-GPU-KOPIEN, Remote unverändert. Echte Original-Firestorm-Framebuffer +1553 zusätzliche Körperbildpunkte, 0 Remote-/Reset-Ghost-Pixel. Vollständiger Linux-Mint-424-Schritte-CMake/vcpkg-x64-Build, 183/183 CTests, 531/531 Python, isolierter Xvfb-GUI-/GPU-/128px-X11-Systemicon-Test PASS. GUI meldet echte lokale Morph-Anzahlen ohne finalen Bake-Nachweis. Upstream OpenGenesisLINK main debdcce15d61 (World-API-Rollen, keine Avatar-Protokolländerung). Authentifiziertes NexVerse-Live-Skin/Outfit/WASD/Yaw-E2E weiterhin OFFEN. Fortschritt 39 %, kein Deployment. Details docs/ALPHA1115_1124_ORIGINAL_LAD_DYNAMIC_COF_SHAPE.md.

**VERBINDLICH:** Nur Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer develop-current, Pi5 ausschließlich ARM64-CI. Vollständiges Mint-x64-vcpkg/CMake/CTest/Python/Xvfb-Firestorm-GPU/GUI-/X11-Icon-Gate vor GitHub-Push. Nächste Alpha.1125–1134.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1114, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1115–1124.**

**ALPHA.1114 – ORIGINALE FIRESTORM COF-UPPER-/LOWER-SHAPE:** GitHub-geprüfte Feature-Code-SHA 076f5dd4f6dbd12da68d03e2bff7014ba33c63f3 (423/423 Mint-CMake-Build, 183/183 CTests, 526/526 Python, echter Xvfb-GPU-/133-Joint-GUI-/X11-Icon-Gate PASS; Pi5 ARM64 CI UNVERIFIZIERT). Linux Mint jam-MS-7D28 Desktop Commander, develop-current. Vier originale Shape-LAD-IDs 104 Big_Belly_Torso, 106 Muscular_Torso, 151 Big_Butt_Legs, 152 Muscular_Legs, alle mit verifizierten Bereichsgrenzen. Original Firestorm Upper- und Lower-LLM-Deltas aus vollständig validierten originalen COF-Shape-LLWearables nur auf eigene Mesh-KOPIEN in GPU-Slots8/9, Remote-Avatare bleiben Original-Slots1/2. Head6/Lashes7 weiter vorhanden. Alle vier Vorschau-Meshes bei Agent-/COF-/Regionswechsel, fehlender Shape oder Teil-Uploadfehler entfernen. F3 zeigt echte GL-Body-Morph-Draws, ausdrücklich kein Live-Bake-/Skin-Beweis. Native original Firestorm-Xvfb-Framebuffer: 833 veränderte eigene Körperpixel, je 1 Upper/Lower-GL-Draw, 0 Remote-Pixel und 0 Reset-Ghost-Pixel. Vollständiger Mint-x64-CMake/vcpkg-Build aller 423 Ziele, 183/183 native CTests, 526/526 Python und isolierter Xvfb-GUI-/Original-GPU-/128px-X11-Taskleistenicon-Test PASS. Live-Grid-COF/Outfit-3/3-Skin/RGB und serverbestätigte Bewegung still open. Projekt 39 %, kein Produktivdeployment. Details docs/ALPHA1105_1114_FIRESTORM_ORIGINAL_BODY_SHAPE.md.

**VERBINDLICH:** Weiterhin NUR Linux Mint Desktop Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Pi5 nur ARM64-CI. Vollständige Mint-x64-CMake/vcpkg-Build/CTest/Python/Xvfb-GUI/Taskleistenicon-Abnahme vor GitHub-Push. Nächste zehn Blöcke Alpha.1115–1124.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.1104, Fortschritt 39 % (38,65 % gewichtet); NÄCHSTE Alpha.1105–1114.**

**ALPHA.1104 ORIGINAL FIRESTORM COF COMPOUND HEAD/WIMPERN-SHAPE:** GitHub-verifizierte Code-SHA 02bba7b4174beaceafe23f7f117a7404871610f3 (voller Mint-x64-Build, 183/183 CTests, 521/521 Python, Firestorm-GPU-Framebuffer und isoliertes Xvfb-GUI/128px-X11-Icon PASS; Pi5 ARM64 CI UNVERIFIZIERT). Linux Mint jam-MS-7D28 Desktop Commander, GitHub develop-current. Vier geprüfte Firestorm-Shape-IDs 770 Elongate_Head [-1,1], 186 Egg_Head [-1.3,1], 686 Head_Eyes_Big [-2,2], 518 Eyelashes_Long [-.3,1.5]. Authentifizierter aktueller vollständig abgerufener Original-COF-Shape-LWWearable-Parameterpfad, drei echte Head-Morphs und vier original Wimpern-Morphs (LLM-COPY) auf SEPARATEN eigenen GPU-Slots6/7; fremde Avatare Head0/Wimpern5 unverändert. Ungültige/fehlende/doppelte Shapes und Uploadfehler räumen vorherige COF-Preview-Meshes sicher ab. F3 eigene GL-Vorschau-Kennzeichnung; nicht echter Skin-/COF-Bake-/WASD-Endnachweis. Echter Firestorm-Xvfb-Framebuffer: Head 46432 Unterschiedspixel gegenüber Einzelmorph, Wimpern 3698, Remote 0; Original nach Reset vollständig restauriert. Vollständiger Linux-Mint-x64-Build PASS; 183/183 native CTests, 521/521 Python und isolierter Xvfb-GUI-/GPU-/128px-X11-Taskleistenicon-Test PASS. Live-Grid-Avatar/Outfit und serverbestätigte Bewegung bleiben offen. Details docs/ALPHA1095_1104_FIRESTORM_COMPOUND_COF_SHAPE.md. Projekt 39 %, kein Deployment.

**VERBINDLICH:** Arbeit NUR Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer develop-current. Pi5 nur ARM64-CI. Immer vollständiges Mint-CMake/vcpkg-x64-Viewer-, 183+ native CTests-, Python-/Xvfb-GUI-/Systemicon-Gate vor Push. Nächste zehn Blöcke Alpha.1105–1114.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1094, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1095–1104.**

**ALPHA.1094 – ECHTE FIRESTORM-MORPHS: 95 HEAD, 30 UPPER, 24 LOWER, 34 EYELASH, AUTHENTIFIZIERTE COF-SELF-SHAPE-VORSCHAU:** GitHub-verifizierte exakte Feature-Code-SHA 442c69e9b7c50c4065aee019add59a0b844195e3 (Linux-Mint-x64-Vollbuild PASS, 183/183 CTests PASS, 516/516 Python PASS, echtes Firestorm-Morph-Framebuffer-Test 45582 Self-Pixel/0 Remote-Pixel und Xvfb-GUI/X11-Icon PASS; Pi5 ARM64-CI UNVERIFIZIERT). Entwicklung ausschließlich Mint jam-MS-7D28 Desktop Commander auf develop-current. Der originale sichere Firestorm-Linden-Binary-LLM-Decoder liest nun alle begrenzten MorphTarget-Sparse-Deltas, Remaps und den End-Morphs-Marker, statt sie zu ignorieren. CPU-Morph verändert eine Kopie der Originalmeshes (Elongate_Head 708 Head-Vertices) ohne Skin-/UV-Index-Ausfall; beschädigte Dateien/Morph-Namen/-Werte werden abgelehnt. Der Renderer hat einen SEPARATEN Self-only Head-Morph-GL-Meshslot 6. Ein fremder Avatar behält weiter originalen Firestorm Head-Slot0. Wenn der eingeloggte Nutzer ausdrücklich alle Original-LLWearables der gültigen COF-Version abruft, wird für exakt ein authentifiziertes Shape-Asset Typ0 und original Firestorm-LAD-VisualParam 770 Elongate_Head im erlaubten Bereich -1..1 der morphte eigene Kopf lokal auf GPU angezeigt. Region/Owner/Login-/COF-Wechsel verwirft die Vorschau automatisch. Keine ungesicherte Abbildung von unbeschrifteten Low158-VisualParams. Eigenständiger F3-Head-Shape-GL-Zähler (nur Vorschau, kein COF-Bake-Erfolg). Echter Original-Firestorm-Xvfb-Framebuffer: 45582 geänderte Self-Head-Pixel bei Testweight1, 0 geänderte Remote-Avatar-Pixel, Original nach Reset exakt restauriert. Linux-Mint-x64-Debug-Vollbuild, 183/183 CTests, 516/516 Python und isoliertes Xvfb-GUI-/GPU-/X11-Taskleistenicon-Gate PASS. Gesamte Firestorm-Morph-Driver-/Outfit-Parität sowie authentifiziertes Live-3/3-Skin/COF und WASD/Yaw-E2E noch OFFEN. Projektstatus 39 %, kein Produktivdeploy. Details docs/ALPHA1085_1094_REAL_FIRESTORM_COF_SHAPE_MORPH.md.

**VERBINDLICH:** Arbeit ausschließlich auf Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current; Pi5 nur ARM64-CI, kein Deployment. Vor Push komplettes Mint-CMake-x64/183+ CTests/Python/Xvfb-GUI/128px-Systemicon-Gate. Nächste zehn Blöcke Alpha.1095–1104.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1084, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1085–1094.**

**ALPHA.1084 – ORIGINALE FIRESTORM-WIMPERN-ALPHAMASKE STATT SYNTHETISCHER VISIBILITY:** Exakte GitHub-bestätigte Feature-Code-SHA d9868e215bf6d41a8046bc4d66539032fc869f60 (vollständiger Mint-x64-Debug-CMake-Build, 183/183 CTests, 511/511 Python, realer Firestorm-RLE-TGA-/GPU-Framebuffer, Xvfb-Original-GPU-/X11-Icon-Test PASS; Pi5 ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28 Desktop Commander, GitHub develop-current. Firestorm avatar_lad.xml Layer 'eyelash alpha' referenziert original head_alpha.tga (sicher dekodierte lokale 512x512 RLE-Graustufen-TGA). Maskierung wird jetzt als separate begrenzte und transaktional hochgeladene GL-Textur NUR beim bisherigen animierten originalen avatar_eyelashes.llm-Shader angewendet, zusätzlich zur echten aktuellen Head-Bake. Shader-Reset nach Wimpern und vor anderem Szenenrender verhindert Materiallecks. 0-Wert-Testmaske: 0 zusätzliche Lash-Pixel, Head bleibt sichtbar; vollständiger Mint-x64-Build mit 183/183 CTests, 511/511 Python und isoliertem Xvfb-Original-Firestorm-GPU-/X11-Icon-Test PASS; ungültige Maske erhält gültigen vorherigen GPU-Zustand; original Firestorm head_alpha.tga: 1062 sichtbare Wimpernpixel, 1 realer Lash-/Masken-GL-Draw. F3 eigener lokaler Wimpern-TGA-GL-Zähler, niemals fälschlicher COF-/Skin-Nachweis. Kein zusätzlicher HTTP, keine Assets ins Git kopiert, kein Produktivdeploy. Volle Makeup-Composite-/Shape-Morph-/Outfit-Pixel- und WASD-Grid-E2E weiter offen; Projektfortschritt 39 %. Details docs/ALPHA1075_1084_FIRESTORM_ORIGINAL_HEAD_ALPHA_TGA_GPU_MASK.md.

**VERBINDLICH:** Arbeiten ausschließlich auf Linux Mint jam-MS-7D28 Desktop Commander, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current. Pi5 ausschließlich ARM64-CI, keine Desktop-Entwicklung dort. Komplettes Mint-x64-Build/CTest/Python/Xvfb-Icon-Gate vor Push; nächste 10 echte Blöcke Alpha.1085–1094.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1074, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1075–1084.**

**ALPHA.1074 – ORIGINALE FIRESTORM-WIMPERN IN ECHTER HEAD-BAKE-GPU-PIPELINE:** GitHub-verifizierte exakte Feature-Code-SHA 9127f42e994a3b3a7397cf6f669b79fe88eb7678 (voller Linux-Mint-x64-Debug-Build, 183/183 CTests, 506/506 Python, originaler Firestorm-Framebuffer 1062 Wimpernpixel/1 Wimpern-Draw, Xvfb-GUI-/GPU-/128px-X11-Icon PASS; Pi5 ARM64 CI UNVERIFIZIERT). Linux Mint Desktop Commander jam-MS-7D28, Branch develop-current. Original gewichtiges avatar_eyelashes.llm in optional sechsten Meshslot geladen und mit echter rigged Joint-Pose auf GPU gezeichnet. NUR wenn aktuell ausgewählte Head-Bake TE existiert und exakt diese Textur im GL-Cache ist; keine erfundene Makeup-/Lashes-UUID, kein Zusatz-HTTP und kein Bypass. Head-Bake-Alpha-Cutout und dunkle Farbe (vollständige head_alpha.tga- und Shape-Morph-Komposition noch offen). F3 zeigt getrennte tatsächliche Wimpern-Head-GL-Draws ohne UUID; Head/Upper/Lower 3/3 anatomische GPU-Beweise bleiben unabhängig. Echter Xvfb-Original-Firestorm-Framebuffer zeigt 1062 zusätzliche Wimpernpixel und 1 GL-Draw, 0 Lash-Draws nach Head-TE-Rücknahme und GPU-Eviction. Vollständiger Mint-x64-Debug-Viewer-Build, 183/183 CTests, 506/506 Python, isolierter Xvfb-GUI-/GPU-/X11-Icon-Test PASS. Kein Live-Login/Outfit-/Skin-/WASD-E2E daraus ableitbar. OpenGenesisLINK upstream main 99341d475b10 (keine Avatar-Wire-Änderung). Fortschritt 39 %, kein Deploy. Details docs/ALPHA1065_1074_ORIGINAL_FIRESTORM_EYELASH_HEAD_GATE.md.

**VERBINDLICH:** Ausschließlich Linux Mint Desktop Commander jam-MS-7D28 und /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Pi5 NUR ARM64-CI. Kompletten Mint-x64-Debug-CMake/C++/Python/Xvfb-GUI/Systemicon-Gate vor GitHub-Push ausführen. Nächste zehn Blöcke Alpha.1075–1084.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1064, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1065–1074.**

**ALPHA.1064 – ORIGINALE BEIDE FIRESTORM-AUGEN ENDLICH IN DER REALEN GPU-RENDERPIPELINE:** Exakte GitHub-verifizierte Feature-Code-SHA db0c6359c5b24ee17199a59aec4a0e03d8c17d14 (vollständiger Linux-Mint-x64-Build, 183/183 CTests, 502/502 Python, Original-Firestorm-Augapfel-Framebuffer 1106+553 Pixel, 0 Ghost-Pixel, Xvfb-GUI/GPU/128px-X11-Icon PASS; Pi5 ARM64-CI UNVERIFIZIERT). Entwicklung Linux Mint Desktop Commander jam-MS-7D28 (nicht Pi), GitHub develop-current. Native avatar_eye.llm als optional fünfter GL-GPU-Meshslot, original linkes/rechtes Firestorm-Joint mEyeLeft/mEyeRight jeweils in aktueller animierter Frame-Pose. Augen nur mit tatsächlich von Self Low158 EYES-TE Face11 (oder Remote-AvatarAppearance) übermittelter GPU-residenter Bake-ID. GetTexture via GENAU EINMALIGE Frame-Admission, priorisiert nach 3 Bodybakes vor Hair mit vollem bisherigen Budget/Retry/Epoch. F3 getrennter Eye-GL-Zähler ohne UUID/ohne falschen 3/3-Body-COF-Beweis. Vollständiger Mint-x64-Debug-Viewer-Build und 183/183 CTests, 502/502 Python sowie isolierter Xvfb-GUI-/Original-GPU-/128px-X11-Icon-Release-Gate PASS. Echter Xvfb-GPU-/Framebuffer: 1106 sichtbare Eyes-Pixel bei 2 originalen Augen, 553 Pixel Unterschied zwischen zwei/ einem Gelenk, 0 Ghost-Pixel nach TE-Rücknahme und GPU-Eviction. Der Test nutzt echte Originalmeshes, aber künstliche Farbe; kein neuer authentifizierter NexVerse-Live-Nachweis. Upstream Servermain neu 99341d475b10 (personal World-API-Berechtigungen, keine Eye-Wire-Änderung). Wimpern/Outfit/COF/Live-Skin-Pixel/WASD still open, Fortschritt 39 %. Details docs/ALPHA1055_1064_FIRESTORM_ORIGINAL_DUAL_EYES.md. Kein Deploy.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current; Pi5 ausschließlich ARM64-CI. Vollständiger Mint-x64-CMake/183 CTests/Python/Xvfb-Systemicon-Gate vor GitHub-Push. Nächste 10 Blöcke Alpha.1065–1074.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1054, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1055–1064.**

**ALPHA.1054 – ORIGINALES FIRESTORM SYSTEMHAAR WIRD SERVER-BASIERT GERENDERT:** Exakte GitHub-Code-SHA 660963c15c95c9ad5b25e7e9e8cb3a175a11c536 (Linux-Mint-x64-Vollbuild, 183/183 CTests, 498/498 Python, echter originaler Hair-Framebuffer 232 Pixel/0 Ghost-Pixel, Xvfb-GUI/Firestorm-GPU/X11-Icon PASS; Pi5 ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28, GitHub develop-current. In den lokal vorhandenen Original Firestorm Nightly-Character-Dateien ist avatar_hair.llm, bereits vom avatar_lad.xml als hairMesh definiert. Der Viewer lädt das optional als vierten GPU-Originalmesh-Slot; die drei Head/Upper/Lower-GL-Skin-Nachweise bleiben unverändert. Authentifiziertes Self Low158-TE Face 5 und zulässige Remote-AvatarAppearance Hair-Referenz werden über exakt dieselbe Frame-Pose-Admission gestreamt, MIT niedrigeren Prioritäten als echte Bodybakes, normalem VRAM-/Retry- und Cap-Limit. Render erst mit serverbestätigtem GPU-residentem Hair-Asset; Frame-GPU-GL-Zähler in F3 ohne UUID. Echter Xvfb-Framebuffer-Test: 232 nachgewiesene originale Hair-Pixel, 0 Hair-Ghost-Pixel nach TE-Rücknahme und Asset-Eviction, 3 anatomische Bodymeshes unverändert. Augen/Wimpern/Mesh-Hair-Attachments noch nicht integriert; neuer authentifizierter Grid-Hair-/Outfit-/Skin-/WASD-E2E still open. Vollständiger Mint-x64-Build/183 von 183 CTests/498 von 498 Python und isolierter Firestorm-GUI-/GPU-/128px-Taskleistenicon-Test PASS. Projektfortschritt 39 %, kein Produktivdeploy. Details docs/ALPHA1045_1054_FIRESTORM_ORIGINAL_HAIR_BAKE.md.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current; Pi5 nur ARM64-CI. Vollständiges Linux-Mint-x64-GUI-Gate vor GitHub-Push. Nächste zehn Blöcke Alpha.1055–1064.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1044, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1045–1054.**

**ALPHA.1044 – GPU-SICHERE MIP-RETTUNG FÜR ECHTE SELF-AVATAR-BODY-BAKES:** Exakte GitHub-Code-SHA a9610722ca5906f4709569cfe51a764a5cdfc52d (Mint-x64-Vollbuild, 183/183 CTests, 494/494 Python, Firestorm-Original-GPU-/Xvfb-GUI-/128px-X11-Icon-Test PASS; Pi5 ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28, develop-current. Avatar Head/Upper/Lower waren bisher immer auf Mip0 fixiert; falls dessen temporäres GPU-Upload-Staging-/Texturbudget nicht passt, konnte die eigentlich vorhandene Skin-Textur auf Dauer verschoben bleiben. Neue rein lokale GPU-Mip-Rettung: nur exakt aktuelle, eingeloggte, GPU-admitted Self-Body-IDs, zuerst strikte Original-Staging/Byteprüfung; bei deren Fehlschlag Mip1–3 mit denselben Budgets und Slot-Prüfungen. Bereits dekodiertes RGBA wird verkleinert, KEIN zusätzlicher GetTexture-HTTP, KEIN Bypass, KEIN Backoff-Reset. Erst nach GL-Upload-Erfolg wird der neue GPU-/Wunsch-Mip gespeichert; 60-Sekunden-Floor verhindert 750ms-Mip0-Schleife, Regionwechsel setzt alles zurück. F3 zeigt Zahl geretteter tatsächlicher Skin-GPU-Uploads. Native Budget-/Epoch-Tests, vier Python-App-Verträge und korrekte EINMALIGE GPU-Avatar-Admission pro Frame; vollständiger Mint-x64-Build, 183/183 CTests, 494/494 Python, isolierter Xvfb-GUI-/Firestorm-GPU-/128px-X11-Icon-Test PASS. Original Firestorm bleibt Basis, Servermain f56e354 unverändert. Ein vollständiger echter 3/3-Server-Skin/COF-Pixel- und frischer WASD/Yaw-E2E bleibt still open, Projekt 39 %. Details docs/ALPHA1035_1044_SELF_BODY_SAFE_GPU_MIP_RESCUE.md. Kein Deploy.

**VERBINDLICH:** Entwicklung nur Linux Mint Desktop Commander jam-MS-7D28 /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 ausschließlich CI-ARM64, kompletter Mint-x64-GUI-Gate vor Push; nächste zehn Blöcke Alpha.1045–1054.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1034, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1035–1044.**

**ALPHA.1034 – AVATAR H/U/L FEHLERDIAGNOSE DIREKT AN REALE GPU-RENDER-QUELLE GEBUNDEN:** Exakte GitHub-Feature-SHA 9f462ef264d30d6a4c71c5cb1bce146089014356 (vollständiger Mint-x64-Build, 183/183 CTests, 490/490 Python, Xvfb-GUI-/Firestorm-GPU-/128px-X11-Icon PASS; Pi5-ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28, develop-current. F3 bisher nur Low158 GetTexture, während aktueller Firestorm-Renderer bei Low158-Teillücken pro Slot provisorische ObjectUpdate-Bakes zeichnet. Jetzt WorldModel::self_bake_selection() für GPU-admitted Self-Frame, aktive Agent/Session/Region und tatsächliche Quellflags ausgewertet: H:L/... U:O/... L:- mit L=bestätigtes Low158 und O=vorläufige ObjTE, keine unzulässige COF-/Skin-Bestätigung. HTTP-404-/GPU-/Retry-Zuordnung nutzt nur exakt selektierte Body-Assets, keine alten oder fremden Szenen-UUIDs. Strg+S-Report sechs reine numerische Status-/Quellenfelder, bisheriger Low158-Beweis separat. Native/Realpath-Tests und vollständiger Mint-x64-Build, 183/183 CTests, 490/490 Python und isolierter Xvfb-Start, originales Firestorm-GPU-Skelett und echtes X11-Taskleistenicon PASS. Servermain f56e354 unverändert. Echter 3/3-Live-Skin-/Outfit-Pixelbeweis und frischer WASD/Yaw-E2E still open; Fortschritt 39 %, kein Produktivdeploy. Details docs/ALPHA1025_1034_REAL_RENDER_BAKE_PROVENANCE.md.

**VERBINDLICH:** Nur Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 nur für ARM64 CI, vollständiger Mint-x64-GUI-Test vor GitHub-Push; nächste zehn Blöcke Alpha.1035–1044.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1024, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1025–1034.**

**ALPHA.1024 – HTTP-/GPU-FEHLER PRO AKTUELLEM SELF-BODY-BAKE KORRELIERT:** Exakte GitHub-Code-SHA 6b30ba400ecda86825b9ec1b5a51501c653e90e9 (Mint PASS: 183/183 CTests, 485/485 Python, Xvfb-Firestorm-GPU und 128x128-X11-Icon; Pi5-ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28, GitHub develop-current. Historische reale Self-Low158 vom 21.10.2026 und gleichzeitige allgemeine GetTexture-404 belegen noch keinen Body-404. Neue read-only AssetRetrySchedule::failure_kind()-Abfrage und inspect_self_bake_gettexture() verknüpfen ausschließlich aktuelle bestätigte Head-/Upper-/Lower-IDs der authentifizierten Agent-/Session-/Regionsepoch mit GPU-Residency und Fehler/Retry-Status. Fremde Szene-404 werden nicht zum Avatarfehler. F3: 'Body-GetTexture H:... U:... L:...' vor weiterem Text; privater Strg+S-Bericht: drei reine numerische Fehlercodes. Keine neue Netzwerk-Anforderung, kein Backoff-Reset, kein eigener Upload, neu protokollierte GetTexture-Fehler ohne rohe Asset-UUID. Native Regressionen und vier neue Python-Verträge, Server-/Matrizenabgleich; vollständiger Mint-x64-Build/183 von 183 CTests/485 von 485 Python/Xvfb-Firestorm-GPU-/128px-Taskleistenicon-Gate PASS. Ein neuer echter Live-Lauf zur tatsächlichen Ursache und 3/3-Skin/Outfit-Pixel-Abnahme ist weiterhin erforderlich; serverbestätigter WASD-/Yaw-E2E für neue SHA still open. Projekt 39 %. Details docs/ALPHA1015_1024_CURRENT_SELF_BAKE_HTTP_CORRELATION.md. Kein Deploy.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 nur ARM64-CI, voller Mint-x64-GUI-Test vor GitHub-Push, nächste zehn Blöcke Alpha.1025–1034.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1014, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1015–1024.**

**ALPHA.1014 – ECHTER SELF-LOW158 LIVE-EMPFANG BESTÄTIGT; SKIN-BLOCKER GETRENNT:** Exakt GitHub Code-SHA c61777510546c00ebcfcfbe0a7696631828ab242 (Mint PASS: 183/183 CTests, 481/481 Python, isolierte GPU-/X11-GUI; Pi5 ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28, develop-current. Historischer Viewer-Log: 21.10.2026 17:57:19 authentifiziertes Self AvatarAppearance Low158 mit 64-Byte TextureEntry und 253 VisualParams angenommen; originales Firestorm-GPU-Skin mit Serveranimation gesteuert und 5 Legacy-Wearables gemeldet. Die vorige pauschale Aussage 'Low158 fehlt' war falsch. Zugleich einige GetTexture HTTP 404 ohne nachgewiesenen Bezug zu Body-Bakes. Neue F3- und Strg+S-Diagnose unterscheidet echte fehlende Low158, Serverbakes 0/3 oder teilweise, ObjektTE-Provisorium, GPU-Residency, GL-Draws und Depth-Sichtbarkeit; bei erster akzeptierter Low158 Bodyref-Zahl 0..3 ohne UUID loggen. Native Report-/Privacy-State-Matrix und Python-Realpath-Contracts; Mint-x64-Vollbuild, 183/183 CTests, 481/481 Python, isolierter Xvfb-Firestorm-GPU- und X11-Taskleistenicon-Test PASS. Servermain f56e354 unverändert. Vollständiger aktueller Skin-/Outfit-Pixelbeweis und serverbestätigter WASD-/Yaw-E2E still open. Fortschritt 39 %. Siehe docs/ALPHA1005_1014_LIVE_LOW158_SKIN_BLOCKER_DIAGNOSIS.md. Kein Deploy.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current; Pi5 nur ARM64 CI. Voller Mint-x64-GUI-Gate vor Feature-Push. Nächste zehn Blöcke Alpha.1015–1024.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.1004, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.1005–1014.**

**ALPHA.1004 – SPÄTE AVATAR-PAKETE NACH KILLOBJECT GESPERRT:** Exakte auf GitHub bestätigte Code-SHA 7d7bc1889ad21d11795766dee2b4feb4bf35dc9b (Mint-Gate PASS: 183/183 CTests, 477/477 Python, Xvfb-GUI-/X11-Icon; Pi5-ARM64-CI UNVERIFIZIERT). Linux Mint jam-MS-7D28, develop-current. Konkreter Fehler: Authentifizierter Self-KillObject setzte zwar die alte LocalID/Terse-/Pose-Bindung zurück, doch verspätete Full/Compressed ObjectUpdate konnten die getötete numerische LocalID und veraltete Self-Pose/WASD-Evidenz vor erneuter legitimer Avatar-Initialisierung rekonstruieren. Session-/Region-/Agent-gebundene begrenzte FIFO 16 KillObject-Fingerprints + Vorprüfung in beiden realen Packet-Handlern implementiert. Exaktes altes TE, leere/ungültige TE ohne neuen Kandidaten sperren; gültige tatsächlich neue Body-TE darf denselben LocalID-Wert wieder anbinden, ohne alte exakte TE wieder freizugeben. Mehrfache Kills erhalten historische COF-Fingerprints. F3 mit KillObject-Sperr- und Replay-Zählern ohne UUIDs. Native Kill-/Rebind-/Multi-Replay-/FIFO-/Reset-Tests und Python Realhandler-Contracts sowie vollständiger Mint-x64-Build, 183/183 CTests, 477/477 Python, isolierter Xvfb-Firestorm-Skelett/GPU- und X11-Taskleistenicon-Gate PASS. OpenGenesisLINK Servermain f56e354694a41adb2334884c21efc45db784cb92 unverändert. Echter NexVerse-Grid-Live-Avatar/COF/Outfit/Skin-GPU und serverbestätigte WASD/Yaw-E2E still open, Pi5 ARM64-Nachweis separat prüfen. Fortschritt bleibt 39 %. Details docs/ALPHA995_1004_KILLED_SELF_POSE_TOMBSTONES.md. Kein Produktivdeploy.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 nur ARM64-CI, voller Mint-x64-GUI-Test vor Push, nächste 10 Blöcke Alpha.1005–1014.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.994, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.995–1004.**

**ALPHA.994 – VERALTETE AVATAR-POSE VOR LOCALID-/WASD-BINDUNG ABGEWIESEN:** Exakte GitHub-Code-SHA c12920ff5cc737969dd3f651d7db8f698227f238 (Mint 183/183 CTests, 474/474 Python, isolierte GUI/Taskleistenicon PASS; Pi5-ARM64-CI NOCH UNVERIFIZIERT). Linux Mint jam-MS-7D28, Branch develop-current. Der alte ObjectUpdate-Body-TE-Replay-Schutz griff in der Weltmodell-Texturpipeline erst NACH dem in application.cpp vorgenommenen eigenen LocalID-/Pose-Rebind; ein authentifiziertes verspätetes Paket aus einem alten COF-Outfit konnte deswegen WASD-/Yaw-Bestätigungen trotz später verworfener Skin-TE verändern. Neue WorldModel-Eingangssperre vor dem Rebind in Full ObjectUpdate und ObjectUpdateCompressed: identische alte LocalID+Bake-TE sowie alte LocalID ohne belegte frische TE dürfen keine aktuelle Self-Pose übernehmen; tatsächlich neue gültige TE bleibt erlaubt. Session-/Region-/Reset-gebundener gesättigter F3-Zähler 'Alte Avatar-Pose vor WASD verworfen', ohne UUIDs. Nativer C++-Ingress-/Replay-/COF-/Neuanmeldetest sowie Python Realhandler-Ordnungsverträge. Servermain f56e354694a41adb2334884c21efc45db784cb92 unverändert, kein neues Avatar-Wire-Format. Live-NexVerse Avatar/COF/Outfit/Skin-GPU und Server-WASD/Yaw-E2E weiterhin offen; Projekt 39 %. Voller Mint-x64-CMake/vcpkg-Viewer-Build und 183/183 CTests, 474/474 Python, isolierte Xvfb-GUI samt Original-Firestorm-Meshes und X11-Taskleistenicon PASS. Code-SHA/Pi5-CI in docs/ALPHA985_994_RETIRED_SELF_POSE_INGRESS_GUARD.md. Kein Produktivdeploy.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 ausschließlich ARM64-CI; vollständiger Mint-x64-Viewer-/Xvfb-Gate vor Feature-Push. Nächste 10 Entwicklungsblöcke Alpha.995–1004.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.984, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.985–994.**

**ALPHA.984 – MULTI-COF-OLD-TE-FIX:** Exakt verifizierter GitHub-Code-Commit 8a6faf4b4baee3d267f1bf23d5f3576d82d9c80b (Mint-x64-Gate PASS; Pi5 ARM64 noch UNVERIFIZIERT). Linux Mint jam-MS-7D28, Branch develop-current. Mehrere zurückgezogene eigene Avatar-ObjectUpdate Body-Bake-Quellen (LocalID plus Head/Upper/Lower) werden nun in einer begrenzten FIFO von acht Einträgen geschützt; Duplikate zusammengeführt. Alte TE werden vor GPU-/HTTP-/Renderplanung verworfen, auch nach mehreren COF-Wechseln. Passendes KillObject entfernt nur passende alte Einträge; Region/Session/Login setzt zurück. F3 zeigt die Anzahl aktiver Outfit-TE-Sperren ohne UUIDs. Native C++-Tests für COF-Kette, Replay, FIFO-Grenze, Kill und Neuanmeldung sowie Python-Contracts. OpenGenesisLINK Servermain f56e354694a41adb2334884c21efc45db784cb92 (#287 nur World-API-Key-Scopes). Authentifizierter Live-Avatar-/COF-/Outfit-/Skin-/WASD-E2E bleibt offen. Mint-x64 CMake/vcpkg, 183/183 CTests, 470/470 Python, isolierte Xvfb GUI/Original-Firestorm-Meshes/X11-Taskleistenicon PASS. Details und exakte CI-SHA siehe docs/ALPHA975_984_MULTI_COF_REPLAY_HISTORY.md. Kein Deploy.

**VERBINDLICH:** Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 nur ARM64-CI. Mint-x64-Build/Xvfb-Gate vor Push. Projekt 39 %, nächste 10 Blöcke Alpha.985–994.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.974, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.975–984.**

**ALPHA.974 EXAKTE CODE-SHA / MINT-GATE / CI:** Code 4798bff70bb86abed265229b6f31c255ab00d355 vollständig auf Linux Mint jam-MS-7D28 getestet und auf GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: vollständiger Linux-x64-CMake/vcpkg-Debug-Viewer 0.2.0-alpha.974, 183/183 native CTests, 467/467 Python, isolierter Xvfb-GUI-Start mit Original-Firestorm-Skelett 133 Joints, 3 GPU-LLM-Bodymeshes/7186 Dreiecke und OGL X11 WM_CLASS/Systemicon 128x128 PASS. Pi5 ARM64 Source Check zur exakten Code-SHA 4798bff70bb86abed265229b6f31c255ab00d355 Run 40012744294 zuletzt IN_PROGRESS (NICHT als Erfolg behaupten). Hosted GitHub Linux x64 Run 40012744160 COMPLETED/FAILURE Job 120616981110 runner_id=0 steps=0, kein Runner/Compiler gestartet. Voriger exakt grüner Pi5: Alpha.964 Code-SHA 7bf7308 Run 40011135542 SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER letzter Self-Object-Bake-Replay-Fehler nach COF-Outfitwechsel behoben: identische alte Avatar-ObjectUpdate TE mit altem LocalID konnte nach bereits erfolgtem alten Bake-Widerruf später per UDP nochmals eintreffen und Skin wiederherstellen oder aktuelle neuere lokale Körpertexturen verwerfen. Jetzt eine alte Body-TE-/LocalID-Provenienz nur als Ablehnungs-Fingerprint gebunden an aktuelles Login/Region, keinerlei alte TE im GPU-/HTTP-Plan; wiederholte alte Pakete werden auch nach gültiger neuer TE/LocalID unterdrückt, echte neue Body-TE und original Low158 sind weiterhin erlaubt. F3 separat Alte Objekt-Bakes erneut verworfen N ohne UUID-Daten. Native Mint/Pi5 WorldModel-Test mit COF100→101, mehrfachen alten Paketen, frischem LocalID, geänderter eigener TE, erneuter alter TE und Login-Reset; Python-Realpath-Contracts. Authentifizierter NexVerse Live-Avatar-/COF-/Skin-/Outfit-/WASD-E2E weiterhin still open. Projekt 39 %, NÄCHSTE zehn Blöcke Alpha.975–984, Linux Mint primär, Pi5 CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Pi5 NUR ARM64 CI, voller Mint-x64-GUI-Gate vor Feature-Push, kein Produktivdeploy.**

23.10.2026 Alpha.965–974: Pi5 Alpha.964 exakte Code-SHA 7bf7308 Run 40011135542 COMPLETED/SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTEN COF-/ObjectUpdate-TE-Replay-Race behoben: Nach Rückzug provisorischer Skin-Bakes beim neuen Low158 COF konnte dieselbe alte ObjectUpdate-TE später erneut empfangen werden und den alten Skin wiederherstellen oder aktuelle eigene LocalID-Bakes verdrängen. Ein einzelner rückgezogener Session-/Regions-gebundener LocalID + Head/Upper/Lower-Fingerprint sperrt nun exakt solche späten identischen TE-Pakete. Sperre bleibt auch NACH erfolgreicher aktueller neuer TE/LocalID bestehen, sodass wiederholte alte Retransmits nicht reaktiviert werden. Verschiedene gültige neue TE und aktuelle neue LocalID dürfen weiterhin fehlende Bakes ergänzen, Server-Low158 ist autoritativ; kein Outfit-/COF-Schreibpaket. Pro Region/Session begrenzter Zähler, Reset bei Login/Region/Kill, F3 Alte Objekt-Bakes erneut verworfen N getrennt von echter Skin-GPU. Native Mint/Pi5 WorldModel-Bake-Test, Python-Source-Gate, voller Linux-Mint-x64-GUI-Test. Echter NexVerse Live-Avatar-/COF-/Outfit-/Skin-/WASD-Nachweis still open. Projekt 39 %, nächste 10 Blöcke Alpha.975–984. Details docs/ALPHA965_974_LATE_OBJECT_BAKE_COF_REPLAY_GUARD.md. Kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.964, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.965–974.**

**ALPHA.964 EXAKTE FEATURE-SHA / MINT / CI:** Code 7bf730812ed149f358a84b12ebe8d24ee6cba14a vollständig auf Linux Mint jam-MS-7D28 geprüft und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: voller CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.964, 183/183 native CTests, 462/462 Python, isoliertes Xvfb-GUI mit originalem Firestorm-Skelett 133 Gelenke, drei GPU-LLM-Körpermeshes/7186 Dreiecke, OGL X11 WM_CLASS/Icon 128x128 PASS. Pi5 ARM64 Source Check für exakt diese Code-SHA Run 40011135542 zuletzt IN_PROGRESS, noch keinen Pi5-Erfolg behaupten. Hosted GitHub x64 Run 40011135336 COMPLETED/FAILURE, Job 120611503544 runner_id=0 steps=0, kein Runner und kein Compiler gestartet. Letzter SHA-genau erfolgreicher Pi5: Alpha.954 SHA c86977f Run 40008385875 SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTEN Avatar-COF-/Körpertextur-Quellenfehler behoben: frische bestätigte Low158-Outfitversion konnte bisher alte vorläufige ObjectUpdate TE Head/Upper/Lower-Faces der vorherigen Kleidung mit neuen Low158-Faces mischen. Jetzt alte Object-TE nur bei akzeptiertem COF-/AppearanceVersion Wechsel verwerfen, Low158 bleibt autoritativ. Vorheriger gültiger COF-Highwater berücksichtigt auch bei Legacy count=0 Zwischenpaketen; identisches, ungültiges oder verworfenes COF sowie erster moderner Login behalten legitimen Fallback. Neue echte Self ObjectUpdate TE darf danach fehlende Bakes wieder auffüllen. F3 COF-Wechsel: alte Objekt-Bakes verworfen-Zähler pro Login/Region, ohne UUIDs oder vorgetäuschte Skin-GPU-Pixel. Native Mint/Pi5 Tests COF100→101/Legacy/Rollback/Reauth und Python Produktionsverträge. Echter authentifizierter NexVerse Avatar-/COF-/Outfit-/Skin-/WASD-Live-E2E bleibt still open. Gesamtfortschritt 39 %, NÄCHSTE 10 Blöcke Alpha.965–974. Linux Mint primär, Pi5 nur CI; kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi5 NUR ARM64 CI. Voller Mint-x64-GUI-Gate vor GitHub-Push, kein Produktivdeploy.**

23.10.2026 Alpha.955–964: Pi5 Alpha.954 Feature-SHA c86977f Run 40008385875 COMPLETED/SUCCESS, OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER Skin-/Outfit-Quellenfehler: Low158 Body-TE ist autoritativ, fehlende Bake-Slots dürfen vorläufig aus echter ObjectUpdate-TE ergänzt werden. Beim nachgewiesenen Wechsel auf eine andere gültige COF-/AppearanceVersion wurde bislang die alte ObjectUpdate-TE weiter mit dem neuen Outfit gemischt. Jetzt alte provisorische Bakes ausschließlich bei tatsächlichem neuen akzeptierten COF-/Formatwechsel entfernen, hochwertige Low158 weiter behalten. Der bestehende COF-Highwater schützt auch bei interleaved Legacy count=0; erste unversionierte Low158, identische/rejected COF und erster moderner Login behalten bisherige Fallback-Logik. Aktuelle neue ObjectUpdate darf wieder echte fehlende Layer ergänzen. F3 zeigt COF-Wechsel: alte Objekt-Bakes verworfen, getrennt von GPU-/Downloadstatus, keine private Textur-ID. Native Mint/Pi5 WorldModel-Test COF100→101, Rollback/Legacy/Zwischennachrichten/Versionformat, Python-Realpath und kompletter Mint-x64-GUI-Gate. Echter NexVerse Avatar-/Low158-/COF-/Outfit-/Skin-/WASD Live-E2E weiterhin still open. Projekt 39 %, nächste zehn Blöcke Alpha.965–974. Details docs/ALPHA955_964_OUTFIT_COF_PROVISIONAL_BAKE_RETIREMENT.md, kein Deployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.954, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.955–964.**

**ALPHA.954 EXAKTE CODE-SHA / MINT RELEASE / CI:** Code c86977f978cc15d3495df6cc88af65c75ce0d7d6 auf Linux Mint jam-MS-7D28 vollständig getestet und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: kompletter CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.954, 183/183 native CTests, 457/457 Python, Xvfb-isolierter GUI-Start mit Original-Firestorm-Skelett 133 Joints, 3 GPU Bodymeshes/7186 Dreiecke, OGL X11 WM_CLASS/Taskleistenicon 128x128 PASS. Pi5 ARM64 Source Check zur EXAKTEN Code-SHA c86977f978cc15d3495df6cc88af65c75ce0d7d6 Run 40008385875 zuletzt IN_PROGRESS; keinen Pi5-Erfolg ohne completed/success behaupten. Hosted GitHub Linux x64 Run 40008385892 COMPLETED/FAILURE, Job 120602080388 runner_id=0 steps=0, kein Runner und kein Compiler gestartet. Letzter tatsächlich bestätigter Pi5-SUCCESS: Alpha.944 Code-SHA 7a97831 Run 39994940156. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTEN SSA-ACK-Verlust vor Inworld-Eintritt behoben: bisher setzte ein neuer gültiger RegionHandshake während awaiting_region_ready auch schon zuverlässig bestätigte SSA bit2 ACK und Timestamp zurück. Jetzt SelfAppearanceHandshakeReceipt.note_refresh bewahrt den ACK-Nachweis bei gleichem Login vor und nach active; noch ausstehende ACK darf nur zur neuesten Pending-Sequenz passen, Neusitzung reset weiterhin. F3 zeigt datensparsamen SSA ACK vor Inworld erhalten-Zähler ohne Skin/COF/WASD-Falschbeweis. Native Mint/Pi5 SSA-ACK-/Replay-Tests, Python real source contracts und voller Linux-Mint-x64-GUI-Test. Echter authentifizierter NexVerse Live Avatar-/COF-/Outfit-/Skin-/WASD-End-to-End-Test weiterhin still open, Projekt 39 %, NÄCHSTE zehn Blöcke Alpha.955–964, Linux Mint primär, Pi5 CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Pi5 NUR ARM64 CI, voller Mint-x64 GUI-Gate vor GitHub-Push, kein Produktivdeploy.**

23.10.2026 Alpha.945–954: Alpha.944 Pi5 Feature-SHA 7a97831 Run 39994940156 COMPLETED/SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER Pre-Inworld SSA-ACK-Verlust behoben: Während awaiting_region_ready setzte ein neuer, unterschiedlicher RegionHandshake bislang sogar eine bereits vom ReliableTracker bestätigte SSA-bit2-Antwort und ihren Timestamp zurück. Alpha.934 schützte diesen Fall nur bei active. Neue SelfAppearanceHandshakeReceipt.note_refresh bewahrt verifizierte ACK vor UND nach Inworld; solange keine SSA-ACK bestätigt ist, bleibt nur die neueste tatsächlich versendete Reliable-Sequenz ACK-fähig. Circuit bleibt vor authentifiziertem AgentMovementComplete korrekt awaiting_region_ready; neuer Login leert alten SSA-Nachweis. F3-Zähler SSA ACK vor Inworld erhalten ohne Fake-Skin/COF/WASD. Native Mint/Pi5 SSA-ACK- und Handshake-Replay-Tests, Python-Realpath und voller Linux Mint x64 GUI-Gate. Live NexVerse Avatar-/Low158-/COF-/Outfit-/Skin-/WASD-E2E weiterhin still open, Projekt 39 %, nächste Alpha.955–964. Details docs/ALPHA945_954_PRE_INWORLD_SSA_ACK_PRESERVATION.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.944, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.945–954.**

**ALPHA.944 EXAKTE FEATURE-SHA / MINT-RELEASE / CI:** Code 7a97831bd9ef52f5a001cd174d70ffba006c0e30 auf Linux Mint jam-MS-7D28 vollständig getestet und auf GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: kompletter CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.944, 183/183 native CTests, 452/452 Python, isolierter Xvfb GUI mit originalem Firestorm Skeleton 133 Joints, drei GPU-LLM Bodymeshes/7186 Dreiecke, OGL X11 WM_CLASS/Icon 128x128 PASS. Pi5 ARM64 Source CI exakt zu Code-SHA 7a97831bd9ef52f5a001cd174d70ffba006c0e30 Run 39994940156 zuletzt IN_PROGRESS, nicht grün behaupten. Hosted GitHub Linux x64 Run 39994940090 COMPLETED/FAILURE Job 120555900346 runner_id=0 steps=0, kein Runner/Compiler gestartet. Letzter SHA-genau erfolgreich verifizierter Pi5: Alpha.934 Code-SHA f9d8a75 Run 39992793374 SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER falscher-Region-Low15-Fehler behoben: richtiges Agent/Session, aber falsches RegionHandle konnte Circuit aktivieren und Low381 verschicken, obwohl WorldModel die falsche Simulatorregion ablehnte. Jetzt voller erwarteter XML-RPC Login-RegionHandle (region_x<<32|region_y), Legacy beide Koordinaten 0 bleibt möglich; x=0 Randregion weiterhin geprüft. F3 getrennte datensparsame Zähler falsche Low15-Loginregion und allgemeine Low15-Ablehnung, keine Avatar-/COF-/Skin-oder WASD-Falschbehauptung. Native echte LLUDP-Testpakete, Mint/Pi5, Python-Realpath-Vertrag, voller Mint-x64-GUI-Gate. Echter NexVerse Live Avatar-/COF-/Outfit-/Skin-/WASD-E2E weiterhin still open. Projekt 39 %, NÄCHSTE zehn Blöcke Alpha.945–954. Linux Mint primär, Pi5 nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 nur ARM64 CI; voller Mint-x64-GUI-Release-Gate vor Push; kein Produktivdeploy.**

23.10.2026 Alpha.935–944: Pi5 Alpha.934 Code-SHA f9d8a75 Run 39992793374 COMPLETED/SUCCESS; OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER Low15 RegionIdentity-Fehler behoben: CircuitConnection konnte bei gültiger AgentID/SessionID, aber fremdem nichtleeren Simulator-RegionHandle bereits active melden und Low381 versenden, obwohl das WorldModel die Fremdregion korrekt verwarf. Nun exakt gleichen Login-64-bit-RegionHandle (region_x<<32|region_y) wie WorldModel verlangen, Legacy ohne beide Koordinaten weiterhin möglich; Weltrand mit einer Nullkoordinate weiter streng geprüft. F3 zeigt datensparsamen separaten Low15 falsche Login-Region N und generische Ablehnungszahl, keine falsche Skin/COF/WASD-Bestätigung. C++ native echte LLUDP-Regressionsfälle, Mint/Pi5 und Python-Quellintegrationsvertrag, voller Mint-x64-GUI-Gate. Live NexVerse Avatar-/COF-/Skin-/Outfit-/WASD-End-to-End weiterhin still open. Projekt 39 %, nächste Alpha.945–954. Details docs/ALPHA935_944_MOVEMENT_REGION_IDENTITY_CIRCUIT_GUARD.md, kein Deployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.934, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.935–944.**

**ALPHA.934 EXAKTE CODE-SHA / MINT-RELEASE / CI:** Code f9d8a7585bdd4ea71211b2159c17ae80c9a1279a auf Linux Mint jam-MS-7D28 vollständig geprüft und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: Voller CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.934, 183/183 native CTests, 447/447 Python, isolierter Xvfb-GUI mit Original Firestorm-Skelett 133 Joints, 3 GPU Bodymeshes/7186 Dreiecke und OGL X11 WM_CLASS/Icon 128x128 PASS. Pi5 Source Check zur GENAUEN Code-SHA Run 39992793374 zuletzt IN_PROGRESS, nicht als Erfolg behaupten. Hosted GitHub Linux-x64 Run 39992793464 COMPLETED/FAILURE Job 120548539766 runner_id=0 steps=0, kein Runner/Compiler gestartet. Letzter SHA-genau bestätigter Pi5-SUCCESS: Alpha.924 Code-SHA 6be966b Run 39991058245. Upstream OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER RegionHandshake-Duplikatfehler behoben: wiederholte reliable LLUDP RegionHandshake-Zustellung löschte SSA-ACK und Timestamp und degradierte CircuitState::active zu awaiting_region_ready. Jetzt circuit-gebundene bounded History von 16 eingehenden Handshake-Sequenzen mit out-of-order/Wrapping, Transport-ACK/verlässliche bestehende Reply-Repetition bleibt, kein doppelter Reply/Reset. Neue RegionHandshake-Sequenz verhandelt Firestorm-SSA-bit2 ohne Rückstufung einer laufenden Szene oder belegte alte SSA-ACK. F3 zeigt Duplikate/Refresh-Zähler ohne Fake Skin/COF/WASD. Echter authentifizierter NexVerse Live-Avatar-/COF-/Outfit-/Skin-/WASD-End-to-End-Nachweis still open. Projekt 39 %, nächste zehn Blöcke Alpha.935–944. Linux Mint primär, Pi5 nur ARM64 CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 NUR ARM64-CI. Voller Mint-x64-GUI-Gate vor Push; KEIN Produktivdeploy.**

23.10.2026 Alpha.925–934: Pi5 Alpha.924 Code-SHA 6be966b Run 39991058245 COMPLETED/SUCCESS; OpenGenesisLINK Servermain ccfc644d unveraendert. ECHTEN LLUDP RegionHandshake-Race behoben: ein Duplikat hat bislang SSA-ACK/Timestamp geloescht und CircuitState::active auf awaiting_region_ready zurueckgesetzt. Jetzt maximal 16 vorige Eingangssequenzen (auch out-of-order) unterdrueckt, aber reliable LLUDP ACK bleibt erhalten; bestehendes Reply bleibt bei ReliableTracker. Neue RegionHandshake-Sequenz wird mit Firestorm SSA-bit2 beantwortet, ohne aktiven Circuit oder bestaetigte SSA-ACK zurueckzustufen. F3 zeigt RegionHandshake Duplikate und Aktualisierung separat von echtem Avatar/COF/Skin/WASD-Readback. Native C++23-Testfaelle fuer Replay, Wrap, FIFO, Reset und SSA-ACK, Pi5 ARM64, Python-Realpath und kompletter Mint-x64-GUI-Gate. Authentifizierter echter NexVerse Avatar/COF/Skin/Outfit/WASD Live-E2E weiterhin still open. Projekt 39 %, NÄCHSTE Alpha.935–944. Details docs/ALPHA925_934_REGION_HANDSHAKE_REPLAY_SSA_STABILITY.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.924, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.925–934.**

**ALPHA.924 EXAKTE FEATURE-SHA / MINT / CI:** Code 6be966b3cd090d6cb3315f612f4dc158bc7d4315 auf Linux Mint jam-MS-7D28 vollständig getestet und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: vollständiger CMake/vcpkg Linux-x64-Debug-Viewer 0.2.0-alpha.924, 182/182 native CTests, 442/442 Python, isolierter Xvfb GUI mit Firestorm Original-Skelett 133 Gelenke, drei GPU-LLM Bodymeshes/7186 Dreiecke, OGL X11 WM_CLASS/Taskleistenicon 128x128 PASS. Pi5 ARM64 Source Check Run 39991058245 zur exakten Code-SHA 6be966b3cd090d6cb3315f612f4dc158bc7d4315 zuletzt IN_PROGRESS, nicht grün behaupten. Hosted GitHub Linux x64 Run 39991058331 COMPLETED/FAILURE, Job 120542614653 runner_id=0 steps=0, also kein Runner und kein Compiler gestartet. Letzter erfolgreicher Pi5 SHA-genau: Alpha.914 Code-SHA 55b719e Run 39989404270 SUCCESS. Upstream OpenGenesisLINK Servermain ccfc644d unverändert. ECHTER Low15-Circuit-Fehler behoben: zuvor reichte die AgentMovementComplete-Paketkennung ohne valide Payload für CircuitState::active und initialen Low381-Wearables-Request. Jetzt gültiger echter LLUDP-Decoder, aktuelle Agent/Session, nichtleerer RegionHandle, plausible Position und zuvor gesendete Bewegungsanfrage erforderlich. Ungültige/fremde Pakete bestätigen weiterhin zuverlässige LLUDP-Zustellung, aktivieren aber keinen Circuit. F3 zeigt echte Low15-Ablehnungsanzahl, keine gefälschte Skin-/COF-/WASD-Bestätigung. C++23 natives Echtdaten-LLUDP-Testpaket mit truncated/foreign/NaN/huge/valid/vertical LookAt und Pi5-Test, Python Realpath, Mint-x64-GUI. Echter NexVerse Live-Avatar-/COF-/Outfit-/Skin-/WASD-GPU-End-to-End-Test weiterhin still open. Projekt 39 %, nächste ZEHN Blöcke Alpha.925–934. Linux Mint primär, Pi5 nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 nur ARM64-CI. Voller Mint-x64-GUI-Release-Gate vor Push, kein Produktivdeploy.**

23.10.2026 Alpha.915–924: Pi5 CI Alpha.914 Code-SHA 55b719e Run 39989404270 COMPLETED/SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTEN LLUDP-Circuit-Fehler behoben: AgentMovementComplete-Paket-ID alleine aktivierte CircuitState::active und löste Low381-Wearables-Request aus, ohne fremde oder defekte AgentMovementComplete-Payload zu prüfen. Jetzt echten Decoder, authentifizierten Agent/Session, RegionHandle, endliche plausible Simulatorposition UND zuvor gesendeten CompleteAgentMovement-Request verlangen. Ungültige Low15 aktiviert keinen Circuit und startet keine Low381; reliable-ACK bleibt korrekt. CircuitStatus zählt abgewiesene Low15, F3 zeigt Low15 abgewiesen: N ohne Skin/COF/WASD-Falschbeweis. C++23 echte LLUDP-Decode-Regression mit Foreign/NaN/truncated/large/valid und vertikalem LookAt, Mint-/Pi5-Integration, Python-Quellprüfung, voller Mint-x64-GUI-Gate. Echter authentifizierter NexVerse Live Avatar-/COF-/Skin-/Outfit-/WASD-E2E bleibt still open; Projekt 39 %, NÄCHSTE Alpha.925–934. Details docs/ALPHA915_924_CIRCUIT_MOVEMENT_COMPLETE_AUTHORITY.md. Kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.914, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.915–924.**

**ALPHA.914 EXAKTE FEATURE-SHA / MINT GUI / CI:** Code 55b719e011a9321cf7a24c60e52cfc8053029c1f lokal Linux Mint jam-MS-7D28 vollständig getestet und auf GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: vollständiger CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.914, 181/181 native CTests, 437/437 Python, Xvfb-isolierter GUI-Start mit Original-Firestorm-Körperskelett 133 Joints, drei GPU-residenten LLM-Meshes/7186 Dreiecke und OGL-X11 WM_CLASS/128x128 Systemicon PASS. Pi5 Source CI zu exakt dieser Code-SHA Run 39989404270 zuletzt IN_PROGRESS, NICHT als Erfolg melden. Hosted GitHub Linux x64 Run 39989404307 COMPLETED/FAILURE, Job 120536947166 runner_id=0 steps=0, kein Compilerstart. Vorheriger exact-SHA Pi5-Erfolg: Alpha.904 Code-SHA 90bde59 Run 39986215063 SUCCESS. OpenGenesisLINK Servermain ccfc644d (#286 OGLVoice-only). WICHTIGE NEUE FUNKTION: bisher nur einmaliger Low381 AgentWearablesRequest konnte trotz Reliable-ACK ohne Low382 bleiben. Jetzt maximal zwei read-only Wiederholungen nach 5s und 12s, stoppen nur bei gültiger Low382 mit Agent/Session-ID des aktuellen Circuits, keine COF-/Outfit-/Skin-Mutation. F3 zeigt Readback-Anzahl und gültigen Wire-Empfang, aber keine gefälschte COF-/GPU-Bestätigung. Authentifizierter NexVerse Live-Avatar/COF/Skin/Outfit/WASD-End-to-End-Beweis weiterhin still open. Projekt 39 %, NÄCHSTE ZEHN Blöcke Alpha.915–924, Linux Mint primär, Pi5 nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Pi5 nur ARM64-CI. Voller Mint-x64-GUI-Release-Gate vor GitHub-Push, kein Produktivdeploy.**

23.10.2026 Alpha.905–914: Pi5 CI Alpha.904 Code SHA 90bde59 Run 39986215063 COMPLETED SUCCESS. OpenGenesisLINK Servermain ccfc644d unverändert. ECHTE fehlende Recovery für Low382-Outfit-Readback ergänzt: zuverlässige ACK auf einmaligen Low381 AgentWearablesRequest war keine Garantie für Low382 AgentWearablesUpdate. Jetzt nach ausbleibendem authentifizierten Readback maximal zwei read-only Low381 Anfragen nach 5s und 12s, identische LoginSession, zuverlässige LLUDP-Zustellung, kein COF-/Kleidungs-/Skin-Upload. Nur echtes korrekt decodiertes Low382 mit passender AgentID/SessionID stoppt. F3 zeigt Low381 Wiederholungen N/2 und gültigen Low382 Wire-Empfang ohne gefälschten Outfit-/GPU-Beweis. Native Mint/Pi5 Test-Suite, Python-Realsource-Contract und voller Mint-x64-GUI-Gate. Authentifizierter NexVerse Inworld Avatar-/COF-/Outfit-/Skin-/WASD-E2E still open. Projekt 39 %, nächste Alpha.915–924. Details docs/ALPHA905_914_WEARABLES_READBACK_RECOVERY.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.904, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.905–914.**

**ALPHA.904 VERIFIZIERTER CODE-COMMIT / MINT / EXAKTE CI:** Feature-Code-SHA 90bde599854eb805eec66ebe00f10c57d1b12594 auf Linux Mint jam-MS-7D28 vollständig gebaut und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg vollständiger Linux-x64-Debug-Viewer 0.2.0-alpha.904, 180/180 native CTests, 432/432 Python, isolierter Xvfb-GUI-Start mit 133 Firestorm Original-Skeleton-Joints, drei GPU Original-Bodymeshes/7186 Dreiecke, OGL WM_CLASS/X11-Systemicon 128x128 PASS. Pi5 ARM64 Source Check Run 39986215063 für exakt Code-SHA 90bde599854eb805eec66ebe00f10c57d1b12594 zuletzt IN_PROGRESS, noch kein grüner Erfolg. Hosted GitHub Linux x64 Run 39986215086 COMPLETED/FAILURE Job 120525941955 runner_id=0 steps=0: kein Runner, kein Compiler. Letzter SHA-exakt erfolgreich abgeschlossener Pi5 Alpha.894 Code 5247d3f Run 39984523949 SUCCESS. OpenGenesisLINK Servermain ccfc644d (#286 nur OGLVoice Go Bootstrap). ECHTER AgentMovementComplete-Yaw-Fehler behoben: Simulatorpaket hat Position/LookAt, keine BodyQuaternion; bisher wurde vorläufiger Login-Drehwinkel als Serverrotation bestätigt und High4 WASD freigegeben. Jetzt echten horizontalen LookAt mit atan2 zu Z-Quat konvertieren, ungültige/Null-LookAt position-only ohne neue Rotations-/WASD-Autorität; bereits verifizierter früherer Server-Heading bleibt. Native Linux/Pi5 und Python-Decoder-/Handler-Tests, voller Mint x64 GUI-Gate. Echter Live-NexVerse-Avatar/COF/Skin/Outfit/WASD Inworld-Test weiterhin still open. Projekt 39 %, nächste ZEHN Blöcke Alpha.905–914, Linux Mint primär, Pi nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 NUR ARM64 CI. Voller Mint-x64-GUI-Gate vor Push; kein Produktivdeploy.**

23.10.2026 Alpha.895–904: Pi5 Alpha.894 Feature-SHA 5247d3f Run 39984523949 zuletzt IN_PROGRESS. Servermain ccfc644d neu #286 dedizierter Voice-Go-Bootstrap, keine Avatar-Wireänderung. ECHTER LLUDP AgentMovementComplete Heading-Fehler: Paket hat nur Position und LookAt-Vec3 statt Quaternion; bisher wurde die vorläufige alte Agent.rotation als serverbestätigter Körper-Yaw akzeptiert und konnte High4 WASD fälschlich zulassen. Neuer echter servergebundener simulator_look_at_heading Konverter berechnet Yaw aus horizontalem endlichen LookAt; Null/vertikal/NaN/Inf nicht als Rotation bestätigt. Neuer WorldModel position-only Readback nimmt Position ohne fiktive BodyQuat/WASD-Freigabe an, bewahrt vorher unabhängig bestätigte Serverrotation. AgentMovementComplete-Handler nutzt echte LookAt-Quat oder position-only, Native C++-/Pi5-Test, Python-Realpath, voller Mint-x64-GUI-Gate. Authentifizierter Live-NexVerse Avatar-/COF-/Skin-/Outfit-/WASD-GPU-E2E still open. Fortschritt 39 %, nächste 10 Blöcke Alpha.905–914. Details docs/ALPHA895_904_AGENT_MOVEMENT_LOOKAT_AUTHORITY.md, kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.894, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.895–904.**

**ALPHA.894 EXAKTE FEATURE-SHA / MINT RELEASE / CI:** Code 5247d3f9d775411a1af7137545062161af702668 unter Linux Mint jam-MS-7D28 mit vollem x64-Release-Gate geprüft und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg vollständiger Linux-x64 Debug-Viewer 0.2.0-alpha.894, 179/179 native CTests, 428/428 Python, isolierter Xvfb GUI mit Firestorm Original-Skelett 133 Joints/3 GPU Bodymeshes 7186 Dreiecke, OGL X11 WM_CLASS/Systemicon 128x128 PASS. Pi5 Source CI exakt zu Code-SHA 5247d3f9d775411a1af7137545062161af702668 Run 39984523949 zuletzt IN_PROGRESS, nicht als Erfolg melden. Hosted GitHub x64 Run 39984524067 COMPLETED/FAILURE Job 120520137562 runner_id=0 steps=0, also KEIN gestarteter Compiler. Letzter bestätigter Pi5-Erfolg Alpha.884 Code-SHA 7e43a2a Run 39982171338 SUCCESS. OpenGenesisLINK Servermain 08f95e7. ECHTER High4/AgentUpdate-WASD-Originfehler behoben: Ein aktiver Circuit und finite provisorische Loginposition (0,0,0) konnten ohne echte Simulator-Self-Pose High4 WASD/Yaw/Flight senden. Jetzt authentifizierte WorldModel-Selbstpose und aktuelle Agent/Session erforderlich, sonst kein MovementController.step/UDP-Control. Regionswechsel löscht alte Outbound-Queue, lokalen Heading/Flight, WASD-/Turn-Readback; F3 zeigt explizit WASD: warte auf bestaetigte Simulatorposition. Authentifizierter Live-NexVerse Avatar-/COF-/Skin-/Outfit-/WASD-End-to-End-Beweis still open. Projektfortschritt 39 %, NÄCHSTE 10 BLÖCKE Alpha.895–904. Linux Mint primär, Pi5 nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current. Raspberry Pi 5 NUR ARM64 CI. Voller Mint-x64-GUI-Gate vor Push; kein Produktivdeploy.**

23.10.2026 Alpha.885–894: Pi5 CI Alpha.884 Feature-SHA 7e43a2a Run 39982171338 COMPLETED SUCCESS. Servermain OpenGenesisLINK 08f95e7 unverändert. ECHTEN High4 WASD-Startfehler behoben: aktive LLUDP-Verbindung und finite provisorische (0,0,0)-Loginposition konnten AgentUpdate/WASD/CameraCenter auch ohne gültigen authentifizierten AgentMovementComplete-Posebeweis zulassen. Neue WorldModel self_avatar_controls_ready Prüfung verlangt serverbestätigte eigene Position und gültige Rotation, UI kontrolliert Agent und Session unter world_mutex. Keine ausgedachten Serverkoordinaten oder gefälschter WASD-Erfolg. Neuer Regionepoch leert OutboundCommandQueue, lokale Yaw- und Flugsteuerung, speculative Turn/WASD-Readback. F3 zeigt bei fehlender Selbstpose ausdrücklich WASD: warte auf bestaetigte Simulatorposition. Native Tests für finite Nullposition, echten Pose-WASD-Bit, fremde Session, Disconnect, Reconnect und Invalid-Pose; Python-Quellvertrag und voller Mint-x64-GUI-Gate. Echte NexVerse Live-Avatar/Low158/COF/Skin/Outfit/WASD Abnahme weiterhin still open. Projekt 39 %, nächste 10 Blöcke Alpha.895–904. Details docs/ALPHA885_894_HIGH4_VERIFIED_SERVER_POSE_AND_EPOCH_RESET.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.884, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.885–894.**

**ALPHA.884 GITHUB FEATURE-SHA / MINT RELEASE / CI:** Code 7e43a2aca847525fa0a4a9cdaaec49db6e0283d7 auf Linux Mint jam-MS-7D28 vollständig getestet, nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: Linux-x64 CMake/vcpkg Debug OGL Viewer 0.2.0-alpha.884, 178/178 native CTests, 424/424 Python-Verträge, isolierter Xvfb Viewer mit Firestorm Original-Skelett 133 Gelenke/3 GPU Bodymeshes 7186 Dreiecke und OGL-X11 WM_CLASS/Icon 128x128 PASS. Exakte Pi5 Source CI Run 39982171338 zuletzt IN_PROGRESS, noch keinen Erfolg behaupten. Hosted GitHub Linux-x64 Run 39982171352 COMPLETED/FAILURE Job 120511984008 runner_id 0, steps 0: keine Runnerzuteilung, kein Compilerstart. Letzter exact-SHA Pi5 SUCCESS: Alpha.874 Code SHA 6eabf2f Run 39979417843. OpenGenesisLINK Servermain 08f95e7 unverändert. ECHTER High9 Identitätsfehler: self_avatar_local_id nun an aktuelle WorldModel Region-GENERATION und Handle gebunden. Vorher gleiches RegionHandle bei neuer Simulator-IP/Port und recycelter LocalID konnte alten Selbstavatar/WASD-Beweis, KillObject und F3-/Render-Anhänge falsifizieren. Gültiger Erstlogin mit Generation 0, Full/Compressed rebinds vor UI Epoch-Refresh werden erhalten, alte Bindungen zurückgesetzt. Authentifizierter NexVerse-Inworld-Avatar-/COF-/Outfit-/Skin-/WASD-E2E weiterhin still open. Projekt 39 %, nächste ZEHN Blöcke Alpha.885–894. Mint primär, Pi ausschließlich CI, kein Deployment.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi5 nur ARM64 CI. Voller Mint-x64-GUI-Gate vor Push. Kein Produktivdeploy.**

23.10.2026 Alpha.875–884: Pi5 CI Alpha.874 Feature-SHA 6eabf2f Run 39979417843 COMPLETED SUCCESS; OpenGenesisLINK Servermain 08f95e7 unverändert. ECHTER eigener LocalID-/Bewegungs-/Anhängefehler gefunden: Nach Simulator-IP/Port-Wechsel mit gleichem RegionHandle wurde WorldModel korrekt geleert, aber Anwendung verwendete self_avatar_local_id nur mit altem RegionHandle ohne Regionsgeneration. Alte High9 ImprovedTerse-Updates ohne UUID konnten numerische LocalID erneut als eigenen Avatar interpretieren, WASD/Yaw fälschlich bestätigen oder Renderer/F3-Attachments falsch zuordnen. Jetzt neues MPL-Modul current_self_avatar_local_id mit numerischer ID, Handle UND WorldModel-Generation, auch Erstlogin-Generation 0 gültig. Full/Compressed bindet Generation neu, High9/Kill und Render-/F3-Attachments prüfen identische Bindung. Epochwechsel entfernt nur wirklich alte Bindung und bewahrt ein vom UDP-Thread bereits korrekt neu gebundenes Full-ObjectUpdate. Native reale same-handle/socket-change-, initial zero-epoch- und rebind-Regressionen; Python-Realpath und kompletter Mint-x64-GUI-Gate. Echte NexVerse Avatar-/Low158/COF/Skin/Outfit/WASD Live-End-to-End-Abnahme weiterhin OFFEN. Gesamtprojekt 39 %, nächste Alpha.885–894. Details docs/ALPHA875_884_SELF_LOCALID_GENERATION_LEASE.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.874, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.875–884.**

**ALPHA.874 FEATURE-SHA / MINT-RELEASE / CI-STATUS:** Code 6eabf2f9d93dbb6188dcfdc60d739ef309bfd965 vollständig auf Linux Mint jam-MS-7D28 gebaut und getestet, nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg Linux-x64 Debug Viewer 0.2.0-alpha.874, 178/178 native CTests, 420/420 Python, isolierter Xvfb GUI mit 133 Original-Firestorm-Joints, drei GPU-residenten Original-Bodymeshes/7186 Dreiecke, OGL WM_CLASS/X11-Systemicon 128x128 PASS. Pi5 ARM64 Source Check zur EXAKTEN SHA Run 39979417843 zuletzt IN_PROGRESS, noch NICHT grün. Hosted GitHub x64 Run 39979417903 COMPLETED/FAILURE, Job 120502450767 runner_id=0 steps=0: Runner nicht zugeteilt, kein Compilerstart. Voriger Pi5 exact-SHA SUCCESS: Alpha.864 Code-SHA 66e0211 Run 39977723209. Servermain 08f95e7 Voice #285, kein geänderter Avatar-Wirevertrag. ECHTER gleicher-RegionHandle-Simulator-IP-/Portwechsel invalidiert jetzt alte Terrain-/Objekt-LocalIDs, Avatar-Posen, Low158/COF/Outfit/Skin/Wearables und frühe FastRez-Nachrichten; die Regiongeneration steigt. Zwischenhandshakes mit leeren IP/Portfeldern behalten den letzten bekannten echten Socket. Auth NexVerse Live Avatar-/COF-/Skin-/WASD- E2E weiterhin OFFEN. Projekt 39 %, NÄCHSTE ZEHN BLÖCKE Alpha.875–884. Linux Mint primär, Raspberry Pi ausschließlich CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi5 nur ARM64 CI. Voller Mint-x64-GUI-Gate vor Push. Kein Produktionsdeploy.**

23.10.2026 Alpha.865–874: Pi5 Alpha.864 Feature-SHA 66e0211 Run 39977723209 COMPLETED SUCCESS; aktueller OpenGenesisLINK Servermain 08f95e7, Voice #285 ohne neuen Avatar-/COF-/WASD-LLUDP-Vertrag. ECHTEN Reconnect-Fehler behoben: Bei gleichem RegionHandle mit gewechselter Simulator-IP oder UDP-Port blieben WorldModel-Szene, Avatar-Pose, Terrain, Original-Meshes, COF-/Skin-/Wearables und früh gepufferte Nachrichten fälschlich im alten Regions-Epoch. Neuer Socketwechsel wird als echte Regionsinvalidierung behandelt; Remote-/Self-Objekte und alte LocalIDs, serverbestätigte Pose und Bakes werden gelöscht, Regiongeneration steigt. Unvollständige Handshake-Updates ohne IP/Port dürfen einen bekannten Endpunkt nicht überschreiben, damit der nächste Socketwechsel zuverlässig erkannt wird. Native WorldModel-Regression für same socket, Port 9000/9001, andere IP und leere Handshakes; Python-Quellintegration und vollständiger Mint-x64-Release-Gate. Echte NexVerse Live-Avatar-/Low158/COF/Outfit/Skin/WASD-End-to-End-Abnahme weiterhin OFFEN, Fortschritt 39 %. Nächste Alpha.875–884. Details docs/ALPHA865_874_SIMULATOR_SOCKET_EPOCH_RESET.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.864, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.865–874.**

**ALPHA.864 EXAKTE FEATURE-SHA / MINT-GATE / CI:** Code 66e0211817709f6b6db96ec2fab25d68b7e679d6 auf Linux Mint jam-MS-7D28 vollständig getestet und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg vollständiger Linux-x64-Debug-Viewer 0.2.0-alpha.864, 178/178 native CTests, 416/416 Python, Xvfb-isolierter GUI-Start mit Firestorm 133 Original-Joints, drei GPU Bodymeshes (7186 Dreiecke), OGL X11 Icon/WM_CLASS 128x128 PASS. Pi5 Source Check Run 39977723209 zur exakten SHA zuletzt IN_PROGRESS, nicht als grün behaupten. Hosted GitHub x64 Run 39977723207 COMPLETED/FAILURE, Job 120496597862 runner_id 0 und steps 0, kein Compiler gestartet. Letzter exakt verifizierter Pi5 SUCCESS: Alpha.854 Code-SHA 584ea15 Run 39975553846. OpenGenesisLINK Servermain 08f95e7 #285 Voice-WebRTC, keine Low158/Low382-Avatar-Wireänderung. BEHOBEN: Älterer FastRez-Frühavatar-Readback konnte nach neuerem live akzeptiertem Low158 den Skin rücksetzen; jetzt direkte akzeptierte Live-Version immer vorrangig, unabhängig von Legacy-COF-Format. Frühantwort zusätzlich an Simulator-IP/UDP-Port und Agent/Session/Region/Generation gebunden. F3 zeigt pending Low158/Low382 ohne falsche Skin-/COF-Behauptung. Echte authentifizierte NexVerse Avatar-/COF-/Skin-/Outfit-/WASD-Inworld-Abnahme weiterhin OFFEN. Projekt 39 %, nächste 10 Blöcke Alpha.865–874, Linux Mint primär, Raspberry Pi nur CI, KEIN Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi5 nur ARM64 CI. Mint x64 GUI Volltest vor Push, kein Produktivdeploy.**

23.10.2026 Alpha.855–864: Pi5 Alpha854 Feature-SHA 584ea15 Run 39975553846 COMPLETED SUCCESS. Server main 08f95e7 (#285 dediziertes Voice-WebRTC, keine neue Avatar-Wireänderung). Low158 Frühantwort nach neuerem direktem Live-Low158 konnte beim UI-Puffer-Flush den neuesten Skin überschreiben, insbesondere unversionierte Legacy-COF-Pakete. Jetzt darf Early Low158 nur ohne bereits akzeptiertes direktes WorldModel-Low158 eingespielt werden; Self-Agent/Session/Region/Generation um tatsächliche Simulator-IP/Port ergänzt, damit Reconnect mit gleichem Regionhandle alte frühe Bakes nicht wiederverwendet. Low382-Serialschutz bleibt. F3 zeigt eindeutig Low158/Low382 gepuffert (warte Circuit) statt Skin/COF-Erfolg. Native echte WorldModel-Regression COF17/42, Serial7/9, Legacy/no COF und Endpoint9000/9001, Python-Realpath-Test, vollständiger Mint-GUI-Release-Gate vor Push. Authentifizierter NexVerse-Inworld-Avatar/COF/Outfit/Skin/WASD weiterhin OFFEN. Fortschritt 39 %, nächste Alpha.865–874. Details docs/ALPHA855_864_EARLY_AVATAR_LIVE_RACE_AND_SOCKET_LEASE.md; kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.854, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.855–864.**

**ALPHA.854 EXAKTE CODE-SHA / MINT / GITHUB-CI:** Feature-Commit 584ea1519dd176fb311597673d6fe2f8f9eb71fc auf Linux Mint jam-MS-7D28 getestet und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh EXIT 0: vollständiger CMake/vcpkg Linux-x64-Debug Viewer, 178/178 native CTests (neuer FastRez-Original-WorldModel-Early-Readback-Test), 412/412 Python, isolierter Xvfb-GUI-Viewer 0.2.0-alpha.854 mit Firestorm 133 Original-Joints, drei GPU-Bodymeshes/7186 Dreiecken und OGL X11 WM_CLASS/Systemicon 128x128 PASS. Pi5 CI zur exakt selben Code-SHA Run 39975553846 zuletzt IN_PROGRESS, NICHT als erfolgreich behaupten. Hosted GitHub x64 Run 39975554311 COMPLETED/FAILURE Job 120489071717 runner_id=0 steps=0, kein Compiler gestartet. Letzter SHA-genau bestätigter Pi5 SUCCESS: Alpha.844 SHA 712dbe9 Run 39840873191. OpenGenesisLINK Servermain 8b3d80e (#276–#284), FastRez #281 WearablesRequestDelayMs=1000 opt-in und 64m nearby-first; neue OGLAssetService-Funktionen benötigen kein Viewer-Wireupdate. BEHOBENER RACE: echte Self-Low158 und Low382 trafen zwischen UDP-Paketempfang und CircuitState::active ein und wurden verworfen. Jetzt zwei strikt Agent/Session/Region/Generation-geprüfte Low158-/Low382-Readbacks gepuffert und erst nach tatsächlicher Circuitfreigabe genau einmal verarbeitet; gültige TE/VisualParams, COF-Antirollback und Low382-U32-Serienordnung geschützt, Cache-Reset bei Regions- oder Sessionwechsel. Live-eingeloggter NexVerse-Avatar/COF/Skin/WASD-End-to-End-Beweis weiter OFFEN. Projekt 39 %, NÄCHSTE ZEHN Blöcke Alpha.855–864, Linux Mint primär und Pi nur CI, KEIN Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current. Raspberry Pi 5 NUR ARM64-CI. Voller Mint-x64-GUI-Gate vor Push; kein Produktivdeploy.**

23.10.2026 Alpha.845–854: Pi5 CI Alpha.844 Feature-SHA 712dbe9 Run 39840873191 COMPLETED SUCCESS. OpenGenesisLINK Servermain von 7396897d auf 8b3d80e (9 Commits #276–#284) aktualisiert. Neu FastRez #281 optionale WearablesRequestDelayMs=1000 statt 4000 ms und Near-First ObjectUpdate bei 64m; kein neuer Viewer Wire-Contract, OGLAssetService bleibt Serveroption. ECHTEN FastRez-Initiallogin-Race-Fehler behoben: Low158 AvatarAppearance und Low382 AgentWearablesUpdate wurden von UDP vor CircuitState::active empfangen, WorldModel.region().connected aber erst danach gesetzt; frühe Nachrichten gingen vollständig verloren. Jetzt bounded Model-Zwischenablage je ein Paket mit Agent/Session/Region/Generation, gültiger vollständiger TextureEntry/VisualParams und aktuellem COF-/Serial-Replay-Schutz. Nach echter Circuit-/WorldModel-Freigabe wird genau einmal serverautoritativer Readback verarbeitet, F3-Zähler korrekt. Alle Region-/Session-/Login-Resets löschen Staging. Native C++-, Pi5- und Python-Realpath-Tests, vollständiger Mint-x64-GUI-Gate vor Push. Echte authentifizierte NexVerse-Avatar-/Skin-/COF-/Outfit-/WASD-Inworld-Tests stehen weiterhin OFFEN; Gesamtprojekt 39 %. NÄCHSTE Alpha.855–864. Details docs/ALPHA845_854_FASTREZ_EARLY_SELF_READBACK.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.844, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.845–854.**

**ALPHA.844 VERIFIZIERTER MINT-BUILD UND EXAKTE GITHUB-CI:** Feature-Code-SHA 712dbe92f0809116b45c74d6b837df993ad741db nach GitHub develop-current gepusht. Linux Mint jam-MS-7D28 scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg vollständiger Linux-x64-Debug-Viewer-Build, 177/177 native CTests, 408/408 Python, isolierter Xvfb GUI-Start 0.2.0-alpha.844 mit Original-Firestorm-Skelett 133 Gelenke, drei GPU-Anatomiembodymeshes und 7186 Dreiecken, OGL X11 WM_CLASS/Icon 128x128 PASS. Pi5 Source CI Run 39840873191 zur exakten SHA zuletzt IN_PROGRESS, noch nicht SUCCESS behaupten. Hosted GitHub x64 Run 39840873213 COMPLETED/FAILURE Job 120041219044 runner_id 0 und steps 0, also kein Compilerstart. Letzter bestätigter Pi5 SUCCESS: Alpha.834 SHA fa2f51d Run 39838069587. OpenGenesisLINK Servermain 7396897d. ECHTER Origin-Avatar-Renderfehler behoben: aktivierter Circuit vor AgentMovementComplete/Self-ObjectUpdate kann keinen bestätigten Firestorm-Eigenkörper mehr am vorläufigen Spawn anzeigen. Serverposition mit Agent/Session/Regionvalidierung aus AgentMovementComplete/Full/Compressed/Terse als einzige Renderfreigabe; Reset bei KillObject, Regionwechsel, Circuitverlust oder neuer Anmeldung. F3 zeigt sonst AgentPosition fehlt. Authentifizierter NexVerse-Avatar/Low158/COF/Skin/WASD Live-Inworld-E2E weiterhin OFFEN. Projekt 39 %, NÄCHSTE ZEHN Blöcke Alpha.845–854, Linux Mint primär, Raspberry Pi nur CI, kein Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 NUR ARM64 CI. Voller Mint-x64 GUI-Gate, kein Produktivdeploy.**

22.10.2026 Alpha.835–844: Vorige Pi5 Alpha.834 Feature-SHA fa2f51d Run 39838069587 SUCCESS; aktueller Servermain 7396897d. ECHTEN Selbstavatar-Renderfehler behoben: aktiviertes CircuitCode/AgentState.present genügte bisher zum Rendern von Firestorm-Bodymeshes am vorläufigen Login-Origin OHNE bestätigte Position. Jetzt authentifizierter WorldModel-Self-Pose-Gate mit passender Agent-ID, Session-ID, Regionhandle und valider Serverposition/Quaternion aus AgentMovementComplete, Full/Compressed/High9 Terse. F3 und Renderproxy zeigen ohne Simulatorposition keine falsche Eigenavatar-Bestätigung. Native Tests für Fake-Posen, eigene KillObject/Region/Session, Reconnect; Python-Quellvertrag, kompletter Mint-x64-GUI-Gate vor GitHub-Push. Live-NexVerse Avatar/Low158/COF/Outfit/Skin/WASD E2E bleibt OFFEN; 39 %. Nächste Alpha.845–854. Details docs/ALPHA835_844_AUTHENTICATED_SELF_POSE_RENDER_GATE.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.834, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.835–844.**

**ALPHA.834 EXAKTE FEATURE-SHA UND MINT/GITHUB-CI:** Code SHA fa2f51ded72fc4ae3d0c78d5f977b4ba212e4d36 auf Desktop Commander Linux Mint jam-MS-7D28 getestet und nach GitHub develop-current gepusht. Vollständiger lokaler scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg Linux-x64-Debug-Viewer, 176/176 native CTests, 404/404 Python-Tests, isolierter Xvfb-GUI-Start Version 0.2.0-alpha.834 mit originalem Firestorm-Skelett 133 Gelenke/drei GPU-residenten Body-Meshes (7186 Dreiecke) und OGL X11 WM_CLASS/Icon 128x128 PASS. GitHub Pi5 CI exakt dieser SHA Run 39838069587 zuletzt IN_PROGRESS, noch NICHT als erfolgreich melden. GitHub Hosted Linux x64 Run 39838069563 COMPLETED/FAILURE Job 120031529209 runner_id 0, steps 0, KEIN Compilerstart. Letzter verifizierter Pi5 CI SUCCESS: Alpha.824 SHA 730f115 Run 39836372832. Aktueller OpenGenesisLINK Servermain 7396897d. ECHTER F9 COF-Anzeigefehler behoben: WorldModel, manueller F9-Check und F9-Live-Check lesen nun dieselbe letzte authentische nicht-null COF-Version aus Firestorm Low158 AppearanceData; trailing COF=0 erzeugt keinen Falsch-Status, rein unversionierte Legacy-Pakete bleiben unbekannt. Authentifizierter NexVerse Inworld-Avatar Low158/COF/Skin/WASD-E2E bleibt OFFEN. Projekt 39 %, NÄCHSTE ZEHN Blöcke Alpha.835–844, Linux Mint primär, Raspberry Pi nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 nur für ARM64 CI, vollständiger Mint-x64-GUI-Gate vor Push; kein Produktivdeploy.**

22.10.2026 Alpha.825–834: vorherige Alpha.824 Pi5 CI SHA 730f115, Run 39836372832 COMPLETED SUCCESS. OpenGenesisLINK Servermain 7396897d unverändert. ECHTEN F9 COF-Anzeigefehler behoben: WorldModel verwendete bei mehrteiligen Firestorm Low158 AppearanceData bereits die letzte gueltige nicht-null COF-Version, aber F9 manuell und F9 Live verwendeten noch den letzten Eintrag selbst wenn COF=0. Neuer gemeinsam benutzter authentischer Protokollselektor synchronisiert WorldModel-Antirollback und beide F9 Anzeigewege. Reine Legacy/Nullversionen werden weiterhin als unbekannt markiert, kein erfundener COF-Erfolg. Native echter Low158-LLUDP-Datagrammdecoder, WorldModel und F9-Bake-Evidence-Tests, Python-Vertrag plus vollständiger Mint-x64-GUI-Gate vor Push. Echter eingeloggter NexVerse Low158/COF/Skin/WASD Inworld-End-to-End-Nachweis bleibt offen. Fortschritt 39 %, nächste Alpha.835–844. Details docs/ALPHA825_834_F9_COF_MULTIRECORD_CONSISTENCY.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.824, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.825–834.**

**ALPHA.824 GITHUB FEATURE-SHA UND CI-PROVENIENZ:** Code SHA 730f115723b10cf47c88e13d05add446e982f560 von Linux Mint jam-MS-7D28 nach GitHub develop-current gepusht. Kompletter Mint-x64-Viewer scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg Debug Build, 176/176 native CTests, 400/400 Python, isolierter Xvfb GUI-Start 0.2.0-alpha.824 mit Firestorm 133 Skelett-Joints und drei GPU-Original-Bodymeshes (7186 Dreiecke), korrektes OGL WM_CLASS/X11 Taskleistenicon 128x128. Pi5 ARM64 Source Check Run 39836372832 fuer genau diese SHA zuletzt IN_PROGRESS; NICHT gruen behaupten. Hosted Linux-x64 Run 39836372803 COMPLETED/FAILURE Job 120025674283 runner_id=0 und steps=0, also kein zugeteilter Runner und kein Compilerstart. Letzter SHA-bestaetigter Pi5 SUCCESS: Alpha.814 SHA f2b8094 Run 39833603079. ECHTER F9-Skin-Fehler: 3 manuelle Nachpruefungen nun pro aktuelle authentifizierte Outfit-BodyBake-Quelle mit insgesamt 12 pro Regions-Epoch und unveraendertem 12-Sekunden-Pacing/AssetRetrySchedule; keine Abkuerzung durch leere/Placeholder/unauthentifizierte Sources. Authentifizierte Live-NexVerse AvatarAppearance/COF/Skin/WASD-Inworld-Sitzung weiter OFFEN. Projekt 39 %, naechste 10 Bloecke Alpha.825–834, Mint primaer, Pi nur CI, kein Produktionsdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Desktop Commander Linux Mint jam-MS-7D28; /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi5 nur ARM64-CI; Mint x64-Vollbuild/GUI-Tests vor Push; kein Produktivdeploy.**

22.10.2026 Alpha.815–824: vorheriger Pi5 Alpha.814 Code-SHA f2b8094 Run 39833603079 SUCCESS. OpenGenesisLINK Servermain 7396897d unverändert. Echter F9 Outfit-Avatar-Bake-Nachprüfen-Fehler: drei manuelle GetTexture-Replans waren pro ganzer Region erlaubt, danach auch für NEUE echte Outfits gesperrt. Jetzt authentifizierte neue Head/Upper/Lower-Serverquelle mit eigenem max. 3-Versuche-Budget, zugleich 12 total pro Regions-Epoch. 12s-Cooldown/HTTP-AssetRetrySchedule bleiben erhalten, Placeholder, leere/Fremd-Sources können keine erneute Freigabe erschleichen. F9 zeigt Outfit- und Regionslimit klar getrennt. Native vollständige Kontroll-/Epoch-/Churn-Regression und Python-Sourcematrix, Mint-x64 Release-Gate. P0 authentifiziertes Live-Avatar/Low158/COF/Skin/WASD bleibt offen. Gesamtprojekt 39 %, nächste Alpha.825–834; Details docs/ALPHA815_824_OUTFIT_BAKE_RECHECK_SCOPE.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.814, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.815–824.**

**ALPHA.814 GITHUB-/CI-STAND:** Code-SHA f2b809453bd68c0e7407569a25b627362010dc17 nach develop-current gepusht. Voller Linux-Mint-x64-Gate Exit 0 mit 176/176 nativen CTests, 396/396 Python, isoliertem Xvfb GUI und OGL-Systemicon PASS. GitHub Pi5 CI Run 39833603079 COMPLETED/SUCCESS. Gehostete x64 CI Run 39833603059 FAILURE vor Runnerstart (Job 120015989833, runner_id 0, steps 0); kein Compilerfehler belegt. Letzter Pi5-SUCCESS Run 39829308631 zur vorherigen Alpha.804-SHA 3f6483b. Aktuelle GPU-gebundene Low158-Head/Upper/Lower-Bake-IDs werden nun mit echten Server-IDs verglichen; fehlende Uebereinstimmung bleibt F3-Warnung. Authentifizierter NexVerse-Avatar/COF/Skin/WASD-Inworld-Test weiter OFFEN. Fortschritt 39 %, naechste Alpha.815–824. Linux Mint primaer, Pi nur CI, kein Deploy.**

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current; Raspberry Pi nur ARM64-CI. Vollständiges Mint-x64-GUI-Gate, keine ungefragten interaktiven Logins und kein Produktivdeploy.**

22.10.2026 Alpha.805–814: Vorherige Alpha.804 Pi5 CI Run 39829308631 für Code-SHA 3f6483b SUCCESS. OpenGenesisLINK-Servermain 7396897d unverändert. Realer F3-Avatar-Skin-Diagnosefehler: vorher konnten drei alte originale GL Body-Draws mit GPU-Residency der aktuellen Low158-TE als aktuelle sichtbare Skin gewertet werden, obwohl die tatsächlich gebundenen Head/Upper/Lower UUIDs abwichen. Renderer erfasst jetzt pro Frame die tatsächlich auf Original-Firestorm-.llm gebundenen Bake-IDs und setzt sie jedes Frame zurück. AvatarPipelineProof vergleicht exakt diese Quellen mit Low158 oder vorläufiger ObjectUpdate-TE; die F3-UI warnt bei abweichendem Texturstand und zeigt nicht grün. Native UI- und Original-OpenGL-FBO-Regressionen. Live-NexVerse-Avatar mit authentifiziertem Outfit, Low158/COF/Skin/WASD weiter still open, Projekt 39 %. NÄCHSTE Alpha.815–824. Details docs/ALPHA805_814_BAKE_GL_SOURCE_PROVENANCE.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.804, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.805–814.**

**ALPHA.804 EXAKTER GITHUB-SHA UND CI-STATUS:** Feature-SHA 3f6483b264d362e60c92f052faea5f9e28f84488 via Linux Mint jam-MS-7D28 nach GitHub develop-current gepusht. Vollstaendiger lokaler CMake/vcpkg Linux-x64-Mint-GUI-Gate EXIT 0: 176/176 native CTests, 392/392 Python, isolierter Xvfb Viewer 0.2.0-alpha.804 mit original Firestorm 133 Skeleton-Joints/3 GPU Bodymeshes, X11 OGL-Icon 128x128 PASS. Pi5 CI exakt SHA: Run 39829298694 jetzt CANCELLED, Run 39829308631 jetzt IN_PROGRESS. Hosted Linux-x64 Run 39829298689 und Run 39829308722 COMPLETED/FAILURE jeweils runner_id 0, steps 0, also kein Compilerstart. Letzter Pi5 SUCCESS: Alpha.794 Code-SHA 567d7a3, Run 39827583351. OpenGenesisLINK Servermain 7396897d unveraendert. Reale WASD-Fixes: Serverheading bei inaktivem Turn als Basis uebernehmen, keine veraltete High4-Quaternion; gescheiterte Queue setzt lokalen Yaw/Fly-State zurueck und veraendert keinen Avatar-Walk-Status. Echter eingeloggter NexVerse Avatar Low158/COF/Skin/WASD Ende-zu-Ende weiterhin OFFEN. Projekt 39 %, NAECHSTE ZEHN BLOECKE Alpha.805–814. Linux Mint primaer, Raspberry Pi nur CI, kein Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Desktop Commander Linux Mint jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current. Raspberry Pi 5 NUR ARM64-CI, x64 GUI Build und Tests auf Linux Mint. Kein interaktiver Viewerstart und kein Produktivdeploy.**

22.10.2026 Alpha.795–804: Bei Beginn Branch develop-current sauber auf SHA 8bc693f; vorherige GitHub Pi5 CI Alpha.794 Code SHA 567d7a3 Run 39827583351 COMPLETED SUCCESS. OpenGenesisLINK Servermain 7396897d nach #275 unverändert. ECHTE Avatar-Yaw-/WASD-Probleme im Haupt-Input-Pfad behoben: MovementController behielt nach erster Login-Initialisierung dauerhaft seinen alten lokalen Heading und konnte nach externer Simulatorrotation stale High4 Body-Quaternionen senden. Controller übernimmt nun serverbestätigte Rotation beim nicht aktiv drehenden, nicht lokal-previewenden AgentUpdate ohne Fly-/F-Keyreset. Zweiter Realbug: last_reported_control_flags und Walk-Animation-Fallback wurden VOR queue.push fortgeschrieben; jetzt nur nach erfolgreicher AgentUpdate-Aufnahme, bei voller Queue Rollback spekulativer Yaw- und Fly-Zustände. Native echte LLUDP High4 Quaternion-Wire-/FullQueue-Regressionen und Python-Code-Integationsvertrag, vollständiger Mint-x64-GUI-Gate vor Push. Authentifiziertes NexVerse Avatar-/Low158-/COF-/Skin-/WASD-Inworld-E2E weiterhin still open, Gesamtprojekt 39 %. Nächste zehn Blöcke Alpha.805–814. Details docs/ALPHA795_804_SERVER_HEADING_REBASE_QUEUE_CONSISTENCY.md. Kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.794, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.795–804.**

**ALPHA.794 EXAKTE FEATURE-SHA, MINT-GATE UND GITHUB-CI:** Feature-Code-SHA 567d7a3ffbc39a4b0a7ad468d8547a8654f5270b auf Linux Mint Desktop Commander jam-MS-7D28 im Branch develop-current gebaut und gepusht. scripts/mint-viewer-validation.sh FULL GATE EXIT 0: vollständiger CMake/vcpkg Linux-x64-Debug-Viewer, 176/176 native CTests inkl. neuer nexverse-local-heading-preview-tests, 388/388 Python-Unittests, isolierter Xvfb Viewer 0.2.0-alpha.794 mit 133 Firestorm Original-Bindjoints, drei GPU-residenten .llm-Körpermeshes und OGL WM_CLASS/X11 Taskleistenicon 128x128 PASS. GitHub Pi5 Source Check CI Run 39827583351 zu exakt dieser SHA zuletzt IN_PROGRESS; nicht als grün behaupten. Hosted GitHub Linux x64 Run 39827583363 COMPLETED/FAILURE Job 119994867860 runner_id 0, steps 0; keine zugeteilte Runnerhardware, kein CMake-/Compilerlauf. Letzter SHA-exakt verifizierter Pi5 SUCCESS: Alpha.784 Code-SHA 37a1068, Run 39825721706. OpenGenesisLINK Servermain 7396897d #275 nur Gruppenland-Key, kein High4-Protokollwechsel. ECHTER Avatar-/WASD-Fehler behoben: Requested-Bodyrotation wird nicht mehr ungefragt ins serverautoritative WorldModel geschrieben. Erfolgreich gequeue-ter TURN bekommt nur kurzen auf aktuelle Session und Agent begrenzten GPU-/Kamera-Preview (350ms); echte Serverbestätigung bleibt Full/Compressed/Terse-Readback. Authentifiziertes NexVerse-AvatarAppearance/COF/Outfit/Skin-GPU/WASD-Inworld-Ende-zu-Ende weiterhin still open. Projekt 39 %, NÄCHSTE ZEHN Blöcke Alpha.795–804, Linux Mint primärer Arbeitsrechner, Pi nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 nur ARM64-CI. Voller Linux Mint x64 GUI-Gate scripts/mint-viewer-validation.sh. Kein sichtbarer Desktop-Login oder Produktivdeploy ohne Auftrag.**

22.10.2026 Alpha.785–794: GitHub Pi5 Alpha.784 Run 39825721706 zur Code-SHA 37a1068 beim Start IN_PROGRESS. Servermain 7396897d, #275 NexGroups-Land-Key ohne OGL-Wireänderung. ECHTER WASD-Drehfehler: der Viewer schrieb die nur angeforderte Body-Quaternion schon beim lokalen MovementController::step VOR Serverbestätigung in WorldModel::apply_agent. Jetzt serverseitige AgentState-Rotation nur aus echtem Simulatorpaket; die unmittelbare GPU-Avatar-/Kamera-Vorschau liegt getrennt in einem max. 350-ms-Session/Agent-geprüften LocalHeadingPreview, nur nach erfolgreichem Turn-Queue-Push. Native C++23 MovementController/ThirdPersonCamera/Lease-Tests plus realer Python-WASD-Integrationsvertrag, vollständiger Mint-x64-GUI-Gate vor Push. Autorisierter Inworld-Avatar/Low158/COF/Skin/WASD-Yaw-Servernachweis weiterhin still open, 39 %. Nächste Alpha.795–804. Details docs/ALPHA785_794_TURN_SERVER_AUTHORITY_MINT.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.784, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.785–794.**

**ALPHA.784 EXAKTER SHA UND GITHUB-CI MIT MINT GUI-PROVENIENZ:** Feature-SHA 37a10685c827760adb548f7295d973ff9c843c7b auf Linux Mint Commander jam-MS-7D28 getestet und direkt in GitHub develop-current gepusht. scripts/mint-viewer-validation.sh FULL-GATE EXIT 0: echter vollständiger CMake/vcpkg Linux-x64-Debug-Viewer, 175/175 native CTests, 384/384 Python-Unittests, isolierter Xvfb-Viewer 0.2.0-alpha.784 mit Firestorm-Skelett 133 Joints/3 originalen GPU-Körpermeshes und korrektes OGL X11 WM_CLASS/Taskleisten-Icon 128x128 PASS. GitHub Pi5 Source Check für exakt diese Feature-SHA Run 39825721706 zuletzt IN_PROGRESS, nicht grün erfinden. Hosted GitHub x64 GUI Run 39825721705 COMPLETED/FAILURE Job 119988361983 runner_id 0, steps 0: kein Runner, kein Compilerstart; lokale Mint-x64-GUI-Prüfung dagegen wirklich PASS. Letzter SHA-exakt nachgewiesener Pi5 SUCCESS Alpha.774 Run 39824131466 SHA a68ca53. OpenGenesisLINK Servermain bd6bf4f unverändert. Echter Low382 AgentWearablesUpdate SerialNum UInt32-Wrap behoben und authentifizierte LLUDP-Paketwiederholungsreihenfolge per native Test geprüft. Echte Live-NexVerse Low158/COF/Outfit/Skin/WASD-Session bleibt still open. Projekt 39 %; NÄCHSTE ZEHN BLÖCKE Alpha.785–794, primär Linux Mint, Raspberry Pi nur CI. Kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 NUR ARM64-CI. Voller Linux Mint x64 GUI-Gate scripts/mint-viewer-validation.sh. Kein ungefragter Desktop-Login/Produktivdeploy.**

22.10.2026 Alpha.775–784: Pi5 CI Alpha.774 SHA a68ca53 Run 39824131466 SUCCESS; Hosted-x64 GitHub Job vor Runnerstart 0 Steps FAILURE; echter lokaler Mint-GUI-Gate grün. Servermain bd6bf4f unverändert. ECHTER Wearables-Readback-Fehler nach uint32-Rollover von Low382 SerialNum behoben: vormals <= blockierte nach 0xffffffff->0 neue authentifizierte Outfit-Rückmeldungen. Nun Modulo-Halbkreisordnung im wirklichen WorldModel nach Session/Agent/Region. Native Low382-LLUDP-Bytepaketfolge verifiziert Wrap, verzögerte Retransmits, Duplikate, mehrfache Sessions, Grenzwerte. Python-Vertrag und kompletter Linux-Mint-x64-GUI-Test vor Push. COF/Outfit/GPU-Skin/WASD authentifiziertes Grid-E2E bleibt still open. Projekt 39 %, nächste Alpha.785–794; docs/ALPHA775_784_LOW382_SERIAL_WRAP_AUTH.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.774, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.775–784.**

**ALPHA.774 SHA-GENAUER MINT-x64-BUILD UND GITHUB-CI-STATUS:** Feature-Code-SHA a68ca53e8c9a669a9994ec4d1bf28b3e2a7dc975 direkt auf Desktop Commander Linux Mint jam-MS-7D28 nach GitHub develop-current gepusht. Kompletter Linux-Mint scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg Debug x64 Vollbuild, 175/175 CTests (inkl. WorldModel COF-pro-Format, echter LLUDP Low158-Wire-Decoder und Original-Firestorm GL-Framebuffer), 381/381 Python-Tests, isolierter Xvfb Viewer 0.2.0-alpha.774 mit Original Firestorm 133 Bind-Gelenken, drei GPU Bodymeshes, OGL X11-Icon 128x128 und WM_CLASS PASS. GitHub Pi5 CI für exakten SHA Run 39824131466 zuletzt IN_PROGRESS, noch KEIN SUCCESS behaupten. Hosted GitHub Linux-x64 Run 39824131896 COMPLETED/FAILURE mit Job 119982788458 runner_id 0 und steps 0, kein Compilerstart, kein Quellcode-Buildfehler belegt. Letzter bestätigter Pi5-CI Erfolg: Alpha.764 SHA f2c53f3 Run 39814357748 SUCCESS. Vorherige Pi5 Alpha.754 Run 39812188923 später CANCELLED (nicht als grün bewerten). Aktueller OpenGenesisLINK-Servermain bd6bf4f: #273 Gruppen-JWT, #274 Gruppenland/NV$-Historie. ECHTER Low158-COF-Replay-Bug über wechselnde AppearanceVersion behoben und mit 3 nativen Pipelineebenen getestet. Keine authentifizierte NexVerse Live-Avatar-/Outfit-/Skin-/WASD-E2E-Sitzung, diese bleibt still open. Gesamtprojekt 39 %; nächste 10 Blöcke Alpha.775–784. Mint primär, Pi nur ARM64-CI, kein Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Desktop Commander Linux Mint jam-MS-7D28; /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 NUR ARM64-CI. Ganzes x64 GUI-Gate scripts/mint-viewer-validation.sh. Kein Produktivdeploy, kein ungefragter Start auf sichtbarem Desktop.**

22.10.2026 Alpha.765–774: vorherige SHA-genaue GitHub Pi5 CI Alpha.764 Run 39814357748 für f2c53f3 COMPLETED SUCCESS. Gehosteter x64 Run 39814357774 ohne Runner (0 Steps) FAILURE, lokale Mint-x64-GUI-Gates grün. OpenGenesisLINK Servermain bd6bf4f +2 Commits #273/#274 zu Gruppen-JWT und Firestorm Land/NV$; keine ungeprüfte Low158-Vertragsänderung behauptet. ECHTER COF-Skin-Replay-Bug: beim Wechsel AppearanceVersion 2 => 3 => 2 ging bislang das Version-2-Highwater verloren, so dass eine ältere Outfit-/Skin-TE angenommen werden konnte. Jetzt pro AppearanceVersion (uint8) separat 256 Highwater-Werte, Reset bei Login/Region, keine Legacy-Count=0-Ablehnung. Native echte WorldModel-, Low158-LLUDP-Wire/Decoder- und Firestorm-Original-OpenGL-Framebufferpixel-Tests belegen Nicht-Rücknahme aktueller Skin und Akzeptanz echt neuer Versionen. Python-Regressionsvertrag und vollständiger Mint-CMake/x64-GUI-Gate vor Push. Authentifizierter Live-Avatar Low158/COF/Outfit/Skin/WASD weiter still open, Projekt 39 %, nächste Alpha.775–784. Details docs/ALPHA765_774_COF_FORMAT_INTERLEAVE_GL.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.764, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.765–774.**

**ALPHA.764 SHA-GENAUER GITHUB- UND MINT-GUI-STATUS:** Alpha.764 Code SHA f2c53f302fa6c8dfcc00361590f14b6c19f510c4 direkt auf Linux Mint Commander jam-MS-7D28 nach GitHub develop-current gepusht. Vollständiger lokaler scripts/mint-viewer-validation.sh EXIT 0: echter vollständiger CMake/vcpkg Linux-x64 Viewer Debug Build; 175 von 175 native CTests inklusive wirklichem WorldModel Low158-Multi-COF-Antirollback und Firestorm-Originalkörper-OpenGL-Framebuffer-Pixeltest; 377 von 377 Python-Tests; Xvfb Viewer 0.2.0-alpha.764 mit 133 originalen Firestorm Gelenken/3 GPU-Originalmeshes und OGL X11-Icon 128x128 PASS. Neue GitHub Pi5 Source Check CI Run 39814357748 für exakt diesen SHA beim letzten Abruf QUEUED. GitHub Hosted Linux x64 CI Run 39814357774 zum selben SHA QUEUED, Job 119948820858 hatte runner_id=null, steps=0; kein Erfolg/Compilerfehler ohne Lauf behaupten. Vorherige Pi5 Alpha.754 Run 39812188923 für SHA 706b611 beim Abruf noch IN_PROGRESS. Letzter tatsächlich verifizierter Pi5 SUCCESS Run 39809196307 für Alpha.744 SHA cbdfd85. Servermain bcb528a ein Gruppen-Test-Commit #272, kein neuer Low158-Vertrag. Alpha.764 repariert versioniertes Multi-COF-Replay mit trailing COF=0 und verifiziert erhaltene echte Skin-Pixel. Authentifizierter NexVerse Low158/COF/Outfit/Skin/WASD Live-Inworld-E2E weiter still open. Projekt 39 %, nächste zehn Blöcke Alpha.765–774, Linux Mint primär, Pi nur CI. Kein Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28; /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi NUR ARM64-CI. Release-Gate scripts/mint-viewer-validation.sh (ganzer CMake/vcpkg x64 Build, 175 native Tests, Python, Xvfb GUI und OGL Taskbar-Icon). Kein Produktivdeploy/kein ungefragter interaktiver Viewer-Start.**

22.10.2026 Alpha.755–764: Der OpenGenesisLINK-Servermain bcb528ac (+1 Commit #272 zu Groups/SQL, keine Low158-Vertragsänderung) geprüft. Im echten selbstavatar Low158/WorldModel-COF-Replay-Pfad konnten mehrere AppearanceData-Zeilen mit trailing COF=0 die bisherigen Antirollback-Prüfungen umgehen und alte Skin-Bakes wiederherstellen. Jetzt wird der letzte aussagekräftige COF-Versionseintrag aus der gesamten originalen Low158-Liste verwendet; no-data und reine 0-Version Legacy-OpenGenesisLINK-Pakete bleiben kompatibel und löschen den COF-Highwater-Wert NICHT. Native echte Model- und Firestorm-Head/Upper/Lower-OpenGL-Pixeltests beweisen: alte Outfitfarbe bleibt verworfen; echte neuere COF-Version ändert reale GPU-Pixel. Python-Contracts hinzugefügt, kompletter Mint-x64-GUI-Gate vor Push. Echter autorisierter NexVerse-Live-Avatar samt COF/Outfit/Skin/WASD weiter still open, 39 % Gesamtfortschritt. Nächste zehn Blöcke Alpha.765–774. Details docs/ALPHA755_764_LOW158_MULTI_COF_SKIN_GL.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.754, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.755–764.**

**ALPHA.754 EXAKTER GITHUB-SHA UND MINT-GUI-CI:** Feature SHA 706b6117421c9b157c67f88058bda1c1903f4cc0 direkt auf Linux-Mint-Commander jam-MS-7D28 nach GitHub develop-current gepusht. Kompletter lokaler scripts/mint-viewer-validation.sh EXIT 0: echter CMake/vcpkg Linux-x64-Debug-Viewer, 175/175 native CTests inkl. GetTexture/GL-Pose-AvatarBakeStreamPriority, 373/373 Python-Tests, isolierter Xvfb Viewer 0.2.0-alpha.754 mit Firestorm-Skelett 133 Joints/3 Original-LLM-Meshes, X11 OGL Taskleistenicon 128x128 PASS. GitHub Pi5 Source Check Run 39812188923 für EXAKTEN Code-SHA zuletzt IN_PROGRESS; keinen Erfolg behaupten, bevor completed/success bestätigt. Hosted GitHub x64 CI Run 39812189063 COMPLETED/FAILURE, Job 119941137572 runner_id 0, steps 0: kein Runner, kein Compilerstart, lokaler Mint-Build unabhängig erfolgreich. Letzter SHA-exakter Pi5-CI Erfolg: Alpha.744 Run 39809196307 SHA cbdfd85 SUCCESS. Aktueller OpenGenesisLINK-Servermain 881b373 unverändert. Echter GetTexture-Bug: GPU-abgewiesene Avatarposition wird nicht mehr in Body-Bake HTTP/VRAM Budget, Self-Replan oder Full-Mip eingerechnet. Echte authentifizierte Live-Grid-Avatar Low158, COF, Skin, Outfit, WASD weiterhin still open. Projekt 39 %, NÄCHSTE ZEHN BLÖCKE Alpha.755–764, primär Linux Mint Commander, Raspberry Pi nur CI, kein Produktivdeploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Desktop Commander Linux Mint jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer Branch develop-current. Raspberry Pi NUR ARM64 CI; scripts/mint-viewer-validation.sh vollständiger x64-GUI-Gate. Kein ungefragter Start am sichtbaren Desktop und kein Deploy.**

22.10.2026 Alpha.745–754: Voriger Pi5 Run 39809196307 für Code-SHA cbdfd85 SUCCESS. Aktueller OpenGenesisLINK Servermain 881b373 unverändert. ECHTER Self-/Remote-Skin-Streaming-Fehler behoben: Renderer filterte ungültige Avatar-GL-Weltpositionen bereits, GetTexture-Priorität/VRAM-Mip-Samples, Self-Bake-Replan und Self-Full-Mip-Bindung verwendeten aber ungefilterte Proxys. Ein gemeinsamer pro-Frame GPU-Avatar-Admission-Adapter schließt abgewiesene Avatare jetzt aus den knappen HTTP-/512-GPU-Texturslots aus und behält echte Server-Head/Upper/Lower-Bakes gültiger eigener/remoter Avatare. Native Firestorm/GetTexture-C++23/VRAM-Regressionsprüfung und Python-Quellvertrag ergänzt; vollständiges Mint-x64-GUI-Gate wird vor Push geprüft. Authentifizierter Grid-Avatar, Low158/COF/Outfit/Skin/WASD-E2E weiter still open. Projekt 39 %, nächste Alpha.755–764, Details docs/ALPHA745_754_AVATAR_BAKE_GPU_STREAM_ADMISSION.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.744, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.745–754.**

**ALPHA.744 EXAKTER GITHUB-SHA UND LINUX-MINT-CI:** Feature SHA cbdfd857c00a6d9c3c897a5c2f3441223d080cf9 vom Desktop Commander auf Linux Mint jam-MS-7D28 nach GitHub develop-current gepusht. VOLLSTÄNDIGER lokaler Mint-x64-GUI-Gate scripts/mint-viewer-validation.sh EXIT 0: CMake/vcpkg Viewer Debug Build, 175/175 native CTests inklusive Single-Name-/Legacy-XML-RPC LoginService, 369/369 Python, isolierter Xvfb GUI Start 0.2.0-alpha.744 mit 133 Firestorm Skeleton Joints, drei Originalkörpermeshes und OGL X11 Taskleisten-Icon 128x128 PASS. GitHub Pi5 CI Run 39809196307 für exakt diesen SHA IN_PROGRESS bei dieser Prüfung, noch KEIN SUCCESS behaupten. Hosted Linux-x64-GitHub CI Run 39809196408 COMPLETED/FAILURE, Job 119930505086 runner_id 0, steps 0: kein Runner, kein Compilerstart. Letzter bestätigter Pi5-CI Erfolg: Alpha.734 Run 39662405115 SHA a31201f. Servermain 881b373 (#268) unterstützt Single-Username. OGL hat GUI/Backend/Serializer entsprechend ergänzt. Authentifizierter Avatar-Low158-/Outfit-/COF-/Skin-/WASD-Live-E2E weiterhin still open. Projektfortschritt 39 %, NÄCHSTE ZEHN BLÖCKE Alpha.745–754, Mint primärer Entwicklungsrechner, Raspberry Pi nur CI, kein Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Desktop Commander Linux Mint jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current; Raspberry Pi NUR ARM64-CI. scripts/mint-viewer-validation.sh voll ausführen. Kein ungefragter interaktiver GUI-Login und kein Deploy.**

22.10.2026 Alpha.735–744: Pi5 CI Run 39662405115 SHA a31201f SUCCESS. Servermain 881b373 (#265–271) behebt mit #268 den Firestorm-7.2.4-Login für einteilige Benutzernamen per XML-RPC username statt first/last. OGL hat diese Namen bisher im GLFW-Login abgewiesen. C++23-GUI/Backend jetzt für Single-Name-Account und klassische First/Last-Benutzer getrennt. XML-RPC sendet username ODER first/last; LoginService verwirft leere/widersprüchliche Namen. Passwort-Hash und explizite TOS-Consent-False/True bleiben korrekt. Native Parser-/FakeTransport-/Login-Tests, Python-Contracts und vollständiger Mint-x64-GUI-Gate. Authentifizierte Inworld- AvatarAppearance/COF/Skin/WASD weiterhin still open, Gesamtprojekt 39 %, nächste Alpha.745–754. Details docs/ALPHA735_744_SERVER_USERNAME_LOGIN_MINT.md. Kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.734, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.735–744.**

**ALPHA.734 CODE-SHA, MINT-VOLLTEST UND CI:** Feature-SHA a31201f2490e40041fa08fff6bc1c7fb8c725129 vom Linux Mint Commander jam-MS-7D28 auf GitHub develop-current gepusht. VOLLSTAENDIGER x64 GUI-Gate scripts/mint-viewer-validation.sh Exit 0, 175/175 native CTests (inkl. echtem 114-Byte LLUDP AgentUpdate-Wire-Test), 365/365 Python-Tests, isolierter Xvfb Viewer 0.2.0-alpha.734 inkl. Firestorm-Skelett/Originalmeshes und OGL X11-Icon 128x128 PASS. GitHub Pi5 CI Run 39662405115 zu EXAKTEM Code-SHA zuletzt IN_PROGRESS. Gehosteter Linux-x64 CI Run 39662405234 COMPLETED/FAILURE VOR START, Job 119438565129 runner_id 0, 0 Steps: KEIN CMake-/Compilerfehler nachgewiesen. Letzter GitHub-Pi5-CI SUCCESS: Alpha.724 Run 39660444288 SHA 09d1c0c. Aktueller OpenGenesisLINK-Servermain 86f293d (#264) welcome/search/destination aktualisiert; öffentliches get_grid_info HTTP 200 auf Mint. Echte WASD-AgentUpdate-Fehler durch NaN-Framezeit/Rotation sowie ungültigen CameraCenter korrigiert und echte seriell codierte Quaternion geprüft. Authentifizierter Live-Avatar/Low158/COF/Skin/WASD-Readback weiter still open. Projekt 39 %, NÄCHSTE ZEHN BLÖCKE Alpha.735–744, primär Linux Mint Commander, Pi NUR CI. Kein Deploy.

**VERBINDLICHER ENTWICKLUNGSRECHNER: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi 5 nur ARM64-CI. Grafik-Test via scripts/mint-viewer-validation.sh. Kein unbeauftragtes Deployment oder interaktiver Viewerstart.**

21.10.2026 Alpha.725–734: GitHub Pi5 Alpha.724 Run 39660444288 für Code-SHA 09d1c0c SUCCESS. Aktueller OpenGenesisLINK Servermain 86f293d (Commit #264) ändert Suche/Zieladresse und welcome in Robust.HG.ini; OGL welcome schon passend. Öffentliches GridInfo http://hg.stadt-nexverse.de/get_grid_info vom Mint-PC HTTP 200 mit Login und offizieller Welcome URL. ECHTER WASD-/AgentUpdate-FEHLER behoben: bei NaN als Framezeit wurden NaN-Yaw/Quaternionen in den LLUDP High4 Packet geschrieben. Neue finite Zeitbegrenzung (0 bis 0.25s), double-normalisierter gültiger Server-Quaternion-Reset und Fallback bei beschädigten Simulatorrotationen. Live-Input unterbricht AgentUpdate-Send bei NaN/Inf Simulatorposition statt ungültigem CameraCenter an Server zu schicken, ohne falsche simulierte Serverposition. Native C++23/Wirebytes/Python-Regressionen prüfen echte 114-Byte-LLUDP High4, Twist/WASD/Low-Level body rotation und lange Pausen. Volles Mint-x64 GUI-Gate für Release. Echter autorisierter Avatar/Outfit/Skin/COF/WASD-Inworld-Test weiter OFFEN. Fortschritt weiterhin 39 %, nächste Alpha.735–744. Details docs/ALPHA725_734_REAL_AGENTUPDATE_MOVEMENT_WIRE_SAFETY.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.724, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.725–734.**

**ALPHA.724 EXAKTE CODE-SHA UND CI:** Feature-SHA 09d1c0c8030361945fab0af2c306ec514f83eee4 direkt auf Linux-Mint-Commander jam-MS-7D28 getestet und nach GitHub develop-current gepusht. scripts/mint-viewer-validation.sh Exit 0: echter kompletter CMake/vcpkg-Debug x64 Build, 175/175 CTest inklusive Original-Firestorm OpenGL-Framebuffer mit ungültigem Self-/Remote-Avatar, 361/361 Python-Tests, isolierter Xvfb GUI Start Version 0.2.0-alpha.724 und OGL X11-Icon/WM_CLASS 128x128 PASS. GitHub Pi5 CI Run 39660444288 für exakten SHA zuletzt IN_PROGRESS. Hosted Linux x64 CI Run 39660444315 COMPLETED/FAILURE, Job 119431724689 runner_id 0 steps 0: NICHT gestartet, kein nachgewiesener Quellcode-Compilerfehler. Letzter SHA-exakt verifizierter Pi5 CI Alpha.714 Run 39655327950 SUCCESS für bdd247a. Neuer GL-Renderer verwirft Avatarposition NaN/Inf, korrigiert ungültige Quaternion, genaue Kamera-Hover-Diagnose und echte GPU-Pixel-Regression. Authentifizierter NexVerse Skin/Outfit/COF/WASD-Live-E2E weiterhin OFFEN. Arbeitsort verbindlich Linux Mint Commander, Pi NUR CI. Projekt 39 %, nächster Zehnerblock Alpha.725–734, kein Deploy.

**VERBINDLICHER ARBEITSORT: Desktop Commander Linux Mint jam-MS-7D28; /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Raspberry Pi 5 NUR ARM64-CI. scripts/mint-viewer-validation.sh FULL BUILD + 175 native CTests + Python + Xvfb/Taskbar Icon. Kein Produktivdeploy / kein ungenehmigter sichtbarer Desktop-Start.**

21.10.2026 Alpha.715–724: GitHub Pi5 Alpha.714 SHA bdd247a Run 39655327950 SUCCESS; OpenGenesisLINK servermain 31f3d129 unverändert. ECHTEN Avatar-GPU-Bug behoben: OpenGL-Pose übernimmt bisher ungeprüfte Position und Quaternion aus LLUDP/WorldModel; NaN/Inf konnte originale Firestorm-Skin-Mesh-Matrix kontaminieren. Neuer zentraler AvatarProxy GPU-Admission-Guard verwirft invalid Position, repariert ungültige Quaternion auf Identity und normalisiert große endliche Quaternionen in double, ohne Serverphysik anzutasten. Echter Renderer set_avatar_proxies, per Frame GL-Diagnostics und Avatar-HUD/F3-Warnung integriert. Kamera-Frustum-Diagnose misst jetzt dieselbe echte, Hover-bereinigte Self-Proxy-Position statt abweichende Agent-Physikposition. Native C++23 Tests und Original-Firestorm-Head/Upper/Lower OpenGL-Framebuffer Regression mit invalid Self und Remote-Pose bestanden. Nur synthetische GL-Pixeltests, authentifizierte Grid-/Skin-/COF-/WASD-Session weiter OFFEN. Projekt weiterhin 39 %, nächste zehn Blöcke Alpha.725–734. Details docs/ALPHA715_724_REAL_GPU_AVATAR_POSE_ADMISSION.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.714, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.715–724.**

**ALPHA.714 EXAKTER SHA / MINT-BUILD / GITHUB-CI:** Feature-SHA bdd247ae6e894c7a135f64735f7da42881d5ac9a vom Linux-Mint-Commander jam-MS-7D28 nach GitHub develop-current gepusht. Echter kompletter Linux-Mint x64 CMake/vcpkg Debug-Build und scripts/mint-viewer-validation.sh Exit 0; 174/174 native CTests, 356/356 Python-Verträge, isolierter OpenGenesisLINK-GUI-Start mit Original-Firestorm-Skelett und OGL WM_CLASS/Icon 128x128 PASS. GitHub Pi5 CI Run 39655327950 für diesen SHA zuletzt IN_PROGRESS; erst nach completed SUCCESS als grün markieren. Gehosteter Linux-x64-CI Run 39655327912 FAIL vor erstem Build-Schritt, Job 119413938353 runner_id 0 steps 0; KEIN Beleg für C++/CMake-Code-Fehler. Lokale Mint-GUI-Prüfung gesondert GRÜN. Neuer Servermain 31f3d129 erfordert offizielle /service/tos Bürgerordnung, die vorherige Login-Whitelist ist korrigiert und im Browser/Modal real verdrahtet. Real authentifiziertes Grid-Avatar-/Skin-/COF-/Outfit-/WASD-E2E weiter OFFEN. Arbeitsort in jedem neuen Chat Linux Mint Commander, Pi nur ARM64-CI. Gesamtprojekt 39 %, NÄCHSTE ZEHN BLÖCKE Alpha.715–724, kein Deploy.

**VERBINDLICHER ARBEITSORT: Linux Mint Desktop Commander jam-MS-7D28, /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, Branch develop-current. Raspberry Pi nur ARM64-CI. scripts/mint-viewer-validation.sh als grafischer Komplett-Gate. Kein Produktivdeploy oder ungefragter Start auf dem sichtbaren Benutzerdesktop.**

21.10.2026 Alpha.705–714: Pi5 GitHub Run 39652812099 für Alpha.704 Feature-SHA eceb506 SUCCESS. OpenGenesisLINK Servermain 31f3d129 ändert verbindlichen Bürgerordnungs-/TOS-Pfad zu https://stadt-nexverse.de/service/tos (#263). Der Viewer erlaubte bisher NUR /buergerverordnung und blockierte den aktuellen Serverlogin. Strenge explizite /service/tos-Allowlist neben älterer offiziell bekannter Legacy-Version, pro Server-Challenge normalisierter HTTPS-Browser-Link, Clipboard und sichtbarer Modaltext im echten OGL-Frontend. Authentischer Login XML-RPC-Test, Native-URL-Angriffs- und Python-Contracts erweitert, beide offiziellen Seiten per öffentlichem HTTPS-HEAD HTTP 200 bestätigt. Entwicklung, x64-GUI und Tests auf Linux Mint Commander. Authentifizierter Grid-/Skin-/COF-/Outfit-/WASD-E2E weiter OFFEN; Projekt 39 %, nächste zehn Blöcke Alpha.715–724. Details docs/ALPHA705_714_SERVER_TOS_CANONICAL_MINT_X64.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.704, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.705–714.**

**ALPHA.704 VERIFIZIERTER MINT-x64-GATE + GITHUB-SHA:** Alpha.704 Feature-Commit eceb5065ef886ce32255b4ac50c649d482ce84ba DIREKT über Desktop Commander Linux Mint jam-MS-7D28 gepusht. Auf Linux Mint reale CMake/vcpkg Debug x64 FULL BUILD PASS, 174/174 CTest, 353/353 Python, Xvfb GUI-Start inklusive Original-Firestorm-133-Joint-Rig/7186 Dreiecke, Viewer UI Renderer, X11 WM_CLASS und originales OGL Taskleistenicon 128x128 PASS. scripts/mint-viewer-validation.sh FULL-GATE Exit 0; kein echtes Live-NexVerse-Grid-Login/Avatar-Skin-/Outfit-/COF-/WASD-E2E. GitHub Pi5 CI Run 39652812099 für exakten SHA IN_PROGRESS bei letzter Prüfung. Hosted Linux-x64 CI Run 39652812111 COMPLETED/FAILURE, Job 119405263374 runner_id 0 und steps 0: KEIN CMake-Kompilierversuch, GitHub-Runner-Zuteilung weiterhin blockiert. Letzter SHA-genau bestätigter Pi5-CI Alpha.694 Run 39650618129 SUCCESS. Primärer Arbeitsort ab sofort und in jedem neuen Chat LINUX MINT COMMANDER jam-MS-7D28; Raspberry Pi nur CI. Projekt weiterhin 39 %, nächste 10 Blöcke Alpha.705–714; kein Produktivdeploy.

**VERBINDLICHES ENTWICKLUNGSGERÄT: Linux Mint Desktop Commander jam-MS-7D28, Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, develop-current. Raspberry Pi 5 nur ARM64 CI. Für GUI/CMake/vcpkg/NVIDIA-Tests primär scripts/mint-viewer-validation.sh auf Mint; niemals unaufgefordert auf sichtbarem Desktop starten oder deployen.**

21.10.2026 Alpha.695–704: Entwicklung auf ausdrücklichen Benutzerauftrag auf Linux Mint Desktop Commander jam-MS-7D28 verlegt. GIT sauber von d669213 nach 805fb3f ff-only synchronisiert. Echter Linux-x64-Debug-Vollbuild via vcpkg/CMake/Ninja PASS, 174/174 native CTest inkl. GL/Avatar/LLUDP/Login-TOS PASS, Xvfb-isolierter Viewer-Start mit original Firestorm-Skelett und 3 GPU-Meshes/UI Renderer PASS, X11 WM_CLASS und OGL Taskleisten-Icon 128x128 PASS, 353 Python-Verträge PASS. Echte x64-GCC -Wconversion Warnungen bei 64-Bit Avatar Skin GL Draws beseitigt (saturierende uint32_t-Diagnose); native Test auf uint64_t Extremwerte erfolgreich; Mint Desktop Full-Validation Gate script neu. GitHub Pi5 Alpha.694 SHA 4150521 Run 39650618129 SUCCESS. Hosted x64-GitHub-CI scheiterte weiterhin vor Runnerstart (0 Steps), lokale Mint-Validierung ist nicht Hosted-CI. Live authentifizierter NexVerse-Avatar/Outfits/COF/Skin/WASD weiter OPEN, Projekt 39 %, nächste zehn Blöcke Alpha.705–714. Details docs/ALPHA695_704_MINT_X64_VIEWER_GUI_GATE.md. Kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.694, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.695–704.**

**CI-STATUS ALPHA.694 NACHGEPFRUEFT:** Linux-x64-GUI-Run 39650618221 (SHA 4150521bd239c562d697802d3fae64f44de9f76f) COMPLETED/FAILURE vor der Jobausfuehrung; Job 119397662761 mit runner_id=0 und steps=0. Zweiter Hosted-Runner-Start ohne GUI-Konfiguration oder Compiler; Ursache nicht aus Jobs ableitbar. Pi5-Run 39650618129 fuer denselben Feature-SHA IN_PROGRESS; noch kein Success nachgewiesen. Echte Live-Grid-/GPU-/Avatar-Skin-/COF-/WASD-Tests weiter offen, Projektstand 39 %. Kein Deploy.

**ALPHA.694 CODE-SHA UND CI:** Feature SHA 4150521bd239c562d697802d3fae64f44de9f76f nach voller lokaler ARM64-Headless-Prüfung (inklusive OGL_AVATAR_HOVER_REAL_CAMERA_PROJECTION_RECOVERED_PASS und OGL_AVATAR_LOW158_HOVER_NAN_INFINITE_GPU_GUARD_PASS) und 351 Python-Tests (1 skip) gepusht. Neuer GitHub-Pi5-Run 39650618129 und Linux-x64-GUI-Run 39650618221 standen unmittelbar nach Push noch QUEUED, kein Erfolg vor COMPLETED/SUCCESS behaupten. Letzter SHA-exakt bestätigter Pi5-CI Alpha.684 39647670145 für SHA 0c38c10 SUCCESS. Voriger Hosted-GUI-Run 39647670237 hatte 0 Runner und 0 Steps, daher kein echter Linux-x64-CMake-Erfolg. Viewer-Build und authentifiziertes NexVerse Avatar-Skin/COF/Outfit/WASD GPU-E2E immer noch OFFEN. Projekt 39 %, nächste zehn Entwicklungsblöcke Alpha.695–704. Kein Produktivdeploy.

21.10.2026 Alpha.685–694: GitHub-Pi5-CI Alpha.684 Run 39647670145 SHA 0c38c10 SUCCESS bestätigt. OpenGenesisLINK-Servermain c9a0322 repariert die explizite TOS-Boolean true/1-Erkennung, OGL sendet bereits korrekten XML-RPC-Boolean bei tatsächlicher Zustimmung. KONKRETER AVATAR-BUG: NaN/Infinity aus Low158/Remote Hover.z wurde bisher mit std::clamp in AvatarProxy.z geschrieben und konnte den eigenen/Fremd-Avatar aus dem GL-Frustum entfernen. Guard im echten Renderer-Pfad auf 0m, gültiges Hover ±2m beibehalten; UI HUD Warnung für aktuelle Self-/Remote-Hover-Reparaturen, echte native Pi5 C++23-Tests inklusive Drittperson-Kameraprojektion, Python-UI-Vertragstests. Hosted Linux-x64-CI hatte Runner-ID 0 / 0 Steps; Full-GUI-Build weiter unbewiesen. Skin/Outfit/COF/WASD-Grid-E2E weiterhin offen. Projekt 39 %, nächste zehn Blöcke Alpha.695–704. Details docs/ALPHA685_694_AVATAR_HOVER_NAN_GL_VISIBILITY.md, kein Deployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.684, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.685–694.**

**ALPHA.684 SHA-CI:** Feature SHA 0c38c102a9f908562dd2f09141eb2cba80c14623 nach lokalem Pi5-ARM64 Headless SUCCESS und 347 Python Tests (1 skip) nach GitHub develop-current gepusht. Pi5 GitHub Run 39647670145 bei letzter Abfrage IN_PROGRESS, nicht als Success bestätigt. NEUE Linux-x64 GUI-CI Run 39647670237 meldet FAILURE, aber Job 119387436606 hatte KEINEN Runner (runner_id 0) und 0 Steps, also KEINERLEI CMake-Konfiguration/Compilation. Ursache des frühen GitHub-Jobabbruchs ungeklärt; kein beweisbarer Viewer-Compilerfehler. Letzter verifizierter Pi5-CI Alpha.674 Run 39645564317 SUCCESS. Grafischer x64-Build, Live-Grid Login/Avatar Skin/COF/Outfits/WASD bleiben OFFEN. 39 Prozent, nächste 10 Blöcke Alpha.685–694. Kein Produktivdeploy.

21.10.2026 Alpha.675–684: Servermain 3728273 OGLSeasons Mono.Addins-Fix geprüft, keine neue belegte Login/LLUDP-Schnittstelle. GitHub SHA-CI Alpha.674 Run 39645564317 SUCCESS. TOS-Retry nach Fehler/Exception entwertet alte Zustimmung; Challenge an eingereichten Avatar/LoginURI gebunden. Echtes sichtbares Login-Modal jetzt mit Tab/Shift+Tab/Enter/Space/Escape und Fokusmarkierung; globaler ESC-Window-Close-Fehler korrigiert. POSIX-Browserprozess wird reaped, Windows ShellExecuteA über shell32 verlinkt. NEUE GitHub Linux x64 GUI-CMake-CI für develop-current kompiliert auch den wirklichen Viewer und XMLRPC/LoginService-Tests (Build-Ergebnis für neuen Feature-SHA noch offen). Pi5 Native/Headless und Python-Tests, kein authentifizierter Grid-/GPU-Beweis. Stand 39 %, nächster Zehnerblock Alpha.685–694. Details docs/ALPHA675_684_TOS_RETRY_KEYBOARD_AND_X64_GUI_CI.md, kein Deploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.674, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.675–684.**

**ALPHA.674 EXAKTER FEATURE-COMMIT:** 24fb21c4c841e17bc2e399c219a6261a9eecdb8a (develop-current). Lokal Pi5 ARM64 Native/Headless mit LOGIN_TOS_GESTURE_SCOPE_SECURITY_AND_LAYOUT_PASS, weiteren C++23-Regressionen und 340 Python-Tests (1 skipped) PASS. Neuer GitHub Actions Run 39645564317 fuer denselben Feature-SHA am 21.10.2026 ca. 19:39 MESZ IN_PROGRESS – erst nach Completed Success als gruen eintragen. Letzter bestaetigter Pi5-CI bleibt Alpha.664 Run 39643087562 fuer SHA 0c9f60ef SUCCESS. WICHTIG: Die neue echte CMake-XML-RPC-Serializer-/Parser- und die sichtbare GUI-Framebuffer-/Browser- sowie produktive Server-TOS-Akzeptanz wurden im aktuellen Pi5-Headless-Lauf NICHT ausgefuehrt. NexVerse-Skin/COF/Outfit/WASD-E2E weiter offen; Projekt 39 %, naechste zehn Bloecke Alpha.675–684, kein Produktivdeploy.

21.10.2026 Alpha.665–674: Neuer OpenGenesisLINK Server-main 7569838 hat versionierte Bürgerordnung TOS im echten Login-Handler eingeführt (reason=tos). P0 im bisherigen OGL erkannt: jeder Login setzte agree_to_tos=true automatisch ohne Benutzerzustimmung. Ab Alpha.674 false bis zum echten Server-TOS-Challenge, eigener sichtbarer Bürgerordnungsdialog mit offizieller HTTPS-Seite, explizitem Zustimmen/Ablehnen und neuem Login mit Passwortneueingabe. Zustand nur für Avatar/Grid gültig, kein vorzeitiger Simulatorstart. Native C++23 Zustands-/Layouttests, CMake XML-RPC- und Python-Verträge ergänzt. Letzter bestätigter Pi5-CI Run 39643087562 (Alpha.664 SHA 0c9f60e) SUCCESS, neuer Alpha.674 GitHub-CI/GUI/NexVerse-E2E noch offen. Projekt 39 %, nächste zehn Blöcke Alpha.675–684. Details docs/ALPHA665_674_SERVER_TOS_EXPLICIT_LOGIN_CONSENT.md; kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.664, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.665–674.**

**ALPHA.664 SHA-GENAUE TESTLAGE:** Feature SHA 0c9f60ef02dbcc35796c7cdf637eb59b94ec48a9 wurde nach nativen Pi5 ARM64 WorldModel-/LLUUDP-/Pose-Regressionen (alle sechs WorldModel-Programme PASS), weiteren Headless-C++23-Prüfungen und 335 Python-Tests mit 1 Skip nach develop-current gepusht. GitHub Actions Run 39643087562 ist fuer exakt diesen SHA um 21.10.2026 19:22 MESZ IN_PROGRESS, KEIN success vor Abschluss behauptet. Voriger Alpha.654 SHA-Run 39639482493 COMPLETE SUCCESS. Der volle Linux-x64-NVIDIA-/Xvfb-/GUI-Viewer-Build und ein authentifizierter NexVerse-Live-Avatar/COF/WASD-End-to-End-Test wurden hier NICHT ausgefuehrt. Projekt 39 %, nächste zehn Blöcke Alpha.665–674. Kein Produktivdeploy.

21.10.2026 Alpha.655–664: gleicher Prim-UUID auf neuer Simulator-LocalID ohne vorheriges KillObject löscht alte Root-/Child-/Grandchild-Meshes, fehlerhafte Prim-/Avatar-Positionen und Quaternionen werden vor WorldModel-Umbindung und Self-WASD-Beweis zurückgewiesen. Echter Full/Compressed/High9 LLUDP-Guard und übersichtliche F7-Zähler (KillObject, ID-Reuse, UUID-Umzug, ungültige Szenendaten) parallel zum Code implementiert. Native C++23 WorldModel-/Headless-/Python-Vertragstests und CMake-Registrierung ergänzt; GitHub-Pi5 CI Alpha.654 Run 39639482493 für SHA 3fdfa60 SUCCESS bestätigt. Neuer Alpha.664 GitHub-SHA-CI und voller GUI/GPU-/NexVerse-Live-E2E-Beleg noch offen. Upstream main 0129e617, ungeprüfte 243 Commit-Diffs weiter MANUAL_REVIEW. Projekt 39 %, nächster Zehnerblock Alpha.665–674, KEIN Produktivdeploy. Details: docs/ALPHA655_664_SCENE_UUID_RELOCATION_AND_AVATAR_POSE_GATES.md.

---

**ALPHA.654 CODE-SHA 3fdfa60de01ea5d79c1b1e7e9fc96d85b16b82aa:** Pi5 ARM64 native WorldModel/LLUDP/TextureEntry, weitere native Headless-C++23-Prüfungen und 329/329 Python Tests (1 skipped) PASS. GitHub Actions Pi5-Lauf 39639482493 fuer GENAU diesen Feature-SHA ist laut API-Abfrage 21.10.2026 18:58 MESZ QUEUED (nicht bestanden oder fehlgeschlagen behaupten). Vollstaendiger x64-NVIDIA-GL-/Xvfb-GUI-Build und authentifizierter NexVerse Grid Avatar/COF/WASD End-to-End weiterhin offen. Letzte bestaetigte Pi5 GitHub CI ist weiterhin Alpha.644 Run 39636189352. Kein Produktivdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.654, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.655–664.**

21.10.2026 Alpha.645–654: Prim-ID/UUID-Reuse ohne KillObject löscht alte Mesh-Linkset-Nachfahren; getrennter F7-Zähler für Prim-Rebinds. Eigener Avatar-Kill oder LocalID-Neubindung setzt falsche frühere WASD-/Yaw-Serverbestätigungen zurück. C++23/ARM64/Headless-Regression, F7-Datenquelle und Quellcode-Verträge ergänzt. OpenGenesisLINK main 0129e617 hat 243 Commits seit reviewed-Baseline: MANUAL_REVIEW offen; kein ungeprüfter Baseline-Advance. Authentifizierter NexVerse E2E und GPU-/x64-Build im aktuellen Durchlauf noch offen. 39 %, nächster Zehnerblock Alpha.655–664. Details: docs/ALPHA645_654_PRIM_UUID_REBIND_AND_SELF_MOTION_LEASE.md. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.644, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.645–654.**

**ALPHA.644 SHA-GENAUE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39636189352 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 Headless und Python Job 119347199873 für exakten Feature-SHA 231446af047539527384b7e54113e82041cdcb67. Mint x64 vollständiger Build 391/391, native C++/OpenGL 167/167, Python 325/325, lokale native C++23/Python Headless-Tests, isolierter Xvfb-GUI-Viewer-Start und X11 WM_CLASS/Systemicon PASS. Die Pi5-CI belegt ARM64-native C++23-Headless/Python-Regressionen, KEINE Pi5-GPU-/GUI-Prüfung und KEINEN neuen authentifizierten NexVerse-Live-Grid-Login. EIGENER LLUDP KillObject LocalID schließt alte Self-Object-/Terse-Bewegungsbindung und alte provisorische Bake-Referenzen, erhält authentifizierte echte Low158. Full und Compressed Self-ObjectUpdate mit LocalID=0 dürfen nicht mehr eigene Bewegung oder Skin-Kandidaten binden. Native realer KillObject-Wire-Decoder/Session-Regression und Original-Firestorm-Head/Upper/Lower-GPU-Framebuffer: kein alter Skin-GL-Draw bei noch GPU-residentem Asset. F3/privater Report nur Source-Count ohne IDs. Authentifiziert sichtbarer eigener NexVerse Skin/COF/Outfit/WASD weiterhin NICHT E2E-verifiziert; 39 %. Nächste zehn Blöcke Alpha.645–654. KEIN Produktivdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.644, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.645–654.**

21.10.2026 Alpha.635–644: ECHTEN EIGENEN AVATAR-LocalID-Kill-Lebensdauerfehler im LLUDP-Dispatcher behoben: KillObject entfernte bisher andere Avatare und Prims, ließ aber self_avatar_local_id bestehen, wodurch verspäteter ImprovedTerseObjectUpdate dieser alten ID weiterhin eigene Rotation/Position und WASD als serverbestätigt ausgeben konnte. Neues aktuelles eigen-ID-/Regions-Gate im echten KillObject-Pfad setzt LocalID und Region bei echtem Kill zurück. WorldModel zieht nur provisorische, an diese LocalID gebundene Self-ObjectUpdate Bake-Referenzen zurück, authentifizierte echte Low158-AvatarAppearance/COF bleibt. Full und Compressed Self ObjectUpdate mit LocalID=0 dürfen Pose/Bewegung nicht binden. Reale Low-LLUDP-Wire-Kill-Tests, Session/Region-Rebind-Nativtests, Original-Firestorm-OpenGL-Framebuffer: NACH authentischem Self-Kill alte Skin-Bake GL draws=0 und NULL Ghost-Pixel trotz weiterhin GPU-residenter alter Textur. F3 und privater Bericht nur gültige eigene Kill-Anzahl, Reset pro neuer Auth-/Region-Epoch. Upstream Server main 171e814 geprüft. NEUER authentifizierter NexVerse Skin-/COF-/Outfit-/WASD-GPU-End-to-End-Befund weiterhin OFFEN, Projekt 39 %. Detail docs/ALPHA635_644_SELF_KILL_LOCALID_MOVEMENT_AND_SKIN_LEASE.md. Bei WEITER zehn Blöcke Alpha.645–654, KEIN Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.634, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.635–644.**

**ALPHA.634 SHA-GENAUE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39633638744 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 native headless/Python Job 119338158550, exakt für Feature-SHA ec8fa96605fb14b932ca1b70487cfa095ca7a57a. Mint x64 vollständiger Build 389/389, native C++/OpenGL 166/166, Python 318/318, lokale native C++23/Python Headless, isolierter Xvfb-GUI-Start, X11 WM_CLASS/Systemicon PASS. Pi5-CI ist ARM64-native Headless-/Python-Prüfung, KEIN Pi5-GPU/GUI-Test und kein authentifizierter Live-NexVerse-Login. Authentischer moderner Self Low158 mit nichtnull COF-Version: ältere Replay-Versionen derselben AppearanceVersion zurückweisen, gleiche/neue akzeptieren. Highwater pro Login/Region auch nach OpenGenesisLINK-Legacy Low158 ohne AppearanceData behalten; originale Firestorm Head/Upper/Lower-GPU-Framebuffer-Regressionskette belegt NULL alte Skin-Pixel bei verspäteter alter COF-Version und neuen farbigen Skin-Frame bei höherer Version. F3 und private Diagnose nur Anzahl verworfener Low158 COF-Replays ohne Asset-UUID. Echter angemeldeter NexVerse eigener Avatar, COF-Kleidung, Mesh und serverbestätigte WASD weiterhin NICHT end-to-end belegt; Gesamtprojekt 39 %. Nächste zehn Blöcke Alpha.635–644. Kein Produktivdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.634, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.635–644.**

21.10.2026 Alpha.625–634: ECHTEN Skin-/COF-Rollback im Self Low158 behoben. Eintreffende moderne AvatarAppearance-Pakete mit älterer COF-Version durften bislang bereits angenommene aktuelle gültige Skin/TextureEntry wieder überschreiben. Neue strikte monotone Version-Highwatermark im WorldModel bei realen nichtnull COF-Versionen derselben AppearanceVersion: ältere ablehnen, gleiche und neuere zulassen. Zwischenzeitliche echte OpenGenesisLINK legacy Low158 ohne AppearanceData/COF bleibt unterstützt und löscht die bekannte moderne Highwatermark nicht. Reset pro Auth-/Session-/Regionwechsel. Echte Low158-LLUUDP-Wire-Decoder-Cases 110/111/112 mit Replay/Legacy, original Firestorm Kopf/Ober/Unterkörper-OpenGL-GPU-Skin-Test: veraltete COF-Pakete verursachen NULL alte Skinpixel; neuerer COF-Stand sichtbar anderes Bild. F3 und privater Report mit reinem Anzahlzähler, keine Secret-/Outfit-IDs; vollständig authentifizierter NexVerse Skin/COF/Outfit/WASD-End-to-End-Nachweis weiterhin OFFEN, Status 39 %. Details docs/ALPHA625_634_MODERN_COF_LOW158_ORDERING_AND_GPU_SKIN.md. Bei WEITER zehn Blöcke Alpha.635–644, kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.624, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.625–634.**

**ALPHA.624 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39631143342 = COMPLETED SUCCESS; self-hosted Pi5 ARM64 Job 119329317595 für exakt Feature-SHA b33fde48bf82e3206f433130736d2abc8accb4bf SUCCESS. Mint x64 Vollbuild 389/389, C++/OpenGL 166/166, Python 311/311, ARM64-kompatible lokale C++23/Python Headless-Suite, isolierter Xvfb-Viewer und X11 Taskleistenicon/WM_CLASS PASS. Die Pi5-CI verifiziert ARM64 native C++23 Headless und Python, KEINE Pi5-GPU/GUI, KEIN auth NexVerse-Live-Login. Neue Source-Konsistenz: Full/Compressed-RemoteAvatar-ObjectUpdate mit wiederverwendeter LocalID löst alte Avatar-UUID und deren Attachment-Mesh-/Child-Subtree auf, selbst bei verlorenem UDP-KillObject. Bei selbem Avatar mit geänderter LocalID bleiben authentische AvatarAppearance/Low158/VisualParams/Outfit-Metadaten erhalten, nur alte ParentID-rooted Meshes invalidieren. Map-only Coarse, LocalID=0 und disconnected ObjectUpdate keine gültige GPU-/3D-Quelle. Echte KillObject-Zähler und implizite Rebind-Zähler getrennt (F7); nativer C++-Kreuz-UUID-/Skin-Metadatentest, original Firestorm-Joint-OpenGL-Framebuffer-Test mit NULL verwaisten alten Attachment-Pixeln bestehen. Vollständig bekleideter eigener Avatar/COF/Skin/WASD im authentifizierten NexVerse-Grid End-to-End weiterhin offen, Stand 39 %; nächste zehn Blöcke Alpha.625–634. KEIN Produktivdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.624, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.625–634.**

21.10.2026 Alpha.615–624: ECHTEN UDP-/Avatar-Identitätsfehler beheben: Bei wiederverwendeter Simulator-LocalID ohne eingetroffenes KillObject konnte alter fremder Avatar unter alter UUID samt 3D-Mesh-/Anhangkette weiterleben, obwohl ein neuer Full/Compressed Avatar ObjectUpdate dieser LocalID eine ANDERE UUID zuordnete. WorldModel::update_avatar_transform invalidiert bei echten Avatarobjektquellen die alte UUID und darunter hängende ParentID-/Mesh-Prims; neue UUID erhält die aktuelle LocalID. Rebind derselben UUID auf neue LocalID erhält aktuelle Appearance-/VisualParams-/Outfit-Metadaten, verwirft nur alte Anhang-Parent-Roots. LocalID=0/getrennter Circuit kein 3D-Nachweis, Coarse bleibt Karte. Separater F7-Zähler für UUID-/LocalID-Rebind (nicht mit echtem UDP KillObject gleichsetzen). Neue native 1:1-ObjectUpdate->WorldModel, originale Firestorm-Joint-OpenGL-Framebuffer-Tests einschließlich ZERO veralteter GPU-Pixel nach Rebind. Upstream OpenGenesisLINK-main 171e814 geprüft, keine neuen authentifizierten NexVerse Avatar-/COF-/WASD-Livelogs. Projektstatus weiterhin 39 %. Mehr docs/ALPHA615_624_REMOTE_AVATAR_LOCALID_UUID_LEASE.md. Bei WEITER zehn Blöcke Alpha.625–634; kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.614, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.615–624.**

**ALPHA.614 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39628852352 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 Job 119321248719 = COMPLETED SUCCESS für exakten Feature-SHA 8ba3f4a89b733a32a7a230d260572dc78f87aecc. Mint x64 Vollbuild 387/387, native C++/OpenGL 165/165, Python 305/305, isolierter Xvfb-GUI-Viewer, lokale ARM64-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5 testete ARM64-native C++23 Headless plus Python, KEINE Pi5-GPU/GUI und KEINEN authentifizierten NexVerse-Live-Login. Tatsächlicher LLUDP-KillObject wird nun auch auf bekannte Fremdavatar-LocalIDs und angehängte Root-/Child-/Grandchild-Prims angewendet. Unabhängige Welt/Avatar-Objekte bleiben; Map-only Coarse kann entfernten 3D-Avatar nicht durch alten Zustand resurrecten. Nativer echter KillObject Wire-Decoder und ursprünglicher Firestorm-OpenGL-Framebuffer belegen Remote-Kill ohne alten Avatar-/Anhangpixel. F7 zählt echte regionsgebundene Avatar-/Prim-Kills ohne UUID. Aktuelle Live-NexVerse-Skin, COF/Outfit und serverbestätigte WASD End-to-End weiterhin OFFEN; Fortschritt 39 %. Nächste zehn Blöcke Alpha.615–624, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.614, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.615–624.**

21.10.2026 Alpha.605–614: NACH priorisiertem Avatar-Admission-Fix weiteren ECHTEN LLUDP KillObject-Fehler gefunden und behoben: Simulator-Kill mit LocalID eines Fremdavatars löschte bisher nur world_model.objects() und ließ world_model.avatars() samt alter animierter Firestorm-3D-Meshes sichtbar. Aktuelle kill_simulator_local_id entfernt genau diesen Remote-Avatar, seine echten ParentID-rooted Attachment-Meshes und alle Kind-Links; die UUID anderer Avatare, unabhängige Weltobjekte und gültige aktuelle Skins bleiben unangetastet. Wiederholte/ungültige Kills erzeugen keine Renderrevision, Coarse-Reappearance bleibt nur Map-only. Native echte Low-KillObject-Wire-Pakete im WorldModel und originaler Firestorm-GPU-Pixeltest prüfen Ghost-Removal. F7 zählt echte Avatar-/Prim-Removals regions-/sitzungsgebunden ohne UUID. Aktueller OpenGenesisLINK Server main e215ea2, neue auth NexVerse Grid-Avatar-/COF-/WASD-Probe fehlt weiter. Status 39 %, Details docs/ALPHA605_614_KILLOBJECT_REMOTE_AVATAR_GHOST_LINKSETS.md. Nächste ZEHN Blöcke Alpha.615–624, KEIN Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.604, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.605–614.**

**ALPHA.604 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39626151169 = COMPLETED SUCCESS, selbstgehosteter Pi5 ARM64 Job 119311827201 = COMPLETED SUCCESS, für exakten Feature-SHA 8805576e6b26708f4250a77f2eb81ae1fbd0c3ec. Mint x64 vollständiger Build 385/385, native C++/OpenGL 164/164, Python 299/299, isolierter GUI-Xvfb Viewer, lokale Pi5-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5 testete ARM64-native C++23 Headless und Python, keine Pi5-GPU/GUI und KEIN authentifizierter Live-NexVerse-Login. CoarseLocationUpdate nur für bytequantisierte Kartenpositionen: 3D-Fremdavatar wird erst nach echtem Full/Compressed ObjectUpdate im aktuellen WorldModel admitted; echte genaue Pose/Low158-Appearance/COF wird von Coarse nicht ersetzt oder gelöscht. F7 trennt Original-Simulator-Avatar-ObjectUpdates von bloßen Kartenpunkten. Native realer Coarse-Wire-Decoder -> WorldModel/Identität/Region Test, original Viewer-UI-OpenGL bestanden. Keine aktuelle authentifizierte NexVerse Skin/COF/Outfit/WASD End-to-End-Bestätigung, Fortschritt 39 %. Nächste zehn Blöcke Alpha.605–614. KEIN Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.604, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.605–614.**

21.10.2026 Alpha.595–604: ECHTEN falschen Avatar-Admission-Fehler behoben: Grobe CoarseLocationUpdate-Kartenpositionen (quantisierte Bytes) wurden bisher als vollständige position_known-Avatare behandelt und als Firestorm-3D-Proxy gezeichnet, obwohl kein echtes Avatar-ObjectUpdate eingetroffen war. Neue Source-Flag simulator_object_update_seen wird nur durch Full/Compressed Simulator-Avatar-Pakete gesetzt; Coarse darf echte präzise Position, Skin/COF/Low158/Animationsdaten nicht ersetzen oder Avatar aus nicht-vollständigem Coarse-Snapshot löschen. Reiner Kartenpunkt bleibt ohne 3D-Beleg, kann bei Fehlen aus Coarse-Snapshot entfernt werden. Realer Coarse-LLUDP-Decoder zu WorldModel C++-Test mit Fremd-/Self-/Nil-/Duplikat-/Region-/Appearance-Invarianten, F7 UI zeigt echte Avatar-Objektpakete getrennt von reinen Kartenpositionen. Aktueller OpenGenesisLINK Server-main SHA e215ea2; keine frischen authentifizierten Grid-Logs verfügbar, nur Xvfb-Läufe. Authentifizierter eigener Avatar, Skin, COF, Outfit und serverbestätigte WASD-Bewegung weiterhin OFFEN, deshalb Projekt 39 %. Details docs/ALPHA595_604_COARSE_MAP_VS_REAL_AVATAR_OBJECT.md. Bei WEITER zehn Blöcke Alpha.605–614, KEIN Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.594, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.595–604.**

**ALPHA.594 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39488686178 = COMPLETED SUCCESS, Pi5-ARM64-Headless Job 118855705576 = SUCCESS, exakt für Feature-SHA c6ecd3fbd05218476ef1ed5e755cc5188378c1b6. Linux Mint x64 Vollbuild 383/383, native C++/OpenGL 163/163, Python 293/293, isolierter Xvfb GUI-Start, lokale Pi5-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5-CI prüft native ARM64 C++23 Headless/Python, KEINE Pi5-GPU/GUI oder authentifizierten NexVerse-Live-Login. Direct RegionHandle-Wechsel, Circuit-Disconnect und neue authentifizierte Agent-/Session-Identität löschen inzwischen alte Prims/Attachments, Fremd-Avatare, Terrain, Parcel, Environment mit Revision und neuer Szene-Epoch. Gleichgebliebene verbundene RegionHandshake/MovementComplete-Pakete bewahren Welt. F7 zeigt echte Szene-Epoch. Frische authentifizierte NexVerse Skin/COF/Outfit/Attachments/WASD-Grid-Abnahme NICHT vorhanden; Status 39 %. Nächste zehn Blöcke Alpha.595–604. Kein Produktivdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.594, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.595–604.**

20.10.2026 Alpha.585–594: REALEN Regionswechsel-/Disconnect-Ghost-Weltfehler behoben. WorldModel.apply_region hatte bei direktem Handlewechsel oder Circuit-Disconnect zwar Self Skin/Appearance entfernt, aber alte Welt-Prims/Anhänge, Fremd-Avatare, Terrain, Parzellen und Environment nicht. Jetzt ganze Regionsszene weg, Renderrevision und Regiongeneration erhöht, present=false. Gleichbleibende verbundene RegionHandshake-/AgentMovementComplete-Pakete bewahren Welt, wiederholte Disconnects erhöhen nicht ständig die Szene-Epoch. Reauth unter neuer Session/Agent verwirft alte regionale Meshes, Anhänge, Terrain und Avatare. Neuer nativer Lebensdauertest für Region/Disconnect/Reconnect/Session und F7-UI Szene-Epoch; vollständige Tests und Pi5 ARM64 SHA-CI folgen. Upstream Server 3f49996 nur Portal/HG-IM. Realer authentifizierter NexVerse-Skin-/COF-/Outfit-/WASD-Beweis OFFEN, Status 39 %. Detail docs/ALPHA585_594_DIRECT_REGION_LEASE_GHOST_SCENE.md. Bei WEITER 10 Blöcke Alpha.595–604, KEIN Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.584, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.585–594.**

**ALPHA.584 SHA-GENAUE Pi5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39467519712 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 Headless Job 118782677990 = SUCCESS, für exakten Feature-SHA 9b97aca5615a6942c4ca4521b86f46029fd9ad08. Mint x64 vollständiger Build 381/381, native C++/OpenGL 162/162, Python 287/287, isolierter Xvfb Viewer, lokale C++23/Python Headless und X11 Taskleistenicon/WM_CLASS PASS. Der Pi5-CI-Lauf prüft native ARM64 C++23 Headless/Python, keine Pi5-GPU oder GUI und keinen auth Live-NexVerse-Avatar. Wirkliche valide Self-ObjectUpdate TE ohne echte Bakes löst nur die alte PROVISORISCHE Skin; beschädigte oder verkürzte TE bleibt ohne Wirkung; gültige Low158 bleibt pro Face führend. Echte original Firestorm Body-Mesh-GL-Framebuffer-Regressionsbeweise für ausbleibende stale Skin bei noch GPU-residentem Texturasset; F3/privater count-only Herkunftsbericht pro Agent-/Regions-Epoch. Live NexVerse COF/Outfit/Skin/WASD weiter nicht bestätigt. Status 39 %, nächste zehn Blöcke Alpha.585–594, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.584, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.585–594.**

20.10.2026 Alpha.575–584: ECHTEN Self-Avatar-Skin-Lebenszeitfehler korrigiert. Eine neue GÜLTIGE vollständige ObjectUpdate TextureEntry mit ausschließlich Placeholder-/Default-Bake-Faces zieht bisher alte, provisorische Self-Bake-Auswahl zurück. Unvollständige oder beschädigte TE (auch Compressed Header) bleibt wirkungslos; Low158-Bake hat pro Face weiterhin Vorrang. Native sichere Placeholder-/Malformed-/Session-/Low158-Tests; tatsächliche Firestorm Head/Upper/Lower-OpenGL-Framebuffer-Regression belegt, dass allein Entfernen der Bake-Referenz ohne VRAM-Eviction keinen alten Skin-Draw mehr erzeugt. Sitzungs-/regionsbegrenzte F3- und privater Diagnosezähler ohne IDs. COF und echte NexVerse Avatar/Skin/Outfit/WASD-Live-Abnahme noch OFFEN, Fortschritt 39 %. Mehr in docs/ALPHA575_584_VALID_SELF_TE_BAKE_WITHDRAWAL.md. Nächste zehn Blöcke Alpha.585–594, kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.574, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.575–584.**

**ALPHA.574 SHA-GENAUE Pi5 ARM64 CI VERIFIZIERT:** GitHub Actions Run 39464572356 = COMPLETED SUCCESS, self-hosted Pi5 ARM64-Headless-Job 118772472994 = COMPLETED SUCCESS, für EXAKTEN Feature-SHA 2701cf5d0307373529901ee300d13f1a3eb3b0f2. Mint x64 vollständiger Build 381/381, native C++/OpenGL 162/162, Python 281/281, Xvfb GUI-Start, lokale Headless und X11 WM_CLASS/Systemicon PASS. Pi5 läuft ARM64 C++23 Headless/Python, nicht GPU/GUI und nicht authentifizierter NexVerse-Live-Login. Neu: CURRENT Simulator ObjectUpdate.State/ParentID erhält für tatsächlich empfangene Root-Prims Vorrang vor älterem Low158-Anhangblock, wodurch stale HUD-/Bone-Punkte, State=0 Detach und Avatarwechsel korrekt getrennt werden; nicht erhaltene Low158-Items bleiben PENDING und sind keine COF-/GPU-Erfolgsbeweise. Native und Firestorm Original-OpenGL-Framebuffer-Tests, F3 und private count-only Diagnostik PASS. Authentifizierter NexVerse Skin/COF/Outfit/Attachments/WASD E2E weiter OFFEN; 39 % (38,65 %). Nächste zehn Blöcke Alpha.575–584, KEIN Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.574, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.575–584.**

20.10.2026 Alpha.565–574: KORRIGIERTEN echten Attachment-Prioritätsfehler: älterer Low158-Anhang wurde bislang vor aktuellem prim ObjectUpdate.State eingefügt; aktueller Parent/State konnte via emplace nicht überschreiben, wodurch alte HUD-/Bone-/Detach-Angaben blieben. Gemeinsame Region-aktuelle reconcile_avatar_attachments priorisiert echte bekannte ObjectUpdate-UUID/ParentID/State bei SELF und fremden Avataren; State0/Avatarwechsel löst stale Low158, unbekannte Low158-Objekte bleiben als pending nur vorgemerkt. Native echte Bindings-/HUD-/Detach-Tests und OpenGL-Framebuffer mit alt Low158=HUD, aktuellem Prim=Schulter belegen Original-Firestorm-GPU-Bone. F3/privater Report zeigen Effektiv-/Pending-/Stale-Zahlen ohne IDs. Authentifizierter Live-NexVerse Avatar/Outfit/COF/WASD weiterhin offen; 39 %. Docs/ALPHA565_574_SERVER_ATTACHMENT_SOURCE_PRIORITY.md. Bei WEITER genau 10 Blöcke Alpha.575–584; kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.564, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.565–574.**

**ALPHA.564 EXAKTE PI5 ARM64 CI VERIFIZIERT:** GitHub Actions Run 39460543881 = COMPLETED SUCCESS, Pi5-ARM64-Headless-Job 118758562602 SUCCESS exakt für Feature-SHA 498c90571b6315da6a8e022ecbb2f6ff0633c61b. Linux Mint x64 Vollbuild 381/381, native C++/OpenGL 162/162, Python 274/274, isolierter GUI-Xvfb Viewer, ARM64-kompatible Headless Suite und X11 Taskleistenicon PASS. Pi5 hat ARM64 native C++23 Headless und Python geprüft, keine Pi5-GPU/GUI und keinen authentifizierten NexVerse-Live-Login. Fix für 64-Byte Compressed-Object-Header: kein Phantomprim/UUID-Recycling von Mesh/Parent/TextureEntry, nur aktuelle passende LocalID+UUID+PCode State/Pose, State=0 löst Avatar-Anhang. Echte Firestorm-GL-Anhangpixel und LLUDP Wire-Header nativ geprüft. Live Skin/COF/Outfit/Attachments/WASD E2E weiterhin offen, Projekt 39 %. Nächste zehn Blöcke Alpha.565–574, keine Produktion.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.564, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.565–574.**

20.10.2026 Alpha.555–564: REALEN 64-Byte ObjectUpdateCompressed-Headerfehler behoben. Gekürzte Pakete konnten Phantomobjekte erstellen oder bei LocalID-Wiederverwendung alte Parent-/Mesh-/TextureEntry-Daten auf fremde UUID übertragen. WorldModel::apply_object_compressed_partial schützt aktuelle Region und exakte LocalID+UUID+Prim-PCode; aktualisiert NUR State/AttachmentPoint und Pose, behält alle echten vollständigen Material-, Mesh- und Parent-Daten. State=0 detach; fremde/duplizierte/ungültige Pakete verwerfen. Echte Compressed-Wire-Decoder-Testkette und original Firestorm-Schulter-Anhang OpenGL-Pixel-Test bestehen. Upstream Server-main 95c2133 änderte nur Statistik/Hypergrid-API, nicht Avatar-Protokoll. Frischer auth Live-NexVerse Skin-/COF-/Outfit-/WASD-Beweis weiterhin offen, Fortschritt 39 %. Details docs/ALPHA555_564_COMPRESSED_PARTIAL_ATTACHMENT_IDENTITY.md. Bei WEITER zehn Blöcke Alpha.565–574; kein produktives Deployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.554, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.555–564.**

**ALPHA.554 PI5 ARM64 CI EXAKT VERIFIZIERT:** GitHub Actions Run 39457621156 = COMPLETED SUCCESS, Pi5 Headless-Job 118748503752, für Feature-SHA 6e85be50e84e685f3049bf90dc6834746ea468c9. Mint x64 vollständiger Build 379/379, native C++/OpenGL 161/161 PASS, Python 266/266 PASS, ARM64-kompatibles Headless, isolierter Xvfb Viewer und X11 Systemicon/WM_CLASS PASS. ECHTE High9 ImprovedTerse-Pose löscht nicht mehr AttachmentState, geparsten TextureEntry, Material/Mesh/ParentID; unbekannte Objekte werden nicht als Phantomprim erstellt. Original Firestorm-Rig gl-Framebuffer-Test belegt veränderte Pixel bei solcher Bewegung. Pi5-CI ist ausschließlich ARM64 C++23 Headless/Python, keine Pi5-GUI und kein auth Live-NexVerse. Avatar-Skin/COF/Outfit/Bewegung im realen NexVerse-Grid noch NICHT aktuell end-to-end bestätigt; Fortschritt 39 %. Nächste zehn Blöcke Alpha.555–564, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.554, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.555–564.**

20.10.2026 Alpha.545–554: WICHTIGEN realen High9/ImprovedTerse-Viewerbug behoben: Terse-Pose-Paket wurde bisher als neuer ObjectState gespeichert, dadurch gingen echter State/AttachmentPoint, geparster TextureEntry, andere Mesh-/Materialdaten verloren; unbekannte LocalIDs konnten Phantomobjekte erzeugen. Neue regions-/identitätsgebundene WorldModel::apply_object_terse ändert nur Pose bekannter Prims, lehnt NaN, unplausible Rotationen, alte/killed/unknown Objekte sowie reine Wiederholungen ab. Native volle Mesh/TE/State/Parent-Regression und echter GPU-Framebuffer-Test für bewegten Firestorm-Schulteranhang PASS. Kein COF/Wear-Eingriff, Live NexVerse Avatar-/Skin-/Outfit-/WASD-End-to-End noch offen, Status weiterhin 39 %. Doku docs/ALPHA545_554_IMPROVED_TERSE_ATTACHMENT_POSE_SAFETY.md. Nächste 10 Blöcke Alpha.555–564. Kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.544, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.545–554.**

**ALPHA.544 PI5 ARM64 CI EXAKT VERIFIZIERT:** Feature-SHA 8a8c0b3d1415bdca3d05eedd47a88f4e20db7360 wurde mit self-hosted Pi5 GitHub Actions Run 39455400291 = COMPLETED SUCCESS und ARM64 Source-Job 118740824512 geprüft. Mint x64 vollständiger Build 377/377, native C++/OpenGL 160/160 PASS, Python 258/258 PASS, isolierter Xvfb Viewer, Headless C++23/Python und X11 WM_CLASS/Systemicon PASS. Pi5 ARM64 ist Headless, KEIN GPU-GUI oder authentifizierter Live-Grid-Test. Echte OpenGenesisLINK-Objektanhänge wurden aus Full/Compressed ObjectUpdate.State ParentID aktueller Avatar LocalID in Firestorm-Skelett/GL-Render integriert, Low158-Anhangdaten haben Vorrang, HUD wird aus 3D-Szene ausgeschlossen. Eigene echte NexVerse COF-Kleidung/Avatar/WASD weiterhin NICHT end-to-end bestätigt; deshalb Projekt weiterhin 39 %. Nächste zehn Blöcke Alpha.545–554. Kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.544, Fortschritt 39 % (38,65 % gewichtet). NÄCHSTE Alpha.545–554.**

20.10.2026 Alpha.535–544: ECHTEN OpenGenesisLINK-Legacy-Attachmentfehler korrigiert. Server-main 6e446b9 sendet Low158 OHNE AttachmentBlock, Root-Prims liefern aber den Anhangpunkt nibble-swapped in normaler/compressed ObjectUpdate.State (z. B. Schulter 0x10, HUD 31 -> 0xF1). Viewer ignorierte dieses Byte und konnte Anhangobjekte im Self-Rig nicht zuordnen. Jetzt Decoder, WorldModel und echter Firestorm-Rig-Renderer für Selbst-/Fremdavatar integriert: nur aktuelle Prim ParentID=Avatar LocalID und erlaubter Slot 1..55, expliziter Low158-Punkt gewinnt. Native Wire-, HUD- und echter animierter OpenGL-Pixeltest. F3 und privater Report zeigen Obj-Anhaenge ohne UUIDs. Kein COF-/Wear-Eingriff, frischer Live-Avatar/GPU-Outfit/WASD noch offen, Status 39 %. Detail docs/ALPHA535_544_LEGACY_AVATAR_OBJECT_ATTACHMENTS.md. Bei WEITER zehn Blöcke Alpha.545–554. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.534, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.535–544.**

**ALPHA.534 SHA-GENAUER Pi5 ARM64 CI-ERFOLG:** GitHub Actions Run 39302449365 = COMPLETED SUCCESS exakt für Feature-SHA 8c6a60aba7e5c04cdf187bc0e48c782ce49d4738. Mint x64 375/375 Build, native C++/OpenGL 159/159 CTest, Python 251/251 PASS, isolierter Xvfb Viewer, native C++23/Python Headless und X11 Taskleistenicon PASS. Self-hosted Pi5 prüfte echte ARM64-C++23-Headless- und Python-Regressionssuite (kein vollwertiges OpenGL-GUI auf Pi). OpenGenesisLINK Server SHA 6e446b9 ScenePresence TURN_LEFT+YAW_POS/TURN_RIGHT+YAW_NEG korrigiert, aus echtem LLUDP-Wire validiert; neuer TurnReadbackMonitor belegt nur danach echte korrekte eigene Simulatorrotation. Real authentifizierter NexVerse WASD-/COF-/Avatar-/Outfit-End-to-End-Test noch nicht erfolgt, daher 39 %. Nächstes Paket zehn Blöcke Alpha.535–544; keine Produktionsbereitstellung.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.534, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.535–544.**

19.10.2026 Alpha.525–534: Aktuellen OpenGenesisLINK-Server-main SHA 6e446b9 geprüft. Echter Bewegungsfehler: TURN_LEFT musste YAW_POS statt YAW_NEG senden, TURN_RIGHT YAW_NEG statt YAW_POS; Drehung und Flugrolle bisher gegensätzlich. Korrigiert und mit vollständigem echtem LLUDP-AgentUpdate-High4-114-Byte-Payload getestet. W/S, A/D und Drehtasten-Gegeneingaben bereinigt, diagonale Bewegung erhalten. Neuer threadgesicherter TurnReadbackMonitor prüft nur NACH real gesendetem UDP und nur echte eigene Simulator-Rotation aus ObjectUpdate/Compressed/Terse bei >=5 Grad korrekt gerichtet. F3 zeigt Drehung ausstehend oder Server bestätigt, keine fingierten ACKs. Re-Login, Logout, Regionwechsel löschen Evidence. Ursprüngliche Skin/Outfit/COF und vollständige WASD-End-to-End-Bewegung im neuen Grid-Build weiter NICHT getestet, Projekt weiterhin 39 %. Detail docs/ALPHA525_534_REAL_OPENGENESIS_YAW_AND_SERVER_TURN.md. Nächste zehn Blöcke Alpha.535–544; zuerst GitHub, aktuelle Übergabe und Upstream prüfen. Kein Produktionsdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.525–534, Projektfortschritt 39 %.**

**ALPHA.524 PI5 ARM64 SHA-CI VERIFIZIERT:** GitHub Actions Run 39299889677 = COMPLETED SUCCESS und Self-hosted ARM64-Pi5-Job 118222412702 = SUCCESS für EXAKTEN Feature-SHA 899f53ee7e13f5a410433e221c51b780d8485b63. Mint x64 Vollbuild 375/375, native C++/OpenGL-Tests 159/159, Python 244/244, x64 Headless und isolierter Xvfb-Viewer sowie echtes X11-Systemicon PASS. Pi5-CI prüft C++23/Python ARM64 Headless, keinen Pi5-GPU- oder Live-NexVerse-Avatar. Spezifischer echter J2C-Tile-Part-Fehler erhält nur EINEN sicheren frischen Downloadversuch; drei unterschiedliche Firestorm-original Head/Upper/Lower GL-Bakes mit GPU-Pixeländerung/Slot-Eviction real getestet. F3 J2C-Rettungszähler erst nach erfolgreichem GPU-Upload, keine Daten erfunden. Keine aktuelle authentifizierte NexVerse Avatar-/COF-/Outfit-/WASD-End-to-End-Abnahme; Fortschritt 39 % (38,65 % gewichtet). Weiter zehn Blöcke Alpha.525–534. Kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.524, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.525–534.**

19.10.2026 Alpha.515–524: ECHTEN früheren NexVerse-GetTexture-JPEG2000-Fehler aus Viewerlog behoben: Für erkannten rohen J2C-Stream mit genau dem Tile-part length/stream-length-Mismatch maximal EIN zusätzlicher frischer Server-GET, alte Transport-Limits und Backoff unangetastet; wenn erneut defekt, korrekter Invalid-Fehler ohne Pixel. Native Testfälle temporär/permanent kaputtes JPEG2000-Tileteil und unzulässiges Format. Auf original Firestorm Head/Upper/Lower .llm werden nun drei VERSCHIEDENE RGB-Skin-Bake-Texturen samt Upper-Replacement und Head-Eviction mit tatsächlichen Framebuffer-Pixeln nachgewiesen. F3 nennt J2C-Retry-Erfolge nur bei real erfolgreichem GPU-Upload und wird beim Regionwechsel zurückgesetzt. Kein aktueller Live-Login-/Outfit-/WASD-Beweis: Projekt weiterhin 39 %. Volltext docs/ALPHA515_524_J2C_TRANSFER_RECOVERY_AND_SKIN_PIXELS.md. Bei WEITER genau 10 Blöcke Alpha.525–534. GitHub-Stand/Server/CI zuerst prüfen, kein Produktionsdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). NÄCHSTE Alpha.515–524, 39 %.**

**ALPHA.514 Pi5 ARM64-CI VERIFIZIERT:** GitHub Actions Run 39296348575 = COMPLETED SUCCESS exakt für Feature-SHA bcc02ddd046bc4133aa8f5b5875996452077835c. Mint x64 Build 375/375 PASS, native C++/OpenGL CTest 159/159 PASS, Python 237/237 PASS, ARM64-kompatibles Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon PASS. Pi5 prüft native ARM64 Headless C++23 + Python, keine native Pi5 GPU/GUI und keinen NexVerse-Live-Login. Authentifizierte Body-Bake-ID-Änderungen führen jetzt ohne 750-ms-Verzögerung zur EXISTIERENDEN GPU-Residency-Neuplanung, einschließlich Eviction alter Bakes; HTTP Retry/Backoff/VRAM-Budget bleibt unberührt. Keine neue bestätigte sichtbare NexVerse Skin/Kleidung/COF/Attachments/WASD, deshalb 39 % (38,65 % gewichtet). Nächste 10 Blöcke Alpha.515–524. Kein Produktionsupdate.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.514, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.515–524.**

19.10.2026 Alpha.505–514: Authentifizierte eigene Head/Upper/Lower-Bake-Änderungen planen jetzt sofort die ECHTE bestehende GetTexture/GPU-Residency-Queue neu (nicht erst mit allgemeinem 750-ms-Timer). Region-/Agentlease/Epoch schützen vor fremden Quellen, identische Body-IDs vermeiden Wiederholungen, entfernte Bakes führen wieder zur VRAM-Eviction. Drei echte Body-IDs verdrängen unter kleinem GPU-Slotbudget alte Texturen; wegfallende Faces machen Weltmaterial wieder möglich. HTTP-Pacing/Retry/VRAM-Schutz bleiben vollständig erhalten. F3 unterscheidet vorläufige Objektquelle bei fehlender und partieller Low158. Keine COF-/Wear-Mutation, kein neuer authentifizierter Grid-GPU-/Outfit-/WASD-End-to-End-Beleg, Status weiterhin 39 %. Details docs/ALPHA505_514_AVATAR_BAKE_EVENT_REPLAN.md. Bei WEITER Alpha.515–524 zehn echte Blöcke. Kein produktives Viewer-Update.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.504, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.505–514.**

**ALPHA.504 Pi5-CI EXAKT BESTÄTIGT:** GitHub Actions Run 39294296008 ist COMPLETED SUCCESS für Feature-SHA 7e8651bc04c47605652f58786fbc9b559e47f343. Mint x64 373/373 Build, 158/158 native C++/OpenGL, 230/230 Python, Headless C++23/Python, Xvfb GUI-Start und X11 Systemicon/WM_CLASS erfolgreich. Pi5-Lauf prüft ARM64 Headless C++ und Python, **nicht** Pi5 GPU/GUI und keinen NexVerse-Live-Login. Neuer Source-Fallback: echte Low158-Bake-ID gewinnt je Face; nur fehlende Faces aus Session/Region/LocalID-authentifiziertem ObjectUpdate vorläufig ergänzen, F3 zeigt GPU/Quelle getrennt. Kein endgültiger Outfit-/COF-/Skin-/WASD-Livebeweis; 39 %. Nächste 10 Blöcke Alpha.505–514, keine produktive Bereitstellung.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.504, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.505–514.**

19.10.2026 Alpha.495–504: Renderlücke behoben: Self Low158 ohne einzelne Bake Head/Upper/Lower IDs unterdrückte auch echte eigene ObjectUpdate TE-Bakes. Jetzt pro Face Low158 bevorzugen, ausschließlich fehlende Faces aus aktueller selbstauthentifizierter ObjectUpdate-TE provisorisch ergänzen. Reale Render-Auswahl integriert; F3 nennt GPU geladen/fehlt aus der provisorischen Quelle, getrennt von autoritativer Low158-/COF-Evidenz. Konflikt-UUIDs, Self-LocalID- und Agentwechsel nativ geprüft. Keine Outfit-Writes. Keine neue NexVerse Avatar-/Outfit-/GPU-/WASD-E2E-Bestätigung; weiterhin 39 %. Details docs/ALPHA495_504_SELF_BAKE_FACE_FALLBACK.md. Nächste zehn Blöcke Alpha.505–514, GitHub/Upstream/CI zuerst, kein Produktivdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.494, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.495–504.**

**ALPHA.494 Pi5 ARM64 CI VERIFIZIERT:** Entwicklungs-SHA 754339d420eb689dc977a0c7a3b5be52b3b9af21 wurde mit GitHub Self-hosted Pi5 Source Check Run 39292283612 = COMPLETED SUCCESS exakt auf diesem Quellstand geprüft. Mint Linux-x64 Build 373/373 PASS, Native C++/OpenGL 158/158 PASS, Python 223/223 PASS, isolierter GUI-Xvfb PASS, Headless C++23/Python PASS, X11 Taskleistenicon/WM_CLASS PASS. CI Pi5 baut keine native GUI oder GPU. Neues serverformatiges synthetisches Low158 mit drei Body-Bake-TE-Overrides erreicht Self-WorldModel/Streamingpriorität; F3 und privater Bericht melden fehlende Bake-IDs unabhängig von GPU-Darstellung. Tatsächlicher authentifizierter NexVerse Self-/COF-/Outfit-/WASD-End-to-End noch nicht belegt. Projekt 39 %; nächster Zehnerblock Alpha.495–504; kein Produktionsdeployment.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.494, Projektfortschritt 39 % (38,65 % gewichtet). NÄCHSTE Alpha.495–504.**

19.10.2026 Alpha.485–494: Alpha.484 echte OpenGenesisLINK Low158-Legacy-Decodierung durch neue vollständige serverformatige Testkette mit Head/Upper/Lower UUID-Face-Overrides (Firestorm 8,9,10), zuverlässigem zerocodiertem LLUDP, TE-Parser, authentifiziertem Self-WorldModel, Streamingpriorität und GPU-Asset-Residency-Plan erweitert. Agentwechsel invalidiert alle drei. F3 unterscheidet keine oder teilweise Server-Bake-IDs von vorhandenen Bake-IDs bei noch fehlenden GPU-Draws. Privater Diagnosebericht zeigt aktuelle Self-Low158-Head-/Upper-/Lower-Bake-Referenzen nur als Zähler/Bools, keine UUIDs. Live Grid/GPU-Avatar/Outfit/WASD noch NICHT verifiziert: Fortschritt 39 %. Siehe docs/ALPHA485_494_LOW158_BODY_BAKE_SOURCE_AND_F3.md. Bei WEITER Alpha.495–504 genau zehn echte Blöcke. Branch develop-current, GitHub SHAs und Upstream prüfen, kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Nächste Alpha.485–494, 39 %.**

**ALPHA.484 PI5 ARM64 CI EXAKT VERIFIZIERT:** Feature-SHA 4afbfbbbc670bacc32b949ee4e1295f8b5b1f4eb, GitHub Actions self-hosted Pi5 Run 39290341106 = completed SUCCESS für exakt diesen SHA. Linux Mint x64 Vollbuild 373/373, Native/GL 158/158, Python 215/215, Headless-native PASS, Xvfb isolierter GUI-Start und X11 Taskleistenicon PASS. Der Pi5-CI umfasst ARM64 C++23 Headless und Python, keinen echten ARM GPU- oder autorisierten NexVerse-Live-Login. Sehr wichtiger echte Upstream Low158 Decoderfix: AppearanceHover 1 ohne AttachmentBlock muss akzeptiert werden. Echte Welt/Avatar mit COF-Bakes/Outfit und WASD nach Release erneut prüfen. Fortschritt 39 % (38,65 % gewichtet), kein produktives Deployment.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.484, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.485–494.**

19.10.2026 Alpha.475–484: WICHTIGER REALER OPENGENESISLINK-SERVERFEHLER IN OGL BEHOBEN. Server-main 293d1d6 LLClientView.SendAppearance sendet Self-Low158 mit AppearanceHover(1), aber KEIN AttachmentBlock-Feld. Alter Viewer verweigerte diese gültige Serverantwort als malformed attachments und verlor reale Skin. Jetzt vollständig geparstes Legacy Hover ohne Anhangblock akzeptiert; moderne Anhänge und abgeschnittene Daten streng geprüft. Eigenes Flag attachment_block_present trennt Legacy ohne Block von modern mit explizitem 0; F3/privater Diagnosebericht/Logs zeigen korrekten Status. Native reliable+zerocoded Serverformat Low158 -> LLUDP -> Self WorldModel erfolgreich, Native C++/GL/Python/Headless, Xvfb, Icon und Pi5 CI. Echtes NexVerse Avatar/Outfit/COF/GPU/WASD weiterhin NICHT neu live verifiziert, Status 39 %. Dokumentation docs/ALPHA475_484_OPENGENESISLINK_REAL_LOW158_LEGACY_WIRE.md. Nächste zehn Blöcke Alpha.485–494. Bei WEITER GitHub Branch/CI und Server upstream zuerst prüfen, keine Produktionsbereitstellung.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.475–484, Fortschritt 39 %.**

**ALPHA.474 PI5 ARM64 CI VERIFIZIERT:** GitHub Run 39288055994 = completed SUCCESS fuer EXACT Feature-SHA a0785f24d77d9c8699ebb80ac27f02489b8b81e2. Mint x64 Build 373/373, native/OpenGL CTest 158/158, Python 207/207, ARM64-kompatibles Headless, isolierter Xvfb-Viewer sowie echtes X11 Taskleistenicon/WM_CLASS PASS. Offizieller Pi5-Lauf nur Headless C++23/Python, KEIN Pi5-OpenGL-GUI-Build. Kein echter authentifizierter NexVerse-Self-Skin-/Outfit-/COF-/WASD-End-to-End-Nachweis, Projektfortschritt weiter 39 % (38,65 % gewichtet). Naechste 10 Bloecke Alpha.475–484. Kein automatisches Produktionsupdate.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.474, Projektstand 39 % (38,65 % gewichtet), NÄCHSTE Alpha.475–484.**

19.10.2026 Alpha.465–474: F3-Avatarfenster hat jetzt echte Outfit-Schaltfläche [O] sowie O-Tastaturkürzel zum vorhandenen F9-Outfit-Floater, inkl. echtem Hit-Test und kompaktem responsivem Layout. Low382/COF-Diagnose zeigt nun getrennt anders, fehlend, zusätzlich, doppelt und offene Einträge und schneidet den Text 'kein Wear-Beweis' nicht ab. FetchInventory2-Originalitems zusätzlich an AgentID gebunden, kein Cross-Login-COF-Readback. Native C++/OpenGL, Python, Headless Pi5 ARM64, Xvfb, Systemicon, GitHub. Noch immer KEIN authentifiziertes NexVerse Avatar-/Outfit-/GPU-/WASD-E2E – Fortschritt 39 %. Details docs/ALPHA465_474_F3_F9_OUTFIT_NAVIGATION.md. Bei WEITER exakt zehn Blöcke Alpha.475–484, zuerst Commit/Upstream/CIs prüfen. Kein Produktivdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.465–474, Projektfortschritt 39 %.**

**ALPHA.464 QUELLSTAND GEPRÜFT 19.10.2026:** Feature-SHA 320abed60c759ec09e7d4950d85a3e4a46fac3c1 auf GitHub gepusht. Linux Mint x64 Build 373/373, native C++/OpenGL CTest 158/158, Python 200/200, ARM64-kompatibles Headless, isoliertes Xvfb und X11-Icon/WM_CLASS PASS. Auf ECHTEM Pi5 wurde exakt Feature-SHA 320abed60c759ec09e7d4950d85a3e4a46fac3c1 direkt per SSH aus dem Repository getestet: PI_ARM64_HEADLESS_EXIT=0, OGL_PI5_NATIVE_HEADLESS_OK, 200 Python-Tests OK (1 unterstützter Skip). GitHub Actions Run 39286040685 ist aktuell QUEUED, NICHT SUCCESS. Kein Arm-GPU/GUI-Test oder authentifizierter NexVerse-Avatar-/Outfit-/WASD-End-to-End-Beleg. Dokumentation und Testresultate nach GitHub; keine Produktion. Nächste zehn Blöcke Alpha.465–474; 39 %.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.464, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.465–474.**

19.10.2026 Alpha.455–464: Legacy Low382-Simulator-Wearables werden read-only mit CurrentOutfitFolder und aktueller FetchInventory2-Originalitem-Liste verglichen (ItemID/AssetID/Typ). Fehlende, abweichende, doppelte und Null-Items werden getrennt gezählt. F9 Outfit-Fenster zeigt Originalserver-Datenabgleich bei Agent/Session/RegionLease/COF-Folder/COF-Version; kein automatisch getragenes Outfit behauptet, kein AgentSetAppearance. Native C++/Python/GL, ARM64 Headless, Xvfb, GitHub und Dokumentation. NexVerse Avatar/Outfit/COF/WASD E2E weiterhin nicht live verifiziert: 39 %. Details docs/ALPHA455_464_LEGACY_WEARABLE_COF_COMPARISON.md. BEI WEITER zehn Blöcke Alpha.465–474; CI nur wenn genau geprüfter Feature-SHA erfolgreich. Kein Produktionsupdate.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. Alpha.455–464, Fortschritt 39 %.**

**ALPHA.454 PRÜFSTAND 19.10.2026:** Feature-SHA 9360106596ac437f97fbc626909eb9d4361a223a: Mint Linux-x64 Build 371/371; native C++/OpenGL 157/157; Python 193/193; Xvfb, X11 Taskleistenicon PASS. Raspberry Pi5 hat EXAKT diesen GitHub-SHA in separatem Test-Checkout mit pi5-headless-tests.sh bestanden: OGL_PI5_NATIVE_HEADLESS_OK, 193 Python-Tests (ein unterstützter Skip). GitHub Actions Pi5 Run 39283789898 noch QUEUED (KEIN CI SUCCESS behaupten). Kein GUI-/GPU-Livebuild am Pi, keine echte authentifizierte NexVerse Avatar/COF/WASD-Session. Projektfortschritt 39 %, nächster Zehnerblock Alpha.455–464. Repo-Hauptentwicklung auf GitHub, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.454, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.455–464.**

19.10.2026 Alpha.445–454: Firestorm Legacy WearablesRequest Low381 bei echtem AgentMovementComplete 1x pro authentifiziertem Circuit READ-ONLY; Low382 WearablesUpdate mit 0..16 WearableType, SerialNum, strikter AgentID/SessionID/Regionprüfung und Last-Serial-Abweisung. Reauth/Regionreset behebt Legacy-Referenz-Leak. F3 unterscheidet Anfrage, alte Legacy-Antwort und reine AssetID-Zahl mit COF OFFEN; privater Report anonym numerisch. C++ echte synthetische LLUDP-Pakettests, OpenGL-F3, Python, ARM64 Headless und Pi5 GitHub CI. Kein automatisches Wear/Unwear, AgentSetAppearance oder Fake-Bake. Echtes NexVerse Avatar/Outfit/COF/WASD weiterhin NICHT live bewiesen: Stand 39 %. Details docs/ALPHA445_454_AUTH_LEGACY_WEARABLE_READBACK.md. Nächste 10 Blöcke Alpha.455–464; CI nur für exakt verifizierten Feature-SHA grün. Kein Produktionsupdate.

---

**ALPHA.444 Pi5-CI VERIFIZIERT:** Featurecommit 3bb183def3c0ef618173f705c635d6a87127c53d, GitHub Self-hosted Raspberry Pi5 ARM64 Source Check Run 39140161516 = SUCCESS exakt für diesen SHA. Mint x64 Build 369/369, CTest incl. real GL 156/156, Python 186/186, ARM64-kompatibles Headless, isolierter Xvfb-Viewer, echtes X11 Icon und WM_CLASS PASS. Kein eigener NexVerse Live-Avatar/Outfit/WASD E2E nachgewiesen; Projektfortschritt 39 % (38,65 % gewichtet), nächste Alpha.445–454.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.444, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.445–454.**

18.10.2026 Alpha.435–444: Low158-Firestorm AttachmentBlock erhält tatsächlich gültige 65+ Einträge statt ungültiger 64er Obergrenze. Byte-Grenze 255 nur Feldsyntax, 1200B LLUDP-MTU bleibt strikt! Echter synthetischer LLUDP Low158 (65 Anhänge) geht bis in den Self-WorldModel-Status. F3 beschriftet SSA-Anhänge als nur gemeldete Serverdaten ohne fälschliche Renderbehauptung. Privater numerischer Report / Xvfb GL-Panelprüfung, Native C++/Python und Pi5 CI. Echtes NexVerse Outfit, Low158/COF, WASD weiterhin NICHT live verifiziert, 39 %. Einzelheiten docs/ALPHA435_444_LOW158_ATTACHMENT_COMPATIBILITY.md. BEI WEITER Alpha.445–454: zehn echte Blöcke; GitHub/Upstream zu Beginn prüfen.

---

**ALPHA.434 PI5-CI ERFOLGREICH VERIFIZIERT:** Featurecommit 83c93130b64b0ec1032b864414b52e89cabb4c22; GitHub self-hosted Pi5 ARM64 Source Check Run 39139112676 SUCCESS für genau diesen SHA. Mint Linux-x64 Vollbuild 367/367, CTest 155/155 incl. GPU OpenGL, Python 180/180, ARM64-kompatibles Headless, isolierter Xvfb-Viewer und X11 Systemicon/WM_CLASS PASS. Echt eingeloggtes NexVerse Self AvatarAppearance/Outfit und reale bestätigte WASD-E2E weiterhin offen. Projektstatus 39 % (38,65 % gewichtet). Nächste zehn Blöcke Alpha.435–444.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.434, Projektstatus 39 % (38,65 % gewichtet), NÄCHSTE Alpha.435–444.**

18.10.2026 Alpha.425–434: WASD-Rückmeldungen verwenden nur echte horizontale >=0,25m Positionsänderungen nach Socket-Send; vertikale Gravitationskorrekturen und >=32m Einzel-Positionssprünge sind keine Bewegungserfolge. Positionssprung setzt Vergleichsbaseline/Uhr neu, stale/zeitlich alte Samples ignoriert, alle Regionlease-Bewegungsbelege zurückgesetzt. F3 und privater Bericht erfassen Gründe ohne Koordinaten oder IDs. Native C++/Python/GL, Pi5 ARM64 und isolierte GUI/Icon-Prüfung. Fortgang docs/ALPHA425_434_WASD_DISCONTINUITY_GUARDS.md. Reales NexVerse AvatarAppearance/Outfit/WASD weiterhin offen, 39 %. BEI WEITER Alpha.435–444, immer zehn reale Blöcke. Aktuelle CI nur für exakt bestätigten SHA als grün notieren.

---

**ALPHA.424 PI5-CI VERIFIZIERT:** Featurecommit 74b1839b7ecdad43f80f7405638a7c1433dddf44, selfhosted Raspberry Pi5 ARM64 GitHub Run 39138145169 SUCCESS exakt für diesen SHA. Mint x64 Build 367/367, Native/NVIDIA-GL 155/155, Python 172/172, Pi5-kompatible Headless, isoliertes Xvfb und X11 Systemicon erfolgreich. Low158/COF, reale NexVerse-Avatar-/Outfit-Skins und serverbestätigte WASD-E2E bleiben OFFEN. Projektstand 39 % (38,65 % gewichtet). Nächste Alpha.425–434.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL Version 0.2.0-alpha.424, Projektfortschritt 39 % (38,65 % gewichtet); NÄCHSTE Alpha.425–434.**

18.10.2026: Alpha.415–424 – Reauth/Agentwechsel/Sessionwechsel/Regionswechsel/Disconnect invalidieren bisherige Self-Low158- und vorläufige ObjectUpdate-Skin-Daten sicher. F3/F9 führen nun vorläufige, tatsächlich vom Simulator gemeldete ObjectTE-Body-Bakes mit realen GPU-Residenz-, Original-GL- und Depth-Nachweisen getrennt von authentisch bestätigtem Low158. Native C++/GL, Python, Pi5-kompatible Headless, isoliertes Xvfb/Icon und Pi5-CI nach exaktem Commit. Keine authentifizierte NexVerse Avatar-/Outfit-/WASD-End-to-End-Abnahme. Fortschritt deshalb 39 % unverändert. Vollständige Dokumentation docs/ALPHA415_424_AUTH_AVATAR_SOURCE_PROOF.md. BEI WEITER Alpha.425–434, immer ZEHN reale Entwicklungsschritte.

---

**ALPHA.414 CI ERFOLGREICH VERIFIZIERT:** Featurecommit 08fa7ad14c3b95a97c2712751eba26dab384b716; GitHub Self-hosted Pi5 ARM64 Source Check Run 39137250919 = SUCCESS exakt für diesen SHA. Mint x64 365/365, native NVIDIA/OpenGL CTest 154/154, Python 165/165, Pi5-kompatibles Headless, isolierter Xvfb-Firestorm-Viewer, Original-X11-Systemicon/WM_CLASS PASS. Vollständiger eigener Avatar/Outfit/COF, echtes NexVerse-WASD und alle Firestorm-Menüs weiterhin NICHT live nachgewiesen; Fortschritt 39 % (38,65 % gewichtet). Bei WEITER Alpha.415–424.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.414, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.415–424.**

18.10.2026 Alpha.405–414: Tatsächliche Self-ObjectUpdate-/Compressed-TE-Header-/Upper-/Lower-Bakes bilden eine streng geprüfte vorläufige Alternative, falls Low158-AvatarAppearance fehlt. Provenienz im F3-Panel sichtbar, Privatbericht nur numerisch; gültige Low158 Self Appearance bleibt autoritativ. Korrekte Region/Agent/Session/LocalID, rebind/Reset/placeholder, Tests. Kein authentifizierter NexVerse Outfit-/GPU-/Bewegungs-E2E-Beweis; Fortschritt weiterhin 39 %. Dokumentation docs/ALPHA405_414_SELF_OBJECT_BAKE_FALLBACK.md. Aktuelle CI erst für neuen SHA als grün markieren. Keine Produktionsinstallation. Bei WEITER Alpha.415–424.

---

**ALPHA.404 CI VERIFIZIERT:** Feature-SHA 19e70975376b44719a6c61989d4748bebc0b9d4d, GitHub Pi5 ARM64 Run 39135891262 SUCCESS exakt für denselben Codecommit. Mint x64 Vollbuild 363/363, Native/GL CTest 153/153, Python 159/159, ARM64-kompatibler Headless, isolierter Xvfb-Viewerstart und X11-WM_CLASS/Systemicon erfolgreich. NexVerse Live AvatarAppearance, Outfit/Bake und WASD-E2E weiterhin OFFEN. Prozentstatus 39 %; nächste Alpha.405–414.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.404, Projektfortschritt 39 % (38,65 % gewichtet), nächste Alpha.405–414.**

18.10.2026 Alpha.395–404: Self AvatarAppearance Low158 TextureEntry wird im Paket-Handler und WorldModel mit Firestorm-Decoder validiert. Korrupte nichtleere TE verdrängen alte Skin nicht. F3 und privater numerischer Bericht zeigen fehlerhafte Texturblöcke; Regionswechsel setzt alte Self-Fehlerzähler zurück. Weiterhin kein authentifiziertes Live-NexVerse-Skin/Outfit/WASD E2E, darum Fortschritt 39 % unverändert. Dokumentation docs/ALPHA395_404_SELF_APPEARANCE_TEXTURE_INTEGRITY.md. Letzte belegte CI Alpha.394 Run 39134722480 SUCCESS. Neue CI erst für exakten SHA bestätigen. Bei WEITER Alpha.405–414.

---

**ALPHA.394 CI VERIFIZIERT:** Featurecommit c51dd658224604886d5a536b3bd22dfc49e83214; GitHub Pi5 ARM64 Actions Run 39134573794 SUCCESS für genau denselben SHA. Mint x64 Vollbuild 360/360, 152/152 Native C++/NVIDIA OpenGL, 152/152 Python, ARM64-kompatibles Headless, isolierter Xvfb-Vollviewer sowie X11 WM_CLASS/Icon PASS. NexVerse AvatarAppearance/Outfit/WASD Live-E2E weiterhin OFFEN. Projektstatus 39 % (38,65 % gewichtet), nächster Lauf Alpha.395–404.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.394, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.395–404.**

18.10.2026 Alpha.385–394: Bewegungsfeedback erst nach tatsächlichem erfolgreichem UDP-AgentUpdate, wartende Queue im F3 sichtbar, alternative echte regionale Self-Positionspakete (Terse/Full/Compressed), numerischer Whitelist-Privatbericht. Alpha.384 CI SHA 719db3f Pi5 Run 39133684078 SUCCESS. Keine real authentifizierte NexVerse AvatarAppearance/Outfit/Bake- und WASD-E2E belegt, deshalb 39 % unverändert. Details: docs/ALPHA385_394_UDP_SEND_AND_SERVER_MOTION.md. Mint x64 + Pi5 ARM64 CI, kein unerlaubtes Deployment. Bei WEITER Alpha.395–404.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.384, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.385–394.**

18.10.2026: Alpha.375–384 – Firestorm avatar_lad.xml Attachment-Slots besitzen jetzt Gelenk-IDs und lokale Offsets, originaler Avatar-Keyframe-/Procedural-Pose-Transform folgt dynamisch auch getragenen Prim-Linksets statt starrer Bind-Position. Echte NVIDIA OpenGL Framebuffer-Fixture: 1.108 geänderte Pixel, kein Bone-Fallback-Drift, HUD weiterhin 0 Pixel. F3 und privater whitelisted Report mit Bone-Pose-Anzahl, alte Funktionen bleiben. Echter NexVerse Live AvatarAppearance/Outfit/Bake/Bewegung weiter NICHT BELEGT; gewichteter Fortschritt bleibt 39 %. docs/ALPHA375_384_ANIMATED_ATTACHMENT_BONES.md. Branch develop-current, Mint Build und Pi5 ARM64 CI, niemals unfreigegebenes Deployment. Nächste Alpha.385–394.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE MIT C++/GUI/TESTS/GITHUB/PI5-CI. Aktuelle OGL-Version 0.2.0-alpha.374, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.375–384.**

Alpha.374 CI VERIFIZIERT: Featurecommit dc7a7e7114154adaa444ded5ade9e5dfb97f1dce; GitHub Pi5-ARM64-Run 39131738405 SUCCESS exakt für denselben SHA. Mint Debug x64 Vollbuild 358/358, 151/151 native NVIDIA/OpenGL CTests, 140/140 Python, ARM64-kompatibler Headless, isolierter Xvfb Viewerstart und X11 Systemicon/WM_CLASS PASS. Echte 21-Level Avatar-Linkset-Framebuffer-Pixel 4729, 21 HUD-Prim-Ausschlüsse, keine verbliebenen HUD-Pixel; F3 zeigt reale Linktiefe. Echter NexVerse AvatarAppearance/Outfit-/WASD-End-to-End weiterhin OFFEN. Projektstatus 39 % (38,65 % gewichtet); nächster Lauf Alpha.375–384.


18.10.2026: Renderer-Fix für tief verschachtelte OpenSim-Anbauteile. Vorher 16-Level-Grenze bei 3D-Matrix trotz 128-Level-HUD-Filter. Jetzt beide über dieselbe sichere Parent-Chain (max 128, Zyklen, unbekannte Eltern ohne erfundenen Avataranker). Echter OpenGL-Framebuffer: 21-stufiges Avatar-Kleidungs-Linkset 4.729 RGB-Pixel, komplette HUD-Linksets 21 Objekte ausgeblendet und 0 übrig gebliebene HUD-Pixel. F3 zeigt echte maximale Linktiefe und Anzahl >16; privater Diagnosebericht speichert nur numerische GL-Hierarchiedaten. Echter NexVerse-Avatar-/Outfit-und WASD-Live-E2E weiterhin OFFEN. Details docs/ALPHA365_374_DEEP_LINKSET_TRANSFORMS.md. Bei neuem Chat zuerst develop-current HEAD, CHATUEBERGABE.md, projectplan.md, docs/PROJECT_PROGRESS.json lesen.

---

**ZEHN-BLÖCKE-REGEL: 10 ECHTE ENTWICKLUNGSBLÖCKE pro WEITER. Viewer 0.2.0-alpha.364, Fortschritt 39 % (38,65 %). Nächste Alpha.365–374.**

Alpha.355–364: HUD Root-, Kind- und Enkel-Prims durch Firestorm Attachment-IDs korrekt aus der Welt ausgeblendet; Zyklen beschränkt, normale Kleidung erhalten. F3 HUD 3D-aus GL-Zaehler. Server main 293d1d6: Profil-/Portal-/HG-Aenderungen, keine belegt neue LLUDP-Viewerpflicht. Echtes NexVerse Inworld Outfit/WASD E2E weiterhin offen. docs/ALPHA355_364_HUD_LINKSET.md.

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); Fortschritt 39 %; nächste Alpha.355–364.**

Alpha.354 CI VERIFIZIERT: Feature-SHA 2ed9f66ea96f49eb1b7ad2211aff87248c2b09c0; Pi5-ARM64 GitHub Actions 39079670826 SUCCESS für exakt diesen SHA. Mint x64 Build 352/352, native CTest 148/148, Python 132/132, ARM64-kompatibles Headless, Xvfb-Viewerstart und X11-Icon/WM_CLASS PASS. F3 WASD trotz offener Diagnose, Strg+S statt S, reale UDP-WASD-Datagramme und AvatarAppearance Fehlerabschnitt sichtbar. Authentifizierter NexVerse Bewegungs-/Skin-/Outfit-Nachweis weiterhin OFFEN. Gesamtfortschritt 39 %; nächster Zehnerlauf Alpha.355–364.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE) MIT C++/GUI/TESTS/GITHUB/PI5-CI. OGL 0.2.0-alpha.354, Gesamtfortschritt 39 % (38,65 % gewichtet), nächster Zehnerdurchlauf Alpha.355–364.**

18.10.2026: F3-Avatarfenster blockiert WASD NICHT mehr; nur Chat-/Teleport-Texteingabe sperrt Bewegung. F3-Bericht benutzt Strg+S statt S. 100-ms-WASD ignoriert Strg+S, kein ungewolltes Rückwärtsgehen. F3 zeigt echte erfolgreich per UDP-Socket versandte Richtungs-AgentUpdates als UDP-WASD, unabhängig von empfangenen Simulator-Positionswerten. Die Avatar-Pipeline-Seitenleiste zeigt den Firestorm-Low158-Decoderabschnitt für abgewiesene Serverpakete ohne UUID. Kein authentifiziertes NexVerse-Avatar-Outfit-/Bake-Pixel-E2E und keine serverseitig bestätigte Bewegung. Details docs/ALPHA345_354_LIVE_INPUT_WASD.md. Nächste Alpha.355–364 beginnt mit GitHub CHATUEBERGABE.md, Branch/Commit, projectplan.md, Fortschritt; Mint x64+Pi5 ARM64; kein autonomer Deploy.

---

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); 39 % Fortschritt; nächste Alpha.345–354.**

Alpha.344: Pi5 CI 39078173957 SUCCESS für Featurecommit c04cf98baa8f6ca47f69577215036ba6768ac014. Mint: 148/148 Native, 126/126 Python, Headless, Xvfb und Taskleistenicon PASS. Live NexVerse Bewegung/Outfit weiterhin offen; Gesamtfortschritt 39 %. Nächste Alpha.345–354.

**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. Viewer 0.2.0-alpha.344, Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste Alpha.345–354.**

18.10.2026: Echte Firestorm-Low158 AvatarAppearance Fehlerphase sichtbar und ohne UUID/Paketdaten protokolliert; F3 enthält den fehlenden Appearance-Abschnitt und serverbasierte WASD-Positionsauswertung. Die Bewegungswarteschlange ersetzt unversendete alte WASD-Snapshots. Der authentifizierte NexVerse Outfit-Pixel- und WASD-Bewegungsnachweis bleibt OFFEN. Siehe docs/ALPHA335_344_AVATAR_MOTION_WIRE.md. Neuer Chat beginnt mit GitHub HEAD, CHATUEBERGABE.md, projectplan.md, docs/PROJECT_PROGRESS.json; Mint NVIDIA/x64- und Pi5-ARM64-Tests.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE mit Code, GUI, Tests, GitHub und Pi5-CI; Gesamtfortschritt 39 %; nächste Alpha.335–344.**

18.10.2026 LIVE-SCREENSHOT/HOTFIX: OGL-Starter bei Cinnamon separat und erhaltend angeheftet (Firestorm bleibt); adaptive GPU-Mesh-Count-Grenze 256..384 bei Detail, 256 im Sparmodus, VRAM-Byte-Limits unverändert. NexVerse-Grid-SelfAppearance fehlt weiterhin, Avatar einfarbig, WASD-Position konstant, 2 Texturen fehlgeschlagen. Details docs/LIVE_SCREENSHOT_2026-10-18_TASKBAR_MESH.md. Gesamtfortschritt 39 % (38,65 % gewichtet), nächster Zehnerlauf Alpha.335–344.

ALPHA.334 VERIFIZIERT: Feature-SHA a8cbb3c7fffab019643601ee4dd7517ef6870c70; GitHub Pi5 ARM64-CI Run 39073820122 SUCCESS genau fuer diesen SHA. Mint x64 Debug Vollbuild und 146/146 Native C++23/OpenGL-Tests, 119/119 Python, Pi5-kompatibles Headless, Xvfb-GUI und Originalicon/WM_CLASS PASS. Echte Firestorm-GPU Pixel: Avatar 3398, Head 234, Upper 1422, Lower 1742, Bake 3398, Eviction 0 stale Pixel. Neues F3 Avatar-Fokus [F4]. Kein authentifiziertes Live-Grid-E2E. Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste 10 Blöcke Alpha.335–344.

ZEHN-BLÖCKE-REGEL – ALPHA.334: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE inkl. C++, GUI, Tests, GitHub, Pi5-CI. Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste 10 Blöcke Alpha.335–344. Original Firestorm OpenGL Framebuffer-Pixel jetzt lokal nachgewiesen: 3398 Avatar-RGB-Pixel, Kopf 234, Oberkörper 1422, Unterkörper 1742 Bake-Pixel, nach GPU-Textur-Eviction 0 veraltete Pixel. Live-NexVerse Login/Outfit/Pixels weiterhin OFFEN.

Alpha.324 verifiziert: Featurecommit 4a6a3fb4aa61e768b701d9db9bebd73476764cef; Pi5 ARM64 GitHub Actions 39070404127 SUCCESS. Mint x64 346/346 Build, 145/145 native, 118/118 Python, Headless, Xvfb und X11 Icon bestanden. Live NexVerse Outfit/Avatar E2E offen. Gesamtfortschritt 39 %, naechste Alpha.325–334.

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE) mit Code/GUI/Tests/GitHub/Pi5-CI. Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste Alpha.325–334.**

# Aktueller OGL Viewer 0.2.0-alpha.324 (17.10.2026)

Alpha.315–324: original Firestorm Head/Upper/Lower LLM Vertex-GPU-Skinning für letzten Paletten-Joint repariert. Pro Mesh uAvatarJointCount, echter Nachbarindex N-2/N-1 mit Blend=1 statt fehlendem Matrixelement; CPU-Palette-/Weight-Grenztests, Native und ARM64 Headless. F3 zeigt echte Weighted-GL-/Clip-Draw-Zahlen. Doku docs/ALPHA315_324_AVATAR_GPU_JOINTS.md.

Nächste zehn Entwicklungsblöcke Alpha.325–334. Immer zuerst neueste GitHub CHATUEBERGABE.md, Branch und Projektplan lesen; Änderungen Mint jam-MS-7D28, ARM64-CI Pi5, isoliertes Xvfb ohne interaktiven Desktop. Live NexVerse inworld Avatar/Outfit/Bake-Pixel E2E noch OFFEN, daher 39 % nicht erhöhen. OpenGenesisLINK Server main 7c6b0cf7, OGLAssetService disabled-by-default.


---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste Gruppe Alpha.315–324.**

**Alpha.314 VERIFIED:** Featurecommit decfc90ed089ff3c7a0b8544ba6fb4907086133c; GitHub pi5-ogl ARM64 CI 39069105455 SUCCESS exakt gleicher SHA. Mint Debug x64 Vollbuild 344/344, 144/144 native C++23/NVIDIA OpenGL CTest, 113/113 Python, Pi5-kompatibler Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon PASS. Authentifizierter NexVerse Live-Self-Avatar-/Outfit-/Bake-Pixel-E2E weiter OFFEN. Gesamtfortschritt **39 %** (38,65 % gewichtet); nächste Alpha.315–324.


Aktueller OGL Viewer 0.2.0-alpha.314 | develop-current | Stand 17.10.2026.

Alpha.305–314: Autoritative Self-AvatarAppearance-Pakete werden im tatsächlichen LLUDP-Handler und WorldModel gegen nachfolgende Vorschau/Trial-Pakete und fehlende TextureEntry/VisualParams abgesichert. Bereits bestätigte Skin-/Firestorm-LLM-Texture-Daten bleiben erhalten. F3 zeigt abgelehnte Trial-/Incomplete-Pakete, privater Export enthält nur numerische Whitelist-Zähler. Native/WorldModel/Python-/GPU-/Pi5-Testregressionen, GitHub commit/push, verifizierten CI-SHA. Details: docs/ALPHA305_314_AUTHORITATIVE_SELF_SKIN.md.

Nächster Zehnerlauf Alpha.315–324. Neue Chats starten mit aktuellem CHATUEBERGABE.md, projectplan.md, docs/PROJECT_PROGRESS.json und GitHub-HEAD. Entwicklung nur Mint-PC, Pi5 für ARM64-CI, Xvfb isoliert. Echtes authentifiziertes NexVerse Avatar-/Outfit-/Bake-Pixel-E2E OFFEN; OpenGenesisLINK Server main 7c6b0cf7 optional OGLAssetService disabled-by-default.


---

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE) auch im neuen Chat. Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Gruppe Alpha.305–314.**

**ALPHA.304 TEST-GATES VERIFIZIERT:** Feature-SHA a6d7942173e330b840611ac1f55c952da3cca564; Pi5 ARM64 CI Run 39068198529 **SUCCESS für exakt denselben Commit**. Mint Debug x64 Fullbuild 342/342 Schritte, **143/143 Native C++23/NVIDIA-GL CTest**, **107/107 Python**, lokaler ARM64-kompatibler Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon **PASS**. Authentifiziertes Live NexVerse SelfAvatarAppearance/Outfit-Bake-Pixel-E2E weiter OFFEN. **Gesamtfortschritt 39 % (38,65 % gewichtet)**. Nächster Zehnerlauf Alpha.305–314.


# OGL Viewer aktueller Projektstand Alpha.304 (17.10.2026)

**Alpha.295–304:** Die echte SSA-LLUDP-RegionHandshakeReply-Zustellung bekommt jetzt eine dedizierte und verifizierte ReliableTracker-ACK-Korrelation gegen die neueste Paket-Sequenz. Alte/duplizierte/falsche ACKs sowie vorherige Regionen zählen nicht. Auth-/Region-bewusstes F3 unterscheidet gesendet, quittiert, serverseitiges SelfAppearance tatsächlich empfangen; nach 15 Sekunden ohne Self erscheint nur eine passive Wartewarnung. Die grüne Self-UI verlangt das aktuelle authentifizierte Paket. Lokaler privater Diagnoseexport enthält nur ACK-Boolean und max. 120s numerische Wartezeit. Details docs/ALPHA295_304_SSA_ACK_READBACK.md.

**Neuer Chat:** zuerst CHATUEBERGABE.md, GitHub develop-current HEAD, projectplan.md, docs/PROJECT_PROGRESS.json lesen; 10 echte Blöcke Alpha.305–314 mit echtem Code und UI, Volltests Mint x64 NVIDIA/isoliertes Xvfb, GitHub Push und Pi5 ARM64-CI. Keine unerlaubten Deploys; ohne authentifizierten Live-Grid-Test bleiben Avatar/Outfits/Bakes-E2E offen. Upstream OpenGenesisLINK Server main 7c6b0cf7252189707066f09ad66c732642b27af9 optionales OGLAssetService disabled-by-default.



---

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE), auch im neuen Chat. Stand 39 %; nächste Gruppe Alpha.295–304.**

**VERIFIZIERTER Alpha.294-Abschluss (17.10.2026):** Feature-Commit d4a216ead2be4b6a842e53ccbebd609ad76bb763; GitHub Pi5 ARM64-CI Run 39066916593 **SUCCESS** exakt für diesen SHA. Linux Mint x64 Fullbuild (340 Schritte), **142/142 native C++23-/NVIDIA-OpenGL-Tests**, **101/101 Python-Tests**, ARM64-kompatibler Headless, isolierter Xvfb-Viewer und X11-Originalicon/WM_CLASS **PASS**. Live-NexVerse-SelfAvatarAppearance-/Outfit-/Bake-/Pixel-E2E weiterhin OFFEN. **Gesamtfortschritt 39 %** (38,65 % gewichtet); nächste Zehnergruppe Alpha.295–304.

# Aktueller Viewerstand Alpha.294 – 17.10.2026

**ZEHN-BLÖCKE-REGEL: bei JEDEM WEITER und in jedem neuen Chat 10 ECHTE ENTWICKLUNGSBLÖCKE, inklusive realem Code, sichtbarer GUI, vollständigen Tests, GitHub-Push und Pi5-CI.**

**Gesamtfortschritt 39 %** (38,65 % gewichtete Schätzung, unverändert). **Nächste Zehnergruppe Alpha.295–304.** docs/PROJECT_PROGRESS.json ist maßgeblich.

**Alpha.285–294:** Echten LLUDP-Handshake-Blocker gefunden und repariert: Bei RegionHandshakeReply fehlte das von Firestorm/Second Life erwartete SSA-Bit2 0x00000004. Canonical Protocol Constant, Default Encoder und produktiver Circuit senden Bit2 jetzt zuverlässig; Cache-Bits0/1 bleiben aus. Status erst nach lokalem Send-Erfolg, F3-Anzeige unterscheidet Handshake-Angebot von tatsächlichem serverseitigem Self-AvatarAppearance-Readback. Private Diagnose enthält nur Send-Bool. Keine unbelegten Live-Grid-Erfolge. Dokumentation docs/ALPHA285_294_SSA_HANDSHAKE.md.

Upstream main 7c6b0cf7252189707066f09ad66c732642b27af9 nur OGLAssetService-Konstruktor/Dependencies, experimentelles Serveraddon disabled-by-default. Mint-Rechner ist Buildhost, Raspberry Pi5 ARM64 CI, isoliertes Xvfb statt echtem Desktop. Eigenavatar-/Outfit-/Bake-Pixel-E2E noch **OFFEN**.



---

Alpha.284 VERIFIZIERT: Featurecommit 2017b48fcc6586e00656bba01c6fe3058d0c98be, GitHub Pi5 ARM64-CI Run 39065802543 SUCCESS für exakt diesen SHA. Linux Mint vollständiger x64 Debug Build (340 Buildsteps), 142/142 Native-C++23/NVIDIA-GL-CTest, 95/95 Python, ARM64-kompatibler Headless, isolierter Xvfb-GUI und Original-Systemicon/WM_CLASS PASS. Authentifizierter Live-NexVerse Avatar/Outfit-Bake-E2E OFFEN. Gesamtfortschritt 39 % (38,65 % gewichtet); nächster Lauf Alpha.285–294.

# Aktueller OGL Viewer Stand – Alpha.284 (17.10.2026)

**ZEHN-BLÖCKE-REGEL: bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE (Code und GUI, Tests, Dokumentation, GitHub-Push, Pi5-CI). Auch im neuen Chat.**

**Gesamtfortschritt 39 % (38,65 % gewichtete Schätzung, unverändert).** Nächste zehn Blöcke **Alpha.285–294**, Quelle docs/PROJECT_PROGRESS.json; Fortschritt niemals automatisch aus Versionsnummern oder CI ableiten.

**Alpha.275–284:** Die echte OpenGL-Sichtbarkeitsmessung der ursprünglichen Firestorm-LLM-Meshes Head/Upper/Lower hat nun drei voneinander getrennte GPU-Abfragen mit je zwei asynchronen Slots. Monotone Ergebnis-Sequenz + Region-Epochen-Guard verhindern veraltete Ergebnisse. Positive Gesamtantwort ist OR über drei Parts statt der letzte abgefragte Wert. F3 Avatar hat eine echte pro-Part-Depth-Anzeige und auf Bake-Seite getrennte Server-TE/GPU/BakeGL/Depth. Sanitärer F3-Report enthält sechs boolesche Part-Werte. Details docs/ALPHA275_284_AVATAR_PART_DEPTH.md.

**Upstream:** OpenGenesisLINK main 7c6b0cf7252189707066f09ad66c732642b27af9 (OGLAssetService Build/Konstruktor/Dependencies), staging / disabled-by-default; kein Viewer-Wirevertrag. Echtes NexVerse AvatarAppearance/Outfit-Bake-Pixel-E2E weiterhin OFFEN. Entwicklungs-PC ausschließlich Mint, Pi5 ist ARM64 CI.


---

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE) auch im neuen Chat. Nächste Gruppe Alpha.275–284. Gesamtfortschritt 39 %.**

**ALPHA.274 ABGESCHLOSSEN – VERIFIED SUCCESS:** GitHub-Featurecommit 427555ab8ccb59adbbc41cd00bf576cbedc62f07; Pi5 ARM64-CI Run 38980155018 SUCCESS für exakt denselben SHA. Linux Mint x64 Vollbuild, **141/141 C++23/NVIDIA-OpenGL CTests**, **90/90 Python**, ARM64-kompatibler Headless, isolierter Xvfb-Vollviewer und originales X11 Systemicon (WM_CLASS) PASS. Gesamtfortschritt 39 % (38,65 % gewichtete Schätzung, unverändert), echter NexVerse SelfAvatarAppearance/Outfit/Bake-Pixel-E2E OFFEN. Nächster Zehnerlauf Alpha.275–284.

Neuester Serverreview 17.10.2026: OpenGenesisLINK main 5fcfc7f486660c8c27586e69eefc62cf0b9d22a1 (+2 Commits nach 8ec7ea; einzig bin/Robust.HG.OGLAsset.ini.example). Dieses vollständige Hypergrid-ROBUST-Beispiel ist EXPERIMENTAL, DISABLED BY DEFAULT und ausdrücklich nur für isoliertes Staging mit eigener DB/Assets/Secrets. Kein Laufzeit-Protokoll- oder Viewer-CAP-Wechsel, KEIN automatisches Aktivieren/Deployment.

# OGL aktueller Stand: Alpha.274 (17.10.2026)

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE. 10 ECHTE ENTWICKLUNGSBLÖCKE je WEITER und neuem Chat.**

**Gesamtfortschritt ca. 39 % (38,65 % gewichtete Schätzung, unverändert).** Nächster Zehnerdurchlauf: **Alpha.275–284**. Bewertungsquelle docs/PROJECT_PROGRESS.json, keine automatische Erhöhung durch Tests oder Version.

**Alpha.265–274:** Authentifizierte SelfAvatarAppearance/Server-TE-Bakes in elf GPU/GL-Statuszeilen im F3-Fenster; echte Schalter B für Übersicht/Bake-Seite 1/2/2/2 und R für streng kontrollierte Replanung fehlender Head/Upper/Lower-GetTexture-Assets. Originales Retry-/VRAM-/Pacer-/CAP-/Region-/Worker-Gating wird nicht umgangen. Details docs/ALPHA265_274_AVATAR_BAKE_INSPECTOR.md.

**Server main 8ec7ea255901260abdeabfc5f17eeb850930d851:** optionales Addon disabled-by-default, kein Viewer-Wire-Protokollwechsel. Mint-PC bleibt der C++/GUI-Buildhost, Raspberry Pi5 nur ARM64-CI. Live-Grid-Avatar-/Outfit-/Pixel-End-to-End bleibt OFFEN.


---

# Aktueller Viewerstand – Alpha.264 (17.10.2026)

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE, auch im neuen Chat. 10 ECHTE ENTWICKLUNGSBLÖCKE pro Weiter.**

**Gesamtfortschritt: ca. 39 %** (38,65 % gewichtete Schätzung, unverändert). **Nächster Zehnerdurchlauf: Alpha.265–274.** Verbindlich zehn echte Code-/GUI-/Testblöcke je WEITER. Quelle: docs/PROJECT_PROGRESS.json.

**VERIFIZIERTER ALPHA.264-ABSCHLUSS:** Featurecommit acaab98d987c6c7602f806da4b33b2de8ae333a5, GitHub pi5-ogl ARM64-CI Run 38978541689 SUCCESS fuer exakt diesen SHA. Linux Mint x64 Vollbuild, 140/140 C++23/NVIDIA-GL-CTest, 85/85 Python, Pi-kompatibles Headless, isolierter Xvfb-GUI-Smoke und Original-X11-Taskleistenicon PASS. Gesamtfortschritt 39 % (38,65 % gewichtet, unveraendert); Grid/Avatar/Outfit-E2E weiter OFFEN. Nachster Lauf Alpha.265–274.

**Alpha.255–264:** echter im Regionsfenster F7 per Maus oder T auslösbarer, server-/epochgebundener Terrain-Nachladeversuch. Sicher begrenzt auf zwei manuelle Neustarts NACH vier automatischen und 30s Endpause; vorhandene Texturlayer bleiben erhalten; kein paralleler HTTP-Download, kein Request ohne CAP. Anzeige der vier Layer, Countdown und CAP-/Downloadstatus, inklusive 16:9/32:9-Layout- und C++-Tests. Nächste Priorität: authentifizierte Self-Avatar-Bakes samt sichtbarer Welt-/Kleidungs-Darstellung. Details: docs/ALPHA255_264_TERRAIN_MANUAL_RECOVERY.md.

**OpenGenesisLINK-Server (neuer Main-HEAD):** main 8ec7ea255901260abdeabfc5f17eeb850930d851 nach sechs weiteren reinen Addon-Build/Workflow-Commits; bisheriger Addon-Merge-HEAD bf55a17524a9f3f175c56d2adeaa4d39083f446f, OGL AssetService-Addon disabled-by-default, KEIN unverifiziertes Deployment/Viewer-API-Enable. GitHub Branch develop-current, Entwicklungsrechner Linux Mint, Pi5 nur ARM64-CI. Kein Live-Avatar-/Terrain-E2E-Nachweis.


---

# CHATÜBERGABE – IM NEUEN CHAT ZUERST LESEN

**ARCHIV 16.10.2026: Zehnerregel gilt weiterhin; der damalige Durchlauf Alpha.255–264 ist abgeschlossen.** In jedem Durchlauf Code UND sichtbare Oberfläche, Regressionen, kompletter Mint-Build, Commit/Push und Pi5 CI; nach jeder Arbeitsrunde den belegbaren Fortschritt in % aus docs/PROJECT_PROGRESS.json angeben. Diese aktuelle Zehnerregel überschreibt historische Fünferreferenzen in älteren Berichten.

## VERBINDLICHER GESAMTFORTSCHRITT – BEI JEDEM PROJEKTSTAND IN PROZENT

**Historischer OGL-Gesamtfortschritt: ca. 39 % (Stand Alpha.254). Aktuell siehe oberster Block Alpha.264.**

**Historisch geplante zehn Entwicklungsblöcke: Alpha.255–264 (abgeschlossen).**

- PFLICHT: Bei JEDER Statusantwort zum Projekt und nach JEDEM Entwicklungs-/Zehnerdurchlauf sowie in JEDEM neuen Chat IMMER XX % Gesamtfortschritt auffuehren; auch wenn unveraendert.
- Einzige Bewertungsquelle docs/PROJECT_PROGRESS.json mit sieben gewichteten Funktionsbereichen (Gewichte ergeben 100 %). Berechnen und validieren: python3 scripts/project-progress.py. Details: docs/PROJEKTFORTSCHRITT.md.
- Nach jedem Durchlauf die sieben Bewertungsbereiche anhand nachweislich implementierter, integrierter und getesteter Funktionen neu pruefen. Fortschritt darf auch unveraendert bleiben oder sinken. Alpha-Version und Zahl der Commits erzeugen KEINEN automatischen Zuwachs.
- Bei neuem Bewertungswert JSON, docs/PROJEKTFORTSCHRITT.md, projectplan.md, CHATUEBERGABE.md und info.md synchron aktualisieren und committen; AGENTS.md enthaelt die dauerhafte Regel. Keine widerspruechlichen Prozentangaben.
- 39 % ist eine gewichtete Projektleitungs-SCHAETZUNG der Reife bis zur vollstaendigen Firestorm/OpenSim/OpenGenesisLINK-Funktionsbasis, KEIN automatischer Testabdeckungsgrad und KEIN Abnahmenachweis. Offene Self-Bakes/Animations-/Social-/Plattform-E2E bleiben Teil des Gesamtumfangs.

## VERBINDLICH AB SOFORT: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE

- Der Nutzer verlangt ohne erneute Rückfrage **zehn zusammenhängende, tatsächlich umgesetzte und getestete Entwicklungsblöcke** bei JEDEM WEITER, auch im neuen Chat. Standard nicht auf drei oder einen Block reduzieren. Nur ausdrückliche neue Nutzeranweisung ändert den Umfang.
- Am Anfang GitHub marlin198323-glitch/nexverse-viewer, Branch develop-current, projectplan.md, AGENTS.md, CHATUEBERGABE.md, info.md, den Server-Repo-HEAD opengenesislink/opengenesislinkserver sowie aktuelle lokale Änderungen prüfen.
- INTERAKTIVE Entwicklung ausschließlich Linux Mint jam-MS-7D28 (Desktop Commander), Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer nach erneuter Verifikation. Raspberry Pi 5 ist nur der pi5-ogl ARM64-CI-Runner. Kein ungefragter sichtbarer Viewerstart auf dem echten Desktop; vollständiger GUI-Smoke nur auf isoliertem Xvfb.
- PRO DURCHLAUF: zehn fortlaufende Alpha-/Block-IDs, zusammenhängende echte Änderungen an Code UND zugehöriger sichtbarer GUI, native C++23/NVIDIA-OpenGL/Python/Headless-Regressionen, vollständiger Mint-x64-Build, Xvfb/Taskleistenicon, Dokumentation in projectplan.md, AGENTS.md, CHATUEBERGABE.md, info.md, Kompatibilitätsmatrizen, GitHub-Commit/Push und erfolgreichen Pi5-ARM64-CI-Lauf nachweisen.
- Nicht getestete Praxisfunktionen als OFFEN kennzeichnen; insbesondere echter authentifizierter NexVerse-Welt-/Avatar-/Outfit-End-to-End-Check bleibt offen. Keine Erfolge oder Commits behaupten, die nicht tatsächlich ausgeführt und geprüft wurden.
- NÄCHSTE ARBEIT BEIM START: aktuelle VERSION, GitHub HEAD, Prozentstand via scripts/project-progress.py und nächste Alpha-Zehnergruppe im jeweils NEUESTEN Status feststellen; jetzt Alpha.255–264. Nicht alte SHA/Alpha-Angaben aus historischen Abschnitten verwenden. Zehnerregel in projectplan.md.

### VERIFIZIERTER RELEASE ALPHA.254 – ALLE 10 BLÖCKE FERTIG

- Version 0.2.0-alpha.254; Gesamtfortschritt ca. 39 %
  (38,65 % gewichtet), nach echten Terrain-Retry-/GPU-Fixes.
- Featurecommit develop-current
  392b609c9820cdf62be49d960501f2627fc56b04.
- GitHub ARM64 Self-hosted pi5-ogl CI Run 38976703736:
  **completed SUCCESS** für exakt diesen Feature-SHA.
- Linux Mint vollständiger x64 Build, 140/140 NVIDIA/OpenGL
  C++23 CTests, 80/80 Python, Pi5-kompatibles Headless,
  Xvfb-Vollviewer und WM_CLASS Original-Taskleistenicon PASS.
- NÄCHSTER 10-BLOCK-DURCHLAUF **Alpha.255–264**,
  wieder direkte Entwicklung auf Mint, GitHub+Pi5 CI.
- **NICHT TESTBELEGT:** aktuelles NexVerse SelfAvatarAppearance,
  komplette Head/Upper/Lower-Bakes, Firestorm-Terrainpixel
  und komplette Inworld-Animations-/Outfit-Parität.
- OpenGenesisLINK Server aktueller geprüfter HEAD
  bf55a17524a9f3f175c56d2adeaa4d39083f446f,
  OGL AssetService Addon disabled-by-default;
  nicht blind aktivieren.

### NEUSTER STAND – OGL ALPHA.254 – FUNKTIONALES TERRAIN-RETRY

- Der verbindliche 10-Block-Lauf Alpha.245–254 beinhaltet
  echte Terrain-Nachladungen, partielle GPU-Layer-Promotion
  und Reparatur des früher kumulativen Terrain-Draw-Zählers;
  F7-Regionsfenster zeigt Material-/Retry-Status.
- Authentifizierte Self-AvatarAppearance-/Animationszähler
  bleiben je Simulatorregion getrennt; ein Keyframe-
  GL-Skin-Erfolg erscheint in Log und Animationsfenster.
- **Gesamtfortschritt jetzt ca. 39 %** statt 38 %:
  Welt-Renderer 43 %, Asset/Materialsystem 47 %,
  38,65 gewichtete Prozentpunkte. Kein Echtraster-E2E.
- Version 0.2.0-alpha.254. Nächster Lauf Alpha.255–264:
  wieder 10 echte Code+GUI-Blöcke mit Mint Fullbuild,
  GitHub-Push und erfolgreicher Pi5-ARM64-CI.
- GitHub Featurecommit/CI aus letztem Ergebnis der
  HEAD-/Actions-Abfrage prüfen, ältere Commits nicht
  als aktuellen SHA verwechseln.
- OpenGenesisLINK Server main d8f4426110379b39be7c0f45161f7c0c6b5d811e
  neuer MeshUPLOAD-LLSD-Validierungspatch; OGL GetMesh
  bleibt davon unverändert, reviewed SHA nicht blind erhöhen.
- **NEUERER SERVER-HEAD beim abschließenden Abgleich:**
  bf55a17524a9f3f175c56d2adeaa4d39083f446f,
  86 zusätzliche Commits seit d8f4426 mit Merge
  OGL AssetService Addon (#231), disabled-by-default.
  Optionales Server-AssetService nur nach getrenntem
  Capabilities-/Security-Review; nie alte Reviewed-Baseline
  automatisch auf den neuesten SHA verschieben.

### ALPHA.244 FERTIG – BESTÄTIGTER AKTUELLER GITHUB-/CI-STAND

- Aktueller Feature-Commit 0f13dd74bef477b97e3cfbc924db1f3370e2a28d auf develop-current, vollständig geprüft; zehn Blöcke 235–244 FERTIG.
- Linux Mint Native/NVIDIA GL 137/137, Python 73/73, Pi5-kompatible Headless, isolierter Xvfb-Vollviewer und Original-Systemicon grün.
- GitHub Self-hosted Runner pi5-ogl: Run 38974639521, SUCCESS, SHA identisch.
- Gesamtfortschritt **ca. 38 % (unverändert)**, gewichtete Projektleitungs-Schätzung. Kein echter neuer NexVerse-Live-Avatar-E2E-Beweis.
- Nächster verbindlicher 10-Block-Durchlauf Alpha.245–254; bei WEITER sofort Code+GUI+Tests auf Linux Mint, dann Commit/Push und Pi5-CI.

### Historische Übergabe vor CI: Alpha.235–244 wird abgeschlossen

- OGL-Version: 0.2.0-alpha.244 | Gesamtfortschritt **38 % (Schätzung, unverändert)**.
- Standard: **10 echte Code-/GUI-/Testblöcke** je Nutzer-„Weiter“.
- Quellstand nach Tests/Push unten nachführen; nächsten Batch Alpha.245–254 ausführen.
- Linux Mint jam-MS-7D28 ist Entwicklungsmaschine; Raspberry Pi ausschließlich pi5-ogl CI.
- Neue tatsächliche Evidenz: Self-Original-LLM-Bakes versus GPU-Bakes,
  eigene Server-Keyframe-GL-Draws, 4/4 echte Terrain-Layer gegen
  graue Ersatzlayer und GPU-gematchte Mesh-LODs.
- Nach wie vor OFFEN: vollständige Firestorm-World/Avatar-Outfit-
  /Animations-Parität im authentifizierten Live-NexVerse-Grid.
- Unveränderte Quelle des Projekt-%-Werts: docs/PROJECT_PROGRESS.json;
  mit python3 scripts/project-progress.py neu berechnen.
- Alte Berichte mit 'fünf Blöcke' sind HISTORISCH und überschreiben
  die aktuelle 10-Block-Anweisung nicht.

### AKTUELLER VERBINDLICHER ENTWICKLUNGSSTAND – ALPHA.234

- Version 0.2.0-alpha.234, alle fünf Blöcke Alpha.230–234 FERTIG.
- Code-Commit develop-current: 52be76d42bc2b99aa92f3e48801d373d1347161f.
- Mint Linux x64 Fullbuild, 134/134 Native/NVIDIA-GL, 62/62 Python, Pi5 kompatible Headless Tests, isolierter Xvfb Viewer und X11 Original-Systemicon erfolgreich.
- GitHub Pi5 ARM64 CI Run 38970180604 SUCCESS für genau diesen Codecommit.
- F2 DETAIL/SPAREN schaltet echten Mesh-LoD-Pfad und klickbaren Status, Kamera 5.5m/F4, lesbareres Avatar-HUD.
- Neue echte AvatarAppearance Self/Fremd/Invalid-Diagnose und GL Terrain Textur-/Graufallback-Status im Welt-HUD.
- NÄCHSTER verpflichtender FÜNF-BLÖCKE-DURCHLAUF Alpha.235–239: Restarbeiten AvatarAppearance Server-/Bake-Komposition, visuelles Terrain und HighLOD-Praxisbefund, Firestorm Animation und UI. Fünf echte Code-/UI-Blöcke, Mint-Build, GitHub und Pi5 CI.
- Authentifizierter Live-Vergleich nach dem neuen Client-Build noch OFFEN, keine vollständige Firestorm-Parität behaupten.

### GEPRÜFTER AKTUELLER STAND / SOFORT WEITERARBEITEN

- Version 0.2.0-alpha.229; alle fünf Blöcke Alpha.225–229 implementiert, lokal grün und Pi5-ARM64-CI SUCCESS.
- Geprüfter Viewer-CODE-Commit: 4a38427699a39385996576d6b632d5cc1e8bfbdd auf develop-current.
- Mint Native/NVIDIA-GL CTest: 131/131; Python: 60/60; Pi5 Headless, isolierter Xvfb-Vollviewer und X11 Original-Icon grün.
- CI-Bericht: https://github.com/marlin198323-glitch/nexverse-viewer/actions/runs/38965645437
- F3-Ladediagnose: zuletzt erreichte Regionen-/Agent-/Circuit-Stufen mit Zeiten, konkrete Ursache für gesperrten Reconnect, Manueller Reconnect-Versuchszähler, privater Export nach Nutzerklick.
- **NÄCHSTER 5-BLÖCKE-DURCHLAUF: Alpha.235–239.** Bei neuem WEITER unverzüglich alle fünf realen Arbeitsblöcke inkl. GUI/Tests/Commit/CI bearbeiten.
- Offen: autorisierter NexVerse-Login mit realer Welt-/Avatar-GPU-Pixelbestätigung und echter LLUDP-Reconnect-Praxistest.

---

# OpenGenesisLINK Viewer – Chat-Übergabe

## Verbindliche OGL-Arbeitsumgebung ab 16.10.2026 (neueste Nutzeranweisung)

- **Primäre interaktive Entwicklungsmaschine ist wieder der Linux-Mint-PC `jam-MS-7D28`** über Remote Desktop Commander (Device-ID `3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d`). Beim Start `list_devices` prüfen und ausdrücklich dieses Gerät auswählen.
- **GitHub bleibt führend:** `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; vor jeder Änderung Git-Status/Remote-SHA abgleichen, lokale uncommittete Änderungen auf Mint nicht überschreiben. Änderungen dort programmieren, lokal nativ/GUI testen und danach zu GitHub pushen.
- **Raspberry Pi 5 `s359142.srv.nexvortex.de` nur als Self-hosted-CI-/ARM64-Test-Runner** für OGL einsetzen, nicht als primären interaktiven Editor, Build-/Grafikrechner oder Ersatz für den PC. Die Pi-Anweisungen unten dokumentieren den historischen Stand bis Alpha.149 und sind in dieser Hinsicht überholt.
- Falls `jam-MS-7D28` offline ist, keine PC-Checkout-/Build-/GUI-Ergebnisse behaupten. Für rein dokumentarische Aktualisierungen darf der GitHub-Connector genutzt werden; keine eigenmächtige Verlagerung der OGL-Programmierarbeit auf den Pi.
- Lokalen Mint-Pfad und tatsächlichen Git-Status bei Wiederverbindung **neu ermitteln**, nicht vom früheren Pfad ausgehen. Als letzter bestätigter GitHub-Stand liegt Alpha.149 auf `develop-current` (`b7dabf4b805d7d9beb93a4452e19a23e66a63e46`); zwischenzeitliche neue Commits immer zuerst prüfen.

## Aktuelle Raspberry-Pi-Entwicklungsumgebung (15.10.2026)

- Ab sofort Remote Desktop Commander auf `s359142.srv.nexvortex.de`, Device-ID `76f55079-2e1f-4660-a6b5-4c9ac490b3be`, Linux-Benutzer `nexdev` und Pfad `/srv/nexdev/development/projects/ogl-viewer` verwenden. Nicht ungefragt zu Linux Mint wechseln; ältere Pfadangaben in diesem Dokument gelten als historische Entwicklungsstände.
- GitHub: `marlin198323-glitch/nexverse-viewer`; Arbeitsbranch `develop-current`, nicht `main` oder `develop`. Die lokale Pi-Arbeitskopie ist als Benutzer `nexdev` erfolgreich auf `develop-current` geklont und per SSH freigeschaltet. `git push --dry-run` hat den Schreibzugriff erfolgreich nachgewiesen, ohne einen Branch anzulegen oder Änderungen zu pushen. Checkout-SHA vor diesem Doku-Update: `06cc2294c86e12922f3ec6b212cd7a222eadcb6d`.
- Eigener OGL-GitHub-Schlüssel liegt unter `/home/nexdev/.ssh/ogl_viewer_git_ed25519`, SSH-Alias `github-ogl-viewer`. Den privaten Schlüssel niemals auslesen, kopieren oder im Chat weitergeben. Checkout nach Freigabe: `bash /srv/nexdev/development/setup-ogl-viewer-checkout.sh`.
- CI: eigener GitHub-Self-hosted-Runner `pi5-ogl` auf Raspberry Pi registriert und Smoke-Test bestanden. Pi-CI prüft bisher nur Quellen/Shell-Syntax; CMake ist auf dem Pi noch nicht installiert; dies ist kein kompletter C++-/Firestorm-/GPU-/Windows-/macOS-Buildnachweis.
- Grid-Diagnose vom Pi: `http://hg.stadt-nexverse.de/get_grid_info` antwortet HTTP 200 und nennt `http://hg.stadt-nexverse.de:80/` als Login. `https://hg.stadt-nexverse.de/get_grid_info` und der HTTPS-Root können Port 443 nicht verbinden (curl 7). `https://stadt-nexverse.de/service/welcome` antwortet 200. HTTP-Discovery weiter verwenden, bis HTTPS auf dem **Gridserver** eingerichtet, mit gültigem Zertifikat getestet und dessen Konfiguration angepasst ist. Keine TLS-Zertifikatsprüfung umgehen.
- Sicherheits- und Funktionsstatus getrennt berichten: Grid-HTTP-Erreichbarkeit bedeutet keine funktionierende HTTPS-Anmeldung und keinen erfolgreichen In-World-Test.


Stand: 11.10.2026

## 1. Projekt

- Produkt: **OpenGenesisLINK Viewer**
- Zweck: eigener OpenSim/OpenGenesisLINK-Viewer für NexVerse
- Sprache: C++23
- **Neuer verbindlicher Auftrag 11.10.2026:** Alle vorhandenen Firestorm-Funktionen als vollständige Funktions-/Implementierungsbaseline erfassen und für OpenGenesisLINK implementieren. Neue OGL-Funktionen nur als Ergänzung/gezielten Umbau; nichts stillschweigend weglassen
- **Ist:** aktueller OGL-C++23-Code ist noch kein tatsächlicher Firestorm-Source-Fork. Quellcodeübernahme/Forkstrategie mit Lizenz-, Release-Branch- und Migrationsprüfung, keine falsche bereits-erledigt-Meldung
- Autoritativer Server: https://github.com/opengenesislink/opengenesislinkserver , vor JEDEM künftigen Entwicklungsblock aktuelle `main`-HEAD-SHA neu prüfen und kompatible Server-CAPS/LLUDP-/API-Codepfade abgleichen
- Firestorm Quell-/Feature-Referenz: https://github.com/FirestormViewer/phoenix-firestorm (offizielle Releasebranches, nicht Nightly); Details: `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`
- Repo: `marlin198323-glitch/nexverse-viewer`
- lokaler Arbeitsbranch: `develop-current`
- GitHub-Handoff-Branch: `develop-current`
- Hinweis: GitHub-`develop` besitzt eine ältere divergierende Historie; **nicht** als aktuellen Stand verwenden, bis sie bewusst reconciled wurde.
- lokal: `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`
- vcpkg: `/home/jam/vcpkg`
- `projectplan.md` ist bindend; seit 11.10.2026 inklusive vollständiger Firestorm-Parität und aktueller OpenGenesisLINK-Server-HEAD-Referenz
- `docs/FIRESTORM_PARITY_MATRIX.md` ist die offene Feature-Inventur; ältere Ausschlüsse von RLV/Phototools gelten nicht mehr

## 2. Neuer Chat – zuerst

```bash
cd '/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer'
git status --short --branch
git log -8 --oneline
cat VERSION
```

Wenn direkt über GitHub gearbeitet wird, zuerst Branch `develop-current` verwenden. Danach lesen: `CHATUEBERGABE.md`, `projectplan.md`, `info.md`, **`docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`**, `docs/ARCHITECTURE.md`, `docs/DESIGN.md`. Zusätzlich vor jedem neuen Feature und Release aktuellen OpenGenesisLINK-Server-`main`-HEAD und dessen relevante Implementierungsdateien prüfen; der einmalige SHA vom 11.10.2026 ersetzt **keine** erneute Aktualitätsprüfung.

Keine destruktiven Resets/Cleans gegen den lokalen Stand.

## 3. Aktueller Code-Stand

Version: `0.2.0-alpha.85`.

Letzter wesentlicher Codecommit vor den Planungscommits:
`6f4d9c4 feat: support OpenSim planar texture mapping`

Vorhanden:
- Phase 1 Grid Discovery
- Phase 2 echter OpenSim Login
- Phase 3 echter LLUDP-Circuit
- Phase 4 CAPS/EventQueue/SimulatorFeatures
- Phase 5 WorldModel
- LayerData Terrain
- Terrain-Detailtexturen
- EEP/WindLight/ExtEnvironment
- sichtbare Prim-Grundformen und dynamische Prim-Geometrie
- OpenSim Face-Ranges
- TextureEntry Default/per-Face
- explizite Prim-UVs
- Planar TexGen
- Fullbright/Shiny/Bumpiness/Media-Daten im Renderer-Vertrag

Letzter vollständiger Teststand: 23/23 Tests grün plus OpenGL Runtime-Smoke; In-World-Sichttest noch offen.

## 4. NexVerse Runtime

- Grid: `hg.stadt-nexverse.de`
- Discovery: `http://hg.stadt-nexverse.de/get_grid_info`
- Welcome: `https://stadt-nexverse.de/service/welcome`
- Discovery bei jedem Start
- In-World-UI erst bei Simulation Ready
- Start maximiert, normales dekoriertes Fenster
- F11 exklusiver Fullscreen
- Widescreen responsiv

## 5. Module Runtime: erster Basisblock implementiert

Version `0.2.0-alpha.38`:

- `NexVerse::Modules` als Core-Library
- öffentliche C-ABI v1 und Lifecycle `ogl_module_get_api`, init/start/stop/shutdown
- Manifest Loader, lokale Modulerkennung, Plattform-/Architektur- und Versionscheck
- Dependencies inklusive Zyklusprüfung, deklarierte Capabilities
- dynamische Bibliotheken, Start-Rollback, umgekehrtes Stoppen
- Aktivieren/Deaktivieren und persistenter Aktivierungszustand
- OGL Example Module als Testmodul
- Debug-Build und 20/20 Tests bestanden, OpenGL Runtime-Smoke erfolgreich

Sicherheitsgrenze: **Nicht signierte native Plugins werden standardmäßig blockiert**.
Developer Mode muss von der Hostanwendung explizit erlaubt werden und alle
Capabilities freigeben. Signatur-/Hashprüfung, sichere Isolation und
Nutzer-Modulverwaltung fehlen noch. Details: `docs/MODULES.md`.

Version `0.2.0-alpha.39` (aktueller Stand):
- ABI v1 unverändert; zusätzlich ABI v2 mit `ogl_host_api_v2`,
  `ogl_diagnostics_snapshot_v1` und Main-Thread `on_frame`-Callback
- OGL Developer Tools und OGL Performance Monitor als echte dynamische
  Testmodule mit Dependency, Lifecycle und read-only Host-Metriken
- `--dev-modules` als ausdrückliche CLI-Freigabe für lokal installierte,
  bereits aktivierte unsignierte Developer-Module; Standard weiter blockiert
- Metriken: FPS, geglättete Framezeit, Draw Calls, Triangles, Objects,
  RAM/VRAM, `simulation_ready`, Frameindex
- Logs im geladenen Viewer erfolgreich; 21/21 Tests grün inklusive ABI
  v1/v2 und Dependency-Test; OpenGL-Runtime-Smoke mit Developer-Modulen OK

**Nächster verbindlicher Arbeitsblock: Phase 8** gemäß `projectplan.md`:
Prims/Materialien/Scene-Darstellung vervollständigen; anschließend
Mesh/Sculpt/Partikel/Lichter. Kein Vulkan-/RTX-/Weather-/Hydro-Vorgriff.
Die Entwickler-Module sind aktuell reine Log-/Diagnosemodule ohne UI,
ohne Produktions-Signaturfreigabe und ohne Sandbox. Anleitung: `docs/MODULES.md`.

## 6. Plugins / Add-ons

Offizielle spätere Registry: `https://addons.opengenesislink.de/`.

Der Zielkatalog umfasst aktuell 38 Plugin-/Modulideen. Online-Store, Download, `.oglpkg`, Signaturen und automatische Updates folgen später.

## 7. Outfit / Text-only / Weather

Outfit-Basis bleibt Core.
Plugins: `OGL Outfit Manager`, `OGL Outfit Preview`.

Text-only ist Core-Modus.

Reale Tag-/Nachtzeit ist Core-Environment-Override.

`OGL Weather` ist Plugin für Regen, Schnee, Nebel, Sturm und Wind; RTX nur optional.

## 8. NextGen Renderer

`OGLRender` ist die Render-Abstraktion.
OpenGL 3.3 bleibt zunächst Legacy/Fallback.
Vulkan wird langfristig primär.
RTX/Hybrid-Raytracing bleibt optional.

Geplant: PBR, HDR, moderne Schatten/Reflexionen, Terrain, Wasser, Atmosphäre, Post Processing, Streaming, GPU Culling, Upscaling.

## 9. HydroEngine

Einstufung: **SPÄTER VORBEREITEN**.

HydroEngine ist serverautoritativ. Der Viewer soll später dynamische Wasserzustände empfangen und darstellen. `region.water_height` bleibt kompatibel. Keine Hydro-Solver-Implementierung jetzt.

## 10. Zeitplanung

Grobe Planungswerte, keine festen Zusagen:

- nutzbare Alpha: 3–6 Monate
- brauchbare Beta: 6–9 Monate
- weitgehender Firestorm-Ersatz: 9–15 Monate
- vollständige langfristige OGL-Plattform: 12–24 Monate

Core-Alltagstauglichkeit kommt vor Vulkan/RTX/Weather/Hydro-Ausbau.

## 11. Build / Test

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
cmake --preset debug
cmake --build --preset debug -j2
ctest --preset debug --output-on-failure
```

Runtime-Smoke:

```bash
timeout 4s env DISPLAY=:0 ./build/debug/nexverse-viewer >/tmp/ogl-run.log 2>&1
cat /tmp/ogl-run.log
```

Timeout 124 ist bei erfolgreichem Start durch `timeout` normal.

## 12. Verhalten im neuen Chat

Wenn der Benutzer nur **„viewer“** oder **„weiter“** schreibt:

1. lokalen Repo-Stand prüfen
2. diese Übergabe + Projektplan lesen
3. keine unnötige Rückfrage
4. Phase 8: Renderer/Prims/Material/Scene weiterbauen
5. build + tests, bei GUI-Änderungen Runtime-Smoke
6. nicht signierte Module ohne ausdrücklichen Developer Mode niemals laden
7. `info.md` und `CHATUEBERGABE.md` aktualisieren
8. Commit und GitHub-Handoff-Branch synchron halten
9. Remote-Stand niemals destruktiv über lokale Arbeit legen

## Phase 8: laufender Codeblock (09.10.2026)

0.2.0-alpha.40: erste Legacy-Shiny-Darstellung pro Objekt/Face im
OpenGL-Fragmentshader, kein vollständiger PBR-/Normal-Map-Pfad.
21/21 CTest grün. Weiter mit Material-/RenderMaterial-Auswertung
und OpenSim-Facegruppen. `projectplan.md` bleibt maßgeblich.


## Neuester Rendering-Block · 0.2.0-alpha.41

- Getrennte deckende/transparente Material-Durchläufe.
- RGBA8-Alpha-Analyse bei Texture-Upload, kein pauschales Blending
  deckender Prim-Texturen.
- Transparent-Objekte stabil back-to-front sortiert;
  transparentes Depth-Write deaktiviert.
- Default `material_id` / `render_material_id` werden jetzt an
  `RenderSceneObject` übergeben, Face-Inheritance durch Tests abgesichert.
- `src/render/material_pass.cpp` + Header + `material_pass_tests.cpp`.
- 22/22 Tests grün, OpenGL-Viewer-Start OK, In-World-Vergleich offen.

**Nächster Schritt:** tatsächliche Legacy-/PBR-Material-Auflösung
aus OpenSim/CAPS anhand validierter Protokollantworten; keine
erfundenen Shader-Parameter aus der Material-UUID ableiten. Als
Fallback bleiben die bestehenden OpenSim-TextureEntry-Eigenschaften.
`projectplan.md` bleibt verbindlich.


## Neuester Legacy-Materials-CAPS-Block · 0.2.0-alpha.42

- Verifizierter OpenSimulator `RenderMaterials` GET: LLSD-XML `Zipped`
  -> zlib -> LLSD Binary Array mit Material-UUIDs und Definitionen.
- Begrenzter Parser, async GET nach CAPS Discovery; Fallback bei
  fehlender/fehlerhafter Capability. 23/23 Tests grün, OpenGL-Start OK.
- DiffuseAlphaMode (0-3) und AlphaMaskCutoff werden in Scene- und
  RenderFace-Daten übertragen; Renderer unterscheidet Mask/Blend/
  Opaque/Emissive und berücksichtigt Depth-Write.
- Noch KEIN vollständiges Live-Update per POST, kein GLTF/PBR-Asset-Fetch,
  keine Normal-/Specular-Maps, kein in-world End-to-end-Test.
- Nächster Schritt: MaterialIDs gezielt per POST nachladen und
  Cache/Scene-Invaliderung bei nachträglichen Änderungen erweitern.
- **Maßgeblich:** `projectplan.md`; Details: `docs/MATERIALS.md`.


## Neuester Entwicklungsblock · 0.2.0-alpha.43

- Phase 8: gezielte `RenderMaterials`-POST-Abfragen anhand
  serverseitig verifizierter OpenSimulator-Zipped-LLSD-Spezifikation.
- `src/protocol/render_materials.cpp`: UUID -> 16-Byte-Binary, zlib-Kompression, LLSD-XML-`Zipped`; bis
  32 deduplizierte UUIDs pro Batch, Antwort auf angefragte IDs begrenzt.
- `src/app/application.cpp`: nach initialem GET unbekannte MaterialIDs
  aus Default-/Face-Materialien asynchron nachladen; Cache bei
  geänderten Einträgen aktualisieren, Scene-Revision invalidieren,
  maximal drei Versuche bei fehlenden IDs, 2-Minuten-Cooldown.
- Tests: 23/23 CTest; GET/POST-Codec inklusive Negativ- und
  Response-Whitelist-Tests. OpenGL-Starttest bestanden.
- Weiter: Legacy-Normal-/Specular-Map-Pipeline, Material-UVs,
  Region-/Teleport-Wechsel und In-World-Validierung; danach PBR.
- Dokumentation `docs/MATERIALS.md`; Projektplan bleibt bindend.


## Neuester Phase-8-Block · 0.2.0-alpha.44

- OpenSimulator `SOPMaterial.cs` bestätigt Legacy-UV- und Glanzfelder
  inklusive Festkommaskalierung 1/10000.
- RenderMaterials-Parser ergänzt `Norm/SpecOffsetX/Y`,
  `Norm/SpecRepeatX/Y`, `Norm/SpecRotation`, `SpecColor`,
  `SpecExp`, `EnvIntensity`, mit Negativtests.
- Application übernimmt getrennte Legacy-Maps in Scene/Face-Material;
  GetTexture-Downloader lädt deren PNGs über denselben begrenzten Cache.
- OpenGL nutzt GL-Texturnummern 5/6, getrennte UV-Uniforms und
  eine derivative-basierte TBN-Normalapproximation sowie
  vereinfachten Specular-Lichtreflex. Ohne Assets Legacy-Fallback.
- 23/23 CTest grün; OpenGL-3.3-Shader-Start erfolgreich, Welcome geladen.
  Keine angemeldete OpenSim-Region visuell geprüft.
- Nächster Arbeitsblock: Region-/Teleport-Lifecycle, Material-Cache-
  Invalidierung und In-World-Vergleich; danach Phase 8 weiterführen.
- Bindend: `projectplan.md`; Detaildokument: `docs/MATERIALS.md`.


## Neuester Phase-8-Block · 0.2.0-alpha.45

- Regionsgebundene Material-Caches: WorldModel-Handle, Generation,
  Seed-Capability bilden eindeutige Anfrage-Epoche.
- `RegionMaterialScope` testet die Epoche unabhängig von Netz/Render.
- Separater regionsgebundener Seed-CAPS-Request für RenderMaterials
  und GetTexture; neue Region erhält eigene Capability-Adressen.
- Stale GET-/POST-Material- und Texture-Download-Resultate werden
  nach Teleport oder Grenzübertritt ignoriert, ohne auf Futures im
  Render-Thread zu warten.
- Pending IDs, Cooldowns, fehlgeschlagene Texture-IDs werden beim
  Regionswechsel invalidiert; Scene-Revision wird neu aufgebaut.
- Noch offen: vollständiger EventQueue-/Circuit-Wechsel beim Teleport,
  GPU-Asset-Eviction und Live-In-World-Test.
- Weiter mit Phase 8 nach `projectplan.md`; neues Scope-Header:
  `include/nexverse/protocol/region_material_scope.hpp`.

Build/Smoketest 0.2.0-alpha.45: 23/23 CTest, OpenGL 3.3,
NexVerse UI und Welcome erfolgreich; Teleport-End-to-End offen.


## Phase 8 – JPEG2000-GetTexture-Reparatur · 10.10.2026 · 0.2.0-alpha.46

Live-Beobachtung 0.2.0-alpha.45 (2026-10-10 00:13 bis 00:18):
- Echter Login erfolgreich, LLUDP UDP LIVE, 17 CAPS-Endpunkte,
  RegionHandshake, WorldModel READY, OpenSim RenderMaterials GET 46,
  POST-Nachladen und Terrain-Mesh bis 126040 Triangles.
- 300 Objekt-Texturfehler, 0 erfolgreich geladene Objekt-Texturen,
  Terrain-Texturen im Fallback. Frühere Fehlerdiagnostik war zu unscharf.
- CAPS EventQueueGet zeitweise erfolgreich, später wiederholte Retries.
  Das ist ein **eigenständiges** noch offenes Problem.
- 0.2.0-alpha.46: vcpkg `openjpeg` + explizites `libpng`.
  `TextureAssetLoader::fetch_rgba` probiert zuerst `format=png`,
  bei fehlender Serverkonvertierung/HTTP-/Decode-Fehler `format=x-j2c`.
- OpenJPEG dekodiert J2C sowie JP2 (inkl. RGB/RGBA, Alpha, Grau,
  grenzenbehaftete Dimensionen/Bit-Tiefe/Datenmengen) in RGBA8.
  Fehlermeldungen enthalten beide Abrufursachen und die Texture-UUID;
  keine geheimen CAPS-URLs im Log.
- Testerweiterungen: echte verlustlose 16x16 J2C-, JP2- und
  J2C-mit-Alpha-Fixtures; PNG-Erfolg, HTTP404-Fallback, ungültiges
  PNG, beide Formate HTTP404, defekte JPEG2000-Daten.
- **23/23 CTest**, Debug-Build, OpenGL-3.3-Smoke und Welcome erfolgreich.
  **Erfolgreiche NexVerse-Welttexturen noch nicht live bestätigt**,
  dafür Viewer neu starten und einloggen.
- Verbindlich `projectplan.md`; technische Details
  `docs/TEXTURES.md`, `docs/MATERIALS.md`.
- Nächste Arbeit: echten Download-Erfolg/Nicht-Erfolg inkl.
  konkreter HTTP-/Decoderfehler im Live-Log verifizieren,
  CAPS-EventQueue-Reconnect und sichtbare Welttexturen.


## CAPS EventQueue Recovery · 10.10.2026 · 0.2.0-alpha.47

- In-World-Live-Logs von alpha.46: CAPS-Live-Batch erfolgreich,
  danach wiederholte Poll-Fehler exakt alle 36–40 Sekunden.
  `CapabilityService::poll_event_queue` hatte nur 35 Sekunden
  HTTP-Total-Timeout; OpenSimulator hält einen leeren Long-Poll
  **bis zu 58 Sekunden**, antwortet dann regulär mit HTTP 502.
- Alpha.47 stellt den EventQueue-Timeout auf 80 Sekunden ein.
  HTTP 502 wird als normales leeres Keepalive behandelt, HTTP 404
  als abgelaufener Endpoint, andere HTTP-/Transport-/LLSD-
  Fehler mit HTTP-Status und Ursache protokolliert.
- EventQueueRecovery: gesunde Batches und 502 resetten den
  Fehlzähler; echte Fehler nutzen begrenzte 1/2/4/8/12-Sekunden-
  Backoffs. Bei 404 sofort, sonst jede dritte echte Störung
  neue `EventQueueGet`-Capability aus dem Seed abrufen.
  Unveränderte URL bewahrt das Ack, neue URL setzt Ack zurück.
  Reconnect-/Idle-Logs dokumentieren den Zustand ohne Token/URL-Leak.
- Der Stopp ist im Backoff nach spätestens 100 ms überprüfbar.
  23/23 CTest inklusive HTTP 502/404/503/Timeout/LLSD-
  Regressionen bestanden; Debug-Build und OpenGL-3.3-GUI-Smoke
  erfolgreich. **Authentifizierter EventQueue-Langzeittest mit alpha.47
  steht noch aus.**
- WICHTIG: Diese Recovery ist für den aktuellen Login-Region-Seed;
  vollständiger Teleport-/Regionswechsel mit neuem UDP-Circuit,
  alter EventQueue-Abmeldung und neuem Seed braucht separaten Block.
- Dokumentation: `docs/CAPS.md`, `projectplan.md` (maßgeblich).
- GitHub Branch weiterhin `develop-current` (nicht `develop`).
- Zum In-World-Test:
  `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl47-live.log`.


## Phase 8/9 – LLSD-Integer + OpenSim-Mesh-Pipeline · 10.10.2026 · 0.2.0-alpha.48

- Grundlage Live-Log alpha.47: Login/WorldModel OK, 293 Objekttexturen,
  4 Terraintexturen, 9 fehlende/defekte Texturassets, normaler
  EventQueue HTTP502-Idle bestätigt. Ein initiales `HTTP=200;
  LLSD integer is invalid` konnte sich selbst erholen.
- `parse_llsd_xml` akzeptiert nun leere/whitespace Integer-Elemente,
  optionale führende Pluszeichen und gültige 64-Bit-XML-Integer;
  vollständige Zahl-/Bereichsprüfung. Binary-LLSD-Wireformat
  bleibt 32 Bit; Langwerte bei Binary-Ausgabe nicht als interoperable
  Transportwerte verwenden.
- `ObjectUpdate.ExtraParams` mit 0x30 Sculpt-Block und Typ 0x05
  wird erkannt und mit UUID im WorldModel gespeichert.
  Partial-Compressed-/Terse-Updates erhalten **bestehende**
  Mesh-Identifier, dekodieren aber noch keine neuen Compressed-
  Mesh-ExtraParams. Das bleibt ein offener Block.
- Regionsgebundene GetMesh/GetMesh2-CAPS, einzelner asynchroner
  Download, bounded 32-MiB-Asset/16-MiB-LOD-Dekompression,
  binärer LLSD-Header-Prefix, High-/Medium-LOD und
  16-Bit-Positions-/UV-/Dreiecksdaten. CPU-Triangulation,
  Face-Material-Ranges, GPU-Upload durch OGLRender.
- UUID-Caches und Region-Epochen; keine alten HTTP-Replies in
  neue Region übernehmen. Ungültiges Mesh bleibt im sichtbaren
  Fallback. 256 einzigartige GPU-Meshes als vorläufiges Limit.
- Tests: 24/24 CTest inkl. synthetischem OpenSim-Mesh-Asset.
  Realer NexVerse-Mesh-Sichttest ohne erneuten Login
  **noch nicht durchgeführt**.
- Mesh-Rigging, Skinning/Animesh, OBJ/DAE-Import, Textur-Sculpties,
  vollständiges LOD-Streaming, Disk-LRU, Compressed-ObjectUpdate
  und kompletter Teleport-Reconnect sind **noch offen**.
- Projektplan verbindlich, Details `docs/MESH.md`.
- GitHub Handoff: `develop-current`, lokaler Branch entsprechend.


## alpha.49 – LLSD-Date und Mesh-Diagnostik · 10.10.2026

- Zuletzt bewiesene Live-Zahlen aus alpha.48:
  448 Meshanforderungen; 232 Erfolge, 216 Fehler
  (208 unbekannter LLSD-Binary-Marker, 7 Submeshes, 1 HTTP 404).
  287 erfolgreiche Objekttexturen, 9 fehlende/defekte Texturen.
  EventQueue HTTP502-Idle korrekt; einmaliger HTTP200-Fehler
  `Unsupported LLSD XML type: date` wurde anschließend recovered.
- Neu: `LlsdType::date`, XML `<date>` und Binary-Marker
  `d` (8-Byte-Big-Endian-IEEE754 epoch seconds) dekodieren.
  LLSD unknown-type Fehler melden jetzt `0xNN at offset N`,
  statt unlesbarem Steuerzeichen.
- Mesh-Asset-Fehlerprotokoll nennt sichere 8-Byte-Hex-Signatur,
  Gesamtlänge und LLSD-Fehlerposition (keine URL/Token).
- Optionales `TexCoord0Domain` erhält jetzt Standard-UV-Domain,
  invalide Submesh-Faces werden mit Rollback übersprungen,
  gültige restliche Faces bleiben renderbar.
- `GetMesh2` wird bevorzugt, `GetMesh` bei Fehler als
  einmaliger alternativer CAPS-Pfad versucht, solange getrennt
  angeboten. Regions-Epochenabsicherung und GPU-Limits bleiben.
- 24/24 Offline-Tests grün, echte In-World-Abnahme alpha.49 offen.
  Die bisherigen 208 unbekannten Mesh-Header sind **noch nicht
  als behoben bestätigt**; neue `prefix_hex`-/Offset-Logs
  sind die Basis für die nächste formatgenaue Korrektur.
- Ganz offene Funktionen: initiale SculptExtraParams in
  ObjectUpdateCompressed, vollständige Mesh-LOD, Rigging/Animesh,
  Teleport-Reconnect sowie 9 fehlerhafte/fehlende Servertexturen.
- Verbindlich `projectplan.md`. Detail `docs/MESH.md`,
  `docs/CAPS.md`. GitHub `develop-current`.
- Live-Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl49-live.log`.


## alpha.50 – Compressed-ObjectUpdate und Mesh-Prims · 10.10.2026

**Status:** Beim Start dieses Blocks gab es kein aktuelles
/tmp/ogl49-live.log auf der Linux-Maschine. Die 208 vorherigen
Mesh-Headerfehler sind deshalb weiterhin unbestätigt.

**Implementiert:**
- Vollständige ObjectUpdateCompressed-Pakete mit CompressedFlags,
  Owner UUID, optionalen Feldern AngularVelocity, Parent,
  ScratchPad, Text, MediaURL, Particles, Sound und NameValues.
- SculptExtraParams Typ 0x30 nach den optionalen Feldern;
  Mesh-UUID und Sculpt-Typ werden ins WorldModel übernommen.
- Vollständige 23-Byte-Prim-Form und TextureEntry-Daten mit
  32-Bit-Längenfeld und Face-Textur sowie Skalierung/Parent-ID.
- Bounds-Checks und Ablehnung abgeschnittener Compressed-Daten.
- Test: synthetisches vollständiges komprimiertes Mesh-Prim
  samt Scale/Parent/TextureEntry, plus Truncation-Test.
- Formatbeleg: OpenMetaverse ObjectManager.cs,
  ObjectUpdateCompressedHandler:
  https://github.com/openmetaversefoundation/libopenmetaverse/blob/master/OpenMetaverse/ObjectManager.cs

**Noch offen:** echter NexVerse-Live-Test; 208 vorherige
Headerfehler über prefix_hex / Byteoffset analysieren;
Mesh-LOD, Rigging/Animesh, Teleport-Regionswechsel und
fehlende Servertexturen.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl50-live.log


## 0.2.0-alpha.51 – Dynamischer GPU-Mesh-Cache · 10.10.2026

**Ausgangsbefund alpha.50:** Der Live-Test meldete 256 erfolgreiche
Mesh-Uploads, 2 vollständig fehlerhafte Mesh-LODs,
3 teilweise gerettete Submeshes. Durch die Bedingung
loaded_ids.size() < 256 wurden danach keine weiteren
Meshes angefordert. Das ist ein Cache-Limit und nicht
die tatsächliche Anzahl von Meshes der NexVerse-Region.

**Implementiert:**
- Neue pure Mesh-Residency-Planung mit nach Entfernung zum
  Avatar priorisierten Mesh-UUIDs und Duplikatentfernung.
- GPU-Asset-Budget 256. Werden durch Ortswechsel neue Meshes
  relevanter, verdrängen sie zuvor geladene entfernte Meshes.
- 24 Meter Prioritätsbonus für residente Meshes vermeidet
  Cache-Churn bei geringfügiger Bewegung.
- Plan-Neuberechnung bei SceneObject-Änderung oder nach
  maximal 750 ms; GPU-Eviction pro Zyklus auf acht begrenzt.
- Renderer: set_mesh_asset liefert Upload-Erfolg, dazu
  remove_mesh_asset und clear_mesh_assets für explizite
  GL-VAO/VBO/EBO-Freigabe auf dem Renderer-Thread.
- Bei Region-Epochenwechsel werden ältere GPU-Meshes gelöscht.
  Stale Mesh-HTTP-Ergebnisse und nicht mehr priorisierte
  Ergebnisse werden nicht hochgeladen.
- Unbekannte/doppelte UUIDs und nicht-finite Positionen
  werden bei der Auswahl ignoriert; Gleichstände deterministisch.
- Zusätzlicher Testsatz für Bewegung, Eviction, Hysterese,
  Deduplikation, NaN, Nullbudget und 320 Meshes; 25/25 CTests.

**Offen und begrenzt:**
- Echte In-World-Abnahme von alpha.51 bei Ortswechsel.
- Noch keine Frustum-/Occlusion-Priorisierung.
- Mesh-Kindposition nur angenähert mit Parent-Position
  plus lokalem Offset (ohne Rotation).
- Keine persistenten Mesh-Disk-/CPU-LOD-Caches;
  beim Zurückkommen können Downloads erneut erfolgen.
- Zwei vollständig ungültige Mesh-LODs, beschädigte oder
  HTTP404-Textur-Assets; Rigging/Animesh und Teleport
  bleiben eigenständige Entwicklungsblöcke.

Live-Test: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl51-live.log


## alpha.52 – Avatar sichtbar machen und Bewegung · 10.10.2026

**Auslöser:** Der Screenshot der verbundenen Welt zeigt
Terrain/Meshes, aber keinen eigenen Avatar und bislang
keine nachvollziehbare Bewegung. WORLD UDP LIVE beweist
weder den Avatar-Renderpfad noch die Bewegungsfähigkeit.

**Implementiert:**
- OpenGL-Avatar-Proxy-Pfad mit sieben Basis-Meshes für
  Kopf, Torso, Arme, Beine, für eigenen Avatar und bis
  zu 63 weitere Avatar-Datenpunkte. Die Darstellung wird
  pro Frame anhand der Agentposition aktualisiert.
- Die Proxies sind absichtlich keine echten OpenSim-Avatare:
  keine Skin-Bakes, Avatar-Meshes, Skeletons,
  Wearables, Haare, Attachments oder Animationen.
- Neues Keyboard-Control-Modul mit W/S/A/D, linker/rechter
  Pfeiltaste zur Rotation, Shift für schnelleres Bewegen,
  Space für Sprung, C für Abwärtsbewegung, F Flug
  per Toggle. Die AgentUpdate-Pakete enthalten dabei
  korrekte Körper-/Kopf-Quaternionen und Kameraachsen.
- Self-Avatar-LocalID wird aus ObjectUpdate bzw.
  ObjectUpdateCompressed gewonnen, mit dem RegionHandle
  gekoppelt und bei ImprovedTerseObjectUpdate
  zur Aktualisierung der serverbestätigten eigenen
  Avatarposition im WorldModel verwendet.
- Bei Steuerungsänderungen und bestätigten Simulator-
  Positionsupdates werden diagnostische Logzeilen geschrieben.
  Fokusverlust sendet keine gedrückten Bewegungs-Keys.
- Nach erfolgreichem Login wird das Login-Eingabefeld
  nicht mehr durch die Bewegungssteuerung verändert.
- Regressionstests für Steuerbits, Quaternion, Fly-Toggle
  und Steuerungsreset; 26/26 CTests grün.

**Noch offen und nicht als fertig behaupten:**
- Echte NexVerse-In-World-Bewegungsabnahme, insbesondere
  muss die Self-Avatar-LocalID und die Terse-Positions-
  Bestätigung mit neuer Version beobachtet werden.
- Richtige Firestorm-Avatar-Appearance (Skeleton,
  Body Shape, Skin-Bakes, Wearables, Attachments,
  Animationen) sowie Mouselook, Camera-Orbit/Zoom,
  Teleport/Regionswechsel, Collision/Reconciliation.
- Die einfachen Avatar-Proxies sind nur eine
  Übergangsdarstellung bis zu Phase-10-Appearance.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl52-live.log


## 0.2.0-alpha.53 – Phase 8: OpenSim-Point-Lights · 10.10.2026

**Verbindliche Nutzerentscheidung vom 10.10.2026:**
Fortsetzung der Entwicklung entlang der Reihenfolge des
projectplan.md. Die zuletzt untersuchten Avatar-/Bewegungs-
und übrigen Rendering-Asset-Fehler werden **zunächst
nicht behoben**, sondern dokumentiert und für eine
spätere gemeinsame Fehlerbehebungsphase vorgemerkt.
Keine implizite Behauptung, dass die Probleme gelöst seien.

**Jetzt neu in Phase 8 implementiert:**
- Point-Light ExtraParams Blocktyp 0x20, 16 Byte:
  4x RGBA-UNORM8 (Alpha = Intensität), dazu
  little-endian float32 Radius, Cutoff und Falloff.
  Implementierung orientiert sich an libopenmetaverse,
  Primitive.LightData.
- Sichere Bounds/Finite-/Radius-Prüfungen;
  Daten werden in vollständigem ObjectUpdate und
  ObjectUpdateCompressed nach SculptExtraParams separat
  dekodiert. Bei Partial-Updates behalten vorhandene
  Objekte ihre bereits bekannte Lichtdefinition.
- Eigenes Feld im WorldModel für lokale Lichtquellen.
- Die acht nächsten Punktlichter werden aus
  den Objektpositionen pro Frame ausgewählt. Child-Prim-
  Position wird vorläufig mit Parent+Offset angenähert.
- Legacy OGLRender: Weltposition aus uModel-Transform,
  acht Point-Light-Uniforms und diffuse Beleuchtung
  von nicht-fullbright Objekt-/Meshflächen nach
  Abstand, Radius, Intensität und Falloff.
- Terrain/Avatar werden in dieser ersten Version
  noch nicht von den lokalen Punktlichtern beleuchtet.
  Keine dynamischen Schatten, keine Projektoren,
  keine PBR-Energiekonservierung.
- Kompletter Debug-Build, 26/26 CTests bestanden,
  GL-3.3-Smoke auf RTX 4070 SUPER erfolgreich.
  **In-World-Sichtabnahme mit echter Lichtquelle
  steht noch aus**.

**Bekannte Fehler für spätere Fehlerphase (unverändert):**
- Eigene Avatar-LocalID wurde in alpha.52 Live-Log nicht
  gebunden, daher keine serverbestätigten
  Self-Avatar-Positionsupdates / keine verifizierte
  Benutzbarkeit der WASD-Bewegung.
- Eigenes Erscheinungsbild ist bisher nur Proxy,
  echtes Skeleton/Bakes/Wearables fehlen.
- Alpha.52: 252 geladene, vier fehlgeschlagene Meshes;
  neun fehlende/beschädigte Objekttexturen.
- Keine aktuelle Bestätigung einer GPU-Cache-Eviction
  bei tatsächlichem Ortswechsel.

**Fortsetzung Phase 8:** Partikel-Basis, weitere
Lichttypen (Projektor/Material-Licht), Render-Performance
und anschließend Phase 9 bzw. weitere Funktionssysteme.
Maßgeblich bleibt der Plan, nicht die Bugliste.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl53-live.log


## 0.2.0-alpha.54 – Phase 8: OpenSim ParticleSystem/PSBlock · 10.10.2026

**Nutzerentscheidung:** Nach projectplan.md die Funktionalität
weiterentwickeln und bekannte Fehler erst später beheben.
Die Priorität bleibt Phase 8 vor neuen großen Folgephasen.

**Geliefert:**
- OpenSim ObjectUpdate PSBlock (Variable1 nach MediaURL) und
  ObjectUpdateCompressed Partikelblock (CompressedFlag 0x08,
  86 Bytes) werden abgegriffen und mit strikter Größenprüfung
  dekodiert; ungültige/fehlende Blöcke blockieren den Prim nicht.
- Protokollkompatibles Legacy-ParticleSystem mit 68-Byte-
  Systemteil und 18-Byte-Partikeldaten (86 Byte) sowie
  erweitertem Format 94–98 Byte mit Größenfeldern,
  Glow-/Blend-Metadaten; Bytes nicht aus C# kopiert, sondern
  eigenständiger C++23-Parser.
- Emissionsparameter: Source/Particle-Lifetime, Burst-Rate,
  BurstCount, BurstRadius, Min/Max-Speed, Beschleunigung,
  Start/End-Farbe, Scale, Textur-UUID (nur Referenz)
  sowie Pattern und Flags.
- Partikelsystem liegt im WorldModel der jeweiligen Prim;
  Terse-/Legacy-Partial-Updates bewahren den bekannten Wert,
  KillObject entfernt den Prim mitsamt Emitter.
- Render-Integration: maximal 32 nahe Emitter in ca. 192m
  Radius, maximal 768 Partikel in einem Drawcall; eigener
  OpenGL-3.3-Shader und Streaming-VBO. Weiche runde Sprites
  mit alphablendfähigem Fade, Z-Test ohne Z-Writes.
  Die Positionen sind deterministisch generierte Grund-
  Partikelpositionen mit Ballistik/Beschleunigung.
- Unterstützte Basispfade: Drop, Explode und grob nach
  oben gerichtete Angle-/Cone-Patterns; keine native
  Target-/Ribbon-/Beam-Logik, Wind, Bounce, Rotationsflags,
  korrekte Emitter-Restart-Events, Textur-Sampling oder
  diskrete persistente Partikel-Physik.
- Smoke-Test: Linux 3440x1440, NVIDIA RTX 4070 SUPER,
  GL 3.3, Welcome-UI lädt, kein Shaderfehler gemeldet.
- CMake-Debug Build und 26/26 CTests; Parser-Regressions-
  tests für Legacy und Extended ParticleSystem,
  invaliden Datenblock, ObjectUpdate und
  ObjectUpdateCompressed mit aktivem PSBlock.
- **Die erste echte In-World-Sichtprüfung ist offen.**
  Simulierte Sprites sind kein fertiger Firestorm-
  Partikelrenderer. Keine Änderungen an der
  bekannten Avatar-/Bewegungssynchronisationslücke.

**Spätere Ausbaustufen im Plan:** Partikeltexturen über
AssetCache, Ziel-Partikel, Ribbon/Beam, Wind/Bounce,
lokal zustandsbehaftete LSL-Emission, Partikel-LOD
und GPU-Instancing. Danach Phase 8 weitere
Material-/Licht-Effekte und Phase 9 Asset-Streaming.

Live-Start: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl54-live.log


## 0.2.0-alpha.55 – Phase 8: Partikeltexturen / GetTexture · 10.10.2026

**Verbindlicher Kurs:** Umsetzung der Funktionen aus projectplan.md
fortsetzen. Die bekannten Avatar-/WASD-/Mesh-/Texturfehler bleiben
auf Nutzerwunsch für die spätere Fehlerphase zurückgestellt.

**Neu:**
- Die in OpenSim ParticleSystem/PSBlock enthaltene Particle
  Texture-UUID wird nicht mehr nur gespeichert: Für sichtbare
  Emitter (maximal 32 im Umkreis) wird die Textur im vorhandenen
  GetTexture-Asset-Loader vor regulären Hintergrund-Objekttexturen
  angefordert und als RGBA8/ggf. JPEG2000 dekodiert.
- Emittierte Bilder teilen denselben 512-Assets-GPU-Cache mit
  Object-TextureEntry; gleiche UUID wird nur einmal angefragt
  und kann für Mesh/Prim und Partikel gleichzeitig genutzt werden.
  Die bereits bekannte Epochen-Prüfung verhindert das Hochladen
  veralteter GetTexture-Replies aus früheren Regionen.
- Neuer Pure-Logic Asset-Priorisierer prüft null/Zero-UUIDs,
  doppelte IDs, bereits geladene und zuvor fehlgeschlagene
  Requests. Bei Bedarf wird auf Objekttexturen zurückgegriffen.
  Eigener automatischer Regressionstest.
- Der Particle-Fragmentshader übernimmt echte Textur-RGBA
  über GL_TEXTURE4 (getrennt nach Textur-UUID gebatcht),
  verrechnet sie mit Start-/End-Farbe und Soft-Mask-Alpha.
  Bei nicht geladenem Asset nutzt er weiterhin die zuvor
  existierende weiche Kreis-Sprite-Darstellung.
- Maximal 768 Partikel/Frame, höchstens 32 Emitter; künftig
  statt eines einzelnen bis zu einem Drawcall je
  verwendeter Partikeltextur plus optional Fallback-Batch.
  Keine eigenen GL-Texturduplikate, kein weiteres HTTP-Subsystem.
- Neue Tests fuer priorisierte Assetwahl, gemeinsame Cache-
  Nutzung, ungültige IDs, geladene und fehlgeschlagene Texturen.
  **27/27 CTests**, CMake-Debug-Build sowie OpenGL-GUI-Smoke
  auf NVIDIA RTX 4070 SUPER ohne Shaderfehler.
- Echte In-World-Sichtabnahme steht noch aus; das vorhandene
  Cachelimit von 512 Assets, transparenzabhängige Sortierung,
  Partikeltextur-Orientierung, diskrete Simulation und
  exakte OpenSim-Partikelmodi bleiben spätere Schritte.
- Gesammelte Avatar-/Bewegungs-/Mesh-Fixphase ausdrücklich
  nicht vorgezogen.

Weitere Planblöcke: Wind-/Bounce-/Target-/Ribbon-Partikel,
Effekt-Lebenszyklen/Blending und komplexere Materialbeleuchtung,
danach Phase 9 Asset-Streaming und übrige Viewerfunktionen.
Protokolldetails: docs/PARTICLES.md.

Live-Log: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl55-live.log


## 0.2.0-alpha.56 – Großrelease Phase 8 / vier Entwicklungsblöcke · 10.10.2026

Auf Nutzerwunsch werden zukünftig **3–4 zusammenhängende Funktionen
pro Release** umgesetzt, mit einem gemeinsamen Abschluss aus Build,
Tests, Versionspflege, Dokumentation und GitHub-Commit. Einzelne
Zwischentests bleiben zur Qualitätssicherung möglich.

**Block A – Partikelphysik:**
- Neuer testbarer reiner Partikelkinematik-Helfer, mit Begrenzung
  von Lebensdauer und festen Integrationsschritten (48).
- Gravity/PSBlock-Beschleunigung, Wind-Flag 0x008 und Bounce-Flag
  0x004. Wind ist aktuell ein **lokal generiertes Feld** und
  keine behauptete Simulator-Windmessung; Bounce erfolgt auf der
  Höhe der Quellprim, nicht durch Terrain-/Collision-Abfragen.
- Flags für InterpColor (0x001) und InterpScale (0x002) werden
  berücksichtigt statt unbedingter Interpolation.

**Block B – Ziel- und Verbindungseffekte:**
- Target UUID aus dem 68-Byte-PSBlock-Systemteil auslesen;
  bekannte Zielprims, Avatare und eigener Agent werden anhand
  UUID dem Emitter zugeordnet; unbekannte Ziele bleiben
  unaufgelöst, ohne auf erfundene Positionen zu springen.
- TargetPos (0x040) und TargetLinear (0x080) im CPU-Sampler,
  Beam (0x200) als verteilte Partikel entlang einer Strecke
  und Ribbon (0x400) als zeitlich versetzte Punktspur.
- Echte zusammenhängende Ribbon-Flächen, SourceRotation,
  Target-Bewegungs-Prediction und spezifische Formmodi sind
  **noch offen**.

**Block C – Transparenz, Blending, Budget:**
- OpenMetaverse ParticleSystem BlendFunc-Enums 0..9 für
  Extended PSBlock werden validiert und mit OpenGL abgebildet.
  Default SourceAlpha=7, OneMinusSourceAlpha=9.
- Neue distanzbasierte Sortierung in acht Tiefenbändern,
  innerhalb eines Bands Batching nach Blend/Texture.
- Feste Obergrenzen von 768 Partikeln und maximal 192
  Zeichen-Batches schützen vor übergroßer Drawcall-Zahl.
- Das ist nur näherungsweise Alpha-Tiefensortierung; echte
  per-Partikel-Sortierung über alle Texturen ist offen.

**Block D – Beleuchtung/Materialgrundlagen:**
- Bei den bis zu acht Legacy-Point-Lights wird zusätzlich
  zum bestehenden Radius/Falloff der Cutoff-Wert im Shader
  als näherungsweiche Kernausblendung berücksichtigt.
- Lokales Blinn-Phong-Highlight für glänzende,
  nicht-fullbright Objektoberflächen ergänzt.
- Kein vollständiges PBR, keine Projektorlichter, keine
  dynamischen Schatten oder Energieerhaltung.

**Abnahme:** Debug-CMake + C++23 erfolgreich, 28/28 CTests
einschließlich neuer reiner Wind-/Bounce-/Target-Tests,
Parser-Tests für TargetUUID und Blend-Enums;
OpenGL-GUI-Smoke auf NVIDIA RTX 4070 SUPER, Version alpha.56,
NexVerse-Welcome geladen, keine Shader-Fehler (der Exit 124
des Smoke-Tests ist die beabsichtigte 7-Sekunden-Begrenzung).
**Authentifizierter In-World-Sichttest dieser Effekte ausstehend.**

**Zurückgestellte Fehler unverändert:** Eigene Avatar-LocalID
und Bewegungsbestätigung, echte Avatar-Skeleton-/Appearance,
defekte/fehlende Mesh-/Texturassets und Teleport/Region-Handoff.
Keine dieser Fehlergruppen wird in diesem Release als gelöst
ausgegeben.

**Nächste gemeinsame Blöcke:** echte PSBlock-Emitter-Lebenszyklen,
Parent-/ObjectRelative- und Target-Pfade, verbundene Ribbons
und Beam-Geometrie, Asset-Caching und moderne Materialeffekte.
Die Reihenfolge bleibt an projectplan.md gebunden.

Live-Abnahme:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl56-live.log


## 0.2.0-alpha.57 – Großrelease: vier zusammenhängende Funktionsblöcke · 10.10.2026

**Block 1 – Emitter-Lebenszyklus / ObjectRelative**
- Emitterstart auf Renderer-Zeitbasis (nicht mehr globaler Partikelphase).
  CRC-Wechsel startet ein emittierendes Prim neu; entfernte Emitter
  werden aus dem Laufzeitstatus entfernt.
- SourceMaxAge stoppt neue Bursts, lässt bereits gestartete Partikel
  innerhalb ihrer eigenen PartMaxAge-Zeit auslaufen.
- Rotationskompatibilität: bei OpenSim SourceFlags ObjectRelative
  (Bit 0) werden die Quellbeschleunigung und Initialgeschwindigkeit
  durch die Objekt-Quaternion transformiert.
- Offene Details: vollständige LSL-Start/Stop- und SimTime-Synchronisation,
  Quelle mit ParentRotation, Simulator-Wind statt synthetischem Wind.

**Block 2 – durchgehende Beam-/Ribbon-Geometrie**
- Neuer Kameraseiten-ausgerichteter Ribbon-Generator mit
  verbundenen Dreiecken statt einer bloßen Punktfolge.
  Separater OpenGL-3.3-Triangle-Shader/-VAO/-VBO, UVs,
  transparente Soft-Edges sowie geteilte GetTexture-Assets.
- Beam folgt dem bekannten Zielpunkt; Ribbons folgen
  einer auf Basis der Partikelkinematik erzeugten Spur.
  Begrenzung 32 Emitter/je 16 Samples und 15 Segmente.
- Offene Details: echte bewegungsabhängige kontinuierliche
  Emitter-Historie, native Ribbon-Rotation und robuste
  Transparenzsortierung aller überlappenden Flächen.

**Block 3 – Partikelmaterial / Glow**
- In Extended PSBlock vorhandener Start/EndGlow wird an den
  GPU-Renderer durchgegeben und für Sprites und Ribbon-Material
  als begrenzte RGB-Aufhellung berücksichtigt; nicht als
  echtes physikalisches HDR- oder PBR-Emissive ausgegeben.
- OpenSim-Blend-Enums 0..9 aus alpha.56 bleiben aktiv.

**Block 4 – Phase 9: wiederverwendbarer Texture-Asset-Cache**
- Frühere harte 512-GPU-Texturen-Grenze wird durch ein
  dynamisches Residency-Budget ersetzt; unbenutzte Texturen
  werden als GL-Texturen freigegeben, höchstens 8 pro
  Planungsrunde (750ms).
- Reihenfolge: nahe Partikelemitter, anschließend Prim-/
  Meshmaterialien; letztere erhalten eine Objekt-Distanz-
  priorisierte Ordnung beim Aufbau der Scene-Revision.
- Neue Region leert den GPU-Objekttexturcache; veraltete
  HTTP-Replies außerhalb von Region/Epoche/Residency werden
  verworfen. Gemeinsames GetTexture-Ladesystem bleibt.
- Grenzen: noch kein persistentes CPU-/Disk-Texture-Cache,
  keine Texture-LOD-/Mip-Streaming-Stufen, keine Occlusion,
  UI/Textur-Distanz-Neusortierung nur bei Scene-Update;
  beim Zurückkehren eviktierter Texturen erneut HTTP möglich.

**Tests:** 30/30 CTests inklusive Lifecycle, Quaternion,
Ribbon-Triangles, Cache-Umschichtung. C++23-Debug-Build
und OpenGL-3.3-Starttest auf NVIDIA RTX 4070 SUPER
ohne gemeldeten Shaderfehler. Der 7-Sekunden-Exit 124
ist der absichtliche Timeout, kein Crash.
**NexVerse In-World-Darstellung bleibt ungeprüft.**

**Auf Nutzerwunsch zurückgestellt:** Avatar-LocalID/
serverbestätigte Bewegung, echte Avatar-Appearance,
defekte/missing Mesh-/Texture-Assets und Teleport.
Kein dieser Fehlerbereiche wird als behoben ausgegeben.

Nächster Großrelease-Block: Asset-Caching-Disk/CPU,
LOD/Prioritätsstufen und weitere OpenSim-Renderer-
Kompatibilität. projectplan.md bleibt verbindlich.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl57-live.log


## 0.2.0-alpha.58 – Phase 9 Großrelease (vier Blöcke) · 10.10.2026

Nutzerentscheid: Jeweils drei bis vier vollständige Entwicklungsblöcke
pro Release; bekannte Avatar-/Bewegungs-/Mesh-Asset-Fehler bis
zur späteren Bugfixphase zurückstellen. Verbindlich: projectplan.md.

**Block A – Persistenter Texture-Disk-Cache:**
- TextureDiskCache lädt/speichert validierte RGBA8-Assetdaten
  unter $XDG_CACHE_HOME/opengenesislink/textures-v1 bzw.
  $HOME/.cache/opengenesislink/textures-v1.
- 8-Byte-Version-Magic, Width/Height, FNV-64-Inhaltshash über
  Dimensionen und Pixeldaten, Größen-/Bounds-Prüfungen, sichere
  UUID-Dateinamen, temp-Datei + Umbenennung.
- 256 MiB Diskbudget, 64 MiB maximales Einzelbild,
  least-recently-used-ähnliche Eviction anhand File-MTime;
  auf Cache-Miss/Fälschung/IO-Problem normaler GetTexture-Download.
- Cache wird im Hintergrund-Texture-Future geprüft; bei Erfolg
  wird HTTP übersprungen. Kein persistenter Mesh-/Animationscache
  in diesem Release, keine echte Hash-/Version-Aushandlung mit
  dem OpenSim-Asset-Service.

**Block B – Dynamische Texturprioritäten:**
- Texturpositionen werden bei Object-/Scene-Update für Default,
  Face sowie Normal-/Specular-Materialien gesammelt.
- Alle 750ms/Scene-Revisionsänderung werden sie ausgehend von
  der aktuellen Avatarposition nach Entfernung neu sortiert.
- Identische UUIDs werden zusammengefasst; bereits GPU-geladene
  Assets erhalten 8m Hysterese gegen Thrashing.
- Partikel-Assetpriorität sowie 512 gleichzeitige GPU-Texturen,
  begrenzte GL-Eviction und Regions-Epochenprüfungen bleiben.

**Block C – Mesh-Sichtgrößenpriorität:**
- MeshDistanceSample führt zusätzlich den maximalen
  Prim-Scale-Durchmesser; Priorisierung gewichtet
  distance / sqrt(clamp(diameter,0.5,16)).
- Größe beeinflusst ausschließlich Assetabruf/Priorität,
  **noch nicht** echte Mesh-LOD-Stufen, View-Frustum-/Occlusion-
  Culling oder Mesh-Vertex-Dekodierung. Defekte Meshes werden
  nicht als repariert bezeichnet.

**Block D – GPU-Sampling und Asset-Diagnose:**
- Bei unterstützter GL_EXT_texture_filter_anisotropic
  aktiviert das Legacy-GL-Backend für Objekttexturen
  bis zu 8x anisotrope Filterung zusätzlich zum Mipmap-Sampler.
  Auf inkompatiblen GPUs bleibt das alte Verhalten.
- Log alle ~60s: kumulative Disk-Cache-Hits, HTTP-Downloads,
  GPU-Evictions, residente Texturen und GPU-Slotbudget.
  Die Zähler sind Sitzungsstatistik, keine serverbestätigte
  Bandbreitenmessung.

**Verifikation:**
- CMake Debug C++23 erfolgreich; 32/32 CTests.
- Neue Tests für persistente Cacheintegrität, manipulierte
  Cache-Dateien, Neustart-/LRU-Verhalten, bewegungsabhängige
  Texture-Distance-Ordering/Hysterese und größenbasierte
  Mesh-Priorität.
- Linux NVIDIA GeForce RTX 4070 SUPER OpenGL 3.3 GUI-Smoke
  bei 3440x1440 mit Welcome-Seite, keine gemeldeten GL-
  Shaderfehler; Exitcode 124 war beabsichtigtes 7s-Timeout.
- **Authentifizierte In-World-Streaming-Abnahme ausstehend.**

**Bekannte, absichtlich unangetastete Probleme:**
Self-Avatar-LocalID/Bewegung, echtes Avatar-Skeleton/Bakes,
noch fehlerhafte/fehlende Mesh-/Texturassets, Teleport.
Keiner dieser Punkte wird als geschlossen markiert.

**Nächste Großrelease-Ideen gemäß Phase 9:** Mesh-Disk-Cache,
RAM-Assetcache, Chunk-/Mip-Streaming-Priorität, GPU/CPU-
Speicherbudget und anschließend Avatar-Funktionssysteme.

Live-Befehl:
./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl58-live.log


## 0.2.0-alpha.59 – GitHub-Handoff · 11.10.2026

**Block A – persistente Mesh-Dateien:** 512 MiB Diskbudget,
Versionmagic OGLMESH1, Little-Endian, UUID-gebundene FNV64-
Integritätsprüfung, Größen-/Index-/Face-Checks und MTime-LRU.
Keine CAPS-Token/URL-Cachedaten.

**Block B – Mesh-RAM-LRU:** threadgeschützte CPU-Geometry-
Residency mit 64 MiB Bytebudget und LRU-Eviction. Durch
GPU-Eviction entfernte Meshes können ohne HTTP erneut
aus dem RAM geladen werden.

**Block C – Integration:** asynchroner Ladepfad RAM → Disk →
GetMesh2/GetMesh; Region-/Epochenschutz und GPU-Budget 256
Meshes (max. acht Evictions je Planung) unverändert.

**Block D – Telemetrie und Tests:** getypte Cacheherkunft,
60s-Metriken über erfolgreiche GPU-Uploads nach Quelle,
CPU-RAM-Bytes und GPU-Slots. Manipulations-, Neustart-,
UUID-, LRU-/Budget- und Fehler-Fallback-Tests.
CMake Debug C++23, **33/33 CTests bestanden**,
OpenGL 3.3 GUI-Starttest auf NVIDIA RTX 4070 SUPER mit
NexVerse-Welcome erfolgreich; Timeout 124 erwartungsgemäß.

**Noch offen:** In-World-Abnahme, echtes Mesh-LOD-Streaming,
GPU/CPU-Bytebudget und Texture-Mip-Streaming.
Avatar-LocalID, WASD, Skeleton/Bakes, defekte Server-Mesh-/
Texturassets und Regions-Teleport bleiben auf Nutzerwunsch
bis zur Bugfixphase zurückgestellt.
`projectplan.md` bleibt verbindlich.
Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl59-live.log`.

Arbeitsstand zu Beginn: `b5271bc` auf `develop-current`.
Nächste 3–4 Blöcke: echte Mesh-LOD-/Mip-Stufen und
GPU-/CPU-Bytebudgets. Vor jedem weiteren Commit
Git-Status prüfen, keine destruktiven Resets/Cleans;
CMake Debug, CTest und GUI-Smoke nach Änderungen.


## 0.2.0-alpha.60 – GitHub-Handoff · 11.10.2026

**Block A – OpenSim-LOD-Decoder:** High, Medium, Low und Lowest. Gewünschte Stufe zuerst; bei fehlenden/defekten Chunks Fallback auf andere gültige Level. Bestehende LLSD-/zlib-/Mesh-Bounds gelten weiter.

**Block B – Adaptive LOD-Planung:** `plan_mesh_lods` gewichtet Entfernung und Primgröße, dedupliziert mehrfach verwendete Mesh-UUIDs und nutzt Hysterese. Aktualisierung alle etwa 750 ms oder bei Szene-Änderung.

**Block C – LOD-getrennte RAM-/Disk-Caches:** High behält den bisherigen `<uuid>.mesh`-Schlüssel; andere Stufen werden unter `<uuid>.<lod>.mesh` inklusive LOD-gebundener Checksumme gespeichert. Budgets 64 MiB RAM und 512 MiB Disk. Nach Server-Fallback bezeichnet der Cache-Key den angeforderten, nicht zwingend den tatsächlich dekodierten LOD.

**Block D – GPU-Swap und Diagnose:** Mesh-LOD-Upgrades und -Downgrades ohne vorherige Entfernung der Mesh-UUID. GPU lädt zuerst neue Buffers, gibt alte erst nach erfolgreichem Upload frei. Aktuelle Regions-Epoche und Ziel-LOD werden vor Upload geprüft. Neues Log: LOD-Swap-Zähler und geschätzte VBO/EBO-Bytes.

**Verifikation:** C++23-Debug-Build, 34/34 CTests (LOD-Codec/Fallback, Cache-Varianten, Hysterese). OpenGL-Start-Smoke zum Release. Weiter offen: authentifizierter In-World-LOD-Test, GPU-Bytebudget, Texture-Mip-Streaming, Rigging/Animesh und zurückgestellte Avatar-/Bewegungs-/Teleport-Fehler.

Live: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl60-live.log`. Maßgeblich: `projectplan.md`, Branch `develop-current`.

**Nächste 3–4 Blöcke:** Texture-Mip/Chunk-Streaming, durchgesetztes GPU-Bytebudget, priorisierte Asset-Anfragen. Verbindlicher Prozess: GitHub-Branch `develop-current` lesen (nicht `develop`), `projectplan.md`, `info.md`, `CHATUEBERGABE.md` und relevante Docs pflegen, Debug-Build/CTest/GUI-Smoke, danach Code und Docs gemeinsam pushen.


## 0.2.0-alpha.61 – Phase 9 Handoff · 11.10.2026

**Block A – CPU-Mip-Vorstufen:** `texture_mip_image` erzeugt bis zu drei kleinere RGBA8-Basisstufen mit alpha-gewichteter Mittelung (premultiplizierte Farbsummen); NPOT-Bilder werden begrenzt unterstützt. Der Renderer erzeugt darauf weiter gewöhnliche OpenGL-3.3-Mipmapketten. Es wird kein JPEG2000-Progressive- oder HTTP-Chunk-Streaming vorgetäuscht.

**Block B – Distanzabhängige Qualitätswahl:** `plan_texture_mip_levels` wählt 0/1/2/3 je nach Agentabstand (24/64/128 m), mit Hysterese an den Grenzwerten. Mehrfach verwendete Texture-UUIDs verwenden die nächste Objektposition; aktive Partikeltexturen erhalten Vorrang mit Level 0. Die Planung läuft bei Scene-Revision oder spätestens etwa alle 750 ms.

**Block C – Asynchroner Full-Resolution-Cache/Hot-Reload:** HTTP und der bestehende 256-MiB-RGBA8-Disk-Cache speichern ausschließlich Originalbilder. Der asynchrone Worker erstellt daraus die gewünschte CPU-Mipstufe. Ändert sich die Entfernung, kann die vorhandene GPU-Textur erneut hochgeladen und verkleinert oder vergrößert werden, ohne die ursprüngliche GetTexture-Assetdatei erneut laden zu müssen. Region-/Epochenschutz, Residency und veraltete Qualitäts-Requests werden vor dem Upload geprüft.

**Block D – Geplantes 256-MiB-GPU-Texturbytebudget:** Mit `gpu_rgba8_mip_bytes` werden RGBA8-Mipketten berechnet; `plan_texture_byte_budget` kann vor Upload Texturen niedrigerer Priorität zur Eviction bestimmen. Höher priorisierte Texturen werden nicht verdrängt, um weiter entfernte Assets nachzuladen. Kann ein Upload nicht ins Byte-/512-Textur-Slotbudget passen, wird er zurückgestellt statt unkontrolliert zu wachsen. Telemetrie: aktuelle geschätzte Texturbytes, LOD-Swaps, zurückgestellte Uploads und GPU-Evictions.

**Verifikation:** Debug-Build C++23, 35/35 CTests einschließlich RGBA-Alpha, ungerader Dimensionen, Mip-Byteberechnung, Distanzhysterese und Budgetpriorität. OpenGL-3.3-GUI-Smoke nach Versionspflege. Live-In-World-Abnahme mit echtem NexVerse-Login steht aus.

**Abgrenzung:** Die 256 MiB sind ein softwareseitig aus RGBA8-Mipdimensionen berechnetes Texturbudget, keine gemessene oder vollständig durchgesetzte physische VRAM-Gesamtgrenze. Terrain-, UI- und Mesh-GPU-Ressourcen gehören nicht zu diesem Texture-Budget. Bei sehr großen/zahlreichen Bildassets können zusätzliche Inhalte unter Budgetdruck vorerst im Fallback bleiben.

**Offen:** Netzwerk-Chunks und native JPEG2000-Progressive-LOD, effizientes CPU-RAM-Caching für sämtliche Texturlevel, gemeinsames GPU-Bytebudget für Mesh/Terrain/Texture, authentifizierter Streaming-Sichttest; bekannte Avatar-/WASD-/Mesh-Asset-/Teleportfehler bleiben auf Nutzerwunsch für die spätere gesammelte Fehlerbehebung dokumentiert.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl61-live.log`.

**Nächste Releasegruppe (3–4 Blöcke):** Asset-Prefetch nach Sichtkegel, Texture-Mip-Debounce/RAM-Slab-Cache, gekoppelte GPU-Budgets für Mesh+Texture+Terrain (zunächst vorsichtig), messbare Streaming-Performance. Weiter nur auf `develop-current` arbeiten, divergiertes `develop` nicht verwenden. Nach jedem Großblock Build + alle Tests + GUI-Smoke, `projectplan.md`/`info.md`/`CHATUEBERGABE.md`/einschlägige Docs aktualisieren, Commit und GitHub-Synchronisierung.


## 0.2.0-alpha.62 – Handoff und nächste Arbeit · 11.10.2026

**Block A – Kameraabhängige Asset-Priorität:** Ein weicher Blickrichtungsfaktor gewichtet Textur- und Mesh-Asset-Anfragen relativ zur Third-Person-Kamera. Objekte hinter dem Betrachter werden nicht ausgeblendet; sehr nahe Objekte bleiben grundsätzlich hoch priorisiert. Die bestehenden Entfernungssortierungen und GPU-Residency-Grenzen bleiben erhalten. Das ist keine echte Frustum- oder Occlusion-Culling-Implementierung.

**Block B – CPU-RAM-Texturcache:** Ein threadgeschützter 96-MiB-LRU-Cache hält vollständige dekodierte RGBA8-Bilder pro gültiger UUID vor. Der asynchrone GetTexture-Pfad arbeitet RAM → persistenter Diskcache → HTTP/Decoder. Für kleinere GPU-Mip-Basisbilder wird das dekodierte Original aus dem RAM wiederverwendet. Keine Cache-Speicherung von CAPS-URLs, Tokens oder ungültigen Bildern.

**Block C – gemeinsam begrenzte GPU-Mesh-/Objekttexturbytes:** Geschätztes, softwareseitiges gemeinsames Budget von 384 MiB mit zusätzlich jeweils 256 MiB als Maximalgrenze für Mesh-Buffer und Objekttexturen. Vor GPU-Upload werden bestehende Ressourcen und ersetzte LOD-Varianten eingerechnet. Bei Überschreitung wird ein Mesh-Upload zurückgestellt; Texturen können zuerst niedrig priorisierte Texturen verdrängen. Bei freiwerdendem Speicher werden zurückgestellte Anfragen wieder berücksichtigt. Dies ist ausdrücklich keine vollständige VRAM-Bilanz: Terrain, UI, Treiber-Overhead und weitere Ressourcen sind nicht enthalten.

**Block D – Streaming-Diagnose und Regressionen:** 60-s-Metriken für RAM-Treffer, RAM-Größe/Einträge, GPU-Mesh-/Texturbytes, gemeinsames geschätztes Budget und Deferred Counts. Durchschnittliche Zeit vom Asset-Worker-Start bis zur Verarbeitung der fertigen Future (keine reine Netzwerklatenz). Neue Tests für Kamera-Priorität, sicheren gemeinsamen Budget-Rechenweg sowie den parallelen Textur-RAM-LRU-Cache.

**Verifikation:** C++23-Debug-Build und 38/38 automatisierte CTests erfolgreich. OpenGL-3.3-Welcome-Starttest nach Versionspflege ausführen. Kein bestätigter In-World-Test des NexVerse-Asset-Streamings.

**Bewusst offen:** echte Frustum-/Occlusion-Abfrage, globale inklusive Terrain/UI gemessene VRAM-Budgetierung, inkrementeller J2C-Netzstream, Avatar-Skeleton/Bakes, Bewegungs-Synchronisierung und vollständige Teleport-/Circuit-Behandlung. Bekannte Mesh-/Texture-Serverfehler bleiben in der späteren Fehlerbehebungsphase.

**Nächste 3–4 Blöcke:** Asset-Streaming-Queue/Prioritätsregeln, planbares Prefetching mit Limits, Cache-/Regions-Invalidierung und zusätzliche Mess- und In-World-Integrationsdiagnostik.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl62-live.log`. Verbindlicher GitHub-Handoff-Branch: `develop-current`.

Neuer Chat: zuerst GitHub-Branch `develop-current` und `CHATUEBERGABE.md`, danach `projectplan.md` und `info.md` lesen. Je Großrelease 3–4 zusammenhängende Blöcke, vor Commit Build, 38 CTests, Linux-GUI-Smoke, dann Versionsdatei, Projektplan, Übergabe und technische Dokumentation mitsamt Code zu GitHub hochladen. `develop` nicht automatisch zusammenführen. Der letzte Release-Commit ist in `git log` zu überprüfen.


## 0.2.0-alpha.63 – GitHub-Übergabe · 11.10.2026

**Block A – GetTexture-/GetMesh-Anfrage-Taktung:** `AssetRequestPacer` taktet neu gestartete Textur-Demand-Worker auf mindestens 125 ms und Mesh-Demand-Worker auf mindestens 180 ms Abstand. Die bereits vorhandene Kamera-/Distanz- und Partikelpriorität bleibt maßgeblich; höchstens je ein aktiver Demand-Worker pro Assettyp und keine künstliche Blockade bereits laufender Requests. Pacer werden bei Regionswechsel zurückgesetzt.

**Block B – begrenztes Texture-Prefetching:** Der Viewer wärmt vor dem eigentlichen GPU-Upload vollständige RGBA8-Bilder aus dem persistenten Texture-Diskcache in den 96-MiB-RAM-LRU vor. Die Auswahl berücksichtigt die bereits priorisierte GPU-Residency-Reihenfolge, ignoriert GPU-residente und bekannte fehlgeschlagene UUIDs und untersucht maximal vier Kandidaten pro Durchgang. Es läuft höchstens ein Prefetch-Worker gleichzeitig, mit mindestens 650 ms Abstand und maximal 48 Prefetch-Kandidaten pro Region. Es finden dabei ausdrücklich keine zusätzlichen HTTP-Anfragen oder GPU-Uploads statt.

**Block C – Regions-Epochen und Cache-Admission:** `TexturePrefetchScope` setzt geprüfte Kandidaten bei neuem `material_scope.epoch()` zurück. Die aktuell gültige Regionsepoche wird atomar an den Prefetch-Worker weitergegeben; ein beim Abschluss veralteter Disk-Read wird vor der Cache-Aufnahme zurückgewiesen. Bestehende GetTexture-/GetMesh-Epochenschutzprüfungen im GPU-Pfad bleiben unangetastet. Diskcache und RAMcache sind UUID-basiert, CAPS-Tokens werden nicht gespeichert. Prefetch-Worker lassen sich nicht während laufender Dateizugriffe zwangsweise abbrechen.

**Block D – Telemetrie und Regressionstests:** Das 60-Sekunden-Streaminglog meldet Demand-Starts je Assettyp sowie Prefetch-Versuche, erfolgreiche RAM-Erwärmungen, Diskcache-Misses und stale Prefetch-Resultate. Neue deterministische Tests für Zeitintervalle, Kandidaten-Limits und Epochensprünge; Integrationstest des echten Disk→RAM-Prefetch-Pfads einschließlich manipuliertem Cachefile. `TextureMemoryCache::contains()` ermöglicht den Kandidatenabgleich ohne teure Bildkopie.

**Verifikation:** C++23-Debug-Build, 40/40 CTests einschließlich neuer Scheduler- und Disk-Prefetch-Tests. Linux OpenGL-3.3-GUI-Starttest nach Versionspflege. Authentifizierter NexVerse-In-World-Streaming-Test ist noch ausstehend.

**Grenzen:** Kein HTTP-Prefetch, kein eigenständiger parallel ladender Mesh-Prefetcher, kein inkrementeller J2C-Assetstream. 384-MiB-Budget bleibt eine Schätzung für Mesh-/Objekttextur-GPU-Ressourcen ohne Terrain/UI und Treiber-Overhead. Unvollständiger Avatar, Bewegung, Teleport und einzelne Mesh-/Texturserverfehler bleiben entsprechend Projektplan für die spätere Bugfixphase dokumentiert.

**Nächste 3–4 Blöcke:** Recovery und Fehlerklassifikation des Asset-Loaders, temporäre Retry-/Backoff-Logik statt permanenter Fail-Sets, robustere Regionsepochen-Aufräumlogik und In-World-Assetmetriken. `projectplan.md` bleibt bindend.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl63-live.log`; Branch `develop-current`.

Verbindlicher Wiedereinstieg im neuen Chat: `develop-current`-Stand und diese Datei lesen, danach `projectplan.md` und `info.md`. Vier gebündelte Entwicklungsblöcke und nach jedem Release Build, alle Tests, GUI-Smoke, Versions- und technische Dokumentationsupdates, gemeinsamer Push nach GitHub. `develop` hat weiterhin eine ältere divergierende Historie und darf nicht automatisch überschrieben werden.


## 0.2.0-alpha.64 – Projektübergabe · 11.10.2026

**Block A – strukturierte Asset-Fehlerklassifikation:** Der GetTexture- und GetMesh-HTTP-Pfad liefert zusätzlich zum Diagnosetext `AssetFailure`: `transient` (Timeout/Transport, 408/425/429/5xx), `missing` (404/410), `invalid` (fehlerhafter Decoder/unerwartete HTTP-4xx) oder `configuration` (400/401/403, fehlender Transport/ungültige CAPS-URI). Bei PNG→J2C beziehungsweise GetMesh2→GetMesh-Fallback bestimmt die kombinierte Fehlerklasse die Wiederholungsstrategie; ein erfolgreicher Fallback gilt als Erfolg.

**Block B – zeitlich begrenzte Retry-/Backoff-Logik:** `AssetRetrySchedule` ersetzt die bisher bis zum Regionswechsel dauerhaft gesperrten Fehler-UUIDs. Temporäre Fehler: nach 2, 4 und 8 Sekunden erneute Versuche, danach fünf Minuten Sperrzeit; HTTP 404/410: zehn Minuten; ungültige Assets: 30 Minuten; Konfigurationsfehler: fünf Minuten. Erfolgreiche GPU-Uploads löschen den zugehörigen Fehlerstatus. Mesh-Fehlerstatus unterscheidet UUID und angeforderten LOD; GPU-Bytebudget-Deferrals bleiben weiterhin getrennt. Die Map ist auf 4096 Einträge begrenzt; bei darüber hinausgehender Kartenfluktuation wird sie geleert.

**Block C – regiongebundene Fehlerverwaltung:** Bei `material_scope.epoch()`-Wechsel werden Mesh- und Textur-Retrytabellen geleert. Bereits laufende Downloads werden nicht auf dem GUI-Thread blockiert oder abgebrochen. Fertige Antworten aus alten Epochen bleiben durch die bestehenden Epochenschutzprüfungen vom GPU-Upload ausgeschlossen; zusätzliche Stale-Counter dokumentieren diese Verwerfungen. Cache-Dateiformate und Mesh-/Texture-Cachebudgets bleiben unverändert.

**Block D – Diagnose und Regressionstests:** Streaminglogs führen `failure_events`, `retry_dispatches`, `backoff_assets`, `transient_failures` und `stale_results` je Mesh-/Textur-Familie. Tests prüfen Klassifikation von HTTP-Antworten, Rechenfolge des Backoffs, Regionsepochen, Zurücksetzen bei Erfolg und beide echten Loader-Fallback-Pfade (404 und 503) mittels Mock-HTTP-Transport.

**Verifikation:** Vollständiger C++23-Debug-Build und 41/41 CTests sind erfolgreich. Nach Versionspflege erfolgen erneuter vollständiger Rebuild und Linux OpenGL-3.3-Welcome-Starttest. Ein authentifizierter NexVerse-In-World-Streaming-Test ist weiterhin nicht ausgeführt.

**Grenzen und offene Arbeiten:** Diese Verbesserung beseitigt nicht die eigentlichen Fehler einzelner Server-Assets. Sie verhindert stattdessen endlose schnelle Retries beziehungsweise permanente lokale Sperren. Tatsächliche Simulator-Synchronisierung, Avatar-LocalID/Appearance, Bewegungsanzeige sowie Regions-Teleport bleiben wie vereinbart für spätere Fehlerblöcke offen. Ein genereller Netzwerkanfrage-Abbruch mittels Cancellation-Token ist nicht implementiert.

**Nächster Release (3–4 Blöcke):** In-World-Integrationsdiagnostik und Echtzeit-Aussagekraft, bessere Unterscheidung von Cache-/Serverfehlern, mögliche asynchrone Netzwerk-Cancellation und Asset-Streaming-Queue-Optimierung; die verbindliche Roadmap bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl64-live.log`; maßgeblicher Branch ist `develop-current`.

Neuer Chat: zuerst GitHub `develop-current` und diese Datei, danach `projectplan.md` und `info.md` lesen. 3–4 koordinierte Blöcke je Release, `cmake --preset debug`, `cmake --build --preset debug -j2`, `ctest --preset debug --output-on-failure`, GUI-Smoke unter DISPLAY=:0; dann Version, Projektplan, Übergabe, technische Docs aktualisieren und Code+Docs auf `develop-current` pushen. `develop` nicht automatisch mergen oder zurücksetzen.


## 0.2.0-alpha.65 – Handoff und nächste Blöcke · 11.10.2026

**Block A – Cache-Miss- und Dateifehlerdiagnose:** `AssetCacheLookup` und die optionale Statusausgabe von `TextureDiskCache::load()` unterscheiden `hit`, `miss`, `corrupt`, `io_error`. Der bereits vorhandene Disk-Cache-Header, die CRC-artige FNV64-Inhaltsprüfung, die 256-MiB-Ressourcengrenze und UUID-Validierung bleiben kompatibel. Texturworker berichten Disk-Misses, beschädigte Einträge und Dateisystemfehler getrennt von HTTP-Versuchen. Diese Quellenklassifikation verändert keine Serverantworten.

**Block B – koordinierte Regions-Epochen im Asset-Worker:** Der bestehende atomare Live-Epochenmarker aus dem Textur-Prefetch wird auch von Demand-GetTexture und GetMesh verwendet. Worker prüfen die Epoche vor Beginn und unmittelbar vor einem bislang nicht gestarteten HTTP-Download. Nach einem bereits laufenden HTTP-Download prüfen sie erneut vor dem Schreiben in den RAM-/Disk-Cache. Die `load_mesh_cached`-Funktion bietet dafür einen optionalen Admission-Callback. Die GPU-Ladepfade verwerfen nach wie vor veraltete Epochen. Bereits laufendes libcurl-HTTP kann noch nicht per Cancellation-Token unterbrochen werden.

**Block C – In-World-Health-Telemetrie:** Nach Aufbau der Simulation schreibt das Rendering alle etwa 30 Sekunden eine Kennzahlenzeile `InWorld asset health` (Regionsepoche, Anzahl der Szenen-Samples, gewünschte vs. bereits GPU-residente UUIDs und Deckungsgrad in Prozent, fehlende Assets, laufende Texture-/Mesh-Worker, Retry-Einträge und geschätzte GPU-Mesh-/Objekttexturbytes). Ohne aktive Simulation erfolgt keine falsche In-World-Bestandsmeldung. Die Coverage-Metrik bedeutet nur UUID-Residency, nicht vollständige grafische Darstellung, Avatar- oder Netzwerk-Korrektheit.

**Block D – Regressionen und Detailzähler:** Der Diskcache-Test prüft saubere Treffer, Miss und beschädigte Datei. Der Meshcache-Test prüft zwei Abbruchsituationen, einmal vor HTTP und einmal nach Abschluss einer künstlich veralteten Downloadfunktion; in beiden Fällen gelangt nichts Falsches in RAM/Disk. Der neue Test `asset_scene_coverage_tests` prüft leere Szenen, deduplizierte UUIDs, teilweise und vollständige Abdeckung. Die GetTexture-/GetMesh-Streaminglogs nennen zusätzlich `skipped_before_http`, `stale_after_http`, `disk_corrupt` und `http_attempts`.

**Teststand:** C++23-Debug-Build und 42/42 CTests nach Implementierung erfolgreich. Für die finale Version erfolgt nochmals vollständiger Rebuild und Linux-OpenGL-3.3-Welcome-Smoke. Es gibt weiterhin keinen authentifizierten NexVerse-In-World-Livetest; die In-World-Kennzahlen sind instrumentiert, aber noch nicht an realen Simulatordaten verifiziert.

**Unverändert offen:** Eigentliche serverseitig fehlende/beschädigte Mesh- und Texturassets, Avatar-/WASD-/Teleport-Funktionsdefizite und die vollständige visuelle Weltabnahme. Die Fehler werden protokolliert, aber nicht fälschlich als bereits behoben markiert. Keine echte libcurl-Abbruch-API in diesem Release.

**Nächste 3–4 Blöcke:** Realistisches Asset-Recovery-Verhalten unter Last, Retry-Schleifen und Prioritäts-Stabilisierung, strukturierte Diagnostics-/Health-Snapshots statt nur Textlogs und Renderer-/Szenenabnahme. `projectplan.md` bleibt verbindlich.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl65-live.log`. Branch: `develop-current`.

Neuer Chat: `develop-current`-Branch auf GitHub und diese Übergabe zuerst prüfen; danach `projectplan.md`, `info.md` und technische Docs. 3–4 gebündelte Blöcke, alle Debug-Tests, OpenGL-GUI-Smoke, Code/Tests/Docs zusammen pushen. Keine ungesicherten lokalen Änderungen oder automatisches Umschalten auf den divergierenden Branch `develop`.


## 0.2.0-alpha.66 – Projektübergabe · 11.10.2026

**Block A – stabile Asset-Auswahl:** `AssetRequestFairness` integriert die bereits vorhandene Kamera-/Entfernungs- und Partikel-Priorisierung mit der pro Asset/LOD geführten Retry-Tabelle. Neue, berechtigte Assets werden innerhalb ihrer bisherigen Reihenfolge bevorzugt; bei einem bereiten Retry wird jeder vierte tatsächliche Dispatch für eine Wiederholungsanfrage reserviert. Bei ausschließlich bereiten Retries werden diese unmittelbar berücksichtigt. Ungültige UUID-Leereinträge, vollständig geladene Assets, Budget-Deferrals und noch aktive Cooldowns bleiben ausgeschlossen. Per-Region-Zähler werden beim Epochensprung zurückgesetzt. Das ist eine kleine Fairness-Schicht vor den bestehenden unabhängigen Textur-/Mesh-Demand-Workern, keine parallele Netzwerk-Multiplexer-Queue.

**Block B – stabile Retry-Zustände unter Last:** `AssetRetrySchedule` setzt bei mehr als 4096 Einträgen nicht mehr die gesamte Retry-Tabelle zurück, sondern verdrängt nur den Eintrag mit dem frühesten Retry-Zeitpunkt. Neue `blocked_count(now)`- und `ready_retry_count(now)`-Metriken trennen noch gesperrte Assets von wieder zulässigen Wiederholungsversuchen. Die bestehenden kurzen 2-/4-/8-Sekunden-Transient-Retries und längeren Cooldowns aus alpha.64 bleiben erhalten. Alte Fehlerzustände werden weiterhin bei einem Regionsepochen-Wechsel invalidiert.

**Block C – strukturierte In-World-Streaming-Snapshots:** Zu den vorhandenen 30-Sekunden-Health-Logs kommt `InWorldAssetSnapshot` mit maschinenlesbarem, numerischem JSON-Schema `ogl.asset.streaming.v1`. Felder: Regionsepoche, Szenensamples, gewünschte/geladene/offene Textur-/Mesh-UUIDs samt Abdeckungsprozent, aktuell laufende Asset-Worker, noch gesperrte und retryfähige Fehler, RAM-Cachebytes und die geschätzte GPU-Mesh-/Objekttexturbelegung inklusive gemeinsamem Softwarebudget. Ohne `simulation_ready` wird kein In-World-Snapshot behauptet; ein leeres Desired-Set ergibt rechnerisch 100 % Coverage und ist kein Nachweis einer vollständigen Welt.

**Block D – Streaming-Integrationstests:** Drei neue CTests: Fairness/Reihenfolge/Duplikate/Deferred-Assets/LOD-Keys, JSON-Schema inklusive leerer Region sowie die gemeinsame Planung aus Texture-Residency, Retry, Byte-Budget, Region-Reset und Health-Snapshot. Zudem ein Stress-Test mit 4098 Failure-Einträgen und den getrennten blocked-/ready-Kennzahlen. C++23-Debug-Build und 45/45 CTests bestanden; vollständiger Release-Rebuild und GUI-Starttest nach Versionserhöhung folgen.

**Wichtige Abgrenzung:** Die neue Integrationstest-Suite prüft die Asset-Planungs- und Budgetkomponenten, nicht einen authentifizierten In-World-Renderer- oder OpenSim-Server-End-to-End-Test. Die bereits bekannten Defizite (Avatar-Körper und Bewegung, fehlende Meshobjekte, fehlerhafte Assetantworten, Teleport und EventQueue) bleiben im Projektplan dokumentiert und werden wie vereinbart in späteren Fehlerbehebungsphasen behandelt. Die GPU-Bytes bleiben geschätzte Mesh-/Objekttexturwerte ohne Terrain-, UI- und Treiber-Overhead.

**Nächster 3–4-Blöcke-Release:** Datengetriebene Auswertung realer Simulatorlogs und Regionen, Entzerrung von Textur-/Mesh-LOD-Swaps, Renderer-GPU-Upload- und Slot-Backpressure sowie reproduzierbare In-World-Testpfade. `projectplan.md` bleibt die maßgebliche Roadmap.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl66-live.log`. Alle Änderungen auf `develop-current`; `develop` nicht automatisch verwenden.

Wiedereinstieg: Repository `marlin198323-glitch/nexverse-viewer`, einzig maßgebliche Branch `develop-current`, lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`; zuerst diese Übergabe, danach `projectplan.md`, `info.md`, einschlägige Docs und `git status` lesen. Pro Release drei bis vier größere Blöcke, anschließend Build, alle CTests, DISPLAY=:0 GUI-Smoke, Release-Version und sämtliche Markdown-Dokumente, Git-Commit und HTTPS-GitHub-Push. Nicht automatisch auf `develop` wechseln oder bestehende Geschichte zurücksetzen.


## 0.2.0-alpha.67 – GitHub-Übergabe · 11.10.2026

**Block A – tatsächliches Viewerlog analysiert:** `tools/ogl_asset_log_report.py` (Python 3, Standardbibliothek) fasst das reale `$HOME/.local/state/opengenesislink/viewer.log` sowie künftige `InWorldAssetSnapshot`-Zeilen in einem numerischen, UUID-/CAP-URL-freien JSON-Bericht zusammen. Der historische Stand vom 11.10.2026 enthielt 230 Mesh- und 56 Texture-Fehlermeldungen, davon 42 mit 404 und eine mit 5xx-Muster; 243 weitere sind historische unklassifizierte Meldungen. Validierte In-World-Snapshots waren in dieser Datei noch nicht vorhanden. Dokumentierte Bedienung: `docs/LOG_ANALYSIS.md`.

**Block B – LOD-Dwell gegen Flackern:** Die neue generische `AssetLodDwell<Quality>` verlangt für wechselnde GPU-residente Mesh-LODs und Textur-Basismips eine über 1400 ms stabile Zielstufe. Neue noch nicht GPU-residente Assets werden sofort bedient; aktive Partikeltexturen müssen weiterhin mit höchster Bildauflösung angefordert werden. Die Anti-Oszillationszustände werden bei Regionswechsel geleert und von verschwundenen Scene-IDs bereinigt. Bereits bestehende Distanz-/Größen-LOD-Hysteresen bleiben wirksam.

**Block C – Renderthread-Upload-Backpressure und Slotfreigabe:** Der gemeinsame `AssetGpuUploadGate` hält mindestens 30 ms Abstand zwischen erfolgreichen Objekttextur- und Mesh-GPU-Uploads. Fertige Worker-Futures bleiben bis zum nächsten Upload-Fenster ausstehend; es gehen keine bereits dekodierten Daten verloren. `plan_texture_slot_admission` prüft zusätzlich zur bytebasierten Planung das 512-Slot-Limit: Ist das Limit erreicht, darf ein eingehender, höher priorisierter Textur-UUID einen niedrig priorisierten GPU-Slot ersetzen. Die Budgetrechnung berücksichtigt bereits geplante Byte-Evictions und verhindert die Verdrängung höher priorisierter Assets. Terrain-/UI-GPU-Uploads gehören nicht zur neuen Sperre.

**Block D – Regressionen und Telemetrie:** Zwei neue C++ CTests für LOD-Dwell/Regionsreset und GPU-Upload-Gate/Slot-Zuteilung; ein Python-CTest prüft den Parser mit Legacylogs, gültigem JSON, defektem Snapshot und Sanitizing unbekannter Felder. Die regulären Textur-Streaminglogs führen nun `mip_dwell_pending`, `slot_priority_evictions`, `gpu_uploads`, `gpu_uploaded_bytes`; die Mesh-Streaminglogs führen `lod_dwell_pending`. Ein vollständiger C++23-Debug-Build und 48/48 CTests waren erfolgreich; nach der Versionspflege werden sie erneut und zusätzlich der Linux-GUI-Smoke geprüft.

**Grenzen:** Die aus echten Logs gewonnenen Zahlen beschreiben historische Verbindungs-/Asset-Fehler und keine bestätigte Beseitigung der 404/Decoderprobleme. Neuer Renderthread-Takt reduziert zeitliche Upload-Spitzen, garantiert aber keine fixe Framezeit. Weiterhin ausstehend sind authentifizierte In-World-Abnahme, vollständige Mesh-Objekte, Avatar-Body/Bewegung, Teleport und EventQueue-Fehlerbehebung.

**Nächster Release (3–4 Blöcke):** Datenbasierte Prüfung anhand neuer In-World-Logs und GPU-Diagnose, weitere Upload-Messungen, scene-priority-Feedback und gezielte Render-Integration. `projectplan.md` ist weiter maßgebend; bei ausdrücklicher Nutzerpräferenz Fehlerbehebung separat.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl67-live.log`. GitHub-Branch `develop-current`.

Neuer Chat: zuerst den GitHub-Branch `develop-current` und `CHATUEBERGABE.md` prüfen, anschließend `projectplan.md`, `info.md` und `docs/LOG_ANALYSIS.md`. Immer 3–4 größere Entwicklungsblöcke, danach CMake-Debug, alle CTests, GUI-Smoke, Versionspflege, komplette Docs und Code auf `develop-current` pushen. Keine automatischen Merges aus `develop`. Authentifizierter Simulator-Test ist weiterhin offen.


## 0.2.0-alpha.68 – Übergabe und Fortschritt · 11.10.2026

**Block A – reale CPU-Uploadzeit statt Worker-Wartezeit:** `AssetUploadTelemetry` misst die Wandzeit, die das Render-Thread-Aufrufen von `set_object_texture(...)` und `set_mesh_asset(...)` benötigt. Das feste Ringfenster hält die letzten 64 Uploadversuche und bietet `recent_avg_us`, `recent_p95_us`, `recent_max_us`, `recent_count`; Nullzustand und Überlauf werden konservativ behandelt. Messgröße ist **CPU-seitige OpenGL-Übermittlung**, keine GPU-Fertigstellungszeit, kein `glFinish` oder OpenGL-Timer-Query.

**Block B – adaptives GPU-Upload-Backpressure:** `AssetGpuUploadGate` bleibt bei schnellen Uploads bei mindestens 30 ms Abstand; langsame CPU-Übermittlungen verlängern das nächste Uploadfenster auf etwa 4× beobachtete Millisekunden, bis maximal 150 ms. Der nächstfolgende kurze Upload senkt das Intervall wieder auf den Grundwert. Mesh und Textur teilen sich den Renderthread-Gate. Bereits fertiggeladene Futures bleiben für spätere Frames verfügbar. Terrain/UI und vorbestehende Renderer-Arbeiten unterliegen dem Gate nicht.

**Block C – Szenenpriorisierung und erweiterte Health-Snapshots:** `AssetScenePriorityFeedback` beobachtet alle etwa 750 ms die GPU-gewünschten UUID-Sets, erkennt erhebliche Wechsel (symmetrische Differenz mindestens ein Viertel der vorherigen/neuen Set-Größe) und erhöht für drei stabile Planzyklen den weichen Loaded-Bonus temporär: Mesh von 24 auf 42 gewichtete Meter, Textur von 8 auf 14. Neue näher gelegene Assets bleiben weiterhin auswählbar; keine UUID wird fest gepinnt. Regionsepochen löschen den Zustand. `InWorldAssetSnapshot` behält `ogl.asset.streaming.v1`, ergänzt rein numerische Felder zu CPU-Uploadavg/p95/max, Sampleanzahl, effektiver Uploadsperre und Texture-/Mesh-Prioritäts-Churn. Der vorhandene Log-Analyzer wertet zulässige Felder aus und berechnet Peak-p95 bzw. Peak-Churn; er gibt keine CAPS-URLs/UUIDs aus.

**Block D – echter Renderer-Integrationstest:** `renderer_gpu_asset_integration_tests.cpp` erzeugt einen unsichtbaren echten GLFW/OpenGL-3.3-Kontext, lädt/repliziert/entfernt synthetische Textur und Mesh auf dem GPU-Pfad, prüft ungültige Meshdaten, führt einen Renderer-Frame aus, kontrolliert `visible_objects`, Renderer-Backend und GL-Fehlerfreiheit. Ohne OpenGL-Display überspringt CTest den Test via Exitcode 77; auf dem autorisierten Linux-PC wurde der Test unter `DISPLAY=:0` vollständig ausgeführt. Zwei weitere C++-Tests prüfen Rolling-Telemetrie und Scene-Prioritätsrückkopplung; erweiterte JSON-/Log-/Upload-Gate-Regressionstests. Gesamt: **51/51 CTests bestanden**, inkl. realem OpenGL-Test, anschließend vollständiger Versions-Rebuild/GUI-Smoke.

**Grenzen:** Der Renderer-Integrationstest arbeitet mit synthetischer Geometrie und einem lokalen GPU-Kontext, ohne authentifizierte NexVerse-Region oder echtes Asset. Er belegt noch nicht, dass Avatar-Body/Bewegung, Mesh-Objekte aus dem Simulator, CAPS-EventQueue, Teleport oder alle Weltassets korrekt sind. Diese bekannten Punkte bleiben bis zur vereinbarten späteren Bugfixphase dokumentiert. CPU-Uploadzeiten dürfen nicht als verifizierte GPU-Latenz bezeichnet werden.

**Nächster 3–4-Blöcke-Release:** Wiederholbare Smoke-/GPU-Profiling-Sessions anhand synthetischer Szenen unterschiedlicher Größe, Renderer-Upload-Fehlertransparenz, Scene-Priority-Schutz unter voller Asset-Residency und Ausbau der testbaren Welt-/Mesh-Renderpfade. Maßgeblich bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl68-live.log`. CLI-Logauswertung: `python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"`; Source Branch `develop-current`.

Wiedereinstieg: GitHub `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`, lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, diese Datei, verbindliches `projectplan.md`, `info.md`, technische Docs lesen. Vier zusammenhängende Blöcke, `cmake --preset debug`, `cmake --build --preset debug -j2`, `env DISPLAY=:0 ctest --preset debug --output-on-failure`, Linux GUI-Smoke und Push nach GitHub. Der neue reale Render-GPU-CTest braucht einen gültigen GL3.3-Kontext und zeigt andernfalls einen expliziten Skip; auf dem Linux-PC ist `DISPLAY=:0` der bekannte aktive X11-Displayserver.


## 0.2.0-alpha.69 – Projektübergabe und nächste Schritte · 11.10.2026

**Block A – echter synthetischer GPU-Kapazitäts-/Belastungstest:** `tests/renderer_gpu_pressure_tests.cpp` erzeugt im versteckten OpenGL-3.3-Kontext auf dem Linux-Testrechner tatsächlich 512 kleine GPU-Objekttexturen und 256 kleine GPU-Meshes (Renderer-Grenzen). Bei 16, 128, 256 und 512 Texturen wird ein Frame mit den aktuell angelegten Szenenobjekten gezeichnet; Tests prüfen `visible_objects`, `draw_calls`, OpenGL-Fehlerfreiheit, abgewiesene 513. Textur/257. Mesh, korrekte Ersetzung bestehender Ressourcen sowie erneute Aufnahme nach Slot-Freigabe. Beispiel: unter dieser synthetischen Auslastung ca. 35 ms CPU-Gesamttestzeit inklusive Context-Init/Uploads/Draws. Diese Zeit ist ausdrücklich KEIN Benchmark realistischer größerer OpenSim-Assets.

**Block B – verlässliche GetTexture-GPU-Upload-Rückmeldung:** Die Renderer-Schnittstelle `Renderer::set_object_texture` meldet nun `bool` statt `void`. `LegacyOpenGlRenderer` baut Texturen immer erst in einem neuen GL-Namen auf, prüft Dimensionen und GL-Fehler, übernimmt sie nur bei erfolgreichem Upload und löscht den vorherigen Textur-Namen erst danach (transaktionale Ersetzung). Bei fehlerhaften Daten, vollem internen GPU-Slotlimit oder GL-Fehlern kehrt sie `false` zurück, ohne eine vorhandene Textur derselben UUID zu zerstören. Die Anwendung zählt `texture_gpu_upload_failures`, gibt eine Warnung aus, plant einen transienten Retry und trägt fehlgeschlagene Uploads nicht mehr als GPU-resident ein. Überlegungen zur atomaren Freigabe *anderer* prioritätsbedingt zuvor verdrängter Texturen bleiben ausdrücklich offen.

**Block C – robusterer GetMesh-GPU-Pfad:** Schon vor GPU-Submission prüft der Renderer Meshvertex-Stride, Dreiecks-Indexvielfache, jeden Index gegen die tatsächlich vorhandenen Vertices und angegebene Face-Bereiche gegen den Indexbuffer. Der eigentliche GL-Mesh-Upload kontrolliert Fehler und gültige VAO/VBO/EBO-Handles und gibt teilweise angelegte GL-Ressourcen bei Fehlern frei. Vorheriger LOD-Buffer bleibt bei gescheiterter Ersetzung erhalten. Viewer meldet `mesh_gpu_upload_failures`, klassifiziert GPU-Uploadfehler als transient und plant einen Backoff statt eines permanenten `invalid`-Fehlerstatus.

**Block D – Vollauslastung und Logdiagnose:** Der neue CTest `asset_full_residency_tests.cpp` übt 80 neue vorrangige Partikeltexturen bei 512 belegten Slots und eine Kamera-Fahrt über 400 Mesh-UUIDs bei 256 GPU-Slots aus. Er kontrolliert Priorität, Freigabe, Slotlimits und das Verbleiben naher/nützlicher Assets. `InWorldAssetSnapshot` führt zusätzliche Zähler `texture_gpu_upload_failures` und `mesh_gpu_upload_failures` im vorhandenen numerischen Schema `ogl.asset.streaming.v1`; der Python-Loganalysator nimmt sie in die sichere Feldauswahl auf und zeigt die jeweiligen Maximalwerte. JSON-, GL-Renderer- und Log-Regressionstests angepasst. C++23-Debug-Build und **53/53 CTests** inklusive zweier realer OpenGL-Kontexttests erfolgreich; vollständiger Release-Rebuild, Versionstest und GUI-Start folgen.

**Unverändert offene Grenzen:** Ein OpenGL-Test mit kleinen künstlichen Assets validiert Ressourcenhandling und GPU-Slots, aber weder echtes GetMesh/OpenSim-Decoding noch Live-Weltgeometrie, Avatar, Netzwerk-EventQueue, Teleport und In-World-Interaktion. Tests laufen auf echtem GL3.3/NVIDIA-Linux mit `DISPLAY=:0`; ohne GL-Display überspringen sich die beiden GPU-Kontext-CTests per Returncode 77. Shared GPU-Budget von 384 MiB ist eine Software-Schätzung ohne Terrain/UI/Treiber-Speicher. Die atomare Textur-Ersetzung gilt nur für denselben UUID-Slot, nicht für bereits vorab verdrängte andere Slots.

**Nächster Release / verbindlicher Plan:** Schrittweise GPU-Upload-Rollback/Recovery bei Slot-Evictions, realistischere Szenengeometrien/Texture-Mip-Lastprofile, strukturierte Upload-Failure-Gründe und weitere Renderer-Timing-Integration. Avatar/Bewegung/Mesh-/Teleport-/CAPS-Bugfixes wie vom Nutzer vereinbart separat.

Run: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Pressure einzeln: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-pressure-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl69-live.log`; nur `develop-current` nutzen.

Wiedereinstieg nach Chatwechsel: `marlin198323-glitch/nexverse-viewer`/`develop-current`, lokal `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, `CHATUEBERGABE.md`, `projectplan.md` und `info.md` prüfen. Pro Release 3–4 große koordinierte Blöcke; Source/Tests/technische Docs aktualisieren, Debug-Preset bauen, unter DISPLAY=:0 ALLE CTests inkl. echter GL-Kontexte, Viewer-GUI-Smoke, GitHub-Branch verify & push. Divergierenden Branch `develop` nicht verwenden. Authentifizierter Live-OpenSim-Welt-Test bleibt offen.


## 0.2.0-alpha.70 – Handoff und nächste Entwicklungsblöcke · 11.10.2026

**Block A – texturschonender, transaktionaler GPU-Austausch:** `Renderer::replace_object_texture(uuid, image, evictions)` übernimmt ein komplett neues OpenGL-Textur-Handle erst nach erfolgreichem `glTexImage2D` und `glGenerateMipmap`. Erst danach werden die vorab ausgewählten Eviction-UUIDs gelöscht und die neue Textur eingebunden. Bei Fehlschlag bleiben alle bisherigen UUID-Slots, inklusive der Eviction-Kandidaten, erhalten. Der Viewer spiegelt die Freigaben erst nach erfolgreichem GL-Commit in `object_texture_loaded_ids`, `texture_gpu_bytes`, Mip-Tabellen und GPU-Bytezähler; abgelehnte Uploads zählen nicht als geladene Ressource. Der bisherige bool-Aufruf `set_object_texture` bleibt als Wrapper erhalten.

**Block B – strukturierte GPU-Uploadergebnisse für Texture UND Mesh:** `GpuUploadResult`/`GpuUploadFailure` enthält `success`, `failure`, `gl_error` mit Klassen `none`, `invalid_input`, `capacity`, `unsupported_dimensions`, `allocation`, `driver_error`. Beide Rendererpfade bieten strukturierte Methoden; der bisherige bool-Mesh-Aufruf ist ein Wrapper. Bei Texturen werden GL-Größenlimits sowie GL-Uploadfehler gemeldet, beim Mesh getrennt fehlerhafte Index-/Vertex-/Face-Daten, belegte 256 Mesh-Slots und sonstige Driver-Fehler. Application-Logs nennen nun `gpu_failure` (und bei Texturen `gl_error`), invalid/unsupported nutzt längeren invalid-Cooldown, transient/kapazitätsbedingt kurzzeitige Retries. Eine verlässliche OpenGL-Mesh-Fehlernummer wird im Renderer bei GL-Fehlern noch nicht als separates numerisches Feld exportiert und ist als Folgeschritt dokumentiert.

**Block C – reproduzierbare GPU-Upload-Lastprofile mit großen Assets:** Neuer CTest `renderer_gpu_profiles_tests.cpp` verwendet unsichtbaren ECHTEN OpenGL-3.3-Context und prüft 40 × 64², 20 × 256² und 6 × 1024² RGBA-Texturen sowie Mesh-Vertexgrids 16/32/64 (8/8/4 Instanzen, bis zu 8192 Dreiecke je Grid). Für jede Stufe werden GPU-Uploads, Renderframe, sichtbare Objekte und GL-Fehler geprüft; CPU-Upload-Zeiten (nicht GPU-Fertigstellungszeit) pro Gruppe protokolliert. Beispiel Linux RTX 4070 SUPER am 11.10.2026: Texture 283/369/4604 µs und Mesh 17/61/157 µs je Upload. Diese Zahlen gelten nur für synthetische Assets/diesen Testlauf, nicht für typische Serverdownloads oder FPS.

**Block D – Rollback-/Überbelegungs-Regressionen:** `renderer_gpu_pressure_tests.cpp` prüft weiterhin tatsächliche Belegung 512 Texturen/256 Meshes, jetzt zusätzlich Transaktions-Rollback bei ungültigem Input, fehlender Slotkapazität, unerlaubter Selbsteviction sowie übergroßen Texturen; erfolgreiches Commit entfernt exakt den vorgesehenen GPU-Victim und macht die neue ID resident. Vorhandene Mesh-Überbelegungs- und ungültige-Index-Tests prüfen die passende strukturierte Fehlerklasse. Insgesamt nach Implementierung **54/54 CTests bestanden**, inklusive drei echter OpenGL-Context-Tests; nach finaler Versionspflege erneut Full-Rebuild und GUI-Smoke.

**Wichtige technische Grenze:** Der Transaktionspfad hält den alten GPU-Textur-Slot und die neue Kandidatenressource kurzfristig gleichzeitig im VRAM. Bei sehr knappem physischem VRAM kann die sichere Transaktion scheitern und die alten Texturen behalten – ergebnisabhängig beabsichtigtes Schutzverhalten, aber später mit zusätzlicher Budgetreserve/Backpressure zu optimieren. Netzwerk-CAPS, echte Meshasset-/Avatar-/Bewegungs-/Teleportfehler wurden wie vereinbart nicht als bereits behoben deklariert. Der OpenGL-Integrationstest ist synthetisch, nicht authentifiziert in einer NexVerse-Region.

**Nächste vier Blöcke:** Strukturiertes Mesh-GL-Fehlerdetail, VRAM-Transientbudget/Reserve bei Rollback-Uploads, Instrumentierung echter Weltobjekt-Draws und Aufbau eines reproduzierbaren In-World-Akzeptanztests. `projectplan.md` bleibt verbindlich, und die ausdrücklich aufgeschobenen Avatar/Bewegungs/EventQueue-Bugfixes bleiben späteren Blöcken vorbehalten.

Start: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Profile: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-profiles-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl70-live.log`; nur `develop-current`.

Neuer Chat: zuerst GitHub-Branch `develop-current` prüfen (`marlin198323-glitch/nexverse-viewer`), lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Dann `CHATUEBERGABE.md`, verbindliches `projectplan.md`, `info.md` und technische Docs lesen. 3–4 größere, zusammenhängende Entwicklungsblöcke, vollständiger C++23-Debug-Build, ALLE CTests inklusive NVIDIA OpenGL 3.3 via `DISPLAY=:0`, GUI-Smoke, Versionspflege, vollständiger Git-Commit/Push und SHA-Verification. Divergierendes `develop` niemals ohne Benutzerweisung mergen; authentifizierte Live-NexVerse-In-World-Verifikation steht weiterhin aus.


## 0.2.0-alpha.71 – Chatübergabe und Folgeblöcke · 11.10.2026

**Block A – konkrete GetMesh-OpenGL-Fehlernummern:** Der vorhandene GL-Uploadhelfer `upload_mesh` kann nun über einen optionalen `GLenum*` die tatsächlich gemeldete OpenGL-Fehlernummer an `Renderer::upload_mesh_asset` weiterreichen. `GpuUploadResult.gl_error` ist bei GL-Fehlern gesetzt; `GL_OUT_OF_MEMORY` wird als `allocation`, andere GL-Fehler als `driver_error` klassifiziert. Der Viewer protokolliert bei gescheiterten Mesh-Uploads `gpu_failure` und `gl_error` zusammen mit Retry-Delay. Kein GL-Fehler/fehlgeschlagene Handle-Anlage kann weiterhin `driver_error` mit `gl_error=0` ergeben; diese semantische Grenze ist beabsichtigt.

**Block B – transaktionale GPU-Textur-Staging-Reserve:** Neue reine C++-Hilfsfunktion `assets::gpu_staging_budget(texture_resident_bytes,mesh_resident_bytes,incoming_bytes,shared_budget_bytes)`. Sie prüft vor einer Kandidaten-Upload-Transaktion den kurzzeitigen geschätzten Höchstbedarf, während Altressourcen für Rollback noch auf der GPU liegen. Sicherheitsabstand: `min(16 MiB, shared_budget / 32)`; bei 384 MiB geteiltem Assetbudget also 12 MiB. Überlauf und Nullbudget werden sicher behandelt. Ein unzureichendes Staging-Budget führt zur nichtdestruktiven Deferral statt zum sofortigen Entfernen alter Texturen; zusätzliches Diagnosefeld `texture_gpu_staging_deferrals` und Warnlog mit numerischen Bytes. Die Angaben sind Software-Schätzungen, **keine** Garantie real freien VRAMs einschließlich Terrain/UI/Driver-Overhead.

**Block C – echte Weltobjekt-Renderzähler:** `RenderDiagnostics` liefert jetzt zusätzlich `scene_draw_calls`, `scene_mesh_draw_calls`, `scene_prim_draw_calls`. Zähler werden je Frame zurückgesetzt und ausschließlich beim tatsächlich ausgeführten glDrawElements aus der Weltobjekt-/Face-Schleife fortgeschrieben; Sky, Terrain, Wasser, Avatar-Proxies, UI zählen nicht dazu. Mesh-Zähler erfordern ein tatsächlich vorhandenes GPU-Mesh-Handle für die Mesh-ID, sonst zählt der tatsächlich gerenderte Fallback als Prim. `InWorldAssetSnapshot` enthält das letzte beendete Frame als `previous_frame_*`, wodurch In-World-Protokolle zwischen Asset-Residency und wirklichen Mesh-Draws unterscheiden. Synthetische Linux-OpenGL-Tests bestätigen Mesh UND Prim sowie die Summe aller Szene-Draws.

**Block D – reproduzierbarer In-World-Akzeptanzprüfer:** `tools/ogl_inworld_acceptance.py` wertet sanitizte Berichte des bestehenden `ogl_asset_log_report.py` aus; erforderlich sind mindestens 3 **aufeinanderfolgende Snapshots derselben Regionsepoche**, aktive Region, nichtleere gewünschte Textur-/Meshsets, mindestens 75 % Coverage beider Familien und ein tatsächlich ausgeführter Scene- sowie GPU-Mesh-Draw im neuesten Snapshot. Nur dann liefert die CLI `evidence_pass`/Exitcode 0; ansonsten `not_verified`/Exitcode 2. JSON liefert pro Kriterium booleschen Prüfstatus; keine UUID/CAPS-URL wird ausgegeben. Zwei zusätzliche CTests für GPU-Reserve und Akzeptanzlogik, erweitert um Regionsepochenwechsel, ergänzen die Renderer-/Snapshot-Regressionen. C++23-Debug und 56/56 CTests wurden nach Implementierung erfolgreich geprüft.

**Lokale Ist-Daten / korrekte Abgrenzung:** Die autorisierte Datei `$HOME/.local/state/opengenesislink/viewer.log` enthielt am 11.10.2026 weiterhin 0 valide `InWorldAssetSnapshot`-Zeilen; der neue Akzeptanzprüfer lieferte folgerichtig `not_verified` mit Exitcode 2. Die Datei verzeichnet historische 230 Mesh- und 56 Texture-Fehler (42 mit 404-Muster, eine mit 5xx-Muster); daraus kann keine aktuelle in-world Rendering-Freigabe abgeleitet werden. Auch `evidence_pass` allein würde Avatarsteuerung, Teleport und EventQueue **nicht** bestätigen. Diese Problemfelder bleiben wie vereinbart in einer späteren Bugfixphase.

**Nächste zusammenhängende Releaseblöcke (alpha.72):** Verbesserte In-World-Snapshot-Serienvalidierung mit Zeitstempeln, konkreter Renderer-Frame-Mesh-Face-Abnahme, GPU-Staging-Deferral-Lebenszyklus/Recovery sowie weiteres Mesh- und Texture-Streaming-Feedback. `projectplan.md` bleibt verbindlich.

CLI: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`; CMake: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Branch ausschließlich `develop-current`.

Wiedereinstieg: Repo `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`, lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, `CHATUEBERGABE.md`, `projectplan.md`, `info.md` und relevante Renderer/Asset-Docs lesen. Größere 3–4 Blöcke pro Release, C++23-Release-Preset Debug bauen, alle CTests unter DISPLAY=:0, GUI Smoke, VERSION/Docs, Commit+Push, lokale/Tracking/GitHub-SHA vergleichen. Keinen automatischen Merge mit dem divergenten Branch `develop` durchführen. Der In-World-Akzeptanzprüfer meldete mit den bisher vorhandenen echten Logs **not_verified**; das ist ausdrücklich kein Softwarefehler und kein bestandener Live-Test.


## 0.2.0-alpha.72 – Fortsetzung und Übergabe · 11.10.2026

**Block A – belastbare Snapshotserien statt altem Log-Zufall:** `AssetStreamingSnapshot` erzeugt bei `simulation_ready` ergänzend `session_started_unix_ms` (Prozesskennung als Startzeit), `emitted_unix_ms` und monoton steigende `sequence`; Schema `ogl.asset.streaming.v1` bleibt additive-only. Der Parser `tools/ogl_asset_log_report.py` zählt als neueste zusammenhängende Serie nur Snapshots derselben Session UND Regionsepoche mit lückenloser Sequenz und realistischem zeitlichen Abstand 15–90 s. Der CLI-Akzeptanzprüfer `tools/ogl_inworld_acceptance.py` nutzt `ogl.inworld.acceptance.v2` und prüft zusätzlich den Echtzeitstatus: letzter Snapshot maximal 180 s alt (10 s Uhr-Toleranz), 3 zusammenhängende Snapshots, nichtleere Desired-Textur- und Mesh-Sets, jeweils mindestens 75 % Residency und tatsächlich ausgeführte Scene- bzw. Mesh-Draws. `--max-age-seconds`, `--min-snapshots`, `--min-coverage` sind konfigurierbar. Ohne die neuen Zeitfelder oder bei historischen Logs ist das Ergebnis absichtlich `not_verified` (Exit 2), kein erfundener Liveerfolg.

**Block B – realer GPU-Mesh-Face-Nachweis:** OpenGL-RenderDiagnostics unterscheidet nun die erfassten `scene_mesh_draw_calls` (wirklich hochgeladenes Mesh) und `scene_missing_mesh_draw_calls`: Letztere zählen echte Prim-Fallback-Draws für Objekte, die eine Mesh-UUID anfordern, aber keine passende GPU-Meshressource besitzen. Ein erweiteter echter GLFW/OpenGL-3.3-Test lädt 2 getrennte Face-Indexbereiche, prüft exakt 2 GPU-Mesh-Drawcalls plus Prim-/Missing-Mesh-Fallback sowie den Frame-Reset nach dem Entfernen der Meshressource. Das In-World-Snapshotfeld `previous_frame_scene_missing_mesh_draw_calls` und der sanitized Log-Report `peak_missing_mesh_fallback_draws` liefern eine konkrete Anzeige der bisher fehlenden Meshobjekte, aber noch keine Fehlerbehebung des OpenSim-Meshdownloads.

**Block C – GPU-Staging-Deferrals werden wieder freigegeben:** `GpuStagingRecovery` speichert zurückgestellte Textur-UUIDs mit dem zugehörigen geschätzten Texture-/Mesh-Byteverbrauch und kontrollierter Cooldownzeit. Ein erneuter Versuch wird ermöglicht, sobald eine der beiden Assetfamilien weniger GPU-Speicher benötigt oder der Retry-Zeitpunkt erreicht ist; wiederholte Deferrals verwenden je UUID steigende 2/4/8/16/32/64-s-Cooldowns, ohne beim Freigeben der Retry-Anfrage die Historie zu verlieren. Erfolgreiche Uploads löschen die Historiereihe; bei Regionswechsel Reset; bei geänderter Desired-Residency verschwundene Assets werden bereinigt. Laufende Worker werden nicht zwangsweise unterbrochen. `texture_gpu_staging_waiting` / `texture_gpu_staging_released` und die bereits vorhandenen Deferrals gehen in In-World-JSON/Streaminglogs und den sicheren Report ein.

**Block D – Regressionen/Streaming-Feedback:** Ein neuer C++-CTest prüft Memory-Drop-Retry, Cooldown-Aufstieg, Recovery, Resolution, Prune und Region-Reset. Der Python-Akzeptanz-CTest prüft fehlende, veraltete und synthetische valide Serien sowie Sessionwechsel, Epochensprünge, Sequenzlücken und unrealistische Abstände; die Renderer-Integration prüft die Multi-Face-Geometrie, tatsächliche GL-Zeichnungen und fehlende Mesh-GPU-Residency. Erwartet sind 57/57 CTests inkl. dreier echter GL-Context-Tests. Die numerische Log-Whitelist wird um die neuen Snapshotfelder erweitert, ohne Asset-UUIDs/CAPS-URLs in Ausgaben aufzunehmen.

**Lokaler Status am 11.10.2026:** Die autorisierte `$HOME/.local/state/opengenesislink/viewer.log` enthält noch 0 neue `InWorldAssetSnapshot`-Zeilen. Der überarbeitete CLI-Prüfer liefert `not_verified`/Exitcode 2. Das ist technisch korrekt und darf nicht als abgeschlossene Live-NexVerse-Regions-, Avatar-, Mesh- oder Bewegungsabnahme dargestellt werden. Bekannte EventQueue-/Teleport-/Avatarprobleme bleiben wie gewünscht auf dem späteren Bugfix-Pfad. Außerdem sind GPU-Bytegrenzen nur Software-Schätzungen und keine Treiber-VRAM-Garantie.

**Folgerelease Alpha.73 / 3–4 Blöcke:** GPU-Streaming-Retry-Integration und Budget-Pressure-Regressionen mit realem GL-Kontext, weitere Missing-Mesh-/Face-Fallback-Statistik, wiederholbare Langzeit-OpenGL-Szenentests sowie echte In-World-Log-Akzeptanz sobald Regionenzugang/Assets bereit sind. Maßgeblich ist weiter `projectplan.md`.

CLI auf dem autorisierten Linux-PC: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"` (Exitcode 0 nur für neueste gültige Live-Daten). Branch ausschließlich `develop-current`.

GitHub: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; lokal `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Für neuen Chat zuerst `git status`, diese Übergabe, `projectplan.md` und `info.md` sowie Asset-/Mesh-/Texture-/Log-Docs. Dann 3–4 größere Entwicklungsblöcke, `cmake --preset debug`, `cmake --build --preset debug -j2`, `env DISPLAY=:0 ctest --preset debug --output-on-failure`, `timeout 7s env DISPLAY=:0 ./build/debug/nexverse-viewer` GUI-Smoke, alle Docs/VERSION, Commit & Push nach GitHub verifizieren. Den divergenten Branch `develop` nicht automatisch mergen. `not_verified` des In-World-Prüfers ist bei fehlenden Live-Daten erwartungsgemäß.


## 0.2.0-alpha.73 – Chatübergabe und nächste 4 Blöcke · 11.10.2026

**Block A – vollständig belegte GetTexture-GPU-Slots korrekt behandeln:** Die Demand-Anfrageprüfung vor HTTP blockierte bisher jede neue Asset-UUID, sobald bereits 512 Texturen GPU-resident waren, obwohl die bestehende `plan_texture_slot_admission` danach bereits eine niedriger priorisierte Textur verdrängen kann. In `src/app/application.cpp` wird bei voller Tabelle jetzt eine rangbewusste Preflight-Slotzulassung (gewünschte und zur Räumung markierte Resident-UUIDs) durchgeführt; nur unmögliche Transfers werden deferred. Nach dem Download prüfen Bytebudget, Staging-Reserve, Slotpriorität und transaktionaler Renderer-Commit weiterhin die tatsächliche Machbarkeit. Unit-Test erweitert für 512 belegte Slots und die Nichtverdrängung höher priorisierter Objekte.

**Block B – tatsächlicher OpenGL-Soak-Test mit GPU-Pressure und Recovery:** `tests/renderer_gpu_soak_tests.cpp` erstellt einen versteckten echten OpenGL-3.3-Kontext mit 24 Weltobjekten, davon 20 gültige GPU-Meshes und vier bewusst nicht hochgeladene Mesh-UUIDs, plus acht echte GPU-Texturen. In 160 gerenderten Frames ersetzt es wiederholt Texturen, ersetzt GPU-Meshes und kontrolliert pro Frame GL-Fehlerfreiheit, korrekte Drawcall-Summe, vorhandene GPU-Mesh-Draws und stabile Zahl fehlender Meshobjekte. 40 gezielte *simulierte* Software-Staging-Druckereignisse werden mit `gpu_staging_budget`/`GpuStagingRecovery` freigegeben; 160 Transaktionen tatsächlich im Renderer ausgeführt. Beispiel auf der Linux RTX 4070 SUPER: 43 ms CPU-Laufzeit für diese kleine synthetische Szene, kein FPS- oder realistischer Simulatorperformance-Benchmark. Ohne GL-Kontext überspringt sich der CTest (Exit 77).

**Block C – präzise Mesh-Fallback-Metriken:** Der Renderer zählt zusätzlich zu bisherigen pro Frame ausgeführten Missing-Mesh-Fallback-Drawcalls jetzt **einzigartige betroffene Szenenobjekte** und die wirklich als Ersatz gezeichneten **Dreiecke**. Da eine Region pro Objekt mehrere Materialfaces und transparente/opaque Renderpasses besitzt, vermeidet ein Frame-lokales Set eine Überzählung der Objekte. Die numerischen `InWorldAssetSnapshot`-Keys `previous_frame_scene_missing_mesh_objects` und `previous_frame_scene_missing_mesh_triangles` sowie der sanitizte Logbericht zeigen das Ergebnis. Reale OpenGL-Multi-Face- und Mesh-Entfernungs-Tests prüfen diese Zähler; fehlende OpenSim-Meshdownloads werden damit diagnostiziert, nicht behauptet repariert.

**Block D – verschärfte In-World-Abnahme und Streaming-Regressionen:** `tools/ogl_inworld_acceptance.py` verlangt nun standardmäßig null zuletzt tatsächlich gerenderte Missing-Mesh-Fallback-Draws, zusätzlich zu frischer Session-/Epoch-/Zeitserie, nichtleerer Region, >=75 % Texture- und Mesh-Residency und tatsächlichen GPU-Mesh-Draws. Nur für bewusst teilweise bereite Szenen kann der Aufrufer `--max-missing-mesh-draws N` setzen. Fehlende Diagnosefelder gelten nicht als null und können keinen Pass erzeugen. Python-Cases prüfen strikten Status und Override; C++-Regressionen GPU-Full-Residency/Byte-Slot-Vorabzulassung/Snapshot werden mit GL-Tests ausgeführt. Insgesamt **58/58 CTests bestanden** (erster vollständiger Entwicklungsbuild).

**Abnahmegrenze:** Auf der autorisierten Linux-Workstation ist die frühere `viewer.log` weiterhin ohne aktuelle In-World-Snapshotserie (`not_verified`), insbesondere ohne Prüfung echter NexVerse-Mesh-, Avatar- und Simulatorinteraktionen. Synthetische Tests ersetzen keinen erfolgreichen Simulatorbeitritt. Seit Alpha.70 bewusst später zu beheben: fehlender Avatar, WASD/Bewegung, Teleport, CAPS EventQueue, teilweise fehlende Meshobjekte. Die aktuelle Arbeit führt den maßgeblichen `projectplan.md` ohne ungefragte Fehlerbehebungsphase fort.

**Nächste 3–4 Releaseblöcke Alpha.74:** Zeitliche Priorität und aktive GPU-Budget-Deferral-Diagnose bei geteiltem Tex-/Mesh-Speicher, Wiederverwendung von Assets über Regions-/LOD-Wechsel, erweitertes realistisches GL-Szenen-/Materialtestprofil und automatische Prüfung wirklich neuer Regionlogs sobald echte Server- und Loginbedingungen erfüllt sind.

Lokaler Debug/Soak: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-soak-tests`. In-World-Prüfer: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repo `marlin198323-glitch/nexverse-viewer`, einzig maßgeblicher Branch `develop-current`.

Wiedereinstieg: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; Workstation-Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst `git status`, `projectplan.md`, `info.md`, diese Übergabe, technische Docs lesen. Wie gewohnt drei bis vier große Entwicklungsblöcke, vollständiger Debug-Build, alle CTests mit `DISPLAY=:0` inklusive echten GLFW/GL3.3-Soak, GUI Smoke, `VERSION` und Docs, GitHub Commit+Push und SHA-Verifikation. Divergierenden Branch `develop` nicht eigenständig mergen. Live-OpenSim bleibt `not_verified` bis echte In-World-Daten vorliegen.


## 0.2.0-alpha.74 – Handoff und nächster Entwicklungsplan · 11.10.2026

**Block A – geteiltes Tex-/Mesh-GPU-Budget und Mesh-Retry-Recovery:** Mesh-Anfragen, deren dekodierte GPU-Geometrie das kombinierte Budget überschreitet, werden nicht mehr unbegrenzt allein durch `mesh_gpu_budget_deferred_ids` blockiert. Neuer `GpuStagingRecovery` auch für Meshes, mit Key `UUID.LOD`, progressivem 2/4/8/16/32/64-s-Cooldown und vorgezogener Re-Evaluierung nach sinkendem Texture- oder Mesh-Residency-Byteverbrauch. Bei Regionswechsel Reset, bei geänderter Scene-Desired-Liste Prune, nach erfolgreichem Mesh-Upload Resolution. Dieselbe Recovery wird nun auch bei nicht passendem Texture-Byte-/Slotplan verwendet, nicht nur bei Staging-Reserveverletzung. Numeric Snapshotfelder `mesh_gpu_budget_waiting`, `mesh_gpu_budget_released`, plus bestehende Textur-Deferrals. Risiken der nur geschätzten (Terrain/UI/Driver nicht eingeschlossenen) GPU-Bytegrenzen bleiben.

**Block B – tatsächliche wiederverwendete CPU-Assets, keine falsche GPU-Hot-Reuse-Behauptung:** Die bisherigen Mesh-/Texture-RAM- und Disk-Caches bleiben regionsübergreifend im Speicher bzw. auf Festplatte bestehen; GPU-Handles werden aus Sicherheitsgründen weiterhin bei Regionswechsel geräumt. Der neue begrenzte `AssetReuseHistory` (max. 4096 UUIDs/Familie) beobachtet ausschließlich erfolgreiche erneute GPU-Uploads und zählt sie als `cached_region_reuse` oder `cached_lod_swap` NUR, wenn der Worker ausdrücklich einen RAM-/Disk-Cache-Hit meldete. Texture-Mipqualität und Mesh-LOD werden pro UUID erfasst, unabhängig vom Cache-Tier. Snapshotfelder für `texture_cpu_cached_region_reuse`, `texture_cpu_cached_lod_reuse`, `mesh_cpu_cached_region_reuse`, `mesh_cpu_cached_lod_reuse`; gleichartige Streaming-Logs. Keine Gewähr, dass dieselbe UUID bei Regionwechsel auf einem fremden Server existiert oder identische GPU-Handles weiterverwendet werden.

**Block C – langlebiger realer OpenGL-Material-/Szenentest:** Der bereits echte GLFW/OpenGL-3.3-Soak mit 160 Frames, 160 transaktionalen Texture-Replacements, 40 simulierten Staging-Deferrals, 20 GPU-Meshes plus vier absichtlich fehlenden Meshobjekten testet jetzt abwechselnd opake und teiltransparente Texturen, per-Face-Alpha-Tints, Fullbright, Shininess sowie Repeat-/Offset-Materialparameter. Szenenmaterial wird in jedem Frame aktualisiert. Echte OpenGL-Frames, `scene_mesh_draw_calls`, `scene_prim_draw_calls`, `scene_missing_mesh_objects`, GL-Fehlerfreiheit und GPU-Assetreplacements werden kontrolliert. Das ist eine synthetische GPU-/Legacy-Material-Regressionsprüfung, keine vollständige PBR- oder NexVerse-InWorld-Visualabnahme.

**Block D – End-to-End GPU-Pressure/Queue-Diagnostik und reproduzierbare Tests:** Neuer CTest `asset_gpu_pressure_pipeline_tests.cpp` simuliert gemeinsam beanspruchtes 384-MiB-Softwarebudget: eine blockierte Mesh-Anfrage wird unmittelbar nach 12-MiB-Texturfreigabe wieder möglich, Staging schützt alte GPU-Assets, Textur-Cooldown läuft ab, Regionswechsel leert Deferrals, RAM-/Disk-Hit bleibt auf Cacheebene für folgende Region nachvollziehbar. Neuer Unit-Test `asset_reuse_history_tests.cpp` kontrolliert bounded History, Regionswechsel, Qualitätswechsel und Nicht-Cache-Fälle. Der Log-Analyser nimmt neue numerische Snapshotfelder in die Whitelist auf und gibt Spitzenwerte der mesh-budget-Warteschlange frei, ohne UUIDs/CAPS-Adressen/Secrets auszugeben; Python-Test prüft dies. Damit **60/60 CTests** im ersten vollständigen Debug-Testlauf.

**Lokale Live-Abgrenzung 11.10.2026:** Der unverändert mit der autorisierten Workstation vorliegende `$HOME/.local/state/opengenesislink/viewer.log` enthält noch keine gültigen `InWorldAssetSnapshot`-Serien; die CLI `tools/ogl_inworld_acceptance.py` ergibt `not_verified`/Exitcode 2 (0 geeignete Snapshots). Keine verifizierte Aussage zur vollständigen NexVerse-Welt, Avatar-Body, WASD/Bewegung, Teleport, fehlenden realen Meshobjekten oder EventQueue. Diese bereits bekannten Themen verbleiben auf der ausdrücklich später geplanten Fehlerbehebungsphase.

**Nächster geplanter Stand alpha.75:** Noch gezielteres CPU/GPU-Cache-Residency-Management mit eindeutiger Kennzeichnung von HTTP-/RAM-/Disk-Treffern und anhaltender Budgetlast, stärkere GL-Legacy-Material-/Multi-Face-Szenen, prüfbare Wiederverbindung nach Regionenwechsel und aussagekräftige Auswertung echter In-World-Testlogs, sobald Authentifizierung und Regionsdaten verfügbar sind. `projectplan.md` bleibt die verbindliche Roadmap.

Repo `marlin198323-glitch/nexverse-viewer`, aktiver Branch `develop-current`, lokale Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Standard: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; anschließend GUI-Smoke und GitHub-SHA-Vergleich.

Für den nächsten Chat: ZUERST GitHub-Branch `develop-current`, `CHATUEBERGABE.md`, verbindliches `projectplan.md`, `info.md`, die Renderer-/Asset-Dokumentation prüfen. Entwicklung in vier großen Blöcken pro Release; alle Quellen, Tests, Version und Docs gemeinsam committen und pushen. CMake-Debug-Build, alle CTests bei `DISPLAY=:0`, Linux-GUI-Welcome-Smoke; SHA von GitHub/Tracking/Local gleich. Branch `develop` ist divergent und nicht ungefragt mergen. Live-OpenSim `not_verified` darf NICHT als bestandener Welt-Test gelten.


## 0.2.0-alpha.75 – Übergabe und nächste Entwicklungsgruppe · 11.10.2026

**Block A – deterministic bounded CPU-Asset-Reuse (RAM vs Disk):** `include/nexverse/assets/asset_reuse_history.hpp` was changed from arbitrary `unordered_map.begin()` eviction to real LRU with a `std::list` and O(1)-average lookup/touch/evict. The 4096 UUID/family limit is maintained. New `AssetReuseHistory::Tier {network,memory,disk}` distinguishes actual reported successful memory/disk-backed GPU reuploads at a later RegionEpoch or different texture mip / mesh LOD; counters `region_ram_hits`, `region_disk_hits`, `lod_ram_hits`, `lod_disk_hits` and `evictions`, plus prior combined hit counts. Application maps the texture loader's explicit `RAM texture cache hit` / `Disk texture cache hit` and mesh `MeshCacheTier` into this tier. New numeric `InWorldAssetSnapshot` and safe log parser fields distinguish the cache provenance (including history LRU evictions). This is CPU-/disk-cache reuse; GPU textures and VBOs remain invalidated on region changes.

**Block B – targeted GPU Deferred/Retry lifecycle:** `GpuStagingRecovery::synchronize(blocked, desired)` prunes obsolete blocked UUIDs in lockstep with active desired sets, **preserving cooldown for still-required UUIDs** and unblocking orphans that have no recovery deadline; a corresponding regression test verifies this. Texture plan changes no longer indiscriminately clear all deferred IDs; specific changed mip-levels release their own state, and full-slot preflight rejection explicitly adds a timed recovery record. Mesh desired-Lod set changes use synchronized `UUID.LOD` deferred keys rather than a blanket clear. Existing actual GPU-memory-eviction code can still trigger early retry, and its software VRAM estimates still exclude other driver allocations. No unauthenticated network or EventQueue behavior is asserted fixed.

**Block C – real native GL3.3 three-face region transition test:** New `tests/renderer_region_transition_tests.cpp` creates a hidden actual GLFW/OpenGL 3.3 context. It uploads three independently materialized faces, including opaque texture, blended tint and alpha-textured face. It verifies three GPU mesh Face-Draws and no missing-Mesh fallback. It then simulates a region change by clearing all GPU meshes and textures, validates correct Prim fallback without any stale GPU mesh, and reuploads the SAME immutable UUID with refreshed texture bytes; a new rendered frame must again show 3 GPU Mesh Face-Draws and 0 missing mesh objects. All stages check `glGetError()`; skip code 77 if no GL context. The test is synthetic, not a real authenticated OpenSim region reconnect.

**Block D – series-wide latest-session In-World acceptance:** The log analyzer now computes only from the most recent contiguous valid `(session_started_unix_ms, region_epoch, sequence, 15–90 s interval)` series `latest_region_min_texture_coverage_pct`, `latest_region_min_mesh_coverage_pct`, `latest_region_min_mesh_draw_calls`, `latest_region_max_missing_mesh_fallback_draws` and `latest_region_max_mesh_budget_waiting`. `tools/ogl_inworld_acceptance.py` now rejects an apparently healthy latest frame if the current series contains an earlier low-coverage frame, absent required mesh-draw evidence, or missing-mesh fallback. Missing fields result in `not_verified`, not assumed zeros. New dedicated `tests/test_ogl_inworld_transition_report.py` tests healthy series, degraded middle frame, missing mesh in earlier frame, missing telemetry field and session/epoch boundaries. End-to-end C++ / OpenGL and Python regression count **62 CTests**, all passed before final version rebuild.

**Proven live status:** The authorized workstation's `$HOME/.local/state/opengenesislink/viewer.log` still contains **zero** valid current `InWorldAssetSnapshot` samples; acceptance returns `not_verified` with exit code 2. It is not possible to claim a real NexVerse Region/Avatar/Movement/Teleport/EventQueue acceptance from the GL tests or these logs. Those user-deferred issues remain in the project roadmap for later troubleshooting.

**Next large release alpha.76 (3–4 blocks):** cache-pressure eviction and persistent asset-tier observations, region-switch cancellation and stale HTTP event contract tests (without assuming live simulator connectivity), wider material/texture transparency scenario coverage, explicit failure diagnosis of missing Real Mesh UUIDs once real region snapshots exist. Keep the planned staged sequence in `projectplan.md`; don't silently move debugging of WASD, avatar, Teleport or CAPS queue ahead.

Build: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. Live check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Private GitHub repo `marlin198323-glitch/nexverse-viewer`, branch ONLY `develop-current`.

For next chat: read authoritative `projectplan.md`, `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md`, `docs/LOG_ANALYSIS.md` from branch `develop-current`. Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Maintain 3–4 substantial release blocks, all source/tests/markdown/VERSION together, full debug build, `DISPLAY=:0` CTests/GL Smoke, GitHub push and exact SHA verification. Never automatically merge the divergent `develop` branch. All authentic in-world acceptance remains `not_verified` without new genuine simulator data.


## 0.2.0-alpha.76 – Handoff und nächster Entwicklungsstand · 11.10.2026

**Block A – echte CPU-RAM-Cache-Belastungstransparenz:** Die beiden bestehenden LRU-Caches `TextureMemoryCache` und `MeshMemoryCache` erfassen nun direkt `hits`, `misses`, `evictions`, `bytes` und `entries`. Der Zugriff auf die Statistiken ist mutexgeschützt, die Evictionzählung erfolgt ausschließlich beim wirklichen Verdrängen eines älteren Eintrags. Ein neuer gemeinsamer Header `asset_memory_cache_stats.hpp` hält das numerische Messformat. Der Viewer überträgt je Familie Treffer, Fehlzugriffe und Verdrängungen in `InWorldAssetSnapshot` (`texture_ram_hits`, `texture_ram_misses`, `texture_ram_evictions`, `mesh_ram_hits`, `mesh_ram_misses`, `mesh_ram_evictions`). Der sanitizte Python-Logbericht berücksichtigt die Felder und berechnet `peak_texture_ram_evictions` / `peak_mesh_ram_evictions`. Vorhandene CPU-RAM-Cachetests prüfen die neuen Zähler und Speichergrenzen.

**Block B – kontrolliert verspätete HTTP-Antwort beim Regionswechsel:** Neuer nativer Test `tests/region_http_epoch_contract_tests.cpp`: Eine asynchrone, simulierte GetMesh-HTTP-Antwort wird nach Beginn des Requests in Region A gezielt zurückgehalten. Nach dem Übergang zu B wird die alte Antwort freigegeben und muss an der vorhandenen Epochensperre verworfen werden, ohne die Mesh-Disk-/RAM-Caches zu befüllen. Anschließend wird ein neuer Request im aktiven B akzeptiert. Auch der Rückwechsel B → A erhält eine neue Epoche und erlaubt einen authentifizierten Cachetreffer, ohne dass die alte Antwort wieder gültig wird. Dies prüft reale asynchrone Steuerlogik mit kontrolliertem Transportcallback und existierender `RegionMaterialScope`/`load_mesh_cached`-Integration, jedoch keinen echten Login/HTTP-Endpunkt.

**Block C – umfangreicherer nativer OpenGL-3.3-Material-/Multiface-Test:** `renderer_region_transition_tests.cpp` prüft weiterhin GPU-Freigabe und Wiederaufbau derselben Mesh-UUID nach einem Regionswechsel. Ergänzend rendert der echte versteckte GLFW/OpenGL-3.3-Kontext **48 Frames mit zwei Objekten und jeweils drei Mesh-Faces**. Face-Tint/Alpha, transparente Textur, Fullbright, Glow, UV-Rotation, UV-Repeat/Offset und Planar-Texturmodus wechseln. Pro Frame müssen sechs GPU-Mesh-Face-Draws, null Missing-Mesh-Objekte, konsistente Drawcall-Summen und GL-Fehlerfreiheit vorliegen. Diese synthetischen Renderer-Regressionen prüfen technische Pfade, nicht die visuelle Pixelgenauigkeit einer realen OpenSim-Region.

**Block D – kategorisierte Missing-Mesh-/GetTexture-Ausfälle:** Neues `AssetFailureTelemetry` mit numerischen Zählern `missing`, `transient`, `invalid`, `configuration`, ohne UUID/CAPS-Adresse. Der reale Applikationspfad zählt kategorisiert fehlgeschlagene Texture- und Mesh-Downloads bzw. GPU-Rejections (mit bestehenden AssetFailure-Regeln); bei erfolgreichen GPU-Uploads wird keine Fehlerzählung erhöht. `InWorldAssetSnapshot` ergänzt `mesh_fail_{missing,transient,invalid,configuration}` und `texture_fail_{missing,transient,invalid,configuration}`. Der vorhandene sichere Python-Analyzer nimmt die Felder auf und meldet `max_mesh_missing_events`, `max_mesh_transient_events`; Tests kontrollieren Kategorieabbildung, Snapshotformat und bestehende Cacheauswertung. Zusammen **64/64 CTests** im Entwicklungsbuild, darunter native GLFW/OpenGL-Tests und der neu hinzugefügte zeitversetzte Regions-HTTP-Test.

**Bewertungsgrenzen / Ist-Status:** Ein Simulator-HTTP-404 / 410 wird getrennt als `missing` dokumentiert, Retry-/Serverfehler als `transient` und Fehlkonfiguration separat. Diese Kategorien erlauben später eine gezielte Ursachenanalyse der realen fehlenden Meshobjekte, beheben aber noch nicht den OpenSim-Netzwerkdownload oder die CAPS-EventQueue. Der autorisierte echte lokale `viewer.log` enthält am 11.10.2026 weiterhin 0 geeignete `InWorldAssetSnapshot`-Zeilen; In-World-Prüfer meldet `not_verified`/Exitcode 2. Ohne authentifizierte Region kann weder Avatar, Bewegung, Teleport noch die Bildqualität vollständig abgenommen werden. Diese bekannten Probleme bleiben wie vereinbart für die spätere Bugfixphase vorgesehen.

**Nächste 3–4 koordinierte Blöcke für Alpha.77:** Auswertung von Cachemiss-Raten und Prioritätskontrolle unter längerer Streaminglast, zuverlässige stale-Response-Trennung auch für Texturdownloads/Prefetch in isolierten Regressionstests, Mesh-Face-/Material-Transparenz-Routing samt GPU-Speicherlast und Diagnose tatsächlicher OpenSim-Regiondaten sobald verfügbar. Der verbindliche `projectplan.md` bestimmt weiterhin die Reihenfolge.

Build/Test auf der autorisierten Workstation: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`. In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.

Wiedereinstieg: Branch `develop-current` des privaten Repos `marlin198323-glitch/nexverse-viewer`; Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Verbindlich sind `projectplan.md`, `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md` und `docs/LOG_ANALYSIS.md`. Bei `weiter`: 3–4 substanzielle zusammenhängende Blöcke implementieren; vollständiger C++23-CMake-Debug-Build, alle CTests mit `DISPLAY=:0`, realer Linux-Viewer-Smoke; VERSION/Docs aktualisieren; Commit+Push auf `develop-current`, lokalen/Tracking/GitHub-SHA vergleichen und clean worktree sicherstellen. Divergentes altes `develop` nicht automatisch mergen. Der erfolgreiche synthetische Region-HTTP-/OpenGL-Test ist keine In-World-Live-Abnahme; echte Region aktuell `not_verified`.


## 0.2.0-alpha.77 – Übergabe und nächste Entwicklungsgruppe · 11.10.2026

**Block A – 30-s-Intervallanalyse echter RAM-LRU-Belastung:** Neues `AssetCachePressureWindow` berechnet nicht nur kumulative, sondern delta-basierte Hitquote und Anzahl echter Evictions pro Asset-RAM-Cache-Sampling-Intervall. `under_pressure` wird nur bei mindestens 8 neuen Cache-Misses, mindestens 2 Verdrängungen und einer Trefferrate unter 50 % aktiviert. Der Renderer beobachtet Textur- und Mesh-RAM-Cache getrennt und veröffentlicht `texture_ram_hit_rate_pct`, `texture_ram_interval_evictions`, `texture_ram_pressure` und entsprechende `mesh_*`-Werte in `ogl.asset.streaming.v1`. Bei aktivem Texturcache-Druck setzt das freiwillige **Disk-only-Texture-Prefetching** aus, während GetTexture-Anforderungen benötigter sichtbarer Weltassets weiterhin starten dürfen. Regionswechsel resetten die Fenster. Cache-Pressure-CTest deckt Druck, Recovery, Nulländerung und Counter-Reset ab; die Schwellwerte sind konservativ und noch nicht live kalibriert.

**Block B – GetTexture-Worker mit überprüfbarer Regionsantwortsperre:** Die bisher nur in `src/app/application.cpp` kopierte RAM/Disk/HTTP-Mip-Laderoutine ist in `include/nexverse/assets/texture_cached_request.hpp` als `load_texture_cached_scoped` extrahiert; die App verwendet diese unmittelbar mit dem existierenden authentifizierten `TextureAssetLoader`, den CPU-/Disk-Caches, gewünschtem Mip und dem atomischen Regionsepochen-Accept-Callback. Vor HTTP und nach einer Antwort wird die Epoche kontrolliert; eine verspätete HTTP-Antwort wird **vor** der RAM-/Disk-Cache-Übernahme verworfen. Zusätzlich wird eine vom angefragten UUID abweichende erfolgreiche Antwort als `invalid` abgelehnt. Der neue native CTest `texture_http_epoch_contract_tests.cpp` hält eine simulierte HTTP-Antwort gezielt bis nach dem A→B→C-Wechsel zurück, prüft keine stale Cache-Befüllung, erfolgreiche Antwort der aktuellen Region, GPU-Mip-reduziertes Arbeitsergebnis bei weiterhin voller RAM-Assetauflösung, RAM- und Disk-Treffer. Das ist ein kontrollierter Transportvertragstest, keine externe Netzabnahme.

**Block C – echte OpenGL Opaque-vs-Alpha-Face-Telemetrie:** `RenderDiagnostics` zählt bei tatsächlich ausgeführten Szene-`glDrawElements` nun getrennt `scene_opaque_draw_calls` und `scene_alpha_draw_calls`, die sich genau auf `scene_draw_calls` summieren müssen. Beide Zahlen werden beim Frame-Beginn zurückgesetzt und als `previous_frame_scene_opaque_draw_calls` / `previous_frame_scene_alpha_draw_calls` in In-World-Health-Snapshots aufgenommen. Die echten GLFW/OpenGL-3.3-Tests `renderer_region_transition_tests.cpp` (2 Meshobjekte, je 3 Materialfaces, 48 wechselnde Materialframes) und `renderer_gpu_soak_tests.cpp` (160 Frames mit Texture-Updates und fehlenden Meshes) fordern nun tatsächliche Draws in BEIDEN Durchläufen und exakte Summengleichheit. GPU-/Materialfunktionalität im synthetischen GL-Kontext bewiesen, nicht Pixelidentität mit dem Live-Simulator.

**Block D – datensparsame Ursachen-/Missing-Asset-Diagnostik:** Neues `AssetMissingIdCounter` speichert maximal 4096 verschiedene intern beobachtete UUID-Strings pro Region/Familie und zählt nur `AssetFailure::missing`, nicht Transient-/GPU-Failures; bei vollem Set werden Overflow-Ereignisse separat gezählt. Regionswechsel setzt die ID-Zählung zurück. Die App zählt so `mesh_missing_unique`, `mesh_missing_unique_overflow`, `texture_missing_unique`, `texture_missing_unique_overflow` und schreibt nur deren numerische Werte in `ogl.asset.streaming.v1`, niemals die eigentlichen UUIDs/CAPS-Adressen. Der sanitizte Python-Analyzer berechnet auf der neuesten gültigen Region-Snapshot-Serie `latest_region_mesh_missing_events_delta`, `latest_region_texture_missing_events_delta`, `latest_region_peak_missing_mesh_unique`, `latest_region_peak_texture_ram_pressure`, `latest_region_peak_mesh_ram_pressure` und die minimalen Cache-Hitraten. `ogl_inworld_acceptance.py` enthält diese Befunde unter `observations`, verändert jedoch keine bisher gültigen Nachweiskriterien. Zusätzliche C++- und Python-Tests prüfen Bounded/Reset/Overflow, zeitliche Serienaggregation und keinen Secret-Leak. Insgesamt **67/67 CTests** im Entwicklungsbuild erfolgreich.

**Live-Status / Abgrenzung 11.10.2026:** Die echte lokale `$HOME/.local/state/opengenesislink/viewer.log` auf der autorisierten Workstation enthält weiterhin 0 gültige aktuelle `InWorldAssetSnapshot`-Zeilen. `tools/ogl_inworld_acceptance.py` liefert `not_verified`, Exitcode 2. Der Code kann ohne authentifizierten Regionsbeitritt nicht als visuelle Abnahme von Meshes, Avatar, Welt, WASD/Bewegung, Teleport oder CAPS-EventQueue gelten. Diese bereits bekannten Mängel bleiben wie vereinbart für die spätere Fehlerbehebungs- und In-World-Abnahmephase dokumentiert. Hinweis: Der begrenzte Missing-ID-Counter meldet bei Sättigung eine Untergrenze unterschiedlicher UUIDs plus Overflows, keine exakte unbeschränkte Kardinalität.

**Nächster geplanter Release alpha.78, 3–4 Blöcke:** Cache-Pressure-Werte gegen fortlaufende Simulator-Samples validieren; transparentes Texture-/Mesh-Fehlverhalten mit vollständigem Regionswechsel und Asset-Staging prüfen; zusätzliche GPU-Szenenfälle für unterschiedliche Alpha-Mask-/Blending-Kontrakte und eine aussagekräftige Diagnose der wirklich vom Server fehlenden Assets sobald Live-Daten vorliegen. Maßgeblich bleibt `projectplan.md`.

Debug-Kommandos: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; In-World-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, nur Branch `develop-current`.

Wiedereinstieg: zuerst den tatsächlichen Status aus `marlin198323-glitch/nexverse-viewer` Branch `develop-current` holen; Workstation-Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Verbindlich `projectplan.md`, `info.md`, diese `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/TEXTURES.md`, `docs/MESH.md`, `docs/LOG_ANALYSIS.md`. Je `weiter` drei bis vier größere zusammenhängende Blöcke implementieren, alle Tests mit OpenGL auf `DISPLAY=:0`, GUI-Smoke, Docs/VERSION, Commit/Push und vollständige lokale/Tracking/GitHub-SHA bestätigen. Divergenten alten Branch `develop` niemals ungefragt zusammenführen. `not_verified` im echten Simulatorlog ist weiterhin kein bestandener Live-Test.


## 0.2.0-alpha.78 – Chatübergabe und Folgeentwicklung

**Block A – RAM-Cache-Druck ohne Prefetch-Pendeln:** `AssetCachePressureWindow` benötigt nach aktivem Texture-/Mesh-Thrashing jetzt zwei aufeinanderfolgende unkritische 30-s-Auswertungsfenster, bevor `under_pressure` zurückgesetzt wird. Dadurch pausiert das rein optionale Texture-Disk-Prefetch auch nach einem zufällig ruhigen Einzelintervall weiter; erforderliche GetTexture-Netzwerkrequests bleiben unabhängig. Ergänzter Unit-Test für die Hysterese und neuer End-to-End-C++-Test `asset_cache_pressure_lru_tests.cpp`, der tatsächlich `TextureMemoryCache` und `MeshMemoryCache` mit jeweils nur zwei residenten Assets über 24 wechselnde UUIDs belastet, echte Hits/Misses/Evictions beobachtet und anschließend die zwei stabilen Recovery-Intervalle sowie den regionsübergreifend erhaltenen RAM-Inhalt kontrolliert.

**Block B – veraltetes Disk-Prefetch während Regionswechsel:** Produktionsfunktion `prefetch_texture_disk_to_memory` delegiert jetzt an einen separat kontrollierbaren `prefetch_texture_disk_to_memory_scoped`-Vertrag. Der integrierte `texture_prefetch_tests.cpp` hält eine asynchrone, gültige Disk-Leseantwort zwischen Start und Cache-Aufnahme an, wechselt dann die Regionsepoche und gibt den Worker frei. Das alte Texture-Bild darf nicht in die RAM-LRU übernommen werden. GetTexture-Hauptpfad und zuvor bestehende HTTP-Regionsepochen-Sperren bleiben unverändert. Das ist ein deterministischer Worker-/Cache-Vertragstest, kein echter Grid-Download.

**Block C – OpenGL3.3-Alpha-Mask-vs-Blend-Routing:** Neue zentrale CPU-Routingfunktion `face_material_requires_blending(tint_alpha,loaded_texture_has_alpha,mode)`. AlphaMode=1 berücksichtigt Texture-Alpha für den transparenten Pass; Mode=2 behandelt Texture-Alpha als GPU-Discard-/Mask-Bedingung im grundsätzlich opaken Pass, sofern Tint-Alpha 1 ist; Mode 0/3 lösen nicht allein wegen Texture-Alpha einen Blend-Pass aus. Der Shader führt `discard` bei AlphaMode 2 wie zuvor aus. Neuer tatsächlich ausgeführter Frame-Zähler `scene_alpha_mask_draw_calls`, zusätzlich zu bestehenden `scene_alpha_draw_calls` und `scene_opaque_draw_calls`, wird in `RenderDiagnostics` und im anonymisierten `InWorldAssetSnapshot.previous_frame_scene_alpha_mask_draw_calls` ausgegeben. Die echten OpenGL-Regionstransitions-/Mesh-Face-Tests prüfen Mask-Faces und sowohl opaque als auch blended Drawcalls; `material_pass_tests.cpp` enthält alle vier Moduskontrakte.

**Block D – realistische Cache-Druck- und Renderer-Sitzungsanalyse:** `tools/ogl_asset_log_report.py` wertet neueste zusammenhängende gültige Region-Snapshotserien aus und berichtet `latest_region_texture_ram_pressure_samples`, `latest_region_mesh_ram_pressure_samples`, `latest_region_peak_alpha_draw_calls`, `latest_region_peak_alpha_mask_draw_calls`, `latest_region_peak_gpu_upload_p95_us`. `tools/ogl_inworld_acceptance.py` ergänzt die sicheren Werte unter `observations`; sie sind Diagnose, nicht automatisch ein weiterer erfolgreicher Abnahmebeweis. Python-Regressionen belegen Anonymisierung und korrekte serielle Metrik. **68/68 CTests** vor finalem Release-Rebuild bestanden, einschließlich echter GLFW/OpenGL-Szenen und CPU-LRU-Stress.

**Ist-/Abnahmegrenzen:** Autorisierte Linux-Workstation `$HOME/.local/state/opengenesislink/viewer.log`: weiterhin **0** gültige aktuelle `InWorldAssetSnapshot`-Einträge, `ogl_inworld_acceptance.py` meldet `not_verified`, Exitcode 2. Daher keine echte NexVerse-Region-, Avatar-, WASD/Bewegungs-, Mesh-Vollständigkeits-, Teleport- oder CAPS-EventQueue-Freigabe. Diese bekannten Themen bleiben wie vereinbart für die spätere Live-Integrations- und Fehlerbehebungsphase dokumentiert. Alle aktuell getesteten Alpha-/Staging- und Cache-Szenen sind synthetisch, auch wenn der tatsächliche NVIDIA-OpenGL3.3-Treiber verwendet wurde.

**Nächster 3–4-Blöcke-Release Alpha.79:** Transparenz-/Maske-Sichtbarkeit zusätzlich mit reproduzierbarer Pixel-/Framebuffer-Abnahme prüfen, kombiniertes Texture-/Mesh-Staging über mehrere Regionswechsel noch strenger validieren, Ressourcen-Counter/Hysterese bei typischen Cachegrößen profilieren und In-World-Akzeptanzmeldungen für echte Simulatorlogs verfeinern sobald aktuelle Regionsdaten vorliegen. Maßgeblich bleibt `projectplan.md`.

GitHub: `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (alten divergenten `develop` nicht mergen). Tests `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Diagnose `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`.

Fortsetzung beim nächsten `weiter`: GitHub `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`, lokale Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Verbindlich zuerst `projectplan.md`, `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md`, `docs/LOG_ANALYSIS.md` lesen und `git status` kontrollieren. 3–4 größere Blöcke, Debug-Build, alle CTests auf DISPLAY=:0, Linux GUI-Welcome Smoke, VERSION/Docs, GitHub Commit+Push und SHA von Local/Tracking/GitHub exakt vergleichen. Keine ungefragte Verschmelzung des divergenten Branches `develop`. Echte In-World-Abnahme bleibt bis aktueller erfolgreicher Regionsdaten `not_verified`.


## 0.2.0-alpha.79 – Chatübergabe und nächster Entwicklungsstand

**Block A – visuelle Pixel-/Framebuffer-Abnahme für Alpha, AlphaMask und opake Meshfaces:** Neuer echter GL3.3-GPU-CTest `tests/renderer_alpha_framebuffer_tests.cpp` erstellt ein verstecktes GLFW-Fenster, lädt ein GPU-Mesh und eine 2×2-Texture und liest nach `render_world()` den zentralen tatsächlich gerenderten RGBA-Pixel mit `glReadPixels`. Geprüft werden ein opakes rotes Face, vollständiger AlphaMask-Discard bei AlphaMode 2/Cutoff 128, anschließend restaurierte Opakheit und Blend-Rendering bei 50 % Alpha; Framepass-Zähler müssen mit der Sichtbarkeit übereinstimmen. Die maskierte Pixelreferenz ist ein vollständig transparenter AlphaMode-1-Face im **gleichen aktiven Szene-Rendermodus**, nicht ein No-Scene-Frame, da diese unterschiedliche Hintergrundpfade nehmen. Der neue GL-Test läuft auf dem echten NVIDIA OpenGL 3.3-Treiber und bestand. Eine synthetische Einzelpixelabnahme ist keine vollständige PBR-/OpenSim-Bildqualitätsfreigabe.

**Block B – drei vollständige GPU-Regionswechsel unter Mesh-/Texture-Staginglast:** Der bestehende tatsächliche GLFW/OpenGL-3.3-Test `tests/renderer_gpu_soak_tests.cpp` führt weiter 160 Frame-Draws, 160 transaktionale Texture-Replacements und 40 künstlich erzeugte Staging-Deferrals aus. Zusätzlich entfernt er bei Frame 40, 80 und 120 alle GPU-Texture- und Mesh-Ressourcen, setzt die Region-bezogene `GpuStagingRecovery` zurück und verifiziert vor dem Wiederaufbau **null GPU-Mesh-Draws, 24 fehlende GPU-Mesh-Objekte** und keine wartenden alten Staging-Einträge. Anschließend werden alle 20 gültigen synthetischen GPU-Meshes und acht Texturen frisch geladen; die vier absichtlich unbereit gehaltenen Meshobjekte bleiben als echte Fallbacks sichtbar. Alle Frame-/Passsummen und GL-Fehlerprüfungen müssen bestehen. Das testet GPU-Regions-Lebenszyklus und Upload-Recovery, nicht die Netz-/Simulatorintegration.

**Block C – Cache-Kapazitätsmatrix für echte Texture-/Mesh-RAM-LRUs:** Neuer C++-CTest `tests/asset_cache_capacity_profiles_tests.cpp` verwendet die realen `TextureMemoryCache`-/`MeshMemoryCache`-Implementierungen mit 8, 64 und 128 zulässigen Asset-Slots. Über sechs deterministische Zugriffswellen mit 96 verschiedenen UUIDs werden 1152 Zugriffe je Familie und Profil durchgeführt, inklusive bevorzugt wiederholten Hotspots, Hit/Miss und LRU-Evictions sowie Kapazitätsgrenzen. Beispiel-Testlauf: 8 Slots 383 Hits/769 Misses/761 Evictions; 64 Slots 875/277/213; 128 Slots 1056/96/0 pro Assetfamilie. Diese Testwerte sind reproduzierbare synthetische Zugriffsmuster und kein kalibriertes Profil realer OpenSim-Assetgrößen/Hardwarelatzenzen. Für alle drei Größen sind triviale Fehlzustände, unkontrolliertes Wachstum und fehlende Wiederverwendung abgesichert.

**Block D – konkrete, datensparsame In-World-Fehlerdiagnose:** `tools/ogl_inworld_acceptance.py` erweitert das bisherige `ogl.inworld.acceptance.v2` um `failure_codes` und `next_check`. Aus den existierenden konkreten Health-Checks werden nur feste sichere Diagnosecodes erzeugt, beispielsweise `no_current_region_snapshots`, `insufficient_region_samples`, `texture_residency_incomplete`, `gpu_mesh_draws_missing`, `mesh_fallback_still_drawn`; niemals UUIDs, CAP-URLs oder Rohlogs. Der empfohlene nächste Prüfpunkt priorisiert fehlende Live-Regionensnapshots, veraltete/unterbrochene Serien, nichtleere Asset-Demandsets, niedrige Residency und echte Mesh-/Fallback-Draws. `evidence_pass` erhält die leere Failure-Liste. Python-Regressionen prüfen fehlende Daten, valide Serien, veraltete/unterbrochene Daten und kritische Missing-Mesh-Fälle. Im aktuellen echten Viewerlog: **0** gültige InWorld-Snapshots, `not_verified`, `failure_codes=['no_current_region_snapshots']`, CLI-Exitcode 2. Der fehlende Live-Test wird ausdrücklich nicht als Fehlerbehebung oder als bestanden ausgegeben.

**Gesamtprüfung:** Debug-C++23-CMake-Build, reale OpenGL-Regressionen und **70/70 CTests** im Entwicklungsstand bestanden. Nach der Versionspflege vollständiger Neuaufbau und GUI-Welcome-Smoke auf NVIDIA RTX 4070 SUPER vorgesehen; GitHub-Push ausschließlich nach bestandener Endprüfung auf `develop-current`.

**Nächste vier geplante Releaseblöcke Alpha.80:** Erweiterte Pixelreferenzen für Mehrfach-Overlays und AlphaSort, kontrollierter GPU-Rollback-/Uploadfehler-Regionswechsel, Cache-/Residency-Priorisierung nach messbarer Druckphase und tatsächliche In-World-Test-Protokollierung sobald erfolgreiche Authentifizierung/Regionsdaten vorliegen. Bekannte Avatar-/WASD-/Teleport-/CAPS-EventQueue-Probleme bleiben wie vereinbart bis zur späteren Integrations-/Bugfixphase zurückgestellt; `projectplan.md` bleibt maßgeblich.

CLI auf autorisiertem Linux-Desktop: `cmake --preset debug && cmake --build --preset debug -j2 && env DISPLAY=:0 ctest --preset debug --output-on-failure`; Live-Check: `python3 tools/ogl_inworld_acceptance.py "$HOME/.local/state/opengenesislink/viewer.log"`. Repository `marlin198323-glitch/nexverse-viewer`, Branch ausschließlich `develop-current`.

Nächster Chat: Repository `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (NICHT den divergenten `develop` übernehmen), lokaler Pfad `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Zuerst Git-Status sowie `projectplan.md` (maßgeblich), `info.md`, `CHATUEBERGABE.md`, `docs/ASSET_STREAMING.md`, `docs/MESH.md`, `docs/TEXTURES.md`, `docs/LOG_ANALYSIS.md` lesen. User will 3–4 größere Entwicklungsblöcke je `weiter`. Nach Implementierung `VERSION`, sämtliche Docs, vollständigen C++23-Debug-Build, alle CTests mit `DISPLAY=:0` und den Linux-GUI-Welcome-Smoke ausführen. Commit+Push nur auf `develop-current`, lokale SHA, Tracking-SHA und GitHub-SHA vergleichen, sauberes Worktree bestätigen. Die echten InWorld-Snapshots fehlen: keinesfalls Synthetic-Framebuffer-Test als erfolgreichen Simulatorbeitritt deklarieren.


## Alpha.80 – Firestorm-CAP-Vertrag, Experience-Read und Thumbnail-Handshake

**Block 1 – geprüfte Firestorm-Seed-CAP-Abdeckung:** `default_seed_capabilities()` fordert beim echten Login-Seed nun zusätzlich `FetchInventoryDescendents2`, `InventoryThumbnailUpload`, `FindExperienceByName`, `GetExperiences`, `AgentExperiences`, `ExperiencePreferences`, `GroupExperiences`, `GetAdminExperiences`, `GetCreatorExperiences`, `IsExperienceAdmin` und `UpdateExperience` an. Vorhandene `FetchInventory2` und `GetExperienceInfo` bleiben. Die Antwort bestimmt die tatsächliche Verfügbarkeit; nicht angebotene CAPS werden nicht simuliert. Der C++-Regressionstest kontrolliert einmalige Anforderung aller relevanten Namen.

**Block 2 – Server-capability-gesteuerte Firestorm-Bereitschaft:** Neue typisierte `FirestormCapsAvailability` mit acht klar definierten CAP-Kategorien (Inventaritem-Fetch, Inventarordner-Fetch, ThumbnailUpload, Owned/Allowed/Permissions/Admin/Update Experiences) berechnet rein aus der geprüften Seed-Antwort die angebotenen Funktionen, nie aus dem Vorhandensein von Beispiel-URLs oder dem Serverproduktnamen. Im produktiven Login-CAPS-Ladebildschirm erscheinen `Startregion: Outfit-Vorschau CAP/- | Experiences CAP/-`, optional die Anzahl eigener/erlaubter Experiences. `CAP` bedeutet *angebotener Endpunkt*, kein erfolgreicher Upload oder Firestorm-Funktionsparität. Der bisherige NexVerse-UI-Stil und die Simulation-Ready-Steuerung bleiben erhalten.

**Block 3 – Read-only Firestorm Experience-Listen, ohne EventQueue zu bremsen:** `parse_agent_experiences` akzeptiert strikt LLSD-Map mit begrenztem `experience_ids`-Array und unterscheidet `purchase`-Feld fehlend von `purchase: 0` (so verlangt es der aktuelle OpenGenesisLINK-Serververtrag für Firestorm). `parse_resident_experiences` validiert `experiences` und `blocked` getrennt. Die OGL-CAPS-Worker-Routine führt die beiden GETs nur bei tatsächlich angebotenen `AgentExperiences`/`GetExperiences` in einer separaten, beim Beenden gejointen Leseaufgabe aus, damit die EventQueue nicht durch diese optionalen Abfragen blockiert wird. Keine Experience-Purchase-/Allow-/Block-/Forget-Verwaltungsaktion erfolgt automatisch; Ergebnisse der ursprünglichen Login-Startregion werden nicht als aktuelle spätere Teleportregion behauptet. Es werden nur Anzahl und CAP-Verfügbarkeit angezeigt; keine Avatar-/Experience-UUID oder Secret-CAP-URL in neuen Diagnoseausgaben.

**Block 4 – Firestorm/OpenGenesisLINK Outfit-/Inventar-Thumbnail-Wire-Vertrag:** `build_thumbnail_begin_request(target,id)` erzeugt nur bei gültiger, nichtleerer Item-/Ordner-UUID das richtige LLSD-XML mit genau einem `item_id` oder `category_id`. `parse_thumbnail_stage` akzeptiert nur autorisierte Antwortformen `{state:upload,uploader:<HTTP(S)-URL>}` bzw. `{state:complete,new_asset:<UUID>}`. Ungültige UUIDs, Dateischemen und unbekannte Zustände werden verworfen. Dies entspricht dem aktuellen OpenGenesisLINK-Servermodul `OpenSim/Region/ClientStack/Linden/Caps/InventoryThumbnailUploadModule.cs`. Die Binär-JPEG2000-Übertragung, Besitz-/Rechteprüfung und endgültige OGL-Inventar-UI sind **nicht implementiert**; der Server selbst bindet One-Time-Uploader an aktive Owner-CAPS. Die serverseitig dokumentierte Firestorm-Outfit-Thumbnail-Cache-Limitation bleibt separat bestehen. `tests/firestorm_compat_tests.cpp` deckt Capability-Abgleich, Listenschema, Größenlimits, LLSD-Serialisierung und Upload-Handshake ab.

**Externe Codequellen, 11.10.2026:** Aktueller OpenGenesisLINK-Server `main` = `6dc73f2adfded7fe5ee94c992daaaba5be2b66c8`, Entwicklungszug 0.9.3.10 Dev / Stable 0.9.3.8, Commit vom 11.10.2026: Experience Manage API Keys können jetzt mit separatem `experiences:viewer:manage`-Scope ausgegeben werden. Dies ist **keine** Viewer-seitige Admin-Key-Nutzung: OGL darf niemals interne World-API-Secrets erhalten. Firestorm offizielle Referenzrelease-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` (Nightly `master` nicht als Release). Relevante Firestorm-Quelle `indra/newview/llfloaterexperiences.cpp` und OGLServer-Dokument `doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md` sowie `FIRESTORM_INVENTORY_THUMBNAIL_09310.md`. Server-/Client-SHAs vor jedem künftigen Featureblock neu vergleichen.

**Nachweis- und Einschränkungsstatus:** Erste vier Firestorm-Vertragsbausteine sind implementiert und lokal synthetisch getestet, aber kein vollständiger Firestorm-Fork und keine vollständige Experience-/Outfit-Benutzerfunktion. Die ursprüngliche reale In-World-Logserie enthält keine frischen Snapshotnachweise, Avatar/WASD/Mesh/EventQueue/Teleport bleiben noch zu integrieren. Weiterhin keine Behauptung einer erfolgreichen NexVerse-Live-Experience-/Inventar-Abnahme. Lizenzentscheidung für tatsächliche Firestorm-LGPL-Source-Übernahme bleibt offen; dieser Commit übernimmt keinen fremden Firestorm-Quellcode.

**Nächste 3–4 Blöcke Alpha.81:** Read-only Experience-Funktionen in die echte OGL-InWorld-Benutzeroberfläche mit regionengebundener Session-/CAP-Lease übernehmen; LLSD Inventar-/Outfit-Thumbnail-Metadaten inklusive Versionswechsel und Rechte abstrahieren; volle Firestorm-Menü-/Funktionsinventur und aktuelle Server-CAPS-Diffs weiter ausfüllen; anschließende kontrollierte Entwicklung von Upload/Experience-Consent mit echter Grid-Permission-/Owner-Abnahme. Firestorm-Parität hat Vorrang vor neuen Renderer-only-Erweiterungen.

Wiedereinstieg: `develop-current`, Repository `marlin198323-glitch/nexverse-viewer` auf Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`; zuerst `projectplan.md`, `docs/FIRESTORM_PARITY_MATRIX.md`, `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`, `info.md`, diese Datei lesen. Vor jedem Codeblock die aktuelle Server-`main`-HEAD-SHA sowie aktuellen offiziellen Firestorm-Releasebranch neu prüfen. Version/Docs/Test/GUI/Push wie bisher als einheitlichen Block abschließen. Der Server-Commit kann sich unabhängig vom OGL-Commit ändern; nie direkte Gleichheit erwarten. Keine API-Secrets speichern/loggen.


## 0.2.0-alpha.81 – Fortsetzung der Firestorm-Parität

Vier zusammengehörende Firestorm-Paritätsblöcke:

1. **Bedienbare Experience-Ansicht:** Nach authentifizierter und bereiter Simulation öffnet F8 einen Floater im NexVerse-Design. Eigene/Erlaubte/Blockierte Experiences lassen sich über Maus oder die Tasten 1–3 auswählen, ESC/F8 schließt. Die Liste enthält echte Simulator-UUID-Präfixe mit ausdrücklich noch fehlenden Anzeigenamen statt erfundener Namen. Fehlende CAPS, noch nicht geladene Daten und leere Listen werden getrennt angezeigt. Solange die Ansicht geöffnet ist, löst WASD keine Avatarbewegung aus. Schreiben, Experience erwerben, Allow/Block/Forget bleiben gesperrt. Der Viewport vor erfolgreichem Simulator-Eintritt bleibt frei von der Experience-UI.

2. **Region-scoped Experience-Leseergebnisse:** FirestormRegionLease bindet Regionhandle, WorldModel-Generation und Seed-Identität an einen threadsicheren Epochentoken. Bei Regionswechsel werden alte CAP-Advert-Flags, Counts und Experience-UUID-Listen gelöscht, der Floater geschlossen. In-flight AgentExperiences/GetExperiences-Antworten werden nach dem Wechsel auch bei gültiger HTTP-200-Antwort verworfen; Lease-Check erneut unter Caps-Mutex. Die Endpoints und UUIDs dürfen nicht in Statuslogs erscheinen. Der EventQueue-Hauptworker bleibt unabhängig. Neuer deterministischer asynchroner CTest belegt die späte GET-Ablehnung. Grenze: Neues Experience-CAPS-Seed-Discovery nach Teleport ist noch nicht umgesetzt, daher wird absichtlich keine alte Liste weiter angezeigt.

3. **Inventar-/Outfit-Metadatenvertrag:** Der MPL-2.0-Adapter parse_inventory_thumbnail_metadata liest die OpenGenesisLINK-LLSD thumbnail.asset_id aus FetchInventory2/FetchInventoryDescendents2, unterscheidet fehlendes Feld, ausdrücklich leere (Zero-UUID) Vorschau und gültige Thumbnail-UUID, lehnt defekte LLSD-Strukturen ab. InventoryFolderThumbnailCache speichert zu Folder-UUID die Version und Asset-ID, lehnt ältere Folderversionen ab und akzeptiert geänderte Thumbnail-IDs auch bei gleicher Version, um die dokumentierte Firestorm-7.2.4-Outfit-Gallery-Caching-Einschränkung nicht blind zu übernehmen. Neuer CTest belegt leere, alte, ersetzte und ungültige Metadaten. Grenze: noch kein echter Inventar-GET/Outfit-Gallery-UI, kein JPEG2000-Upload, keine reale Server-Readback-Abnahme.

4. **Konkrete Funktionsparität und Verträge:** Offizielle Firestorm-Releasequelle Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d; belegt über indra/newview/llfloaterexperiences.cpp (Tabs, GetExperiences/AgentExperiences, Acquire-Button). Aktueller OpenGenesisLINK Server main @ 6dc73f2adfded7fe5ee94c992daaaba5be2b66c8; Belege doc/NexVerse/FIRESTORM_EXPERIENCE_ACQUIRE_09310.md, doc/NexVerse/FIRESTORM_INVENTORY_THUMBNAIL_09310.md und serverseitige Thumbnail/Experience-Implementierung. Versionsgrenzen: OpenGenesisLINK 0.9.3.10 Dev und Firestorm 7.2.4.80712. Die Firestorm-Funktions-/Verhaltensbaseline bleibt verbindlich, der bestehende OGL-C++23-Viewer ist noch kein Firestorm-Code-Fork.

**Nachweise und Grenze:** 73/73 lokale CTests nach Integration, einschließlich zweier neuer Firestorm-Regressionen und realen GPU-Tests. Ein bestandener synthetischer Lease-/Metadatentest ist KEIN NexVerse-In-World-Test. Eine tatsächliche authentifizierte Regionsabnahme für diesen Stand fehlt weiterhin. Der F8-Floater wird nur im Simulationszustand gezeichnet; Login-Welcome-Smoke allein prüft keine Interaktion nach einem realen Simulatorbeitritt.

**Alpha.82-Priorität:** neue Regions-Experience-CAPS nach Teleport entdecken, Experience-Namen/Profile mit GetExperienceInfo lesen, servergebundene FetchInventory2/FetchInventoryDescendents2-Read-only-Outfit-Gallery aufbauen sowie Write/Consent/Upload nach separatem Owner-/Permissions-/Grid-Vertrag kontrolliert vorbereiten. Firestorm-Parität zuerst, Zusatzrenderer nachrangig.

Folgechat: Branch develop-current (alten divergenten develop nicht übernehmen), Workstation /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer. Vor JEDEM Release die aktuelle offizielle Firestorm-Release-SHA und OpenGenesisLINK Server-main-SHA neu prüfen. Maßgeblich projectplan.md und docs/FIRESTORM_PARITY_MATRIX.md. Beim Weiter: 3–4 größere Blöcke, VERSION/Docs, Build, alle CTests auf DISPLAY=:0, GUI-Smoke, Commit/Push, Local/Tracking/GitHub SHA gleich und Git-Worktree sauber. Keine Live-Erfolgsmeldung ohne aktive Simulatorsitzung.


## 0.2.0-alpha.82 – Handoff und nächste Firestorm-Blöcke

**Block A – dynamische Firestorm-CAPS nach Region-/Teleportwechsel:** Neue Protokollroutine fetch_firestorm_region entdeckt AgentExperiences, GetExperiences, GetExperienceInfo, FetchInventory2/FetchInventoryDescendents2 und InventoryThumbnailUpload anhand des echten aktuellen Simulator-Seeds. Die Routine holt autorisierte Experience-Listen ausschließlich lesend; CAP-Verfügbarkeit bedeutet keinen bestätigten Betrieb und keine Schreibberechtigung. In der App laufen Region-Read-Worker unabhängig vom bestehenden Login-CAPS/EventQueue-Longpoll, mit maximal vier ausstehenden Anfragen und nichtblockierender Future-Abholung; bei erschöpften Slots wird der aktuellste Regionsrequest nach Freigabe erneut begonnen. Ergebnisse werden nur für den noch gültigen FirestormRegionLease-Token unter Caps-Mutex übernommen. Alte UUIDs/Capabilities werden beim Regionswechsel zuerst verworfen. Wichtig: Die allgemeine Simulatorverbindung/EventQueue-Reconnect wird dadurch nicht automatisch repariert; die Verfügbarkeit einer neuen Region setzt eine vom WorldModel gelieferte gültige Seed-CAP voraus.

**Block B – echte Experience-Anzeigenamen über Firestorm GetExperienceInfo:** Authentifizierte read-only Profile werden optional vom pro Region angebotenen GetExperienceInfo-Endpoint über die Firestorm-Query /id/?page_size=12&public_id=UUID abgefragt. Nur tatsächliche IDs aus eigenen/erlaubten/blockierten Listen, maximal zwölf, werden angefragt. Der LLSD-Parser verlangt experience_keys[] mit public_id und name, validiert UUID, begrenzt Namenslänge auf 128 Zeichen und lehnt Steuerzeichen ab. F8 zeigt bei Erfolg den vom Simulator gelieferten Experience-Namen; bei fehlender/nicht unterstützter CAP bleibt die eindeutige Kennzeichnung ID XXXXXXXX (Name nicht geladen). Regionsepoche schützt auch Profilresultate, keine CAP-URLs, Experience-UUIDs oder Admin-API-Keys in neuen strukturierten Logs.

**Block C – lesende Outfit-Galerie aus echten OpenSim-Loginordnern:** F9 öffnet nach erfolgreichem Eintritt in eine Simulation ein Fenster im NexVerse-Design. Firestorms echte LLFolderType-Codes FT_MY_OUTFITS=48 und FT_OUTFIT=47 identifizieren aus der vom OpenSim-Login gelieferten inventory-skeleton die Outfit-Ordner. Die ersten zehn realen Ordnernamen werden aus maximal 256 akzeptierten UUID-validierten Einträgen angezeigt; ungültige und doppelte IDs werden ignoriert, Steuercodes in Titeln bereinigt. F8- und F9-Ansichten schließen einander aus, ESC/F9 schließt die Outfit-Ansicht, während offener Ansicht werden WASD-/Movement-Tasten nicht an den Agenten übergeben. Keine erfundene Vorschau oder Behauptung eines funktionierenden Wear/Rez. **Grenze:** Die Anzeige stammt derzeit aus dem Login-Skeleton; der aktive FetchInventory2/FetchInventoryDescendents2-Readback, Asset-Mesh-/JPEG2000-Vorschauen, COF-Verknüpfung, Outfits anziehen und Rechteprüfung bleiben offen.

**Block D – überprüfbarer Firestorm-/OpenGenesisLINK-Wire-Vertrag:** Der vollständige serverseitige Snapshot vom 11.10.2026 nutzt OpenGenesisLINK main fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b (neuer als alpha.81 6dc73f2). Offizielle Firestorm-Release-Referenz Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d. Implementierungsquellen: Firestorm indra/llmessage/llexperiencecache.cpp und .h (public_id, experience_keys, /id/), indra/llinventory/llfoldertype.h (46/47/48); OGL-Server NexVerse/RegionModules/Experiences/NexExperienceModule.cs mit GetExperienceInfo via variable path und getrennten Permission-/Manage-Keys. Der OGL-Viewer besitzt und benötigt keine zentralen World-API-Keys. Neuer nativer HTTP-Fixture-Test simuliert Seed->Listen->Profilaufrufe und verspäteten Region-Seed nach dem Wechsel, neuer Outfit-Gallery-Test validiert Firestorm-Ordner-Typen, Duplikate, Sanitizing und echte Login-Ordnerdaten. Die Tests beweisen kontrollierten OGL-Vertrag, KEINE authentifizierte Live-Regionsabnahme oder vollständigen Firestorm-Quellfork.

**Abnahmegrenzen:** Die neue F8-/F9-Bedienung ist nur während WorldModel.ready sichtbar; GUI-Welcome-Smoketest ohne echte NexVerse-Simulation belegt daher keinen Live-UI-Nachweis. Noch keine echte InWorldAssetSnapshot-Serie im bisherigen Log. EventQueue, Avatar, tatsächliche WASD-Bewegung, Upload, Wearables/COF, Marketplace/Groups/Voice und sonstige Firestorm-Funktionen bleiben gemäß verbindlicher Paritätsmatrix als noch nicht nachgewiesen/implementiert gekennzeichnet. Nach Versions-Neubau volle Tests und NVIDIA-OpenGL-Smoke durchführen.

**Nächste vier geplante Blöcke Alpha.83:** echten FetchInventory2-/FetchInventoryDescendents2-Readback der Outfitfolder mit serveradvertisierter CAPS-POST-Struktur, Thumbnail.asset_id und diskretem Cache-Vertrag implementieren; Outfit-Vorschauen aus Assetpipeline als Bilder anzeigen, keine leeren Platzhalter; Experience-Auswahl mit Profildetails (Name, Beschreibung, Berechtigungen) vervollständigen; EventQueue/Teleport-Region-CAPS- und Firestorm-Features mit echter autorisierter Simulator-Sitzung regressionsprüfen. Funktionen nur bei tatsächlichen Serverrechten aktivieren.

Nächster Start: Repo marlin198323-glitch/nexverse-viewer, Branch develop-current, autorisierte Workstation /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer. Vor JEDEM Release neue HEAD-SHA des OpenGenesisLINK-Server main und aktuellen Firestorm offiziellen Release-Branch prüfen; projectplan.md, docs/FIRESTORM_COMPATIBILITY_CONTRACT.md, docs/FIRESTORM_PARITY_MATRIX.md sind verbindlich. Bei weiter 3–4 größere Blöcke umsetzen, danach VERSION/Docs, Debug-Build, alle CTests mit DISPLAY=:0, Linux-Welcome-GUI-Smoketest, Commit/Push und Local/Tracking/GitHub-SHA vergleichen. Keine Live-Weltbeweise aus synthetischen Tests ableiten.


## 0.2.0-alpha.83 – Firestorm-Parität, Entwicklungsblöcke und Weiterentwicklung

**Block 1 – echter Firestorm-/OpenGenesisLINK-Inventarabruf per CAPS-POST:** `firestorm_inventory_fetch.hpp/.cpp` erstellt aus dem authentifizierten OpenSim-Login-Agenten und den ermittelten Outfit-Foldern ein LLSD-`FetchInventoryDescendents2`-POST mit `folders[]`, `folder_id`, `owner_id`, `sort_order`, `fetch_folders`, `fetch_items`. Bis 16 Outfits und ihre gültigen übergeordneten Folder werden doppelfrei abgefragt, weil nur die *Parent-Categories* die Thumbnail-Metadaten der Outfit-Ordner liefern. Antworten werden auf erlaubte Folder-/Parent-UUIDs, nichtnegative Versionen, max. 64 Folder-/1024-Category-Entries und den aktuellen FirestormRegionLease-Epochentoken begrenzt. Gelesen werden `categories[].thumbnail.asset_id` und versionsgebundene Foldermetadaten. Keine künstlichen Inventarordner, fremden Owner oder URL-Leaks. Der Zugriff bleibt ausschließlich lesend und nur über eine vom aktuellen Simulator beworbene Capability.

**Block 2 – echte Outfit-Vorschaubilder durch vorhandene GetTexture-/JPEG2000-/PNG-Pipeline:** Nach erfolgreichem, epochentreuem Inventar-Readback stellt die Produktions-App die vom Server gelieferten `thumbnail.asset_id` IDs und den tatsächlich beworbenen `GetTexture`-CAP bereit. Bis zu acht unterschiedliche, tatsächlich vorhandene Bilder werden in einer auf drei gleichzeitige Ladevorgänge begrenzten asynchronen Pipeline über den vorhandenen `assets::TextureAssetLoader::fetch_rgba` (PNG mit JPEG2000-Fallback) dekodiert und erst auf dem OpenGL-UI-Thread über `UiRenderer::create_rgba_texture` in sichtbare F9-Vorschau-Bilder umgewandelt. Auflösung für UI-Upload höchstens 1024×1024; Asset-ID und Regionen-Epoche werden unmittelbar vor GPU-Upload und beim Zeichnen kontrolliert. Beim Regionswechsel werden neue/alte Tasks, ID-Zuordnungen und GL-Textures getrennt behandelt; alte GL-Handles werden gelöscht, late replies verworfen. Fehler oder vom Grid nicht angebotene Caps lassen den ehrlichen Text `Vorschau -` stehen. Kein Bitmap wird erfunden und kein Firestorm-Thumbnail-Upload vorgetäuscht.

**Block 3 – Firestorm Experience-Profildetails und Benutzerselektion:** Die bestehende F8-Ansicht nutzt nun `ExperienceProfile` aus den vom Server über `GetExperienceInfo /id/?page_size=12&public_id=...` tatsächlich gelieferten LLSD-Objekten. Neben dem Namen werden Beschreibung (maximal 160 Zeichen zur Darstellung aus max. 4096), Firestorm-Maturity-Codes 13/21/42 und ein Aktivstatus aus Property-Bit 6 verarbeitet; alle Texte werden auf Länge und Steuerzeichen geprüft. Ein einzelner Experience-Eintrag ist im vorhandenen Viewer-Design per Maus oder Pfeiltasten markierbar; 1/2/3 wechseln zwischen eigenen/erlaubten/blockierten Listen. Zurückhaltender Fallback, wenn kein aktuelles Profil verfügbar ist. Keine automatische Zustimmung, kein Kauf, keine Write- oder Admin-API-Key-Nutzung.

**Block 4 – geprüfter Server-/Firestorm-Vertrag und Regressionen:** Offizieller Firestorm-Release-Branch `Firestorm_7.2.4` @ `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` mit `indra/llinventory/llfoldertype.h` (Outfits 47 / My Outfits 48) und `indra/llmessage/llexperiencecache.cpp` (GetExperienceInfo). Offizieller OpenGenesisLINK-Server `main` @ `fcac896e7e8b71412395ba508c5f4fcb8d9c2e1b`, insbesondere `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs`, `OpenSim/Region/ClientStack/Linden/Caps/FetchInventory2Module.cs` und `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Neuer CTest `firestorm_inventory_fetch_tests.cpp` simuliert Owner-/Folder-POST, echte Parent-Category-Thumbnail-LLSD, gesperrte Altregion-Requests und ungültige CAP-URLs. Erweiterter `firestorm_region_read_tests.cpp` testet Region-Seed -> Read-only Experience-Listen -> Profile mit Beschreibung/Maturity -> Folder-/Thumbnail-Response und Texture-CAP, ohne echte Avatar-Authentifizierung zu behaupten. Galerie-Skeleton-Test verwirft auch ungültige Parent-UUIDs. Debug C++23 und insgesamt 76 CTests nach finalem Versions-Neubau vollständig prüfen.

**Strikte Status-/Abnahmegrenze:** `tested_synthetic` für die protokollierten Antworten, Bilddekodierung und GL-Uploadpfade; **nicht** `live_verified`, weil aktuell keine authentifizierte NexVerse-Simulator-/Avatar-/Outfit-Sitzung vorliegt. Kein vollständiger Firestorm-Code-Fork, nur testbare eigene Adapter entsprechend Firestorm-Featurebasis. Die bestehende allgemeine Teleport-/EventQueue- und Avatar-/WASD-Integration ist weiterhin offen. F8/F9 sind nur nach `simulation_ready` sichtbar; Welcome-GUI-Smoketest belegt die neue UI nicht. Vollständiges COF-/Wearables-/Outfitwechsel-Verhalten, Folder-Events und Berechtigungen fehlen. Die verbesserte Cache-Metadatenroutine darf nicht als Nachweis ausgegebener Thumbnail-Bilder auf einem echten Grid missverstanden werden.

**Nächste 3–4 Blöcke Alpha.84:** kontinuierliche Firestorm-Inventarordner-Synchronisierung und `FetchInventory2`-Items samt COF-Lesezustand (nicht nur Login-Skeleton); bedienbare Outfit-Details/Tragen mit Rechte- und Permission-Checks gegen Serverprotokoll; Experience-Berechtigungsdialog mit expliziter Nutzeraktion und aktuellem Simulator-Lease; danach eine echte NexVerse-In-World-Testabnahme inklusive Avatar, Bewegung, Regionswechsel und den noch offenen EventQueue-/Mesh-Problemen. Vollständige Firestorm-Parität bleibt vorrangig.

Nächster Chat: GitHub `marlin198323-glitch/nexverse-viewer`, Branch **develop-current** (nicht divergenten `develop` verwenden), Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Vor jedem neuen Block aktuelle OpenGenesisLINK-Server-main-SHA und offiziellen Firestorm-Release prüfen. Maßgeblich `projectplan.md`, `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`, `docs/FIRESTORM_PARITY_MATRIX.md` und diese Datei. Bei `weiter` mindestens 3–4 zusammenhängende, tatsächlich implementierte Blöcke bevorzugen. Debug-C++23-Build, alle CTests mit `DISPLAY=:0`, Linux-Welcome-GUI-Smoke, Commit/Push nach `develop-current` und Local/Tracking/GitHub-SHA vergleichen. Kein fehlender Live-Avatar-/Outfit-Test wird als bestanden dargestellt.


## 0.2.0-alpha.84 – Firestorm-Parität und reproduzierbare Fortsetzung

**Block 1 – Firestorm COF und wirkliche Outfit-Inventargegenstände (lesend):** Der `FetchInventoryDescendents2`-Reader liest jetzt neben den Parent-Thumbnail-`categories` die im Simulatorantwort-`items[]` enthaltenen Inventargegenstände pro Outfit-Folder. UUID, Asset-ID, Name, Asset-/Inventory-Type werden als typisierte `OutfitInventoryItem`-Einträge nur aus gültigen und begrenzten LLSD-Maps übernommen (max. 512 Antwortitems, höchstens 256 verwendete pro Ordner, Namen max. 128 Zeichen mit Entfernung von Steuerzeichen). Der separate Current Outfit Folder (COF, Firestorm Typ 46) wird aus der authentifizierten OpenSim-Login-Skeleton ermittelt und mit dem gleichen Owner-gebundenen CAPS-POST abgefragt, auch wenn keine gespeicherten Outfits vorhanden sind. Der Owner der Serverantwort wird kanonisch als UUID-Bytes gegen den eingeloggten Agenten geprüft; fremde/ältere Antworten dürfen keine Anzeigedaten ersetzen. **Keine Wearables-/COF-Mutation**, kein unberechtigter Asset-Upload.

**Block 2 – versionsgebundene, inkrementelle Inventarsynchronisierung:** Neue `protocol::FirestormInventorySync` hält Folderversion, Items und Thumbnail-Asset-IDs ausschließlich innerhalb der Regionsepoche und verwirft alte Versionen, unvollständige/fehlende Thumbnail-Maps sowie verspätete Regionsantworten. Ein gleichversionierter, aber wirklich geänderter Thumbnail-Asset-ID-Link aktualisiert die Galerie. Solange F9 geöffnet ist, prüft die Anwendung im 30-Sekunden-Abstand mit einem **maximal einen** Hintergrund-Future den bereits beworbenen `FetchInventoryDescendents2`-CAP erneut. Render-/EventQueue-Threads warten nicht auf diese Requests. Die alte GL-Vorschau wird nur bei echter Asset-ID-Änderung freigegeben, danach wieder geladen. Neuer deterministischer `firestorm_inventory_sync_tests.cpp` prüft Version-Ordering, unveränderte/ersetzte Thumbnails, ignorierte Legacy-Omissions und Region-Epoch-Reset. **Keine echte EventQueue-basierte Folder-Notifikation**, Polling während sichtbarer Galerie ist ein sicherer Zwischenstand.

**Block 3 – bedienbare F9-Outfit-Details inklusive COF-Readback:** Im bestehenden NexVerse-Look lässt sich einer der ersten zehn in der Liste sichtbaren Outfit-Folder per Maus oder Auf-/Ab-Taste auswählen. Ein rechtes Detailfeld zeigt dessen tatsächlich vom Server gelesene Gegenstandsnamen (bis sechs), Anzahl und den separaten aktuellen COF-Linkstand, sofern dieser geladen wurde. Fehlt die Owner-CAP oder ein aktueller Abruf, steht ausdrücklich „Inhalt noch nicht geladen“. Existing PNG/JPEG2000-GetTexture-Thumbnail-Darstellung bleibt aktiver Bildpfad; fehlende Bilder bleiben unbestätigt statt gefälscht. **Outfit anziehen, BOM, Wearables, Animationen und synchronisierte Avatar-Appearance sind noch offen**, solange ihre kompletten Firestorm-LLUDP-/CAPS-Rechte und eine echte NexVerse-Simulation fehlen.

**Block 4 – Firestorm ExperiencePermissions mit ausdrücklicher Nutzereinwilligung:** Der neue `ExperiencePermissionConsent`-Vertrag ermöglicht im F8-Fenster bei vorhandener, aktueller `ExperiencePreferences`-CAP genau `A` (= Erlauben vorbereiten) oder `B` (= Blockieren vorbereiten), gefolgt von `Enter` zur **separaten Bestätigung**. `Escape`, Regionswechsel oder veraltete Epochs verwerfen die Vorbereitung. Erst nach Bestätigung startet eine asynchrone, owner-/Region-Lease-gebundene `HTTP PUT`-Operation mit offizieller LLSD-Map `{experience_uuid:{permission:Allow|Block}}`. OpenGenesisLINK liefert die bestätigten `experiences`/`blocked`-Listen; nur wenn die gewählte ID genau im Zielset und nicht im Gegenset steht, zeigt OGL eine erfolgreiche Änderung an. Die generische HTTP-Abstraktion besitzt dafür eine fail-closed PUT-Methode; libcurl-Weiterleitungen für mutierende PUT-Requests sind gesperrt. Die anderen Firestorm-Aktionen `Forget`/DELETE, Purchase, Experience-Editor und Adminrechte bleiben **nicht freigeschaltet**. Neuer Test prüft weder versehentliche Writes noch falsche Erfolgsbestätigungen: ohne explizite Enter-Bestätigung null PUT-Requests, epoch-bound stale reject, negative Serverantwort, erlauben/blockieren.

**Server-/Firestorm-Quellenprüfung (11.10.2026):** Offizielle Firestorm-Releasequelle `FirestormViewer/phoenix-firestorm` Branch `Firestorm_7.2.4` bei `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d`. OpenGenesisLINK Server `main` nun **`efc9b3a73ce04f6f57a1958c29ec0a439ed76d0f`** (neuer als alpha.83 `fcac896e`): Source `OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs` mit `items`, `categories.thumbnail.asset_id`, `owner_id`, `version`; `NexVerse/RegionModules/Experiences/NexExperienceModule.cs` mit leasegebundenem `ExperiencePreferences` GET/PUT/DELETE, nur Avatar-/CAP-gebundene Berechtigungen. Server-HEAD entfernt OGL-Pathfinding aus 0.9.3.10-Dev-Scope: Firestorm-Pathfinding bleibt gewünschte Baseline, aber kann auf diesem Server derzeit **nicht als kompatible NavMesh-Funktion** behauptet werden. Neues Feature nur bei Server-Vertrag und später mit tatsächlichen Rights-/Live-Checks.

**Evidenz und Grenzen:** Versionsbuild und sämtliche 78 lokalen CTests inklusive realer GPU-Tests sowie Linux-GUI-Smoke sind vor GitHub-Release erneut durchzuführen. `tested_synthetic` beschreibt bestätigten OGL-Wire-Code und reproduzierbare Tests, **nicht** erfolgreiche Live-Outfit-Wear, Consent, Thumbnail-Erneuerung oder In-World-Avatar-/EventQueue-Funktion. Ungeprüfte bzw. fehlende Firestorm-Bereiche bleiben in der Matrix ausdrücklich offen. UI-Nachweise aus dem Welcome-Ladebildschirm belegen F8/F9 erst in einer echten `simulation_ready`-Sitzung **nicht**.

**Nächste 3–4 Blöcke Alpha.85:** COF-/Outfit-Wear- und Appearance-/Attachment-Protokoll gegen Firestorm-Source und OpenGenesisLINK-Rechte nachbauen; Firestorm-Inventar-Baum samt On-Demand FetchInventory2 und Ordner-Event-Diffs ausbauen, danach optionales Permissions-Forget mit Bestätigung und Lease; fehlende EventQueue-/Avatar-/Mesh-/WASD-Integration gegen authentifizierte NexVerse-Grid-Sitzung live prüfen. Der vollständige Firestorm-Funktionskatalog bleibt verpflichtend.

**Nächster Chat und GitHub:** `marlin198323-glitch/nexverse-viewer`, Branch **`develop-current`**, Workstation `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Bindend `projectplan.md`, `docs/FIRESTORM_PARITY_MATRIX.md`, `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`. Vor jedem Release aktuellen OpenGenesisLINK-Server-main-HEAD und offiziellen Firestorm-Releasebranch neu verifizieren, nicht auf SHA `efc9b3a` als automatisch aktuell vertrauen. 3–4 Featureblöcke umsetzen, `VERSION`/Docs, Debug-C++23-Build, alle CTests `DISPLAY=:0`, Linux-GUI-Smoke, Commit+Push und drei SHA-Werte vergleichen; keine Live-Beweise aus synthetischen Tests ableiten.


## 0.2.0-alpha.85 – Firestorm-Stand und Fortsetzungsanweisung

**Block 1 – Firestorm-ähnlicher Inventarbaum F10 mit echter Ordnernavigation:** Die neue lesende Inventaransicht `F10` zeigt Ordner aus `LoginSession.inventory_skeleton` sowie live nachgeladene `FetchInventoryDescendents2`-Kategorien (Name, Typ, Parent-UUID, Version). Auf-/Ab-Pfeile und Mausklick markieren Ordner, Enter/Rechts öffnen und Links/Backspace gehen zurück; `F10`/ESC schließen. Die Fenster F8/F9/F10 schließen einander aus, Movement-Tasten werden bei geöffnetem Inventar blockiert. Der generische Fetch-Adapter fragt im Hintergrund jeweils genau einen ausgewählten Ordner mittels gültigem, regionengebundenem Simulator-CAP ab; bis zu 256 abgefragte Ordner und 4096 lokal modellierte Folder verhindern ungebremste Rekursion. `owner_id`, `folder_id`, Parent-Zuordnung, LLSD-Typen/Versionsnummer und aktuelle Regionsepoche werden vor Übernahme geprüft. Auch ungültige/fehlende Capabilities führen nicht zu HTTP-Dauerschleifen. Unterordner außerhalb des Login-Skeleton erscheinen erst nach erfolgreichem Simulator-Fetch, nicht aus erfundenen Daten.

**Block 2 – echter `FetchInventory2`-Einzelitem-Abruf für Outfits:** `firestorm_inventory_items.hpp/.cpp` implementiert die OpenGenesisLINK-konforme `POST`-Anfrage `items:[{item_id,owner_id}]` an die vom aktuellen CAPS-Seed beworbene `FetchInventory2`-URL, gebündelt auf maximal 32 IDs aus der aktuell angezeigten F9-Outfitliste. Die Antwort `agent_id` und `items[].permissions.owner_id` müssen exakt zum angemeldeten Avatar gehören; unbekannte IDs und ungültige UUIDs/LLSD-Typen werden herausgefiltert. Lesen und Sanitizing der Itemnamen, Asset-IDs und Typen sind getestet. Die F9-Auswahl startet einen asynchronen On-Demand-Fetch; dessen Ergebnis bleibt mit Folder und Regionsepoche verknüpft. Ein Fetch-Ergebnis eines zuvor gewählten Ordners darf dem neuen nicht als aufgelöste Referenz untergeschoben werden. Die volle Avatar-/Wearables-Appearance-Pipeline ist noch nicht vorhanden.

**Block 3 – COF-/Outfit-Wear-Vorprüfung statt fehlerhaftem Blind-Wear:** Nach Vergleich mit Firestorm `indra/newview/llappearancemgr.cpp` überprüft `firestorm_outfit_wear_preflight` vorhandene Outfit-Items, Current Outfit Folder (COF, LLFolderType=46), referenzierte LLAssetType-24-Links und unverstandene Folder-Links (Typ 25). F9 zeigt einen expliziten Status für noch nicht geladene Inventare/COF, leere Outfit-Folder, nicht aufgelöste Linkziele und noch fehlende Wearable-/Attachment-/Avatar-Appearance-Synchronisation. `can_wear()` bleibt **absichtlich false**: Firestorm-Wear benötigt COF-Linktransaktionen, tatsächliche Wearable-Assets, Attachment-Regeln und Appearance-Abgleich. Der Server- und Firestorm-Quellvergleich erlaubt keinen erfundenen einzelnen `Wear`-CAP; die vollständige Outfit-Aktion bleibt offen. Ein User-Klick darf keinen falschen Erfolg anzeigen.

**Block 4 – explizit bestätigtes Firestorm Experience-Forget:** `F8` unterstützt bei vorhandener aktueller `ExperiencePreferences`-CAP nun zusätzlich `X` zur Vorbereitung von „Vergessen“, gefolgt von **Enter** zur ausdrücklichen Bestätigung (ESC Abbruch). Der Connector nutzt den OpenGenesisLINK-Vertrag `DELETE` auf `<current-capability>?<UUID>`, gegen ungültige IDs und abgelaufene Regionsepochen geschützt. `IHttpTransport::remove()` ist standardmäßig fail-closed, `CurlHttpTransport` unterbindet Redirects für mutierende DELETE-Requests. Erst wenn die Serverantwort die betroffene ID weder in `experiences` noch in `blocked` enthält, meldet der Viewer einen bestätigten Erfolg. Vorbereitete Aktionen werden auch beim Schließen/Wechseln zwischen F8/F9/F10 verworfen. Tests beweisen keine DELETE ohne gesondertes Enter, korrekten HTTP-Queryvertrag, Servernegativantwort- und Stale-Lease-Schutz. Experience-Erwerb, Rollen-/Admin-Bearbeitung und beliebige Write-CAPs bleiben gesperrt.

**Quelldisziplin, 11.10.2026:** Offizielle Firestorm-Basis `FirestormViewer/phoenix-firestorm`, Branch `Firestorm_7.2.4`, HEAD `10bd3c9f930c76e1427ddd4ecece6cdf36b4406d` einschließlich `indra/newview/llappearancemgr.cpp`, `indra/llinventory/llfoldertype.h`, Experience-Floater. Aktueller OpenGenesisLINK-Server `main`, HEAD **`885fa72a10fec770d6dc1220f08766437159a084`** – neuer als Alpha.84 (`efc9b3a`); darin u. a. ExperienceInfo-ID-Fix (`2194527`) und ein interaktives Firestorm-LSL-E2E-Testskript (`885fa72`). Für Inventory/COF sind `OpenSim/Framework/InventoryItemBase.cs` und `OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs` maßgeblich (inkl. `item_id`, `permissions.owner_id`, `agent_id`), für Experience-Forget `NexVerse/RegionModules/Experiences/NexExperienceModule.cs`. Vor weiteren Codeänderungen immer aktuellen Server-HEAD erneut prüfen; eine lokal vorhandene Testdatei beim Server beweist KEINE vom Viewer vollzogene Live-Abnahme.

**Tests und Grenzen:** Für Alpha.85 wurden neue CTests `firestorm-inventory-items-tests` und `firestorm-inventory-tree-wear-tests` integriert sowie Forget-/COF-/F10-Owner-/Region-Regressionen erweitert. Insgesamt 80 CTests geplant und im finalen Debug-C++23-Versions-Neubau mit echten GPU-Tests zu prüfen. Ein synthetischer HTTP-Fixture-Test ist `tested_synthetic`, nicht `live_verified`. Der GUI-Welcome-Smoke verifiziert nicht die post-login F8/F9/F10-Fenster; echtes NexVerse-InWorld-Testing, Avatar, Bewegung, EventQueue/Teleport, Meshes und Outfit-Wear bleiben als nicht abgenommen markiert.

**Nächste 3–4 Blöcke Alpha.86:** Echte Firestorm-COF-/Wearable-Asset- und Attachment-Transaktion anhand `llappearancemgr.cpp` und OpenGenesisLINK-Serverrechten entwerfen/implementieren und mit Live-Avatar-Readback absichern; den F10-Baum um gezielten Item-Readback und Folder-EventQueue-Änderungen ergänzen; Experience-Consent-/Avatar-/Permission-Dialoge gegen aktuellen Simulator robuster machen; authentifizierte In-World-Abnahme (Avatar, WASD, Mesh und EventQueue) mit realem Grid-Zugang priorisieren. Volle Firestorm-Funktionsparität bleibt die verbindliche Baseline.

**Repo/Workstation:** `marlin198323-glitch/nexverse-viewer`, Branch `develop-current` (alten divergenten `develop` **nicht** einsetzen), `/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer`. Source-Baseline `projectplan.md`, `docs/FIRESTORM_PARITY_MATRIX.md`, `docs/FIRESTORM_COMPATIBILITY_CONTRACT.md`, offizielle Firestorm-Releasequelle und aktuelle OGL-Server-HEAD. Bei `weiter`: mindestens drei bis vier **tatsächlich implementierte** zusammenhängende Blöcke, VERSION/Docs, alle CTests unter `DISPLAY=:0` inkl. realen GPU-Tests, GUI-Smoke, Git-Commit+Push und drei identische SHAs prüfen. Keinen synthetischen Test als echte Live-Simulatorabnahme darstellen.


## 0.2.0-alpha.86 – Firestorm F10 Inventar, Items, Detailabruf · 11.10.2026

Block 1 – Lesender F10-Folder-Readback: FetchInventoryDescendents2 sendet
fetch_folders=true und fetch_items=true an die aktuelle vom Simulator
beworbene CAP. Antwort: authentifizierte owner_id, angefragte folder_id,
version >= 0, items[]; pro Item UUID, parent_id des aktiven Folders,
permissions.owner_id des angemeldeten Agenten, gültige Typen und bereinigte
Namen. Maximal 512 zurückgegebene / 256 dargestellte Einträge je Folder.
Keine serverseitige Wear- oder Inventory-Mutation.

Block 2 – versionsgebundene Sync und Lease: InventoryFolderContents hält
Items und Server-Folderversion zusammen. Older-version Responses werden
verworfen; neue leere Arrays können Einträge entfernen. Region-Lease
schützt gegen alte HTTP-Resultate; Regionswechsel invalidiert alle
Folder-Inhalte, Pollzeiten und Detailansichten. Maximal ein asynchroner
F10-Folder-Fetch gleichzeitig; 30-Sekunden-Polling für sichtbaren Folder,
R führt einen manuellen Refresh aus. Der InventoryTree bleibt auf 4096
Folder begrenzt; bestehende Nodes werden beim Limit noch aktualisiert.

Block 3 – F10 UI und Einzelitem: links reale Unterordner, rechts echte
Itemnamen aus dem CAPS-Readback, keine erfundenen Gegenstände. J/K und
Mausklick wählen Item; I fordert via aktueller ownergebundener
FetchInventory2-CAP nur die ausgewählte UUID an. Ergebnis passt
ausschließlich zur aktuellen Selection, Folder-ID und Regionsepoche.
F10 bleibt read-only; Auf/Ab/Enter navigieren den Ordnerbaum.

Block 4 – nachprüfbarer Protokollvertrag: Servercode
OpenSim/Capabilities/Handlers/FetchInventory/FetchInvDescHandler.cs,
OpenSim/Framework/InventoryItemBase.cs::ToLLSDxml und
OpenSim/Capabilities/Handlers/FetchInventory/FetchInventory2Handler.cs
zeigen die erforderlichen item_id, parent_id, permissions.owner_id,
version und item-/folder-Strukturen. Offizieller Server main
885fa72a10fec770d6dc1220f08766437159a084 und Firestorm Releasebranch
Firestorm_7.2.4 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d
vor diesem Entwicklungsblock verifiziert. Deterministische
Regressionen prüfen Owner-/Parent-Spoofing, versionsgebundene Updates,
empty Items, Fetch-Requestvertrag und Region-Invalidierung.

WICHTIG: Build/CTest/OpenGL-Welcome-Smoke werden lokal durchgeführt.
Sie ergeben nur tested_synthetic, keine bestätigte Live-Anmeldung und
keinen Firestorm-Vollfork. F10-Readback, Outfit-Wear, Avatar, WASD,
Teleport, Meshes, EventQueue und COF-Transaktionen müssen am
authentifizierten NexVerse-Grid weiter geprüft werden.

Nächste Blöcke: Inventar-Folder-Event-Diffs nach vom aktuellen Server
tatsächlich ausgesendetem CAPS/UDP-Vertrag; Firestorm COF/Wearables/
Attachment-/Appearance-Rechtemodell und Lifecycle; interaktive InWorld-
Abnahme inkl. Avatar, Bewegung, Mesh und EventQueue. Projektplan und
Firestorm-Funktionsbaseline bleiben verbindlich.
Branch: develop-current, Repo: marlin198323-glitch/nexverse-viewer.


## 0.2.0-alpha.87 – Firestorm/OpenSim Inventory EventQueue und Rechte · 11.10.2026

**Block 1 – echter Server-EventQueue-Vertrag:** Die OpenGenesisLINK
EventQueueGetHandlers.cs sendet BulkUpdateInventory mit AgentData.AgentID,
ItemData.ItemID/FolderID/OwnerID und FolderData.FolderID/ParentID.
Null-UUID-Folderdaten sind beim Item-Event Dummy-Records, kein
Inventarordner. Der neue Parser firestorm_inventory_events.cpp prüft
den authentifizierten Agent, einzelne Itembesitzer, UUID-Formate sowie
Arraygrenzen bis 512. Ungültige/fremde Daten werden nicht übernommen.

**Block 2 – Thread-/Regionsgrenzen:** Eine mutexgeschützte
InventoryEventInbox übergibt bis zu 256 eindeutige Folder-IDs vom
EventQueue-Netzwerkthread an den OpenGL-Main-Thread. Bei Overflow
wird ein begrenzter vollständiger erneuter read-only Abruf markiert.
Alte Regionsepochen werden abgelehnt. Ein veralteter Login-
EventQueue-Poll wird nach Teleport/Crossing nicht weiter betrieben.
Die vollständige neue LLUDP-/CAPS-EventQueue-Anmeldung ist noch offen.

**Block 3 – F9/F10 inkrementelle Aktualisierung:** Echte Eventhints
invalidieren betroffene F10-Folder und markieren diese für einen
erneuten CAPS-Request. Eine Invalidierungsrevision schützt die
asynchronen F10-Antworten vor einem neueren Ereignis. Für F9 werden
veraltete FetchInventoryDescendents2-Antworten ebenso abgelehnt;
eine begrenzte read-only Refresh-Anfrage folgt einem Ereignis
frühestens zwei Sekunden nach der letzten Abfrage. Kein Event wird
direkt als Inventardaten oder ein erfolgreicher Wear-Vorgang benutzt.

**Block 4 – F10 Item-Folder- und Auswahlprüfung:** FetchInventory2
validiert optional die parent_id eines im ausgewählten F10-Folder
angefragten Items zusätzlich zu Besitzer und Region. Eine ungültige
erwartete Folder-UUID blockiert bereits vor HTTP. Verschobene Items
oder verspätete Ergebnisse dürfen nicht als aktuelle Itemdetails
unter anderem ausgewähltem Item erscheinen. F9 bleibt ownergebunden.

Die neuen Negativtests pruefen synthetische EventQueue-Responses mit
Fremd-Owner, falschen Foldern, Dummy-Foldern, falschem Parent, zu
grossen Arrays, Regionsepoche und Overflow. Debug C++23 plus nun
81 CTests sind vor Release komplett erneut auszuführen. Ein
bestandenes CTest ist nur tested_synthetic, nicht live_verified.

Geprüfte Codequelle OpenGenesisLINK main:
ed37b9db720924c1d7105ea499a8c96430e9b7ea.
Gegenüber Alpha.86 nur ein Economy-/Hypergrid-Commit; Inventory-
Protokoll unverändert. Firestorm offizieller Releasebranch
Firestorm_7.2.4:
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
OGL ist weiterhin ein eigenständiger C++23-Viewer, kein vollständiger
Firestorm-Quellcodefork. Firestorm-Funktionsparität ist verbindlich.

Nächste Blöcke: Regionswechsel einschließlich neuem Simulator-
Circuit und CAPS/EventQueue-Lifecycle; authentifizierte Avatar-,
WASD- und Mesh-InWorld-Abnahme; sonstige Inventory-LLUDP-Änderungs-
und Löschpfade; COF-/Wearables-/Attachment-Protokoll mit tatsächlichen
Serverrechten und einem Avatar-Appearance-Readback. Projektplan
und Firestorm-Paritätsmatrix bleiben verbindlich.
GitHub Branch: develop-current (nicht develop).


## 0.2.0-alpha.88 – Region-Handoff, LLUDP/CAPS EventQueue · 11.10.2026

**Block 1 – Firestorm/OpenSim Region LLUDP handoff:** Die vorher nur mit
der Loginregion verbundene CircuitConnection wird beim Wechsel von
simulator_ip/port aus einem geprüften TeleportFinish-/CrossedRegion-
WorldModel-Ziel gestoppt. Nur nach Abschluss des bisherigen
Circuit-Futures werden der gemeinsame Outbound-Command-Queue und
lokale Movement-/Avatar-Identitäten zurückgesetzt und eine neue
CircuitConnection zum Ziel mit unverändert authentifizierter
AgentID/SessionID/CircuitCode gestartet. Neue Verbindung führt
UseCircuitCode, CompleteAgentMovement und RegionHandshake erneut aus.
Dabei läuft absichtlich nur EIN LLUDP-Worker zur gleichen Zeit;
Routen mit ungültigem Port/IP bleiben gesperrt. Ohne bestätigtes
AgentMovementComplete bleibt die Welt nicht SIMULATION READY.

**Block 2 – neue, regiongebundene EventQueueGet:** Der vorherige
Login-EventQueue-Worker beendet sein Polling, sobald WorldModel-
Regionhandle/Seed oder die FirestormRegionLease nicht mehr passen.
region_event_queue.cpp entdeckt den vom ZIELSIMULATOR über dessen
Seed-CAP angebotenen EventQueueGet-Endpunkt erst nach aktiver
passender LLUDP-Verbindung. Der Worker führt OpenSim-Long-Polling mit
separater ack-Sequenz, HTTP-502-Idle-Behandlung, Retry-/404-Seed-
Wiederabfrage und Ack-Reset nur bei neuer Endpunkt-URL aus. Nur
aktuelle Epoch-Events gehen an CapsEventDispatcher und die
ownergebundene Inventar-Inbox. In-flight alte Polls laufen aus und
werden ohne UI-Thread-Join am Frame-Ende entsorgt. Maximal vier
neue EQ-Futures gleichzeitig; beim Shutdown gejoint.

**Block 3 – Schutz gegen alte Regionen-Pakete und HTTP-Rückläufer:**
Der UDP-Paketcallback prüft nun vor jedem WorldModel-Dispatch die
aktuelle WorldModel-simulator_ip/port gegen die tatsächlich benutzte
CircuitConnection sowie das Stop-Flag. Stale UDP-Daten aus der
alten Region verändern somit nach Teleport nicht die aktuelle Welt.
Der Login-CAPS-Worker verwirft nach Regionswechsel alte Pollergebnisse,
einschließlich späte EnvironmentSettings/ExtEnvironment-Änderungen.
Bei LLUDP-Neustart werden Avatar-LocalID/RegionHandle unter
WorldModel-Mutex und Movement-Controller zurückgesetzt; alte
Outbound-Befehle werden nicht an den neuen Simulator gesendet.

**Block 4 – reproduzierbare Ablauf-/Protokolltests:** Neuer
RegionCircuitHandoff-Vertrag mit Aktionen unchanged, wait, launch,
invalid_destination sowie source-gebundener Packet-Guard.
Neue CTests region-circuit-handoff-tests und
region-event-queue-tests mit einer synthetischen CAPS-HTTP-Fixture
prüfen SeedCAP, ack, Duplicate-Batch, HTTP404-CAP-Refresh mit
eigenständigem ack, sowie Epoch-/Stop-Invalidierung. Zusätzlich
werden sämtliche bisherigen CTests samt realen GPU-Tests bei
DISPLAY=:0 und ein GUI-Welcome-Smoke durchgeführt.

Quellreferenzen am 11.10.2026 neu verglichen:
OpenGenesisLINK Server main:
f9ee61580a1b6aaf1d420b545ffce883b3dade0c,
seit Alpha.87 nur Wirtschaft/Hypergrid-Bank-Payout, keine
Änderung an Region-Caps/EventQueueGetHandlers.cs;
offizieller Firestorm Releasebranch Firestorm_7.2.4:
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.

**Grenzen:** Der neue mehrregionale UDP/CAPS-Lifecycle ist
tested_synthetic und gebaut; ein realer Teleport zu einem
zweiten NexVerse-Simulator, aktueller Avatar, WASD, Mesh-Skinning,
Inventar-Mutationen und COF-Wear sind weiterhin not_live_verified.
Firestorm bleibt vollständige Funktionsbaseline; OGL nutzt bislang
eigene C++23-Module und ist noch kein Firestorm-Codefork.
Eine EventQueue-Bestätigung allein ist noch keine Authentifizierung
einer vollständigen Avatar-Appearance.

**Nächste Blöcke:** Live-Login/Teleport und Avatar-Render/Movement
auf dem laufenden NexVerse/OpenGenesisLINK-Grid abnehmen; LLUDP-
Inventar-RemoveInventoryItem/RemoveInventoryFolder als serverseitige
Readback-Invalidierung anbinden; COF-/Wearables-/Attachments und
Meshes/Skeleton bis zum echten Avatar-Live-Readback vervollständigen;
anschließend weitere Firestorm-Parität nach projectplan.md.
Branch develop-current, OGL-Repository marlin198323-glitch/nexverse-viewer.


## 0.2.0-alpha.89 – echtes AgentUpdate-Quaternion, WASD/Avatar-Abnahme und Asset-Readback · 11.10.2026

**Block 1 – entscheidender OpenSim-WASD-Protokollfix:** Die aktuelle,
live angemeldete Alpha.88 hatte korrekt erkannte ControlFlags 1/2,
aber neun identische Simulator-Positionsantworten auf
(151.568924,193.071442,22.558577). Ursache im Viewer-Quellcode:
encode_agent_update serialisierte BodyRotation UND HeadRotation mit
je 16 Byte statt dem Linden-Message-Template-Typ LLQuaternion mit
je 12 Byte. Dadurch standen State, Camera-Vektoren und ControlFlags
auf dem Wire um zusammen acht Bytes versetzt; OpenSim konnte den
Tastenbits nicht das beabsichtigte ControlFlags-Feld zuordnen.
AgentUpdate High 4 hat jetzt genau 114 Byte Nutzdaten; ControlFlags
LE-U32 liegt bei Offset 109, Flags beim letzten Byte. Ein
bytegenauer Regressionstest prüft die Zielbytes und verhindet einen
künftigen stillen Offset-Regression. Die lokale Tastenerkennung
(WASD, Pfeiltasten, Shift, F) bleibt unverändert. Ob OpenSim danach
wirklich Positionsänderungen meldet, ist ohne eine frische
authentifizierte Sitzung **noch nicht live_verified**.

**Block 2 – Regionswechsel READY repariert:** Alpha.88 prüfte
world_model.region().simulator_ip/port irrtümlich weiter gegen
active_session->simulator_ip/port von der initialen Login-Region.
Das führte nach erfolgreichem Destination-Circuit zu einer
permanenten SIMULATION-NOT-READY-Sperre. Jetzt prüft
region_circuit_ready explizit den aktiven
circuit_target_ip/port und den Stop-Status. Die Unit-Tests prüfen
die Destination nach Wechsel, noch gestoppte und fremde Circuits.

**Block 3 – sichtbarer Avatar-Proxy als echte GPU-Aktivität
nachweisbar:** OGLRenderer zeichnete bereits eine klar provisorische
7-Teile-Selbstavatarfigur, zählte diese aber nie getrennt.
RenderDiagnostics enthält jetzt avatar_proxy_draw_calls und
self_avatar_proxy_draw_calls für tatsächlich ausgeführte
glDrawElements, über Frame-Reset abgesichert. Eine echte
GL3.3/GPU-Regression prüft sieben Self-Drawcalls, null nach
Entfernen und unveränderte Mesh-Szenezähler. Die laufende
InWorld-Anwendung meldet einmal pro Region erst nach
tatsächlichem Self-Proxy-Renderdraw eine Diagnose, deutlich
markiert als **KEIN** gebackener/skinnierter Firestorm-Avatar.
Die frühere WorldModel-Zeile avatars=0 betrifft allein die
Liste anderer Avatare, nicht die SelfAvatar-ID.

**Block 4 – Asset-Live-Auswertung und warmes Szenenfenster:**
Die echte Alpha.88-Sitzung am 11.10.2026 enthält 7
zusammenhängende InWorldAssetSnapshots. Letzter Snapshot:
252/256 Mesh-Assets (98%), 286/295 Texturen (96%),
vier fehlgeschlagene Meshes (1 missing, 3 invalid),
neun fehlgeschlagene Texturen (6 missing, 3 invalid);
keine GPU-Upload-Failures, dafür bei 200 Objekten noch
964 Fallback-Mesh-Drawcalls. Der Decoder versucht bereits
die anderen verfügbaren LODs und den alternativen GetMesh-CAP,
deshalb sind die echten Defekte/HTTP404 derzeit NICHT mit
synthetischen Geometrien zu überdecken. Vor dem Live-Grid-Abgleich
bleiben diese Assets serverseitig zu prüfen.

Der bestehende Acceptance-Prüfer benutzte bisher für die
Steady-State-Assetabnahme das Minimum der GESAMTEN Regionsepoche,
einschließlich der anfänglichen 2%-Downloadphase. Nun verwendet
er bei unveränderter Session-/Regions-/Sequenz-/Zeitvalidierung
genau die letzten konfigurierten N (normalerweise 3)
aufeinanderfolgenden, aktuellsten Numerik-Snapshots und verlangt
innerhalb dieses Fensters alle Draw-, Coverage- und
Fallback-Grenzen. Frühere Peaks bleiben im historischen Report.
Der Log-Report gibt dafür ausschließlich eine begrenzte,
numerische (max. 64), datensparsame Snapshot-Reihe aus. 15
Python-Regressionstests inklusive Warm-up, Stale-Sample,
fehlende GPU-Draws und unvollständige Fallback-Serien erfolgreich.
Die alte Alpha.88-Session bleibt mangels Frische und aufgrund
tatsächlich fehlender Meshes **not_verified**.

**Quellen:** Linden AgentUpdate Message-Layout:
https://wiki.secondlife.com/wiki/AgentUpdate;
LLQuaternion Wire 12 statt 16 Byte:
https://wiki.secondlife.com/wiki/Message_Layout;
offizieller Firestorm Releasebranch Firestorm_7.2.4
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d;
OpenGenesisLINK Server main
16d219f142926c7e00327e4ca91ecb1f8fc213e6.
Die letzte Server-Änderung modifizierte LSL/Economy,
nicht das AgentUpdate-Wire-Format.

**Weiter offen:** Eine neue echte Alpha.89-Login-Session
muss Bewegung anhand sich ändernder Serverposition,
sichtbaren SelfAvatar-Proxy im Live-Renderer sowie
aktualisierte Textur/Mesh-Asset-Readbacks nachweisen.
Skinned Avatare, Appearance/Bake, COF/Wearables/Attachments,
Regionen-Teleport End-to-End bleiben unvollständig.
Firestorm-Funktionsumfang weiterhin verbindliche Baseline;
eigene C++23-Viewer-Implementierung weiterhin kein
Firestorm-Quellcodefork.

GitHub Branch: develop-current; Repo:
marlin198323-glitch/nexverse-viewer. Bei Folgeturn
CHATUEBERGABE.md vor dem Projektplan lesen.


## Alpha.90 – LLUDP-Inventarentfernung und sichere Cache-Aktualisierung (13.10.2026)

Vier zusammenhaengende Aenderungsbloecke sind im GitHub-Quellstand vorbereitet:
1. Linden-/Firestorm-MessageIds Low 270 RemoveInventoryItem, 276 RemoveInventoryFolder und 284 RemoveInventoryObjects wurden explizit in die LLUDP-Protokollregistrierung aufgenommen.
2. Neue strikte Pruefung von AgentID + SessionID (UUID), je 1-Byte Variable-Blockzaehler und 16-Byte Inventory-IDs (hoechstens 64). Falsche Sitzung, ungültige Record-IDs, Restbytes, abgeschnittene und uebergrosse Pakete werden verworfen. Dies sind Hinweise vom verbundenen Simulator, keine direkten Inventar-Mutationen.
3. Im Live-PacketDispatcher werden solche Nachrichten mit der aktuell verbundenen WorldModel-Sitzung verglichen, anschliessend ueber die regionsgebundene FirestormRegionLease als full_refresh in dieselbe InventoryEventInbox geleitet. F9/Galerie, F10-Ordner und F10-Einzelitem-Details werden ueber den bestehenden MainThread/CAPS-Readback-Pfad invalidiert. Ohne verifizierte Folder-Parent-Kante wird nicht geraten oder geloescht. Wiederholte Hinweise werden zusammengefasst; alte Epochen verworfen.
4. Tests wurden erweitert: LLUDP-MessageIds, Single-/Dual-Variable-Block-Layout, gueltige/ungueltige Agent-/Session-IDs, Empty/Overlength/Truncated/Zero UUID, Inbox Full-Refresh, Regionepoch und Koaleszierung.

**Wichtige Einschraenkung:** Der autorisierte Linux-Rechner jam-MS-7D28 war am 13.10.2026 offline (Desktop Commander seit ca. 46 Stunden nicht verbunden). Deshalb wurde dieser Alpha.90-Quellcodeblock per GitHub-Repository aktualisiert, aber es fand **kein** lokaler C++23-Debug-Build, kein CTest, kein GUI-Test und keine authentifizierte InWorld-Abnahme statt. Der Block ist STATUS **committed_unbuilt**, nicht tested_synthetic oder live_verified. Ungetestete Beta-/Produktiv-Freigaben bleiben verboten; Build/Test beim naechsten verfuegbaren Desktop-Kontakt zuerst nachholen.

**Quellen:** FirestormViewer/phoenix-firestorm, branch Firestorm_7.2.4, scripts/messages/message_template.msg (RemoveInventoryItem 270, RemoveInventoryFolder 276, RemoveInventoryObjects 284). OpenGenesisLINK Server main 953063775933b6d7fab4f746d055587b8d74f2ea (IClientInventory.cs und Scene.Inventory.cs als Sendepfade). Der neue Server hat zudem ein optionales OGLViewerAdvisorModule; Viewerkennung `OpenGenesisLINK Viewer` bleibt kompatibel, sagt aber nichts ueber Funktionsparitaet aus.

**Naechste Prioritaet:** Build/Test/Paketlayout live pruefen, Alpha.89 AgentUpdate-WASD-Positionsdelta aus neuer Live-Session pruefen, SelfAvatar/Appearance und Mesh-Skinning, danach vollstaendige Inventar-COF/Wear-Transaktionen und End-to-End Teleport. Firestorm bleibt Funktionsbaseline, nicht austauschbar.


## 0.2.0-alpha.91 – Build-Hotfix, avatar server motion evidence und LLUDP-Regression · 13.10.2026

**Block 1 – Alpha.90 erstmals echt auf Linux gebaut:** Am 13.10.2026
war Desktop Commander auf jam-MS-7D28 wieder erreichbar.
Der vorher per GitHub veröffentlichte unbuilt-Stand Alpha.90 enthielt
einen Compilerfehler in firestorm_inventory_events_tests.cpp:
ein lokales SESSION-Lambda-Standardargument. Die UUID-Konstante
wurde in den Namespace-Scope verschoben. Anschließend liefen
83/83 CTests mit echter NVIDIA-GL-Umgebung erfolgreich.
Damit ist der unveränderte Alpha.90-Paketpfad erstmals gebaut
und synthetisch abgenommen. Der alte Hinweis committed_unbuilt
bleibt als historischer Zwischenstatus erhalten; Alpha.91 löst ihn ab.

**Block 2 – WASD-Reaktion des Simulators endlich getrennt messen:**
Neue header-only, mutex-geschützte
protocol::MovementFeedbackMonitor verarbeitet unidirektionale
AgentUpdate-WASD-ControlFlags auf dem UI-Thread und echte
serverautoritierte Self-ImprovedTerseObjectUpdate-Positionen
auf dem LLUDP-Worker. Nach wenigstens 25 cm Netto-Positionsänderung
in einer zusammenhängenden Bewegungsanforderung wird genau ein
"Avatar motion VERIFIED" protokolliert (nur serverbasierte
Positionsreaktion, ausdrücklich nicht Avatar-Bake/Animation).
Nach mindestens 4 Sekunden und 3 Terse-Samples ohne
Positionsänderung wird genau eine präzise WARN-Protokollmeldung
ausgegeben, keine voreilige Bewegungsfreigabe.
Gegensinnige Tasten und reine Rotation zählen NICHT als
Fortbewegung. Bei neuer Eingabe und beim Regionen-/Login-Circuit-
Reset werden alte Ergebnisse nicht fortgeschrieben. Der Monitor
manipuliert WorldModel oder Network-Payload nie. Deterministische
Tests prüfen Tasten, Stillstand, bestätigten Positionsdelta,
Warnungs-/Bestätigungs-Deduplizierung, neue Eingaben und
Teleport-Reset. **Eine frische authentifizierte Alpha.91-
InWorld-Session wurde nicht durchgeführt**, keine echte WASD-
Freigabe behauptet.

**Block 3 – zusätzliche sichere Server-Inventory-Varianten:**
Die Alpha.90-Parser für Firestorm RemoveInventoryItem (Low 270),
RemoveInventoryFolder (Low 276), RemoveInventoryObjects (Low 284)
wurden um Live-ähnliche Grenzwert-Regressionen für 64 gültige
UUIDs, 65 ungültige, zwei Variablenblöcke mit insgesamt 64
UUIDs, unpassende Nachrichtentypen und gebrochene Blocklängen
erweitert. Selbst valide Meldungen sind weiterhin nur
Readback-Invalidierungen; nicht die Freigabe, Servergegenstände
ohne Rückleseprüfung zu löschen.

**Block 4 – kompletter Firestorm-AgentUpdate-Wire-Golden-Test:**
Der bisherige Test prüfte 114-Byte-Nutzdaten und den
ControlFlags-Offset. Jetzt werden zusätzlich BodyRotation
LLQuaternion x/y/z, HeadRotation x/y/z, State, CameraCenter
und CameraAt, FarClip sowie Flags bytegenau gegen Firestorms
High-4-Layout geprüft. Dadurch fällt erneut eine
versehentliche 16-Byte-Quaternion oder vertauschte Camera-
und Controls-Position bei CI/CTest sofort auf.

**Aktuelle Quellreferenzen:** FirestormViewer/phoenix-firestorm
Firestorm_7.2.4 @ 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
OpenGenesisLINK/opengenesislinkserver main @
953063775933b6d7fab4f746d055587b8d74f2ea.
Auf Serverseite gibt es inzwischen unter anderem
OGLViewerAdvisor und IAR/Portal-Protokolle; diese sind nicht
mit einem vollständig implementierten Avatar-Wear/Appearance-
Protokoll gleichzusetzen.

**Abnahme:** Alpha.91 C++23 Build und alle CTests samt GPU-
Tests auf DISPLAY=:0 durchführen; Python Regression und GUI
Welcome-Smoke prüfen. Neues Logszenario hat nach 11.10.2026
bislang keine authentifizierte Sitzung. Nicht freigegeben:
echte NexVerse-WASD-/Teleport-Live-Abnahme, Firestorm Avatar
Bakes/Skeleton/Outfit-Wear/COF, fehlende Assets und originale
Firestorm-Vollparität.

**Folgepriorität:** Neue OGL-Login-Session starten, WASD
mindestens fünf Sekunden halten und auf die neue Avatar-
Motion-Diagnose prüfen; danach Position, SelfAvatar und
Textures/Mesh-Readback; echte zwei-Regionen-Circuit-/CAPS-
Teleports; serverseitige OGL-IAR/Appearance-Spezifikationen
für Wearables/COF und Attachments vergleichen.
Branch develop-current, Repository
marlin198323-glitch/nexverse-viewer, projectplan.md maßgeblich.


## Alpha.92 – Firestorm AvatarAppearance Hover/Attachments und sauberer SelfAvatar · 13.10.2026

**Block 1 – gültige moderne Firestorm-AvatarAppearance-Pakete lesen:**
Der OGL-Decoder für Trusted Low 158 verlangte bislang,
dass die Nutzdaten bereits nach dem AppearanceData-Block
mit 9 Bytes je Eintrag enden. Der offizielle Firestorm-Branch
Firestorm_7.2.4 hat seit Langem zusätzlich AppearanceHover
Variable mit LLVector3 (12 Byte je Datensatz) und
AttachmentBlock Variable mit LLUUID + U8 AttachmentPoint
(17 Byte je Datensatz). Solche vollständigen Nachrichten
wurden vom bisherigen OGL-Decoder verworfen.
Der neue Decoder liest beide Blöcke samt Anzahl, begrenzt
maximal 16 Hover- und 64 Attachment-Datensätze,
prüft exakte Blocklängen, gültige finite Floatwerte
und blockiert Restbytes, fehlende zweite Zähler und
abgeschnittene Attachment-IDs. Er akzeptiert weiterhin
das alte OpenSim-No-Tail-Format. Der Decoder behandelt
serverseitige Attachment-Metadaten ausschließlich als
Readback, NICHT als vollzogenen Wear-/Attach-Vorgang.

**Block 2 – eigene Appearance ist kein zweiter Avatar:**
WorldModel verwaltet das vom Server für die authentifizierte
AgentID gesendete AvatarAppearance-Readback getrennt von
der Liste fremder Avatar-Entities. Nur exakt die AgentID
darf self_appearance setzen; fremde Daten werden abgelehnt.
Self Appearance und die remote-Avatar-Map werden bei
WorldModel.reset und bei begin_region_transition getrennt
invalidiert. WorldSnapshot gibt die eigene Appearance
für spätere COF-/Bake-/Skinned-Avatar-Stufen weiter.
Der AvatarAppearance-LLUDP-Handler protokolliert das
erste Self-Readback der Region mit Anzahl Texturbytes,
VisualParams und Attachments OHNE private UUIDs/Assetinhalte.
Ein gültiges Self-Paket wird nicht mehr als fremder Avatar
in der Weltobjektliste gespeichert. Die vorhandene
7-Teile-Self-Geometrie bleibt ausdrücklich nur ein Proxy.

**Block 3 – remote appearance nicht durch Pose-Pakete verlieren:**
ObjectUpdate, ObjectUpdateCompressed, ImprovedTerseObjectUpdate
und CoarseLocationUpdate aktualisieren Position/Rotation
über die neue zentrale WorldModel.update_avatar_transform-
Methode; diese erhält den bereits empfangenen COF-
Versionsstand, Textur-Einträge, VisualParams und
Attachment-/Hover-Blöcke, auch wenn AvatarAppearance
vor ObjectUpdate eintrifft. Ein neuer Remote-Avatar hat
position_known=false, bis ein echter Positionsevent
eintrifft. Reine Appearance-Hinweise erzeugen deshalb
keinen sichtbaren Proxy als Phantomavatar bei (0,0,0).
Bei CoarseLocationUpdate bleibt die vorhandene LocalID
bestehen. SelfAvatar-Pose wird darüber grundsätzlich
nicht als fremde Entity verdoppelt.

**Block 4 – minimale optische Hover-Parität und Tests:**
Der aktuelle Renderer positioniert den provisorischen
Avatar-Proxy um den Server-HoverHeight-Z-Wert (bounded
-2m..2m) höher/tiefer. Die serverautoritative Position
im WorldModel und die Collision-/Bewegungskoordinaten
werden dadurch NIE verändert; Skelett und echte
Baked-Wearable-/Skins-Textur-Projektion bleiben offen.
Die bestehenden Decoder-/WorldModel-Tests wurden um
moderne Hover-/Attachment-Blöcke, Legacy-Fallback,
bösartige/abgeschnittene/überlange Blockvarianten,
NaN-Hover, korrekte SelfAgent-Grenzen, WorldSnapshot,
Regionswechsel-Invalidierung und unveränderte
Appearance bei Pose-/Coarse-Updates erweitert.

**Quellen:** FirestormViewer/phoenix-firestorm,
Firestorm_7.2.4,
scripts/messages/message_template.msg (Trusted
AvatarAppearance Low 158: Sender, ObjectData,
VisualParam, AppearanceData, AppearanceHover,
AttachmentBlock). Offizieller OpenGenesisLINK Server
opengenesislink/opengenesislinkserver main am
13.10.2026: c29f3e5a70ccc0971b22371f1d13fd1de23be240.
Server-IAR- und OGL-Portal-Arbeit ist keine
automatische Viewer-Appearance/Bake-Reparatur.

**Abnahme:** Vollständiger C++23-Linux-Debug-Build,
83 CTests mit NVIDIA GPU und GUI-Smoke,
Python-Bausteine und Git-Diff-Check.
Die bisherigen lokalen Daten sind nur synthetisch;
eine reale Alpha.92-Login-/SelfAvatar-Appearance-
Abnahme benötigt einen Benutzerlogin in NexVerse.
Kein Test beweist fertige Firestorm-Avatarkleidung,
Rigging, Mesh-Skeleton, Skinning, Animation, COF
Wear oder Attachment-Rendering.

**Nächster Schritt:** Regionbasierte echte
SelfAppearance-Readbacks und ihre COF-Version in den
frischen Logs nach Anmeldung validieren; danach
GPU-Baked-Skin-Layering, LLAvatarSkeleton/Animation,
Appearance-Cache, COF-Outfit-/Attachment-Transaktion
mit echten Serverrechten, Mesh-Live-Probleme und
echten Regionswechsel testen.
Branch develop-current. projectplan.md maßgeblich.


## Alpha.93 – Firestorm baked Head/Upper/Lower auf OpenGL-Avatar-Proxy; Avatar-Parent-Transform · 13.10.2026

**Block 1 – Firestorm Avatar-TextureEntry-Face-Indizes mit gültigen
Bakes:** Der offizielle Firestorm 7.2.4 LLAvatarAppearanceDefines
benennt als gebackene Textur-FACES: Head=8, Upper=9, Lower=10,
Eyes=11, Skirt=19, Hair=20, LeftArm=40, LeftLeg=41,
Aux1=42, Aux2=43, Aux3=44. Neuer
include/nexverse/protocol/avatar_bakes.hpp liest alle 11 nur
aus der bereits streng validierten TextureEntry des
AvatarAppearance-Server-Readback. Der TextureEntry-Parser
merkt jetzt für jede Face ausdrücklich, ob eine Texture-UUID
per Face-Bitfield übergeben wurde. Ein geerbter Default-
Texture-Eintrag, bloßer Color-/UV-Override, Zero-UUID oder
offizielle Firestorm IMG_USE_BAKED_*-Placeholder-UUID
erzeugt damit ausdrücklich KEINEN falschen Baked-Asset-
Download. Sichtbar in der provisorischen Avatar-Geometrie
sind jetzt die Slots Head/Upper/Lower; alle übrigen acht
werden bereits sicher identifiziert, aber noch nicht gerendert.

**Block 2 – echte GetTexture-Residency für Avatar-Bakes:**
Der Avatar-Proxy erhält die drei gültigen serverseitigen
Head/Upper/Lower Asset-UUIDs. Sie werden beim bestehenden
750-ms-Prioritäts-/Mip-/Cache-/Budgetplan als nahe
Avatar-Textur-Samples zusammen mit den Weltobjekt-
Materialtexturen berücksichtigt. Dadurch erfolgen Abruf,
Retry, Disk-/RAM-Cache und GPU-Upload über die
bestehende GetTexture-Protokoll-/LLUDP-CAPS-Pipeline,
ohne unkontrollierte parallele Avatar-HTTP-Worker oder
die bisherigen Material-/Region-Circuit-Budgets zu
umgehen. Der kombinierte Prioritätsvektor wird nur
während einer tatsächlichen Residency-Planung aufgebaut,
nicht in jedem Frame. Ändert AvatarAppearance bei
unveränderter Objektliste eine Bake, wird sie beim
nächsten Planner-Durchlauf erkannt. Auch Resident-
Texturen fremder Avatare werden entsprechend eingeplant.

**Block 3 – tatsächliche GL-Baked-Textur-Draws am
provisorischen Körper:** Der Legacy-OpenGL3.3-Renderer
verwendet beim Head-Teil die BakedHead-ID, beim Torso
und Armen die BakedUpper-ID, bei den Beinen die
BakedLower-ID; die bestehende OpenGL-Objekttextur-
GPU-Tabelle liefert das resident GL_TEXTURE_2D.
Es werden richtige Shader-Textursamples gezeichnet
und die temporäre Textur-/Shaderbindung wieder
sauber zurückgesetzt. Wenn UUID fehlt oder nicht
GPU-resident ist, bleibt der vorherige untexturierte
Farbproxy sichtbar. Zusätzlich wurden
avatar_baked_texture_draw_calls und
self_avatar_baked_texture_draw_calls in RenderDiagnostics
als echte glDrawElements-Zähler eingeführt. Der Viewer
meldet nach tatsächlichem Self-Draw einmal je Region
eine privacy-sichere Logzeile. GPU-Regression mit
residiertem Testtexturasset beweist sieben Textur-
Drawcalls, mit Entfernen desselben Assets null
Textur-Drawcalls und weiterhin sieben Proxy-Draws.
**Diese vereinfachte UV-Darstellung ist noch KEIN
originales Firestorm-Avatar-Skelett, keine korrekte
UV-Körperprojektion und kein Avatar-Mesh-Skinning.**

**Block 4 – Attachment-Root-Transform über echte
Simulator-Local-IDs:** Bei SceneObject.parent_id mit
passender gebundener Avatar-LocalID wird der
Objekt-Root an die aktuelle Avatarposition und
Rotation gebunden. Zuvor fand die Renderer-
Parent-Resolver-Funktion nur Parent-PRIMs; ein
Attachment an einem Avatar-Root wurde wie ein
losgelöstes Weltobjekt positioniert. Die neue
find_attachment_avatar_parent-Hilfe ist strikt
nach lokalen Nichtnull-IDs gebunden, verarbeitet
Self- und andere Avatare ohne UUID-Raten und fällt
bei nicht bekanntem Parent auf bisherigen Verhalten
zurück. Echte Attachment-Punkte der Firestorm-
Skelettknochen, Attachment-Animationen,
Attachments mit bereits eigenen Prim-Hierarchien,
BoneOffsets und Fashion/Wear COF liegen weiterhin
auf dem nächsten Projektplan-Abschnitt.

**Tests/Abnahme:** Test-TextureEntry-Face-Bitfields
für 8/9/10, non-baked Default-Face, offizielle
IMG_USE_BAKED-Placeholder und Null-UUID,
GPU-Residency und fehlende Textur, 7 vs 0
Textur-Drawcalls, SelfAvatar-Zähler,
Attachment-LocalID-Matching und Unbekannt/Null
als Negativfälle; komplette C++23 Debug-Build-/CTest-
und Python-Regressionen sowie GPU- und GUI-Smoke
durchführen. Aktuell KEIN neuer authentifizierter
Grid-Login nach Alpha.92-Smoke; BakedAsset-Live-
Render und Avatarbewegung können deshalb noch
NICHT live_verified behauptet werden.

**Quellen:** offizieller Firestorm Branch
Firestorm_7.2.4:
indra/llappearance/llavatarappearancedefines.h,
indra/llappearance/llavatarappearancedefines.cpp
(Face-Indices) und
indra/llcommon/indra_constants.cpp
(IMG_USE_BAKED_* Platzhalter).
OpenGenesisLINK Server GitHub
opengenesislink/opengenesislinkserver,
Main-Stand c29f3e5a70ccc0971b22371f1d13fd1de23be240
(am 13.10.2026 beim Codeabgleich).
OpenGenesisLINK bleibt OpenSim-Grundprotokoll;
Firestorm ist die verbindliche Standard-Funktionsbaseline,
kein bereits vollständig übernommener Source-Fork.

**Nächste 3–4 Blöcke:** reale Alpha.93-NexVerse-Session
mit AvatarAppearance-Bakes und drei GPU-Textur-
Drawcalls testen; korrektes SL-/Firestorm-
Avatar-Skeleton und RigMesh-VertexWeights,
BakedTexture-UV und Wearable-Assets;
AttachmentPoint-/Skeleton-Joint-Transforms;
authentifizierter COF-Outfit- und Avatar-Bake-
Readback ohne ungeprüfte Mutation. Anschließend
Teleport zwischen Regionen und Mesh-HTTP-404-
Assets im Grid nachprüfen.
Branch develop-current; zuerst CHATUEBERGABE.md,
projectplan.md maßgeblich.


## Alpha.94 (13.10.2026) – BESCHLEUNIGTER FIRESTORM ORIGINAL-BODY MILESTONE

Verbindlicher Modus ab hier: Bei "weiter" acht bis zwoelf technisch
zusammenhaengende Module/Pakete in einem integrierten, lokal gebauten
und getesteten GitHub-Commit fertigstellen, sofern realistisch.
Immer CHATUEBERGABE.md zuerst, dann projectplan.md, danach Code,
CTest/Python/GPU und Commit/Push; keine erfundenen Live-Nachweise.

Paket 1: Firestorm Linden Binary Mesh 1.0 Originalparser in
include/nexverse/assets/firestorm_avatar_mesh.hpp und
src/assets/firestorm_avatar_mesh.cpp; das Firestorm-
LLPolyMeshSharedData::loadMesh On-Disk-Layout mit Header,
Gewicht/Detail-Flags, Position, Rotation, Scale, spaltenweise
Positionen/Normals/Binormals, originale UVs, F32-Weighs, Dreieckindices,
JointNames. 8MiB-Dateibudget, <=20000 Vertices, <=256 JointNames,
finite Floatwerte und sichere Index-/Längenbegrenzungen.
Firestorms Gewichte sind 1-basiert und teilweise sparse (Upper-
Body-Mesh kennt Weight 14 bei 12 JointNames), nicht als direkten
C++-Index benutzen. MorphTargets nur unparsed trailing, spaeter.

Paket 2: Keine erfundenen Avatarformen oder unerlaubte Binärpakete:
Original Firestorm-Ressourcen ueber NEXVERSE_AVATAR_ASSETS,
assets/character oder bestehendes ~/firestorm/character erkennen.
Ohne lokale Ressourcen bleibt alter 7-Teile-Farbproxy sichtbar.
Fuer Windows/macOS ist ein gepruefter Packaging-Pfad weiter offen.

Paket 3: Drei ORIGINAL Firestorm Kopf/Ober-/Unterkoerper-.llm-
Meshdateien geladen (4.320 Vertices, 7.186 Dreiecke);
speichern Original-UV und SkinGewicht-/Joint-Informationen.

Paket 4: Renderer set_avatar_body_mesh fuer 3 GPU-Meshes mit
realen VAO/VBO/EBO und Original-UVs; Ersatz der 7 Kasten-Draws
durch drei realistische Body-Draws, wenn alle 3 vorhanden sind.
Separat gemessene avatar_anatomical_mesh_draw_calls,
self_avatar_anatomical_mesh_draw_calls, atomare Uploads,
OpenGL Cleanup und 7-Proxy-Fallback.

Paket 5: Head/Upper/Lower verwenden weiterhin die schon
funktionierende GetTexture/Firestorm Baked-UUID-GPU-Tabelle.
Fehlende gebackene Texturen nutzen sichtbare neutrale Tints;
keine ungepruefte externe URL oder parallele Downloadpipeline.

Paket 6: Original avatar_skeleton.xml geladen und validiert:
133 Joint-/Bone-Knoten mit Eltern, lokaler Translation,
Euler-Angaben und Restbindpositionen (XML-Tiefen-/Bytegrenzen).
Rotationsgetriebenes Skinning ist NOCH NICHT implementiert.

Paket 7: Original avatar_lad.xml mit 55 Attachment IDs,
Joint-Namen und BindOffsets; mRoot (Avatar-Center)
und mScreen (HUD) sind virtuelle Frames und keine
physikalischen Skeleton-Bones.

Paket 8: SceneObject Original-UUID + WorldModel-
AvatarAppearance AttachmentBlock ID/Point + validierte
Avatar-LocalID zum Bind-Pose-Ankern der Objekt-Root-Pose.
Firestorm-HUD Attachments IDs 31–38 sind nicht als
3D-Weltobjekte zu zeichnen; eine separate 2D-HUD-Pipeline fehlt.

Paket 9: Lokale originale Firestorm-Binaries in echten Unit-
und GL-Regressionen: .llm-, Skeleton- und Attachment-
Daten; bösartige Header/Abbruch/Count; echter Upload
aller originalen 7.186 Dreiecke mit OpenGL 3.3;
separate Textur-Bind/Fallback Draws. CTest auf CI
ohne installierte Firestorm-Assets markiert SKIP 77
statt fälschlich FAIL oder bestanden zu behaupten.

Paket 10: Lokaler GUI-Smoke unter Linux NVIDIA
RTX4070 SUPER: Startup meldet 133 Skeleton-Joints,
55 Attachment-Punkte und 7.186 GPU-residente
Original-Avatar-Dreiecke; NexVerse-Welcome geladen.
Eine echte neue angemeldete InWorld-Session fand
NICHT statt: Bewegung/Skin/COF/3D-Welt und echte
Avatar-Self-Draws noch nicht live_verified.

**WICHTIG:** Dies ist nun direkte Integration ORIGINALER
Firestorm-Avatarressourcen/Formate, aber OGL ist nach
wie vor eine separate C++23-Anwendung und kein
vollstaendiger Firestorm-Quellcodefork.
Nicht als fertig deklarieren: LLJoint-Weight-Skinning,
Animations-/Facial-/WearableMorph-Pipeline,
HUD-UI, COF-Wear-Rechte/Mutationen, weitere
Firestorm-Funktionsparitaet, volle Plattformassets
und Login-/Regionswechsel-E2E.

**Naechste schnelle Phase:** Original Firestorm Vertex
Weights + JointNames auf mPelvis/Spine/Head/
Arme/Beine mit deformierender CPU/GPU-Pose,
LLAnimation und Shape-Morph-Parametern; danach
echte NexVerse Live-Sitzung, COF/Wear/
Attachments, Mesh-Live-Defekte, Teleport.
Keine weiteren provisorischen Ersatz-Avatarformen.

Quellen: FirestormViewer/phoenix-firestorm,
Branch Firestorm_7.2.4, Commit
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d,
insbesondere LLPolyMesh, avatar_lad.xml,
avatar_skeleton.xml. OpenGenesisLINK Server
ist OpenSim- und Protokollreferenz.


## Alpha.95 – BESCHLEUNIGTER ORIGINAL-FIRESTORM-GPU-SKINNING-BLOCK (13.10.2026)

Der beschleunigte Arbeitsschritt umfasst mehrere zusammenhaengende
Renderer-, Netzwerk-, WorldModel-, Assets-, Test- und
Dokumentationsmodule. GitHub-Branch develop-current,
projectplan.md ist massgeblich. Firestorm_7.2.4
(LLAvatarJointMesh::setupJoint und
getBaseSkeletonAncestor in llavatarjointmesh.cpp)
sowie OpenGenesisLINK Server sind technische Referenzen.

**1. Firestorm Joint-Palettenalgorithmus:** Der Mesh-.llm-
F32-Weight indiziert nicht einfach die Original JointNames,
sondern Firestorms Depth-First-LLJointRenderData-Palette,
die BASE-Ancestors zusaetzlich enthaelt. Der original
avatar_skeleton.xml Parser speichert jetzt support=base/
extended; firestorm_joint_render_palette erzeugt die
echte Folge Head=3 (mChest,mNeck,mHead), Upper=15,
Lower=9. Echte Original-.llm-Weights wurden auf
diese Palettengrenzen geprueft.

**2. Original Vertex Skin Buffers:** RenderMeshGeometry
kennt Original SkinWeights und Palette Indices. Der OpenGL
Renderer laedt diese F32-Werte in separaten VertexBuffer,
GLSL attribute location 4. Falsche/fehlende Counts,
NaN/OutOfRange oder laengere Paletten werden verworfen,
bevor sie alte GPU-Bodies ueberschreiben.

**3. Neues OpenGL3.3 Shader-Weighting:** Der
Originalvertex wird anhand palette[floor(weight)] und
palette[floor(weight)+1] mit fract(weight) gemischt;
GLSL uAvatarJointPalette[32], uAvatarSkinEnabled nur
auf Avatar-Body-VAO, nicht auf Weltprim-/Terrain-VAO.
Original UV/Bake GetTexture aus Alpha.93/94 bleiben.

**4. 133 Bone Pose-Traversal:** Joint Parents und
Local-Bind-Translations ergeben eine gemeinsame Pose,
die zur Bindpose ruecktransformiert wird. Keine
CPU-Vertex-Updates pro Frame; per-Avatar GPU-Joint-Matrix.
Die erste Pose ignoriert komplexe LLAvatarJoint
Rest-Euler-/Morph-Bind-Semantik, echte Firestorm-Vollparitaet
weiter offen.

**5. Deformierende Walk-Prototyp-Animation:** Bei Bewegung
werden mHipLeft/Right, mKneeLeft/Right, mShoulderLeft/Right,
mElbowLeft/Right und mChest zu einer deterministischen
Schrittbewegung animiert. Es werden Original-Koerpermeshes
und Original-Gewichte verwendet, keine Ersatzgeometrie.
Dies ist prozedural und NOCH NICHT der vom Server
uebertragene AvatarAnimation-/LLKeyframeMotion-Stream.

**6. SelfAvatar-Eingabe und Sicherheit:** WASD-AgentUpdate
liefert nur den vorlaeufigen Walk-Animation-Intent.
Eine echte Simulator-Positionsreaktion bleibt strikt von
der Animation getrennt (Alpha.91 MovementFeedbackMonitor).
Login-/Regionswechsel resetten das ControlFlag.

**7. Andere Avatare:** WorldModel speichert in AvatarState
last_translation_at nach serverseitigen Deltas >0.05m
und kleiner als 3m; innerhalb 650ms erscheint die
Walk-Pose. Kleinste Quantisierungswerte sowie
Teleport-Discontinuities triggern nicht; Regionenwechsel
setzen RemoteAvatar-Stati zurueck.

**8. GPU-State und Ressourcen:** SkinVBO wird bei
Shutdown, Ersetzen und Fehler geloescht. UModel beim
Body wird explizit auf AvatarWorld gesetzt;
uAvatarSkinEnabled nach den Originalkoerper-Drawws
zurueckgesetzt, sodass Prim-/Partikel-Shader nicht
versehentlich deformiert werden.

**9. Diagnosen und konkrete GPU-Tests:** Separate
avatar_weighted_skin_draw_calls,
self_avatar_weighted_skin_draw_calls sowie
avatar_locally_animated_skin_draw_calls. GPU-Fixtures
nutzen die Original Firestorm Head/Upper/Lower-.llm
Dateien samt Skeleton/Pallette im NVIDIA-OpenGL3.3.
Idle hat drei gewichtet gezeichnete Bodies und
null animierte SkinDraws; Moving drei gewichtete
und drei aktiv animierte. Negative SkinCount-
und PaletteWeight-Tests duerfen keine
gueltigen GPU-Objekte ueberschreiben.

**10. Vollstaendige Systemintegritaet:** Die Original
Firestorm 133-Joint/55-Attachment-Definition,
NexVerse-Welcome, OpenSim LLUDP/CAPS,
Head/Upper/Lower-Baked-Textures und Mesh-
Regionevents bleiben in derselben App.
Neue Dokumentationsfiles und testbare
Quelle bleiben auf develop-current.

**Nicht freigegeben:** Kein frisch authentifizierter
Grid-Live-Test fuer Alpha.95 erfolgt. Die Animation
ist vorlaeufig, keine echten Server-Animationsfiles
werden gespielt. Die originale LLKeyframeMotion
Priority/Blend/Fly/Jump/Sit, MorphTargets,
Joint-Normals, vollstaendige Bone-Restrotation,
Ankle/Attachment-Bone-Animation,
Appearance/COF/Wear und die konkrete
NexVerse-E2E-Avatarwelt bleiben offen.
Die Firestorm-Funktionsparitaet ist keinesfalls
vollstaendig; OGL ist weiterhin eine separate
C++23-Entwicklung mit originalen Ressourcen/
ausgewaehlter Firestorm-Formatsemantik.

**Naechste beschleunigte Arbeitspakete:**
Firestorm-kompatible AvatarAnimation LLUDP-ID/
Animationsstates mit Agent-/Session-/Region-
Validierung; original LLKeyframeMotion/Ani-
Dateien fuer Idle/Walk/Run/Jump/Sit/Fly,
Pose-Blending nach Priority und Loop,
Vertex-Normal-Skinning, JointOffsets/
MorphVisualParams, echte Attachments-
SkeletalRotation, Wearable/COF-Transaktionen
und authentifizierte NexVerse Live-Sichtprobe.
Quelle: FirestormViewer/phoenix-firestorm
Firestorm_7.2.4 commit
10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.

## Alpha.96 – Servergesteuerte Firestorm-/OpenSim-AvatarAnimation (13.10.2026)

Alpha.96 ist ein beschleunigter, zusammenhaengender Funktionsblock:
Protokoll, WorldModel, Avatar-GPU, Live-Readback, Negativtests,
GUI-Build und Dokumentation in EINEM GitHub-Commit.

1. Referenz: Firestorm 7.2.4 message_template.msg,
   AvatarAnimation High 20 Trusted Unencoded, Simulator -> Viewer.
   Die Feldreihenfolge lautet Sender.ID UUID, AnimationList Variable
   AnimID UUID + AnimSequenceID S32, AnimationSourceList Variable
   ObjectID UUID und PhysicalAvatarEventList Variable TypeData Variable1.
   High20 traegt KEINE Session-ID und KEIN Region-Handle.
2. Ein neuer strikt begrenzter Decoder in
   include/nexverse/protocol/avatar_animation.hpp parst alle vier
   Bloecke samt Variablen-Anzahlen; max. 96 Animationen/Quellen,
   32 Physik-Ereignisse und 12 KiB Nutzdaten.
3. UUID-Normalisierung, volle Byteoffset-Pruefung, kein unerwarteter
   Rest, verwaiste/geloeschte Source- und Event-Counts invalid,
   zero-Sender und Null-AnimIDs gesperrt. Physikbloecke nur roh
   eingelesen, keine ungepruefte Ausfuehrung oder Aktorbewegung.
4. Bei doppelter Animation-UUID entscheidet die hoechste
   AnimSequenceID; ein leerer vollstaendiger Snapshot bedeutet
   Stop aller vorherigen Animationen des konkreten Absenders.
5. WorldModel enthaelt per SelfAgentID getrennte Animation-Readbacks,
   remote AvatarState.animation und WorldSnapshot.self_animations.
   Region/Epochenwechsel und Reset invalidieren alles, Pose-
   /Appearance-Updates loeschen keine laufenden Animationen.
6. Wenn die Animation eines fremden Avatars vor ObjectUpdate kommt,
   wird sie fuer hoechstens 64 fremde IDs vorgehalten, ohne ein
   sichtbares Phantomobjekt bei 0,0,0 zu erzeugen. Der erste
   ObjectUpdate oder Coarse-Poseschritt uebernimmt sie.
   Beim Entfernen des Avatars wird der Pending-Eintrag invalidiert.
7. LLUDP-Handler laeuft nur auf aktuell verbundenem Simulator-
   Circuit und verbundenem Weltmodell; die Nachricht selbst
   enthaelt KEIN eigenes session_id oder region_handle Feld.
   Kein falscher Identity-Check gegen ein nicht vorhandenes Feld.
   Self-Logging verwendet nur anonyme Bloeckchen-Zaehler.
8. Original-Firestorm Standardanimation-UUIDs aus
   indra/llcharacter/llanimationstates.cpp (Walk/New, Run/New,
   Jump/PreJump/Land, Fly/FlySlow, Sit/SitFemale/SitGround, Hover)
   werden erkannt. Eigene fremde Custom UUIDs bleiben als
   beobachtete Assets im Modell, aber werden NICHT als
   bestaetigte Standard-Bewegung gemeldet.
9. Eine per-Avatar Motion-Wahl verhindert, dass ein Sit/Fly/
   Jump-Readback zu einer standarisieren Gehpose degradiert
   wird. Ein einfacher konservativer Auswahlvorrang fuer die
   vorlaeufige lokale Pose ist Sit > Jump > Fly > Hover > Run
   > Walk; dies ist KEIN Asset-Priority-/Blend-Playback.
10. Der bestehende Original-Firestorm Weight/Joint-GLSL-Pfad
    verwendet nun sechs sichtbare Modi Idle, Walk, Run, Jump,
    Fly, Sit plus Hover; Run hat hoehere Schrittrate, Sit
    beugt Huefte und Knie, Jump und Fly haben eigene
    Hueft-/Arme-Posen. Keine Ersetzung der echten Koerpermeshes
    durch Eigenbau-Boxen, keine Aenderung von Simulator-Koordinaten.
11. Separater GPU-Zaehlpfad
    avatar_server_selected_skin_draw_calls wird NUR fuer
    tatsaechlich erkannte Simulator-Standardanimations-UUIDs
    aktiv. Bei leerem Snapshot oder unbekannter Custom-UUID
    wird kein Server-Standardbewegungs-Beleg gefaelscht.
12. Neue eigene CTest-Suite mit golden High20-LLUDP-Header,
    Sender-ID, signed Sequence IDs, doppelten UUIDs, Quellen,
    PhysicalEvent Variable1, Empty-Stop, Truncation, Trailing
    Bytes, Count-Limits, Null-IDs; WorldModel-Epoch/
    Pending-/Bounded-/Readback-Tests; echte Original
    Firestorm Mesh GPU Posen fuer Run/Jump/Sit/Fly/Hover.

AUSDRUECKLICH WEITERHIN OFFEN: Der Simulator bestaetigt hier
AnimUUIDs und Aktivierungssequenz, NICHT die Animations-Kurven.
OGL nutzt weiterhin einfache prozedurale Posen statt echte
Firestorm LLKeyframeMotion-/Animation-Asset-Dateien.
Somit fehlen Asset-Fetch/Decoding, vollstaendige Prioritaet,
Loop/Blend-In/Out, Gesichtsanimationen, Breathing,
Sit-on-Object Attachment-Bone-Fortschritt und
NexVerse-Live-Validierung nach Login. Die Datei
docs/ALPHA96_SERVER_ANIMATION.md ist Quellvertragsreferenz.

FOLGEBLOCK: Original LLKeyframeMotion v1/v2 binaeres Decoding
und priorisierte Asset-Clip-Wiedergabe mit realen
Joint-Rotation-Quaternions; serverseitige AnimationList mit
AnimationSource und Sit/Stand/COF koppeln; danach GPU-
SkinNormal-Transformation, Animation blending und
NexVerse-E2E mit authentifiziertem Benutzerlogin.

## Alpha.97 – NexVerse/Firestorm UI parallel zu Live-Funktionen (13.10.2026)

Der Nutzer verlangt, dass Menüs, Buttons, Links, klickbare
Fenster und NexVerse-Design zeitgleich zur Backend-Entwicklung
funktionieren. Alpha.97 integriert im selben Release folgende
UI-/Netzwerk-/Fenster-/Test-Blöcke, nicht nur eine UI-Mockup-Datei:

1. Eigene ui/viewer_shell.hpp mit gemeinsamer Zustands-/Hitbox-
   Geometrie für Top-Menü, linke Navigationsleiste, Bottom-Dock
   und modale Floater. Das verhindert auseinanderdriftende
   sichtbare Buttons und unsichtbare Klickbereiche.
2. Fest sichtbare obere Menüs Avatar / Kommunikation / Welt /
   Ansicht / Hilfe samt anklickbaren Dropdown-Einträgen,
   aktivem Selektionsstil und korrekt ausgegrauten Funktionen.
3. Sieben Dock-Aktionen (Chat, Bewegen, Teleport, Inventar,
   Outfits, Erlebnisse, Karte), die auf bestehende Protokoll-
   oder echte Fenstermodule verlinken – keine falschen
   Sprechen/Bauen-Buttons als fertige User-Aktion.
4. Bestehende linke Rail Welt/Orte/Inventar ist anklickbar.
   Freunde/Gruppen/Events/Marketplace sichtbar, aber "IN ARBEIT"
   und inaktiv, solange API/Funktionshandler fehlen.
5. Neue echte OpenGL-Fenster: Lokaler Chat, Teleport-
   Koordinaten, Region/Simulator-Daten, Mini-Region-Karte,
   Avatar-Status, Grafik/Einstellungen und Steuerung/Hilfe.
   Neues ui/viewer_shell_render.cpp und Header.
   NexVerse Dark/Cyan/Violett im vorhandenen UI-Renderer.
6. Inworld ChatInput mit UTF8, Backspace, Enter, Senden,
   Sendewarteschlange via LocalChatMessage (echtes
   ChatFromViewer LLUDP), und Status ohne erfundene
   Zustellbestätigung; kein Schreiben des Chatinhalts in Logs.
7. Eingehende ChatFromSimulator-Nachrichten mit sender, text,
   max. 48 Zeilen im lokalen Viewer-Fenster (mutex protected),
   Empfang bleibt im aktuellen Simulator-Circuit.
8. Teleport-Fenster mit editierbaren X/Y/Z-Feldern,
   TAB/Backspace/Enter, strenger Koordinaten-/Region-
   Bereichsprüfung; expliziter Benutzerklick sendet
   vorhandenes TeleportLocation OutboundCommand in denselben
   autorisierten RegionHandle; kein simuliertes TeleportFinish.
9. Live-WorldModel-Statusfenster mit tatsächlicher Region,
   Terrain-/Parcel-/Objektzahl/Avataren und Position;
   Karte benutzt nur wirkliche Remote- und Self-Positionen,
   nennt sich bewusst Regionsausschnitt, NICHT globale Karte.
   Avatar-Fenster zeigt echt empfangene Appearance,
   Animations-/Attachment-Zahlen.
10. Bestehende F8-Experiences-, F9-Outfit- und F10-Inventar-
    Floater per Klick verknüpft. Outfit- und Inventarfenster
    besitzen nun echte X-Schließen-Buttons. Inventar erhält
    Oeffnen/Zurueck/Aktualisieren mit nutzbarer Klickfunktion.
11. Modale Fenster halten die WASD-/Tastensteuerung von
    der Welt fern und blockieren Click-through; ESC und
    F6 Chat / F7 Region / F8–F10 Bestand / F11 Vollbild.
    Breite via Height-Scale bleibt auf 16:9, 21:9 und 32:9
    konsistent, sowohl Koordinaten als auch Zeichenfläche.
12. Neue UI-Unit- und echte GL-Render-Tests mit synthetischen
    Daten/Viewportgrößen. Es bleibt Pflicht, die tatsächlichen
    Menüs nach NexVerse InWorld-Login zusätzlich visuell
    zu prüfen. Nur ein lokaler GUI-Smoke beweist diesen
    vollständigen Benutzerfluss NICHT.

**Abgrenzung:** Original Firestorm Full-Funktionsparität
ist NICHT erreicht; private/externen Links, Freunde,
Gruppen, Events, Marketplace, Bauen und Sprachchat
werden nicht als funktionierend vorgetäuscht.
Unveränderte offene Kernentwicklung: Firestorm LLKeyframeMotion
Assetclips, Avatar-Shape-Morphs, tatsächliche COF/Wear-
Transaktionen, Attachments-Bone-Rotation und Live-Welt-
Bewegung. **Ab Alpha.98 muss jedes dieser Features
im SELBEN Block Menü/Fenster/Button/Status und
Funktions- + UI-Tests erhalten, nicht nur C++-Backend.**

Quellen:
- FirestormViewer/phoenix-firestorm, Firestorm_7.2.4
  SHA 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
- opengenesislink/opengenesislinkserver main,
  SHA 4a2fc74e245bb948181db301b15246f915828fbc
  (13.10.26 19:18Z, neue oglGetRegionDimensions LSL API –
  kein automatischer Viewer-UI-Endpunkt).
- docs/VIEWER_UI_PARITY_PLAN.md ist ab jetzt der
  verbindliche UI-Paritäts-/Klickstatus pro Funktion.

## Alpha.98 – LLKeyframeMotion + sichtbares Animationsfenster (13.10.2026)

**Pflicht: Weiter parallel Backend und echte UI.**
Alpha.98 implementiert gemeinsam Decoder, GPU-Joint-Wiedergabe,
sichtbare Firestorm/NexVerse-Animationen-Floater, Maus-Interaktion,
Unit-Tests, GL-Abnahme und Übergabedokumentation.

1. Quellenstand: FirestormViewer/phoenix-firestorm Release
   Firestorm_7.2.4 @10bd3c9f930c76e1427ddd4ecece6cdf36b4406d;
   LLKeyframeMotion.cpp/h. OpenGenesisLINKServer main
   @86148202c69e1eb80f0845b82d39450da5e5d39c;
   beide am 13.10.26 neu abgefragt.
2. Format: LLKeyframeMotion Binary Version 1.0 / Legacy 0.1,
   U16 Version/Sub, S32 Priority, F32 Duration, CString Emote,
   F32 LoopIn/Out, S32 Loop, F32 EaseIn/Out, U32 HandPose,
   U32 JointCount, pro Joint CString Name, S32 Priority,
   S32 RotationCount, Time+Quat XYZ, S32 PositionCount,
   Time+Pos XYZ, S32 ConstraintCount.
3. Neuer Reader unter include/nexverse/assets/
   firestorm_keyframe_motion.hpp mit LE Endian Reader,
   finite/bounded Zeit-, Position- und Keycount-Pruefung,
   nur erlaubte Versionen, max 2 MiB, 256 Joints, 32768 Keys,
   verstaendliche Ablehnung falscher/trunkierter Daten.
   Legacy Zeit/Rotation als F32 Euler ZYX, modern U16 QuatVec3;
   Constraint/IK-Pakete aktuell explizit abgelehnt,
   nicht als fertig vorgetaeuscht.
4. Animationskurven: normalisierte LLQuaternion-Rekonstruktion,
   SLERP mit kuerzestem Weg und lineare Position-Interpolation,
   LoopIn/LoopOut-Zeitberechnung.
5. Original Firestorm Head/Upper/Lower-GLSL BodyMesh:
   RenderAvatarProxy besitzt optionale schreibgeschuetzte
   FirestormKeyframeMotion Referenz. Ist ein validierter Clip
   vorhanden, wendet der Shader dessen Gelenkrotationen an;
   mPelvis verarbeitet Positionskurven. Eigene temporäre
   Fallback-Bewegungen bleiben, wenn kein Clip vorhanden ist.
   Diagnose: avatar_keyframe_skin_draw_calls.
6. Das Live-Asset-Fetch per CAPS/AssetTransfer ist NOCH NICHT
   mit dem Renderer verbunden. High20 liefert AnimID/Sequenz,
   nicht automatisch Animations-Assetbytes! Ein GPU-Testclip
   ist kein bestaetigter serverseitiger Animationserfolg.
7. Viewer-Oberfläche parallel: Dropdown Avatar > Animationen,
   unteres Dock Animationen. Eigener ViewerWindow::animations
   mit Liste der echten High20-SelfAvatar AnimUUIDs und
   signierten AnimSequenceIDs, anklickbare Selektion und
   Detailstatus. Kein Fake-Play-/Stop-Button, solange Asset,
   Berechtigung und Server-Stopp noch fehlen.
8. Maus- und GPU-Test: menu_hit / animation-row selection
   in 1100/700, FullHD und Ultrawide; GL3.3-Fenster wird
   sichtbar per ViewerShellRenderer gezeichnet.
9. Parser-Golden-Tests aus synthetischer echter Binärstruktur,
   alt/neu, Quaternion/Loop/Position; jede Trunkierung,
   ungueltige Version/Priority/NaN/Constraints werden abgewiesen.
10. Renderer GPU-Test mit echtem Firestorm Original-LLM
    Skeleton/Mesh: explizit bereitgestellter Clip ruft
    3 tatsaechliche GPU-Skin-Draws auf.
11. Weiter offene Firestorm-Funktionsparitaet: AssetService-
    Fetch/Cache, StartStop SequenceID Reset, Prioritaets-
    und Layer-Blending, Ease/Fade, AnimSources/IK/Constraints,
    Bone-Normal-Skinning, Rigged-Avatar-Outfits/COF und
    Live-End-to-End NexVerse-Login.
12. Folgeblock Alpha.99 MUSS AnimationAsset-Fetch +
    Cache/Playback UND die passende GUI-Clip/Fehler/Loading/
    Priority/Loop-Anzeige in demselben GitHub-Commit enthalten.

Wichtige Dokumente:
docs/VIEWER_UI_PARITY_PLAN.md, docs/ALPHA98_KEYFRAME_AND_UI.md,
docs/FIRESTORM_PARITY_MATRIX.md und projectplan.md
werden bei jedem weiteren Block aktualisiert.

## Alpha.99 – aktueller OpenGenesisLINK GitHub-Check und Core/Plugin-Entscheidungen (13.10.2026)

Neue verbindliche Arbeitsregel vom Nutzer: Bei JEDEM
Entwicklungs-/Funktionsblock und direkt vor jedem
GitHub-Push Live-HEAD und Diff von
https://github.com/opengenesislink/opengenesislinkserver.git
über main prüfen, parallel das aktuelle OGL
develop-current HEAD/Projektdateien anschauen.
Automatischer Preflight:
python3 scripts/check_opengenesislink_server.py --strict
mit reviewed SHA
docs/OPENGENESISLINK_SERVER_BASELINE.json.
Neue SHA erst NACH manueller Dateiprüfung
in docs/OPENGENESISLINK_INTEGRATION_MATRIX.md
dokumentieren und das Ledger aktualisieren.
Exit 2: Serverchanges – Review nötig;
Exit 3: Serverstatus unbekannt. Nicht fälschlich
als aktuell behaupten oder Server-C# blind portieren.

Tatsächlich neu entdeckt: Beim ersten GitHubvergleich
Server main 86148202, beim zweiten Durchlauf 4 neuere
Commits bis 011ed5183889ebff72d10a49162b3223830d8ba5.
Neue Server-/Client-Bewertung:
* Nachbarregions-llListen Chat: ausschließlich SERVER-ONLY.
  Kein neues LLUDP-Paket; normaler ChatFromViewer reicht.
* Native LSL oglGetRegionDimensions: Server-Funktion;
  geplantes Scripteditor-Autocomplete als VIEWER CORE.
  Z ist NICHT über Viewer-RegionState bekannt;
  nicht erfinden.
* Wiederholtes Touch-Hold: Server generiert LSL touch;
  Viewer Grab/DeGrab als Firestorm CORE später testen.
* Neuester Robust-Inventory-Thumbnail-GETITEM
  bestätigt vor Erfolg gespeicherte UUID. CORE Thumbnail/
  Outfit-CAPS bleibt im Viewer; Cachetest unterscheidet
  ausgelassenes vs explizit leeres thumbnail-Feld.
* RTX, realistische Wettereffekte und weitere
  experimentelle Ergänzungen optionales Plugin,
  niemals Firestorm Standardfunktionen auslagern.

Parallel Code + sichtbare GUI Alpha.99:
Lokales Chatfenster besitzt Fluestern/Sagen/Rufen
Schaltflächen, markiert aktiven Modus, zeigt
ehrliches Feedback und sendet authentifizierte
LLUDP ChatFromViewer Typen 0/1/2 auf Kanal 0.
Server konfiguriert 10/20/100 m standardmäßig;
Viewer setzt Reichweite nicht selbst.
Maus-Hitbox und LLUDP-Byte-Regressionsprüfungen
implementiert; neue Python-GitHub-Gate-Tests.

Bewusst NOCH OFFEN: Die aus Alpha.98 bekannte
echte Animation-Asset-Fetch/Clip-Playback-Kette
liegt im naechsten Releaseblock (Alpha.100) mit
gleichzeitigem Animationsfenster/Loading/Status.
Kein Server-ChatCrossRegion-/Thumbnail-E2E-Test
ohne authentifizierten NexVerse-Login vorgetäuscht.

## Zweiter erneuter GitHub-Check direkt vor Alpha.99-Release

Beim verpflichtenden Release-Check von Server SHA 011ed518
meldete GitHub erneut **4 weitere Commits** bis SHA
2c7f2953681eda3fe0a66700e38621f31077d822
(13.10.2026 20:15:28 UTC). Betroffene Dateien
ausschliesslich Dokumentation:
doc/NexVerse/09312_RC1_STABLE_ACCEPTANCE.md,
doc/NexVerse/LSL_PARITY_09311_09312.md,
doc/NexVerse/ROADMAP.md.

**Release-Sicherheitsentscheidung:** OpenGenesisLINK
main ist im Stand 0.9.3.12 DEVELOPMENT,
**NICHT RC1 oder Stable**. Die veröffentlichte
Stable ist v0.9.3.10 (12.10.2026).
22 geplante Operator-/InWorld-Gates beginnen
NOT_RUN, kein CI-Gruen als Live-PASS ausgeben.
Insbesondere Native-OGL Login, Voice, COF/
Outfits, Firestorm Simulator-Login/Teleport,
Chat über Regionsgrenzen, Touch-Hold und
Inventory-Thumbnail-Reconnect sind nicht
durch Alpha.99-InWorld-Tests bestätigt.
OpenGenesisLINK Roadmap verschiebt OGL
Experiences 2.0, Pathfinding 2.0 und
My Outfits 2.0 bis NACH 0.9.4.0 Generation 1.
Das betrifft diese erweiterten OGL-Features,
nicht die Pflicht zu Firestorm Standard-
Funktionsparität im Basiskern.
Keine neue C++-Protokollcodeänderung nötig,
da der Upstream-Diff nur .md-Dateien umfasst.
Geprüften SHA nach manuellem Review aktualisiert.

## GitHub Nachprüfung Alpha.99: IAR GZip Streams

Nochmals 6 Server-Commits entdeckt: 2c7f295 auf
2254d2b3733c5fb171e6e608bc909ff1ef8a7acd
(13.10.2026 20:20:43 UTC). Änderungen in
NexVerse/RegionModules/Archives/
OglIarOperationManager.cs und
doc/NexVerse/IAR_CONSOLE_PROGRESS_09312.md.
Managed Stream-IAR exportiert nun komprimiertes
GZip/TAR und importiert über GZipStream; der
begrenzte Preflight/Archive-Inspector bleibt
vor mutierenden Importen verpflichtend.
**Einordnung SERVER-ONLY**, kein bestehendes
Firestorm-OGL-Viewer-Paket, kein Client-Plugin
für serverseitige Archivströme. Live-Archiv
Export/Import und TOCTOU-/Quota-Gates sind offen.
Die Revision nach Quellenreview in die
Server-Baseline übernommen. Alpha.99 fokussiert
weiter die Viewer-Core-Chatmodi und
korrekte GUI, keinen IAR-Servercode-Port.

## Alpha.100 – Server-Animationsassets + GPU + sichtbarer Viewer (13.10.2026)

Pflicht-Preflight: Vor Beginn OGL develop-current bf2c02b sowie
OpenGenesisLINK main 2254d2b3733c5fb171e6e608bc909ff1ef8a7acd
live abgeglichen. Beim Release vor GitHub Push main erneut prüfen.
Firestorm Tag Firestorm_7.2.4 scripts/messages/message_template.msg
und Server OpenSim/Region/ClientStack/Linden/UDP/LLClientView.cs
(HandleTransferRequest, MakeAssetRequest, SendAsset,
SendAssetNotFound) sind die Quelldokumente.

1. CORE, kein optionales Plugin: LLUDP TransferRequest Low153.
   TransferID UUID16, ChannelType S32=2, SourceType S32=2,
   Priority F32, Params Variable2=AssetID UUID16+AssetType
   Animation S32=20. Kein erfundener HTTP-/CAPS-GetAsset-Endpunkt.
2. Server-Antwort TransferInfo Low154 und TransferPacket High17,
   korrekte Var2-Offsets Info byte32, Packet byte28,
   strict Bounds, UUID/Channel/Type/Status/Size/Packet-Order.
   Status -2 bei OpenGenesisLINK SendAssetNotFound/Size0
   erscheint sofort als Fehler statt nur Timeout.
3. Neuer AnimationTransferInbox: Mutex, aktuelle RegionEpoch,
   TransferID, max 2 MiB, max 4096 Chunks, 4 Pending,
   16 Responses, out-of-order Rekonstruktion, Duplicate-
   Checks und Konflikt-Abbruch, stale TransferID ignoriert.
   Kein nicht gepruefter Clip wird gecacht.
4. Ein realer laufender Request ueber authentifizierten
   LLUDP OutboundCommandQueue/CircuitConnection, eingehende
   Info/Packet via PacketDispatcher. Nur aktive High20
   SelfAnimation UUIDs werden angefragt. Max 10s Timeout,
   keine unendliche Auto-Retry-Schleife.
5. Region-/Sim-Wechsel loescht alle Pending-Daten und Clip
   Cache. Bis 64 immutable Clips pro Region; CPU
   Firestorm LLKeyframeMotion Decoder validiert echte Bytes.
6. Renderer: auf echte Firestorm Head/Upper/Lower LLM 133-
   Joint GPU-Weights angewandte Clip-Kurven, Auswahl nach
   Clip-Prioritaet. Keine ungepruefte UUID als Assetdaten.
   Animationsbeginn wird pro Avatar+AnimUUID+SequenceID
   verankert, statt bei globaler Viewer-Laufzeit zu starten.
7. Optischer Viewer im GLEICHEN Block: Avatar>Animationen,
   Dock>Animationen, markierbare Zeilen, Server-Sequence,
   Live-Transferstatus, Asset geladen/N Joints,
   Asset fehlt/ungültig/Timeout und aktiver Download
   erneut-Button nur bei Fehler. Keine Fake-Play-/Stop-Klicks.
8. Testfälle mit echtem Firestorm/OpenSim Low153/Low154/
   High17 WireGolden, valide LLKeyframeMotion v1 Payload,
   Info nach Chunks, Duplikate, Out-of-Order, falsche
   IDs/Size/Type/Var2, -2 Notfound, Epoch-Reset,
   UI-Klick-/OpenGL- und original GPU Joint-Skin-Test.
9. Alle Projektdokumente, Versionsdatei und Projektplan
   im selben Github Commit aktualisiert.

Noch offen: Echte authentifizierte NexVerse InWorld-Tests
der AnimationAssetTransfers und GPU-Bewegung, Firestorm-
vollständiges Priority-/Fade-/Blend-Layering, IK,
AnimationSource-Sit, Gesicht, COF/Wear; nicht als
live validiert ausgeben. Alpha.101: Live-Protokoll-
Pruefung und parallele UI/COF-/ScriptEditor-Fortsetzung.
Jeder neue Block weiterhin mit vorangehender und
abschließender aktueller OpenGenesisLINK GitHub main Prüfung.

## Alpha.100 wiederholter Server-GitHub-Releaseabgleich

Beim vorletzten Live-GitHub-Preflight neue Server SHA
689c65ce9fa6d8816e8f3c3be3aa4d0e4bab50a3
(13.10.2026 20:25:25 UTC) ermittelt. Ein Commit nach
2254d2b: doc/NexVerse/CROSS_REGION_LSL_CHAT_09312.md,
tools/ci/verify_cross_region_chat.py,
tools/diagnostics/ogl_cross_region_chat_smoke.lsl.
CHAT01/CHAT02 erhalten einen LSL-Testwuerfel fuer
Region-A/Region-B, Chat Kanal0 und /7, Distanzfilter
20m, NexBus, Prozessgrenzen. Dokumentations-/Test-
Aenderung, KEINE neue LLUDP Client-Schnittstelle.
Einordnung SERVER-ONLY/Operator-Test, OGL Standard-
ChatFromViewer unveraendert. Die Smoke-Prozedur wurde
nicht real mit zwei Simulatoren ausgefuehrt und darf
nicht als PASS bewertet werden. Reviewed-SHA nach
Quellvergleich im Baseline-Ledger aktualisiert.

## Alpha.101 – Firestorm Objekt-Touch und Skriptkanal-Chat mit nutzbarer GUI (13.10.2026)

Erneuter Live-GitHub-Preflight vor dem Block:
OpenGenesisLINK main SHA
689c65ce9fa6d8816e8f3c3be3aa4d0e4bab50a3
und OGL develop-current 2820c5d, Arbeitsverzeichnis
vor Beginn sauber. Neuer Release-GitHub-Check ist Pflicht.

**Block A – Original Firestorm/OpenGenesisLINK Kernprotokoll:**
Nachrichtenvorlage Firestorm_7.2.4 scripts/messages/
message_template.msg und Serverquelle
OpenSim/Region/ClientStack/Linden/UDP/LLClientView.cs
Handler HandleObjectGrab, HandleObjectDeGrab,
HandleObjectGrabUpdate geprüft. Neue
include/nexverse/protocol/object_touch.hpp baut
reliable ObjectGrab Low117 mit AgentID/SessionID,
LocalID, GrabOffset vec3=0 und SurfaceInfo Anzahl 0,
sowie ObjectDeGrab Low119 mit Agent/Session,
LocalID und SurfaceInfo Anzahl 0.
Server akzeptiert leere SurfaceTouchEventArgs, weil
es sich noch nicht um präzises 3D-Mesh-Face-Picking
handelt. Kein angenommener FaceIndex/UV/Normal.
Beide Pakete benutzen die vorhandene
authentifizierte OutboundQueue/CircuitConnection.
OpenGenesisLINK erzeugt wiederholte touch()
bei stehendem Mausklick serverseitig,
nicht vom Viewer erfundene 100ms-Events.

**Block B – sichtbare Welt-Bedienoberfläche:**
Welt > Objekte beruehren öffnet einen echten
ViewerShell-Floater. Der Viewer verwendet nur
WorldModel-Prims (pcode prim) mit parent_id==0,
sortiert nach Abstand zu eigener Avatarposition,
bis fünf Root-Prims in höchstens 15 Metern.
Anklickbare Zeilen und hervorgehobenes LocalID-Target.
Objektliste wird unmittelbar aus verbundenem
Live-Simulatorzustand gefüllt, keine Attrappen.
Button BERUEHREN unter dem Cursor halten:
GLFW_PRESS -> ObjectGrab, GLFW_RELEASE ->
ObjectDeGrab; Release wird auch nach Dragging
aus dem Fenster oder ESC erfasst.
Aktive Berührung und Queue-/Erfolgsmeldung
sind im Fenster sichtbar. Server erhält
Grab/DeGrab, aber das Echo von touch() aus
einem echten Skript ist noch nicht beobachtet.

**Block C – Sicherheits- und Regionsschutz:**
Vor ObjectGrab wird erneut geprüft, dass das
ausgewählte Root-Prim noch im aktuellen
WorldModel und innerhalb 15 m existiert,
die Agentposition verfügbar und Region aktiv ist.
Nie Child/Attachment zufällig als fremdes Root-Prim
anfragen, oder LocalID=0/ungültige UUID verwenden.
Beim Regionswechsel werden Selected/Held/Requests
verworfen und keine alte LocalID im neuen
Simulator benutzt. Release über zuverlässig
gesendetes ObjectDeGrab, wenn Circuit noch läuft.
Wenn zwischenzeitlich getrennt, greift
serverseitiger Touch-Hold-Fail-safe; Live-Test offen.

**Block D – Kanal 7 für serverseitigen CHAT01/CHAT02-Test:**
include/nexverse/protocol/chat_channel_command.hpp
interpretiert Firestorm Standardnachrichten wie
/7 Beep und /-1729 Private als lokale Skriptchat-
Channels und sendet korrekten S32 Channel mit
ChatFromViewer, keine separate NexBus-Funktion.
Plain Text bleibt Kanal0, ungültige Kanalkommandos
oder leere Nachrichten werden nicht gesendet.
Chat-Floater zeigt angeforderten Kanal an.

**Abnahme:**
tests/object_touch_tests.cpp prüft LLUDP Low117/119
mit exakten Originalfeldern, Reliable Flag,
Agent/Session, LocalID, SurfaceInfo Count=0,
Inputfailclosed sowie /7 und negative Kanäle.
tests/viewer_shell_tests.cpp prüft Menü > Objekte,
Root-Prims Zeilenauswahl und BERUEHREN-Klick auf
mehreren Screenformaten. ViewerShell OpenGL3.3
Regression enthält echten Objekt-Floater.
Vollständige C++-/Python-/GUI-Tests vor Push.

**Grenzen/Nächster Block:**
Es handelt sich um eine echte berührbare
Listenauswahl, noch NICHT um Firestorm
Maus-Raycast im dreidimensionalen Weltobjekt,
präzises Face-/UV- und ChildPrim-Touching
oder echten Live-Grenzregionaltest.
Der serverseitige 100ms-Touch-Hold kann damit
grundsätzlich ausgelöst werden, Live-Nachweis
gegen NexVerse benötigt Anmeldung und LSL-
Testwürfel. Alpha.102 soll Original-Renderer-
Objekt-Raycast und Face-Hit-Info, danach
weitere COF-/ScriptEditor-Funktionen stets
parallel mit sichtbaren Menüs/Fenstern liefern.

OpenGenesisLINK main unbedingt bei jedem
Entwicklungsblock und unmittelbar vor GitHub
Commit/Push neu prüfen. Kernstandard nie
als Zusatzplugin auslagern; Firestorm/LLUDP
ist Viewer CORE.

## Alpha.102 – 3D-Welt-Klickauswahl und aktuelle Serverprüfung (14.10.2026)

Zu Beginn OGL develop-current SHA
3592d2e9f08daa40cc8ed1ee0e155da9a4887adf
und OpenGenesisLINK Server main SHA
2cc59ceaada1c6791ac8320d3d5638cae941b13a
live geprüft. Sechs neue Servercommits gegen 689c65ce
mit Archiv-Gates, deutscher Konsole, CrossRegion CI
und Dev-Releasekennzeichnung wurden nach Quellen
und Dokumentation beurteilt. VersionInfo.cs:
0.9.3.12 mit Flavour.Dev, NICHT RC oder Stable.

Architekturentscheidung: Neue Robust-Feature-Control
für OAR/IAR Import/Export liegt ausschließlich
beim Betreiber, signierte Node-Policies (ES256 P-256),
Admin API UserLevel >=200, kurze TTL/fail-closed.
Die PrivateKeys/NexBus-Keys werden NIEMALS
im Endnutzer-OGL-Viewer gespeichert, dieser benötigt
keinen /api/v1/feature-control Admin-PATCH.
Archive, Konsole, Testinfrastruktur SERVER-ONLY.
Standard Firestorm LLUDP und ObjectTouch bleiben
hingegen VIEWER-CORE und sichtbare Viewer-Funktion.

Viewer Neuerung: include/nexverse/render/world_object_picker.hpp
baut einen CameraRay passend zur OpenGL-Kamera
(60 Grad vertikal, Fenster-Aspekt, Mouse Top-Left).
Dieser schneidet gerotete BoundingBoxes (Quaternion
und Objekt-Skalierung) der aktuellen WorldModel
Root-Prims. Positiver nächster Treffer gewinnt.
NaN, negative/Null-Größe, Out-of-Screen,
Behind-Camera und zu weit entfernte Prims
werden abgewiesen; das ist bewusst noch
KEIN exaktes Mesh-/Sculpt-Triangle Picking,
keine erfundene FaceID/UV SurfaceInfo.

Im selben Block GUI: Linksklick in echte 3D-Welt,
nicht durch NexVerse Topbar/Dock/Nav/HUD/Modal,
liest auf Mainthread und unter WorldMutex den
authoritativen Agent, Kamera, Root-Prim-Transform.
Erneuter 15m-Abstandsschutz. Ein Hit aktiviert
echte ObjectGrab Low117 über bestehenden
authentifizierten Circuit und zeigt ausgewähltes
Objekt im Objekte-Floater. Neuer Button Objekte
im unteren Viewer-Dock und Welt-Menü. Ausgewählte
Prim bleibt sichtbar, auch wenn sie nicht zu den
fünf nächstgelegenen gehört. GLFW Release und
Alt-Tab-Fokusverlust senden DeGrab Low119;
Maustaste Press+Release im selben Poll-Frame
führt nicht zu dauerhaft hängenbleibendem Grab.
RegionEpoch verwirft veraltete LocalID/Pick.
Touch-Hold bleibt serverseitig; kein Client-Timer.

Tests: neue world_object_picker_tests.cpp mit
Ray-Intersection, Frontmost, Rotation 90 Grad,
invalid floats/scales, widescreen 1280/3440/5120
und hinter der Kamera. viewer_shell_tests.cpp
kontrolliert WorldHitboxen vs. HUD.
Existing original LLUDP Grab/DeGrab und
OpenGL-Shell-Tests bleiben als Release-Gates.

Noch offen: Triangle-BVH für echte Mesh-Objekte,
FaceIndex/UV/ST, ChildPrim PassTouches,
serverseitig nachgewiesene touch_start/touch_end,
COF-Wear/Unwear und vollständiger ScriptEditor.
Ein neuer authentifizierter NexVerse-Inworld-Test
fand nicht statt. Nächste Version erweitert
präzise Oberflächentreffer und Outfit/Scripting
PARALLEL mit ihren sichtbaren Fenstern.
Vor JEDEM folgenden Block und unmittelbar
vor dem GitHub Push aktuelle Server SHA erneut
prüfen und die Source/Feature-Matrix aktualisieren.


## Alpha.102 zweite Upstream-Nachpruefung: 0.9.3.12 Release-Label

Ein weiterer Server-Commit nach der anfaenglichen
Sechs-Commit-Pruefung: neuer main SHA
1841c4ee8ef580730be7d306023e594380f8b19f
(14.10.2026 16:41:53 UTC), aus 2cc59cea.
Die VersionInfo.cs ist nun auf Flavour.Release
gestellt; Dokumentation RELEASE_0.9.3.12.md
und 09312_RC1_STABLE_ACCEPTANCE.md betonen aber:
**KEIN** allein dadurch publizierter GitHub-Tag,
KEINE nachgewiesene produktive Installation und
KEINE vollstaendige 22-Punkte-Release-Evidence.
Die zuletzt unabhaengig verifizierte/tagged
Stable bleibt v0.9.3.10. Endnutzer-OGL darf
keine Server-Livefreigabe aus dem Code-Label
ableiten. Keine neue Client-LLUDP/CAPS-Funktion
aus diesen Release-Dokumenten abzuleiten.
Server-Baseline-SHA nach Review aktualisiert.

## Alpha.103 – Mesh Face/UV Picking plus sichtbarer Outfit-Refresh (14.10.2026)

Vor Entwicklungsbeginn waren OGL develop-current
2dc9cf7e7f9d72de292eaa89c251019812eda4e2 und
OpenGenesisLINK main 1841c4ee8ef580730be7d306023e594380f8b19f
per GitHub verifiziert. GitHub server main vor Release
erneut prüfen (scripts/check_opengenesislink_server.py --strict).
Firestorm Original-LLUDP ObjectGrab Low117/DeGrab Low119
sowie vorhandene GetMesh/InventoryDescendents2-CAPS sind
viewer-core, kein optionales Pflicht-Plugin. Robust
Feature Control (ES256 signierte OAR/IAR Operator-Policy)
bleibt ausschließlich im Server, nicht im User Viewer.
OpenGenesisLINK Server-Code 0.9.3.12 trägt Release Label;
separater GitHub Stable Tag, Deployment und
vollständige Abnahme sind davon zu unterscheiden.

(1) GetMesh-Dreiecke im Renderer statt nur Prim-BoundingBox:
include/nexverse/render/world_object_picker.hpp erweitert
den quaternion-korrekten CameraRay/OBB Broadphase um
Möller-Trumbore Triangle-Raycast in der echten GetMesh
Geometry (interleaved xyz, normals, uv, indices, FaceRanges).
Nearest positive intersection liefert LocalID, dreidimensionalen
Punkt, Face-Index und baryzentrisch interpoliertes UV.
Liegt das Klickziel innerhalb der Box aber zwischen zwei
echten Mesh-Dreiecken, wird KEIN falscher Treffer angenommen.

(2) GPU/CPU Cache kohärent: Nur nach erfolgreichem GPU-
Mesh-Upload erhält die CPU-Pick-Seite dieselbe
RenderMeshGeometry als immutable Assetkopie.
Separater Budgetdeckel 24MiB, einzelnes Mesh max 12MiB,
höchstens 65.536 Vertices bzw 65.536 Triangles pro
Asset. GPU Eviction und Regionwechsel räumen
auch CPU Pick-Geometrie auf. Bei nichtgeladenem Mesh
oder Sculpt-/parametrischen Prims weiter klar bezeichnete
OBB Näherung, keine vorgespiegelte volle Firestorm Parität.

(3) Sichtbare Viewer UI: Welt > Objekte beruehren zeigt
beim 3D-Klick auf residenten Mesh das exakte
Mesh Face und UV, bei Näherung den ehrlichen
BoundingBox-Hinweis. Selektionswechsel/RegionReset
verwerfen frühere Werte. LLUDP ObjectGrab/DeGrab
bleibt verlässlich, aber SurfaceInfo Count=0 bis
eine komplett korrekte Normale/Binormale/ST/Position
nach Firestorm/OpenSim Vorbild implementiert ist.
UI Face-Anzeige ist kein Beleg fuer llDetectedTouchFace.

(4) Outfit Galerie F9: echter neuer Aktualisieren-Button,
der an den bereits bestehenden authentifizierten
Firestorm InventoryDescendents2 read-only Sync
anknüpft (Owner/RegionEpoch, asynchron,
Rate Limit >=2s). Tatsächliche Outfitordner,
Thumbnailcache und COF-Readback werden aktualisiert.
Outfit wear/unwear und COF-Mutationsprotokoll
bleiben ausdrücklich gesperrt (can_wear=false);
niemals einen wirkungslosen Tragen-Button anzeigen.

(5) Tests: synthetic resident Triangle trifft
Face 7 und UV=(0.5,0.5), mesh hole ergibt
keinen Phantom-Treffer, ungültige Indizes,
NaN, 90° Quaternion, 16:9/Ultrawide,
LLUDP Wire-Golden, GUI und GPU-Smoketest.
Volle CTest/Python-Regressionssuite vor Release.

OFFEN: Verifizierter realer NexVerse in-world Touch,
vollständige LSL SurfaceInfo, BVH Mesh/ChildPrim,
COF CreateInventoryLink/Wear/Unwear und echte
Avatar-Appearance-Pipeline. Keine produktive
Server-E2E-Abnahme behaupten. Alpha.104
soll BVH/SurfaceInfo und vollständigen COF Core
in gemeinsamem Funktions+UI Release priorisieren.

## Alpha.104 – Firestorm TouchSurface und Outfit/COF UI (14.10.2026)

Live source precheck vor Entwicklung: OGL develop-current ac9384d358fe96ea8897c1f753fab7c51e57869a; OpenGenesisLINK Server main 1841c4ee8ef580730be7d306023e594380f8b19f. Vor Commit/Push erneut mittels scripts/check_opengenesislink_server.py --strict prüfen. Firestorm_7.2.4 scripts/messages/message_template.msg und indra/newview/lltoolgrab.cpp, Server LLClientView.cs HandleObjectGrab/HandleObjectDeGrab sind die technischen Referenzen. Alle Firestorm-Standardfunktionen im Viewer Core, nicht in Zusatzplugins.

Arbeitspaket 1: Echtes Firestorm ObjectGrab Low117 und ObjectDeGrab Low119 SurfaceInfo, optional eine Variable-Count=1 Struktur mit UVCoord Vec3, STCoord Vec3, FaceIndex S32, Position Vec3, Normal Vec3, Binormal Vec3, zusammen 64 Byte, authentifiziert und RELIABLE. Vorherige Beruehrung ohne genaue Meshdaten bleibt Count=0. Ungueltige/nonfinite Werte oder Basisvektoren werden abgelehnt.

Arbeitspaket 2: GetMesh CPU-Kamera-Triangleray aus Alpha.103 erzeugt bei gueltiger FaceRange und nicht-entarteter UV-Basis WeltNormal und WeltBinormal (Objektrotation und ungleiche Skalierung). OpenSim TextureEntry der ausgewaehlten Face liefert Repeat/Offset/Rotation fuer ST-Koordinaten. Keine falsche Face-Information fuer OBB-Naeherung, planar-texturierte Meshes oder fehlende Texturmetadaten. Region-/Objektwechsel setzt SurfaceInfo zurueck. Derselbe Hit fuer Grab und DeGrab.

Arbeitspaket 3: GUI Welt > Objekte beruehren zeigt Face/UV und ob die komplette SurfaceInfo tatsaechlich gesendet wurde, statt nur einen scheinbaren Treffer.

Arbeitspaket 4: Outfit-Galerie (F9) parallel erweitert: zwei echte Buttons Aktualisieren und COF pruefen. Neuer COF-Pruefen-Klick ist mit der vorhandenen authentifizierten FetchInventory2-Anfrage verbunden, avatareigentuemer- und regionepoch-gebunden, 2s RateLimit, niemals parallele/silent Mutationen. UI meldet Ladeauftrag, Ergebnis-Itemanzahl, Fehler oder fehlende CAPS. Alte Resultate beim Neuladen invalidieren und Regionwechsel setzt Anfrage/Status zurueck. include/nexverse/ui/outfit_gallery_layout.hpp macht Hitboxes testbar.

Arbeitspaket 5: Tests LLUDP Low117/119 64-Byte-Golden inkl S32 Face und 5 Vector3, invalid/Count0; Triangle-UV-/Normal-/Binormal-Regression; Outfit Floater Close/Refresh/COF-Pruefen/Folder-Hitboxen auf 1280x720, 1920x1080, 3440x1440, 5120x1440; kompletter C++23 Debug Build, CTests, Python und NVIDIA GL3.3 Smoke.

NICHT als implementiert behaupten: COF CreateInventoryLink/Wear/Unwear und AvatarAppearance Mutation mit Rollback, echtes InWorld LSL touch_start/touch/touch_end/touchUV/touchST, Sculpt-/Hollow-Prims und ChildPrim-PassTouches sowie Produktions-Login-E2E. Naechster Block: korrekter authorisierter COF-Write/Readback mit Transaktions- und Reconciliation-Tests, sichtbarer funktionaler Outfit-Wear/Unwear-Bedienung, Inworld-Facecheck. Vor jeder Arbeit und Release erneut aktuelle Server-HEADs pruefen.

## Alpha.104 verpflichtender GitHub Release-Preflight

Vor GitHub Release am 14.10.2026 wurde neuer
OpenGenesisLINK main SHA
5831913f3dc840aa9661b83fc844546f9f637070
gegen 1841c4ee entdeckt: 1 Commit, 6 Dateien.
Begutachtet OglApiDocsAccess.cs, NexApiDocsPage.cs,
NexVerseWorldApiConnector.cs und
doc/OpenGenesisLINK/WORLD_API_CONTROL_CENTER.md
sowie CI. Admin Anmeldung:
GET /api/v1/docs ist nur Login Shell,
GET /api/v1/docs/panel liefert Dashboard
nur mit HTTPS, authentifiziertem GridUser,
aktuellem LoginAllowed, UserLevel>=200,
AdminAll Scope und servervalidiertem Token.
Machine Keys sind kein Admin Login.
**SERVER-ONLY_OPERATOR_ADMIN_UI**, keine
neuen OpenSim LLUDP/CAPS Viewer-Pakete,
kein Admin-ControlCenter mit Adminbearer im
Endnutzer-OGL Viewer. OpenGenesisLINK ist
Serverprodukt, NexVerse ein konkretes Grid;
Legacy Namespaces und Header bleiben nur fuer
Abwaertskompatibilitaet. Live Admin
Login/Deployment ist nicht automatisch getestet.
Reviewed SHA in Baseline aktualisiert.

## Alpha.105 – COF-Protokoll, sicherer Aenderungsplan und Outfit-UI

Ausgangsstand OGL develop-current f788dd626d1cf6351db073a87f1b93cc0369cbd6, OpenGenesisLINK main 5831913f3dc840aa9661b83fc844546f9f637070; Skript scripts/check_opengenesislink_server.py --strict vor Arbeit und Release. Originale Firestorm_7.2.4 sources scripts/messages/message_template.msg / indra/newview/llviewerinventory.cpp und Server LLClientView.cs untersucht.

Block 1: Echte Firestorm LLUDP LinkInventoryItem Low426 und RemoveInventoryItem Low270 im Viewer-Kern implementiert: Agent/Session UUID, CallbackID, FolderID, null TransactionID, OldItemID, Type=AT_LINK(24), InvType, variable NUL-terminierte Name/Description; Removal InventoryData count=1 ItemID. RELIABLE Warteschlange und Circuit transportieren beide Packetvarianten. Hard reject invalid/zero UUIDs, Strings, CallbackID; Encoder ist **keine** vollstaendige Wear-Methode.

Block 2: Versioniertes COF Diff/Preflight (firestorm_cof_change_plan.hpp). Aus serverseitig gelesenem Outfit und COF werden echte Link-Ziele, Hinzufuegungen, Entfernungen und identische Eintraege berechnet. Fehlender COF-Readback/-Version, defekte IDs, Ordnerlinks, unaufgeloeste Links, doppelte Ziele, unbekannte Assettypen sowie >32 Aktionen werden explizit abgelehnt. Die Ergebnisse sind NUR VORSCHAU (plan.executable() immer false). Keine ungeprueften COF-Operationen aus UI in Sendewarteschlange.

Block 3: Echtes sichtbares F9 Outfitfenster um Aenderungsplan-Button und rechtsseitige Vorschau mit Anzahl +/−/=, Version, zu erstellenden und zu entfernenden Links ergaenzt. Bei Auswahlwechsel, neuen CAPS-Inventardaten, manuellem COF-Verify oder Regionenwechsel verfaellt die alte Vorschau. UI zeigt ausdruecklich 'keine Serveraenderung'.

Block 4: Strikter Readback-Reconciler fuer die spaetere freigegebene Transaktion: Ein lokales UDP ACK gilt NICHT als Wear. Nur neuer serverlesener COF mit passender RegionEpoch, mindestens erforderlichem Versionsfortschritt, ausschließlich gueltigen AT_LINK-Eintraegen und exakt passender Zielmenge kann confirmed liefern. Falscher RegionEpoch, Version, Itemtyp oder Zielmenge => fail-closed.

Block 5: Tests tests/firestorm_cof_planning_tests.cpp: Wire-Goldens Low426/270, Feldreihenfolge, Callback, NullTransactionID, var1 Stringlaengen, UUID/Input-Validation, Queue-Typen, Plan +/−/=, veraltete Version, unsupported folder links/duplicate/unresolved targets, Reconciliation positives/negatives. tests/outfit_gallery_layout_tests.cpp testet neue Button-Hitbox auf 1280/1920/3440/5120 Bildschirmbreiten. Vollstaendiger C++23 Debug Build, CTest, Python sowie NVIDIA OpenGL GUI Smoke vor GitHub-Push.

NICHT FERTIG: Multipacket COF-Wear/Unwear-Transaktionsdurchfuehrung mit ItemCallbackId-Mapping, serverseitigen Itemdaten/Appearance und Recovery bei Teilfehlern, echten Tragen/Ablegen-Buttons oder authentifizierter NexVerse-Inworld-Abnahme. OGL Viewer wird nicht als vollstaendig Firestorm kompatibel ausgegeben. Als Alpha.106 folgen expliziter Consent, servergepruefter Mutationsablauf/Rollback und Sichtbarkeit von Wear, nach belastbarem Protokollbeleg. Kern plus UI weiterhin gemeinsam.

## Alpha.106 – bestaetigte einzelne COF-Verknuepfung und reale Outfit-UI (14.10.2026)

Vor Entwicklungsbeginn live geprueft: OGL Viewer develop-current
ba138e0c04ca2593cf88b88b9bc97108eef98c63,
OpenGenesisLINK Server main
5831913f3dc840aa9661b83fc844546f9f637070
mit scripts/check_opengenesislink_server.py --strict.
Firestorm_7.2.4 llviewerinventory.cpp/message_template.msg
und OpenGenesisLINK LLClientView HandleLinkInventoryItem/HandleRemoveInventoryItem
geben das Originalprotokoll vor. Architektur: Firestorm
Inventory/COF im Viewer-Core, keine Plugins,
Robust-Admin API/PrivateKeys weiter server-only.

**Block 1 – Ein-Link-Mutationsschutz:**
include/nexverse/protocol/firestorm_cof_link_consent.hpp ist
ein echter gepruefter In-Memory-Zustandsautomat
idle/stage/confirm/waiting_server/verified/not_confirmed/
stale. Freigabe nur fuer COF ChangePlan mit genau
EINER neuen Link-ID und KEINER Entfernung.
Verlaessliche Quelle: owner- und RegionEpoch-
geprueftes InventoryDescendents2 mit positiver
COF-Versionsnummer, komplett validierten IDs
und vorhandener aktueller User-Sitzung.
Alle groesseren Outfitwechsel bleiben fail closed.

**Block 2 – ausdruckliche Zustimmung:** Im sichtbaren
F9 Outfit-Fenster kann der Nutzer im COF-
Aenderungsplan auf 'Einen COF-Link anlegen'
klicken. Der erste Klick bereitet nur vor,
der zweite Klick innerhalb 20 Sekunden bestaetigt
nach frischer Pruefung von RegionEpoch,
COF-Version, Zielitem, Auswahldaten und
Login/Simulationsbereitschaft. Nach Timeout,
Schliessen des Fensters oder Regionwechsel
wird unbestaetigte Freigabe verworfen.
Kein automatischer COF-Schreibzugriff.

**Block 3 – echter Versand mit Fallback:** Zweiter
freigegebener Klick stellt GENAU EINE
LinkInventoryItem Low426 in die bestehende
RELIABLE LLUDP CommandQueue/Circuit, mit
kontinuierlicher Callback-ID (nie Null).
Queue full -> kein falsches Erfolgsergebnis.
RemoveInventoryItem Low270 wird NICHT als
Outfitwechsel ausgelöst und keine bestehenden
COF-Links werden dabei gelöscht.
Link-Queue erfolgreich bedeutet ausdrücklich
NICHT erfolgreicher Wear oder Appearance.

**Block 4 – echte Serverbestaetigung:** Bereits vorhandener
CAP InventoryDescendents2 Readback wird fuer 18s
alle mindestens 2s erneut angestoßen, auch bei
geschlossenem F9-Fenster, aber NIE erneut
eine Link-Mutation gesendet. Erfolg erst,
wenn aktueller Server-COF eine hoehere Version
als vor Ausfuehrung meldet UND ein echtes
AT_LINK Item auf das genaue gewuenschte
Ziel verweist. Keine positive Aussage nur
auf Grund von LLUDP ACK oder lokaler Cachemeldung.
Fehlende Antwort -> 'nicht bestaetigt'.
Session/Regionswechsel -> unbekannter
Serverzustand, nie mutig erneut ausfuehren.

**Block 5 – COF pruefen repariert:** Bisherige
COF pruefen Schaltflaeche hatte die
On-Demand-FetchInventory2 (ausgewaehltes Outfit)
gestartet; das war keine tatsaechliche
COF-Pruefung. Jetzt wird vom Server ueber
InventoryDescendents2 *wirklich* der
Current Outfit Folder (FT_CURRENT_OUTFIT=46)
inklusive Version, Link-Item-Liste
und korrekter Owner/Region gelesen. Der Button
zeigt Server-Version und Anzahl Links oder
Fehlermeldung. FetchInventory2 bleibt fuer
aufzulösende einzelne Outfit-Item-Targets
separat weiterhin vorhanden.

**Block 6 – Stale-Response-Sicherheit:** Jedes
COF-Pruefen merkt eine erwartete neue Request-
Seriennummer und akzeptiert kein noch laufendes
altes CAPS-Ergebnis als Beleg fuer den
gerade gedrueckten COF-Pruefen-Button.
Regionwechsel setzt Seriennummer und offene
Verifikation zurueck.

**Block 7 – echte grafische Bedienoberflaeche:**
F9/Outfit-Galerie hat Aenderungsplan, genau
eine einzelne erlaubte Linkanlage, getrennte
Bestaetigungsbeschriftung und Statusmeldungen
fuer Request, ausstehende Serverantwort,
vom COF bestaetigte Verknuepfung oder Timeout.
Status bleibt auch bei geschlossenem Vorschau-
Overlay auf der F9-Oberflaeche sichtbar.
Bestätigung und Readback sind nicht
als visuell bestaetigtes Avatar-Outfit
gekennzeichnet.

**Block 8 – Regression/Tests:** Neue
tests/firestorm_cof_link_consent_tests.cpp
testet 20s Consent-Frist, 18s Readback,
Stale Epoch/Version/Target, mindestens
einseitiges No-Deletions Gate, 2-Klick-
Sperre und Ausbleiben automatischer
Resends. Erweiterte
tests/outfit_gallery_layout_tests.cpp
testen Linkbutton-Hitbox in allen
vier Bildschirmformaten. Bestehende
tests/firestorm_inventory_fetch_tests.cpp
verifizieren COF-Folder-ID, ServerVersion
42 und echten COF-Link per authentifiziertem
InventoryDescendents2 neben ausgewähltem
Outfit. Kompletter Build/CTest/Python und
Linux NVIDIA OpenGL Smoke bleiben
verbindliche Release-Gates.

**Offen – nicht als implementiert ausgeben:**
vollstaendiges Firestorm Wear/Unwear mit
mehreren Links, sicheren Loeschungen,
Callback-/Appearance-Bestätigung,
Serverseitigem Inventar-Rollback bei
Teilfehlern, erfolgreicher GPU-Avatar-
Appearance, authentifizierter NexVerse
Grid-Inworld Test mit Live-Outfit.
Alpha.107 priorisiert echte fehlertolerante
mehrstufige COF/Appearance-Verarbeitung,
nachvollziehbare Undo/Pending-Operationen,
Outfit Tragen/Ablegen nur nach serverseitiger
Ende-zu-Ende-Abnahme sowie GUI im selben Release.

## Alpha.107 – COF-Transaktionen und sichtbare GUI (14.10.2026)

PRECHECK: OGL develop-current 7b00962acf67ea5cd9e1e4588e660f453b863dd0, OpenGenesisLINK Server main 5831913f3dc840aa9661b83fc844546f9f637070. scripts/check_opengenesislink_server.py --strict vor Arbeit und Push, Server bei neuen Commits neu prüfen. Firestorm_7.2.4 scripts/messages/message_template.msg + llviewerinventory.cpp, OpenGenesisLINK LLClientView Handler LinkInventoryItem/RemoveInventoryItem sind Originalreferenz. Firestorm/OpenSim Kernfunktionen gehören direkt in OGL, keine Pflichtplugins; Robust Betreiber-Admin-Kontrollen bleiben SERVER_ONLY.

1. COF BatchAdd (include/nexverse/protocol/firestorm_cof_batch_add.hpp): 2-4 neue Links als sequenzieller Befehl mit expliziter 2-Klick-Bestätigung unter 20s, nur wenn KEINE Links zu entfernen sind und mindestens eine bestehende COF-Verknüpfung verbleibt. Kein gemischter Outfitwechsel.
2. Protokoll/Server: eine reale authentifizierte RELIABLE Low426 pro Schritt, niemals mehrere ungeprüfte Commands. Nach jedem Link owner-/RegionEpoch-gebunden InventoryDescendents2 Current Outfit Folder mit höherer Serverversion und GENAU den erwarteten, eindeutigen AT_LINK-Zielen. Erst danach folgende Mutation. Ein UDP ACK allein zählt nicht als serverseitiger Erfolg.
3. Fehlerbehandlung: Regionwechsel, Loginende, Queue Full, fehlende oder widersprüchliche Serverantwort, 18s Timeout, alte COF-Version oder fremde Links => STOPP mit Teilfortschritt, KEIN Auto-Retry, KEIN zerstörerisches Auto-Rollback. Nach unbestimmtem Ergebnis sind weitere COF-Writes bis zum vom Nutzer ausgelösten frischen COF-Readback gesperrt.
4. Einzel-Link-Ablegen (include/nexverse/protocol/firestorm_cof_unlink_consent.hpp): exakt EIN RemoveInventoryItem Low270 nur bei einem zu entfernenden Link, keinen Additions, mind. einem erhaltenen COF-Link, echter doppelter Benutzerbestätigung, Versions-/Epoch-Prüfung. Server-Readback muss höhere Version, verschwundene konkrete LinkItemID und unveränderten erwarteten Restbestand belegen. Kein automatisches erneutes Löschen.
5. F9 GUI parallel: Der Outfit-Änderungsplan zeigt bei 2-4 ADD/no REMOVE den aktiven Button '2-4 Links sequenziell anlegen'. Bei exactly 1 REMOVE/no ADD zeigt die gleiche Stelle 'Einen COF-Link ablegen'. Erste Aktion vorbereitet; zweite BESTAETIGEN. Progress N/M, Erfolgs- und Fehlerzustände sichtbar, auch nach Schließen beim bereits gestarteten Server-Readback. Nicht ausgeführte Bestätigungen beim Schließen verworfen; neuer COF-Status nicht als fertiger Avatar-Look deklariert.
6. Mutationsausschluss: Single Link Add, Multi-Add und Single-Unlink nie parallel; Unsicherheit sperrt alle weiteren, neue echte CURRENT OUTFIT FOLDER CAPS Anfrage hebt Sperre erst nach erfolgreichem Readback auf. Stale CAPS-Antworten und unautorisierte Sessions dürfen Freigabe nicht ersetzen.
7. Tests: tests/firestorm_cof_batch_tests.cpp und tests/firestorm_cof_unlink_tests.cpp prüfen sequentielle Bestätigung, richtige Link-IDs, Versionen, fehlende/missgebildete Server-Liste, Timeout, unerwartete Änderungen, Regionwechsel, Queue failure, nur 2-4 ADD ohne DELETE oder eine sichere Entfernung. Single-Link Consent Regression auf fail-closed Unklarheit aktualisiert; F9 Hitbox auf 1280,1920,3440,5120 getestet. Vollständiger C++23 Build, CTest, Python sowie Linux OpenGL GPU GUI-Smoke.
8. Übergabe/Dokumentation: VERSION, projectplan.md, info.md, CHATUEBERGABE.md, Firestorm-/UI-/Server Integrationsmatrix und docs/ALPHA107_COF_BATCH_UNLINK_UI.md im selben GitHub Commit aktualisieren.

OFFEN: Vollständiges Firestorm Wear/Unwear mit gemischten Add/Remove, bestätigter AvatarAppearance, GPU Outfitvisualisierung und robustem atomarem Rollback. Das serverseitige Bestätigen einer COF-Verknüpfung bedeutet NICHT, dass der Avatar sie sichtbar trägt. Ein authentifizierter NexVerse E2E Inworld-Test wurde nicht durchgeführt. Nächster Release priorisiert Server Appearance/E2E, echte vollständige Wear-/Unwear-Knöpfe erst nach bewiesener Ende-zu-Ende-Funktionalität.

## Alpha.107 zusätzlicher Release-Serververgleich (14.10.2026)

Während der Alpha.107 Release-Pflichtprüfung erschien ein neuer OpenGenesisLINK-Server Commit: 2b7c19c9396eb2f75fd60a41a33a05655909da6b (17:50:43 UTC, #207). Vorher 5831913f3dc840aa9661b83fc844546f9f637070. Fünf Dateien manuell geprüft: OglApiDocsAccess.cs, NexVerseWorldApiConnector.cs, Robust.ini.example, Robust.HG.ini.example und tools/ci/verify_ogl_docs_admin_ui.py.

Inhalt: abgesicherte AdminDocsTrustedProxyAddresses für TLS-Termination, verifizierte Peer-IP (Loopback oder explizite Allowlist), exakter X-Forwarded-Proto https, keine Annahme untrusted X-Forwarded-For. Anonyme Dashboard-Zugriffe bleiben 401; bei Admin-Bearer kann Proxy-HTTPS-Konfigurationsfehler 426 statt irreführendem 401 gemeldet werden. Owner/GridUser UserLevel >=200 und AdminAll Scope bleiben zwingend. Klassifikation SERVER_ONLY_OPERATOR_ADMIN_SECURITY: weder neue LLUDP/CAPS-Benutzer-Viewer Funktion noch OGL Plugin, kein Token oder Admin-Schlüssel im Desktop Viewer.

Source Review abgeschlossen; geprüften SHA-Pin auf 2b7c19c... aktualisiert. Release-Gate danach erneut ausführen.

## Alpha.108 – AvatarAppearance COF-Version und optische Outfit-GUI parallel (14.10.2026)

UPSTREAM: Vor jeder Entwicklungsaktion OGL develop-current SHA c6b19b7e7dc86643643e40499d8e967cba722ac4, OpenGenesisLINK Server main SHA 2b7c19c9396eb2f75fd60a41a33a05655909da6b mit scripts/check_opengenesislink_server.py --strict; unmittelbar vor Release erneut prüfen. Firestorm_7.2.4 scripts/messages/message_template.msg: AvatarAppearance Trusted LLUDP Low158 Sender ID, IsTrial, TextureEntry, VisualParam sowie AppearanceData AppearanceVersion U8, CofVersion S32, Flags U32. Original Firestorm als Core Referenz, Server als OpenSim Protokollquelle; kein User-Admin-Bearer/FeatureControl in Viewer.

1. Neuer read-only Appearance-Evidenzpfad include/nexverse/protocol/firestorm_appearance_verification.hpp. Ein erfolgreicher COF-Write-Queue-Eintritt (SingleAdd, BatchAdd, SingleUnlink) merkt RegionEpoch und aktuelle Anzahl authentifizierter SELBST-AvatarAppearance-Serverpakete (atomarer self_appearance_serial). Erst nach realer COF-Serverbestätigung des finalen Links mit nachweisbarer COF-Version wird die erwartete Appearance CofVersion gesetzt. Akzeptiert wird ausschließlich ein seit Mutationsstart neu empfangenes self AvatarAppearance vom authentifizierten Agenten auf gleicher Region, kein Trial, mit exakt erwarteter CofVersion sowie nichtleeren TextureEntry und VisualParams. Kein Optimistic-Wear.
2. Beobachterzustände: WaitingCOF, WaitingAppearance, StaleAppearance, UnexpectedNewerAppearance, MatchingServerAppearance, TimedOut, WrongRegion. Keine passenden Nachrichten binnen 30s nach bestätigtem COF oder COF-Mutation nach 120s => klarer Nichtnachweis. Paketversionsabweichungen und fehlende TextureEntry/VisualParams nicht als Erfolg deklarieren. Ein korrektes Serverpaket beweist NUR, dass die Appearance-Version empfangen wurde, NICHT dass Bake-/Mesh-/GPU-Darstellung funktioniert.
3. Main Thread/Packet Dispatcher: Schon vorhandenes decode_avatar_appearance + WorldModel.apply_self_appearance auf dem aktiven Simulator-Circuit verwendet. self_appearance_serial inkrementiert nur bei akzeptierten SELF Paketen innerhalb WorldModel-Mutex; kein neues fingiertes Server-CAPS/REST-Protokoll. COF-Verified Event aus Alpha106/107 triggert nun die zweite Evidenzprüfung. Region- und Sessionwechsel invalidieren beide Evidenzketten.
4. F9 Outfit-Galerie: Neuer echter Button Appearance pruefen (read-only Status des zuletzt empfangenen eigenen AvatarAppearance aus WorldModel im Vergleich zur aktuellen CurrentOutfitFolder InventoryDescendents2 CAPS Version). Zeigt tatsächliche Simulator-CofVersion, CAPS-CofVersion und ob sie zusammenpassen. Während laufender Mutation zeigt die Galerie fortlaufend präzise Appearance-Trackingstufe samt Warnung, dass sichtbare Bakes noch nicht abgenommen sind.
5. GUI-Layout: Outfit floater war 500 skalierte Pixel hoch; zehn Outfit-Zeilen endeten bei y+412, die zusätzlichen COF-Statuszeilen begannen vor y+412 und überlagerten Einträge. Auf 675 skalierte Pixel erweitert, Linkstatus und Appearance-Status als eigener Footer unter der Ordnerliste, Buttons in eindeutigem F9 Panel. Dieselbe OutfitGalleryLayout Berechnung steuert nun Mauskoordinaten UND Draw-Geometrie; Höhe adaptiv bis 480 Pixel Displays, Ultrawide/16:9 validiert.
6. Tests: tests/firestorm_appearance_verification_tests.cpp: gültige Version, andere Avatar-UUID, Trial-Paket, fehlende AppearanceData, stale/neuer, leere Texture/VisualParams, alte Paketseriennummer, Timeouts 30/120s, Regionwechsel, erfolgreiche genau versionierte Nachricht. tests/outfit_gallery_layout_tests.cpp: Appearance-Button, getrennte List/Statuszonen, 480/600/720/1080/1440 Bildschirmhöhen, 16:9 und Ultrawide. Alte 96 CTest und 17 Pythonunittests bleiben Pflicht; Linux RTX4070 SUPER OpenGL3.3 GUI Smoke vor Push.
7. VERSION, projectplan.md, info.md, CHATUEBERGABE.md, UI- und Firestorm-Kompatibilitätsmatrix, Serverbaseline und docs/ALPHA108_APPEARANCE_UI.md als ein Commit. Nach Commit SHA mit lokal, Tracking, GitHub vergleichen.
8. Architekturoffen: Firestorm AgentIsNowWearing Low383 und AgentSetAppearance Low84 erfordern erst korrektes Mapping der Wearable-Types, COF-Link-/Inventory-Wearable-Flags, VisualParam-/CachedTexture/Bake-Pipeline. Ohne diese Prüfung keine mutierende Appearance-Fake-Pakete versenden. Vollständiges Outfit tragen/ablegen mit kombinierten Add+Remove, sicheren Rollbacks, Bakes, Avatar GPU-Ankleiden und echtem NexVerse-InWorld-E2E weiterhin offen; den Umfang nicht als erledigt deklarieren.

NÄCHSTER BLOCK Alpha.109: Firestorm Wearable-Type Zuordnung aus echten Inventory Flags/AssetData und WearableSlots, AgentIsNowWearing/AgentSetAppearance Protokolle sauber modellieren und absichern, echtes Outfits-Wear/Unwear mit sichtbaren Buttons nur nach vollständiger serverseitiger COF+Appearance+GPU-Abnahme. Parallel Renderer/GUI und Core wie bisher, nach jedem Block GitHub/Markdown aktualisieren.

## Alpha.109 – Original Firestorm Wearables, Appearance-Pakete und sichtbare F9-Bedienung (14.10.2026)

GitHub Precheck: OGL Viewer develop-current 984fbabfaf03135b4c399caacedc81317ca9a95d und OpenGenesisLINK Server main 2b7c19c9396eb2f75fd60a41a33a05655909da6b mit striktem Server-Check geprüft. Original Firestorm_7.2.4 llwearabletype.h/.cpp, llinventorydefines.h sowie message_template.msg als Wire-/Datenreferenz. Betreiber-Admin-APIs bleiben SERVER_ONLY.

Block 1: Besitzer- und RegionEpoch-gebundene InventoryDescendents2 und FetchInventory2 speichern jetzt optionales U32 Flags-Feld, fehlende Flags niemals als WT_SHAPE=0 behandeln. COF Linkflags niemals als Original-Item-Flags interpretieren.

Block 2: Firestorm Wearable-Typen WT_SHAPE0 SKIN1 HAIR2 EYES3 SHIRT4 PANTS5 SHOES6 SOCKS7 JACKET8 GLOVES9 UNDERSHIRT10 UNDERPANTS11 SKIRT12 ALPHA13 TATTOO14 PHYSICS15 UNIVERSAL16 exakt aus InventoryFlags SubtypeMask 0x0000ff. Original Bodypart AssetType13, Clothing AssetType5, wearable InvType18, vier obligatorische Shape/Skin/Hair/Eyes. Multiwear-Layer ohne echtes Bake und kaputte/doppelte Links werden explizit blockiert, Attachments getrennt gezählt.

Block 3: Im Viewer-Core stehen zwei Original-LLUDP-Paketencoder bereit: AgentIsNowWearing Low383 mit AgentID, SessionID, 17 WearableData UUID/U8 und AgentSetAppearance Low84 mit Serial, AvatarSize Vector3, CacheUUID und TextureIndex, TextureEntry Variable2, VisualParam Variable1. Registrierte MessageIDs in LLUDP, begrenzte Laengen, falsche IDs/Cache/Size abgelehnt, synthetische Bytegolden-Tests. Kein mutierender Versand: echte tragbare Assets, passende Layer, Bakes, CachedTexture und AvatarSize müssen zuerst verifiziert werden.

Block 4: F9 Outfit Galerie hat echten Button Wearables pruefen, der originale COF-Zielitem-IDs serverseitig per authentifiziertem FetchInventory2 abruft, auf 32 COF Links begrenzt, Duplikate verwirft und Antworten nur bei gültigem Agent/RegionEpoch und unveränderter COF-Version akzeptiert.

Block 5: Sichtbare rechte F9 Wearable-Ansicht listet die 17 originalen Firestorm Slots Shape bis Universal samt Anzahl, sobald der echte Server Readback vorliegt. Darunter echte Fehlermeldungen zu fehlenden Subtypeflags, inkompatiblen Assettypen, doppelten Links, fehlenden Bodyparts und unaufgelösten Originalitems. Bildschirmlayout nutzt gemeinsame Hitbox/Draw Geometrie in 800x480, 1024x600, 1280x720, 1920x1080, 3440x1440, 5120x1440. Keine irreführende Wear-Erfolgsanzeige.

Block 6: Neue tests/firestorm_wearable_slots_tests.cpp pruefen 17 Slot-Daten, Null-/Missing Flags, Shape0, Bodypartvollständigkeit, Subtypefehler, Multiwear, COF-Konflikte, Low383/Low84 Golden-Packet, CacheIDs und MTU-Validierung. Alte Owner-CAPS-Tests mit Flagserhalt ergänzt und F9 Klickbereich getestet.

Block 7: Vollständiger C++23 Debug Build, CTest/Python, Linux NVIDIA GPU/OpenGL GUI-Smoke, Server-GitHub Check vor Push, Git commit/push develop-current mit dreifacher SHA-Prüfung und sauberem Worktree bleiben Release-Gates.

Block 8: VERSION, projectplan.md, info.md, CHATUEBERGABE.md, Firestorm/UI/Server-Kompatibilitätsmatrix und docs/ALPHA109_WEARABLE_PROTOCOL_UI.md gemeinsam fortführen.

Offen Alpha.110: Wearable Assetformate decodieren, Firestorm CachedTextures/Bake/Layers, Avatar Morph/Attachment GPU-Outfit synchronisieren; echte vollständige Tragen/Ablegen Aktion erst nach vollständig bestätigtem Server COF, Wearable und AvatarAppearance und visuell geprüftem NexVerse Inworld Avatar. Keine ungeprüften Appearance-Pakete versenden und keinen fertigen Outfitwechsel behaupten.

## Alpha.110 – echte Firestorm LLWearable Assets, LLUDP Transfer und Bake-Layer Status in F9 (14.10.2026)

UPSTREAM: GitHub Viewer develop-current 22f78a4934a91208327ae47d3e5c554315100526 und OpenGenesisLINK Server main 2b7c19c9396eb2f75fd60a41a33a05655909da6b live geprüft; strict Servercheck erfolgreich. Firestorm_7.2.4 indra/llappearance/llwearable.cpp, llavatarappearancedefines.h/.cpp, scripts/messages/message_template.msg als verbindlicher Daten- und Protokollvertrag. AdminProxy-/Robust-Operator APIs und Signaturschlüssel SERVER_ONLY, kein Plugin für originale Firestorm Wearable-Basics.

**Block 1 – LLWearable v1 Text-Parser im Viewer-Core**: Neu include/nexverse/assets/firestorm_wearable_asset.hpp. Bounded bis 2 MiB, maximal 8192 Zeilen / 2047 Zeichen pro Zeile, Legacy Header und Version1, Name, Beschreibung, permissions0, sale_info0, Inventar-Wearabletype 0..16, maximal 1000 eindeutige VisualParams ID/Float und 100 eindeutige lokale TextureEntry UUIDs. Negative, falsche UUID, doppelte IDs, Infinity/NaN, fehlerhafte Blöcke, Typunterschiede zum Original-Inventory werden verworfen. DefinitionVersion24 Legacy Migration und unbekannte künftige Formate werden explizit NICHT als vollständig kompatibel ausgegeben.

**Block 2 – original LLUDP Asset Anfrage**: include/nexverse/protocol/wearable_asset_transfer.hpp verwendet Firestorm/OpenSim TransferRequest Low153, Channel2/Source2, Params AssetUUID und AssetType5 (Clothing) /13 (Bodypart). Kein fingierter HTTP Endpoint. Separate WearableTransferInbox mit RegionEpoch, zufälliger TransferID pro Anfrage und höchstens vier zeitgleichen Pufferungen. Antworttypen TransferInfo Low154 und TransferPacket High17 inkl. Typ/ID/Gesamtlänge/Segmentindex/Endflag, geordnete Rekonstruktion auch bei Out-of-Order, 2MiB und 4096-Paket-Limits, fehlende/duplizierte/korrupte/negative Antworten fail closed.

**Block 3 – echter Circuit-Dispatch**: OutboundCommandQueue und src/protocol/circuit.cpp können nach eigenem Benutzerklick WearableAssetRequest per zuverlässigem LLUDP versenden. PacketDispatcher für TransferInfo/Packet teilt eingehende Simulationstransfers anhand ihrer TransferID getrennten AnimationTransferInbox und WearableTransferInbox zu. Regionwechsel setzt Inbox zurück; andere Regionen und fremde Assettypen werden ignoriert. Keine selbstständig wiederholte Wearable-Mutation, kein Outfit-Auto-Wear.

**Block 4 – F9 Download-Aktion**: F9 Outfit-Galerie besitzt zusätzlichen funktionalen Button 'Original-Wearable-Assets laden' in eigener linker Bedienzeile ohne Konflikt mit COF Batch Button. Nach vorherigem 'Wearables pruefen' werden nur echte owner-/region-/COF-Version-geprüfte originale Wearable Item-AssetUUIDs angenommen, bis 16 unterschiedliche IDs, nur 5/13 und InvType18. Nur bewusster Klick startet serialisierte authentifizierte Downloads mit 12s Timeout pro Asset und ohne blinde Auto-Retry. Die rekonstruierten Assetheader werden gegen die Flags des Original-Items geprüft. COF/Region Wechsel löscht Ansicht und verwirft alte Ergebnisse.

**Block 5 – Firestorm Original 45 TE Indizes und 11 Baked Outputs**: llavatarappearancedefines.h bestimmt TEX_NUM_INDICES=45, gebackene TE Indizes 8,9,10,11,19,20,40..44; llavatarappearancedefines.cpp definiert lokale Quellen je Bake-Region. Der Parser ordnet lokale Wearable Texturen den Originalgruppen Head, Upper, Lower, Eyes, Skirt, Hair, LeftArm, LeftLeg und Aux1-3 zu. Bereits gebackene Indizes sind OUTPUTS und dürfen nicht als belegte lokale Inputs gezählt werden. Eine lokale TextureUUID oder parsbare VisualParam bedeutet weder JPEG2000-Pixel noch fertige TextureBake.

**Block 6 – optischer F9 Status gekoppelt**: Download-Button im linken Bereich y447..474, rechts 17 originale Slots, unten separater durchgängig sichtbarer Assetfortschritt mit Anzahl dekodierter Wearables, VisualParams/TextureUUIDs und korrekt getrennten Bake-Layer-Referenzen. Layout und Hit-Test teilen exakt dieselbe Koordinate, 800x480 bis Ultrawide 5120x1440. Ein bestätigter COF, heruntergeladenes LLWearable und fertige GPU Darstellung bleiben verschiedene Statusstufen.

**Block 7 – verifizierte C++ Regressionen**: tests/firestorm_wearable_asset_tests.cpp für echtes Firestorm LLWearable v1 Format mit name/permissions/sale/type/param/texture und Negativfällen; tests/firestorm_wearable_transfer_tests.cpp für Low153 golden bytes, korrekte Typen 5/13, TransferInfo, multi-segment/out-of-order, unbekannte Region/UUID/Type, Server Asset NotFound, ungültiger/duplizierter Transfer. Outfitgallery Hitbox um Asset Button bei sechs Monitorgrößen ergänzt. Alter CTest/Python/Renderer/GPU GUI Release Gate bleibt unverändert.

**Block 8 – GitHub/Server und Übergabe**: VERSION 0.2.0-alpha.110, projectplan.md, info.md, CHATUEBERGABE.md, Firestorm/UI/Server Parity Matrix, docs/ALPHA110_WEARABLE_TRANSFER_BAKE.md, erneuter Check OpenGenesisLINK main und GitHub Commit/Push mit lokal=Tracking=Remote SHA; vollständiger Debug Build/CTest/Python plus Linux NVIDIA OpenGL 3.3 Welcome Smoke.

**Noch offen für Alpha.111**: Tatsächliche lokale JPG2000-TextureAssets via GetTexture/UDP laden und dekodieren, Firestorm TexLayer-XML/Masken und Shader-Layer-Komposition, ReBake/CacheID und Upload zum OpenGenesisLINK Simulator, korrekte visual_params/morphs und sichtbares AV GPU Mesh/Texture Skin, vollständiger Outfit-Wear/Unwear mit Add+Remove, reversibler Teiltransaktion und authentifizierter NexVerse E2E Abnahme. LLWearable heruntergeladen bedeutet ausdrücklich NICHT angezogen oder gerendert. Alle AgentSetAppearance/AgentIsNowWearing Mutationen bleiben bis zur belegten Pipeline deaktiviert.

## Alpha.110 aktualisierte Live-Server-Prüfung (14.10.2026)

Nach Entwicklung, vor Release-Gate, neues OpenGenesisLINK main SHA 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94, Commit #208 ('simple GridUser Level 200 Control Center login'); Altstand 2b7c19c9396eb2f75fd60a41a33a05655909da6b. GitHub Commit-Patches und Quellcode MANUELL geprüft: NexApiDocsPage.cs, NexVerseWorldApiConnector.cs, OglApiDocsAccess.cs sowie die beiden CI-Checks. Nur 1 Commit/5 Dateien. Operator Control Center wechselt von eigener '/api/v1/auth/admin/session' plus vorheriger AdminAll/TLS-Proxy-Absicherung zum normalen GridUser '/api/v1/auth/session' mit serverseitigem UserLevel >=200. TrustedProxyAllowlist/TLS-Sondercheck wurden aus dem Admin-Dashboard-Flow entfernt.

Klassifizierung: SERVER_ONLY_OPERATOR_ADMIN_AUTH_REVIEW, **NICHT** im normalen OGL Viewer implementieren, keine Admin-Tokens oder WorldAPI Operator-Einstellungen im Viewer. Kein Änderung am OpenSim Firestorm LLUDP TransferRequest153/Info154/Packet17 oder User-Inventory-CAPS. Sicherheitshinweis: Server-Betreiber muss bei diesem vereinfachten Dashboard-Login echte HTTPS-Terminierung und Rollen-/UserLevel-Grenze vor Exposition prüfen; das wird durch eine Viewer-Regression nicht automatisch abgedeckt. Server-Baseline nach Review auf 5d9c616... gesetzt; strikter Check anschließend erneut.

## Alpha.111 – reale Firestorm GetTexture PNG/JPEG2000 und F9 Wearable-Bildvorschau (14.10.2026)

Baselines: Viewer develop-current vorher 997d174bbbf0c39d9842b2314e5b63868aa5c494; OpenGenesisLINK Server main 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94; Original Firestorm_7.2.4 llavatarappearancedefines.h/.cpp, TexIndex/ETextureIndex 0..44, 11 Bake Groups. Server bei jeder relevanten Entwickleraktion live erneut prüfen und neue Commits manuell klassifizieren (VIEWER_CORE vs PLUGIN vs SERVER_ONLY).

1. Core Planner include/nexverse/assets/firestorm_wearable_texture_plan.hpp: Jeder Original Wearable-Asset TextureEntry Index muss der original Firestorm-Wearableklasse entsprechen; die elf bereits gebackenen Ausgabeslots (8,9,10,11,19,20,40..44) sind KEINE lokalen Downloadquellen. Nur UUID-valide lokale TE mit original 11 Bake Source Group, dedupliziert und auf 12 GetTexture Assets begrenzt. Truncated/Invalid/Baked Counts separat.
2. Bereits existierenden OGL TextureAssetLoader verwenden: Authentifiziertes Region CAPS GetTexture mit PNG und x-j2c/JPEG2000 Fallback, Dekodierung RGBA8 einschließlich Alpha; niemals einen erfundenen Asset-Endpunkt bauen. In F9 nur nach explizitem Texturen-laden-Klick und zuvor vollständig erfolgreichem owner-, region- und COF-versionierten Original LLWearable-Download, ohne offene Fehler oder Mutationsbefehle. Sequenzielle std::async Downloads, maximal 12 Bilder, Downsampling durch vorhandene texture_mip_image auf 128px.
3. F9 Oberfläche parallel: zwei getrennte tatsächlich klickbare Buttons Wearable-Assets laden und Texturen laden, eigene Hitboxen mit Abstand, daneben 17 echte Firestorm Wearable Slots und OpenGL-Miniaturbild neben jeweiligem Slot, sofern ein JPEG2000/PNG Original geladen wurde. Separate Status-Zeile GetTexture geladen/gesamt und Fehlertext; Footer als 10 nicht überlappende Zeilen y481..651 unter den Buttons (y447..474). Alle Fenstergrößen 800x480, 1024x600, 1280x720, 1920x1080, 3440x1440, 5120x1440.
4. Das Bild ist die lokale unveränderte Quell-Textur; KEIN sichtbarer Avatar-Wear, KEINE korrekte TexLayer Composite-Bake. Pro Wearable maximal ein kleines Vorschau-Bild, keine Übertragung zu Avatar Skin Shader, kein Upload einer angeblich fertigen Bake, kein AgentSetAppearance oder AgentIsNowWearing mutierender Send.
5. GetTexture Ergebnisformat enthält immutable RegionEpoch, COF-Version, COF UUID, Asset UUID, Wearable-Typ und Bake-Gruppe. Nur erwartete UUID unter aktuell authentifiziertem Grid, aktuellem Simulator und COF-Version auf GUI-Thread als max128px RGBA8 GPU-Texture upload. Alte asynchrone Antworten ignorieren; GPU Handles bei Region/COF/neuer Wearable-Asset-Lesung/Viewer-Shutdown löschen.
6. Wichtiges Alfa110 Bugfix: Wiederholter unveränderter Firestorm InventoryDescendents2 Region Readback hat bisher geprüfte Wearable-Inventar- und LLWearable-Daten jedes Mal gelöscht. Jetzt nur bei tatsächlich geänderter CurrentOutfitFolder UUID oder Version invalidieren. Separater Guard erkennt auch Änderungen durch F9 periodischen COF Readback unmittelbar vor Draw.
7. Neue Regression tests/firestorm_wearable_texture_plan_tests.cpp: originales 45 TE/Wearable-Owner Mapping, elf bake inputs versus baked outputs, ungültige TE/UUID und doppelte TextureIDs, 12er Limit, RegionEpoch/COF invalid, echte JPEG2000 J2C Fixture -> RGBA8 -> Mip; bestehende PNG/J2C TextureLoader CTest. F9-Layout testet zwei Buttons mit Abstand in sechs Viewports. Vollständiger CMake Build, CTest, Python-Unittests, echter Linux OpenGL3.3 GPU GUI-Smoketest.
8. VERSION, projectplan.md, info.md, CHATUEBERGABE.md, Firestorm/Server/UI-Parity Matrix und docs/ALPHA111_LOCAL_TEXTURE_PREVIEW.md zeitgleich im GitHub Branch develop-current commit/push, dreifache SHA-Prüfung, sauberes Worktree.

OFFEN Alpha.112: originale Firestorm avatar_lad.xml TexLayer Masken, Skin Layer Pixel-Compositing in richtiger Reihenfolge, Fallback/JPEG2000 per texture slot, GPU Bodymap, echtes Bake-Texture CacheID/Upload und Server-Nachricht, vollständiges Tragen/Ablegen samt sicherem Multi-COF Rollback und authentifizierte NexVerse E2E sichtbare Avatar-Kleidung. Nicht als umgesetzt deklarieren, solange weder Layerbild noch GPU/simulatorischer Outfitwechsel nachgewiesen ist.

## Alpha.112–115 – vier integrierte Firestorm-TexLayer-Entwicklungsblöcke (14.10.2026)

BASIS: OGL develop-current 8d8e79e7798b4db294ebb875029498a0237496df, OpenGenesisLINK main 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94. Firestorm_7.2.4 original lokal installierte character/avatar_lad.xml, originale ETextureIndex-Namen 0..44 und elf Bake-Inputgruppen. Firestorm-Assets wegen Lizenzprüfung nicht ungeprüft in Viewer-GitHub kopieren. Suchpfad NEXVERSE_AVATAR_ASSETS oder vorhandene Benutzerinstallation.

ALPHA.112 (CORE original XML): Neue firestorm_texlayer_manifest.hpp und src/assets/firestorm_texlayer_manifest.cpp lesen tatsächliche avatar_lad.xml mit tinyxml2. Sicherheitsgrenze 1 MiB, 11 eindeutige Körperregionen Head Upper Lower Eyes Skirt Hair LeftArm LeftLeg Aux1-3, bis 512 Layer, echte Reihenfolge, canonical Firestorm ETextureEntry Source-Namen, Texturabmessungen. Explizites Erfassen von Maske/Visibility/ParamAlpha, global/fixed color, TGA, alpha_only, Bump und Mehrfachtexturen. Ungültige oder unerwartete XML-TE-/Regionsdaten werden abgelehnt.

ALPHA.113 (CORE Teilkomposition): Neue firestorm_texlayer_composite.hpp mischt ausschließlich parameterfreie einfache lokale Farbtexturlagen in exakt aus XML ermittelter Reihenfolge mittels RGBA8 Porter-Duff source-over bis 128px. Andere Layer werden als unsupported oder fehlend gezählt; keine stillschweigende Fälschung von Firestorm Masken, Paramweights oder baked cache. is_verified_bake ist immer false.

ALPHA.114 (Daten und Scope): Authentifizierte, vom Original-Wearable verifizierte GetTexture JPEG2000/PNG Bilddaten werden pro originalem Firestorm TextureEntry lokal mitgeführt, auch wenn mehrere TEs dieselbe AssetUUID referenzieren. Erfordert passenden Owner/RegionEpoch/COF Version, verwirft CPU- und GPU-Vorschauen bei Regionwechsel, COF-Versionsänderung, Inventarrefresh oder Viewer-Shutdown. Kein AgentSetAppearance Versand, kein Server Asset Write.

ALPHA.115 (sichtbare F9-GUI + Renderer): Zwei weitere ECHTE klickbare Buttons TexLayer lesen und Teillagen mischen in Outfit F9. Lokal installiertes Original XML wird geladen und Anzahl/Komplexität angezeigt; nach validen User-RGBA-Texturen zeigt GPU 3 transparente Head/Upper/Lower-Teillagenbilder wo verfügbar und explizit Anzahl ausgelassener/fehlender Layer. Footer um TexLayer Statuszeile ergänzt. Input und Draw-Koordinaten gemeinsam, Tests auf sechs Fenstergrößen. Test liest echte lokale Firestorm XML: 89 Layer, davon nur 3 vollständig ohne Masken/Param/TGA/Bump direkt sicher für einfachen Teilpreview; das ist KEIN vollständiger Avatar Bake.

Release Gates: C++23 Debug CMake, sämtliche CTests einschließlich synthetischer TexLayer/Alpha-Blend/Masken-Gegenproben und echtem installiertem Originalfile, Python-Unittests, Linux NVIDIA RTX 4070 SUPER OpenGL3.3 GUI Smoke, aktueller OpenGenesisLINK SHA Strict-Check, Commit Push develop-current, local=tracking=GitHub SHA, sauberes Arbeitsverzeichnis. VERSION, projectplan.md, info.md, CHATUEBERGABE.md, Firestorm/UI/Server-Kompatibilität und docs/ALPHA112_115_TEXLAYER_MULTI_BLOCK.md aktualisieren.

NÄCHSTER SAMMELBLOCK Alpha.116–119: Original Firestorm param_alpha/Maske TGA Quelle lesen, Farb-/Layerparameter mit belegter TexLayer-Semantik anwenden, vollständige Layerkomposition mit korrekten Defaults und UV sowie GPU Avatar-Bake und serverkonformes Upload/Cache/Appearance-Readback. Volles COF Tragen/Ablegen einschließlich atomarer Bestätigung/Rollback und authentifizierte NexVerse-Inworld Abnahme bleibt gesperrt, bis End-to-End belegbar. KEINE vollständig angezogene 3D Avatar-Kleidung behaupten.

## Alpha.112–115 Live-Serverreview nach Implementierung (14.10.2026)

Der OpenGenesisLINK Server main ist vor Release auf Commit 20a7fd765527807fd9d7887965b5f5f28d42d68f (#209) vorgerückt, zuvor 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94. Alle neun geänderten Dateien und GitHub-Patches überprüft: Control Center v2, WorldAPI Metadaten für 0.9.3.12 als Release (früher 0.9.3.12 entwicklung), Version-aware ChatGPT Guidance, Operator Diagnose/CI, BaseHttpServer malformed XML-RPC => HTTP400 statt impliziter Erfolgsantwort, HG InstantMessage Log-Throttle nur für unauflösbare Empfänger OHNE Aussetzen der eigentlichen UAS/Zustellversuche.

Entscheidung SERVER_ONLY_WORLDAPI_OPERATOR_DIAGNOSTICS_AND_HG_XMLRPC: Keine User-Viewer LLUDP Transfer/GetTexture/COF Änderungen, kein neues API/Secret im OGL Viewer. Weiterhin OpenGenesisLINK=Server-Produkt, NexVerse=konkretes Grid. Die Rollenprüfung UserLevel >=200 ist nur für den Serveradministrator; darf NICHT in die Anmeldung des normalen Viewers einfließen. Release gilt als serverseitige Metadatenangabe, nicht als Live-NexVerse-Abnahme einer fertig gebackenen Avatar-Kleidung. Baseline aktualisiert auf 20a7fd... und strikten GitHub-Check anschließend wiederholen.

## Alpha.116–119 – Sammelblock Firestorm TGA-Masken, param_alpha und sichtbare F9-Teilkomposition (14.10.2026)

Baseline OGL develop-current Alpha.115 87181e7e54a7703710ddc38a4a84782cd082e2bc; geprüfter OpenGenesisLINK Server main 20a7fd765527807fd9d7887965b5f5f28d42d68f. Firestorm Referenz weiterhin lokal installierte Originale character/avatar_lad.xml und character/*.tga, niemals ungeprüft Asset-Inhalte im GitHub einchecken. OpenSim und Firestorm Standartfeatures sind CORE und nicht Plugin. Alle vier Teilblöcke zusammen mit GUI und Tests.

Block Alpha.116 – echte lokale TGA-Ressourcen. Neu include/nexverse/assets/firestorm_tga.hpp und src/assets/firestorm_tga.cpp: bounded TGA raw2/RLE10 RGB/BGRA sowie raw3/RLE11 8-Bit Graustufen-Alpha, Bildorigin Top/Bottom, Kanalreihenfolge BGR->RGBA. Reject Palette, Right-Origin, invalid header, malformed RLE count, abgeschnittene Pixel/ID, >2048 Dimension und >16 MiB. Nur sichere lokale Basenames unter gefundenem Firestorm character directory, nie beliebige absolute Pfade oder externe HTTP Quellen. Real installiertes glove_length_alpha.tga (512x512 RLE11) dekodiert, Test mit Raw/RLE/Invalid.

Block Alpha.117 – param_alpha Semantik aus Original XML. Erweiterter Firestorm TexLayer XML Parser speichert pro layer param id, wearable owner, value_min/max/default, param_alpha domain, multiply_blend, skip_if_zero, tga_file und Identifikation alpha-only vs gleichzeitig color param; Grenzen/Finiteprüfung, <=16 Params pro layer. Unbekannte, ungültige und gemischte Effekte weiter nicht als fertig unterstützt melden; bestehende TexLayer-Farbbearbeitung bleibt offen. 89 echte Firestorm original layers weiterhin nur 3 unkomplizierte Quellen; neue Metadaten unterstützen mehr maskenorientierte Diagnose, nicht Komplettabnahme.

Block Alpha.118 – erweiterte CPU Masken-Teilvorschau. Neu include/nexverse/assets/firestorm_texlayer_mask_preview.hpp. Actual LLWearable visual_params der aktuellen owner-/epoch-/COF-verified FetchAsset Daten, Konflikte gleicher ParamID mit verschiedenen Werten werden ignoriert. Geeignete einzelne param_alpha TGA Masken und relative Weight 0..1 werden im XML Layer-Order verwendet und mit alpha-gesteuertem RGBA Porter-Duff source-over kombiniert (max128x128 Preview). Nur Parameter mit wirklichem Wearable Asset Value; keine geratenen Defaults. Masken für unbekannte/komplexe/color/bump/visibility/TGA texture Effekte bleiben unsupported. Firestorm domain/multiply_blend/Masken-Kombinatorik ist noch nicht pixelgenau; Ergebnis absichtlich diagnostische unvollständige lokale Vorschau (real_bake false).

Block Alpha.119 – gekoppelte echte Viewer-Oberfläche und GL Renderer. F9 vorhandener echter Klick Masken/Teillagen statt der einfachen Komposition. Bis zu 12 vertrauenswürdig lokal installierte Masken werden nur für aktiven validierten COF/Region und verfügbare Original TextureEntry RGBA geladen. Bildmaterial wird mit dem vorhandenen OpenGL UI-Renderer in den drei Head/Upper/Lower Vorschauflächen aktualisiert. Status nennt applizierte RGB-Schichten, TGA Masken, unsupported, missing und Maskenfehler. Unveränderte Region/COF/Owner-Gates und Bild-Handle-Release auf Regions-/COF-Wechsel und Shutdown. Keinerlei AgentSetAppearance, kein serverseitiges Bake-Hochladen, kein 3D-Body-Skin-Verändern.

Tests: neuer tests/firestorm_tga_mask_tests.cpp mit Raw BGR, original RLE Graumasken, beschädigter Stream, oversized, Origin, Pfad-Traversal, Original Firestorm glove_length_alpha.tga, XML alpha_params, masked pixel-alpha Weight 0.5 und fehlende Param/Mask Rejection. Bestehender XML Test und F9 Interaktionshitbox, vollständige 103+ CTest Suite, Python unittests, Linux NVIDIA GPU OpenGL 3.3 GUI Smoke vor Release. VERSION 0.2.0-alpha.119, projectplan.md, info.md, CHATUEBERGABE.md, Paritymatrizen und docs/ALPHA116_119_FIRESTORM_MASKS.md gemeinsam commiten/pushen.

Folgeblock Alpha.120–123 (erneut vier): original Firestorm LLTexLayerCompositor komplexe Bump/Farbtint/Mehrfach-Alpha Regeln, Pixel-RGB/UV Vergleich mit Firestorm Referenzdateien; korrekte Bake-ID/Cache/Upload Nur nach Originalserver Vertrag, tatsächliche Avatar GPU Haut-/Bekleidungs-Anbindung, eine überprüfbare atomare Outfit Tragen/Ablegen Aktion mit Serverbestätigung und Rollback. Ohne auth Live NexVerse Weltbild bleiben Outfit-Wechsel, vollständiges TextureBake und Avatar-GPU-Darstellung offen; keine fiktiven Vollzugsmeldungen.

## Alpha.120–123 – vier verbundene Firestorm-Farb- und Bake-Diagnoseblöcke (14.10.2026)

BASELINE Viewer develop-current 97345466837df755317b9439c3beaa87050aa5cf, OpenGenesisLINK main 20a7fd765527807fd9d7887965b5f5f28d42d68f. Referenzen: Original FirestormViewer Firestorm_7.2.4 indra/llappearance/lltexlayer.cpp mit findNetColor/calculateTexLayerColor und indra/llappearance/lltexlayerparams.cpp getNetColor/parseXml sowie lokal installierte avatar_lad.xml. Firestorm/OpenSim Core nicht als Plugin, keine AdminSecrets im Endnutzer-Viewer.

Alpha.120 – Original TexLayerColor XML: firestorm_texlayer_manifest speichert fixed_color RGBA, global_color als nicht erratene symbolische Referenz sowie param_color id, min/max, Operation ADD/MULTIPLY/BLEND und bis 32 individuelle RGBA Keyframes. Mixed color+alpha Parameter klassifiziert, strict IDs/Finiteness/8-bit-RGBA. Die echte Firestorm XML besitzt **99 ColorParams in 71 Layern und 25-stufige EyeShadow-Paletten** (im Test).

Alpha.121 – echter Farbinterpolator: neues firestorm_texlayer_color.hpp berechnet palettenbasierte Farben aus REALEN zuvor owner/COF/Region verifizierten Wearable Visual Param Werten, festen Farben und Original ADD/MULTIPLY/BLEND in Reihenfolge. Unbekannte global skin/hair/eye color bleiben unaufgelöst und werden nicht durch Defaultwerte vorgetäuscht.

Alpha.122 – sichere CPU/GPU-F9 Farb-/Masken-Teilvorschau: firestorm_texlayer_mask_preview verbindet param_color mit lokalen source RGBA und nur unterstützter einzelner TGA-param_alpha Alpha-Maske, Farbtint inklusive Alpha im Porter-Duff source-over, in der original XML-Reihenfolge; drei Head/Upper/Lower OpenGL 128px Vorschauen. Unaufgelöste Farben, fehlende ParamIDs, nicht unterstützte Visibility/Bump/multiple Masken werden explizit übersprungen und gezählt. KEIN vollständiges, pixelgleiches Firestorm Bake.

Alpha.123 – echte F9-Simulator-Schnittstelle: TexLayer lesen zeigt tatsächliche Farb-/Layer-Zahlen, expliziter Button Farben/Masken erstellt lokale Vorschau und meldet blocked/colored/masked/unknown. Appearance pruefen zeigt den direkt vom authentifizierten Simulator empfangenen AvatarAppearance TextureEntry-BakeUUID-Zähler 0..11 ohne Firestorm Platzhalter plus echten COF-Version Vergleich. Nicht gleichsetzen mit einer erfolgreichen clientseitigen Bake, Outfit Tragen/Ablegen, Avatar-GPU-Skin oder Upload. Vorhandener Renderer verwendet nur original serverseitig empfangene Baked Texture IDs, keine ungesicherte lokale Vorschau an einer angeblichen BakeID.

Release Gates: Vollständiger C++23 Debug Build, alle CTest und Python Tests inklusive realer Firestorm 25-Farbpalette, Linux NVIDIA RTX4070SUPER OpenGL3.3 GPU GUI Smoke, server strict GitHub SHA Check vor Commit/Push, lokale SHA=Tracking=GitHub SHA und sauberes Worktree. VERSION, projectplan.md, info.md, CHATUEBERGABE.md, sechs Parity-Docs und docs/ALPHA120_123_COLOR_AND_BAKE_AUDIT.md aktualisieren.

Nächster Viererblock Alpha.124–127: tatsächliche Firestorm global skin/hair/eye Farbwert-Daten aus Avatar/COF und multiple param alpha, Domain/Mask Multiply/Morph mit Pixelvergleich; dann 11 korrekt compositierte Bake-Regionen, Server-getreues Baked Asset Upload/Cache und kontrolliertes echtes Outfit Wear/Unwear mit Rollback nur nach nachweislich erfolgreichem NexVerse Inworld Test. Diese Teile sind bis dahin nicht erledigt.


## 14.10.2026 – Viererblock Alpha.124–127 – Firestorm global color + multiple masks + 11 bake regions

- Starting HEAD 1020c5b (develop-current), Firestorm_7.2.4 original avatar_lad.xml/LLTexLayer referenced. Actual code version 0.2.0-alpha.127.
- 124: Parse actual skin_color, hair_color, eye_color global palettes from bounded original XML with strict names, owner, IDs and RGBA checks.
- 125: Resolve true global shades from previously authenticated region-/COF-/owner-verified LLWearable visual parameters partitioned by original type (Skin1, Hair2, Eyes3); never invent missing defaults. Linked into F9 source preview.
- 126: ADD/MULTIPLY order for multiple local param_alpha TGA masks in bounded diagnostic preview. Incomplete nonzero domain/morph/bump/visibility, first multiply and missing weights stay blocked; no Firestorm GPU pixel-parity claim.
- 127: Read-only input coverage for all eleven bake source regions. F9 reports global tint and multiple mask counts plus original-region image availability. Immutable NO-BAKE/NO-UPLOAD guard and no AgentSetAppearance mutation.
- New CTests for original palette three colors, owner type, two masks and 11-region coverage. Existing TGA test updated to fail closed for unsupported domain. 106/106 local CTests passed before final release gate.
- OpenGenesisLINK main 4e99976630f1008db9014ea2b86c3723b09e9fe6 (#210), six-file patch manually reviewed. All local Hypergrid friends now receive recovered online/offline status after relog; SERVER_ONLY_HYPERGRID_FRIENDS_DELIVERY_NO_NEW_VIEWER_WIRE, no LLUDP message changes. No operator API or keys inside Viewer.
- Still open: true complete 11-region Firestorm baked pixels/CacheID/asset upload; actual 3D avatar skin, Wear/Unwear and reversible server confirmation, live NexVerse in-world visual test.
- Full description: docs/ALPHA124_127_GLOBAL_MASK_COVERAGE.md.

## 15.10.2026 – Alpha.128–131 – Pi5 Avatar-Bake-Provenance, F9, native CI, Server-Gate

**Umgebung:** Raspberry Pi 5 s359142.srv.nexvortex.de, User nexdev, Repo marlin198323-glitch/nexverse-viewer, Branch develop-current, Starting SHA e349222f09632791ac825299c0725866b3d3015e. Keine Prod-Änderungen, keine Mint-Operationen.

- **128 (Core):** Neues avatar_bake_evidence.hpp: originale elf Firestorm-Bake-TE-Slots werden als fehlend, serverseitig referenziert oder tatsächlich im aktuellen GPU-Resident-Set vorliegend gezählt; Body Head/Upper/Lower separat. upload_authorized bleibt false. Weder serverseitige UUID noch GPU-Textur allein beweisen eine vollwertige Firestorm-Pixel-Bake.
- **129 (F9 UI):** Echter Klick auf Appearance pruefen gibt COF Simulator/Inventar, elf Server-Referenzen, elf residente GPU-Quellen sowie Head/Upper/Lower (G/S/-) in einer kurzen lesbaren Statuszeile aus. Beginnt mit KEIN BAKE. Der bisherige authentifizierte SelfAvatar-/Epoch-Guard bleibt.
- **130 (Pi5 CI):** Neues scripts/pi5-headless-tests.sh kompiliert zwei echte C++23-Tests mit ARM64 g++14, führt sie aus und startet Python-Unittests. Der etablierte Self-hosted pi5-ogl Workflow verwendet das Script; Shell-/Quellcodechecks erhalten. Neuer CMake-Testeintrag für Host-Toolchains. Keine Installation von Paketen und kein CMake-/GPU-Build auf dem Pi.
- **131 (Upstream fail-closed):** scripts/check_opengenesislink_server.py ordnet Flug-Regionswechsel, Upload-Berechtigung, Hypergrid-Inventar und Asset-Löschung präziser zu. GitHub Compare mit 300 oder mehr Dateien wird als möglicherweise abgeschnitten erkannt und im strict-Modus blockiert. Python-Regression bestätigt Exit 2.
- **Server-Review:** upstream main d31c03168f0f04f4ce02e82ee3246e16de744748; 22 neue Commits gegenüber freigegebener Baseline 4e99976630f1008db9014ea2b86c3723b09e9fe6. Betrachtete Einzelpatches: #217 einheitliche Permissions-LevelUpload für UDP/CAPS (Server-Gate); #221 echtes Flug-Velocity bei CrossRegion (bestehender Viewer-Protokollvertrag); #223 Server-Prefetch von fliegenden Avataren (kein zusätzliches Viewer-LLUDP); #212 öffentliche Hypergrid-Inventar-Autorisierung (Server). WorldAPI/Operator-Token bleiben server-only. Der kummulative Vergleich endet bei 300 Dateien wegen vieler entfernten bin-Asset-/Cachedateien in #215/#216. Baseline deshalb NICHT künstlich auf neues SHA gesetzt. Vor Pin alle Einzelcommits inklusive vollständig entfernter Dateien klassifizieren.
- **Tatsächliche Tests (Pi5):** Zwei native ARM64 C++23-Tests PASS, 18 Python-Unittests PASS, git diff --check PASS, bash Syntax PASS. Upstream strict Exit 2 (neue nicht abschließend geprüfte Server-Commits, explizit blockiert). CMake und vcpkg fehlen auf Pi: vollständige CTest-/Viewer-/OpenGL-/GPU-E2E noch NICHT ausgeführt.
- **Offene echte Features:** vollständiger elf-Regionen-Bake mit originalem Firestorm-Pixelvergleich, überprüfter Simulator-CacheID-/Upload mit Berechtigungen, GPU-Textur-Skin im laufenden Grid, bestätigtes Wear/Unwear mit Rollback, authentifizierte NexVerse-Inworld-Abnahme. Erst nach Nachweisen freigeben.
- **Nächste Blöcke:** (a) autorisierte Body-Bake-GetTexture-Streaming/Priorisierung, (b) lokale UV/Pixel-Testfixtures mit legal vorhandenen Firestorm-Assets, (c) Pi CMake/vcpkg in isolierter Build-Umgebung mit <=2 Jobs, (d) Upstream-Einzelcommits bis vollständiger Review abgeschlossen.

## 15.10.2026 – Alpha.132–135 – Avatar Streaming / F9-Live / Pi5
SelfAvatar Head/Upper/Lower Server-Bake-IDs erhalten Vorrang im gemeinsamen
GPU-Residency-Budget, nahe Avatare folgen bis 96m. Opt-in F9 Appearance pruefen
zeigt lokalen Live-GPU-/COF-/Server-TE-Status alle 750ms, ohne Extra-CAPS-Requests.
4 native Pi-C++23-Tests und 18 Python-Tests PASS; CMake/GPU-GUI NICHT getestet.
Upstream main d31c0316 hat 22 noch nicht vollständig geprüfte Commits
(300-Dateien-Vergleich abgeschnitten); striktes Gate bleibt BLOCKIERT.
Kein echtes Bake-Upload, kein abgeschlossener Outfit-Wear/Unwear.
Siehe docs/ALPHA132_135_AVATAR_BAKE_STREAMING.md.

## 15.10.2026 – Alpha.136–139 – UV-Gate, echter GL-Mesh-Beleg, F9-Probe

- Alpha.136: Original Firestorm UV/xyz/Normal-/Indexwerte vor
  Avatar-GL-Upload validieren, gültige wiederholende UV nicht verändern.
- Alpha.137: Read-only per-frame selbst Avatar H/U/L Counter nur nach
  echten OpenGL-Bake-Draw-Aufrufen des anatomischen Original-LLM-Meshes;
  provisorische Proxy-Fallbacks sind separat und ausgeschlossen.
- Alpha.138: F9-Galerie zeigt per H/U/L den tatsächlichen 3D Body
  GL-Draw-Status (G/-), klickgesteuert und ohne Pixel-Paritätsbehauptung;
  Layout bei schmalem Fenster ohne zweite kollidierende Statuszeile.
- Alpha.139: Native Pi5 5 C++23 PASS, Python 22 Tests PASS, CMake-CTest
  und Self-hosted Pi5-Script ergänzt. Volle GL-/GUI-/CMake-Prüfung
  wegen fehlender Build-Toolchain nicht gelaufen.
- Geprüfter Server-Main-HEAD d31c03168f0f04f4ce02e82ee3246e16de744748;
  Upstream-Vergleich mit 22 neuen Commits weiterhin strict BLOCKIERT
  (300-Dateien-GitHub-Compare-Trunkierung), Baseline nicht erhöht.
- Offene Freigaben: Original-UV-/Pixelgleichheit auf GPU, echte Avatar-
  Skin/Outfit/Wear/Unwear, Simulator CacheID/Upload und GUI-E2E.
- Detailreport: docs/ALPHA136_139_GPU_BODY_UV_PROOF.md.

## 15.10.2026 – Alpha.140–144 – Fünf größere Entwicklungsblöcke

140: Echte Server-GetTexture GL-Handle-Vorschau für originalen Head/Upper/Lower
in der bestehenden Rendering-API, read-only, Session-/Region-gebunden;
Platzhalter und fehlende GPU Assets bleiben ungerendert.

141: F9 Outfit-Galerie zeigt drei echte, vorhandene Server-Bake-Bilder
anstelle erfundener Skin-Platzhalter, samt "nicht geladen"-Status.
Keine neuen Web/CAPS-Abfragen durch diese GUI.

142: Shader setzt Avatar-uObjectTexGenPlanar auf false und uObjectScale
auf 1 zurück, damit Prim-Welttexturprojektion die Original-LLM-UV
nicht verfälscht.

143: Vorhandene SelfAvatar-Bake-IDs werden auf Quell-MIP0 bevorzugt;
shared VRAM 256MiB, GPU Slots 512, Retry/Region/Admission bleiben gültig.

144: GitHub Server-Inventarisierung commitweise/paginiert vollständig
durchgeführt: OpenGenesisLINK main d31c03168f0f04f4ce02e82ee3246e16de744748,
22 Commits, 1276 Dateieinträge, davon 1208 MANUAL_REVIEW.
Report: docs/OPENGENESISLINK_UPSTREAM_INVENTORY_20261015.json.
Pinned Server-SHA bleibt 4e99976630f1008db9014ea2b86c3723b09e9fe6;
strict Vergleich/Freigabe weiterhin BLOCKIERT.

Pi5 native 7 C++23-Tests PASS und 28 Python PASS (finaler GitHub CI
separat prüfen); CMake, GPU, Firestorm Inworld und echte Wear/Unwear
NICHT getestet. Vollständige technische Beschreibung:
docs/ALPHA140_144_GPU_BAKE_PREVIEW_AND_UPSTREAM_AUDIT.md.

## 15.10.2026 – Alpha.145–149 (5 Entwicklungsblöcke)

145: SelfAvatar-Bake nur mit gleicher AvatarAppearance/Login/WorldModel
AgentUUID und aktiver Region; F9 und MIP0 ebenfalls geschützt.
146: Klickbarer F9 Tab Server-Bakes / Outfit-Inhalt mit 3 echten
219x75 GPU-Bildvorschauen Head/Upper/Lower aus Server-GetTexture.
147: Kleine Fenster und F9 Lifecycle abgesichert; keine überdeckten
Statuszeilen, automatische Tab-Rücksetzung bei Schließen/Regionwechsel.
148: OpenGenesisLINK Server main d31c0316, 22 Commits, 1276
Dateieinträge. 1087 Cache-/Terrain-Quellen in einem einzigen
Cleanup-Commit sind durch neue Regeln klassifizierbar.
Neuer Live-Abruf scheiterte GitHub API 403 Rate Limit, deshalb
altes vollständig inventarisiertes Protokoll beibehalten;
Server-Reviewed-SHA NICHT erhöht, strict Gate bleibt BLOCKIERT.
149: Defekte C++ Präprozessor-Include-Zeile und falsches CMake-Ziel
NexVerse::Ui repariert, Regressionen ergänzt. Pi5 8 native C++23,
31 Python PASS; voller CMake-/GUI-/GPU-/Inworld-Test NICHT erfolgt.
Belege: docs/ALPHA145_149_AUTH_F9_SERVER_REVIEW.md.

## 16.10.2026: Alpha.150-154, Desktop Commander Mint x64 native build

- OGL returned to primary Linux-PC jam-MS-7D28 via Desktop Commander,
  checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer. GitHub
  develop-current fast-forwarded 12 commits without discarding changes.
  Pi5 is only the automatic GitHub ARM64 CI runner.
- Complete x64 CMake/vcpkg/Ninja viewer build OK. After new native
  locator regression: 113/113 CTests and 33/33 Python pass, including
  hardware NVIDIA RTX 4070 SUPER OpenGL renderer GPU tests.
- Real GL body bake draws H/U/L and live preview handles verified,
  GPU cache eviction invalidation verified, OpenJPEG J2C/JP2 RGBA
  parity fixture verified.
- Firestorm source warning fixes and lossless 64-bit COF evidence;
  VERSION changes trigger CMake configure, --version alpha.154.
- Viewer now auto-discovers locally extracted Firestorm Release on
  Desktop/Schreibtisch, favoring Nightly if extracted. 133 joints,
  55 attachments, 4,320 vertices/7,186 original mesh triangles loaded
  onto GPU. GUI 1600x900 isolated login screen + Welcome and GridInfo
  loads, but user account/world/inworld-avatar E2E remains untested.
- Server main ec760cac added a server-only J2K OpenJPEG layer-boundary
  retry. Earlier server compatibility review remains blocked;
  baseline SHA unchanged. Report:
  docs/ALPHA150_154_LINUX_MINT_FIRESTORM_GPU_BUILD.md.

## 16.10.2026 – Alpha.155-159 – Fünf größere Linux-Mint-Blöcke

155: Nightly 7.2.5 original Firestorm character-Assets lokal im
User-Cache extrahiert. Alle 96 Dateien SHA256-gleich mit Release
7.2.4. Cache-Nightly jetzt priorisierte Autoerkennung, nichts im GitHub.
156: Read-only F9 Server-Bakes unmittelbar per Tab anzeigbar,
ohne separate Appearance-Live-Diagnose; GPU-Status 0/3 bis 3/3,
SelfAvatar/Region/Epoch-Schutz bleibt bestehen.
157: Original OpenGL3.3-RGBA8 GPU-Bake-Readback gegen
hochgeladene Testpixel sowie Handle-/Texture-Lifetime geprüft.
158: OpenGenesisLINK main ec760cac J2KDecoderModule.cs
serverseitiger CSJ2K zu OpenJPEG Retry eigens klassifiziert.
Gesamt-Upstream-Review weiter offen; baseline SHA unverändert.
159: Start-Login zeigt echten Firestorm-Originalbody-GPU-Status
oder Warnung bei Ersatzgeometrie. Alpha.159, Ninja x64 FULL BUILD,
113/113 Native CTests, 34/34 Python PASS. Isolierter 1600x900 UI-
Start lädt 133 Rig-Joints, 55 Attachment-Slots und 7186
Original-Körperdreiecke, NexVerse-Welcome vorhanden.
Echter Login/Avatar/COF-Inworld-Nachweis steht weiter aus.
Bericht: docs/ALPHA155_159_NIGHTLY_F9_GPU_UI.md.

## 16.10.2026 – Alpha.160–164 – Fünf Entwicklungsblöcke auf Mint

160: Sichtbare Viewer-Teststarts als Problem erfasst. Native GLFW
Renderer-Tests sind unsichtbar; vollständige Viewer-Smokes öffnen
künftig keinesfalls automatisch das echte Desktop-Display.
161: scripts/mint-isolated-gui-smoke.sh: nur virtueller Xvfb,
max. 8 Sekunden mit eindeutigem Startup-Beleg, kein Display-Fallback,
isolierte XDG-Daten, automatisches Aufräumen. Python-Fakeviewer-
Test beweist virtuelle DISPLAY-Übertragung selbst bei DISPLAY=:776.
Ohne Xvfb auf Pi überspringt nur die grafische Fixture sicher.
162: Inworld-Systemstatus unterscheidet Original-LLM GL 0/3..3/3,
Fallback-Proxy, fehlenden Draw und tatsächliche H/U/L-Bake
GL-Draws. Source-Nachweis keine Pixel- oder Inworld-Avatar-Parität.
163: MIP0/F9 SelfAvatar wird über find_if(is_self) gewählt, nie
blind als erstes Element; Login-AgentUUID und Region/Epoch bleiben.
164: Vollständiger nativer Linux-x64 Viewer-Build, 114/114 CTests
inklusive versteckter NVIDIA GPU-Kontexte, 38/38 Python PASS.
Echter Viewer Xvfb-Smoke inklusive Original Firestorm 133 Joints,
7.186 Body-Dreiecke und UI init PASS, ohne Desktop-Fenster.
Server main ec760cac unverändert, reviewed_sha nicht angehoben.
Projektbericht: docs/ALPHA160_164_ISOLATED_GUI_AND_AVATAR_HUD.md.
Echter Login/Welt-/Avatar-/Outfit-/COF-Inworld-Test weiterhin offen.

## 16.10.2026 – Alpha.165–169 – Frühe Weltanzeige ohne Terrain-Gate

165: WorldModel::scene_ready() trennt bereits autorisierte
Simulator-/Agent-Sitzung von spät eingehenden TerrainLayerData;
strenges WorldModel::ready() bleibt unverändert abhängig
von gültigem Terrain. Zwei App-Gates nutzen scene_ready().
166: Inworld HUD zeigt Terrain 0..256 Patches, Objektanzahl aus
WorldModel, tatsächliche GL Draws und Mesh-Fehlfallbacks separat.
167: CAPS EventQueue-Lifecycle sichtbar; angekündigte CAPS URL
gilt nicht als CAPS LIVE, ohne erfolgreichen Eventempfang.
168: Neue native C++ Tests plus versteckter NVIDIA-GL3.3-Test
beweisen tatsächlich gezeichnete Weltmeshes ohne Terrain.
169: Voller Mint-CMake/GPU Build und Xvfb-GUI-Smoke ohne
Desktopfenster, Commit nach GitHub develop-current und ARM64
Pi5 CI. OpenGenesisLINK main ec760cac unverändert.
Eine authentifizierte Welt/Avatar-E2E-Sichtprüfung bleibt offen.
Bericht: docs/ALPHA165_169_EARLY_SCENE_AND_WORLD_DIAG.md.

## 16.10.2026 – Alpha.170–174: Avatar-Pipeline / Inworld UI (5 Blöcke)

170: Netzwerk-/CAPS-Statuskarte und Systemstatus-Überschrift
überdeckten sich nach Alpha.169; Koordinaten von Überschrift,
GL-/RAM-/VRAM-Zeilen untereinander korrigiert.
171: Neues AvatarPipelineProof: valide eigene AvatarAppearance
mit Agent-ID/Region/Epoch, echte 11 TE-Bake UUIDs, aktuelle
GPU-residente IDs und wirklich ausgeführte LLM-/BakeGL-Draws;
kein Fake Bake/Wear/Upload.
172: Zusätzliche sichtbare Inworld-Avatar-Karte zeigt fehlende
Pakete, Server TE N/11 GPU M/11, echte H/U/L GPU-/GL-Stufen.
Warnfarbe bis alle drei Bodies tatsächlich mit 3 Server-Bakes
gezeichnet sind; bei engen Fensterhöhen keine Überdeckung.
173: Native C++ CTests und NVIDIA-GL3.3-Regression bestätigen
den aktuellen Serverref/GPU/Draw-Zustand sowie GPU-Eviction.
174: Voller Linux-x64 CMake/Ninja, Native GL/Python und
Pi5 ARM64 CI; Voll-Viewer-Test nur virtuell unter Xvfb.
Echter autorisierter Grid-Login und tatsächliche Welt-/
Avatar-Framebuffer-Sichtbarkeit weiterhin offen.
Referenz: docs/ALPHA170_174_AVATAR_PIPELINE_GUI.md.

## 16.10.2026 – Alpha.175–179 – Fünf Linux-Mint-Entwicklungsblöcke

175: Echte einzigartige Welt-Texture-/Mesh-Asset-IDs
und gegen aktuelle Region GPU-residente Asset-UUIDs
abgleichen, leere und mehrfach benutzte UUIDs korrekt
berücksichtigen; native C++23/Pi5-Regression hinzugefügt.
176: Rechts im Inworld-HUD erscheint Tex N/M GPU |
Mesh P/Q GPU aus wirklich gesampelten Weltobjekten,
Texture-/Mesh-GL-Cache; keine erfundenen Assets/Anfragen.
177: Verbindungs-/System-/Avatar-Statuskarten jetzt mit
rail_height/scale-Layoutgrenzen und 12pt Abstand zur
unten verankerten FPS-Leiste. Native Breakpoint-Tests.
178: NVIDIA GL3.3 RegionTransition Test verifiziert
GPU Texture vorhanden und nach Eviction nicht mehr,
duplizierte UUID nur einmal; mehr UI/GPU-Integration.
179: Voller Linux-x64-Debug CMake/GPU/Python/Headless
und Xvfb-isolierter Voll-Viewer-Smoke, GitHub push
von Mint und Self-hosted Pi5-CI. Kein sichtbar gestarteter
Viewer auf dem Nutzerdesktop. Echter NexVerse
Welt-/Avatar-/Outfit-Inworld-E2E bleibt offen.
Bericht: docs/ALPHA175_179_WORLD_ASSET_STREAM_AND_RESPONSIVE_HUD.md.

## 16.10.2026 – Alpha.180–184 | Third-Person-Kamera/GPU-Bildnachweis

180: Serverseitige Agent-Quaternion vor Kameraberechnung als double
normieren und nur validen horizontalen Heading verwenden. Nonunit,
Null, NaN/Inf und absurde Positionen mit sicherem Kamera-Fallback;
Drittperson 8m hinter, 4,2m über Avatarblickpunkt.
181: Tatsächliche Viewer-Third-Person-Projektion im App-Renderpfad
nutzt abgesicherte Pose statt unnormierte Server-Quaternion.
182: Im bestehenden Inworld-Status wird bei ungültiger Serverpose
"Kamera: ungültige Serverpose (Fallback)" angezeigt.
183: Hidden NVIDIA-OpenGL3.3-Test belegt, dass echtes Weltmesh
bei dieser Kameraperspektive im GL-Framebuffer sichtbare
RGBA-Pixel erzeugt, auch ohne TerrainLayerData.
184: Echte GLSL-Fallback-Sicht bei Nullquaternion erzeugt
identische Framebufferpixel ohne GL-Fehler; Header-C++,
CMake-x64-, Pi5- und Python-Regressionen ergänzt.
Kein tatsächlicher authentifizierter Grid-/Avatar-Framebuffer-
E2E-Test durchgeführt. Kompletter Server-Upstream-Review
bleibt blockiert, reviewed SHA unverändert.
Bericht: docs/ALPHA180_184_THIRD_PERSON_CAMERA_AND_GL_FRAMEBUFFER.md.

## 16.10.2026 – Alpha.185–189 | 3D-Weltfokus und echte Renderer-Cull-Verbesserung

185: GL-Renderer zählt nicht mehr vollständig hinter der
Kamera liegende Objekte als sichtbare erfolgreiche Draws;
konservative Kamera-Forward/Radius/FarClip-Prüfung,
nicht-endliche Objektdaten abgewiesen.
186: Neue sichtbare HUD-AUS/AN-[F5]-Schaltfläche
ermöglicht freie Weltansicht ohne große Seitenleisten.
GL-Szene, Menüs, F9 und Simulator laufen weiter.
187: Maus trifft exakt denselben Widescreen HUD-Button,
unsichtbare Dashboard-Hit-Zonen werden nicht länger
getroffen; Klick in freie Weltansicht geht zur Objektauswahl.
188: Neue native C++23/Pi5-Tests für F5/Widescreen/
Click-Routing und Objekt-Forward-Culling; versteckter
NVIDIA-GL-Framebuffer-Test bestätigt hinter der Kamera
0 GL WorldDraws und unveränderte Hintergrundpixel.
189: Voller nativer Linux-x64 Build, CTests/Python und
Xvfb-GUI-Smoke nur im isolierten Display. Nach grünen
Gates Push auf develop-current und Pi5 ARM64 CI.
Tatsächliche NexVerse Grid-Avatar-/Welt-E2E-Sichtprüfung
bleibt offen, Firestorm11 Bake/Wear/COF idem,
Server reviewed_sha bleibt ungeändert.
Report: docs/ALPHA185_189_WORLD_FOCUS_AND_VISIBILITY.md.

## 16.10.2026 – Alpha.190–194: Inworld-Zoom und F9 UI (fünf Blöcke)

190: Third-Person-Kamera unterstützt verstellbare 3..24m statt fester
8m, behält Normierung/Avatarziel/ungültige Quat-Fallbacks.
191: Echte GLFW Wheel +/- und Keyboard/Keypad +/- steuern lokal dieselbe
Kameradistanz; modal Inputs und nicht authentifizierte Simulation gesperrt.
Renderer, World-Ray-Objektpick und Textur-/Mesh-Streaming verwenden
denselben Kamera-Zoomzustand, keine LLUDP Bewegung.
192: Echte sichtbare +/- Kamerabuttons und Live-Meterlabel
im HUD und F5-freien Weltfokus; Widescreen Hit-Rects.
193: F9 Outfit Server-Bake-Read-only-Vorschau per B und sichtbarem
Server-Bakes [B] Button; NVIDIA OpenGL-GPU Framebuffer-Test mit
nachweisbar verschiedenen Pixeln bei 3m und 24m Zoom.
194: Vollständige native Mint-x64/NVIDIA/Python/Pi5-Headless-
Regression plus isolierter Xvfb Vollviewer-Test vor
GitHub-Commit, Pi5 Selfhosted ARM64 CI danach.
Echter Login/Welt-/Avatar-End-to-End-Frame weiter offen,
Firestorm TexLayer-/COF Wear-Parität nicht bewiesen.
Report: docs/ALPHA190_194_CAMERA_ZOOM_AND_F9_BAKES.md.

## 16.10.2026 – Alpha.195–199 – Avatar ansehen: Front, Seite, Rueckseite

195: Lokaler Third-Person-Kameraorbit relativ zur Avatar-Quaternion
0, +/-90 und 180 Grad für echte Front-/Seiten-/Rückansicht.
Behebung: Invalid/NaN-Zoom jetzt auch im Normierungsnenner bereinigt.
196: V wechselt Front/Hinten, [ und ] je +/-45 Grad mit
Session-/Modalinput-Guard, kein Avatar-/LLUDP-Update.
197: Drei sichtbare anklickbare Orbit-Schalter neben Zoom
und reale Orientierung Hinten/Vorne/Rechts/Links/Seite
in F5-Navigation sowie F5-freier Weltansicht. Gleiche
Kamera für GL-Renderer, Raycast und Asset-Priorität.
198: Native C++23-Mint/Pi5-Regression plus echter versteckter
NVIDIA-Framebuffer-Meshpixel-Test für 0 und 180 Grad
ohne TerrainLayerData; kein Live-Grid-Avatarbeweis.
199: Voller Mint-CMake-/NVIDIA-CI-/Python-/Headless-Test,
Xvfb-GUI-Smoke; GitHub develop-current + Pi5 ARM64 CI.
OpenGenesisLINK main ec760cac, reviewed_sha unverändert,
echter Avatar-/Welt-Login-E2E weiterhin offen.
Details: docs/ALPHA195_199_AVATAR_CAMERA_ORBIT.md.

## 16.10.2026 – Alpha.200–204 – Originalsystemicon und Login-Refresh

200: Nutzer-Originalicon 1254px JPEG identisch per SHA-256 mit
/home/jam/Downloads/icon.jpeg, daraus 32/48/64/128/256/512
RGBA-PNG mit transparenten Ecken, Windows ICO und macOS ICNS.
201: GLFW X11-Fensterklasse opengenesislink-viewer, echtes 128px
Cinnamon-Taskleistenicon mit _NET_WM_ICON ARGB32 Pixelproperty,
XGetWindowProperty per Xvfb+Metacity readback verifiziert.
202: Der alte Login-Header-Platzhalter links oben wird durch
echte Original-OG-Icon-OpenGL-Textur ersetzt; WelcomePage/
WebKit/https://stadt-nexverse.de/service/welcome ausdrücklich
UNVERÄNDERT, kein erzeugtes Mockup.
203: Dezent moderner Navy/Cyan-Header und Login-Karte;
Linux .desktop und hicolor Icons per CMake, Mint User-Starter
unter ~/.local/share/applications installiert. Windows .ico/.rc
und macOS .icns im Build definiert (nicht nativ gebaut).
204: Native PNG-/Logo-Test, Python-Branding-Contract, versteckte
X11-Icon-/WM_CLASS Prüfung und volle Linux CI sowie Pi5
Headless Workflow; echte Grid-Avatar-/Welt-E2E weiterhin offen.
Details: docs/ALPHA200_204_BRANDING_SYSTEM_ICON_AND_LOGIN.md.

## 16.10.2026 – Alpha.205–209: Avatar-Sichtbarkeitskette und Welt-Eintritt

205: Neue AvatarFrame-NDC-/FoV-/Viewport-Zentrumsprojektion für
echte AgentPosition und ThirdPersonCamera: Front/Hinten/Seite,
Zoom und ultrabreite Fenster, finite Values, backward/near guards.
206: Live AvatarPipelineProof verknüpft aktuelle Agent-Position,
Kamera, In-Viewport-Status und wirklich erfolgte GPU-
Originalmesh-/Fallback-Draws getrennt von AvatarAppearance-/
Server-Bake-Erfolg; keine Behauptung von Bildpixel-Sichtbarkeit.
207: Nutzer-Originalicon aus Alpha.204 auch als echte GL-Textur
oben links in der 3D-Welt-HUD-Leiste; neue Avatarstatus-Zeile.
Login und WebKit Welcome-Seite bleiben bestehen.
208: Authentifizierter Weltstart: konkrete Blocker LLUDP,
Region-Handle/Connected, Agent-/Session-ID und AgentPosition
im bisherigen Lade-Overlay, Fortschritt vor WorldReady nie 100%.
Native Tests für jede fehlende Stufe; WorldModel-Lesezugriff locked.
209: F5-Modus meldet nur Avatar-/Kamerafehler als kleine Zeile,
ansonsten volle Weltansicht. Native Mint/GL/Python, Pi5 Headless
und Xvfb-Vollviewer-Gates. Server-main ec760cac unverändert.
Echter Login-/Avatar-Pixelnachweis weiterhin NICHT getestet.
Detail: docs/ALPHA205_209_WORLD_ENTRY_AND_AVATAR_VIEW_EVIDENCE.md.

## VERBINDLICHE FÜNF-BLÖCKE-REGEL (16.10.2026)

Auf jedes WEITER fünf implementierte und getestete Blöcke; OGL Mint via Desktop Commander, Pi nur ARM64-CI. Vorab OpenGenesisLINK Server checken. Code, sichtbare Viewer-UI, Tests, Plan, Handoff, Commit/Push und Pi-CI zusammen. Blockaden explizit, keine Fake-Fertig-Behauptung. Siehe projectplan.md Alpha.210–214.

## 16.10.2026 – Alpha.210–214: fünf verbindliche Blöcke und echte GL-Avatar-Fragmente

210: Ein verbindlicher Fünferdurchlauf pro WEITER mit fünf
implementierten Arbeiten + sichtbarer UI, Tests, kompletter
Mint-Build, GitHub und Pi5-CI ist im Projektplan,
AGENTS.md und Chatübergabe fest verankert.
211: Original-LLM-Self-Avatar-Body bekommt asynchron
GL_ANY_SAMPLES_PASSED für tatsächliche Fragmente nach
Tiefentest; nonblocking Doppelquery, Renderer-/GPU-
Lebenszyklus und bisherige Draws weiter korrekt.
212: Epoch-sichere Queryinvalidate nach Regionwechsel,
UI trennt UNKNOWN/Depth-Samples 0/Depth-Samples sichtbar
von bloßer GPU-Submission und fehlenden Avatar-Bakes.
213: Sichtbarer Avatar-Fokusbutton [F4] + Tastatur-Kürzel,
echte Kamera 8m/frontal, funktioniert mit/ohne F5 HUD,
Widescreen, ohne LLUDP-Agent-Rotation oder COF-Schreibzugriff.
214: Echte NVIDIA-OpenGL-Regression für beantwortete,
verdeckte und epoch-invalide Avatar-Queries, native
C++23 Kamera-Button-/Python-Contract, vollständige
Mint/GPU/Headless/Xvfb und Github/Pi5 CI-Gates.
Noch KEIN realer authentifizierter NexVerse Avatar-
oder Weltbildbeweis, Firestorm Bake/Wear weiter offen.
Bericht docs/ALPHA210_214_FIVE_BLOCK_RULE_AVATAR_SAMPLES.md.

Alpha.214 CI-Freigabe: develop-current f83d0a25424c4eb35e3f0bb663cd525dfdadb080; Raspberry Pi5 GitHub Actions run 38958578939 SUCCESS. Alle fünf Arbeitsblöcke 210–214 in projectplan.md auf FERTIG. Künftiges WEITER umfasst verbindlich wieder zehn echte Blöcke. Echtes Grid-/Avatar-E2E bleibt offen.

## 16.10.2026 – Alpha.215–219: Frische GPU-Meshsichtbarkeit und lokaler F3-Diagnosebericht

215: Async original Self-LLM GL Depth-Sample-Messung bei Kamera/
FoV-Wechsel oder fehlenden Originalmeshdaten sofort entwerten.
NVIDIA-OpenGL-Test belegt query-invalidations nach Kameraänderung.
216: Private, whitelisted Avatar-Diagnose-Datei mode 0600 auf
Nutzerwunsch, ohne Agent/Session/Grid-UUIDs, Passwörter, URLs, Chat.
Nur Renderer-, Avatar-, Bake-, Scene- und Framebufferzahlen.
217: Echter vorhandener Avatarstatus [F3] Floater mit
Kamera-/Depthfragment-/ServerBake-Live-Daten, klickbarem
"Diagnose speichern [S]" und Key S; World-Focus bleibt beim
Dialog gesperrt, Welcome und F4–F11 weiter unverändert.
218: C++23 File/Mode/Whitelist-/Widescreen-HitTests,
GPU-Camera-/Avatar-Clear-Tests und Python Routing/Secret Assertions.
219: Mint x64 Full Build, alle GL/Native/Python/Headless und
isolierter Xvfb-Vollviewer, Commit develop-current, Pi5-CI.
Bericht docs/ALPHA215_219_AVATAR_DIAGNOSIS_AND_QUERY_FRESHNESS.md.
Echter Grid-Avatar-/Welt-Pixelbeweis weiterhin offen.

Alpha.215–219 final: Linux Mint x64 Viewer-Build,
128/128 native NVIDIA GL/C++23/GUI-Tests, 50/50 Python,
Pi5-kompatibler Headless-Satz und realer isolierter
Xvfb-Viewer-Start sowie X11 Systemicon erfolgreich.
GitHub Code-Commit a47bc2c676624aea92763b0eff67a9cec3a9cd9c;
ARM64 pi5-ogl Run 38960517438 conclusion SUCCESS.
Alle fünf Blöcke im projectplan.md auf FERTIG.
Nächstes WEITER: verbindlich fünf Blöcke Alpha.220–224.
Echter NexVerse-Live-Avatar-/Welt-Pixelnachweis weiterhin OFFEN.
F3 -> Avatarstatus, S -> privater technischer Diagnosebericht
unter ~/.local/state/opengenesislink-viewer/diagnostics/;
kein Login/Session-ID/TexturUUID-Export.

## 16.10.2026 – Alpha.220–224: Verbindungsdiagnose bereits während Welt-Ladevorgang

220: echte getrennte Region-/Handle-/Agent-/Circuit-Statusflags
unter Mutex und Terrain/Objekt/Avatar/Other-Paketzähler im
whitelisted F3-Report, kein zurückgerechnetes WorldReady.
221: F3/ESC/S und klickbarer Lade-DIAGNOSE-Schalter mit Modal
über Original-Welcome-Webseite, Export nur auf Nutzeraktion;
Widescreen-Mausflächen getestet.
222: Manuelles LLUDP-Reconnect nach final beendetem FAILED-Worker,
gleiches sicheres Regionziel, 15s Cooldown, Queue/Motion Reset,
keine parallele/automatische neue Netzwerkverbindung.
223: Fortschrittsbalken ausschließlich Pflichtmilestones bis
AgentPosition, zeitbasierte Hinweise ab 15s und
typisierte begrenzte Enum-Whitelist im Report.
224: alle lokalen Native/GPU/Python/Headless/Xvfb Tests,
Mint-GitHub-Push und Pi5-CI-Freigabe.
Bericht: docs/ALPHA220_224_LOADING_DIAGNOSTICS_AND_MANUAL_RETRY.md.
Echter Login, Welt-/Avatar-Pixel und Real-Retry weiterhin OFFEN.

Alpha.220–224 CI-Abschluss (16.10.2026):
Version 0.2.0-alpha.224, zehn echte Blöcke FERTIG.
GitHub Code SHA 45f4ccc168d1eb4bcd6532e2fb21f3732c1e8eef.
Mint x64 Full CMake/Ninja Build, 130/130 Native/NVIDIA GL CTest,
55/55 Python, Pi5-kompatible Headless-C++23, isolierter
Vollviewer-Xvfb-Start und echtes X11 Taskbar-Icon PASS.
GitHub Pi5 Self-Hosted Runner pi5-ogl
https://github.com/marlin198323-glitch/nexverse-viewer/actions/runs/38963625356
conclusion SUCCESS, matching code SHA verified.
F3 ist jetzt auch bei erfolgreichem Login aber noch fehlender
WorldReady sichtbar; S speichert private Diagnose unter
~/.local/state/opengenesislink-viewer/diagnostics/.
Nur bei endgültigem beendetem FAILED LLUDP Worker kann über
Klick ein neuer Versuch angefordert werden, 15s Cooldown.
Nächster verpflichtender Fünferdurchlauf Alpha.225–229.
Echter NexVerse-Login-Welt-/Avatar-Pixel-Nachweis und echter
Reconnect-E2E nach wie vor nicht getestet.

## 16.10.2026 – Alpha.225–229: Welt-Entry-Verlauf, Reconnect-Gründe, F3 und fünf-Blöcke-Regel

225: Ganz oben in CHATUEBERGABE.md dauerhafte Fünfer-Arbeitsregel
für jeden neuen Chat, in AGENTS/Plan/info gespiegelt; zusätzlich
bounded 5-Einträge WorldEntryTrace ohne Geheimnisse/Netzwerkinfos.
226: Typisierter Grund warum manuelles LLUDP Retry gesperrt/frei
ist (noch arbeitender Worker, Region-Endpoint, 15s Cooldown, etc).
227: F3 privater mode-0600 Export mit Enum-Stufenverlauf,
begrenzter Dauer, manuellem Retry-Zähler und Statuscode.
228: Letzte zwei Stufen + Zeit, Retry-Sperrgrund und Versuchszähler
echter sichtbarer F3-Ladebildschirm, Python Contract und native
Privacy-/Ring-/Policy-Tests.
229: Mint-x64 full/GL/Native/Python/Headless/Xvfb/Systemicon,
GitHub commit/push und pi5-ogl CI.
Bericht docs/ALPHA225_229_WORLD_ENTRY_TRACE_AND_RECONNECT_REASON.md.
Kein noch ungetesteter Live-NexVerse-Welt-/Avatar-/LLUDP-Erfolg behauptet.

## 16.10.2026 – Alpha.230–234: Avatar/World-Optik und echte Mesh-Qualität

230: Tatsächlich empfangenes LLUDP AvatarAppearance Self/Fremd/
Invalid/Region beim Avatar-HUD unterscheidbar, keine UUIDs.
231: GL Terrain-Shader-Textur- vs. Graufallback-Draw,
Region+Materialrevision geprüft, detailreicheres Material
und sichtbarer Bodenstatus.
232: Echter F2 DETAIL/SPAREN-Knopf und Tastatur, reale
High/Medium/Low LOD-Streaming-Policy mit Hysterese/Neuplanung.
233: 5.5m statt 8m Standardkamera und F4 Fokus,
groessere Widescreen-Texte in Avatar/CAPS/World-Status.
234: Mint x64 Native/NVIDIA OpenGL/Python/Headless/
Xvfb/Icon, GitHub und Pi5-CI Freigabe.
Bericht docs/ALPHA230_234_VISIBLE_WORLD_AVATAR_MESH_UI.md.
Echte NexVerse-Bake-/Avatar-/Terrain-Pixelparitaet OFFEN.
\n\n**v0.4.0.0 EventQueue Replay-Fix (24.10.2026):** Im HTTP-200-Sofortduplikat derselben ACK-ID konnte der Long-Poll eine CPU-Busy-Loop verursachen. Nun 150ms pausierter, Region-Lease- und Stop-abbrechbarer ACK-Deduplikationspfad. Native Regression: vier sofortige identische ACKs -1/17/17/17, genau ein zugestelltes Event und >=300ms Gesamtpause. v0.4.0.0-T02 bleibt IN_ARBEIT; Grid-Login, Teleport und tatsächliche WASD-/Yaw-Live-Abnahme weiterhin OFFEN. Kein Main-Merge oder Release.\n
## v0.4.0.0 – Interaktive Abnahme und EventQueue, Batch 02

- Offline-Projektabnahme: 12 Versionen, 36 Aufgaben, 34 Kriterien, 7
  separat ausgewiesene reale Release-Gates. Suche/Filter, Prüfer,
  Status, Befund und Nachweis, lokales Browser-Storage, JSON
  Import/Export und Druckfunktion.
- Lokaler Aufruf: `python3 scripts/ogl-acceptance.py --build`
  und `xdg-open build/acceptance/ABNAHME_DASHBOARD.html`.
  Anleitung: `docs/INTERAKTIVE_PROJEKTABNAHME.md`.
- Exportvalidierung verhindert fremde und veraltete SHA,
  gefälschte Releasefreigabe oder BESTANDEN ohne Beleg.
  Manuelle Reviews verändern die offizielle Roadmap nie.
- CAPS EventQueue: Nonempty HTTP-200 ohne ACK-ID wird nicht zugestellt;
  leere idlose Antworten kontrolliert gepaced; native und
  Python-Regressionen. Siehe Batch-02-Dokumentation.
- v0.4.0.0-T02 weiter IN_ARBEIT, T01/T03 und A01–A03
  authentifizierte Login/Teleport/WASD-E2E weiter OFFEN.
  38,65 % gewichtet, Release BLOCKED, kein main-Merge.

**INTERAKTIVE QA BATCH 02 GEMESSEN:** Linux Mint 185/185 CTests, 643/643 Python, Original-Firestorm-Xvfb-GPU-/GUI-/128px-Systemicon PASS. Selbstständiges Offline-Abnahmefenster nach jedem Git-Commit mit `python3 scripts/ogl-acceptance.py --build` neu generieren, per `xdg-open build/acceptance/ABNAHME_DASHBOARD.html` starten. Manuelle Bewertungen sind keine offiziellen E2E-Releasefreigaben, v0.4.0.0 weiter IN_ARBEIT.

## v0.4.0.0 – Begleitete Live-Abnahme statt technischem Quiz

Acht einfache praktische Prüfschritte: Start, Grid-Login,
Welt/F3, WASD, Yaw, Springen/Fliegen, Teleport und Re-Login.
Jeder Schritt hat tatsächliche Bedienanweisung und erwartete
Beobachtung; die Nutzerseite meldet nur Funktioniert, Fehler oder
Noch nicht möglich. Technische Log-/Simulatorprüfung übernimmt
die Projektentwicklung. Manuelle Antworten mit Zeitstempel werden
im lokalen SHA-gebundenen JSON-Protokoll gespeichert und sicher
validiert, ändern aber niemals reale E2E-Beweise oder den
offiziellen Status. Details: docs/ACCEPTANCE_GUIDED_V0400.json
und docs/INTERAKTIVE_PROJEKTABNAHME.md.
Echte Grid-Live-Endtests A01–A03 weiterhin offen.
Keine Freigabe, kein main-Merge.

## Geführte Abnahme (v0.4.0.0) – QA nachgewiesen

Die echte Chromium-UI wurde im isolierten Headless-Modus bedient:
Geführter Test #1 mit dem Button Funktioniert, Speicherung, Export
der Antwort und unveränderter Releasefreigabe: PASS.
Die Import-/Export-Validierung lehnt unbekannte Schritte,
unzulässige Statuswerte, falsche Versions-/Git-SHA und
erfundene Releaseberechtigungen ab. Der geführte Testplan
besteht aus 8 eindeutigen, auf die Roadmap referenzierten Schritten.

Lokale Linux-Mint-Abnahme: 185/185 native CTests, 647/647
Python-Tests und original Firestorm-Xvfb-GPU-/GUI-/128px-X11-
Taskleistenicon PASS, vollständiger CMake/vcpkg-x64-Debug-Build.
Das Dashboard kann per scripts/ogl-acceptance.py --build
zur aktuell geprüften Git-SHA neu erstellt werden.

**Nicht abgenommen:** Aktuelle authentifizierte Live-Grid-Anmeldung,
CAPS-/Teleport-Region-Endtest, serverbestätigte WASD-/Yaw-/Höhen-
bewegung; weiterhin v0.4.0.0 IN_ARBEIT und v1 RELEASE BLOCKED.
Der Nutzer soll nur die acht visuellen Testschritte und Beobachtungen
liefern; die technischen Beweise bleiben Aufgabe der Entwicklung.


## Abnahme v0.4.0.0 – Schritt 3 Welt/F3, 25.10.2026

Login/CAPS/UDP und sichtbare Region bestätigt, aber untexturierte
Oberflächen, unvollständige Meshes und fehlender vollständiger
Avatar-Skin/Outfit-Nachweis aus zwei Screenshots.
Reales Sitzungslog 15:40–15:48: 784 GetMesh GPU Upload
`gpu_failure=capacity`, 43 GetTexture-Fehler, 2 ungültige
Mesh-LODs. Ursache P0: Renderer begrenzt auf 256 verschiedene
GPU-Mesh-Assets in `src/render/opengl_renderer.cpp`.
P0 OGL-WORLD-003-MESH-CAPACITY, P1 OGL-WORLD-003-TEXTURE-DECODE
und OGL-WORLD-003-MESH-INVALID, Avatar-E2E weiter P0 OFFEN.
**Schritt 3 als fehlerhafte Teilabnahme protokolliert**, kein
Release-Erfolg. Details in
`docs/ABNAHME_V0400_SCHRITT03_WELT_ASSET_GPU_2026-10-25.md`.
Nächstes: Schritt 4 WASD mit serverseitiger Positionsbestätigung.

## v0.4.0.0 – Echt programmierter Neon-Login gemäß erster Bildvorlage

C++/OpenGL Login vollständig neu gestaltet: ursprüngliche
Live-NexVerse-Welcome-Seite ohne 56%-Schleier, kein mittiges
Popup, großes OGL-Header-Branding, Grid-/Live-Status, breite
responsive glasartige Avatar-/Passwort-/WELT-BETRETEN-Leiste,
echter Passwortanzeigen-Toggle. Ein identisches Layout wird
für GL-Darstellung und reale GLFW-Klicks auf 800/1100/1280/
1648/1920/3440px verwendet; maximiertes, aber dekoriertes
und frei resizables Viewer-Fenster. Originaler Login-/ToS-/
CAPS-Backendpfad bleibt erhalten. Testbasis: 1 nativer
Multi-Auflösungs-CTest, 11 Python-Verträge und echte isolierte
1648x928 OpenGL-Screenshot-Inspektion.
Details: docs/V0_4_0_0_NEON_LOGIN_REFERENCE_IMPLEMENTATION.md.
Offen bleiben authentifizierte Grid-E2E und nicht interaktive
WebKit-Offscreen-Links. Fortschritt 38,65 % (ca. 39 %);
v0.4.0.0 weiter IN_ARBEIT, kein main-Merge/Deploy.

**V0.4.0.0 NEON LOGIN LINUX-MINT-ABNAHME (25.10.):**
Vollständiger x64-CMake/vcpkg-Debug-Build und 186/186 native
CTests, 658/658 Python, isolierter echter Firestorm-Xvfb-GPU-/GUI-/
128px-OGL-Systemicon-Test bestanden.
GL-Login im 1648×928-Referenzformat lokal visuell überprüft.
Neues NeonLoginLayout ist responsiv und in Maus-/Renderpfad
gemeinsam implementiert. Weiterhin keine authentifizierte
Grid-Login-/WASD-/Outfit-Endabnahme dieser Code-SHA,
kein main-Merge/Produktivdeploy.

## v0.4.0.0 – Login-UI kompakt und Schließen beschleunigt (25.10.2026)

Gemäß echten User-Screenshots Header 93 -> 72 px und
unteres Login-Dock 105 -> 77 px bei 1648x928.
Eingaben 298x49 -> 245x39 px, Welt-Betreten
259x49 -> 210x39 px. Echte WebKit-Hero-Seite
nutzt zusätzliche Bildhöhe, Input-Fokusrahmen
nicht mehr leuchtend, stattdessen dezente Färbung.
Aktiver CurlHttpTransport unterstützt atomaren Abbruch
aller aktiven HTTP-Übertragungen; die Viewer-Schließroutine
fordert LLUDP/CAPS/HTTP-Stop VOR GPU-Aufräumen an.
Echter C++-Loopback-Stall-/Abbruchtest und
`Viewer shutdown cleanup elapsed_ms`-Log hinzugefügt.
Reguläres Herunterfahren in echter eingeloggter
Grid-Sitzung bleibt noch zu messen; kein neues
vollständiges Release-E2E. Einzelheiten in
`docs/V0_4_0_0_NEON_COMPACT_AND_FAST_SHUTDOWN.md`.

**KOMPAKTES NEON-DESIGN + SCHLIESSEN QA BESTANDEN:** Linux Mint 187/187 native CTests, 666/666 Python, Original-Firestorm-Xvfb-OpenGL-/GUI-/128px-X11-Systemicon PASS; echter HTTP-Stall/Stop-Loopback-Test PASS. Realer eingeloggter Viewer-Abschluss und Grid-Funktionsparität weiterhin offen. Projektversion v0.4.0.0, 38,65 % gewichtet, main NICHT ändern.

**T02 Region-CAPS-Reconnect (25.10.2026):** Erstes fehlgeschlagenes Seed-CAPS/EventQueueGet kann in derselben Region mit 5–60s Cooldown neu gestartet werden; alte Worker dürfen aktuelle Region nicht deaktivieren; HTTP 404 + gleicher Queue-URI setzt ACK zurück. Native Region-Tests inkl. HTTP503/404/Lease PASS, Linux Mint x64 komplett 188/188 CTests, 680/680 Python, Firestorm Xvfb/GL/X11-Icon PASS. Echte Live-Grid-Region/Teleport-Abnahme offen. Dokument: docs/V0_4_0_0_REGION_EVENTQUEUE_RECONNECT_20261025.md. Projekt 39 %; v0.4.0.0 IN_ARBEIT.

**v0.4.0.0 P0 CAPS-REPLAY Login + Region (25.10.2026):** Die Login-CAPS- und Region-CAPS-EventQueue schützen jetzt beide vor nichtadjazenten Batch-Replays (z.B. 17/18/17), behalten beim Replay den aktuellen ACK, resetten die 64er Recent-Historie nach 404 selbst bei identischer URL und pacen ID-lose leere 200-Antworten. Workerlogs melden ausschließlich Zähler, keine URLs/Token/UUID/Eventinhalte. Linux Mint vollständig: 188/188 native CTests, 685/685 Python, Firestorm Xvfb OpenGL GUI und X11-Systemicon PASS. Authentifizierter Live-Grid-Login/Teleport/Avatar-/WASD-Nachweis OFFEN. Details docs/V0_4_0_0_CAPS_REPLAY_LOGIN_REGION_FIX_20261025.md. Version v0.4.0.0 IN_ARBEIT, Projektfortschritt weiterhin 39 % (38,65 %). Kein main-Merge, kein Release.

**P0 v0.4.0.0-T03 – POST-TELEPORT BEWEGUNG (25.10.2026):** Der WASD- und Steigen/Sinken-Nachweismonitor darf nach einem großen Simulatorpositionssprung nicht mehr die VOR dem Teleport gesendeten AgentUpdate-Datagramme für neue Serverpositionsänderungen als Beweis verwenden. Beide Monitore setzen die socket-sent-Freigabe zurück und zeigen 'neuer UDP-Versand ausstehend'; reine UI-Queue-Eingaben bleiben unbestätigt. Erst tatsächlicher neuer UDP-Send und authentischer Simulatorpositionsreadback können Bewegung/Höhe erneut belegen. Native Regressionen und vier Python-Verträge ergänzt. Linux Mint: 188/188 CTests, 689/689 Python, Firestorm-Xvfb-OpenGL-GUI/X11-Icon PASS. Authentifizierter Live-Teleport, COF/Skin und WASD/Flugserverreadback OFFEN. Details docs/V0_4_0_0_POST_TELEPORT_MOVEMENT_PROOF_20261025.md. Status v0.4.0.0 IN_ARBEIT, Projekt 38,65 % (39 %); kein main-Release.

**P0 v0.4.0.0 FastRez/Skin + Mesh-Log (25.10.2026):** Authentifizierter Logbefund vom 25.10. 15:40:08: Low158 SELF wurde vor Circuit-Ready gepuffert und danach übernommen; bislang fehlte in diesem Pfad die Head/Upper/Lower-Bake-Diagnose. Ergänzt sind reine Anzahl-Meldungen 'body_bakes_low158', 'body_bakes_candidates', 'provisional_object_faces' ohne Asset-IDs und ohne falsche GPU-/COF-Behauptungen. 102.762 Mesh-Upload-Erfolgseinträge im historischen Log werden durch erste/dann jede 128. aggregierte Erfolgsinfo reduziert (keine GPU-Funktionsänderung). Lokale Tests: 189/189 native, 694/694 Python, Firestorm-Xvfb-GL/GUI/X11-Icon PASS. GetMesh-LOD-invalid_faces sowie GetTexture-JPEG2000-Metadatenwarnung als nächste konkrete Assetfehler offen. Dokument: docs/V0_4_0_0_FASTREZ_BAKE_DIAG_AND_MESH_LOG_20261025.md. Live-E2E-Avatar/Outfit bleibt offen. v0.4.0.0 IN_ARBEIT; 39% (38,65% gewichtet), kein main-Merge.

**P0 v0.4.0.0 GetMesh/JPEG2000 25.10.2026:** Valid triangles within an OpenSim mesh face survive out-of-range or degenerate sibling triangles; fully invalid faces/LODs still reject and use normal LOD fallback. Error counts: invalid_faces/invalid_triangles and successful skipped_invalid_triangles. JPEG2000 reports reason=image_extent, image_dimensions, channel_count, component_dimensions, component_subsampling or component_precision, with existing 4096px/64MiB/16bit caps unchanged. Local Mint gates: 190/190 CTests, 699/699 Python, Firestorm-Xvfb OpenGL/GUI/X11 icon PASS. Real authenticated J2C asset not yet retested; Avatar/COF/skin E2E and live shutdown pending. Documentation: docs/V0_4_0_0_MESH_TRIANGLE_RECOVERY_J2C_METADATA_20261025.md. Project still 39% (38.65% weighted); v0.4.0.0 IN_ARBEIT, no main merge.

**P0 v0.4.0.0 GetTexture PNG-/J2C-Verwechslung (25.10.2026):** Manche GetTexture-Simulatorantworten können trotz angefordertem PNG einen nativen J2C-/JP2-Bytestream enthalten. Der Viewer erkennt nun J2C-SOC oder JP2-Signatur und verwendet erst nach vollständiger, validierter OpenJPEG-RGBA-Dekodierung direkt diesen Body ohne zweite HTTP-Anfrage. Fehlende/abgeschnittene Signatur oder J2C-Decoderfehler führen weiterhin zur unabhängigen x-j2c-Downloadprüfung; Sicherheitslimits bleiben unverändert. Native Netzwerk-Mocktests J2C+JP2+truncated und separater Signatur-CTest ergänzt. Linux Mint: **191/191 native CTests, 704/704 Python, Firestorm-Xvfb-GL/GUI/X11-Icon PASS**. Historische JPEG2000-Metadatenablehnungen im authentifizierten Grid nicht neu geprüft. Details: docs/V0_4_0_0_TEXTURE_PNG_NATIVE_J2C_RECOVERY_20261025.md. v0.4.0.0 IN_ARBEIT, Projekt **39 % (38,65 % gewichtet)**; main unverändert.

**P0 v0.4.0.0 AVATAR BAKE 3/3 LIVE-TRACE (25.10.2026):** Die bisherige avatar_skin_blocker_key-GL-Stufe konnte 3+ Aufrufe aus nur einem Körperteil als drei vollständig gezeichnete Body-Bakes zählen. Korrigiert: Nur drei jeweils CURRENT-Low158-identische, tatsächlich auf originalen Firestorm-LLM-Head/Upper/Lower-Teilen gebundene und gezeichnete Body-Texturen erfüllen die Stufe; acht Head-Draws sind keine 3/3. Native Userdiagnose exportiert einzelne Head-/Upper-/Lower-Zeichnungswerte. Neuer privatsphäreschonender AvatarBakeProgressTrace protokolliert nach authentifiziertem Login pro Region nur Statusänderungen (bei Blockade maximal alle 30 Sekunden, schnelle Zustandswechsel mindestens 250 ms), GetTexture-Stage 0-6, Server/ObjTE-Quelle 0-2, GPU/GL/Depth-Stufen; KEINE UUIDs, Credentials oder Endpoint-URLs. Regionwechsel und Logout setzen History zurück, kein automatischer Diagnose-Dateiexport. Linux Mint: 192/192 native CTests, 710/710 Python, Firestorm-Xvfb-GL/GUI und X11-Icon PASS. Echte Head-/Upper-/Lower-RGB- und COF-/Outfit-Grid-Abnahme weiterhin OFFEN; ebenfalls authentifizierter Shutdown. Dokument: docs/V0_4_0_0_AVATAR_BAKE_3OF3_LIVE_TRACE_20261025.md. v0.4.0.0 IN_ARBEIT, Projekt 39 % (38,65 %). main unverändert.
