**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.574, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.575–584.**

**ALPHA.574 SHA-GENAUE Pi5 ARM64 CI VERIFIZIERT:** GitHub Actions Run 39464572356 = COMPLETED SUCCESS, self-hosted Pi5 ARM64-Headless-Job 118772472994 = COMPLETED SUCCESS, für EXAKTEN Feature-SHA 2701cf5d0307373529901ee300d13f1a3eb3b0f2. Mint x64 vollständiger Build 381/381, native C++/OpenGL 162/162, Python 281/281, Xvfb GUI-Start, lokale Headless und X11 WM_CLASS/Systemicon PASS. Pi5 läuft ARM64 C++23 Headless/Python, nicht GPU/GUI und nicht authentifizierter NexVerse-Live-Login. Neu: CURRENT Simulator ObjectUpdate.State/ParentID erhält für tatsächlich empfangene Root-Prims Vorrang vor älterem Low158-Anhangblock, wodurch stale HUD-/Bone-Punkte, State=0 Detach und Avatarwechsel korrekt getrennt werden; nicht erhaltene Low158-Items bleiben PENDING und sind keine COF-/GPU-Erfolgsbeweise. Native und Firestorm Original-OpenGL-Framebuffer-Tests, F3 und private count-only Diagnostik PASS. Authentifizierter NexVerse Skin/COF/Outfit/Attachments/WASD E2E weiter OFFEN; 39 % (38,65 %). Nächste zehn Blöcke Alpha.575–584, KEIN Produktionsdeploy.

# Alpha.565–574 – aktuelle Simulator-Anhänge statt veralteter Low158-Bindings

20.10.2026 | develop-current | 0.2.0-alpha.574 | Gesamtfortschritt 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke

565. GitHub develop-current HEAD 5df8c115, CHATUEBERGABE.md, projectplan.md und Progress geprüft; Alpha.564 Pi5-ARM64-CI Run 39460543881 erfolgreich SHA-genau. OpenGenesisLINK Server main SHA 95c2133 unverändert.
566. Avatar-/Attachment-GPU-Quellen untersucht: Viewer fügte bisher erst Low158-Anhangspunkte ein und konnte neue aktuelle Objekt-Anhangpunkte wegen std::map::emplace nicht überschreiben; Low158 konnte damit frühere oder falsche Skelettpunkte/alte HUD-Einteilungen im aktuellen Frame beibehalten.
567. Read-only AvatarAttachmentSelection/reconcile_avatar_attachments implementiert: ein echtes aktuelles Simulator-Prim mit identischer UUID und bekanntem Avatar-Parent/State gewinnt vor älterem Low158. Nur Anhangpunkte 1..55, Prim-PCode, bekannte LocalID und aktueller Parent sind zulässig.
568. Detach, Avatarwechsel und Umbindung berücksichtigt: Prim mit aktuellem State=0, ParentID=0/anderer Avatar oder ungültigem Slot darf nicht durch alten Low158 auf einen Firestorm-Bone gesetzt werden. Region/AvatarLocalID=0 liefert keine fiktive Bindung.
569. Noch nicht geladene Low158-IDs bleiben nur als ausstehende Zuordnung vorgemerkt; sie werden nicht als sichtbares, GPU-residentes oder COF-bestätigtes getragenes Objekt ausgegeben. Identische GUIDs werden nicht mehrfach eingebunden.
570. Den identischen Reconciler in den produktiven Self- und Remote-RenderAvatarProxy-Pfad eingebaut. Alte getrennte Low158-First/Obj-Emplace-Pfade vollständig ersetzt. Firestorm-Skelett-/HUD-Renderer behält seinen bestehenden gerenderten Weltzustand.
571. Native Tests: veraltete HUD->Schulter- und Schulter->HUD-Punkte, unbekannte pending Low158, echte Welt-/Fremdobjekte, State=0 nach Detach, Umhängen auf HUD und Wechsel des Avatar-Parents. Kein Rebind auf stale Low158.
572. Echter isolierter OpenGL-Framebuffer-Test: Das originale Firestorm-Schulter-Gelenk stellt den tatsächlichen Current-Object-Anhang dar, obwohl alte Low158-Daten fälschlich HUD-Slot 31 nennen.
573. F3 zeigt zusätzlich Zuordnung/alte Low158-Anhänge; privater Avatarbericht dokumentiert nur Anzahl current/pending/stale/effective, keine privaten Objekt-, Inventar- oder Konten-UUIDs. Neue Python-Source-Verträge und UI-/Report-Tests.
574. Vollständiger Mint x64 Build/OpenGL Native CTest/Python, Pi5-kompatibles Headless, isoliertes Xvfb-GUI und X11-Systemicon; nach GitHub Feature-Push SHA-genaue Pi5 ARM64-CI sowie Handoff/Projektplan/Progress auf Alpha.575–584 aktualisieren.

## Grenzen

Das ist eine echte Prioritäts- und Lebenszeitkorrektur für serverseitig vorhandene Attachments. Der Viewer erfindet damit keine fehlenden Skin-/Wearables-/COF-Daten, lädt nicht automatisch nicht bereitgestellte Mesh- oder Kleidungstexturen und hat noch keine neue authentifizierte NexVerse-End-to-End-Sitzung mit komplettem Avatar, getragener Kleidung und WASD-Bewegung gesehen. Deshalb Fortschritt 39 %, keine produktive Bereitstellung.

Nächste 10 Blöcke Alpha.575–584: frische echte Grid-Login-/Low158-/GetTexture-/COF- und GPU-Logs priorisieren, Avatar und Welt prüfen und parallel fehlende Firestorm-UI-Funktionen integrieren.
