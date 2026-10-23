# Alpha.1105–1114 – originale Firestorm-Ober-/Unterkörperform aus COF

24.10.2026 | Linux Mint Desktop Commander jam-MS-7D28 | develop-current | 0.2.0-alpha.1114

## Zehn umgesetzte Blöcke

1105. Linux Mint via Desktop Commander gewählt, nicht Pi5. GitHub develop-current lokal und origin 82a77de07f3f2ff87d15b7641509ce5c6bca5602 synchron, Arbeitsbaum sauber. CHATUEBERGABE.md und projectplan.md zuerst gelesen.
1106. OpenGenesisLINK-Server Upstream abgeglichen, Main 99341d475b1089a5762e529c5a3c0a06171074e0. Original-Firestorm avatar_lad.xml und zwei tatsächliche LLM-Dateien auf verfügbare MeshMorphs und IDs geprüft.
1107. Vier konkrete Formparameter aus originalem LAD verifiziert: 104 Big_Belly_Torso [0..1], 106 Muscular_Torso [0..1.4], 151 Big_Butt_Legs [0..1], 152 Muscular_Legs [0..1.5]; original upper_body.llm und lower_body.llm enthalten exakt diese vier Morph-Ziele.
1108. Bestehendes bounded und NaN-fail-closed COF Shape-Mapping um eigene Upper- und Lower-Morph-Pools erweitert; keine erratenen Low158-Positionsindices, kein zusätzlicher Simulatordownload.
1109. Original-Firestorm-Linden-LLM-Sparse-Deltas auf KOPIEN der originalen gewichteten Ober- und Unterkörper-Meshes angewendet, ohne Originalgeometrie, Indizes, UVs oder Skin-Boneweights zu verändern.
1110. Optionalen Self-only GL-GPU-Meshslot8 Upper und 9 Lower eingeführt. Remote nutzt unverändert originale 1/2, Self Head6/Lashes7 bleiben erhalten. Anatomische 3-Mesh-Draw-/Bake-Gates weiterhin nur Head/Upper/Lower, neue lokale Morph-GL-Zähler separat.
1111. Authentifiziertes explizites vollständiges COF-Shape-Original-LLWearable liefert nun für alle vier Bereiche Head/Lashes/Upper/Lower geprüfte Parameter. Erfolg nur bei allen benötigten GPU-Mesh-Uploads; bei fehlendem/mehrfachem/unzulässigem neuem Shape, Regions-/Agent-/COF-Versionwechsel oder Teilfehler alle vier lokalen Vorschauen entfernen.
1112. F3 zeigt tatsächliche getrennte Upper-/Lower-Morph-Draws (lokal, kein Grid-Bake-Proof). Kein COF-/Skin-/WASD-E2E-Erfolg aus lokalen GL-Zählern abgeleitet.
1113. Native Original-Firestorm-LLM-Tests bestätigen 2 Upper-/2 Lower-Morphs, Positionsänderungen, Bounds/NaN-Ablehnung. Echter isolierter Xvfb-OpenGL-Framebuffer: 833 sichtbare Self-Body-Pixel anders, 1/1 Morph-GL-Draws, 0 fremde Avatarpixel verändert, 0 Restpixel nach Reset. Fünf zusätzliche Python-Quell-/GPU-/COF-Verträge.
1114. VERSION, alle zentralen MD, Projektstand-JSON und 3 Paritätsmatrizen aktualisiert. Vor GitHub-Push: vollständiger Linux-Mint-CMake/vcpkg-x64-Viewer-Build, native CTests, Python, isolierte Xvfb-Original-Firestorm-GPU/GUI/Systemicon-Abnahme. Kein Produktivdeploy.

## Einschränkungen

Echte originale Firestorm-Meshes, echte GPU und echte COF-Parameterpfade im Code, aber die isolierten Bildtests verwenden synthetische Testparameter. Kein authentifizierter Grid-Skin-/COF-/Outfit-Live-Pixelnachweis und kein serverbestätigter WASD-/Yaw-E2E-Nachweis. Weitere Bone-/Driver-/Morph-/Clothing-/Attachment-Funktionen sind noch offen. Fortschritt unverändert 39 % (38,65 gewichtet). Nächste Blöcke Alpha.1115–1124.

## QA

Echter OpenGL-Original-Firestorm-Morph-Test 833 Pixel / Upper/Lower je 1 Draw, 0 Remote-Pixel und 0 Reset-Pixel PASS; voller CMake/vcpkg-x64-Build aller 423 Ziele PASS; 183/183 native CTests, 526/526 Python-Tests, isolierter Xvfb-GUI-Start, originaler Firestorm-133-Joint-Skeleton-/GPU-Pfad und 128x128-X11-Taskleistenicon PASS. Exakte Feature-SHA und Pi5-ARM64-CI nach Push ergänzen.
