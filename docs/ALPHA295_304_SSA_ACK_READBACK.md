# Alpha.295–304 – Authentifizierter SSA-Handshake: ACK und eigener Avatar

17.10.2026 | Viewer 0.2.0-alpha.304 | develop-current | **Gesamtfortschritt 39 % (38,65 % gewichtet)**

## Zehn echte Entwicklungsblöcke

| Block | Realer Code und UI | Prüfung |
|---|---|---|
| 295 | GitHub Viewer-HEAD auf 7b119c9 / develop-current verifiziert, zuerst CHATUEBERGABE.md gelesen. OpenGenesisLINK Server main 7c6b0cf7252189707066f09ad66c732642b27af9 unverändert: optionales ROBUST-AssetService-Addon. | Live GitHub |
| 296 | SelfAppearanceHandshakeReceipt mit exakter LLUDP-Sequenz eingeführt, separates sent/acknowledged/reset statt einer globalen ACK-Boolean. | Native C++ |
| 297 | CircuitConnection wertet reale explizite PacketAck- und angehängte ACKs nur nach positivem ReliableTracker.acknowledge(ack) aus; Client-ACK ist nicht mit Server-AvatarAppearance verwechselt. | Native / Source |
| 298 | Bei neuem RegionHandshake und neu gestarteter Verbindung wird die alte Handshake-Sequenz entwertet. Verspätete ACKs, falsche Sequenzen, duplicate ACKs und ungeprüfte ACKs zählen nicht. | native positives/negatives |
| 299 | Erst bei tatsächlichem passenden ACK wird monotoner ACK-Zeitpunkt gespeichert. Keine Simulatoradresse, UUID, Zugangsdaten oder CAP-URL. | Circuit + Clock |
| 300 | Wahrheitsgetreue SSA-Stufen: Region fehlt, Bit2 nicht gesendet, gesendet/ACK ausstehend, ACK bestätigt/Self ausstehend, tatsächlicher authentifizierter Self-Paketempfang. | UI / C++ |
| 301 | Nach 15s bestätigtem ACK ohne tatsächlich eingetroffenes Self-Paket zeigt F3 passive Wartewarnung. Keine ungeprüften HTTP-Anfragen, keine neue Broadcast/LLUDP-Retry-Policy, kein behaupteter Serverfehler. | C++ timeout tests |
| 302 | Die F3-Avatar-Ampel nutzt nur noch aktuelles authentifiziertes Regionen-Self-Paket aus AvatarPipelineProof statt bloß möglicherweise älterem WorldModel-Snapshot. Alle drei F3-Seiten zeigen aktuelle SSA-Evidenz. | App / GUI |
| 303 | Privater Diagnoseexport erweitert nur whitelisted ACK-Boolean und auf höchstens 120s begrenzte monotone Wartezeit. Keine Secrets/Account-/Avatar-ID. Neue Native-Receipt-Testbinärdatei, LLUDP-Golden-Tests und Python-Wiring-Contracts, ARM64-Headless aufgenommen. | 0600 / native / Python |
| 304 | VERSION, Projektplan, Info, Übergabe, Matrizen, Prozentstand und Volltests auf Mint x64/NVIDIA, Python, Headless, isoliertes Xvfb/Originalicon, Push nach GitHub und Pi5-CI nach tatsächlichem SHA. | CI/Gates |

## Was dieser Stand nicht behauptet

Ein zuverlässiger Handshake-ACK des Simulators ist nur eine Zustellbestätigung des RegionHandshakeReply mit SSA-Bit2, **nicht** die Antwort mit AvatarAppearance. Erst die akzeptierte serverseitige Appearance in der aktiven authentifizierten Region beweist Empfang; GPU-/Original-LLM-/BakeGL-Belege sind davon nochmals zu unterscheiden. Die Warteanzeige nach 15 Sekunden zeigt nur bislang fehlende Daten an und gibt keine Aussage zur Serverursache.

NexVerse-Grid-Login, Simulatorantwort mit SelfAvatarAppearance, sichtbare Kleidung, Outfit-/Bake-Assets und in der echten Welt sichtbare Pixel wurden hier mangels authentifizierter Live-Grid-Sitzung nicht durchgeführt. Firestorm-Parität und Production-Deployment bleiben **OFFEN**. Keine Aktivierung des experimentellen OpenGenesisLINK OGLAssetService-Serveraddons.

**Gesamtfortschritt weiterhin 39 %** (38,65 % gewichtete Schätzung). Nächster verbindlicher Zehnerdurchlauf **Alpha.305–314**: reales SelfAppearance-Serverreadback mit Testaccount, Outfit-/Bake-/Avatar-Inworld-Pixelbeweis, UI-/Funktionstests.

## Tatsächlich bestandene Freigabegates

**ALPHA.304 TEST-GATES VERIFIZIERT:** Feature-SHA a6d7942173e330b840611ac1f55c952da3cca564; Pi5 ARM64 CI Run 39068198529 **SUCCESS für exakt denselben Commit**. Mint Debug x64 Fullbuild 342/342 Schritte, **143/143 Native C++23/NVIDIA-GL CTest**, **107/107 Python**, lokaler ARM64-kompatibler Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon **PASS**. Authentifiziertes Live NexVerse SelfAvatarAppearance/Outfit-Bake-Pixel-E2E weiter OFFEN. **Gesamtfortschritt 39 % (38,65 % gewichtet)**. Nächster Zehnerlauf Alpha.305–314.
