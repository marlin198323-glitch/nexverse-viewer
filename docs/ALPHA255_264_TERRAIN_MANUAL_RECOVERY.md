# Alpha.255–264 – Regionsfenster: sicherer Terrain-Neuversuch

17.10.2026 | OGL 0.2.0-alpha.264 | develop-current | **39 % Gesamtfortschritt (38,65 % gewichtete Schätzung, unverändert)**

## Echte Entwicklungsblöcke und Umsetzung

| ID | Code/Integration/Tests | Stand |
|---|---|---|
| 255 | OpenGenesisLINK main bf55a17524a9f3f175c56d2adeaa4d39083f446f geprüft: Merge des disabled-by-default OGL AssetService-Addons; kein neuer Viewer-CAP-Vertrag ohne Opt-in-/Autorisierungs-Review. | Codevertrag unverändert, geprüft |
| 256 | TerrainDetailRetry behält die Materialgeneration/-revision bei; manueller Restart leert niemals bereits dekodierte vier RGBA-Layer. | implementiert, Native-Test |
| 257 | Neustart nur nach Ausschöpfen von vier Automatikversuchen; finale 30-s-Pause, höchstens zwei ausdrücklich gewählte manuelle Restart-Budgets je Regionmaterial. | implementiert, Native-Test |
| 258 | Nach manuellem Neustart gehen ausschließlich noch fehlende Layer durch missing(ids) an GetTexture; keine erneuten gültigen Layer-Reuploads. | implementiert, Native-Tests |
| 259 | Kein Retry während laufendem Download, ohne GetTexture-CAP, ohne gültige IDs oder bei erschöpfter manueller Quote; die tatsächliche Material-/CAP-Epoche wird im Hauptloop nochmals geprüft. | implementiert, Native-/Python-Tests |
| 260 | Regionsfenster F7 zeigt echten Button Fehlende Layer nachladen [T]; in nicht erlaubten Zuständen deaktiviert, Modal-Klick nicht an Welt durchgereicht. | implementiert, Layouttest 1100/1920/3440 px |
| 261 | Tastatur T nur im authentifizierten aktiven Regions-Floater, dieselbe bewachte Request-Queue wie Mouse. | implementiert, Python-Wiring |
| 262 | F7 zeigt L1–L4 getrennt (geladen/fehlt/nicht gesetzt), CAP-/In-flight-/Countdown-/Erschöpfungsstatus und Aktionsergebnis, ohne Asset-ID/Token/URL anzuzeigen. | implementiert, Native-/Python-Test |
| 263 | C++23 Policy-/UI-/Hitbox-Regressionen und Source-Contract-Tests gegen leere CAP, Race/Epoch, Upload-Erhalt und 16:9–32:9. | implementiert, synthetisch |
| 264 | VERSION/Projektfortschritt, Plan, Übergabe, Matrizen und vollständiger Mint x64 Build, NVIDIA GL CTests, Python, Pi5-kompatibler Headless, isolierter Xvfb-Start und GitHub Pi5 ARM64-CI. | CI-Status im letzten Chat-Abschluss verifizieren |

## Neuer Upstream-Abgleich vor GitHub-Push

17.10.2026: OpenGenesisLINK Server main = 8ec7ea255901260abdeabfc5f17eeb850930d851, sechs Commits vorwärts seit bf55a17524a9f3f175c56d2adeaa4d39083f446f. Diff: ausschließlich addon-modules/OGLAssetService/README.md, addon-modules/OGLAssetService/build.sh, .github/workflows/ogl-assetservice-addon-modules.yml und runprebuild.sh (vier geänderte Build-/CI-Dateien). runprebuild.sh baut über compile.sh das Addon; README bestätigt ausdrücklich: keine automatische ROBUST-Aktivierung, Legacy FSAssetService bleibt Default. Keine geänderte GetTexture-/GetMesh-/Avatar-CAPS, daher Viewer-Protokoll unverändert; Produktionsaktivierung und Token-/CAPS-Vertrag weiterhin getrennt zu prüfen.

## Sicherheits-/Kompatibilitätsgrenzen

- Neuversuche starten ausschließlich durch Nutzerinteraktion, wenn die vier automatischen Versuche ausgeschöpft und ihre finale Pause beendet sind. Höchstens zwei manuelle Freigaben je Terrain-Revision; Regionswechsel setzt sämtliche Freigaben zurück.
- Ein asynchroner HTTP-Download wird niemals dupliziert; bestehende erfolgreiche Layer bleiben erhalten. Antworten alter Region-/Material-Epochen werden bereits von material_scope.matches verworfen.
- Das Fenster zeigt die separat heruntergeladenen Layer. GPU-Textursichtbarkeit ist davon getrennt als tatsächlicher GPU-Draw-Wert gemeldet. Die F7-Statusanzeige ersetzt keinen authentifizierten Pixelvergleich.
- OpenGenesisLINK main @ bf55a17524a9: OGL AssetService ist disabled-by-default und serverseitig. KEIN Produktionsdeploy, keine automatische Änderung von GetTexture/GetMesh, keine Übernahme operator-only APIs/Secrets ohne gesonderten Auth-, CAPS- und Sicherheitsvertrag.
- Kein Login gegen das echte NexVerse-Grid oder Firestorm-Paritäts-E2E in diesem Block nachgewiesen. Server-Bakes/Outfits und Windows/macOS weiterhin offen.

## Testnachweise

- Native: nexverse-terrain-detail-retry-tests, nexverse-terrain-recovery-status-tests, nexverse-viewer-shell-tests, plus vollständiges ctest --test-dir build/debug.
- Python: tests/test_alpha255_264_manual_recovery_contract.py und komplette python3 -m unittest discover -s tests -p test*.py.
- GUI: scripts/mint-isolated-gui-smoke.sh, Xvfb mit Software-GL statt sichtbarem Desktop.
- Runner: GitHub Actions pi5-ogl für ARM64-Headless (Ergebnis separat mit Run-SHA belegen).

**Bewertung: 39 % unverändert**, da sichere Bedienbarkeit und Diagnose einen begrenzten Teilbereich verbessern, aber der Kernfehlstand echter Avatar-/Welt-/Outfit-Funktionsparität fortbesteht.

Nächste zehn Blöcke: **Alpha.265–274** – serverautoritatives Self-AvatarAppearance/Bakes, eigene LLM-Texturen und tatsächliche Inworld-Pixelbelege; mit Firestorm-Fenstern und Aktionen statt bloßen Statusmeldungen.
