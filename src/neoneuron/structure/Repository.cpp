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

#include "Repository.h"

namespace neoneuron
{

    Repository::Repository() :
        _uidGenerator(0),
        _neuronAddedListener([this](auto*) { incrementVersion(); }),
        _synapseAddedListener([this](auto*) { incrementVersion(); }),
        _elementRemovedListener([this](auto) { incrementVersion(); }),
        _clearListener([this](auto*) { incrementVersion(); })
    {
    }

    size_t Repository::getDatasetsAmount() const
    {
        return _datasets.size();
    }

    std::tuple<mindset::UID, NamedDataset*, bool> Repository::addDataset(mindset::Dataset&& dataset, std::string name)
    {
        mindset::UID uid = _uidGenerator++;
        auto named = std::make_shared<NamedDataset>(uid, std::move(name), std::move(dataset));
        auto [ptr, ok] = _datasets.insert({uid, std::move(named)});
        if (ok) {
            ptr->second->getDataset().getNeuronAddedEvent() += _neuronAddedListener;
            ptr->second->getDataset().getCircuit().getSynapseAddedEvent() += _synapseAddedListener;
            ptr->second->getDataset().getNeuronRemovedEvent() += _elementRemovedListener;
            ptr->second->getDataset().getCircuit().getSynapseRemovedEvent() += _elementRemovedListener;
            ptr->second->getDataset().getClearEvent() += _clearListener;
            incrementVersion();
        }
        return {uid, ptr->second.get(), ok};
    }

    bool Repository::removeDataset(mindset::UID uid)
    {
        bool removed = _datasets.erase(uid) > 0;
        if (removed) {
            incrementVersion();
        }

        return removed;
    }

    void Repository::clear()
    {
        _datasets.clear();
        incrementVersion();
    }

    std::optional<std::shared_ptr<NamedDataset>> Repository::getDatasetAsOwned(mindset::UID uid)
    {
        auto it = _datasets.find(uid);
        if (it != _datasets.end()) {
            return it->second;
        }
        return {};
    }

    std::optional<NamedDataset*> Repository::getDataset(mindset::UID uid)
    {
        auto it = _datasets.find(uid);
        if (it != _datasets.end()) {
            return it->second.get();
        }
        return {};
    }

    std::optional<const NamedDataset*> Repository::getDataset(mindset::UID uid) const
    {
        auto it = _datasets.find(uid);
        if (it != _datasets.end()) {
            return it->second.get();
        }
        return {};
    }

    std::optional<mindset::Neuron*> Repository::getNeuron(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second->getDataset().getNeuron(gid.internalId);
        }
        return {};
    }

    std::optional<const mindset::Neuron*> Repository::getNeuron(GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second->getDataset().getNeuron(gid.internalId);
        }
        return {};
    }

    std::optional<std::pair<NamedDataset*, mindset::Neuron*>> Repository::getNeuronAndDataset(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            auto neuron = it->second->getDataset().getNeuron(gid.internalId);
            if (neuron.has_value()) {
                return std::make_pair(it->second.get(), neuron.value());
            }
        }
        return {};
    }

    std::optional<std::pair<const NamedDataset*, const mindset::Neuron*>> Repository::getNeuronAndDataset(GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            auto neuron = it->second->getDataset().getNeuron(gid.internalId);
            if (neuron.has_value()) {
                return std::make_pair(it->second.get(), neuron.value());
            }
        }
        return {};
    }

    std::optional<mindset::Synapse*> Repository::getSynapse(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second->getDataset().getCircuit().getSynapse(gid.internalId);
        }
        return {};
    }

    std::optional<const mindset::Synapse*> Repository::getSynapse(GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second->getDataset().getCircuit().getSynapse(gid.internalId);
        }
        return {};
    }

    std::optional<std::pair<NamedDataset*, mindset::Synapse*>> Repository::getSynapseAndDataset(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            auto synapse = it->second->getDataset().getCircuit().getSynapse(gid.internalId);
            if (synapse.has_value()) {
                return std::make_pair(it->second.get(), synapse.value());
            }
        }
        return {};
    }

    std::optional<std::pair<const NamedDataset*, const mindset::Synapse*>> Repository::getSynapseAndDataset(
        GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            auto synapse = it->second->getDataset().getCircuit().getSynapse(gid.internalId);
            if (synapse.has_value()) {
                return std::make_pair(it->second.get(), synapse.value());
            }
        }
        return {};
    }

} // namespace neoneuron