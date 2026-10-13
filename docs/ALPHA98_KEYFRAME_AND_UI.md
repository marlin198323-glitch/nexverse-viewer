# Alpha.98 – originale Firestorm-Animationen und GUI

Firestorm Firestorm_7.2.4 LLKeyframeMotion.cpp/h
SHA 10bd3c9f930c76e1427ddd4ecece6cdf36b4406d.
OpenGenesisLINK server main SHA
86148202c69e1eb80f0845b82d39450da5e5d39c.

Der Decoder verarbeitet binäres LLKeyframeMotion v1.0
und Legacy v0.1 mit Priority, Duration, Loop/Ease,
Emote, Joint-Rotation/Position-Tracks. Er prüft
Little Endian, Bounds, NaN, Namen, Counts und
Zeitstempel. Er akzeptiert derzeit keine IK-Constraints;
eine ignorierte Kontaktsimulation würde Firestorm-
Parität vortäuschen. Quaternions werden normalisiert,
SLERP und lineares Positions-Sampling stehen zur Verfügung.

RenderAvatarProxy nimmt optional einen immutable validierten
FirestormKeyframeMotion Clip entgegen. Vorhandene
Keyframes verformen Original-LLM-Bones im OpenGL3.3 GPU Skinner.
mPelvis-Positionskeys werden übernommen, nicht alle
anderen Pos-Offsets. Fehlt der Clip, bleiben die
provisorischen Builtin-Posen aus Alpha.96 bestehen.
Der High20 AnimationList Decoder enthält KEINE Clipbytes.
Echter GetAsset/Transfer-Fetch aus dem NexVerse-Simulator
und Priority-/Transition-Blending werden noch benötigt.

Sichtbar im Viewer: Avatar > Animationen und
unteres Dock Animationen. Das Fenster zeigt echte
WorldModel-SelfAvatar High20 AnimIDs und signed SequenceIDs,
klickbare Zeilen plus selected Details und ehrlichen
'Clip-Asset: noch nicht geladen' Status; keine ungeprüften
Play/Stop-Befehle an den Simulator. ESC/X und responsive
ViewerShell bleiben.

Tests: firestorm_keyframe_motion_tests.cpp
Golden 1.0/0.1, Trunkierung/NaN/Constraints,
Loop/Slerp; renderer_region_transition_tests.cpp
echtes Firestorm Original LLM mit CPU Jointcurve und
3 GPU Skin Draws; viewer_shell_tests.cpp
Dropdown/Row Click; viewer_shell_gl_tests.cpp
OpenGL3.3 UI-Pixel-Zeichnung. Vollständige CTest-Suite
und Live-NexVerse Login bleiben klar getrennt.
