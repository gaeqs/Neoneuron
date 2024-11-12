//
// Created by gaeqs on 11/11/2024.
//

#include "NeoneuronUINeuronList.h"

#include <imgui_internal.h>
#include <neoneuron/ui/imgui/ImGuiCustomComponents.h>
#include <neoneuron/ui/style/Fonts.h>


namespace neoneuron {
    void NeoneuronUINeuronList::neuronRow(const std::vector<Neuron>& neurons, size_t row, size_t elements) {
        for (size_t i = 0; i < elements; ++i) {
            size_t id = row * elements + i;
            if (neurons.size() <= id) break;
            auto& neuron = neurons[id];
            if (i != 0) ImGui::SameLine();
            neuronSection(neuron, id);
        }
    }

    void NeoneuronUINeuronList::neuronSection(const Neuron& neuron, size_t id) const {
        std::string name = "Neuron " + std::to_string(id) + "##" + std::to_string(id);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));

        if (ImGui::BeginChild(name.c_str(), ImVec2(150.0f, ImGui::GetTextLineHeightWithSpacing() * 5),
                              ImGuiChildFlags_Borders)) {
            ImGui::PushFont(fonts::getFont(fonts::SS3_18).value_or(nullptr));
            ImGuiTitle(name.c_str(), ImVec2(-1.0f, 25.0f));
            ImGui::PopFont();

            ImGui::Text("Segments: %d", neuron.getSegments().size());
        }

        ImGui::PopStyleColor();

        ImGui::EndChild();
    }

    NeoneuronUINeuronList::NeoneuronUINeuronList(NeoneuronRender* render) : _render(render) {}

    void NeoneuronUINeuronList::onPreDraw() {
        if (ImGui::Begin("Neurons")) {
            auto& neurons = _render->getNeuronScene().getNeurons();

            int elements = static_cast<int>(ImGui::GetContentRegionAvail().x / 150.0f);
            if (elements <= 0) elements = 1;

            ImGuiListClipper clipper;
            clipper.Begin(neurons.size() / elements + (neurons.size() % elements > 0 ? 1 : 0));

            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                    neuronRow(neurons, row, elements);
                }
            }
        }
        ImGui::End();
    }
}
