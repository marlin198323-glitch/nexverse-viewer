# Alpha.325–334 – Original-Firestorm-Avatar tatsächlich im GL-Framebuffer

Stand 17.10.2026 | Viewer 0.2.0-alpha.334 | Branch develop-current | **39 % Gesamtfortschritt (38,65 % gewichtet)**

## Verifizierte Zehnerentwicklung

| Block | Implementierung und Ergebnis |
|---|---|
| 325 | GitHub-Handoff/Branch und aktuellen OpenGenesisLINK-Server main 7c6b0cf7 verglichen; keine neue Protokollanforderung. |
| 326 | Native OpenGL-3.3-Framebuffer-Testumgebung mit isoliertem unsichtbarem GLFW-Fenster eingebaut, CTest registriert, fehlende Original-Assets explizit als SKIP 77. |
| 327 | Die originalen Firestorm-LLM-Dateien Head/Upper/Lower und die originale 133-Joint-Rig-Palette geladen und echte GL-Vergleichsbilder ohne und mit Avatar gezogen; **3398 RGB-Pixel tatsächlich verändert**. |
| 328 | Echtes RGBA-Bake-Asset als GPU-Textur zugeordnet; GL-Framebuffer mit untexturiertem Avatar verglichen: **3398 veränderte Pixel** und 3 echte Original-Body Bake-Draws. |
| 329 | Head als einzige texturierte anatomische GL-Region: **234 sichtbare RGB-Pixeländerungen**. |
| 330 | Upper als einzige texturierte GL-Region: **1422 sichtbare RGB-Pixeländerungen**. |
| 331 | Lower als einzige texturierte GL-Region: **1742 sichtbare RGB-Pixeländerungen**. |
| 332 | GPU-Bake-Textur entfernt und erneut gerendert; **0 stale RGB-Pixel**, keine verbliebenen Textur-Draws. |
| 333 | Im F3-Avatarfenster vier tatsächlich anklickbare Aktionen statt bisher drei: Bericht, Bakes wechseln, Bakes neu prüfen, **Avatar fokussieren [F4]**; Mouse-/Keyboard-Aktionspfad und Layout-/GL-Tests. |
| 334 | Gesamter Mint-x64-/OpenGL-/Python-/Headless-/Xvfb-/Systemicon-Test, GitHub-Push mit Pi5-ARM64-SHA-CI und alle Versions-, Projektplan- und Übergabedateien. |

## Aussagegrenzen

Die veränderten Pixel kommen von einem lokalen **synthetischen Render-Test** mit den echten Firestorm-Meshes, echter GL-FBO/Framebuffer-Ausgabe und selbst erzeugten Bake-Fixture-Pixeln. Das belegt, dass die GPU die drei Originalteile mit Texturen tatsächlich sichtbar darstellen kann, **nicht**, dass der NexVerse-Server die benötigten AvatarAppearance/Bake-Assets in einer authentifizierten Sitzung geliefert hat. Es wurden keine Grid-Zugangsdaten benötigt, keine Serveränderungen und keine Appearance-Uploads durchgeführt. Der F3-Fokus verändert nur die lokale Kamera.

**Gesamtreife unverändert 39 % (38,65 % gewichtet).** Nächster Zehnerdurchlauf **Alpha.335–344**, Priorität: echter NexVerse Login und nachgewiesene Appearance-/Texture-Serverpakete, Outfit-Anzeige und reale Inworld-GPU-Pixel.
