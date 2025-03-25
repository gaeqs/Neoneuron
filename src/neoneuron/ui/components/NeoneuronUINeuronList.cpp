//
// Created by gaeqs on 11/11/2024.
//

#include "NeoneuronUINeuronList.h"

#include <mnemea/DefaultProperties.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/imgui/ImGuiCustomComponents.h>

namespace neoneuron
{
    bool NeoneuronUINeuronList::neuronSection(const mnemea::Neuron* neuron, size_t id, bool selected) const
    {
        std::string name;

        auto prop = _render->getNeuronScene()->getDataset().getProperties().getPropertyUID(mnemea::PROPERTY_NAME);
        if (prop.has_value()) {
            if (auto opt = neuron->getProperty<std::string>(prop.value()); opt.has_value()) {
                name = opt.value();
            } else {
                name = "Neuron " + std::to_string(id);
            }
        } else {
            name = "Neuron " + std::to_string(id);
        }

        std::string taggedName = name + "##" + std::to_string(id);

        bool deleted = false;
        if (selected) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 1, 0, 1));
        }
        if (ImGui::Button(taggedName.c_str(), ImVec2(-1.0f, 25.0f))) {
            bool control = ImGui::IsKeyDown(ImGuiKey_ModCtrl);
            auto& selector = _render->getNeuronScene()->getSelector();
            if (!control) {
                selector.clearSelection();
                selector.selectNeuron(neuron->getUID());
            } else {
                if (selected) {
                    selector.deselectNeuron(neuron->getUID());
                } else {
                    selector.selectNeuron(neuron->getUID());
                }
            }
        }
        if (selected) {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }

        if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
        {
            ImGui::Text(name.c_str());
            ImGui::Separator();
            if (ImGui::MenuItem("Delete")) {
                auto& scene = _render->getNeuronScene();
                deleted = scene->removeNeuron(neuron->getUID());
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Duplicate")) {
                mnemea::Neuron copy(*neuron);
                copy.setUID(_render->getNeuronScene()->findAvailableUID());
                auto prop =
                    _render->getNeuronScene()->getDataset().getProperties().defineProperty(mnemea::PROPERTY_NAME);
                copy.setPropertyAsAny(prop, name + " (copy)");
                _render->getNeuronScene()->addNeuron(std::move(copy));
            }
            ImGui::EndPopup();
        }

        return deleted;
    }

    void NeoneuronUINeuronList::neuronList()
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        ImGui::BeginChild("List", ImVec2(0, ImGui::GetContentRegionAvail().y / 3.0f), ImGuiChildFlags_Borders);

        auto* scene = _render->getNeuronScene().get();
        auto& selectedNeurons = scene->getSelector().getSelectedNeurons();

        std::vector<mnemea::Neuron*> neurons;
        for (auto& neuron : scene->getDataset().getNeurons()) {
            neurons.push_back(&neuron);
        }

        ImGuiListClipper clipper;
        clipper.Begin(neurons.size());

        bool deleted = false;
        while (!deleted && clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                auto* neuron = neurons.at(row);
                if (neuronSection(neuron, row, selectedNeurons.contains(neuron->getUID()))) {
                    deleted = true;
                    break;
                }
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void NeoneuronUINeuronList::neuronInformation() const
    {
        ImGui::BeginChild("Information", ImVec2(0, ImGui::GetContentRegionAvail().y));

        auto& selectedNeurons = _render->getNeuronScene()->getSelector().getSelectedNeurons();

        if (selectedNeurons.size() != 1) {
            ImGui::EndChild();
            return;
        }

        auto selectedNeuron = *selectedNeurons.begin();

        auto* scene = _render->getNeuronScene().get();
        auto* prototype = scene->findPrototypeNeuron(selectedNeuron).value_or(nullptr);

        if (prototype == nullptr) {
            ImGui::EndChild();
            return;
        }

        auto morph = prototype->getMorphology();

        ImGui::Text("Id: %d", prototype->getUID());
        ImGui::Text("Sections: %d", morph.has_value() ? morph.value()->getNeuritesAmount() : 0);
        ImGui::Text("Properties provided:");

        auto& storage = _render->getNeoneuronApplication()->getPropertyStorage();

        std::vector<std::string> segmentProperties;
        std::vector<std::string> undefinedProperties;

        auto& props = _render->getNeuronScene()->getDataset().getProperties();
        for (const auto& [name, uid] : props.getPropertiesUIDs()) {
            auto optionalValue = prototype->getPropertyAsAny(uid);
            if (!optionalValue.has_value()) {
                continue;
            }
            auto prop = storage.get(name);
            if (!prop.has_value()) {
                undefinedProperties.push_back(name);
                continue;
            }

            if (prop.value()->getTarget() == PropertyTarget::SEGMENT) {
                segmentProperties.push_back(name);
                continue;
            }

            bool closed = false;
            if (collapsingHeaderWithCloseButton(prop.value()->getDisplayName().c_str(), closed)) {
                ImGui::Indent();
                if (auto editor = prop.value()->getEditor(); editor != nullptr) {
                    auto value = optionalValue.value();
                    if (editor(&value, prototype, scene)) {
                        prototype->setPropertyAsAny(uid, value);
                        scene->refreshNeuronProperty(selectedNeuron, name);
                    }
                }
                ImGui::Unindent();
            }
            if (closed) {
                prototype->deleteProperty(uid);
                scene->refreshNeuronProperty(selectedNeuron, name);
            }
        }

        if (!segmentProperties.empty() && ImGui::CollapsingHeader("Segment properties")) {
            ImGui::Indent();
            for (const auto& prop : segmentProperties) {
                ImGui::Text(prop.c_str());
            }
            ImGui::Unindent();
        }
        if (!undefinedProperties.empty() && ImGui::CollapsingHeader("Undefined properties")) {
            ImGui::Indent();
            for (const auto& prop : undefinedProperties) {
                ImGui::Text(prop.c_str());
            }
            ImGui::Unindent();
        }

        neuronNewProperty(prototype);

        ImGui::EndChild();
    }

    void NeoneuronUINeuronList::neuronNewProperty(mnemea::Neuron* prototype) const
    {
        static int selectedProperty = -1;

        int amount = 0;
        const char* names[100];
        const DefinedProperty* properties[100];

        auto& storage = _render->getNeoneuronApplication()->getPropertyStorage();
        for (auto& [name, prop] : storage) {
            auto uid = _render->getNeuronScene()->getDataset().getProperties().getPropertyUID(name);

            if (prop.getGenerator() != nullptr && prop.getTarget() != PropertyTarget::SEGMENT &&
                (!uid.has_value() || !prototype->getPropertyAsAny(uid.value()).has_value())) {
                properties[amount] = &prop;
                names[amount++] = prop.getDisplayName().c_str();
                if (amount >= 100) {
                    break;
                }
            }
        }

        selectedProperty = std::min(selectedProperty, amount - 1);
        if (amount > 0) {
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("New property:");
            ImGui::Combo("##property", &selectedProperty, names, amount);
            ImGui::PopItemWidth();
            ImGui::BeginDisabled(selectedProperty < 0);
            ImGui::SameLine();
            if (ImGui::Button("Add property", ImVec2(-1.0f, 0.0f))) {
                auto* prop = properties[selectedProperty];
                auto val = prop->getGenerator()(prototype, _render->getNeuronScene().get());
                auto uid = _render->getNeuronScene()->getDataset().getProperties().defineProperty(prop->getName());
                prototype->setPropertyAsAny(uid, val);
                _render->getNeuronScene()->refreshNeuronProperty(prototype->getUID(), prop->getName());
            }
            ImGui::EndDisabled();
        }
    }

    NeoneuronUINeuronList::NeoneuronUINeuronList(NeoneuronRender* render) :
        _render(render)
    {
    }

    void NeoneuronUINeuronList::onPreDraw()
    {
        if (ImGui::Begin("Neurons")) {
            neuronList();
            neuronInformation();
        }
        ImGui::End();
    }
} // namespace neoneuron
