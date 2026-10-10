# OpenGenesisLINK Viewer – Avatar und Bewegung

Stand 0.2.0-alpha.52 vom 10.10.2026

Ein eingeloggter OpenSim-Viewer kann Welt und Meshes anzeigen,
ohne selbst bereits spielbar zu sein. Die bisherige lokale
AgentUpdate-Ausgabe reichte ohne Self-Avatar-Synchronisierung
und Avatar-Renderpfad nicht aus.

## Grundbedienung

W/S: vorwärts und rückwärts.
A/D: seitwärts links und rechts.
Pfeiltasten links/rechts: Avatar drehen.
Pfeiltasten oben/unten: vorwärts/rückwärts.
Shift: schneller.
Leertaste: springen / aufwärts.
C: abwärts.
F: Flugmodus an/aus mit erneutem Tastendruck.
F11: maximierte Ansicht / exklusives Fullscreen umschalten.

Die Steuerung sendet nur bei aktivem LLUDP-Circuit
und fokussiertem Fenster AgentUpdate an den OpenSim-Server.
Positionsdaten werden nicht als serverbestätigt ausgegeben,
solange kein passendes Self-Avatar-Paket eingetroffen ist.

## Avatar-Darstellung

Bis zur echten Avatar-Appearance werden eigene und andere
bekannte Avatare mit einem vereinfachten Humanoid aus
vorhandenen primitiven GPU-Meshes visualisiert. Das macht
Drehung und Position visuell kontrollierbar, ersetzt aber
weder Skeleton, Texturen/Bakes, Kleidung, Mesh-Outfits,
Attachments noch Animationen.

## Status und Live-Test

Zur Abnahme nach der Anmeldung W/A/S/D und Pfeiltasten
betätigen; die Logs sollten dann Folgendes zeigen:
- Avatar controls active
- AgentUpdate controls changed
- Avatar self local-id bound
- Avatar self position synchronized

Fehlt die LocalID oder die Simulator-Positionsbestätigung,
wird die Ursache anhand der Avatar-Update-Pakete untersucht.
Kein Teleport, keine unbegrenzte Fake-Client-Prediction.

Die weitere Reihenfolge steht im verbindlichen projectplan.md.


## alpha.52 – Avatar sichtbar machen und Bewegung · 10.10.2026

**Auslöser:** Der Screenshot der verbundenen Welt zeigt
Terrain/Meshes, aber keinen eigenen Avatar und bislang
keine nachvollziehbare Bewegung. WORLD UDP LIVE beweist
weder den Avatar-Renderpfad noch die Bewegungsfähigkeit.

**Implementiert:**
- OpenGL-Avatar-Proxy-Pfad mit sieben Basis-Meshes für
  Kopf, Torso, Arme, Beine, für eigenen Avatar und bis
  zu 63 weitere Avatar-Datenpunkte. Die Darstellung wird
  pro Frame anhand der Agentposition aktualisiert.
- Die Proxies sind absichtlich keine echten OpenSim-Avatare:
  keine Skin-Bakes, Avatar-Meshes, Skeletons,
  Wearables, Haare, Attachments oder Animationen.
- Neues Keyboard-Control-Modul mit W/S/A/D, linker/rechter
  Pfeiltaste zur Rotation, Shift für schnelleres Bewegen,
  Space für Sprung, C für Abwärtsbewegung, F Flug
  per Toggle. Die AgentUpdate-Pakete enthalten dabei
  korrekte Körper-/Kopf-Quaternionen und Kameraachsen.
- Self-Avatar-LocalID wird aus ObjectUpdate bzw.
  ObjectUpdateCompressed gewonnen, mit dem RegionHandle
  gekoppelt und bei ImprovedTerseObjectUpdate
  zur Aktualisierung der serverbestätigten eigenen
  Avatarposition im WorldModel verwendet.
- Bei Steuerungsänderungen und bestätigten Simulator-
  Positionsupdates werden diagnostische Logzeilen geschrieben.
  Fokusverlust sendet keine gedrückten Bewegungs-Keys.
- Nach erfolgreichem Login wird das Login-Eingabefeld
  nicht mehr durch die Bewegungssteuerung verändert.
- Regressionstests für Steuerbits, Quaternion, Fly-Toggle
  und Steuerungsreset; 26/26 CTests grün.

**Noch offen und nicht als fertig behaupten:**
- Echte NexVerse-In-World-Bewegungsabnahme, insbesondere
  muss die Self-Avatar-LocalID und die Terse-Positions-
  Bestätigung mit neuer Version beobachtet werden.
- Richtige Firestorm-Avatar-Appearance (Skeleton,
  Body Shape, Skin-Bakes, Wearables, Attachments,
  Animationen) sowie Mouselook, Camera-Orbit/Zoom,
  Teleport/Regionswechsel, Collision/Reconciliation.
- Die einfachen Avatar-Proxies sind nur eine
  Übergangsdarstellung bis zu Phase-10-Appearance.

Logstart: ./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl52-live.log
