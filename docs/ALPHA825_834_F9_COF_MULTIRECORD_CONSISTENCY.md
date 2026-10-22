# Alpha.825–834 – F9 COF-Version konsistent zum echten Low158 WorldModel

22.10.2026 | Linux Mint Desktop Commander jam-MS-7D28
Branch develop-current | Version 0.2.0-alpha.834 | 39 % (38,65 % gewichtet)

## Zehn durchgeführte Entwicklungsblöcke
825. GitHub develop-current SHA 0fef35ec1ee53dca1d16e4d4ef83d9bb6d79f962
und sauberen Linux-Mint-Checkout geprüft; zuerst CHATUEBERGABE.md
und projectplan.md gelesen. Voriger Pi5 Alpha.824 Run 39836372832
für SHA 730f115 completed/success. Linux Mint bleibt primär;
Pi5 nur für CI, kein Produktivdeploy.

826. Aktuellen OpenGenesisLINK Servermain
7396897d3d24ab8fac238ded1e2760d7b4544f13 geprüft.
Keine neueren AvatarAppearance/Low158- oder COF-
Wire-Protokolländerungen erkennbar.

827. ECHTEN UI-/COF-Diagnosefehler gefunden: der
authentifizierte WorldModel-COF-Highwater-Schutz
verwendete seit Alpha.764 die letzte nicht-null COF-Version
aus allen AppearanceData-Einträgen; die beiden F9-
Pfade nutzten dagegen blind appearance_data.back().cof_version.
Bei offiziellen Firestorm Low158 Mehrfachdatensätzen wie
(Version 2, COF 17), (Version 2, COF 0) zeigte F9
fälschlich COF 0 und damit OFFEN, obwohl das Model
COF 17 verwendete.

828. Neues reines MPL-2.0-Protokollmodul
include/nexverse/protocol/avatar_cof_revision.hpp:
latest_nonzero_avatar_cof() wählt die jüngste
nicht-null COF-Revision in Originalreihenfolge,
avatar_cof_version_for_display() gibt bei
vollständig unbekannter/Legacy-Quelle bewusst 0 zurück.

829. Den realen WorldModel::apply_self_appearance()-Guard
auf denselben Selektor umgestellt. Der bereits getestete
Antirollback-Highwater pro AppearanceVersion,
Session-/Region-Gate, alte Firestorm-Body-Skin-Verhinderung
und die Legacy-count=0-Kompatibilität bleiben erhalten.

830. Beide tatsächlichen F9-Aufrufstellen in
src/app/application.cpp an den gemeinsamen Selektor
angeschlossen: manuell angestoßener Skin-/COF-Vergleich
UND automatisch regelmäßig aktualisierte Live-Ansicht.
Kein COF-Status wird aus dem letzten COF=0-Füllblock
oder künstlich clienterzeugten Outfits abgeleitet.

831. Native C++23 AvatarBakeEvidence-Regression
für mehrteilige AppearanceData 17+0, 42+0,
kein AppearanceData sowie reine Nullversionen
ergänzt. Der Status COF 17/17 OK darf NUR bei
tatsächlich vorhandenem COF 17 entstehen;
Legacy 0 darf kein Scheinerfolg sein.

832. Low158 LLUDP Wire-Decoder-Regressionsfall
in avatar_appearance_large_attachment_wire_tests.cpp
erweitert: authentische Paketbytes durch den
Standarddecoder, zwei AppearanceData-Zeilen,
trailing 0; ausgewählte gültige Revision 110 statt 0.
Nach wie vor kein erfundener Login-/Avatar-Success.

833. Python-Quellintegrationsvertrag sichert beide F9-
Stellen, WorldModel-Quelle, echten Low158-Wiretest
und den F9-Protokollstatus. Vollständiger Linux-Mint
x64 CMake/vcpkg Debug Build, sämtliche nativen CTests,
Python, isolierter Xvfb-Viewer und OGL-X11-Taskleistenicon.

834. VERSION, CHATUEBERGABE.md, projectplan.md,
AGENTS, info, docs/PROJECT_PROGRESS.json sowie
Server-/Firestorm-/UI-Paritätsmatrizen auf Alpha.834
aktualisiert. Erst nach grünem Mint-x64 Release-Gate
Code nach GitHub develop-current pushen und die
exakte SHA mit Pi5-CI und gehosteter x64-CI dokumentieren.

## Weiter offen
Die Korrektur stellt konsistente F9-COF-Diagnose sicher,
nicht den vollständigen erfolgreichen Inworld-Avatar.
Authentifizierte NexVerse Low158-/COF-/Skin-/Outfit-
Downloads, tatsächliche GPU-Inworld-Avatar-Pixel
und simulatorbestätigte WASD-/Yaw-End-to-End-Sitzung
sind weiterhin OFFEN. Projekt 39 %, nächster
Zehnerblock Alpha.835–844 auf Linux Mint.

## Vollständige lokale Mint-x64-Freigabe
scripts/mint-viewer-validation.sh EXIT 0:
CMake/vcpkg Linux-x64-Debug-Viewer vollständig gebaut,
176/176 native CTests inklusive realem Low158-Wiredecoder
und WorldModel-/F9-COF-Tests, 404/404 Python-Prüfungen,
isolierter Xvfb-GUI-Start 0.2.0-alpha.834 mit originalem
Firestorm-Skelett (133 Gelenke), drei GPU-Körpermeshes,
7186 Dreiecken und OGL-X11-Icon/WM_CLASS 128x128.
Kein authentifizierter NexVerse-Login, kein
Inworld-Avatar-/Skin-/WASD-Livetest, kein Deploy.
