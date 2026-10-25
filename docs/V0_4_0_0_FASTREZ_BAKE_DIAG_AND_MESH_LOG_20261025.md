# OGL v0.4.0.0 – FastRez-Bake-Diagnose und Mesh-Log-Drosselung

**25.10.2026 – Linux Mint `jam-MS-7D28`, Branch `develop-current`, Ausgangscommit `8011ff4eb011f3179a5f363ff8db5eb6c4aed91a`.**

## Beleg aus echten Logdaten

Das lokale Langzeitlog `~/.local/state/opengenesislink/viewer.log` umfasst am 25.10. 144.966 Zeilen / ca. 12,6 MB, davon **102.762** Einträge `OGLRender mesh geometry uploaded` seit 10.10. Dies ist Protokollvolumen über mehrere Läufe und nicht der Beweis einer GPU-Endlosschleife.

Der authentifizierte NexVerse-Lauf vom **25.10.2026 15:40:08** zeigt `WorldModel AgentMovementComplete validated`, `AvatarAppearance SELF buffered until ... circuit is ready` und unmittelbar danach `Early authenticated Low158 SELF released ...`. Die frühe Low158 wurde also übernommen; bisher fehlte jedoch deren Gegenprobe auf echte Head/Upper/Lower-Bake-Referenzen. Später folgen echte Mesh-LOD-Dekodierwarnungen und JPEG2000-/GetTexture-Metadatenfehler. Diese beiden assetbezogenen Fehler werden durch die hier implementierte Beobachtbarkeit noch **nicht** repariert.

## Zehn bearbeitete Teilblöcke

1. Sauberen `develop-current`-GitHub-/Linux-Mint-Stand, `CHATUEBERGABE.md`, `ROADMAP.md` und vorherige CI prüfen.
2. Live-Laufzeitlog strukturiert und ohne Asset-UUID-/Session-/CAPS-Geheimnisse auf Avatar, Outfit, Meshes und Fehler durchsuchen.
3. Tatsächliche frühe Low158-Admittanz nach Circuit-Freigabe nachweisen, statt fälschlich einen nicht zugestellten AvatarAppearance-UDP-Readback anzunehmen.
4. Früh eingetroffene, authentifizierte Low158 unmittelbar nach `flush_early_self_readbacks()` auf echte Head/Upper/Lower-Bakes prüfen.
5. Zusätzlich die aktuell vom Renderer auswählbaren Body-Bake-Kandidaten zählen sowie getrennt die Anzahl provisorisch aus `ObjectUpdate` stammender Faces angeben.
6. Im Log ausdrücklich unterscheiden zwischen empfangenem Simulatorreadback, vorhandener Bake-Referenz, GPU-Laden und tatsächlich sichtbarer Skin – **kein erfundener GPU- oder COF-Erfolg**.
7. 102.762 frühere einzelne erfolgreiche `upload_mesh_asset()`-INFO-Meldungen als signifikante Diagnose- und Logdateilast identifizieren.
8. Erfolgreiche Mesh-Uploads nur noch bei der ersten und bei jeder 128. erfolgreichen Operation als aggregierte `count`-, `triangles_total`- und `resident`-Information protokollieren; **keine Änderung an Mesh-Uploads, VRAM oder Fehlermeldungen**.
9. Neue native Regression `nexverse-mesh-upload-log-checkpoint-tests`, erweiterter authentifizierungsnaher `nexverse-early-avatar-readback-tests` ohne gefälschte Bakes, fünf Python-Verträge für Bake-Provenienz und Log-Frequenz.
10. Vollständiges Linux-Mint-Gate: **189/189 native CTests**, **694/694 Python**, isolierter Original-Firestorm-Xvfb-OpenGL-/GUI- und X11-Systemicon-Test: **PASS**.

## Nächste P0-Blöcke

- Live-NexVerse-Login nach diesem Commit und frühe Logzeile `body_bakes_low158=.../3; body_bakes_candidates=.../3; provisional_object_faces=...` prüfen (kein UUID-Dump).
- Falls 0/3: Simulator/SSA/Current Outfit/Bake-Produktion gegen OpenGenesisLINK-Server untersuchen. Falsche Texturreferenzen dürfen nicht vom Client erfunden werden.
- Falls Bake-Referenzen vorhanden: echten `GetTexture`-JPEG2000-Assetpfad und Decoderwarnung `JPEG2000 image metadata exceeds safe limits` sowie GPU-Upload behandeln.
- Mesh-Asset mit `Mesh LOD contains no valid renderable triangles; invalid_faces=1` isoliert untersuchen (LODs/Faces, dekodierte Triangles), ohne Mesh-ID offenzulegen.
- Authentifizierte Teleport-, WASD-/Flug- und Shutdown-Dauertests bleiben offen.

**Version `v0.4.0.0` IN_ARBEIT. Projektgesamtfortschritt: 38,65 % (39 %). Kein Produktivdeployment, kein main-Merge.** Die erfolgreiche lokale GUI-Simulation ersetzt keinen aktuellen Login-/Outfit-E2E-Nachweis.
