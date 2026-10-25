# OGL v0.4.0.0 – OpenSim-GetMesh-Dreiecksreparatur und JPEG2000-Metadatenursachen

**25.10.2026, Desktop Commander Linux Mint jam-MS-7D28, develop-current.** Ausgangscommit e25ece43904a26d24e39ae1914f86b19e2810e3a.

## Reproduzierter Befund

Die authentifizierten Viewer-Logs vom 25.10.2026 enthalten `Mesh LOD contains no valid renderable triangles; invalid_faces=1` und `GetTexture ... JPEG2000 image metadata exceeds safe limits`. Die beiden Meldungen sind unterschiedlich zu behandeln: Ein Mesh-Face kann neben kaputten Dreiecksreferenzen noch gültige Dreiecke enthalten. Bei JPEG2000 darf ein auffälliger Header dagegen nicht durch Lockerung der Bild-/Speicher-/Farbtiefenbegrenzungen als gültig angenommen werden.

## Zehn bearbeitete Blöcke

1. `develop-current`, Übergabe, Roadmap, Projektplan und GitHub-CI vor Änderungen geprüft; lokaler Ausgangsbranch sauber.
2. Aktuelle GetMesh-/JPEG2000-Decoder und zugehörige echten Logwarnungen untersucht, ohne Asset-UUIDs, URLs oder geheime Sessiondaten in die Dokumentation zu übernehmen.
3. GetMesh-Schutzgrenzen (32 MB komprimierter Asset, 16 MB LOD, 200.000 Vertices, 1.200.000 Indizes) beibehalten.
4. GetMesh kann jetzt eine gültige Dreiecksfläche innerhalb desselben Faces erhalten, auch wenn andere Dreiecke dieses Faces außerhalb der Vertex-Liste verweisen.
5. Dreiecke mit doppelten Eckpunktindizes oder nach Dekodierung exakt null Flächeninhalt werden nicht mehr fälschlich als renderbare Geometrie ausgegeben.
6. Vollständig ungültige Faces/LODs bleiben **ungültig** und nutzen wie bisher die LOD-Fallback-Prüfung. Erfolgsmeldungen enthalten `skipped_invalid_triangles=N`; Fehlermeldungen zählen `invalid_faces` und `invalid_triangles`, keine Mesh-ID.
7. JPEG2000-Header-/Komponentenbeschränkungen in einen sicheren, von OpenJPEG unabhängigen Metadaten-Validator ausgelagert. Fehlerschlüssel unterscheiden u. a. `image_extent`, `image_dimensions`, `channel_count`, `component_dimensions`, `component_subsampling`, `component_precision`.
8. Keine JPEG2000-Sicherheitsgrenze erhöht: 4.096 px je Seite, 64 MiB RGBA, ein bis vier Komponenten und maximal 16 Bit je Komponente bleiben bestehen. Nur eine präzisere Diagnose ist implementiert; das bislang betroffene Grid-Asset ist noch nicht erneut authentifiziert getestet.
9. Native Mesh-/Texture-/JPEG2000-Regressionstests und fünf neue Python-Quellverträge prüfen Teilrettung, komplett beschädigte Faces, Ablehnungsgründe und unveränderte Sicherheitslimits.
10. Vollständiger Linux-Mint-Gate über `scripts/mint-viewer-validation.sh`: **190/190 CTests**, **699/699 Python-Tests**, isoliertes Xvfb-/Firestorm-OpenGL-/GUI-Gate sowie X11-Taskleistenicon **PASS**.

## Freigabe und nächste Diagnoseschritte

Die Mesh-Teilfehlerbehandlung ist lokal repariert. JPEG2000-Metadaten werden präziser kategorisiert, aber der konkrete vom OpenGenesisLINK-Grid gelieferte fehlerhafte J2C-Inhalt wurde **nicht** erneut geladen. Nächste Prüfung: neuer authentifizierter Viewer-Lauf, die `reason`-Meldung des betroffenen Textur-Assets ohne URL/UUID erfassen, dann Ursachenbereich oder gegebenenfalls Serverasset untersuchen. Danach Head-/Upper-/Lower-Bakes mit tatsächlichen GPU-Draw-Readbacks vergleichen, Mesh-LODs aus realer Region erneut testen und die tatsächliche Shutdown-Dauer messen.

**Version v0.4.0.0 IN_ARBEIT, Gesamtprojekt weiterhin 38,65 % (39 % gerundet), main/Release unverändert.** Keine Produktionsinstallation durchgeführt.
