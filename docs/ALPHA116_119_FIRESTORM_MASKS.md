# Alpha.116–119 – Four integrated Firestorm mask, param and GUI development blocks

Baseline: OGL develop-current before changes 87181e7e54a7703710ddc38a4a84782cd082e2bc, OpenGenesisLINK Server main 20a7fd765527807fd9d7887965b5f5f28d42d68f, Firestorm_7.2.4 original installed avatar_lad.xml and locally installed character TGAs. No original third-party TGA data bundled in repository.

Alpha116 original TGA input: safe basename-only reader, 16MiB/2048px bounds, true compressed RLE grayscale 8-bit mask and BGR/BGRA RGB; raw variants too. Correct origin and BGR to RGBA8. Actual glove_length_alpha.tga 512x512 and invalid RLE tested.

Alpha117 original XML params: param_alpha metadata id, wearable, mask TGA filename, scalar bounds/default/domain/multiply_blend/skip_if_zero, alpha-only eligibility vs unsupported color/bump/visibility. Validated finite XML ranges, maximum 16 parameters per layer.

Alpha118 new diagnostic mask compositor: conservative single param_alpha mask for a local source texture and real COF-authenticated wearable visual weight; rejects absent or conflicting params or missing/unsupported masks. Grayscale mask strength scales the source alpha in ordered RGBA preview. Domain/multiply and multi-param Firestorm rules are not pixel-equivalent and stay unsupported as an in-world bake. real_bake always false.

Alpha119 visible F9 UI: Masken/Teillagen click uses local original mask images with real owner/epoch/COF-valid LLWearable RGBA and visual values; up to 12 local masks; three OpenGL Head/Upper/Lower partial alpha composites shown where eligible; footer reports applied, masks, missing and unsupported. NO AgentSetAppearance or actual avatar skin mutation.

Tests: firestorm_tga_mask_tests and existing Firestorm XML/UI regression, whole CTest/Python, RTX 4070 SUPER OpenGL 3.3 GUI smoke, strict server SHA and GitHub triple-SHA sync before release.

Next Alpha120-123: complete original Firestorm LLTexLayer fixed/global colors, multiple param/masks/multiply/domain, pixel-accurate 11 baked outputs and UV, asset upload/cache and authorized real-grid appearance readback only after verified backend; full transactional user Wear/Unwear remains pending.
