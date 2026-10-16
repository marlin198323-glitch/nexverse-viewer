"""Non-invasive integration test for the virtual-display GUI runner."""
from pathlib import Path
import os
import subprocess
import shutil
import tempfile
import unittest

ROOT=Path(__file__).resolve().parent.parent
RUN=ROOT/"scripts/mint-isolated-gui-smoke.sh"

class MintIsolatedGuiSmokeTests(unittest.TestCase):
    def test_uses_xvfb_exclusively_and_timeout(self):
        content=RUN.read_text()
        self.assertIn("xvfb-run -a",content)
        self.assertIn("env -u WAYLAND_DISPLAY",content)
        self.assertIn("timeout --signal=TERM",content)
        self.assertIn("refuse to use user desktop",content)
        self.assertNotIn("export DISPLAY=:0",content)

    @unittest.skipUnless(shutil.which("xvfb-run"),
                         "virtual X server unavailable on headless Pi CI")
    def test_fake_viewer_never_sees_desktop_display(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder)
            fake=root/"pretend-viewer.sh"
            version=(ROOT/"VERSION").read_text().strip()
            fake.write_text(f"""#!/usr/bin/env bash
echo "Starting OpenGenesisLINK Viewer {version}"
echo "Firestorm original skeleton bind joints=133"
echo "Firestorm ORIGINAL three-part avatar bind-pose meshes GPU-resident"
echo "NexVerse UI renderer initialized"
printf '%s' "$DISPLAY" > "$OGL_GUI_FAKE_OUTPUT"
echo "RECEIVED_DISPLAY=$DISPLAY"
sleep 20
""")
            fake.chmod(0o700)
            env=os.environ.copy()
            env["DISPLAY"]=":776"
            env["OGL_GUI_FAKE_OUTPUT"]=str(root/"display.txt")
            run=subprocess.run(["bash",str(RUN),str(fake)],
                capture_output=True,text=True,timeout=18,env=env,check=False)
            self.assertEqual(run.returncode,0,run.stderr)
            self.assertIn("OGL_GUI_ISOLATED_SUCCESS",run.stdout)
            received=(root/"display.txt").read_text()
            self.assertTrue(received.startswith(":"),received)
            self.assertNotEqual(received,":776")
            self.assertNotIn("RECEIVED_DISPLAY=:776",run.stdout)

if __name__=="__main__":
    unittest.main()
