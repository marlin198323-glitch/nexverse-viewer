# Alpha.655–664 – Prim-UUID-Umzug, gültige Szene und F7-Diagnose

21.10.2026 | develop-current | OGL 0.2.0-alpha.664 | Projekt 39 % (38,65 % gewichtet).

## Zehn konkrete Entwicklungsblöcke

655. Handoff zuerst gelesen, lokalen Pi5-Checkout SHA e37c4d1 mit develop-current verglichen. Alpha.654 exakter Pi5 ARM64 CI Run 39639482493 gegen Feature-SHA 3fdfa60 **SUCCESS** bestätigt. OpenGenesisLINK Server main 0129e617 geprüft; 243 Commits gegenüber früherem reviewed-SHA sind immer noch nicht einzeln manuell freigegeben.
656. Echten weiteren Ghost-Mesh-Fall im WorldModel::upsert_object untersucht: verlorenes KillObject und gleiche gültige Prim-UUID auf einer ANDEREN Simulator-LocalID. Alter Root und verwaiste Mesh-Kinder konnten zusätzlich sichtbar bleiben.
657. Neues WorldModel-UUID-Umzugs-Handling verwirft die alte Root-/Child-/Grandchild-Kette vor Einsetzen desselben realen Prims an seiner neuen LocalID. Der neue UUID-Index erlaubt schnelle Suche ohne vollständigen Scene-Map-Scan pro Stream-Paket und wird bei Kill/Remove/Reset sauber mitgeführt. Anderer UUID-Wert auf derselben LocalID bleibt separat behandelter bisheriger Rebind-Fall.
658. Validierung im echten Prim-Upsert VOR einer Kollision/World-Eviction: Nicht-finite oder unsinnig große Positionsdaten, Rotation und Skalierung dürfen einen gesunden Root-/Mesh-Unterbaum nicht löschen. Leer-UUID, ID 0 und Disconnected-Circuit bleiben ohne Render-Aufnahme.
659. WorldModel weist fehlerhafte vollständige Avatar-ObjectUpdate-Posen vor möglichen Avatar-/Attachment-LocalID-Umbindungen zurück. Coarse-Positionen bleiben Kartendaten, nicht als GPU-Avatar-Abnahme ausgegeben.
660. In den echten LLUDP-Handlern für Full-/Compressed-Own-Avatar und High9 Own/Remote werden Positionen und Quaternionen VOR dem Übernehmen als AgentPosition, Kamera oder WASD-/Yaw-Readback auf Gültigkeit geprüft. Nicht valide Quelle begründet keinen neuen erfolgreichen Avatar- oder Motion-Nachweis.
661. Neue eigene, saturierende Zähler für Prim-UUID-Umzug und unbrauchbare WorldModel-Szenenupdates, Reset über WorldModel::reset, Session und Region; F7 enthält getrennte sichtbare Zeilen für reale KillObject-Zahlen, LocalID-Rebinds, UUID-Umzug und zurückgewiesene Szenendaten, ohne private UUIDs.
662. Native C++23-Pi5-Regressionsprogramme für echte WorldModel-Quellen: identische UUID mit neuer LocalID, alter mehrstufiger Mesh-Unterbaum, fremde Objekte bleiben, korrupter Ersatz beschädigt keine aktuelle Szene, Remote-Avatar-Kollision, High9/Self-Evidenz-Gültigkeit, Regionsreset.
663. Neue Python-Quellcode-/UI-Vertragstests, CMake-Testregistrierung und Pi5-Headless-Skript. Der ARM64-Runner kompiliert WorldModel-/LLUDP-/TextureEntry-Quellen einmal und verknüpft sechs unabhängige native WorldModel-Regressionen. Ältere UI-/Identitäts-Vertragstests an tatsächlich strengere Checks statt exakter Textmuster angepasst.
664. Version, Projektplan, Handoff, Firestorm-/OpenGenesisLINK-Kompatibilität und Statusdaten dokumentiert; HEAD/Test-Ergebnisse und GitHub-CI folgen getrennt. Kein Produktivdeploy.

## Evidenzgrenzen und offene Aufgaben

Diese Verbesserungen verhindern bestimmte alte Meshes und fehlerhafte Weltposes. Sie BEWEISEN NICHT, dass ein angekleideter Avatar, Outfits/COF, Texturen und WASD im authentifizierten NexVerse-Grid inzwischen sichtbar funktionieren. Die lokalen ARM64-C++23-/Python-Headless-Tests schließen keinen vollständigen Linux-x64-GPU/CMake-Viewer-Build, keinen Xvfb-Start und keine echte Simulator-Anmeldung ein. Eine SHA-genaue GitHub-Pi5-CI für Alpha.664 darf erst als grün eingetragen werden, wenn der GitHub-Workflow für den Code-SHA abgeschlossen ist.

OpenGenesisLINK main gegen reviewed-Baseline (0129e617 / 4e999766) meldet 243 neue Commits und über 300 geänderte Dateien (truncated): Caps-/Server-/Permissions-Diffs weiter MANUAL_REVIEW. Reviewed SHA bleibt unverändert. Firestorm-Parität und Grid-E2E sind offen. Der aus den Gewichten berechnete Fortschritt bleibt 38,65 % = 39 % angezeigt.

Nächste **zehn** echte Entwicklungsblöcke Alpha.665–674: einen authentifizierten NexVerse-Grid/Avatar/F3/F7/COF/WASD/Skin-Screenshot priorisieren, beobachtete Bugs beheben, UI-Firestorm-Parität parallel fortsetzen. Kein automatisches Produktivdeploy.
