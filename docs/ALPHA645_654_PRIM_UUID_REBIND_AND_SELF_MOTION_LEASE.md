# Alpha.645–654: Prim-LocalID/UUID und Self-Movement-Lebensdauer

21.10.2026; develop-current; OGL 0.2.0-alpha.654; Projekt 39 % (38,65 % gewichtet).

## Zehn reale Entwicklungsblöcke
645. Nach GitHub-Sync auf d669213 die aktuelle Übergabe, Tests und Server-Stand geprüft.
646. Weltmodell-Bug entdeckt: eine neu belegte Prim-LocalID mit neuer UUID überschrieb bisher nur den Root, ohne alte Child-/Grandchild-Meshes abzubauen.
647. Upsert ersetzt bei einer UUID-Kollision den vollständigen alten Linkset-Unterbaum vor der neuen Prim-Quelle.
648. Gleiche UUID hält Nachfahren. LocalID 0, unbekannte UUID oder unverbundener Circuit können keine Renderobjekte einsetzen.
649. Neuer eigener saturierender Zähler für implizite Prim-Rebinds, zurückgesetzt bei Auth-/Session-/Regionswechsel, getrennt von echtem UDP-KillObject.
650. F7-Region-Floater zeigt getrennte Avatar-/Prim-Identitätswiederverwendung ohne vertrauliche UUIDs.
651. Ein empfangenes KillObject für den aktuell eigenen Avatar entwertet alle bisherigen WASD- und Yaw-Erfolgsanzeigen.
652. Ein neues Self Full- oder Compressed ObjectUpdate mit anderer LocalID entwertet frühere Bewegungs- und Turn-Belege ebenfalls.
653. Native C++23 Regressionen für Root/Child/Grandchild, UUID-Bindung, High9, Region, Reset und echten Position-/Turn-Readback; Python-Dispatcher- und UI-Verträge.
654. CMake-Registrierung, ARM64-Headless-Script, Warnungsbereinigung des Low158-Decoders, Versions-/Projektplan-/Übergabe-Updates; kein Produktionseinsatz.

## Tests, Serverabgleich und Grenzen
Raspberry Pi 5: echte ARM64-C++23- und Python-Headless-Tests. Native WorldModel-Tests verknüpfen den tatsächlichen Code. Ein vollständiger Linux-x64-/NVIDIA-GPU-/Viewer-Build, Xvfb-Systemicon-Test und ein authentifizierter NexVerse-Simulator-Login wurden hier NICHT erneut ausgeführt. Der letzte SHA-genau verifizierte Pi5-CI-Run bleibt zunächst Alpha.644, bis ein neuer Workflow für den neuen Feature-Commit erfolgreich ist.

OpenGenesisLINK main am 21.10.2026: aktueller HEAD 0129e6179aaae6eec5cc9a6af7ff7ec6b1d4392b; reviewed-Baseline 4e99976630f1008db9014ea2b86c3723b09e9fe6. Der strikte Checker meldet 243 seitdem nicht vollständig einzeln geprüfte Commits und einen auf 300 Dateien gekürzten Compare. KEINE Aktualisierung des Reviewed-SHA, bis der manuelle Protokoll-/CAPS-/Serverreview abgeschlossen ist.

ECHTER Grid-Skin/COF/Outfit/Mesh-/WASD-Pixel- und Bewegungsnachweis weiterhin OFFEN. Deshalb gewichtet 38,65 %, angezeigt 39 %. Nächster Zehnerblock Alpha.655–664: aktuellen authentifizierten Grid/F3/F7/Low158/GetTexture/COF/AgentUpdate-Nachweis priorisieren, UI- und Serverfeatures parallel weiterbauen. Kein Produktivdeploy.
