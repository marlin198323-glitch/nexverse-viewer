# Alpha.140–144 – Live-GPU-Preview, Körpertexturqualität, Serverinventar

Datum: 15.10.2026. Projekt OpenGenesisLINK Viewer 0.2.0-alpha.144.
Entwicklung: Raspberry Pi 5, ARM64, nexdev, Branch develop-current.
Basis-Commit 842d782929231e020187e8c95498174600c2eeb9.

## Block 140 – Render-Thread-Schnittstelle für originale Server-Bake-Texturen
Renderer bietet ausschließlich auf dem Render-Thread einen read-only
avatar_bake_preview_handle an: vorhandenes GL-Texturobjekt oder 0U, ohne
neuen GetTexture-Aufruf. Header avatar_bake_preview.hpp selektiert
Head/Upper/Lower nur bei gültiger aktueller Region und GPU-Residency.
Null- und Firestorm-Placeholder-IDs werden ausgeschlossen. Nur der
bereits mit dem authentifizierten WorldModel verbundene Self-Avatar ist
eine Quelle; keine freien benutzerdefinierten UUID-URLs.

## Block 141 – Sichtbare F9-Vorschau mit drei echten Bildern
Die bestehende klickbare F9-Action Appearance pruefen aktiviert neben
TE-/COF-/GL-Diagnostik im Outfit-Fenster drei Vorschauplätze Head,
Upper und Lower. Die Bilder kommen aus demselben Render-GL-TextureCache
wie die sichtbaren originalen .llm-Körpermeshes. Nicht geladene Assets
stehen mit nicht geladen am Platz, ohne synthetischen Fake-Avatar.
Die Anzeige erfolgt nur mit gültigem current-region/lease und
SelfAvatar-Proxy, liest Handle unmittelbar auf dem UI-Renderthread.
Die Vorschau ist kein lokaler Firestorm TexLayer-Bake und kein neuer
Wear-/Upload-Befehl.

## Block 142 – Avatar-Shader sicher vom vorherigen Weltmaterial getrennt
Beim Start des Avatar-Passes wird uObjectTexGenPlanar ausdrücklich auf
false und uObjectScale auf (1,1,1) gesetzt, zusätzlich zum schon
zurückgesetzten uUsePrimDeform. So erben die originalen Firestorm
Körper-UVs keine Planarprojektion des zuvor gerenderten prim materials.
Keine Änderung des originalen LLM UV-Satzes.

## Block 143 – Schärfere echte Textur des eigenen Avatars
Nach Nähe-Distanz-Hysterese und LOD-Dwell werden die drei existierenden
SelfAvatar-Bake-UUIDs im Texturplan auf CPU Basis-Mip 0 priorisiert.
Kein neues Texture-ID-Element, keine Umgehung von 256MiB-Bytebudget,
512 GPU Slots, Cache/Retry/epoch checks. Existiert nur eine
kleinere heruntergeladene Textur, bleibt die Darstellung bis
erfolgreichem regulären GetTexture- und GPU-Upload entsprechend.
Die höhere Qualitätsanforderung kann wegen VRAM-Mangel zurückgestellt
werden; ein F9-Screenshot wurde nicht als live bestätigt.

## Block 144 – Vollständiges commitweises OpenGenesisLINK-Inventar
Das neue Skript scripts/audit_opengenesislink_commits.py holt die
GitHub-Commit-Dateilisten einzeln paginiert (100 Einträge pro Seite),
statt sich auf max. 300 Dateien im kumulierten Compare zu verlassen.
Vollständigkeit: durchgehend commit-SHA, head-SHA, ahead_by-Zahl,
Seitenlimit, eindeutige Pfade, geschlossene Fehlerpfade überprüft.
Live-Ergebnis für Server main d31c03168f0f04f4ce02e82ee3246e16de744748:
22 Commits mit insgesamt 1.276 Änderungen an Dateipfaden.
Das maschinenlesbare Protokoll ist
docs/OPENGENESISLINK_UPSTREAM_INVENTORY_20261015.json.
1.208 Dateieinträge sind weiterhin manuell zu prüfen, u. a. der
umfangreiche Cache-/bin-Asset-Aufräumcommit mit 1.088 Einträgen.
Inventar vollständig bedeutet NICHT, dass Protokoll/Code geprüft
oder Firestorm-Viewer-Kompatibilität freigegeben sind.
Pinned reviewed_sha 4e99976630f1008db9014ea2b86c3723b09e9fe6
wird nicht automatisch geändert. Strict-Gate bleibt Exit 2.

## Nachweise und Grenzen
Auf Raspberry Pi5 lokal tatsächlich durchlaufen: sieben native g++14
C++23-Programme mit -Wall/-Wextra/-Wpedantic/-Werror; 28 Python
Unittests einschließlich Source-GL-Guard und Upstream-Commit-Paging;
bash -n und git diff --check. Self-hosted CI muss nach Push getrennt
ausgewertet werden. Kein vollständiger Viewer-CMake-Build und keine
OpenGL-/GUI-E2E-Prüfung, da CMake/vcpkg sowie Firestorm Original-Dateien
für ausführbare GPU-Tests fehlen. Windows/Linux-x64/macOS Builds ebenso
nicht ausgeführt. Live Avatar-Wear/Unwear, Source TexLayer-Pixelparität
und Server-Bake-Asset-Upload/CacheID bleiben sicher gesperrt.

## Nächste Blockgruppe
1. Autorisierter Viewer GUI/GPU Build und visueller Vergleich gegen
   originale Firestorm UV/LLM Assets am geeigneten Grafik-Host.
2. Prüfpipeline für Original TexLayer/Palette/Masken Pixelabweichungen,
   shaderseitig sichtbare echte Skin-Deformation.
3. Echte Outfit-Wear/Unwear-Bestätigung samt COF-Version,
   Rollback und verifizierter Simulator-Appearance readback.
4. 1.208 Upstream-Dateieinträge auf Code/Assets klassifizieren und
   risikorelevante Simulator-LLUDP-/CAPS-Änderungen manuell prüfen.
5. F9 Vorschau/Status und allgemeine Viewer-Navigation optisch
   in einer gestarteten Simulation auf Widescreen/Fullscreen testen.
