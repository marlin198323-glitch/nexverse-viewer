# Alpha.545–554 – ImprovedTerse verliert keine Avatar-Anhangpunkte und TextureEntries mehr

20.10.2026 | develop-current | Version 0.2.0-alpha.554 | Status 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke

545. GitHub develop-current HEAD fd8afd8, CHATUEBERGABE.md, Projektplan, Fortschritt und letzte erfolgreiche Alpha.544 Pi5-ARM64-CI 39455400291 geprüft. OpenGenesisLINK Server main SHA 6e446b9 erneut bestätigt.
546. In der laufenden Application echten High9 ImprovedTerseObjectUpdate Fehler gefunden: Pose-only-Pakete wurden in neue ObjectState-Instanzen umgewandelt und per upsert_object gespeichert; dadurch gingen realer State-Byte/AttachmentPoint, Mesh- und geparste TextureEntry-Daten verloren. Unbekannte LocalIDs erzeugten Phantomobjekte.
547. Neue WorldModel::apply_object_terse-Methode nur für bestehende aktuelle Server-Prims, ausschließlich Position und Rotation ändern, alle übrigen Eigenschaften unangetastet.
548. Regions-/Identitätsschutz: Nur region.connected, existierende local_id, bekannter Prim-PCode und gültige echte Objekt-UUID werden akzeptiert. Unbekannte Pakete ohne Full/Compressed ObjectUpdate verworfen.
549. Validierung: NaN/unendliche Positionen, nichtfinite oder physikalisch unplausible Quaternionen verworfen. Identische Serverpose erhöht nicht mehrfach object_revision und verhindert redundantes Szenen-Neuplanen.
550. Tatsächlichen High9-Handler auf WorldModel::apply_object_terse umgestellt; bestehende WorldModel-Mutex und Regionhandle-Schutz bleiben erhalten, Self-/Remote-Avatar-Pfade unverändert.
551. Native Modellregression für echten Schulteranhang, Mesh-Sculpt-ID, State, ParentID, UUID, vollständigen und geparsten TextureEntry, Material, Skala. Tests für Duplikat, unbekanntes Objekt, invaliden Quaternion, NaN, KillObject, Regionwechsel.
552. Echte OpenGL-Framebuffer-Regression: ImprovedTerse bewegt ein bekanntes WorldModel-Prim an einem Firestorm-Skelett-Anker; geänderte Position erzeugt tatsächlich neue sichtbare RGB-Pixel und die Anhangszuordnung bleibt erhalten.
553. Python-Quellvertrag für Invarianten, tatsächliche Dispatcher-Verdrahtung, fehlenden Phantom-Upsert, getrennte Skin-/Attachment-HUD-Funktionen und Dokumentationsstand.
554. Vollständige x64 Build-, native C++-/OpenGL-CTest-, Python-, ARM64-kompatible Headless-, Xvfb-Viewer- und X11-Systemicon-Prüfungen; Entwicklungscode GitHub, SHA-genaue Pi5-CI und neue Übergabe auf Alpha.555–564.

## Grenzen

Dies repariert die korrekte Übernahme echter bereits empfangener Avatar-Anhang-, Material- und Mesh-Daten während der Simulation. Es ist weder eine COF-Mutation noch der Beweis für getragene Kleidung. Vollständige eigene Avatar-Skin, Anhangtexturen, COF-Outfit und simulatorbestätigte WASD-Bewegung bleiben in einem NEUEN authentifizierten NexVerse-Grid-Live-Lauf nicht bestätigt. Gesamtfortschritt daher 39 Prozent. Keine produktive Bereitstellung.

Nächste zehn Blöcke Alpha.555–564: Live-NexVerse-Avatar/Skin/Kleidung/Bewegung prüfen und Firestorm GUI gleichzeitig nach tatsächlichen Befunden vervollständigen.
