# OGL Viewer v0.4.0.0 – P0: Avatar-Bewegung, Shape-Upload und Resize invalidieren alte GPU-Depth-Beweise

**25.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Basis-Commit `51682f31e17352f7f1f09a426e4e39dfa9bd61b1`.**

## Ausgangsfehler

Die originalen Firestorm-LLM-Head-/Upper-/Lower-Meshes werden mit nicht blockierenden `GL_ANY_SAMPLES_PASSED`-Queries beobachtet. Regionwechsel, Kamera und zuletzt Bake-Texturquelle invalidierten ältere Resultate bereits. Drei Änderungen waren noch nicht gedeckt:

- Self-Avatar bewegt oder dreht sich bei unveränderter Kamera: Geometrieposition/Silhouette der zuvor abgefragten Pixel ist nicht mehr aktuell.
- Originale/an den COF-Shape-Morph angepasste Head-/Upper-/Lower-LLM-Geometrie wird neu auf die GPU geladen, aber die Server-Bake-UUID bleibt gleich.
- Das Viewerfenster wird in der Größe verändert: Eine andere Aspect-Ratio verändert die Projektion, ohne dass Kamera-Position, Vorwärtsvektor oder vertikale FOV zwingend wechseln.

Das sind **falsch-positive Beweisaussagen**, keine nachgewiesenen Fehler im tatsächlich vom Simulator gelieferten Skin-Asset. Die Änderungen dürfen keine Bakes vortäuschen und die Darstellungs-Pipeline nicht bei jeder minimalen numerischen Änderung unbegrenzt zurücksetzen.

## Zehn Entwicklungs-/Testblöcke

1. Aktuellen `develop-current`-Commit mit GitHub abgeglichen, Arbeitsbaum sauber; `CHATUEBERGABE.md`, `projectplan.md`, `ROADMAP.md` und CI geprüft.
2. Renderer-Pfade für asynchrone sechs GL-Queries, Pose-Sanitizing, Source-/Camera-Epochen und echte Self-GPU-Draws untersucht.
3. Reine `SelfAvatarVisibilityPoseGate` eingeführt; speichert nur drei Koordinaten und normalisierte Quaternion, keine Avatar- oder Session-IDs.
4. Bei tatsächlich akkumuliertem Positionswechsel über 5 cm beziehungsweise Yaw/Pitch/Roll-Änderung über ungefähr zwei Grad wird alte GL-Beweisspur invalidiert. Unterhalb des Schwellwerts bleibt der gemessene Zustand stabil, aber kleine Schritte akkumulieren gegen den letzten Invalidierungspunkt.
5. Quaternionen `q` und `-q` werden als identische Rotation behandelt. Ein Self-Verlust/Logout wird erkannt, die nächste Anwesenheit gilt als neue Query-Quelle.
6. Den Pose-Gate im echten `set_avatar_proxies()` nach der Validierung der vom Simulator übernommenen Position/Rotation verbunden. Source- und Pose-Vergleich werden **beide** aufgerufen, kein Kurzschluss, danach höchstens ein Query-Reset.
7. Nach erfolgreichem GPU-Upload originaler Head-/Upper-/Lower-Meshes (Slots 0–2) oder Self-COF-Shape-Morph (Slots 6/8/9) alte GPU-Query-Ergebnisse verwerfen. Defekte Uploads verändern diesen Status nicht; Hair/Eyes/Eyelashes sind keine Head/Upper/Lower-Depth-Subjects.
8. Eine geänderte Renderer-Viewport-Größe setzt alte GPU-Ergebnisse zurück; das erneute Setzen derselben Größe ausdrücklich nicht.
9. Eigenständiger nativer C++-Test überprüft Jitter, kumulative Bewegung, Quaternionsignatur, Rotation und Logout. Der echte Xvfb-OpenGL-Regionstest prüft bei feststehender Kamera neue Position, Rotation, Upper-Mesh-Upload, Fensterresize, stabile erneute Abfragen und unveränderten Resize. Sieben Python-Verträge prüfen den konkreten C++-Pfad.
10. Vollständige Linux-Mint-x64-Build-, CTest-, Python-, Firestorm-Xvfb-OpenGL-/GUI- sowie X11-Systemicon-Abnahme; Projektplan, Roadmap, Status-JSON und Chatübergabe aktualisieren. CI des neuen GitHub-Commits anschließend separat prüfen.

## Sicherheits-/Performance-Grenzen und reale Live-Abnahme

Es wird **nicht** auf jedem Frame zurückgesetzt, sofern Position, Rotation, Bake-Identität, Körpermesh und Viewport unterhalb der tatsächlichen Änderungsgrenzen bleiben. Während echter Bewegung darf die aktuelle Depth-Abnahme vorübergehend `unknown` anzeigen: Das ist korrekter als ein alter, angeblich gültiger Sichtbarkeitsnachweis. Diese Maßnahme behauptet keine aktuelle RGB-Skin und ersetzt weder die COF-Wearable-Verarbeitung noch die echten Grid-Head-/Upper-/Lower-Bakes. Bewegte Skelettanimation ohne veränderte Avatar-Weltpose bleibt eine gesonderte Frage zur Aktualität zeitgebundener GPU-Queries.

Nach einem authentifizierten NexVerse-Login müssen `Avatar SELF bake pipeline`, aktuelles COF-Outfit, Head-/Upper-/Lower-Texturen, Mesh-/GetTexture-Ereignisse, WASD/Flug nach Teleport und die wirkliche angemeldete Shutdown-Dauer ausgewertet werden. Alte Testläufe ohne Login liefern dafür keine ausreichende Evidenz.

Upstream-Prüfung `opengenesislink/opengenesislinkserver`: letzter geprüfter Commit `59576aa062be3aa9edb3ba716e28106e4677f905` behandelt Stadtportal-/WorldAPI-Passwortreset-Dokumentation und enthält für diesen GL-Client-Fix keine erkennbare Protokollreferenz.

**Zielversion `v0.4.0.0` IN_ARBEIT; Projektfortschritt unverändert 38,65 % (39 % gerundet). Kein `main`-Merge, kein Release/Deployment.**
