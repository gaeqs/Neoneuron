//
// Created by gaeqs on 11/11/2024.
//

#ifndef IMGUICUSTOMCOMPONENTS_H
#define IMGUICUSTOMCOMPONENTS_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace neoneuron
{
    void ImGuiTitle(const char* label, ImVec2 size);

    bool collapsingHeaderWithCloseButton(const char* label, bool& closed);
} // namespace neoneuron

#endif //IMGUICUSTOMCOMPONENTS_H
