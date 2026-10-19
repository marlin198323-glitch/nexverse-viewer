# Alpha.525–534 – korrekte OpenGenesisLINK-Drehrichtung und echte Simulator-Pose-Evidenz

19.10.2026 | develop-current | Version 0.2.0-alpha.534 | Projekt 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke

525. GitHub Branch develop-current HEAD 4421f8a, Übergabe, projectplan.md, Projektfortschritt und letzte SHA-genaue Pi5 ARM64 CI 39299889677 SUCCESS geprüft.
526. Tatsächlichen aktuellen OpenGenesisLINK Server-main SHA 6e446b9 (neuer API-Key-Commit, World/ScenePresence.cs ohne Änderung) gelesen: ScenePresence.cs Zeilen 2763/2764 verwendet TURN_LEFT + YAW_POS und TURN_RIGHT + YAW_NEG; Scripthandling Zeilen 5935 ff bestätigt YAW_NEG -> ROT_RIGHT und YAW_POS -> ROT_LEFT.
527. REALEN Navigationsfehler korrigiert: alter Viewer sendete TURN_LEFT + YAW_NEG und TURN_RIGHT + YAW_POS, entgegengesetzt zur eigenen Quaternion und Server-Autorität. Nun beide Paare korrekt zugeordnet, einschließlich Flugsteuerung.
528. Native C++-Verifikation dekodiert echte AgentUpdate High4 LLUDP-Pakete inklusive 114 Byte langen Firestorm-Message-Template-Payloads und 32-Bit ControlFlags ab Byte 109. Beide Richtungen und Fly-Flags exakt geprüft.
529. Gleichzeitig gedrückte Links/Rechts-Drehtasten und Gegenrichtungen W/S beziehungsweise A/D erzeugen nicht mehr widersprüchliche Flags; gültiges diagonales W+A erhält weiterhin beide Bewegungsbits. Gewünschte Bewegungen bleiben serverautoritativ.
530. TurnReadbackMonitor ergänzt: Threadsafe nur tatsächlich versandte Dreh-Socketkontrollen verfolgen, nie Eingabe, lokale Quaternion oder enqueue als Simulatorbestätigung ausgeben.
531. Rotationsbeleg ausschließlich aus eigenen gültigen, zum aktuellen Simulator gehörenden ObjectUpdate-, ObjectUpdateCompressed- und ImprovedTerseObjectUpdate-Paketen. Mindestens 5 Grad in gesendeter Drehrichtung nach tatsächlichem UDP-Senden erforderlich; inkonsistente, alte, fehlerhafte oder gegensätzliche Rotationswerte bestätigen nichts.
532. Integrationspfad in aktuellem Viewer inklusive Flight und Sitzungsübergang: Logmeldungen werden nur nach bestätigter echter Self-Rotation erzeugt, nie bei Eingaben; Monitor auf Login/Logout/Regionwechsel neu initialisiert. Im vorhandenen F3-Avatar-Bewegungsstatus wird während der Drehung ausstehend oder tatsächlich bestätigt angezeigt.
533. Native Regression für beide Drehrichtungen, LLUDP-Bytefelder, gedrückte Gegentasten, Flugrotation, serverseitige Pose nach UDP, falsche Gegenrichtung, ungültige Quaternion, Out-of-order-Zeit, fehlende Baseline und Reset. Python-Verträge sichern Codeverdrahtung und Dokumentation.
534. Linux-x64 Vollbuild und alle C++-/OpenGL-Tests, Python, ARM64-kompatible Headless, Xvfb GUI-Start und X11 Taskleistenicon durchführen, auf GitHub develop-current committen und self-hosted Pi5 ARM64 CI für genauen Source-SHA verifizieren. Handoff auf Alpha.535–544.

## Wesentliche Grenze

Korrekte Flags und echte Pose-Rückmeldung verbessern Steuerung und Diagnose. Ohne neue authentifizierte, tatsächliche Grid-Sitzung ist eine durchgängige Steuerung W/A/S/D, volle sichtbare Welt und komplett getragene eigene Avatar-Skin/Kleidung und Anhänge weiterhin NICHT abgenommen. Kein künstliches Bewegen des WorldModels und keine erfundene Serverantwort. Daher gewichteter Fortschritt 38,65 %, gerundet 39 %. Keine produktive Bereitstellung.

**Nächste zehn Blöcke Alpha.535–544:** echte aktualisierte Grid-Logs und eigene tatsächliche Navigation/Skin/COF im aktuellen Viewer prüfen; zusätzliche Firestorm-Funktions- und UI-Integrationen mit realem Serverbeleg.
