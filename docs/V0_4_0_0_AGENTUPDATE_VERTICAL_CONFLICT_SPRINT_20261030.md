# OGL Viewer v0.4.0.0 – Echte High4-AgentUpdate-Steuerbits: Auf/Ab-Konflikte und Sprintachsen

**30.10.2026 | Linux Mint `jam-MS-7D28` | GitHub-Branch `develop-current` | Ausgangscommit `57040429a714323de7f9582334fcd6d0ab65414e`.**

## Festgestellter funktionaler Fehler

Bei gleichzeitig gedrückter Leertaste (Springen/Steigen) und C (Sinken) sendete der Firestorm-basierte OGL-Viewer in der **tatsächlich serialisierten LLUDP High4 AgentUpdate** beide widersprüchlichen Richtungsflags `control_up_pos` und `control_up_neg`. Die WASD-Achsen und die Links-/Rechts-Drehung waren bereits gegen gegensätzliche Tasten abgesichert, die vertikale Achse jedoch noch nicht.

Zudem aktivierte Shift (`keys.sprint`) immer gleichzeitig `control_fast_at`, `control_fast_left` und `control_fast_up`, unabhängig von tatsächlichen Richtungswünschen. So gingen selbst beim bloßen Drehen oder Stillstand Beschleunigungsflags mit, obwohl keine dazugehörige Bewegung angefordert war. Das ist unnötiger und teilweise widersprüchlicher Simulatorinput; ein nachgewiesener Einfluss auf konkrete NexVerse-Live-Bewegungen ist noch offen.

## Zehn Entwicklungs- und Verifikationsblöcke

1. Aktiven Linux-Mint-Commander, sauberen lokalen Git-Branch und `CHATUEBERGABE.md` zuerst geprüft; `origin/develop-current` identisch.
2. Beide GitHub-CI-Workflows des letzten GetMesh-Commits `57040429` geprüft: Source Check und vollständiger Linux-x64-Build erfolgreich.
3. Neueste lokale Viewer-Logs geprüft: Die aktuellen 17-Uhr-Läufe enthalten keine authentifizierte `AgentMovementComplete`-Sitzung oder einen echten WASD-/Flug-Readback. Keine erfundene Live-Abnahme.
4. `MovementController`, GLFW-Tasteneingaben, gesicherte LLUDP-Queue, `encode_agent_update`, die echten High4-ControlFlags im Wirepayload und den socketseitigen Sendebeleg verglichen.
5. Den Vertikalkonflikt korrigiert: `move_up = jump && !descend`, `move_down = descend && !jump`. Gleichzeitiges Space+C erzeugt keine zueinander widersprüchlichen Höhenflags. Das Freigeben einer Taste aktiviert die andere ohne Neustart.
6. Shift-`control_fast_at` nur bei W/S, `control_fast_left` nur bei A/D und `control_fast_up` nur bei Space/C verwenden; stillstehende und ausschließlich drehende Steuerung sendet keine Fast-Movement-Flags. Diagonale Bewegung behält die jeweils passenden Beschleunigungsflags.
7. Native `movement_controls_tests.cpp` um die **wirklich serialisierten High4-Wirebits** für Space allein, C allein, Space+C, Shift+W, Shift+A, Shift+Space/C, Shift+W+A+Space, reine Shift- und Drehzustände ergänzt.
8. Native Sequenztests für das Auflösen der Space+C-Kombination und die Flug-F-Toggle-Kante ergänzt. Flug bleibt aktiv, selbst wenn Space+C neutralisiert wird; nach Loslassen einer Taste wird die entsprechende Vertikalrichtung plus `FAST_UP` korrekt gesendet.
9. `VerticalMovementFeedbackMonitor`-Regression erweitert: Ein Widerspruch von Auf/Ab darf trotz nachfolgender Höhenkorrektur keinen vermeintlich bestätigten Sprung erzeugen; erst der nächste eindeutig versendete UP-Command kann durch eine tatsächliche Simulatorhöhenänderung bestätigt werden. Neun Python-Vertragstests für Source-/High4-Wire-Verkabelung, Flugsteuerung, Feedback und Security ergänzt.
10. Gesamtes Linux-Mint-Gate einschließlich CMake/C++/CTest, Python, originalem Firestorm-Xvfb-/OpenGL-/GUI-Test und X11-Systemicon durchlaufen; Projektplan, Roadmap, Chatübergabe und Fortschritts-JSON synchronisieren; anschließend nach `develop-current` committen und GitHub-CI kontrollieren.

## Unverändert und noch offen

Die Änderungen verändern **keine** Serverposition, Kamera oder Avatarrotation künstlich und behaupten weder zuverlässige LLUDP-ACKs für AgentUpdate noch bestätigte Bewegung. Das bestehende `MovementFeedbackMonitor`-Kriterium von mindestens 0,25 m Simulatorbewegung, das vertikale Readback und die Region-/Session-Sicherungen bleiben erhalten.

**Authentifizierte Live-Grid-Abnahme weiter offen:** W/S/A/D, Drehung, Space/C, Shift, F-Flug und deren tatsächliche Serverpositions-/Höhenrückmeldung, Teleport/Regionswechsel, dreifache Head-/Upper-/Lower-Skin-RGB-Bakes, COF/Kleidung sowie die Shutdown-Dauer nach Login. Die Welcome-Website und das kompakte Login-Design wurden nicht verändert.

Upstream am 30.10.2026: OpenGenesisLINK-Server SHA `08bf15f306bb4f882f24f32b84eb8df3a3175cea`, Login-Zeitzonenänderung ohne Bezug zum High4-Bitlayout.

**Zielversion `v0.4.0.0` IN ARBEIT. Projektfortschritt 38,65 % gewichtet (39 % gerundet), kein `main`-Merge, kein Release oder Produktivdeploy.**
