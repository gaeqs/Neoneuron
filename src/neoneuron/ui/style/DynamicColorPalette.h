// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by gaeqs on 14/11/25.
//

#ifndef NEONEURON_DYNAMICCOLORPALETTE_H
#define NEONEURON_DYNAMICCOLORPALETTE_H

#include <neon/color/Color.h>

#include "imgui.h"

namespace neoneuron
{

    struct ColorRole
    {
        neon::Color<> normal;
        neon::Color<> light;
        neon::Color<> lighter;
        neon::Color<> dark;
        neon::Color<> darker;
        neon::Color<> darkest;

        ColorRole();

        ColorRole(neon::Color<neon::HSLAColorSpace> base);
    };

    struct DynamicColorPalette
    {
        ColorRole primary;
        ColorRole secondary;
        ColorRole tertiary;
        ColorRole surfaceContainer;
        ColorRole primaryAlt;
        neon::Color<> surface;
        neon::Color<> surfaceTint;

        DynamicColorPalette();

        explicit DynamicColorPalette(float hue);
    };
} // namespace neoneuron

#endif // NEONEURON_DYNAMICCOLORPALETTE_H
