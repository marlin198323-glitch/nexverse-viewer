# Alpha.395–404 – Self AvatarAppearance-Texturintegrität und Regionsdiagnose

18.10.2026 | develop-current | Version 0.2.0-alpha.404 | Projektfortschritt 39 % (38,65 % gewichtet).

## Zehn Entwicklungsblöcke

| Block | Umsetzung |
|---|---|
| 395 | GitHub HEAD, CHATUEBERGABE, Projektplan, OpenGenesisLINK Server-main geprüft; Pi5-Vor-CI erfolgreich. |
| 396 | Echten Integritätsfehler lokalisiert: nichtleere, aber kaputte Self-TextureEntry konnte vorher gültige Skin-Daten ersetzen. |
| 397 | Firestorm-TE-Parser als zentrale Identitäts-neutrale Inhaltvalidierung in die Protokollebene integriert. |
| 398 | WorldModel blockiert jede direkte Übernahme ungültiger Self-TE, auch bei Umgehung des LLUDP-Handlers. |
| 399 | LLUDP-Low158-Handler nimmt nur gültige Self-TE an; korruptes Paket kann alten Skin nicht mehr verdrängen. |
| 400 | Regionswechsel setzt auch Trial-/Incomplete-/invalid-TE-Zähler und Decoderfehlerphase zurück. |
| 401 | F3-Self-Avatarstatus meldet abgelehnte fehlerhafte Texturblöcke separat und erkennt erhaltene Skin-Daten. |
| 402 | Privater numerischer Diagnosebericht: verworfene invalid-TE, ohne Identitäten/Texture-Payload. |
| 403 | C++/Python-Regressionen für vollständige TE, absichtlich zerstörte TE, Preview, inkomplette Parameter und Regionsstatus. |
| 404 | Vollbuild, Native/OpenGL/Python, ARM64-kompatible Headless, isolierter Xvfb-Start und Taskleistenicon, GitHub und Pi5-CI. |

## Abgrenzung

Testdaten sind synthetisch. Die Abwehr fehlerhafter LLTextureEntry ist überprüft, aber das Auftreten eines echten authentifizierten NexVerse-Low158-Self-AvatarAppearance, korrekte Bakes/Kleidung und bestätigte WASD-Bewegung sind weiter NICHT NACHGEWIESEN. Keine erfundenen Skin-UUIDs, kein Einloggen in ein fremdes Grid, kein Produktionsdeployment.

**Nächste zehn Blöcke Alpha.405–414:** echte Live-Grid-Paketdaten sicher diagnostizieren; SSA/EventQueue- und Wearable-/Bake-Readback verbinden und echte Renderer-GPU-Skin-Evidenz liefern. Gleichzeitig Firestorm-GUI mit allen zugehörigen Bedienelementen funktional weiterbauen.
