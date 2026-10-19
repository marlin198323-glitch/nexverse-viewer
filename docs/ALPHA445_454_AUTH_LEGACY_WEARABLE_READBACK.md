**ALPHA.454 PRÜFSTAND 19.10.2026:** Feature-SHA 9360106596ac437f97fbc626909eb9d4361a223a: Mint Linux-x64 Build 371/371; native C++/OpenGL 157/157; Python 193/193; Xvfb, X11 Taskleistenicon PASS. Raspberry Pi5 hat EXAKT diesen GitHub-SHA in separatem Test-Checkout mit pi5-headless-tests.sh bestanden: OGL_PI5_NATIVE_HEADLESS_OK, 193 Python-Tests (ein unterstützter Skip). GitHub Actions Pi5 Run 39283789898 noch QUEUED (KEIN CI SUCCESS behaupten). Kein GUI-/GPU-Livebuild am Pi, keine echte authentifizierte NexVerse Avatar/COF/WASD-Session. Projektfortschritt 39 %, nächster Zehnerblock Alpha.455–464. Repo-Hauptentwicklung auf GitHub, kein Produktionsdeploy.

# Alpha.445–454 – Original Firestorm-Startabfrage für getragene Wearables

19.10.2026 | develop-current | 0.2.0-alpha.454 | Projektfortschritt 39 % (38,65 % gewichtet).

## Zehn Entwicklungsblöcke
445. GitHub HEAD 70851bd, erfolgreiche Alpha.444 Pi5-CI, Übergabe, Projektplan und OpenGenesisLINK Server main 293d1d6 überprüft; echte Logs zeigen SSA-Handshake, aber keinen angenommenen eigenen Low158.
446. Offizielle SecondLife/Firestorm Nachrichtenformate Low381 AgentWearablesRequest, Low382 AgentWearablesUpdate in original message_template.msg und offizieller Wiki überprüft.
447. Read-only Low381 Encoder für AgentID/SessionID gebaut; invalide UUID wird nicht gesendet; keine Wear-Mutation, kein AgentSetAppearance.
448. Low382 Decoder mit exakt passenden Längen, Original WearableData (ItemID,AssetID,Type), U32 SerialNum, 0..16 WearableType, leere Legacy-Dummy-Antwort erlaubt.
449. Circuit sendet einmal pro authentifizierter Circuit-Lebensdauer nach echtem AgentMovementComplete einen zuverlässigen Read-only Request. Separater Socket-Send-Status, nicht mit Antwort verwechselt.
450. Eingehenden Low382 an WorldModel integriert, nur aktuelle authentifizierte Agent- und Session-ID, neuere SerialNum gewinnt.
451. Nach WorldModel Reset, Region-/Session-/Userwechsel und Disconnect Legacy-Wearables-Referenzen löschen; gefundenen Reauth-Leak behoben.
452. F3 Viewer-Oberfläche zeigt ausstehende Anfrage, Legacy-Antwort sowie echte Anzahl nicht-null AssetIDs, immer mit Hinweis COF offen.
453. Privater Diagnosebericht nur Send-/Empfangs-Bools, Zählwerte und Seriennummer, keine Wearable UUIDs. C++ Test mit echten synthetischen LLUDP-Frames (17 Slots, 0 Dummy, Replay, Foreign, Session, Truncation, ungültiger Slot, Region) sowie Xvfb-OpenGL-F3-Test.
454. Gesamte Linux-CMake-/Native-GL-/Python-/ARM64-Headless-/GUI-/Systemicon-Regression und Pi5 SHA-gebundene CI, Feature-Commit/Projektplan/Übergabe.

## Beweisgrenzen
Ein Low382-Readback ist ein **historischer** Wearables-Mechanismus. Moderne OpenSim-Simulatoren können darauf Dummy-Daten zurückgeben. Lesen oder ein erfolgreicher Socket-Send bestätigt nicht eine vollständige Current Outfit Folder (COF), tatsächlich getragene Kleidung, GPU-Texturen, selbst empfangene Low158 AvatarAppearance oder erfolgreiche WASD-Bewegung. Es gibt keine automatischen Outfit-Mutationen, keine Fake-Bakes und keinen produktiven Rollout. Projektfortschritt deswegen weiterhin 39 %.

Nächste zehn Blöcke Alpha.455–464: tatsächliche Live-Wearables/SSA-Signale auslesen und gegen COF/Assets/GPU abgleichen; Outfits, Renderer und Firestorm-Bedienfunktionen weiter integrieren.
Quellen: https://wiki.secondlife.com/wiki/AgentWearablesRequest und https://wiki.secondlife.com/wiki/AgentWearablesUpdate und originale SecondLife Viewer scripts/messages/message_template.msg.
