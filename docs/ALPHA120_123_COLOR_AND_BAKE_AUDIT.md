# Alpha.120-123 Firestorm source color plus simulator bake-UUID audit

Baseline OGL Viewer develop-current 97345466837df755317b9439c3beaa87050aa5cf; OpenGenesisLINK Server main 20a7fd765527807fd9d7887965b5f5f28d42d68f. Reference Firestorm_7.2.4 lltexlayer.cpp (findNetColor and calculateTexLayerColor), lltexlayerparams.cpp (LLTexLayerParamColor interpolation and param_color XML), installed character/avatar_lad.xml 99 param_color items across 71 color layers and up to 25-palette eye-shadow values.

120. Core XML: strict Firestorm fixed_color (4 x uint8), global_color symbolic reference, up to 16 color params/layer with bounded 32 RGBA keyframes each, operations add/multiply/blend, strict ID/range/finite validation. Mixed alpha+color classification. Firestorm assets remain local, none bundled in repo.

121. Core color math: original Firestorm color-key interpolation with REAL worn original visual-parameter weight, initial fixed color or zero, ADD/MULTIPLY/BLEND, clamp. Global skin/hair/eye color references and missing worn weights fail closed; no fabricated default.

122. Local renderer and F9: compositing source RGBA local images with safely resolved tint and alpha mask (single supported param_alpha); colored/masked/missing/unsupported counts and 3 Head/Upper/Lower UI OpenGL images. This is a diagnostic 128px PARTIAL image only, never a baked texture or skin.

123. Server readback: existing F9 Appearance pruefen action now reports nonplaceholder server-provided AvatarAppearance TextureEntry Baked UUIDs (11 Firestorm slots) and server COF version vs Inventory CAPS version, without pretending to upload/confirm an image. Renderer retains original server-origin Avatar baked IDs; F9 local preview never sent as simulator asset.

Tests check original real Firestorm 25-entry palette, Add/Multiply/Blend/fixed/alpha operations and fail-closed global color/invalid XML, plus previous TGA/Mask/Appearance/UI regressions. All C++/Python and native GL3.3 GPU smoke before GitHub.

Still missing exact global colors, Firestorm pixel parity for multiple alpha/domain/morph masks, official baked texture upload and cache, true mesh-attached outfit appearance, rollback and authenticated in-world NexVerse test. Continue in four combined blocks Alpha124-127.
