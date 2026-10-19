# Alpha.475–484 – echter OpenGenesisLINK Low158-Server-Skinfix

19.10.2026 | develop-current | 0.2.0-alpha.484 | Projektfortschritt 39 % (38,65 % gewichtet).

## Quellcodebeleg des tatsächlichen Fehlers

OpenGenesisLINK-Server, upstream main SHA 293d1d674bc023f4a1d069f0844c51c42e410cbd:
- OpenSim/Region/Framework/Scenes/ScenePresence.cs um Zeile 2317 ruft SendAppearanceToAgent(this) für den eingeloggten Root-Agenten auf.
- OpenSim/Region/ClientStack/Linden/UDP/LLClientView.cs um Zeile 4402 sendet zuverlässig und zerocodiert Low158, mit AvatarID, trial=false, TE Variable2, VisualParams Variable1, AppearanceData 0, AppearanceHover 1 Float3.
- Der Server serialisiert absichtlich KEINEN AttachmentBlock und keinen abschließenden Blockzähler. Der alte Viewer verwarf genau diese reale gültige Antwort nach dem vollständigen Hover als malformed attachments, sodass der Server-Skin nicht zum WorldModel gelangte.

## Zehn Entwicklungsblöcke

1. (475) GitHub HEAD 4202116, CHATUEBERGABE, projectplan, Fortschritt und letzter Pi5 ARM64-CI-Erfolg 39288055994 geprüft.
2. (476) Den aktuellen OpenGenesisLINK Server main SHA 293d1d6 geklont und die tatsächlichen Low158-Bytefolgen und SendAppearanceToAgent(this)-Aufrufe untersucht.
3. (477) Decoder akzeptiert nach vollständigen gültigen Hover-Daten das Ende eines legacy Low158-Pakets ohne AttachmentBlock. Modern unvollständige Daten/Hover/Anhanglängen bleiben fehlerhaft.
4. (478) Neues attachment_block_present Flag im tatsächlichen AvatarAppearanceMessage. Ein ausgelassener Legacy-Block unterscheidet sich von einem modern explizit gesendeten Nullblock.
5. (479) F3 sagt bei eigener vom Simulator empfangener Low158 ohne Anhangblock korrekt Legacy-Block fehlt anstatt Serverantwort ausstehend. Bei modernem Paket bleibt die tatsächliche Anhangzahl ersichtlich.
6. (480) Privater Diagnosebericht protokolliert self_attachment_block_received=no für Legacy-Pakete ohne IDs. Erster akzeptierter Self-Low158-Logeintrag unterscheidet modern_attachment_block=present und legacy_attachment_block=omitted.
7. (481) Nativer Originalserver-Reproduktionstest kodiert ein vollständiges zuverlässiges, ZEROCODIERTES Low158-Datagramm mit AppearanceData=0 und AppearanceHover=1, aber keinem AttachmentBlock. Anschließend LLUDP Decode, AvatarAppearance Decode, TE-Validierung und authentifiziertes eigenes WorldModel.
8. (482) Native Negativ- und Gegenproben: abgeschnittene Hover-Bytes, fremde Avatar-ID, corrupt TE wird vom WorldModel blockiert, moderne explizite Anhänge einschließlich 65 und 255 Parser-Maximum bleiben kompatibel.
9. (483) OpenGL-F3-Test unterscheidet authentifiziertes Legacy Low158 mit fehlendem AttachmentBlock von noch gar keinem Self-Low158. Python-Verträge sichern exakte Serverformat- und GUI-Anforderungen.
10. (484) Gesamter Linux x64 Build, Native CTest/GL, Python, ARM64 Headless, isoliertes Xvfb und echtes X11 Systemicon, GitHub Feature-Commit und zugehörige Pi5 CI; Handoff auf Alpha.485–494.

## Grenzen und nächster Schritt

Die neue Quellcode-Analyse belegt einen echten Ablehnungsfehler, aber noch keine Live-Abnahme im NexVerse-Grid nach der Korrektur. Keine garantierte vollständige Körper-Skin, GPU-residente Kopf/Oberkörper/Unterkörper-Bakes, sichtbare Kleidung/Attachments oder serverbestätigte WASD-Bewegung ohne neue authentifizierte Logs und Screenshots. Projektstatus daher weiter 39 %. Kein produktives Deployment. Die frischen Logs sollten erstmals AvatarAppearance SELF server readback accepted und legacy_attachment_block=omitted enthalten, danach müssen echte Bake-Referenzen und Rendererstatus geprüft werden.

Nächste zehn Blöcke Alpha.485–494: Live-Self-Low158, echte Bakes/COF/GetTexture, sichtbarer Avatar/Outfit und parallele Firestorm-UI weiter bearbeiten.
