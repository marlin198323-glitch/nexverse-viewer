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

## Alpha.98 – echter Animation-Keyframe-Code, Live noch offen

Neue Firestorm-Animation-Assets lassen sich synthetisch
decodieren und 3 originalgewichtete LLM GPU Draws pro
Avatarframe pruefen. Neues Fenster Avatar > Animationen
zeigt echte Self High20 ID/Sequence, sobald authentifiziert.
Ein frischer NexVerse InWorld Login hat auch fuer Alpha.98
nicht stattgefunden. Echt geladene Animation-Assets,
automatische Playback/Priority sowie Outfits nicht
als live-verifiziert ausweisen.

## Alpha.99 – GitHub Source Review, Live-Viewer noch offen

Die Main-HEAD-Änderung des Servers
86148202 auf 011ed518 wurde live durch
GitHub-Preflight entdeckt und Source/Doku
inspiziert; verifiziert sind Robust-Inventory-
Thumbnail-Update-Änderungen. Chat Flüstern/
Sagen/Rufen ist mit UI und korrektem LLUDP
gekoppelt, aber nicht mit einem neuen
NexVerse-Login im Simulator erprobt.
Keine aktuelle InWorld-E2E-Behauptung.

## Alpha.100 – Live-Abnahme offen

Ein originärer LLUDP AssetTransfer ist im Viewer nun
mit Firestorm-KeyframeGPU und Animations-Floater verknüpft.
Alle Golden Wire-/Chunk-/NotFound-/GPU-Tests sind
synthetisch. Für Alpha.100 wurde keine neue
authentifizierte NexVerse-InWorld-Sitzung ausgeführt.
Tatsächlich ausgelieferte Animationsassets und laufende
Bewegungen müssen dort gesondert geprüft werden.

## Alpha.101 – Objektberuehrung UI + Original LLUDP, Live noch offen

LLUDP Low117/119 Pakete und Maus-Press/-Release
sind nun mit sichtbarer Root-Prim-Liste integriert;
NexVerse-Testskript ogl_lsl_touch_hold_smoke.lsl
ist zur spaeteren InWorld-Abnahme vorgesehen.
Firestorm /7 Beep privater Kanal im Viewer-Core.
Kein neuer authentifizierter InWorld-Test und
keine bestaetigten realen touch()/llListen-Skriptereignisse.

## Alpha.102 – WorldRay mit GPU-Basis, Live-Abnahme offen

Viewer-Maus kann erstmals echte WorldModel-
Objekte aus Kamerarichtung per BoundingBox
selektieren und die vorhandenen Grab/DeGrab
in die zuverlässige LLUDP Queue geben.
Synthetic Ray-/GL-/Wiretests ersetzen keine
InWorld-NexVerse-Sitzung: weder LSL touch()
noch exakte Mesh-Face/UV-Erkennung bestätigt.
Server 0.9.3.12 bleibt Dev.

## Alpha.103 – Tests statt angenommener Grid-Abnahme

Echter GetMesh Triangleray, Face/UV und
Objekt-Floater sind lokal verifiziert,
nicht durch einen neu angemeldeten
NexVerse Avatar. F9 Outfit Galerie kann
ihren autorisierten CAPS Readback neu
anfordern; Wear/Unwear immer noch offen.

## Alpha.104

Alpha.104: LLUDP Original TouchSurface Byte-Golden, TriangleNormal/Binormal, GUI/Outfit-Fetch Tests lokal. Ohne authentifizierte NexVerse-InWorld-Testumgebung kein LSL-Face-/UV-Ereignis und kein erfolgreicher COF-Wear nachgewiesen.

## Alpha.106

Alpha.106: Ein-Link-COF Zustand
statisch und per Fakeowner CAP
testbar, Firestorm Low426 Bytegolden,
WorldRenderer Linux GL3.3 GUI Smoke.
Es wurde NICHT im echten NexVerse Grid
ein Kleidungsstueck angezogen,
keine AvatarAppearance aus neuem COF
von Server und GPU bestaetigt.
Missing/Epoch/Timeout werden nicht
als erfolgreicher Wear markiert.

## Alpha.107

Alpha.107: COF Batch ADD und Single UNLINK sind per synthetischem Server-Readback im C++-Test abgesichert. Ohne NexVerse Login keine beobachtete AvatarAppearance oder tatsächliches Tragen/Ablegen der Bekleidung; serverseitiger Linkerfolg ist nicht sichtbarer Avatar-Look.

## Alpha.108

Alpha108: Lokale synthetische Tests pruefen COF-versionsbezogene SELF AvatarAppearance Serverpakete und UI. Keine frische authentifizierte NexVerse-InWorld-Sitzung, kein visueller Bake-/GPU-Outfit-Beweis, keine Kombination aus mehreren COF Add+Remove mit Rollback. Alle drei Punkte offen.
