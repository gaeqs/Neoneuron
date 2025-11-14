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

#include "neoneuron/util/ColorUtils.h"

namespace neoneuron
{
    ColorRole::ColorRole() :
        ColorRole(0.0f, 0.48f)
    {
    }

    ColorRole::ColorRole(float hue, float saturation) :
        normal(hslToRgb({hue, saturation, 0.5f, 1.0f})),
        light(hslToRgb({hue, saturation, 0.6f, 1.0f})),
        lighter(hslToRgb({hue, saturation, 0.7f, 1.0f})),
        dark(hslToRgb({hue, saturation, 0.4f, 1.0f})),
        darker(hslToRgb({hue, saturation, 0.3f, 1.0f}))
    {
    }

    DynamicColorPalette::DynamicColorPalette() :
        DynamicColorPalette(0.0f)
    {
    }

    DynamicColorPalette::DynamicColorPalette(float hue) :
        primary(hue, 0.48f),
        secondary(hue, 0.16f),
        tertiary(hue + 0.166f, 0.48f),
        surface(hslToRgb({hue, 0.02f, 0.10f, 1.0f})),
        surfaceTint(hslToRgb({hue, 0.5f, 0.22f, 1.0f}))
    {
    }
} // namespace neoneuron
