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

#include "NeoneuronUINodeEditor.h"

#include "nodes/CameraNode.h"
#include "nodes/DatasetNode.h"
#include "nodes/RepositoryNode.h"
#include "nodes/RepresentationNode.h"
#include "nodes/SelectionNode.h"
#include "nodes/SynapseRepresentationNode.h"
#include "nodes/ViewportNode.h"

namespace neoneuron
{

    void NeoneuronUINodeEditor::loadDefault()
    {
        auto repo = _editor.addNode<RepositoryNode>(_application);
        auto viewport = _editor.addNode<ViewportNode>(_application);
        auto representation = _editor.addNode<RepresentationNode>(_application);

        _editor.setNodePosition(repo, {150, 50});
        _editor.setNodePosition(viewport, {150, 150});
        _editor.setNodePosition(representation, {500, 100});

        for (auto& [name, output] : repo->getOutputs()) {
            for (auto& [iName, input] : representation->getInputs()) {
                if (name == iName) {
                    output->addLink(input.get());
                }
            }
        }

        for (auto& [name, output] : viewport->getOutputs()) {
            for (auto& [iName, input] : representation->getInputs()) {
                if (name == iName) {
                    output->addLink(input.get());
                }
            }
        }
    }

    NeoneuronUINodeEditor::NeoneuronUINodeEditor(NeoneuronApplication* application) :
        _application(application)
    {
        _editor.showMinimap(true);

        _factories.push_back(RepositoryNode::createFactory());
        _factories.push_back(RepresentationNode::createFactory());
        _factories.push_back(SynapseRepresentationNode::createFactory());
        _factories.push_back(ViewportNode::createFactory());
        _factories.push_back(CameraNode::createFactory());
        _factories.push_back(DatasetNode::createFactory());
        _factories.push_back(SelectionNode::createFactory());
    }

    NeoneuronUINodeEditor::~NeoneuronUINodeEditor() = default;

    void NeoneuronUINodeEditor::onStart()
    {
        loadDefault();
    }

    void NeoneuronUINodeEditor::onUpdate(float deltaTime)
    {
    }

    void NeoneuronUINodeEditor::onPreDraw()
    {
        if (ImGui::Begin("Scene editor")) {
            float sidebarWidth = 200.0f;

            ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, 0), ImGuiChildFlags_Border);

            for (size_t i = 0; i < _factories.size(); ++i) {
                ImGui::Button(_factories[i].getDisplayName().c_str());
                if (ImGui::BeginDragDropSource(ImGuiChildFlags_None)) {
                    ImGui::SetDragDropPayload("neoneuron:node", &i, sizeof(size_t));
                    ImGui::Text(_factories[i].getDisplayName().c_str());
                    ImGui::EndDragDropSource();
                }
            }

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("neoneuron:main", ImVec2(0, 0), ImGuiChildFlags_None);
            _editor.render();
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("neoneuron:node")) {
                    IM_ASSERT(payload->DataSize == sizeof(size_t));
                    size_t index = *static_cast<size_t*>(payload->Data);
                    if (_factories.size() > index) {
                        auto& factory = _factories[index];
                        auto* node = factory.create(_application, _editor);
                        if (node != nullptr) {
                            _editor.setNodeScreenPosition(node, ImGui::GetMousePos());
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::EndChild();
        }
        ImGui::End();
    }

    void NeoneuronUINodeEditor::addFactory(NodeFactory factory)
    {
        _factories.push_back(std::move(factory));
    }
} // namespace neoneuron