# Alpha.535–544 – Legacy-Anhänge aus echten Simulator-Objekten

20.10.2026 | develop-current | 0.2.0-alpha.544 | Projektfortschritt 39 % (38,65 % gewichtet).

## Zehn Entwicklungsblöcke

535. GitHub develop-current HEAD aa89f4c, Übergabe, projectplan und Fortschrittsdateien sowie vorherige erfolgreiche Alpha534-CI-Dokumentation geprüft.
536. OpenGenesisLINK Server main SHA 6e446b9 Quellcode LLClientView.cs abgerufen. Server liefert in älteren Low158-Paketen keinen AttachmentBlock, aber das echte Root-Prim-ObjectUpdate.State beziehungsweise Compressed-ObjectData.State kodiert den AttachmentPoint nibble-geswapped.
537. Full-ObjectUpdate-Decoder übernimmt das bisher übersprungene State-Byte. Wire-Test für 0x10, Firestorm Anhangpunkt 1.
538. Compressed-ObjectUpdate-Decoder übernimmt state=data[21]. Native Wire-Test für 0xF1, HUD Anhangpunkt 31.
539. Das WorldModel bewahrt State, ParentID und LocalID für aktuelle Full- und Compressed-Objekte.
540. Read-only Attachment-Inferenz wählt nur Root-Prims mit ParentID=aktuelle Avatar LocalID, gültiger Prim-PCode, UUID und Anhangpunkt 1 bis 55. Unbekannte, nicht eigene oder obsolete Objekte bleiben ausgeschlossen.
541. Im echten Avatar-Renderpfad werden Self- und Remote-Avatar-Maps aus dem WorldModel ergänzt. Explizite Low158-Attachmentpunkte bleiben bevorzugt.
542. Native Wire-, Current-Parent- und HUD-Klassifikation sowie realer OpenGL-Framebuffer-Animationstest zeigen eine aus dem echten WorldModel State abgeleitete Firestorm-Skelettanbindung; HUD und Kinder bleiben aus dem 3D-Renderer ausgeschlossen.
543. F3 und privater Avatarbericht zeigen den Unterschied zwischen expliziten Low158-Anhängen und über Simulator-Objekte nachgewiesenen Attachment-Roots; nur Zähler, niemals UUIDs.
544. Gesamte x64 CMake/CTest/OpenGL- und Python-Tests, ARM64-Headless, isoliertes Xvfb und Systemicon sowie GitHub, Pi5 SHA-CI und Projektübergabe auf Alpha.545–554.

## Grenzen und nächste Priorität

Der Fix macht aktuelle Anhang-Root-Prims beim klassischen OpenGenesisLINK Low158 ohne AttachmentBlock sichtbar beziehungsweise positionierbar, wenn der Server sie tatsächlich liefert. Die Anhang-ID ist eine echte serverseitige Objektbeziehung, nicht die Bestätigung vollständiger COF-Wearables oder vollständiger GPU-gerenderter Originalkleidung. Der neue Build wurde noch nicht mit einem frischen echten NexVerse-Login bis zur sichtbaren Skin und Outfitsteuerung end-to-end geprüft. Projektfortschritt 39 %, kein Produktionsdeployment.

Nächste zehn Blöcke Alpha.545–554: echter Grid-Login, F3-Bericht und die fehlenden Skin-, COF- und Attachment-Assets/Render-Evidenz; parallele Firestorm UI-Funktionen.
