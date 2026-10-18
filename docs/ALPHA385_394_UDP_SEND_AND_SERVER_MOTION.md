**ALPHA.394 CI VERIFIZIERT:** Featurecommit c51dd658224604886d5a536b3bd22dfc49e83214; GitHub Pi5 ARM64 Actions Run 39134573794 SUCCESS für genau denselben SHA. Mint x64 Vollbuild 360/360, 152/152 Native C++/NVIDIA OpenGL, 152/152 Python, ARM64-kompatibles Headless, isolierter Xvfb-Vollviewer sowie X11 WM_CLASS/Icon PASS. NexVerse AvatarAppearance/Outfit/WASD Live-E2E weiterhin OFFEN. Projektstatus 39 % (38,65 % gewichtet), nächster Lauf Alpha.395–404.

# Alpha.385–394 – echte WASD-UDP-Sendebestätigung und Serverpositionsquellen

18.10.2026 | develop-current | Version 0.2.0-alpha.394 | **39 % (38,65 % gewichtet, unverändert)**

## Zehn Entwicklungsblöcke

| Block | Umsetzung |
|---|---|
| 385 | GitHub HEAD 719db3f, Übergabe, Fortschritt, Projektplan, OpenGenesisLINK main 293d1d6 geprüft; Alpha.384 Pi5 ARM64 CI run 39133684078 SUCCESS zum exakt gleichen SHA. |
| 386 | Fehler bestätigt: WASD-Monitor startete nach Queue-Push, nicht nach tatsächlichem Datagrammversand; mögliche falsche Wartezeit/Stallwarnung. |
| 387 | CircuitConnection-AgentUpdateSentCallback nach erfolgreichem send_datagram. Unreliable UDP bleibt ohne Simulator-ACK. |
| 388 | Threadsafe queued_controls: echtes F3 „wartet auf UDP-Versand“ statt vorgetäuschter Sendung. |
| 389 | Superseded/rückgängig gemachte Richtungsbefehle können den Feedbackmonitor nicht nachträglich aktivieren. |
| 390 | Full ObjectUpdate für echte self-Position mit geprüfter Agent-ID und Region in Feedback aufgenommen. |
| 391 | Compressed ObjectUpdate für echte self-Position aufgenommen, bisheriger ImprovedTerseObjectUpdate bleibt. |
| 392 | F3 zeigt bestätigte Positionsevidenz mit tatsächlichem Streamnamen Terse, ObjectUpdate oder Compressed. |
| 393 | Privater Diagnosebericht enthält nur Bool/Numerik über Queue, UDP, Serverbewegung und Streamquelle; Native- und Python-Regressionsprüfungen. |
| 394 | Voller Mint-x64 Build, OpenGL/CTest, Python, Pi5-kompatibles Headless, isolierter Xvfb Viewer und Icon, Commit/Push develop-current, Pi5-CI nach SHA. |

## Nachweisgrenzen

Native simulierte Serverpositionstests beweisen: sieben Sekunden Queue-only WASD zählen NICHT als tatsächliche Übertragung; spätes send-Ereignis nach Loslassen wird ignoriert; erst reale Sendereignisse beginnen den 4s-Stallmonitor. Alternative Serverpositionen gelten erst ab mindestens 0,25 m Veränderung als bestätigte Bewegung. Dies ist **noch kein authentifizierter NexVerse-Live-Netzwerktest**. Echtweltliche Low158 Self-AvatarAppearance / SSA, Bake/Outfit-Rendering und belastbare WASD-Positionsänderung bleiben OFFEN. Kein Simulatordeploy und keine experimentelle AssetService-Freischaltung.

**Nächster Durchlauf: Alpha.395–404**. Unter Verwendung echter neuer Live-Grid-F3-Logs AvatarAppearance-Low158-Pakete/SSA eingrenzen und korrigieren, echte Server-WASD/Avatar-/Outfit-End-to-End nachweisen und sichtbare Firestorm-Bedienfunktionen weiter implementieren. Projektfortschritt nur bei nachgewiesener vollständiger Anforderungsabdeckung erhöhen.
