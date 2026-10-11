# OpenGenesisLINK – Auswertung der Asset-/Simulatorlogs

Stand: 0.2.0-alpha.68 · 11.10.2026

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


## alpha.68 – Neue Performancekennzahlen

**Block A – reale CPU-Uploadzeit statt Worker-Wartezeit:** `AssetUploadTelemetry` misst die Wandzeit, die das Render-Thread-Aufrufen von `set_object_texture(...)` und `set_mesh_asset(...)` benötigt. Das feste Ringfenster hält die letzten 64 Uploadversuche und bietet `recent_avg_us`, `recent_p95_us`, `recent_max_us`, `recent_count`; Nullzustand und Überlauf werden konservativ behandelt. Messgröße ist **CPU-seitige OpenGL-Übermittlung**, keine GPU-Fertigstellungszeit, kein `glFinish` oder OpenGL-Timer-Query.

**Block B – adaptives GPU-Upload-Backpressure:** `AssetGpuUploadGate` bleibt bei schnellen Uploads bei mindestens 30 ms Abstand; langsame CPU-Übermittlungen verlängern das nächste Uploadfenster auf etwa 4× beobachtete Millisekunden, bis maximal 150 ms. Der nächstfolgende kurze Upload senkt das Intervall wieder auf den Grundwert. Mesh und Textur teilen sich den Renderthread-Gate. Bereits fertiggeladene Futures bleiben für spätere Frames verfügbar. Terrain/UI und vorbestehende Renderer-Arbeiten unterliegen dem Gate nicht.

**Block C – Szenenpriorisierung und erweiterte Health-Snapshots:** `AssetScenePriorityFeedback` beobachtet alle etwa 750 ms die GPU-gewünschten UUID-Sets, erkennt erhebliche Wechsel (symmetrische Differenz mindestens ein Viertel der vorherigen/neuen Set-Größe) und erhöht für drei stabile Planzyklen den weichen Loaded-Bonus temporär: Mesh von 24 auf 42 gewichtete Meter, Textur von 8 auf 14. Neue näher gelegene Assets bleiben weiterhin auswählbar; keine UUID wird fest gepinnt. Regionsepochen löschen den Zustand. `InWorldAssetSnapshot` behält `ogl.asset.streaming.v1`, ergänzt rein numerische Felder zu CPU-Uploadavg/p95/max, Sampleanzahl, effektiver Uploadsperre und Texture-/Mesh-Prioritäts-Churn. Der vorhandene Log-Analyzer wertet zulässige Felder aus und berechnet Peak-p95 bzw. Peak-Churn; er gibt keine CAPS-URLs/UUIDs aus.

**Block D – echter Renderer-Integrationstest:** `renderer_gpu_asset_integration_tests.cpp` erzeugt einen unsichtbaren echten GLFW/OpenGL-3.3-Kontext, lädt/repliziert/entfernt synthetische Textur und Mesh auf dem GPU-Pfad, prüft ungültige Meshdaten, führt einen Renderer-Frame aus, kontrolliert `visible_objects`, Renderer-Backend und GL-Fehlerfreiheit. Ohne OpenGL-Display überspringt CTest den Test via Exitcode 77; auf dem autorisierten Linux-PC wurde der Test unter `DISPLAY=:0` vollständig ausgeführt. Zwei weitere C++-Tests prüfen Rolling-Telemetrie und Scene-Prioritätsrückkopplung; erweiterte JSON-/Log-/Upload-Gate-Regressionstests. Gesamt: **51/51 CTests bestanden**, inkl. realem OpenGL-Test, anschließend vollständiger Versions-Rebuild/GUI-Smoke.

**Grenzen:** Der Renderer-Integrationstest arbeitet mit synthetischer Geometrie und einem lokalen GPU-Kontext, ohne authentifizierte NexVerse-Region oder echtes Asset. Er belegt noch nicht, dass Avatar-Body/Bewegung, Mesh-Objekte aus dem Simulator, CAPS-EventQueue, Teleport oder alle Weltassets korrekt sind. Diese bekannten Punkte bleiben bis zur vereinbarten späteren Bugfixphase dokumentiert. CPU-Uploadzeiten dürfen nicht als verifizierte GPU-Latenz bezeichnet werden.

**Nächster 3–4-Blöcke-Release:** Wiederholbare Smoke-/GPU-Profiling-Sessions anhand synthetischer Szenen unterschiedlicher Größe, Renderer-Upload-Fehlertransparenz, Scene-Priority-Schutz unter voller Asset-Residency und Ausbau der testbaren Welt-/Mesh-Renderpfade. Maßgeblich bleibt `projectplan.md`.

Start: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl68-live.log`. CLI-Logauswertung: `python3 tools/ogl_asset_log_report.py "$HOME/.local/state/opengenesislink/viewer.log"`; Source Branch `develop-current`.

Zusätzliche erlaubte Snapshot-Felder: `gpu_upload_cpu_samples`, `gpu_upload_cpu_avg_us`, `gpu_upload_cpu_p95_us`, `gpu_upload_cpu_max_us`, `gpu_upload_pacing_ms`, `texture_priority_churn`, `mesh_priority_churn`. Die Berichtsaggregate `peak_gpu_upload_cpu_p95_us`, `peak_texture_priority_churn`, `peak_mesh_priority_churn` sind `null`, solange keine entsprechenden In-World-Snapshots vorliegen. Alle neuen Daten sind numerisch und die Ausgabe enthält nach wie vor keine UUIDs, CAP-URLs oder komplette Rohlogzeilen.
