# Alpha.675–684 – TOS-Zustimmung, Login-Retry, Tastaturbedienung und echter x64-Viewer-Build

21.10.2026 | develop-current | OpenGenesisLINK Viewer 0.2.0-alpha.684
Gesamtfortschritt weiterhin 39 % (38,65 % gewichtet).

## Upstream und Build-Ausgangslage
Alpha.674 GitHub Actions Run 39645564317, exakter Feature-SHA 24fb21c4c841e17bc2e399c219a6261a9eecdb8a: COMPLETED/SUCCESS. Vorhandene develop-current CI baute bisher nur ARM64 C++23-Headless-Teile; CMake/Vcpkg/Linux-x64-OpenGL-UI wurde auf diesem Branch NICHT tatsächlich kompiliert. Der Pi5 selbst hat kein installiertes cmake/tinyxml2/GLFW/vcpkg Toolchain-Set für den grafischen Gesamtbuild. Darum jetzt ausdrücklich eine separate gehostete Ubuntu-24.04-Workflow-Strecke für den vollständigen Viewer und die echten LoginService/XMLRPC-Tests.

Upstream Server opengenesislink/opengenesislinkserver main 3728273e5662cef5280af6cff202108c74fb0aa0: gezielter Compare seit 7569838fd125d2338bb0a57ec88476648610d887 zeigt genau 1 Commit, Korrektur der OGLSeasons Mono.Addins-Modulregistrierung in addon/CI/README, KEIN belegter neuer Viewer-LLUDP- oder Login-TOS-Vertrag. Historischer älterer Upstream-Review-Rückstand bleibt offen.

## Zehn reale Arbeitsblöcke
675. Aktuelles GitHub develop-current, CHATUEBERGABE.md, Projektplan, sauberen Pi5-Checkout und Alpha.674 SHA-CI geprüft; CI für Alpha.674 tatsächlich erfolgreich abgeschlossen.
676. Genaues neues OpenGenesisLINK Servermain 3728273 geprüft, neue OGLSeasons-Änderung als serverseitigen Modulfix eingeordnet; TOS-Serververtrag blieb gegenüber 7569838 unverändert.
677. P0-Zustandsproblem im TOS-Retry behoben: nach jeder Nicht-TOS-Anmeldeablehnung oder Login-Worker-Ausnahme wird eine zuvor zugestimmte TOS-Freigabe vernichtet, damit kein späterer Login-Versuch automatisch agree_to_tos=true erbt.
678. Login-Future-Request bindet TOS-Challenge an den zum Sendezeitpunkt tatsächlich verwendeten Avatar und LoginURI; mutable Login-UI/GridInfo-Refresh können keine andere Identität/URL in den Zustimmungszustand einschleusen. Alte Freigaben bei Scope-Wechsel zurückgesetzt.
679. Echte Tastaturnavigation im Login-TOS-Modal ergänzt: Tab, Shift+Tab, Enter, Numpad-Enter, Leertaste und Escape. Eine noch gesperrte Zustimmung wird im Fokusumlauf übersprungen; keine Tastatureingabe an versteckte Anmeldefelder.
680. Im realen OpenGL-UI-Renderpfad fokussierte Bürgerordnung-/Zustimmen-/Ablehnen-Schaltflächen sichtbar cyan umrandet. Zustand nach Öffnen des Dokuments fokussiert direkt die ausdrückliche Zustimmung; Maus und Keyboard nutzen dieselbe Aktion.
681. Fehler in globaler ESC-Priorität aufgedeckt und behoben: Modal-ESC muss vor GLFWs Schließen-des-gesamten-Viewers-Behandlung liegen. Die Ablehnung bleibt an dieser Stelle explizit ohne Session/Auto-Login.
682. Externer offizieller Browserstart über posix_spawnp wird per Hintergrund-waitpid bereinigt, um dauerhafte Zombie-Child-Prozesse zu verhindern. Hinweistext behauptet nur gestarteten Browseraufruf, nicht tatsächliches Lesen. Windows ShellExecuteA bekommt die benötigte shell32-Library im CMake-Target.
683. Neues GitHub Actions Linux x64 Full Viewer CMake Workflow für develop-current: Ubuntu-24.04 + GL/X11/WebKitGTK/Vcpkg, echter nexverse-viewer + Login XML-RPC-CMake-Tests, kein Deploy. Neue native Keyboard-Regression, Python-Tests zu Zustimmungs-Lebensdauer, Bedienung, ESC, Kindprozess und GUI-Buildvertrag; vorherige Regressionen behalten.
684. Version, README-/Projektplan-/Handoff-/Firestorm- und Server-Kompatibilitätsmatrix sowie Gewichtungsdaten aktualisiert. Native Pi5-Regression und kompletter Python-Satz erneut ausführen, SHA-genauen CI-Gesamtstatus getrennt verifizieren. Kein Produktivdeployment.

## Verifizierung und offene E2E-Prüfungen
Diese Änderungen sind überprüfbare Code- und Workflow-Arbeiten. Die neue Linux-x64-GUI-CI muss für den neuen Code-SHA erst real durchlaufen und kann auch Compiler-/Dependencyfehler aufdecken. Ein grüner Pi5-Headless-Lauf allein würde keinen erfolgreichen GUI-Build beweisen. Selbst ein Linux-x64-Build ist noch kein grafischer Screenshot/Xvfb-Funktionstest und keine echte NexVerse-Grid-Anmeldung.

Kritische End-to-End-Tests weiterhin offen: produktiver versionierter Erstlogin mit echter Bürgerordnung und Zustimmungsspeicherung; x64-GUI-Screenshot, Systemicon; AvatarAppearance/COF/Outfit/Skin/Mesh im Renderframe; AgentUpdate/WASD und Serverreadback. Firestorm-Parität unvollständig. Nächster Zehnerblock Alpha.685–694: Linux-GUI-CI auswerten, Buildfehler beheben, echte Grid-/Avatar-Funktionen priorisieren. Der Fortschritt wird nicht ohne Inworld-E2E-Evidenz künstlich erhöht.
