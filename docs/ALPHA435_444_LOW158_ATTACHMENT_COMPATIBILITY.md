# Alpha.435–444 – Firestorm Low158 Multiattachment und GUI-Provenienz

18.10.2026 | develop-current | 0.2.0-alpha.444 | Gesamtstatus **39 %** (38,65 % gewichtet).

## Zehn Entwicklungsblöcke

| Block | Umsetzung / Nachweis |
|---|---|
| 435 | Aktuellen Repo-HEAD 6f1af0d, Pi5-SUCCESS Run 39139112676, CHATUEBERGABE, projectplan.md und OpenGenesisLINK Server-main 293d1d6 geprüft. |
| 436 | Reale Protokoll-Inkompatibilität identifiziert: AvatarAppearance Low158 AttachmentBlock ist LLUDP Variable 1 mit U8-Länge. Der selbst auferlegte Grenzwert 64 verwarf legitime vollständige Pakete mit 65+ Anhängen. |
| 437 | Künstliche Limitierung 64 aus Parser entfernt; exakt überprüfte 17-Byte-Einträge und vollständige Restlänge weiter erzwungen. Keine unlimitierten Arrays: Byte-Zähler bis maximal 255. |
| 438 | Neu nativer C++-Test: tatsächliches LLUDP Low158 Datagramm (65 Attachment-Einträge) decodiert und nach Prüfung bis zur eigenen WorldModel-Self-Appearance geleitet. Die maximale Ein-Byte-Zählergröße wird separat auf Dekoderebene geprüft. |
| 439 | Kein Aufweichen der LLUDP-MTU: 255 Attachments überschreiten die tatsächliche 1200-Byte-Paketgrenze und werden auf Datagrammebene explizit abgewiesen. Der Parser testet nur die syntaktische Feldkapazität, ohne zu behaupten, dass so ein großes Datagramm transportiert werden kann. |
| 440 | Abgeschnittene Blöcke, falsche Längen sowie fremde Agentenkennungen verworfen; bestehende korrekte Self-Appearance und Anhänge bleiben erhalten. Agent-/Sessionwechsel bzw. Regionswechsel invalidieren alte Anhänge. |
| 441 | F3-Avatarstatus zeigt die Zahl der wirklich empfangenen eigenen SSA-AttachmentBlock-Einträge und kennzeichnet sie als nur vom Server gemeldet, NICHT automatisch im Render-Scenegraph sichtbar. |
| 442 | F3 passt die Zeile auf 72 Zeichen an; fehlende SSA-Serverantwort und explizit null übermittelte Anhänge unterscheiden sich. Alte fälschliche Beschriftung "Objekte" entfernt. |
| 443 | Privater Bericht ergänzt booleschen Empfangsstatus und Anzahl, ohne Objekt-UUIDs, AV-/Session-ID oder Asset-Links; strikte aktuelle Session-/Regionlease-Prüfung. OpenGL-Panel-Test mit 85/0 Anhängen und C++-Diagnoseberichtregression. |
| 444 | Gesamter Build Linux-x64, native C++/NVIDIA-GL-CTests, Python-Verträge, ARM64-kompatibles Headless, Xvfb und Taskleistenicon, GitHub Commit/Push und exakt SHA-basierter Pi5-ARM64 CI-Gate. |

## Technische Grenzen

Firestorm/OpenSim erlaubt mehrere angehängte Objekte je Befestigungspunkt. Der verfügbare U8 Blockzähler ist nicht mit einer Zusage über 255 per normalem LLUDP zugestellte Objekte gleichzusetzen: MTU 1200 Bytes wird **weiterhin** eingehalten. Der Test mit 65 benutzt ein reales synthetisches Low158-Datagramm, keinen Live-Grid-Login. F3 zählt nur vom Simulator gemeldete IDs, nicht geladene Anhänge, tatsächlich sichtbare Meshes oder Kleidung.

Es liegt weiterhin **kein Live-Nachweis** für das autoritativ eintreffende Self-Low158-Paket in einer authentifizierten NexVerse-Sitzung, alle eigentlichen Wearables/COF, auf GPU gezeichnete eigene Skin- und Anhang-Objekte oder serverbestätigte WASD vor. Daher Projektwert 39 % unverändert. Keine produktive Aktualisierung.

**Nächste zehn Blöcke Alpha.445–454:** Authentifizierten Server-Readback und Live-Asset-Fluss prüfen, Self-Mesh-/Attachment-Rendering inkl. Firestorm-Bedienaktionen weiter integrieren; keine erfundenen Erfolgsmeldungen ohne Live-Nachweis.
