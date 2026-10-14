# Alpha.107 – Firestorm COF Batch, Single Unlink und sichtbare Bedienoberflaeche

## Source
OGL develop-current initial 7b00962acf67ea5cd9e1e4588e660f453b863dd0. OpenGenesisLINK main 5831913f3dc840aa9661b83fc844546f9f637070. Referenzen Firestorm_7.2.4 message_template.msg und llviewerinventory.cpp, OpenGenesisLINK LLClientView LinkInventoryItem/RemoveInventoryItem. Kein neuer User-Server-CAPS-Contract erfunden.

## Add two to four links
CofBatchAdd requires original owner-scoped COF snapshot with nonzero version, at least one preexisting link, and a read-only plan listing only 2-4 additions and no removals. F9 GUI requires an initial action followed by explicit confirmation within 20 seconds. Sends first authenticated RELIABLE Low426, then awaits a new owner-scoped InventoryDescendents2 Current Outfit Folder response with greater version and EXACT expected unique AT_LINK target set. Only then the next queued command. 18s timeout per step, no auto retry and no destructive auto rollback. On partial failure display confirmed N/M and demand a user-triggered genuine COF readback before any further COF writes.

## Remove exactly one link
CofSingleUnlinkConsent requires exactly one remove, zero adds, nonempty remaining COF, explicit two-click confirmation, same region/version and real server link UUID. One RELIABLE Low270 only; success after greater authoritative folder version with removed LinkItemID absent and exactly the expected remaining link targets. An ambiguous result never triggers automatic retry.

## Visible GUI and tests
Outfit-Galerie F9 > Aenderungsplan shows '2-4 Links sequenziell anlegen' or 'Einen COF-Link ablegen' only if respective plan is valid, then 'BESTAETIGEN'. Progress, stop, stale region, unknown server answer and individual/batch statuses are visible. Closing F9 cancels pending first clicks, ongoing server readback continues. Single Add, Batch and Single Unlink are mutually exclusive.

New tests firestorm_cof_batch_tests.cpp, firestorm_cof_unlink_tests.cpp; updated existing single-link fail-closed test and outfit_gallery_layout_tests.cpp at four resolutions. Full C++/Python/GPU tests and exact GitHub SHA match are release requirements.

Not implemented or verified: combined Outfit Wear/Unwear (add and remove together), atomic rollback/appearance, actual avatar outfit rendering update, real authenticated NexVerse inworld end-to-end acceptance. A server-confirmed COF Link is NOT an avatar visibly wearing it.

## Alpha.107 zusätzlicher Release-Serververgleich (14.10.2026)

Während der Alpha.107 Release-Pflichtprüfung erschien ein neuer OpenGenesisLINK-Server Commit: 2b7c19c9396eb2f75fd60a41a33a05655909da6b (17:50:43 UTC, #207). Vorher 5831913f3dc840aa9661b83fc844546f9f637070. Fünf Dateien manuell geprüft: OglApiDocsAccess.cs, NexVerseWorldApiConnector.cs, Robust.ini.example, Robust.HG.ini.example und tools/ci/verify_ogl_docs_admin_ui.py.

Inhalt: abgesicherte AdminDocsTrustedProxyAddresses für TLS-Termination, verifizierte Peer-IP (Loopback oder explizite Allowlist), exakter X-Forwarded-Proto https, keine Annahme untrusted X-Forwarded-For. Anonyme Dashboard-Zugriffe bleiben 401; bei Admin-Bearer kann Proxy-HTTPS-Konfigurationsfehler 426 statt irreführendem 401 gemeldet werden. Owner/GridUser UserLevel >=200 und AdminAll Scope bleiben zwingend. Klassifikation SERVER_ONLY_OPERATOR_ADMIN_SECURITY: weder neue LLUDP/CAPS-Benutzer-Viewer Funktion noch OGL Plugin, kein Token oder Admin-Schlüssel im Desktop Viewer.

Source Review abgeschlossen; geprüften SHA-Pin auf 2b7c19c... aktualisiert. Release-Gate danach erneut ausführen.
