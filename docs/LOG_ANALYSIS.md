# OpenGenesisLINK – Auswertung der Asset-/Simulatorlogs

Stand: 0.2.0-alpha.69 · 11.10.2026

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


## alpha.69 – GPU-Uploadfehler im numerischen Report

**Block A – echter synthetischer GPU-Kapazitäts-/Belastungstest:** `tests/renderer_gpu_pressure_tests.cpp` erzeugt im versteckten OpenGL-3.3-Kontext auf dem Linux-Testrechner tatsächlich 512 kleine GPU-Objekttexturen und 256 kleine GPU-Meshes (Renderer-Grenzen). Bei 16, 128, 256 und 512 Texturen wird ein Frame mit den aktuell angelegten Szenenobjekten gezeichnet; Tests prüfen `visible_objects`, `draw_calls`, OpenGL-Fehlerfreiheit, abgewiesene 513. Textur/257. Mesh, korrekte Ersetzung bestehender Ressourcen sowie erneute Aufnahme nach Slot-Freigabe. Beispiel: unter dieser synthetischen Auslastung ca. 35 ms CPU-Gesamttestzeit inklusive Context-Init/Uploads/Draws. Diese Zeit ist ausdrücklich KEIN Benchmark realistischer größerer OpenSim-Assets.

**Block B – verlässliche GetTexture-GPU-Upload-Rückmeldung:** Die Renderer-Schnittstelle `Renderer::set_object_texture` meldet nun `bool` statt `void`. `LegacyOpenGlRenderer` baut Texturen immer erst in einem neuen GL-Namen auf, prüft Dimensionen und GL-Fehler, übernimmt sie nur bei erfolgreichem Upload und löscht den vorherigen Textur-Namen erst danach (transaktionale Ersetzung). Bei fehlerhaften Daten, vollem internen GPU-Slotlimit oder GL-Fehlern kehrt sie `false` zurück, ohne eine vorhandene Textur derselben UUID zu zerstören. Die Anwendung zählt `texture_gpu_upload_failures`, gibt eine Warnung aus, plant einen transienten Retry und trägt fehlgeschlagene Uploads nicht mehr als GPU-resident ein. Überlegungen zur atomaren Freigabe *anderer* prioritätsbedingt zuvor verdrängter Texturen bleiben ausdrücklich offen.

**Block C – robusterer GetMesh-GPU-Pfad:** Schon vor GPU-Submission prüft der Renderer Meshvertex-Stride, Dreiecks-Indexvielfache, jeden Index gegen die tatsächlich vorhandenen Vertices und angegebene Face-Bereiche gegen den Indexbuffer. Der eigentliche GL-Mesh-Upload kontrolliert Fehler und gültige VAO/VBO/EBO-Handles und gibt teilweise angelegte GL-Ressourcen bei Fehlern frei. Vorheriger LOD-Buffer bleibt bei gescheiterter Ersetzung erhalten. Viewer meldet `mesh_gpu_upload_failures`, klassifiziert GPU-Uploadfehler als transient und plant einen Backoff statt eines permanenten `invalid`-Fehlerstatus.

**Block D – Vollauslastung und Logdiagnose:** Der neue CTest `asset_full_residency_tests.cpp` übt 80 neue vorrangige Partikeltexturen bei 512 belegten Slots und eine Kamera-Fahrt über 400 Mesh-UUIDs bei 256 GPU-Slots aus. Er kontrolliert Priorität, Freigabe, Slotlimits und das Verbleiben naher/nützlicher Assets. `InWorldAssetSnapshot` führt zusätzliche Zähler `texture_gpu_upload_failures` und `mesh_gpu_upload_failures` im vorhandenen numerischen Schema `ogl.asset.streaming.v1`; der Python-Loganalysator nimmt sie in die sichere Feldauswahl auf und zeigt die jeweiligen Maximalwerte. JSON-, GL-Renderer- und Log-Regressionstests angepasst. C++23-Debug-Build und **53/53 CTests** inklusive zweier realer OpenGL-Kontexttests erfolgreich; vollständiger Release-Rebuild, Versionstest und GUI-Start folgen.

**Unverändert offene Grenzen:** Ein OpenGL-Test mit kleinen künstlichen Assets validiert Ressourcenhandling und GPU-Slots, aber weder echtes GetMesh/OpenSim-Decoding noch Live-Weltgeometrie, Avatar, Netzwerk-EventQueue, Teleport und In-World-Interaktion. Tests laufen auf echtem GL3.3/NVIDIA-Linux mit `DISPLAY=:0`; ohne GL-Display überspringen sich die beiden GPU-Kontext-CTests per Returncode 77. Shared GPU-Budget von 384 MiB ist eine Software-Schätzung ohne Terrain/UI/Treiber-Speicher. Die atomare Textur-Ersetzung gilt nur für denselben UUID-Slot, nicht für bereits vorab verdrängte andere Slots.

**Nächster Release / verbindlicher Plan:** Schrittweise GPU-Upload-Rollback/Recovery bei Slot-Evictions, realistischere Szenengeometrien/Texture-Mip-Lastprofile, strukturierte Upload-Failure-Gründe und weitere Renderer-Timing-Integration. Avatar/Bewegung/Mesh-/Teleport-/CAPS-Bugfixes wie vom Nutzer vereinbart separat.

Run: `env DISPLAY=:0 ctest --preset debug --output-on-failure`; GPU-Pressure einzeln: `env DISPLAY=:0 ./build/debug/nexverse-renderer-gpu-pressure-tests`; Viewer: `./build/debug/nexverse-viewer 2>&1 | tee /tmp/ogl69-live.log`; nur `develop-current` nutzen.

Zwei neue erlaubte Felder in In-World-Snapshots: `texture_gpu_upload_failures` und `mesh_gpu_upload_failures`. Der Bericht enthält die Aggregate `max_texture_gpu_upload_failures` und `max_mesh_gpu_upload_failures`, bei fehlenden Live-Snapshots `null`. Der Renderer-Stresstest liefert keine vertraulichen Asset-UUIDs oder Simulatordaten.
