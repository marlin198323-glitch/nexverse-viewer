# Alpha.230–234 – Sichtbare Welt, Avatar, Terrain, Mesh-LOD, Kamera/UI

Datum 16.10.2026. Version 0.2.0-alpha.234.
Arbeitsplatz Linux Mint jam-MS-7D28 Benutzer jam, Desktop Commander.
Branch develop-current; Repo marlin198323-glitch/nexverse-viewer.
Ausgangsstand 9e6e834503170c9eb3b2d1f7ec4ccc38f176dfdb.
Pi5 dient nur als Self-hosted ARM64-CI; kein Entwicklungscheckout.
Original-Logo, Systemicon und NexVerse Welcome-Webseite bleiben erhalten.

## Alpha.230 – Echte AvatarAppearance statt irreführender Platzhalter

Der LLUDP Handler zaehlt atomar alle AvatarAppearance-Pakete:
eingegangen, nicht dekodierbar, wegen Region-Status verworfen,
fremde Avatare und tatsaechlich vom WorldModel akzeptierte
Self-AvatarAppearance-Pakete. Nur ein akzeptierter Self-Readback
darf Appearance belegen; Ankunft eines Fremd-Pakets oder GL-Draw
reicht ausdrücklich NICHT. Das reale Avatar-HUD unterscheidet
jetzt fehlende, fremde, invalide und alte-Region Pakete,
ohne UUID, Avatarname oder Session-Geheimnisse zu zeigen.
Native C++23-Regression prueft alle getypten Anzeigestufen.
Keine neuen, ungenehmigten AgentSetAppearance-Pakete gesendet.
Der Server muss den fehlenden Self-Readback weiterhin liefern.

## Alpha.231 – Graues Terrain: Shader-Nachweis und Materialkontrast

Terrain-Texturen wurden nachweislich heruntergeladen und auf
die GPU geladen, konnten aber bei nicht uebereinstimmender
RegionGeneration/MaterialRevision im Renderer ungenutzt bleiben.
Die bestehende Bedingung wird jetzt per reinem C++23-Helper
getestet und unterscheidet im RenderDiagnostics fuer den
echten GL Terrain Draw tatsaechlich genutzte Textur-Branch
von der einfarbigen Vertex-Fallback-Branch.
Sichtbar im rechten Systemstatus:
Boden DETAIL-TEXTURE GL / GRAU MATERIAL FEHLT / kein GPU-Draw.
Shader-Mix gewichtet echte Terrain-Texturen mit 96.5% statt
88% und vermeidet zusaetzliches Ausgrauen durch Vertexfarbe.
Ein echter NexVerse-Screenshot und Firestorm-Pixelvergleich
sind weiterhin fuer Terrain-Materialparitaet erforderlich.

## Alpha.232 – DETAIL / SPAREN ist ein echtes Mesh-LOD-Control

MeshLodPolicy behaelt BALANCED als alten Profilpfad.
Neues DETAIL-Profil waehlt bei apparenter Distanz kleiner
45 high, kleiner 110 medium, kleiner 210 low, danach lowest;
die Hysterese arbeitet konsistent je Profil.
Echter F2-Shortcut und neben AVATAR F4 gezeichneter
DETAIL [F2] / SPAREN [F2]-Mausbutton wirken auf dieselbe
im Live-Streaming genutzte Mesh-LOD-Policy, auch bei F5-HUD-
Ausblendung. Profilaenderungen invalidieren den alten
LOD-Dwellzustand und planen neu. GPU-Budget und asynchrone
Downloader bleiben aktiv. So viele High-LOD-Meshes wie
im Server-Asset wirklich vorhanden sind, aber keine
erfundenen echten Meshes. Native LOD/Hysterese/Click
Tests und Python Wiring Contract vorhanden.

## Alpha.233 – Standardkamera / Widescreen wirklich nutzbar

Default-Kamera und F4 Frontal-Fokus von 8m auf 5.5m
reduziert. Zoom bleibt voll steuerbar mit Mausrad,
+/- und 3–24m Limits. Es gibt keine LLUDP-Agentenrotation.
Die kritischen Live-Texte im rechten UI
AvatarAppearance, Bake, GPU-Fragmente, Welt-/CAPS-Zustand
sind nun 8.7–9.1 statt nur 7.0–8.0 Skaleneinheiten groß.
Authentifiziertes F3-Fenster, original Firestorm
Outfit/F9 und WelcomePage unveraendert.
Native Camera- und Contracttests pruefen die Aenderung.

## Alpha.234 – Freigabe und Grenzen

Release-Gates: vollstaendiger Linux-Mint CMake/Ninja
Debug-x64 Build, Native C++23 und echte NVIDIA OpenGL
CTests, Python, Pi5-kompatibles Headless, isolierter
Xvfb GUI-Viewer, WM_CLASS Systemicon, Quellenprüfung,
GitHub Commit/Push, Pi5-Self-hosted-ARM64-CI für denselben SHA.
Projektplan, AGENTS.md, info.md, CHATUEBERGABE.md und die drei
Kompatibilitaetsmatrizen muessen aktualisiert werden.

Server-Vergleich: opengenesislink/opengenesislinkserver main
ec760cacfff2fe505cf51991985553ed434eb121, Oct 16 08:13Z.
Reviewed Server Baseline bleibt ungeaendert, da Patch-Level
Review der neueren Commits nicht abgeschlossen ist.

OFFEN: Echte Self-AvatarAppearance vom NexVerse Simulator,
vollstaendige ServerBake-/Outfit-Texturen, Terraingrau/
Materiallayer wie Firestorm und saemtliche Mesh-LoDs;
Animations-/COF-Paritaet weiterhin nicht end-to-end getestet.
Testfixtures/Debug GPU Draws sind KEIN Beleg sichtbarer
vollstaendiger Avatar-Texturen nach echtem Login.
