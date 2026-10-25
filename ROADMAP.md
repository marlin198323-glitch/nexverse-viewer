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

**T01 weitere UI-/Shutdown-Teilarbeit (25.10.):** Oberer 58-px-Header, unterer 60-px-Dock, Eingaben 205×32, Welt-Button 180×32 bei 1648×928; kein gezeichneter Außen-Fokusrahmen. Login bleibt echte WebKit-Welcome/GLFW/OpenGL-Implementierung. 50-ms-abbrechbarer LLUDP-Retry-Schlaf, explizite Async-Join-Telemetrie und Gesamt-Shutdown-Messung nach GLFW-Abbau. 187/187 native + 667/667 Python und Firestorm-Xvfb-GL/X11-Icon lokal PASS. Echte authentifizierte Shutdown-Dauer und Live-Grid-E2E weiterhin offen: docs/V0_4_0_0_MINI_LOGIN_AND_MEASURED_SHUTDOWN.md.

**T01 visuelle Feinabstimmung:** Dokumentierte reale GUI-Prüfung für 1648x928 und 2048x838. WebKit-Zoom vereinheitlicht Seite für kurze UltraWide-Fenster ohne verzerrten Text oder verdeckte News. Fokusglow entfernt, Einfassungen dauerhaft neutral, Header/Dock weiterhin kompakt. 187/187 native CTests + 671/671 Python, Original-Firestorm-Xvfb-GL/GUI/X11-Icon PASS. Noch kein authentifizierter E2E-Login. Details docs/V0_4_0_0_VISUAL_REFERENCE_RECHECK_2026_10_25.md.

**T01 WebKit-Snapshot-Fehlerbehebung:** Native Snapshot-Revisionsprüfung, Callback-Lifetime-Gate, 3 bounded retries, 30s-WebKit-Refresh, GPU-Frame-Generations-Upload und realer Xlib-Resize-Regressionstest für vier Größen; 188/188 native und 675/675 Python, GUI/GL/Icon PASS. Authentifiziertes Grid-E2E bleibt offen. Details docs/V0_4_0_0_WELCOME_SNAPSHOT_ERROR_REPAIR_2026_10_25.md.

**T01 UI/UX-Teilarbeit:** programmierter Neon-Login statt Popup, Original-NexVerse-Welcome, responsiver Footer und echte GLFW-Hitboxes, Paswortanzeige, sechsfache Auflösungsabnahme. Backend-Grid-E2E bleibt offen. Siehe docs/V0_4_0_0_NEON_LOGIN_REFERENCE_IMPLEMENTATION.md.

- [ ] v0.4.0.0-T02 – CAPS, EventQueue, LLUDP, MovementComplete und Region-/Teleport-Reconnect
**v0.4.0.0 P0 CAPS-REPLAY Login + Region (25.10.2026):** Die Login-CAPS- und Region-CAPS-EventQueue schützen jetzt beide vor nichtadjazenten Batch-Replays (z.B. 17/18/17), behalten beim Replay den aktuellen ACK, resetten die 64er Recent-Historie nach 404 selbst bei identischer URL und pacen ID-lose leere 200-Antworten. Workerlogs melden ausschließlich Zähler, keine URLs/Token/UUID/Eventinhalte. Linux Mint vollständig: 188/188 native CTests, 685/685 Python, Firestorm Xvfb OpenGL GUI und X11-Systemicon PASS. Authentifizierter Live-Grid-Login/Teleport/Avatar-/WASD-Nachweis OFFEN. Details docs/V0_4_0_0_CAPS_REPLAY_LOGIN_REGION_FIX_20261025.md. Version v0.4.0.0 IN_ARBEIT, Projektfortschritt weiterhin 39 % (38,65 %). Kein main-Merge, kein Release.

**T02 Region-CAPS-Reconnect (25.10.2026):** Erstes fehlgeschlagenes Seed-CAPS/EventQueueGet kann in derselben Region mit 5–60s Cooldown neu gestartet werden; alte Worker dürfen aktuelle Region nicht deaktivieren; HTTP 404 + gleicher Queue-URI setzt ACK zurück. Native Region-Tests inkl. HTTP503/404/Lease PASS, Linux Mint x64 komplett 188/188 CTests, 680/680 Python, Firestorm Xvfb/GL/X11-Icon PASS. Echte Live-Grid-Region/Teleport-Abnahme offen. Dokument: docs/V0_4_0_0_REGION_EVENTQUEUE_RECONNECT_20261025.md. Projekt 39 %; v0.4.0.0 IN_ARBEIT.


**T02 Teilfortschritt (Batch 02):** EventQueue HTTP-200 ohne bestätigbare ACK-ID wird nicht zugestellt, leere idlose Antworten werden kontrolliert gepaced. Interaktives QA-Dashboard unter `docs/INTERAKTIVE_PROJEKTABNAHME.md` verfügbar; keine automatisierte Live-Freigabe. Voller Region-/Teleport-E2E weiter offen.

**Geführte manuelle Abnahme:** Acht Bedien- und Beobachtungstests in docs/ACCEPTANCE_GUIDED_V0400.json. Nutzer meldet sichtbare Resultate, Entwickler prüft Logs/Server-Readback. Weiterhin keine offizielle Live-Freigabe.

**P0 v0.4.0.0 FastRez/Skin + Mesh-Log (25.10.2026):** Authentifizierter Logbefund vom 25.10. 15:40:08: Low158 SELF wurde vor Circuit-Ready gepuffert und danach übernommen; bislang fehlte in diesem Pfad die Head/Upper/Lower-Bake-Diagnose. Ergänzt sind reine Anzahl-Meldungen 'body_bakes_low158', 'body_bakes_candidates', 'provisional_object_faces' ohne Asset-IDs und ohne falsche GPU-/COF-Behauptungen. 102.762 Mesh-Upload-Erfolgseinträge im historischen Log werden durch erste/dann jede 128. aggregierte Erfolgsinfo reduziert (keine GPU-Funktionsänderung). Lokale Tests: 189/189 native, 694/694 Python, Firestorm-Xvfb-GL/GUI/X11-Icon PASS. GetMesh-LOD-invalid_faces sowie GetTexture-JPEG2000-Metadatenwarnung als nächste konkrete Assetfehler offen. Dokument: docs/V0_4_0_0_FASTREZ_BAKE_DIAG_AND_MESH_LOG_20261025.md. Live-E2E-Avatar/Outfit bleibt offen. v0.4.0.0 IN_ARBEIT; 39% (38,65% gewichtet), kein main-Merge.

- [ ] v0.4.0.0-T03 – WASD, Rotation, Sprung/Flug und Kamera im echten Grid inklusive serverseitigem Positions-/Yaw-/Z-Readback
**P0 v0.4.0.0-T03 – POST-TELEPORT BEWEGUNG (25.10.2026):** Der WASD- und Steigen/Sinken-Nachweismonitor darf nach einem großen Simulatorpositionssprung nicht mehr die VOR dem Teleport gesendeten AgentUpdate-Datagramme für neue Serverpositionsänderungen als Beweis verwenden. Beide Monitore setzen die socket-sent-Freigabe zurück und zeigen 'neuer UDP-Versand ausstehend'; reine UI-Queue-Eingaben bleiben unbestätigt. Erst tatsächlicher neuer UDP-Send und authentischer Simulatorpositionsreadback können Bewegung/Höhe erneut belegen. Native Regressionen und vier Python-Verträge ergänzt. Linux Mint: 188/188 CTests, 689/689 Python, Firestorm-Xvfb-OpenGL-GUI/X11-Icon PASS. Authentifizierter Live-Teleport, COF/Skin und WASD/Flugserverreadback OFFEN. Details docs/V0_4_0_0_POST_TELEPORT_MOVEMENT_PROOF_20261025.md. Status v0.4.0.0 IN_ARBEIT, Projekt 38,65 % (39 %); kein main-Release.

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
**P0 v0.4.0.0 AVATAR BAKE 3/3 LIVE-TRACE (25.10.2026):** Die bisherige avatar_skin_blocker_key-GL-Stufe konnte 3+ Aufrufe aus nur einem Körperteil als drei vollständig gezeichnete Body-Bakes zählen. Korrigiert: Nur drei jeweils CURRENT-Low158-identische, tatsächlich auf originalen Firestorm-LLM-Head/Upper/Lower-Teilen gebundene und gezeichnete Body-Texturen erfüllen die Stufe; acht Head-Draws sind keine 3/3. Native Userdiagnose exportiert einzelne Head-/Upper-/Lower-Zeichnungswerte. Neuer privatsphäreschonender AvatarBakeProgressTrace protokolliert nach authentifiziertem Login pro Region nur Statusänderungen (bei Blockade maximal alle 30 Sekunden, schnelle Zustandswechsel mindestens 250 ms), GetTexture-Stage 0-6, Server/ObjTE-Quelle 0-2, GPU/GL/Depth-Stufen; KEINE UUIDs, Credentials oder Endpoint-URLs. Regionwechsel und Logout setzen History zurück, kein automatischer Diagnose-Dateiexport. Linux Mint: 192/192 native CTests, 710/710 Python, Firestorm-Xvfb-GL/GUI und X11-Icon PASS. Echte Head-/Upper-/Lower-RGB- und COF-/Outfit-Grid-Abnahme weiterhin OFFEN; ebenfalls authentifizierter Shutdown. Dokument: docs/V0_4_0_0_AVATAR_BAKE_3OF3_LIVE_TRACE_20261025.md. v0.4.0.0 IN_ARBEIT, Projekt 39 % (38,65 %). main unverändert.

- [ ] v0.5.0.0-A02 – Kleidung und Outfits aus COF dauerhaft sichtbar nach Relog/Teleport/Regionwechsel
- [ ] v0.5.0.0-A03 – Remote-Avatar-Skin/Shape/Attachments und Änderungsdialoge geprüft

### v0.6.0.0 – Welt-Renderer, Meshes und Materialsystem
**Status:** GEPLANT – Entwicklungsmeilenstein, kein Produktionsrelease
**Aufgaben:**
- [ ] v0.6.0.0-T01 – Prim/Skulpt/Mesh/LOD/Animesh, Streaming und Objekt-Hierarchie
**P0 v0.4.0.0 GetMesh/JPEG2000 25.10.2026:** Valid triangles within an OpenSim mesh face survive out-of-range or degenerate sibling triangles; fully invalid faces/LODs still reject and use normal LOD fallback. Error counts: invalid_faces/invalid_triangles and successful skipped_invalid_triangles. JPEG2000 reports reason=image_extent, image_dimensions, channel_count, component_dimensions, component_subsampling or component_precision, with existing 4096px/64MiB/16bit caps unchanged. Local Mint gates: 190/190 CTests, 699/699 Python, Firestorm-Xvfb OpenGL/GUI/X11 icon PASS. Real authenticated J2C asset not yet retested; Avatar/COF/skin E2E and live shutdown pending. Documentation: docs/V0_4_0_0_MESH_TRIANGLE_RECOVERY_J2C_METADATA_20261025.md. Project still 39% (38.65% weighted); v0.4.0.0 IN_ARBEIT, no main merge.

**P0 v0.4.0.0 GetTexture PNG-/J2C-Verwechslung (25.10.2026):** Manche GetTexture-Simulatorantworten können trotz angefordertem PNG einen nativen J2C-/JP2-Bytestream enthalten. Der Viewer erkennt nun J2C-SOC oder JP2-Signatur und verwendet erst nach vollständiger, validierter OpenJPEG-RGBA-Dekodierung direkt diesen Body ohne zweite HTTP-Anfrage. Fehlende/abgeschnittene Signatur oder J2C-Decoderfehler führen weiterhin zur unabhängigen x-j2c-Downloadprüfung; Sicherheitslimits bleiben unverändert. Native Netzwerk-Mocktests J2C+JP2+truncated und separater Signatur-CTest ergänzt. Linux Mint: **191/191 native CTests, 704/704 Python, Firestorm-Xvfb-GL/GUI/X11-Icon PASS**. Historische JPEG2000-Metadatenablehnungen im authentifizierten Grid nicht neu geprüft. Details: docs/V0_4_0_0_TEXTURE_PNG_NATIVE_J2C_RECOVERY_20261025.md. v0.4.0.0 IN_ARBEIT, Projekt **39 % (38,65 % gewichtet)**; main unverändert.

**P0 v0.4.0.0 GetTexture umgekehrter Format-Fallback + GPU-Log-Datenschutz (25.10.2026):** Auf Linux Mint wurde eine reproduzierbare Formatfalle im Loader behoben: Eine valide PNG-Antwort auf einen x-j2c-CAPS-Request wurde zuvor ausschließlich mit OpenJPEG dekodiert und selbst bei intakten PNG-Pixeln als INVALID mit 30 Minuten Sperrfrist bewertet. Die jetzt vollständige 8-Byte-PNG-Signatur routet zu vorhandenem sicherem libpng-RGBA-Decoder (4096px, 32MiB), unvollständiges PNG bleibt INVALID ohne zweite Netzwerk-Schleife. Transport-Mock für valide und abgeschnittene PNG-Antwort + Signatur-Native-Tests und sechs Python-Verträge ergänzt. GetTexture-GPU-Upload-Budget-/Fehlermeldungen enthalten keine Asset-UUID mehr, technische Fehlerdetails bleiben sichtbar. Linux Mint x64: **192/192 CTests, 716/716 Python, Firestorm-Xvfb-GL-/GUI-/X11-Icon PASS**. Live-Grid-JPEG2000-Fehler, Head/Upper/Lower-Bakes, COF, Avatar-Skin und angemeldeter Shutdown weiterhin OFFEN. Dokument docs/V0_4_0_0_GETTEXTURE_REVERSE_PNG_AND_PRIVACY_20261025.md. v0.4.0.0 IN_ARBEIT, 39 % (38,65 % gewichtet), kein main-Release.

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
