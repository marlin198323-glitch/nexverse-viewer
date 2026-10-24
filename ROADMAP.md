# OpenGenesisLINK Viewer – Versionierte Roadmap

**Aktives Entwicklungsziel v0.4.0.0 – IN ARBEIT, nicht released.**
**Erstes vollständiges Release v1.0.0.0; erster Hotfix danach v1.0.0.1.**
Gesamtfortschritt: 38,65 % gewichtet, ca. 39 %. Kein Produktivrelease.

Maschinenlesbare Meilensteine: `docs/RELEASE_ROADMAP.json`.
Freigabe: `docs/RELEASE_VERSION_POLICY.md`. Historische Alpha-Blöcke stehen weiterhin im Projektplan.

## Versionen

### v0.3.0.0 – Aktuelle technische Entwicklungsbasis
**Status:** FERTIG (geprüfte Entwicklungsbasis, NICHT auf main veröffentlicht)
**Aufgaben:**
- [x] v0.3.0.0-T01 – Bestehende C++23/OpenSim/OpenGenesisLINK-Architektur, Grid-Login, LLUDP und CAPS inventarisieren
- [x] v0.3.0.0-T02 – Ist-Zustand: Self-Avatar lokal sichtbare Teilfarben, F3/F9, Flugbutton, authentifizierte Live-Beweise als offen ausweisen
- [x] v0.3.0.0-T03 – VERSION/CMake/Programm/README/Projektplan auf vierstellige Nummer und 39%-Stand angleichen
**Abnahmekriterien:**
- [x] v0.3.0.0-A01 – Vierteiliges v-Label in Build, Runtime und Metadaten identisch
- [x] v0.3.0.0-A02 – Dokumentierte vollständige siebenfach kritische Release-Lücken; keine Freigabe oder main-Merge

### v0.4.0.0 – Grid, Login, Regionen und reale Avatarsteuerung
**Status:** IN ARBEIT – gehärteter Login/CAPS-Transport implementiert, reale Grid-Endabnahme offen
**Aufgaben:**
- [ ] v0.4.0.0-T01 – Grid Discovery/SSO/Login und Session/Passwortreset-Fehlerwege
**T01 Teilfortschritt:** URI-/Login-/Seed-CAPS-Autoritätsprüfung,
kein automatisches Weiterleiten von Secret-POSTs oder CAPS-Token-GETs;
echter Loopback-307-Test, GridInfo-Cache- und CAPS-Sicherheitsregressionen.
Noch offen: vollständiges Grid-SSO/Passwortreset und Live-Grid-Login.
Die vollständige Version v0.4.0.0 bleibt IN ARBEIT.

- [ ] v0.4.0.0-T02 – CAPS, EventQueue, LLUDP, MovementComplete und Region-/Teleport-Reconnect
- [ ] v0.4.0.0-T03 – WASD, Rotation, Sprung/Flug und Kamera im echten Grid inklusive serverseitigem Positions-/Yaw-/Z-Readback
**Abnahmekriterien:**
- [ ] v0.4.0.0-A01 – Echter authentifizierter NexVerse-Login/Re-Login, Region-Handoff, Teleport und Rückkehr reproduzierbar
- [ ] v0.4.0.0-A02 – W/S/A/D/Yaw/Jump/Fly auf dem aktuellen Server tatsächlich sichtbar und protokolliert, nicht nur UDP-Send
- [ ] v0.4.0.0-A03 – Region-/Accountwechsel ohne alte Daten oder falsche Bewegungsbestätigungen

### v0.5.0.0 – Avatare, Skin, Outfits und Wearables
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.5.0.0-T01 – Firestorm-original Skeleton, Shape/Drivers und Animationen auf aktuellem Server vollständig
- [ ] v0.5.0.0-T02 – CurrentOutfitFolder, LLWearable, Kleidung/Tattoo/Alpha/Universal-Layer und Bake-Cache/Upload
- [ ] v0.5.0.0-T03 – Mesh-Avatare, Skinning, Skinned-Attachments, HUD und Remote-Avatare
**Abnahmekriterien:**
- [ ] v0.5.0.0-A01 – Self Head/Upper/Lower echte Serverbakes mit jeweils authentifiziertem GetTexture und RGB-Pixel-Endbeweis 3/3
- [ ] v0.5.0.0-A02 – Kleidung und Outfits aus COF dauerhaft sichtbar nach Relog/Teleport/Regionwechsel
- [ ] v0.5.0.0-A03 – Remote-Avatar-Skin/Shape/Attachments und Änderungsdialoge geprüft

### v0.6.0.0 – Welt-Renderer, Meshes und Materialsystem
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.6.0.0-T01 – Prim/Skulpt/Mesh/LOD/Animesh, Streaming und Objekt-Hierarchie
- [ ] v0.6.0.0-T02 – JPEG2000, TextureEntry, PBR/glTF, Transparenz, UV, Texturqualität
- [ ] v0.6.0.0-T03 – Terrain, Wasser, Himmel, Sonne/Mond, Licht, EEP, Wetter und Sichtbarkeit
**Abnahmekriterien:**
- [ ] v0.6.0.0-A01 – Referenzregion mit echten Serverassets visuell korrekt gegen Firestorm verglichen
- [ ] v0.6.0.0-A02 – Keine fehlenden priorisierten 3D-Objekte/Texturen unter validem Assetdienst
- [ ] v0.6.0.0-A03 – Begrenzter RAM/VRAM/CPU bei repräsentativer Szene, GPU-Fehler protokolliert

### v0.7.0.0 – Inventar, Bauen, Objekte, Karte und Grundstücke
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.7.0.0-T01 – Inventarordner/-items, Suche, Upload/Download, Rechte, Wear/Detach
- [ ] v0.7.0.0-T02 – Objekte Rez/Move/Rotate/Scale/Edit, Linkset, Touch, Skripte und Rückmeldung
- [ ] v0.7.0.0-T03 – Land/Parzellen, Regionskarte, Landmarken, lokale und interregionale Teleports
**Abnahmekriterien:**
- [ ] v0.7.0.0-A01 – Alle sichtbaren Buttons CRUD/Verwenden/Touch haben nachprüfbares Backend und Server-Readback
- [ ] v0.7.0.0-A02 – Permission-/Owner-Tests für Inventar/Objekte und Simulatorlimits
- [ ] v0.7.0.0-A03 – Nach Ab-/Anmeldung bleiben eigene Änderungen konsistent

### v0.8.0.0 – Kommunikation, Freunde, Gruppen und Identität
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.8.0.0-T01 – Lokaler Chat, Flüstern/Sagen/Rufen, IM, Offline-Nachrichten
- [ ] v0.8.0.0-T02 – Freundesliste, Online-Status, Gruppen, Rollen, Einladungen und Rechte
- [ ] v0.8.0.0-T03 – Profile, Bilder, Suche, Einwilligungen, Abuse/Bug Report
**Abnahmekriterien:**
- [ ] v0.8.0.0-A01 – Chats/IM/Freunde/Gruppen über zwei echte Avatare getestet
- [ ] v0.8.0.0-A02 – Rollen-/Datenschutz-/Berechtigungsprüfungen einschließlich Sperr- und Logoutfällen
- [ ] v0.8.0.0-A03 – Jede UI-Aktion ist mit echten Simulator-/CAPS-Endpunkten verbunden

### v0.9.0.0 – Firestorm-GUI- und Bedienungsparität
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.9.0.0-T01 – Hauptmenüs, Fenster, Floater, Toolbar, Modal/Buttons/Felder/Tabellen/Suche
- [ ] v0.9.0.0-T02 – Inventar-, Avatar-, Land-, Gruppen-, Freunde-, Bau- und Kommunikation-UIs
- [ ] v0.9.0.0-T03 – Widescreen, Skalierung, Eingabefokus, Tastatur, Maus, Kamera, Einstellungsseiten und OGL-Branding
**Abnahmekriterien:**
- [ ] v0.9.0.0-A01 – Firestorm-Funktionsmatrix vollständig ohne tote Bedienelemente abgearbeitet
- [ ] v0.9.0.0-A02 – Visuelle und funktionale End-to-End-Prüfung aller Menüs auf verschiedenen Auflösungen
- [ ] v0.9.0.0-A03 – Aktionen tatsächlich auf Backend ausgeführt, nicht nur Placeholder/Toast

### v0.9.5.0 – Stabilität, Sicherheit, Performance und Modulgrenzen
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.9.5.0-T01 – HTTP/LLUDP/CAPS Fehler- und Retry-/Timeout-Tests, Grenzen, Wiederherstellung
- [ ] v0.9.5.0-T02 – Speicher-/GPU-Leaks, große Regionen/Avatare, Performance und Barrierefreiheit
- [ ] v0.9.5.0-T03 – MPL-2.0/Firestorm-Quell- und Assetlizenzen, signierte Updates, Module, Logs und Crashreports
**Abnahmekriterien:**
- [ ] v0.9.5.0-A01 – Langlauf-/Lasttests und fehlerfreie Speicher-/VRAM-Grenzen dokumentiert
- [ ] v0.9.5.0-A02 – Angriffe auf Login/Auth/Tokens/URLs/CAPS/Assets geprüft
- [ ] v0.9.5.0-A03 – Alle extern eingebundenen Komponenten rechtlich/reproduzierbar dokumentiert

### v0.9.9.0 – Release Candidate und plattformübergreifende Abnahme
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.9.9.0-T01 – Windows, Linux x64 und macOS Installer inklusive Systemicon/Uninstall/Update
- [ ] v0.9.9.0-T02 – Reproduzierbare CI/CD-Artefakte, Buildsignaturen und Freigabe-Workflow
- [ ] v0.9.9.0-T03 – Echte vollständige Grid-Tests: Avatar-Outfit 3/3, Movement/Teleport, Welt/UI/Assets
**Abnahmekriterien:**
- [ ] v0.9.9.0-A01 – Sieben echte Evidence-IDs und exakter getesteter Release-Commit erfolgreich
- [ ] v0.9.9.0-A02 – Plattformtests Windows/Linux/macOS bestanden und Artefakte installierbar
- [ ] v0.9.9.0-A03 – Alle P0/P1-Blocker geschlossen, Rollback und Release Notes verfügbar

### v1.0.0.0 – Erste vollständige stabile Veröffentlichung
**Status:** GEPLANT – Main-Merge und Release nur nach vollständigem Gate
**Aufgaben:**
- [ ] v1.0.0.0-T01 – Sämtliche vorangegangenen Meilensteine vollständig integriert und abgenommen
- [ ] v1.0.0.0-T02 – 100 % gesamte Firestorm-/OpenGenesisLINK-Funktionsmatrix mit Live-Grid-Endbeweisen
- [ ] v1.0.0.0-T03 – Finale Doku, Lizenzprüfung, Changelog und getestete reproduzierbare Release-Artefakte
**Abnahmekriterien:**
- [ ] v1.0.0.0-A01 – scripts/ogl-release-readiness.py Exitcode 0; gewichteter Projektwert 100 %; alle 7 realen Nachweise eindeutig zuordenbar
- [ ] v1.0.0.0-A02 – Vollständiger Mint- und Linux/Windows/macOS-Release-Test mit identischer Code-SHA
- [ ] v1.0.0.0-A03 – Erst dann Pull Request nach main, Merge nach Freigabe, annotierter v1.0.0.0-Tag und GitHub-Release

### v1.0.0.1 – Erstes Wartungs- und Hotfixrelease
**Status:** BLOCKIERT – v1.0.0.0 muss vorher auf main veröffentlicht werden – Main-Merge und Release nur nach vollständigem Gate
**Aufgaben:**
- [ ] v1.0.0.1-T01 – Nach v1.0.0.0 gemeldete Bugs reproduzieren und beheben
- [ ] v1.0.0.1-T02 – Sicherheits-/Regressionstests und ggf. Protokoll-Fixes
- [ ] v1.0.0.1-T03 – Changelog, Artefakte, Upgrade-/Rollbacktest
**Abnahmekriterien:**
- [ ] v1.0.0.1-A01 – Alle gemeldeten Bugs des Hotfix-Scopes reproduzierbar geschlossen
- [ ] v1.0.0.1-A02 – Sämtliche v1.0.0.0-Freigaben nicht regressiert, Plattformtests und exakte SHA grün
- [ ] v1.0.0.1-A03 – Nach Review Release-PR nach main, Merge, v1.0.0.1-Tag, GitHub-Release

### v1.1.0.0 – Optionale größere Funktionen nach stabilem Release
**Status:** GEPLANT – Main-Merge und Release nur nach vollständigem Gate
**Aufgaben:**
- [ ] v1.1.0.0-T01 – Nur neue separat freigegebene Erweiterungen, z. B. modernere Renderer-Backendpfade
- [ ] v1.1.0.0-T02 – Neue Features und Plugins ohne Bruch der v1.0-ABI/API
- [ ] v1.1.0.0-T03 – Eigenständige abgenommene Roadmap mit Release-Gate
**Abnahmekriterien:**
- [ ] v1.1.0.0-A01 – v1.0.0.x-Stabilität und Firestorm-Funktionsumfang weiter garantiert
- [ ] v1.1.0.0-A02 – Neue Feature-E2E/Upgrades über gleiche Release-Regeln abgenommen

## Versionsbezogene /goal-Aufträge

- `/goal v0.4.0.0 fertigstellen`: sämtliche Aufgaben/Abhängigkeiten und Abnahme der Zielstufe abschließen und auf `develop-current` synchronisieren.
- `/goal v1.0.0.0 komplett fertigstellen und releasen`: sämtliche Vorgängerversionen/Firestorm-Funktionen plus 7 echte Grid/Plattform-Evidenzen vollständig abnehmen; erst dann geprüften PR nach `main` mergen, Tag und GitHub-Release.
- `/goal v1.0.0.1 fertigstellen`: erst nach echtem v1.0.0.0-Release; Hotfix vollständig testen und danach PR nach main sowie Tag/Release.
- `/goal weiter`: offene Punkte der aktuellen Zielversion; interne Zehnerblöcke verändern keine öffentliche Version.

**Fertig** bedeutet implementiert, mit funktionierender GUI/Backend, auf echten Gridfällen getestet, dokumentiert und alle Gates bestanden. Fehlt etwas, ist die Stufe BLOCKIERT; kein falscher main-Merge.
