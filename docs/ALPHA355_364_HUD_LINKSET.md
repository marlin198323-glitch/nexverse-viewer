# Alpha.355–364 – Firestorm HUD-Linksets

18.10.2026 · Viewer 0.2.0-alpha.364 · develop-current · **39 %** gewichtet 38,65 %.

Zehn tatsächliche Arbeitsblöcke:

355. GitHub, Handoff, neue NexVerse-Logs und OpenGenesisLINK Server-main 293d1d6 gesichtet (60 neuere Commits, überwiegend Portal/Profil/HG).
356. Renderer-HUD-Sichtbarkeitsfehler identifiziert: nur direkte HUD-Root-Prims wurden gefiltert.
357. Read-only Klassifikation über OpenSim-avatar-local-ID und tatsächlich vom Server gelieferte Attachment-Zuordnung.
358. HUD-Root, Kind- und Enkelprims aus der 3D-Welt ausgeschlossen.
359. Fehlende Eltern, Zyklus- und Selbst-Parent-Fälle ohne unendliche Rekursion behandelt.
360. Normale Kleidung, Weltobjekte und unbekannte Slot-IDs bleiben sichtbar.
361. Klassifikation im tatsächlichen OpenGL-Sichtbarkeitspfad.
362. Frameweise GL-Zähler für HUD-aus/ungültige Eltern, F3 zeigt reale HUD-3D-Ausschlüsse.
363. Native Tests, CMake CTest und Pi5-Headless-Verträge.
364. Mint Build/Tests/isolierter Xvfb/Icon-Gate, GitHub-Push/Pi5 SHA-CI und neue Chatübergabe.

**Kein authentifizierter Live-Grid-Avatar-/Skin-/Outfit-/WASD-E2E-Beweis.** Die Objekte auf dem Screenshot könnten HUD-Kinder sein; die tatsächliche AttachmentBlock-ID ist noch zu prüfen. Der letzte Viewer-Logstart um 01:35 war kein Live-Login. Upstream-Änderungen begründen keine automatische Aktivierung experimenteller Servermodule.

**Gesamtfortschritt 39 %**, nächste zehn Blöcke **Alpha.365–374**.
