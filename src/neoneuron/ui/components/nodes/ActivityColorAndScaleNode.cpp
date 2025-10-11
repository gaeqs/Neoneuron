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

#include "ActivityColorAndScaleNode.h"

#include <neoneuron/ui/components/nodes/EventSequenceNode.h>
#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    ActivityColorAndScaleNode::ActivityColorAndScaleNode(NeoneuronApplication* application) :
        Node("ActivityColorAndScale"),
        _application(application),
        _activityColorAndScale(
            std::make_shared<ActivityNeuronColorAndScaleSE>(&application->getRender().getApplication())),
        _timeline(nullptr)
    {
        defineInput<Timeline*>("Timeline", false);
        defineInput<EventSequenceId>("Sequence", false);
        defineOutput<std::shared_ptr<NeuronColorAndScaleSE>>("Color and scale", _activityColorAndScale);
    }

    ActivityColorAndScaleNode::~ActivityColorAndScaleNode()
    {
        if (_timeline != nullptr) {
            _timeline->removeTimeAware(_activityColorAndScale);
        }
        sendOutput("Color and scale", std::any());
    }

    void ActivityColorAndScaleNode::onInputChange(const std::string& name, const std::any& value)
    {
        if (name == "Timeline") {
            if (_timeline != nullptr) {
                _timeline->removeTimeAware(_activityColorAndScale);
            }

            auto timeline = getInput<Timeline*>("Timeline");
            if (!timeline.has_value()) {
                return;
            }

            _timeline = *timeline;
            _timeline->addTimeAware(_activityColorAndScale);
        } else if (name == "Sequence") {
            auto sequenceId = getInput<EventSequenceId>("Sequence");
            if (!sequenceId.has_value()) {
                _activityColorAndScale->clearActivity();
                return;
            }

            auto dataset = _application->getRepository().getDataset(sequenceId->datasetId);
            if (!dataset) {
                _activityColorAndScale->clearActivity();
                return;
            }

            auto sequenceName =
                dataset.value()->getDataset().getProperties().getPropertyName(sequenceId->eventSequenceId);

            if (!sequenceName) {
                _activityColorAndScale->clearActivity();
                return;
            }

            auto activity = dataset.value()->getDataset().getActivity(sequenceId->activityId);

            if (!activity) {
                _activityColorAndScale->clearActivity();
                return;
            }

            auto sequence =
                activity.value()->getProperty<mindset::EventSequence<std::monostate>>(sequenceId->eventSequenceId);

            if (!sequence) {
                _activityColorAndScale->clearActivity();
                return;
            }

            GID gid = {sequenceId->datasetId, sequenceId->activityId};
            _activityColorAndScale->setActivity(gid, *sequenceName, std::move(*sequence));
        }
    }

    NodeFactory ActivityColorAndScaleNode::createFactory()
    {
        return NodeFactory("Activity to color and scale", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<ActivityColorAndScaleNode>(app);
        });
    }
} // namespace neoneuron
