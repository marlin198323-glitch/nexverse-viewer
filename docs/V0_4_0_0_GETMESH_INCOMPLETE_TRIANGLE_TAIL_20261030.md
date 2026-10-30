# OGL Viewer v0.4.0.0 – OpenSim GetMesh: vollständige Dreiecke vor abgebrochener TriangleList behalten

**30.10.2026 | Linux Mint `jam-MS-7D28` | GitHub-Branch `develop-current` | Ausgangscommit `09d253eda6e3b2116e7a330fd4200d49c411aacb`.**

## Ausgangsbefund

Im früheren authentifizierten Viewer-Log vom **25.10.2026** wurden zwei fehlgeschlagene `GetMesh`-Downloads mit `Mesh LOD contains no valid renderable triangles; invalid_faces=1` protokolliert. Die Ursache dieser konkreten Assets ist aus den Protokollen **nicht rekonstruierbar**. Ein separat reproduzierbarer Decoderfehler bestand jedoch in `src/assets/mesh_loader.cpp`: Ein `TriangleList`-Binärblock wurde vollständig abgelehnt, wenn seine Länge nicht durch 6 teilbar war. Da drei 16-Bit-Indizes genau sechs Byte benötigen, konnte damit ein gültiges Dreieck am Anfang verloren gehen, sobald am Ende 1 bis 5 unvollständige Datenbytes angehängt waren. Das verhinderte den sichtbaren Mesh-Upload für die gesamte betroffene Face/LOD, selbst wenn vor dem beschädigten Rest noch gültige Geometrie lag.

## Zehn echte Entwicklungs-/Prüfblöcke

1. Auf Linux Mint `CHATUEBERGABE.md` und die Projekt-Roadmap zuerst gelesen; sauberen `develop-current`-Arbeitsbaum und gleichen GitHub-Commit bestätigt.
2. GitHub-CI zum Ausgangscommit abgefragt: Source Check erfolgreich, Linux-x64-Build zum Zeitpunkt der ersten Prüfung noch in Bearbeitung.
3. Historische authentifizierte Logs gezählt und mit der tatsächlichen `GetMesh`-Fehlermeldung abgeglichen; konkrete Asset-UUIDs nicht übernommen.
4. GetMesh-HTTP-CAPS, LLSD-Header, LOD-Fallback, Zlib, Submesh-Ablauf und OpenGL-Geometrie-Upload analysiert. Bestehende Reparaturen für beschädigte vollständige Dreiecke und alternative LODs identifiziert.
5. Den Decoder für `TriangleList` so geändert, dass nur mindestens sechs vorhandene Bytes akzeptiert werden. Aus vollständigen 6-Byte-Gruppen werden bestehende originale 16-Bit-Indizes gelesen; 1 bis 5 unvollständige Endbytes werden nicht mehr als Index interpretiert.
6. Die übrige Index-, Doppeltindex-, Flächen- und Nullflächenvalidierung unverändert belassen. Bei ausschließlich unvollständigem oder ausschließlich ungültigem Dreiecksbestand bleibt das Mesh abgelehnt; **keine gefälschten Dreiecke oder UV-Daten**.
7. Datensparsame, rein numerische Diagnosen `skipped_incomplete_triangle_bytes=N` für eine erfolgreich gerettete Fläche und `invalid_tail_bytes=N` für ausschließlich ungültige LOD ergänzt; keine Asset-ID-/CAPS-URL-Details.
8. Nativen GetMesh-CTest mit echten komprimierten Binär-LLSD-Mesh-Fixtures um vollständiges gültiges Dreieck plus 1–5 kaputte Endbytes ergänzt. Indexanzahl und Face-Geometrie exakt geprüft.
9. Weitere native Negativtests: ausschließlich 1–5 unvollständige Bytes bleiben ungültig, zwei vollständige ungültige Dreiecke plus beschädigter Endrest ebenfalls; Mesh-Cache- und OpenGL-GPU-Integration unverändert. Neun zusätzliche Python-Vertragstests überprüfen Grenzen, Statusmeldungen, GetMesh-Verkabelung und Decoder-Sicherheit.
10. Gesamte Linux-Mint-x64-Abnahme mit C++-Build, nativen CTests, Python-Unittests, isoliertem originalen Firestorm-Xvfb-/OpenGL-/GUI-Test und X11-Desktopicon. `projectplan.md`, `ROADMAP.md`, Fortschritts-JSON sowie `CHATUEBERGABE.md` synchronisiert, Commit nach `develop-current` und GitHub-CI-Prüfung.

## Was ausdrücklich offen bleibt

Die Änderung erweitert die Wiederherstellung eines **spezifischen** fehlerhaften Byte-Endes, nicht generell aller ungültigen OpenSim-Meshes. Die beiden historischen Meshfehler können durch andere nicht renderbare Daten verursacht worden sein. Der Live-Nachweis muss anhand eines neuen authentifizierten `GetMesh`-Downloads erfolgen. Die bestehenden Ressourcenlimits (16 MiB je dekomprimiertem LOD, 200.000 Vertices, 1.200.000 Indizes), Invalid-Index-Sperren, GetTexture-/JPEG2000-Sicherheitslimits und GPU-Budgets bleiben erhalten.

Offene Zielabnahmen: vollständige Head-/Upper-/Lower-Serverbakes und COF/Outfit-Skin, authentifizierter Regionenwechsel/Teleport, tatsächliches WASD-/Flug-Readback und gemessene Shutdown-Dauer nach Login.

Upstream am **30.10.2026** geprüft: `opengenesislink/opengenesislinkserver` zuletzt `08bf15f306bb4f882f24f32b84eb8df3a3175cea` (Login-Zeitzone). Kein serverseitiger Protokollumbau für diesen lokalen Mesh-Decoder-Fix erforderlich.

**Version `v0.4.0.0` IN ARBEIT, Projekt 38,65 % gewichtet (39 % gerundet); kein `main`-Merge und keine Produktivfreigabe.**
