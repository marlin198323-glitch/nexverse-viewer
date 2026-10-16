# Alpha.200–204 – Original OpenGenesisLINK Icon, Linux-Taskleiste und Login-Refresh

16.10.2026. OGL Viewer v0.2.0-alpha.204.
Entwicklungsrechner: Linux Mint jam-MS-7D28, User jam, Remote Desktop Commander.
Repository marlin198323-glitch/nexverse-viewer, Branch develop-current;
Basis a014347676fa338695253e96a7686da78fb9c447.
Raspberry Pi 5 ist NUR GitHub Self-hosted ARM64 CI.

## Benutzeranweisung
Vorliegendes vom Nutzer bereitgestelltes Original-Icon integrieren,
NICHT neu erfinden, KEIN neues Bild generieren, Welcome-Seite weiter
unter https://stadt-nexverse.de/service/welcome einbetten. Nur Icon
links oben im bestehenden Login und als echtes System-/Taskleistenicon
für die Desktop-Anwendung. Vorhandene Bedienlogik erhalten,
optische Anpassung an Blau/Cyan-OG-Branding.

## Alpha.200 – Verifiziertes Original und System-Assets
Original: /home/jam/Downloads/icon.jpeg (bereits auf Mint vorhanden);
bitidentisch mit Chat-Anhang icon(1).jpeg:
SHA-256 c4639d33efd86cf7c5a9c6bb6c24702b995f376c0953dd67ce81eafcc6c0084e.
Aus dieser Quelle wurden lossless RGBA-PNG-Assets mit transparenten
abgerundeten Ecken in 32, 48, 64, 128, 256 und 512px erstellt.
Die innere Grafik und der OpenGenesisLINK Viewer Schriftzug werden
NICHT retuschiert oder als neues Logo neu interpretiert.
Originaluser JPEG bleibt unverändert; nur skalierte Icon-Dateien
als Anwendungssymbole werden eingecheckt.

## Alpha.201 – Echte Linux/Cinnamon/X11-Windowsymbole
Beim GLFW-Fensterstart wird das tatsächlich aus der PNG dekodierte
128px-Originalicon mit glfwSetWindowIcon gesetzt.
GLFW_X11_CLASS_NAME und INSTANCE_NAME sind
opengenesislink-viewer, passend zur Desktop-Entry StartupWMClass.
Das Fenster übergibt zusätzlich per XChangeProperty die echte
_NET_WM_ICON ARGB32-EWMH-Datenliste; der originale 256px Logo-
Bitmap dient getrennt dem Login-Header. Die 128px-Größe verhindert
überlange X11-Requests für mehr als 65535 CARDINAL Elemente.
Im echten, unsichtbaren Xvfb + Metacity-Fenster führt ein
XGetWindowProperty-Readback zu format=32, CARDINAL,
erster Wert width=128, zweiter height=128 und folgenden
16.384 Pixeln. Der GUI-Test prüft tatsächliches Logdatum
und realen WM_CLASS-Wert. Keine sichtbaren Testfenster auf DISPLAY=:0.

## Alpha.202 – Logo wirklich im Login oben links
src/app/application.cpp ersetzt den alten 56x56 Cyan-Platzhalter
im Login-Header durch das echte OpenGenesisLINK Logo als
OpenGL RGBA Textur (renderer.image). Sie wird einmalig nach UI-
Initialisierung hochgeladen und vor UI/GL-Shutdown freigegeben.
Die neue native PNG::PNG-Routine src/ui/brand_icon.cpp dekodiert
nur lokale Dateien (max 1024x1024; keine Netzwerkanfragen),
mit gesicherter Pfadauflösung relativ zum Viewer-Binary,
Installationsshare, macOS-Bundle oder Entwicklungscheckout.
Der vorhandene WebKit-/OpenSim-WelcomePage-Frameupdate-Pfad,
Login, Grid-Discovery und Eingabebuttons wurden nicht ersetzt
und unverändert erhalten.

## Alpha.203 – Zurückhaltender Design-Refresh und Installation
Login-Header mit dunklerem Blau, klarerer Cyan-Unterkante,
Originalicon mit dezenter Outline; Anmelde-Overlay mit leicht
hellerem Nachtblau/Cyan-Rahmen. Kein erfundener Grid-Status,
keine Mockup-Startseite. Der Linux Desktop-Entry
packaging/linux/opengenesislink-viewer.desktop verwendet
Name OpenGenesisLINK Viewer, Icon opengenesislink-viewer und
StartupWMClass opengenesislink-viewer. CMake install() liefert
App-Starter und hicolor-Icongrößen. User-Linux-Mint-Installer
scripts/install-mint-desktop-icon.sh installiert nur
~/.local/share/applications und ~/.local/share/icons/hicolor,
ohne andere Desktop-Einträge, Pinned-Items oder Systemdateien
zu löschen. Auf Mint installierter Entwickler-Starter verweist
auf echten build/debug/nexverse-viewer-Pfad. Für Windows ist
eine mehrgrößige .ico samt .rc für das Windows-Exe-Icon
integriert; für macOS eine .icns und CMake Bundle-Ressource
(Finder/Dock); Windows und macOS müssen gesondert auf den
jeweiligen Zielsystemen gebaut und getestet werden.

## Alpha.204 – Tests/CI und Projektfortsetzung
Native brand_icon_tests: echter PNG-Dateifund und Alpha-Kanäle.
Python test_ogl_branding: sämtliche PNG-Formate,
Startfenster GLFW-Icon & X11-Klasse, Logo im Login,
unveränderte WebKit-Welcome-API sowie .desktop-Vertrag.
Xvfb+Metacity read-only Test test-mint-window-branding.sh
prüft X11 WM_CLASS und tatsächlich installierte
128x128-ARGB-Fenster-Iconproperty, nicht nur GPU/GLFW-Calls.
Vollständige Linux-x64-/GL-/Python-/Pi5-Headless-Suite und
Xvfb-Startup-Smoke vor GitHub Commit. Verbindlicher
Server-Referenzabgleich OpenGenesisLINK bleibt bei
ec760cacfff2fe505cf51991985553ed434eb121,
reviewed_sha unverändert, ältere 22 Server-Commits
nicht automatisch freigeben.

## Weiterhin offene Funktionalität
Welt und eigener Avatar nach authentifiziertem NexVerse-
Grid-Login wurden weiterhin nicht Ende-zu-Ende bestätigt,
COF Wear/Unwear/Firestorm TexLayer11 und vollständige
OpenSim-/OpenGenesisLINK-Protokollkompatibilität bleiben
wie bisher im Projektplan. Das Branding ändert daran nichts.
Die Original-Welcome-Seite bleibt explizit erhalten.
