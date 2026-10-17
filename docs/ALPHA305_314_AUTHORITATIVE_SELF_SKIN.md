# Alpha.305–314 – bestätigte Avatar-Skin vor Teilpaketen schützen

17.10.2026 | Viewer 0.2.0-alpha.314 | develop-current | Gesamtfortschritt 39 % (38,65 % gewichtet, unverändert).

## Zehn Entwicklungsblöcke

| ID | Tatsächliche Entwicklung |
|---|---|
| 305 | Zuerst CHATUEBERGABE auf aktuellem GitHub-Branch geprüft, OpenGenesisLINK Server-main 7c6b0cf weiterhin unverändert und optionales OGLAssetService deaktiviert. |
| 306 | Datenfluss Low158 zu WorldModel und weiter zu originalem Firestorm-LLM-Rendering analysiert; reproduzierbare Ersetzungsgefahr gefunden, wenn ein Trial-/Teilpaket nach bestätigter Skin eintrifft. |
| 307 | Gemeinsame SelfAppearanceAdmission-Policy mit aktueller Region, richtigem Owner und explizitem Preview/Trial-Ausschluss implementiert. |
| 308 | Fehlende TextureEntry und fehlende VisualParams getrennt als nicht autoritative Pakete erkannt. Keine erfundenen Bake-UUIDs und kein Upload. |
| 309 | WorldModel.apply_self_appearance erzwingt Admission vor dem Ersetzen des gültigen vorhandenen Self-Snapshots. |
| 310 | Tatsächlicher Low158-Pakethandler nutzt dieselbe Admission. Trial-/Teilpakete erhöhen nicht die akzeptierte Server-Serial und lösen kein neues Skin-Streaming aus. |
| 311 | F3-Eigenavatarübersicht zeigt abgelehnte Preview-/Teilpakete und ob ein aktuell bestätigter Skin erhalten blieb. |
| 312 | Privater Diagnoseexport um ausschließlich numerische und whitelisted Trial-/Incomplete-Zähler erweitert, keine Identitäten, Asset-IDs oder CAP-URLs. |
| 313 | Native C++ Admission-, WorldModel-Nachfolge- und Diagnose-Tests, eigener Headless-Pi5-C++-Test sowie Python-Source-/GUI-Contracts eingebaut. |
| 314 | Mint-NVIDIA/x64-Build, CTests, Python, Pi5 Headless, isoliertes Xvfb/WM_CLASS/Systemicon, GitHub-Push und CI auf exaktem SHA, Dokumentation/Handoff/Projektfortschritt. |

## Bewertung

Ein vollständiger, bestätigter Simulator-Self-Snapshot bleibt bei einem nachfolgenden unvollständigen Paket erhalten. Trial-Preview-Daten sind nicht der autoritative Avatar. Der Report zählt die abgelehnten Pakete kumulativ für die Sitzung (nicht als jeweilige Regionsepochen-Summe). Die Änderung erzeugt keine serverseitige AvatarAppearance-Nachricht und keinen Bake-Upload. Authentifizierter NexVerse Self-Avatar-/Outfit-/Bake-/Mesh-Pixel-E2E bleibt OFFEN.

**Gesamtfortschritt: 39 % (38,65 % gewichtet), nächste Gruppe Alpha.315–324.**
