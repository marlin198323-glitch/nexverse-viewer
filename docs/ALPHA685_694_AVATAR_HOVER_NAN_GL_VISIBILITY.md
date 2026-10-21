# Alpha.685–694 – Avatar-Hover NaN/Infinity vor GL beseitigt und CI-Evidenz

21.10.2026 | develop-current | OpenGenesisLINK Viewer 0.2.0-alpha.694
Gesamtfortschritt: 39 % (38,65 % gewichtet).

## Zehn konkrete Entwicklungsblöcke

685. Pi5-Checkout develop-current, GitHub-HEAD, CHATUEBERGABE.md und projectplan.md geprüft; Alpha.684 GitHub-Pi5-CI Run 39647670145 für Code-SHA 0c38c102a9f908562dd2f09141eb2cba80c14623 ist SUCCESS.
686. Neueste OpenGenesisLINK-Servermain c9a032206202ccdbe44d7d4788e34c4955f7d273 mit Vorgänger 3728273 verglichen. Ein neuer Commit #262: LLLoginHandlers akzeptiert explizite TOS-Zustimmung als bool true, String true und numerische 1, aber nicht false/0. Andere Änderungen betreffen Portal-HTTPS-Proxy und Dokumentation. Viewer liefert bereits ausdrücklich bestätigtes XML-RPC-Boolean. Keine neue API erfunden.
687. Einen tatsächlichen Avatar-Renderpfad-Bug analysiert: AvatarAppearance.hover_heights.front().z und das Hover fremder Avatare wurden bislang durch std::clamp direkt zu den realen AvatarProxy GL-Transformationen addiert. std::clamp von NaN ergibt NaN; dadurch konnte ein gültiger eingeloggter Avatar trotz AgentPosition unsichtbar werden.
688. Zentralen MPL-2.0-Guard render::safe_avatar_hover eingeführt: NaN, +Infinity und -Infinity werden nur für die visuelle Darstellung auf 0m korrigiert. Normale Werte bleiben erhalten, extreme endliche Werte weiterhin auf ±2m begrenzt. Simulatorphysik, Animation, AgentPosition und LLUDP-Pakete nicht manipuliert.
689. Guard in den realen eigenen AvatarProxy-Aufbau integriert, vor dem Addieren zur Z-Koordinate. Bestehende Low158-/Obj-TE-Bake-Quellen bleiben unverändert. Damit wird kein fehlendes Skin-Asset oder ungeprüfter Low158 als grüner Skin-Beleg ausgegeben.
690. Guard zusätzlich beim echten fremden AvatarProxy-Aufbau verdrahtet. Ungültige Remote-Hover-Daten werden nicht mehr an den Renderer durchgegeben; der bestehende 64-Avatar-Frame-Budgetpfad bleibt erhalten.
691. AvatarPipelineProof um tatsächliche pro-Frame-Statuswerte für Self-/Remote-Hover-Fehler erweitert. Das bereits existierende Avatar-Pipeline-HUD zeigt bei ungültigen Datensätzen den Warnhinweis Hover korrigiert: Self 1 / Fremde N. Keine privaten Avatar-UUIDs oder fortlaufende Logflut.
692. Native C++23-Regressionen für NaN/±Infinity, normale Hover-Werte, endliche Grenzwerte und Statusanzeige ergänzt. Zusätzlicher echter Third-Person-Kamera-Projektionstest bestätigt, dass ein korrigierter Hover wieder einen gültigen Avatarpunkt im Kamerafrustum ergibt. Ein Unit-Test, KEIN GPU-Pixel-/Live-Nachweis.
693. Beide neuen nativen Tests in CMake und Pi5-Headless-Runner aufgenommen, Python-Verträge für echten GL-Proxy-/HUD-Quellcode ergänzt. GitHub Hosted-Linux-x64-UI CI von Alpha.684 dokumentiert: Run 39647670237 kam nicht über die Runner-Zuteilung hinaus (runner_id 0, 0 Steps). Kein gültiger Code-Compilerfehler belegbar.
694. Version, Projekt-/Viewer-/Server-/Firestorm-Matrizen, zentralen Chat-Handoff, AGENTS und JSON-Projektstatus aktualisiert. Vollständige Tests vor Code-Commit und SHA-genaue GitHub Actions nach Push zu überprüfen; ausdrücklich kein Produktionsdeploy.

## Offene Abnahmen
Die neue Alpha repariert eine konkrete NaN-Quelle in den eigenen und fremden Avatar GL-Proxy-Positionen. Sie beweist noch nicht, dass die originalen Firestorm-Meshes, Skin, COF und Outfits nach authentifizierter NexVerse-Anmeldung fertig dargestellt werden. Ebenso fehlen GUI-Framebuffer-/Screenshot-/GPU- und serverbestätigte WASD-E2E-Beweise. Der Linux-x64 Hosted-Runner ist vor dem Job gescheitert, nicht im Compiler; der vollständige CMake-GUI-Build ist deshalb weiterhin UNVERIFIZIERT.

Alte Serveränderungen seit der Review-Baseline 4e99976630f1008db9014ea2b86c3723b09e9fe6 weiter MANUAL_REVIEW. Nächster Zehnerblock Alpha.695–704, Projekt 39 %. Kein Produktivdeploy.
