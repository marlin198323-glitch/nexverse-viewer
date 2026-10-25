# v0.4.0.0 – Kompakter OGL-Neon-Login und schnelleres Schließen

## Auftrag (25.10.2026)

Vergleich des realen Mint-Screenshots mit der gewünschten
Neon-Login-Vorlage: Header/Dock zu groß, zu breite Inputs,
auffälliger Fokusrahmen und langsames Schließen nach Login.

## Tatsächliche Implementierung, kein Bild-Mockup

1. Header bei 1648x928 von 93 auf **72 px** reduziert.
2. Login-Dock von 105 auf **77 px** reduziert.
3. Branding, Grid-Information und GRID/LOGIN/WORLD-Karten
   samt Abstand und Typografie proportional angepasst.
4. Avatarname/Passwort von 298x49 auf **245x39 px** reduziert.
5. Welt-Betreten-Button von 259x49 auf **210x39 px** reduziert.
6. Kein heller cyanfarbener Eingabefeld-Fokusrahmen mehr,
   nur sehr dezente Hintergrundtönung bei aktiver Fokussierung.
7. Echte WebKit-Welcome-Seite nutzt nun dynamisch die Fläche
   zwischen reduziertem Header und Dock.
8. GLFW-Eingaben und OpenGL zeichnen die gleichen Layoutrechtecke.
9. Threadsicherer HTTP-Transport-Stop: atomare `request_stop()`,
   libcurl-Progressabbruch über `CURLOPT_XFERINFOFUNCTION`
   und frühe Prüfung vor neuen Requests.
10. Beim Fensterschließen werden `circuit_stop`, `caps_stop`
    und `transport->request_stop()` vor dem GPU-Abbau ausgelöst.
11. Mit `Viewer shutdown cleanup elapsed_ms` wird künftig die
    tatsächliche Dauer bis nach dem Future-Join protokolliert.
12. Ein echter lokaler HTTP-Loopback-Test hält eine Antwort
    künstlich offen; die Anfrage wird bei Stop vor dem
    konfigurierten 4500-ms-Timeout abgebrochen.
13. Native Layoutprüfung für 800x600, 1100x700, 1280x720,
    1648x928, 1920x1080 und 3440x1440 sowie zusätzliche
    Python-UI-/Shutdown-Verträge.

## Noch offen

Der lokal verifizierte Netzwerkabbruch ersetzt keinen
tatsächlich nach Login gemessenen gesamten Viewer-Abschluss.
CPU-intensive `std::async`-Arbeit kann noch Zeit kosten.
Für echte Auth-Sitzung beim nächsten Test Viewer regulär
mit X schließen und den `Viewer shutdown cleanup elapsed_ms`
im Log auswerten. Keine Passwörter oder CAPS-Token teilen.

Die Projektversion bleibt v0.4.0.0 IN_ARBEIT,
gewichteter Stand 38,65 %. Skin/Outfit/WASD/Teleport noch
nicht vollständig end-to-end abgenommen; main unverändert.

## Lokale technische Abnahme

Linux Mint vollständiger CMake/vcpkg-Debug-Build PASS, **187/187 native CTests**, **666/666 Python-Tests**, original Firestorm Xvfb-OpenGL-/GUI-/128px-X11-Taskleistenicon PASS. Echter C++-Loopback-Test für laufenden HTTP-Abbruch PASS; aktive langlaufende Antwort etwa 0,5 s nach Stop-Signal vor einem konfigurierten 4,5-s-Timeout beendet. Der Header/Dock-/Input-Fokus-Test für sechs Auflösungen PASS. Authentifizierte vollständige Grid-Schließdauer bislang nicht gemessen.
