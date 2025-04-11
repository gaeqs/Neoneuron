//
// Created by gaeqs on 2/04/25.
//

#include "NeoneuronUINodeEditor.h"

#include "nodes/DatasetNode.h"
#include "nodes/RepositoryNode.h"
#include "nodes/RepresentationNode.h"

namespace neoneuron
{

    NeoneuronUINodeEditor::NeoneuronUINodeEditor(NeoneuronApplication* application) :
        _application(application)
    {
        _editor.showMinimap(true);

        _factories.push_back(RepositoryNode::createFactory());
        _factories.push_back(RepresentationNode::createFactory());
    }

    NeoneuronUINodeEditor::~NeoneuronUINodeEditor() = default;

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
                    ImGui::EndDragDropTarget();
                }
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