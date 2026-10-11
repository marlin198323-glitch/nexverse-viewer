# Firestorm ↔ OpenGenesisLINK ↔ OGL – Paritätsmatrix

Stand: 11.10.2026. Dies ist die **offene, inkrementell zu verifizierende** Soll-/Ist-Matrix. Keine pauschalen Erfolgsmeldungen!

Primärdokument: [FIRESTORM_COMPATIBILITY_CONTRACT.md](FIRESTORM_COMPATIBILITY_CONTRACT.md).

## Vergleichsquellen (nur historischer Snapshot dieser Aufnahme)

| Quelle | Versions-/Vergleichsstand | Konsequenz |
|---|---|---|
| Firestorm | offizieller Release-Branch `Firestorm_7.2.4` (vor Implementierung SHA und Nachfolgereleases neu prüfen) | gesamter Funktionsumfang / Verhalten / erforderliche UI-Aktionen |
| OpenGenesisLINK Server | `main` @ `cb3ebb6ff05160bf9bdb20e78372a68d7b723512` | CAPS, LLUDP, LLSD, LSL, Berechtigungen und tatsächliche Features |
| Server Stable | `0.9.3.8` | Verfügbarkeit im Stable separat verifizieren |
| Server Dev | `0.9.3.10 Dev` | Dev-Code oder grüne Tests sind **keine** Bestätigung auf laufendem NexVerse-Grid |
| OGL Viewer | `0.2.0-alpha.79`, `develop-current` @ `96e1418d076d1d2ca9cf44e9d4ba7d2f1ddd6c21` | Login-/Renderer-/Cache-Grundlagen vorhanden, kein vollständiger Firestorm-Fork und keine vollständige Simulatorabnahme |

Vor jedem neuen Funktionsblock **Server und Firestorm neu abfragen**, Vergleichs-SHA in Änderungen vermerken; diese Tabelle niemals als automatisch aktuellen HEAD ansehen.

## Feature-Familien – Soll und evidenzbasiertes Ist

| Firestorm-Featuregruppe | OGL-Status der Vollparität | Prüfnachweis / weitere Aktion |
|---|---|---|
| Gridverwaltung, Login und Session | `uninventoried` | pro Firestorm-Funktion CAP/LLUDP-/Login-Pfad inventarisieren, echtes Grid testen |
| Bewegung, Kamera, Sitzen, Fliegen, Teleport, Autopilot | `uninventoried` | aktuelle C++-Movement-Tests ersetzen keine avatarbasierte Live-Abnahme |
| Vollständige Avatar-Appearance, Skin, BOM, Mesh Bodies, AO, Gesten und Animationen | `uninventoried` | bisher fehlender sichtbar vollständiger Avatar und Wearables/COF |
| HUD, Attachments, Outfits und Avatar-Skinnning | `uninventoried` | Outfit-/Appearance-Nachrichten und Skin-/Skeleton-/LOD-Verhalten vergleichen |
| Inventar, Drag/Drop, Permissions, Suche, Trash, Upload und Gallery | `uninventoried` | Server-`InventoryThumbnailUpload` vorhanden im Dev; Cache-Limitation beachten |
| Chat/IM/Voice/Groups/Friends/Profiles/Search | `uninventoried` | pro Modul Server- und Clientimplementierung prüfen; OGLVoice in Server-Dev deferred |
| Regionen, Karten, Minimap, Land, Estate und Hypergrid | `uninventoried` | Rechte, Objekt-/Parzellenstatus und Grid-Kompatibilität nachweisen |
| Build-/Objekt-/Face-/Script-Editing und Import | `uninventoried` | Firestorm-Editoraktionen, Simulatormitteilungen und Rechte inventarisieren |
| Modernes World-Rendering, Texture/Mesh, Media, Licht, EEP/PBR/Alpha | `tested_synthetic` für **ausgewählte Rendererpfade**, **nicht** Vollparität | OpenGL-Pixel-, GPU- und Cachetests; In-World-Assets unverifiziert |
| Snapshot, Film, Phototools, Preferences, Debug-Settings | `uninventoried` | einschließlich früher ausgenommener Power-User-Funktionen |
| RLV/RLVa und vergleichbare erweiterte Viewerwerkzeuge | `uninventoried` | sichere bewusst steuerbare Unterstützung und Rechte-/Protokollprüfung |
| Experience-Zustimmung, Preferences, Allow/Block/Forget | `uninventoried` | Server dokumentiert Dev-Handler; echte Firestorm-/Avatar-/CAPS-Live-Abnahme offen |
| Pathfinding/Navigation/NavMesh | `server_unsupported` für komplette Firestorm-Havok/NavMesh-Parität im geprüften 0.9.3.10-Dev-Scope | Server bietet native Teilfunktionen; keine erfundenen `RetrieveNavMeshSrc`-Payloads |
| OGLVoice als zentrale neue OpenGenesisLINK-Funktion | `deferred_with_reason` laut geprüftem Server-Roadmap-Scope | nicht als vorhandenes Firestorm-Voice-Feature verwechseln |
| Wirtschaft, Geld, Marketplace, Gruppenrechte, sonstige Grid-Dienste | `uninventoried` | erst Gridfähigkeit und Berechtigungen prüfen, danach OGL-Funktion liefern |

**Regel:** `uninventoried` bedeutet fehlender vollständiger Einzelnachweis, nicht automatisch, dass keinerlei OGL-Code existiert. Einzelnachweise und Ausnahmen müssen aufgeschlüsselt werden. `tested_synthetic` beweist nur die genannten Testpfade. `server_unsupported`/ `deferred_with_reason` gilt nur für den dokumentierten, zeitlich gebundenen Serverumfang.

## Pflichtschema für Einzel-Funktionen

| Firestorm-Funktion / Menüpfad | Firestorm Branch + Commit + Datei | OpenGenesisLINK Server Commit + API/CAPS/LLUDP | OGL Datei/Funktion | Status | Tests | Abweichung |
|---|---|---|---|---|---|---|
| *Pro Firestorm-Aktion ergänzen; noch nicht inventarisiert* | – | – | – | `uninventoried` | – | – |

## Nächste vier Arbeitsblöcke (nach dem Prioritätsentscheid)

1. Funktionsinventar des aktuellen offiziellen Firestorm-Release-Branches **inklusive** Power-User-Menüs aus Originalquellen erstellen; Ergebnis in Einzelzeilen der obigen Matrix festhalten.
2. Aktuellen OpenGenesisLINK-`main`-HEAD und Servercode zu LLUDP/CAPS/Login/Inventory/Appearance/Groups/Experience/Voice gegen Firestorm-Verhalten prüfen; Live-Fähigkeit getrennt notieren.
3. Geeignete Firestorm-Source-Module auf rechtlich/technisch mögliche Übernahme, API-Adapter und Projekt-Lizenzfolgen bewerten; nicht ungeprüft Quellcode als OGL/MPL ausgeben.
4. Funktionale Lücken zuerst schließen, neue Renderer-/Pluginfunktionen weiterhin in separaten Blöcken, sofern sie Firestorm-Parität und Serververtrag nicht verdrängen.
