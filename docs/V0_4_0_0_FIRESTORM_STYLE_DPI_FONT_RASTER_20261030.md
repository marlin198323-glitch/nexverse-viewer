# OGL Viewer v0.4.0.0 – Firestorm-orientierte, DPI-unabhängige UI-Schriften

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Ausgangsstand `db849584fec408e94240ae769563649a9c5e1124`**

## Fehler und Nachweis durch Nutzer-Screenshots

Die realen Screenshots vom 30.10. um **22:26:19** (NexVerse-Welcome/Login) und **22:37:25** (authentifizierte 3D-Welt) zeigen extrem kleine, teils unscharfe UI-Schrift im 58px-Header, 60px-Login-Dock, Weltmenü und beiden HUD-Panels. Die Webseite selbst ist ein WebKit-Snapshot und wird ausdrücklich nicht neu gestaltet oder durch ein Bild ersetzt.

Tatsächliche Ursache im OGL-GPU-Text-Renderer: `UiRenderer::initialize_text()` rasterisierte nur **ASCII 32–126 bei 48px**, skaliert danach jeden Buchstaben auf 5–10px herunter und verwendet `GL_LINEAR`. Bei HiDPI oder wechselnden Fensterformaten führte die 48→7-Verkleinerung zu schlechter Schärfe und die reinen Byte-Schleifen zu verschluckten deutschen Umlauten. Die Neon-Felder haben feste kleine Schriftskalierungen; im Welt-HUD wird `h/1080` auf bis zu 0,74 reduziert. Diese Daten sind **keine Eigenschaft der RTX 4070 SUPER**.

## Firestorm-Referenz (lokal installierte Originaldistribution)

Die unveränderte lokale Firestorm-x64-Distribution unter `/home/jam/firestorm` enthält:
- `app_settings/settings.xml`: **`UIScaleFactor`** mit Default `1.0`, ausdrücklich als UI-Größe relativ zum Standardlayout.
- `app_settings/settings.xml`: separate **`FSFontSizeAdjustment`** (Font-Größe relativ zum Standard in zusätzlichen Punkten).
- `fonts/fonts.xml` und `skins/default/xui/en/fonts.xml`: separate Font-Konfiguration, `DejaVuSans.ttf` als Linux-Fallback und mehrere Sprach-/Font-Fallbacks.

Firestorm behandelt also **Layout-/UI-Skalierung, Schriftgröße und Schriftfamilie getrennt**, anstatt ein einziges riesiges Glyphenbild auf alle Textgrößen herunterzurechnen. Wir verwenden diese Architekturprinzipien, **nicht die Firestorm-UI als Screenshot** und kopieren keine Font-Dateien in das Git-Repository.

## Zehn echte Implementierungs- und Prüfschritte

1. `CHATUEBERGABE.md` und `ROADMAP.md` zuerst vom Linux-Mint-Commander gelesen; sauberer `develop-current`-Stand `db849584` gegen GitHub bestätigt. Keine Änderungen am Raspberry Pi.
2. Beide Screenshots bewertet und neueste reale Viewerlogs abgeglichen. Ein erneuter Login um **22:35:48** war erfolgreich; LLUDP/CAPS live. Aktuell getrennt offenes Materialproblem: um 22:39 **335/354** Texture-GPU-UUIDs, davon **18 Mip0 und 317 reduziert**; **2802** tatsächlich texturierte und **24** Fallback-Draws; **384/384** Mesh-UUIDs. Diese Welttexturauflösung wird nicht durch veränderte UI-Fonts geheilt.
3. Originale Firestorm-Distribution `/home/jam/firestorm/app_settings/settings.xml` und `fonts/fonts.xml` ausgewertet; getrennte `UIScaleFactor`- und `FSFontSizeAdjustment`-Parameter sowie Font-Fallbacks verifiziert. OGL-UI-/Glyph-Pipeline und Neon-/World-HUD-Skalierung bis zu `FT_Set_Pixel_Sizes(face, 0, 48)` nachverfolgt.
4. Gerätunabhängige `font_raster_policy.hpp` mit realen FreeType-Rastergrößen **12/16/24/36/56/96px** eingeführt. Für die tatsächliche Textgröße wird die nächstpassende physische Auflösung gewählt; 48→7-Bilinearskalierung entfällt.
5. Abseits der bestehenden kompakten Layoutgeometrie eine **lesbare Mindestschriftgröße 10,5 logische Pixel** eingeführt. Layout-/Hitboxen und Welcome-WebKit-CSS bleiben unverändert; ein kleineres Fenster darf die Schrift nicht bis zur Unlesbarkeit schrumpfen. Bei großen Schriftgrößen bleibt die bestehende Textgrößen-Hierarchie erhalten.
6. Das tatsächliche `glfwGetFramebufferSize()/glfwGetWindowSize()`-Verhältnis pro Frame an den UI-Fontrenderer weitergegeben. Die Auswahl der Glyphenauflösung berücksichtigt damit **1x, 1,5x, 2x und 3x** HiDPI/Retina-Fenster ohne NVIDIA-/AMD-/Intel-Sonderfälle. GLFW-Mauskoordinaten, Trefferflächen, OpenGL-Weltviewport und WebKit-Snapshot bleiben unberührt.
7. Alle FreeType-Textmess-/Draw-Schleifen auf Unicode-Codepoints statt einzelner UTF-8-Bytes umgestellt, mit Prüfung gegen überlange/surrogate/abgeschnittene Sequenzen und sichtbarem Ersatzzeichen `?` bei nicht darstellbaren Glyphen. ASCII, Latin1/Latin Extended, Griechisch und Kyrillisch aus vorhandenen Systemfonts geladen, ohne Font-Assets auszuliefern.
8. Für jede Pixel-Rastergröße und jeden Unicode-Codepoint eine getrennte Glyph-Textur plus Metrik; `text_width()` und `text()` wählen denselben Bucket. GPU-Glyph-Texturen werden beim Shutdown weiterhin explizit freigegeben. OGL-Font-Log nennt geladene TTF und Rastergrößen (keine privaten Daten).
9. Native `neon_login_layout_tests.cpp` auf mehrere Auflösungen, HiDPI-Rasterwahl, Nicht-Schrumpfen, echte deutsche/greek/cyrillic Unicode-Codepoints, ungültige UTF-8-Sequenzen und getrennte Glyph-Cache-Keys erweitert. Zwölf Python-Regressionskontrakte prüfen wirkliche Codepfade, systemabhängige DPI-Metrik, gleichwertige Textbreite-/Draw-Berechnung und unverändertes WebKit.
10. Volles Linux-Mint-Gate ausgeführt: **195/195 CTests, 910/910 Python-Tests**, Firestorm-basiertes isoliertes Xvfb/OpenGL-GUI, X11-Taskbar-Systemicon **PASS**; danach `CHATUEBERGABE.md`, Roadmap, `projectplan.md` und Fortschritts-JSON aktualisieren, Commit auf `develop-current` und CI prüfen.

## Abgrenzung, generelle Freigabe

Dies ist ein universeller Schrift-/HiDPI-Bugfix, keine lokale Einstellung für 1952x800 oder die RTX 4070 SUPER. Er schärft Text und verhindert zu kleine Basisgrößen auf verschiedenen Fenstern/Monitoren, garantiert aber nicht automatische pixelgenaue Identität von Seitenleisten auf jedem Seitenverhältnis. Bei sehr kleinen Fenstern können längere Statusmeldungen weiterhin zusätzliche Umbruch-/Layoutarbeit benötigen. Firestorm bietet darüber hinaus Nutzerregler für `UIScaleFactor` und `FSFontSizeAdjustment`; solche Nutzerpräferenzen sind ein weiterer funktionaler Ausbaupunkt und werden hier nicht als bereits implementiert behauptet.

Der original NexVerse-Welcome-Hintergrund, Branding, 58px/60px-Chrome, Welt-Renderer, Shader, LLUDP, Login, Session und Bild-Assets wurden nicht geändert. Eine neue nutzerseitige Bild-/Schärfeabnahme **nach** dem Rebuild ist erforderlich; die bisherigen Screenshots zeigen den Stand **vor** dieser Korrektur. Bisheriger Versionsfortschritt **38,65 % gewichtet, rund 39 %**. `v0.4.0.0` bleibt **IN ARBEIT**, kein `main`-Merge oder Release.
