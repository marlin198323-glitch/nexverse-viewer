# Alpha.1025–1034 – tatsächliche Avatar-Renderer-Bakequelle je Körperteil zuordnen

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | OGL 0.2.0-alpha.1034

## Zehn technische Entwicklungsblöcke

1025. Linux-Mint Desktop Commander jam-MS-7D28 verbunden, GitHub develop-current von 4281297 synchron und Worktree sauber; zuerst CHATUEBERGABE.md, projectplan.md sowie Alpha.1015–1024 gelesen.
1026. OpenGenesisLINK Server main gegen GitHub verifiziert (f56e354694a41adb2334884c21efc45db784cb92). Keine neue Low158-/ObjectUpdate-/GetTexture-Wire-Anforderung.
1027. Widerspruch im bisherigen F3 aufgedeckt: Renderer nimmt WorldModel::self_bake_selection(), die Alpha1024-GetTexture-Diagnose betrachtete dagegen ausschließlich Low158. Bei teilweise fehlenden Low158-Bakes kann das aktuelle selbstidentifizierte ObjectUpdate pro Slot einen provisorischen Ersatz liefern, den die Diagnose unterschlug.
1028. Gemeinsame read-only Assets-Inspektion um SelfBakeSource{none,self_low158,provisional_object_te} und SelfRenderBakeGetTextureEvidence aus tatsächlichem selektiertem IDs-/Sourceflags erweitert. Autoritatives Low158 hat immer Vorrang; ein ObjectUpdate ist niemals Beweis für authentifiziertes COF.
1029. Lookup der GPU-Residency und regionsepochgebundenen Fehlerklasse erfolgt NUR für die bereits ausgewählte und eindeutig gekennzeichnete aktuelle Body-ID. Fehlende IDs, Placeholder oder IDs ohne akzeptierte Quellenkennzeichnung können weder GPU- noch HTTP404-Beweis ergeben.
1030. Echten Application-Frame integriert: Self-AgentID und SessionID müssen zur aktiven Sitzung passen, Region/Firestorm-Lease aktiv sein, eigene Pose vorhanden sein, der GPU-Admission-Self-Proxy muss akzeptiert und die aktuellen gewählten IDs müssen identisch sein. Abweichende/ältere Frames erhalten keinen Erfolgseintrag.
1031. F3 zeigt statt irreführend reiner Low158-Werte explizit H:L/GPU U:O/HTTP404 etc.; L=Low158 und O=provisorische ObjectUpdate-Bakes. Der ursprüngliche reine Low158-Nachweis bleibt als separate Sicht erhalten.
1032. Strg+S-Sicherheitsbericht um drei nummerische Quellen- und drei nummerische renderer-effective GetTexture-Statusfelder erweitert; sämtliche Agent-/Session-/Asset-UUIDs fehlen in den neuen Reportfeldern. Kein neues Netzwerk, keine Änderung am Retryplan oder COF-Bake-Status.
1033. Native Tests für gemischte Low158-/ObjTE-Slots, echtes aktuelles Body-404, neues Low158 überlagert altes ObjTE, ausschließlich ObjTE vor Low158, Placeholder, Epoch-Wechsel, Private-Report-Nummern. Python prüft WorldModel-Vorrang, echte App-Sitzung/GPU-Admission und F3-/Report-Provenienz.
1034. VERSION, alle Start-/Übergaberegeln, Projektplan, Fortschritt-JSON, Firestorm-/Server-/UI-Paritätsmatrizen angepasst. Vollständiger Mint-x64-Build, alle CTests und Python sowie isolierter GPU- und GUI-/X11-Icon-Test vor GitHub Push; Pi5 ARM64 exact SHA separat. Kein Produktivdeploy.

## Grenzen

Korrigiert ist die Zuordnung zu den tatsächlich für den aktuellen Renderframe ausgewählten TextureEntry-Quellen. Nicht bewiesen ist, dass pro tatsächlichem NexVerse-Login alle 3 authentifizierten Body-Bakes existieren, vom Server als HTTP200 geliefert und sichtbar als finale Skin-Pixel gerendert werden. Provisorische ObjectUpdate-Quelle bleibt ausdrücklich provisorisch, auch bei GPU-Status. Authentifizierter Grid-Live-WASD/Yaw-E2E nach diesem Commit ist ebenfalls offen.

## Validierungsnachweis

Vollständiger Mint-x64-CMake/vcpkg-Debug-Viewer-Build erfolgreich. Native 183/183 CTests bestanden, nach Aktualisierung des alten F3-UI-Vertrags: 490/490 Python-Tests bestanden. scripts/mint-viewer-validation.sh --tests-only EXIT 0 mit echtem isoliertem Xvfb-GUI-Start, 133 Firestorm-Original-Joints, drei GPU-LLM-Körpermeshes (7186 Dreiecke), X11 WM_CLASS und echtem 128x128-Taskleistenicon PASS. Kein neuer Live-Grid-Login, kein aktueller COF/Outfit-/Avatar-Skin-GPU-Pixel-/WASD-E2E-Beweis. Exakte, über GitHub bestätigte Feature-SHA 9f462ef264d30d6a4c71c5cb1bce146089014356 auf develop-current. GitHub zeigt zum Prüfzeitpunkt keine Commit-Status-Einträge und keine zugeordneten PR-Workflow-Runs. OGL Pi5 Runner.Listener und Runner.Worker aktiv; dennoch kein abgeschlossener ARM64-CI-Erfolg dieser exakten SHA belegt: Pi5 ARM64-CI UNVERIFIZIERT. Kein Produktivdeploy. Fortschritt bleibt 38,65% gewichtet / 39% gerundet, nächster Block Alpha.1035–1044.
