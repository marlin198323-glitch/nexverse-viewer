# OGL v0.4.0.0 – P0: Bewegungsnachweis nach Teleport / Positionssprung

Stand 25.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current` | Ausgangscommit `cee143898da0c878f15955532b3b528d15c58b79`

## Technischer Fehler

Die bisherigen Beweismonitore für horizontale WASD-Richtung und vertikales Steigen/Sinken unterdrückten große Simulator-Positionssprünge (>= 32 m) als *Teleport ähnliches Ereignis*. Beide ließen jedoch die **bereits vor dem Sprung gesendete** Steuerungsfreigabe aktiv. Eine kleine spätere Positionsänderung (>= 0,25 m) konnte die **alte** Richtung bestätigen, obwohl nach dem Sprung noch kein neuer UDP-`AgentUpdate` gesendet wurde. Das ist eine falsche serverseitige Verifikationsaussage, nicht notwendigerweise ein Fehler in der eigentlichen Avatarbewegung.

## Zehn überprüfbare Bearbeitungsblöcke

1. Ausgangsbranch/Commit, sauberen Linux-Mint-Checkout, `CHATUEBERGABE.md`, `ROADMAP.md` und vorherigen CI-Stand geprüft.
2. Upstream `opengenesislink/opengenesislinkserver` auf relevante Änderungen geprüft (letzter Upstream-Commit `41d9466ca4fb3213d46620a4def15aec1fd98af8` betrifft WorldAPI AssetService).
3. Tatsächlichen `MovementFeedbackMonitor` mit socket-sent `AgentUpdate`, verifizierten selbst Avatar-Positionen und Regionsreset in `application.cpp` abgeglichen.
4. **WASD-Fix:** Ein großer Positionssprung deaktiviert die vorherige `active_`-Freigabe, verwirft `expected_direction_` und alte Steuerbits; nur eine spätere tatsächliche UDP-Sendeaktion darf die Beweismessung erneut starten.
5. **F3-Status WASD:** Nach Sprung sichtbar `Positionssprung/Teleport, neuer UDP-Versand ausstehend`, ohne behauptete erfolgreiche Serverbewegung.
6. **Steigen/Sinken-Fix:** `VerticalMovementFeedbackMonitor` widerruft bei Sprung altes `sent_`, alte Flug-/Richtungssignale und die Bestätigung; erst ein neuer tatsächlicher UDP-Send erlaubt einen neuen Höhenvergleich.
7. **F3-Status Höhe:** Neue `awaiting_fresh_udp_after_jump`-Momentaufnahme und unterscheidbarer Wartestatus. Abbruch der Tasteneingabe oder erneuter echter UDP-Send räumt die Warteanzeige korrekt auf.
8. Native C++-Regressionsfälle aktualisiert: Nach Teleport können weitere reine Simulatorpositionen und **nur lokal gequeue-te** Eingaben weder WASD noch Jump/Fly freigeben. Ein anschließender echter Socket-Send plus passende 0,25-m-Position führt erneut zu gültigem Nachweis.
9. Vier zusätzliche Python-Quellverträge prüfen Freigabegrenzen, F3-Texte, Testpfade und bestehende socket-sent-Callbacks/Regionreset.
10. Vollständige Linux-Mint-x64-Regression: **188/188 CTests**, **689/689 Python-Tests**, isolierter Firestorm-Xvfb-OpenGL-/GUI-Test und X11-Systemicon PASS.

## Verbleibende Abnahmegrenzen

Die Tests reproduzieren authentifizierungsnahe Zustände mit künstlichen Simulatorpositionen, **keinen tatsächlichen Live-Grid-Teleport**. Es ist weiterhin offen, ob das aktuelle NexVerse-Grid AvatarAppearance/Head-Upper-Lower-Bakes, vollständiges COF und serverseitig bestätigte Steuerung tatsächlich bereitstellt. Vor einem Release sind ein authentifizierter Login, ein Regionswechsel mit echten Logs/Readbacks sowie Verifikation nach Schließen erforderlich.

Ziel `v0.4.0.0`: weiterhin **IN_ARBEIT**, Projektgesamtfortschritt unverändert **38,65 % (39 % gerundet)**. Kein Release, kein Merge nach `main`, keine Änderung an Produktivinstallationen.
