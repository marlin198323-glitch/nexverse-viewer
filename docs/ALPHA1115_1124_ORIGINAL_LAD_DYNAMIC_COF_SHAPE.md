# Alpha.1115–1124 – dynamischer originaler Firestorm-LAD-COF-Shape-Katalog

24.10.2026 | Linux Mint Desktop Commander jam-MS-7D28 | develop-current | 0.2.0-alpha.1124

## Zehn Entwicklungsblöcke

1115. Linux Mint Desktop Commander jam-MS-7D28, nicht Pi5. Sauberer GitHub develop-current HEAD 69b917dad91e2b526da3e06c42d04e84b78861b4, zuerst CHATUEBERGABE.md und projectplan.md gelesen.
1116. Originales Firestorm avatar_lad.xml und tatsächliche Mesh-Parameter analysiert; upstream OpenGenesisLINK Server main debdcce15d6175a3adc1cd81fd5bd0715317080b über GitHub abgeglichen. World-API-Rollenrechte neu, keine Avatar-Wire-Änderung.
1117. Festen Acht-ID-Shape-Katalog als reale Lücke identifiziert. Neue sichere dynamische Katalogfunktion für installierte Original-Firestorm-LAD-XML und echte LLM-MorphTarget-Namen eingeführt.
1118. Zulässige Originaldatei höchstens 1 MiB, höchstens 512 Shape-Parameter, nur die exakt vier Head/Wimpern/Upper/Lower-LLM-Dateien, tinyxml2, nur wearable=shape mit param_morph und echter, pro Datei enthaltener MorphTarget-Bezeichnung. Keine Remote-Pfade.
1119. Shape-Parameter-IDs/-Grenzen ausschließlich direkt aus originalem XML, endliche Floats, individuelle Originalbereiche, konflikthafte duplicate Namen ablehnen, identische shared Mappings einmal zulassen, missing original LAD/LLM fail-closed. Keine Interpretation unbeschrifteter Low158-Bytes.
1120. Bestehenden sicheren CPU-Morphapplier auf maximal 96 validierte Original-Sparse-MorphTargets je LLM angehoben. Morphte Original-Mesh-KOPIEN behalten indices, UV und joint weights; keine fremden Avatar-Geometriedaten verändern.
1121. Tatsächlichen authentifizierten CurrentOutfitFolder-LLWearable-Shape-Galerie-Fetch umgestellt: lädt jetzt Laufzeit-Katalog statt feste Acht-ID-Liste. Bestehende Owner/Regionlease/Epoch/COF-Version-, Completeness-, Fehler- und Single-Shape-Sperren erhalten; alte Self-only GPU-Preview-Meshes bei Fehler entfernt.
1122. COF-Galerie meldet konkrete aktive Morphanzahlen für Kopf/Wimpern/Upper/Lower und ausdrücklich nur lokale Vorschau, niemals ohne Grid-E2E finalen Skin-/COF-/Outfit-Erfolg.
1123. Native Original-Firestorm-Tests: zusätzlich zu bisherigen acht echten IDs die aus installiertem LAD/LLM verifizierten Fat_Torso id634 und Fat_Legs id635. Head3/Wimpern4/Upper3/Lower3 Morphs, Negativtests gegen unzulässige Werte, Infinite/unknown ID/fehlende Assets. ECHTER Xvfb-OpenGL-Framebuffer zeigt zusätzliche 1553 Body-Pixel gegenüber alter Shape, 0 fremde Avatarpixel und 0 Ghostpixel nach COF-Reset. Fünf neue Python-Source-/Scope-/GPU-Tests und historische Quellverträge aktualisiert.
1124. VERSION/Projektplan/Chatübergabe/AGENTS/info/Fortschritt-JSON und 3 Paritätsmatrizen aktualisiert. Vollständige Mint-CMake/vcpkg-x64/C++/Python/Xvfb-Original-Firestorm-GPU-/GUI-/X11-Icon-Gate vor GitHub-Push. SHA-/Pi5-CI-Status danach überprüft und dokumentiert. Kein Produktivdeploy.

## Technische Grenzen

Originales LAD plus Original-LLM ermöglichen mehr direkt kompatible MorphTargets. Nicht komplett: Firestorm VisualParam-Driver, Bone-/JointMorphs, Wearable-Alpha/Outfit/Attachments, finale serverbestätigte RGB-3/3-Skin-/COF-/Outfit-Darstellung und echte serverbestätigte WASD/Yaw-Bewegung sind weiterhin offen. Die GPU-Tests verwenden echte Original-Firestorm-Dateien und reale OpenGL-Pixel, aber synthetische ausdrücklich vorgegebene COF-Testwerte. Projektstand bleibt 39 % (38,65 % gewichtet).

## QA

Original-LAD/LLM-C++-Regression und ECHTER isolierter Xvfb-OpenGL-Framebuffer PASS: Head3/Lashes4/Upper3/Lower3 originale Mesh-Morphs; zwei weitere echte IDs 634/635; 1553 zusätzliche Self-Body-Pixel, 0 Remote-/Reset-Ghost-Pixel. Fünf neue Python-Source-Verträge PASS. Vollständiger Linux-Mint-vcpkg/CMake x64-Debug-Build über 424 Bauschritte EXIT 0, 183/183 native CTests und 531/531 Python-Tests PASS, isolierter Xvfb-GUI-Start, originales 133-Joint-Firestorm-Skelett, drei Original-GPU-Körpermeshes/7186 Dreiecke, WM_CLASS und echtes 128x128-X11-Taskleistenicon PASS. Noch KEIN authentifiziertes Live-Grid-COF-/Skin-/WASD-/Yaw-E2E. Exakte Feature-SHA/Pi5-ARM64-CI nach GitHub-Push ergänzen. Nächste zehn Blöcke Alpha.1125–1134.
