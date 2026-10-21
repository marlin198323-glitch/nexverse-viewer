**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.614, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.615–624.**

**ALPHA.614 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39628852352 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 Job 119321248719 = COMPLETED SUCCESS für exakten Feature-SHA 8ba3f4a89b733a32a7a230d260572dc78f87aecc. Mint x64 Vollbuild 387/387, native C++/OpenGL 165/165, Python 305/305, isolierter Xvfb-GUI-Viewer, lokale ARM64-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5 testete ARM64-native C++23 Headless plus Python, KEINE Pi5-GPU/GUI und KEINEN authentifizierten NexVerse-Live-Login. Tatsächlicher LLUDP-KillObject wird nun auch auf bekannte Fremdavatar-LocalIDs und angehängte Root-/Child-/Grandchild-Prims angewendet. Unabhängige Welt/Avatar-Objekte bleiben; Map-only Coarse kann entfernten 3D-Avatar nicht durch alten Zustand resurrecten. Nativer echter KillObject Wire-Decoder und ursprünglicher Firestorm-OpenGL-Framebuffer belegen Remote-Kill ohne alten Avatar-/Anhangpixel. F7 zählt echte regionsgebundene Avatar-/Prim-Kills ohne UUID. Aktuelle Live-NexVerse-Skin, COF/Outfit und serverbestätigte WASD End-to-End weiterhin OFFEN; Fortschritt 39 %. Nächste zehn Blöcke Alpha.615–624, kein Produktionsdeploy.

# Alpha.605–614 – echte KillObject-Avatar-/Mesh-Beseitigung

21.10.2026 | develop-current | 0.2.0-alpha.614 | Fortschritt 39 % (38,65 % gewichtet)

## Zehn Entwicklungs- und Integrationsblöcke

605. GitHub develop-current SHA 1ed7737ead651d4e6ce2238f346b8428aca70ab7, letzte Übergabe, projectplan.md, Version/JSON und erfolgreiche Alpha.604-Pi5-ARM64-CI 39626151169 geprüft. Keine neue authentifizierte NexVerse-Sitzung: lokale Logs vom 21.10. enthalten Xvfb- und Quelltestläufe.
606. Aktuellen OpenGenesisLINK Server-main e215ea2a3a17141ea205fa562a7898217de70383 kontrolliert; Avatar-/KillObject-Kompatibilität mit realem vorhandenen Viewer-LLUDP-Decoder abgeglichen.
607. REALEN Viewer-Bug erkannt: Der produktive LLUDP-KillObject-Handler rief lediglich WorldModel::remove_object(LocalID) auf, obwohl Avatar-Objekte in WorldModel::avatars() liegen. Weil nach Alpha.604 die Coarse-Karte zu Recht keine vollständigen Avatare mehr löscht, konnten entfernte fremde Avatare und ihre Mesh-Anhänge dauerhaft als 3D-Geister bestehen bleiben.
608. WorldModel::kill_simulator_local_id implementiert. Bei gültiger aktueller Region prüft es die tatsächliche Simulator-LocalID, entfernt ein betroffenes Fremdavatar samt pending Animation und lässt alle anderen Avatar-UUIDs unberührt.
609. Dieselbe KillObject-Verarbeitung löscht echte Root-Prims sowie alle von ihnen abstammenden Child-/Linkset-Prims. Ein Avatar-Kill mit eigener LocalID entfernt damit auch seine echten angehängten Mesh-/Attachment-Roots und Unterobjekte, ohne fremde Avatar- oder Weltobjekte zu löschen.
610. Verhindert Endlosschleifen bei defekten Parent-Zyklen durch visited-Set; kein Avatar-Kill bei LocalID 0, getrenntem Simulator oder unbekannter/repetierter LocalID. Ein erfolgreicher Linkset-Kill verursacht nur eine gemeinsame Render-Object-Revision. Die Reconciliation kann dadurch veraltete Attachment-Prims nicht erneut am alten Firestorm-Knochen finden.
611. Nativer C++-Test synthetisiert echtes Low-KillObject-LLUDP-Wire-Paket, führt es über bestehenden Decoder ins aktuelle WorldModel und prüft entfernten Remote-Avatar, Root/Child/Grandchild, unabhängiges Fremdavatar und Weltobjekte, doppelte KillObject-Pakete, reines Coarse-Wiederauftauchen ohne 3D-Beweis, erneuten Regionwechsel.
612. Echte OpenGL-Framebuffer-Regressionsprüfung: zunächst im Firestorm-Joint-Renderer sichtbaren animierten Attachment-Prim erzeugen; nach dem echten WorldModel-Kill des Avatar-LocalID keine Mesh-/Avatar-Pixel mehr, kein alter Body-Proxy und keine Reaktivierung des KillObject-Rootprims.
613. F7-Regionsfenster zeigt aus WorldModel nur Zähler für tatsächlich entfernte Remote-Avatare und Prims dieser Region. Die Zähler werden beim Region-/Session-/Agentwechsel zurückgesetzt; weder private AgentUUIDs noch Asset-/Outfit-IDs im F7 angezeigt. Native UI-/OpenGL- und Python-Tests sichern diese Angabe und den Dispatcherpfad.
614. Gesamte Mint-x64-Build- und native OpenGL-CTest-/Python-Regression, Pi5-kompatibler C++23-Headless-Lauf, isolierter Xvfb-GUI-Start und X11-Icon. Code nach GitHub pushen, SHA-genaue Pi5-ARM64-CI abwarten und neuen Fortschrittsstand sowie Projektplan/Handoff auf Alpha.615–624 setzen.

## Unverifizierter Live-Zustand

Dies ist eine echte Korrektur des Server-Avatar-Lebenszyklus und der GPU-Folgen beim KillObject. Es beweist NICHT, dass ein vollständiger eigener Avatar mit Skin, COF-Kleidung und Texturen in der laufenden NexVerse-Sitzung sichtbar ist, oder dass dort WASD serverseitig bestätigt wurde. Dafür fehlen weiterhin neue authentifizierte Laufdaten und tatsächliche eigene Skin-/COF-Beweise. Fortschritt deshalb weiterhin 39 %, kein Produktivdeploy.

Nächste zehn Blöcke Alpha.615–624: tatsächliche Low158-/COF-/GetTexture-/Mesh- und WASD-Livedaten nach Login prüfen und die fehlenden Firestorm-UI-Funktionen parallel entwickeln.
