# Alpha.275–284 – Sichtbare Firestorm-Avatar-Meshes: echte GPU-Depth-Belege

17.10.2026 | OGL 0.2.0-alpha.284 | develop-current | **39 % Gesamtfortschritt (38,65 % gewichtet, unverändert)**

## Zehn Entwicklungsblöcke

| Block | Tatsächliche Änderung / Integration | Verifikation |
|---|---|---|
| 275 | OpenGenesisLINK main 7c6b0cf7252189707066f09ad66c732642b27af9 geprüft. Neuer OGLAssetService Build-/Konstruktor-/Dependency-Fix in 3 Serverdateien, kein bestätigter neuer Viewer-GetTexture-/AvatarAppearance-Wirevertrag. | GitHub Upstream Diff |
| 276 | 3 getrennte Head/Upper/Lower-GPU-Tiefentests statt einer gemeinsamen Query. Jedes Original-LLM-Mesh besitzt zwei asynchrone OpenGL-Query-Slots, insgesamt sechs statt zwei. | renderer.cpp |
| 277 | GL_ANY_SAMPLES_PASSED für jedes einzelne originale Firestorm-GL-Mesh unmittelbar um glDrawElements, nicht um Avatar-Proxy-Ersatzgeometrie. | Native GPU |
| 278 | Nonblocking Query-Lesen nur nach GL_QUERY_RESULT_AVAILABLE; separate Part-known/Part-passed Zustände und Regions-Epoch-Prüfung. | Native/Source |
| 279 | Monotone Query-Ausgabesequenzen und letzte verifizierte Sequenz je Body-Part verhindern verspätete ältere GPU-Resultate als neue Sichtbarkeitsresultate. | Core/Source |
| 280 | Aggregierte Sichtbarkeit OR über Head, Upper und Lower: unsichtbares Lower überschreibt sichtbares Head nicht länger. Null-Proof wenn kein aktueller Original-LLM-Draw. | AvatarPartVisibility C++ Test |
| 281 | Echte F3-Avatarübersicht zeigt Head/Upper/Lower GPU-Tiefentest je Part (V sichtbar / 0 verdeckt / ? wartend / - kein Original-LLM). | Viewer GUI |
| 282 | F3 Bake-Seite zeigt für Head, Upper und Lower getrennt echten Server-TE, GPU-Residenz, BakeGL-Draw und individuellen Depth-Test. | Inspector Native/GUI |
| 283 | Manuell speicherbarer datensparsamer F3-Diagnoseexport: sechs boolesche Head/Upper/Lower Depth Known/Passed-Felder, keine Session/UUID/CAPS/Chatinhalte. | Existing Export Pipeline |
| 284 | Native C++23-Regression, Pi5-Headless-Liste, fünf Python-Source-/UI-Contracts, vollständiger Mint-x64/NVIDIA-GL-CMake-Build, Xvfb/Vollviewer/Originalicon, SHA-belegter GitHub-Push und Pi5-CI. | FERTIG: Mint 142/142 Native C++/GL, 95/95 Python, Headless/Xvfb/Icon PASS; Pi5 SHA 2017b48fcc6586e00656bba01c6fe3058d0c98be CI Run 39065802543 SUCCESS |

## Grenzen

Dies sind **echte, aber zeitversetzte OpenGL-Depth-Test-Belege** für Original-Body-Geometrie. Sie ersetzen keine Live-Grid-AvatarAppearance, keine authentifizierten Outfit-/Bake-Pixel, keine Mesh-Anhänge und keine echte plattformübergreifende End-to-End-Abnahme. Alle drei Body-Teile können auch ohne passende Serverbakes als tintfarbene Originalmeshes gezeichnet werden; Depth sichtbar bedeutet daher ausdrücklich **nicht** Outfit/Bake korrekt. Ein positiver Head-Test beweist nicht, dass Lower sichtbar ist. Region-Wechsel und fehlender Originalkörper entwerten die Abfrage.

Mint x64 GPU/GUI ausschließlich im isolierten Xvfb; keine sichtbare Viewer-Aktion auf dem echten Desktop. Pi5 ARM64 Source/Native Headless, keine native GPU-Releasemessung. OpenGenesisLINK ROBUST/OGLAssetService bleibt experimental und disabled-by-default, kein ungeprüfter Live-Deploy.

**39 % Gesamtfortschritt unverändert. Nächste Zehnergruppe Alpha.285–294.**

## Tatsächlicher Testabschluss

Feature-SHA 2017b48fcc6586e00656bba01c6fe3058d0c98be; GitHub Pi5 ARM64 Run 39065802543 SUCCESS auf exakt demselben Featurecommit. Mint x64 CMake Vollbuild 340 Schritte, 142/142 native C++23/NVIDIA GL Tests, 95/95 Python, ARM64-kompatibler Headless, isoliertes Xvfb-System und X11 Originalicon PASS. Authentifizierter Live-NexVerse Avatar-/Outfit-/Pixel-E2E bleibt ausdrücklich OFFEN; 39 % Gesamtfortschritt unverändert.
