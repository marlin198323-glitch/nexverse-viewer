# Alpha.425–434 – Simulator-WASD: Bewegung vs. Teleport oder Gravitationskorrektur

18.10.2026 | develop-current | Version 0.2.0-alpha.434 | Projektstand 39 % (38,65 % gewichtet).

## Zehn überprüfbare Blöcke

| Block | Implementierung / Test |
|---|---|
| 425 | GitHub develop-current SHA d832801, CI für Alpha.424 SUCCESS, CHATUEBERGABE, Projektplan/Progress und OpenGenesisLINK Server main 293d1d6 geprüft. |
| 426 | Falschen WASD-Erfolgsnachweis bei rein vertikaler Avatarbewegung entdeckt: bisher wurde XYZ gesamt geprüft; Bewegungsfeedback bewertet nun ausschließlich mindestens 0,25 m horizontale Self-X/Y-Änderung für W/A/S/D. |
| 427 | Ein deutlich großer Simulator-Positionssprung (>= 32m pro einzelner Serverprobe, XYZ) gilt als potentieller Teleport/Regionsevent, NICHT als nachgewiesene WASD-Bewegung. |
| 428 | Nach Sprung im selben Regionlease Serverbaseline und Antwortuhr neu gesetzt, bisherige Bestätigung/Stall aufgehoben, eigenes MovementEvidence::server_position_discontinuity und klarer F3-Hinweis. Kein erfundener Erfolg. |
| 429 | Out-of-order und vor dem tatsächlich gesendeten UDP-AgentUpdate datierte Serverproben können keine Bewegung bestätigen und verzerren auch nicht mehr die aktuelle Baseline. |
| 430 | Serverposition-Probenzähler sowie Sprung-/Stale-Zähler gegen unsigned 32bit Überlauf gesichert; finite Werte bleiben Pflicht. |
| 431 | Bei tatsächlichem Wechsel der Firestorm-Regionlease Bewegungsmonitor zurücksetzen (wie bereits Appearance/GL-Zustand), damit keine alten Bewegungserfolge in ein Zielgebiet mitgenommen werden. |
| 432 | Die drei realen Rückkanäle ObjectUpdate, Compressed und ImprovedTerse protokollieren den Sprung als ausgeschlossen, nicht als WASD-verified; alte Quelllogik ohne berechnete Vorhersage erhalten. |
| 433 | F3-Bewegungsstatus zeigt nach Sprung die ausstehende neue Serverantwort. Whitelist-Privatbericht enthält nur Anzahl ignorierter Sprünge/staler Proben und bools; KEINE Weltkoordinaten, Agentenkennungen oder Session-IDs. |
| 434 | C++-Regressionen: 3D Z-only ohne Erfolg, Server-Stall trotz Z, Teleport-/Sprungbaseline, danach echte 0,25m X/Y, stale Packet, ohne echten UDP-Send kein Erfolg. Python-Verträge, Mint x64/C++/OpenGL, ARM64 Headless, Xvfb/Taskleistenlogo, GitHub Push und SHA-gebundene Pi5-CI. |

## Evidenzgrenzen

Diese Tests validieren Offline-Verarbeitung von synthetischen Self-Positionen und echten Codepfaden, aber keine Live-NexVerse-Session. Der Sprungschwellwert ist eine konservative Heuristik für die Diagnose, kein Beweis eines Teleports. Live-Low158-Self-AvatarAppearance, vollständiges eigenes Outfit, Bakes und tatsächlich von der Simulation bestätigte WASD-Bewegung sind noch offen. Deshalb keine voreilige Erhöhung über 39 %. Kein produktives Deployment.

**Bei WEITER Alpha.435–444:** echte Grid-Live-Self-Messages/SSA/COF und Outfit-Bake/Renderer-E2E priorisieren; fehlende Firestorm-UI-Aktionen ergänzen. Upstream OpenGenesisLINK Server bei jeder Runde auf Updates prüfen.
