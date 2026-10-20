**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.594, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.595–604.**

**ALPHA.594 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39488686178 = COMPLETED SUCCESS, Pi5-ARM64-Headless Job 118855705576 = SUCCESS, exakt für Feature-SHA c6ecd3fbd05218476ef1ed5e755cc5188378c1b6. Linux Mint x64 Vollbuild 383/383, native C++/OpenGL 163/163, Python 293/293, isolierter Xvfb GUI-Start, lokale Pi5-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5-CI prüft native ARM64 C++23 Headless/Python, KEINE Pi5-GPU/GUI oder authentifizierten NexVerse-Live-Login. Direct RegionHandle-Wechsel, Circuit-Disconnect und neue authentifizierte Agent-/Session-Identität löschen inzwischen alte Prims/Attachments, Fremd-Avatare, Terrain, Parcel, Environment mit Revision und neuer Szene-Epoch. Gleichgebliebene verbundene RegionHandshake/MovementComplete-Pakete bewahren Welt. F7 zeigt echte Szene-Epoch. Frische authentifizierte NexVerse Skin/COF/Outfit/Attachments/WASD-Grid-Abnahme NICHT vorhanden; Status 39 %. Nächste zehn Blöcke Alpha.595–604. Kein Produktivdeploy.

# Alpha.585–594 – alte Avatare/Anhänge bei Regionswechsel entfernen

20.10.2026 | develop-current | 0.2.0-alpha.594 | Projekt 39 % (38,65 % gewichtet)

## Zehn reale Entwicklungs- und Prüfblöcke

585. GitHub develop-current HEAD d0f558ed, CHATUEBERGABE.md, projektplan.md, Fortschritt und erfolgreiche exakte Alpha.584 Pi5 ARM64-CI Run 39467519712 geprüft.
586. Aktuellen OpenGenesisLINK Server-main SHA 3f499963f01de70ab330e8233972afd820b13ea6 geprüft; Änderungen seit 95c2133 betreffen HG-/Portal-Instant-Messaging und nicht das LLUDP Avatar-Protokoll.
587. WorldModel.apply_region Lebensdauerfehler reproduziert: Ein direkter Regionhandle-Wechsel oder Circuit-Disconnect entfernte die eigene Skin, aber ließ alte Weltobjekte, fremde Avatare, Terrain, Parzellen und Environment unangetastet.
588. Bei echtem Regionhandle-Wechsel oder verbunden->getrennt alle regionalen Objekte, Avatar-/Anhang-Root-Prims, Terrain, Parcel, Environment und self appearance/wearables/animation entfernen; agent.present=false, Objekt-Revision und Regiongeneration erhöhen.
589. Normale gleichbleibende verbundene RegionHandshake-/AgentMovementComplete-Updates behalten Szene, vorhandene Texturen und Revision. Wiederholte Disconnect-Polls erhöhen weder Regiongeneration noch Renderrevision erneut.
590. Wiederverbinden darf vorherige Objekte und Bindings nicht stillschweigend wiederherstellen: Für neue Weltobjekte und Avatare müssen echte aktuelle Simulatorpakete eintreffen. Direkter Regionswechsel invalidiert ohne explizites begin_region_transition.
591. Eine neue Anmeldung derselben AgentID unter anderer SessionID sowie Anmeldung einer anderen AgentID invalidiert zusätzlich Regionobjekte, Avatare, Terrain/Parcel/Environment und lease generation; reine Agent-Positionsupdates mit gleichen Credentials behalten gültige Szene.
592. Native vollständige WorldModel-Lebensdauertests enthalten echten Mesh-Prim-State/ParentID, Fremd-Avatar, Terrainpatch, Parcel, Environment und geprüftes Dis-/Reconnect sowie Regions- und Sessionwechsel. Keine alte Anhangszuordnung lebt nach einem Wechsel weiter.
593. F7-Regionsfenster zeigt die tatsächlich aus WorldModel stammende Szene-Epoch zusätzlich zum tatsächlichen verbunden/getrennt Zustand. C++ UI-/OpenGL-Test und Python-Vertrag sichern diesen sichtbaren Datenpfad.
594. Vollständige Mint x64 Build-/native OpenGL-CTest-/Python-Regression und Pi5-kompatible Headless-, Xvfb GUI-/X11 Icon-Checks. GitHub Featurecommit, SHA-genaue ARM64-Pi5-CI, danach Übergabe- und Projektplanupdates auf Alpha.595–604.

## Grenzen

Der Fix verhindert, dass veraltete Avatar- und Objektdaten beim echten Wechsel im Viewer verbleiben. Er erzeugt keine fehlenden echten Serverpakete. Eine neue authentifizierte NexVerse-Sitzung mit vollständig sichtbarer eigener Skin/Kleidung, Mesh/HUD-Anhängen und simulatorbestätigter WASD-Bewegung fehlt noch als End-to-End-Beweis. Projektstatus 39 %, kein Produktivdeploy.

Nächste zehn Blöcke Alpha.595–604: echten Grid-Login und aktuelle F3/F7 Logs, Low158/GetTexture/COF/GPU prüfen und Firestorm-UI parallel entwickeln.
