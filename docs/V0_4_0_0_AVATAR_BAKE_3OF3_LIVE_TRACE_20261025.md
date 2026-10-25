# OGL v0.4.0.0 – Low158 Head/Upper/Lower 3/3 Beweis und Live-Stufenprotokoll

**25.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current` | Ausgangscommit `192a0d691bf1789fd971dc4306bfe4339f37f582`**

## Gesicherter Ausgangsbefund

Im Log vom 25.10.2026 15:40 wurde `AvatarAppearance SELF` vor der Circuit-Bereitschaft gepuffert und danach übernommen. Im späteren lokalen Testlauf um 20:02 wurden **keine** neuen AvatarAppearance-/Bake-Daten eines authentifizierten Loginversuchs empfangen. Der lokale Shutdown betrug dabei 26 ms, nicht belastbar für einen angemeldeten Viewer.

Der bisherige `avatar_skin_blocker_key()` meldete die Bake-Draw-Stufe als erreicht, sobald der aggregierte GL-Zähler mindestens drei Zeichenaufrufe zeigte. Ein mehrfach gezeichneter Kopf konnte dadurch fälschlich als vollständiger Head-/Upper-/Lower-Nachweis erscheinen.

## Zehn abgeschlossene Bearbeitungsblöcke

1. Linux-Mint-Checkout und GitHub `develop-current` mit `CHATUEBERGABE.md`, Roadmap und Versionsstand abgeglichen; Ausgangsbaum sauber.
2. Vorhandene Login-/CAPS-/Avatar-/Textur-/Shutdown-Logs datensparsam ausgewertet; aktuelle lokale Tests von echtem Grid-Login unterschieden.
3. `AvatarPipelineProof.current_body_bake_sources_drawn` mit drei tatsächlichen originalen Firestorm-LLM-Körper-Zeichenaufrufen korreliert.
4. Neue `AvatarDiagnosticReport.self_low158_current_body_gl_drawn[3]` erfasst pro Kopf/Oberkörper/Unterkörper nur aktuelle, passend zur authentifizierten Low158-Textur gezeichnete GL-Slots.
5. Blockerregel korrigiert: **Nur wenn alle drei aktuellen Körper-Slots wirklich gezeichnet wurden**, ist der GL-Teil vollständig. Summierte Draw-Calls allein reichen nicht.
6. Bestehenden privaten Benutzerdiagnosebericht um die drei einzelnen Low158-GL-Statusfelder erweitert; ohne Asset-IDs, Identität oder CAPS-URLs.
7. Neuen `AvatarBakeProgressTrace` implementiert: Wird nur bei gültiger authentifizierter Sitzung mit aktiver Simulatorverbindung und aktuellem Regionsepoch aktiv.
8. Stufenfortschritt als kurze, privacy-sichere Viewer-INFO-Meldung: erster Zustand, relevante Statusänderungen, höchstens ein ungelöster Status alle 30 Sekunden und mindestens 250 ms zwischen fluktuierenden Zustandswechseln; Regionwechsel und Logout resetten die History.
9. Native Regressionen `nexverse-avatar-diagnostic-report-tests` (acht Head-Draws sind keine 3/3) und `nexverse-avatar-bake-progress-trace-tests` (Stufen, Zeitbegrenzung, Epochengrenzen, terminaler Status, keine Geheimnisse) ergänzt. Zusätzlich sechs neue Python-Vertragsprüfungen.
10. Vollständigen lokalen Mint-x64-CMake-/CTest-/Python-/Firestorm-Xvfb-OpenGL-GUI-/X11-Icon-Gate ausgeführt; die exakten Testzahlen sind im Fortschrittsbericht erfasst.

## Logausgabe nach nächstem echten Grid-Login

Die neue Zeile beginnt mit `Avatar SELF bake pipeline: blocker=` und enthält **keine Avatar-UUIDs, Asset-UUIDs, Session-IDs oder URLs**:

`low158_refs=n/3; low158_gettexture=H,U,L; selected_source=H,U,L; selected_gettexture=H,U,L; current_low158_gl=H,U,L; gpu_body=n/3; depth_known=0|1; depth_passed=0|1; provisional_objte=0|1; final_skin_rgb_unverified`

`GetTexture`-Stufen: 0 keine Referenz, 1 GPU geladen, 2 ladbar, 3 HTTP404/410, 4 vorübergehend, 5 Konfiguration, 6 ungültige Daten. `selected_source`: 0 keine, 1 echte Low158, 2 nur vorläufige `ObjectUpdate`-TE. `current_low158_gl=1,1,1` verlangt alle drei **aktuellen** Texturquellen auf den jeweiligen originalen GL-Körpermeshes. `final_skin_rgb_unverified` erinnert daran, dass ein bestandener Tiefentest weiterhin **kein** Pixel-Readback einer sichtbaren korrekten Skin/COF-Belegung ist.

## Abnahme und Restrisiken

Die Entwicklung verbessert die Tatsachentreue der Diagnostik und ermöglicht die Eingrenzung der Avatar-/Outfitfehler im nächsten **echten** Login. Sie ersetzt keine Head-/Upper-/Lower-Textur oder Server-COF-Daten, erzeugt keine Bakes und ändert nicht das historische Firestorm-Rendering. Eine erfolgreiche vollständige 3/3-Server-/GPU-/RGB-Abnahme ist daher weiterhin **offen**. Ebenso offen bleiben live WASD/Flug nach Teleport und echte angemeldete Shutdown-Dauer.

**Version v0.4.0.0 IN_ARBEIT, Gesamtprojekt 38,65 % (39 %), kein Release auf main.**
