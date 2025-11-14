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
// Created by gaeqs on 30/10/25.
//

#ifndef NEONEURON_COLORUTILS_H
#define NEONEURON_COLORUTILS_H
#include "imgui.h"

#include <imblueprint/Node.h>

namespace neoneuron
{

    ImBlueprint::NodeTitleStyle standardTitleStyle(uint16_t hue);

    /**
     * Transforms a color in the HSL color space into a color in the HSV space.
     * The vector format is the following:
     * - x: hue
     * - y: saturation
     * - z: light / value
     * - w: alpha
     * @param hsl the color in the HSL color space.
     * @return the color in the HSV color space.
     */
    ImVec4 hslToHsv(ImVec4 hsl);

    /**
     * Transforms a color in the HSL space into a color in the RGB space.
     * The vector format is the following:
     * - x: hue / R
     * - y: saturation / G
     * - z: light / B
     * - w: alpha
     * @param hsl the color in the HSL color space.
     * @return the color in the RGB color space.
     */
    ImVec4 hslToRgb(ImVec4 hsl);

} // namespace neoneuron

#endif // NEONEURON_COLORUTILS_H
