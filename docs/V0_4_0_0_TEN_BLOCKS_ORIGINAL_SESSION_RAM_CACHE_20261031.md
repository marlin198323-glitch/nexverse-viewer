# OGL Viewer v0.4.0.0 – 10 Blöcke: Sitzungs-RAM für Original-Wearables, CAP-Fehlerreaktion

Datum: 31.10.2026 | Linux Mint jam-MS-7D28 | GitHub develop-current | v0.4.0.0 IN_ARBEIT

## Authentifizierter Ausgangsbefund und Abnahmegrenzen

- Die beiden GitHub-CI-Läufe für **37c1db4** sind grün: Linux Mint x64 Source Check und vollständiger Linux x64 Viewer Build.
- Der nächste Benutzerlogin mit dem **37c1db4**-HTTP-ViewerAsset-Transfer hat NOCH NICHT stattgefunden. Älteste laufende Livelogs enden in der Sitzung vom 31.10.2026 ab 00:42:04, also mit dem vorherigen Binary.
- Letzte authentifizierte Weltprobe 00:45:34: **269/280 Texturen GPU-resident (96 %), 384/384 Meshes (100 %)**. Diese Coverage beschreibt Textur-IDs und KEINE visuelle Vollauflösung: 241 der 269 residenten Texturen waren reduzierte Mips. Mindestens zehn einzigartige fehlende Textur-Assets in der letzten Probe, einige HTTP-404. Mesh-LOD-Duplikate: 0.
- Echter Server-COF-Rohinhalt: Array vorhanden, 0 Items, 0 vom Parser verworfene Items. Low382 Original-Wearables **5/5** und primäre Bodyparts **4/4** extern gegen Originalinventar bestätigt, aber **0/3 echte Low158 Serverbakes** und **kein nachgewiesener lokaler Head/Upper/Lower GL-Farb-Upload**. Das heutige Update löst NICHT automatisch serverseitige Bakes/Outfit-Verknüpfungen.
- Simulator-Main im OpenGenesisLINK Repo unverändert unter `1b8fb1cbbcddbf3f229879bd5046d0a1b1c72b3f`.
- Hintergrund ViewerAsset-CAP: https://opensimulator.dev/wiki/Capabilities. Das Grid muss diese Capability tatsächlich anbieten; sie wird nicht erfunden.

## Zehn umgesetzte Entwicklungsblöcke

1. **Aktuelle GitHub- und Livelog-Abnahme:** vorheriger HTTP-Originaltransfer jetzt CI-success; noch keine neue authentifizierte HTTP-CAP-Sitzung. Offene 0/3 Bakes nicht als repariert melden.
2. **RAM-only Originalwearable-Cache:** neuer `WearableOriginalSessionCache`, maximal 24 echte dekodierte Firestorm-Originalassets, kein persistenter Speicher, keine inventar- oder personenbezogenen Logwerte.
3. **Login-/Session-Isolation:** Cache wird auf Account-, Sessionwechsel und Logout geleert; Regionenwechsel darf nur Bytes aus demselben authentifizierten Konto/Sitzung wiederverwenden. Cache-Referenzen können nicht an andere Anmeldungen übergehen.
4. **Originalinventar-Bindung:** jeder Cache-Hit verlangt ein neu vorliegendes Originalitem mit exakt passendem Owner, Session, Item-UUID, Asset-UUID, Asset-/Inventartyp und Wearable-Flags; nicht bloß Asset-UUID-Vergleich.
5. **Strenger Typ-/Daten-Check:** nur echtes Outfit-Wearable Clothing (5) oder Bodypart (13), Inventartyp 18, gültige Nicht-Null-UUIDs, passender Subtype 0..16; parse-limits für Firestorm visual params/textures, verbotene Typmischung.
6. **Schneller Wiederverwendungsweg:** nach positivem Cache-Hit keine neue HTTP-/LLUDP-Anfrage; die gespeicherten ORIGINAL-Bytes gehen weiterhin durch denselben Renderer-Thread Original-Owner-/COF-Version-/Subtype-Akzeptanzpfad wie Netzwerkanfragen. Keine künstlichen Bakes.
7. **Stale-Antwortschutz:** ursprüngliche Session-ID wird bei jedem LLWearable-Download und ursprüngliche Owner-ID bei jeder Antwort explizit revalidiert. Regions-/COF-Wechsel verwirft schwebende Cache-Antworten; kein Cross-Avatar-Leak.
8. **CAP-Methode richtig interpretieren:** HTTP 405/501 markiert eine für diese Region nicht unterstützte ViewerAsset-GET-Methode; weitere Wearables gehen direkt über den bestehenden LLUDP-Rückfall statt wiederholt HTTP-Timeouts zu erleiden. HTTP 404 bleibt asset-spezifisch und sperrt keine anderen Originale.
9. **Ehrliche Diagnose:** Live-Log und `scripts/ogl-asset-diagnose.py` zählen Session-RAM-Cache-Hits und deaktivierte GET-CAPs getrennt von bestätigten HTTP-Originalassets, LLUDP-Transfers und echten GPU-Avatar-Uploads.
10. **Native- und Python-Regressionen + Handover:** C++-Tests prüfen 24er-Eviction, Owner-/Session-/Typ-/Flags-/Item-/Asset-Verwechslung, Kontoausgleich und 405/501; 17 neue Python-Verträge prüfen Integration, Datenschutz, Stale-Guard, Diagnose. `CHATUEBERGABE.md`, `ROADMAP.md`, `projectplan.md` und `docs/RELEASE_ROADMAP.json` aktualisiert.

## Bedienung und nächste Live-Abnahme

Viewer auf Linux Mint nach regulärem Beenden der vorherigen Sitzung starten:

```bash
cd "/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer"
python3 scripts/ogl-cache-status.py
./build/debug/nexverse-viewer
```

Nach dem Grid-Login:

```bash
python3 scripts/ogl-asset-diagnose.py
```

Zu prüfen: `ViewerAsset CAP available=0/1`, `Avatar original wearable accepted:`, `Avatar original wearable RAM cache hit:`, `Avatar original wearable LLUDP/HTTP timeout:`, und **erst dann** `Avatar local unbaked self-preview: source=authenticated_legacy_originals gpu_head_upper_lower=N/3`. Die letzte Zeile ist nur ein echter lokaler GL-Erfolgsbeleg, wenn N größer 0 ist. Alle drei finalen Head/Upper/Lower-Serverbakes müssen zusätzlich direkt aus aktuellen Low158-Paketen und echten GPU-Uploads bestätigt werden. Fehlende Serverbakes werden nie durch erfundene UUIDs ersetzt.

## Verifizierte Build- und Testabnahme

- Linux Mint `cmake --build build/debug --parallel 6`: **PASS**, vollständiges GUI-Viewer-Binary nach Session-Guards.
- Native CTest `ctest --test-dir build/debug --parallel 6 --output-on-failure`: **195/195 PASS**; alter LLUDP-Wearable-Test um echtes Sitzungs-RAM/Eviction/Item/Flags/Owner/Session und HTTP 405/501 ergänzt.
- Python `python3 -m unittest discover -s tests -p 'test_*.py'`: **1049/1049 PASS**, 17 neue Session-Cache-/CAP-/Stale-/Privacy-Verträge.
- `bash scripts/pi5-headless-tests.sh`: **PASS**, inklusive **1049/1049 Python** und Native-Headless-Prüfungen.
- `bash scripts/test-mint-window-branding.sh`: **PASS**, Systemicon 128x128, X11 WM_CLASS `opengenesislink-viewer`.
- `git diff --check`, JSON-Validierung Release-Roadmap: **PASS**.
- Der C++-Build/Test ist keine Bestätigung eines eingeloggten ViewerAsset-Originaldownloads und keiner fertigen eigenen oder fremden Avatar-Skin. Live-Abnahme weiterhin OFFEN.

## Offene Punkte

- Neue HTTP-Assetquelle unter echtem Grid-Login nach neuem Viewerstart prüfen; das letzte getestete Binary hatte die Capability bereits kompiliert, aber die Benutzer-Session noch nicht neu angemeldet.
- Die einmalige Erstladung der Original-Wearables wird durch RAM-Caching allein nicht schneller, sofern HTTP und LLUDP beide fehlschlagen. Erstes Fetch-/Assetserver-Protokoll und Firestorm-Skin-Layer-Komposition bleiben die nächsten E2E-Prioritäten.
- Viewer ist noch nicht vollständig Firestorm-paritätisch (Avatar-Skin, Hair/Eyes, Alpha/Wearables, Attached Mesh, volle Texturauflösung); Serverasset-404 bleiben unabhängig vom lokalen Cache.
- Fortschritt **38,65 % gewichtet / rund 39 %** unverändert, keine main-Releasefreigabe.
