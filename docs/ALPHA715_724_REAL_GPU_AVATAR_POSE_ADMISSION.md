# Alpha.715–724 – Firestorm-Avatar GPU Pose-Schutz und echtes OpenGL-Framebuffer-Readback

21.10.2026 | Linux Mint Commander jam-MS-7D28 | develop-current |
Version 0.2.0-alpha.724 | Gesamtfortschritt 39 % (38,65 % gewichtet).

## Zehn Arbeitsblöcke mit Code und Prüfung

715. Zuerst CHATUEBERGABE.md und projectplan.md auf Linux Mint
gelesen, clean HEAD 039aa2c und GitHub develop-current verifiziert.
Pi5 GitHub-CI Run 39655327950 für echten Alpha.714 Code-SHA
bdd247ae6e894c7a135f64735f7da42881d5ac9a:
COMPLETED / SUCCESS. Linux-Mint-Rechner jam-MS-7D28 bleibt
verbindlicher Arbeitsplatz; Raspberry Pi nur CI.
716. OpenGenesisLINK Servermain 31f3d129 bestätigt, kein neuer
Commit seit dem einzeln überprüften #263 zu TOS /service/tos.
Keine neue spekulative OpenSim-Schnittstelle eingebaut.
717. Echten GPU-Problemweg in src/render/opengl_renderer.cpp
analysiert: set_avatar_proxies übernimmt ganze Simulatorpose;
quaternion_rotation nutzt sqrt aus float x*x+... und würde
bei NaN/Infinity ein GPU-Matrix-NaN erzeugen. Auch ungeprüfte
nicht-finite RenderAvatarProxy-Positionen konnten auf drei originale
LLM-Körpermeshes und deren Bake-Shaders übergreifen.
718. Zentralen MPL-2.0 GPU-Admission-Guard
include/nexverse/render/avatar_proxy_pose_integrity.hpp gebaut.
Reject Position NaN/±Infinity/Größenordnung >= 1e6;
gültige Rotation in double vor Ausführung der GL-Matrix normalisieren,
nichtfinite/Zero-Quaternion auf Identity. Unveränderte
authoritative WorldModel-AgentState-/LLUDP-Daten.
719. Guard in der echten set_avatar_proxies-Prozedur vor
Avatar-GL-Submission, nicht nur in einer Test- oder HUD-Funktion
verdrahtet. Schlechte Pose verwerfen, gültige Avatare
im selben Frame weiter akzeptieren. Maximales Budget 64.
720. Echte RenderDiagnostics um pro-Frame-Zähler der verworfenen
Posen, korrigierten Quaternionen und verworfenen Self-Pose
erweitert. Der GL-basierten AvatarPipelineProof die
authentischen Renderer-Admission-Ergebnisse zugewiesen;
F3/HUD zeigt Warnstatus, bei Self-Drop klare Meldung
GPU-Pose ungueltig (kein Mesh). Keine Pseudo-Skin-Freigabe.
721. Tatsächliche Kamera-Statusberechnung auf die visuelle
AvatarProxy-Position einschließlich gültigem Low158-Hover
umgestellt. Zuvor prüfte das HUD immer render_agent.position
ohne Hover, obwohl das GL-Mesh eine andere Z-Position hatte;
damit konnte der HUD-Status fälschlich Sichtbarkeit anzeigen.
722. Native C++23-Tests für NaN, ±Infinity, unplausible
Positionen, Zero/NaN-Quaternion, riesige aber endliche
Quaternionwerte, sichere Normalisierung und Frustum-Projektion
mit 1m Hover ergänzt; CMake/Pi5-Headless-Runner erweitert.
723. Echten existierenden Firestorm-Original-Körper-
Framebufferpixeltest erweitert (Xvfb, OpenGL, originale
Head/Upper/Lower LLM). Bei invalid Self-Pos keine GL-Pixel,
kein Original-Mesh-Draw; bei invalid Quaternion vorhandene
Meshpixel wieder identisch; invalid Remote-Pose darf echte
Self-Pixel nicht beeinträchtigen. CTest des realen GL-Pixelpfads
bestanden; vollständigen Mint-CMake-Test-Gate durchführen.
724. VERSION, sämtliche Projektplan-/Handoff-/Server-/UI-/
Firestorm-Matrizen und JSON-Fortschritt konsistent
aktualisieren. Volle Linux Mint x64 Build, CTest, Python, Xvfb
und OGL-Taskbar Icon prüfen; nach Erfolg auf GitHub
develop-current committen und exakte SHA-CI separat festhalten.
Kein Deploy, kein Login auf interaktivem Benutzerdesktop.

## Grenzen der Evidenz
Framebuffer-Tests und die 133-Joint Firestorm Bind-Pose-Meshes
beweisen Rendering mit synthetischen kontrollierten Daten;
authentifizierte Real-Grid-NexVerse-Skin-/COF-/Outfit-/Bake-
Sichtbarkeit oder Server-WASD-Readback bleibt UNBESTÄTIGT.
Die lokale x64-GPU-Kompatibilität wird im isolierten Xvfb mit
Software-OpenGL getestet; NVIDIA RTX 4070 SUPER ist installiert,
aber kein echter Inworld-NVIDIA-Pixel-/Session-Abnahmebeweis.
Gehostete GitHub-x64-CI war zuletzt vor Runnerzuteilung
(0 Steps) fehlgeschlagen, nicht am OGL-Compiler.
Die prozentuale Gesamtschätzung bleibt deshalb unverändert 39 %.

Nächste zehn Arbeitsblöcke **Alpha.725–734**: realen,
authentifizierten NexVerse-CAPS/Low158-/COF-/Outfit- und
AgentUpdate/WASD-E2E-Weg priorisieren; Linux Mint primär.
