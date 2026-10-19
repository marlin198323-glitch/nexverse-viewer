**ZEHN-BLÖCKE-REGEL: BEI JEDEM WEITER ZEHN ECHTE ENTWICKLUNGSBLÖCKE (10 ECHTE ENTWICKLUNGSBLÖCKE). Alpha.465–474, Projektfortschritt 39 %.**

**ALPHA.464 QUELLSTAND GEPRÜFT 19.10.2026:** Feature-SHA 320abed60c759ec09e7d4950d85a3e4a46fac3c1 auf GitHub gepusht. Linux Mint x64 Build 373/373, native C++/OpenGL CTest 158/158, Python 200/200, ARM64-kompatibles Headless, isoliertes Xvfb und X11-Icon/WM_CLASS PASS. Auf ECHTEM Pi5 wurde exakt Feature-SHA 320abed60c759ec09e7d4950d85a3e4a46fac3c1 direkt per SSH aus dem Repository getestet: PI_ARM64_HEADLESS_EXIT=0, OGL_PI5_NATIVE_HEADLESS_OK, 200 Python-Tests OK (1 unterstützter Skip). GitHub Actions Run 39286040685 ist aktuell QUEUED, NICHT SUCCESS. Kein Arm-GPU/GUI-Test oder authentifizierter NexVerse-Avatar-/Outfit-/WASD-End-to-End-Beleg. Dokumentation und Testresultate nach GitHub; keine Produktion. Nächste zehn Blöcke Alpha.465–474; 39 %.

# Alpha.455–464 – Legacy-Wearables gegen authentifizierte COF-Originalitems

19.10.2026 | develop-current | 0.2.0-alpha.464 | Projektstand 39 % (38,65 % gewichtet)

## Zehn Entwicklungsblöcke

455. GitHub HEAD 1cf0686, CHATUEBERGABE, Projektplan, Fortschrittsdatei und OpenGenesisLINK Server main 293d1d6 geprüft; Alpha.454 Pi5 GitHub-CI noch queued.
456. Legacy Low382 und CurrentOutfitFolder Original-Item-Inventar getrennt geprüft; bisher fehlte der Verknüpfungsnachweis.
457. Read-only LegacyWearableCofAudit mit echter COF-Symlink-Auflösung über Original-ItemID. Nur Wearable-Assettypen 5/13 und Firestorm Subtypes 0..16.
458. Exakter Vergleich ItemID + AssetID + WearableType, Abweichungen, fehlende Originalitems und Null/Dummy-Einträge gezählt, kein falscher Wear-Nachweis.
459. Vergleich der vom Simulator gemeldeten eigenen Low382-IDs mit unabhängigen CAPS FetchInventory2 und FetchInventoryDescendents2 Original-Items.
460. Vergleich nur bei gültigem aktiven Agent und Session sowie aktuellem Regionlease, COF-Folder und COF-Versionsnummer.
461. F9 Outfit-Floater um separat als read-only markierte Legacy/COF-Prüfstatuszeile ergänzt. Keine automatische Inventarmutation.
462. COF unbekannt oder Low382 fehlend wird als ausstehender Quellenstatus gezeigt. Vollständige ID-Übereinstimmung bleibt ohne SSA und Bakes KEIN realer Wear-Beweis.
463. Native C++-Regressionen für Übereinstimmung, Asset- und Typabweichungen, Duplikate, Null-Slots, fehlende COF-Originals, ungültige Flags und reine Zählausgabe; Python-Verträge für Agent/Session/Lease/F9.
464. Vollständige Linux-x64- und native GL-Tests, Python, ARM64-kompatibles Headless, isolierte Xvfb-GUI und Taskleistenicon; GitHub Push und Source-SHA-gebundene Pi5-Prüfung.

## Grenzen
Die Legacy Low382-Antwort hat keine COF-Version. Ein Vergleich mit einem später gelesenen COF beweist nur konsistente Referenzen zum jeweiligen Beobachtungszeitpunkt, nicht tatsächlich getragene Kleidung, GPU-Texturen, Low158 Server-Self-Appearance oder WASD. Vollständige NexVerse Avatar-/Outfit-/COF-/WASD-Liveintegration weiterhin offen. Projektwert bleibt 39 %. Kein produktives Deployment. Kein AgentSetAppearance-Upload oder ungeprüfte Outfit-Änderung.

Nächste zehn Blöcke Alpha.465–474: echte Live-Server-Outfit-/Appearance-Evidenz und COF-Versionen, GPU-Bakes/Anhänge/Firestorm-Menüs weiter implementieren.
