# OGL v0.4.0.0 – Loginfehler `presence`: sichere Diagnose, lesbare Hinweise und lokale Logauswertung

**30.10.2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Ausgangscommit `45c68bc6d6eb97e6de6b3307613859e38d758de2`.**

## Ausgangsbefund

Der Avatar kommt aktuell nicht über die Loginphase. Die echten lokalen GPU-Viewer-Logs vom **30.10.2026, 21:22–21:33 Uhr** protokollieren insgesamt sieben fehlgeschlagene Logins mit `OpenSim login failed, reason: presence`. Die NexVerse-GridInfo wird geladen und die tatsächliche Login-URI als zulässig angenommen. Es gibt **keine** nachgewiesene gültige Simulator-Sitzung, keinen CAPS-/LLUDP-Handshake und keine aktualisierten Inworld-/Avatar-RGB-Bakes.

Die aktuelle OpenGenesisLINK-Serverquelle `OpenSim/Services/LLLoginService/LLLoginResponse.cs` verwendet denselben XML-RPC-Reason-Code **`presence`** für **drei** Fehlerantworten: `AlreadyLoggedInProblem` (Avatar bereits online), `LoginBlockedProblem` (Logins beschränkt, Konto-/Client-/UserLevel-Regel), `UnverifiedAccountProblem` (Konto noch nicht bestätigt). `LLLoginService.cs` weist unter anderem ein Online-`GridUserInfo` mit gültiger `LastRegionID` als `AlreadyLoggedInProblem` zurück. Aus `reason=presence` **allein** lässt sich die konkrete Ursache nicht unterscheiden. Der Viewer-Log schrieb bislang ausschließlich das mehrdeutige Feld `reason` und nicht die separate, entscheidende Servermeldung.

## Zehn bearbeitete Entwicklungs- und Prüfblöcke

1. `CHATUEBERGABE.md` und `ROADMAP.md` zuerst auf Linux Mint gelesen; sauberen Git-Branch `develop-current` und identischen GitHub-Commit verifiziert.
2. Beide GitHub-CI-Workflows des Ausgangscommits kontrolliert: Source-Check und vollständiger Linux-x64-Viewer-Build **PASS**.
3. Die aktuellen lokalen GPU-Logs mit letzten `presence`-Ablehnungen zeitlich und technisch abgeglichen; authentifizierte Simulator-/Avatar-Vorgänge weiter nicht nachgewiesen.
4. Die **echte** serverseitige Definition von `LLFailedLoginResponse` und die `LLLoginService`-Entscheidungszweige aus `opengenesislink/opengenesislinkserver` geprüft. Keine Empfehlung zum blinden Löschen von Presence-Daten.
5. `protocol/login_failure_diagnostics.hpp` ergänzt: `presence_already_online`, `presence_restricted`, `presence_unverified` und **`presence_unknown`** anhand der bekannten Servertextmuster; bei fehlender/unbekannter Servermeldung ausdrücklich **keine unbegründete Diagnose**.
6. Im tatsächlichen Login-Fehlerzweig `src/app/application.cpp` kurze deutsche Hinweise aus festen Strings eingesetzt. Die übrige Loginbehandlung, ToS-Zustimmung, einmalige XML-RPC-POST-Anforderung, Login-Identität und Circuit-Startbedingung bleiben unverändert. **Keine automatische Login-Retry-Schleife und kein serverseitiges Session-Clearing.**
7. Das Log schreibt statt beliebigem untrusted `reason` nur noch einen **allowlist-basierten Kategoriecode**. Passwort, XML-RPC-Response-Nachricht, URL, Avatar-/Session-ID und unbekannte Reason-Inhalte werden nicht hinzugefügt.
8. Die native Login-Testdatei um reale serverseitige `presence`-Fixtures für die drei Kategorien, ambige Antwort, potenziell geheimhaltige URLs/Token, überlange Rückmeldung und erhaltene ToS-Logik erweitert. Zusätzliche Python-Verträge prüfen sichere Diagnose, unveränderten Authentifizierungsweg und eigene Tests.
9. `scripts/ogl-login-diagnostics.py` als **reines Lese- und Auswertungswerkzeug** ergänzt. Standardmäßig nur seit dem letzten Viewerstart; mit `--all-sessions` alle Anmeldefehler. Es gibt ausschließlich Uhrzeiten und zugelassene Kategorien aus, **keine Lognachrichten**, Passwörter, URLs oder Kontodaten. Sieben eigene Tests prüfen Filter, manipulierte Kategorien und CLI.
10. Die kompakte 60px-Loginleiste behält die Original-Welcome-Fläche. Die bisherige 86-**Byte**-Abschneidung für Statusmeldungen wurde durch an realer Schriftbreite orientiertes, UTF-8-codepoint-sicheres Kürzen mit `...` ersetzt, ohne den Header/Footer zu vergrößern. Native Neon-Layouttests prüfen kleine/große Bildschirmmaße, deutsche Umlaute und Renderer-Grenzen; weitere Python-Verträge schützen das Layout. Vollständiges Linux-Mint-x64-Gate: **195/195 native CTests, 872/872 Python-Unittests, Firestorm-Xvfb-/OpenGL-/GUI-Abnahme und X11-Systemicon PASS**. Nach der Änderung am alten ToS-Source-Vertrag wurden sämtliche Tests erneut ausgeführt.

## So liest du die Login-Diagnose auf Linux Mint

Aus dem Projektverzeichnis:

```bash
python3 scripts/ogl-login-diagnostics.py
python3 scripts/ogl-login-diagnostics.py --all-sessions
```

Ein künftiger Loginfehler bekommt einen Kategoriecode, zum Beispiel `presence_already_online` oder `presence_restricted`. Die **alten** Logs enthalten nur den unspezifischen Code `presence`; rückwirkend ist daraus keine genauere Ursache ableitbar.

## Notwendiger nächster Schritt auf dem Gridserver (nur lesend)

Der ROBUST-/OpenGenesisLINK-Administrator prüft die `[LLOGIN SERVICE]`-Einträge zum konkreten Loginzeitpunkt, bei Bedarf die `[OGL VIEWER ACCESS]`-Annahmeprüfung und folgende Zustände: Kontofreigabe (`LoginAllowed`), `UserLevel` gegenüber `MinLoginLevel`, `AllowedClients`/`DeniedClients`, `GridUserInfo.Online` samt `LastRegionID` und die vorhandene Presence-Session. Eventuelle Server-Log-Zeitzonen beachten. Die älteren OGL-Viewer-Logs enthielten keine eindeutige Antwortnachricht, deshalb ist die Diagnose vorerst **serverseitig offen**.

**Keine** Nutzerkonten, `GridUser`-/Presence-Datensätze, Datenbanken, Zugangsschlüssel oder Login-Server-Konfigurationen wurden verändert. Es wurden für diese Arbeit keine neuen persönlichen Loginversuche oder POSTs ausgeführt.

## Noch offen

Die echte Anmeldung des Avatars gegen den NexVerse-Gridserver, CAPS-/LLUDP-`UseCircuitCode`, World-/Teleport-Handoff, echte Head-/Upper-/Lower-Bake-RGB 3/3 und COF-Outfit, WASD/Flug-Serverposition und der authentifizierte Shutdown-Nachweis. Das lokale Xvfb-GUI-Gate darf nicht als Live-Grid-Abnahme bezeichnet werden.

**Zielversion `v0.4.0.0` IN ARBEIT, gewichteter Gesamtfortschritt weiterhin 38,65 % (39 % gerundet), kein `main`-Merge oder Release.**
