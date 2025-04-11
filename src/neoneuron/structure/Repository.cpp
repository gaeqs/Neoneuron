//
// Created by gaeqs on 09/04/2025.
//

#include "Repository.h"

namespace neoneuron
{

    Repository::Repository() :
        _uidGenerator(0),
        _neuronAddedListener([this](auto*) { incrementVersion(); }),
        _neuronRemovedListener([this](auto) { incrementVersion(); }),
        _clearListener([this](auto*) { incrementVersion(); })
    {
    }

    std::tuple<mindset::UID, mindset::Dataset*, bool> Repository::addDataset(mindset::Dataset&& dataset)
    {
        mindset::UID uid = _uidGenerator++;
        auto [ptr, ok] = _datasets.insert({uid, std::move(dataset)});
        if (ok) {
            ptr->second.getNeuronAddedEvent() += _neuronAddedListener;
            ptr->second.getNeuronRemovedEvent() += _neuronRemovedListener;
            ptr->second.getClearEvent() += _clearListener;
            incrementVersion();
        }
        return {uid, &ptr->second, ok};
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

    std::optional<mindset::Dataset*> Repository::getDataset(mindset::UID uid)
    {
        auto it = _datasets.find(uid);
        if (it != _datasets.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<const mindset::Dataset*> Repository::getDataset(mindset::UID uid) const
    {
        auto it = _datasets.find(uid);
        if (it != _datasets.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<mindset::Neuron*> Repository::getNeuron(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second.getNeuron(gid.internalId);
        }
        return {};
    }

    std::optional<const mindset::Neuron*> Repository::getNeuron(GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second.getNeuron(gid.internalId);
        }
        return {};
    }

    std::optional<std::pair<mindset::Dataset*, mindset::Neuron*>> Repository::getNeuronAndDataset(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            auto neuron = it->second.getNeuron(gid.internalId);
            if (neuron.has_value()) {
                return std::make_pair(&it->second, neuron.value());
            }
        }
        return {};
    }

    std::optional<std::pair<const mindset::Dataset*, const mindset::Neuron*>> Repository::getNeuronAndDataset(
        GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            auto neuron = it->second.getNeuron(gid.internalId);
            if (neuron.has_value()) {
                return std::make_pair(&it->second, neuron.value());
            }
        }
        return {};
    }

    std::optional<mindset::Synapse*> Repository::getSynapse(GID gid)
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second.getCircuit().getSynapse(gid.internalId);
        }
        return {};
    }

    std::optional<const mindset::Synapse*> Repository::getSynapse(GID gid) const
    {
        auto it = _datasets.find(gid.datasetId);
        if (it != _datasets.end()) {
            return it->second.getCircuit().getSynapse(gid.internalId);
        }
        return {};
    }
} // namespace neoneuron