#!/usr/bin/env python3

#  Copyright (c) 2025. VG-Lab/URJC.
#
#  Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
#  This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License version 3.0 as published
#  by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#  Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
#  This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License version 3.0 as published
#  by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#  Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
#  This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License version 3.0 as published
#  by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#  Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
#  This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License version 3.0 as published
#  by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#  Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
#  This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License version 3.0 as published
#  by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#  Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
#  This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License version 3.0 as published
#  by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

import subprocess
from pathlib import Path
import shutil
import sys

APP = "neoneuron"
BUILD_DIR = Path("cmake-build-release")
APPDIR = BUILD_DIR / "appdir"
DIST_DIR = BUILD_DIR / "dist"
TOOLS_DIR = Path("tools")
LINUXDEPLOY = TOOLS_DIR / "linuxdeploy-x86_64.AppImage"


def run(command, **kwargs):
    print(f"🛠️  Running: {' '.join(str(arg) for arg in command)}")
    subprocess.run(command, check=True, **kwargs)


def check_required_files():
    required = [
        APPDIR / "usr" / "bin" / APP,
        APPDIR / "usr" / "share" / "applications" / f"{APP}.desktop",
        APPDIR / "usr" / "share" / "icons" / "hicolor" / "512x512" / "apps" / f"{APP}.png",
    ]
    for f in required:
        if not f.exists():
            print(f"❌ Missing required file: {f}")
            sys.exit(1)


def ensure_apprun():
    apprun = APPDIR / "AppRun"
    if not apprun.exists():
        print("➕ Creating AppRun...")
        apprun.write_text(f"""#!/bin/bash
HERE="$(dirname "$(readlink -f "$0")")"
exec "$HERE/usr/bin/{APP}" "$@"
""")
        apprun.chmod(0o755)


def copy_metadata_files():
    desktop_src = APPDIR / "usr" / "share" / "applications" / f"{APP}.desktop"
    desktop_dst = APPDIR / f"{APP}.desktop"
    if not desktop_dst.exists():
        shutil.copy2(desktop_src, desktop_dst)

    icon_src = APPDIR / "usr" / "share" / "icons" / "hicolor" / "512x512" / "apps" / f"{APP}.png"
    icon_dst = APPDIR / f"{APP}.png"
    if not icon_dst.exists():
        shutil.copy2(icon_src, icon_dst)


def main():
    print("📦 Building and installing the project...")
    run(["cmake", "--build", str(BUILD_DIR), "--target", "install"])

    check_required_files()
    ensure_apprun()
    copy_metadata_files()

    if not LINUXDEPLOY.exists():
        print(f"⬇️  Downloading linuxdeploy to {LINUXDEPLOY}...")
        TOOLS_DIR.mkdir(exist_ok=True)
        url = "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
        run(["wget", url, "-O", str(LINUXDEPLOY)])
        LINUXDEPLOY.chmod(0o755)

    print("🚀 Running linuxdeploy...")
    env = dict(NO_STRIP="1", **dict(os.environ))
    run([
        str(LINUXDEPLOY),
        "--appdir", str(APPDIR),
        "--desktop-file", str(APPDIR / f"{APP}.desktop"),
        "--icon-file", str(APPDIR / f"{APP}.png"),
        "--output", "appimage"
    ], env=env)

    DIST_DIR.mkdir(exist_ok=True)
    for appimage in BUILD_DIR.glob("*.AppImage"):
        dest = DIST_DIR / appimage.name
        shutil.move(str(appimage), dest)
        print(f"✅ AppImage created: {dest}")


if __name__ == "__main__":
    import os

    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"❌ Error while running: {e.cmd}")
        sys.exit(1)
