**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); Fortschritt 39 %; nächste Alpha.355–364.**

Alpha.354 CI VERIFIZIERT: Feature-SHA 2ed9f66ea96f49eb1b7ad2211aff87248c2b09c0; Pi5-ARM64 GitHub Actions 39079670826 SUCCESS für exakt diesen SHA. Mint x64 Build 352/352, native CTest 148/148, Python 132/132, ARM64-kompatibles Headless, Xvfb-Viewerstart und X11-Icon/WM_CLASS PASS. F3 WASD trotz offener Diagnose, Strg+S statt S, reale UDP-WASD-Datagramme und AvatarAppearance Fehlerabschnitt sichtbar. Authentifizierter NexVerse Bewegungs-/Skin-/Outfit-Nachweis weiterhin OFFEN. Gesamtfortschritt 39 %; nächster Zehnerlauf Alpha.355–364.

# Alpha.345–354 – F3-Bewegung, echte UDP-WASD-Versandnachweise und Live-Appearance-Fehler

18.10.2026 | OGL Viewer 0.2.0-alpha.354 | Branch `develop-current` | **Gesamtfortschritt 39 % (38,65 % gewichtet)**

## Zehn konkret abgearbeitete Entwicklungsblöcke

| Block | Tatsächlicher Code / visueller Effekt / Test |
|---|---|
| 345 | Aktuellen GitHub-HEAD, `CHATUEBERGABE.md`, Projektstand und OpenGenesisLINK Server main `7c6b0cf7252189707066f09ad66c732642b27af9` geprüft; keine neue LLUDP-Protokolländerung oder experimentellen Serverdeploy freigegeben. |
| 346 | Realen WASD-Blocker in `ViewerShellState::input_captured()` gefunden: jedes offene F3-Diagnosefenster unterband alle `AgentUpdate`-Richtungstasten. |
| 347 | Separates `movement_input_captured()` implementiert: Chat-/Teleport-Textedit sperren Bewegung, die nicht-modalen Avatar-/Regionsfenster nicht. Vorherige Bedienung der übrigen Fenster und Tasten bleibt erhalten. |
| 348 | F3-Berichtskürzel von einfachem `S` auf **Strg+S** geändert. `S` kann während der Diagnose regulär rückwärts laufen; sichtbare F3-Schaltfläche entsprechend beschriftet. |
| 349 | Gleichzeitiges `Strg+S` erzeugt keinen Rückwärtsimpuls in der 100-ms-WASD-Abfrage. Direkte native ViewerShell-Regressionen unterscheiden Textfenster und Diagnosefenster. |
| 350 | Der LLUDP-Circuit zählt nur `AgentUpdate`-Pakete nach erfolgreichem `send_datagram` am UDP-Socket; gesendete Richtungsbewegungen besitzen einen eigenen Zähler. Keine Behauptung eines Server-ACKs. |
| 351 | Begrenzte Circuit-Statusaktualisierung für die Versandzähler; das F3-Avatarfenster zeigt **UDP-WASD** zusammen mit der bestehenden unabhängigen serverseitigen Positionsrückmeldung. |
| 352 | Rechte Avatar-Pipeline-Seitenleiste zeigt bei ungültigem echtem Firestorm-`AvatarAppearance`-Paket die bereits vom produktiven Decoder klassifizierte Fehlstelle auch ohne geöffnetes F3, ohne Avatar-UUID/Session oder Rohpakete. |
| 353 | Erweiterte nativen C++-/OpenGL-ViewerShell- und Circuit-Verträge sowie Python-Regressionsverträge für WASD im F3, Strg+S, UDP-Socket-Zähler, Avatar-Fehlermeldung und kontinuierliche Statuswahrheit. |
| 354 | Vollständige Mint x64/GPU Build-/CTest-/Python-/Pi5-Headless-/Xvfb-/Icon-Gates, GitHub-Featurecommit und Pi5 ARM64 SHA-CI, anschließend Fortschritt und Handoff in allen relevanten .md und JSON auf `Alpha.355–364`. |

## Strikte Beweisgrenzen

`UDP-WASD` bedeutet **vom Client erfolgreich an den lokalen UDP-Socket übergeben**, nicht serverseitig ausgeführt oder zuverlässig quittiert. Die vorhandene `MovementFeedbackMonitor`-Anzeige bestätigt Bewegung ausschließlich anhand tatsächlich empfangener `ImprovedTerseObjectUpdate`-Serverpositionen. Eine weiterhin stationäre Position bleibt korrekt als unbestätigt markiert.

Die Live-Logdateien vom 18. Oktober bis 01:11 zeigten keine neue authentifizierte NexVerse-Sitzung mit den Änderungen aus dieser Runde. Daher ist nicht nachgewiesen, dass sich der Avatar jetzt im Grid bewegt oder seine echte Server-Skin einschließlich Outfits/Bakes angezeigt wird. `AvatarAppearance`-Decoder wurde bewusst nicht durch ungesicherte Toleranzen oder erfundene Bake-Assets umgangen.

Der vorherige Linux-Cinnamon-Systemicon-Fix und adaptive 256–384-Mesh-Residentgrenze bleiben erhalten; OGLAssetService am Server bleibt standardmäßig deaktiviert.

**Projektfortschritt bleibt 39 %.** Nächster Zehnerdurchlauf **Alpha.355–364**: authentifizierter Live-Login, F3-UDP-Zählung gegen Serverposition, genaue Parser-Failure-Phase, sichtbare eigene Bakes, Attachment-Transformationen und Terrain-Texturen prüfen.
