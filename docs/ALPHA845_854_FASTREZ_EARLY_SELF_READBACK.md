# Alpha.845–854 – OpenGenesisLINK FastRez: frühe Avatar-/Outfitantworten nicht verlieren

Stand: 23.10.2026 | Linux Mint jam-MS-7D28 | Branch develop-current
Version 0.2.0-alpha.854 | Gewichteter Projektfortschritt 39 % (38,65 %)

## Zehn Entwicklungsblöcke
845. Sauberen develop-current Checkout und letzte SHA cd6f223
auf Linux Mint geprüft; CHATUEBERGABE.md und projectplan.md
zuerst gelesen. Pi5 Alpha.844 SHA 712dbe9 Run 39840873191
war SUCCESS.

846. OpenGenesisLINK Servermain aktualisiert: zuvor 7396897d,
jetzt 8b3d80e (9 Commits, #276–#284). #281 FastRez
setzt opt-in WearablesRequestDelayMs=1000 statt 4000,
Near-First 64m ObjectUpdate; Wire-Format von
AvatarAppearance/AgentWearablesUpdate bleibt gleich.
Neue optionale OGLAssetService/FSAssets-Funktionen
sind serverseitig, KEINE Migration/Viewer-API nötig.

847. ECHTEN initialen Race-Fehler identifiziert:
Der LLUDP-Thread kann Low158 und Low382 empfangen,
während WorldModel.region().connected im UI-Hauptthread
noch false ist (CircuitState::active noch nicht gespiegelt).
Die Low158-Callback warf die einzige frühe eigene
AvatarAppearance weg, Low382 wurde ebenfalls nicht
akzeptiert. FastRez reduziert die Antwortwartezeit
und kann diese verlorene Avatar-/Outfitmeldung begünstigen.

848. Im authentifizierten WorldModel eine strikt gebundene
Zwei-Slot-Zwischenablage für früh eintreffende eigene
Low158-AvatarAppearance und Low382-AgentWearablesUpdate
implementiert. Maximal ein strukturell valides Paket
je Nachrichtentyp, keine unbeschränkte UDP-Warteschlange.

849. Low158-Staging prüft aktuelle Agent-ID,
Login-Session, Regionhandle/-generation, vollständige
Firestorm-TextureEntry, nicht-Trial und VisualParams.
Bei vorübergehendem Circuitstatus false erfolgt
KEINE voreilige COF-/Skin-/WorldModel-Bestätigung.

850. Alte Replay-Antworten werden durch letzten echten
AppearanceVersion/COF-Vergleich sowie für Low382 durch
SerialNum-U32-Wrap-sichere Reihenfolge nicht an Stelle
neuerer Vorabdaten übernommen. Falsche Agent/Session/
Regiondaten können das Staging nicht aktualisieren.

851. Sobald die aktuelle Region, Session und der
aktive Circuit tatsächlich übereinstimmen, werden
beide gespeicherten Simulatorpakete einmalig über
die bisherigen WorldModel-Authentifizierungs-,
COF-Antirollback- und Wearables-Serial-Gates verarbeitet.
Daten aus anderer Generation/Session werden verworfen.

852. Alle aktuellen Login-/Regionwechsel-/Logout-
Cleanup-Pfade löschen den Vorab-Cache. Die echte
Hauptschleife meldet erfolgreiche nachträgliche
Low158-/Low382-Übernahme korrekt in den vorhandenen
F3-Diagnosezählern und zeigt keine erfundene
sichtbare Skin oder gefälschte Simulatorbewegung.

853. Native C++23-Regression prüft FastRez-Frühantworten,
spätere Freigabe exakt einmal, COF-17 + trailing-0,
ältere COF/Serial-Replays, fremde Agent/Session,
Trial/ungültige Texturen, Loginwechsel, Regionswechsel
und Reset. In Mint-CMake und Pi5-ARM64-WorldModel-CI
eingebunden. Python-Realpath-Vertrag sichert echten
Low158-/Low382-Handler und Circuit-Freigabestelle.
Voller Mint-x64/GUI-/Python-/native Gate vor Push.

854. VERSION, projectplan.md, CHATUEBERGABE.md, AGENTS,
Fortschritt-JSON und Firestorm/Server/UI-Matrizen
aktualisieren und nach erfolgreichem Release-Gate
Code nach GitHub develop-current pushen. Pi5-CI für
genaue SHA prüfen; Hosted x64 von lokalem Mint-Gate
unterscheiden. Kein Produktivdeploy.

## Offener P0 End-to-End-Test
Diese Korrektur verhindert verlorene frühe Antworten bei
FastRez und nicht rechtzeitig veröffentlichtem Circuit.
Sie beweist KEINE wirkliche authentifizierte NexVerse-Session
mit AvatarAppearance, Skin-/Outfit-/COF-GPU-Pixeln oder
simulatorbestätigter WASD-/Yaw-Bewegung. Weiter 39 %.
Nächste Alpha.855–864.

## Vollstaendiger Linux-Mint-x64-Release-Test
scripts/mint-viewer-validation.sh EXIT 0:
CMake/vcpkg x64 Debug Vollviewer,
178/178 native CTests einschließlich neuem
nexverse-early-avatar-readback-tests;
412/412 Python;
isolierter Xvfb Viewer 0.2.0-alpha.854,
Firestorm Original-Skelett 133 Joints,
drei GPU-residente LLM Bodymeshes/7186 Dreiecke,
X11 WM_CLASS + Originalicon 128x128 PASS.
Kein Live-Login oder Produktivdeploy.
