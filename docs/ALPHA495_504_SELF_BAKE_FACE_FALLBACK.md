# Alpha.495–504 – provisorischer Bake-Ersatz nur für fehlende Low158-Faces

19.10.2026 | develop-current | 0.2.0-alpha.504 | Projektstatus 39 % (38,65 % gewichtet).

## Zehn Entwicklungsblöcke

495. Branch und Commit de623f6, Übergabe, Projektplan und Fortschrittsdatei sowie Alpha.494 Pi5-CI 39292283612 SUCCESS für exakten SHA geprüft.
496. Tatsächliche Renderlücke entdeckt: Eine gültige Self Low158 ohne einige Head-/Upper-/Lower-Bake-IDs unterdrückte bisher alle eigenen gültigen ObjectUpdate-Kandidaten.
497. SelfRenderBakeSelection eingeführt, je Körper-Face Low158 zwingend bevorzugt; nur fehlende Faces dürfen aus authentifiziertem aktuellen Self ObjectUpdate ergänzt werden.
498. Die echte self_render_bakes Renderer-Auswahl bezieht nun diese drei Quellen-IDs und macht die fehlenden Face-Assets dem bestehenden Streaming zugänglich.
499. Agent-/Session-/Region-/Self-LocalID-Rebind invalidiert die provisorische Quelle; placeholder, fremde und fehlerhafte UUIDs bleiben verworfen.
500. AvatarPipelineProof hält die autoritativen Low158-Bake-Evidenzen unabhängig von den vorläufigen ObjectUpdate-Referenzen und deren GPU-Residenz.
501. F3-Avatarinspektor kennzeichnet Ersatz-Faces als Obj-TE vorlaeufig / GPU geladen oder GPU fehlt und nennt ausdrücklich kein COF-Beweis.
502. Native Tests mit widersprechender Low158-Head UUID und Objekt-Head UUID bestätigen Low158-Head-Vorrang und erlauben provisorische Upper-Face; LocalID-Erneuerung löscht die provisorischen Werte.
503. UI-/Vertrags- und Native-Regressionsprüfungen validieren source labels, GPU-State und ausbleibende automatische Outfit-Mutationen; Vollbuild, GL/CTest, Python, Headless/Xvfb/X11 Icon.
504. VERSION, Übergabe, Projektplan, Fortschritts-JSON und zehn-Blöcke-Regel aktualisieren, Source-Commit und Pi5-CI für exakten Commit prüfen, finalen Doku-Commit erstellen; nächstes Paket Alpha.505–514.

## Grenzen
Self ObjectUpdate TE ist eine streng vorläufige Quelle. Seine IDs können älter sein, und sind kein Nachweis von COF, serverseitig getragener Kleidung oder endgültigen GPU-Pixeln. Vollständig bekannte autoritative Low158-Bake-Faces werden niemals ersetzt. Es wurden keine Kleider- oder COF-Mutationen ausgeführt. Die echte NexVerse Avatar-Skin-/Outfit-/Attachment-/WASD-End-to-End-Sichtbarkeit bleibt offen. Fortschritt bleibt 39 %, kein Produktionsdeployment.

Nächste zehn Blöcke Alpha.505–514: tatsächliche Live-Simulator-Avatar- und TextureEntry-Belege mit F3-R-Diagnose und GPU-/WASD-Funktionsprüfung, Firestorm-Funktionen mit sichtbarer Oberfläche weiter ausbauen.
