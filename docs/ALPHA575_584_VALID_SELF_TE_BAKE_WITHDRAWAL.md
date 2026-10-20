# Alpha.575–584 – gültiger Bake-Rückzug räumt vorläufige eigene Skin auf

20.10.2026 | develop-current | Viewer 0.2.0-alpha.584 | Fortschritt 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke

575. GitHub develop-current HEAD 87ec271, Übergabe, Projektplan, VERSION und vorherige erfolgreiche Pi5-CI 39464572356 geprüft.
576. OpenGenesisLINK Server main 95c21334cfc299ebe03e91414f7ae762e1fc3c45 geprüft; alte echte Viewerlogs belegen GetTexture/Low158-Probleme, aber noch keinen neuen authentifizierten GPU-Avatar-Lauf nach Alpha.574.
577. Reproduzierter Fehler: Trotz einer gültigen aktuellen vollständigen ObjectUpdate TextureEntry mit ausschließlich zurückgenommenen oder Placeholder-Bakes behielt der Viewer die zuvor angenommene PROVISORISCHE Skin-Auswahl. Fehlende beziehungsweise beschädigte TextureEntries müssen dagegen gültige ältere Bakes weiterhin erhalten.
578. observe_self_object_bakes unterscheidet jetzt ungültige, fehlende oder verkürzte TextureEntry (kein Eingriff) von gültiger vollständiger TE mit keinen echten Bake-UUIDs. Im gültigen Rücknahmefall wird nur die alte vorläufige Auswahl derselben LocalID entfernt.
579. Native C++-Tests für all-Placeholder, all-Default, gültiges erneutes Annehmen, Duplikat, beschädigtes/fehlendes Paket, neue LocalID und Session-/Regionswechsel.
580. Per-Face-Authentizität: Bereits vorhandene echte Low158 Head-/Upper-/Lower-Bakes bleiben trotz Wegfall der ObjectUpdate-Provisorien erhalten. Keine Änderung der COF-/Wearables-Daten oder simulierte Appearance-Antwort.
581. Echte OpenGL-Framebuffer-Regression: Firestorm-Original-Body-Meshes mit GPU-residenter Bake-Textur; eine entfernte Referenz beendet ihren GPU-Skin-Draw bereits im nächsten Frame, obwohl das alte Asset noch im VRAM liegt. Erneute Bindung zeichnet wieder echte andere Pixel.
582. Sitzungs-/regionsgebundene reine Quellevent-Zähler self_object_bake_withdrawals; nur gültige TE-Rücknahmen zählen, keine Serverfehlversuche, F3 im Skin-Streaming-Modus informiert.
583. Privater Avatar-Diagnosebericht enthält nur die Rücknahme-Anzahl ohne Nutzer-, Asset- oder Inventar-UUID. Python-Quellvertrag sichert Validierung, Lebensdauer und GPU-Regression.
584. Vollständiger Mint x64 Build, native C++/OpenGL CTest, Python, ARM64-kompatible Headless, isolierte Xvfb-GUI und X11-Iconprüfung, GitHub-Featurecommit, exakte Pi5-ARM64-CI und aktualisierte Projektübergabe Alpha.585–594.

## Grenzen

Diese Änderung beseitigt veraltete provisorische Skin-Referenzen bei einer echten aktuellen Serverrücknahme, beschafft aber keine fehlenden Servertexturen, keine vollständigen COF-Kleidungsobjekte und keine simulatorbestätigte Bewegung. Ein NEUER authentifizierter NexVerse-Grid-Lauf mit voll sichtbarem Avatar und Outfit fehlt weiterhin. Daher 39 % Projektstand, kein produktives Deployment.

Nächste zehn Blöcke Alpha.585–594: echten Live-Grid-Login und aktuelle Avatar-/COF-/GPU-/WASD-Logs priorisieren, Firestorm-Oberfläche parallel vervollständigen.
