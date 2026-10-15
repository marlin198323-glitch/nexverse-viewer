# Alpha.136–139 – Firestorm Body-UV-Gate und tatsächliche GL-Draw-Evidence

Datum: 15.10.2026. Nur Pi5 ARM64 unter nexdev, Branch develop-current.
Beginn HEAD b464b08ee30f8ecf769e128a94e2b549c72bd41c.
Viewer-Codeversion 0.2.0-alpha.139.

## 136 – Original UV Validierungsgrenze vor GPU Upload
Neue read-only Funktion audit_avatar_body_uv() im Header
include/nexverse/render/avatar_body_uv_evidence.hpp. Der Renderer
set_avatar_body_mesh verweigert ungültige Interleaved Geometry vor
GL-Upload: nicht-endliche xyz, Normale oder UV, überzogene Original-UVs,
ungültige Indizes, inkonsistente 8-Float Vertex-/Tri-Struktur und
vollständig degenerierte U- oder V-Spanne.
Originale Firestorm UV-Koordinaten werden weder normalisiert noch
gespiegelt; gültige Werte außerhalb [0,1] bleiben erlaubt. Das ist nur
ein Safety-Check, keine gegen Firestorm gemessene Pixelparität.

## 137 – Echte serverseitige Body-Textur pro Original-LLM-GL-Draw
RenderDiagnostics erweitert um
self_avatar_body_bake_gl_draws[Head,Upper,Lower], pro Frame zurückgesetzt.
In opengl_renderer.cpp jeweils NACH glDrawElements nur für den
Original-Firestorm-anatomical_ready .llm-Pfad erhöht, und nur wenn die
serverseitig authentifizierte Bake-UUID in object_textures_ ein gültiges
OpenGL-Texture-Objekt adressiert. Der 7-part procedural-proxy Fallback
erhöht diese neue Metrik niemals. Der Counter beweist einen OpenGL
Draw-Submit pro Mesh+Textur; keine sichtbare Pixelparität oder Wear.

## 138 – Sichtbare echte F9 Benutzeroberfläche
Die Outfit-Galerie (F9, bestehender Button Appearance pruefen) zeigt
zusätzlich die aktuellen Original-LLM-Body-GL-Ereignisse H/U/L als
G=originaler Mesh-Draw mit gebundener Bake-Textur, -=kein Draw.
Status ist als keine Pixelparität beschriftet; solange der Nutzer
Appearance nicht explizit aktiviert hat, erscheint ein Hinweis zum
Anklicken. Für kompakte Fenster wird statt einer zweiten,
überlaufenden Statuszeile nur die kurze Diagnose oben gerendert.
F9-Bake-/COF-/Inventaraktionen bleiben vorhanden, keine neue
simulatorschreibende Funktion.

## 139 – Native Pi5 Regressionen / CI / Server
Neues C++23 Header-only Testprogramm
tests/avatar_body_uv_evidence_tests.cpp mit positiven UV- und negativen
NaN/Inf/UV-Overflow/Index-/Strukturfällen sowie F9-Diagnosezuständen.
Zusätzlich tests/test_avatar_body_gpu_wiring.py mit vier
Quellcode-Integrationschecks: GL-Proof Counter ist nur im anatomischen
Draw-Pfad, Proxy-Fallback zählt nicht, Reset vor Rendern, UV-Guard vor
GPU-Upload und F9-Ausgabe erhält echte Diagnostics.
CMake-CTest-Registrierung und vorhandenes Pi5 CI Skript ergänzt.
Tatsächliche Pi5 Tests: 5 C++23 Programme PASS, 22 Python-Unittests PASS,
bash -n / git diff --check PASS. Der grafische OpenGL-/GLFW-Run,
vollständige CMake-Compilation und Firestorm-original
Mesh-/UV-Pixelvergleich wurden auf Pi NICHT ausgeführt.

## Upstream und noch offene Arbeit
OpenGenesisLINK Server main ist weiterhin
d31c03168f0f04f4ce02e82ee3246e16de744748.
Seit freigegebener Baseline 4e99976630f1008db9014ea2b86c3723b09e9fe6
22 neue Commits; striktes Compare-Gate weiterhin wegen >=300 Dateien
und unvollständig geprüfter Einzelcommits BLOCKIERT. Kein Hochsetzen
von reviewed_sha, keine Server-Codeänderung oder ungeprüften
Protokollannahmen.

Noch offen: Firestorm Original .llm Assetdateien auf genehmigtem
GPU-Testhost gegen reale Skin-UV/Shader-Pixel prüfen, Upload und
CacheID-/COF-Readback unter simulatorsicherer Autorisierung,
vollständige Wear/Unwear-Rollback-Abnahme sowie vollständiger
Linux-x64/Windows/macOS Build. Nächste Blöcke: echte GPU-End-to-End-
Diagnose mit korrekter OpenGL-Toolchain, Firestorm TexLayer/Bake-Pixel
mit Original-Assets und robuste Grenzfälle von Shader/Skel-Animation.
