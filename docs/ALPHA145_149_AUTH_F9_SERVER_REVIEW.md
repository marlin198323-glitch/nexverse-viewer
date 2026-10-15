# Alpha.145–149 – Avatar-Identität, echte F9-Bake-Galerie und Upstream-Review

Stand 15.10.2026, OGL Viewer 0.2.0-alpha.149. Pi5 ARM64 als nexdev,
develop-current ab b7f772c5026754aad56d5d5e7e636235afe95215.
Firestorm-Original-Avatar-Basis und OpenGenesisLINK Server-Protokoll
bleiben maßgeblich; kein Live-/Produktions-Deploy.

## 145: Authentifizierte SelfAvatar-Quelle
self_bake_owner_is_current prüft AvatarAppearance AgentUUID,
aktuelle Login-AgentUUID, WorldModel AgentUUID und Simulator-Region.
Nur bei identischer nichtleerer UUID und verbundener Region gelangen
Head/Upper/Lower-Refs in den eigenen RenderProxy. F9-Live-Prüfung
und MIP0-Priorisierung kontrollieren dieselbe Authentizität.
Geisterhafte eigene Haut durch fremde/stale Serverpakete verhindert,
ohne den separaten RemoteAvatar-Pfad anzufassen.

## 146: Sichtbarer klickbarer F9-Bake-Bereich
Der bestehende F9-Floater besitzt jetzt einen bedienbaren Tab
Server-Bakes / Outfit-Inhalt. Drei Head/Upper/Lower Server GPU
Texturen werden aus demselben OpenGL-Cache wie der Renderer im
rechten Detailbereich groß angezeigt (219x75 Pixel je Vorschau).
Fehlt ein Image-Handle, steht dort Nicht geladen. Keine neuen
Simulator-CAPS/LLUDP-Anfragen, kein BakeUpload oder Wear.

## 147: Responsive Fenster und Lifecycle
Das alte winzige Bake-Bildband, das unter Statusmeldungen lag, entfällt.
Die Vorschau überdeckt nur bei Aktivierung den rechten Detailbereich;
Outfitliste und geschützte COF-Meldungen bleiben erreichbar.
Unterhalb 600 skalierter Punkte Breite fehlt der nicht passende Tab.
Bei geschlossenem F9, Regionwechsel und Logout wird der Tab zurückgesetzt.
Native Hit-Tests in Widescreen und kompakter 420px-Ansicht bestanden.

## 148: OpenGenesisLINK-Änderungsanalyse
Server HEAD d31c03168f0f04f4ce02e82ee3246e16de744748 weiterhin
unverändert. Persistiertes Live-Inventar: 22 Commits/1276 geänderte
Dateieinträge. Der Einzelcommit 41bfb0eb59be7195fb6fe86f0f4086477de1d955
hat genau 1088 Pfade, davon 1000 Cache-Dateien in bin/opt/cache,
87 Terrain-/Importdaten in bin/nexverse-deutschland und .gitignore.
Scanner erkennt diese bekannten nicht-Viewer-Code-Pfadgruppen jetzt
separat. Alle übrigen Code-/LLUDP-/CAPS-Änderungen weiterhin manuell
zu prüfen. Ein neuer komplettes Live-Inventar endete mit GitHub
HTTP 403 API Rate Limit; vorhandenes Inventar bleibt bestehen
und die dort gespeicherte historische Klassifikation von 1208
MANUAL_REVIEW Einträgen wurde NICHT als neue Livezahl überschrieben.
Freigegebener Server-Pin 4e99976630f1008db9014ea2b86c3723b09e9fe6
bleibt unverändert; strict review BLOCKIERT.

## 149: Integrationsreparaturen und Tests
In application.cpp wurde eine zuvor vorhandene fehlerhafte
zusammengezogene C++ Include-Direktive (literales Backslash-n)
zu zwei echten Include-Zeilen korrigiert. Neuer nativer F9
Layout-Test wurde korrekt an die tatsächlich vorhandene CMake
Aliasbibliothek NexVerse::UI gebunden statt NexVerse::Ui.
Statische Quellcode-Regressionen verhindern das Wiederauftreten.
Pi5: 8 native g++14 C++23-Tests mit -Werror PASS;
31 Python Unit-/Wiring-Tests PASS; git diff --check und
Bash-Syntax PASS. GitHub Self-hosted CI nach Push separat prüfen.
Kompletter CMake-/OpenGL-Viewer und GUI-E2E auf dem Pi mangels
CMake/GLFW/GLAD und Firestorm-Assets NICHT ausgeführt.

## Nächste 4–5 Blöcke
Vollständiger CMake-Viewer-Build auf geeigneter isolierter Plattform;
laufender GL-GPU Smoke mit echten Firestorm Body-UVs; Firestorm
TexLayer/Masken- und Kleidungskomposition ohne falsche Paritätsfreigabe;
COF-Wear/Unwear mit expliziter Autorisierung, serverseitigem Readback
und Rollback; vollständiger Patchreview des OpenGenesisLINK-Servers
sobald dessen GitHub-API wieder verfügbar ist.
