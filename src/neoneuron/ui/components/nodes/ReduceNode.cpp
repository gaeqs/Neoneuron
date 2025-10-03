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

#include "ReduceNode.h"

#include <neoneuron/structure/RepositoryView.h>

namespace
{
    template<typename T>
    void reduce(std::vector<T>& vec, float conservePercentage)
    {
        if (!vec.empty()) {
            size_t size = vec.size();
            int conserve = std::round(size * conservePercentage);
            float step = static_cast<float>(size) / conserve;
            int writeIndex = 0;
            for (int i = 0; i < conserve; ++i) {
                int idx = std::round(i * step);
                if (idx >= size) {
                    break;
                }
                vec[writeIndex++] = std::move(vec[idx]);
            }
            vec.resize(writeIndex);
        }
    }
} // namespace

namespace neoneuron
{

    void ReduceNode::sendData() const
    {
        DEBUG_PROFILE_ID(_application->getRender().getApplication().getProfiler(), id, "Reduce Node recompute");
#ifdef RELEASE_DEBUG
        idProfileStack;
#endif
        auto data = getMultipleInputs<RepositoryView>("Data");
        if (!data.has_value() || data->empty()) {
            sendOutput("Reduced", std::any());
            return;
        }

        Repository* repo = nullptr;
        std::vector<GID> neurons;
        std::vector<GID> synapses;

        float percentage = _percentage / 100.0f;

        for (auto view : *data) {
            repo = view.getRepository();
            neurons.insert(neurons.end(), view.getNeuronsGIDs().begin(), view.getNeuronsGIDs().end());
            synapses.insert(synapses.end(), view.getSynapsesGIDs().begin(), view.getSynapsesGIDs().end());
        }

        reduce(neurons, percentage);
        reduce(synapses, percentage);

        sendOutput("Reduced", RepositoryView(repo, std::move(neurons), std::move(synapses)));
    }

    ReduceNode::ReduceNode(NeoneuronApplication* application) :
        Node("Reduce"),
        _application(application),
        _repository(&application->getRepository()),
        _selector(&application->getSelector()),
        _percentage(50.0f),
        _active(false)
    {
        defineInput<RepositoryView>("Data", true);
        defineOutput<RepositoryView>("Reduced");
        sendData();
    }

    ReduceNode::~ReduceNode()
    {
        sendOutput("Reduced", std::any());
    }

    void ReduceNode::onInputChange(const std::string& name, const std::any& value)
    {
        if (name == "Data") {
            sendData();
        }
    }

    void ReduceNode::renderBody()
    {
        ImGui::SetNextItemWidth(100.0f);
        ImGui::SliderFloat("OutputSize", &_percentage, 0.0f, 100.0f, "%.0f%%");

        bool active = ImGui::IsItemActive();
        if (_active && !active) {
            sendData();
        }
        _active = active;
    }

    NodeFactory ReduceNode::createFactory()
    {
        return {"Reduce", [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<ReduceNode>(app); }};
    }
} // namespace neoneuron
