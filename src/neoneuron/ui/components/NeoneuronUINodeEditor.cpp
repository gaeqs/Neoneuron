//
// Created by gaeqs on 2/04/25.
//

#include "NeoneuronUINodeEditor.h"

namespace
{

    class TestNode : public ImBlueprint::Node
    {
      public:
        TestNode() :
            Node("Test node")
        {
            defineInput<int>("Hello!");
            defineInput<int>("Hello1!");
            defineInput<int>("Hello2!");
            defineInput<int>("Hello3!");
            defineInput<int>("Hello4!");
            //defineOutput<int>("World");
        }

        void renderBody() override
        {
            ImGui::Dummy(ImVec2(50, 20));
        }
    };

} // namespace

namespace neoneuron
{

    NeoneuronUINodeEditor::NeoneuronUINodeEditor()
    {
        _editor.showMinimap(true);
    }

    NeoneuronUINodeEditor::~NeoneuronUINodeEditor()
    {
    }

    void NeoneuronUINodeEditor::onUpdate(float deltaTime)
    {
    }

    void NeoneuronUINodeEditor::onPreDraw()
    {
        ImGui::Begin("Container Window");

        float sidebarWidth = 200.0f;

        ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, 0), ImGuiChildFlags_Border);
        ImGui::Text("Sidebar Content");
        ImGui::Button("Button 1");
        if (ImGui::BeginDragDropSource(ImGuiChildFlags_None)) {
            int i = 10;
            ImGui::SetDragDropPayload("neoneuron:node", &i, sizeof(int));
            ImGui::Text("Button 1...");
            ImGui::EndDragDropSource();
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Main", ImVec2(0, 0), ImGuiChildFlags_None);
        _editor.render();
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("neoneuron:node")) {
                IM_ASSERT(payload->DataSize == sizeof(int));
                neon::debug() << "OWO";
                auto node = _editor.addNode<TestNode>();
                neon::debug() << node;
                auto pos = _editor.getNodePosition(node);
                neon::debug() << pos.x << " - " << pos.y;
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace neoneuron