# Alpha.285–294 – Eigenavatar-SSA-Handshake (Firestorm-Kompatibilität)

17.10.2026 | OGL 0.2.0-alpha.294 | develop-current | **39 % Gesamtfortschritt** (38,65 % gewichtet, unverändert).

## Verifizierter Fehler und Protokollvergleich

Bisher versendete die echte CircuitConnection ihren RegionHandshakeReply mit dem Default Flags=0. Die offiziellen Second-Life-Viewer-Quellen indra/newview/llviewerregion.h und llviewerregion.cpp definieren und verwenden dagegen REGION_HANDSHAKE_SUPPORTS_SELF_APPEARANCE als 1 << 2, also 0x00000004 auf dem Wire. Die vorhandene 0-Kennung kann deshalb verhindern, dass ein kompatibler Simulator eigene AvatarAppearance-Informationen sendet. Das Setzen ist lediglich eine Viewer-Fähigkeitsmeldung, keine Garantie für eine Serverantwort. Referenz: https://github.com/secondlife/viewer/blob/main/indra/newview/llviewerregion.cpp

## Zehn reale Entwicklungsblöcke

| Block | Code, GUI und Tests |
|---|---|
| 285 | Aktueller OpenGenesisLINK-Server main 7c6b0cf geprüft: nur OGLAssetService Plugin-Konstruktor/Build/Dependencies, kein neuer Avatar-Wirevertrag. SSA-Handshake-Firestorm-Referenz und echten Bitwert ermittelt. |
| 286 | Neue Protokollkonstanten für SSA Bit 2, separate LL-Objektcache-Bits 0/1, statischer Schutz gegen irrtümliches Aktivieren nicht implementierter Cache-Funktionen. |
| 287 | LLUDP-Standard RegionHandshakeReply erhält Flag 4 statt 0; expliziter alter Flags=0-Pfad bleibt möglich, Byteposition/Little Endian bleibt identisch. |
| 288 | Echte CircuitConnection kodiert und versendet Bit2 zuverlässig beim Regions-Handshake (Reliable LLUDP). Keine Avatar-, Wearable-, Bake- oder Admin-Uploads. |
| 289 | Circuit-Status SSA Bit2 gesendet erst nach dem erfolgreichen lokalen Sendeaufruf; jeder neue Regions-Handshake setzt diesen Status vor dem Versand zurück. |
| 290 | Wahrheitsgetreue Anzeige unterscheidet Region wartet, SSA nicht gesendet, SSA gesendet/Self-Paket ausstehend und echte Self-Paketannahme. |
| 291 | F3-Avatarfenster zeigt diese SSA-Information auf Übersicht und beiden Bake-Seiten; bestehende Fensterbedienung bleibt erhalten. |
| 292 | Manuell speicherbarer datensparsamer F3-Diagnoseexport ergänzt ausschließlich Bit2-Sende-Boolean ohne UUIDs, Chat, Kennwörter oder Capability-URLs. |
| 293 | Erweiterte LLUDP-Golden-Wire-Regression für Flag4, Bytes 32–35, Reliable, Cache-Bits 0/1 aus, expliziten Flags=0-Pfad; Empfangsstatus-/Exporttests und neue Python-Verkabelungs-Contracts. |
| 294 | Vollständiger Mint-x64-Build und native NVIDIA/OpenGL-/Python-/Headless-/Xvfb-/WM_CLASS-Gates, Projektdokumentation, GitHub-Push und Pi5 ARM64-CI. |

## Beleg- und Sicherheitsgrenzen

SSA Bit2 sendet die Fähigkeit des Viewers; erst ein tatsächlich empfangenes und authentifiziertes eigenes AvatarAppearance-Paket ist der Serverbeleg. Es werden keine ausgefallenen Statusmeldungen als Server-Antwort interpretiert, keine fremden Avatarbakes übernommen und keine LL-Objektcache-Funktionen vorzeitig aktiviert. Ein erfolgreiches Headless/Xvfb-Ergebnis ersetzt keine Live-NexVerse-Anmeldung. Server-eigener OGLAssetService bleibt optional, experimentell und deaktiviert.

**Projektfortschritt 39 % (38,65 % gewichtete Schätzung), unverändert. Nächste zehn Blöcke Alpha.295–304.**
