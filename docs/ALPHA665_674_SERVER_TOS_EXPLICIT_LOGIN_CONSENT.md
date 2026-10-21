# Alpha.665–674 – OpenGenesisLINK versionierte Bürgerordnung, ausdrückliche Zustimmung

21.10.2026 | develop-current | OGL 0.2.0-alpha.674 | Fortschritt 39 % (38,65 % gewichtet).

## Verifizierter Serververtrag
Upstream opengenesislink/opengenesislinkserver main 7569838fd125d2338bb0a57ec88476648610d887. Gezielter Compare ab 0129e6179aaae6eec5cc9a6af7ff7ec6b1d4392b: 1 Commit, #260, 15 Dateien. Offizielle technische Doku: doc/NexVerse/FIRESTORM_LOGIN_WELCOME_TOS.md, tatsächliche LLLoginHandlers.cs und LLLoginService.cs. Server prüft Passwort vor TOS; Antwort bei fehlender Zustimmung: login=false, reason=tos, message=http://stadt-nexverse.de/buergerverordnung. Firestorm antwortet nach Zustimmung mit agree_to_tos=true; Server persistiert TOS-Version/UTC. Neuer TOS-Versionstand erzwingt erneute Zustimmung. Historischer Full-Compare zur reviewed-Baseline ist separat weiter offen.

**P0-FEHLER:** Bisheriger Viewer-XMLRPC-Login setzte agree_to_tos ohne Benutzeraktion IMMER auf true.

## Zehn Entwicklungsblöcke
665. Branch-HEAD/Handoff/Projektplan und Alpha.664 CI SHA überprüft. GitHub Pi5 Run 39643087562 für SHA 0c9f60ef02dbcc35796c7cdf637eb59b94ec48a9 erfolgreich.
666. Neuen OpenGenesisLINK Server-main / Login-TOS-Commit #260 einzeln mit C#-Loginhandler, Service und Dokumentation abgeglichen, Login-Antwort und true-Retry belegt.
667. Echtes XMLRPC-Feld im Viewer auf Default false umgestellt und explizite bool agree_to_tos-Option in LoginCredentials eingeführt.
668. Eigene verbindliche Zustandsmaschine: nur reason=tos vom Server und bekannte offizielle URL eröffnen Modal; Session-/Avatar-/Grid-Scope, kein Vorrats-Consent.
669. URL-Allowlist beschränkt Bürgerordnung auf offizielle http/https Adresse. Der Browser-Button öffnet stattdessen ausschließlich die feste HTTPS-Seite, ohne untrusted URL/Shell-Interpolation.
670. Sichtbares Pre-Login-Modal im eigentlichen Viewer mit Bürgerordnungslink, Dokument öffnen, erst danach aktiver expliziter Zustimmungs-Schaltfläche und Ablehnen.
671. Klick-/Keyboard-Sperre für den gesamten Hintergrundlogin, ESC lehnt ab. Browseröffnung über OS-Handler, Clipboard-Fallback; bewusstes erneutes Passworteingeben nach Zustimmung.
672. Tatsächliche Login-Future-Auswertung verarbeitet reason=tos, verhindert vorzeitigen Agent-/CAPS-/LLUDP-Start und begrenzt true auf Avatar/Grid-Kombination. Weitere Server-Challenge invalidiert alten Consent; erfolgreiche Session löscht ihn.
673. Native C++23-Tests für TOS-Server-Challenge, URL-Missbrauch, 3 Viewport-HitTests und User-Gesture sowie echte CMake-Login-Testfälle für XML-RPC false/true und reason=tos; Pi5-Headless-/Python-Vertragstests ergänzt.
674. Versions-/Projektplan-/Chatübergabe-/Kompatibilitätsdokumente aktualisiert, SHA-genaue Pi5 CI neuer Version separat abzuwarten. Kein Produktivdeploy.

## Grenzen und Folgestand
OGL verwendet den externen Systembrowser für den VOLLSTÄNDIGEN offiziellen Bürgerordnungstext, keinen eingebetteten Firestorm-Webview. Browserklick beweist nicht tatsächliches Lesen; menschliche ausdrückliche Zustimmung ist separat erforderlich. Bei fehlendem Browserhandler wird die feste HTTPS-Adresse kopiert; kein automatisch akzeptierendes Verhalten. Passwort wird nach erstem abgelehnten Login aus dem Request-State gelöscht und für den aktiven zweiten Login neu verlangt. Die lokale Native-/Python-Validierung beweist keinen realen Grid-Login oder GPU/UI-Framebuffer. Produktives TOS kann serverseitig deaktiviert sein; nicht behaupten, dass es bereits live verlangt wird.

Ältere ungeprüfte Upstream-Kompatibilitätscommits sind MANUAL_REVIEW; reviewed-SHA 4e99976630f1008db9014ea2b86c3723b09e9fe6 nicht ohne vollständigen Review ändern. Avatar-Skin, COF, Outfits, WASD und Firestorm-Parität in echter NexVerse-Sitzung noch offen. 38,65 % gerundet 39 %. Nächste zehn Blöcke Alpha.675–684.
