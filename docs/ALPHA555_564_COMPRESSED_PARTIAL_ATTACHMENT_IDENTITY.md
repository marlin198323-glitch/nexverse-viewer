# Alpha.555–564 – sichere partielle ObjectUpdateCompressed-Pakete ohne verlorene Avatar-Anhänge

20.10.2026 | develop-current | 0.2.0-alpha.564 | Projektfortschritt 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke

555. GitHub develop-current sauber bei a059b03; CHATUEBERGABE.md, projectplan.md, Fortschritt-JSON und Pi5-CI Run 39457621156 (SUCCESS für Alpha.554-SHA) geprüft.
556. OpenGenesisLINK-Server-main neu SHA 95c21334cfc299ebe03e91414f7ae762e1fc3c45; Änderungen gegenüber 6e446b9 betreffen Statistik-/Hypergrid-API und nicht das Avatar-ObjectUpdate-Protokoll.
557. ECHTEN ObjectUpdateCompressed-Fehler gefunden: 64-Byte-Header ohne Shape, ParentID, Mesh und TextureEntry konnte unbekannte Phantom-Prims anlegen. Bei wiederverwendeter LocalID konnte er die alten Parent-/Mesh-/TE-Daten unter einer anderen UUID weiterführen.
558. Neue WorldModel::apply_object_compressed_partial nimmt nur bereits vorhandene aktuelle Prims bei exakter LocalID, UUID und Prim-PCode an. Vollständige Compressed-Pakete gehen weiter über den Full-Prim-Dekoder.
559. Ausschließlich State (nibble-swapped AttachmentPoint), Position und Rotation des 64-Byte-Headers werden übernommen. Bestehende echte Mesh-/Material-/Shape-/ParentID-/TextureEntry-/Sculpt-Daten bleiben unangetastet.
560. Doppelte Daten erhöhen keine Renderrevision. NaN/Inf, unplausible Rotation, unbekannte LocalIDs, andere UUID und falsche PCodes werden abgewiesen. State=0 löst den alten Attachmentpunkt korrekt, ohne COF oder Wearables zu ändern.
561. Nativer Wire-Test synthetisiert echtes LLUDP ObjectUpdateCompressed mit Regionhandle, Flags, Länge und 64-Byte ObjectData. Decoder -> WorldModel zeigt erlaubten Wechsel auf Anhangpunkt 3, während eine fremde UUID bei gleicher LocalID unverändert abgewiesen wird.
562. Realer OpenGL-Framebuffer-Test zeigt geänderte Pixel bei Positionsaktualisierung eines am animierten Firestorm-Schultergelenk befestigten Prims. Der gültige Attachmentpunkt bleibt bestehen.
563. Python-Vertrag kontrolliert Quellpfade, Identitätsprüfung, Verlustfreiheit von Original-Mesh/TextureEntry, Regionsbindung, Re-Attach/Detach und Abweisung von Phantomobjekten.
564. Vollständige lokale Linux-x64 Build/CTest/OpenGL/Python, ARM64-kompatible Headless-Pipeline, isolierter Xvfb-Viewer und X11-Systemicon. Danach GitHub Featurecommit, Pi5-CI SHA-genau, aktualisierte Übergabe auf Alpha.565–574.

## Grenzen

Die Änderung repariert reale Material- und Anhang-Synchronisierung bei unvollständigen Compressed ObjectUpdates. Sie bestätigt nicht vollständige COF-Kleidung oder einen tatsächlich fertig dargestellten Avatar. Eigene Skin, getragene Kleidung, Mesh/HUD und WASD im authentifizierten NexVerse-Grid bleiben für einen neuen End-to-End-Lauf offen. Fortschritt weiterhin 39 Prozent, kein produktives Deployment.

Nächste zehn Entwicklungsblöcke: Alpha.565–574, Schwerpunkt echte Avatar-/COF-/GPU-/WASD-Live-Belege und parallel Firestorm Viewer-GUI.
