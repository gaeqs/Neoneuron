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
    inline void setupCommonStyleParameters()
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
    }

    inline void StyleColorsDark(const DynamicColorPalette& palette)
    {
        setupCommonStyleParameters();
        ImGuiStyle* style = &ImGui::GetStyle();

        ImVec4* colors = style->Colors;
        colors[ImGuiCol_Text] = ImVec4(0.93f, 0.93f, 0.94f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_ChildBg] = palette.surfaceContainer.darkest.toImGuiVector();
        colors[ImGuiCol_PopupBg] = palette.surfaceContainer.light.toImGuiVector();
        colors[ImGuiCol_Border] = palette.primary.darkest.toImGuiVector();
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_FrameBgHovered] = palette.surfaceContainer.normal.toImGuiVector();
        colors[ImGuiCol_FrameBgActive] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_MenuBarBg] = palette.surfaceContainer.light.toImGuiVector();
        colors[ImGuiCol_ScrollbarBg] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_ScrollbarGrab] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_ScrollbarGrabHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_ScrollbarGrabActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_CheckMark] = palette.primary.normal.toImGuiVector();
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
        colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabDimmed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_DockingPreview] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_PlotLines] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_PlotLinesHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_PlotHistogram] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_PlotHistogramHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_TableHeaderBg] = palette.surfaceContainer.normal.toImGuiVector();
        colors[ImGuiCol_TableBorderStrong] = palette.surfaceContainer.light.toImGuiVector();
        colors[ImGuiCol_TableBorderLight] = palette.surfaceContainer.lighter.toImGuiVector();
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.29f, 0.62f, 0.91f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.29f, 0.62f, 0.91f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.29f, 0.62f, 0.91f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        colors[ImGuiCol_Border] = ImVec4(0.13f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TextLink] = palette.primary.light.toImGuiVector();
    }

    inline void StyleColorsLight(const DynamicColorPalette& palette)
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        ImGui::StyleColorsLight(style);
        setupCommonStyleParameters();

        ImVec4* colors = style->Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_ChildBg] = palette.surfaceContainer.darkest.toImGuiVector();
        colors[ImGuiCol_PopupBg] = palette.surfaceContainer.light.toImGuiVector();
        colors[ImGuiCol_Border] = palette.primary.darkest.toImGuiVector();
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_FrameBgHovered] = palette.surfaceContainer.normal.toImGuiVector();
        colors[ImGuiCol_FrameBgActive] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_MenuBarBg] = palette.surfaceContainer.light.toImGuiVector();
        colors[ImGuiCol_ScrollbarBg] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_ScrollbarGrab] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_ScrollbarGrabHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_ScrollbarGrabActive] = palette.primary.dark.toImGuiVector();
        colors[ImGuiCol_CheckMark] = palette.primary.normal.toImGuiVector();
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
        colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabDimmed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_DockingPreview] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_PlotLines] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_PlotLinesHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_PlotHistogram] = palette.primary.normal.toImGuiVector();
        colors[ImGuiCol_PlotHistogramHovered] = palette.primary.light.toImGuiVector();
        colors[ImGuiCol_TableHeaderBg] = palette.surfaceContainer.normal.toImGuiVector();
        colors[ImGuiCol_TableBorderStrong] = palette.surfaceContainer.light.toImGuiVector();
        colors[ImGuiCol_TableBorderLight] = palette.surfaceContainer.lighter.toImGuiVector();
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = palette.surfaceContainer.dark.toImGuiVector();
        colors[ImGuiCol_Border] = ImVec4(0.13f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TextLink] = palette.primary.light.toImGuiVector();
    }
} // namespace neoneuron

#endif // LIGHTTHEME_H
