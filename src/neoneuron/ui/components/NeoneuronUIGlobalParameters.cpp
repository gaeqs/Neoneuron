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

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>
#include <neoneuron/ui/components/NeoneuronUIGlobalParameters.h>
#include <neoneuron/ui/style/MaterialSymbols.h>

namespace neoneuron
{
    NeoneuronUIGlobalParameters::NeoneuronUIGlobalParameters(NeoneuronApplication* application) :
        ToolWindow(application, NeoneuronFiles::SETTINGS_TOOL_GLOBAL_PARAMETERS, ICON_MS_GLOBE "Global parameters")
    {
    }

    void NeoneuronUIGlobalParameters::drawWindow()
    {
        auto& d = getNeoneuronApplication()->getRender().getRenderData();

        ImGui::PushItemWidth(ImGui::GetFontSize() * -16);

        ImGui::SliderFloat("Radius strength", &d.radiusStrength, 0.01f, 3.0f);
        ImGui::SliderFloat("Soma radius strength", &d.somaRadiusStrength, 0.01f, 3.0f);
        ImGui::SliderFloat("Start clip", &d.startClip, 0.01f, 0.5f);
        ImGui::SliderFloat("End clip", &d.endClip, 0.01f, 0.5f);
        ImGui::SliderFloat("Split height", &d.splitHeight, 0.01f, 0.5f);
        ImGui::SliderFloat("Split arc strength", &d.splitArcStrength, 0.01f, 0.5f);

        int aux = static_cast<int>(d.rotationIndexOffset);
        ImGui::SliderInt("Rotation index offset", &aux, 0, 15);
        d.rotationIndexOffset = static_cast<uint32_t>(aux);

        aux = static_cast<int>(d.childrenRotationIndexOffset);
        ImGui::SliderInt("Children rotation index offset", &aux, 0, 15);
        d.childrenRotationIndexOffset = static_cast<uint32_t>(aux);

        aux = static_cast<int>(d.minChildrenForJoint);
        ImGui::SliderInt("Min children required to generate a joint", &aux, 2, 8);
        d.minChildrenForJoint = static_cast<uint32_t>(aux);

        aux = static_cast<int>(d.verticesPerCircle);
        ImGui::SliderInt("Vertices per circle", &aux, 1, 16);
        d.verticesPerCircle = static_cast<uint32_t>(aux);

        aux = static_cast<int>(d.somaLatitudes);
        ImGui::SliderInt("Soma latitudes", &aux, 1, 64);
        d.somaLatitudes = static_cast<uint32_t>(aux);

        aux = static_cast<int>(d.somaLongitudes);
        ImGui::SliderInt("Soma longitudes", &aux, 1, 64);
        d.somaLongitudes = static_cast<uint32_t>(aux);

        ImGui::SliderFloat("Soma connection push factor", &d.somaConnectionPushFactor, 0.01f, 0.9f);
        ImGui::SliderFloat("Soma weight power", &d.somaWeightPower, 0.2f, 5.0f);
        ImGui::SliderFloat("Soma sphere weight", &d.somaSphereWeight, 0.00f, 2.0f);
        ImGui::SliderFloat("Soma connection max weight", &d.somaConnectionMaxWeight, 0.00f, 2.0f);

        ImGui::ColorEdit3("Default color", d.defaultColor.toPointer());
        ImGui::ColorEdit3("Selected color", d.selectedColor.toPointer());

        aux = static_cast<int>(d.lod);
        ImGui::SliderInt("Level of detail", &aux, 0, 7);
        d.lod = static_cast<uint32_t>(aux);

        float hue = getNeoneuronApplication()->getRender().getUI().getColorPalette().primary.normal[0];
        if (ImGui::SliderFloat("Theme hue", &hue, 0.0f, 1.0f)) {
            getNeoneuronApplication()->getRender().getUI().setColorPalette(hue);
        }

        ImGui::PopItemWidth();
    }
} // namespace neoneuron
