# OGL v0.4.0.0 – optische Referenzabnahme, 25.10.2026

## Ausgangspunkt
Verglichen wurden die beiden Nutzerscreenshots: Neon-Designreferenz 1672x941 sowie echter Linux-Mint-Viewer 2048x838. Die UI wurde im isolierten Xvfb-Vollrenderer bei 1648x928 und 2048x838 zusätzlich als echte OpenGL/WebKit-Screenshotaufnahme kontrolliert. Kein Bildmockup, keine künstlichen Grid-Daten und keine Veränderung der URL https://stadt-nexverse.de/service/welcome.

## Ursachen / implementierte Korrekturen

1. **Proportionen bei ultrabreiten Fenstern:** Die vorherige Viewerskalierung `min(width/1648,height/928)` machte Texte/Elemente trotz großer Breite zu klein. Die UI-Skalierung ist nun an nutzbare Höhe 820 gekoppelt und auf 0,62..1,06 begrenzt.
2. **Dock-Balance:** Der Beginn der realen Formular-Hitboxen liegt in regulären Fenstergrößen bei ca. 26 % der Breite statt bei einem zu weit links fixierten 315px-Start. Der funktionierende Passwort-Sichtbarkeitsschalter bleibt unabhängig rechts verankert. Kleines Fenster erhält den gesonderten kompakten Fall.
3. **Welcome-Seite unverändert:** Zusätzlicher 7%-Dunkelschleier entfernt. Weiter Original-WebKit und dynamisch gelieferte Grid-Informationen und Nachrichten, kein Ersatz durch statisches Motiv.
4. **WebKit statt Texturverzerrung:** Die Browseraufnahme wird exakt in der verfügbaren Fläche zwischen Header und Dock angefordert und unverzerrt gezeichnet. Eine erste Version mit fester 810px-Browserhöhe bei kurzen ultrabreiten Monitoren zeigte zwar das Logo, schnitt aber den Nachrichtenbereich ab; sie wurde im selben Arbeitslauf verworfen.
5. **Korrektur nach grafischer Nachprüfung:** Für Desktopbreite ab 1450px und verfügbare Contenthöhe unter 810px verwendet WebKit stattdessen einen einheitlichen Browserzoom von `clamp(content_height/810, 0.80, 1.0)`. Das erhält NexVerse-Herologo **und** News-Karten im 2048x838-Screenshot, ohne vertikales Zusammendrücken. Bei Normalhöhe bleibt 100%-WebKit-Zoom.
6. **Inputs:** Gleicher äußerst dünner neutraler Rand im aktiven und inaktiven Zustand, kein Cyan-Fokus-Glow; Fokus nur durch minimale Änderung der Hintergrundfläche. Eingabefelder 205x32, Welt-Button 180x32 am 1648x928-Referenzpunkt.
7. **Textlesbarkeit:** Statuschips, Formularplatzhalter und Welt-betreten-Beschriftung vergrößert, ohne Header/Dock wieder aufzublähen. Header 58, Dock 60 px am 1648x928-Referenzpunkt.
8. **Korrektes asynchrones GTK-Resize:** Erst Window/WebView-Größe verändern, im anschließenden GTK-Pump den neuen Snapshot anfordern. Wechsel beim Login benutzt wieder ganze Fensterhöhe.
9. **Funktionalität:** Native GLFW-Hitboxen der Loginaktionen verwenden exakt das C++-Layout der OpenGL-Zeichnung. Passwortsichtbarkeit bleibt interaktiv; keinerlei SSO- oder Credential-Bypass.
10. **Testbeweise:** Native C++-Layouttest für sechs Bildschirmgrößen inklusive ultrabreiter 2048x838-Erweiterung, neue Python-Verträge für WebKit-Aufnahme/Zoom, Original-WebKit, Fokus, echte Hitbox und native App-Bildaufnahmen in isoliertem Xvfb. Screenshotprüfung belegt die Präsenz von Hero-Logo und News-Karten bei 2048x838.

## Weiterhin getrennt abnehmen

Die HTML-Seite ist weiterhin eine WebKit-Snapshotdarstellung und kein vollständiger interaktiver Browser. Ihre Seitenschaltflächen bleiben Aufgabe der Portal-/WebKit-Interaktionsintegration. Die nativen Loginfelder sind real und getestet. Der echte authentifizierte Login, Weltbewegung, Skin/Outfit, Teleport sowie die vollständige Exit-Dauer nach angemeldeter Sitzung sind nicht durch Design-/Xvfb-Tests bewiesen.

**Keine Releasefreigabe.** Zielversion `v0.4.0.0` bleibt IN_ARBEIT, gesamtes Projekt 38,65 % (~39 %). Entwicklung nur auf Linux Mint `jam-MS-7D28`; `main` unverändert.
