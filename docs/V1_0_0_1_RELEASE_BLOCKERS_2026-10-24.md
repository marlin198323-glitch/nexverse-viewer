# OGL Viewer – v1.0.0.1 Releaseprüfung (24.10.2026)

**BLOCKIERT – kein v1.0.0.1 Release erfolgt.**

## Real überprüfter GitHub-Ausgangsstand

- `develop-current`: SHA `d1e28f4074e4a405fecd1fedf8b5245f77e9d727` vor diesem Durchgang.
- `origin/main`: SHA `f4d4a93ecb9b032e0fa675b42cadb8c7ecff80c0`.
- Aktuelle Runtime- und Buildversion `v0.3.0.0`; `git tag -l 'v1*'` gab keine Tags zurück.
- v1.0.0.0 noch nicht mit einem GitHub-Release auf main veröffentlicht.
- 38,65 % Gesamtprojektfortschritt; 7/7 echte Release-Endbeweise fehlen.
- Keine nach v1.0.0.0 entstandenen Produktionsfehler für einen Hotfix.
- Die Release-CLI `--release-check v1.0.0.1` hat Exitcode 2.

## Implementiert statt vorzeitig veröffentlicht

- Eine unabhängige harte Vorgängerprüfung wurde zur Release-CLI hinzugefügt.
- Erforderlich sind ein annotierter stabiler Tag, identischer Tag auf
  dem Remote, ein main-Vorfahr-Commit mit VERSION v1.0.0.0 und
  eine identische, offiziell dokumentierte GitHub-Release-Veröffentlichung.
- Elf Regressionstests verwenden ein isoliertes Git-Testrepository.
  Ein gültiger künstlicher Release-Pfad wird akzeptiert; fehlende,
  leichte, inkonsistente oder ungepushte Tags und inkonsistente
  Belege werden zurückgewiesen.
- Die Roadmap und Projektübergabe kennzeichnen v1.0.0.1 als BLOCKIERT;
  die eigentliche Version bleibt unverändert bei v0.3.0.0.
- Kein Merge nach main, kein v1.0.0.1-Tag, kein GitHub-Release,
  kein Produktivdeployment.

## Für einen echten Hotfixrelease noch erforderlich

1. v0.4.0.0 bis v0.9.9.0 implementieren und real abnehmen.
2. v1.0.0.0 mit 100 % und sieben echten Grid/UI/Plattform-Nachweisen
   vollständig releasen: main-PR, exakte SHA-CI, Release-Tag und GitHub-Release.
3. Echte Post-v1.0.0.0-Produktionsfehler reproduzieren und beheben.
4. v1.0.0.1 inklusive Regression, Sicherheit, Upgrade/Rollback und
   Build/Installer auf der exakt geprüften Code-SHA abnehmen.
5. Erst dann konditional den v1.0.0.1-PR nach main mergen, taggen
   und den GitHub-Release publizieren.

Verbindlich: `docs/RELEASE_VERSION_POLICY.md`,
`docs/RELEASE_ROADMAP.json`,
`docs/GOAL_100_PERCENT_RELEASE_ACCEPTANCE.md`.

## Abnahme dieses Freigabeschutzes

Linux Mint, unveränderte öffentliche Entwicklungsversion v0.3.0.0:
**184/184 native CTests, 617/617 Python, vollständiger Debug-/vcpkg-Build
sowie Original-Firestorm-Xvfb-GPU-/GUI-/128x128-X11-Icon-Gate PASS**.
CLI `python3 scripts/ogl-version.py --release-check v1.0.0.1`
ergibt erwartungsgemäß **Exit 2 (BLOCKED, Vorgängertag fehlt)**.
Diese Prüfungen betreffen den technischen Release-Schutz, nicht eine
echte v1.0.0.1-Freigabe. Keine authentifizierten Grid-Live-Endtests
in diesem Arbeitsdurchgang und keine Änderung auf `main`.
