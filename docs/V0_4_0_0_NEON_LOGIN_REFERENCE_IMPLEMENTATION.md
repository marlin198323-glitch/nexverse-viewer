# v0.4.0.0 – Echter programmierter Neon-Login nach Referenzbild

25.10.2026. Linux Mint Commander jam-MS-7D28; GitHub
marlin198323-glitch/nexverse-viewer, Branch develop-current.

## Zwölf nachprüfbare Arbeitsschritte

1. Repo-SHA, VERSION v0.4.0.0, CHATUEBERGABE.md und ROADMAP.md geprüft.
2. Reale NexVerse-WebKit-Welcome-Seite als Hintergrund beibehalten.
3. Stark verdunkelndes Login-Overlay samt mittigem Popup aus aktivem Renderpfad entfernt.
4. Originales OGL-Icon, großer Header und Cyan-/Neon-Branding.
5. Mittige aktive Grid-Info und drei echte GRID/LOGIN/WORLD-Statuskarten.
6. Welcome-Snapshot unter Header mit Hero, Karten und News lesbar ausgerichtet.
7. Breites glasartiges Footer-Dock mit Avatar/Password und 259px-Welt-Betreten-Button.
8. Gemeinsame NeonLoginLayout-Geometrie für Rendering und GLFW-Hitboxen.
9. Funktionierender Passwort-Zeigen-Toggle ohne Session- oder Passwortpersistenz.
10. Dekoriertes, minimier-/maximierbares, resizables maximiertes Fenster startet in tatsächlicher Monitorauflösung.
11. Ein nativer CTest für sechs Auflösungen und elf Python-Verhaltensverträge.
12. Echte isolierte Xvfb/OpenGL-UI-Aufnahme bei 1648×928 (lokales QA-Artefakt build/neon-login-proof/login_full_final.png); kein KI-Bild.

**Nicht behaupten:** Echtes Login nach Codeänderung, authentifizierte CAPS-,
Outfit-, WASD-E2E sowie v1-Release. Alle weiterhin offen. Die zwei
Buttons der WebKit-Seite (Bürger werden/Welt entdecken) sind weiterhin
Bildelemente aus dem Offscreen-Snapshot, keine funktionsfähige
Viewer-Linkdelegation. Keine irreführende Angemeldet-bleiben-Funktion.

Version bleibt v0.4.0.0 IN_ARBEIT, Fortschritt 38,65 %.
Keine Änderungen an main oder Produktivdeploy.

## Vollständige technische Abnahme (Linux Mint x64)

CMake/vcpkg-Debug-Build inkl. echter OpenGL-Anwendung **PASS**,
**186/186 native CTests**, **658/658 Python-Tests**,
echter isolierter Original-Firestorm-Xvfb-GPU-/GUI-Start inklusive
133 Skeleton-Joints/55 Attachments/7.186 Körpermeshdreiecken PASS.
Originales OGL-System-/Taskleistenicon **128×128 Pixel**,
korrekte WM_CLASS und X11-Window-Proof PASS.
Marker: OGL_MINT_X64_FULL_VALIDATION_PASS.
Ein eigenständiger nativer NeonLoginLayout-CTest deckt
800×600, 1100×700, 1280×720, 1648×928, 1920×1080
und 3440×1440 ab. Elf zusätzliche Python-Verträge
prüfen reale GLFW-Click-Hits, einheitliches Layout,
Passwortanzeige, Grid-Login-Backend und Branding.

Zwei echte im isolierten GUI-Run aufgenommene PNG-Beweisscreenshots
bestätigen das tatsächliche Browser/GL-Layout (der letzte
in build/neon-login-proof/login_full_final.png): obere Statuschips,
offener NexVerse-Hero, sichtbare Grid-Karte und große Loginleiste.
Dies sind lokale QA-Artefakte, keine generierten Designbilder.
Es gibt **keinen** authentifizierten Live-Grid-Login-Nachweis
der neuen UI in diesem Build-Lauf.

Exakte nach GitHub-Push geprüfte Feature-Code-SHA separat dokumentieren;
GitHub-ARM64-CI noch nicht als bestanden behandeln.
