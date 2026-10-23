# Alpha.1015–1024 – GetTexture-Fehler dem tatsächlich aktuellen Avatar-Bake zuordnen

2026-10-23 | Linux Mint jam-MS-7D28 | develop-current | OGL 0.2.0-alpha.1024

## Zehn Entwicklungsblöcke

1015. GitHub develop-current aus dem sauberen Linux-Mint-Checkout bei 88a0a40 mit origin abgeglichen; zuerst CHATUEBERGABE.md, projectplan.md, AGENTS.md und Alpha.1005–1014 gelesen.
1016. Echte Live-Evidenz vom 21.10.2026 beibehalten: authentifizierter Self-Low158 empfangen, gleichzeitig einzelne GetTexture-HTTP404. Es gibt KEINEN Beleg, dass diese 404 gerade Head/Upper/Lower betrafen; keine erfundene Ursachenanalyse.
1017. GetTexture-HTTP/GPU-Weg in src/app/application.cpp geprüft: gemeinsamer AssetRetrySchedule speichert je Asset FailureKind, aber F3/Report hatten keine Zuordnung zu *aktuellen* Body-Bake-UUIDs.
1018. AssetRetrySchedule um read-only failure_kind(key) ergänzt. Das liest nur die bestehende regionsepochgebundene Retry-Tabelle, setzt keine Sperren zurück und startet keine Downloads.
1019. avatar_bake_recheck.hpp: read-only inspect_self_bake_gettexture pro H/U/L gegen vorhandene GPU-Residency und genau aktuelle IDs; unterscheidet fehlende/Platzhalter-Serverreferenz, GPU vorhanden, neu anforderbar, missing 404/410-Backoff, transient, Konfiguration und invalide Assets.
1020. Selbst wenn eine beliebige fremde Szene-Textur einen HTTP404 lieferte, wird dies nie dem Avatar zugeschrieben, es sei denn dieselbe UUID steht im authentifizierten aktuellen Self-Low158. Epoch-Wechsel und Quellwechsel entfernen alte Attribution automatisch ohne Retry-Reset.
1021. Echter Low158-Application-/Renderer-Pfad nutzt nur die verifizierte AgentID/SessionID/RegionLease und den Read-only-AssetRetrySchedule. Privater Strg+S-Avatar-Diagnosebericht erhält drei numerische Fehlercodes ohne UUID, Session- oder URL-Strings.
1022. Im F3-Bake-Streamstatus steht Body-GetTexture H/U/L mit lesbarem Kurzstatus an erster Stelle (u. a. GPU, HTTP404, retry, auth/cfg, bereit). Der neu ausgegebene GetTexture-Fehlerlog schreibt nicht länger rohe Asset-UUID in die Fehlermeldung.
1023. Native C++-Regressionen: echte per-key 404-Korrelation, fremde Szenen-404, geänderte neue Outfit-ID, GPU, Placeholder, transient/config/invalid, Retry-Ablauf und Regionsepoch-Reset ohne Beeinflussung der Netzwerk-Retry-Logik. Python-Verträge sichern echten App-/F3-/Privatreport-Pfad.
1024. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS/info, Fortschritt-JSON und Firestorm-/OpenGenesisLINK-/UI-Paritätsmatrizen aktualisiert. Vollständiger Mint-x64 CMake/vcpkg/CTests/Python/Xvfb-/Taskleistenicon-Release-Gate vor GitHub-Push; Pi5-CI der exakten SHA danach. Kein Produktivdeploy.

## Aussagegrenzen

Die Attributionslogik ist implementiert und synthetisch getestet. Erst ein neuer authentifizierter NexVerse-Live-Lauf kann zeigen, ob die konkret fehlenden Body-Skin-Assets bei GetTexture 404/410, Auth-/CAP-Konfiguration, Transient, GPU-Staging oder bereits an einer fehlenden Server-Bake-UUID scheitern. Es liegt keine aktuelle vollständige 3/3 Outfit-Skin-RGB-GPU-Pixelabnahme vor. Die bisherige historische Serveranimation bleibt ein separat gültiger Befund. WASD-/Yaw-E2E für diesen Commit muss ebenfalls neu bestätigt werden.

## Validierung und GitHub

Vollständiger Mint-x64-CMake/vcpkg-Debug-Viewer-Build durchgeführt. Nach Anpassung des bestehenden assessment-Textvertrags: scripts/mint-viewer-validation.sh --tests-only EXIT 0, 183/183 native CTests, 485/485 Python, isolierter Xvfb-GUI-Start mit Original-Firestorm-133-Joint-Skelett, drei GPU-Body-LLM (7186 Dreiecke) und echten 128×128-X11-Taskleistenicon/WmClass PASS. Initialer kompletter Build (C++) PASS und native Tests PASS; erste Python-Gesamtsuite hatte ausschließlich 25 ältere Metadaten-Vertragsfehler wegen Großschreibung von still open, vor Gate-0 behoben. Kein authentifizierter neuer Grid-Login im Xvfb-Test. GitHub-Code-SHA/Pi5-Status nach Push ergänzen. Projektfortschritt weiter 38,65 % gewichtet / 39 % gerundet. Nächste Alpha.1025–1034.
