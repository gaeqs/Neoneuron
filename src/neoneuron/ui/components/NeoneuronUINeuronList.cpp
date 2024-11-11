//
// Created by gaeqs on 11/11/2024.
//

#include "NeoneuronUINeuronList.h"


namespace neoneuron {
    NeoneuronUINeuronList::NeoneuronUINeuronList(NeoneuronRender* render) : _render(render) {}

    void NeoneuronUINeuronList::onPreDraw() {
        if (ImGui::Begin("Neurons")) {
            auto& neurons = _render->getNeuronScene().getNeurons();

            ImGuiListClipper clipper;
            clipper.Begin(neurons.size());

            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                    auto& neuron = neurons[row];
                    std::string name = "Neuron" + std::to_string(neuron.getId()) + "##" + std::to_string(row);
                    if (ImGui::CollapsingHeader(name.c_str())) {
                        ImGui::Text(name.c_str());
                    }
                }
            }
        }
        ImGui::End();
    }
}
