# OpenGenesisLINK Viewer – verbindlicher Projektfortschritt

**Gesamtfortschritt: ca. 38 % – gewichtete Schätzung.**
Stand 16.10.2026 | Viewer 0.2.0-alpha.234 | Branch develop-current.
Die Prozentzahl beschreibt die geschätzte Funktionsreife
zur vollständigen Firestorm/OpenSim/OpenGenesisLINK-Basis.
Sie ist weder Codezeilen- oder Testquote noch eine Freigabe.

## Gewichtete Projektbewertung

| Bereich | Gewicht | Teilfortschritt | Beitrag |
|---|---:|---:|---:|
| Grid, Login, LLUDP, CAPS und Regionsverbindung | 15 % | 75 % | 11.25 Prozentpunkte |
| 3D-Welt, Terrain, Himmel und Renderer | 20 % | 40 % | 8 Prozentpunkte |
| Avatar, Server-Bakes, Outfits und Animationen | 20 % | 25 % | 5 Prozentpunkte |
| Texturen, Meshes, LOD und Materialsystem | 15 % | 45 % | 6.75 Prozentpunkte |
| Viewer-GUI, Menüs, Fenster und Bedienung | 15 % | 30 % | 4.5 Prozentpunkte |
| Inventar, Kommunikation, Teleport, Bauen und weitere Firestorm-Funktionen | 10 % | 10 % | 1 Prozentpunkte |
| Plattform-Releases, Stabilität, Sicherheit und E2E | 5 % | 25 % | 1.25 Prozentpunkte |

**Formel:** Summe (Gewicht × Teilfortschritt)/100
= **37,75 %**, gerundet **38 %**. Alle Gewichte ergeben 100 %.
Teilwerte sind fachliche, anhand nachweisbarer Funktionen
begründete Schätzungen und keine Abnahme.

## Nachweise und verbleibende Lücken

- **Grid, Login, LLUDP, CAPS und Regionsverbindung (75 %):** Realer Login/LLUDP-Handshake und SCENE_READY im Nutzerlog, EventQueue und Diagnosen; stabile Regionsübergänge noch nicht vollständig E2E belegt.
- **3D-Welt, Terrain, Himmel und Renderer (40 %):** Echte GL-Szenen- und Terrain-Draws; Terrain grau, Umgebung und Licht noch nicht Firestorm-paritätisch.
- **Avatar, Server-Bakes, Outfits und Animationen (25 %):** Originale Firestorm-LLM-Meshes sichtbar; aktuelles Self-AvatarAppearance fehlt, Bakes/Kleidung/Animation nicht vollständig.
- **Texturen, Meshes, LOD und Materialsystem (45 %):** GetTexture/GetMesh und GPU-Streaming funktionieren, aber Low-LOD, Ersatzmeshes, Materialqualität und fehlerhafte Assets offen.
- **Viewer-GUI, Menüs, Fenster und Bedienung (30 %):** Originalicon, Welcome-Seite, Login, HUD, Diagnose, Kamera und Grundmenüs vorhanden; viele Firestorm-Bedienfunktionen noch offen.
- **Inventar, Kommunikation, Teleport, Bauen und weitere Firestorm-Funktionen (10 %):** Teilfunktionen und erste Oberflächen vorhanden; vollständige Bedienung, soziale Funktionen und Inworld-Aktionen vielfach unvollständig.
- **Plattform-Releases, Stabilität, Sicherheit und E2E (25 %):** Mint-x64-Nativbuild/NVIDIA-GL-Tests/Pi5-CI grün; Windows/macOS-Vollabdeckung und echte Grid-End-to-End-Abnahme fehlen.

## Pflichtablauf bei jedem neuen Projektstand / neuen Chat

1. Die ganz oben stehende Prozentregel in CHATUEBERGABE.md,
   projectplan.md und AGENTS.md zuerst lesen; Branchstand,
   Version und vorheriges Testergebnis erneut ermitteln.
2. Auf dem Linux-Mint-Checkout ausführen:
   python3 scripts/project-progress.py
3. Nach jedem Projektblock jeden Teilbereich neu bewerten;
   nur belegte, integrierte und getestete Funktionen
   duerfen den Fortschritt erhoehen. Fehler koennen ihn senken;
   bei unveraendertem Befund bleibt der Prozentwert identisch.
4. Immer in der Nutzerantwort UND zentral in
   docs/PROJECT_PROGRESS.json, docs/PROJEKTFORTSCHRITT.md,
   projectplan.md, CHATUEBERGABE.md und info.md
   den aktuellen Gesamtfortschritt in XX % synchron halten.
5. 100 % nur mit echter vollstaendiger Firestorm-/
   OpenGenesisLINK-Funktionsparitaet samt Live-Grid-Avatar,
   Bakes, Outfits, Animation, vollstaendiger Welt/
   GUI und Plattform-Releases. Keine 100 % nur durch CI.

**Nächster Durchlauf:** Alpha.235–239 mit wieder fünf
echten Entwicklungsblöcken. **Besonders offen** sind Self-
AvatarAppearance, vollständige Bakes/Outfits/Animationen,
Terrain, High-LOD-Objekte, fehlende Firestorm-Funktionen
und Windows/macOS-E2E.
