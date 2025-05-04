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

#include "DatasetNode.h"

#include <mindset/Dataset.h>
#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    void DatasetNode::sendDataset(NamedDataset* dataset)
    {
        sendOutput("Data", RepositoryView::ofDataset(&_application->getRepository(), dataset));
        _datasetVersion = dataset->getDataset().getVersion();
        _circuitVersion = dataset->getDataset().getCircuit().getVersion();
    }

    DatasetNode::DatasetNode(NeoneuronApplication* application) :
        Node("Dataset"),
        _application(application),
        _selectedDataset(0)
    {
        defineOutput<RepositoryView>("Data");
    }

    void DatasetNode::renderBody()
    {
        constexpr size_t MAX_DATASETS = 1024;
        const char* names[MAX_DATASETS];
        mindset::UID uids[MAX_DATASETS];
        NamedDataset* selectedDataset = nullptr;

        int i = 0;
        for (auto [uid, dataset] : _application->getRepository().getDatasets()) {
            if (i >= MAX_DATASETS) {
                break;
            }
            names[i] = dataset->getName().c_str();
            uids[i] = uid;

            if (!_selectedDatasetUID.has_value()) {
                _selectedDatasetUID = uid;
                _selectedDataset = i;
                selectedDataset = dataset;
                sendDataset(dataset);
            } else if (_selectedDatasetUID == uid) {
                _selectedDataset = i;
                selectedDataset = dataset;
            }

            ++i;
        }

        if (selectedDataset == nullptr && i > 0) {
            // Dataset has been deleted
            _selectedDataset = 0;
            _selectedDatasetUID = uids[0];
            selectedDataset = _application->getRepository().getDataset(uids[0]).value();
            sendDataset(selectedDataset);
        } else if (i == 0 && _selectedDatasetUID.has_value()) {
            _selectedDatasetUID = {};
            _selectedDataset = 0;
            sendOutput("Data", std::any());
        } else if (selectedDataset != nullptr &&
                   (selectedDataset->getDataset().getVersion() != _datasetVersion ||
                    selectedDataset->getDataset().getCircuit().getVersion() != _circuitVersion)) {
            sendDataset(selectedDataset);
        }

        ImGui::SetNextItemWidth(std::min(ImGui::GetContentRegionAvail().x, 200.0f));
        if (ImGui::Combo("##dataset", &_selectedDataset, names, i)) {
            _selectedDatasetUID = uids[_selectedDataset];
            auto named = _application->getRepository().getDataset(_selectedDatasetUID.value());
            if (named.value()) {
                selectedDataset = named.value();
                sendDataset(selectedDataset);
            } else {
                selectedDataset = nullptr;
                sendOutput("Data", std::any());
            }
        }

        if (selectedDataset != nullptr) {
            ImGui::Text("Neurons: %d", selectedDataset->getDataset().getNeuronsAmount());
            ImGui::Text("Synapses: %d", selectedDataset->getDataset().getCircuit().getSynapses().size());
        }
    }

    NodeFactory DatasetNode::createFactory()
    {
        return NodeFactory("Dataset",
                           [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<DatasetNode>(app); });
    }
} // namespace neoneuron
