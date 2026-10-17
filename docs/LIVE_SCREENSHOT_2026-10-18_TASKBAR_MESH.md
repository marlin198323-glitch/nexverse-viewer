# Bildschirmfoto und Live-Log – 18.10.2026

**Viewer:** alpha.334 mit gezieltem Cinnamon-/Mesh-Update, Gesamtfortschritt **39 % (38,65 % gewichtet)**, nächste Zehnergruppe **Alpha.335–344**.

## Durch Screenshot und Live-Log nachgewiesen

1. Simulator: Login, LLUDP, SSA-Handshake, CAPS, Terrain, Objektmeshes und echte Serveranimationen funktionieren. **Kein nachweislich angenommenes Self-AvatarAppearance**; ein Fremd-AvatarAppearance-Paket beweist nicht die eigene Skin. Eigenavatar wird als einfarbiger Firestorm Originalkörper gezeigt, darunter scheinen versetzte Kleidungs-/Attachmentmeshes. Nicht als Outfit- oder Bake-Parität behaupten.
2. Bewegung: AgentUpdate-Steuerbits wechseln, aber synchronisierte Serverposition wiederholt unverändert. Ursache offen; prüfen, ob Simulator Bewegung quittiert und die Objektupdates korrekt verarbeitet werden.
3. Assetstream: 256/256 GPU-Meshes trotz 292 Szenensamples, 7 Platzhalterobjekte. 58/60 Texturen, davon 1 ungültiges JPEG2000/PNG und 1 HTTP 404. 0 GL-Upload-Fehler, kein Absturz, 502 bei EventQueue-Idle toleriert.
4. Sichtbare graue Terrain-/Materialbereiche müssen separat geprüft werden; der Mesh-Count allein erklärt sie nicht.

## Implementierter Linux-Taskleistenfix

Das OGL-Fenster besitzt schon ein natives X11-Logo und die passende GLFW-WM_CLASS. Die Cinnamon grouped-window-list hatte aber nur andere Anwendungen inklusive Firestorm angeheftet, nicht den OGL-Starter.
- Neuer sicherer Launcher-Pinner: scripts/pin-mint-cinnamon-launcher.py, standardmäßig nur Vorschau; --apply erweitert atomar die vorhandene Pin-Liste um opengenesislink-viewer.desktop, ohne vorhandene Elemente zu löschen oder ihre Reihenfolge zu ändern.
- Installer scripts/install-mint-desktop-icon.sh ruft diesen Pinner beim installierten Cinnamon-Desktop auf, installiert die Original-Icon-Größen und den OGL-Starter.
- Bereits auf Mint umgesetzt und verifiziert: OGL_CINNAMON_PIN=PINNED, anschließend ALREADY_PINN, genau ein OGL-Eintrag und Firestorm weiterhin angeheftet.
- Prüfung des tatsächlich angezeigten Symbols steht bis zum nächsten Start **über den OGL-Programmstarter** aus. Cinnamon wurde bewusst nicht neu gestartet.

## Implementierte begrenzte GPU-Mesh-Anpassung

Die feste 256-Asset-Grenze wird durch eine Szenengrößen-basierte Zielanzahl ersetzt: Detailmodus 256 bis maximal 384, Sparmodus bleibt 256. Bei 292 Samples werden 292 Meshes priorisierbar. Separate Schutzgrenzen von 256 MiB Mesh-VRAM und 384 MiB gemeinsamem GPU-Cache, Retry und Upload-Pacing bleiben unverändert. Native Tests für alle Grenzen. Erfolg im realen NexVerse-Grid noch zu bestätigen.

## Verbindliche nächste technische Prioritäten

P0 Selbstavatar-Bakes und Outfit-Assets mit realem Server-Readback; P0 WASD-/Simulatorposition; P1 Bone-Transform der Attachment-/Kleidungsmeshes; P1 fehlende Meshs und flaches graues Terrain; P2 JPEG2000/HTTP404 der 2 Assets. Keine unbestätigten Reparaturen behaupten. Nächste Alpha.335–344, **39 %** unverändert.
