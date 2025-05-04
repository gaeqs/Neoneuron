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

#include "RepositoryView.h"

namespace neoneuron
{

    RepositoryView::RepositoryView(Repository* repository, std::vector<GID> neurons, std::vector<GID> synapses) :
        _repository(repository),
        _neurons(std::move(neurons)),
        _synapses(std::move(synapses))
    {
    }

    Repository* RepositoryView::getRepository() const
    {
        return _repository;
    }

    const std::vector<GID>& RepositoryView::getNeurons() const
    {
        return _neurons;
    }

    const std::vector<GID>& RepositoryView::getSynapses() const
    {
        return _synapses;
    }

    RepositoryView RepositoryView::ofDataset(Repository* repository, NamedDataset* dataset)
    {
        std::vector<GID> neurons;
        std::vector<GID> synapses;

        {
            auto lock = dataset->getDataset().readLock();
            for (mindset::UID neuronUid : dataset->getDataset().getNeuronsUIDs()) {
                neurons.emplace_back(dataset->getUID(), neuronUid);
            }
        }
        {
            auto circuitLock = dataset->getDataset().getCircuit().readLock();
            for (mindset::UID synapseUid : dataset->getDataset().getCircuit().getSynapsesUIDs()) {
                synapses.emplace_back(dataset->getUID(), synapseUid);
            }
        }

        return {repository, std::move(neurons), std::move(synapses)};
    }

    RepositoryView RepositoryView::ofFullRepository(Repository* repository)
    {
        std::vector<GID> neurons;
        std::vector<GID> synapses;

        for (auto [uid, dataset] : repository->getDatasets()) {
            {
                auto lock = dataset->getDataset().readLock();
                for (mindset::UID neuronUid : dataset->getDataset().getNeuronsUIDs()) {
                    neurons.emplace_back(uid, neuronUid);
                }
            }
            {
                auto circuitLock = dataset->getDataset().getCircuit().readLock();
                for (mindset::UID synapseUid : dataset->getDataset().getCircuit().getSynapsesUIDs()) {
                    synapses.emplace_back(uid, synapseUid);
                }
            }
        }

        return {repository, std::move(neurons), std::move(synapses)};
    }

    std::optional<RepositoryView> RepositoryView::combine(const std::vector<RepositoryView>& views)
    {
        if (views.empty()) {
            return {};
        }
        if (views.size() == 1) {
            return views[0];
        }

        Repository* repository = nullptr;
        std::unordered_set<GID> neurons;
        std::unordered_set<GID> synapses;

        for (auto& view : views) {
            if (repository == nullptr && view.getRepository() != nullptr) {
                repository = view.getRepository();
            }
            neurons.insert(view.getNeurons().begin(), view.getNeurons().end());
            synapses.insert(view.getSynapses().begin(), view.getSynapses().end());
        }

        return RepositoryView(repository, std::vector(neurons.begin(), neurons.end()),
                              std::vector(synapses.begin(), synapses.end()));
    }

} // namespace neoneuron