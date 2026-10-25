# OGL Viewer v0.4.0.0 – P0: Veraltete GPU-Depth-Nachweise beim Skin-/Outfit-Wechsel

Datum: 25.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Basis `45dc41d02ad1f3955cba960879f094078a394ea1`.

## Fehlerursache

Der OGL-Renderer besitzt sechs nicht blockierende `GL_ANY_SAMPLES_PASSED`-Abfragen (zwei je originalem Firestorm-LLM Head/Upper/Lower). Die Ergebnisse werden über einen Generation-/Sequence-Zähler gegen Regions- und Kamerawechsel geschützt.

Ändert der Simulator aber in **derselben Region** die Head-/Upper-/Lower-Bake-Textur (COF/Outfit-Wechsel), so blieb der Query-Generation-Zähler bisher unverändert. Ein altes, bereits abgeschlossenes Sichtbarkeitsergebnis konnte dadurch weiterhin als Nachweis für neu ausgewählte Texturen gelten. Die vorige Korrektur verlangte zwar alle drei bekannten GPU-Teilabfragen, aber die Ergebnisse mussten noch nicht aus **derselben aktuellen Bake-Generation** stammen.

Zusätzlich konnte ein Texturasset erfolgreich unter derselben ID ausgetauscht oder aus dem GPU-Cache entfernt werden. Änderungen des Alpha-Kanals könnten den Depth-Test verändern, ohne dass die alte GPU-Sichtbarkeit invalidiert würde.

## Zehn erledigte Prüf-/Entwicklungsblöcke

1. `CHATUEBERGABE.md`, Projektplan, Roadmap, GitHub-/Mint-Branch und offene CI vor jeder Codeänderung prüfen.
2. Renderpfad, asynchrone Query-Slots, GL-Sequence-/Region-Epochen, Selbstavatar-Proxies und Kamera-Invalidierung vollständig auf Nachweisgrenzen prüfen.
3. Reproduzierbaren fehlenden Invalidierungspfad identifizieren: Gleiche Region/Kamera, neue Self-Avatar-Body-Bake-IDs.
4. Reine kleine `SelfAvatarVisibilitySourceGate` eingeführt: genau drei selektierte Bake-IDs pro Self-Avatar, optionales Self-Vorhandensein. Nicht in Logs, Reports, Web oder Dateien persistiert.
5. Bei Änderung irgendeines authentisch dem Renderer übergebenen Head-/Upper-/Lower-Bake-IDs oder bei Verlust/Wiederkehr des Self-Avatars `reset_self_avatar_visibility()` ausführen. Unveränderte Quellen führen nicht zu dauerndem Reset.
6. Nach erfolgreicher GPU-Textur-Ersetzung unter derselben Self-Bake-ID ebenfalls die sechs alten asynchronen Depth-Ergebnisse verwerfen. Keine solche Invalidierung bei fehlgeschlagenem Upload.
7. Wird eine verwendete Self-Bake-ID aus dem GPU-Texture-Cache entfernt oder der Texturcache geleert, sind die Depth-Ergebnisse ebenfalls ungültig. Unbeteiligte Texturuploads verändern den Self-Depth-Status nicht.
8. Eigenständigen nativen Gate-CTest für Source-Wechsel, Identität, Rückkehr, Placeholder-/Missing-Slot und expliziten Reset ergänzt.
9. Bestehenden echten, isolierten Linux-Mint-OpenGL-3.3-Regionstest erweitert: zunächst asynchron bestätigte Sichtbarkeit, dann aktuelles Upper-Bake wechseln, alte Query-Ergebnisse ablehnen, gleiche Quellen wieder zulassen, altes Bake-Set zurücksetzen, gleiche Bake-ID neu hochladen und unbeteiligte Textur hochladen. Sechs neue Python-Verträge für die tatsächliche C++-Verkabelung und Datenschutz.
10. Vollständige Mint-x64-Abnahme (C++-Build, CTests, Python-Tests, Original-Firestorm Xvfb/OpenGL-GUI und X11-Systemicon), Projektplan, Roadmap, Fortschritts-JSON und Chatübergabe aktualisiert. GitHub-Commit und CI separat prüfen.

## Abnahmegrenzen

Dieser Fix verhindert **falsch-positive GPU-Sichtbarkeit nach COF-/Bake-Änderungen**. Er erstellt weder Server-Bakes noch COF-Outfits und ist kein RGB-Pixel-Beweis. `avatar_skin_blocker_key` bleibt daher nach gültigen GL-/Depth-Ereignissen bei `depth_visible_final_skin_pixels_unverified`.

Der nächste echte NexVerse-Login muss weiterhin `Avatar SELF bake pipeline`, Low158-Refs, GetTexture-Stufen, aktuelle Source-/GL-/Depth-Teilzustände und den sichtbaren Avatar kontrollieren. Gleiches gilt für Regionswechsel, WASD/Flug und Shutdown nach authentifiziertem Login.

**v0.4.0.0 bleibt IN_ARBEIT, Gesamtfortschritt 38,65 % (~39 %). Kein Merge nach `main`, keine Produktionsinstallation.**
