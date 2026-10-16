Neuester Serverreview 17.10.2026: OpenGenesisLINK main 5fcfc7f486660c8c27586e69eefc62cf0b9d22a1 (+2 Commits nach 8ec7ea; einzig bin/Robust.HG.OGLAsset.ini.example). Dieses vollständige Hypergrid-ROBUST-Beispiel ist EXPERIMENTAL, DISABLED BY DEFAULT und ausdrücklich nur für isoliertes Staging mit eigener DB/Assets/Secrets. Kein Laufzeit-Protokoll- oder Viewer-CAP-Wechsel, KEIN automatisches Aktivieren/Deployment.

# Alpha.265–274 – Avatar-Bakes: echte GUI und begrenzte GetTexture-Replanung

17.10.2026 | 0.2.0-alpha.274 | develop-current | Gesamtfortschritt **39 % (38,65 % gewichtete Schätzung, unverändert)**

## Zehn echte Entwicklungsblöcke

| Block | Tatsächliche Entwicklung | Nachweis |
|---|---|---|
| 265 | Viewer HEAD 72ca214 und OpenGenesisLINK-Server main 8ec7ea255901260abdeabfc5f17eeb850930d851 erneut überprüft. Neuester Patch nur Server-AssetService-Addon-Build/CI, keine neue AvatarAppearance/GetTexture-Wire-Schnittstelle. | GitHub SHA/Diff |
| 266 | Neuer C++23 AvatarBakeInspector für elf Firestorm-TE-Regionen Head/Upper/Lower/Eyes/Skirt/Hair/LeftArm/LeftLeg/Aux1–3. Fehlende Serverbakes sind nicht gleich noch nicht GPU-geladenen Texturen. | Native |
| 267 | Pro Head/Upper/Lower echter Original-LLM-GL-Draw; positiver Pixelbeleg nur nach aktivem SelfAvatar, kompletter ServerBake-GPU/GL-Draw und asynchronem GPU-Depth-Test. | Native |
| 268 | Strikte AvatarBakeRecheck-Policy für authentifizierte aktive Self-/Region-Epoche und angebotenen GetTexture-CAP. Maximal drei manuelle Replan-Aktionen je Region, zwölf Sekunden Pause. | Native |
| 269 | Missing-only Head/Upper/Lower mit Empty-/Placeholder-/Duplikat-Filter und bestehendem AssetRetrySchedule-Backoff. Kein Aushebeln vorheriger Fehlschläge. | Native |
| 270 | Tatsächlich ausgeführte Neuberechnung der Texture-Residency-Priorität im Hauptloop; normaler Downloadworker, Fairness, HTTP-Pacer, VRAM-/Staging-Budget, asynchrone Epoch-Sicherung bleiben zuständig. Kein direkter Extra-HTTP-Aufruf. | Core integration |
| 271 | Sichtbares Avatar-F3-Fenster mit Übersicht, Bake-Seite 1/2 und 2/2 mit elf Slots, echtem GPU-/LLM-GL-/Depth-Status. | Viewer GUI |
| 272 | B-Button und Tastatur B wechseln die Detailseiten, R-Button/Tastatur R löst die sichere Neuplanung aus; disabled-Fälle und Bildschirmgrößen 1100x700, 1920x1080, 3440x1440 getestet. | Layout/Hitboxes |
| 273 | Live-Hinweise zu Self-TE, Capability, Inflight, Backoff, Request-Quota und anforderbaren fehlenden Bake-Layern, ohne fremde Avatar-Namen, UUIDs, Tokens oder Admin-Zugangsdaten. | Native/Python source contracts |
| 274 | VERSION, Docs, Entwicklungsplan, Firestorm-/Server-Matrizen, CMake-Nativtest, Pi5-Headless, voller Mint-x64/NVIDIA GL Build, Xvfb+Systemicon, GitHub-Push+Pi5 CI. | FERTIG: Mint 141/141 native GL, 90/90 Python, Headless/Xvfb/Icon PASS; Pi5 Run 38980155018 SUCCESS auf 427555ab8ccb59adbbc41cd00bf576cbedc62f07 |

## Sicherheits- und Funktionsgrenzen

- Die Serverdaten kommen nur vom bereits implementierten autoritativen OpenSim AvatarAppearance-Decoder und sind an den aktuell authentifizierten Agenten und Regions-Lease gebunden. Neue Pixelbehauptungen aus serverseitigen UUIDs sind ausdrücklich verboten.
- Die Replan-Schaltfläche initiiert **KEIN** AvatarBake-Upload, **KEIN** neues AvatarAppearance-Paket und **KEIN** Netzwerkrequest außerhalb der bestehenden, durch Fehler-Backoff, Fairness, VRAM und Upload-Policy begrenzten GetTexture-Pipeline. Sie kann nur aktuell zulässige fehlende Server-Texturen früher einplanen.
- Drei explizite Replan-Versuche pro Materialepoche, frühestens alle 12 s. Regionswechsel setzt die Zähler zurück. Kein paralleler Worker und kein CAP/Session-Bypass.
- **KEIN** vollständiger Avatar-/Outfit- oder Firestorm-E2E-Pixelbeleg ohne realen authentifizierten NexVerse-Simulatorvergleich. Headless/GUI-Smoke beweist nicht, dass SelfAvatarAppearance vom laufenden Grid geliefert wird.
- Der OpenGenesisLINK-Server-head 8ec7ea25590126 enthält ein disabled-by-default AssetService-Addon, das zwar mitgebaut wird, aber weder automatisch deployed noch aktiviert werden darf. Keine unbekannten Secrets/Admin-Token in den Viewer.

**Projektfortschritt weiterhin 39 %.** Nächste verpflichtende 10 Blöcke: **Alpha.275–284**, mit Fokus auf echte AvatarAppearance-Serverantworten, komplette Bake-/Outfit-Texturen und GPU-Pixelnachweis samt Funktion und Oberfläche.

## Validierter Release-Gate-Abschluss

Featurecommit 427555ab8ccb59adbbc41cd00bf576cbedc62f07; GitHub Pi5 ARM64 CI Run 38980155018 **SUCCESS** auf exakt diesem SHA. Mint-CMake Debug x64 vollständig gebaut, 141/141 native C++23/NVIDIA GL, 90/90 Python, Pi5-kompatibler Headless-Test, versteckter Xvfb-Vollviewer und Original WM_CLASS/Systemicon erfolgreich. Kein echter Grid-Avatar-/Outfit-E2E-Test; Gesamtfortschritt 39 %.
