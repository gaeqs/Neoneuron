#!/bin/bash
#
# Copyright (c) 2025. VG-Lab/URJC.
#
# Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
# This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License version 3.0 as published
# by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

set -e

OUT_DIR=out
BUILD_DIR=/home/gaeqs/CLionProjects/neoneuron/cmake-build-release
APPDIR=$BUILD_DIR/appdir
APP=neoneuron
LINUXDEPLOY=linuxdeploy-x86_64.AppImage

DESKTOP=$APPDIR/usr/share/applications/neoneuron.desktop
ICON=$APPDIR/usr/share/icons/hicolor/512x512/apps/neoneuron.png

cp $APPDIR/usr/share/applications/neoneuron.desktop $APPDIR/neoneuron.desktop
cp $APPDIR/usr/share/icons/hicolor/512x512/apps/neoneuron.png $APPDIR/neoneuron.png

mkdir -p $OUT_DIR
cd $OUT_DIR

LINUXDEPLOY=linuxdeploy-x86_64.AppImage

if [ ! -f "$LINUXDEPLOY" ]; then
    echo "⬇️  Downloading linuxdeploy..."
    wget -q --show-progress https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/$LINUXDEPLOY
    chmod +x "$LINUXDEPLOY"
else
    echo "✅ linuxdeploy found"
fi

NO_STRIP=1 ./$LINUXDEPLOY --appdir $APPDIR --desktop-file $DESKTOP --icon-file $ICON --output appimage