# Alpha.112–115: Vier Firestorm TexLayer Blöcke im OGL Viewer

Source: OGL Viewer develop-current 8d8e79e7798b4db294ebb875029498a0237496df, OpenGenesisLINK Server main 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94, local Firestorm_7.2.4 character/avatar_lad.xml. Do not redistribute Firestorm third-party assets as part of viewer repo before asset-license review.

Alpha112: TinyXML2 bounded (max1MiB, 512 TexLayers) 11 original regions, original 45 local ETextureIndex names, parser for ordered normal/bump, alpha masks, color params, local alpha-only and external TGA. Original local file parsed 89 TexLayers with exactly 3 simple color-only inputs; other effects require Firestorm parity.

Alpha113: Safe, read-only 128px RGBA source-over unpremultiplied alpha compositor for XML-ordered simple local textures; masks, param Alpha, tint, bump, external TGA not simulated, counted as skipped. is_verified_bake is always false and composites never enter simulator or avatar GPU skin.

Alpha114: Actual previously downloaded user-original COF/region/asset typed GetTexture RGBA8 images mapped per canonical original TextureEntry, reused for duplicate asset UUID and invalidated with current owner/epoch/COF version and GPU lifetime.

Alpha115: Real F9 TexLayer lesen / Teillagen mischen actions; manifest status and three Head/Upper/Lower OpenGL previews where safe local layers are available. Six window-size UI hitbox tests, synthetic red/blue alpha-over and real Firestorm original XML test.

Next 4-stage Alpha116-119: source alpha mask TGA decoder, wearable params and exact color interpolation, full TexLayer alpha/layer composite, GPU/CacheID/Server AvatarAppearance and real user-granted COF Wear/Unwear validation. Never claim verified avatar bake before true assets and in-world check.

## Alpha.112–115 Live-Serverreview nach Implementierung (14.10.2026)

Der OpenGenesisLINK Server main ist vor Release auf Commit 20a7fd765527807fd9d7887965b5f5f28d42d68f (#209) vorgerückt, zuvor 5d9c616470ac5b30de2afdadd6d5f82b33b0ae94. Alle neun geänderten Dateien und GitHub-Patches überprüft: Control Center v2, WorldAPI Metadaten für 0.9.3.12 als Release (früher 0.9.3.12 entwicklung), Version-aware ChatGPT Guidance, Operator Diagnose/CI, BaseHttpServer malformed XML-RPC => HTTP400 statt impliziter Erfolgsantwort, HG InstantMessage Log-Throttle nur für unauflösbare Empfänger OHNE Aussetzen der eigentlichen UAS/Zustellversuche.

Entscheidung SERVER_ONLY_WORLDAPI_OPERATOR_DIAGNOSTICS_AND_HG_XMLRPC: Keine User-Viewer LLUDP Transfer/GetTexture/COF Änderungen, kein neues API/Secret im OGL Viewer. Weiterhin OpenGenesisLINK=Server-Produkt, NexVerse=konkretes Grid. Die Rollenprüfung UserLevel >=200 ist nur für den Serveradministrator; darf NICHT in die Anmeldung des normalen Viewers einfließen. Release gilt als serverseitige Metadatenangabe, nicht als Live-NexVerse-Abnahme einer fertig gebackenen Avatar-Kleidung. Baseline aktualisiert auf 20a7fd... und strikten GitHub-Check anschließend wiederholen.
