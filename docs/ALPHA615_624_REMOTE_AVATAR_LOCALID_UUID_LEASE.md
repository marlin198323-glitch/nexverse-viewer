# Alpha.615–624 – gültige Avatar-UUID besitzt die neue Simulator-LocalID

21.10.2026 | develop-current | 0.2.0-alpha.624 | Gesamtprojekt 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke und Testaufträge

615. GitHub develop-current bei HEAD c350aee4752ff3b0b357dd6c10476663aaf39def abgeglichen, CHATUEBERGABE.md, projectplan.md, VERSION und Fortschritts-JSON gelesen; bestätigte Alpha.614 Pi5 ARM64 CI 39628852352 für Feature-SHA 8ba3f4a kontrolliert.
616. Aktuellen OpenGenesisLINK-Server-main 171e814ee0f454af0118c54320700c6336af4141 über git ls-remote geprüft. Keine neue authentifizierte NexVerse-Sitzung in den lokal vorhandenen Viewer-Logs vom 21.10.; Logs stammen aus isolierten GUI-Tests.
617. ECHTE Lücke in WorldModel und Full/Compressed-Avatar-Objektpaketpfaden: Simulator LocalIDs werden nach Entfernung/Teleport möglicherweise wiederverwendet. Ist das ältere UDP KillObject verloren, wurde beim Full ObjectUpdate der neuen UUID mit gleicher LocalID einfach ein zweiter 3D-Fremdavatar angelegt; alte Avatar-Mesh-Anhänge blieben im Rig/Renderer.
618. Der tatsächliche Remote Avatar ObjectUpdate-Transformpfad überprüft jetzt vor Bindung einer neuen LocalID, ob eine andere bereits bestätigte Avatar-UUID diese LocalID im aktuellen Regionslease besitzt. Bei Kollision wird der alte Avatar samt Root-/Kind-Linkset-Prims und ausstehender Animation aus genau diesem LocalID-Baum entfernt.
619. Die alte UUID wird auch dann nicht mit einer neuen lokalen Anhang-ParentID gezeichnet, wenn der gleiche bestätigte Avatar eine NEUE LocalID meldet. Das Original-AvatarAppearance/Low158-TextureEntry, VisualParams und Attachment-Metadaten bleiben für die UNVERÄNDERTE UUID erhalten; nur die an der alten LocalID hängenden Prims müssen mit echten neuen Serverpaketen bestätigt werden.
620. Der Lokal-ID-Wechsel ist keine empfangene KillObject-Nachricht. WorldModel::kill_simulator_local_id erhält dafür eine interne No-KillObject-Zähloption, und die neue reine revisionsgebundene rebind-Zahl steigt unabhängig von den tatsächlichen UDP-KillObject-Zählern. Doppelte/identische Full ObjectUpdates produzieren keine neuen Rebinds.
621. Neue native C++-Regression für Alice->Bob LocalID=51 mit altem Mesh-Root/Child, unverändertem Charlie LocalID=53, danach Bob 51->54 mit erhaltener tatsächlicher Low158-Metadaten und verworfenen alten Root-Kind-Anhängen. Coarse-Kartenmeldung über alte Alice bleibt rein auf Karte ohne 3D-Avatar; echter späterer KillObject zählt separat.
622. Validierung: Full/Compressed-Avatarpakete mit LocalID 0 oder getrennter Region autorisieren KEINEN 3D-Remote-Avatar. Bestehende reine Coarse-Map-Daten können den echten Simulator-Objektbesitz nicht vorgeben.
623. Original Firestorm-Joint-OpenGL-Framebuffer-Test reproduziert sichtbares Avatar-Anhangobjekt, neues tatsächliches WorldModel-ObjectUpdate einer anderen UUID bei selber LocalID und anschließend NULL alte GPU-Anhangpixel. F7 unterscheidet jetzt tatsächliche KillObject Avatar/Prim von erkannten impliziten UUID-/LocalID-Wiederbindungen.
624. Vollständiger Linux Mint x64 Build, C++/OpenGL CTest, Python-Quell- und Vertragstests, Pi5-kompatible Headless-Suite, isolierter Xvfb-GUI-Start und X11-Systemicon; GitHub Featurecommit und exakte Pi5-ARM64-CI; abschließend Übergabe/Projektplan/JSON fortschreiben auf Alpha.625–634.

## Grenzen und offene End-to-End-Punkte

LocalID-Eigentum kann ohne Simulator-Sequenznummer keine absichtlich verzögerten Alt-Pakete von neueren unterscheiden; der jeweils empfangene autoritative volle ObjectUpdate bestimmt die aktuelle Zuordnung. Wir behaupten keinen neuen echten Grid-Login und keinen nachgewiesenen vollständig bekleideten eigenen Avatar. Echt aktuelle Skin, COF-Outfit-/Wearable-Assets, Mesh-Uploads, GetTexture/JPEG2000 und simulatorbestätigte WASD müssen weiterhin in einer frisch authentifizierten NexVerse-Sitzung bewiesen werden. Weiterhin 39 % Projektstand, kein Produktivdeploy.

Nächste zehn Entwicklungsblöcke Alpha.625–634: eigene Avatar-/Skin-/COF-Pipeline und gleichzeitig Firestorm-Fenster, Navigation und sichtbare Benutzeraktionen weiter integrieren, echte Logs vorziehen.
