//
// Created by gaeqs on 29/11/24.
//

#include "NeoneuronUIGlobalParameters.h"

#include <neoneuron/render/complex/ComplexNeuronScene.h>

namespace neoneuron {
    NeoneuronUIGlobalParameters::NeoneuronUIGlobalParameters(NeoneuronRender* render)
        : _render(render) {}

    void NeoneuronUIGlobalParameters::onPreDraw() {
        auto& d = _render->getRenderData();
        auto* scene = _render->getNeuronScene().get();
        if (ImGui::Begin("Global parameters")) {
            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

            ImGui::SliderFloat("Radius strength", &d.radiusStrength, 0.01f, 3.0f);
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
            ImGui::SliderInt("Min children required to generate a joint", &aux, 1, 8);
            d.minChildrenForJoint = static_cast<uint32_t>(aux);

            aux = static_cast<int>(d.verticesPerCircle);
            ImGui::SliderInt("Vertices per circle", &aux, 1, 16);
            d.verticesPerCircle = static_cast<uint32_t>(aux);

            if (auto* complexScene = dynamic_cast<ComplexNeuronScene*>(scene)) {
                bool wireframe = complexScene->isWireframeMode();
                ImGui::Checkbox("Wireframe", &wireframe);
                complexScene->setWireframeMode(wireframe);
            }

            ImGui::PopItemWidth();
        }
        ImGui::End();
    }
}
