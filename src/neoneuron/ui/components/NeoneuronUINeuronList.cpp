//
// Created by gaeqs on 11/11/2024.
//

#include "NeoneuronUINeuronList.h"

#include <neoneuron/render/simple/SimpleNeuronScene.h>
#include <neoneuron/ui/imgui/ImGuiCustomComponents.h>


namespace neoneuron {
    void NeoneuronUINeuronList::neuronSection(const PrototypeNeuron& neuron, size_t id) {
        std::string name = "Neuron " + std::to_string(id) + "##" + std::to_string(id);

        bool selected = _selectedNeuron == neuron.getId();
        if (selected) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 1, 0, 1));
        }
        if (ImGui::Button(name.c_str(), ImVec2(-1.0f, 25.0f))) {
            _selectedNeuron = neuron.getId();
        }

        if (selected) {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }
    }

    void NeoneuronUINeuronList::neuronList() {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        ImGui::BeginChild(
            "List",
            ImVec2(0, ImGui::GetContentRegionAvail().y / 3.0f),
            ImGuiChildFlags_Borders
        );

        auto* render = _render->getNeuronScene().get();
        auto& neurons = render->getPrototypeNeurons();

        ImGuiListClipper clipper;
        clipper.Begin(neurons.size());

        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                neuronSection(neurons[row], row);
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void NeoneuronUINeuronList::neuronInformation() {
        ImGui::BeginChild(
            "Information",
            ImVec2(0, ImGui::GetContentRegionAvail().y)
        );

        if (!_selectedNeuron.has_value()) {
            ImGui::EndChild();
            return;
        }

        auto* prototype = _render->getNeuronScene()->findPrototypeNeuron(_selectedNeuron.value()).value_or(nullptr);

        if (prototype == nullptr) {
            ImGui::EndChild();
            return;
        }


        ImGui::Text("Id: %d", prototype->getId());
        ImGui::Text("Sections: %d", prototype->getSegments().size());
        ImGui::Text("Properties provided:");
        ImGui::Indent();

        for (const auto& name: prototype->getProperties() | std::views::keys) {
            ImGui::Text(name.c_str());
        }

        ImGui::Unindent();

        ImGui::EndChild();
    }

    NeoneuronUINeuronList::NeoneuronUINeuronList(NeoneuronRender* render) : _render(render) {}

    void NeoneuronUINeuronList::onPreDraw() {
        static char BUFFER[32];
        if (ImGui::Begin("Neurons")) {
            ImGui::InputText("Filter", BUFFER, 32);
            neuronList();
            neuronInformation();
        }
        ImGui::End();
    }
}
