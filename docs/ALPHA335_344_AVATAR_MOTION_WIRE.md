# Alpha.335–344 – NexVerse Live-Wire: Bewegungs- und Appearance-Diagnose

18.10.2026 · develop-current · Viewer 0.2.0-alpha.344 · **Gesamtfortschritt 39 % (38,65 % gewichtet)**

## Zehn überprüfbare Entwicklungsblöcke

| Block | Änderung / Evidenz |
|---|---|
| 335 | GitHub develop-current und CHATUEBERGABE gelesen, OpenGenesisLINK Server-main 7c6b0cf geprüft. Kein freigegebenes Deployment. |
| 336 | Nutzer-Screenshot + tatsächliche Live-Logs geprüft: 1 ungültiges AvatarAppearance, kein SelfReadback, WASD-Steuerbits wechseln ohne Positionsänderung. |
| 337 | OutboundCommandQueue für AgentUpdate coalesced: neuester noch nicht übertragener Steuer-Snapshot ersetzt alten; Chat/Teleport bleiben FIFO. |
| 338 | MovementFeedback wird nur nach erfolgreicher Aufnahme in die bounded Queue aktualisiert; eine abgewiesene WASD-Eingabe wird nicht als versendet deklariert. |
| 339 | Firestorm 7.2.4 message_template.msg Low158 gegen Original-Decoder geprüft: Sender, TextureEntry, VisualParams, AppearanceData, AppearanceHover, Attachments. |
| 340 | Echter AvatarAppearance-Paketeingang klassifiziert den verwerfenden Decoderabschnitt (header, sender, texture_entry, visual_params, appearance_data, hover, attachments). Erste Fehlermeldung ohne UUIDs oder Paketdaten. |
| 341 | F3-Eigenavataransicht zeigt betroffenen Appearance-Abschnitt, ohne Authentifizierung oder erfolgreiches Self-Paket zu behaupten. |
| 342 | Lokaler 0600-Avatar-Diagnosebericht enthält ausschließlich numerischen Fehlerphasencode. Keine persönlichen Paketbytes, IDs oder Capability-Adressen. |
| 343 | F3 zeigt echten unabhängigen WASD-Status aus Serverposition: keine Eingabe, ausstehend, wiederholt stillstehend, serverseitig geändert. Native Tests für Queue FIFO, letzte Eingabe, Firestorm-Wire-Felder und Bewegungsmonitor. |
| 344 | Vollständiger Mint CMake/NVIDIA OpenGL Build, native CTests, Python, ARM64-kompatibles Headless, isolierter Xvfb-Start, Originalicon/WM_CLASS, GitHub Push und Pi5 CI mit identischem SHA. |

## Abgrenzung

Der Decoder meldet jetzt den fehlerhaften Wire-Abschnitt für die NÄCHSTE echte NexVerse-Sitzung. Die neue Information lässt nicht rückwirkend erkennen, weshalb das eine AvatarAppearance-Paket im Screenshot ungültig war; es gibt kein archiviertes Rohpaket. Keine ungesicherten Serverpakete werden akzeptiert, und es werden keine Fake-Bake-UUIDs eingesetzt.

Die aktuell angepasste WASD-Queue verhindert überholte Controls; sie bestätigt NICHT, dass der Simulator den Avatar bewegt. Bewegung wird weiterhin ausschließlich aus echten Server-Positionsupdates bestätigt. Der Taskleisten-/Cinnamon-Fix und das adaptive Mesh-Budget der vorigen Runde bleiben bestehen.

**Gesamtfortschritt 39 %**, nächste zehn Blöcke **Alpha.345–354** mit einem neuen Live-Grid-Test: AvatarAppearance-Fehlerphase aus F3/Log, WASD-Status, Texturen/Meshes, Outfit-Darstellung.
