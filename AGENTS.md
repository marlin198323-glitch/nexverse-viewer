**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.754, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.755–764.**

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

# nexverse-viewer – Projektanweisungen

**ARCHIV 16.10.2026: IMMER 10 ECHTE ENTWICKLUNGSBLÖCKE PRO WEITER; Alpha.255–264 inzwischen abgeschlossen.** Nach jedem Durchlauf Prozentfortschritt berechnen, GitHub/Mint/Pi5 CI validieren.

## VERBINDLICH: GESAMTFORTSCHRITT BEI JEDEM STATUS IN %

**Bei jeder Meldung zum OGL-Projektstand, jedem Zehnerdurchlauf und in jedem neuen Chat IMMER den Gesamtfortschritt als XX % nennen, auch ohne Veraenderung.** Historisch Alpha.254: **ca. 39 % gewichtete Schaetzung**. Einzige Quelle docs/PROJECT_PROGRESS.json, pruefen mit python3 scripts/project-progress.py. Bei nachgewiesenen Aenderungen JSON, docs/PROJEKTFORTSCHRITT.md, projectplan.md, CHATUEBERGABE.md und info.md gemeinsam aktualisieren. Alpha-Version oder gruene CI-Testquote sind KEIN automatischer Fertigstellungsgrad.

## VERBINDLICHE ZEHN-BLÖCKE-REGEL (16.10.2026)

**NEUER CHAT: zuerst CHATUEBERGABE.md mit zehn verpflichtenden Entwicklungsblöcken und dem jeweils letzten Commit lesen. Bei JEDEM WEITER ohne Rückfrage alle zehn echten Blöcke inkl. Mint-Build/GitHub/Pi5-CI bearbeiten, niemals nur Plan schreiben.**

Bei jedem WEITER zehn echte zusammenhängende Entwicklungsblöcke mit Code+sichtbarer UI, Tests, GitHub-Servercheck, vollständigem Mint-Build und Xvfb, GitHub-Push und Pi5-CI; Status und Block-IDs in projectplan.md nachführen. Blockiert != fertig. Keine Pi-Entwicklung, kein sichtbarer Desktop-Viewer-Start. Diese Regel gilt auch in neuen Chats.

- **OpenGenesisLINK Upstream main (17.10.2026):** bf55a17524a9f3f175c56d2adeaa4d39083f446f (OGL AssetService Addon #231, 86 weitere Commits seit d8f4426), getrennte deaktivierte Serverseite; nicht ohne Auth-/API-/Security-Review in Viewer oder Produktion zwangsintegrieren. Projektplan P1 optionaler Vertrag in Alpha.255–264.

- **Alpha.254 funktionale Terrain-Reparatur (17.10.2026):** Nur fehlende Terrain-RGBA-Layer bis vier Versuche mit Backoff erneut laden; erfolgreiche Layer je Materialepoch erhalten. Original-GL akzeptiert höhere Vollständigkeit in derselben Revision, verwirft Downgrades; Terrain-Draw-Metriken werden PRO FRAME reset. NVIDIA GPU-Promotion-Test, F7 Retrystatus, Self-AvatarAppearance-Paketzähler-Reset bei Regionswechsel und echte Self-Keyframe-Skin-Logs/Animations-UI. Gesamtfortschritt **39 % (vorher 38 %)**. Weiter 10 echte Code+GUI-Blöcke Alpha.255–264, Mint/Vollbuild/Xvfb/GitHub/Pi5-CI obligatorisch. Kein authentifizierter Grid-Avatar-Pixelbeweis.

## Verbindliche OGL-Arbeitsumgebung ab 16.10.2026 (neueste Nutzeranweisung)

- **Alpha.234 VisibleWorld/Avatar/LOD (16.10.2026):** Echten LLUDP-SelfAppearance-Status und aktuelle GL-Terrain Textur/Fallback-Draws im HUD melden. F2 DETAIL/SPAREN schaltet echte Mesh-LOD-Policy mit Hysterese und Neuplanung, 5.5m Standard-/F4 Kamera und lesbarere HUD-Texte. Fuenf echte Bloecke pro WEITER mit Mint FullBuild, GitHub und Pi5-CI zwingend, keine falsche Grid-/Bake-E2E-Fertigmeldung.
- **Alpha.224 Welt-Lade-Diagnose (16.10.2026):** F3/ESC/S schon ab authentifiziertem Ladefenster ohne WorldReady; echte Region/Agent/LLUDP/CAPS-Daten und Enum-Whitelist-Bericht. Manuelles Circuit-Retry nur auf expliziten Klick bei beendetem FAILED Worker, bekanntem Regionziel und 15s Cooldown, niemals parallele oder ungefragte UDP Worker. Ladefortschritt nur Pflichtstufen, nicht optionale Texture/CAPS/Objektpakete. Fünf-Blöcke-Regel, Mint FullBuild/Xvfb, GitHub und Pi5 CI.
- **Alpha.219 Avatar-Diagnose/F3 (16.10.2026):** Alte Async-GL-Depth-Sichtbarkeits-Daten bei Kamera-/FoV-Wechsel oder fehlendem originalen Meshdraw invalidieren. F3 Avatarstatus zeigt echtes GPU-Depth-/Bake-/Kamera-Evidence und "Diagnose speichern [S]" als bedienbare UI. Report nur nach bewusstem Klick in private Datei Mode 0600 ohne UUIDs, Login/Session-Secrets oder Chat. Fünf echte Blöcke inklusive Mint-Full-Build, Xvfb, GitHub und Pi5-CI sind bei jedem WEITER verpflichtend. Echter NexVerse-Live-Avatar-Login bleibt offen.
- **Alpha.214 Avatar-Samples/F4 (16.10.2026):** Asynchrone GL_ANY_SAMPLES_PASSED für originalen Self-Firestorm LLM-Körper, Regionen-Epoch invalidiert; AVATAR [F4] ist echte klickbare Frontkamera. Keine glFinish im Produktiv-Viewer. Alle fünf Arbeitsblöcke 210–214 von Pi5-CI getestet.
- **Alpha.209 Avatar/Welt-Entry (16.10.2026):** Welt-Loadpanel nennt tatsächliche blockierende Stufe (LLUDP/Region/Agent) statt 100% trotz fehlender Szene. Avatar-Proof projektiert authentischen Agentkopf ins Blickfeld, trennt Original-LLM-GL-Draws/Proxy von Appearance-Bakes; keine Avatar-Pixelbehauptung. Original-Brandicon bleibt im Login und in Welt-HUD. F5-Hinweis nur bei fehlendem Avatar. Native/Python-GPU/Xvfb/CI-Gates und GitHub docs fortführen.
- **Alpha.204 Original-Branding (16.10.2026):** Nutzer-Originalicon verbindlich für Login-Header und echtes Cinnamon-Taskleistenicon (_NET_WM_ICON RGBA/X11, WM_CLASS), Linux Desktop-Entry sowie Windows .ico/macOS .icns. Kein neues Bild/Mockup und KEIN Austausch des bestehenden WelcomePage WebKit Browserinhalts. GUI-Tests ausschließlich über Xvfb, native + Headless-CI vor GitHub-Commit.
- **Alpha.199 Avatar-Orbit (16.10.2026):** Front-, Seiten- und Rueckansicht des eigenen Avatars per V/[ ] und drei echte UI-Buttons. Kein AgentUpdate beim Kameradreh; Welt-Renderer, Raycast und Asset-Priorisierung verwenden dieselbe Pose. NaN-Zoom-Fallback gilt auch im Kamera-Nenner. NVIDIA GPU Framebuffer Front/Back und Headless Native Tests vor Commit, Voll-Viewer nur Xvfb.
- **Alpha.194 Zoom (16.10.2026):** Echte Kamera 3..24m per Wheel, Keyboard und sichtbaren UI Plus-/Minus-Buttons. Identische Pose für OpenGL, Objektpick und Texture-/Mesh-Priority. F9 B wechselt read-only Server-Bakes. Mint GPU-Framebufferzoom-Test + kompletter Xvfb-Smoke vor GitHub und Pi5-Headless-CI; keine Desktop-Popups.
- **Alpha.189 3D-Weltfokus (16.10.2026):** Bedingter HUD-F5-Toggle und gleich positionierter echter Mausbutton. Im versteckten Zustand Welt-Mausklicks zur Objektauswahl routen, keine unsichtbaren Menüs aktivieren; volle Firestorm-Simulator-/GL-Funktion bleibt aktiv. Szenendraws hinter Blickebene und jenseits far_clip begrenzen; Nvidia-GL-Framebuffer-Tests plus C++23-Toggle-/Culling-Regressions vor jeder Freigabe. Vollständiger GUI-Smoke ausschließlich Xvfb.
- **Alpha.184 Kamera (16.10.2026):** Zum Rendern Third-Person-ServerQuaternion normalisieren, endliche Position/Yaw verwenden und ungültige Serverpose als sichtbaren Kamera-Fallback statt erfolgreich gezeichnete Welt ausgeben. Native C++23 Third-Person-Tests und versteckte NVIDIA-Framebuffer-GL-Pixeltests sind Pflicht. Kein kompletter Viewer automatisch sichtbar auf DISPLAY=:0 starten.
- **Ab Alpha.179 (16.10.2026):** Inworld-Weltstatus zeigt eindeutige Texture-/Mesh-UUIDs und tatsächlich GPU-residente passende Assets, nicht nur Asset-Requests. Verbindungs-/System-/Avatar-Diagnose nur bei ausreichender rechter Rail-Höhe oberhalb der FPS-Leiste zeichnen. Native Mint-x64-/NVIDIA-Tests, Headless-CI und isolierter Xvfb-Vollviewer-Smoke bleiben Pflicht; kein sichtbarer Viewer-Test auf User-Desktop ohne Auftrag.
- **Ab Alpha.174 (16.10.2026):** Inworld-Avatar-Pipeline zeigt authentifizierte AvatarAppearance, Firestorm Server TE UUIDs, aktuelle GPU-residente Bake-IDs und echte LLM-/BakeGL-Submissions getrennt. Keine positive Avatar-Fertig-Behauptung ohne alle drei BakeGL-Draws. System/CAPS-Status-Layout nicht überdecken. Bei voller Viewer-GUI nur isoliertes Xvfb, keine sichtbare Desktop-App ohne Nutzerauftrag.
- **Ab Alpha.169 (16.10.2026):** Bei verbundenem Simulator und gültigem Agent dürfen Szene/UI bereits vor Terrainpaketen rendern: WorldModel::scene_ready() ist der App-Gate, ready() bleibt strenger Terrain-Gate. Terrainfortschritt/WorldModel-Objekte/GL-Draws/CAPS-Status getrennt darstellen. Kein wirklicher Welt-oder Avatar-Login-Beweis ohne autorisierten echten Gridtest. Voll-Viewer weiterhin niemals ungefragt auf Desktop starten.
- **Ab Alpha.164: Viewer darf bei ChatGPT-Entwicklung nie ungefragt sichtbar auf dem Mint-Desktop gestartet werden.** Native OpenGL-CTests erzeugen GLFW_VISIBLE=false Fenster; vollständige Starttests ausschließlich mit scripts/mint-isolated-gui-smoke.sh unter xvfb-run, ohne Fallback auf vorhandenes DISPLAY. Nur explizite Nutzer-Anweisung autorisiert sichtbar gestarteten Viewer. Linux-x64 CTest/GPU und Mint-Desktop bleiben primär, Pi5 nur headless CI. Siehe Alpha.160-164 Bericht.

- **Primäre interaktive Entwicklungsmaschine ist wieder der Linux-Mint-PC `jam-MS-7D28`** über Remote Desktop Commander (Device-ID `3a1e9ba9-ea8a-4650-b8f7-84f8f653f20d`). Beim Start `list_devices` prüfen und ausdrücklich dieses Gerät auswählen.
- **Alpha.159 Stand 16.10.2026:** Originale lokale Nightly-7.2.5 Firestorm Character-Dateien liegen nur im ~/.cache/nexverse-viewer/firestorm-nightly-7-2-5-81754/character, 96/96 Dateien SHA256-gleich mit Release 7.2.4. Neue Viewer-Änderungen auf Mint mit vollständigem x64 Build/NVIDIA-GPU-CTest prüfen, nach Push Pi5-CI prüfen. Authentifizierte Welt/Outfit-Tests sind offen; nicht mit Start-/Headless-Tests verwechseln.
- **Verifizierter Mint-Checkout am 16.10.2026:** /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer, CMake debug mit /home/jam/vcpkg, RTX 4070 SUPER OpenGL 4.6. Originale Firestorm-Assets bleiben lokal, keine Binaries davon committen.
- **GitHub bleibt führend:** `marlin198323-glitch/nexverse-viewer`, Branch `develop-current`; vor jeder Änderung Git-Status/Remote-SHA abgleichen, lokale uncommittete Änderungen auf Mint nicht überschreiben. Änderungen dort programmieren, lokal nativ/GUI testen und danach zu GitHub pushen.
- **Raspberry Pi 5 `s359142.srv.nexvortex.de` nur als Self-hosted-CI-/ARM64-Test-Runner** für OGL einsetzen, nicht als primären interaktiven Editor, Build-/Grafikrechner oder Ersatz für den PC. Die Pi-Anweisungen unten dokumentieren den historischen Stand bis Alpha.149 und sind in dieser Hinsicht überholt.
- Falls `jam-MS-7D28` offline ist, keine PC-Checkout-/Build-/GUI-Ergebnisse behaupten. Für rein dokumentarische Aktualisierungen darf der GitHub-Connector genutzt werden; keine eigenmächtige Verlagerung der OGL-Programmierarbeit auf den Pi.
- Lokalen Mint-Pfad und tatsächlichen Git-Status bei Wiederverbindung **neu ermitteln**, nicht vom früheren Pfad ausgehen. Als letzter bestätigter GitHub-Stand liegt Alpha.149 auf `develop-current` (`b7dabf4b805d7d9beb93a4452e19a23e66a63e46`); zwischenzeitliche neue Commits immer zuerst prüfen.

## OpenGenesisLINK Viewer (OGL)-spezifisch
- GitHub: `marlin198323-glitch/nexverse-viewer`; Pi-Pfad: `/srv/nexdev/development/projects/ogl-viewer`.
- **Verbindlicher aktueller Arbeitsbranch `develop-current`, nicht `main` und nicht der ältere divergierende `develop`.** Bestehende lokale Linux-Mint-Kopie nicht löschen oder blind zurücksetzen.
- Zuerst `CHATUEBERGABE.md`, `projectplan.md`, `info.md`, Architektur- und Firestorm-Kompatibilitätsdokumentation lesen.
- Firestorm-Feature-Baseline erhalten/implementieren; vor jedem relevanten Block den aktuellen OpenGenesisLINK-Server-Stand in `opengenesislink/opengenesislinkserver` prüfen. GUI/UX parallel zu Protokoll, Bewegung, Welt-/Avatar-/Meshdarstellung entwickeln.
- Pi ARM64 eignet sich für ausgewählte Quelltext-/CMake-/Headless-Tests; vollständige Windows/macOS- und Linux-x64-GUI/GPU-Builds auf geeigneten Hostsystemen ausführen. Nicht behaupten, damit Firestorm-Parität oder GUI-E2E geprüft zu haben.

# Historischer Pi5-Entwicklungsstand (15.10.2026, nur Archiv)

Der folgende historische Abschnitt ist für interaktive OGL-Entwicklung **überholt**. Maßgeblich sind ausschließlich die neueren Mint-/GitHub-Regeln oben. Pi5 bleibt nur ARM64 CI.

## Zentrale Arbeitsumgebung
- Raspberry Pi 5 (ARM64), Gerät in Remote Desktop Commander: `s359142.srv.nexvortex.de`.
- Device-ID: `76f55079-2e1f-4660-a6b5-4c9ac490b3be`. Bei mehreren Geräten in ChatGPT `list_devices` ausführen und exakt diesen Pi auswählen, nicht `jam-MS-7D28`.
- Angemeldeter Linux-Arbeitsbenutzer des Commander-Dienstes: `nexdev`; Home: `/home/nexdev`. Für Programmierarbeiten keine Root-Sitzung verwenden.
- Projektbasis auf dem Pi: `/srv/nexdev/development/projects/`.
- Zentrale Pi-Anweisung: `/srv/nexdev/development/PI5_ENTWICKLUNGSREGELN.md` (nur bei Remote-Zugriff lesbar).
- Desktop Commander ist als `nexdev-commander.service` eingerichtet und war zuletzt online. Verbindung vor jedem Zugriff prüfen. Auto-Neustart eingerichtet, Neustart-Test noch nicht abgeschlossen.

## Verbindlicher Ablauf bei „weiter“
1. Zuerst dieses `AGENTS.md`, die vorhandene Chat-Übergabe, den Projektplan und den aktuellen Branch/Commit prüfen. Keine alten Statusmeldungen als gegenwärtig annehmen.
2. GitHub bleibt die führende Quelle für alle Änderungen. Entweder direkt über den GitHub-Connector programmieren oder über Desktop Commander in einem überprüften Pi-Checkout arbeiten; niemals zwei parallele Schreibvorgänge auf dieselben Dateien/Branches unkoordiniert starten.
3. Auf dem Pi vor Bearbeitung Git-Status, Remotes und Branch prüfen, neue Remote-Commits abholen und lokale Änderungen bewahren. Kein blindes `reset --hard`, `clean -fd`, Überschreiben oder erzwungenes Pushen.
4. Mehrere zusammenhängende Entwicklungsblöcke (auf Nutzerwunsch typisch 4–5 größere Blöcke) über Core, Renderer, GUI, Tests und Protokoll gemeinsam bearbeiten; Tests gezielt und reproduzierbar ausführen. Ergebnisse und offene Prüfungen unterscheiden.
5. Automatische Tests nach GitHub-Pushes werden künftig von separaten **GitHub Self-hosted Runnern** auf dem Pi ausgeführt. **Stand 15.10.2026: alle fünf Runner sind registriert und wurden mit erfolgreichem GitHub-Runnersmoke getestet. Reguläre Projekt-Tests sind davon separat zu prüfen.** GitHub sendet CI-Jobs an Runner, niemals an Desktop Commander. Commander ist für interaktive Entwicklungsarbeit/Diagnose.
6. Pi hat 8 GB RAM: umfangreiche Builds und parallele Jobs begrenzen, CPU/RAM/SSD kontrollieren. Nicht unterstützte Plattformen (z. B. Windows/macOS/x64 GUI) auf geeigneten Build-Hosts testen.
7. Entwicklungstests nur mit isolierten Benutzerrechten und Testdaten ausführen. Keine Produktion, produktive DB, echte Kundendaten oder Produktionssecrets für CI nutzen. SSH-Keys und Zugangsdaten nicht in Code/Logs/Chats ablegen.
8. GitHub-Actions-Minuten sparen: zuerst lokale gezielte Prüfungen, später Self-hosted CI und Caches, GitHub-gehostete Komplettbuilds nur für passende Releases. Bestehende CI/CD-Trigger vor Änderungen prüfen; keinen Prod-Deploy durch reine Entwicklung auslösen.
9. Pro Durchlauf geänderte Dateien, Commit-SHA, tatsächlich gelaufene Tests/Ergebnis, Fehler, offene Punkte und nächste Aufgaben in bestehenden Projekt-/Übergabedateien nachführen, ohne diese zu ersetzen oder neue widersprüchliche Pläne anzulegen.
10. Projekt-Code und CI-Konfiguration nicht als „fertig“ ausweisen, bevor echte Tests bzw. freigegebene Produktionsprüfung stattgefunden haben.

**Wichtig für ChatGPT Projects:** Dieses Repository-Dokument verändert die separaten ChatGPT-Projektanweisungen nicht automatisch. Dort einmalig Anweisung hinterlegen, zu Beginn dieses `AGENTS.md` des jeweiligen Repositories zu lesen.

## Stand Pi5-Runner-Migration (15.10.2026)
- Alle fünf Repo-Runner wurden auf Raspberry Pi 5 registriert und per GitHub Actions Smoke auf separatem Branch erfolgreich getestet.
- Pi5 develop-current CI: Quelltext-/Bash-Prüfung plus native ARM64 C++23 und Python-Regressionstests. CMake/vcpkg und Windows/macOS/Linux-x64 GUI/GPU-Builds bleiben offen.
- Ein erfolgreicher Smoke-Test beweist Runner-/Checkout-Funktion, nicht die vollständige Projekt-Testabdeckung.
