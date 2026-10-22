# Alpha.765–774 – COF-Rückfallschutz für wechselnde AppearanceVersion

22.10.2026 | Linux Mint Desktop Commander jam-MS-7D28
Entwicklungsbranch develop-current | Viewer 0.2.0-alpha.774
Gewichteter Entwicklungsfortschritt 38,65 % = 39 %.

## Umgebung und Stand
Linux Mint PC (jam-MS-7D28) primärer Arbeits- und Grafiktestrechner.
Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer.
Raspberry Pi 5 ausschließlich ARM64-GitHub-CI.
Sauberer origin-gleicher Checkout zu Beginn: SHA
7b5d0ed9274c48bde8f429714730ba8167c5401b.
CHATUEBERGABE.md und projectplan.md vor Änderungen gelesen.
SHA-genaue Pi5-CI für vorherigen Alpha.764-Code f2c53f3:
Run 39814357748 completed/success.
Hosted x64 GitHub CI Run 39814357774 failure vor Runner-Zuteilung,
Job 119948820858 runner_id=0, steps=0; kein OGL-Compilerfehler.
Lokales Mint x64-Gate für Alpha.764 PASS.

## Zehn konkrete Arbeitsblöcke
765. GitHub Branch/Commit mit Linux-Mint-Checkout verifiziert,
Handoff/Projektplan gelesen und vorherigen Pi5-Run exakt
gegen Feature-SHA überprüft. Kein fremdes Repo verändert.

766. Neuen OpenGenesisLINK-Servermain
bd6bf4f28cbf72c9ba01033b481db9dc1e68c4ce
mit vorherigem bcb528ac verglichen. Exakt zwei neue Commits
#273 Gruppen-JWT-HTTP-SQL-Autorisierung und #274
Firestorm-Gruppenland-/NexDollar-Verlauf.
Neue Client-Funktionen daraus nicht erfunden; kein
neuer AvatarAppearance-COF-Vertrag aus den Diffs abgeleitet.

767. ECHTEN Cross-Format-Low158-Skin-Replay-Fehler ermittelt:
WorldModel hielt COF-Highwater nur für die zuletzt
empfangene AppearanceVersion. Ein Wechsel 2 => 3 => 2
ließ verspätete ältere Version-2-COF-Pakete akzeptieren.
Das konnte jüngere authentifizierte Skin-Texturen ersetzen.

768. In include/nexverse/world/world_model.hpp
ein festes, begrenztes uint32[256]-Highwater je
AppearanceVersion ergänzt. WorldModel::apply_self_appearance
prüft den aktuellen AppearanceVersion-COF-Wert gegen
dessen bisherigen Formatwert; Gleichstände bleiben
zulässig, ältere Werte werden abgewiesen.
Das frühere letzte-Format-Watermark bleibt kompatibel;
keine Änderungen an den Simulator-Nachrichten.

769. Die neue Tabelle wird bei genau denselben vier
bestehenden echten Sitzungs-/Regions-/Logout-Resets
geleert wie die bisherigen Single-Format-Watermarks.
Legacy AppearanceData count=0 und nur COF=0
aktualisieren keine Versionsobergrenzen.

770. Native tests/self_appearance_texture_integrity_tests.cpp
mit alternierenden Format-2/3-Revisionsfolgen,
COF 900/901/902 und 10/11 ergänzt. Verspätete Version-2-
und Version-3-COF-Retransmits werden unabhängig
vom zuletzt verwendeten anderen Format abgewiesen.

771. Echte LLUDP Low158-Wire-Regressions in
tests/avatar_appearance_large_attachment_wire_tests.cpp:
synthetisch erzeugte, aber vollständig nach OpenSim
kodierte AvatarAppearance-Pakete mit zwei
AppearanceData-Zeilen (COF + trailing COF=0),
AppearanceVersion 2/3 und echten UDP Message IDs
werden über den Standarddecoder in WorldModel eingespeist.
Schutz trotz wechselnder Formate getestet; Legacy-Kompatibilität
weiterhin unter derselben CTest-Regressionssuite.

772. Tatsächlichen OpenGL-Framebuffer-Test
tests/avatar_body_framebuffer_pixels_tests.cpp
mit echten drei Firestorm Original-LLM-Meshes erweitert:
eine andere AppearanceVersion darf den Skin aktualisieren,
ein später eintreffender alter COF-Wert des vorherigen
Formats darf die sichtbaren Body-Pixel nicht mehr zurückrollen.
Gezielter Xvfb-GL-Test bestanden.

773. Python-Vertrag
tests/test_alpha765_774_cof_format_replay_contract.py
für echte Datenstruktur, vier Resetpfade,
Low158-Wiretest und Firestorm-GL-Pixeltest hinzugefügt.
Vollständigen Mint-x64-Gate mit sämtlichen nativen
CTest- und Python-Prüfungen, isoliertem Xvfb-GUI-Start
und OGL-Taskbar-Icon ausführen.

774. VERSION, Chatübergabe, projektplan.md, AGENTS,
docs/PROJEKTFORTSCHRITT.md und Paritätsmatrizen,
Fortschritt JSON auf Alpha.774 aktualisieren. Nach
erfolgreichem lokalen Linux-Mint-Release-Gate
direkt auf GitHub develop-current pushen und
exakte Feature-SHA-/CI-Ergebnisse dokumentieren.
Kein Produktivdeploy und kein unaufgeforderter
Start im interaktiven Linux-Desktop.

## Nicht nachgewiesen
Das authentifizierte NexVerse Login/Live-Avatar-E2E
inklusive Low158 AvatarAppearance, COF-/Outfit-Bakes,
tatsächlich gerenderter NVIDIA-Inworld-Skin sowie
serverbestätigte WASD-/Yaw-Bewegung ist weiterhin
still open. GPU-Pixeltests verwenden kontrollierte
synthetische Low158-Testdaten, keine private Sitzung.
39 % gewichteter Entwicklungsstand unverändert.
Nächste zehn Blöcke Alpha.775–784 auf Linux Mint.

## Vollständig geprüfter Mint-x64-Release-Stand
scripts/mint-viewer-validation.sh EXIT 0: vollständiger
CMake/vcpkg Linux-x64-Debug-Viewer-Build,
175/175 native CTests inklusive WorldModel per-Format-COF,
vollständigem LLUDP Low158-Firestorm AppearanceData-Wire-Decoder
sowie originalem Firestorm Head/Upper/Lower-Framebuffer-Pixeltest;
381/381 Python-Tests, Xvfb Viewer 0.2.0-alpha.774,
133 Firestorm-Joints, drei original-GPU-residente LLM-Körpermesh-Teile
(7186 Dreiecke), X11 WM_CLASS und OGL-Systemicon 128x128 PASS.
Kein echtes eingeloggtes Grid-/WASD-/Skin-E2E.

## Exakter GitHub-Code-Commit und Folge-CI
Feature SHA a68ca53e8c9a669a9994ec4d1bf28b3e2a7dc975.
GitHub Pi5 Run 39824131466 IN_PROGRESS beim letzten Abruf.
Hosted Linux x64 Run 39824131896 COMPLETED/FAILURE;
Job 119982788458 runner_id 0, 0 ausgeführte Steps,
kein GitHub-hosted Compiler-/CMake-Lauf gestartet.
Voriger Pi5 Alpha.764 Feature-SHA f2c53f3
Run 39814357748 SUCCESS.
Alter Pi5 Alpha.754 Run 39812188923 wurde CANCELLED.
Getrennt davon Linux-Mint-x64-GUI-Volltest Exit 0:
175 native, 381 Python, isoliertes UI/Original LLML/Taskleisten-Icon.
Authentifizierte NexVerse-Live-Avatar-/Skin-/WASD-Beweise weiterhin OFFEN.
