# Alpha.132–135 – Avatar-Bake-Streaming und F9-Live-Status

15.10.2026; Raspberry Pi 5 ARM64 als nexdev, Repo marlin198323-glitch/nexverse-viewer develop-current.
Ausgangs-Commit e07e19186cd82daf052850550ec43c482da2614e, Version 0.2.0-alpha.135.

## 132 – SelfAvatar/Bake-Priorität
Neues Header-Only-Modul include/nexverse/assets/avatar_bake_stream_priority.hpp.
Original Server-TE-Refs Head/Upper/Lower zuerst für SelfAvatar, danach für benachbarte Avatare (3D-Radius maximal 96 m), nahe zuerst.
Doppelte/leere/Null- und Firestorm-Placeholder-IDs ausgeschlossen; nicht-finite Positionen gesperrt.
Standardlimit 12 UUIDs, hartes Limit 24. Andere acht Bake-Regionsdaten werden nicht fälschlich als renderbare Körper-Meshes vorgetäuscht.

## 133 – Gemeinsamer GPU-Residency-Budgetplan
plan_texture_residency unterstützt optionale Avatar-Prioritäten vor Partikeln/Welttexturen.
Gemeinsame max. 512 GPU-Slots bleiben erhalten; GetTexture, Mips, Cache,
Eviction, Failure/Retry und Region-Epoch-Lifecycle unverändert.
Bei verschwundenen Avatar-Refs wird die entsprechende GPU-Residency im nächsten Plan freigegeben.
Keine neue Asset API, kein Upload und kein Pixelgleichheitsbeleg.

## 134 – Opt-in F9 Live
Der bereits klickbare Button Appearance pruefen aktiviert eine rein lokale,
periodische 750ms-Aktualisierung der Server-TE/GPU-/COF-Evidence.
Text beginnt mit KEIN BAKE LIVE, Head/Upper/Lower G/S/-.
Region-Lease, authentifizierter Agent und gültiges SelfAvatarAppearance werden kontrolliert.
Nach Regionswechsel oder Schließen endet Monitoring, COF-Aktionsstatus behält Vorrang.
Die regelmäßige Anzeige sendet keine zusätzlichen CAPS-/LLUDP-Aufrufe.

## 135 – Pi CI und Regression
Neues tests/avatar_bake_stream_priority_tests.cpp: Self vor remote, nähere Avatare,
NaN/Null/Duplicate, voller GPU-Budgetplan, Eviction nach Verschwinden,
begrenztes Prefetch. CMake/CTest-Registrierung hinzugefügt.
scripts/pi5-headless-tests.sh startet vier C++23-Programme mit g++14 und
-Wall -Wextra -Wpedantic -Werror und die Python-Regressionssuite.
Tatsächliches Pi-Ergebnis: vier C++-Tests PASS, 18 Python-Tests PASS,
Git diff --check und Bash-Syntax PASS.

## Serverabgleich und offene Gates
Live opengenesislink/opengenesislinkserver main d31c03168f0f04f4ce02e82ee3246e16de744748,
22 Commits seit geprüfter Baseline 4e99976630f1008db9014ea2b86c3723b09e9fe6.
Serverpakete: Flug-CrossRegion Velocity #221, Child-Avatar-Prefetch #223,
einheitliche Asset LevelUpload Gate #217, sichere HG Inventarverwaltung #212.
Vergleich >300 geänderte Dateien unvollständig; striktes Upstream-Gate bleibt
BLOCKIERT und Baseline wird nicht ungeprüft angehoben.
Vollständiger Viewer CMake/CTest, GPU/GL/F9-E2E und authentifizierte NexVerse-
Pixel-/Outfit-Abnahme sind NICHT auf Pi ausgeführt; CMake/vcpkg/GPU fehlen.
Nächste Schritte: echte UV-/LLM-Bake-Texturzuordnung messen, Firestorm Textur-
und Masken-Pixelvergleich, sichere Upload-/COF-Readback-/Rollback-Gates,
separater nativer GUI-Build auf geeignetem Buildhost.
