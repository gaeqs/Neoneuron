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

#ifndef EDITORS_H
#define EDITORS_H
#include <any>
#include <functional>
#include <optional>
#include <string>
#include <typeindex>

namespace neoneuron
{

    using Editor = std::function<bool(const std::string&, std::any*, const std::optional<std::string>& hint)>;

    bool registerEditor(std::type_index type, Editor editor, bool override = false);

    bool unregisterEditor(std::type_index type);

    bool editorFor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

    namespace editor
    {
        bool boolEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

        bool intEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

        bool longEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

        bool floatEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

        bool doubleEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

        bool stringEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);

        bool stringEditorRaw(const std::string& name, std::string* value, const std::optional<std::string>& hint);

        bool stringVectorEditor(const std::string& name, std::any* value, const std::optional<std::string>& hint);
    } // namespace editor

} // namespace neoneuron

#endif // EDITORS_H
