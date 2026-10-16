# Alpha.215–219 – Kein veralteter Avatar-GL-Beleg, F3-Diagnose und sicherer lokaler Export

Stand 16.10.2026. OGL Viewer 0.2.0-alpha.219.
Quelle Mint PC jam-MS-7D28 User jam, Remote Desktop Commander.
GitHub Branch develop-current, Ausgang SHA
1c93f3fa3478e86ddc8a3471fa5b6f94d18694ad.
Pi5 nur ARM64 Self-hosted CI, kein Viewer-Entwicklungscheckout.

## Alpha.215 – Sichtbare GPU-Avatarfragmentsignale dürfen nicht veralten
Vorher wurde eine GL_ANY_SAMPLES_PASSED Query einmal asynchron gelöst,
danach konnte "sichtbar" auch stehen bleiben, wenn die Kamera inzwischen
gezoomt/gedreht oder der Originalmesh-Renderpfad weggefallen war.
LegacyOpenGlRenderer::set_camera prüft geänderte Position, Forward,
Left, Up und vertikalen FoV und invalidiert die alten GPU-Fragmente
mit demselben Region-Epoch-System wie beim Simulatorwechsel.
Gleiche Pose wird NICHT bei jedem Frame unnötig zurückgesetzt.
Renderer::end_frame entwertet die frühere Messung sofort,
wenn gar kein originales Firestorm .llm Self-Body-Mesh gezeichnet wird.
Unfertige GPU-Abfragen dürfen asynchron auslaufen und ihre
veralteten Resultate nach Kamerawechsel nicht mehr als aktuell
angezeigt werden; keine glFinish-Blockade im Live Viewer.
Die echte NVIDIA OpenGL-Regression prüft Kamerawechsel
und entfernte Avatare/Originalmeshes bei bereits bekannten Queries.

## Alpha.216 – Vertraulicher lokaler Avatarbericht
Neue typisierte AvatarDiagnosticReport-Struktur exportiert
AUSSCHLIESSLICH Whitelist-Fakten:
Login ja/nein, Simulation/Region/Agent vorhanden, empfangenes
AvatarAppearance, Kamera gültig, Agent im Blickfeld,
originale LLM-/Fallback-/Bake-GL-Draws, ob GPU-Query aktuell,
ob GL-Fragmente Tiefentest bestanden, Weltobjekt-/fehlende-Mesh-
GL-Zähler sowie Framebufferauflösung.
Keine Usernamen, keine Session-, Agent- oder Grid-UUIDs,
Passwörter, Capability-URLs, Chattexte, Simulatoradressen oder
sonstige Profil-/Netzwerkmetadaten in der Textdatei.
Speichern erfolgt NUR nach ausdrücklichem Nutzerklick/Key,
in ~/.local/state/opengenesislink-viewer/diagnostics/
Datei avatar-diagnose-<UTC-Mikrosekunden>.txt.
Auf Linux erstellt O_CREAT|O_EXCL|O_NOFOLLOW (wenn verfügbar)
eine private Datei Mode 0600. Bei unvollständigem Write wird
die Datei entfernt; keine unaufgeforderte Telemetrie.

## Alpha.217 – Echte sichtbare F3-Avatarstatus-Bedienung
Die vorhandene Firestorm-orientierte ViewerShell Avatar-Floater-
Oberfläche bleibt erhalten und bekommt einen wirklichen F3-
Toggle inklusive Avatar-Menülabel "Avatarstatus [F3]".
Die echte Dialogansicht zeigt jetzt laufend Kamera/GL-Query/
Head-Upper-Lower-ServerBake-Evidenz zusätzlich zu Avatarposition
und Animationen. "Diagnose speichern [S]" ist eine echte
anklickbare Taste; S im geöffneten Avatarstatus führt identischen
UI-gebundenen Save aus. Modaler HitTest schluckt alle Klicks
auf World-Objekte hinter dem Dialog; keine LLUDP/COF-Pakete.
Die Dateispeicherung zeigt einen Pfadhinweis oder eine
Fehlermeldung im Floater, Benutzer muss Bericht aktiv auslösen.
WelcomePage aus der NexVerse-Webseite, F4-/F5-Kamera,
F6-/F7-Chat/Region, F8-/F9-/F10-Fenster und Systemicon bleiben.

## Alpha.218 – Sicherheit und native Regression
Neuer C++23-Nativetest schreibt Bericht in isoliertes temporäres
Testverzeichnis und prüft die tatsächlichen Dateirechte 0600,
Whitelisting und Feldformat, ohne HOME zu verändern.
ViewerShell-Layouttest prüft echte Speichern-/Schließen-Hitboxen
bei 1100x700, 1920x1080 und 3440x1440;
Python-Contract prüft F3/S-Eventhandler, Image/Avatarstatus,
GL-Query-Invalidierung und dass keine Passwort-/UUID-
JSON-Schreibaktionen in der Exportquelle auftreten.
Geänderter GPU-Integrationstest bestätigt, dass Kamerawechsel
und Wegfall von Originalmesh-Draws das Statussignal zu UNKNOWN
zurücksetzen. Diese Tests sind lokal; erfordern kein Grid-Login.

## Alpha.219 – Integrationsprüfung, fünf Blöcke dokumentieren und CI
Vollständiges Mint CMake/Ninja Linux-x64 Debug-Binary und
Native/NVIDIA OpenGL CTests, sämtliche Python-Tests,
Pi5-kompatibler C++23 Headlesssatz auf Mint und ein
tatsächlicher vollständiger Viewer-Start ausschließlich
auf einem separaten virtuellen Xvfb Display.
Kein sichtbarer Teststart auf DISPLAY=:0. Original-OG-
Systemicon wird zusätzlich per X11/WM_CLASS geprüft.
Änderungen und alle Doku in einem Commit auf
marlin198323-glitch/nexverse-viewer develop-current,
danach Pi5-ARM64 Runner verifizieren. Projectplan
und CHATUEBERGABE.md sind bei jedem WEITER verbindlich.

## OpenGenesisLINK Servercheck und unveränderte Referenz
Server main am 16.10.2026:
ec760cacfff2fe505cf51991985553ed434eb121
fix(j2k): retry texture layer decoding with OpenJPEG after CSJ2K errors.
Die 23 Commit-Änderungen seit dem reviewed-Baseline-SHA
4e99976630f1008db9014ea2b86c3723b09e9fe6
sind nicht durch neuen Server-Kompatibilitätstest abgenommen.
Die Alpha.215–219-Anpassungen verwenden reine lokale
GPU- und Diagnosepfade; keine serverseitige Änderung.

## Ehrliche Restarbeit
Echter NexVerse-Live-Login, sichtbar gerenderter eigener
Avatar und Welt, Firestorm Bake-/TexLayer-11-Parität,
vollständige Avatar-Anims und COF Wear/Unwear werden NICHT
durch Headless/OpenGL-Fixture-Tests bewiesen. F3-Report
hilft, die verantwortliche Pipeline-Stufe bei einem
künftigen autorisierten Login ohne Zugangsdatenexport
konkret nachzuweisen. Eigentliche Sichtbarkeit bleibt offen.
