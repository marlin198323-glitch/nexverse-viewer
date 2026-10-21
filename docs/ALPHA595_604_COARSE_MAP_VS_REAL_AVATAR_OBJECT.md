# Alpha.595–604 – Coarse Map-Positionen sind kein echter 3D-Avatar

21.10.2026 | develop-current | Viewer 0.2.0-alpha.604 | Projektfortschritt 39 % (38,65 % gewichtet)

## Zehn reale Entwicklungsblöcke

595. Entwicklungsbranch develop-current am sauberen HEAD 0673ecbe geprüft, CHATUEBERGABE.md und Projektplan gelesen; vorherige Alpha.594 Pi5-ARM64-CI 39488686178 war in der Übergabe als SUCCESS dokumentiert. GitHub-API meldete vorübergehend 404 bei der erneuten Run-Abfrage; lesender Git-SSH-Zugriff war intakt.
596. Aktuellen OpenGenesisLINK Server-main SHA e215ea2a3a17141ea205fa562a7898217de70383 per git ls-remote geprüft. Die vorhandenen neuen lokalen Logs stammen aus isoliertem Xvfb-Test vom 20.10., nicht aus neuem authentifiziertem NexVerse-Login.
597. Reproduzierter Viewer-Quellfehler: CoarseLocationUpdate mit quantisierten Positionsbytes wurde bisher als vollständiger Avatar mit position_known gespeichert, woraufhin die 3D-Render-Schleife bereits einen Körper/Avatar-Proxy zeichnete, obwohl kein ObjectUpdate dieses Avatars eingetroffen war.
598. AvatarState merkt ausdrücklich, ob ein echter Full- oder Compressed-Avatar-ObjectUpdate empfangen wurde. Nur diese beiden objektidentitätsbestätigten Pfade setzen simulator_object_update_seen=true. AvatarAnimation, Low158 allein, CoarseLocation und Terse dürfen eine fehlende 3D-Objektquelle nicht als vollständig melden.
599. Neuer WorldModel-Coarse-Synchronisierungspfad verwendet echte Dekodierung, ignoriert Self-, Nil-, doppelte, nichtfinite und überzählige Map-Einträge, behandelt Coarse-Positionen nur als Kartenquelle und überschreibt NIE genaue bekannte ObjectUpdate-Positionen durch grobe Bytekoordinaten.
600. Coarse-Snapshots dürfen detaillierte Avatare auch dann nicht löschen, wenn sie dort nicht aufgeführt sind. Nicht mehr aufgeführte reine Coarse-Punkte können hingegen entfernt werden, sofern sie keine echten Low158-/Appearance-Metadaten erhalten haben. Vollständige Skin/COF/Wearable/Animation bleibt erhalten.
601. Der produktive Renderer nimmt andere Avatare erst dann als 3D-RenderAvatarProxy auf, wenn deren echte ObjectUpdate-Herkunft und Position bestätigt sind. Kartenpositionen allein erzeugen keine unsichtbar als Erfolg bezeichnete Firestorm-Körpergeometrie.
602. Nativer C++-Test dekodiert ein echtes Low CoarseLocationUpdate-On-Wire-Paket mit 16-Byte-AgentUUID und quantisierter Z-Höhe, leitet es in WorldModel und prüft Map-only, danach echtes Remote-ObjectUpdate, Detail-Pose-Vorrang, Low158-/COF-Metadaten, Avatar-Duplikat/Pruning, neues Regionlease und korrupten Datagramm-Fall.
603. F7-Regionsfenster unterscheidet direkt aus WorldModel die Anzahl eingetroffener Avatar-ObjectUpdates von reinen Coarse-Kartenpositionen. Damit sind ausstehende Avatar-Objekte nachvollziehbar und keine GPU-/Skin-Beweise vorgetäuscht. Native C++-/OpenGL-UI- und Python-Verträge sichern die verdrahteten Felder.
604. Linux-Mint-x64 vollständiger CMake-Build, native C++/OpenGL CTest, alle Python-Verträge, isolierter Xvfb-Viewer, ARM64-kompatibler Headless-Lauf, X11 WM_CLASS/Systemicon. Nach Source-Push genaue Pi5-ARM64-CI abwarten und Erfolg mit geprüftem SHA in Projektplan/Übergabe dokumentieren. Nächstes Paket Alpha.605–614.

## Grenzen

Der Fix verhindert Avatar-Phantome und das Überschreiben präziser Simulatorposen, aber liefert keine vom Grid fehlenden AvatarAppearance-, Skin-, COF- oder Mesh-Daten. Vollständig sichtbar getragener eigener Avatar und WASD mit bestätigter realer Simulatorantwort müssen in einer frischen authentifizierten NexVerse-Sitzung geprüft werden. Projektprozent daher weiterhin 39 %. Kein Produktivdeploy.

Nächste zehn Blöcke Alpha.605–614: echte eingeloggte Avatar-/COF-/Low158-/GetTexture-Logs priorisieren, fehlende Avatar-GPU-Darstellung und Firestorm-Bedienelemente weiter integrieren.
