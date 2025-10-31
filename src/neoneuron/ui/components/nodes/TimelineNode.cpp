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

#include <neoneuron/render/component/camera/CameraController.h>
#include <neoneuron/ui/components/nodes/TimelineNode.h>
#include <neoneuron/ui/style/MaterialSymbols.h>

CMRC_DECLARE(resources);

namespace neoneuron
{

    TimelineNode::TimelineNode(NeoneuronApplication* application) :
        NeoneuronNode("Timeline", ICON_MS_TIMELINE),
        _application(application),
        _timeline(application->getRender().addTimeline())
    {
        defineOutput<Timeline*>("Timeline", _timeline);
    }

    TimelineNode::~TimelineNode()
    {
        sendOutput("Timeline", std::any());
        _application->getRender().removeTimeline(_timeline);
    }

    void TimelineNode::onInputChange(const std::string& name, const std::any& value)
    {
    }

    NodeFactory TimelineNode::createFactory()
    {
        return NodeFactory("Timeline",
                           [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<TimelineNode>(app); });
    }
} // namespace neoneuron