# OGL Viewer – Bestandsaudit 24.10.2026

Entwicklungsbranch `develop-current` (Linux Mint Desktop Commander).
Basis-Commit vor Umstellung `f5bf6cc43e94f6b91aaf9333ef8d31ffa58463d4`.
Projektstruktur, Dokumentationsumfang, Versionsquellen, Source-Module,
Tests und Release-Workflows systematisch inventarisiert. Dies ist
keine zeilenweise Sicherheitsreview aller Dateien und kein Live-Grid-Test.

**Umfang:** 56 C++-Quellen in `src`, 178 Header in `include/nexverse`,
180 native C++-Testdateien, 110 Python-Testdateien vor dieser Änderung (nachher 111), 161 Dateien in `docs`
und 3 GitHub-Workflows. Zuletzt lokal belegt: 184/184 native CTests,
606/606 Python und isolierte Original-Firestorm-GPU-/Xvfb-GUI-/X11-Icon-Tests.

## Inkonsistenzen

1. Öffentlich angezeigte Version bisher 0.2.0-alpha.1194,
   CMake-Projektlabel 0.1.0 und README alpha.85.
2. Drei UI-Ausgaben ergänzten ein eigenes v, das im gewünschten Schema zu einem doppelten vv geführt hätte.
3. `projectplan.md` über 7.300 Zeilen und `CHATUEBERGABE.md`
   über 4.800 Zeilen dokumentieren vorwiegend historische Alpha-Blöcke;
   eine stabile stufenweise Release-Roadmap fehlte.
4. 38,65 % gewichteter Stand ist eine Schätzung, keine 100-%-Freigabe.
5. Sieben echte Release-E2E-Beweise fehlen laut
   `scripts/ogl-release-readiness.py`. Der Produktiv-Release bleibt BLOCKED.

## Bereiche

| Bereich | Stand | Kernlücken |
| --- | ---: | --- |
| Grid/Login/LLUDP/CAPS/Region | 75 % | echte Regionwechsel-/Reconnect-/Teleporttests |
| Welt/Terrain/Renderer | 43 % | World-/Licht-/Wasser-/Wetterparität bei realem Grid |
| Avatar, Outfits, Animationen | 25 % | echtes 3/3-RGB-Skin, vollständiges Outfit/COF, verifiziertes WASD/Yaw |
| Mesh/Textur/PBR/LOD | 47 % | fehlende Mesh-/Material-Assets und PBR/glTF-Livefälle |
| GUI/Menüs/Fenster | 30 % | komplette Firestorm-Bedienoberflächen und funktionierende Buttons |
| Inventar/IM/Freunde/Gruppen/Bauen | 10 % | tatsächliche Serveraktionen statt Platzhalter |
| Release/Security/Plattformen | 25 % | Windows/macOS, exakte SHA-CI, Sicherheit/Lizenzen |

**Gesamt: 38,65 %** gewichtet, rund 39 %. Das C++23-Projekt ist
eine eigenständige Implementierung mit Firestorm als Funktionsreferenz
und OpenGenesisLINK/OpenSim als autoritativem Serververtrag, nicht
ein 1:1 Firestorm-Quellcode-Fork. MPL-2.0-/Asset-Lizenzen und
vollständige Parität benötigen weiterhin eigene Prüfung.

Neue Planung: **v0.3.0.0 (Entwicklung)** bis v0.9.9.0,
**v1.0.0.0 vollständig releasefähig**, **v1.0.0.1 Hotfix**.
Details `ROADMAP.md` und `docs/RELEASE_ROADMAP.json`.
Keine Änderungen an main oder Release-Tags durch diese Umstellung.

## Abschluss der technischen Entwicklungsbasis

Der erste vierteilige Mint-Neubuild zeigte drei ältere native Version-/Modulvertragsfehler. Die Runtime unterstützt jetzt neben vMAJOR.MINOR.PATCH.REVISION weiterhin die alten Alpha-Modul-Manifeste. Danach volle Linux-Mint-x64-CMake/vcpkg-/OpenGL-Regression mit 184/184 nativen CTests, 606/606 Python und isoliertem original Firestorm-Xvfb-GPU-GUI/X11-Icon PASS. Das Startlog meldet OpenGenesisLINK Viewer v0.3.0.0. Nur die Versionsbaseline ist damit FERTIG; der v1.0.0.0-Release bleibt BLOCKED und main unverändert.
