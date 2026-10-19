# Alpha.485–494 – verifizierbare Self-Low158-Körperbakes und Avatar-F3-Fehlertrennung

19.10.2026 | Branch develop-current | 0.2.0-alpha.494 | Projektfortschritt 39 % (38,65 % gewichtet).

## Zehn tatsächliche Entwicklungsblöcke

| Block | Umsetzung und Test |
|---|---|
| 485 | Quellstand 661ecef, develop-current, CHATUEBERGABE, Projektplan, Fortschrittsdatei und erfolgreich für den exakten SHA validierte Alpha.484 Pi5 ARM64 CI Run 39290341106 geprüft. |
| 486 | Aktuellen OpenGenesisLINK Server-main SHA 293d1d6 erneut gelesen: SendAppearance ist reliable zero-coded Low158, AppearanceHover aber kein AttachmentBlock. Zusätzliche Quellrecherche zu GetBakesBytes, keine Behauptung echter Live-Assets ohne Grid-Log. |
| 487 | Echten Simulator-Low158-Wire-Fixture mit drei voneinander verschiedenen TextureEntry-Override-UUIDs für die Firestorm-Faces 8 (Head), 9 (Upper), 10 (Lower) ergänzt. Keine Default-Face-Bakes erfunden. |
| 488 | Volle Native Kette vom zuverlässigen zero-codierten LLUDP über Low158, gültigen TE-Parser und aktuellen authentifizierten Self-WorldModel bis extract_avatar_bakes und self_render_bakes geprüft. |
| 489 | Dieselben drei tatsächlich dekodierten Body-IDs durch reale prioritize_avatar_body_bakes und plan_texture_residency laufen lassen. Die drei Body-Faces gewinnen unter 5 Asset-Slots vor Partikel und Weltmaterial; alte Assets kommen zur Eviction. |
| 490 | Re-Auth/Agentwechsel nach erfolgreichem Bake-Readback löscht die drei Referenzen aus dem Self-WorldModel. Weder fremde UUID noch alter Skin darf weitergerendert werden. |
| 491 | Private gespeicherte Avatar-Diagnose um getrennte Empfangsflag, 11-Slot gültige Bake-Referenzen, 3-Slot Body-Referenzen und Head/Upper/Lower-Bools ergänzt. Keine IDs, Assets, Nutzertexte oder Daten in Logs/Report. |
| 492 | F3-Avatarinspektor erkennt explizit Low158 vorhanden, aber keine echten Body-Bake-IDs; ebenso teilweise fehlende Head/Upper/Lower-IDs. GPU-Upload/-Draw-Failures bleiben getrennte spätere Diagnose. |
| 493 | Neue native Tests für Body-Face-TE, Priorität und Eviction, Session-Isolation, private Numeric-Reports und F3-Server-vs-GPU-Status. Python-Vertrag für 10 Blöcke und Release/Handoff. |
| 494 | x64 Vollbuild, CTest mit nativen GL-Tests, Python, ARM64-kompatible Headless-Tests, Xvfb-Viewer und Systemicon. GitHub Feature-Commit, Pi5 CI exakt für Feature-SHA, dokumentierte Übergabe und nächste zehn Blöcke Alpha.495–504. |

## Bedeutung

Die Alpha.484-Korrektur sorgt dafür, dass reale Legacy Low158 vom OpenGenesisLINK Server nicht mehr wegen des fehlenden AttachmentBlocks verworfen werden. Hier wird nachgewiesen, dass zusätzliche gültige Head-, Upper- und Lower-Bake-Face-IDs aus einem **synthetischen** Paket bis zur aktuellen Self-Streaming-Priorität und Bake-Diagnose durchlaufen. Es ist kein Beleg, dass der tatsächliche Live-Server alle drei IDs liefert oder dass die zugehörigen Asset-Pixel bereits geladen und vom eigenen Rig in der Welt gezeichnet werden.

Die F3-Auskunft ist nun eindeutig: keine/teilweise Bake-IDs = Datenquelle/Simulator; gültige IDs ohne GPU/Draw = Textur-/Rendererstrecke. Die privaten Werte können über die bereits vorhandene F3-R-Diagnosefunktion sicher gespeichert werden. **Kein produktiver Rollout; kein authentifizierter Grid-Avatar/COF/Outfit/WASD-E2E-Test.** Gewichteter Fortschritt unverändert 38,65 % / rund 39 %.

**Nächste zehn Blöcke Alpha.495–504:** Live-Test mit aktuell aus dem Viewer erzeugten Logs und Bildschirmdarstellung; echte NexVerse Server-Low158 / Textur-Residenz / Head-/Upper-/Lower-GPU-Zeichnung und Firestorm-Avatar/Kleidung, dann Weltbewegung/Fenster weiter integrieren.
