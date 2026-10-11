# OpenGenesisLINK – Auswertung der Asset-/Simulatorlogs

Stand: 0.2.0-alpha.67 · 11.10.2026

Das Werkzeug `tools/ogl_asset_log_report.py` fasst lokale Viewerlogs zusammen, ohne UUIDs, CAP-URLs, Namen oder beliebige Rohmeldungen in den Bericht zu kopieren.

Auswertung des normalen lokalen Viewerlogs:

```bash
cd '/home/jam/Schreibtisch/OGL-Viewer/nexverse-viewer'
python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"
```

Für eine Ein-Zeilen-JSON-Ausgabe (zum Import in Diagnosetools): Option `--compact` verwenden. Das Programm schreibt ausschließlich nach stdout und ändert die Eingabedatei nicht.

Beim Stand vom 11.10.2026 wurden in der vorhandenen lokalen `viewer.log` 230 ältere GetMesh- und 56 GetTexture-Fehlermeldungen gezählt; davon 42 mit Hinweis auf HTTP 404 und eine auf HTTP 5xx. Die übrigen 243 Meldungen enthalten in dieser historischen Datei keinen eindeutigen strukturierten Fehlerstatus. Der Bericht ist eine **historische Protokollauswertung**, keine Aussage über aktuelle Live-Fehler nach alpha.67.

Neues Snapshotformat: `InWorldAssetSnapshot {"schema":"ogl.asset.streaming.v1",...}`. Es enthält nur feste numerische Felder zur Region, GPU-Residency, Backoff, RAM-Belegung und Mesh-/Textur-Abdeckung. Der Analyzer prüft das Schema, zählt beschädigte Snapshotzeilen und wertet die niedrigste gemeldete Texture-/Mesh-Abdeckung aus. Unbekannte oder möglicherweise sensitive JSON-Felder werden entfernt. **Die bis alpha.66 erzeugten lokalen Smoke-Logs enthalten noch keine gültigen In-World-Snapshots**: Diese entstehen erst nach einem tatsächlichen erfolgreichen Regionsbeitritt mit `simulation_ready`.

Test: `python3 tests/test_ogl_asset_log_report.py` ist optional; im normalen Debug-CTest ist der Test bei vorhandenem Python3-Interpreter bereits registriert.

Grenzen: Das Werkzeug erkennt Muster aus protokollierten Fehlermeldungen, aber repariert keinen Mesh-Decoder, keine Avatar-Darstellung, keine EventQueue und keine Serverseite. Ein Log mit 100 % Asset-UUID-Residency garantiert noch keine vollständig gezeichnete Welt.
