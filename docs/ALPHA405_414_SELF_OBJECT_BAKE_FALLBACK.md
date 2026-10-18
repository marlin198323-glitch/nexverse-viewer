# Alpha.405–414 – Selbstavatar-Texturen aus echten Simulator-ObjectUpdates

18.10.2026 | develop-current | Version 0.2.0-alpha.414 | Gesamtstand 39 % (38,65 % gewichtet).

## Zehn Entwicklungsblöcke

| Nr. | Änderung und Nachweis |
|---|---|
| 405 | Branch-/Commit-/CI-/Übergabe-/Projektplan geprüft. OpenGenesisLINK Server main bleibt bei 293d1d6; vorheriges Alpha.404 Pi5 CI grün. |
| 406 | Identifiziert: zwei reale Simulatorpaket-Decoder stellen eigene Avatar-TextureEntry bereit, aber der Self-ObjectUpdate-Pfad verwendet bisher nur die Pose. Ohne Low158-Self-AvatarAppearance werden Bake-Faces trotz vorhandener TE ignoriert. |
| 407 | Strikt gekapselte SelfAvatarObjectBakes mit unverwechselbarer Herkunft als vorläufige Self-ObjectUpdate-Kandidaten im WorldModel ergänzt. |
| 408 | Nur tatsächlich authentifizierte eigene Agent-/Session-ID, passender Regionshandle und LocalID; gemeinsame gültige Firestorm-TextureEntry-/Bake-Face-Analyse, keine Default-Face- und Placeholder-Skins. |
| 409 | Vollständige ObjectUpdate-Selbstavatar-TE wird nach Identitäts-/Regionsprüfung gesammelt. |
| 410 | ObjectUpdateCompressed-Selbstavatar-TE wird unter denselben Bedingungen gesammelt; inkomplette/fehlende TE erfindet keine Textur. |
| 411 | Renderer erhält aktuelle echte Head/Upper/Lower-Face-UUIDs, jedoch nur vorläufig und nur solange kein vollständiges autoritatives Low158-Self-AvatarAppearance vorliegt. Nachfolgende Self-TE hat strikt Vorrang, auch bei fehlenden Bakes. |
| 412 | Lebensdauer bei Regionswechsel, Reset, anderer Identität/Session und wechselnder Self-LocalID begrenzt; identische wiederholte Updates verursachen kein weiteres Log-Spam. |
| 413 | F3-Bake-Panel kennzeichnet Obj-TE vorlaeufig (Self-Low158 fehlt). Privater Diagnosebericht erhält nur numerische Kandidatenanzahl und Aktivitätsstatus; keine UUIDs oder Texturadressen. |
| 414 | Native Testfixture für gültige Firestorm-Bake-Masks, Head/Upper-Assets, ausgeschlossene Placeholder, Fremdavatar, falsche Region, LocalID-Wechsel, Sessionwechsel, Low158-Vorrang; Python-Vertrag, Linux-GL-Vollbuild, isoliertes Xvfb/Icon, Pi5 ARM64-CI. |

## Keine überzogenen Live-Behauptungen

Die OpenGL/GPU-Bake-Nachladepipeline erhält kandidatische, vom Simulator gemeldete echte TextureEntry-IDs (nicht generierte oder hochgeladene Texturen), wenn Low158 nicht eintrifft. Der Server kann solche IDs liefern; noch nicht nachgewiesen ist, dass er es in der aktuellen authentifizierten NexVerse-Sitzung tatsächlich tut. Deshalb bleiben Live-Low158, vollständiger eigener Avatar/Skin/Outfit, tatsächliche Server-WASD-Bewegung und vollständige Firestorm-GUI weiterhin offen. Der Fortschritt erhöht sich nicht ohne End-to-End-Nachweis.

Nächste Alpha.415–424: Self-Appearance-Bake-Candidate Source im Live-Log verifizieren, falls verfügbar; echte GPU-Asset-Status in F3 auswerten; Server-Appearance/COF- und Wearable-Integrationsfehler beheben, GUI/Floater-Aktionen funktional ergänzen. Keine Produktionsinstallation ohne ausdrückliche Freigabe.
