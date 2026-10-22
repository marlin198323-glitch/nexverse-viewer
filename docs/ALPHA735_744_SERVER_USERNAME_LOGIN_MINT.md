# Alpha.735–744 – Echte einteilige Avatar-Namen für OGL Login (#268)

22.10.2026 | develop-current | Linux Mint Commander jam-MS-7D28
Viewer 0.2.0-alpha.744, gewichteter Projektfortschritt **38,65 % = 39 %**.

## Verbindliche Arbeitsumgebung
Primär: Linux Mint Desktop Commander jam-MS-7D28,
Pfad /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer,
Branch develop-current. Raspberry Pi nur ARM64-CI.
Beim Start sauberer HEAD 9325cbf19814d615726f87bc97834f4b6721ecfa,
gleich GitHub-HEAD. CHATUEBERGABE.md, projectplan.md zuerst gelesen.
GitHub-Pi5-CI Alpha.734 Run 39662405115 zur Code-SHA
a31201f2490e40041fa08fff6bc1c7fb8c725129: SUCCESS.
Erster Git-Fetch kurz aufgrund blockiertem Linux SSH-Agent fehl;
Wiederholung von ls-remote und fetch später erfolgreich.

## Zehn Entwicklungsblöcke
735. Lokalen Mint develop-current und GitHub-Head, CHATUEBERGABE.md,
Projektplan und SHA-genaue Pi5-CI überprüft, keine Nutzerdaten verändert.

736. OpenGenesisLINK Servermain 881b373738ce889f4965ca2478bdb455f1d827f2
gegen vorherigen 86f293d948390b2546a92f848eef5ee17ccdd626 verglichen:
9 neue Commits #265–271. Die Login-kritische Korrektur #268
in OpenSim LLLoginHandlers.cs unterstützt Firestorm-7.2.4-Login mit
XML-RPC username/passwd, statt nur first/last/passwd. Die
Server-Resident-Namensauflösung erfolgt serverseitig.
Weitere Serveränderungen bei Economy, Gruppen, Portal, Sicherheit
bleiben ohne erfundenen Viewer-Client-Vertrag.

737. Echten P0-Login-Blocker im OGL festgestellt:
split_avatar_name verwies Einwortnamen ohne Netzwerkversuch ab.
Gemeinsamen C++23 Parser parse_avatar_login_identity für
einteilige Avatarkontonamen und klassische First/Last-Namen
eingeführt. Leere/Whitespace/Steuerzeichen werden abgelehnt;
mehrteilige Legacy-Namen bleiben erhalten.

738. In der tatsächlichen GLFW-Anmeldung Einwortnamen als
LoginCredentials.account_username übergeben; klassische
First/Last-Aufteilung bleibt. Fehlermeldung verlangt keinen
künstlichen Nachnamen mehr. Keine Erfindung eines Resident-Suffixes.

739. LoginCredentials rückwärtskompatibel um optionales
account_username am Ende des bestehenden DTO ergänzt.
Das XML-RPC-Requestformat verwendet ENTWEDER genau username
ODER die bisherigen first/last Felder, niemals beides.
Passwort-Hash und Options bleiben unverändert.

740. LoginService prüft explizite Account- oder gültige Legacy-
Identität. Whitespace-only-Account und widersprüchliche
gleichzeitige Single-Account/First-Last-Identitäten abweisen;
Login-URI weiterhin ausschließlich aus GridInfo.

741. TOS-Consent auch beim Single-Name-Login mit default false
sichergestellt; true nur nach ausdrücklich bestätigter
Serverchallenge, Avatar- und Grid-Scope bleiben unverändert.
Aktueller /service/tos Browser-Link bleibt unangetastet.

742. Echte C++23 nexverse-login-tests erweitert:
ein- und mehrteilige Namen, Trimming, ungültige Eingaben,
Legacy- und neue XML-RPC-Form, Hash statt Klartext,
Consent false/true und gleichzeitige Identität ablehnen.

743. FakeTransport-Integration am echten LoginService prüft
neuen username POST bei Erfolg und server reason=tos.
Neue Python-Quell-/GUI-/Serializer-Verträge zusätzlich.
Vollständiger Linux Mint CMake/vcpkg x64 GUI Gate umfasst
native CTests, Python, Xvfb Viewer und X11 OGL-Taskbar-Icon.
Native Test ist noch KEIN authentifizierter Grid-Login.

744. VERSION, CHATUEBERGABE.md, projectplan.md, AGENTS,
Fortschritts-JSON, Server-/Firestorm-/UI-Matrizen und
Neuchat-Handoff aktualisieren; Code-SHA und GitHub CI
nach echtem Push exakt dokumentieren, kein Produktivdeploy.

## Grenzen und Folgephase
Diese Alpha löst den konkreten lokalen Accountnamen-Blocker,
belegt aber keine tatsächliche erfolgreiche NexVerse-Anmeldung.
Eigene Low158 AvatarAppearance, COF/Outfit/3x Skin-Bake-Pixel
und Simulator-WASD-/Yaw-Readback sind weiterhin still open.
Gesamtfortschritt 39 %. Nächste zehn Blöcke Alpha.745–754
auf Linux Mint, Raspberry Pi nur CI. Ältere nicht reviewte
Server-Commits bleiben MANUAL_REVIEW.

## Nachweisbare lokale Releaseprüfung
Vollständiger Linux Mint x64-Gate
scripts/mint-viewer-validation.sh: Exit 0.
CMake/vcpkg Debug Viewer erfolgreich;
**175 native CTests** einschließlich realem XML-RPC
Single-Name/TOS/Legacy-Login und OpenGL-Renderer,
**369 Python-Tests** erfolgreich, isolierter Xvfb GUI-Start
OpenGenesisLINK 0.2.0-alpha.744 (133 Firestorm
Skelettgelenke, 3 Originalkörpermeshes, 7186 Dreiecke)
sowie X11 WM_CLASS und OGL Taskleisten-Icon
128x128 bestanden. Keine interaktive Desktopanmeldung,
keine realen Avatarpasswörter, kein Produktivdeploy.

## Nach Code-Push: SHA-genaue CI-Beobachtung

Feature-Commit cbdfd857c00a6d9c3c897a5c2f3441223d080cf9.
GitHub Pi5 Run 39809196307 IN_PROGRESS bei letzter Prüfung.
Gehosteter GitHub-x64-Run 39809196408 COMPLETED/FAILURE,
Job 119930505086 mit runner_id 0 und 0 ausgeführten Schritten:
Weder Konfiguration noch Viewer-Compiler auf Hosted-Runner
gestartet. Vollständiger Linux-Mint-CMake/175 CTest/369 Python/
Xvfb-GUI/Taskleisten-Icon dagegen lokal nachweislich erfolgreich.
Keine echte authentifizierte Grid-Avatar-Sitzung durchgeführt.
