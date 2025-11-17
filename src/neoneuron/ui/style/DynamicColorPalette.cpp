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

#include "DynamicColorPalette.h"

namespace neoneuron
{
    ColorRole::ColorRole() :
        ColorRole(neon::Color<neon::HSLAColorSpace>(0.0f, 0.48f, 0.5f, 1.0f))
    {
    }

    ColorRole::ColorRole(neon::Color<neon::HSLAColorSpace> base) :
        normal(base),
        light(base.lighter(0.05f)),
        lighter(base.lighter(0.2f)),
        dark(base.darker(0.05f)),
        darker(base.darker(0.2f)),
        darkest(base.darker(0.3f))
    {
    }

    void ColorRole::alternateTheme()
    {
        normal[2] = 1.0f - normal[2];
        light[2] = 1.0f - light[2];
        lighter[2] = 1.0f - lighter[2];
        dark[2] = 1.0f - dark[2];
        darker[2] = 1.0f - darker[2];
        darkest[2] = 1.0f - darkest[2];
    }

    DynamicColorPalette::DynamicColorPalette() :
        DynamicColorPalette(0.0f, false)
    {
    }

    DynamicColorPalette::DynamicColorPalette(float hue, bool lightTheme) :
        primary({hue, 0.48f, 0.35f, 1.0f}),
        secondary({hue, 0.16f, 0.35f, 1.0f}),
        tertiary({hue + 0.166f, 0.48f, 0.35f, 1.0f}),
        surfaceContainer({hue, 0.10f, 0.11f, 1.0f}),
        primaryAlt({hue + 0.05f, 0.48f, 0.35f, 1.0f}),
        surface(neon::Color<neon::HSLAColorSpace>(hue, 0.02f, 0.00f, 1.0f)),
        surfaceTint(neon::Color<neon::HSLAColorSpace>(hue, 0.5f, 0.15f, 1.0f))
    {
        if (lightTheme) {
            alternateTheme();
        }
    }

    void DynamicColorPalette::alternateTheme()
    {
        surface[2] = 1.0f - surface[2];
        surfaceTint[2] = 1.0f - surfaceTint[2];
        primary.alternateTheme();
        secondary.alternateTheme();
        tertiary.alternateTheme();
        surfaceContainer.alternateTheme();
        primaryAlt.alternateTheme();
    }
} // namespace neoneuron
