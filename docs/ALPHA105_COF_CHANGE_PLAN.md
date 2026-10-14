# Alpha.105 – COF protocol and visible change preview

Reviewed OGL f788dd626d1cf6351db073a87f1b93cc0369cbd6 and OpenGenesisLINK Server 5831913f3dc840aa9661b83fc844546f9f637070, plus Firestorm_7.2.4 llviewerinventory.cpp and message_template.msg, OpenGenesisLINK LLClientView HandleLinkInventoryItem / HandleRemoveInventoryItem.

Low426 LinkInventoryItem wire: AgentData AgentID+SessionID UUIDs, InventoryBlock CallbackID U32, FolderID UUID, TransactionID UUID null, OldItemID UUID, Type S8 (24=AT_LINK), InvType S8, Name Variable1 (NUL-terminated), Description Variable1. Low270 RemoveInventoryItem uses AgentData UUIDs and one ItemID UUID in InventoryData variable block. Both are authenticated and RELIABLE through existing circuit; bad IDs/inputs rejected. Wire data is production format but no UI dispatch occurs.

Read-only planner compares server-reported Outfit and COF items: desired link targets, add, remove, unchanged. Fail-close on unverified COF version, invalid/missing UUIDs, folder links, unsupported types, unresolved or duplicated targets and operation cap of 32. Never mistake a local packet ACK for an accomplished wearable change: reconciliation compares a newly server-read COF with expected target set and required folder-version increase on the original region epoch, with strict invalid item rejection.

F9 Outfit Gallery now has a clickable Aenderungsplan action and right-side list: actual + / - / unchanged counts and COF version, additions and removals. The preview invalidates on server inventory update, COF recheck or region crossing. No bogus Wear button and no COF mutation yet. A later release requires explicit confirmation, multi-step write/ack/readback with partial-failure recovery and actual avatar appearance evaluation.

Tests: wire goldens, queue, conflicting/old/duplicated links, rejected folder-link and stale version, positive and negative server reconciliation, 16:9/21:9/32:9 hitbox tests; CTest/Python/NVIDIA GL GUI smoke run as gates. No authenticated NexVerse Wear/Unwear performed.
