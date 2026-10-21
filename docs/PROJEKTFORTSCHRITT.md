**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.604, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.605–614.**

**ALPHA.604 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39626151169 = COMPLETED SUCCESS, selbstgehosteter Pi5 ARM64 Job 119311827201 = COMPLETED SUCCESS, für exakten Feature-SHA 8805576e6b26708f4250a77f2eb81ae1fbd0c3ec. Mint x64 vollständiger Build 385/385, native C++/OpenGL 164/164, Python 299/299, isolierter GUI-Xvfb Viewer, lokale Pi5-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5 testete ARM64-native C++23 Headless und Python, keine Pi5-GPU/GUI und KEIN authentifizierter Live-NexVerse-Login. CoarseLocationUpdate nur für bytequantisierte Kartenpositionen: 3D-Fremdavatar wird erst nach echtem Full/Compressed ObjectUpdate im aktuellen WorldModel admitted; echte genaue Pose/Low158-Appearance/COF wird von Coarse nicht ersetzt oder gelöscht. F7 trennt Original-Simulator-Avatar-ObjectUpdates von bloßen Kartenpunkten. Native realer Coarse-Wire-Decoder -> WorldModel/Identität/Region Test, original Viewer-UI-OpenGL bestanden. Keine aktuelle authentifizierte NexVerse Skin/COF/Outfit/WASD End-to-End-Bestätigung, Fortschritt 39 %. Nächste zehn Blöcke Alpha.605–614. KEIN Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.604, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.605–614.**

21.10.2026 Alpha.595–604: ECHTEN falschen Avatar-Admission-Fehler behoben: Grobe CoarseLocationUpdate-Kartenpositionen (quantisierte Bytes) wurden bisher als vollständige position_known-Avatare behandelt und als Firestorm-3D-Proxy gezeichnet, obwohl kein echtes Avatar-ObjectUpdate eingetroffen war. Neue Source-Flag simulator_object_update_seen wird nur durch Full/Compressed Simulator-Avatar-Pakete gesetzt; Coarse darf echte präzise Position, Skin/COF/Low158/Animationsdaten nicht ersetzen oder Avatar aus nicht-vollständigem Coarse-Snapshot löschen. Reiner Kartenpunkt bleibt ohne 3D-Beleg, kann bei Fehlen aus Coarse-Snapshot entfernt werden. Realer Coarse-LLUDP-Decoder zu WorldModel C++-Test mit Fremd-/Self-/Nil-/Duplikat-/Region-/Appearance-Invarianten, F7 UI zeigt echte Avatar-Objektpakete getrennt von reinen Kartenpositionen. Aktueller OpenGenesisLINK Server-main SHA e215ea2; keine frischen authentifizierten Grid-Logs verfügbar, nur Xvfb-Läufe. Authentifizierter eigener Avatar, Skin, COF, Outfit und serverbestätigte WASD-Bewegung weiterhin OFFEN, deshalb Projekt 39 %. Details docs/ALPHA595_604_COARSE_MAP_VS_REAL_AVATAR_OBJECT.md. Bei WEITER zehn Blöcke Alpha.605–614, KEIN Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.594, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.595–604.**

**ALPHA.594 EXAKTE PI5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39488686178 = COMPLETED SUCCESS, Pi5-ARM64-Headless Job 118855705576 = SUCCESS, exakt für Feature-SHA c6ecd3fbd05218476ef1ed5e755cc5188378c1b6. Linux Mint x64 Vollbuild 383/383, native C++/OpenGL 163/163, Python 293/293, isolierter Xvfb GUI-Start, lokale Pi5-kompatible C++23/Python Headless-Suite und X11 WM_CLASS/Systemicon PASS. Pi5-CI prüft native ARM64 C++23 Headless/Python, KEINE Pi5-GPU/GUI oder authentifizierten NexVerse-Live-Login. Direct RegionHandle-Wechsel, Circuit-Disconnect und neue authentifizierte Agent-/Session-Identität löschen inzwischen alte Prims/Attachments, Fremd-Avatare, Terrain, Parcel, Environment mit Revision und neuer Szene-Epoch. Gleichgebliebene verbundene RegionHandshake/MovementComplete-Pakete bewahren Welt. F7 zeigt echte Szene-Epoch. Frische authentifizierte NexVerse Skin/COF/Outfit/Attachments/WASD-Grid-Abnahme NICHT vorhanden; Status 39 %. Nächste zehn Blöcke Alpha.595–604. Kein Produktivdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.594, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.595–604.**

20.10.2026 Alpha.585–594: REALEN Regionswechsel-/Disconnect-Ghost-Weltfehler behoben. WorldModel.apply_region hatte bei direktem Handlewechsel oder Circuit-Disconnect zwar Self Skin/Appearance entfernt, aber alte Welt-Prims/Anhänge, Fremd-Avatare, Terrain, Parzellen und Environment nicht. Jetzt ganze Regionsszene weg, Renderrevision und Regiongeneration erhöht, present=false. Gleichbleibende verbundene RegionHandshake-/AgentMovementComplete-Pakete bewahren Welt, wiederholte Disconnects erhöhen nicht ständig die Szene-Epoch. Reauth unter neuer Session/Agent verwirft alte regionale Meshes, Anhänge, Terrain und Avatare. Neuer nativer Lebensdauertest für Region/Disconnect/Reconnect/Session und F7-UI Szene-Epoch; vollständige Tests und Pi5 ARM64 SHA-CI folgen. Upstream Server 3f49996 nur Portal/HG-IM. Realer authentifizierter NexVerse-Skin-/COF-/Outfit-/WASD-Beweis OFFEN, Status 39 %. Detail docs/ALPHA585_594_DIRECT_REGION_LEASE_GHOST_SCENE.md. Bei WEITER 10 Blöcke Alpha.595–604, KEIN Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.584, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.585–594.**

**ALPHA.584 SHA-GENAUE Pi5 ARM64 CI ERFOLGREICH:** GitHub Actions Run 39467519712 = COMPLETED SUCCESS, self-hosted Pi5 ARM64 Headless Job 118782677990 = SUCCESS, für exakten Feature-SHA 9b97aca5615a6942c4ca4521b86f46029fd9ad08. Mint x64 vollständiger Build 381/381, native C++/OpenGL 162/162, Python 287/287, isolierter Xvfb Viewer, lokale C++23/Python Headless und X11 Taskleistenicon/WM_CLASS PASS. Der Pi5-CI-Lauf prüft native ARM64 C++23 Headless/Python, keine Pi5-GPU oder GUI und keinen auth Live-NexVerse-Avatar. Wirkliche valide Self-ObjectUpdate TE ohne echte Bakes löst nur die alte PROVISORISCHE Skin; beschädigte oder verkürzte TE bleibt ohne Wirkung; gültige Low158 bleibt pro Face führend. Echte original Firestorm Body-Mesh-GL-Framebuffer-Regressionsbeweise für ausbleibende stale Skin bei noch GPU-residentem Texturasset; F3/privater count-only Herkunftsbericht pro Agent-/Regions-Epoch. Live NexVerse COF/Outfit/Skin/WASD weiter nicht bestätigt. Status 39 %, nächste zehn Blöcke Alpha.585–594, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.584, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.585–594.**

20.10.2026 Alpha.575–584: ECHTEN Self-Avatar-Skin-Lebenszeitfehler korrigiert. Eine neue GÜLTIGE vollständige ObjectUpdate TextureEntry mit ausschließlich Placeholder-/Default-Bake-Faces zieht bisher alte, provisorische Self-Bake-Auswahl zurück. Unvollständige oder beschädigte TE (auch Compressed Header) bleibt wirkungslos; Low158-Bake hat pro Face weiterhin Vorrang. Native sichere Placeholder-/Malformed-/Session-/Low158-Tests; tatsächliche Firestorm Head/Upper/Lower-OpenGL-Framebuffer-Regression belegt, dass allein Entfernen der Bake-Referenz ohne VRAM-Eviction keinen alten Skin-Draw mehr erzeugt. Sitzungs-/regionsbegrenzte F3- und privater Diagnosezähler ohne IDs. COF und echte NexVerse Avatar/Skin/Outfit/WASD-Live-Abnahme noch OFFEN, Fortschritt 39 %. Mehr in docs/ALPHA575_584_VALID_SELF_TE_BAKE_WITHDRAWAL.md. Nächste zehn Blöcke Alpha.585–594, kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.574, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.575–584.**

**ALPHA.574 SHA-GENAUE Pi5 ARM64 CI VERIFIZIERT:** GitHub Actions Run 39464572356 = COMPLETED SUCCESS, self-hosted Pi5 ARM64-Headless-Job 118772472994 = COMPLETED SUCCESS, für EXAKTEN Feature-SHA 2701cf5d0307373529901ee300d13f1a3eb3b0f2. Mint x64 vollständiger Build 381/381, native C++/OpenGL 162/162, Python 281/281, Xvfb GUI-Start, lokale Headless und X11 WM_CLASS/Systemicon PASS. Pi5 läuft ARM64 C++23 Headless/Python, nicht GPU/GUI und nicht authentifizierter NexVerse-Live-Login. Neu: CURRENT Simulator ObjectUpdate.State/ParentID erhält für tatsächlich empfangene Root-Prims Vorrang vor älterem Low158-Anhangblock, wodurch stale HUD-/Bone-Punkte, State=0 Detach und Avatarwechsel korrekt getrennt werden; nicht erhaltene Low158-Items bleiben PENDING und sind keine COF-/GPU-Erfolgsbeweise. Native und Firestorm Original-OpenGL-Framebuffer-Tests, F3 und private count-only Diagnostik PASS. Authentifizierter NexVerse Skin/COF/Outfit/Attachments/WASD E2E weiter OFFEN; 39 % (38,65 %). Nächste zehn Blöcke Alpha.575–584, KEIN Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.574, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.575–584.**

20.10.2026 Alpha.565–574: KORRIGIERTEN echten Attachment-Prioritätsfehler: älterer Low158-Anhang wurde bislang vor aktuellem prim ObjectUpdate.State eingefügt; aktueller Parent/State konnte via emplace nicht überschreiben, wodurch alte HUD-/Bone-/Detach-Angaben blieben. Gemeinsame Region-aktuelle reconcile_avatar_attachments priorisiert echte bekannte ObjectUpdate-UUID/ParentID/State bei SELF und fremden Avataren; State0/Avatarwechsel löst stale Low158, unbekannte Low158-Objekte bleiben als pending nur vorgemerkt. Native echte Bindings-/HUD-/Detach-Tests und OpenGL-Framebuffer mit alt Low158=HUD, aktuellem Prim=Schulter belegen Original-Firestorm-GPU-Bone. F3/privater Report zeigen Effektiv-/Pending-/Stale-Zahlen ohne IDs. Authentifizierter Live-NexVerse Avatar/Outfit/COF/WASD weiterhin offen; 39 %. Docs/ALPHA565_574_SERVER_ATTACHMENT_SOURCE_PRIORITY.md. Bei WEITER genau 10 Blöcke Alpha.575–584; kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.564, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.565–574.**

**ALPHA.564 EXAKTE PI5 ARM64 CI VERIFIZIERT:** GitHub Actions Run 39460543881 = COMPLETED SUCCESS, Pi5-ARM64-Headless-Job 118758562602 SUCCESS exakt für Feature-SHA 498c90571b6315da6a8e022ecbb2f6ff0633c61b. Linux Mint x64 Vollbuild 381/381, native C++/OpenGL 162/162, Python 274/274, isolierter GUI-Xvfb Viewer, ARM64-kompatible Headless Suite und X11 Taskleistenicon PASS. Pi5 hat ARM64 native C++23 Headless und Python geprüft, keine Pi5-GPU/GUI und keinen authentifizierten NexVerse-Live-Login. Fix für 64-Byte Compressed-Object-Header: kein Phantomprim/UUID-Recycling von Mesh/Parent/TextureEntry, nur aktuelle passende LocalID+UUID+PCode State/Pose, State=0 löst Avatar-Anhang. Echte Firestorm-GL-Anhangpixel und LLUDP Wire-Header nativ geprüft. Live Skin/COF/Outfit/Attachments/WASD E2E weiterhin offen, Projekt 39 %. Nächste zehn Blöcke Alpha.565–574, keine Produktion.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.564, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.565–574.**

20.10.2026 Alpha.555–564: REALEN 64-Byte ObjectUpdateCompressed-Headerfehler behoben. Gekürzte Pakete konnten Phantomobjekte erstellen oder bei LocalID-Wiederverwendung alte Parent-/Mesh-/TextureEntry-Daten auf fremde UUID übertragen. WorldModel::apply_object_compressed_partial schützt aktuelle Region und exakte LocalID+UUID+Prim-PCode; aktualisiert NUR State/AttachmentPoint und Pose, behält alle echten vollständigen Material-, Mesh- und Parent-Daten. State=0 detach; fremde/duplizierte/ungültige Pakete verwerfen. Echte Compressed-Wire-Decoder-Testkette und original Firestorm-Schulter-Anhang OpenGL-Pixel-Test bestehen. Upstream Server-main 95c2133 änderte nur Statistik/Hypergrid-API, nicht Avatar-Protokoll. Frischer auth Live-NexVerse Skin-/COF-/Outfit-/WASD-Beweis weiterhin offen, Fortschritt 39 %. Details docs/ALPHA555_564_COMPRESSED_PARTIAL_ATTACHMENT_IDENTITY.md. Bei WEITER zehn Blöcke Alpha.565–574; kein produktives Deployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.554, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.555–564.**

**ALPHA.554 PI5 ARM64 CI EXAKT VERIFIZIERT:** GitHub Actions Run 39457621156 = COMPLETED SUCCESS, Pi5 Headless-Job 118748503752, für Feature-SHA 6e85be50e84e685f3049bf90dc6834746ea468c9. Mint x64 vollständiger Build 379/379, native C++/OpenGL 161/161 PASS, Python 266/266 PASS, ARM64-kompatibles Headless, isolierter Xvfb Viewer und X11 Systemicon/WM_CLASS PASS. ECHTE High9 ImprovedTerse-Pose löscht nicht mehr AttachmentState, geparsten TextureEntry, Material/Mesh/ParentID; unbekannte Objekte werden nicht als Phantomprim erstellt. Original Firestorm-Rig gl-Framebuffer-Test belegt veränderte Pixel bei solcher Bewegung. Pi5-CI ist ausschließlich ARM64 C++23 Headless/Python, keine Pi5-GUI und kein auth Live-NexVerse. Avatar-Skin/COF/Outfit/Bewegung im realen NexVerse-Grid noch NICHT aktuell end-to-end bestätigt; Fortschritt 39 %. Nächste zehn Blöcke Alpha.555–564, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.554, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.555–564.**

20.10.2026 Alpha.545–554: WICHTIGEN realen High9/ImprovedTerse-Viewerbug behoben: Terse-Pose-Paket wurde bisher als neuer ObjectState gespeichert, dadurch gingen echter State/AttachmentPoint, geparster TextureEntry, andere Mesh-/Materialdaten verloren; unbekannte LocalIDs konnten Phantomobjekte erzeugen. Neue regions-/identitätsgebundene WorldModel::apply_object_terse ändert nur Pose bekannter Prims, lehnt NaN, unplausible Rotationen, alte/killed/unknown Objekte sowie reine Wiederholungen ab. Native volle Mesh/TE/State/Parent-Regression und echter GPU-Framebuffer-Test für bewegten Firestorm-Schulteranhang PASS. Kein COF/Wear-Eingriff, Live NexVerse Avatar-/Skin-/Outfit-/WASD-End-to-End noch offen, Status weiterhin 39 %. Doku docs/ALPHA545_554_IMPROVED_TERSE_ATTACHMENT_POSE_SAFETY.md. Nächste 10 Blöcke Alpha.555–564. Kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.544, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.545–554.**

**ALPHA.544 PI5 ARM64 CI EXAKT VERIFIZIERT:** Feature-SHA 8a8c0b3d1415bdca3d05eedd47a88f4e20db7360 wurde mit self-hosted Pi5 GitHub Actions Run 39455400291 = COMPLETED SUCCESS und ARM64 Source-Job 118740824512 geprüft. Mint x64 vollständiger Build 377/377, native C++/OpenGL 160/160 PASS, Python 258/258 PASS, isolierter Xvfb Viewer, Headless C++23/Python und X11 WM_CLASS/Systemicon PASS. Pi5 ARM64 ist Headless, KEIN GPU-GUI oder authentifizierter Live-Grid-Test. Echte OpenGenesisLINK-Objektanhänge wurden aus Full/Compressed ObjectUpdate.State ParentID aktueller Avatar LocalID in Firestorm-Skelett/GL-Render integriert, Low158-Anhangdaten haben Vorrang, HUD wird aus 3D-Szene ausgeschlossen. Eigene echte NexVerse COF-Kleidung/Avatar/WASD weiterhin NICHT end-to-end bestätigt; deshalb Projekt weiterhin 39 %. Nächste zehn Blöcke Alpha.545–554. Kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.544, Fortschritt 39 % (38,65 % gewichtet). NÄCHSTE Alpha.545–554.**

20.10.2026 Alpha.535–544: ECHTEN OpenGenesisLINK-Legacy-Attachmentfehler korrigiert. Server-main 6e446b9 sendet Low158 OHNE AttachmentBlock, Root-Prims liefern aber den Anhangpunkt nibble-swapped in normaler/compressed ObjectUpdate.State (z. B. Schulter 0x10, HUD 31 -> 0xF1). Viewer ignorierte dieses Byte und konnte Anhangobjekte im Self-Rig nicht zuordnen. Jetzt Decoder, WorldModel und echter Firestorm-Rig-Renderer für Selbst-/Fremdavatar integriert: nur aktuelle Prim ParentID=Avatar LocalID und erlaubter Slot 1..55, expliziter Low158-Punkt gewinnt. Native Wire-, HUD- und echter animierter OpenGL-Pixeltest. F3 und privater Report zeigen Obj-Anhaenge ohne UUIDs. Kein COF-/Wear-Eingriff, frischer Live-Avatar/GPU-Outfit/WASD noch offen, Status 39 %. Detail docs/ALPHA535_544_LEGACY_AVATAR_OBJECT_ATTACHMENTS.md. Bei WEITER zehn Blöcke Alpha.545–554. Kein Produktivdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.534, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.535–544.**

**ALPHA.534 SHA-GENAUER Pi5 ARM64 CI-ERFOLG:** GitHub Actions Run 39302449365 = COMPLETED SUCCESS exakt für Feature-SHA 8c6a60aba7e5c04cdf187bc0e48c782ce49d4738. Mint x64 375/375 Build, native C++/OpenGL 159/159 CTest, Python 251/251 PASS, isolierter Xvfb Viewer, native C++23/Python Headless und X11 Taskleistenicon PASS. Self-hosted Pi5 prüfte echte ARM64-C++23-Headless- und Python-Regressionssuite (kein vollwertiges OpenGL-GUI auf Pi). OpenGenesisLINK Server SHA 6e446b9 ScenePresence TURN_LEFT+YAW_POS/TURN_RIGHT+YAW_NEG korrigiert, aus echtem LLUDP-Wire validiert; neuer TurnReadbackMonitor belegt nur danach echte korrekte eigene Simulatorrotation. Real authentifizierter NexVerse WASD-/COF-/Avatar-/Outfit-End-to-End-Test noch nicht erfolgt, daher 39 %. Nächstes Paket zehn Blöcke Alpha.535–544; keine Produktionsbereitstellung.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.534, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.535–544.**

19.10.2026 Alpha.525–534: Aktuellen OpenGenesisLINK-Server-main SHA 6e446b9 geprüft. Echter Bewegungsfehler: TURN_LEFT musste YAW_POS statt YAW_NEG senden, TURN_RIGHT YAW_NEG statt YAW_POS; Drehung und Flugrolle bisher gegensätzlich. Korrigiert und mit vollständigem echtem LLUDP-AgentUpdate-High4-114-Byte-Payload getestet. W/S, A/D und Drehtasten-Gegeneingaben bereinigt, diagonale Bewegung erhalten. Neuer threadgesicherter TurnReadbackMonitor prüft nur NACH real gesendetem UDP und nur echte eigene Simulator-Rotation aus ObjectUpdate/Compressed/Terse bei >=5 Grad korrekt gerichtet. F3 zeigt Drehung ausstehend oder Server bestätigt, keine fingierten ACKs. Re-Login, Logout, Regionwechsel löschen Evidence. Ursprüngliche Skin/Outfit/COF und vollständige WASD-End-to-End-Bewegung im neuen Grid-Build weiter NICHT getestet, Projekt weiterhin 39 %. Detail docs/ALPHA525_534_REAL_OPENGENESIS_YAW_AND_SERVER_TURN.md. Nächste zehn Blöcke Alpha.535–544; zuerst GitHub, aktuelle Übergabe und Upstream prüfen. Kein Produktionsdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.525–534, Projektfortschritt 39 %.**

**ALPHA.524 PI5 ARM64 SHA-CI VERIFIZIERT:** GitHub Actions Run 39299889677 = COMPLETED SUCCESS und Self-hosted ARM64-Pi5-Job 118222412702 = SUCCESS für EXAKTEN Feature-SHA 899f53ee7e13f5a410433e221c51b780d8485b63. Mint x64 Vollbuild 375/375, native C++/OpenGL-Tests 159/159, Python 244/244, x64 Headless und isolierter Xvfb-Viewer sowie echtes X11-Systemicon PASS. Pi5-CI prüft C++23/Python ARM64 Headless, keinen Pi5-GPU- oder Live-NexVerse-Avatar. Spezifischer echter J2C-Tile-Part-Fehler erhält nur EINEN sicheren frischen Downloadversuch; drei unterschiedliche Firestorm-original Head/Upper/Lower GL-Bakes mit GPU-Pixeländerung/Slot-Eviction real getestet. F3 J2C-Rettungszähler erst nach erfolgreichem GPU-Upload, keine Daten erfunden. Keine aktuelle authentifizierte NexVerse Avatar-/COF-/Outfit-/WASD-End-to-End-Abnahme; Fortschritt 39 % (38,65 % gewichtet). Weiter zehn Blöcke Alpha.525–534. Kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.524, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.525–534.**

19.10.2026 Alpha.515–524: ECHTEN früheren NexVerse-GetTexture-JPEG2000-Fehler aus Viewerlog behoben: Für erkannten rohen J2C-Stream mit genau dem Tile-part length/stream-length-Mismatch maximal EIN zusätzlicher frischer Server-GET, alte Transport-Limits und Backoff unangetastet; wenn erneut defekt, korrekter Invalid-Fehler ohne Pixel. Native Testfälle temporär/permanent kaputtes JPEG2000-Tileteil und unzulässiges Format. Auf original Firestorm Head/Upper/Lower .llm werden nun drei VERSCHIEDENE RGB-Skin-Bake-Texturen samt Upper-Replacement und Head-Eviction mit tatsächlichen Framebuffer-Pixeln nachgewiesen. F3 nennt J2C-Retry-Erfolge nur bei real erfolgreichem GPU-Upload und wird beim Regionwechsel zurückgesetzt. Kein aktueller Live-Login-/Outfit-/WASD-Beweis: Projekt weiterhin 39 %. Volltext docs/ALPHA515_524_J2C_TRANSFER_RECOVERY_AND_SKIN_PIXELS.md. Bei WEITER genau 10 Blöcke Alpha.525–534. GitHub-Stand/Server/CI zuerst prüfen, kein Produktionsdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). NÄCHSTE Alpha.515–524, 39 %.**

**ALPHA.514 Pi5 ARM64-CI VERIFIZIERT:** GitHub Actions Run 39296348575 = COMPLETED SUCCESS exakt für Feature-SHA bcc02ddd046bc4133aa8f5b5875996452077835c. Mint x64 Build 375/375 PASS, native C++/OpenGL CTest 159/159 PASS, Python 237/237 PASS, ARM64-kompatibles Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon PASS. Pi5 prüft native ARM64 Headless C++23 + Python, keine native Pi5 GPU/GUI und keinen NexVerse-Live-Login. Authentifizierte Body-Bake-ID-Änderungen führen jetzt ohne 750-ms-Verzögerung zur EXISTIERENDEN GPU-Residency-Neuplanung, einschließlich Eviction alter Bakes; HTTP Retry/Backoff/VRAM-Budget bleibt unberührt. Keine neue bestätigte sichtbare NexVerse Skin/Kleidung/COF/Attachments/WASD, deshalb 39 % (38,65 % gewichtet). Nächste 10 Blöcke Alpha.515–524. Kein Produktionsupdate.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.514, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.515–524.**

19.10.2026 Alpha.505–514: Authentifizierte eigene Head/Upper/Lower-Bake-Änderungen planen jetzt sofort die ECHTE bestehende GetTexture/GPU-Residency-Queue neu (nicht erst mit allgemeinem 750-ms-Timer). Region-/Agentlease/Epoch schützen vor fremden Quellen, identische Body-IDs vermeiden Wiederholungen, entfernte Bakes führen wieder zur VRAM-Eviction. Drei echte Body-IDs verdrängen unter kleinem GPU-Slotbudget alte Texturen; wegfallende Faces machen Weltmaterial wieder möglich. HTTP-Pacing/Retry/VRAM-Schutz bleiben vollständig erhalten. F3 unterscheidet vorläufige Objektquelle bei fehlender und partieller Low158. Keine COF-/Wear-Mutation, kein neuer authentifizierter Grid-GPU-/Outfit-/WASD-End-to-End-Beleg, Status weiterhin 39 %. Details docs/ALPHA505_514_AVATAR_BAKE_EVENT_REPLAN.md. Bei WEITER Alpha.515–524 zehn echte Blöcke. Kein produktives Viewer-Update.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.504, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.505–514.**

**ALPHA.504 Pi5-CI EXAKT BESTÄTIGT:** GitHub Actions Run 39294296008 ist COMPLETED SUCCESS für Feature-SHA 7e8651bc04c47605652f58786fbc9b559e47f343. Mint x64 373/373 Build, 158/158 native C++/OpenGL, 230/230 Python, Headless C++23/Python, Xvfb GUI-Start und X11 Systemicon/WM_CLASS erfolgreich. Pi5-Lauf prüft ARM64 Headless C++ und Python, **nicht** Pi5 GPU/GUI und keinen NexVerse-Live-Login. Neuer Source-Fallback: echte Low158-Bake-ID gewinnt je Face; nur fehlende Faces aus Session/Region/LocalID-authentifiziertem ObjectUpdate vorläufig ergänzen, F3 zeigt GPU/Quelle getrennt. Kein endgültiger Outfit-/COF-/Skin-/WASD-Livebeweis; 39 %. Nächste 10 Blöcke Alpha.505–514, keine produktive Bereitstellung.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.504, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.505–514.**

19.10.2026 Alpha.495–504: Renderlücke behoben: Self Low158 ohne einzelne Bake Head/Upper/Lower IDs unterdrückte auch echte eigene ObjectUpdate TE-Bakes. Jetzt pro Face Low158 bevorzugen, ausschließlich fehlende Faces aus aktueller selbstauthentifizierter ObjectUpdate-TE provisorisch ergänzen. Reale Render-Auswahl integriert; F3 nennt GPU geladen/fehlt aus der provisorischen Quelle, getrennt von autoritativer Low158-/COF-Evidenz. Konflikt-UUIDs, Self-LocalID- und Agentwechsel nativ geprüft. Keine Outfit-Writes. Keine neue NexVerse Avatar-/Outfit-/GPU-/WASD-E2E-Bestätigung; weiterhin 39 %. Details docs/ALPHA495_504_SELF_BAKE_FACE_FALLBACK.md. Nächste zehn Blöcke Alpha.505–514, GitHub/Upstream/CI zuerst, kein Produktivdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.494, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.495–504.**

**ALPHA.494 Pi5 ARM64 CI VERIFIZIERT:** Entwicklungs-SHA 754339d420eb689dc977a0c7a3b5be52b3b9af21 wurde mit GitHub Self-hosted Pi5 Source Check Run 39292283612 = COMPLETED SUCCESS exakt auf diesem Quellstand geprüft. Mint Linux-x64 Build 373/373 PASS, Native C++/OpenGL 158/158 PASS, Python 223/223 PASS, isolierter GUI-Xvfb PASS, Headless C++23/Python PASS, X11 Taskleistenicon/WM_CLASS PASS. CI Pi5 baut keine native GUI oder GPU. Neues serverformatiges synthetisches Low158 mit drei Body-Bake-TE-Overrides erreicht Self-WorldModel/Streamingpriorität; F3 und privater Bericht melden fehlende Bake-IDs unabhängig von GPU-Darstellung. Tatsächlicher authentifizierter NexVerse Self-/COF-/Outfit-/WASD-End-to-End noch nicht belegt. Projekt 39 %; nächster Zehnerblock Alpha.495–504; kein Produktionsdeployment.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.494, Projektfortschritt 39 % (38,65 % gewichtet). NÄCHSTE Alpha.495–504.**

19.10.2026 Alpha.485–494: Alpha.484 echte OpenGenesisLINK Low158-Legacy-Decodierung durch neue vollständige serverformatige Testkette mit Head/Upper/Lower UUID-Face-Overrides (Firestorm 8,9,10), zuverlässigem zerocodiertem LLUDP, TE-Parser, authentifiziertem Self-WorldModel, Streamingpriorität und GPU-Asset-Residency-Plan erweitert. Agentwechsel invalidiert alle drei. F3 unterscheidet keine oder teilweise Server-Bake-IDs von vorhandenen Bake-IDs bei noch fehlenden GPU-Draws. Privater Diagnosebericht zeigt aktuelle Self-Low158-Head-/Upper-/Lower-Bake-Referenzen nur als Zähler/Bools, keine UUIDs. Live Grid/GPU-Avatar/Outfit/WASD noch NICHT verifiziert: Fortschritt 39 %. Siehe docs/ALPHA485_494_LOW158_BODY_BAKE_SOURCE_AND_F3.md. Bei WEITER Alpha.495–504 genau zehn echte Blöcke. Branch develop-current, GitHub SHAs und Upstream prüfen, kein Produktionsdeploy.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Nächste Alpha.485–494, 39 %.**

**ALPHA.484 PI5 ARM64 CI EXAKT VERIFIZIERT:** Feature-SHA 4afbfbbbc670bacc32b949ee4e1295f8b5b1f4eb, GitHub Actions self-hosted Pi5 Run 39290341106 = completed SUCCESS für exakt diesen SHA. Linux Mint x64 Vollbuild 373/373, Native/GL 158/158, Python 215/215, Headless-native PASS, Xvfb isolierter GUI-Start und X11 Taskleistenicon PASS. Der Pi5-CI umfasst ARM64 C++23 Headless und Python, keinen echten ARM GPU- oder autorisierten NexVerse-Live-Login. Sehr wichtiger echte Upstream Low158 Decoderfix: AppearanceHover 1 ohne AttachmentBlock muss akzeptiert werden. Echte Welt/Avatar mit COF-Bakes/Outfit und WASD nach Release erneut prüfen. Fortschritt 39 % (38,65 % gewichtet), kein produktives Deployment.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.484, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.485–494.**

19.10.2026 Alpha.475–484: WICHTIGER REALER OPENGENESISLINK-SERVERFEHLER IN OGL BEHOBEN. Server-main 293d1d6 LLClientView.SendAppearance sendet Self-Low158 mit AppearanceHover(1), aber KEIN AttachmentBlock-Feld. Alter Viewer verweigerte diese gültige Serverantwort als malformed attachments und verlor reale Skin. Jetzt vollständig geparstes Legacy Hover ohne Anhangblock akzeptiert; moderne Anhänge und abgeschnittene Daten streng geprüft. Eigenes Flag attachment_block_present trennt Legacy ohne Block von modern mit explizitem 0; F3/privater Diagnosebericht/Logs zeigen korrekten Status. Native reliable+zerocoded Serverformat Low158 -> LLUDP -> Self WorldModel erfolgreich, Native C++/GL/Python/Headless, Xvfb, Icon und Pi5 CI. Echtes NexVerse Avatar/Outfit/COF/GPU/WASD weiterhin NICHT neu live verifiziert, Status 39 %. Dokumentation docs/ALPHA475_484_OPENGENESISLINK_REAL_LOW158_LEGACY_WIRE.md. Nächste zehn Blöcke Alpha.485–494. Bei WEITER GitHub Branch/CI und Server upstream zuerst prüfen, keine Produktionsbereitstellung.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.475–484, Fortschritt 39 %.**

**ALPHA.474 PI5 ARM64 CI VERIFIZIERT:** GitHub Run 39288055994 = completed SUCCESS fuer EXACT Feature-SHA a0785f24d77d9c8699ebb80ac27f02489b8b81e2. Mint x64 Build 373/373, native/OpenGL CTest 158/158, Python 207/207, ARM64-kompatibles Headless, isolierter Xvfb-Viewer sowie echtes X11 Taskleistenicon/WM_CLASS PASS. Offizieller Pi5-Lauf nur Headless C++23/Python, KEIN Pi5-OpenGL-GUI-Build. Kein echter authentifizierter NexVerse-Self-Skin-/Outfit-/COF-/WASD-End-to-End-Nachweis, Projektfortschritt weiter 39 % (38,65 % gewichtet). Naechste 10 Bloecke Alpha.475–484. Kein automatisches Produktionsupdate.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.474, Projektstand 39 % (38,65 % gewichtet), NÄCHSTE Alpha.475–484.**

19.10.2026 Alpha.465–474: F3-Avatarfenster hat jetzt echte Outfit-Schaltfläche [O] sowie O-Tastaturkürzel zum vorhandenen F9-Outfit-Floater, inkl. echtem Hit-Test und kompaktem responsivem Layout. Low382/COF-Diagnose zeigt nun getrennt anders, fehlend, zusätzlich, doppelt und offene Einträge und schneidet den Text 'kein Wear-Beweis' nicht ab. FetchInventory2-Originalitems zusätzlich an AgentID gebunden, kein Cross-Login-COF-Readback. Native C++/OpenGL, Python, Headless Pi5 ARM64, Xvfb, Systemicon, GitHub. Noch immer KEIN authentifiziertes NexVerse Avatar-/Outfit-/GPU-/WASD-E2E – Fortschritt 39 %. Details docs/ALPHA465_474_F3_F9_OUTFIT_NAVIGATION.md. Bei WEITER exakt zehn Blöcke Alpha.475–484, zuerst Commit/Upstream/CIs prüfen. Kein Produktivdeployment.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.465–474, Projektfortschritt 39 %.**

**ALPHA.464 QUELLSTAND GEPRÜFT 19.10.2026:** Feature-SHA 320abed60c759ec09e7d4950d85a3e4a46fac3c1 auf GitHub gepusht. Linux Mint x64 Build 373/373, native C++/OpenGL CTest 158/158, Python 200/200, ARM64-kompatibles Headless, isoliertes Xvfb und X11-Icon/WM_CLASS PASS. Auf ECHTEM Pi5 wurde exakt Feature-SHA 320abed60c759ec09e7d4950d85a3e4a46fac3c1 direkt per SSH aus dem Repository getestet: PI_ARM64_HEADLESS_EXIT=0, OGL_PI5_NATIVE_HEADLESS_OK, 200 Python-Tests OK (1 unterstützter Skip). GitHub Actions Run 39286040685 ist aktuell QUEUED, NICHT SUCCESS. Kein Arm-GPU/GUI-Test oder authentifizierter NexVerse-Avatar-/Outfit-/WASD-End-to-End-Beleg. Dokumentation und Testresultate nach GitHub; keine Produktion. Nächste zehn Blöcke Alpha.465–474; 39 %.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.464, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.465–474.**

19.10.2026 Alpha.455–464: Legacy Low382-Simulator-Wearables werden read-only mit CurrentOutfitFolder und aktueller FetchInventory2-Originalitem-Liste verglichen (ItemID/AssetID/Typ). Fehlende, abweichende, doppelte und Null-Items werden getrennt gezählt. F9 Outfit-Fenster zeigt Originalserver-Datenabgleich bei Agent/Session/RegionLease/COF-Folder/COF-Version; kein automatisch getragenes Outfit behauptet, kein AgentSetAppearance. Native C++/Python/GL, ARM64 Headless, Xvfb, GitHub und Dokumentation. NexVerse Avatar/Outfit/COF/WASD E2E weiterhin nicht live verifiziert: 39 %. Details docs/ALPHA455_464_LEGACY_WEARABLE_COF_COMPARISON.md. BEI WEITER zehn Blöcke Alpha.465–474; CI nur wenn genau geprüfter Feature-SHA erfolgreich. Kein Produktionsupdate.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. Alpha.455–464, Fortschritt 39 %.**

**ALPHA.454 PRÜFSTAND 19.10.2026:** Feature-SHA 9360106596ac437f97fbc626909eb9d4361a223a: Mint Linux-x64 Build 371/371; native C++/OpenGL 157/157; Python 193/193; Xvfb, X11 Taskleistenicon PASS. Raspberry Pi5 hat EXAKT diesen GitHub-SHA in separatem Test-Checkout mit pi5-headless-tests.sh bestanden: OGL_PI5_NATIVE_HEADLESS_OK, 193 Python-Tests (ein unterstützter Skip). GitHub Actions Pi5 Run 39283789898 noch QUEUED (KEIN CI SUCCESS behaupten). Kein GUI-/GPU-Livebuild am Pi, keine echte authentifizierte NexVerse Avatar/COF/WASD-Session. Projektfortschritt 39 %, nächster Zehnerblock Alpha.455–464. Repo-Hauptentwicklung auf GitHub, kein Produktionsdeploy.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. OGL 0.2.0-alpha.454, Projektfortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.455–464.**

19.10.2026 Alpha.445–454: Firestorm Legacy WearablesRequest Low381 bei echtem AgentMovementComplete 1x pro authentifiziertem Circuit READ-ONLY; Low382 WearablesUpdate mit 0..16 WearableType, SerialNum, strikter AgentID/SessionID/Regionprüfung und Last-Serial-Abweisung. Reauth/Regionreset behebt Legacy-Referenz-Leak. F3 unterscheidet Anfrage, alte Legacy-Antwort und reine AssetID-Zahl mit COF OFFEN; privater Report anonym numerisch. C++ echte synthetische LLUDP-Pakettests, OpenGL-F3, Python, ARM64 Headless und Pi5 GitHub CI. Kein automatisches Wear/Unwear, AgentSetAppearance oder Fake-Bake. Echtes NexVerse Avatar/Outfit/COF/WASD weiterhin NICHT live bewiesen: Stand 39 %. Details docs/ALPHA445_454_AUTH_LEGACY_WEARABLE_READBACK.md. Nächste 10 Blöcke Alpha.455–464; CI nur für exakt verifizierten Feature-SHA grün. Kein Produktionsupdate.

---

**ALPHA.444 Pi5-CI VERIFIZIERT:** Featurecommit 3bb183def3c0ef618173f705c635d6a87127c53d, GitHub Self-hosted Raspberry Pi5 ARM64 Source Check Run 39140161516 = SUCCESS exakt für diesen SHA. Mint x64 Build 369/369, CTest incl. real GL 156/156, Python 186/186, ARM64-kompatibles Headless, isolierter Xvfb-Viewer, echtes X11 Icon und WM_CLASS PASS. Kein eigener NexVerse Live-Avatar/Outfit/WASD E2E nachgewiesen; Projektfortschritt 39 % (38,65 % gewichtet), nächste Alpha.445–454.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.444, Fortschritt 39 % (38,65 % gewichtet), NÄCHSTE Alpha.445–454.**

18.10.2026 Alpha.435–444: Low158-Firestorm AttachmentBlock erhält tatsächlich gültige 65+ Einträge statt ungültiger 64er Obergrenze. Byte-Grenze 255 nur Feldsyntax, 1200B LLUDP-MTU bleibt strikt! Echter synthetischer LLUDP Low158 (65 Anhänge) geht bis in den Self-WorldModel-Status. F3 beschriftet SSA-Anhänge als nur gemeldete Serverdaten ohne fälschliche Renderbehauptung. Privater numerischer Report / Xvfb GL-Panelprüfung, Native C++/Python und Pi5 CI. Echtes NexVerse Outfit, Low158/COF, WASD weiterhin NICHT live verifiziert, 39 %. Einzelheiten docs/ALPHA435_444_LOW158_ATTACHMENT_COMPATIBILITY.md. BEI WEITER Alpha.445–454: zehn echte Blöcke; GitHub/Upstream zu Beginn prüfen.

---

**ALPHA.434 PI5-CI ERFOLGREICH VERIFIZIERT:** Featurecommit 83c93130b64b0ec1032b864414b52e89cabb4c22; GitHub self-hosted Pi5 ARM64 Source Check Run 39139112676 SUCCESS für genau diesen SHA. Mint Linux-x64 Vollbuild 367/367, CTest 155/155 incl. GPU OpenGL, Python 180/180, ARM64-kompatibles Headless, isolierter Xvfb-Viewer und X11 Systemicon/WM_CLASS PASS. Echt eingeloggtes NexVerse Self AvatarAppearance/Outfit und reale bestätigte WASD-E2E weiterhin offen. Projektstatus 39 % (38,65 % gewichtet). Nächste zehn Blöcke Alpha.435–444.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.434, Projektstatus 39 % (38,65 % gewichtet), NÄCHSTE Alpha.435–444.**

18.10.2026 Alpha.425–434: WASD-Rückmeldungen verwenden nur echte horizontale >=0,25m Positionsänderungen nach Socket-Send; vertikale Gravitationskorrekturen und >=32m Einzel-Positionssprünge sind keine Bewegungserfolge. Positionssprung setzt Vergleichsbaseline/Uhr neu, stale/zeitlich alte Samples ignoriert, alle Regionlease-Bewegungsbelege zurückgesetzt. F3 und privater Bericht erfassen Gründe ohne Koordinaten oder IDs. Native C++/Python/GL, Pi5 ARM64 und isolierte GUI/Icon-Prüfung. Fortgang docs/ALPHA425_434_WASD_DISCONTINUITY_GUARDS.md. Reales NexVerse AvatarAppearance/Outfit/WASD weiterhin offen, 39 %. BEI WEITER Alpha.435–444, immer zehn reale Blöcke. Aktuelle CI nur für exakt bestätigten SHA als grün notieren.

---

**ALPHA.424 PI5-CI VERIFIZIERT:** Featurecommit 74b1839b7ecdad43f80f7405638a7c1433dddf44, selfhosted Raspberry Pi5 ARM64 GitHub Run 39138145169 SUCCESS exakt für diesen SHA. Mint x64 Build 367/367, Native/NVIDIA-GL 155/155, Python 172/172, Pi5-kompatible Headless, isoliertes Xvfb und X11 Systemicon erfolgreich. Low158/COF, reale NexVerse-Avatar-/Outfit-Skins und serverbestätigte WASD-E2E bleiben OFFEN. Projektstand 39 % (38,65 % gewichtet). Nächste Alpha.425–434.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL Version 0.2.0-alpha.424, Projektfortschritt 39 % (38,65 % gewichtet); NÄCHSTE Alpha.425–434.**

18.10.2026: Alpha.415–424 – Reauth/Agentwechsel/Sessionwechsel/Regionswechsel/Disconnect invalidieren bisherige Self-Low158- und vorläufige ObjectUpdate-Skin-Daten sicher. F3/F9 führen nun vorläufige, tatsächlich vom Simulator gemeldete ObjectTE-Body-Bakes mit realen GPU-Residenz-, Original-GL- und Depth-Nachweisen getrennt von authentisch bestätigtem Low158. Native C++/GL, Python, Pi5-kompatible Headless, isoliertes Xvfb/Icon und Pi5-CI nach exaktem Commit. Keine authentifizierte NexVerse Avatar-/Outfit-/WASD-End-to-End-Abnahme. Fortschritt deshalb 39 % unverändert. Vollständige Dokumentation docs/ALPHA415_424_AUTH_AVATAR_SOURCE_PROOF.md. BEI WEITER Alpha.425–434, immer ZEHN reale Entwicklungsschritte.

---

**ALPHA.414 CI ERFOLGREICH VERIFIZIERT:** Featurecommit 08fa7ad14c3b95a97c2712751eba26dab384b716; GitHub Self-hosted Pi5 ARM64 Source Check Run 39137250919 = SUCCESS exakt für diesen SHA. Mint x64 365/365, native NVIDIA/OpenGL CTest 154/154, Python 165/165, Pi5-kompatibles Headless, isolierter Xvfb-Firestorm-Viewer, Original-X11-Systemicon/WM_CLASS PASS. Vollständiger eigener Avatar/Outfit/COF, echtes NexVerse-WASD und alle Firestorm-Menüs weiterhin NICHT live nachgewiesen; Fortschritt 39 % (38,65 % gewichtet). Bei WEITER Alpha.415–424.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.414, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.415–424.**

18.10.2026 Alpha.405–414: Tatsächliche Self-ObjectUpdate-/Compressed-TE-Header-/Upper-/Lower-Bakes bilden eine streng geprüfte vorläufige Alternative, falls Low158-AvatarAppearance fehlt. Provenienz im F3-Panel sichtbar, Privatbericht nur numerisch; gültige Low158 Self Appearance bleibt autoritativ. Korrekte Region/Agent/Session/LocalID, rebind/Reset/placeholder, Tests. Kein authentifizierter NexVerse Outfit-/GPU-/Bewegungs-E2E-Beweis; Fortschritt weiterhin 39 %. Dokumentation docs/ALPHA405_414_SELF_OBJECT_BAKE_FALLBACK.md. Aktuelle CI erst für neuen SHA als grün markieren. Keine Produktionsinstallation. Bei WEITER Alpha.415–424.

---

**ALPHA.404 CI VERIFIZIERT:** Feature-SHA 19e70975376b44719a6c61989d4748bebc0b9d4d, GitHub Pi5 ARM64 Run 39135891262 SUCCESS exakt für denselben Codecommit. Mint x64 Vollbuild 363/363, Native/GL CTest 153/153, Python 159/159, ARM64-kompatibler Headless, isolierter Xvfb-Viewerstart und X11-WM_CLASS/Systemicon erfolgreich. NexVerse Live AvatarAppearance, Outfit/Bake und WASD-E2E weiterhin OFFEN. Prozentstatus 39 %; nächste Alpha.405–414.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.404, Projektfortschritt 39 % (38,65 % gewichtet), nächste Alpha.405–414.**

18.10.2026 Alpha.395–404: Self AvatarAppearance Low158 TextureEntry wird im Paket-Handler und WorldModel mit Firestorm-Decoder validiert. Korrupte nichtleere TE verdrängen alte Skin nicht. F3 und privater numerischer Bericht zeigen fehlerhafte Texturblöcke; Regionswechsel setzt alte Self-Fehlerzähler zurück. Weiterhin kein authentifiziertes Live-NexVerse-Skin/Outfit/WASD E2E, darum Fortschritt 39 % unverändert. Dokumentation docs/ALPHA395_404_SELF_APPEARANCE_TEXTURE_INTEGRITY.md. Letzte belegte CI Alpha.394 Run 39134722480 SUCCESS. Neue CI erst für exakten SHA bestätigen. Bei WEITER Alpha.405–414.

---

**ALPHA.394 CI VERIFIZIERT:** Featurecommit c51dd658224604886d5a536b3bd22dfc49e83214; GitHub Pi5 ARM64 Actions Run 39134573794 SUCCESS für genau denselben SHA. Mint x64 Vollbuild 360/360, 152/152 Native C++/NVIDIA OpenGL, 152/152 Python, ARM64-kompatibles Headless, isolierter Xvfb-Vollviewer sowie X11 WM_CLASS/Icon PASS. NexVerse AvatarAppearance/Outfit/WASD Live-E2E weiterhin OFFEN. Projektstatus 39 % (38,65 % gewichtet), nächster Lauf Alpha.395–404.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.394, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.395–404.**

18.10.2026 Alpha.385–394: Bewegungsfeedback erst nach tatsächlichem erfolgreichem UDP-AgentUpdate, wartende Queue im F3 sichtbar, alternative echte regionale Self-Positionspakete (Terse/Full/Compressed), numerischer Whitelist-Privatbericht. Alpha.384 CI SHA 719db3f Pi5 Run 39133684078 SUCCESS. Keine real authentifizierte NexVerse AvatarAppearance/Outfit/Bake- und WASD-E2E belegt, deshalb 39 % unverändert. Details: docs/ALPHA385_394_UDP_SEND_AND_SERVER_MOTION.md. Mint x64 + Pi5 ARM64 CI, kein unerlaubtes Deployment. Bei WEITER Alpha.395–404.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). OGL 0.2.0-alpha.384, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.385–394.**

18.10.2026: Alpha.375–384 – Firestorm avatar_lad.xml Attachment-Slots besitzen jetzt Gelenk-IDs und lokale Offsets, originaler Avatar-Keyframe-/Procedural-Pose-Transform folgt dynamisch auch getragenen Prim-Linksets statt starrer Bind-Position. Echte NVIDIA OpenGL Framebuffer-Fixture: 1.108 geänderte Pixel, kein Bone-Fallback-Drift, HUD weiterhin 0 Pixel. F3 und privater whitelisted Report mit Bone-Pose-Anzahl, alte Funktionen bleiben. Echter NexVerse Live AvatarAppearance/Outfit/Bake/Bewegung weiter NICHT BELEGT; gewichteter Fortschritt bleibt 39 %. docs/ALPHA375_384_ANIMATED_ATTACHMENT_BONES.md. Branch develop-current, Mint Build und Pi5 ARM64 CI, niemals unfreigegebenes Deployment. Nächste Alpha.385–394.

---

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE MIT C++/GUI/TESTS/GITHUB/PI5-CI. Aktuelle OGL-Version 0.2.0-alpha.374, Gesamtfortschritt 39 % (38,65 % gewichtet), nächste Alpha.375–384.**

Alpha.374 CI VERIFIZIERT: Featurecommit dc7a7e7114154adaa444ded5ade9e5dfb97f1dce; GitHub Pi5-ARM64-Run 39131738405 SUCCESS exakt für denselben SHA. Mint Debug x64 Vollbuild 358/358, 151/151 native NVIDIA/OpenGL CTests, 140/140 Python, ARM64-kompatibler Headless, isolierter Xvfb Viewerstart und X11 Systemicon/WM_CLASS PASS. Echte 21-Level Avatar-Linkset-Framebuffer-Pixel 4729, 21 HUD-Prim-Ausschlüsse, keine verbliebenen HUD-Pixel; F3 zeigt reale Linktiefe. Echter NexVerse AvatarAppearance/Outfit-/WASD-End-to-End weiterhin OFFEN. Projektstatus 39 % (38,65 % gewichtet); nächster Lauf Alpha.375–384.


18.10.2026: Renderer-Fix für tief verschachtelte OpenSim-Anbauteile. Vorher 16-Level-Grenze bei 3D-Matrix trotz 128-Level-HUD-Filter. Jetzt beide über dieselbe sichere Parent-Chain (max 128, Zyklen, unbekannte Eltern ohne erfundenen Avataranker). Echter OpenGL-Framebuffer: 21-stufiges Avatar-Kleidungs-Linkset 4.729 RGB-Pixel, komplette HUD-Linksets 21 Objekte ausgeblendet und 0 übrig gebliebene HUD-Pixel. F3 zeigt echte maximale Linktiefe und Anzahl >16; privater Diagnosebericht speichert nur numerische GL-Hierarchiedaten. Echter NexVerse-Avatar-/Outfit-und WASD-Live-E2E weiterhin OFFEN. Details docs/ALPHA365_374_DEEP_LINKSET_TRANSFORMS.md. Bei neuem Chat zuerst develop-current HEAD, CHATUEBERGABE.md, projectplan.md, docs/PROJECT_PROGRESS.json lesen.

---

**ZEHN-BLÖCKE-REGEL: 10 ECHTE ENTWICKLUNGSBLÖCKE pro WEITER. Viewer 0.2.0-alpha.364, Fortschritt 39 % (38,65 %). Nächste Alpha.365–374.**

Alpha.355–364: HUD Root-, Kind- und Enkel-Prims durch Firestorm Attachment-IDs korrekt aus der Welt ausgeblendet; Zyklen beschränkt, normale Kleidung erhalten. F3 HUD 3D-aus GL-Zaehler. Server main 293d1d6: Profil-/Portal-/HG-Aenderungen, keine belegt neue LLUDP-Viewerpflicht. Echtes NexVerse Inworld Outfit/WASD E2E weiterhin offen. docs/ALPHA355_364_HUD_LINKSET.md.

**ZEHN-BLÖCKE-REGEL – BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE); Fortschritt 39 %; nächste Alpha.355–364.**

Alpha.354 CI VERIFIZIERT: Feature-SHA 2ed9f66ea96f49eb1b7ad2211aff87248c2b09c0; Pi5-ARM64 GitHub Actions 39079670826 SUCCESS für exakt diesen SHA. Mint x64 Build 352/352, native CTest 148/148, Python 132/132, ARM64-kompatibles Headless, Xvfb-Viewerstart und X11-Icon/WM_CLASS PASS. F3 WASD trotz offener Diagnose, Strg+S statt S, reale UDP-WASD-Datagramme und AvatarAppearance Fehlerabschnitt sichtbar. Authentifizierter NexVerse Bewegungs-/Skin-/Outfit-Nachweis weiterhin OFFEN. Gesamtfortschritt 39 %; nächster Zehnerlauf Alpha.355–364.

**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE) MIT C++/GUI/TESTS/GITHUB/PI5-CI. OGL 0.2.0-alpha.354, Gesamtfortschritt 39 % (38,65 % gewichtet), nächster Zehnerdurchlauf Alpha.355–364.**

18.10.2026: F3-Avatarfenster blockiert WASD NICHT mehr; nur Chat-/Teleport-Texteingabe sperrt Bewegung. F3-Bericht benutzt Strg+S statt S. 100-ms-WASD ignoriert Strg+S, kein ungewolltes Rückwärtsgehen. F3 zeigt echte erfolgreich per UDP-Socket versandte Richtungs-AgentUpdates als UDP-WASD, unabhängig von empfangenen Simulator-Positionswerten. Die Avatar-Pipeline-Seitenleiste zeigt den Firestorm-Low158-Decoderabschnitt für abgewiesene Serverpakete ohne UUID. Kein authentifiziertes NexVerse-Avatar-Outfit-/Bake-Pixel-E2E und keine serverseitig bestätigte Bewegung. Details docs/ALPHA345_354_LIVE_INPUT_WASD.md. Nächste Alpha.355–364 beginnt mit GitHub CHATUEBERGABE.md, Branch/Commit, projectplan.md, Fortschritt; Mint x64+Pi5 ARM64; kein autonomer Deploy.

---

Alpha.344: Pi5 CI 39078173957 SUCCESS für Featurecommit c04cf98baa8f6ca47f69577215036ba6768ac014. Mint: 148/148 Native, 126/126 Python, Headless, Xvfb und Taskleistenicon PASS. Live NexVerse Bewegung/Outfit weiterhin offen; Gesamtfortschritt 39 %. Nächste Alpha.345–354.

**ZEHN-BLÖCKE-REGEL: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE. Viewer 0.2.0-alpha.344, Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste Alpha.345–354.**

18.10.2026: Echte Firestorm-Low158 AvatarAppearance Fehlerphase sichtbar und ohne UUID/Paketdaten protokolliert; F3 enthält den fehlenden Appearance-Abschnitt und serverbasierte WASD-Positionsauswertung. Die Bewegungswarteschlange ersetzt unversendete alte WASD-Snapshots. Der authentifizierte NexVerse Outfit-Pixel- und WASD-Bewegungsnachweis bleibt OFFEN. Siehe docs/ALPHA335_344_AVATAR_MOTION_WIRE.md. Neuer Chat beginnt mit GitHub HEAD, CHATUEBERGABE.md, projectplan.md, docs/PROJECT_PROGRESS.json; Mint NVIDIA/x64- und Pi5-ARM64-Tests.

---

18.10.2026 LIVE-SCREENSHOT/HOTFIX: OGL-Starter bei Cinnamon separat und erhaltend angeheftet (Firestorm bleibt); adaptive GPU-Mesh-Count-Grenze 256..384 bei Detail, 256 im Sparmodus, VRAM-Byte-Limits unverändert. NexVerse-Grid-SelfAppearance fehlt weiterhin, Avatar einfarbig, WASD-Position konstant, 2 Texturen fehlgeschlagen. Details docs/LIVE_SCREENSHOT_2026-10-18_TASKBAR_MESH.md. Gesamtfortschritt 39 % (38,65 % gewichtet), nächster Zehnerlauf Alpha.335–344.

ALPHA.334 VERIFIZIERT: Feature-SHA a8cbb3c7fffab019643601ee4dd7517ef6870c70; GitHub Pi5 ARM64-CI Run 39073820122 SUCCESS genau fuer diesen SHA. Mint x64 Debug Vollbuild und 146/146 Native C++23/OpenGL-Tests, 119/119 Python, Pi5-kompatibles Headless, Xvfb-GUI und Originalicon/WM_CLASS PASS. Echte Firestorm-GPU Pixel: Avatar 3398, Head 234, Upper 1422, Lower 1742, Bake 3398, Eviction 0 stale Pixel. Neues F3 Avatar-Fokus [F4]. Kein authentifiziertes Live-Grid-E2E. Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste 10 Blöcke Alpha.335–344.

ZEHN-BLÖCKE-REGEL – ALPHA.334: Bei jedem WEITER 10 ECHTE ENTWICKLUNGSBLÖCKE inkl. C++, GUI, Tests, GitHub, Pi5-CI. Gesamtfortschritt 39 % (38,65 % gewichtet). Nächste 10 Blöcke Alpha.335–344. Original Firestorm OpenGL Framebuffer-Pixel jetzt lokal nachgewiesen: 3398 Avatar-RGB-Pixel, Kopf 234, Oberkörper 1422, Unterkörper 1742 Bake-Pixel, nach GPU-Textur-Eviction 0 veraltete Pixel. Live-NexVerse Login/Outfit/Pixels weiterhin OFFEN.

Alpha.324 verifiziert: Featurecommit 4a6a3fb4aa61e768b701d9db9bebd73476764cef; Pi5 ARM64 GitHub Actions 39070404127 SUCCESS. Mint x64 346/346 Build, 145/145 native, 118/118 Python, Headless, Xvfb und X11 Icon bestanden. Live NexVerse Outfit/Avatar E2E offen. Gesamtfortschritt 39 %, naechste Alpha.325–334.

**AKTUELL 17.10.2026: OGL 0.2.0-alpha.324, Gesamtfortschritt 39 % (38,65 % gewichtet). Nächster Zehnerdurchlauf Alpha.325–334; bisherige andere Termine darunter historisch.**

# OpenGenesisLINK Viewer – verbindlicher Projektfortschritt

**Gesamtfortschritt: ca. 39 % – gewichtete Schätzung.** **Nächster Zehnerlauf Alpha.315–324.** **Nächster Zehnerdurchlauf Alpha.305–314.**
Stand 17.10.2026 | Viewer 0.2.0-alpha.324 | Branch develop-current. Nächster Zehnerlauf Alpha.295–304.
Die Prozentzahl beschreibt die geschätzte Funktionsreife
zur vollständigen Firestorm/OpenSim/OpenGenesisLINK-Basis.
Sie ist weder Codezeilen- oder Testquote noch eine Freigabe.

**Alpha.255–264:** begrenztes manuelles Terrain-Nachladen, eigener Regions-F7-Button/T, L1–L4-/CAP-/Backoff-Status und konservierte RGBA-Layer. Tests/Bericht in docs/ALPHA255_264_TERRAIN_MANUAL_RECOVERY.md. **39 % bleiben unverändert.** **Nächster Zehnerdurchlauf Alpha.265–274.**

**Alpha.264 CI:** Commit acaab98d987c6c7602f806da4b33b2de8ae333a5; GitHub Pi5 ARM64 Run 38978541689 SUCCESS (SHA identisch), Mint 140/140 native, 85/85 Python, Headless/Xvfb/Taskleistenicon PASS. Reale Grid-E2E-Prüfung weiterhin offen.

**Alpha.265–274:** F3 Head/Upper/Lower + 8 weitere echte Server-Bake-GPU-/GL-/Depth-Stufen, begrenzte GetTexture-Replanung mit unverändertem Backoff. 39 % (38,65 %) unverändert; kein authentifizierter NexVerse-Live-Avatar-/Outfit-/Pixel-E2E-Beleg. **Nächster Zehnerdurchlauf Alpha.275–284.**\n\n**Alpha.274 verifiziert:** Featurecommit 427555ab8ccb59adbbc41cd00bf576cbedc62f07; pi5-ogl ARM64-CI 38980155018 SUCCESS mit identischem Feature-SHA. Mint x64 Fullbuild, 141/141 C++/NVIDIA GL, 90/90 Python, Headless und isoliertes Xvfb/Icon PASS. Bewertung 39 % unverändert.

**Alpha.275–284:** Drei getrennte Firestorm Original-Body OpenGL-Depth-Abfragen inkl. echter GPU-Region-/Sequence-Guards, F3-UI/Export. 39 % unverändert (38,65 %), da echtes NexVerse AvatarAppearance/Outfit-E2E weiter offen. **Nächste Zehnergruppe Alpha.285–294.**

Alpha.284 VERIFIZIERT: Featurecommit 2017b48fcc6586e00656bba01c6fe3058d0c98be, GitHub Pi5 ARM64-CI Run 39065802543 SUCCESS für exakt diesen SHA. Linux Mint vollständiger x64 Debug Build (340 Buildsteps), 142/142 Native-C++23/NVIDIA-GL-CTest, 95/95 Python, ARM64-kompatibler Headless, isolierter Xvfb-GUI und Original-Systemicon/WM_CLASS PASS. Authentifizierter Live-NexVerse Avatar/Outfit-Bake-E2E OFFEN. Gesamtfortschritt 39 % (38,65 % gewichtet); nächster Lauf Alpha.285–294.

**Alpha.285–294:** Originaler SelfAvatarAppearance-SSA-Handshake-Blocker behoben: RegionHandshakeReply Bit2 statt bisher Flags0. Echte NexVerse-Serverantwort sowie Outfit-/Bake-Pixel-E2E bleiben OFFEN, daher **39 % (38,65 %)** unverändert. **Nächster Zehnerlauf Alpha.295–304.**

**VERIFIZIERTER Alpha.294-Abschluss (17.10.2026):** Feature-Commit d4a216ead2be4b6a842e53ccbebd609ad76bb763; GitHub Pi5 ARM64-CI Run 39066916593 **SUCCESS** exakt für diesen SHA. Linux Mint x64 Fullbuild (340 Schritte), **142/142 native C++23-/NVIDIA-OpenGL-Tests**, **101/101 Python-Tests**, ARM64-kompatibler Headless, isolierter Xvfb-Viewer und X11-Originalicon/WM_CLASS **PASS**. Live-NexVerse-SelfAvatarAppearance-/Outfit-/Bake-/Pixel-E2E weiterhin OFFEN. **Gesamtfortschritt 39 %** (38,65 % gewichtet); nächste Zehnergruppe Alpha.295–304.

**Alpha.295–304:** Reliable-Tracker-korrelierte echte SSA-Handshake-ACKs (keine alten/duplizierten ACKs), 15s-Warte-Status, authentifizierter Self-UI-Green-Gate und lokale min. 0600-Diagnose. Live NexVerse SelfAppearance-/Outfit-/Pixel-E2E bleibt OFFEN. **39 % (38,65 %)** unverändert, nächste Alpha.305–314.

**ALPHA.304 TEST-GATES VERIFIZIERT:** Feature-SHA a6d7942173e330b840611ac1f55c952da3cca564; Pi5 ARM64 CI Run 39068198529 **SUCCESS für exakt denselben Commit**. Mint Debug x64 Fullbuild 342/342 Schritte, **143/143 Native C++23/NVIDIA-GL CTest**, **107/107 Python**, lokaler ARM64-kompatibler Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon **PASS**. Authentifiziertes Live NexVerse SelfAvatarAppearance/Outfit-Bake-Pixel-E2E weiter OFFEN. **Gesamtfortschritt 39 % (38,65 % gewichtet)**. Nächster Zehnerlauf Alpha.305–314.

Alpha.305–314: echte Self Low158 und WorldModel behalten bestätigte Avatar-Skin trotz Trial-/Teilpaket; F3 und private Diagnose zählen verworfene Nachrichten. NexVerse Live-Outfit-E2E weiter OFFEN. Gesamtfortschritt **39 %** (38,65 %), nächster Zehnerlauf **Alpha.315–324**.

**Alpha.314 VERIFIED:** Featurecommit decfc90ed089ff3c7a0b8544ba6fb4907086133c; GitHub pi5-ogl ARM64 CI 39069105455 SUCCESS exakt gleicher SHA. Mint Debug x64 Vollbuild 344/344, 144/144 native C++23/NVIDIA OpenGL CTest, 113/113 Python, Pi5-kompatibler Headless, isolierter Xvfb-Viewer und X11 WM_CLASS/Systemicon PASS. Authentifizierter NexVerse Live-Self-Avatar-/Outfit-/Bake-Pixel-E2E weiter OFFEN. Gesamtfortschritt **39 %** (38,65 % gewichtet); nächste Alpha.315–324.

Alpha.315–324: original Firestorm LLM Vertex-GPU-Skinning für tatsächliche Bone-Palette repariert. N-1 Joint wird nicht mit ungebundener Matrix vermischt. F3 weist echte Weight-/Clip-GL Draws aus. Authentifizierte NexVerse Avatar-/Outfit-/Bake-Pixel-E2E weiter OFFEN. Gesamtfortschritt **39 %**, nächster Zehnerlauf **Alpha.325–334**.

## Gewichtete Projektbewertung

| Bereich | Gewicht | Teilfortschritt | Beitrag |
|---|---:|---:|---:|
| Grid, Login, LLUDP, CAPS und Regionsverbindung | 15 % | 75 % | 11.25 Prozentpunkte |
| 3D-Welt, Terrain, Himmel und Renderer | 20 % | 43 % | 8.6 Prozentpunkte |
| Avatar, Server-Bakes, Outfits und Animationen | 20 % | 25 % | 5 Prozentpunkte |
| Texturen, Meshes, LOD und Materialsystem | 15 % | 47 % | 7.05 Prozentpunkte |
| Viewer-GUI, Menüs, Fenster und Bedienung | 15 % | 30 % | 4.5 Prozentpunkte |
| Inventar, Kommunikation, Teleport, Bauen und weitere Firestorm-Funktionen | 10 % | 10 % | 1 Prozentpunkte |
| Plattform-Releases, Stabilität, Sicherheit und E2E | 5 % | 25 % | 1.25 Prozentpunkte |

**Formel:** Summe (Gewicht × Teilfortschritt)/100
= **38,65 %**, gerundet **39 %**. Alle Gewichte ergeben 100 %.
Teilwerte sind fachliche, anhand nachweisbarer Funktionen
begründete Schätzungen und keine Abnahme.

## Nachweise und verbleibende Lücken

- **Grid, Login, LLUDP, CAPS und Regionsverbindung (75 %):** Realer Login/LLUDP-Handshake und SCENE_READY im Nutzerlog, EventQueue und Diagnosen; stabile Regionsübergänge noch nicht vollständig E2E belegt.
- **3D-Welt, Terrain, Himmel und Renderer (43 %):** GL-Szenen-/Terrain-Draws, progressive Materiallayer; Live-Terrainpixel und Firestorm-Umgebung weiter offen.
- **Avatar, Server-Bakes, Outfits und Animationen (25 %):** Originale Firestorm-LLM-Meshes sichtbar; aktuelles Self-AvatarAppearance fehlt, Bakes/Kleidung/Animation nicht vollständig.
- **Texturen, Meshes, LOD und Materialsystem (47 %):** GetTexture/GetMesh, GPU-Streaming und bounded Missing-Layer-Retry; vollständige High-LOD-/Materialqualität und Grid-E2E offen.
- **Viewer-GUI, Menüs, Fenster und Bedienung (30 %):** Originalicon, Welcome-Seite, Login, HUD, Diagnose, Kamera und Grundmenüs vorhanden; viele Firestorm-Bedienfunktionen noch offen.
- **Inventar, Kommunikation, Teleport, Bauen und weitere Firestorm-Funktionen (10 %):** Teilfunktionen und erste Oberflächen vorhanden; vollständige Bedienung, soziale Funktionen und Inworld-Aktionen vielfach unvollständig.
- **Plattform-Releases, Stabilität, Sicherheit und E2E (25 %):** Mint-x64-Nativbuild/NVIDIA-GL-Tests/Pi5-CI grün; Windows/macOS-Vollabdeckung und echte Grid-End-to-End-Abnahme fehlen.

## Pflichtablauf bei jedem neuen Projektstand / neuen Chat

1. Die ganz oben stehende Prozentregel in CHATUEBERGABE.md,
   projectplan.md und AGENTS.md zuerst lesen; Branchstand,
   Version und vorheriges Testergebnis erneut ermitteln.
2. Auf dem Linux-Mint-Checkout ausführen:
   python3 scripts/project-progress.py
3. Nach jedem Projektblock jeden Teilbereich neu bewerten;
   nur belegte, integrierte und getestete Funktionen
   duerfen den Fortschritt erhoehen. Fehler koennen ihn senken;
   bei unveraendertem Befund bleibt der Prozentwert identisch.
4. Immer in der Nutzerantwort UND zentral in
   docs/PROJECT_PROGRESS.json, docs/PROJEKTFORTSCHRITT.md,
   projectplan.md, CHATUEBERGABE.md und info.md
   den aktuellen Gesamtfortschritt in XX % synchron halten.
5. 100 % nur mit echter vollstaendiger Firestorm-/
   OpenGenesisLINK-Funktionsparitaet samt Live-Grid-Avatar,
   Bakes, Outfits, Animation, vollstaendiger Welt/
   GUI und Plattform-Releases. Keine 100 % nur durch CI.

**Nächster Zehnerdurchlauf:** Alpha.295–304 mit zehn
echten Entwicklungsblöcken. **Besonders offen** sind Self-
AvatarAppearance, vollständige Bakes/Outfits/Animationen,
Terrain, High-LOD-Objekte, fehlende Firestorm-Funktionen
und Windows/macOS-E2E.


## 16.10.2026 – Alpha.235–244, Fortschritt unverändert: 38 %

Die neuen Änderungen zeigen echte Self-Avatar-Keyframe-/Bake-
GL-Submissions, Terrain-Shader- und 4/4-Textur-Layer-Zustände,
sowie die LOD-Match-Quote statt bloßer Asset-UUID-Abdeckung.
Sie verbessern Diagnose, Bedienelemente und Testbarkeit, sind
aber **kein Nachweis**, dass der Simulator alle Self-AvatarBakes
liefert oder die Welt bereits Firestorm-paritätisch aussieht.
Die sieben gewichteten Teilbewertungen werden daher noch
nicht erhöht; **37,75 % werden weiterhin als 38 % angezeigt**.
Nächster 10-Block-Lauf: Alpha.245–254.

## Abschluss Alpha.235–244 – GitHub ARM64-CI bestätigt

Featurecommit 0f13dd74bef477b97e3cfbc924db1f3370e2a28d
auf develop-current, Pi5-CI GitHub Run 38974639521
conclusion SUCCESS (SHA korrekt). Linux Mint x64 Fullbuild,
137/137 Native/NVIDIA GL CTest, 73/73 Python, Pi5-kompatibler
Headless-Satz, echter isolierter Xvfb-Vollviewer-Smoke und
Linux-Original-Systemicon PASS. Alle 10 Blöcke 235–244
im projectplan.md FERTIG. Neuer Standard 10 statt 5 Blöcke
bleibt ganz vorne in CHATUEBERGABE.md, projectplan.md und
AGENTS.md. Nächster Durchlauf Alpha.245–254.
Gesamtfortschritt weiterhin 38 % (unverändert), da
NexVerse-Live-Firestorm-/Bake-/Terrain-Pixelbeleg OFFEN.


## 17.10.2026 – Alpha.245–254: 39 % statt 38 %

Neuer echter Code behebt die einmalige Terrain-Materialabfrage:
maximal vier fehlende Texturlayer-Retry-Versuche, persistente
Erfolge bei Teilausfall und echte progressive GPU-Promotion im
selben Materialepoch. Die OpenGL-Terrain-Zeichenaufrufe
melden jetzt nur aktuelle Frames. +0,90 Prozentpunkte auf
38,65 % ungerundet = 39 % gerundet; Live-E2E bleibt OFFEN.
Naechster Zehnerdurchlauf Alpha.255–264.

## GitHub CI Release-Abschluss Alpha.254 – 17.10.2026

Feature SHA 392b609c9820cdf62be49d960501f2627fc56b04,
Pi5-Self-hosted Runner Run 38976703736 completed SUCCESS
für exakt diesen Codecommit. Linux Mint x64
Ninja-Fullbuild, 140/140 Native/NVIDIA CTests,
80/80 Python, Headless-C++23, isolierter Xvfb-Viewer
und X11-Originalsystemicon sämtlich PASS.
Alle 10 Blöcke Alpha.245–254 im projectplan.md FERTIG.
Gesamtfortschritt 39 % aus den gewogenen Bereichswerten,
kein E2E-Beweis für den authentifizierten OpenSim-Avatar.
Nächster Zehnerdurchlauf Alpha.255–264.
