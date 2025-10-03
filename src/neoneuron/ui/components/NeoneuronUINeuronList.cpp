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

#include "NeoneuronUINeuronList.h"

#include <mindset/DefaultProperties.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/imgui/ImGuiCustomComponents.h>
#include <neoneuron/ui/style/MaterialSymbols.h>

namespace neoneuron
{
    bool NeoneuronUINeuronList::neuronSection(GID gid, mindset::Dataset* dataset, const mindset::Neuron* neuron,
                                              size_t id, bool selected) const
    {
        std::string name;

        auto prop = dataset->getProperties().getPropertyUID(mindset::PROPERTY_NAME);
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
            auto& selector = getNeoneuronApplication()->getSelector();
            if (!control) {
                selector.selectNeuron(SelectionMode::OVERRIDE_ALL, gid);
            } else {
                if (selected) {
                    selector.deselectNeuron(gid);
                } else {
                    selector.selectNeuron(SelectionMode::APPEND, gid);
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
                deleted = dataset->removeNeuron(neuron->getUID());
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Duplicate")) {
                mindset::Neuron copy(*neuron);
                copy.setUID(dataset->findSmallestAvailableNeuronUID());
                auto prop = dataset->getProperties().defineProperty(mindset::PROPERTY_NAME);
                copy.setProperty(prop, name + " (copy)");
                dataset->addNeuron(std::move(copy));
            }
            ImGui::EndPopup();
        }

        return deleted;
    }

    void NeoneuronUINeuronList::neuronList()
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        ImGui::BeginChild("List", ImVec2(0, ImGui::GetContentRegionAvail().y / 3.0f), ImGuiChildFlags_Borders);

        auto& repo = getNeoneuronApplication()->getRepository();
        auto& selectedNeurons = getNeoneuronApplication()->getSelector().getSelectedNeurons();

        std::vector<std::tuple<GID, mindset::Dataset*, mindset::Neuron*>> neurons;
        for (auto [uid, dataset] : repo.getDatasets()) {
            for (auto neuron : dataset->getDataset().getNonContextualizedNeurons()) {
                neurons.emplace_back(GID(uid, neuron->getUID()), &dataset->getDataset(), neuron);
            }
        }

        ImGuiListClipper clipper;
        clipper.Begin(neurons.size());

        bool deleted = false;
        while (!deleted && clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                auto& [gid, dataset, neuron] = neurons.at(row);
                if (neuronSection(gid, dataset, neuron, row, selectedNeurons.contains(gid))) {
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

        auto& selectedNeurons = getNeoneuronApplication()->getSelector().getSelectedNeurons();

        if (selectedNeurons.size() != 1) {
            ImGui::EndChild();
            return;
        }

        auto selectedNeuron = *selectedNeurons.begin();

        auto optionalNeuron = getNeoneuronApplication()->getRepository().getNeuronAndDataset(selectedNeuron);

        if (!optionalNeuron.has_value()) {
            ImGui::EndChild();
            return;
        }

        auto [dataset, neuron] = *optionalNeuron;
        auto morph = neuron->getMorphology();

        ImGui::Text("Id: %d-%d", dataset->getUID(), neuron->getUID());
        ImGui::Text("Sections: %d", morph.has_value() ? morph.value()->getNeuritesAmount() : 0);
        ImGui::Text("Properties provided:");

        auto& storage = getNeoneuronApplication()->getPropertyStorage();

        std::vector<std::string> segmentProperties;
        std::vector<std::string> undefinedProperties;

        auto& props = dataset->getDataset().getProperties();
        for (const auto& [name, uid] : props.getPropertiesUIDs()) {
            auto optionalValue = neuron->getPropertyAsAny(uid);
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

            auto& render = getNeoneuronApplication()->getRender();
            bool closed = false;
            if (collapsingHeaderWithCloseButton(prop.value()->getDisplayName().c_str(), closed)) {
                ImGui::Indent();
                if (auto editor = prop.value()->getEditor(); editor != nullptr) {
                    auto value = optionalValue.value();
                    if (editor(&value, neuron)) {
                        neuron->setProperty(uid, value);
                        render.refreshNeuronProperty(selectedNeuron, name);
                    }
                }
                ImGui::Unindent();
            }
            if (closed) {
                neuron->deleteProperty(uid);
                render.refreshNeuronProperty(selectedNeuron, name);
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

        neuronNewProperty(selectedNeuron, &dataset->getDataset(), neuron);

        ImGui::EndChild();
    }

    void NeoneuronUINeuronList::neuronNewProperty(GID gid, mindset::Dataset* dataset, mindset::Neuron* prototype) const
    {
        static int selectedProperty = -1;

        int amount = 0;
        const char* names[100];
        const DefinedProperty* properties[100];

        auto& storage = getNeoneuronApplication()->getPropertyStorage();
        for (auto& [name, prop] : storage) {
            auto uid = dataset->getProperties().getPropertyUID(name);

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
                auto val = prop->getGenerator()(prototype);
                auto uid = dataset->getProperties().defineProperty(prop->getName());
                prototype->setProperty(uid, val);
                getNeoneuronApplication()->getRender().refreshNeuronProperty(gid, prop->getName());
            }
            ImGui::EndDisabled();
        }
    }

    void NeoneuronUINeuronList::drawWindowContents()
    {
        neuronList();
        neuronInformation();
    }

    NeoneuronUINeuronList::NeoneuronUINeuronList(NeoneuronApplication* application) :
        ToolWindow(application, NeoneuronFiles::SETTINGS_TOOL_NEURON_LIST, ICON_MS_NEUROLOGY "Neurons")
    {
    }
} // namespace neoneuron
