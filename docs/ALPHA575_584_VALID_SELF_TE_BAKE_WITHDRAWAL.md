**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.584, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.585–594.**

**ALPHA.584 SHA-GENAUE Pi5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39467519712 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 Headless Job 118782677990 = SUCCESS, für exakten Feature-SHA 9b97aca5615a6942c4ca4521b86f46029fd9ad08. Mint x64 vollständiger Build 381/381, native C++/OpenGL 162/162, Python 287/287, isolierter Xvfb Viewer, lokale C++23/Python Headless und X11 Taskleistenicon/WM_CLASS PASS. Der Pi5-CI-Lauf prüft native ARM64 C++23 Headless/Python, keine Pi5-GPU oder GUI und keinen auth Live-NexVerse-Avatar. Wirkliche valide Self-ObjectUpdate TE ohne echte Bakes löst nur die alte PROVISORISCHE Skin; beschädigte oder verkürzte TE bleibt ohne Wirkung; gültige Low158 bleibt pro Face führend. Echte original Firestorm Body-Mesh-GL-Framebuffer-Regressionsbeweise für ausbleibende stale Skin bei noch GPU-residentem Texturasset; F3/privater count-only Herkunftsbericht pro Agent-/Regions-Epoch. Live NexVerse COF/Outfit/Skin/WASD weiter nicht bestätigt. Status 39 %, nächste zehn Blöcke Alpha.585–594, kein Produktionsdeploy.

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
