//
// Created by gaeqs on 29/11/24.
//

#include "NeoneuronUIGlobalParameters.h"

namespace neoneuron {
    NeoneuronUIGlobalParameters::NeoneuronUIGlobalParameters(NeoneuronRender* render)
        : _render(render) {}

    void NeoneuronUIGlobalParameters::onPreDraw() {
        auto& d = _render->getRenderData();
        if (ImGui::Begin("Global parameters")) {
            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

            ImGui::SliderFloat("Radius strength", &d.radiusStrength, 0.01f, 3.0f);
            ImGui::SliderFloat("Start clip", &d.startClip, 0.01f, 0.5f);
            ImGui::SliderFloat("End clip", &d.endClip, 0.01f, 0.5f);
            ImGui::SliderFloat("Split height", &d.splitHeight, 0.01f, 0.5f);
            ImGui::SliderFloat("Split arc strength", &d.splitArcStrength, 0.01f, 0.5f);

            int offset = static_cast<int>(d.rotationIndexOffset);
            ImGui::SliderInt("Rotation index offset", &offset, 0, 15);
            d.rotationIndexOffset = static_cast<uint32_t>(offset);

            offset = static_cast<int>(d.childrenRotationIndexOffset);
            ImGui::SliderInt("Children rotation index offset", &offset, 0, 15);
            d.childrenRotationIndexOffset = static_cast<uint32_t>(offset);
            ImGui::PopItemWidth();
        }
        ImGui::End();
    }
}
