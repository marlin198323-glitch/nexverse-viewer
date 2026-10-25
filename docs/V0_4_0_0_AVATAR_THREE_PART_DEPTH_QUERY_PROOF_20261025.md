# OGL v0.4.0.0 – P0 Head/Upper/Lower: Asynchrone GPU-Depth-Beweise

25.10.2026 | Linux Mint `jam-MS-7D28` | `develop-current` | Basis `8cfb91e4befba43a48147f4289ebcb33c0fe2985`

## Auslöser / ursächlicher Codefehler

Die OpenGL-Renderengine arbeitet mit drei separaten, asynchronen Depth-/Fragment-Queries für Kopf, Oberkörper und Unterkörper des originalen Firestorm-LLM-Körpers. `self_avatar_visible_samples_known` wird bereits **true**, wenn eine beliebige der drei Abfragen abgeschlossen ist. Der bisherige `avatar_skin_blocker_key()` behandelte diesen Gesamtwert als vollständige Abnahme der Depth-Stufe. Beispiel: `known={true,false,false}`, `passed={true,false,false}`; obwohl Upper/Lower noch pending waren, erschien die Pipeline als `depth_visible_final_skin_pixels_unverified`.

Die Diagnose konnte so ein vollständiges 3/3-GL-/Skin-Sichtbarkeitsniveau behaupten, obwohl erst ein einziges Teilergebnis vorlag. Das betrifft die Beweis-/Statuslogik, nicht die tatsächliche OpenGL-Renderinggeometrie.

## Zehn bearbeitete Blöcke

1. Aktuellen GitHub-Branch, lokale unmodifizierte Dateien, `CHATUEBERGABE.md` und Roadmap zuerst geprüft; Ausgangs-Commit identisch.
2. Letztes Linux-Mint-CI und historisches Viewerlog vom 25.10. geprüft; kein neuer authentifizierter Avatar-Login seit Implementierung des Low158-Pipeline-Tracers.
3. GPU-Abfragepfad im originalen OpenGL-Renderer auf `GL_QUERY_RESULT_AVAILABLE`, eigenes Query-Epoch, drei Slot-Einzelergebnisse und vorzeitige `any_of`-Aggregation geprüft.
4. Reader der Live-Diagnose (`AvatarDiagnosticReport.avatar_depth_known/avatar_depth_passed`) mit realen Rendererdaten verglichen.
5. `avatar_skin_blocker_key` gehärtet: **Alle drei** GPU-Teilergebnisse müssen abgeschlossen sein, bevor `avatar_depth_fragments_zero` oder die anschließende Sichtbarkeitsstufe überhaupt erreicht werden kann.
6. Sind alle drei Abfragen abgeschlossen und keine meldet sichtbare Fragmente, ist `avatar_depth_fragments_zero` korrekt. Meldet mindestens eine sichtbare Fragmente, bleibt `depth_visible_final_skin_pixels_unverified`; das beweist explizit noch keine korrekte finale Skin-Farbe.
7. Präzise native Negativtests `known=1,0,0` und `known=1,1,0` ergänzen die bereits vorhandenen 3/3 Low158-GL-Tests. Gültige `known=1,1,1` mit/ohne sichtbare Fragmente geprüft.
8. Live-`AvatarBakeProgressTrace` um `depth_part_known=H,U,L` und `depth_part_passed=H,U,L` erweitert; nur Datenschutz-konforme boolesche Werte, keine UUID-/Session-/CAPS-Daten.
9. Native Tracer-Tests prüfen unvollständige Query-Stufe, anschließende vollständige Query-Stufe, gescheiterte Depth-Fragmente, asynchrones Debouncing und Regionepoch-Wechsel. Sechs neue Python-Vertragstests überprüfen die gesamte Kette zwischen tatsächlicher Rendererquerie und Livebericht.
10. Vollständiges Mint-x64-Gate mit C++-Build, nativen CTests, Python-Verträgen, isolierter Firestorm-Xvfb-GUI/OpenGL- und X11-Systemicon-Prüfung. Anschließend alle Fortschrittsdokumente synchronisiert.

## Was danach weiterhin offen bleibt

Die Freigabestufe `depth_visible_final_skin_pixels_unverified` heißt weiterhin, dass **mindestens** ein ursprünglich servergebundener Körperbereich sichtbare GPU-Depth-Fragmente erzeugt hat, nicht dass alle drei Bereiche vollständig sichtbar sind. Sie ist **kein** RGB-Pixel-Readback, kein COF- oder Skin-/Kleidungsnachweis. Die tatsächliche 3/3-RGB-/Outfit-Abnahme, Teleport/WASD und der authentifizierte Shutdown erfordern einen neuen realen Grid-Lauf. Die Arbeiten senken nachweislich falsch-positive Abnahmen, ersetzen keine fehlenden Server-Bakes.

Ziel `v0.4.0.0` IN_ARBEIT, Gesamtfortschritt unverändert 38,65 % (39 % gerundet). Kein Release, kein `main`-Merge und keine Produktivinstallation.
