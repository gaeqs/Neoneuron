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

#include "Editors.h"

#include "imgui.h"
#include "neon/structure/Application.h"

#include <functional>
#include <unordered_map>

namespace
{
    int imGuiStringCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            auto* str = static_cast<std::string*>(data->UserData);
            str->resize(data->BufTextLen);
            data->Buf = str->data();
        }
        return 0;
    }

    std::unordered_map<std::type_index, neoneuron::Editor> EDITORS = {
        {                    typeid(bool),         neoneuron::editor::boolEditor},
        {                     typeid(int),          neoneuron::editor::intEditor},
        {                    typeid(long),         neoneuron::editor::longEditor},
        {                   typeid(float),        neoneuron::editor::floatEditor},
        {                  typeid(double),       neoneuron::editor::doubleEditor},
        {             typeid(std::string),       neoneuron::editor::stringEditor},
        {typeid(std::vector<std::string>), neoneuron::editor::stringVectorEditor},
    };

} // namespace

namespace neoneuron
{

    bool registerEditor(std::type_index type, Editor editor, bool override)
    {
        if (override) {
            return EDITORS.insert_or_assign(type, std::move(editor)).second;
        }
        return EDITORS.emplace(type, std::move(editor)).second;
    }

    bool unregisterEditor(std::type_index type)
    {
        return EDITORS.erase(type) > 0;
    }

    bool editorFor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
    {
        auto it = EDITORS.find(value->type());
        if (it != EDITORS.end()) {
            return it->second(name, value, hint);
        }
        return false;
    }

    namespace editor
    {

        bool boolEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<bool>(value);
            if (v == nullptr) {
                return false;
            }
            return ImGui::Checkbox(name.c_str(), v);
        }

        bool intEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<int>(value);
            if (v == nullptr) {
                return false;
            }
            return ImGui::InputInt(name.c_str(), v);
        }

        bool longEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<long>(value);
            if (v == nullptr) {
                return false;
            }
            return ImGui::InputScalar(name.c_str(), ImGuiDataType_S64, v);
        }

        bool floatEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<float>(value);
            if (v == nullptr) {
                return false;
            }
            return ImGui::InputFloat(name.c_str(), v);
        }

        bool doubleEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<double>(value);
            if (v == nullptr) {
                return false;
            }
            return ImGui::InputDouble(name.c_str(), v);
        }

        bool stringEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<std::string>(value);
            if (v == nullptr) {
                return false;
            }

            return stringEditorRaw(name, v, hint);
        }

        bool stringEditorRaw(const std::string& name, std::string* value, const std::optional<std::string>& hint)
        {
            value->reserve(1024);

            size_t buf_size = value->capacity() + 1;
            value->resize(buf_size - 1);
            char* buf = value->data();

            ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;
            bool edited = ImGui::InputText(name.c_str(), buf, buf_size, flags, imGuiStringCallback, value);
            value->resize(strlen(buf));
            return edited;
        }

        bool stringVectorEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint)
        {
            auto* v = std::any_cast<std::vector<std::string>>(value);
            if (v == nullptr) {
                return false;
            }

            bool result = false;

            ImGui::Text(name.c_str());
            ImGui::SameLine();
            if (ImGui::Button("+")) {
                v->emplace_back();
                result = true;
            }

            size_t index = 0;
            for (auto it = v->begin(); it != v->end();) {
                std::string buttonId = "x##" + std::to_string(index);
                std::string editorId = "##" + std::to_string(index++);
                bool deleted = false;
                if (ImGui::Button(buttonId.c_str())) {
                    deleted = true;
                }
                ImGui::SameLine();
                result |= stringEditorRaw(editorId, &*it, hint);
                if (deleted) {
                    result = true;
                    it = v->erase(it);
                } else {
                    ++it;
                }
            }

            return result;
        }

    } // namespace editor
} // namespace neoneuron