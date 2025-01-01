//
// Created by gaeqs on 11/11/2024.
//

#include "ImGuiCustomComponents.h"

#include <imgui_internal.h>

namespace neoneuron {
    void ImGuiTitle(const char* label, ImVec2 size_) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize(size_, label_size.x + style.FramePadding.x * 2.0f,
                                          label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return;

        // Render
        const ImU32 col = ImGui::GetColorU32(ImGuiCol_Button);
        ImGui::RenderNavHighlight(bb, id);
        ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

        if (g.LogEnabled)
            ImGui::LogSetNextTextDecoration("[", "]");
        ImGui::RenderTextClipped(bb.Min + style.FramePadding,
                                 bb.Max - style.FramePadding,
                                 label, NULL, &label_size,
                                 ImVec2(0, 0.5f), &bb);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    }

    bool collapsingHeaderWithCloseButton(const char* label, bool& closed) {
        ImGui::PushID(label);
        bool open = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize(" X ").x - ImGui::GetStyle().FramePadding.x * 2);
        if (ImGui::Button(" X ", ImVec2(0.0f, 0.0f))) {
            closed = true;
        }
        ImGui::PopID();
        return open;
    }
}
