# Alpha.89 – Abgrenzung der vorherigen Alpha.88-Live-Sitzung

Stand 11.10.2026, Log: ~/.local/state/opengenesislink/viewer.log;
OpenGenesisLINK Viewer 0.2.0-alpha.88, Einstieg 21:39:48 Uhr.
Die Datei ist privat auf dem autorisierten Linuxrechner geblieben;
hier stehen nur aggregierte technische Kennzahlen.

## Vom echten Simulator bereits beobachtet

- Login bestätigt und LLUDP UDP LIVE, CAPS EventQueue LIVE; Idle HTTP 502 normal
- WorldModel READY, RegionHandshake, 8 Terrain-Patches beim READY-Event
- Avatar-Self-LocalID vom ObjectUpdate erkannt; Selbstpositions-Terse wiederholt
  neunmal ohne messbare Änderung: (151.568924, 193.071442, 22.558577)
- AgentUpdate Input-Steuerflags 1 und 2 tatsächlich angefordert;
  Wire-Protokollfehler in Alpha.88 festgestellt: LLQuaternion 16 statt 12 Byte
- Sieben konsekutive Region-Epoch-2-Snapshots; am Ende 286/295 Texturen
  (96%), 252/256 Meshes (98%); bis dahin 9 Texturfehler
  (6 HTTP404/Missing, 3 Invalid) und 4 Meshfehler (1 Missing, 3 Invalid)
- Zuletzt 836 echte GPU-Mesh-Drawcalls pro analysiertem Frame, jedoch
  weiterhin 964 Mesh-Placeholder-Drawcalls für 200 Objekte;
  kein vollständiger Mesh-Live-Vollständigkeitsbeweis

## Grenzen und offene Aufgaben

HTTP404 heißt: Der angesprochene Simulator lieferte kein Asset
über beide getesteten GetTexture-/GetMesh-CAP-Endpunkte.
Invalid bedeutet: Der Decoder erhielt über die CAP kein von ihm
als renderbar anerkanntes Asset. Ohne serverseitige Originalbytes
kann nicht eindeutig zwischen tatsächlich defekten Serverassets
und einem noch nicht unterstützten legalen Datenformat entschieden
werden; keine automatische Mutations-/Reparaturannahme.

Der letzte Legacy-Live-Check meldet not_verified:
alte Session, verbleibende GPU-Mesh-Fallbacks und fehlende volle
Asset-Residency. Änderungen in Alpha.89 wurden zu diesem Zeitpunkt
nur mit C++/Python-GPU-Fixtures abgenommen, nicht durch neuen
Avatar-Login. Für den nächsten Live-Test Viewer neu starten,
Anmeldung und WASD testen sowie live InWorldAssetSnapshots prüfen.


## Alpha.91-Hinweis – neuer, noch unbestätigter Live-Nachweis

Seit der 11.10.2026-Alpha.88-Session war beim aktuellen lokalen
Quellstand bis zur Entwicklung von Alpha.91 kein neuer
authentifizierter NexVerse-Login in der Viewerlog-Datei vorhanden.
Der neue thread-sichere SelfAvatar-Terse/WASD-Monitor
protokolliert im Fall einer neuen Session Bestätigung oder
Stillstand. Keine vorherige synthetische GPU-/CTest-Prüfung
beweist tatsächliche WASD-Bewegung oder gebackene Kleidung.


## Alpha.92 – Erwartete Live-Telemetrie

Nach erfolgreichem Login und bestätigtem AvatarAppearance Low 158
wird für die EIGENE AgentID einmal je Region
'AvatarAppearance SELF server readback accepted' geschrieben.
Dabei erscheinen nur Byte-/Eintragszahlen, niemals Anmelde-
Tokens oder Attachment-UUIDs. Die Logzeile belegt ausschließlich
Empfang der Metadaten, keine korrekt gerenderte Skin/Outfit-Bake.
Nach neuer Alpha.92-Sitzung sind Self-Readback, Avatar-motion-
Evidence, echte Bewegung und Avatarposition gesondert abzunehmen.


## Alpha.93 – notwendiger authentifizierter NexVerse-Test

Neue Alpha.93-Logzeile bei echtem GPU-Head/Upper/Lower-Bake-Draw:
'Avatar SELF Head/Upper/Lower baked TEXTURE submitted to OpenGL'.
Die Logzeile zählt tatsächliche Drawcalls, nicht nur
Asset-IDs oder den Empfang von AvatarAppearance.
Sie behauptet ausdrücklich kein Firestorm-Skeleton-
Skinning oder UV-Korrektheit. Voraussetzung:
User-Login, Server-AvatarAppearance mit expliziten
Bake-Face-UUIDs, erfolgreiche GetTexture und GPU-Residency.
Ohne diese Live-Sitzung bleiben Avatar-Bake und WASD
not_live_verified. AvatarAttachment-Parent-Pose hat
bislang ebenfalls nur reproduzierbare Unit-/GPU-Tests.


## Alpha.94 – neu eingefuehrte Real-Mesh-Live-Indikatoren

Lokaler Start bestaetigt:
Firestorm original skeleton bind joints=133; attachment slots=55;
OGLRenderer Firestorm ORIGINAL three-part avatar bind-pose
meshes GPU-resident; triangles=7186.
Nur nach echtem NexVerse Login/World-Ready wird das
separate Self-Avatar-GPU-Zaehlereignis
Avatar SELF Firestorm original .llm bind-pose MESH
actually drawn in GPU protokolliert.
Dies wurde NOCH NICHT im angemeldeten Grid gemessen.
Keine behauptete Animation oder vollständige Firestorm-Paritaet.

## Alpha.95 – explizite Grenze der Skinning-Telemetrie

Eine Avatar SELF Logzeile fuer real drei Original-
SkinWeight-GPU-Draws belegt ausschliesslich
GPU-Shader-Rendering. Es gab in diesem
Entwicklungsblock keinen neu autentifizierten
NexVerse-Login und keinen bestaetigten E2E
Server-AvatarAnimation-State. Kein Live-Verified
fuer WASD oder Avatar-Outfit-Appearance behaupten.

## Alpha.96 – new AvatarAnimation High20 acceptance evidence

The client logs one anonymous SELF AvatarAnimation
readback per region with counts of active animations,
sources and physical event blocks. It does not log
animation asset UUIDs, credentials, COF or tokens.
Real authenticated NexVerse logins have NOT taken place
after the development block; synthetic High20 fixtures
and original GL GPU pose tests alone do not prove
OpenSim animation clips, correct inworld locomotion,
avatar outfits or COF.

## Alpha.97 – echte inworld UI weiterhin zu verifizieren

Der OpenGL-UI-Test beweist pixelbasiert, dass das neue
Menü und die InWorld-Floater mit echtem GLSL/Fonts gerendert
werden, und die Unit-Testserie beweist die Klick-Zuordnung.
Dies ist NICHT identisch mit einem authentifizierten
NexVerse-Login. ChatFromSimulator/ChatFromViewer/TeleportFinish
und aktuelle Inventar/COF-Fenster müssen mit Login und
echtem Simulator im nächsten Live-Durchlauf getestet werden.
Die GUI deaktiviert unfertige Firestorm-Features,
anstatt klickbare Schein-Buttons zu rendern.
