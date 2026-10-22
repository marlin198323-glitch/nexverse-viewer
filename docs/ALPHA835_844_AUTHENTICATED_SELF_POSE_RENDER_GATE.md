# Alpha.835–844 – Selbstavatar-Rendern nach echter Simulatorposition

22.10.2026 | Linux Mint Desktop Commander jam-MS-7D28
Branch develop-current | OGL 0.2.0-alpha.844 | Fortschritt 39 %.

## Zehn Entwicklungsblöcke
835. Sauberen Linux Mint develop-current HEAD 1edf752,
CHATUEBERGABE.md und projectplan.md geprüft. Vorige
Pi5 CI Alpha.834 SHA fa2f51d Run 39838069587 SUCCESS.

836. Aktuellen OpenGenesisLINK Servermain 7396897d überprüft,
keine neuen Low158-/AgentMovementComplete-Vertragsänderungen.

837. Realen Render-Autoritätsfehler gefunden: die Anwendung setzt
AgentState.present mit einem aktiven LLUDP-Circuit bereits bevor
ein Simulator eine echte Agentposition geschickt hat.
Damit konnten Original-Firestorm-Meshes am vorläufigen Login-
Origin angezeigt und als eigener Avatar interpretiert werden.

838. WorldModel::accept_simulator_self_pose prüft nun
Agent-ID, Session-ID, Regionshandle und gültige
Simulatorposition/Quaternion, bevor ein Körper als
renderbar gilt. self_avatar_renderable verlangt eine
aktive Szene UND die serverbestätigte Eigenposition.

839. AgentMovementComplete aktualisiert den authentifizierten
Pose-Nachweis. Die Nachricht darf vor circuit.connected
eintreffen, erst ein aktiver Circuit macht daraus eine
darstellbare Eigenavatarposition.

840. Full ObjectUpdate, Compressed ObjectUpdate und
ImprovedTerseObjectUpdate speisen denselben Model-Gate.
High9 benötigt zuvor die aktuelle gültige Selbstavatar-
LocalID im aktuellen Regionhandle; unsent WASD/Keyboard-
Eingaben können keine Simulationposition bestätigen.

841. Eigenes Original-Firestorm-LLM-Renderproxy wird erst
nach self_avatar_renderable aufgebaut. Terrain, andere
echte Avatarobjekte und WorldGeometry bleiben separat
von der Selbstavatarpositionsbestätigung.

842. F3-Avatarstatus und datensparsamer Diagnosebericht
melden bei aktivem Circuit ohne echte Simulatorposition
weiterhin AgentPosition fehlt statt den Körper als
bestätigt sichtbar anzugeben.

843. Native C++ WorldModel-Regression beweist fehlenden
Avatar trotz SceneReady, Zurückweisung fremder IDs und
ungültiger Quaternionen/Koordinaten, bestätigte Pose,
eigenes KillObject, Regionswechsel, Verbindungsverlust,
Reconnect, Sessionwechsel und Logout. Test in CMake
und Pi5 ARM64-CI registriert; Python prüft sämtliche
echten Handler und F3/Renderer-Verdrahtung.

844. VERSION und zentrale Projektpläne/Chatübergabe,
Fortschritt-JSON, Firestorm-/Server-/UI-Matrizen
aktualisieren. Vollständiger Mint x64 CMake/CTest/
Python/Xvfb-GUI-Gate vor Feature-Push nach GitHub;
exakte SHA und CI-Ergebnisse anschließend dokumentieren.

## Grenze
Nachgewiesen ist die richtige Zuordnung der eigenen
Körperposition zum echten Simulator. Ein authentifizierter
NexVerse Live-Login mit tatsächlich sichtbar geladenem
Outfit, Low158/COF/Skin und serverbestätigten WASD-/
Yaw-Daten ist damit noch nicht belegt.
Projekt 39 %, nächste Alpha.845–854, kein Deploy.

## Vollständig bestandene Mint-x64-Releaseprüfung
scripts/mint-viewer-validation.sh EXIT 0:
kompletter Linux-x64 CMake/vcpkg Debug Viewer,
177/177 native CTests (neuer echter WorldModel
self_avatar_pose_authority Test eingeschlossen),
408/408 Python-Tests, isolierter Xvfb-Viewer
Version 0.2.0-alpha.844 mit 133 Original Firestorm
Bindpose-Joints und drei GPU Original .llm-Körpermeshes,
7186 Dreiecke, korrektes WM_CLASS und OGL X11-Systemicon
128x128. Kein Live-NexVerse-Login und kein Produktivdeploy.
