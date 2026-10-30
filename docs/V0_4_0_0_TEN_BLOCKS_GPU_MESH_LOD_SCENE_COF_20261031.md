# OGL v0.4.0.0 – Zehn weitere Blöcke: Mesh-Upload-Sturm, Textur-VRAM und Avatar-COF

Stand: 31.10.2026 | Linux Mint jam-MS-7D28 | Branch `develop-current`. Keine Veröffentlichung auf main.

## Authentifizierter Ist-Befund aus dem Log 31.10.2026, 00:04–00:05

- Erfolgreicher Grid-Login; EEP-Himmel erfolgreich vom Simulator empfangen.
- Letzter Welt-Snapshot nach 30s: **238 angeforderte Texturen, 182 GPU-resident, 56 noch ausstehend**. 29 Mip0, 153 reduzierte. Vier einzigartige Texture-Missings, echte HTTP-404-Antworten; Material-/Himmel-Visualqualität muss visuell abgenommen werden.
- GetTexture 450 Dispatches, 183 Disk-Hits, 6 RAM-Hits, nur ein Netzwerk-Erfolg; **GPU-Texturbudget 268429792 von 268435456 Bytes** nahezu vollständig verbraucht.
- GetMesh 552 Dispatches / 551 Worker-Completions, 185 Disk-Hits, 366 RAM-Hits, **366 LOD-Swaps**, obwohl 185 GPU-Resident-ID angezeigt werden. Im selben Live-Log **554 GetMesh-Upload-Ereignisse, nur 186 einzigartige UUID/LOD-Paare**; drei häufige Wiederholungskandidaten bis 237-mal. Kernursache: Im Cache-Vorzugspfad war `existing->second==desired` als zulässig markiert, obwohl der normale Pfad es ausdrücklich ausschloss.
- Eigener Avatar: authentifizierte Low158 Appearance empfangen, aber **Head/Upper/Lower 0/3 echte Bakes**, 0/3 GPU; Legacy Low382 meldet 5 Wearable-Einträge, beweist nicht Current Outfit Folder oder vollständig geladenes Avatar-Outfit. Kein vertrauenswürdiger vollständiger Bake-Transfer nachgewiesen.

## Zehn implementierte Entwicklungsblöcke

1. **Live-Messwertanalyse**: aktuelle, nach dem vorherigen Commit eingeloggte Sitzung auf Mint bewertet; Problem als redundant ausgelöste GPU-Mesh-Uploads statt langsamen HTTP-GetMesh eingegrenzt.
2. **Einheitlicher Mesh-Anforderungsfilter**: neue `mesh_gpu_request_needed`-Regel in `mesh_lod_policy.hpp`. Identische GPU-LOD wird nie erneut geladen, außer bei echtem Qualitätssprung oder neuer GPU-Residency.
3. **Beide GetMesh-Scheduler korrigiert**: Cache-Vorzugs- und regulärer Netzwerkpfad rufen nun dieselbe Bedarfsprüfung auf. Die bestehende Zwei-von-drei-Warmcache-Fairness bleibt.
4. **GPU-LOD-Nativregression**: echte C++-Tests für bereits residente High-LOD, gewünschte LOD-Änderung, Kapazitätsgrenze, neue Assets und vorhandene Slots.
5. **Schneller COF-Autostart**: authentifizierte, schreibgeschützte InventoryDescendents2-Abfrage, einmalig sofort pro aktueller Regionsepoche, statt auf die bisherige 30s-Pollfrist zu warten, sofern ein vorhandener COF ohne gültige Items erkannt wird.
6. **Owner-/Regions-/CAPS-Diagnose bei fehlendem Avatar-COF**: unterscheidet fehlenden COF im Login-Skeleton von fehlenden Itemdaten; protokolliert nur Verfügbarkeitsbits und keine UUIDs, Login-Daten oder CAPS-URLs. Kein inventarveränderndes AgentSetAppearance.
7. **GPU-bewusste Materialbild-Rettung**: `choose_scene_texture_budget_mip_rescue` ermittelt nach originalem RGBA-Decode die kleinste echte Mip-Verkleinerung (höchstens Mip6), die in Shared-GPU-, Staging- und Texturfamilienbudget passt, statt ein fehlendes Material vorzutäuschen.
8. **Avatar- und Bildqualitätsschutz**: eigene sowie fremde Head/Upper/Lower-/Eyes-/Hair-Bakes sind von der gewöhnlichen Welttextur-Reduktion ausgeschlossen. Die hochwertige Variante wird stets zuerst verwendet, wenn sie sicher hineinpasst.
9. **Mip-Akzeptanz/Cooldown/Testdiagnose**: nur nach erfolgreichem atomarem GPU-Upload ein 90s-Cooldown (maximal 256 Einträge) gegen sofortiges Mip-Up-/Downgrade; reset bei Regionsepoche. `ogl-asset-diagnose.py` zählt wiederholte Mesh-Uploads und berichtet über COF-CAPs ohne Identifikatoren.
10. **Vollständige Tests, Roadmap und Übergabe**: native GPU/LOD/Mip-/Cooldown-Tests; 13 zusätzliche Python-Verträge; Dokumentation des nachweisbaren Befundes, der Live-Lücken und aller Release-Sperren. Der Nutzerstart bleibt unverändert.

## Benutzerstart

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach dem Einloggen zur Diagnose:

```bash
python3 scripts/ogl-asset-diagnose.py
```

## Nachweisbare lokale Prüfungen

- Linux Mint x64 CMake/vcpkg Vollbuild: **PASS**.
- Native CTest: **195/195 PASS**, insbesondere die neuen Mesh-LOD-Bedarfsfälle und Szene-Textur-Mip-Rettung/Cooldown.
- Python Regressionen: **969/969 PASS**, davon 13 neue für Cache-Dedup, COF-Bootstrap, echte Materialpixel, Datenschutz und GPU-Budgets.
- Original X11-Systemicon (128px) und Viewer GUI-Start: **PASS**.
- `bash scripts/pi5-headless-tests.sh`, exakte GitHub-Source-Check-Teststrecke (inklusive 969 Python-Tests): **PASS**.
- Echte Nachher-InWorld-FPS-/Residency-/Avatar-Bake-Abnahme: **OFFEN**.

## Nächste echte Abnahme (nicht als erledigt ausgeben)

- Beim nächsten authentifizierten Viewer-Lauf nach diesem Commit müssen `GetMesh streaming metrics: lod_swaps`, unique UUID/LOD repeats, `scene_mip_rescue_uploads`, Texture-Residency und tatsächliche Mip0-Qualität neu gemessen werden. **Keine behauptete FPS- oder Prozentverbesserung ohne Live-Werte.**
- Den serverseitig fehlenden Head-/Upper-/Lower-Bake-Nachweis und die echte COF-/Outfit-Auflösung prüfen. Bei 0/3 bleibt das graue/nackte Avatarbild unbehoben, bis ein valider Grid-Bake-/Appearance-Workflow existiert.
- Fehlende serverseitige Asset-UUIDs (HTTP404), WindLight-Sky und Sonne, Bewegungs-/Teleport-Readback und übrige Firestorm-Viewer-Parität bleiben offen.
- Fortschritt unverändert **38,65 % gewichtet (gerundet 39 %)**. `v0.4.0.0` IN ARBEIT. **Kein main-Merge, Tag oder Release.**
