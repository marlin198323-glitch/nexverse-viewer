# OpenGenesisLINK – CAPS/EventQueue

Stand: 0.2.0-alpha.47 · 10.10.2026

## Anlass

Im Live-Test von 0.2.0-alpha.46 war die EventQueue zunächst aktiv,
danach traten in etwa 36–40-Sekunden-Abständen wiederkehrende
Warnungen auf. Der Viewer hatte bislang 35 Sekunden HTTP-Total-
Timeout. OpenSimulator hält die EventQueue im Normalfall bis zu
58 Sekunden offen (60-Sekunden-Keepalive minus 2 Sekunden).

OpenSimulator-Referenz:
https://github.com/opensim/opensim/blob/master/OpenSim/Region/ClientStack/Linden/Caps/EventQueue/EventQueueGetModule.cs

## Rückmeldungen und Behandlung

| HTTP/Transport | Klassifikation | Aktion |
|---|---|---|
| 200, gültiges LLSD | Event-Batch | Ack aktualisieren, Events dispatchen, Fehlzähler zurücksetzen |
| 502 | Leere EventQueue | Kein Fehler; Ack bewahren, erneut pollen |
| 404 | Abgelaufene EventQueue | Sofort Seed-CAP `EventQueueGet` neu erfragen, begrenzte Retry-Pause |
| 503, übrige HTTP-Fehler | Fehler | HTTP-Status und Ursache loggen, Backoff |
| TCP-/HTTP-Timeout | Fehler | Ursache loggen, Backoff, periodisch Seed-Neuanfrage |
| 200, defektes LLSD | Protokollfehler | Ursache loggen, Ack unverändert, Backoff |

Bei normalem 502-Idle wird mit 150 ms Abstand neu gepollt, um
einen Hotloop bei falsch konfigurierten Servern zu verhindern.

CAPS-Kommunikation: POST `application/llsd+xml` mit
`ack` (letzte Batch-ID oder undef) und `done=false`.

## Recovery

- Connect-Timeout 8 Sekunden, Poll-Total-Timeout 80 Sekunden.
- Echte Fehlversuche: 1 / 2 / 4 / 8 / 12 Sekunden Retry-Pause.
- Seed-Re-Discovery bei jeder dritten echten Störung oder
  unmittelbar bei HTTP 404.
- Gleiche EventQueue-URL: Ack erhalten. Geänderte URL:
  Ack zurücksetzen und Queue neu aufbauen.
- Gesundes Idle (502) oder gültige Events setzen Fehlzähler zurück.
- Shutdown-Stopp wird während des Retry-Backoffs in
  maximal 100 ms berücksichtigt, nicht erst nach ganzer Pause.
- CAPS-Token und volle Capability-URLs niemals loggen.

## Abnahme und Grenzen

Tests prüfen 502, 404, 503, TCP-Timeout, malformed LLSD,
Ack-Transport, 80-Sekunden-Timeout, Retry-Backoff und
Re-Discovery-Plan. Debug-Build, 23/23 CTest und sechs Sekunden
OpenGL-GUI-Smoke erfolgreich.

**Ein neuer authentifizierter In-World-Langzeittest mit
0.2.0-alpha.47 fehlt noch.**

Der Worker verwendet weiterhin die **ursprüngliche Login-
Seed-Capability**. Ein Regions-/Teleportwechsel kann die alte
EventQueue nicht automatisch durch die Queue einer anderen Region
ersetzen. Dieser vollständige Circuit-/Queue-Wechsel kommt im
separaten Teleport-Lifecycle-Block.
