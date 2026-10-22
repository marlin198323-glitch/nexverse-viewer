# Alpha.745–754 – Avatar-GetTexture-Budget entspricht echter GPU-Pose

22.10.2026 | Linux Mint Desktop Commander jam-MS-7D28 |
Branch develop-current | OGL 0.2.0-alpha.754
Projektfortschritt 39 % (38,65 % gewichtet).

## Arbeitsumgebung
Verbindlicher primärer Code- und GUI-Rechner: jam-MS-7D28,
Checkout /home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer.
Raspberry Pi 5 nur ergänzender ARM64-CI-Runner.
CHATUEBERGABE.md und projectplan.md zuerst gelesen,
Checkout sauber und GitHub HEAD 3492907580ea5df4566a0fd6e20a9a3dfac8afa1.
Vorige GitHub-Pi5-CI Run 39809196307 für Code-SHA
cbdfd857c00a6d9c3c897a5c2f3441223d080cf9
COMPLETED SUCCESS. OpenGenesisLINK-Servermain
881b373738ce889f4965ca2478bdb455f1d827f2
unverändert, nach zuletzt verglichenen neun Servercommits
#265–271 und Firestorm Single-Name-Login #268.

## Zehn technische Entwicklungsblöcke

745. Linux-Mint-Commander, GitHub-Branch, Handoff,
Projektplan und letzten exakten Pi5-Run geprüft; keine
unaufgeforderten Änderungen am sichtbaren Nutzerdesktop.

746. Unveränderten aktuellen OpenGenesisLINK-Servermain
881b3733 festgestellt. Kein erfundener Low158/COF/API-
Vertragswechsel. Ältere nicht überprüfte Upstream-Änderungen
weiter MANUAL_REVIEW.

747. Echter Bild-/Skin-Bug im Renderer/Streaming gefunden:
src/render/opengl_renderer.cpp::set_avatar_proxies
verwirft aus Sicherheitsgründen nichtfinite bzw. unplausible
Avatar-Weltpositionen vor OpenGL. Aber src/app/application.cpp
schickte dieselben abgewiesenen Avatar-Proxys weiterhin
ungeprüft in GetTexture-Bodykandidaten und die
Texture-Mip-/VRAM-Planung. Selbst-/Fremd-Bake-Streaming
und GPU-Zulassung konnten deshalb unterschiedliche
Avatarbestände und falsche Budgetprioritäten verwenden.

748. Gemeinsam benutzten C++23 GPU-Admission-Adapter
include/nexverse/assets/avatar_bake_gpu_admission.hpp
(MPL-2.0) implementiert. Er verwendet den vorhandenen
render::sanitize_avatar_proxy_pose für eine Kopie
der Proxypositionen, filtert ungültige eigene/fremde
Posen und übernimmt ausschließlich echte vorhandene
servergegebene Head/Upper/Lower-Asset-IDs. Keine
erfundenen UUIDs, keine veränderte Simulatorphysik
und keine unzulässige Low158-Aufwertung.

749. In der echten Anwendungs-/GetTexture-Pipeline
nur einmal pro Frame den GPU-geprüften Avatarbestand
ermittelt und den Self-Avatar daraus gesucht.
avatar_bake_source_refresh, automatische
GetTexture-Replans und die manuelle Nachprüfung
verwenden daher dieselbe zugelassene Self-Referenz;
bei ungültiger eigener Pose keine falsche erneute
Body-Bake-Einplanung.

750. Auch die priorisierten GetTexture-Head/Upper/Lower-
Requests aller Avatare und die in
resident_texture_samples eingespeisten Assetpositionen
auf die zugelassenen GPU-Positionen umgestellt.
Ungültige Remotes konsumieren keine knappen 512
gemeinsamen GPU-Texturplätze bzw. HTTP-Request-Slots,
während gültige Remotes weiterhin gestreamt werden.

751. Die bisherige Original-Firestorm-Self-Bake-
Full-Mip-Priorität ebenfalls an die zugelassene
eigene Renderpose gebunden. Self wird nicht anhand
einer anfälligen festen Liste-Position ausgewählt;
das 3-Layer-Protokoll Head/Upper/Lower bleibt
unverändert. Ohne echte GPU-Zulassung kein
irreführender 0-Mip-/Vollskin-Status.

752. Native C++23-Regression in
tests/avatar_bake_stream_priority_tests.cpp
mit tatsächlichem RenderAvatarProxy-Eingang ergänzt:
NaN Remote-Daten verwerfen, korrekt validierte
Self-Bakes bei geänderter Listenreihenfolge zuerst,
gültige Remote-Faces danach, keine ungültigen
Asset-IDs im plan_texture_residency, beschädigte
Remote/Self-Quaternion bei valider Position sanieren
statt gültige Bakes zu verlieren, ungültige Self-Pose
ohne erdachte Skin-Bereitschaft.
Bestehende Firestorm/VRAM-Texturpriorität bleibt.

753. Python-Quellvertrag
test_alpha745_754_avatar_bake_gpu_admission_contract.py
für echte gemeinsame pro-Frame-Zulassung,
GetTexture-Replan, Voll-Mip und Codeverdrahtung
ergänzt. Linux Mint nativer betroffener CMake-Target
nexverse-avatar-bake-stream-priority-tests sowie
nexverse-viewer erneut erfolgreich gebaut/getestet.

754. Version, Projektplan, AGENTS, Chatübergabe,
Fortschritts-JSON und Firestorm-/UI-/Server-
Paritätsmatrizen konsistent dokumentiert.
Vollständigen Linux-Mint x64 CMake/vcpkg-Gate,
native CTests, Python, isolierten Xvfb Viewer
und OGL-Icon erneut prüfen, dann Code-Commit/
Push zu develop-current. SHA-genaue CI-Ergebnisse
nach dem Push getrennt erfassen. Kein Deploy.

## Ausstehende P0-Abnahme
Die Änderung beweist, dass der GPU-abgelehnte
Avatar keine GetTexture-/VRAM-Slots mehr verbraucht.
Sie ist noch KEIN Beweis für einen echten
authentifizierten NexVerse-Avatar mit erhaltener
Low158 AvatarAppearance, COF-/Outfit-/Skin-Assets,
echt gebackenen Head/Upper/Lower-Pixeln und
serverbestätigter WASD-/Yaw-Bewegung.
Diese Live-Grid-/GPU-Ende-zu-Ende-Tests sind
still open. Projekt bleibt 39 %.
Nächster Block Alpha.755–764 auf Linux Mint.

## Lokale tatsächliche Alpha.754-Releaseprüfung
Linux Mint Desktop Commander jam-MS-7D28,
scripts/mint-viewer-validation.sh Exit 0:
vollständiger CMake/vcpkg-Debug Linux-x64-Viewer-Build,
**175/175 native CTests** inklusive der realen
AvatarBakeStreamPriority-/VRAM-/GetTexture-Prüfung,
**373/373 Python-Tests**,
isolierter Xvfb-Viewerstart OpenGenesisLINK
Version 0.2.0-alpha.754 mit originalem Firestorm
133-Joint-Rig, drei GPU-residenten Körpermeshes
(7186 Dreiecke), originalem OpenGenesisLINK
X11 WM_CLASS und Taskleistenicon 128x128.
Kein Login/Passwort/Produktivdeploy, kein Inworld-Test.
