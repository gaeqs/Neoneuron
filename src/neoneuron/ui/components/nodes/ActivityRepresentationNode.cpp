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

#include "ActivityRepresentationNode.h"

#include <neoneuron/render/activity/ActivityRepresentation.h>
#include <neoneuron/structure/RepositoryView.h>
#include <neoneuron/ui/components/nodes/EventSequenceNode.h>

namespace neoneuron
{

    void ActivityRepresentationNode::drawProgressBar(ActivityRepresentation* ptr)
    {
        auto used = static_cast<float>(ptr->getUsedInstanceMemory()) / 1024.0f / 1024.0f;
        auto allocated = static_cast<float>(ptr->getAllocatedInstanceMemory()) / 1024.0f / 1024.0f;
        std::string text = std::format("{:.2f}/{:.2f} MiB", used, allocated);
        ImGui::ProgressBar(ptr->getUsedInstanceMemoryPercentage(), ImVec2(200, 0), text.c_str());
        ImGui::Text("Bytes: %d", ptr->getUsedInstanceMemory());
    }

    ActivityRepresentationNode::ActivityRepresentationNode(NeoneuronApplication* application) :
        Node("Activity Representation"),
        _application(application),
        _timeline(nullptr)
    {
        defineInput<RepositoryView>("Data", true);
        defineInput<Viewport*>("Viewport", true);
        defineInput<Timeline*>("Timeline", false);
        defineInput<EventSequenceId>("Sequence", false);
        _representation = _application->getRender().addRepresentation<ActivityRepresentation>();
        defineOutput<std::weak_ptr<AbstractNeuronRepresentation>>("Representation", _representation);
    }

    ActivityRepresentationNode::~ActivityRepresentationNode()
    {
        sendOutput("Representation", std::any());
        if (_timeline != nullptr) {
            _timeline->removeTimeAware(_representation.lock());
        }
        if (auto ptr = _representation.lock()) {
            _application->getRender().removeRepresentation(ptr.get());
        }
    }

    void ActivityRepresentationNode::renderBody()
    {
        auto ptr = _representation.lock();
        if (!ptr) {
            return;
        }

        drawProgressBar(ptr.get());
    }

    void ActivityRepresentationNode::onInputChange(const std::string& name, const std::any& value)
    {
        auto ptr = _representation.lock();
        if (!ptr) {
            return;
        }
        if (name == "Data") {
            auto data = getMultipleInputs<RepositoryView>("Data");
            if (!data.has_value()) {
                ptr->clearData();
                return;
            }

            auto combined = RepositoryView::combine(data.value());
            if (combined.has_value()) {
                ptr->refreshData(combined.value());
            } else {
                ptr->clearData();
            }
        } else if (name == "Viewport") {
            auto viewports = getMultipleInputs<Viewport*>("Viewport");
            if (!viewports.has_value()) {
                ptr->setViewports({});
                return;
            }

            auto& vec = viewports.value();

            std::unordered_set<const Viewport*> set(vec.begin(), vec.end());
            ptr->setViewports(set);
        } else if (name == "Timeline") {
            if (_timeline != nullptr) {
                _timeline->removeTimeAware(_representation.lock());
            }

            auto timeline = getInput<Timeline*>("Timeline");
            if (!timeline.has_value()) {
                return;
            }

            _timeline = *timeline;
            _timeline->addTimeAware(_representation.lock());
        } else if (name == "Sequence") {
            auto sequenceId = getInput<EventSequenceId>("Sequence");
            if (!sequenceId.has_value()) {
                _representation.lock()->clearActivity();
                return;
            }

            auto dataset = _application->getRepository().getDataset(sequenceId->datasetId);
            if (!dataset) {
                _representation.lock()->clearActivity();
                return;
            }

            auto sequenceName =
                dataset.value()->getDataset().getProperties().getPropertyName(sequenceId->eventSequenceId);

            if (!sequenceName) {
                _representation.lock()->clearActivity();
                return;
            }

            auto activity = dataset.value()->getDataset().getActivity(sequenceId->activityId);

            if (!activity) {
                _representation.lock()->clearActivity();
                return;
            }

            auto sequence =
                activity.value()->getProperty<mindset::EventSequence<std::monostate>>(sequenceId->eventSequenceId);

            if (!sequence) {
                _representation.lock()->clearActivity();
                return;
            }

            GID gid = {sequenceId->datasetId, sequenceId->activityId};
            _representation.lock()->setActivity(gid, *sequenceName, std::move(*sequence));
        }
    }

    NodeFactory ActivityRepresentationNode::createFactory()
    {
        return NodeFactory("Activity Representation", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<ActivityRepresentationNode>(app);
        });
    }
} // namespace neoneuron
