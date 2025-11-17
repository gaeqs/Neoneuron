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

#ifndef LIGHTTHEME_H
#define LIGHTTHEME_H

#include <imgui.h>

namespace neoneuron
{
    inline void StyleColorsDark(const DynamicColorPalette& palette)
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->WindowPadding = {8, 8};
        style->FramePadding = {8, 6};
        style->ItemSpacing = {8, 4};
        style->ItemInnerSpacing = {4, 4};
        style->TouchExtraPadding = {0, 0};
        style->IndentSpacing = 21;
        style->ScrollbarSize = 14;
        style->GrabMinSize = 12;

        style->WindowBorderSize = 0.0f;
        style->ChildBorderSize = 1.0f;
        style->PopupBorderSize = 1.0f;
        style->FrameBorderSize = 0.0f;
        style->TabBorderSize = 0.0f;
        style->TabBarBorderSize = 0.0f;
        style->TabBarOverlineSize = 0.0f;

        style->WindowRounding = 7.0f;
        style->ChildRounding = 7.0f;
        style->FrameRounding = 4.0f;
        style->PopupRounding = 4.0f;
        style->ScrollbarRounding = 2.0f;
        style->GrabRounding = 4.0f;
        style->TabRounding = 4.0f;

        style->DockingSeparatorSize = 6.0f;

        style->SeparatorTextBorderSize = 1.0f;
        style->WindowMenuButtonPosition = ImGuiDir_Right;

        ImVec4* colors = style->Colors;
        colors[ImGuiCol_Text] = ImVec4(0.93f, 0.93f, 0.94f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.76f, 0.29f, 1.00f);
        colors[ImGuiCol_SliderGrab] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_SliderGrabActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_Button] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_ButtonHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_ButtonActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_Header] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_HeaderHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_HeaderActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_Separator] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_SeparatorHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_SeparatorActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_ResizeGrip] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_ResizeGripHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_ResizeGripActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabHovered] =ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabDimmed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_DockingPreview] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.29f, 0.66f, 0.91f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.29f, 0.62f, 0.91f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.29f, 0.62f, 0.91f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.29f, 0.62f, 0.91f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        colors[ImGuiCol_Border] = ImVec4(0.13f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TextLink] = palette.primary.light.toImGuiVector();
    }

    inline void StyleColorsLight()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->WindowPadding = {8, 8};
        style->FramePadding = {4, 3};
        style->ItemSpacing = {8, 4};
        style->ItemInnerSpacing = {4, 4};
        style->TouchExtraPadding = {0, 0};
        style->IndentSpacing = 21;
        style->ScrollbarSize = 14;
        style->GrabMinSize = 12;

        style->WindowBorderSize = 0.0f;
        style->ChildBorderSize = 1.0f;
        style->PopupBorderSize = 1.0f;
        style->FrameBorderSize = 0.0f;
        style->TabBorderSize = 0.0f;
        style->TabBarBorderSize = 0.0f;
        style->TabBarOverlineSize = 0.0f;

        style->WindowRounding = 7.0f;
        style->ChildRounding = 7.0f;
        style->FrameRounding = 4.0f;
        style->PopupRounding = 4.0f;
        style->ScrollbarRounding = 2.0f;
        style->GrabRounding = 4.0f;
        style->TabRounding = 4.0f;

        style->SeparatorTextBorderSize = 1.0f;
        style->WindowMenuButtonPosition = ImGuiDir_Right;
        ImGui::StyleColorsLight(style);
    }
} // namespace neoneuron

#endif // LIGHTTHEME_H
