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

#include "ColorUtils.h"

namespace neoneuron
{

    ImBlueprint::NodeTitleStyle standardTitleStyle(uint16_t hue)
    {
        constexpr float STANDARD_SATURATION = 0.81f;
        constexpr float STANDARD_VALUE = 0.61f;
        constexpr float STANDARD_HOVERED_VALUE = 0.71f;

        float h = static_cast<float>(hue) / 360.0f;
        float analogous = h + 0.083f;

        ImU32 color = ImColor::HSV(h, STANDARD_SATURATION, STANDARD_VALUE);
        ImU32 analogousColor = ImColor::HSV(analogous, STANDARD_SATURATION, STANDARD_VALUE);

        ImU32 hoveredColor = ImColor::HSV(h, STANDARD_SATURATION, STANDARD_HOVERED_VALUE);
        ImU32 hoveredAnalogousColor = ImColor::HSV(analogous, STANDARD_SATURATION, STANDARD_HOVERED_VALUE);

        return {
            ImBlueprint::NodeTitleStyleEntry(color, analogousColor, color, analogousColor),
            ImBlueprint::NodeTitleStyleEntry(hoveredColor, hoveredAnalogousColor, hoveredColor, hoveredAnalogousColor),
            ImBlueprint::NodeTitleStyleEntry(color, analogousColor, color, analogousColor)};
    }

    ImVec4 hslToHsv(ImVec4 hsl)
    {
        float v = hsl.z + hsl.y * std::min(hsl.z, 1.0f - hsl.z);
        float s = std::abs(v) < FLT_EPSILON ? 0.0f : 2.0f * (1.0f - hsl.z / v);
        return ImVec4(hsl.x, s, v, hsl.w);
    }

    ImVec4 hslToRgb(ImVec4 hsl)
    {
        ImVec4 hsv = hslToHsv(hsl);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hsv.x, hsv.y, hsv.z, r, g, b);
        return ImVec4(r, g, b, hsl.w);
    }

} // namespace neoneuron