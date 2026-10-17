# Alpha.315–324 – Firestorm-Avatar: GPU-Weight-Joints und sichtbares Original-LLM

17.10.2026 | Viewer 0.2.0-alpha.324 | develop-current | **39 %** (38,65 % gewichtet, unverändert).

## Konkreter Renderingfehler

Der OpenGL-Vertexshader der originalen Firestorm-Head-/Upper-/Lower-LLM-Meshes hat bei dem letzten gültigen Eintrag des tatsächlichen Teilmesh-Jointpalettenbereichs eine nicht belegte nächste Matrix gelesen. Das konnte gewichtete Vertexpositionen am Körper verformen. Jetzt werden die beiden letzten gültigen Matrizen mit Faktor 1 auf das letzte Gelenk aufgelöst. Die maximale GPU-Palette umfasst 32 Einträge; jede der drei LLM-Meshes verwendet ihren eigenen tatsächlichen Umfang.

## Zehn umgesetzte Blöcke

| ID | Tatsächliche Änderung oder Prüfung |
|---|---|
| 315 | GitHub develop-current mit Übergabe, HEAD und OpenGenesisLINK Server-main 7c6b0cf überprüft, kein geänderter LLUDP-Vertrag. |
| 316 | Firestorm LLM Avatar-Vertexlayout, echte GPU Jointpaletten, original-UVs und Shader-Skinning bis DrawElements untersucht. |
| 317 | Vertexshader bekommt eigene JointCount-Uniform und begrenzt das letzte Gelenk auf tatsächliche Matrixgröße statt Identitätsmatrix. |
| 318 | Head, Upper, Lower binden jeweils ihren eigenen GPU-Palettenumfang statt eines globalen Werts. |
| 319 | Letzter Index N-1 wird zum Paar N-2/N-1 mit Blendfaktor 1, sonst echte benachbarte Gelenk-Interpolation. |
| 320 | Sicherer CPU-Uploadprüfer für Gewichtspaare und tatsächliche Palette 2 bis 32; negative, zu große und nichtendliche Gewichte abgelehnt. |
| 321 | F3-Avatarfenster zeigt tatsächliche Original-Weight-GL- und aktive Keyframe-Clip-GL-Draws, ausdrücklich ohne erfundene Live-Grid-Pixelparität. |
| 322 | Neuer nativer Grenztest aller Palettengrößen, last-bone und Nahwerte, OOB und NaN; CMake-CTest und Pi5-Headless-Liste. |
| 323 | OpenGL-Originalbody UV, Regionstransition, GUI-Source-Contracts und kompatible Viewerfunktion geprüft. |
| 324 | Voller Mint x64/NVIDIA GL CMake/CTest/Python/Headless/Xvfb/Icon-Gate, GitHub-Featurecommit, Pi5 ARM64-CI und aktualisierte Projekt-/Übergabedokumentation. |

## Grenzen

Die Änderung behebt ein reales Gelenkinterpolationsproblem des ursprünglichen Firestorm-GL-Körpermeshes. Sie erzeugt keine neue AvatarAppearance, keine Wearable-Meshes und keinen Simulator-Bake-Upload. Authentifizierter NexVerse Live-Grid-/Outfit-/Baked-Texture-Pixelbeweis weiterhin OFFEN, Xvfb ist kein Grid-E2E-Test. OGLAssetService des OpenGenesisLINK Servers bleibt experimental disabled-by-default.

**Gesamtfortschritt 39 %**, nächste 10 Blöcke **Alpha.325–334**.
