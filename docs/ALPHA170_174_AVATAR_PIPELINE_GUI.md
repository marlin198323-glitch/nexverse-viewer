# Alpha.170–174 – Avatar-Pipeline, GPU-Readback und responsive Inworld-GUI

Datum: 16.10.2026. OGL Viewer 0.2.0-alpha.174.
Linux-Mint-PC jam-MS-7D28 via Desktop Commander, Checkout
/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer. GitHub Branch
develop-current, Stand vor diesen Änderungen:
ac9010a49636b3c396979c09faf49eef8094a763.
Pi5 bleibt ARM64 GitHub CI, nicht primärer Entwickler.
Kein ungefragter sichtbarer Viewer-Start. Kein echter Login,
keine Outfit-Mutation und keine Produktion-Deploys.

## Alpha.170 – Systemstatus und CAPS überdeckten sich
Nach der Netzwerkkarten-Erweiterung in Alpha.169 befanden sich
Systemstatus-Überschrift und Renderer-/RAM-/VRAM-Zeilen noch
bei alten Koordinaten, sodass sie den CAPS-Status überlagern
konnten. Die gesamte Systemstatus-Karte ist jetzt konsistent
unterhalb der 205-pixelhohen Netzwerk-Karte positioniert:
Netzwerkkarte y+10..215, Systemkarte y+225..359,
Avatar-Diagnose ab y+370. Renderer-/RAM-/VRAM-Werte sind
wieder innerhalb ihrer Karte. Die neuen Werte erhalten
statische Regressionen im Linux-UI-Test.

## Alpha.171 – Authentische AvatarAppearance-Bake-Provenienz
Die neue header-only UI-Datenstruktur AvatarPipelineProof hält
nur lesbare, aktuelle Belege für:
- echtes WorldModel AvatarAppearance-Paket des aktiven Agent,
- 11 OpenSim/Firestorm Server-TextureEntry-Bake-UUIDs,
- echte bereits im lokalen GPU-Cache resident vorhandene Bake-UUIDs,
- die vom Renderer im gerade gerenderten Frame ausgeführten
  originalen Firestorm LLM-Körper- und Head/Upper/Lower-Bake-GL-Draws.
Die UI-Verkabelung prüft simulation_ready, aktives Login,
RegionLease-Epoch, dass Region verbunden ist sowie
AvatarAppearance/Session/Renderer-IDs identisch sind. GPU-Evidence
kommt nur aus object_texture_loaded_ids derselben Region,
kein clientseitig erfundener Bake-Upload. Ohne valides Paket
steht ausdrücklich "Appearance: Paket fehlt". Bei empfangenen
Texturen aber fehlendem GL-Draw bleibt der Status warnend.

## Alpha.172 – Sichtbarer Inworld-Avatar-Pipeline-Status
Neue dritte Karte rechts im Inworld HUD mit
"Avatar-Pipeline", "Server TE N/11 GPU M/11", "H:G U:S
L:- | LLM N BakeGL M" (G=GPU, S=Server-UUID ohne GPU).
Positive Farbe nur, wenn alle drei Server-Bake-UUIDs
GPU-resident sind und die originalen drei LLM-Meshes
im Frame mit allen 3 Bake-Texturen gezeichnet wurden.
Keine Behauptung real sichtbarer Framebuffer-Avatarpixel.
Die Karte wird bei engen Fensterhöhen ausgeblendet,
statt die untere FPS-/Frame-Diagnostik zu übermalen.
F9 Outfit-/Bake-Tabs sowie alle bestehenden Buttons
bleiben davon unberührt.

## Alpha.173 – Native Renderer/GPU-Integration
Neuer C++23-Pi5-Headless- und Linux-CTest
avatar_pipeline_proof_tests.cpp belegt fehlende Pakete,
genuine Server-IDs, GPU Residency, LLM/BakeGL Submission,
stale / unvollständige Belege und read-only NoUpload.
Bestehender Renderer RegionTransition CTest auf
verstecktem Linux-NVIDIA-OpenGL3.3 Context prüft
dieselben UI-Belege mit tatsächlich getätigten
GL-Draws und nach Entfernen der GPU-Texturen die
korrekte Rückstufung auf nur Server-UUID-Referenzen.
Die Fixtures sind nicht als echte authentifizierte
OpenGenesisLINK-Avatarpakete auszugeben.

## Alpha.174 – Voller Mint-x64-Build und Abgrenzung
CMake/Ninja auf Mint inkl. vollem ausführbaren Viewer,
alle GLFW_OpenGL_CTests und Python regressions.
Voll-Viewer-GUI-Smoke ausschließlich mit
scripts/mint-isolated-gui-smoke.sh unter Xvfb;
kein unerwartetes sichtbares Fenster. Nach erfolgreicher
Mint-Validierung GitHub develop-current pushen und
Self-hosted ARM64 Pi5-CI prüfen.
Aktuelle OpenGenesisLINK Server-Referenz:
ec760cacfff2fe505cf51991985553ed434eb121.
Die ältere 22-Commit-Serverprüfung bleibt BLOCKIERT,
reviewed_sha 4e99976630f1008db9014ea2b86c3723b09e9fe6
unverändert.

## Explizit offen
Echter erfolgreicher NexVerse-Login, Welt und Original-Avatar
sichtbar in einem authentifizierten Framebuffer, komplette
Firestorm TexLayer-Pixelkomposition für alle elf Bakes,
sicheres COF Wear/Unwear mit Nutzerzustimmung/Readback/
Rollback und vollständige Firestorm-Menüparität sind
NICHT mit Headless- oder lokalen GPU-Fixture-Tests bewiesen.

## Nächste fünf Blöcke
1. Echten Test-Grid-Login mit ausdrücklicher Erlaubnis
   und keinen eingebetteten Zugangsdaten nachvollziehen.
2. Welt und Avatar tatsächlich sichtbare GL-Pixel vs
   Framebuffer/Simulator-AvatarAppearance verifizieren.
3. F9/Wearables mit serverbestätigtem aktuellem COF
   und konsequenter Berechtigungslogik ergänzen.
4. Firestorm UI Navigation und Bedienbarkeit zusammen
   mit Asset-/Netzwerkcodes weiter vervollständigen.
5. Ältere Server-CAPS-/LLUDP-Patches einzeln abhaken,
   Kompatibilitätspin erst nach vollständiger Freigabe.
