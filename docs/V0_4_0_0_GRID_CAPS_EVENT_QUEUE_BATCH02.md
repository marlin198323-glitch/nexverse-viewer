# v0.4.0.0 – Interaktive QA und EventQueue-ACK-Sicherheit (Batch 02)

## Zehn bearbeitete Entwicklungsblöcke

1. Linux Mint Commander, develop-current und CHATUEBERGABE.md/ROADMAP.md geprüft.
2. Roadmap mit zwölf Stufen und offizielles siebenfaches Release-Gate inventarisiert.
3. Offlinefähige interaktive Projektabnahme als selbstständiges HTML-/JS-Dashboard implementiert.
4. Versionswahl, Suche, Filter, pro Kriterium Status, Prüfer, Befund, Nachweis.
5. Lokale Zwischenspeicherung, revisionsgebundener Import-/Export und Druckansicht.
6. Fail-closed CLI für veraltete SHA, falsche Version, fehlende Belege und vorgetäuschte Freigabe.
7. Keine Manipulation von Roadmapstatus und final_release_evidence durch Browserbewertungen.
8. Echten Chromium-Headless-DOM-/JS-Selbsttest und zehn Python-Sicherheitsregressionen angelegt.
9. EventQueue ohne echte ACK-ID darf keine Events zustellen; leere idlose HTTP-200-Antworten werden gepaced. Native plus vier Source-Verträge.
10. Gesamten Mint-x64-Build/CTest/Python/Xvfb-Original-Firestorm-GUI-/Icon-Tests abschließen, GitHub pushen, Übergabe und Projektplan aktualisieren.

## Weiter offen

v0.4.0.0-T01 Login/SSO, T02 Region/CAPS/Teleport-Reconnect,
T03 WASD/Yaw/Jump/Flight und A01-A03 authentifizierte Grid-E2E
bleiben offen. Gesamter Viewer 38,65 % gewichtet. Kein main-Release.

## Nachweis der lokalen Gesamtabnahme

Linux Mint x64 voller CMake/vcpkg-Debug-Build, **185/185 native CTests**, **642/642 Python-Tests**, echter Original-Firestorm-Xvfb-GPU-/GUI-/128px-X11-Taskleistenicon-Test PASS. Interaktive Chromium-DOM-/JS-Aktion PASS. Das ist kein echter Grid-Login-, Outfits-, Teleport- oder WASD-/Yaw-Endtest und keine Releasefreigabe.
