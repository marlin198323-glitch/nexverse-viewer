# OGL v0.4.0.0 – P0 Region-EventQueue Wiederanlauf

Datum: 25.10.2026. Arbeitsort: Linux Mint jam-MS-7D28 / Desktop Commander. Basis: develop-current `a6a4d16827e6a1526d87ee2b118a3e0dccc068ce`.

## Diagnose

Auf einem Regions- oder Teleportwechsel startet die Anwendung die neue CAPS-EventQueue nur einmal pro FirestormRegionLease-Epoche. Gibt der Zielsimulator beim ersten Seed-CAPS-Aufruf einen kurzfristigen Fehler (z.B. HTTP 503) oder vorübergehend kein EventQueueGet zurück, beendet sich der Worker mit `advertised=false`, während `region_event_started_epoch` gesetzt bleibt. Die Zielregion bleibt dann ohne EventQueue, bis eine weitere Regionsänderung eine neue Epoche erzeugt.

Zweiter Fehler: Nach HTTP 404 kann ein Simulator einen neuen EventQueueGet-Endpunkt an **derselben URL** wieder anbieten. Die bisherige Implementierung setzte den ACK-Zähler nur bei geänderter URL zurück und konnte dadurch den ACK des abgelaufenen Endpunkts erneut senden.

## 10 zusammenhängende Entwicklungsblöcke

1. **Aktuellen Branch/Plan/CI prüfen:** sauberer Checkout, Entwicklungsstand v0.4.0.0; Release auf main unverändert.
2. **Upstream prüfen:** opengenesislink/opengenesislinkserver letzter geprüfter Commit 41d9466ca4fb3213d46620a4def15aec1fd98af8 (WorldAPI-AssetService, kein passender CAPS-Protokollpatch).
3. **Epoche in Ergebnissen:** `RegionEventQueueStats.issued_epoch` wird vor jeder Abbruchbedingung gesetzt, auch bei anfänglicher Seed-Störung.
4. **Wiederanlauf-Steuerung:** neuer reiner `RegionEventRestartGate`; nur die Resultate der tatsächlich gestarteten Regionsepoche dürfen einen Retry auslösen.
5. **Gedrosselte Retries:** nach erfolgloser initialer CAPS-Entdeckung 5, 10, 20, 40 und maximal 60 Sekunden Wartezeit, kein Busy-Loop auf jedem Frame.
6. **Sichere Regionswechsel:** Retryfrist der vorherigen Region blockiert neue Epoche nicht, ältere Worker dürfen aktiven neuen Worker nicht zurücksetzen.
7. **Login-/EventQueue-Trennung:** bereits bestehende Login-CAPS-Epoche wird nicht doppelt gestartet, ein erfolgreich angekündigter Worker wird nicht blind erneut gestartet.
8. **ACK nach 404:** bei erfolgreicher Seed-Wiederermittlung ACK zurücksetzen, auch wenn der neue EventQueueGet-Endpunkt denselben URI-String hat.
9. **Exceptions:** Fehler der Regionqueue-Arbeit werden als epochgebundenes `advertised=false`-Resultat zurückgegeben; die Steuerung kann kontrolliert retryen, keine unkontrollierten Future-Ausnahmen.
10. **Regressionsbeweise:** native `nexverse-region-event-queue-tests` prüfen vorübergehende HTTP-503-Seed-Störung, Re-Discovery derselben URL nach 404, ACK-Folge [-1,17,-1], Regionlease-/Epochengrenzen, Fünf-/Zehn-Sekunden-Retry und gültige neue Region. Zusätzlich Python-Quellvertrag `tests/test_v0400_region_eventqueue_retry_contract.py`.

## Freigabegate und offene P0-Abnahme

Die lokalen Tests und Linux-Mint-x64-Xvfb-Firestorm-Checks müssen für exakt den zugehörigen Code-Commit grün sein. **Kein Claim einer echten authentifizierten Teleport-/Region-/CAPS-Session:** Es fehlen aktuelle reale Session-Logs sowie serverbestätigte RegionCrossing-/WASD-/AvatarAppearance-/COF-Bake-Beweise. Dieser Fix belegt sichere initiale Rekonnektierung in deterministischen Mock-Tests; er beweist nicht die Gesamtabnahme.

Status: `v0.4.0.0` IN_ARBEIT, Projektgewichtung 38,65 % (=39 % gerundet), kein main-Merge, Tag oder Deployment.
