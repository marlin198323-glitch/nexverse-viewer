# Alpha.805–814 – Tatsächlich gebundene Avatar-Skin-IDs statt bloßer GL-Zähler

22.10.2026 | Linux Mint Desktop Commander jam-MS-7D28 | develop-current
Version 0.2.0-alpha.814. Gewichteter Projektfortschritt 39 %.

## Zehn Entwicklungsblöcke
805. Aktuellen Mint-Checkout, sauberen Branch, GitHub-SHA 927da8e sowie zuerst CHATUEBERGABE.md und projectplan.md gelesen. Pi5 CI der Alpha.804 Code-SHA 3f6483b war erfolgreich (Run 39829308631).
806. OpenGenesisLINK-Servermain 7396897d erneut geprüft; keine neueren Avatar-/COF-/Skin-Protokolländerungen.
807. Realen Fehler in der F3-Avatar-Skin-Anzeige festgestellt: Drei vorhandene Low158-Bake-UUIDs, GPU-Residency und drei originale Firestorm-GL-Draws wurden gemeinsam als aktuelle Skin gewertet, selbst wenn die gezeichneten Texturen von einer älteren Outfit-/ObjTE-Version stammten.
808. RenderDiagnostics für die echten Head-, Upper- und Lower-.llm-Meshes um die exakt in diesem Frame tatsächlich per OpenGL gebundenen GPU-Textur-UUIDs erweitert. Fehlt eine gebundene Textur, bleibt die jeweilige ID leer; alle drei IDs werden pro Frame zurückgesetzt.
809. AvatarPipelineProof fordert nun für positiven Current-Low158-Skin-Status nicht nur je einen GL-Draw, sondern auch eine Übereinstimmung aller drei aktuellen Low158-TE-Bake-IDs mit den tatsächlichen Render-IDs.
810. Dieselbe Quellenzuordnung wird für vorläufige, authentifizierte Self-ObjectUpdate-TE-Bakes gesondert verwendet; deren GL-Pixel sind weiterhin ausdrücklich kein Low158-/COF-Nachweis.
811. F3-Skin-Anzeige unterscheidet echte unvollständige GL-Submissions von einem gezeichneten, aber veralteten Bake-Texturstand. Letzterer erzeugt eine Warnung statt einer positiven Skin-Anzeige.
812. Native C++23-Regression für den eigentlichen AvatarPipelineProof und provisional ObjectTE sowie echten Firestorm-OpenGL-Framebuffer mit verschiedenen Head-/Upper-/Lower-Assets ergänzt. Alte/abweichende UUIDs dürfen den Skin-Status nicht auf grün setzen.
813. Vollständigen Mint-x64-Viewer inklusive neuem Renderer-Diagnostikvertrag, 176 native CTests, Python-Verträge, isoliertem Xvfb-GUI-Start und OGL-Taskleistenicon validieren. Dies ist keine echte Anmeldung im Grid.
814. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS.md, Fortschritt-JSON und Server-/Firestorm-/UI-Paritätsmatrizen auf Alpha.814 aktualisieren. Nach grünem Gate auf GitHub develop-current pushen und SHA-genaue Pi5-/Hosted-CI gesondert dokumentieren. Kein Deployment.

## Offene P0-Abnahme
Nachweisbar ist die Renderer-Herkunft der verwendeten Skin-UUIDs, nicht ein tatsächlich eingeloggter NexVerse-Avatar. Low158 AvatarAppearance, COF-Outfit, authentifizierte Skin-Downloads, NVIDIA-Inworld-Pixel und serverbestätigte WASD-/Yaw-Bewegung bleiben live **offen**. Gesamtprojekt weiterhin 39 %. Nächster Zehnerblock Alpha.815–824.

## Vollstaendige lokale Freigabepruefung
Mint script `scripts/mint-viewer-validation.sh` EXIT 0: voller Linux-x64 Debug Viewer, 176/176 native CTests, 396/396 Python, Firestorm Original-LLM Body GPU Textur-UUIDs, isolierter Xvfb GUI-Start Viewer 0.2.0-alpha.814 mit 133 Bind-Joints, drei GPU-Bodymeshes (7186 Dreiecke) und OGL Systemicon 128x128. Keine echte Grid-Session.

## GitHub Code-SHA und CI
Feature-SHA f2b809453bd68c0e7407569a25b627362010dc17. Pi5 Source Check Run 39833603079 COMPLETED/SUCCESS beim Abruf; noch nicht grün. Hosted Linux-x64 Run 39833603059, Job 120015989833: FAILURE vor Runnerallokation (runner_id 0, 0 Steps), kein Compiler gestartet. Voriger Pi5 Alpha.804 Run 39829308631 zur SHA 3f6483b SUCCESS. Der lokale Linux Mint Gate war EXIT 0 mit 176 CTests, 396 Python, Xvfb GUI und OGL X11-Icon. Kein Live-Login.
