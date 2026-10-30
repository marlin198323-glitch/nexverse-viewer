# OGL v0.4.0.0 – Login-Blocker `presence_restricted`: XML-RPC-Version an OpenGenesisLINK-Zulassungsregel anpassen

**30. Oktober 2026 | Linux Mint `jam-MS-7D28` | Branch `develop-current` | Basiscommit `53646d9e0e0f62298f2f91058546d47f406dd074`.**

## Reproduzierter Fehler und aktuelle Loglage

Die aktuellen **authentifizierten Loginversuche** wurden vom Gridserver am 30. Oktober um **21:50:39, 21:53:16 und 21:57:29** jeweils mit **`presence_restricted`** abgelehnt. Die ursprünglichen `reason: presence`-Einträge waren mehrdeutig; seit dem letzten Diagnostik-Commit wird die vom Server zurückgegebene Ablehnung korrekt als `presence_restricted` eingeordnet. GridInfo-Discovery/HTTP-Login-Verbindung funktionieren, aber kein Simulator-Circuit und kein Avatar-Bake sind aktiv.

Das CMake-Buildlabel und `core::version()` des tatsächlichen Viewers lauten **`v0.4.0.0`** (aus der repo-eigenen Datei `VERSION`). Bisher übergab `src/app/application.cpp` diesen String unverändert an `LoginService::login(..., "OpenGenesisLINK Viewer", viewer_version)`, und `src/protocol/login.cpp` schrieb ihn in das XML-RPC-Feld `<name>version</name>`.

Der aktuelle OpenGenesisLINK-Servercode definiert in `OpenSim/Framework/OglApprovedViewerPolicy.cs` das Kriterium `IsNumericVersion`: Das erste Zeichen des XML-RPC-`version`-Wertes **muss eine Ziffer sein**; ein führendes `v` wird abgelehnt. Die echte Viewer-Kanalkennung `OpenGenesisLINK Viewer` ist an sich zugelassen. Die Serverseite kann das Paar (`version=v0.4.0.0`, `channel=OpenGenesisLINK Viewer`) deshalb vor Kontoprüfung und Presence-Anlage als `LLFailedLoginResponse.LoginBlockedProblem` zurückweisen. Alle `LoginBlockedProblem`-Quellen verwenden denselben äußeren Code `presence_restricted`. **Die konkrete Ablehnungsursache beim Live-Grid muss trotzdem noch mit einem erneuten echten Login oder ROBUST-Log geprüft werden**, da serverseitig zusätzlich `AllowedClients`, `DeniedClients`, Konto-`LoginAllowed` und `MinLoginLevel` greifen können.

Upstream-Referenzen:
- <https://github.com/opengenesislink/opengenesislinkserver/blob/08bf15f306bb4f882f24f32b84eb8df3a3175cea/OpenSim/Framework/OglApprovedViewerPolicy.cs>
- <https://github.com/opengenesislink/opengenesislinkserver/blob/08bf15f306bb4f882f24f32b84eb8df3a3175cea/OpenSim/Services/LLLoginService/LLLoginService.cs>

## Zehn tatsächlich ausgeführte Arbeitsblöcke

1. `CHATUEBERGABE.md`, Version, Roadmap und den sauberen Linux-Mint-Branch zuerst gelesen; lokalen und Remote-HEAD `53646d9` abgeglichen.
2. GitHub-CI geprüft: Source Check der Basis **PASS**, vollständiger Linux-x64-Build beim ersten Check noch in Bearbeitung.
3. Neueste lokale Viewer-Logs mit `scripts/ogl-login-diagnostics.py --all-sessions` ausschließlich lesend ausgewertet: **8 ältere `presence`**, **3 neuere `presence_restricted`**. Keine Passwörter, Accountnamen oder Session-URLs ausgegeben.
4. Aktuelle, relevante OpenGenesisLINK-Serverquelle über GitHub überprüft: `OglApprovedViewerPolicy.IsAllowed(clientVersion,channel)`, `IsNumericVersion` und sämtliche potentiell verwechselbaren `LoginBlockedProblem`-Ursachen.
5. Lokalen `VERSION`-Wert `v0.4.0.0` bis zum echten `LoginService::login` und `build_login_xml_rpc` nachverfolgt. Nachgewiesene Inkompatibilität: Der Server akzeptiert als numerischen Versionswert **`0.4.0.0`**, nicht **`v0.4.0.0`**.
6. Im gemeinsamen Protokoll-Header `ogl_login_wire_version()` ergänzt: Nur bei einem wirklich numerisch beginnenden `v`-/`V`-Label das reine Darstellungspräfix entfernen. Andere Werte, alte `0.1.0-alpha.1`-Testaufrufe und echte Vieweridentität unverändert.
7. Im zentralen XML-RPC-`build_login_xml_rpc()` die Version vor der Übertragung normalisiert; angezeigte Version, Desktop-Paketversion, Versionsdatei, Kanalname, Passwort-Hashing, ToS-Felder und alle Gridadressen bleiben unverändert.
8. Eine **rein öffentliche** Protokolldiagnose vor dem Login-Request ergänzt: Kanal und tatsächlich gesendete Wire-Version werden protokolliert, **keine** Zugangsdaten, URL-Token oder Sessionparameter. Beim nächsten authentifizierten Benutzer-Login ist die Korrektur in `viewer.log` belegbar.
9. Native `tests/login_tests.cpp` um Abnahme des **tatsächlich serialisierten** TinyXML2-XML-RPC-`channel`-/`version`-Werts, `LoginService`-Transport, `v`-/`V`-/numerischem Label, unverändertem Legacy-Versionstring und Geheimnisfreiheit erweitert. Zwölf neue Python-Vertragstests. Das Testfixture prüft XML-Member-Inhalt unabhängig von Formatierung. Lokale Zieltests PASS.
10. Gesamtes Linux-Mint-Gate durchgeführt: **195/195 native CTests, 884/884 Python-Unittests, isolierter original Firestorm-Xvfb-/OpenGL-/GUI-Test und X11-Systemicon PASS**. Danach Projektplan/Übergabe/Roadmap/JSON synchronisieren, Commit und Push nach `develop-current`, GitHub-CI prüfen. Kein `main`-Merge.

## Nächster manueller Schritt und Sicherheitsgrenzen

Die neue Debug-Binary lässt die GUI-Version weiterhin als `v0.4.0.0` erscheinen, sendet jedoch `version=0.4.0.0` mit `channel=OpenGenesisLINK Viewer`. Beim nächsten Loginversuch auf dem **neu gebauten** Viewer müssen folgende neuen Zeilen geprüft werden:

- `OpenSim login client identity: channel=OpenGenesisLINK Viewer; wire_version=0.4.0.0`
- entweder `OpenSim login succeeded; simulator session established` plus LLUDP/CAPS-Belege oder ein exakt klassifizierter neuer Fehler, z. B. `presence_restricted`.

Es wurde **kein automatischer Login mit Benutzerpasswort ausgeführt**, keine fremde Viewerkennung vorgetäuscht und keine Datenbank/Presence-Sitzung geändert. Für einen weiter bestehenden `presence_restricted` müssen gezielt die ROBUST-`[OGL VIEWER ACCESS]`- und `[LLOGIN SERVICE]`-Zeilen des Loginzeitpunktes und die Freigaberegeln (`AllowedClients`, `DeniedClients`, `LoginAllowed`, `UserLevel`, `MinLoginLevel`) untersucht werden.

Der Viewer verwendet derzeit eine vom Grid öffentlich veröffentlichte **HTTP**-Loginadresse; diese Änderung ersetzt **keine HTTPS-Transportverschlüsselung**.

**Offen:** erfolgreicher authentifizierter Login, CAPS/LLUDP, Region/Teleport, WASD-/Flug-Server-Readback, echte Head-/Upper-/Lower-RGB-Bakes 3/3, COF/Outfit und Schließdauer nach Login. Zielversion `v0.4.0.0` bleibt **IN ARBEIT**, Gesamtfortschritt **39 % (38,65 % gewichtet)**, kein Release.
