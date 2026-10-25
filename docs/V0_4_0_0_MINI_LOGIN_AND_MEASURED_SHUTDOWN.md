# v0.4.0.0 – Mini-Login und instrumentierter schneller Shutdown

Datum: 25.10.2026. Host: Linux Mint `jam-MS-7D28`, Branch `develop-current`.

## Referenz und Umsetzung

Basis sind die vom Nutzer bereitgestellten Screenshots (Neon-Designreferenz und tatsächlicher 1920-Pixel-Mint-Viewer). Die bestehende reale WebKit-Welcome-Seite wird nicht ersetzt; die GLFW-Mausbereiche verwenden dieselben Rechtecke wie der OpenGL-Renderer.

- Obere Leiste von 72 auf **58 px** bei 1648×928; Logo, Brandtext, Grid und Statuschips proportional verkleinert.
- Untere Loginleiste von 77 auf **60 px** bei 1648×928; dunklere, feinere Randgestaltung.
- Avatarname und Passwort von 245×39 auf **205×32 px**, World-Button von 210×39 auf **180×32 px**.
- Passwortsichtbarkeit bleibt eine echte funktionierende Aktion; kein vorgetäuschtes Anmelde-Persistenzversprechen.
- Fokus zeigt **keinen äußeren Cyan-Rahmen**: ausschließlich minimale Hintergrundtönung.
- Die Welcome-Textur nutzt weiterhin die Fläche `l.footer_y-l.header_h`, wobei sich die nutzbare Bildhöhe erhöht. Keine Änderung an der Welcome-Seite selbst.
- Hitbox-/Layout-Nativtest und Python-Verträge für 800×600, 1100×700, 1280×720, 1648×928, 1920×1080, 3440×1440 angepasst.

## Schließen mit angemeldeter Sitzung

- Bereits vorhandene atomare `CurlHttpTransport::request_stop()`-Abbrüche von HTTP/CAPS/Assets werden vor GPU-Aufräumen ausgelöst und bleiben erhalten.
- Die einsekündige LLUDP-Reconnect-Wartezeit ist in unterbrechbare 50-ms-Schritte aufgeteilt; dadurch reagiert der Circuit auch während Retry-Schlaf unmittelbar auf den Stopwunsch.
- Vor der Zerstörung lokalen Sitzungszustands werden sämtliche verbliebenen `std::async`-Futures ausdrücklich synchronisiert. Die Messpunkte melden Worker > 100 ms sowie Wartewarnungen > 150 ms und bewahren den Lebenszyklus ihrer Capture-Referenzen.
- Neuer Gesamtzeit-Messpunkt `Viewer shutdown total elapsed_ms=` **nach** `glfwDestroyWindow` und `glfwTerminate`; der bestehende `Viewer shutdown cleanup elapsed_ms=` bleibt erhalten.
- Kein riskantes `detach`, kein `std::terminate`, kein erzwungenes Abschießen verbleibender Thread-Aufgaben.

## Prüfstand

Linux Mint x64: Build PASS; **187/187** native CTests, **667/667** Python-Tests; isolierter echter GUI-/OpenGL-/original Firestorm-Rig/X11-128px-Systemicon-Gate PASS. HTTP-Stall-Abbruch und Credential-Redirect-Sperre PASS; vollständiges lokales Skript `scripts/mint-viewer-validation.sh --tests-only`: `OGL_MINT_X64_FULL_VALIDATION_PASS`.

**Live-Grid-Abnahme ausstehend:** Der tatsächliche gesamte Schließvorgang nach authentifiziertem NexVerse-Login ist noch nicht gemessen. Bei einem Test nach Login über den normalen X-Button schließen und nur die sanitisierten Zeilen `Viewer shutdown ... elapsed_ms` / `Viewer shutdown waiting on ...` protokollieren; keine Kennwörter, Session-IDs oder CAPS-Tokens teilen. Weitere CPU-intensive Arbeiten können blockieren; die Marker identifizieren künftig den Worker.

Versionsziel `v0.4.0.0` weiterhin IN_ARBEIT. Gesamtstand **38,65 % (39 %)**. Ohne reale Avatar-/Outfit-/WASD-/Region-/Teleport-E2E-Prüfung weder Release nach `main` noch Deployment.
