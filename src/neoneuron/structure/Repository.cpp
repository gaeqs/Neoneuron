//
// Created by gaeqs on 09/04/2025.
//

#include "Repository.h"

namespace neoneuron
{

    Repository::Repository() :
        _uidGenerator(0)
    {
    }

    std::tuple<mindset::UID, mindset::Dataset*, bool> Repository::addDataset(mindset::Dataset&& dataset)
    {
        mindset::UID uid = _uidGenerator++;
        auto [ptr, ok] = _datasets.insert({uid, std::move(dataset)});
        return {uid, &ptr->second, ok};
    }

    bool Repository::removeDataset(mindset::UID uid)
    {
        return _datasets.erase(uid) > 0;
    }

    void Repository::clear()
    {
        _datasets.clear();
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