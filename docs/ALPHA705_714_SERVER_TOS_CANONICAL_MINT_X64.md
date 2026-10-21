# Alpha.705–714 – Aktueller OpenGenesisLINK TOS-Vertrag auf Linux Mint

21.10.2026 | develop-current | Viewer 0.2.0-alpha.714 | Fortschritt 39 % (38,65 % gewichtet)

Primärsystem: Linux Mint Commander jam-MS-7D28, Checkout
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer. Raspberry Pi ausschließlich ARM64 CI.
Stand beim Arbeitsbeginn: synchroner, sauberer Branch HEAD 2dd6fe9, Übergabe und
Projektplan gelesen. Voriger Pi5 GitHub CI: Alpha.704 SHA eceb506 Run 39652812099 SUCCESS.
Gehostete x64 CI zuletzt vor Runnerzuweisung gescheitert; lokaler Mint x64 Full Build PASS.

## Zehn Entwicklungsblöcke

705. Sauberen develop-current auf Linux Mint, CHATUEBERGABE.md, projektplan.md
und SHA-genaue vorige GitHub Pi5 CI überprüft.
706. Neuen Servermain 31f3d12913a5136f071669682b866664eb895341 durch Einzelvergleich mit
c9a032206202ccdbe44d7d4788e34c4955f7d273 verifiziert: genau Commit #263.
OpenGenesisLINK LoginService fordert jetzt standardmäßig die offizielle
https://stadt-nexverse.de/service/tos. Firestorm erhält per HTTP denselben
Pfad als Redirect-Einstieg. Kein beliebiges TOS-Dokument.
707. Den konkreten OGL-Login-Blocker behoben: der Viewer ließ nur
/buergerverordnung zu und wies die neue gültige Serverantwort reason=tos
als nicht vertrauenswürdig zurück. /service/tos als kanonischen Pfad ergänzt.
708. Alte offizielle /buergerverordnung für installierte ältere Server eng
begrenzt beibehalten. Beide bekannten Pfade mit explizitem HTTP/HTTPS
und optionalem finalem Slash, aber niemals fremde Hostnamen, Userinfo,
Query, Fragment, Ports oder Path Traversal autorisiert.
709. LoginTosConsent trägt nun die feste HTTPS-Adresse der konkreten
vertrauenswürdig geprüften Server-Challenge im zustandsgebundenen Feld
document_url, nicht die rohe untrusted XMLRPC-Message. Reset löscht
die alte Auswahl. Zustimmung nach wie vor nur ausdrücklich.
710. Echter OpenGenesisLINK Login-UI-Modaldialog zeigt diese
aktuell ausgewählte Adresse. OS-Browser, Windows ShellExecuteA,
POSIX xdg-open und Zwischenablage-Fallback verwenden denselben
kanonisch erlaubten HTTPS-Pfad; keine Shell-Injektion.
711. Native C++23 URL-Sicherheitsregressionen für aktuellen
HTTP/HTTPS-TOS-Pfad, Legacy-Kompatibilität, Reset,
Avatar/Grid-Gültigkeit, offene/gesperrte Zustimmung und bekannte
URL-Manipulationen erweitert.
712. Echten LoginService XML-RPC-Parser-/Serializer-Test auf die
neue HTTPS /service/tos Challenge aktualisiert, Default-False und
nach expliziter Zustimmung True unverändert. Python-Quell-/UI-
Vertragstests ergänzen echte Modal-/Browserpfade.
713. HTTP-HEAD auf Linux Mint: offiziell /service/tos 200 OK,
Legacy /buergerverordnung ebenfalls 200. Beweist HTTP-Erreichbarkeit,
nicht vollständige juristische Textfassung oder erfolgreiche
serverseitige Zustimmungs-DB. Vor Freigabe lokaler Mint-x64-GUI-Gate
mit CMake/Vcpkg, 174 CTests, Python, isoliertem Xvfb und OGL X11-Icon
durchführen. Authentifizierten Serverzugang nicht erfinden.
714. Projektstand, Version, Chatübergabe, AGENTS, Firestorm/UI/Server-
Matrizen und JSON-Fortschritt aktualisieren, Code direkt auf Mint
testen und nach GitHub develop-current pushen. CI-Erfolg erst
nach exakter SHA-Überprüfung behaupten; kein Deploy.

## Unverändert offen
Realer Live-Login auf NexVerse samt TOS-Datenbank-Readback,
eigenes serverbestätigtes Low158 AvatarAppearance, GPU-sichtbares
Outfit/Skin/COF/Bakes und AgentUpdate/WASD/Turn sind weiterhin
NICHT als authentifizierte Ende-zu-Ende-Funktion geprüft.
Der globale Fortschritt bleibt bei 39 %; nächster Zehnerblock
Alpha.715–724 auf Linux Mint Commander.
