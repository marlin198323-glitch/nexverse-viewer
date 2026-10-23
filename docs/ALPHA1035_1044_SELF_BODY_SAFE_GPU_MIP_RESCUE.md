# Alpha.1035–1044 – automatische GPU-sichere Avatar-Bake-Mip-Rettung

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | OGL 0.2.0-alpha.1044

## Zehn echte Entwicklungsblöcke

1035. Linux-Mint-Gerät jam-MS-7D28 über Desktop Commander ausgewählt. GitHub develop-current SHA e9ab467 mit origin abgeglichen, Worktree sauber; zuerst CHATUEBERGABE.md und projectplan.md samt Alpha.1025–1034 gelesen.
1036. OpenGenesisLINK Server-main mit GitHub abgeglichen (f56e354694a41adb2334884c21efc45db784cb92). Kein neuer Nachweis eines Server-Body-Bake-404; keine erfundene Netzursache.
1037. GPU-Residenz, Texture-Mip-Planung, ursprüngliche Firestorm-Self-Pin-Mip0-Policy und GPU-Staging-Bytebudget untersucht. Konkreter Fehlerpfad: Eigene Bakes werden bei zu hohem transienten Full-Mip-Peak verschoben, obwohl bereits dekodierte kleinere Mip-Bilder passen können.
1038. Pure assets::choose_self_avatar_budget_mip_rescue eingeführt. Prüft zuerst das Original gegen gemeinsamen GPU-Staging-Reserve- und Texture-Familien-Bytebudget; wählt nur nach Original-Fehlschlag die erste passende kleinere Stufe bis höchstens Mip3. Ohne Sicherheitsbudget-Bypass.
1039. SelfAvatarMipRescueCooldown ergänzt: 60 Sekunden Qualitätsuntergrenze nach erfolgreichem GPU-Mip-Rettungsupload, maximal 16 kurzlebige Einträge und sofortige Rücksetzung beim Region-/Materialepochwechsel. Danach darf Mip0 erneut probiert werden.
1040. Realer GetTexture-Completion-/Upload-Pfad nutzt nur aktuell eingeloggten, aktiven Firestorm-Regionlease, renderbaren Self-Avatar und dessen *GPU-admitted* Head-/Upper-/Lower-Quelle. GPU-Admission wird GENAU EINMAL pro Frame berechnet und für Render/Planner/Rettung wiederverwendet, damit alle Pfade identische Quellen haben. Anderen Texturen wird kein Sonderpfad eingeräumt.
1041. Falls volle Qualität nicht passt, wird die schon in RAM dekodierte RGBA8 in-place kleiner gerechnet. Der vorhandene GPU-Staging-/Byte-/Slot-/GL-Transaktionsweg bleibt unverändert und muss weiterhin vollständig zustimmen; kein weiterer HTTP-Request oder Retry-Reset.
1042. Qualitätsfloor und texture_desired_mips werden ausschließlich nach echtem erfolgreichem GPU-Upload gesetzt. Unpassende Kandidaten bleiben blockiert. F3 meldet nur die Anzahl nachgewiesener Self-Body-Mip-Rettungsuploads, ohne Asset-/Session-IDs; die Pixel-Endabnahme bleibt offen.
1043. Native Tests für 4096x4096-Textur unter 384MiB Shared-Budget, Ergebnis mip2, Normalfall ohne Qualitätsreduzierung, unwiederbringlichen Druck, max. Stufe und Regionsepoch-/60s-Lebensdauer. Python-Verträge überprüfen echten Worker-/GPU-/Self-Admission-Pfad und fehlendes Doppel-HTTP.
1044. Versions- und Übergabedokumentation, Projektplan, Fortschritt-JSON, Firestorm-/OpenGenesisLINK-/UI-Paritätsmatrizen aktualisiert. Vollständiger Linux-Mint-x64-CMake/vcpkg-Build, 183 native CTests, komplette Python-Tests und Xvfb-GUI/GPU/X11-Icon-Test als Gate. Exakte SHA/ARM64-Pi5-Status nach GitHub-Push. Kein Produktivdeploy.

## Technische Abgrenzung

Der neue Pfad reagiert auf reale, bereits im Client berechnete GPU-Staging-/Texture-Familien-Bytekosten. Er behebt NICHT fehlende gültige Low158-Head-/Upper-/Lower-Server-UUIDs, Server-HTTP404, beschädigte Wearables/COF oder fehlende finale sichtbare Pixel. Es gibt keinen neuen authentifizierten Grid-Live-Nachweis für 3/3 Body-Skin und serverbestätigtes WASD/Yaw. Keine neuen Serveranfragen und keine Bereitstellung.

## Validierung

Gezielter nativer Self-Avatar-Mip-Policy-Test und vier Python-Realpath-Contracts PASS. Vollständiger Linux-Mint-x64-Debug-Viewer und alle 183/183 CTests PASS. Nach Behebung einer älteren GPU-Admission-Quellpfadprüfung, die exakt eine Shared-Admission je Renderframe verlangt, 494/494 Python-Tests PASS (scripts/mint-viewer-validation.sh --tests-only EXIT 0). Isolierter Xvfb-GUI-Start, Original-Firestorm-Skelett 133 Joints, drei Originalkörpermeshes/7186 Dreiecke auf GPU, X11 WM_CLASS und 128x128 Taskleistenicon PASS. Kein neuer authentifizierter Grid-Login im GUI-Smoke. Exakte via GitHub bestätigte Code-SHA a9610722ca5906f4709569cfe51a764a5cdfc52d auf develop-current. GitHub-Commit-Statusabfrage lieferte keine Status-Einträge und die Run-Abfrage keine PR-Workflow-Runs für diese SHA. Raspberry Pi5-OGL Runner.Listener und Worker sind aktiv, dennoch kein abgeschlossener ARM64-CI-Erfolg dieser exakten SHA belegt: Pi5 ARM64-CI UNVERIFIZIERT. Kein Produktivdeploy. Gewichtete Projektbewertung unverändert 38,65 % / gerundet 39 %. Nächste Alpha.1045–1054.
