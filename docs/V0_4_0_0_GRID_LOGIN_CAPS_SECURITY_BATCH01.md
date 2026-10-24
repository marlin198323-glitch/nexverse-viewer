# v0.4.0.0 – Grid/Login/CAPS – Entwicklungsblock 01

24.10.2026 | Linux Mint Desktop Commander | develop-current

## 10 echte erledigte Teilblöcke

1. GitHub develop-current synchron, CHATUEBERGABE.md, ROADMAP.md, projectplan.md und alle authentifizierten Grid-/CAPS-/Login-Verträge gelesen. Baseline v0.3.0.0 abgeschlossen, nun Ziel v0.4.0.0.
2. Gemeinsames HTTP-URI-Gate gegen betrügerische user@host-Anteile und Authority-Verwechslungen gehärtet.
3. Port 1..65535, IPv6-Klammern, DNS-Labelbegrenzungen, Steuerzeichen, Backslash, URL-Fragmente und Längenlimits eingeführt; legitimes OpenSim http://hg.stadt-nexverse.de:8002/ und HTTPS-CAPS bleiben erlaubt.
4. Manipulierte GridInfo-Login-URIs überschreiben bei erfolgreicher HTTP-Antwort nie einen gültigen Login-Cache.
5. LoginService verwirft illegale Ziel-URIs vor dem sensitiven XML-RPC-POST; native Tests prüfen unveränderten bisherigen Zielhost/POST-Body.
6. Seed-CAPS und zurückgelieferte LLSD-EventQueue-/GetTexture-URLs verwerfen Authority-Spoofing, ungültige Ports und Token-Abzweigungsadressen.
7. Der libcurl-Transport folgt keinem GET-Redirect, weil CAPS-Bearer-Token in URL-Pfaden oder Querystrings weitergegeben werden könnten.
8. Der libcurl-Transport folgt keinem POST-307/308-Redirect, weil Passwort-Hash oder authentifizierter CAPS-Request-Body abfließen könnten; PUT/DELETE bleiben ebenfalls fail-closed.
9. Neuer echter lokaler HTTP-307-Server und libcurl-Client als UNIX-nativer CTest, prüft GET und POST und verlangt exakt eine Anfrage, keine Weiterleitung nach /stolen.
10. Native Login/CAPS/GridDiscovery/GridInfo-Tests plus zehn Python-Sicherheitskontrakte integriert; vollständiges Linux-Mint-x64-Build/CTest/Python/Xvfb-GPU/GUI/X11-Icon-Gate zur Abnahme.

## Offene v0.4.0.0 Kriterien

- T01 weiterhin IN_ARBEIT: echtes Grid/SSO, Sessionfehler, Passwortreset, authentifizierter Login.
- T02 OFFEN: EventQueue, Region-Handoff, Teleport und Reconnect mit aktuellem echten Simulator.
- T03 OFFEN: W/S/A/D/Yaw/Jump/Flight mit authentifiziertem Simulatorpositions-/Rotations-Readback.
- A01/A02/A03 noch OFFEN. v0.4.0.0 nicht vollständig und kein main-Release.
- Historischer GridInfo-Discovery-Endpunkt nutzt unverschlüsseltes HTTP: URL-Validierung allein kann keine Echtheit/Vertraulichkeit eines HTTP-Servers beweisen. Vertrauenswürdiges HTTPS und Serverendtests bleiben als gesondertes P0-Thema offen.
- Gesamtfortschritt unverändert 38,65 % gewichtet (39 %); v1.0.0.0 und v1.0.0.1 BLOCKED. Keine Produktivbereitstellung.

## Tests

Vollständiger Mint-x64-vcpkg/CMake-Neubuild 428 Schritte PASS. 185/185 native CTests PASS, inklusive echtem lokalen libcurl-307-HTTP-Redirecttest; 628/628 Python-Tests PASS, inklusive zehn neuen Grid-/CAPS-/Login-Sicherheitsverträgen. Isolierte echte Firestorm-Xvfb-GPU-/GUI-Abnahme startet Viewer v0.4.0.0, Original-133-Joint-Rig, 55 Attachpoints/7186 Dreiecke und echtes OGL-128x128-X11-Taskleistenicon PASS. OGL_MINT_X64_FULL_VALIDATION_PASS. NO_LIVE_GRID_LOGIN_OR_AVATAR_SKIN_COF_WASD_CLAIM. Exakte über GitHub bestätigte Feature-Code-SHA a4441680e3b261d4754afe2d2ca1e6e9e713758e. GitHub hat für diese SHA zum Prüfzeitpunkt keine Commit-Status-Einträge und keine PR-Workflow-Runs geliefert. ARM64-/Mint-GitHub-CI für EXAKTE Code-SHA UNVERIFIZIERT, daher NICHT als PASS gekennzeichnet. Lokaler Linux-Mint-Test 428/428 Build, 185/185 native CTests, 628/628 Python, isolierte original Firestorm-Xvfb-GPU-/GUI-/128px-Systemicon-Prüfung PASS. v0.4.0.0 bleibt IN ARBEIT, authentifizierter Grid-Endtest offen, kein main-Merge.
