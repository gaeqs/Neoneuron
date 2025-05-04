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

#include "NodeFactory.h"

namespace neoneuron
{

    NodeFactory::NodeFactory(std::string displayName,
                             std::function<ImBlueprint::Node*(ImBlueprint::Editor&, NeoneuronApplication*)> creator) :
        _displayName(std::move(displayName)),
        _creator(std::move(creator))
    {
    }

    std::string NodeFactory::getDisplayName() const
    {
        return _displayName;
    }

    ImBlueprint::Node* NodeFactory::create(NeoneuronApplication* application, ImBlueprint::Editor& editor) const
    {
        if (_creator != nullptr) {
            return _creator(editor, application);
        }
        return nullptr;
    }
} // namespace neoneuron
